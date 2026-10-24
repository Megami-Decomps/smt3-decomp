#!/usr/bin/env python3
"""Name an invented type from the IDB's own type at the same place.

The IDB carries the HD symbol recovery with real PS2 layouts, so wherever one of
our invented types stands in a place the IDB types too, that type is the answer:
a parameter slot of a function whose arity agrees, a return type, or a member at
the same offset and extent of a record the IDB has. Votes are written out as a
report for tools/hd_align.py, which applies its own guards, and as a merge plan
for tools/hd_merge.py where the name is already in the tree."""

import argparse
import json
import re
import sys
from collections import Counter, defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import ida_members  # noqa: E402
from cparse import struct_fields  # noqa: E402
from hd_align import our_types  # noqa: E402
from hd_positions import SIG, split_params  # noqa: E402
from ida_fill import ida_sizes, layout, project_sizes  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
# A slot the IDB could not type says nothing.
USELESS = {"void", "int", "unsigned", "signed", "char", "_DWORD", "_BYTE", "_WORD",
           "_QWORD", "__int64", "__int32", "__int16", "__int8", "float", "double",
           "bool", "u8", "s8", "u16", "s16", "u32", "s32", "u64", "s64", "f32"}


def invented():
    return {l.split()[1] for l in
            (ROOT / "config" / "smt3" / "drift_baseline.txt").read_text().splitlines()
            if l.startswith("type ")}


def protos():
    out = {}
    for line in (ROOT / "config" / "smt3" / "ida_protos.txt").read_text().splitlines():
        parts = line.split(None, 2)
        if len(parts) == 3 and not line.startswith("#"):
            out[parts[1]] = parts[2]
    return out


def head(text):
    """The type name a declarator starts with, or None."""
    words = [w for w in re.findall(r"[A-Za-z_]\w*", text)
             if w not in ("struct", "union", "const", "volatile", "enum", "register")]
    return words[0] if words else None


def collect(base=None):
    """(votes, why): {our type: Counter(IDB type)} and one evidence line each."""
    base, proto = base or invented(), protos()
    ours = our_types()
    idb = ida_members.load(ROOT / "config" / "smt3" / "ida_members.txt")
    sizes = project_sizes(ida_sizes())
    votes = defaultdict(Counter)
    why = {}

    def vote(t, target, evidence):
        target = re.sub(r"[\*\[].*$", "", target).strip()
        target = re.sub(r"\b(struct|union|const|enum)\b", "", target).strip()
        if t in base and target and target != t and target not in USELESS:
            votes[t][target] += 1
            why.setdefault((t, target), evidence)

    for src in sorted((ROOT / "src").rglob("*.c")):
        text = src.read_text(errors="replace")
        for m in SIG.finditer(text):
            fn = m.group(2)
            if fn not in proto:
                continue
            theirs = re.search(r"\((.*)\)", proto[fn])
            if not theirs:
                continue
            mine, yours = split_params(m.group(3)), split_params(theirs.group(1))
            if len(mine) == len(yours) and mine:
                for a, b in zip(mine, yours):
                    ta, tb = head(a), head(b)
                    if ta and tb:
                        vote(ta, tb, f"{fn}: parameter typed {tb} by the IDB")
            rt, their_rt = head(m.group(1)), head(proto[fn])
            if rt and their_rt:
                vote(rt, their_rt, f"{fn}: returns {their_rt} in the IDB")

    for path in sorted(list((ROOT / "src").rglob("*.c")) + list((ROOT / "include").rglob("*.h"))):
        for kind, name, fields, _ in struct_fields(path.read_text(errors="replace")):
            if not name or name not in idb:
                continue
            rows = layout(fields, sizes)
            if not rows:
                continue
            by_offset = {o: (s, t) for o, s, t, _ in idb[name][2]}
            for (off, size, _), (ty, _, _, _) in zip(rows, fields):
                ours_t = head(ty)
                if not ours_t or off not in by_offset or by_offset[off][0] != size:
                    continue
                vote(ours_t, by_offset[off][1],
                     f"{name}: the member at 0x{off:x} is {by_offset[off][1]} in the IDB")
    return votes, why


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--min-votes", type=int, default=2)
    ap.add_argument("--report", type=Path, help="write a report for hd_align.py")
    ap.add_argument("--merge-plan", type=Path, help="write a plan for hd_merge.py")
    args = ap.parse_args()

    ours = our_types()
    votes, why = collect()
    renames, merges = [], []
    for t, cands in sorted(votes.items()):
        ranked = cands.most_common()
        best, n = ranked[0]
        runner = ranked[1][1] if len(ranked) > 1 else 0
        if n < args.min_votes or n < 2 * max(runner, 1):
            continue
        (merges if best in ours else renames).append((t, best, n, why[(t, best)]))
    for t, best, n, evidence in renames:
        print(f"rename {t:30s} -> {best:28s} {n} votes   {evidence}")
    for t, best, n, evidence in merges:
        print(f"merge  {t:30s} -> {best:28s} {n} votes   {evidence}")
    print(f"\n{len(renames)} renames, {len(merges)} merges the tree already has the name for")

    if args.report:
        args.report.write_text(json.dumps({"batches": [
            {"batch": "ida_type_votes", "result": {"functions": [
                {"name": evidence.split(":")[0],
                 "types": [{"ours": t, "hd": best, "evidence": evidence}],
                 "members": []}
                for t, best, _, evidence in renames]}}]}, indent=1))
        print(f"{len(renames)} -> {args.report}")
    if args.merge_plan:
        args.merge_plan.write_text("".join(f"{t}\t{b}\n" for t, b, _, _ in merges))
        print(f"{len(merges)} -> {args.merge_plan}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
