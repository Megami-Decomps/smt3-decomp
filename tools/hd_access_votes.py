#!/usr/bin/env python3
"""Pair members by where they are touched in a shared function body.

Every layout rule runs out at the same place: a record whose members are all the
same width admits a one-slot shift that matches at every offset, so nothing about
the *shape* of the two records can say whether their name belongs at our offset
or one along. See tools/hd_rename_named.py.

The bodies can say it. A function the remaster also defines does the same thing
in the same order, so the nth member access in our copy is the nth in theirs.
That order comes from control flow, not from the layout, which is what makes it
independent evidence: a real rename votes the same way in every function that
touches it, and a shift does not.

The alignment is only taken where it is unambiguous. Both bodies must yield the
same number of accesses through the same object, every vote for a member must
agree, and a member must be seen at least twice unless the only function that
touches it touches it once.

    python3 tools/hd_access_votes.py                 # the plan, as a TSV
    python3 tools/hd_access_votes.py --show fclPanelPos_t
"""

import argparse
import re
import sys
from collections import defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hd_resolve  # noqa: E402
from hd_members import load_hd  # noqa: E402
from hd_pairs import definitions  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
HD_SRC = Path("/home/wsl/projects/smt3hd-reconstructed-sources/src")
TYPE_MAP = ROOT / "config" / "smt3" / "hd_type_map.txt"

UNNAMED = re.compile(r"^(?:unk(?:nown)?(?:[_0-9]|[0-9A-Fa-f]+$|$)|field_0x|dummy\d|unused\d)", re.I)
WORTHLESS = re.compile(r"^(?:pad+\d*|resv\d*|reserve\d*|reserved\d*|dummy\d*)$", re.I)
ARTIFACT = re.compile(r"^(?:_\d+|m_\w+|klass|monitor|vtable|_vtbl|obj|fields|padding\d*)$")
# `base->member` or `base.member`, with the base as a bare identifier: a chain
# through several records cannot be attributed to one object.
ACCESS = re.compile(r"\b([A-Za-z_]\w*)\s*(->|\.)\s*([A-Za-z_]\w*)\b")


def type_map():
    """ours -> HD, for the records a previous pass paired."""
    out = {}
    if TYPE_MAP.exists():
        for line in TYPE_MAP.read_text().splitlines():
            if line.startswith("#") or not line.strip():
                continue
            p = line.split("\t")
            if len(p) >= 2:
                out[p[0]] = p[1]
    return out


def accesses(body):
    """[(base, member)] in source order, skipping what cannot be attributed."""
    return [(m.group(1), m.group(3)) for m in ACCESS.finditer(body)]


def sequences(ours, theirs):
    """[(our_member, their_member)] when the two bodies line up one for one.

    Grouped by base so that a function touching two records still pairs each,
    and taken only when both sides touch that base the same number of times.
    """
    mine, hd = defaultdict(list), defaultdict(list)
    for base, member in accesses(ours):
        mine[base].append(member)
    for base, member in accesses(theirs):
        hd[base].append(member)
    out = []
    for base, ms in mine.items():
        hs = hd.get(base)
        if hs is None or len(hs) != len(ms):
            continue
        out.extend(zip(ms, hs))
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--show", metavar="TYPE", help="print the votes for one record")
    ap.add_argument("--min-votes", type=int, default=2)
    args = ap.parse_args()

    if not HD_SRC.exists():
        sys.exit(f"the reconstruction is not at {HD_SRC}")

    ours = {}
    for path in (ROOT / "src").rglob("*.c"):
        ours.update(definitions(path))
    theirs = {}
    for path in HD_SRC.rglob("*.c"):
        theirs.update(definitions(path))
    shared = sorted(set(ours) & set(theirs))

    members_of = hd_resolve.struct_members(ROOT)
    funcs = hd_resolve.function_types(ROOT)
    tmap = type_map()
    hd_records = load_hd()

    def theirs_has(owner, member):
        """Is `member` a member of the remaster's copy of our `owner`?

        A vote is only as good as the base on their side being the same object as
        ours, which a shared variable name does not establish: `itfColor_t` voted
        `components` -> `pm` from a body where their `pm` belongs to something
        else entirely. Requiring the name to be a real member of their record is
        what ties the two sides together.
        """
        for key in (owner, tmap.get(owner)):
            if key and member in hd_records.get(key, ()):
                return True
        return False

    # owner -> our member -> their member -> how many functions said so
    votes = defaultdict(lambda: defaultdict(lambda: defaultdict(int)))
    for fn in shared:
        body = ours[fn]
        scope = hd_resolve.scopes(body)
        names = hd_resolve.file_scope(body, scope)
        for mine, hd in sequences(body, theirs[fn]):
            if mine == hd or ARTIFACT.match(hd) or WORTHLESS.match(hd):
                continue
            for pos, end, owner in hd_resolve.accesses(
                    body, mine, scope, names, members_of, funcs):
                if owner and owner != hd_resolve.UNKNOWN:
                    votes[owner][mine][hd] += 1
                break

    if args.show:
        for mine, cand in sorted(votes.get(args.show, {}).items()):
            print(f"{mine}: " + ", ".join(f"{h} x{n}" for h, n in
                                          sorted(cand.items(), key=lambda kv: -kv[1])))
        return

    rows, split, shared_target, swapped, unknown_target = [], [], [], [], []
    for owner, ms in sorted(votes.items()):
        picked = {}
        for mine, cand in sorted(ms.items()):
            if len(cand) > 1:
                split.append((owner, mine, dict(cand)))
                continue
            hd, n = next(iter(cand.items()))
            if n < args.min_votes and not UNNAMED.match(mine):
                continue
            if not theirs_has(owner, hd):
                unknown_target.append((owner, mine, hd))
                continue
            picked.setdefault(hd, []).append((mine, n))
        # A proposal whose target is also a member of ours in this record is a
        # swap or a rotation, not a rename: `sdfGrPacketHeader_t` offered
        # dmatag -> giftag and giftag -> dmatag because the two bodies touch the
        # union's arms in opposite order.
        own = set(ms)
        cycles = [(hd, c) for hd, c in picked.items() if hd in own]
        for hd, claims in cycles:
            swapped.append((owner, [c[0] for c in claims], hd))
            del picked[hd]
        for hd, claims in picked.items():
            # Several of ours voting for one of theirs means the remaster has an
            # array where we have scalars (`fclPanel_t`'s four words are their
            # `TexUV[4]`). That is a structural reading, not a rename.
            if len(claims) > 1:
                shared_target.append((owner, hd, [c[0] for c in claims]))
                continue
            rows.append((owner, claims[0][0], hd, claims[0][1]))

    for owner, mine, hd, n in rows:
        print(f"{owner}\t{mine}\t{hd}")
    print(f"# {len(rows)} proposals over {len({r[0] for r in rows})} records; "
          f"{len(split)} members had disagreeing votes, "
          f"{len(shared_target)} of their names were claimed by several of "
          f"ours, {len(swapped)} named a member of ours, "
          f"{len(unknown_target)} named nothing in their copy of the record",
          file=sys.stderr)
    for owner, claims, hd in swapped:
        print(f"# swap: {owner}.{{{', '.join(claims)}}} vote {hd}, which is ours too",
              file=sys.stderr)
    for owner, hd, claims in shared_target:
        print(f"# array: {owner}.{{{', '.join(claims)}}} all vote {hd}",
              file=sys.stderr)


if __name__ == "__main__":
    main()
