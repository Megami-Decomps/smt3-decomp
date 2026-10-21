#!/usr/bin/env python3
"""Leads for an invented type name, from IDB types of the same layout.

Leads, not answers, and the tool deliberately cannot apply them. A shape is not an
identity: of 258 invented types, 165 have no IDB layout of that shape, 84 match
several, and every unique match read as a coincidence between unrelated things, like
MdlObjectAttachment onto fclribbon_elem_t. Confirm from how the code uses the type."""

import argparse
import re
import sys
from collections import defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import ida_members  # noqa: E402
from cparse import struct_fields  # noqa: E402
from ida_fill import ida_sizes, layout  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]

CONVENTION = re.compile(r"_(?:t\d?|u|e|b|s|tag\d?)$")
INVENTED = re.compile(r"^[A-Z][a-z0-9]")
EXTERNAL = {"FILE", "_Bigint", "_glue", "_atexit", "__sbuf", "__siov", "__suio",
            "sceGsDispEnv", "sceSifDmaData", "sceDmaTag", "sceGifTag"}

# A layout of only 4-byte words is shared by hundreds of types, so a match on one
# says nothing. Require either some variety in the extents or a decent length.
def distinctive(rows):
    sizes = {size for _, size, _ in rows}
    return len(rows) >= 6 or len(sizes) > 1


def module_of(name):
    """The module prefix a type name carries, lowercased: SdfPool -> sdf."""
    m = re.match(r"^([A-Z][a-z0-9]*|[a-z0-9]+)", name)
    if not m:
        return ""
    head = m.group(1).lower()
    return re.sub(r"\d+$", "", head)


def idb_index(idb):
    """signature -> [names], the signature being size and each member's place."""
    index = defaultdict(list)
    for name, (kind, size, members) in idb.items():
        if kind != "struct":
            continue
        sig = (size, tuple((off, msize) for off, msize, _, _ in members))
        index[sig].append(name)
    return index


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--all", action="store_true", help="list the ambiguous ones too")
    args = ap.parse_args()

    sizes = ida_sizes()
    idb = ida_members.load(ROOT / "config/smt3/ida_members.txt")
    if not idb:
        sys.exit("config/smt3/ida_members.txt missing; run tools/ida_members.py")
    index = idb_index(idb)

    files = sorted(list((ROOT / "src").rglob("*.c")) + list((ROOT / "include").rglob("*.h")))
    forced, several, nothing = [], [], 0
    seen = set()
    for path in files:
        text = path.read_text(errors="replace")
        for kind, name, fields, line in struct_fields(text):
            if not name or name in EXTERNAL or name in seen:
                continue
            if not INVENTED.match(name) or CONVENTION.search(name):
                continue
            seen.add(name)
            if kind != "struct":
                continue
            rows = layout(fields, sizes)
            if rows is None or not rows:
                nothing += 1
                continue
            total = rows[-1][0] + rows[-1][1]
            rel = path.relative_to(ROOT).as_posix()
            hits = []
            for pad in (0, 4, 8, 12):  # trailing alignment our source may not spell
                sig = (total + pad, tuple((off, size) for off, size, _ in rows))
                hits += index.get(sig, [])
            hits = sorted(set(hits))
            # A layout match across modules is a coincidence, not a recovery:
            # MdlObjectAttachment and fclribbon_elem_t share a shape and nothing else.
            same_module = [h for h in hits if module_of(h) == module_of(name)]
            if not hits:
                nothing += 1
            elif len(same_module) == 1 and distinctive(rows):
                forced.append((name, same_module[0], rel, line, len(rows)))
            else:
                several.append((name, hits, rel, line, len(rows)))

    print(f"{len(forced)} invented type(s) match exactly one IDB layout of the same "
          f"module. Each is a LEAD: read the code before believing it.")
    for name, hit, rel, line, n in forced:
        print(f"    {name:<30} ?= {hit:<34} {n:>3} members  {rel}:{line}")
    print(f"\n{len(several)} match several, or are too plain to tell apart:")
    for name, hits, rel, line, n in (several if args.all else several[:12]):
        print(f"    {name:<30} ?  {', '.join(hits[:4])}"
              + (" ..." if len(hits) > 4 else ""))
    print(f"\n{nothing} have no IDB layout of that shape at all")
    return 0


if __name__ == "__main__":
    sys.exit(main())
