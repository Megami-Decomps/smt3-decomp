#!/usr/bin/env python3
"""Decide, per SLUS_209.11 INCLUDE_ASM function, whether dds-decomp already has it in C.

    DDS_DECOMP=../dds-decomp python3 tools/dds_reuse.py

Reads the split asm, the retail ELF and a dds-decomp checkout; no DDS1 binary.
Writes build/dds_reuse.tsv. Method and limits: docs/dds-port.md."""

from __future__ import annotations

import argparse
import bisect
import collections
import difflib
import json
import os
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
VERSION = "smt3"
DELTA = 0xFF000  # vram - file offset, constant across the image
SECTIONS = {".rodata": (0x0052DC80, 0x0054C178)}
# DDS1's main .text, from dds-decomp's splat config; above it are the overlays.
DDS1_TEXT = (0x00100000, 0x002F0000)

GLABEL = re.compile(r"^glabel (\w+)")
INSN = re.compile(r"/\* [0-9A-F]+ ([0-9A-F]{8}) ([0-9A-F]{8}) \*/\s+(\S+)")
JAL = re.compile(r"\bjal\s+(\w+)")
REF = re.compile(r"%(?:lo|gp_rel)\(([A-Za-z_]\w*)(?:\s*\+\s*(0x[0-9A-F]+|\d+))?\)")
HEX_SUFFIX = re.compile(r"_([0-9A-F]{8})$")
DATA_LABEL = re.compile(r"^(?:dlabel|glabel|jlabel) (\w+)\n\s*/\* [0-9A-F]+ ([0-9A-F]{8})", re.M)
SECTION_FILE = re.compile(r"\.(rodata|lit4|sdata|data|bss|sbss)\.s$")
SKIP_DIRS = {"data", "nonmatchings", "matchings", "sdk"}
INCLUDE_ASM = re.compile(r"INCLUDE_ASM\([^,]+,\s*\"([^\"]+)\"\s*,\s*(\w+)")
FUNC_SYM = re.compile(r"^(\w+)\s*=\s*(0x[0-9A-Fa-f]+);\s*//\s*type:func")
YAML_C = re.compile(r"^\s*-\s*\[\s*(0x[0-9A-Fa-f]+)\s*,\s*c\s*,\s*([^\]\s]+)")
DDS_PLACEHOLDER = re.compile(r"^game/code_[0-9A-Fa-f]+$")

# Alignment knobs. A run of this many consecutive equal sizes is treated as settled;
# shorter runs and interpolated pairs are reported as weaker.
STRONG_RUN = 6
MEDIUM_RUN = 3
# Two sizes count as the same function recompiled when they differ by no more than this.
SIZE_SLACK = 8
SIZE_RATIO = 0.05
# Widest offset a gap-filled pair may sit at, so one DP stays cheap on a long gap.
BAND = 120


# --- SLUS_209.11 side -------------------------------------------------------------


class Smt3:
    """Every function in the split asm, with the signals the asm itself carries."""

    def __init__(self, version: str = VERSION):
        serial = json.loads((ROOT / "config" / "versions.json").read_text())[version]["serial"]
        self.elf = (ROOT / "orig" / version / serial).read_bytes()
        asm = ROOT / "asm" / version
        if not asm.exists():
            raise SystemExit(f"no {asm}; run configure.py first")
        self.data = self._data_symbols(asm)
        self.data_addrs = sorted(self.data)
        self.by_name = {name: addr for addr, name in sorted(self.data.items())}
        self.funcs = self._functions(asm)
        self.index = {f["addr"]: i for i, f in enumerate(self.funcs)}
        self.fallback = self._fallbacks(ROOT / "src" / version)

    @staticmethod
    def _data_symbols(asm: Path) -> dict[int, str]:
        out = {}
        for path in sorted(asm.rglob("*.s")):
            if SECTION_FILE.search(path.name):
                for name, addr in DATA_LABEL.findall(path.read_text()):
                    out[int(addr, 16)] = name
        return out

    def _functions(self, asm: Path) -> list[dict]:
        out = []
        for path in sorted(asm.rglob("*.s")):
            parts = path.relative_to(asm).parts
            if SKIP_DIRS & set(parts) or SECTION_FILE.search(path.name):
                continue
            unit = str(path.relative_to(asm))[:-2]
            cur = None
            for line in path.read_text().splitlines():
                if m := GLABEL.match(line):
                    cur = {"name": m.group(1), "unit": unit, "insns": 0, "calls": [],
                           "strings": set()}
                    continue
                if cur is None:
                    continue
                if m := INSN.search(line):
                    if "addr" not in cur:
                        cur["addr"] = int(m.group(1), 16)
                        out.append(cur)
                    cur["insns"] += 1
                if m := JAL.search(line):
                    cur["calls"].append(m.group(1))
                for ref in REF.finditer(line):
                    if (s := self._string_ref(ref)) is not None:
                        cur["strings"].add(s)
        out.sort(key=lambda f: f["addr"])
        for i, f in enumerate(out):
            f["size"] = out[i + 1]["addr"] - f["addr"] if i + 1 < len(out) else f["insns"] * 4
        return out

    def _string_ref(self, ref: re.Match) -> str | None:
        # D_0054E744 carries its address; a name from symbol_addrs.txt does not, so those
        # resolve through the labels the data asm defines.
        hexed = HEX_SUFFIX.search(ref.group(1))
        base = int(hexed.group(1), 16) if hexed else self.by_name.get(ref.group(1))
        if base is None:
            return None
        return self.string(base + (int(ref.group(2), 0) if ref.group(2) else 0))

    def string(self, addr: int) -> str | None:
        """The .rodata literal at addr, or None if the bytes are not one.

        A literal ends before the next symbol and the rest of its slot is NUL padding;
        anything else is a table that happens to start with text."""
        lo, hi = SECTIONS[".rodata"]
        if not lo <= addr < hi:
            return None
        off = addr - DELTA
        end = self.elf.find(b"\0", off)
        if end < 0:
            return None
        i = bisect.bisect_right(self.data_addrs, addr)
        limit = (self.data_addrs[i] - DELTA) if i < len(self.data_addrs) else None
        if limit is not None and (end >= limit or any(self.elf[end:limit])):
            return None
        s = self.elf[off:end]
        if len(s) < 4 or not all(32 <= c < 127 or c in (9, 10, 13) for c in s):
            return None
        return s.decode() if any(chr(c).isalnum() for c in s) else None

    @staticmethod
    def _fallbacks(src: Path) -> set[str]:
        out = set()
        for path in src.rglob("*.c"):
            out |= {m.group(2) for m in INCLUDE_ASM.finditer(path.read_text())}
        return out


# --- dds-decomp side --------------------------------------------------------------


C_DEF = re.compile(
    r"^(?![#\s])(?P<hdr>[A-Za-z_][^;{}#]*?)\b(?P<name>\w+)\s*\((?P<args>[^;{}]*)\)\s*"
    r"(?:__attribute__\s*\(\([^()]*\)\)\s*)?\{", re.M)


def c_definitions(text: str) -> list[tuple[str, str]]:
    """Top-level function definitions of one C file, in source order, as (name, body).

    Source order is address order in a decomp tree, which is the whole point of reading
    it: the order is the only link to the DDS1 symbol table that needs no binary. A
    definition starts in column 1, so an indented `if (...) {` cannot be mistaken for one."""
    out = []
    for m in C_DEF.finditer(text):
        hdr, name = m.group("hdr"), m.group("name")
        if "=" in hdr or name in C_KEYWORD or hdr.split()[0] in ("typedef", "return"):
            continue
        i, depth = m.end() - 1, 0
        while i < len(text):
            if text[i] == "{":
                depth += 1
            elif text[i] == "}":
                depth -= 1
                if depth == 0:
                    break
            i += 1
        out.append((name, text[m.end() - 1:i + 1]))
    return out


C_STRING = re.compile(r"\"((?:[^\"\\\n]|\\.)*)\"")
C_CALL = re.compile(r"\b(\w+)\s*\(")
C_KEYWORD = {"if", "for", "while", "switch", "return", "sizeof", "do", "else"}


def unescape(s: str) -> str:
    try:
        return s.encode("latin-1", "replace").decode("unicode_escape")
    except (UnicodeDecodeError, UnicodeEncodeError):
        return s


class Dds:
    """DDS1 functions in address order, each tagged with what dds-decomp has for it."""

    def __init__(self, root: Path):
        self.root = root
        src = root / "src" / "dds1"
        if not src.exists():
            raise SystemExit(f"no {src}: point --dds-decomp or $DDS_DECOMP at the checkout")
        self.in_c: dict[str, dict] = {}
        self.asm: set[str] = set()
        self.order: dict[str, list[str]] = {}
        for path in sorted(src.rglob("*.c")):
            unit = str(path.relative_to(src))[:-2]
            text = path.read_text(errors="replace")
            self.asm |= {m.group(2) for m in INCLUDE_ASM.finditer(text)}
            names = []
            for name, body in c_definitions(text):
                strings = {unescape(m.group(1)) for m in C_STRING.finditer(body)}
                calls = [m.group(1) for m in C_CALL.finditer(body) if m.group(1) not in C_KEYWORD]
                self.in_c[name] = {"file": unit, "strings": strings, "calls": len(calls)}
                names.append(name)
            self.order[unit] = names
        self.funcs = self._symbols(root)

    def _symbols(self, root: Path) -> list[dict]:
        rows = []
        for line in (root / "config/dds1/symbol_addrs.txt").read_text().splitlines():
            if m := FUNC_SYM.match(line.strip()):
                rows.append({"addr": int(m.group(2), 16), "name": m.group(1)})
        rows.sort(key=lambda r: r["addr"])
        units = []
        for line in (root / "config/dds1/SLUS_209.74.yaml").read_text().splitlines():
            if m := YAML_C.match(line):
                units.append((int(m.group(1), 16) + DELTA, m.group(2)))
        units.sort()
        starts = [a for a, _ in units]
        for i, row in enumerate(rows):
            nxt = rows[i + 1]["addr"] if i + 1 < len(rows) else row["addr"] + 8
            row["size"] = nxt - row["addr"]
            j = bisect.bisect_right(starts, row["addr"]) - 1
            row["unit"] = units[j][1] if j >= 0 else "?"
            info = self.in_c.get(row["name"])
            row["have"] = "asm" if row["name"] in self.asm else ("c" if info else "unknown")
            row["file"] = info["file"] if info else ""
            row["strings"] = info["strings"] if info else set()
            row["calls"] = info["calls"] if info else 0
        lo, hi = DDS1_TEXT
        return [r for r in rows if lo <= r["addr"] < hi]


# --- alignment --------------------------------------------------------------------


def close(x: int, y: int) -> bool:
    return abs(x - y) <= max(SIZE_SLACK, SIZE_RATIO * max(x, y))


def align(a: list[dict], b: list[dict]) -> dict[int, dict]:
    """Pair the two address-ordered function lists, monotonically.

    The frame is the diff of the two size sequences: a run of equal sizes at the same
    place in both lists is the same code compiled twice. Each remaining gap is then
    aligned on size closeness and call count, which recovers functions whose size moved."""
    sizes_a = [f["size"] for f in a]
    sizes_b = [f["size"] for f in b]
    blocks = [blk for blk in difflib.SequenceMatcher(None, sizes_a, sizes_b, autojunk=False)
              .get_matching_blocks() if blk.size >= 2]
    out: dict[int, dict] = {}
    for blk in blocks:
        for k in range(blk.size):
            out[a[blk.a + k]["addr"]] = {"dds": b[blk.b + k], "how": "run", "run": blk.size}

    frame = blocks + [difflib.Match(len(a), len(b), 0)]
    prev_a = prev_b = 0
    for blk in frame:
        gap = _gap(a, b, prev_a, blk.a, prev_b, blk.b)
        out.update(gap)
        prev_a, prev_b = blk.a + blk.size, blk.b + blk.size
    return out


def _gap(a, b, a0, a1, b0, b1) -> dict[int, dict]:
    """Banded alignment of one gap on size closeness, with call count as a tie-break."""
    n, m = a1 - a0, b1 - b0
    if n <= 0 or m <= 0:
        return {}

    def score(i, j):
        fa, fb = a[a0 + i], b[b0 + j]
        if not close(fa["size"], fb["size"]):
            return -0.2
        s = 1.0
        if abs(len(fa["calls"]) - fb["calls"]) <= 1:
            s += 0.3
        return s

    def band(i):
        centre = i * m // max(n, 1)
        return range(max(0, centre - BAND), min(m, centre + BAND + 1))

    dp = [collections.defaultdict(float) for _ in range(n + 1)]
    for i in range(1, n + 1):
        row, prev = dp[i], dp[i - 1]
        best_left = 0.0
        for j in band(i - 1):
            s = score(i - 1, j)
            best_left = max(best_left, row[j], prev[j + 1], prev[j] + s)
            row[j + 1] = best_left
    out = {}
    i, j = n, m
    while i > 0 and j > 0:
        s = score(i - 1, j - 1)
        if abs(dp[i][j] - (dp[i - 1][j - 1] + s)) < 1e-9:
            if s > 0:
                out[a[a0 + i - 1]["addr"]] = {"dds": b[b0 + j - 1], "how": "gap", "run": 0}
            i -= 1
            j -= 1
        elif abs(dp[i][j] - dp[i - 1][j]) < 1e-9:
            i -= 1
        else:
            j -= 1
    return out


def literal_anchors(smt3: Smt3, dds: Dds) -> dict[int, dict]:
    """Functions tied by a .rodata literal that only one function on each side uses."""
    a_by = collections.defaultdict(list)
    for f in smt3.funcs:
        for s in f["strings"]:
            a_by[s].append(f)
    b_by = collections.defaultdict(list)
    for f in dds.funcs:
        for s in f["strings"]:
            b_by[s].append(f)
    out = {}
    for s, fas in a_by.items():
        fbs = b_by.get(s)
        if fbs and len(fas) == 1 and len(fbs) == 1:
            out[fas[0]["addr"]] = fbs[0]
    return out


def confidence(pair: dict, literal: bool) -> str:
    if pair["dds"]["have"] != "c":
        return "none"
    if literal or pair["run"] >= STRONG_RUN:
        return "high"
    if pair["run"] >= MEDIUM_RUN:
        return "medium"
    return "low"


# --- report -----------------------------------------------------------------------

FIELDS = ["smt3_addr", "smt3_func", "smt3_unit", "size", "insns", "calls", "confidence",
          "dds_func", "dds_file", "dds_unit", "dds_have", "dds_size", "run", "how",
          "evidence"]
HEADER = [
    "SLUS_209.11 INCLUDE_ASM functions against the C dds-decomp already has.",
    "Produced by tools/dds_reuse.py; method, numbers and limits in docs/dds-port.md.",
    "No DDS1 binary is read, so no pair here is proven at the byte level: confidence",
    "high means a shared .rodata literal or a run of at least six consecutive equal",
    "function sizes, medium a run of three to five, low a single size match or a",
    "size-close pair recovered inside a gap.",
    "",
    "evidence: lit=shared literal, size=exact size match, calls=call counts within one,",
    "unit=both sides sit in a unit of the same name.",
]


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--dds-decomp", type=Path,
                    default=Path(os.environ.get("DDS_DECOMP", "../dds-decomp")),
                    help="dds-decomp checkout ($DDS_DECOMP)")
    ap.add_argument("--output", type=Path, default=ROOT / "build" / "dds_reuse.tsv")
    ap.add_argument("--unit", help="print the per-function table for one smt3 unit")
    ap.add_argument("--dry-run", action="store_true", help="report counts, write nothing")
    args = ap.parse_args()

    smt3, dds = Smt3(), Dds(args.dds_decomp.resolve())
    print(f"SLUS_209.11: {len(smt3.funcs)} functions, {len(smt3.fallback)} still INCLUDE_ASM")
    have_c = sum(1 for f in dds.funcs if f["have"] == "c")
    print(f"DDS1: {len(dds.funcs)} functions in main .text, {have_c} of them in dds-decomp's C,"
          f" {sum(1 for f in dds.funcs if f['have'] == 'asm')} still INCLUDE_ASM there")

    pairs = align(smt3.funcs, dds.funcs)
    anchors = literal_anchors(smt3, dds)
    agreed = missed = conflict = 0
    for addr, b in anchors.items():
        have = pairs.get(addr)
        if have is None:
            missed += 1
        elif have["dds"]["addr"] == b["addr"]:
            agreed += 1
        else:
            conflict += 1
        # A shared literal is direct evidence; the size alignment is circumstantial.
        pairs[addr] = {"dds": b, "how": "literal", "run": have["run"] if have else 0}
    print(f"aligned {len(pairs)} function pairs; of {len(anchors)} literal anchors the size"
          f" alignment reproduced {agreed}, missed {missed} and contradicted {conflict}")

    rows = []
    for f in smt3.funcs:
        pair = pairs.get(f["addr"])
        lit = f["addr"] in anchors
        if pair is None:
            conf, b = "none", None
        else:
            conf, b = confidence(pair, lit), pair["dds"]
        evidence = []
        if b is not None:
            if lit:
                evidence.append("lit")
            if b["size"] == f["size"]:
                evidence.append("size")
            if abs(len(f["calls"]) - b["calls"]) <= 1:
                evidence.append("calls")
            if same_unit(f["unit"], b["unit"]):
                evidence.append("unit")
        rows.append({
            "smt3_addr": f"0x{f['addr']:08X}", "smt3_func": f["name"], "smt3_unit": f["unit"],
            "size": f"0x{f['size']:X}", "insns": f["insns"], "calls": len(f["calls"]),
            "confidence": conf,
            "dds_func": b["name"] if b else "", "dds_file": b["file"] if b else "",
            "dds_unit": b["unit"] if b else "", "dds_have": b["have"] if b else "",
            "dds_size": f"0x{b['size']:X}" if b else "",
            "run": pair["run"] if pair else 0, "how": pair["how"] if pair else "",
            "evidence": ",".join(evidence),
            "_fallback": f["name"] in smt3.fallback, "_bytes": f["size"], "_unit": f["unit"],
        })

    summarize(rows, pairs, smt3, dds)
    if args.unit:
        for r in rows:
            if r["_unit"] == args.unit:
                print("\t".join(str(r[c]) for c in FIELDS))
    if args.dry_run:
        return
    args.output.parent.mkdir(parents=True, exist_ok=True)
    body = "\n".join("\t".join(str(r[c]) for c in FIELDS) for r in rows)
    args.output.write_text("\n".join(f"# {line}" for line in HEADER) + "\n"
                           + "\t".join(FIELDS) + "\n" + body + "\n")
    print(f"wrote {args.output}")


def same_unit(a: str, b: str) -> bool:
    if "code_" in a or DDS_PLACEHOLDER.match(b):
        return False
    norm = lambda s: re.sub(r"[^a-z0-9]", "", s.rsplit("/", 1)[-1].lower())
    return norm(a) == norm(b)


ORDER = ["high", "medium", "low", "none"]


def summarize(rows, pairs, smt3: Smt3, dds: Dds) -> None:
    fb = [r for r in rows if r["_fallback"]]
    counts = collections.Counter(r["confidence"] for r in fb)
    byte_counts = collections.Counter()
    for r in fb:
        byte_counts[r["confidence"]] += r["_bytes"]
    total_bytes = sum(r["_bytes"] for r in fb)
    print(f"\n{len(fb)} INCLUDE_ASM functions, 0x{total_bytes:X} bytes of .text")
    for conf in ORDER:
        n = counts[conf]
        print(f"  {conf:<6} {n:5d} functions  0x{byte_counts[conf]:06X} bytes"
              f"  {100 * n / max(len(fb), 1):5.1f}%")
    served = sum(counts[c] for c in ("high", "medium", "low"))
    print(f"  served by existing C: {served} functions"
          f" (0x{sum(byte_counts[c] for c in ('high', 'medium', 'low')):X} bytes),"
          f" {100 * served / max(len(fb), 1):.1f}%")
    no_pair = sum(1 for r in fb if r["confidence"] == "none" and not r["dds_func"])
    print(f"  of the none rows, {no_pair} have no DDS1 counterpart at all and"
          f" {counts['none'] - no_pair} pair to code dds-decomp has not decompiled either")

    print("\nBy unit (fallbacks only, units with a served function, biggest first):")
    per = collections.defaultdict(collections.Counter)
    for r in fb:
        per[r["_unit"]][r["confidence"]] += 1
        per[r["_unit"]]["_bytes"] += r["_bytes"]
    ranked = sorted(per.items(), key=lambda kv: -sum(kv[1][c] for c in ("high", "medium", "low")))
    print(f"  {'unit':<34}{'asm':>5}{'high':>6}{'med':>5}{'low':>5}{'none':>6}  bytes")
    for unit, c in ranked[:40]:
        total = sum(c[k] for k in ORDER)
        print(f"  {unit:<34}{total:5d}{c['high']:6d}{c['medium']:5d}{c['low']:5d}"
              f"{c['none']:6d}  0x{c['_bytes']:06X}")
    rest = ranked[40:]
    if rest:
        print(f"  ... {len(rest)} more units,"
              f" {sum(sum(c[k] for k in ('high', 'medium', 'low')) for _, c in rest)} served")

    # Independent check: the unit names were carried from DDS1 by hash pairing, which
    # shares no input with a size-sequence diff.
    agree = dis = 0
    for r in rows:
        if r["dds_func"] and "code_" not in r["smt3_unit"] \
                and not DDS_PLACEHOLDER.match(r["dds_unit"] or "x"):
            if same_unit(r["smt3_unit"], r["dds_unit"]):
                agree += 1
            else:
                dis += 1
    print(f"\nValidation, pairs where both sides sit in a named unit: {agree} agree on the"
          f" unit name, {dis} disagree ({100 * agree / max(agree + dis, 1):.1f}%)")


if __name__ == "__main__":
    main()
