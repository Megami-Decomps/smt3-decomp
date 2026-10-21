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
    """[(name, file, [(member, value)])]."""
    out, cur = [], None
    if not ENUMS.exists():
        return out
    for line in ENUMS.read_text().splitlines():
        if line.startswith("E "):
            p = line.split()
            cur = (p[1], p[2], [])
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


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("query", nargs="?", help="a type name, or part of one")
    ap.add_argument("-m", "--members", metavar="TYPE", help="HD member names for a type")
    ap.add_argument("-v", "--value", help="enum members holding this value")
    args = ap.parse_args()

    types, enums = load_types(), load_enums()
    if args.members:
        return show_members(args.members, types)

    if args.value is not None:
        want = int(args.value, 0)
        found = [(e, m) for e, _, members in enums for m, v in members if v == want]
        print(f"{len(found)} HD enum member(s) with value {want} ({want:#x}):")
        for ename, member in found[:40]:
            print(f"  {member:<34} {ename}")
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
