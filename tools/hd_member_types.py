#!/usr/bin/env python3
"""Name an invented type from the member slot it occupies in a type already named.

Where our struct has an IDB counterpart aligning member for member, the IDB's type
for a slot is the type ours holds there. It propagates, so the tool iterates until
nothing further is settled."""

import argparse
import re
import sys
from collections import defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import ida_members  # noqa: E402
from cparse import struct_fields  # noqa: E402
from ida_fill import ida_sizes, layout, project_sizes  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]

CONVENTION = re.compile(r"_(?:t\d?|u|e|b|s|tag\d?)$")
INVENTED = re.compile(r"^[A-Z][a-z0-9]")
BASES = {
    "void", "s8", "u8", "s16", "u16", "s32", "u32", "s64", "u64", "f32", "f64",
    "bool", "char", "int", "short", "long", "float", "double", "unsigned", "signed",
    "const", "struct", "union", "enum",
}


def bare(text):
    text = re.sub(r"\b(struct|union|const|volatile)\b", " ", text)
    return re.sub(r"\s+", " ", text).strip()


def idb_type(text):
    """The named type an IDB member row carries, if it has one.

    Strip pointers and array extents only. Stripping digits as well turned
    P2A_ANIMATION_tag into a PA_ANIMATION_tag that does not exist."""
    name = re.sub(r"\[[^\]]*\]", "", text).replace("*", "").strip()
    return name if name and name not in BASES and CONVENTION.search(name) else None


def collect():
    """name -> (fields, [paths]) for every struct the tree defines."""
    out = {}
    for path in sorted(list((ROOT / "src").rglob("*.c"))
                       + list((ROOT / "include").rglob("*.h"))):
        for _, name, fields, _ in struct_fields(path.read_text(errors="replace")):
            if not name:
                continue
            if name not in out:
                out[name] = (fields, [path])
            elif path not in out[name][1]:
                out[name][1].append(path)
    return out


def propose(ours, idb, sizes, taken):
    """invented type -> {candidate: {"holder.member"}} from aligned member slots."""
    pairs = defaultdict(lambda: defaultdict(set))
    for name, (fields, _) in ours.items():
        if name not in idb:
            continue
        _, _, imembers = idb[name]
        rows = layout(fields, sizes)
        # Align by index, which is only sound when the two agree member for member.
        if rows is None or len(rows) != len(imembers):
            continue
        for (text, stars, member, _), irow in zip(fields, imembers):
            mine = bare(text)
            if not INVENTED.match(mine) or CONVENTION.search(mine):
                continue
            theirs = idb_type(irow[2])
            if not theirs or theirs in taken:
                continue
            # Our pointer against their inline array is the same type; a real depth
            # gap is not, since `SdfDrawNode **` equates SdfDrawNode*, not SdfDrawNode.
            theirs_stars = irow[2].count("*")
            inline_array = "[" in irow[2] and theirs_stars == 0
            if stars != theirs_stars and not (stars == 1 and inline_array):
                continue
            pairs[mine][theirs].add(f"{name}.{member}")
    return pairs


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--apply", action="store_true")
    ap.add_argument("--rounds", type=int, default=4)
    args = ap.parse_args()

    idb = ida_members.load(ROOT / "config" / "smt3" / "ida_members.txt")
    if not idb:
        sys.exit("config/smt3/ida_members.txt missing; run tools/ida_members.py")

    total = 0
    for round_no in range(1, args.rounds + 1):
        sizes = project_sizes(ida_sizes())
        ours = collect()
        taken = set(ours)
        pairs = propose(ours, idb, sizes, taken)

        forced, split = [], []
        for name, cands in sorted(pairs.items()):
            if len(cands) == 1:
                target, where = next(iter(cands.items()))
                forced.append((name, target, sorted(where)))
            else:
                split.append((name, cands))
        claims = defaultdict(list)
        for name, target, _ in forced:
            claims[target].append(name)
        contested = {t for t, who in claims.items() if len(who) > 1}
        forced = [row for row in forced if row[1] not in contested]

        print(f"round {round_no}: {len(forced)} settled by their slot, "
              f"{len(split)} conflicting, {len(contested)} contested")
        for name, target, where in forced:
            print(f"    {name:<30} -> {target:<34} {', '.join(where[:2])}")
        if not forced:
            break
        total += len(forced)
        if not args.apply:
            break
        for name, target, _ in forced:
            for path in sorted(list((ROOT / "src").rglob("*.c"))
                               + list((ROOT / "include").rglob("*.h"))):
                text = path.read_text()
                swapped, n = re.subn(rf"\b{re.escape(name)}\b", target, text)
                if n:
                    path.write_text(swapped)
    print(f"\n{total} type(s) named from member slots"
          + ("; relink and verify the checksum" if args.apply else ""))
    return 0


if __name__ == "__main__":
    sys.exit(main())
