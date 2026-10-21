#!/usr/bin/env python3
"""Merge an invented type onto a recovered one the tree already has.

Identical member names in the same order settle it with no IDB or HD corpus needed:
SdfGsDrawDefaultsRegisters carried prmodecont, colclamp, dthe and texa, which is
exactly include/draw.h's sdfGrDrawDefParmPacket_t."""

import argparse
import re
import sys
from collections import defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from cparse import struct_fields  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]

CONVENTION = re.compile(r"_(?:t\d?|u|e|b|s|tag\d?)$")
INVENTED = re.compile(r"^[A-Z][a-z0-9]")
# Member names carrying no information cannot establish that two types are the same.
EMPTY = re.compile(r"^(?:unk\w*|field_\w+|pad+\w*|resv\w*|dummy\w*|reserved?\d*)$", re.I)
MIN_MEMBERS = 3


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--apply", action="store_true")
    ap.add_argument("--rounds", type=int, default=5)
    args = ap.parse_args()
    total = 0
    for _ in range(args.rounds):
        done = one_round(args)
        total += done
        if not done or not args.apply:
            break
    if args.apply:
        print(f"\n{total} merged in total; relink and verify the checksum")
    return 0


def one_round(args):
    """A merge can expose the next: BillRenderPair matches billRenderPair_t only once
    its children member's own type, BillChildPayload, has become billChildPayload_t."""
    invented, recovered = {}, defaultdict(list)
    for path in sorted(list((ROOT / "src").rglob("*.c"))
                       + list((ROOT / "include").rglob("*.h"))):
        for _, name, fields, line in struct_fields(path.read_text(errors="replace")):
            if not name:
                continue
            names = tuple(f[2] for f in fields)
            if len(names) < MIN_MEMBERS or any(EMPTY.match(m) for m in names):
                continue
            # Member types as well as names. PcpScatterPlainInstance and
            # effPCPWhirlwindPoly_tag share every member name and are still different
            # records, their param members being different types.
            members = tuple(
                (re.sub(r"\s+", " ",
                        re.sub(r"\b(struct|union|const)\b", " ", f[0])).strip(),
                 f[1], f[2], tuple(f[3])) for f in fields)
            if INVENTED.match(name) and not CONVENTION.search(name):
                invented.setdefault(name, (members, path, line))
            elif CONVENTION.search(name):
                if name not in [n for n, _ in recovered[members]]:
                    recovered[members].append((name, path))

    forced, several = [], []
    for name, (members, path, line) in sorted(invented.items()):
        hits = sorted({n for n, _ in recovered.get(members, [])})
        if len(hits) == 1:
            # Merging into a name this file already defines would declare it twice.
            if any(n == hits[0] for _, n, _, _ in
                   struct_fields(path.read_text(errors="replace"))):
                continue
            forced.append((name, hits[0], path.relative_to(ROOT).as_posix(), line,
                           len(members)))
        elif hits:
            several.append((name, hits))
    # Two of ours onto one target in one file is the same redefinition.
    seen = defaultdict(list)
    for name, target, rel, _, _ in forced:
        seen[(target, rel)].append(name)
    clash = {k for k, who in seen.items() if len(who) > 1}
    forced = [row for row in forced if (row[1], row[2]) not in clash]

    print(f"{len(forced)} invented type(s) duplicate a recovered one member for member:")
    for name, target, rel, line, n in forced:
        print(f"    {name:<32} -> {target:<32} {n:>3} members  {rel}:{line}")
    print(f"\n{len(several)} match more than one:")
    for name, hits in several[:8]:
        print(f"    {name:<32} ?  {', '.join(hits[:4])}")

    if args.apply:
        for name, target, *_ in forced:
            for path in sorted(list((ROOT / "src").rglob("*.c"))
                               + list((ROOT / "include").rglob("*.h"))):
                text = path.read_text()
                swapped, n = re.subn(rf"\b{re.escape(name)}\b", target, text)
                if n:
                    path.write_text(swapped)
            print(f"  {name} -> {target}")
    return len(forced)


if __name__ == "__main__":
    sys.exit(main())
