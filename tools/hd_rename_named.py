#!/usr/bin/env python3
"""Propose the remaster's member name wherever ours differs from it.

tools/hd_members.py only touches `unk_` members. But the remaster's names are
authoritative for named members too, and the tree is full of decomp-era readings
that disagree with them. This pairs members by offset and width and prints the
ones to change, as the `type<TAB>old<TAB>new` rows tools/hd_align.py --plan
takes.

Pairing by position does not work, however it is guarded. Equal member counts do
not prove the alignment: `sdfMovieInitParm_t` carries a member the remaster
lacks, so every name after it is one slot out. Requiring a majority of names to
agree already does not work either, and neither does bracketing a disagreeing
run between two agreeing members: a rotation satisfies both, which is how
`scrProcessWork_s` came to offer `CP` -> `Header` *and* `Header` -> `Code`.

Offset and width narrow it, and an agreeing name somewhere in the record anchors
the two readings to each other. That is still not enough to rename a member of
ours that already says something: in a record whose members are all four bytes
wide, a one-slot shift matches at every offset, so `effSwordTrackParams_t` will
happily offer `step` -> `blend` and `blend` -> `abgr`. By default, then, only
placeholders of ours are proposed, where there is no reading to lose. `--all`
prints the rest for a human to read one record at a time; do not pipe it to
--apply.

    python3 tools/hd_rename_named.py > plan.tsv
    python3 tools/hd_align.py --plan plan.tsv --apply --resolve --no-types
"""

import argparse
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from cparse import struct_fields  # noqa: E402
from hd_members import WORTHLESS  # noqa: E402
from ida_fill import ida_sizes, layout, project_sizes  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
HD_TYPES = ROOT / "config" / "smt3" / "hd_types.txt"

# Ours being an offset placeholder is what makes their real name a gain; it is
# not a licence to take a placeholder of theirs.
UNNAMED = re.compile(r"^(?:unk(?:nown)?(?:[_0-9]|[0-9A-Fa-f]+$|$)|field_0x|dummy\d|unused\d)", re.I)
# il2cpp spellings that are not original member names.
ARTIFACT = re.compile(r"^(?:_\d+|m_\w+|klass|monitor|vtable|_vtbl|obj|fields|padding\d*)$")


def hd_layouts():
    """type -> {(offset, size): name}, for the records that carry offsets."""
    out, cur = {}, None
    for line in HD_TYPES.read_text().splitlines():
        if line.startswith("T "):
            cur = line.split()[1]
            out.setdefault(cur, {})
        elif line.startswith("M ") and cur:
            p = line.split(None, 4)
            if p[1].isdigit() and p[2].isdigit():
                out[cur][(int(p[1]), int(p[2]))] = p[-1].split()[-1]
    return {k: v for k, v in out.items() if v}


def our_layouts(sizes):
    """type -> ({(offset, size): name}, [paths]), for one shape per name."""
    out = {}
    for path in sorted(list((ROOT / "src").rglob("*.c")) + list((ROOT / "include").rglob("*.h"))):
        for kind, name, fields, _ in struct_fields(path.read_text(errors="replace")):
            if not name or kind == "union":
                continue
            rows = layout(fields, sizes)
            if rows is None:
                continue
            shape = {(o, s): m for o, s, m in rows}
            if name not in out:
                out[name] = (shape, [path])
            elif out[name][0] == shape and path not in out[name][1]:
                out[name][1].append(path)
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--all", action="store_true",
                    help="also propose for members of ours that already carry a "
                         "real name; these need a reading, not a rule")
    ap.add_argument("--unpaired", action="store_true",
                    help="report the records whose offsets never line up instead")
    ap.add_argument("types", nargs="*", help="only these records")
    args = ap.parse_args()

    hd = hd_layouts()
    mine = our_layouts(project_sizes(ida_sizes()))
    names = args.types or sorted(set(hd) & set(mine))
    rows, unpaired = [], []
    for name in names:
        if name not in hd or name not in mine:
            continue
        shape = mine[name][0]
        paired = [(shape[k], hd[name][k]) for k in shape if k in hd[name]]
        if not paired:
            unpaired.append((name, len(shape), len(hd[name])))
            continue
        # One member whose name and place both agree anchors the two readings to
        # each other. Without it, a record of uniform-width members can be a
        # whole slot out and still match at every offset.
        if not any(old == new for old, new in paired):
            unpaired.append((name, len(shape), len(hd[name])))
            continue
        for old, new in paired:
            if old == new or ARTIFACT.match(new) or WORTHLESS.match(new):
                continue
            # Ours already says something; only the code can say whether their
            # name belongs at this offset or one along. See the module docstring.
            if not args.all and not UNNAMED.match(old):
                continue
            rows.append((name, old, new))

    if args.unpaired:
        for name, a, b in unpaired:
            print(f"{name}\tours {a} members, theirs {b}, no offset in common")
        return
    for name, old, new in rows:
        print(f"{name}\t{old}\t{new}")
    print(f"# {len(rows)} proposals over {len(names) - len(unpaired)} records; "
          f"{len(unpaired)} records share no offset", file=sys.stderr)


if __name__ == "__main__":
    main()
