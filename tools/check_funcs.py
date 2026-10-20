#!/usr/bin/env python3
"""Compare named functions in the linked image against the retail bytes.

    python tools/check_funcs.py smt3 sceDmaGetChan sceGsGetIMR

Looks each name up in both images, so a function whose C is a different length
is still compared against its own retail span instead of its neighbour's.
"""
from __future__ import annotations

import argparse
import re
import struct
import subprocess
import sys
from pathlib import Path

import retail_elf

ROOT = Path(__file__).resolve().parent.parent
NM = ROOT / "tools" / "bin" / "mips-ps2-decompals-nm"
SYM = re.compile(r"\s*([A-Za-z_][A-Za-z0-9_]*)\s*=\s*(0x[0-9A-Fa-f]+)\s*;")


def retail_spans(version: str) -> dict[str, tuple[int, int]]:
    """{name: (vram, size)} sized to the next symbol, as the split asm does."""
    syms = sorted({(int(m.group(2), 16), m.group(1))
                   for m in (SYM.match(line) for line
                             in (ROOT / "config" / version / "symbol_addrs.txt").read_text().splitlines())
                   if m})
    addrs = sorted({a for a, _ in syms})
    out = {}
    for addr, name in syms:
        end = next((a for a in addrs if a > addr), addr)
        out[name] = (addr, end - addr)
    return out


def built_syms(elf: Path) -> dict[str, tuple[int, int]]:
    out = {}
    text = subprocess.run([str(NM), "-S", "--defined-only", str(elf)],
                          capture_output=True, text=True, check=True).stdout
    for line in text.splitlines():
        parts = line.split()
        if len(parts) == 4 and parts[2] in "tT":
            out[parts[3]] = (int(parts[0], 16), int(parts[1], 16))
    return out


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("version")
    ap.add_argument("names", nargs="+")
    ap.add_argument("--words", type=int, default=12, help="differing words to print")
    args = ap.parse_args()

    retail = (ROOT / retail_elf.RETAIL[args.version]).read_bytes()
    segs = retail_elf.load_segments(retail)
    spans = retail_spans(args.version)
    build = ROOT / "build" / args.version / f"{retail_elf.VERSIONS[args.version]['serial']}.elf"
    built = build.read_bytes()
    bsegs = retail_elf.load_segments(built)
    bsyms = built_syms(build)

    bad = 0
    for name in args.names:
        if name not in spans:
            print(f"{name:24} no retail symbol")
            bad += 1
            continue
        if name not in bsyms:
            print(f"{name:24} not in the linked image")
            bad += 1
            continue
        va, size = spans[name]
        bva, bsize = bsyms[name]
        want = retail[retail_elf.va_to_off(segs, va):][:size]
        got = built[retail_elf.va_to_off(bsegs, bva):][:bsize]
        if want == got:
            print(f"{name:24} match  {size} bytes")
            continue
        # A C body one or more instructions shorter than the retail span matches
        # when the remainder is the alignment fill .text supplies, as the
        # production report also allows.
        if bsize < size and got == want[:bsize] and set(want[bsize:]) <= {0}:
            print(f"{name:24} match  {bsize} bytes + {size - bsize} alignment fill")
            continue
        bad += 1
        print(f"{name:24} DIFF   retail {size} bytes, built {bsize}")
        shown = 0
        for i in range(0, min(size, bsize), 4):
            (w,) = struct.unpack_from("<I", want, i)
            (g,) = struct.unpack_from("<I", got, i)
            if w != g and shown < args.words:
                print(f"    +{i:03X}  retail {w:08X}  built {g:08X}")
                shown += 1
    sys.exit(1 if bad else 0)


if __name__ == "__main__":
    main()
