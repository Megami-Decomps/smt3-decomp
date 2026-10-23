#!/usr/bin/env python3
"""Fold one of our invented records into the record the tree already has for it.

A swarm often reports an invented type as a record the tree declares elsewhere:
`QuadF32` is `sdfVector4_t`. That is not a rename hd_align can take, since the
target name is taken, but it is a merge when both layouts are the same byte for
byte. The layouts are compared first, the members are paired by position, and the
build is what proves the fold changed no code."""

import argparse
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from hd_align import ROOT, all_sources, apply_resolved, fix_single, our_types, struct_spans  # noqa: E402
from ida_fill import WORTHLESS, ida_sizes, layout, project_sizes  # noqa: E402
import re  # noqa: E402


def shapes(name, ours, sizes):
    """Every distinct layout this tree gives one type, as [(offset, size, member)]."""
    out = []
    for fields in ours.get(name, {}).values():
        rows = layout([(ty.replace("*", ""), ty.count("*"), m, dims)
                       for ty, m, dims in fields], sizes)
        if rows:
            out.append(rows)
    return out


def declarations(name, ours):
    """{member: its declaration text} from the target's own definition."""
    out = {}
    for fields in ours.get(name, {}).values():
        for ty, m, dims in fields:
            star = " " + "*" * ty.count("*") if "*" in ty else " "
            out.setdefault(m, f"{ty.replace('*', '')}{star}{m}"
                           + "".join(f"[{d}]" for d in dims) + ";")
    return out


def member_uses():
    """member name -> how many `.name` / `->name` uses the tree has."""
    counts = {}
    for path in all_sources():
        for name in re.findall(r"(?:->|\.)\s*([A-Za-z_]\w*)",
                               path.read_text(errors="replace")):
            counts[name] = counts.get(name, 0) + 1
    return counts


def align(mine, theirs, uses):
    """(renames, splits) pairing our layout against the target's, or a reason it cannot.

    A split replaces one of our members with several of theirs, so it is refused
    for a member anything reads: rewriting those uses is decompilation, not a
    rename."""
    byoff = {o: (s, m) for o, s, m in theirs}
    renames, splits = {}, {}
    for off, size, name in mine:
        if byoff.get(off, (None, None))[0] == size:
            target = byoff[off][1]
            if target != name:
                renames[name] = target
            continue
        inside = [(o, s, m) for o, s, m in theirs if off <= o < off + size]
        if len(inside) < 2 or inside[0][0] != off:
            return None, None, f"our {name} at {off} matches nothing of theirs"
        if sum(s for _, s, _ in inside) != size or inside[-1][0] + inside[-1][1] != off + size:
            return None, None, f"their members do not tile our {name}"
        if any(not m for _, _, m in inside) or all(WORTHLESS.match(m) for _, _, m in inside):
            return None, None, f"their members inside our {name} are all padding"
        if uses.get(name, 0):
            return None, None, f"our {name} covers {len(inside)} of theirs and is read"
        splits[name] = [m for _, _, m in inside]
    return renames, splits, None


def split_in(our_t, splits, decls, ours):
    """Replace each covered member's declaration with the target's own lines."""
    for path in ours[our_t]:
        text = path.read_text()
        lines = text.split("\n")
        for first, last in reversed(struct_spans(lines, our_t)):
            body = lines[first:last + 1]
            for old, pieces in splits.items():
                for i, line in enumerate(body):
                    if re.search(rf"\b{re.escape(old)}\b", line) and line.rstrip().endswith(";"):
                        indent = re.match(r"\s*", line).group(0)
                        body[i:i + 1] = [indent + decls[p] for p in pieces]
                        break
            lines[first:last + 1] = body
        path.write_text("\n".join(lines))


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("plan", type=Path, help="TSV of `ours<TAB>target` rows")
    ap.add_argument("--apply", action="store_true")
    ap.add_argument("--fix", action="store_true")
    args = ap.parse_args()

    ours = our_types()
    sizes = project_sizes(ida_sizes())
    uses = member_uses()
    types, members, splits, refused = {}, {}, {}, []
    for line in args.plan.read_text().splitlines():
        if not line.strip() or line.startswith("#"):
            continue
        a, b = line.split("\t")[:2]
        if a not in ours or b not in ours:
            refused.append(f"{a} -> {b}: one of them is not defined in the tree")
            continue
        mine, theirs = shapes(a, ours, sizes), shapes(b, ours, sizes)
        if not mine or not theirs:
            refused.append(f"{a} -> {b}: a layout could not be computed")
            continue
        if len({tuple((o, s) for o, s, _ in r) for r in mine}) != 1 or \
                len({tuple((o, s) for o, s, _ in r) for r in theirs}) != 1:
            refused.append(f"{a} -> {b}: the tree gives one of them several layouts")
            continue
        # Ours may stop short of theirs: `VuWork` is `corePktCb_t` without its last
        # two words. Every member of ours still has to pair off one for one, which
        # align() checks, so only the tail is left out.
        if mine[0][-1][0] + mine[0][-1][1] > theirs[0][-1][0] + theirs[0][-1][1]:
            refused.append(f"{a} -> {b}: {mine[0][-1][0] + mine[0][-1][1]} bytes "
                           f"against {theirs[0][-1][0] + theirs[0][-1][1]}")
            continue
        pairs, split, why = align(mine[0], theirs[0], uses)
        if why:
            refused.append(f"{a} -> {b}: {why}")
            continue
        clash = [m for m, t in pairs.items() if t in {n for _, _, n in mine[0]} and t != m]
        if clash:
            refused.append(f"{a} -> {b}: {', '.join(clash)} would take a name "
                           f"{a} already has")
            continue
        types[a] = (b, 1)
        if pairs:
            members[a] = pairs
        if split:
            splits[a] = split
    for r in refused:
        print("refused", r)
    for a, (b, _) in sorted(types.items()):
        print(f"merge {a} -> {b}"
              + (f"  ({len(members.get(a, {}))} renamed" if members.get(a) else "  (")
              + (f", {len(splits.get(a, {}))} split)" if splits.get(a) else ")"))
        for old, pieces in splits.get(a, {}).items():
            print(f"    {old} -> " + ", ".join(pieces))
    print(f"{len(types)} merges, {sum(map(len, members.values()))} members renamed, "
          f"{sum(map(len, splits.values()))} split, {len(refused)} refused")
    if args.apply and types:
        for a, split in splits.items():
            split_in(a, split, declarations(types[a][0], ours), ours)
        touched, counts = apply_resolved(types, members, ours)
        print(f"touched {len(touched)} files; {sum(counts.values())} uses placed by type")
        if args.fix and members:
            n, stuck, rc = fix_single(members)
            for s in stuck:
                print("stuck", s)
            print(f"placed {n} more by elimination; {len(stuck)} stuck; ninja exit {rc}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
