#!/usr/bin/env python3
"""Map our struct types to SMT3HD's by the types they co-occur with, function by
function.

Several of our types hold the right members under the wrong name: our `rstWork_t`
is HD's `rstData_t`, so nothing keyed on the name finds its member names. Resolving
every member access in both copies of one function gives the types each side reads
there, and a pair that keeps meeting across functions, with no competition, is the
same record."""

import argparse
import re
import sys
from collections import Counter, defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hd_resolve  # noqa: E402
from hd_pairs import definitions  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
HD = Path("/home/wsl/projects/smt3hd-reconstructed-sources")
ACCESS = re.compile(r"(->|\.)\s*([A-Za-z_]\w*)\b")
# il2cpp decorations on a type name, and the records it invents.
STRIP = re.compile(r"(_o|_c|_Fields|_StaticFields|_array|_tag_o)$")
NOISE = re.compile(r"^(?:System_|Il2Cpp|UnityEngine|Method|VirtualInvoke)")
UNNAMED = re.compile(r"^(?:unk(?:nown)?(?:[_0-9]|$)|field_0x|dummy\d|unused\d)", re.I)


def fold(names):
    return {n.lower() for n in names}


def strip(ty):
    prev = None
    while prev != ty:
        prev, ty = ty, STRIP.sub("", ty)
    return ty


def il2cpp_records():
    """{name: {member: type}} from the remaster's own metadata.

    The reconstruction's sources name records its headers never declare, which is
    why resolving an HD body needs this corpus as well."""
    out, cur = {}, None
    path = ROOT / "config" / "smt3" / "hd_il2cpp_types.txt"
    if not path.exists():
        return out
    for line in path.read_text().splitlines():
        if line.startswith("T "):
            cur = line.split()[1]
            out[cur] = {}
        elif line.startswith("M ") and cur:
            parts = line.split(None, 4)
            if len(parts) > 4:
                out[cur][parts[4].strip()] = parts[3]
    return out


def tree_index(root):
    """(members, functions, per-file [(scopes, globals, {fn: body})])."""
    members = hd_resolve.struct_members(root)
    funcs = hd_resolve.function_types(root)
    return members, funcs


def owners_in(text, body_start, body, members, scope_list, globals_, funcs):
    """Every type whose member the body reads, counted."""
    out = Counter()
    for m in ACCESS.finditer(body):
        pos = body_start + m.start()
        names = dict(globals_)
        for start, end, local in scope_list:
            if start <= pos < end:
                names.update(local)
                break
        ty = hd_resolve.owner_at(text, pos, names, members, funcs)
        if ty and ty != hd_resolve.UNKNOWN:
            out[ty] += 1
    return out


def per_file(path, members, funcs):
    text = path.read_text(errors="replace")
    scope_list = hd_resolve.scopes(text)
    globals_ = hd_resolve.file_scope(text, scope_list)
    out = {}
    for name, body in definitions(path).items():
        start = text.index(body)
        out[name] = owners_in(text, start, body, members, scope_list, globals_, funcs)
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--out", type=Path)
    ap.add_argument("--min-votes", type=int, default=2)
    ap.add_argument("--verbose", action="store_true")
    ap.add_argument("--all", action="store_true", help="print every candidate pair")
    args = ap.parse_args()

    our_members, our_funcs = tree_index(ROOT)
    hd_members, hd_funcs = tree_index(HD)
    for name, members in il2cpp_records().items():
        hd_members.setdefault(name, {}).update(
            {m: ty for m, ty in members.items() if m not in hd_members.get(name, {})})
    print(f"{len(our_members)} of our records, {len(hd_members)} of HD's")

    hd_side = {}
    for path in sorted((HD / "src").glob("*.c")):
        for fn, owners in per_file(path, hd_members, hd_funcs).items():
            hd_side.setdefault(fn, Counter()).update(owners)

    votes = defaultdict(Counter)
    ours_total, hd_total = Counter(), Counter()
    shared = 0
    for path in sorted((ROOT / "src").rglob("*.c")):
        for fn, owners in per_file(path, our_members, our_funcs).items():
            theirs = hd_side.get(fn)
            if not owners or not theirs:
                continue
            shared += 1
            for ours in owners:
                ours_total[ours] += 1
                for hd in theirs:
                    name = strip(hd)
                    if NOISE.match(name) or name not in hd_members:
                        continue
                    votes[ours][name] += 1
                    hd_total[name] += 1
    print(f"{shared} functions resolved in both trees")

    # Co-occurrence alone maps any two records one function reads together, so a
    # pair also has to name a record our tree does not have, and agree on the
    # member names our copy does carry.
    rows = []
    for ours, cands in sorted(votes.items()):
        if ours in hd_members:
            continue  # the name already agrees, so there is nothing to map
        named = {m for m in our_members.get(ours, {}) if not UNNAMED.match(m)}
        for best, n in cands.most_common(4):
            if best == ours or best in our_members or n < args.min_votes:
                continue
            runner = max((c for b, c in cands.items() if b != best and b not in our_members),
                         default=0)
            if not args.all and runner * 2 > n:
                continue
            agree = fold(named) & fold(hd_members.get(best, {}))
            if not args.all and not (agree or n >= 4):
                continue
            rows.append((n, ours, best, runner, len(agree), sorted(agree)[:4]))
            break
    rows.sort(reverse=True)
    print(f"\n{len(rows)} of our types map to an HD record we do not have "
          f"({args.min_votes}+ votes, double the runner-up, a member name agreeing "
          f"or 4+ votes):")
    for n, ours, best, runner, k, agree in rows:
        print(f"   {ours:34s} -> {best:30s} {n:3d} votes, runner-up {runner}, "
              f"{k} names agree {' '.join(agree)}")
    if args.out:
        args.out.write_text("# ours<TAB>HD<TAB>votes<TAB>agreeing member names\n"
                            + "".join(f"{o}\t{b}\t{n}\t{k}\n"
                                      for n, o, b, _, k, _ in rows))
        print(f"\n{len(rows)} pairs -> {args.out}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
