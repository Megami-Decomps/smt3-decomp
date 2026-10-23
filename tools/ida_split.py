#!/usr/bin/env python3
"""Replace an unnamed member with the IDB's named fields that tile its bytes.

`ida_fill.py` names an unk_ member whose offset and extent match one IDB member.
Where the IDB instead covers that range with several named fields, the member can be
split into them. That is codegen-neutral while nothing reads it, so the split is
refused when anything does, and every other member's offset and the struct's size
are recomputed and compared before the file is written."""

import argparse
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import ida_members  # noqa: E402
from cparse import struct_fields  # noqa: E402
from hd_align import struct_spans  # noqa: E402
from ida_fill import WORTHLESS, extent, ida_sizes, layout, project_sizes  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
# The names tools/check_drift.py counts as drift.
UNNAMED = re.compile(r"^(?:unk(?:nown)?(?:[_0-9]|$)|field_0x|dummy\d|unused\d)", re.I)
CLAIMS = re.compile(r"^(?:unk|field_0x)_?([0-9A-Fa-f]+)$")
IDB_TYPE = re.compile(r"^([A-Za-z_]\w*(?:\s+\w+)*?)\s*(\**)\s*((?:\[[^\]]*\])*)$")


def sources():
    return sorted(list((ROOT / "src").rglob("*.c")) + list((ROOT / "include").rglob("*.h")))


def member_uses(texts):
    """member name -> how many `.name` / `->name` uses the tree has."""
    counts = {}
    for text in texts:
        for name in re.findall(r"(?:->|\.)\s*([A-Za-z_]\w*)", text):
            counts[name] = counts.get(name, 0) + 1
    return counts


def parse_idb_type(text):
    """(base, stars, dims) for an IDB member type such as `u32[2]` or `char *`."""
    m = IDB_TYPE.match(text.strip())
    if not m:
        return None
    dims = re.findall(r"\[([^\]]*)\]", m.group(3))
    return m.group(1).strip(), len(m.group(2)), dims


def declaration(ityp, iname):
    parsed = parse_idb_type(ityp)
    if not parsed:
        return None
    base, stars, dims = parsed
    stars_text = " " + "*" * stars if stars else " "
    return f"{base}{stars_text}{iname}" + "".join(f"[{d}]" for d in dims) + ";"


def tiling(off, size, imembers, taken, sizes):
    """The IDB's named fields covering exactly [off, off+size), or None.

    Each piece has to sit where our layout engine would put it, so the alignment is
    checked too: a split that shifted a later member would change codegen."""
    pieces = [m for m in imembers if off <= m[0] < off + size]
    if len(pieces) < 2:
        return None
    cursor, out = off, []
    for ioff, isize, ityp, iname in sorted(pieces):
        if ioff != cursor or ioff + isize > off + size:
            return None
        if not iname or WORTHLESS.match(iname) or iname in taken:
            return None
        parsed = parse_idb_type(ityp)
        if not parsed:
            return None
        base, stars, dims = parsed
        ext, align = extent(base, stars, dims, sizes)
        if ext != isize or ioff % align:
            return None
        text = declaration(ityp, iname)
        if not text:
            return None
        out.append((ioff, isize, base, stars, dims, iname, text))
        cursor += isize
    return out if cursor == off + size else None


def proposals(sizes, idb, uses):
    """{type: (fields, [(old, [(name, decl)])])} for every splittable member."""
    out = {}
    for path in sources():
        for kind, name, fields, _ in struct_fields(path.read_text(errors="replace")):
            if not name or kind == "union" or name in out or name not in idb:
                continue
            rows = layout(fields, sizes)
            if not rows:
                continue
            claims = [(o, nm) for o, _, nm in rows if CLAIMS.match(nm)]
            if any(int(CLAIMS.match(nm).group(1), 16) != o for o, nm in claims):
                continue
            _, isize, imembers = idb[name]
            if rows[-1][0] + rows[-1][1] > isize:
                continue
            taken = {nm for _, _, nm in rows}
            picks = []
            for off, size, nm in rows:
                if not UNNAMED.match(nm) or uses.get(nm, 0):
                    continue
                tiles = tiling(off, size, imembers, taken, sizes)
                if not tiles:
                    continue
                picks.append((nm, [(t[5], t[6]) for t in tiles]))
                taken |= {t[5] for t in tiles}
            if picks:
                out[name] = (fields, picks)
    return out


def copies(name, fields):
    """The files defining this type with exactly these members."""
    out = []
    for path in sources():
        for _, n, f, _ in struct_fields(path.read_text(errors="replace")):
            if n == name and f == fields and path not in out:
                out.append(path)
    return out


def rewrite(path, name, picks, sizes):
    """Split the members in every definition of `name` in one file.

    Returns the number of definitions rewritten, or raises when the recomputed
    layout moves another member or changes the size."""
    text = path.read_text()
    before = {n: (o, s) for f_name, (o, s, n) in []} or None
    lines = text.split("\n")
    spans = struct_spans(lines, name)
    if not spans:
        return 0
    old_rows = None
    for _, n, f, _ in struct_fields(text):
        if n == name:
            old_rows = layout(f, sizes)
            break
    done = 0
    for first, last in reversed(spans):
        body = lines[first:last + 1]
        for old, pieces in picks:
            for i, line in enumerate(body):
                if re.search(rf"\b{re.escape(old)}\b", line) and line.rstrip().endswith(";"):
                    indent = re.match(r"\s*", line).group(0)
                    body[i:i + 1] = [indent + decl for _, decl in pieces]
                    break
        lines[first:last + 1] = body
        done += 1
    new_text = "\n".join(lines)
    # The split must leave every other member where it was, and the struct's size
    # unchanged: anything else would move a field the code already reads.
    for _, n, f, _ in struct_fields(new_text):
        if n != name:
            continue
        new_rows = layout(f, sizes)
        if new_rows is None:
            raise SystemExit(f"{path}: {name} no longer lays out")
        removed = {old for old, _ in picks}
        added = {nm for _, pieces in picks for nm, _ in pieces}
        kept_old = [(o, s, nm) for o, s, nm in old_rows if nm not in removed]
        kept_new = [(o, s, nm) for o, s, nm in new_rows if nm not in added]
        if kept_old != kept_new:
            raise SystemExit(f"{path}: {name} would move a member; refusing")
        if old_rows[-1][0] + old_rows[-1][1] != new_rows[-1][0] + new_rows[-1][1]:
            raise SystemExit(f"{path}: {name} would change size; refusing")
        break
    path.write_text(new_text)
    return done


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--apply", action="store_true")
    ap.add_argument("--only", help="one type name")
    args = ap.parse_args()

    sizes = project_sizes(ida_sizes())
    idb = ida_members.load(ROOT / "config" / "smt3" / "ida_members.txt")
    uses = member_uses([p.read_text(errors="replace") for p in sources()])
    found = proposals(sizes, idb, uses)

    total, written = 0, 0
    for name in sorted(found):
        if args.only and name != args.only:
            continue
        fields, picks = found[name]
        where = copies(name, fields)
        print(f"=== {name}   {where[0].relative_to(ROOT).as_posix()}"
              + (f" (+{len(where) - 1} more copies)" if len(where) > 1 else ""))
        for old, pieces in picks:
            print(f"    {old:<18} -> " + "  ".join(d for _, d in pieces))
        total += len(picks)
        if args.apply:
            for path in where:
                written += rewrite(path, name, picks, sizes)
    print(f"\n{total} member(s) splittable in {len(found)} struct(s)")
    if args.apply:
        print(f"{written} definition(s) rewritten; relink and check every changed unit")
    return 0


if __name__ == "__main__":
    sys.exit(main())
