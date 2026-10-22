#!/usr/bin/env python3
"""Turn per-function alignment reports into a rename plan, and apply it.

A report says our type X is HD's Y and our member X.a is HD's Y.b. A proposal becomes
a rename only if Y is an HD type, b a member of it, the scalar class agrees, the
proposals for one type are injective and keep HD's member order, and nothing collides."""

import argparse
import json
import re
import sys
from collections import Counter, defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from cparse import struct_fields  # noqa: E402
from ida_fill import member_owners  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
CONVENTION = re.compile(r"_(?:t\d?|u|e|b|s|tag\d?)$")


def stem(name):
    """`effPCPSparkleSphereParam_t` -> `effpcpsparklesphere`, what a function name carries."""
    return re.sub(r"(param|part|list)\d*$", "", CONVENTION.sub("", name).lower())

FLOAT = {"f32", "f64", "float", "double"}
INT = {"s8", "u8", "s16", "u16", "s32", "u32", "s64", "u64", "char", "short", "int",
       "long", "bool", "unsigned", "signed"}


def scalar_class(ty):
    """float, int, ptr or struct, for a type text such as `sdfVector4_t*` or `u8`."""
    base = ty.replace("struct ", "").replace("const ", "").strip()
    if "*" in base or base.endswith("_array") or base in ("void*",):
        return "ptr"
    base = base.split("[")[0]
    if base in FLOAT:
        return "float"
    if base in INT:
        return "int"
    return "struct"


def hd_types():
    """name -> [(type, member)]"""
    out, cur = {}, None
    for line in (ROOT / "config" / "smt3" / "hd_types.txt").read_text().splitlines():
        if line.startswith("T "):
            cur = line.split()[1]
            out[cur] = []
        elif line.startswith("M ") and cur:
            p = line.split(None, 4)
            out[cur].append((p[3], p[4].strip() if len(p) > 4 else ""))
    return out


def our_types():
    """name -> {file: [(type, member, dims)]}, every definition of every struct."""
    out = defaultdict(dict)
    for path in sorted(list((ROOT / "include").rglob("*.h")) + list((ROOT / "src").rglob("*.c"))):
        for _, name, fields, _ in struct_fields(path.read_text(errors="replace")):
            if name:
                out[name][path] = [(ty + "*" * stars, member, dims) for ty, stars, member, dims in fields]
    return out


def load_reports(paths):
    for path in paths:
        data = json.loads(Path(path).read_text())
        batches = data.get("batches", [data]) if isinstance(data, dict) else data
        for b in batches:
            res = b.get("result", b) if isinstance(b, dict) else b
            if not res:
                continue
            for fn in res.get("functions", []):
                yield b.get("batch", path), fn


def plan(reports, hd, ours, verbose):
    type_votes = defaultdict(Counter)
    member_votes = defaultdict(Counter)
    evidence = {}
    for batch, fn in reports:
        for t in fn.get("types", []):
            type_votes[t["ours"]][t["hd"]] += 1
            evidence.setdefault(("type", t["ours"], t["hd"]), f"{fn['name']}: {t['evidence']}")
        for m in fn.get("members", []):
            key = (m["our_type"], m["our_member"])
            member_votes[key][(m["hd_type"], m["hd_member"])] += 1
            evidence.setdefault(("member", key, (m["hd_type"], m["hd_member"])),
                                f"{fn['name']}: {m['evidence']}")

    refused = []
    types = {}
    for ours_t, votes in type_votes.items():
        if ours_t not in ours:
            refused.append(f"type {ours_t}: not defined in the tree")
            continue
        if len(votes) > 1:
            refused.append(f"type {ours_t}: proposals disagree {dict(votes)}")
            continue
        (hd_t, n), = votes.items()
        if ours_t in hd:
            refused.append(f"type {ours_t} -> {hd_t}: {ours_t} is itself an HD type")
            continue
        if hd_t not in hd:
            refused.append(f"type {ours_t} -> {hd_t}: not an HD type")
            continue
        # One report is enough only when the function's own name carries the type's stem.
        if n < 2 and stem(hd_t) not in evidence[("type", ours_t, hd_t)].split(":")[0].lower():
            refused.append(f"type {ours_t} -> {hd_t}: one report, from a function not named for it")
            continue
        if hd_t in ours:
            refused.append(f"type {ours_t} -> {hd_t}: the tree already defines {hd_t}")
            continue
        if not CONVENTION.search(hd_t):
            refused.append(f"type {ours_t} -> {hd_t}: not a recovered-looking name")
            continue
        types[ours_t] = (hd_t, n)
    # Two of our types claiming one HD type would define it twice; both are two
    # readings of one record, which is a merge rather than a rename.
    claims = defaultdict(list)
    for ours_t, (hd_t, n) in types.items():
        claims[hd_t].append((n, ours_t))
    for hd_t, claimants in claims.items():
        if len(claimants) < 2:
            continue
        claimants.sort(reverse=True)
        (top_n, top), (second_n, _) = claimants[0], claimants[1]
        keep = top if top_n >= 3 and top_n >= 3 * second_n else None
        for n, ours_t in claimants:
            if ours_t != keep:
                refused.append(f"type {ours_t} -> {hd_t}: claimed by {len(claimants)} of our types"
                               + (f", {keep} wins on {top_n} reports" if keep else ""))
                del types[ours_t]

    def hd_name_of(t):
        return types[t][0] if t in types else t

    members = defaultdict(dict)  # our type -> {our member: hd member}
    for (our_t, our_m), votes in member_votes.items():
        if our_t not in ours:
            refused.append(f"member {our_t}.{our_m}: type not in the tree")
            continue
        if len(votes) > 1:
            refused.append(f"member {our_t}.{our_m}: proposals disagree {dict(votes)}")
            continue
        ((hd_t, hd_m), n), = votes.items()
        if hd_name_of(our_t) != hd_t:
            refused.append(f"member {our_t}.{our_m} -> {hd_t}.{hd_m}: {our_t} is not {hd_t} here")
            continue
        if hd_t not in hd or hd_m not in {m for _, m in hd[hd_t]}:
            refused.append(f"member {our_t}.{our_m} -> {hd_t}.{hd_m}: no such HD member")
            continue
        if our_m == hd_m:
            continue
        # Our member must exist in the definition and agree in scalar class.
        defs = ours[our_t]
        found = [(ty, dims) for fields in defs.values() for ty, m, dims in fields if m == our_m]
        if not found:
            refused.append(f"member {our_t}.{our_m}: not a member of our {our_t}")
            continue
        hd_ty = next(ty for ty, m in hd[hd_t] if m == hd_m)
        ours_cls = scalar_class(found[0][0]) if not found[0][1] else "struct"
        hd_cls = scalar_class(hd_ty)
        if ours_cls != hd_cls and not (hd_cls == "ptr" and ours_cls == "struct"):
            refused.append(f"member {our_t}.{our_m} ({found[0][0]}) -> {hd_t}.{hd_m} ({hd_ty}): class differs")
            continue
        existing = {m for fields in defs.values() if any(f[1] == our_m for f in fields)
                    for _, m, _ in fields}
        if hd_m in existing:
            refused.append(f"member {our_t}.{our_m} -> {hd_m}: {our_t} already has a member {hd_m}")
            continue
        members[our_t][our_m] = hd_m

    # Injective, and in HD's order relative to the members both sides already share.
    for our_t in list(members):
        hd_t = hd_name_of(our_t)
        order = {m: i for i, (_, m) in enumerate(hd[hd_t])}
        for fields in ours[our_t].values():
            seq = []
            for _, m, _ in fields:
                new = members[our_t].get(m, m)
                if new in order:
                    seq.append((m, new, order[new]))
            bad = set()
            for (m1, n1, i1), (m2, n2, i2) in zip(seq, seq[1:]):
                if i2 <= i1:
                    bad.add(m1 if m1 in members[our_t] else m2)
            for m in bad:
                refused.append(f"member {our_t}.{m} -> {members[our_t][m]}: out of HD member order")
                del members[our_t][m]
        targets = Counter(members[our_t].values())
        for m, new in list(members[our_t].items()):
            if targets[new] > 1:
                refused.append(f"member {our_t}.{m} -> {new}: two members map to it")
                del members[our_t][m]
        if not members[our_t]:
            del members[our_t]

    if verbose:
        for r in refused:
            print("refused", r)
    return types, members, refused, evidence


ERROR = re.compile(r"^(src/[^:]+\.c|include/[^:]+\.h):(\d+): .* has no member named `(\w+)'", re.M)
ACCESS = r"(\.|->)(\s*){old}\b"


def all_sources():
    return sorted(list((ROOT / "include").rglob("*.h")) + list((ROOT / "src").rglob("*.c")))


def rename_bodies(our_t, picks, ours):
    """Rename the members inside every definition of our_t."""
    for path in ours[our_t]:
        text = path.read_text()
        lines = text.split("\n")
        for _, sname, _, line in struct_fields(text):
            if sname != our_t:
                continue
            start, depth, end = line - 1, 0, line - 1
            for i in range(start, len(lines)):
                depth += lines[i].count("{") - lines[i].count("}")
                if depth == 0 and i > start:
                    end = i
                    break
            for old, new in picks.items():
                for i in range(start, end + 1):
                    lines[i] = re.sub(rf"\b{re.escape(old)}\b", new, lines[i])
        path.write_text("\n".join(lines))


def apply(types, members, ours, owners):
    """Rename struct bodies, then uses: tree-wide where no other type declares the
    old name, else left for the compiler to attribute (see fix)."""
    touched, deferred = set(), defaultdict(list)
    for our_t, picks in members.items():
        rename_bodies(our_t, picks, ours)
        touched.update(ours[our_t])
    for our_t, picks in members.items():
        for old, new in picks.items():
            shared = owners.get(old, set()) - {our_t}
            if shared or any(old in p and t != our_t for t, p in members.items()):
                if new not in deferred[old]:
                    deferred[old].append(new)
            else:
                pat = re.compile(ACCESS.format(old=re.escape(old)))
                for path in all_sources():
                    text = path.read_text(errors="replace")
                    swapped = pat.sub(rf"\1\2{new}", text)
                    if swapped != text:
                        path.write_text(swapped)
                        touched.add(path)
    if types:
        pat = re.compile(r"\b(" + "|".join(re.escape(t) for t in types) + r")\b")
        for path in all_sources():
            text = path.read_text(errors="replace")
            new = pat.sub(lambda m: types[m.group(1)][0], text)
            if new != text:
                path.write_text(new)
                touched.add(path)
    return touched, dict(deferred)


def fix(deferred, rounds=40):
    """Let the compiler attribute each deferred use.

    A line that errors with "no member named old" is a use of a type that lost
    `old`, so its next remaining `old` access is rewritten to the first candidate.
    An error naming a candidate we introduced means the guess was wrong for that
    access: it is put back and the next candidate tried, until none is left."""
    import subprocess
    introduced = {new: old for old, news in deferred.items() for new in news}
    state = defaultdict(list)  # (file, line) -> [{old, new, tried}]
    fixed, given_up = 0, []
    for _ in range(rounds):
        out = subprocess.run(["ninja", "-k", "0"], cwd=ROOT, capture_output=True, text=True)
        if out.returncode == 0:
            return fixed, given_up, 0
        progress = False
        for path, line, name in sorted(set(ERROR.findall(out.stdout + out.stderr))):
            p, i = ROOT / path, int(line) - 1
            lines = p.read_text().split("\n")
            if name in deferred:
                entry = {"old": name, "new": deferred[name][0], "tried": {deferred[name][0]}}
                swapped, n = re.subn(ACCESS.format(old=re.escape(name)), rf"\1\2{entry['new']}", lines[i], count=1)
                if not n:
                    continue
                state[(path, i)].append(entry)
            elif name in introduced and (path, i) in state:
                entry = next((e for e in state[(path, i)] if e["new"] == name), None)
                if entry is None:
                    continue
                left = [c for c in deferred[entry["old"]] if c not in entry["tried"]]
                if not left:
                    swapped = re.sub(ACCESS.format(old=re.escape(name)), rf"\1\2{entry['old']}", lines[i], count=1)
                    state[(path, i)].remove(entry)
                    given_up.append(f"{path}:{line}: {entry['old']} fits none of {deferred[entry['old']]}")
                else:
                    swapped = re.sub(ACCESS.format(old=re.escape(name)), rf"\1\2{left[0]}", lines[i], count=1)
                    entry["new"] = left[0]
                    entry["tried"].add(left[0])
                    fixed -= 1
            else:
                continue
            lines[i] = swapped
            p.write_text("\n".join(lines))
            fixed += 1
            progress = True
        if not progress:
            return fixed, given_up, out.returncode
    return fixed, given_up, 1


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("reports", nargs="+")
    ap.add_argument("--apply", action="store_true")
    ap.add_argument("--no-types", action="store_true", help="members only")
    ap.add_argument("--fix", action="store_true", help="after --apply, resolve deferred uses by compiling")
    ap.add_argument("-v", "--verbose", action="store_true")
    args = ap.parse_args()

    hd = hd_types()
    ours = our_types()
    types, members, refused, evidence = plan(list(load_reports(args.reports)), hd, ours, args.verbose)
    if args.no_types:
        types = {}
    for t, (hd_t, n) in sorted(types.items()):
        print(f"type {t} -> {hd_t}  ({n} functions; {evidence[('type', t, hd_t)]})")
    for t, picks in sorted(members.items()):
        hd_t = types[t][0] if t in types else t
        for m, new in picks.items():
            print(f"member {t}.{m} -> {new}  ({evidence[('member', (t, m), (hd_t, new))]})")
    print(f"{len(types)} types, {sum(map(len, members.values()))} members, {len(refused)} refused")
    if args.apply:
        touched, deferred = apply(types, members, ours, member_owners())
        print(f"touched {len(touched)} files; {len(deferred)} member names left for the compiler: "
              + ", ".join(f"{o}->{'|'.join(n)}" for o, n in deferred.items()))
        if args.fix:
            fixed, given_up, rc = fix(deferred)
            for g in given_up:
                print("unresolved", g)
            print(f"fixed {fixed} uses; {len(given_up)} unresolved; ninja exit {rc}")


if __name__ == "__main__":
    main()
