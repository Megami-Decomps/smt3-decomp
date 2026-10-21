#!/usr/bin/env python3
"""Look up what SMT3HD and the IDB call something, before inventing a name.

A bare name searches the types; -m TYPE lists that type's HD members; -v VALUE
finds enum members holding a value. Member names and order are the corpus's
authority, its sizes advisory: see tools/hd_types.py."""

import argparse
import re
import sys
from difflib import get_close_matches
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
TYPES = ROOT / "config" / "smt3" / "hd_types.txt"
ENUMS = ROOT / "config" / "smt3" / "hd_enums.txt"
IDA = ROOT / "config" / "smt3" / "ida_types.txt"

SUFFIX = re.compile(r"(_t\d?|_tag\d?|_s|_u|_e|_b)$")


def stem(name):
    return SUFFIX.sub("", name).lower()


def load_types():
    """name -> (kind, size, file, [(offset, size, type, member)])."""
    out, cur = {}, None
    if not TYPES.exists():
        return out
    for line in TYPES.read_text().splitlines():
        if line.startswith("T "):
            p = line.split()
            cur = p[1]
            out[cur] = (p[2], p[3], p[4], [])
        elif line.startswith("M ") and cur:
            p = line.split(None, 4)
            out[cur][3].append((p[1], p[2], p[3], p[4] if len(p) > 4 else ""))
    return out


def load_enums():
    """[(name, file, [(member, value)], merged)]."""
    out, cur = [], None
    if not ENUMS.exists():
        return out
    for line in ENUMS.read_text().splitlines():
        if line.startswith("E "):
            p = line.split()
            cur = (p[1], p[2], [], len(p) > 4 and p[4] == "merged")
            out.append(cur)
        elif line.startswith("V ") and cur:
            p = line.split(None, 2)
            cur[2].append((p[2], int(p[1])))
    return out


def load_ida():
    if not IDA.exists():
        return []
    return [line.split()[0] for line in IDA.read_text().splitlines()
            if line.strip() and not line.startswith("#")]


def show_members(name, types):
    hit = name if name in types else None
    if not hit:
        want = stem(name)
        for cand in types:
            if stem(cand) == want:
                hit = cand
                break
    if not hit:
        print(f"no HD type named {name}")
        return 1
    kind, size, src, rows = types[hit]
    print(f"{kind} {hit}   {len(rows)} members, HD size {size} (advisory)   {src}")
    for off, sz, text, member in rows:
        print(f"  {off:>5} {sz:>5}  {text:<28} {member}")
    return 0


def enum_gaps(enums):
    """HD enums the tree uses some members of but not the rest.

    A partly adopted enum is the strong case: its members already appear in that
    code, so the bare literals left beside them mean the same thing and the enum is
    known to apply. An enum with no member in the tree is a different job, needing
    a reason to believe it belongs wherever it would go."""
    # Adopted means the tree declares the name as a constant, not merely that the
    # word appears: half these HD members are bare words (ALL, IT, ATTACK, INIT)
    # that match an unrelated identifier and read as adoption when they are not.
    declared, used = set(), set()
    for path in list((ROOT / "src").rglob("*.c")) + list((ROOT / "include").rglob("*.h")):
        text = path.read_text(errors="replace")
        declared |= set(re.findall(r"^\s*#\s*define\s+(\w+)", text, re.M))
        for body in re.findall(r"\benum\b[^{;]*\{(.*?)\}", text, re.S):
            declared |= {m for m in re.findall(r"[A-Za-z_]\w*", body)}
        used |= set(re.findall(r"[A-Za-z_]\w*", text))

    def adopted(enum, member):
        """The reconstruction strips the enum's prefix off its members, so the tree
        re-adds it: NB_PHASE's `NON` is declared here as NB_PHASE_NON, because a bare
        NON would collide. Either spelling counts."""
        return member in declared or (enum and f"{enum}_{member}" in declared)

    rows = []
    for name, src, members, bad in enums:
        names = [m for m, _ in members]
        have = [m for m in names if adopted(name, m)]
        missing = [m for m in names if not adopted(name, m)]
        clash = [m for m in missing if m in used]
        rows.append((len(have), len(names), len(clash), name, src, bad, missing))
    full = sum(1 for r in rows if r[0] == r[1])
    partial = [r for r in rows if 0 < r[0] < r[1]]
    absent = [r for r in rows if r[0] == 0]
    print(f"{len(enums)} HD enums: {full} fully declared here, {len(partial)} in part, "
          f"{len(absent)} not at all")
    print("\npartly declared, widest gap first:")
    for have, total, clash, name, src, bad, missing in sorted(partial,
                                                              key=lambda r: r[0] - r[1]):
        mark = "  (merged enum)" if bad else ""
        risk = f", {clash} name(s) already taken by something else" if clash else ""
        print(f"  {have:>3}/{total:<4} {name:<26} {src}{mark}{risk}")
        print(f"        missing: {', '.join(missing[:10])}"
              + (" ..." if len(missing) > 10 else ""))
    risky = [r for r in absent if r[2]]
    print(f"\nof the {len(absent)} undeclared, {len(risky)} would collide with names "
          f"the tree already uses for something else:")
    for have, total, clash, name, src, bad, _ in sorted(risky, key=lambda r: -r[2])[:10]:
        print(f"  {name:<26} {total:>3} members, {clash} already taken   {src}")
    return 0


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("query", nargs="?", help="a type name, or part of one")
    ap.add_argument("-m", "--members", metavar="TYPE", help="HD member names for a type")
    ap.add_argument("-v", "--value", help="enum members holding this value")
    ap.add_argument("--gaps", action="store_true",
                    help="HD enums the tree adopted only in part")
    args = ap.parse_args()

    if args.gaps:
        return enum_gaps(load_enums())

    types, enums = load_types(), load_enums()
    if args.members:
        return show_members(args.members, types)

    if args.value is not None:
        want = int(args.value, 0)
        found = [(e, m, bad) for e, _, members, bad in enums for m, v in members if v == want]
        clean = [(e, m) for e, m, bad in found if not bad]
        print(f"{len(found)} HD enum member(s) with value {want} ({want:#x}), "
              f"{len(found) - len(clean)} from a merged enum:")
        for ename, member in clean[:40]:
            print(f"  {member:<34} {ename}")
        for ename, member, bad in found[:40]:
            if bad:
                print(f"  {member:<34} {ename}  (merged enum: unreliable)")
        print("\nA value alone rarely settles a constant; confirm from how the code "
              "uses it.")
        return 0

    if not args.query:
        ap.error("give a name, -m TYPE or -v VALUE")

    q = args.query
    names = list(types)
    print(f"HD corpus: {len(names)} types, {len(enums)} enums")
    if q in types:
        print(f"\nexact HD type: {q}")
        show_members(q, types)
    want = stem(q)
    same = [n for n in names if stem(n) == want and n != q]
    if same:
        print(f"\nsame stem, different suffix: {', '.join(same)}")
    sub = [n for n in names if q.lower() in n.lower() and n != q][:20]
    if sub:
        print(f"\nHD types containing {q!r}:")
        for n in sub:
            print(f"  {n:<40} {types[n][2]}")
    close = [n for n in get_close_matches(q, names, n=8, cutoff=0.7) if n not in sub]
    if close:
        print(f"\nclose HD names: {', '.join(close)}")

    ida = load_ida()
    ihit = [n for n in ida if q.lower() in n.lower()][:12]
    if ihit:
        print(f"\nIDB types containing {q!r}:")
        for n in ihit:
            print(f"  {n}")
    if not (same or sub or close or ihit) and q not in types:
        print(f"\nnothing in the HD corpus or the IDB resembles {q!r}.")
        print("A type with no counterpart still takes the convention: "
              "lowercase module prefix and a _t suffix.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
