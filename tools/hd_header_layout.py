#!/usr/bin/env python3
"""Match an invented record against the records of one HD header, by layout.

Every record in `libsdf.h` exists on the PS2, so an invented `Sdf*` packet is one
of them even where no signature says which: HD passes them as `void *`. Scoping
the layout match to a single header is what makes it decisive."""

import argparse
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from cparse import struct_fields  # noqa: E402
from ida_fill import ida_sizes, layout, project_sizes  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
CONVENTION = re.compile(r"_(?:t\d?|u|e|b|s|tag\d?)$")
INVENTED = re.compile(r"^[A-Z][a-z0-9]")

# The remaster holds a DMA or GIF tag by pointer where the hardware needs it
# inline, so our record is theirs plus 8 bytes per tag.
TAG = re.compile(r"(?:sdfDmaTag_t|sdfGifTag_t|sdfVifCode_t)")


def hd_header_records(header):
    """name -> (size, [(offset, size, type, member)]) for one HD header."""
    out, cur = {}, None
    for line in (ROOT / "config" / "smt3" / "hd_types.txt").read_text().splitlines():
        if line.startswith("T "):
            p = line.split()
            cur = p[1] if len(p) > 4 and p[4] == header else None
            if cur:
                out[cur] = (int(p[3]) if p[3].isdigit() else -1, [])
        elif line.startswith("M ") and cur:
            p = line.split(None, 4)
            out[cur][1].append((int(p[1]) if p[1].isdigit() else -1,
                                int(p[2]) if p[2].isdigit() else -1,
                                p[3], p[4].strip() if len(p) > 4 else ""))
    return {k: v for k, v in out.items() if v[1]}


def our_records(only):
    out = {}
    for path in sorted(list((ROOT / "include").rglob("*.h")) + list((ROOT / "src").rglob("*.c"))):
        if only and not re.search(only, path.as_posix()):
            continue
        for _, name, fields, line in struct_fields(path.read_text(errors="replace")):
            if name and INVENTED.match(name) and not CONVENTION.search(name) and name not in out:
                out[name] = (path, line, fields)
    return out


def score(rows, fields, hd_size, hd_members):
    """How well our computed layout fits an HD record, or None if it cannot.

    The comparison is on the offsets both sides agree on, since HD pointerizes a
    tag our record holds inline and appends its own fields at the end."""
    if not rows:
        return None
    ours_size = rows[-1][0] + max(rows[-1][1], 0)
    tags = sum(1 for ty, stars, _, _ in fields if TAG.search(ty) and not stars)
    if hd_size > 0 and not (hd_size <= ours_size <= hd_size + 8 * max(tags, 1) + 8):
        return None
    byoff = {o: (s, ty, m) for o, s, ty, m in hd_members}
    agree = sum(1 for off, ext, _ in rows if off in byoff and byoff[off][0] == ext)
    return agree if agree >= max(2, len(rows) // 2) else None


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--header", default="libsdf.h")
    ap.add_argument("--only", help="regex on our file path")
    ap.add_argument("--out", type=Path, help="write a type plan TSV of the forced matches")
    args = ap.parse_args()

    hdr = hd_header_records(args.header)
    sizes = project_sizes(ida_sizes())
    ours = our_records(args.only)
    print(f"{len(hdr)} records in {args.header}, {len(ours)} invented records to place")

    claimed, rows_out = {}, []
    for name, (path, line, fields) in sorted(ours.items()):
        rows = layout(fields, sizes)
        hits = []
        for hd_name, (hd_size, hd_members) in hdr.items():
            s = score(rows, fields, hd_size, hd_members)
            if s is not None:
                hits.append((s, hd_name, hd_size))
        hits.sort(reverse=True)
        if not hits:
            continue
        ours_size = rows[-1][0] + max(rows[-1][1], 0) if rows else -1
        best = hits[0]
        forced = len(hits) == 1 or best[0] >= hits[1][0] + 2
        mark = "->" if forced else "?="
        print(f"  {name} ({ours_size} bytes, {path.name}:{line}) {mark} "
              + ", ".join(f"{h[1]}[{h[2]}, {h[0]} offsets]" for h in hits[:4]))
        if forced:
            claimed.setdefault(best[1], []).append(name)
            rows_out.append((name, best[1]))
    if args.out:
        keep = [(a, b) for a, b in rows_out if len(claimed[b]) == 1]
        args.out.write_text("".join(f"{a}\t{b}\n" for a, b in keep))
        print(f"{len(keep)} forced and unclaimed -> {args.out}")


if __name__ == "__main__":
    main()
