#!/usr/bin/env python3
"""Propose HD member names for unk_ members, where the gap forces the answer.

Members both sides agree on are anchors. Between two of them, an equal-length run
forces the pairing; an unequal gap is reported, not guessed. `--apply` rewrites the
forced ones, after which the build has to be relinked."""

import argparse
import re
import sys
from difflib import SequenceMatcher
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from cparse import structs  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
TYPES = ROOT / "config" / "smt3" / "hd_types.txt"

UNNAMED = re.compile(r"^(?:unk(?:nown)?(?:[_0-9]|$)|field_0x)", re.I)
# A name the remaster uses for padding carries no meaning worth adopting, so an
# unk_ member stays as it is rather than trading one empty name for another.
WORTHLESS = re.compile(r"^(?:pad+\d*|padd\d*|resv\d*|reserve\d*|reserved\d*|dummy\d*)$", re.I)


IL2CPP = ROOT / "config" / "smt3" / "hd_il2cpp_types.txt"


def load_hd():
    """type -> [member names in declaration order], headers first.

    The remaster's metadata (tools/hd_il2cpp_types.py) carries records the
    reconstruction's headers never declare, in the same order."""
    out = {}
    for path in (TYPES, IL2CPP):
        if not path.exists():
            continue
        cur = None
        for line in path.read_text().splitlines():
            if line.startswith("T "):
                key = line.split()[1]
                cur = None if out.get(key) else key
                if cur:
                    out[cur] = []
            elif line.startswith("M ") and cur:
                out[cur].append(line.split()[-1])
    return {k: v for k, v in out.items() if v}


def project_structs():
    """name -> (members, [paths defining that exact member list], line of the first).

    Copies of a type are not always the same shape: nbCamera_AllTable_t is three
    members in nbCameraCommand.c and two in nbCameraSkill.c, where one `u8 [8]`
    covers the first two. A proposal read off one shape must not be written to the
    other, so only identical copies are collected."""
    out = {}
    files = sorted(list((ROOT / "src").rglob("*.c")) + list((ROOT / "include").rglob("*.h")))
    for path in files:
        for _, name, members, line in structs(path.read_text(errors="replace")):
            if not name:
                continue
            if name not in out:
                out[name] = (members, [path], line)
            elif members == out[name][0] and path not in out[name][1]:
                out[name][1].append(path)
    return out


def propose(ours, theirs):
    """[(index, our name, HD name)] for each forced pairing, plus the ambiguous gaps.

    SequenceMatcher gives the anchors: the members the two lists share, in order."""
    picks, unsure = [], []
    blocks = SequenceMatcher(a=ours, b=theirs, autojunk=False).get_matching_blocks()
    oi = ti = 0
    for ai, bi, size in blocks:
        ours_gap, theirs_gap = list(range(oi, ai)), list(range(ti, bi))
        if ours_gap:
            if len(ours_gap) == len(theirs_gap):
                for o, t in zip(ours_gap, theirs_gap):
                    if UNNAMED.match(ours[o]) and not WORTHLESS.match(theirs[t]):
                        picks.append((o, ours[o], theirs[t]))
            else:
                for o in ours_gap:
                    if UNNAMED.match(ours[o]):
                        unsure.append((ours[o], [theirs[t] for t in theirs_gap]))
        oi, ti = ai + size, bi + size
    return picks, unsure


def span(lines, start):
    """Line range of the definition opening at `start`."""
    depth = 0
    for i in range(start, len(lines)):
        depth += lines[i].count("{") - lines[i].count("}")
        if depth == 0 and i > start:
            return i
    return start


def apply(path, name, renames):
    """Rename a member in every copy of the type this file defines, and in the
    file's uses when no other struct here declares the same member name.

    Where another struct does, the uses are left for the compiler to point at: an
    access through the renamed type stops compiling, and one through the other type
    still does, which attributes each site without guessing at it."""
    text = path.read_text()
    defs = structs(text)
    done, deferred = 0, []
    for old, new in renames:
        shared = any(n != name and old in members for _, n, members, _ in defs)
        lines = text.split("\n")
        for _, sname, _, line in structs(text):
            if sname != name:
                continue
            start = line - 1
            end = span(lines, start)
            body = "\n".join(lines[start:end + 1])
            swapped, n = re.subn(rf"\b{re.escape(old)}\b", new, body)
            if n:
                lines[start:end + 1] = swapped.split("\n")
                done += 1
        text = "\n".join(lines)
        if shared:
            deferred.append(old)
        else:
            # Only `.name` and `->name` are member uses. A bare identifier is not,
            # and rewriting one renames every unrelated use of a common word.
            text = re.sub(rf"(\.|->)(\s*){re.escape(old)}\b", rf"\g<1>\g<2>{new}", text)
    path.write_text(text)
    return done, deferred


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--apply", action="store_true")
    ap.add_argument("--only", help="one type name")
    args = ap.parse_args()

    hd, proj = load_hd(), project_structs()
    total_picks = total_unsure = touched = 0
    pending = []
    for name in sorted(proj):
        if args.only and name != args.only:
            continue
        members, paths, line = proj[name]
        if name not in hd or not any(UNNAMED.match(m) for m in members):
            continue
        picks, unsure = propose(members, hd[name])
        if not picks and not unsure:
            continue
        where = paths[0].relative_to(ROOT).as_posix()
        extra = f" (+{len(paths) - 1} more copies)" if len(paths) > 1 else ""
        print(f"=== {name}   {where}:{line}{extra}")
        for _, old, new in picks:
            print(f"    {old:<22} -> {new}")
        for old, cands in unsure:
            shown = ", ".join(cands[:6]) or "(nothing between the anchors)"
            print(f"    {old:<22} ?  {shown}")
        total_picks += len(picks)
        total_unsure += len(unsure)
        if args.apply and picks:
            for path in paths:
                n, deferred = apply(path, name, [(o, nn) for _, o, nn in picks])
                touched += n
                for old in deferred:
                    pending.append(f"{path.relative_to(ROOT).as_posix()}  {name}.{old}")
    print(f"\n{total_picks} forced, {total_unsure} ambiguous")
    if args.apply:
        print(f"{touched} declaration(s) renamed; relink and verify the checksum")
        if pending:
            print("\nuses left for the compiler to attribute (another struct in the "
                  "same file shares the member name):")
            for row in pending:
                print(f"  {row}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
