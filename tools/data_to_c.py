#!/usr/bin/env python3
"""Draft C definitions for a range of the split .data, to move a unit's .data into C.

Prints each data symbol in [start, end) with the unit's extern for it. The
output is a draft: fix types and casts by hand, then the build must match.
"""
import argparse
import pathlib
import re
import struct
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent
LINE = re.compile(r"\s*/\*\s*[0-9A-F]+\s+([0-9A-F]{8})\s+(?:[0-9A-F]+\s*)?\*/\s*\.(\w+)\s*(.*)")
IMAGE = ROOT / "orig/smt3/SLUS_209.11"
VRAM_DELTA = 0xFF000


def load_symbols():
    funcs = set()
    for line in open(ROOT / "config/smt3/symbol_addrs.txt"):
        m = re.match(r"(\w+)\s*=\s*0x[0-9A-Fa-f]+;.*type:func", line)
        if m:
            funcs.add(m.group(1))
    return funcs


def read_data(start, end):
    """Yield (name, vram, bytes, {vram: (directive, operand)}) for symbols in [start, end)."""
    image = IMAGE.read_bytes()
    labels, lines = [], {}
    for path in sorted((ROOT / "asm/smt3/data").glob("data*.data.s")):
        pending = None
        for line in open(path):
            m = re.match(r"dlabel (\w+)", line)
            if m:
                pending = m.group(1)
                continue
            m = LINE.match(line)
            if m:
                vram = int(m.group(1), 16)
                if pending:
                    labels.append((vram, pending))
                    pending = None
                lines[vram] = (m.group(2), m.group(3).strip())
    labels.sort()
    for i, (vram, name) in enumerate(labels):
        if start <= vram < end:
            stop = min(labels[i + 1][0] if i + 1 < len(labels) else end, end)
            yield name, vram, image[vram - VRAM_DELTA:stop - VRAM_DELTA], lines


def extern_decl(source, name):
    for line in source.splitlines():
        if re.search(r"\b%s\b" % re.escape(name), line) and line.lstrip().startswith("extern"):
            return line.strip()
    return None


def word_value(vram, raw, lines, funcs):
    directive, operand = lines.get(vram, ("word", ""))
    if directive == "float":
        f = struct.unpack("<f", raw)[0]
        return repr(f) + "f" if f == f else "0x%08X" % struct.unpack("<I", raw)[0]
    if directive == "word" and re.match(r"[A-Za-z_]", operand):
        m = re.match(r"(\w+)(?:\s*\+\s*(0x[0-9A-Fa-f]+))?$", operand)
        sym, off = m.group(1), m.group(2)
        if sym in funcs and not off:
            return sym
        return "&%s" % sym if not off else "(u8 *)&%s + %s" % (sym, off)
    return "0x%08X" % struct.unpack("<I", raw)[0]


def emit(name, start, data, lines, funcs, source):
    decl = extern_decl(source, name)
    out = ["/* %08X size 0x%X%s%s */" % (start, len(data), " (4 mod 8)" if start % 8 else "",
                                        "; " + decl if decl else "")]
    if start % 4 == 0 and len(data) % 4 == 0:
        vals = [word_value(start + i, data[i:i + 4], lines, funcs) for i in range(0, len(data), 4)]
        out.append("u32 %s[%d] = {" % (name, len(vals)))
        for i in range(0, len(vals), 4):
            out.append("    " + ", ".join(vals[i:i + 4]) + ",")
    else:
        out.append("u8 %s[%d] = {" % (name, len(data)))
        for i in range(0, len(data), 16):
            out.append("    " + ", ".join("0x%02X" % b for b in data[i:i + 16]) + ",")
    out.append("};")
    return "\n".join(out)


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("unit", help="unit path under src/smt3, e.g. effect/effPart")
    ap.add_argument("start", type=lambda s: int(s, 16), help="first vram, hex")
    ap.add_argument("end", type=lambda s: int(s, 16), help="end vram (exclusive), hex")
    args = ap.parse_args()
    funcs = load_symbols()
    src = ROOT / "src/smt3" / (args.unit + ".c")
    source = src.read_text() if src.exists() else ""
    found = False
    for name, vram, data, lines in read_data(args.start, args.end):
        print(emit(name, vram, data, lines, funcs, source))
        found = True
    if not found:
        sys.exit("no data symbols in range")


if __name__ == "__main__":
    main()
