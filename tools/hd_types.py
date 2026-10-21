#!/usr/bin/env python3
"""Export the SMT3HD reconstruction's struct members to config/smt3/hd_types.txt.

Member names and their order are what this corpus is for. Sizes and offsets are
advisory only: the remaster rewrote inline arrays and embedded structs as pointers
(`u8 Name[24]` became `u8 *Name`) and appends its own fields, so 45 of the 146
sizes the project has proven disagree. Checked in because the HD tree is not part
of this repo, as with config/smt3/ida_types.txt."""

import argparse
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
DEFAULT_HD = Path("/home/wsl/projects/smt3hd-reconstructed-sources")
DEFAULT_OUT = ROOT / "config" / "smt3" / "hd_types.txt"
DEFAULT_ENUMS = ROOT / "config" / "smt3" / "hd_enums.txt"

# PS2 ABI: ee-gcc 2.96, 32-bit pointers.
BASE = {
    "s8": 1, "u8": 1, "char": 1, "bool": 1, "_Bool": 1,
    "s16": 2, "u16": 2, "short": 2,
    "s32": 4, "u32": 4, "int": 4, "long": 4, "f32": 4, "float": 4,
    "s64": 8, "u64": 8, "f64": 8, "double": 8, "long long": 8,
    "s128": 16, "u128": 16,
    "void": 0,
}
# types.h spells these as anonymous-struct typedefs, so they never appear as
# `struct X {` and have to be stated.
VECTORS = {
    "sdfVector2_t": (8, 4), "sdfVector3_t": (12, 4), "sdfVector4_t": (16, 4),
    "sdfQuaternion_t": (16, 4), "sdfMatrix4_t": (64, 4),
    "sdfColor_t": (16, 4), "sdfColor32_t": (4, 1),
}

STRUCT_OPEN = re.compile(r"^(struct|union)\s+(\w+)\s*\{")
MEMBER = re.compile(r"^\s+(.+?)\s*;\s*$")
DECL = re.compile(r"^(.*?)(\**)\s*(\w+)\s*((?:\[[^\]]*\])*)$")

ENUM_OPEN = re.compile(r"^typedef\s+enum\b[^{]*\{")
ENUM_CLOSE = re.compile(r"^\}\s*(\w+)?\s*;")
ENUM_MEMBER = re.compile(r"^\s*([A-Za-z_]\w*)\s*(?:=\s*(-?(?:0[xX])?[0-9A-Fa-f]+))?\s*,?\s*$")


def parse_enums(hd):
    """[(typedef name, file, [(member, value)])] for every HD enum.

    Values are explicit in the reconstruction, which is what makes a bare literal
    in our tree safe to replace with the member that carries the same value."""
    out = []
    for path in sorted((hd / "include").glob("*.h")):
        lines = path.read_text(errors="replace").splitlines()
        i = 0
        while i < len(lines):
            if not ENUM_OPEN.match(lines[i]):
                i += 1
                continue
            members, i, nxt = [], i + 1, 0
            while i < len(lines):
                close = ENUM_CLOSE.match(lines[i])
                if close:
                    out.append((close.group(1), path.name, members))
                    break
                m = ENUM_MEMBER.match(lines[i])
                if m:
                    if m.group(2) is None:
                        value = nxt
                    else:
                        value = int(m.group(2), 0)
                    members.append((m.group(1), value))
                    nxt = value + 1
                i += 1
            i += 1
    return out


def parse(hd):
    """name -> (kind, members, file) for every HD struct and union."""
    out = {}
    for path in sorted((hd / "include").glob("*.h")):
        lines = path.read_text(errors="replace").splitlines()
        i = 0
        while i < len(lines):
            m = STRUCT_OPEN.match(lines[i])
            if not m:
                i += 1
                continue
            kind, name = m.group(1), m.group(2)
            members, i = [], i + 1
            while i < len(lines) and not lines[i].startswith("}"):
                mm = MEMBER.match(lines[i])
                if mm and not mm.group(1).startswith(("//", "/*")):
                    d = DECL.match(mm.group(1).strip())
                    if d:
                        dims = re.findall(r"\[([^\]]*)\]", d.group(4))
                        members.append((d.group(1).strip(), len(d.group(2)), d.group(3), dims))
                i += 1
            out[name] = (kind, members, path.name)
            i += 1
    return out


def sizes(types):
    """Resolve each struct's size and member offsets. name -> (size, align, rows)."""
    done, busy = {}, set()

    def size_align(text, stars):
        if stars:
            return 4, 4
        text = re.sub(r"\b(struct|union|const|volatile|unsigned|signed)\b", " ", text).strip()
        text = re.sub(r"\s+", " ", text)
        if text in BASE:
            return BASE[text] or 1, BASE[text] or 1
        if text in VECTORS:
            return VECTORS[text]
        if text in types:
            r = resolve(text)
            return (r[0], r[1]) if r else (None, None)
        return None, None

    def resolve(name):
        if name in done:
            return done[name]
        if name in busy or name not in types:
            return None  # a self-reference only ever appears behind a pointer
        busy.add(name)
        kind, members, _ = types[name]
        off, align, rows, ok = 0, 1, [], True
        for text, stars, mname, dims in members:
            sz, al = size_align(text, stars)
            if sz is None:
                ok = False
                break
            count = 1
            for d in dims:
                try:
                    count *= int(d, 0)
                except ValueError:
                    count *= 1  # UNKNOWN_ARRAY_SIZE
            total = sz * count
            if kind == "union":
                rows.append((0, total, text + "*" * stars, mname, dims))
                off, align = max(off, total), max(align, al)
                continue
            off = (off + al - 1) // al * al
            rows.append((off, total, text + "*" * stars, mname, dims))
            off += total
            align = max(align, al)
        busy.discard(name)
        if not ok:
            done[name] = None
            return None
        done[name] = ((off + align - 1) // align * align, align, rows)
        return done[name]

    for name in types:
        resolve(name)
    return done


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--hd", type=Path, default=DEFAULT_HD)
    ap.add_argument("--out", type=Path, default=DEFAULT_OUT)
    ap.add_argument("--enums-out", type=Path, default=DEFAULT_ENUMS)
    args = ap.parse_args()

    enums = parse_enums(args.hd)
    rows = [
        "# SMT3HD reconstruction enums, from tools/hd_types.py.",
        "# E <typedef name|-> <file> <count>   then one V row per member:",
        "# V <value> <name>",
    ]
    for name, src, members in sorted(enums, key=lambda e: (e[0] or "", e[1])):
        rows.append(f"E {name or '-'} {src} {len(members)}")
        for mname, value in members:
            rows.append(f"V {value} {mname}")
    args.enums_out.write_text("\n".join(rows) + "\n")
    print(f"{len(enums)} HD enums, {sum(len(m) for _, _, m in enums)} members "
          f"-> {args.enums_out}")

    types = parse(args.hd)
    resolved = sizes(types)
    lines = [
        "# SMT3HD reconstruction struct members, from tools/hd_types.py.",
        "# Member names and order are authoritative; size and offset are ADVISORY,",
        "# because the remaster rewrote inline arrays and embedded structs as pointers.",
        "# T <name> <kind> <size|?> <file>   then one M row per member:",
        "# M <offset|?> <size|?> <type> <name>",
    ]
    ok = 0
    for name in sorted(types):
        kind, members, src = types[name]
        r = resolved.get(name)
        if r:
            ok += 1
            size, _, rows = r
            lines.append(f"T {name} {kind} {size} {src}")
            for off, sz, text, mname, _ in rows:
                lines.append(f"M {off} {sz} {text} {mname}")
        else:
            lines.append(f"T {name} {kind} ? {src}")
            for text, stars, mname, _ in members:
                lines.append(f"M ? ? {text}{'*' * stars} {mname}")
    args.out.write_text("\n".join(lines) + "\n")
    print(f"{len(types)} HD types, {ok} fully sized -> {args.out}")


if __name__ == "__main__":
    main()
