#!/usr/bin/env python3
"""Name unk_ members from the IDB, keyed on offset.

A member is renamed only when our computed offset and extent both equal an IDB
member's, so one `u8[8]` covering two IDB fields is refused. Our own unk<NN> names
check the layout model first, NN being the offset the name claims: a struct where any
of them disagrees is skipped whole."""

import argparse
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import ida_members  # noqa: E402
from cparse import struct_fields  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
IDA_TYPES = ROOT / "config" / "smt3" / "ida_types.txt"

UNNAMED = re.compile(r"^(?:unk|field_0x)_?([0-9A-Fa-f]+)$")
WORTHLESS = re.compile(r"^(?:pad+\d*|padd\d*|resv\d*|reserve[d]?\d*|dummy\d*|unused\d*"
                       r"|unk\w*|field_\w+)$", re.I)

BASE = {
    "s8": 1, "u8": 1, "char": 1, "bool": 1, "_Bool": 1, "s16": 2, "u16": 2, "short": 2,
    "s32": 4, "u32": 4, "int": 4, "long": 4, "f32": 4, "float": 4, "s64": 8, "u64": 8,
    "f64": 8, "double": 8, "s128": 16, "u128": 16, "void": 0,
}
VECTORS = {
    "sdfVector2_t": (8, 4), "sdfVector3_t": (16, 16), "sdfVector4_t": (16, 16),
    "sdfQuaternion_t": (16, 16), "sdfMatrix4_t": (64, 16), "sdfColor_t": (16, 4),
    "sdfColor32_t": (4, 1),
}
QWORD = re.compile(r"\bQWORD_ALIGNED\b")


def ida_sizes():
    out = {}
    if not IDA_TYPES.exists():
        return out
    for line in IDA_TYPES.read_text().splitlines():
        if line and not line.startswith("#"):
            parts = line.split()
            if len(parts) == 3 and parts[2].lstrip("-").isdigit():
                size = int(parts[2])
                if size > 0:
                    out[parts[0]] = size
    return out


def extent(text, stars, dims, sizes):
    """(bytes, alignment) for one declarator, or (None, None) if the type is unknown."""
    if stars:
        unit, align = 4, 4
    else:
        clean = re.sub(r"\b(struct|union|const|volatile|unsigned|signed)\b", " ", text)
        clean = re.sub(r"\s+", " ", clean).strip()
        if clean in BASE:
            unit = align = BASE[clean] or 1
        elif clean in VECTORS:
            unit, align = VECTORS[clean]
        elif clean in sizes:
            unit = sizes[clean]
            align = 16 if unit % 16 == 0 and unit >= 16 else min(unit, 8) or 1
        else:
            return None, None
    count = 1
    for dim in dims:
        try:
            count *= int(dim, 0)
        except ValueError:
            return None, None
    return unit * count, align


def layout(fields, sizes):
    """[(offset, size, name)], or None when any member's type is unknown."""
    offset, rows = 0, []
    for text, stars, name, dims in fields:
        size, align = extent(text, stars, dims, sizes)
        if size is None:
            return None
        offset = (offset + align - 1) // align * align
        rows.append((offset, size, name))
        offset += size
    return rows


def candidates(path, sizes, idb):
    """[(type, line, [(old, new)])] for one file."""
    text = path.read_text(errors="replace")
    out = []
    for kind, name, fields, line in struct_fields(text):
        if not name or name not in idb:
            continue
        if not any(UNNAMED.match(f[2]) for f in fields):
            continue
        if kind == "union":
            continue
        rows = layout(fields, sizes)
        if rows is None:
            continue
        # Our own offset-named members check the model before it is used.
        claims = [(off, nm) for off, _, nm in rows if UNNAMED.match(nm)]
        if any(int(UNNAMED.match(nm).group(1), 16) != off for off, nm in claims):
            continue
        ikind, isize, imembers = idb[name]
        if rows and rows[-1][0] + rows[-1][1] > isize:
            continue  # our struct is larger than the IDB's: not the same type
        by_offset = {off: (sz, mname) for off, sz, _, mname in imembers}
        # A name already in our struct cannot be reused: tagpipeParts has mdl at
        # offset 6 where the IDB has it at 5, and taking the IDB's name for the
        # member at 5 would declare mdl twice.
        taken = {nm for _, _, nm in rows}
        picks = []
        for off, size, nm in rows:
            if not UNNAMED.match(nm) or off not in by_offset:
                continue
            isz, iname = by_offset[off]
            if isz != size or not iname or WORTHLESS.match(iname) or iname in taken:
                continue
            picks.append((nm, iname))
            taken.add(iname)
        if picks:
            out.append((name, line, picks))
    return out


def rename_in(path, tname, picks):
    """Rename inside every copy of tname this file defines, and in the file's uses
    when no other struct here declares the same member name."""
    text = path.read_text()
    done, deferred = 0, []
    for old, new in picks:
        shared = any(n != tname and any(f[2] == old for f in fields)
                     for _, n, fields, _ in struct_fields(text))
        lines = text.split("\n")
        for _, sname, _, line in struct_fields(text):
            if sname != tname:
                continue
            start, depth = line - 1, 0
            end = start
            for i in range(start, len(lines)):
                depth += lines[i].count("{") - lines[i].count("}")
                if depth == 0 and i > start:
                    end = i
                    break
            body = "\n".join(lines[start:end + 1])
            swapped, n = re.subn(rf"\b{re.escape(old)}\b", new, body)
            if n:
                lines[start:end + 1] = swapped.split("\n")
                done += 1
        text = "\n".join(lines)
        if shared:
            deferred.append(old)
        else:
            text = re.sub(rf"\b{re.escape(old)}\b", new, text)
    path.write_text(text)
    return done, deferred


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--apply", action="store_true")
    ap.add_argument("--limit", type=int, help="only the first N structs")
    args = ap.parse_args()

    sizes, idb = ida_sizes(), ida_members.load(ROOT / "config/smt3/ida_members.txt")
    if not idb:
        sys.exit("config/smt3/ida_members.txt missing; run tools/ida_members.py")

    files = sorted(list((ROOT / "src").rglob("*.c")) + list((ROOT / "include").rglob("*.h")))
    total, structs_hit, deferred_all, in_headers = 0, 0, [], []
    for path in files:
        found = candidates(path, sizes, idb)
        if args.limit and structs_hit >= args.limit:
            break
        for tname, line, picks in found:
            rel = path.relative_to(ROOT).as_posix()
            # A struct in a header is used across the tree, and an offset-named member
            # like unk04 belongs to dozens of other structs too, so a tree-wide rename
            # would hit the wrong ones. Those are reported for hand work.
            if path.suffix == ".h":
                in_headers.append((tname, rel, line, picks))
                continue
            print(f"=== {tname}   {rel}:{line}")
            for old, new in picks:
                print(f"    {old:<22} -> {new}")
            total += len(picks)
            structs_hit += 1
            if args.apply:
                _, deferred = rename_in(path, tname, picks)
                deferred_all += [f"{rel}  {tname}.{d}" for d in deferred]
    print(f"\n{total} member(s) in {structs_hit} struct(s) named by offset")
    if in_headers:
        print(f"\n{len(in_headers)} struct(s) defined in a header, left for hand work "
              f"because their uses are spread across the tree:")
        for tname, rel, line, picks in in_headers:
            names = ", ".join(f"{o} -> {n}" for o, n in picks)
            print(f"  {tname}  {rel}:{line}   {names}")
    if args.apply:
        print("relink and verify the checksum")
        if deferred_all:
            print(f"\n{len(deferred_all)} left for the compiler to attribute:")
            for row in deferred_all[:40]:
                print(f"  {row}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
