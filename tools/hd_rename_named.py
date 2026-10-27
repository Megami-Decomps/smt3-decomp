#!/usr/bin/env python3
"""Propose the remaster's member name wherever ours differs from it.

tools/hd_members.py only touches `unk_` members. But the remaster's names are
authoritative for named members too, and the tree is full of decomp-era readings
that disagree with them. For every record of ours whose name is an HD name, this
pairs members by declaration order and prints the ones to change, as the
`type<TAB>old<TAB>new` rows tools/hd_align.py --plan takes.

A proposal is only made where the alignment is proven, not merely possible.
Equal member counts are not enough, and neither is a majority of names already
agreeing: `sdfMovieInitParm_t` carries a member the remaster lacks, so every
name after it is one slot out and positional pairing proposed `rgbMode` for our
`resourceWord`. The test is bracketing. A run of disagreeing names that has an
agreeing member on both sides maps onto a run of theirs of the same length, so
each pairing in it is forced. A run that reaches either end of the record has no
such witness and is withheld, since an extra or missing member shifts it. Those,
and the records whose member counts differ, go to tools/type_match.py --serve.

    python3 tools/hd_rename_named.py > plan.tsv
    python3 tools/hd_align.py --plan plan.tsv --apply --resolve --no-types
"""

import argparse
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from hd_members import WORTHLESS, load_hd, project_structs  # noqa: E402

# Ours being an offset placeholder is what makes their real name a gain; it is
# not a licence to take a placeholder of theirs.
UNNAMED = re.compile(r"^(?:unk(?:nown)?(?:[_0-9]|[0-9A-Fa-f]+$|$)|field_0x|dummy\d|unused\d)", re.I)
# il2cpp spellings that are not original member names.
ARTIFACT = re.compile(r"^(?:_\d+|m_\w+|klass|monitor|vtable|_vtbl|obj|fields|padding\d*)$")


def bracketed(ours, theirs):
    """Indices whose pairing is forced by an agreeing member on either side."""
    n, out, i = len(ours), set(), 0
    while i < n:
        if ours[i] == theirs[i]:
            i += 1
            continue
        j = i
        while j < n and ours[j] != theirs[j]:
            j += 1
        if i > 0 and j < n:
            out.update(range(i, j))
        i = j
    return out


def proposals(ours, theirs):
    """[(old, new)] for the members whose names disagree, or None if unusable."""
    if len(ours) != len(theirs):
        return None
    safe = bracketed(ours, theirs)
    out = []
    for k, (mine, hd) in enumerate(zip(ours, theirs)):
        if k not in safe:
            continue
        if mine == hd or ARTIFACT.match(hd):
            continue
        # A name of theirs that says nothing is never worth taking, whatever
        # ours is: `pad` for our `alphaState` loses information, and `padd` for
        # our `unk10` only trades one empty name for another while making the
        # drift count look better than the tree is.
        if WORTHLESS.match(hd):
            continue
        out.append((mine, hd))
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--skipped", action="store_true",
                    help="list the records whose member counts differ instead")
    ap.add_argument("types", nargs="*", help="only these records")
    args = ap.parse_args()

    hd = load_hd()
    mine = project_structs()
    names = args.types or sorted(set(mine) & set(hd))
    skipped, rows = [], []
    for name in names:
        if name not in hd or name not in mine:
            continue
        ours = list(mine[name][0])
        picks = proposals(ours, hd[name])
        if picks is None:
            skipped.append((name, len(ours), len(hd[name])))
            continue
        rows.extend((name, old, new) for old, new in picks)

    if args.skipped:
        for name, a, b in skipped:
            print(f"{name}\tours {a} members, theirs {b}")
        return
    for name, old, new in rows:
        print(f"{name}\t{old}\t{new}")
    print(f"# {len(rows)} proposals over {len(names) - len(skipped)} records; "
          f"{len(skipped)} records skipped on member count", file=sys.stderr)


if __name__ == "__main__":
    main()
