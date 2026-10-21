#!/usr/bin/env python3
"""Name an invented type by matching its layout against its own unit's HD file.

Tree-wide a layout says nothing, a struct of four words fitting hundreds of types.
But 280 of our 300 units carry an HD source file's name, and one file mentions only a
few dozen types, so matching exactly one of those is evidence."""

import argparse
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import ida_members  # noqa: E402
from cparse import struct_fields  # noqa: E402
from ida_fill import ida_sizes, layout, project_sizes  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
DEFAULT_HD = Path("/home/wsl/projects/smt3hd-reconstructed-sources")

CONVENTION = re.compile(r"_(?:t\d?|u|e|b|s|tag\d?)$")
INVENTED = re.compile(r"^[A-Z][a-z0-9]")
# A layout of uniform words is shared too widely to identify anything, even in one
# file; require some variety in the extents or a decent length.
MIN_MEMBERS = 4


def distinctive(rows):
    return len(rows) >= MIN_MEMBERS or len({s for _, s, _ in rows}) > 1


def hd_pool(hd):
    """HD file stem -> the type names that file mentions."""
    pool = {}
    for path in sorted((hd / "src").glob("*.c")):
        words = set(re.findall(r"[A-Za-z_]\w*", path.read_text(errors="replace")))
        pool[path.stem] = words
    return pool


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--hd", type=Path, default=DEFAULT_HD)
    ap.add_argument("--apply", action="store_true")
    ap.add_argument("--all", action="store_true")
    args = ap.parse_args()

    sizes = project_sizes(ida_sizes())
    idb = ida_members.load(ROOT / "config" / "smt3" / "ida_members.txt")
    if not idb:
        sys.exit("config/smt3/ida_members.txt missing; run tools/ida_members.py")
    pool = hd_pool(args.hd)

    # Every name the tree already uses, so a rename cannot collide with one.
    taken = set()
    for p in sorted(list((ROOT / "src").rglob("*.c")) + list((ROOT / "include").rglob("*.h"))):
        for _, name, _, _ in struct_fields(p.read_text(errors="replace")):
            if name:
                taken.add(name)

    forced, several, nothing = [], [], 0
    seen = set()
    for path in sorted((ROOT / "src").rglob("*.c")):
        names = pool.get(path.stem)
        if not names:
            continue
        for kind, name, fields, line in struct_fields(path.read_text(errors="replace")):
            if not name or kind == "union" or name in seen:
                continue
            if not INVENTED.match(name) or CONVENTION.search(name):
                continue
            seen.add(name)
            rows = layout(fields, sizes)
            if not rows or not distinctive(rows):
                nothing += 1
                continue
            mine = [(o, s) for o, s, _ in rows]
            total = rows[-1][0] + rows[-1][1]
            hits = []
            for cand in names:
                if cand not in idb or cand in taken:
                    continue
                _, isize, imembers = idb[cand]
                if [(o, s) for o, s, _, _ in imembers] == mine and total <= isize:
                    hits.append(cand)
            hits = sorted(set(hits))
            rel = path.relative_to(ROOT).as_posix()
            if len(hits) == 1:
                forced.append((name, hits[0], rel, line, len(rows)))
            elif hits:
                several.append((name, hits, rel, line))
            else:
                nothing += 1

    # Two of our types claiming one IDB type cannot both be right, and renaming both
    # would declare the same name twice: EffPCPPairedEventRow and
    # EffPCPSpawnRangeEventRow both fit effPCPSpiralBEDPart_tag, in one file.
    claims = {}
    for row in forced:
        claims.setdefault(row[1], []).append(row[0])
    contested = {t for t, who in claims.items() if len(who) > 1}
    if contested:
        print(f"{len(contested)} target(s) claimed by more than one of our types, "
              f"so none of them is forced:")
        for t in sorted(contested):
            print(f"    {t:<34} <- {', '.join(sorted(claims[t]))}")
        print()
    forced = [row for row in forced if row[1] not in contested]

    print(f"{len(forced)} invented type(s) match exactly one type their own unit's HD "
          f"file mentions:")
    for name, hit, rel, line, n in forced:
        print(f"    {name:<30} -> {hit:<34} {n:>3} members  {rel}:{line}")
    print(f"\n{len(several)} match several in their unit:")
    for name, hits, rel, line in (several if args.all else several[:10]):
        print(f"    {name:<30} ?  {', '.join(hits[:4])}")
    print(f"\n{nothing} match none, or are too plain to tell apart")

    if args.apply:
        done = 0
        for name, hit, *_ in forced:
            touched = 0
            for p in sorted(list((ROOT / "src").rglob("*.c"))
                            + list((ROOT / "include").rglob("*.h"))):
                text = p.read_text()
                swapped, n = re.subn(rf"\b{re.escape(name)}\b", hit, text)
                if n:
                    p.write_text(swapped)
                    touched += 1
            print(f"  {name} -> {hit}   {touched} file(s)")
            done += 1
        print(f"\n{done} renamed; relink and verify the checksum")
    return 0


if __name__ == "__main__":
    sys.exit(main())
