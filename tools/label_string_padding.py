#!/usr/bin/env python3
"""Give a name to retail bytes spimdisasm folds into string alignment padding.

spimdisasm's string renderer emits `.asciz` plus the `.align` that pads it to
a word, but never checks whether a named symbol sits inside that padding, so
a flag byte declared right after a string gets no dlabel anywhere. Byte
values are unchanged: `.align` already pads with the same zero.

    python3 tools/label_string_padding.py [smt3]
"""
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent

STRING = re.compile(
    r'(?P<head>    /\* (?P<fileoff>[0-9A-F]+) (?P<addr>[0-9A-F]{8}) \*/ \.asciz "(?P<text>[^"\\]*)"\n'
    r'    /\* (?P<dump>[0-9A-F]+) \*/\n)'
    r'\.align (?P<shift>\d+)\n'
)
ROW = re.compile(r"^(\w+) = 0x([0-9A-Fa-f]+);(.*)$", re.M)
DLABEL = re.compile(r"^dlabel (\w+)$", re.M)
D_NAME = re.compile(r"D_[0-9A-F]{8}")


def named_bytes(version):
    """u8-sized symbols from symbol_addrs.txt, plus splat's own D_ auto-names
    for the same addresses (from undefined_syms_auto.txt), that have no
    dlabel anywhere in the generated asm yet."""
    defined = {m.group(1) for m in DLABEL.finditer(
        "\n".join(p.read_text() for p in (ROOT / "asm" / version).rglob("*.s")))}
    candidates = {}
    sa = ROOT / "config" / version / "symbol_addrs.txt"
    for name, addr, attrs in ROW.findall(sa.read_text()):
        if "type:u8" in attrs and "size:0x1" in attrs and name not in defined:
            candidates[int(addr, 16)] = name
    auto = ROOT / "config" / version / "undefined_syms_auto.txt"
    if auto.exists():
        for name, addr, _ in ROW.findall(auto.read_text()):
            if D_NAME.fullmatch(name) and name not in defined and int(addr, 16) not in candidates:
                candidates[int(addr, 16)] = name
    return candidates


def split_padding(match, candidates):
    addr = int(match["addr"], 16)
    fileoff = int(match["fileoff"], 16)
    pad_end = addr + len(match["dump"]) // 2
    gap_start = addr + len(match["text"]) + 1
    shift = int(match["shift"])
    if gap_start > pad_end or pad_end - gap_start >= (1 << shift):
        return match.group(0)
    names = [candidates.get(a) for a in range(gap_start, pad_end)]
    if not any(names):
        return match.group(0)
    out = [match["head"]]
    for i, name in enumerate(names):
        a = gap_start + i
        off = fileoff + (a - addr)
        if name is None:
            out.append(f"    /* {off:06X} {a:08X} */ .byte 0x00\n")
        else:
            out.append(f"\nnonmatching {name}\n\ndlabel {name}\n"
                        f"    /* {off:06X} {a:08X} */ .byte 0x00\n"
                        f"enddlabel {name}\n")
    return "".join(out)


def label(version):
    candidates = named_bytes(version)
    hit = set()

    def repl(m):
        new = split_padding(m, candidates)
        if new != m.group(0):
            hit.update(re.findall(r"^nonmatching (\w+)$", new, re.M))
        return new

    for path in sorted((ROOT / "asm" / version).rglob("*.s")):
        text = path.read_text()
        new_text = STRING.sub(repl, text)
        if new_text != text:
            path.write_text(new_text)
    print(f"{version}: labeled {len(hit)} bytes folded into string padding")


if __name__ == "__main__":
    for v in sys.argv[1:] or ["smt3"]:
        label(v)
