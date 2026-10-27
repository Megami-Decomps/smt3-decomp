#!/usr/bin/env python3
"""Find an already-matched function whose retail code has the shape of an unmatched one.

    python3 tools/find_similar.py                     # ranked worklist
    python3 tools/find_similar.py --for NAME [--body]  # templates for one function
    python3 tools/find_similar.py --clusters           # unmatched sibling groups
    python3 tools/find_similar.py --unit field/fldDraw

Every function already in C is a solved (assembly, C) pair. When an unmatched
function's instruction sequence is close to a matched one, that match's C body
is a template and the work drops to substituting constants, offsets and
callees. The same comparison groups the unmatched functions among themselves,
which is the per-unit sibling search generalised across the whole image.

Both sides come from one objdump of the retail .text, cached under build/, so a
function written in C and one still in assembly normalise identically. Four
tiers, coarsening in this order:

    exact   mnemonic, registers, immediates
    noimm   mnemonic, registers
    noreg   mnemonic, immediates
    mnem    mnemonic

A `jal` target is wildcarded in every tier, because two siblings call different
functions. Branch and intra-text jump targets become offsets relative to the
instruction, which is control-flow shape rather than an address.

The ranked worklist scores a candidate by the retail bytes it would land
divided by the shape distance left to close, so a large function with a close
template outranks a small one with an exact template.
"""
from __future__ import annotations

import argparse
import bisect
import collections
import difflib
import hashlib
import json
import math
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
VERSION = "smt3"
CACHE = ROOT / "build" / "find_similar"
TIERS = ("exact", "noimm", "noreg", "mnem")

GPR = {
    "zero", "at", "v0", "v1", "a0", "a1", "a2", "a3",
    "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7", "t8", "t9",
    "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "s8",
    "k0", "k1", "gp", "sp", "ra", "fp",
}
DIS_LINE = re.compile(r"^\s*([0-9a-f]+):\t([0-9a-f]{8}) \t(\S+)\s*(.*)$")
INCLUDE_ASM = re.compile(r"^INCLUDE_ASM\([^\n]*\b(\w+)\);", re.M)
FUNC_ASM = re.compile(r"^glabel (\w+)\n(.*?)^endlabel \1", re.M | re.S)
ASM_INSN = re.compile(r"^\s*/\* [0-9A-F]+ ([0-9A-F]{8}) [0-9A-F]{8} \*/", re.M)
NUM = re.compile(r"^-?(?:0x[0-9a-fA-F]+|\d+)$")
CALLS = {"jal", "jalx"}
# Below this an instruction sequence carries no shape: a prologue and a jr $ra.
MIN_INSNS = 8
# A template more than this much longer or shorter is not one worth reading.
SIZE_BAND = 0.15
# Sequence comparison is the expensive step; only the best of the cheap score run it.
RERANK = 40
# Shortest shared name prefix worth offering as a same-family candidate.
FAMILY_PREFIX = 8


def text_range(elf: Path) -> tuple[int, int]:
    out = subprocess.run(
        [str(ROOT / "tools/bin/mips-ps2-decompals-objdump"), "-h", str(elf)],
        capture_output=True, text=True, check=True).stdout
    m = re.search(r"\.text\s+([0-9a-f]+)\s+([0-9a-f]+)", out)
    return int(m.group(2), 16), int(m.group(2), 16) + int(m.group(1), 16)


def disassemble(elf: Path) -> list[tuple[int, str, str]]:
    """(address, mnemonic, operand text) for every instruction of .text.

    -z is required: without it objdump prints "..." for a run of zero words,
    which silently drops the nops inside 1609 of this image's functions.
    """
    out = subprocess.run(
        [str(ROOT / "tools/bin/mips-ps2-decompals-objdump"),
         "-d", "-z", "--section=.text", str(elf)],
        capture_output=True, text=True, check=True).stdout
    return [(int(m.group(1), 16), m.group(3), m.group(4))
            for line in out.splitlines() if (m := DIS_LINE.match(line))]


def operands(text: str) -> tuple[list[str], list[int]]:
    """Registers in order and numeric immediates in order."""
    regs, imms = [], []
    for tok in re.split(r"[,()\s]+", text.split("<")[0].strip()):
        if not tok:
            continue
        if tok.startswith("$") or tok in GPR:
            regs.append(tok)
        elif NUM.match(tok):
            imms.append(int(tok, 0))
    return regs, imms


def tokens(addr: int, mnem: str, text: str, lo: int, hi: int) -> tuple[str, str, str, str]:
    """One instruction at each of the four tiers."""
    regs, imms = operands(text)
    if mnem in CALLS:
        imms = []
    elif imms and (mnem[0] == "b" or mnem == "j") and lo <= imms[-1] < hi:
        imms = imms[:-1] + [imms[-1] - addr]
    r, i = ",".join(regs), ",".join(str(v) for v in imms)
    return f"{mnem} {r} {i}", f"{mnem} {r}", f"{mnem}  {i}", mnem


def inventory(version: str = VERSION) -> list[dict]:
    """Every function of every game unit: name, address, instruction count, unit, state.

    The inventory tools/progress.py counts, so the two agree: a unit is a
    src/<v>/<unit>.c paired with asm/<v>/<unit>.s, and a function is still
    assembly when the C file holds its INCLUDE_ASM line.
    """
    out = []
    src = ROOT / "src" / version
    for unit in sorted(src.rglob("*.c")):
        name = unit.relative_to(src).with_suffix("").as_posix()
        full = ROOT / "asm" / version / f"{name}.s"
        if not full.exists():
            continue
        asm = set(INCLUDE_ASM.findall(unit.read_text()))
        for fname, body in FUNC_ASM.findall(full.read_text()):
            addrs = ASM_INSN.findall(body)
            if addrs:
                out.append({"name": fname, "addr": int(addrs[0], 16), "n": len(addrs),
                            "unit": name, "state": "asm" if fname in asm else "c"})
    out.sort(key=lambda f: f["addr"])
    return out


def corpus(version: str = VERSION, force: bool = False) -> dict[str, dict]:
    """Every .text function with its four normalised sequences and its callees."""
    serial = json.loads((ROOT / "config" / "versions.json").read_text())[version]["serial"]
    elf = ROOT / "orig" / version / serial
    cache = CACHE / f"{version}.json"
    srcs = sorted((ROOT / "src" / version).rglob("*.c"))
    stamp = str(max([elf.stat().st_mtime_ns] + [p.stat().st_mtime_ns for p in srcs]))
    if cache.exists() and not force:
        got = json.loads(cache.read_text())
        if got.get("stamp") == stamp:
            return got["funcs"]
    lo, hi = text_range(elf)
    inv = inventory(version)
    starts = {f["addr"]: f for f in inv}
    # The retail ELF is stripped, so objdump cannot name a call target itself.
    names = {f["addr"]: f["name"] for f in inv}
    funcs, cur, left = {}, None, 0
    for addr, mnem, text in disassemble(elf):
        if addr in starts:
            f = starts[addr]
            cur = funcs[f["name"]] = dict(f, seq=[[], [], [], []], calls=[])
            left = f["n"]
        if cur is None or left == 0:
            continue
        t = tokens(addr, mnem, text, lo, hi)
        for k in range(4):
            cur["seq"][k].append(t[k])
        left -= 1
        if mnem in CALLS:
            _, imms = operands(text)
            cur["calls"].append(names.get(imms[0], hex(imms[0])) if imms else "?")
    short = [f["name"] for f in funcs.values() if len(f["seq"][0]) != f["n"]]
    if short or len(funcs) != len(inv):
        raise SystemExit(f"disassembly does not cover the split asm: {short[:5]} "
                         f"{len(funcs)} of {len(inv)} functions")
    for f in funcs.values():
        f["hash"] = [hashlib.blake2b("\n".join(s).encode(), digest_size=8).hexdigest()
                     for s in f["seq"]]
    CACHE.mkdir(parents=True, exist_ok=True)
    cache.write_text(json.dumps({"stamp": stamp, "funcs": funcs}))
    return funcs


# --- similarity ------------------------------------------------------------------

class Index:
    """Nearest matched function for an unmatched one, by instruction sequence."""

    def __init__(self, funcs: dict[str, dict], tier: int = 3):
        self.funcs, self.tier = funcs, tier
        pool: dict[str, int] = {}
        for f in funcs.values():
            f["tok"] = tuple(pool.setdefault(s, len(pool)) for s in f["seq"][tier])
            f["bag"] = collections.Counter(f["tok"])
            f["len"] = math.sqrt(sum(v * v for v in f["bag"].values())) or 1.0
        self.by_size: dict[int, list[dict]] = collections.defaultdict(list)
        for f in funcs.values():
            if f["state"] == "c" and f["n"] >= MIN_INSNS:
                self.by_size[f["n"]].append(f)
        self.sizes = sorted(self.by_size)

    def ratio(self, a: dict, b: dict) -> float:
        return difflib.SequenceMatcher(None, a["tok"], b["tok"], autojunk=False).ratio()

    def nearest(self, target: dict, top: int = 3, state: str = "c") -> list[tuple[float, dict]]:
        """The closest functions in `state`, best first, as (ratio, function)."""
        if target["n"] < MIN_INSNS:
            return []
        lo, hi = target["n"] * (1 - SIZE_BAND), target["n"] * (1 + SIZE_BAND)
        if state == "c":
            pool = [f for s in self.sizes[bisect.bisect_left(self.sizes, lo):
                                          bisect.bisect_right(self.sizes, hi)]
                    for f in self.by_size[s]]
        else:
            pool = [f for f in self.funcs.values()
                    if f["state"] == state and lo <= f["n"] <= hi and f["n"] >= MIN_INSNS]
        bag, norm = target["bag"], target["len"]
        cheap = sorted(
            ((sum(v * c["bag"].get(k, 0) for k, v in bag.items()) / (norm * c["len"]), c)
             for c in pool if c["name"] != target["name"]), key=lambda x: -x[0])
        scored = [(difflib.SequenceMatcher(None, target["tok"], c["tok"],
                                           autojunk=False).ratio(), c)
                  for _, c in cheap[:RERANK]]
        return sorted(scored, key=lambda x: -x[0])[:top]


def family(funcs: dict[str, dict], name: str) -> list[dict]:
    """Matched functions sharing a long name prefix with this one.

    A size band hides the variant that grew: effBTLModelLook2 is 272 bytes
    against effBTLModelLook's 172, so the sequence search never offers it. A
    shared prefix does not care about size. It is only a candidate generator,
    since effBTLModelHide shares the same stem and is a different function:
    rank what comes back by sequence ratio and read the top of that.
    """
    if len(name) < FAMILY_PREFIX or name.startswith(("func_", "D_")):
        return []
    out = []
    for g in funcs.values():
        if g["state"] != "c" or g["name"] == name:
            continue
        i = 0
        while i < min(len(name), len(g["name"])) and name[i] == g["name"][i]:
            i += 1
        if i >= FAMILY_PREFIX:
            out.append(g)
    return out


def clusters(funcs: dict[str, dict], tier: int = 3) -> list[list[dict]]:
    """Groups of two or more functions with the same normalised sequence."""
    by = collections.defaultdict(list)
    for f in funcs.values():
        by[f["hash"][tier]].append(f)
    out = [sorted(v, key=lambda f: f["addr"]) for v in by.values() if len(v) > 1]
    return sorted(out, key=lambda v: -sum(f["n"] * 4 for f in v if f["state"] == "asm"))


def body(funcs: dict[str, dict], name: str, version: str = VERSION) -> str | None:
    """The C definition of a matched function, with its file and line."""
    f = funcs.get(name)
    if f is None or f["state"] != "c":
        return None
    path = ROOT / "src" / version / f"{f['unit']}.c"
    text = path.read_text()
    m = re.search(rf"^[A-Za-z_][^;#\n]*\b{re.escape(name)}\s*\([^;{{]*\)\s*\{{", text, re.M)
    if m is None:
        return None
    i, depth = m.end() - 1, 0
    while i < len(text):
        if text[i] == "{":
            depth += 1
        elif text[i] == "}":
            depth -= 1
            if depth == 0:
                break
        i += 1
    line = text[:m.start()].count("\n") + 1
    return f"// {path.relative_to(ROOT)}:{line}\n{text[m.start():i + 1]}"


def differences(a: dict, b: dict) -> list[str]:
    """What substituting b's C for a would have to change, at the exact tier.

    Only meaningful when the two agree on mnemonics, registers and instruction
    count; otherwise the shape itself differs and the diff is noise.
    """
    out = []
    for op, i0, i1, j0, j1 in difflib.SequenceMatcher(
            None, a["seq"][0], b["seq"][0], autojunk=False).get_opcodes():
        if op == "equal":
            continue
        for k in range(max(i1 - i0, j1 - j0)):
            mine = a["seq"][0][i0 + k] if i0 + k < i1 else "-"
            theirs = b["seq"][0][j0 + k] if j0 + k < j1 else "-"
            out.append(f"    {mine:<40} template {theirs}")
    for x, y in zip(a["calls"], b["calls"]):
        if x != y:
            out.append(f"    call {x:<35} template {y}")
    if len(a["calls"]) != len(b["calls"]):
        out.append(f"    {len(a['calls'])} calls, template has {len(b['calls'])}")
    return out


def asm_path(f: dict, version: str = VERSION) -> str:
    return f"asm/{version}/nonmatchings/{f['unit']}/{f['name']}.s"


# --- reporting -------------------------------------------------------------------

def row(ratio: float, t: dict, c: dict, diffs: int | None = None) -> str:
    left = f"{diffs:>4} sub" if diffs is not None else f"{ratio:.3f}   "
    return (f"{left} {t['name']:<36} {t['n'] * 4:>5}B {t['unit']:<30} "
            f"<- {c['name']:<36} {c['n'] * 4:>5}B {c['unit']}")


def best_template(funcs: dict[str, dict], index: Index,
                  t: dict) -> tuple[float, dict] | None:
    """The closest matched function by sequence, or by name family if that is closer."""
    near = index.nearest(t, top=1)
    fam = sorted(((index.ratio(t, g), g) for g in family(funcs, t["name"])),
                 key=lambda x: -x[0])[:1]
    return max(near + fam, key=lambda x: x[0]) if near or fam else None


def worklist(funcs: dict[str, dict], tier: int, unit: str | None, count: int,
             floor: float) -> None:
    """Unmatched functions ranked by retail bytes per substitution left to make.

    The ranking reads the exact-tier difference count, not the sequence ratio:
    at a ratio of 0.8 the median template still differs in two thirds of its
    instructions, which is a rewrite rather than a substitution.
    """
    index = Index(funcs, tier)
    targets = [f for f in funcs.values() if f["state"] == "asm" and f["n"] >= MIN_INSNS
               and (unit is None or f["unit"].startswith(unit))]
    rows = []
    for t in targets:
        best = best_template(funcs, index, t)
        if best is None or best[0] < floor:
            continue
        diffs = len(differences(t, best[1]))
        rows.append((t["n"] * 4 / (diffs + 1), diffs, best[0], t, best[1]))
    rows.sort(key=lambda r: -r[0])
    print(f"{len(rows)} of {len(targets)} unmatched functions have a matched "
          f"{TIERS[tier]} template at ratio {floor:.2f} or better "
          f"({sum(r[3]['n'] * 4 for r in rows)} retail bytes). "
          f"'sub' is how many instructions the template does not already supply.")
    for _, diffs, ratio, t, c in rows[:count]:
        print(row(ratio, t, c, diffs))


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("version", nargs="?", default=VERSION)
    ap.add_argument("--for", dest="target", help="one unmatched function to find templates for")
    ap.add_argument("--body", action="store_true", help="print the best template's C")
    ap.add_argument("--clusters", action="store_true",
                    help="groups of unmatched functions with the same shape")
    ap.add_argument("--unit", help="restrict to units whose path starts with this")
    ap.add_argument("--tier", choices=TIERS, default="mnem")
    ap.add_argument("--floor", type=float, default=0.9,
                    help="lowest sequence ratio to report (below 0.9 a template is a rewrite)")
    ap.add_argument("-n", "--count", type=int, default=40)
    ap.add_argument("--force", action="store_true", help="rebuild the disassembly cache")
    ap.add_argument("--stats", action="store_true", help="per-tier cluster and neighbour counts")
    args = ap.parse_args()

    funcs = corpus(args.version, args.force)
    tier = TIERS.index(args.tier)

    if args.stats:
        stats(funcs)
    elif args.clusters:
        show_clusters(funcs, tier, args.unit, args.count)
    elif args.target:
        show_one(funcs, tier, args.target, args.body)
    else:
        worklist(funcs, tier, args.unit, args.count, args.floor)


def stats(funcs: dict[str, dict]) -> None:
    asm = [f for f in funcs.values() if f["state"] == "asm"]
    print(f"{len(funcs)} functions, {len(asm)} still assembly")
    for k, tier in enumerate(TIERS):
        by = collections.defaultdict(list)
        for f in funcs.values():
            by[f["hash"][k]].append(f)
        twin = [f for f in asm if any(g["state"] == "c" for g in by[f["hash"][k]])]
        sib = [f for f in asm if len(by[f["hash"][k]]) > 1
               and not any(g["state"] == "c" for g in by[f["hash"][k]])]
        print(f"  {tier:6} {len(twin):4} unmatched with an identical matched twin "
              f"({sum(f['n'] * 4 for f in twin)}B), {len(sib):4} with unmatched siblings only "
              f"({sum(f['n'] * 4 for f in sib)}B)")


def show_clusters(funcs: dict[str, dict], tier: int, unit: str | None, count: int) -> None:
    out = []
    for group in clusters(funcs, tier):
        rest = [f for f in group if f["state"] == "asm"]
        if len(rest) < 2 or group[0]["n"] < MIN_INSNS:
            continue
        if unit and not any(f["unit"].startswith(unit) for f in group):
            continue
        out.append(group)
    free = sum(f["n"] * 4 for g in out for f in g if f["state"] == "asm") \
        - sum(g[0]["n"] * 4 for g in out)
    print(f"{len(out)} clusters hold {sum(1 for g in out for f in g if f['state'] == 'asm')} "
          f"unmatched functions; {free} retail bytes follow from the first of each")
    for group in out[:count]:
        shown = ", ".join(f"{f['name']}[{'C' if f['state'] == 'c' else 'asm'}]"
                          for f in group[:8])
        more = f", +{len(group) - 8} more" if len(group) > 8 else ""
        print(f"  {group[0]['n'] * 4}B x{len(group)} {shown}{more}")
        for u in sorted({f["unit"] for f in group}):
            print(f"      {u}")


def show_one(funcs: dict[str, dict], tier: int, name: str, want_body: bool) -> None:
    t = funcs.get(name)
    if t is None:
        raise SystemExit(f"{name}: not a function of any unit")
    index = Index(funcs, tier)
    print(f"{name} {t['n'] * 4}B {t['unit']} ({t['state']})\n  {asm_path(t)}")
    best = None
    for label in ("c", "asm"):
        near = index.nearest(t, top=3, state=label)
        kind = "matched templates" if label == "c" else "unmatched siblings"
        print(f"  {kind}:" if near else f"  no {kind}")
        for ratio, c in near:
            print("  " + row(ratio, t, c))
        if label == "c" and near:
            best = near[0]
    fam = sorted(((index.ratio(t, g), g) for g in family(funcs, name)), key=lambda x: -x[0])
    seen = {c["name"] for _, c in index.nearest(t, top=3)}
    fam = [(r, g) for r, g in fam if g["name"] not in seen][:3]
    if fam:
        print("  matched name-family variants:")
        for ratio, g in fam[:3]:
            print("  " + row(ratio, t, g))
        if best is None or fam[0][0] > best[0]:
            best = fam[0]
    if best is None:
        return
    diff = differences(t, best[1])
    print(f"  against {best[1]['name']}: {len(diff)} exact-tier differences" if diff
          else f"  against {best[1]['name']}: identical beyond the callees")
    for line in diff[:40]:
        print(line)
    if len(diff) > 40:
        print(f"    ... {len(diff) - 40} more")
    if want_body:
        print()
        print(body(funcs, best[1]["name"])
              or f"// no C definition found for {best[1]['name']}")


if __name__ == "__main__":
    main()
