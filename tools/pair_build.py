#!/usr/bin/env python3
"""Pair functions between two Nocturne builds through their IDA databases.

    python3 tools/pair_build.py --from-instance SLUS_209.11 --to-instance SLPM_654.62 \
        --to-elf <path> --to-text-end 0x002E64C4

Pairs on masked code, so relinking does not defeat it, then aligns the leftovers in
address order. Writes one row per pair and per unpaired function on either side.
Both IDBs are read only. Method and measured accuracy: docs/maniax-build.md."""

from __future__ import annotations

import argparse
import base64
import json
import os
import struct
import sys
import zlib
from bisect import bisect_right
from collections import defaultdict
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent
IDA_SCRIPTS = Path(
    os.environ.get("IDA_SKILL_SCRIPTS", Path.home() / ".claude/skills/ida-instances/scripts")
)
PORTS = os.environ.get("IDA_MCP_PORTS", "13337-13437")

# vram - file offset, constant across both images.
DELTA = 0xFF000
TEXT_START = 0x00100000
# Trivial stubs match by chance, so pair_dds1.py's 16-byte floor is kept.
MIN_SIZE = 16
# Widest offset a gap-filled pair may sit at, so one DP stays cheap on a long gap.
BAND = 120
# Two sizes count as the same function recompiled when they differ by no more than this.
SIZE_SLACK = 8
# How far either side of the interpolated address to look for an unmarked counterpart.
SEARCH_WINDOW = 0x2000

# Opcodes whose low 16 bits can hold a relocated value (lui, addiu, ori, loads, stores,
# lq/sq, ...). Masking them lets two differently linked builds compare equal. Branches
# are PC-relative and deliberately absent: their offsets are real signal.
IMM_OPS = frozenset((0x0F, 0x09, 0x0C, 0x0D, 0x19, 0x1E, 0x1F, 0x20, 0x21, 0x23,
                     0x24, 0x25, 0x27, 0x28, 0x29, 0x2B, 0x2C, 0x2D, 0x31, 0x34,
                     0x35, 0x37, 0x39, 0x3C, 0x3D, 0x3F))

IDA_PAYLOAD = r'''
def export():
    # py_eval execs this with split globals and locals, so the imports have to live
    # inside the function for its body to see them.
    import base64, hashlib, json, struct, zlib
    import ida_bytes, ida_funcs, idautils, idc

    IMM_OPS = frozenset((0x0F, 0x09, 0x0C, 0x0D, 0x19, 0x1E, 0x1F, 0x20, 0x21, 0x23,
                         0x24, 0x25, 0x27, 0x28, 0x29, 0x2B, 0x2C, 0x2D, 0x31, 0x34,
                         0x35, 0x37, 0x39, 0x3C, 0x3D, 0x3F))

    rows = []
    for ea in sorted(idautils.Functions()):
        func = ida_funcs.get_func(ea)
        if func is None:
            continue
        size = func.end_ea - ea
        if len(list(idautils.Chunks(ea))) != 1 or size % 4:
            # A split function has no single extent to hash.
            rows.append([ea, size, 0, "", "", idc.get_func_name(ea)])
            continue
        code = ida_bytes.get_bytes(ea, size)
        if code is None or len(code) != size:
            rows.append([ea, size, 0, "", "", idc.get_func_name(ea)])
            continue
        words = bytearray()
        for i in range(0, size, 4):
            w = struct.unpack_from("<I", code, i)[0]
            op = w >> 26
            if op in (2, 3):
                w &= 0xFC000000          # j / jal: target is a link-time address
            elif op in IMM_OPS:
                w &= 0xFFFF0000          # keep opcode and registers, drop the immediate
            words += struct.pack("<I", w)
        rows.append([ea, size, size // 4,
                     hashlib.sha1(bytes(words)).hexdigest()[:16],
                     hashlib.sha1(code).hexdigest()[:16],
                     idc.get_func_name(ea)])

    blob = json.dumps(rows, separators=(",", ":"))
    return base64.b64encode(zlib.compress(blob.encode(), 9)).decode()


FUNC_BLOB = export()
len(FUNC_BLOB)
'''

# The MCP transport truncates a reply above roughly 20 KB, so the blob is read in pages.
PAGE = 16384


def eval_in_ida(ida, port, code, new_locals):
    out = ida.call_tool(port, "py_eval", {"code": code, "new_locals": new_locals})
    if out.get("stderr"):
        raise SystemExit(f"IDAPython failed on port {port}:\n{out['stderr']}")
    return out.get("result")


def collect(instance, cache):
    """Every function in one IDB as [addr, size, insns, masked, raw, name]."""
    if cache and cache.exists():
        return json.loads(cache.read_text())
    sys.path.insert(0, str(IDA_SCRIPTS))
    import ida

    port = ida.resolve_target(instance, ida.get_instances(ida.parse_ports(PORTS), refresh=False))
    total = int(eval_in_ida(ida, port, IDA_PAYLOAD, True))
    pages = []
    for start in range(0, total, PAGE):
        page = eval_in_ida(ida, port, f"FUNC_BLOB[{start}:{start + PAGE}]", False)
        if len(page) != min(PAGE, total - start):
            raise SystemExit(f"short page at {start}: got {len(page)} characters")
        pages.append(page)
    rows = json.loads(zlib.decompress(base64.b64decode("".join(pages))))
    if cache:
        cache.parent.mkdir(parents=True, exist_ok=True)
        cache.write_text(json.dumps(rows))
    return rows


def mask_words(code):
    """One function's words with link-time fields zeroed, as a tuple."""
    out = []
    for i in range(0, len(code) - len(code) % 4, 4):
        w = struct.unpack_from("<I", code, i)[0]
        op = w >> 26
        if op in (2, 3):
            w &= 0xFC000000
        elif op in IMM_OPS:
            w &= 0xFFFF0000
        out.append(w)
    return tuple(out)


def callees(code, addr):
    """jal targets inside one function, as a set of addresses."""
    out = set()
    for i in range(0, len(code) - len(code) % 4, 4):
        w = struct.unpack_from("<I", code, i)[0]
        if w >> 26 == 3:
            out.add(((addr + i) & 0xF0000000) | ((w & 0x03FFFFFF) << 2))
    return out


class Side:
    """One build's function inventory plus its .text bytes."""

    def __init__(self, rows, elf_path, text_end):
        self.text = Path(elf_path).read_bytes()
        self.end = text_end
        self.funcs = [f for f in (
            {"addr": r[0], "size": r[1], "insns": r[2], "masked": r[3], "raw": r[4],
             "name": r[5]}
            for r in rows) if TEXT_START <= f["addr"] < text_end]
        self.funcs.sort(key=lambda f: f["addr"])
        self.addrs = [f["addr"] for f in self.funcs]
        self.by_addr = {f["addr"]: f for f in self.funcs}
        for f in self.funcs:
            code = self.bytes(f["addr"], f["size"])
            f["callees"] = callees(code, f["addr"]) if f["masked"] else set()
        self.ends = [f["addr"] + f["size"] for f in self.funcs]

    def bytes(self, addr, size):
        off = addr - DELTA
        return self.text[off:off + size]

    def marked(self, addr):
        """The function containing addr, or None if the bytes are unmarked."""
        i = bisect_right(self.addrs, addr) - 1
        if i < 0:
            return None
        f = self.funcs[i]
        return f if addr < f["addr"] + f["size"] else None


def longest_increasing(pairs):
    """Keep the longest run of pairs rising on both sides (patience sort on the b side)."""
    tails, tail_at, prev = [], [], [-1] * len(pairs)
    for i, (_a, b) in enumerate(pairs):
        j = bisect_right(tails, b)
        if j == len(tails):
            tails.append(b)
            tail_at.append(i)
        else:
            tails[j] = b
            tail_at[j] = i
        prev[i] = tail_at[j - 1] if j else -1
    out = []
    i = tail_at[-1] if tail_at else -1
    while i >= 0:
        out.append(pairs[i])
        i = prev[i]
    return out[::-1]


def anchor_pairs(usa, maniax, min_size):
    """Pairs whose masked hash is unique on both sides, cut to one monotone run."""
    def index(side):
        by_hash = defaultdict(list)
        for f in side.funcs:
            if f["masked"] and f["size"] >= min_size:
                by_hash[f["masked"]].append(f["addr"])
        return by_hash

    a, b = index(usa), index(maniax)
    raw = sorted((a[k][0], b[k][0]) for k in set(a) & set(b)
                 if len(a[k]) == 1 == len(b[k]))
    kept = longest_increasing(raw)
    return kept, len(raw)


def score(a, b):
    """How strongly two functions look like the same code. 0 means do not pair."""
    if a["masked"] and a["masked"] == b["masked"]:
        return 10, "masked-hash"
    if a["size"] == b["size"] and a["insns"] == b["insns"] and len(a["callees"]) == len(b["callees"]):
        return 5, "size+insns+calls"
    if a["size"] == b["size"]:
        return 3, "size"
    if abs(a["size"] - b["size"]) <= SIZE_SLACK and len(a["callees"]) == len(b["callees"]):
        return 1, "near-size+calls"
    return 0, ""


def align_gap(xs, ys, band):
    """Monotone alignment of two short runs, maximizing total score. Skips are free."""
    n, m = len(xs), len(ys)
    if not n or not m:
        return []
    best = [[0] * (m + 1) for _ in range(n + 1)]
    move = [[0] * (m + 1) for _ in range(n + 1)]
    for i in range(1, n + 1):
        lo = max(1, i - band)
        hi = min(m, i + band)
        for j in range(lo, hi + 1):
            s, _why = score(xs[i - 1], ys[j - 1])
            take = best[i - 1][j - 1] + s if s else -1
            skip_a = best[i - 1][j]
            skip_b = best[i][j - 1]
            if take >= skip_a and take >= skip_b:
                best[i][j], move[i][j] = take, 1
            elif skip_a >= skip_b:
                best[i][j], move[i][j] = skip_a, 2
            else:
                best[i][j], move[i][j] = skip_b, 3
        for j in range(hi + 1, m + 1):
            best[i][j], move[i][j] = best[i][j - 1], 3
    out = []
    i, j = n, m
    while i > 0 and j > 0:
        step = move[i][j]
        if step == 1:
            out.append((xs[i - 1], ys[j - 1]))
            i -= 1
            j -= 1
        elif step == 2:
            i -= 1
        elif step == 3:
            j -= 1
        else:
            break
    return out[::-1]


def gap_fill(usa, maniax, pairs, band):
    """Align the functions left between consecutive anchors."""
    taken_a = set(pairs)
    taken_b = set(pairs.values())
    bounds = sorted(pairs.items())
    out = []
    edges = [(TEXT_START - 1, TEXT_START - 1)] + bounds + [(usa.end, maniax.end)]
    for (a0, b0), (a1, b1) in zip(edges, edges[1:]):
        xs = [f for f in usa.funcs if a0 < f["addr"] < a1 and f["addr"] not in taken_a]
        ys = [f for f in maniax.funcs if b0 < f["addr"] < b1 and f["addr"] not in taken_b]
        for a, b in align_gap(xs, ys, band):
            s, why = score(a, b)
            out.append((a["addr"], b["addr"], s, why))
    return out


def callee_agreement(a, b, pairs):
    """Of a's calls to paired functions, how many land on b's matching call."""
    mapped = {pairs[c] for c in a["callees"] if c in pairs}
    if not mapped:
        return 0, 0
    return len(mapped & b["callees"]), len(mapped)


def interpolate(addr, bounds):
    """The Maniax address a USA address should sit at, from the nearest pairs."""
    i = bisect_right([a for a, _ in bounds], addr) - 1
    if i < 0:
        return bounds[0][1] - (bounds[0][0] - addr) if bounds else None
    a, b = bounds[i]
    return b + (addr - a)


def find_unmarked(usa, maniax, func, expect, window, lo_bound, hi_bound):
    """Where func's masked code sits in unclaimed Maniax bytes, or None.

    Ambiguous when the masked code occurs more than once in the window, which is what a
    short stub does, so a second hit rejects the candidate instead of picking one."""
    want = mask_words(usa.bytes(func["addr"], func["size"]))
    if not want:
        return None
    lo = max(TEXT_START, lo_bound, expect - window)
    hi = min(maniax.end - func["size"], hi_bound - func["size"], expect + window)
    hit = None
    for addr in range(lo, hi + 1, 4):
        if mask_words(maniax.bytes(addr, func["size"])) == want:
            if hit is not None:
                return None
            hit = addr
    return hit


COLUMNS = [
    ("kind", "pair, pair-unmarked, usa-only or maniax-only"),
    ("usa_addr", "USA function start vram"),
    ("usa_name", "USA function name from the IDB"),
    ("usa_size", "USA function size in bytes"),
    ("maniax_addr", "paired Maniax vram"),
    ("maniax_size", "paired Maniax function size, or the matched region length"),
    ("delta", "maniax_addr - usa_addr"),
    ("tier", "high, medium, low or none"),
    ("evidence", "what produced the pair"),
]
FIELDS = [name for name, _ in COLUMNS]

PREAMBLE = [
    "Function pairing from SLUS_209.11 (USA) onto SLPM_654.62 (Nocturne Maniax).",
    "Produced by maniax_pair.py. Masked-code anchors, then monotone sequence",
    "alignment per gap, then a byte search for counterparts IDA has not marked as",
    "functions in the Maniax IDB. Complete accounting: every function on either",
    "side appears exactly once.",
]


def header():
    lines = [f"# {line}" for line in PREAMBLE] + ["#", "# Columns, tab separated:"]
    width = max(len(name) for name in FIELDS)
    lines += [f"#   {name:<{width}}  {what}" for name, what in COLUMNS]
    return "\n".join(lines) + "\n"


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--from-instance", default="SLUS_209.11",
                    help="donor IDB, the side whose names are being carried")
    ap.add_argument("--to-instance", required=True, help="recipient IDB")
    ap.add_argument("--from-elf", default=str(ROOT / "orig/smt3/SLUS_209.11"))
    ap.add_argument("--to-elf", required=True)
    ap.add_argument("--from-text-end", type=lambda s: int(s, 0), default=0x002E6E44)
    ap.add_argument("--to-text-end", type=lambda s: int(s, 0), required=True)
    ap.add_argument("--min-size", type=int, default=MIN_SIZE)
    ap.add_argument("--band", type=int, default=BAND)
    ap.add_argument("--window", type=lambda s: int(s, 0), default=SEARCH_WINDOW)
    ap.add_argument("--cache-dir", type=Path, default=HERE / "cache")
    ap.add_argument("--output", type=Path, required=True)
    ap.add_argument("--deltas", type=Path)
    args = ap.parse_args()

    def cache(stem):
        return args.cache_dir / f"{stem}.json" if args.cache_dir else None

    usa = Side(collect(args.from_instance, cache("from")), args.from_elf, args.from_text_end)
    maniax = Side(collect(args.to_instance, cache("to")), args.to_elf, args.to_text_end)
    print(f"{args.from_instance}: {len(usa.funcs)} functions   "
          f"{args.to_instance}: {len(maniax.funcs)} functions")

    anchors, raw_count = anchor_pairs(usa, maniax, args.min_size)
    pairs = dict(anchors)
    tier = {a: ("high", "unique-masked-hash") for a, _ in anchors}
    print(f"anchors: {len(anchors)} kept of {raw_count} unique-masked-hash candidates")

    filled = gap_fill(usa, maniax, pairs, args.band)
    for a, b, s, why in filled:
        pairs[a] = b
        if s == 10:
            tier[a] = ("high", "masked-hash-in-gap")
        elif s == 5:
            tier[a] = ("medium", why)
        else:
            tier[a] = ("low", why)
    print(f"gap alignment added {len(filled)} pairs")

    # Callee agreement is only computable once the pairing exists, so it promotes
    # weak pairs rather than producing them.
    promoted = 0
    for a, (level, why) in list(tier.items()):
        if level != "low":
            continue
        hit, total = callee_agreement(usa.by_addr[a], maniax.by_addr[pairs[a]], pairs)
        if total >= 3 and hit >= total * 0.75:
            tier[a] = ("medium", f"{why}+callees {hit}/{total}")
            promoted += 1
    print(f"{promoted} low pairs promoted to medium on callee agreement")

    bounds = sorted(pairs.items())
    anchor_addrs = [a for a, _ in bounds]
    unmarked = overlap = ambiguous = 0
    unmarked_at = {}
    claimed = set(pairs.values())
    for f in usa.funcs:
        if f["addr"] in pairs or f["size"] < args.min_size or not f["masked"]:
            continue
        expect = interpolate(f["addr"], bounds)
        if expect is None:
            continue
        # The counterpart must sit between the neighbouring pairs, so neither a backwards
        # hit nor a stub that matches everywhere can enter the pairing.
        i = bisect_right(anchor_addrs, f["addr"])
        lo_bound = (bounds[i - 1][1] + usa.by_addr[bounds[i - 1][0]]["size"]) if i else TEXT_START
        hi_bound = bounds[i][1] if i < len(bounds) else maniax.end
        hit = find_unmarked(usa, maniax, f, expect, args.window, lo_bound, hi_bound)
        if hit is None:
            ambiguous += 1
            continue
        if hit in claimed or maniax.marked(hit) is not None:
            overlap += 1
            continue
        unmarked_at[f["addr"]] = hit
        claimed.add(hit)
        unmarked += 1
    print(f"{unmarked} USA functions matched unmarked Maniax bytes, {overlap} matched inside "
          f"an already paired or marked function, {ambiguous} found no unique hit")

    rows = []
    for f in usa.funcs:
        a = f["addr"]
        if a in pairs:
            b = maniax.by_addr[pairs[a]]
            level, why = tier[a]
            if f["raw"] and f["raw"] == b["raw"]:
                why += "+byte-identical"
            rows.append(("pair", a, f["name"], f["size"], b["addr"], b["size"],
                         b["addr"] - a, level, why))
        elif a in unmarked_at:
            b = unmarked_at[a]
            rows.append(("pair-unmarked", a, f["name"], f["size"], b, f["size"],
                         b - a, "high", "masked-bytes-at-unmarked-region"))
        else:
            rows.append(("usa-only", a, f["name"], f["size"], "", "", "", "none",
                         "no counterpart" if f["masked"] else "unhashable (split chunks)"))
    for f in maniax.funcs:
        if f["addr"] not in claimed:
            rows.append(("maniax-only", "", "", "", f["addr"], f["size"], "", "none",
                         "no counterpart"))

    def fmt(v):
        return f"0x{v:08X}" if isinstance(v, int) and v > 0xFFFF else str(v)

    body = []
    for kind, ua, un, us, ma, ms, d, level, why in rows:
        body.append("\t".join([kind, fmt(ua), str(un), str(us), fmt(ma), str(ms),
                               str(d), level, why]))
    args.output.write_text(header() + "\t".join(FIELDS) + "\n" + "\n".join(body) + "\n")
    print(f"wrote {args.output}")

    # Delta runs: the pairing is only credible if maniax_addr - usa_addr is piecewise
    # constant, stepping at insertions and deletions.
    steps = []
    last = None
    for a, b in sorted(list(pairs.items()) + list(unmarked_at.items())):
        d = b - a
        if last is None or d != last[0]:
            steps.append([d, a, a, 1])
            last = steps[-1]
        else:
            last[2], last[3] = a, last[3] + 1
    if args.deltas:
        args.deltas.write_text(
            "delta\tfirst_from_addr\tlast_from_addr\tpairs\n"
            + "\n".join(f"{d}\t0x{lo:08X}\t0x{hi:08X}\t{n}" for d, lo, hi, n in steps) + "\n")
    print(f"{len(steps)} delta runs" + (f", wrote {args.deltas}" if args.deltas else ""))

    counts = defaultdict(int)
    for _kind, _ua, _un, _us, _ma, _ms, _d, level, _why in rows:
        counts[level] += 1
    print("tiers: " + "  ".join(f"{k}={counts[k]}" for k in ("high", "medium", "low", "none")))


if __name__ == "__main__":
    main()
