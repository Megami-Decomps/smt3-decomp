#!/usr/bin/env python3
"""Find smt3 INCLUDE_ASM functions whose body is word-identical to a dds-decomp one.

Unlike tools/dds_reuse.py, which aligns the two games on function sizes, this reads
both split asm trees and compares instruction words directly, masking only the fields
the linker fills in: j/jal targets and %hi/%lo/%gp_rel immediates. An equal body means
dds-decomp's C compiles to the same instructions, and the masked operands line up
positionally, so the donor's symbol at each slot names the smt3 symbol at that slot.

    python3 tools/dds_bodies.py --dds-decomp ../dds-decomp [--port <func>]
"""

from __future__ import annotations

import argparse
import hashlib
import json
import re
import sys
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
# Below this a body says nothing: dozens of stubs share a two-instruction return.
MIN_WORDS = 16

INSN = re.compile(r"^\s*/\* [0-9A-Fa-f]+ [0-9A-Fa-f]+ ([0-9A-Fa-f]{8}) \*/\s+(.*?)\s*$")
GLABEL = re.compile(r"^\s*glabel\s+(\S+)")
INCLUDE_ASM = re.compile(r'INCLUDE_ASM\(\s*[^,]+,\s*"([^"]+)"\s*,\s*(\w+)\s*\)')
RELOC = re.compile(r"%(hi|lo|gp_rel)\(([^)]+)\)")
# A C definition at column zero: return type, name, open paren.
C_DEF = re.compile(r"^\w[\w \*]*?(\w+)\(", re.M)


def parse_asm(path):
    """Every glabel body in one splat .s file as name -> (masked words, symbols)."""
    out, name, words, syms = {}, None, [], []
    for line in path.read_text(errors="replace").splitlines():
        label = GLABEL.match(line)
        if label:
            if name and words:
                out[name] = (words, syms)
            name, words, syms = label.group(1), [], []
            continue
        insn = INSN.match(line)
        if not insn or name is None:
            continue
        word = int.from_bytes(bytes.fromhex(insn.group(1)), "little")
        text = insn.group(2)
        op = word >> 26
        reloc = RELOC.search(text)
        if op in (2, 3):
            words.append(word & 0xFC000000)
            syms.append(("call", text.split()[-1]))
        elif reloc:
            words.append(word & 0xFFFF0000)
            syms.append((reloc.group(1), reloc.group(2)))
        else:
            words.append(word)
    if name and words:
        out[name] = (words, syms)
    return out


def digest(words):
    return hashlib.sha1(b"".join(w.to_bytes(4, "little") for w in words)).hexdigest()[:16]


def smt3_fallbacks():
    """One row per INCLUDE_ASM line that the build actually compiles."""
    ninja = (ROOT / "build.ninja").read_text()
    rows = defaultdict(list)
    for src in sorted(ROOT.glob("src/smt3/**/*.c")):
        rel = str(src.relative_to(ROOT))
        if rel not in ninja:
            continue
        for unit, fn in INCLUDE_ASM.findall(src.read_text(errors="replace")):
            asm = ROOT / "asm/smt3/nonmatchings" / unit / f"{fn}.s"
            if not asm.exists():
                continue
            body = parse_asm(asm).get(fn)
            if not body or len(body[0]) < MIN_WORDS:
                continue
            rows[digest(body[0])].append(
                {"fn": fn, "unit": unit, "src": rel, "words": len(body[0]), "syms": body[1]})
    return rows


def dds_donors(dds):
    """One row per dds-decomp function that is written out in C."""
    rows = defaultdict(list)
    for ver in ("dds1", "dds2"):
        skip, home = set(), {}
        for src in dds.glob(f"src/{ver}/**/*.c"):
            text = src.read_text(errors="replace")
            for _, fn in INCLUDE_ASM.findall(text):
                skip.add(fn)
            for m in C_DEF.finditer(text):
                home.setdefault(m.group(1), str(src.relative_to(dds)))
        for asm in (dds / f"asm/{ver}/matchings").rglob("*.s"):
            for fn, (words, syms) in parse_asm(asm).items():
                if fn in skip or len(words) < MIN_WORDS:
                    continue
                rows[digest(words)].append(
                    {"fn": fn, "ver": ver, "src": home.get(fn), "words": len(words),
                     "syms": syms})
    return rows


def symbol_map(smt3, donor):
    """Donor symbol -> smt3 symbol, read off the positionally aligned masked slots."""
    pairs = defaultdict(set)
    for (ka, a), (kb, b) in zip(smt3["syms"], donor["syms"]):
        if ka == kb:
            pairs[b].add(a)
    return {k: sorted(v) for k, v in sorted(pairs.items())}


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--dds-decomp", default="../dds-decomp", type=Path)
    ap.add_argument("--json", type=Path, help="write the full hit list here")
    ap.add_argument("--port", help="print the symbol map for one smt3 function")
    args = ap.parse_args()

    dds = args.dds_decomp.expanduser().resolve()
    if not (dds / "asm/dds1").is_dir():
        sys.exit(f"no dds-decomp checkout at {dds}")

    fallbacks, donors = smt3_fallbacks(), dds_donors(dds)
    hits = []
    for h, rows in fallbacks.items():
        if h not in donors:
            continue
        for row in rows:
            hits.append({"smt3": row, "donors": donors[h],
                         "map": symbol_map(row, donors[h][0])})
    hits.sort(key=lambda x: -x["smt3"]["words"])

    if args.port:
        for hit in hits:
            if hit["smt3"]["fn"] != args.port:
                continue
            print(f"{hit['smt3']['unit']}/{hit['smt3']['fn']}  {hit['smt3']['words'] * 4} bytes")
            for d in hit["donors"]:
                print(f"  donor {d['ver']} {d['fn']} {d['src']}")
            for donor_sym, smt3_syms in hit["map"].items():
                print(f"  {donor_sym:44s} -> {', '.join(smt3_syms)}")
            return
        sys.exit(f"{args.port} has no word-identical donor")

    total = sum(h["smt3"]["words"] * 4 for h in hits)
    for hit in hits:
        s, d = hit["smt3"], hit["donors"][0]
        print(f"{s['words'] * 4:6d}  {s['unit']}/{s['fn']}\n"
              f"        {d['ver']} {d['fn']} {d['src']}")
    print(f"\n{len(hits)} functions, {total} bytes (0x{total:X})")
    if args.json:
        args.json.write_text(json.dumps(hits, indent=1))


if __name__ == "__main__":
    main()
