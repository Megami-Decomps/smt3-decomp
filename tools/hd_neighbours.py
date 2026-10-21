#!/usr/bin/env python3
"""Name an invented type from the HD header neighbours of the types beside it.

The original headers keep related types together, a param followed by its work type
and its parts, so the types our file already names anchor a small pool. Matching a
layout against four candidates means something; against 1341 it does not."""

import argparse
import re
import sys
from collections import defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import ida_members  # noqa: E402
from cparse import struct_fields  # noqa: E402
from ida_fill import ida_sizes, layout, project_sizes  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
HD_TYPES = ROOT / "config" / "smt3" / "hd_types.txt"

CONVENTION = re.compile(r"_(?:t\d?|u|e|b|s|tag\d?)$")
INVENTED = re.compile(r"^[A-Z][a-z0-9]")
SPAN = 3


def hd_order():
    """name -> (file, index), and (file -> [names in declaration order])."""
    where, per_file = {}, defaultdict(list)
    for line in HD_TYPES.read_text().splitlines():
        if line.startswith("T "):
            parts = line.split()
            if len(parts) >= 6:
                name, src, idx = parts[1], parts[4], int(parts[5])
                where[name] = (src, idx)
                per_file[src].append((idx, name))
    for src in per_file:
        per_file[src].sort()
    return where, per_file


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--span", type=int, default=SPAN)
    ap.add_argument("--apply", action="store_true")
    args = ap.parse_args()

    where, per_file = hd_order()
    idb = ida_members.load(ROOT / "config" / "smt3" / "ida_members.txt")
    sizes = project_sizes(ida_sizes())

    taken = set()
    for path in sorted(list((ROOT / "src").rglob("*.c"))
                       + list((ROOT / "include").rglob("*.h"))):
        for _, name, _, _ in struct_fields(path.read_text(errors="replace")):
            if name:
                taken.add(name)

    forced, several = [], []
    seen = set()
    for path in sorted((ROOT / "src").rglob("*.c")):
        defs = [(n, f, l) for _, n, f, l in
                struct_fields(path.read_text(errors="replace")) if n]
        anchors = [n for n, _, _ in defs if n in where]
        if not anchors:
            continue
        # The pool: everything declared within `span` of an anchor in its own header.
        pool = set()
        for anchor in anchors:
            src, idx = where[anchor]
            for other_idx, other in per_file[src]:
                if abs(other_idx - idx) <= args.span:
                    pool.add(other)
        pool -= taken
        if not pool:
            continue
        for name, fields, line in defs:
            if name in seen or not INVENTED.match(name) or CONVENTION.search(name):
                continue
            seen.add(name)
            rows = layout(fields, sizes)
            if not rows or len({s for _, s, _ in rows}) < 2 or len(rows) < 4:
                continue
            mine = [(o, s) for o, s, _ in rows]
            total = rows[-1][0] + rows[-1][1]
            hits = sorted({c for c in pool if c in idb
                           and [(o, s) for o, s, _, _ in idb[c][2]] == mine
                           and total <= idb[c][1]})
            rel = path.relative_to(ROOT).as_posix()
            if len(hits) == 1:
                forced.append((name, hits[0], rel, line, len(rows)))
            elif hits:
                several.append((name, hits))

    claims = defaultdict(list)
    for name, target, *_ in forced:
        claims[target].append(name)
    contested = {t for t, who in claims.items() if len(who) > 1}
    forced = [row for row in forced if row[1] not in contested]

    print(f"{len(forced)} invented type(s) match one HD neighbour of a type beside "
          f"them:")
    for name, target, rel, line, n in forced:
        print(f"    {name:<30} -> {target:<34} {n:>3} members  {rel}:{line}")
    print(f"\n{len(several)} match several neighbours, {len(contested)} contested")

    if args.apply:
        for name, target, *_ in forced:
            for path in sorted(list((ROOT / "src").rglob("*.c"))
                               + list((ROOT / "include").rglob("*.h"))):
                text = path.read_text()
                swapped, n = re.subn(rf"\b{re.escape(name)}\b", target, text)
                if n:
                    path.write_text(swapped)
            print(f"  {name} -> {target}")
        print(f"\n{len(forced)} renamed; relink and verify the checksum")
    return 0


if __name__ == "__main__":
    sys.exit(main())
