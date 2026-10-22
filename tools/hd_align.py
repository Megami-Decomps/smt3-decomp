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

ROOT = Path(__file__).resolve().parents[1]
CONVENTION = re.compile(r"_(?:t\d?|u|e|b|s|tag\d?)$")


# il2cpp names an embedded base `_0`, `_1`, and carries its own runtime fields.
# None of these is an original member name, and taking one is a regression:
# `effBTLSystem_Attack_tag.pFormation` became `_0`.
ARTIFACT = re.compile(r"^(?:_\d+|m_\w+|klass|monitor|vtable|_vtbl|obj|fields|"
                      r"padding\d*|_padding\w*)$")


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


def idb_shapes():
    """name -> (size, member count) from the IDB, whose layouts sit at real addresses."""
    out, cur = {}, None
    for line in (ROOT / "config" / "smt3" / "ida_members.txt").read_text().splitlines():
        if line.startswith("T "):
            p = line.split()
            cur = p[1]
            out[cur] = [int(p[3]) if p[3].isdigit() else -1, 0]
        elif line.startswith("M ") and cur:
            out[cur][1] += 1
    return {k: tuple(v) for k, v in out.items()}


def layout_fits(ours_t, hd_t, ours, shapes, sizes):
    """Whether our record's shape corroborates the name, by the rules type
    recovery settled on: a size that fits, or an equal member count.

    Size is one-way. Ours larger than the IDB's is as likely to mean our shape is
    wrong as the name, so it does not corroborate; ours fitting inside does."""
    from ida_fill import layout
    if hd_t not in shapes:
        return False
    size, count = shapes[hd_t]
    for fields in ours[ours_t].values():
        rows = layout([(ty.replace("*", "").strip(), ty.count("*"), m, d)
                       for ty, m, d in fields], sizes)
        if rows:
            offset, extent_, _ = rows[-1]
            if 0 < size and offset + extent_ <= size:
                return True
        if count and len(fields) == count:
            return True
    return False


def hd_types():
    """name -> [(type, member)], from the HD corpus and the IDB's own layouts.

    Both carry the HD symbol recovery, and the IDB holds records the
    reconstruction's headers do not, so a target absent from one is often in the
    other. HD wins where both describe a record, its member order being what that
    corpus is authoritative on."""
    out, cur = {}, None
    for line in (ROOT / "config" / "smt3" / "hd_types.txt").read_text().splitlines():
        if line.startswith("T "):
            cur = line.split()[1]
            out[cur] = []
        elif line.startswith("M ") and cur:
            p = line.split(None, 4)
            out[cur].append((p[3], p[4].strip() if len(p) > 4 else ""))
    cur = None
    for line in (ROOT / "config" / "smt3" / "ida_members.txt").read_text().splitlines():
        if line.startswith("T "):
            name = line.split()[1]
            cur = None if out.get(name) else name
            out.setdefault(cur, [])
        elif line.startswith("M ") and cur:
            p = line.split(None, 4)
            out[cur].append((p[3], p[4].strip() if len(p) > 4 else ""))
    return {k: v for k, v in out.items() if k and v}


def normalise(name, corpus):
    """The corpus name a report means, undoing il2cpp's decorations.

    Its output carries an `_o` suffix for the object wrapper and qualifies a
    file-local record with its file (`effBattleSystem_effBTLSystem_Attack_tag`),
    neither of which is the original type name."""
    if name in corpus:
        return name
    for cand in ([name[:-2]] if name.endswith("_o") else []) + \
                [name.split("_", i)[-1] for i in range(1, name.count("_") + 1)]:
        if cand in corpus:
            return cand
        if cand.endswith("_o") and cand[:-2] in corpus:
            return cand[:-2]
    return name


def our_types():
    """name -> {file: [(type, member, dims)]}, every definition of every struct."""
    out = defaultdict(dict)
    for path in sorted(list((ROOT / "include").rglob("*.h")) + list((ROOT / "src").rglob("*.c"))):
        for _, name, fields, _ in struct_fields(path.read_text(errors="replace")):
            if name:
                out[name][path] = [(ty + "*" * stars, member, dims) for ty, stars, member, dims in fields]
    return out


DECL = re.compile(r"^\s*(?:const\s+|volatile\s+|struct\s+|union\s+|unsigned\s+|signed\s+)*"
                  r"[A-Za-z_]\w*[\s*]+([A-Za-z_]\w*)\s*(?:\[[^\]]*\])*\s*;")
OPEN = re.compile(r"\b(?:struct|union)\b[^;{]*\{")


def nested_member_names():
    """Member names declared below the top level of a struct or union.

    `cparse.struct_fields` records only depth-1 members, so a name an anonymous
    nested struct declares looks unowned: rewriting `.fontIndex` tree-wide also
    rewrote `setupBytes.fontIndex`, which frMsgInfo_t does not declare."""
    out = set()
    for path in all_sources():
        depth = 0
        for line in path.read_text(errors="replace").split("\n"):
            if depth >= 2:
                m = DECL.match(line)
                if m:
                    out.add(m.group(1))
            opens = line.count("{")
            if depth == 0 and opens and not OPEN.search(line):
                continue  # a function body or an initialiser, not a record
            depth = max(0, depth + opens - line.count("}"))
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
    from ida_fill import ida_sizes, project_sizes
    shapes = idb_shapes()
    sizes = project_sizes(ida_sizes())
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
    # Every identifier in code positions: a one-line typedef or a forward
    # declaration of the target name is a collision the struct parser cannot see.
    strip = re.compile(r'/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"', re.S)
    used = set()
    for path in all_sources():
        used.update(re.findall(r"\b[A-Za-z_]\w*\b", strip.sub(" ", path.read_text(errors="replace"))))
    applied = {}
    for ours_t, votes in type_votes.items():
        votes = Counter({normalise(k, hd): v for k, v in votes.items()})
        if ours_t not in ours:
            # An earlier pass may have taken this rename already, in which case the
            # report still carries our old spelling and its members are still owed.
            done = [h for h in votes if h in ours and h in hd]
            if len(done) == 1:
                applied[ours_t] = done[0]
            else:
                refused.append(f"type {ours_t}: not defined in the tree")
            continue
        ranked = votes.most_common()
        if len(ranked) > 1 and not (ranked[0][1] >= 2 and ranked[0][1] >= 2 * ranked[1][1]):
            refused.append(f"type {ours_t}: proposals disagree {dict(votes)}")
            continue
        hd_t, n = ranked[0]
        if ours_t in hd:
            refused.append(f"type {ours_t} -> {hd_t}: {ours_t} is itself an HD type")
            continue
        if hd_t not in hd:
            refused.append(f"type {ours_t} -> {hd_t}: not an HD type")
            continue
        # One report needs corroboration: the function's own name carrying the
        # type's stem, or two member names our record already shares with it.
        named_for = any(stem(hd_t) in e.split(":")[0].lower()
                        for k, e in evidence.items() if k[:2] == ("type", ours_t))
        shared = len({m for f in ours[ours_t].values() for _, m, _ in f}
                     & {m for _, m in hd[hd_t]})
        fits = layout_fits(ours_t, hd_t, ours, shapes, sizes)
        if n < 2 and not named_for and shared < 2 and not fits:
            refused.append(f"type {ours_t} -> {hd_t}: one report, not named for it, "
                           f"{shared} member names shared, shape disagrees")
            continue
        if hd_t in ours or hd_t in used:
            refused.append(f"type {ours_t} -> {hd_t}: the tree already uses the name {hd_t}")
            continue
        if ARTIFACT.match(hd_t) or hd_t.endswith(("_array", "_StaticFields", "_Fields")):
            refused.append(f"type {ours_t} -> {hd_t}: an il2cpp artifact, not a name")
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
        if t in types:
            return types[t][0]
        return applied.get(t, t)

    members = defaultdict(dict)  # our type -> {our member: hd member}
    for (reported_t, our_m), votes in member_votes.items():
        votes = Counter({(normalise(a, hd), b): v for (a, b), v in votes.items()})
        our_t = applied.get(reported_t, reported_t)
        if our_t != reported_t:
            for key in [k for k in evidence if k[0] == "member" and k[1] == (reported_t, our_m)]:
                evidence.setdefault(("member", (our_t, our_m), key[2]), evidence[key])
        if our_t not in ours:
            refused.append(f"member {reported_t}.{our_m}: type not in the tree")
            continue
        ranked = votes.most_common()
        if len(ranked) > 1 and not (ranked[0][1] >= 2 and ranked[0][1] >= 2 * ranked[1][1]):
            refused.append(f"member {our_t}.{our_m}: proposals disagree {dict(votes)}")
            continue
        (hd_t, hd_m), n = ranked[0]
        if hd_name_of(our_t) != hd_t:
            refused.append(f"member {our_t}.{our_m} -> {hd_t}.{hd_m}: {our_t} is not {hd_t} here")
            continue
        if hd_t not in hd or hd_m not in {m for _, m in hd[hd_t]}:
            refused.append(f"member {our_t}.{our_m} -> {hd_t}.{hd_m}: no such HD member")
            continue
        if ARTIFACT.match(hd_m):
            refused.append(f"member {our_t}.{our_m} -> {hd_m}: an il2cpp artifact, not a name")
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
        # The remaster rewrote inline arrays and embedded records as pointers, so an
        # HD pointer standing where we hold the bytes inline is the expected shape.
        pointerized = hd_cls == "ptr" and (ours_cls in ("struct", "int") or found[0][1])
        if ours_cls != hd_cls and not pointerized and {ours_cls, hd_cls} != {"struct", "int"}:
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
                    # Two members we did not rename being out of HD's order is
                    # the tree's own layout reading, not this plan's mistake.
                    bad.update(m for m in (m1, m2) if m in members[our_t])
            for m in bad:
                if m in members[our_t]:
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


def struct_spans(lines, name):
    """[(first, last)] line index spans of every definition of struct `name`.

    Found by name rather than by struct_fields' line number, which is off in
    some files and once renamed members of the neighbouring struct instead."""
    spans = []
    header = re.compile(rf"\b(?:struct|union)\s+{re.escape(name)}\b")
    closer = re.compile(rf"^\s*\}}\s*{re.escape(name)}\s*;")
    for i, line in enumerate(lines):
        if header.search(line) and ("{" in line or (i + 1 < len(lines) and lines[i + 1].strip().startswith("{"))):
            depth, j = 0, i
            while j < len(lines):
                depth += lines[j].count("{") - lines[j].count("}")
                if depth == 0 and "{" in "".join(lines[i:j + 1]):
                    spans.append((i, j))
                    break
                j += 1
        elif closer.match(line):
            depth, j = 0, i
            while j >= 0:
                depth += lines[j].count("}") - lines[j].count("{")
                if depth == 0:
                    spans.append((j, i))
                    break
                j -= 1
    return sorted(set(spans))


def rename_bodies(our_t, picks, ours):
    """Rename the members inside every definition of our_t."""
    for path in ours[our_t]:
        lines = path.read_text().split("\n")
        spans = struct_spans(lines, our_t)
        if not spans:
            print(f"warning: no body of {our_t} found in {path}")
        for first, last in spans:
            for old, new in picks.items():
                for i in range(first, last + 1):
                    lines[i] = re.sub(rf"\b{re.escape(old)}\b", new, lines[i])
        path.write_text("\n".join(lines))


def apply_resolved(types, members, ours):
    """Rewrite each member use whose object expression resolves to the renamed type.

    This is what a shared member name needs: `->color` belongs to whichever type
    the base identifier was declared as, so only the accesses that resolve to our
    type are rewritten and the rest are left exactly as they were. An access the
    resolver cannot place is left alone and shows up as a compile error naming the
    old member, which is loud rather than silent."""
    import hd_resolve

    members_of = hd_resolve.struct_members(ROOT)
    funcs = hd_resolve.function_types(ROOT)
    touched, counts = set(), Counter()
    for our_t, picks in members.items():
        rename_bodies(our_t, picks, ours)
        touched.update(ours[our_t])
    for path in all_sources():
        text = path.read_text(errors="replace")
        if not any(old in text for picks in members.values() for old in picks):
            continue
        scope_list = hd_resolve.scopes(text)
        globals_ = hd_resolve.file_scope(text, scope_list)
        edits = []
        for our_t, picks in members.items():
            for old, new in picks.items():
                for start, end, owner in hd_resolve.accesses(
                        text, old, scope_list, globals_, members_of, funcs):
                    if owner == our_t:
                        edits.append((start, end, old, new))
        for start, end, old, new in sorted(edits, reverse=True):
            text = text[:start] + re.sub(rf"{re.escape(old)}$", new, text[start:end]) + text[end:]
            counts[old] += 1
        if edits:
            path.write_text(text)
            touched.add(path)
    if types:
        pat = re.compile(r"\b(" + "|".join(re.escape(k) for k in types) + r")\b")
        for path in all_sources():
            text = path.read_text(errors="replace")
            swapped = pat.sub(lambda m: types[m.group(1)][0], text)
            if swapped != text:
                path.write_text(swapped)
                touched.add(path)
    return touched, counts


def fix_single(members, rounds=25):
    """Place the accesses the resolver could not, where only one target is possible.

    An access the resolver left alone errors with the old member name. When the
    whole plan renames that name to exactly one target, and the error line holds
    exactly one unresolvable access of it, the placement is forced rather than
    guessed. Anything else is reported and left."""
    import subprocess
    import hd_resolve

    targets = defaultdict(set)
    for picks in members.values():
        for old, new in picks.items():
            targets[old].add(new)
    single = {o: next(iter(v)) for o, v in targets.items() if len(v) == 1}
    members_of = hd_resolve.struct_members(ROOT)
    funcs = hd_resolve.function_types(ROOT)
    placed, stuck = 0, []
    for _ in range(rounds):
        out = subprocess.run(["ninja", "-k", "0"], cwd=ROOT, capture_output=True, text=True)
        if out.returncode == 0:
            return placed, [], 0
        progress = False
        stuck = []
        for path, line, name in sorted(set(ERROR.findall(out.stdout + out.stderr))):
            if name not in single:
                stuck.append(f"{path}:{line}: {name} has {len(targets[name])} possible targets")
                continue
            p = ROOT / path
            text = p.read_text()
            lines = text.splitlines()
            start = sum(len(l) + 1 for l in lines[:int(line) - 1])
            end = start + len(lines[int(line) - 1])
            sc = hd_resolve.scopes(text)
            unknown = [a for a in hd_resolve.accesses(
                text, name, sc, hd_resolve.file_scope(text, sc), members_of, funcs)
                if a[2] is None and start <= a[0] < end]
            if len(unknown) != 1:
                stuck.append(f"{path}:{line}: {len(unknown)} unresolvable uses of {name}")
                continue
            s, e, _ = unknown[0]
            p.write_text(text[:s] + re.sub(rf"{re.escape(name)}$", single[name], text[s:e]) + text[e:])
            placed += 1
            progress = True
        if not progress:
            return placed, stuck, out.returncode
    return placed, stuck, 1


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


def untaken(candidates, tried, placed, old):
    """The first candidate not already placed on this line for a different member.

    Two members of one struct can offer the same candidate (`angle` and
    `angleStep` both offer `rotate`), and collapsing them onto it compiles while
    reading the wrong offset: `part->rotate += part->rotate` came out 4 words
    wrong. Repeats of the SAME member on one line are legitimate, though
    (`dst->color = src->color` when both are that type), so only a clash between
    two different members is refused."""
    taken = {e["new"]: e["old"] for e in placed}
    for c in candidates:
        if c not in tried and taken.get(c, old) == old:
            return c
    return None


def fix(deferred, rounds=80):
    """Let the compiler attribute each deferred use.

    A line that errors with "no member named old" is a use of a type that lost
    `old`, so its next remaining `old` access is rewritten to the first candidate.
    An error naming a candidate we introduced means the guess was wrong for that
    access: it is put back and the next candidate tried, until none is left."""
    import subprocess
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
            # A candidate can itself be a deferred old name (`flags` offers `flag`,
            # and `flag` is deferred too), so continuing an existing chain has to be
            # tried first or the loop abandons the outer rename's remaining candidates.
            entry = next((e for e in state.get((path, i), ()) if e["new"] == name), None)
            if entry is None and name in deferred:
                pick = untaken(deferred[name], set(), state.get((path, i), ()), name)
                if pick is None:
                    given_up.append(f"{path}:{line}: {name} has no candidate free on this line")
                    continue
                entry = {"old": name, "new": pick, "tried": {pick}}
                swapped, n = re.subn(ACCESS.format(old=re.escape(name)), rf"\1\2{pick}", lines[i], count=1)
                if not n:
                    continue
                state[(path, i)].append(entry)
            elif entry is not None:
                pick = untaken(deferred[entry["old"]], entry["tried"],
                               [e for e in state[(path, i)] if e is not entry], entry["old"])
                if pick is None:
                    swapped = re.sub(ACCESS.format(old=re.escape(name)), rf"\1\2{entry['old']}", lines[i], count=1)
                    state[(path, i)].remove(entry)
                    given_up.append(f"{path}:{line}: {entry['old']} fits none of {deferred[entry['old']]}")
                else:
                    swapped = re.sub(ACCESS.format(old=re.escape(name)), rf"\1\2{pick}", lines[i], count=1)
                    entry["new"] = pick
                    entry["tried"].add(pick)
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
    ap.add_argument("reports", nargs="*")
    ap.add_argument("--plan", type=Path,
                    help="apply a TSV of `type<TAB>old<TAB>new` rows instead of reports, "
                         "for proposals another tool produced (tools/ida_fill.py)")
    ap.add_argument("--apply", action="store_true")
    ap.add_argument("--no-types", action="store_true", help="members only")
    ap.add_argument("--fix", action="store_true", help="after --apply, resolve deferred uses by compiling")
    ap.add_argument("--resolve", action="store_true",
                    help="place each member use by the resolved type of its object "
                         "expression, which is what a shared member name needs")
    ap.add_argument("--safe-only", action="store_true",
                    help="skip members whose old name another type also declares, so every "
                         "rewrite is unambiguous and no compile-and-guess pass is needed")
    ap.add_argument("--exclude", type=Path,
                    help="file of member names to drop from the plan, one per line; "
                         "write it from a previous run's unresolved list")
    ap.add_argument("--unresolved-out", type=Path,
                    help="write the member names --fix could not place, for --exclude")
    ap.add_argument("-v", "--verbose", action="store_true")
    args = ap.parse_args()

    hd = hd_types()
    ours = our_types()
    if args.plan:
        types, members, refused, evidence = {}, defaultdict(dict), [], {}
        for line in args.plan.read_text().splitlines():
            if not line.strip() or line.startswith("#"):
                continue
            our_t, old_m, new_m = line.split("\t")
            if ARTIFACT.match(new_m) and not ARTIFACT.match(old_m):
                refused.append(f"member {our_t}.{old_m} -> {new_m}: an il2cpp artifact, not a name")
            elif our_t in ours and any(m == old_m for f in ours[our_t].values() for _, m, _ in f):
                members[our_t][old_m] = new_m
            else:
                refused.append(f"member {our_t}.{old_m}: not a member of our {our_t}")
    else:
        types, members, refused, evidence = plan(
            list(load_reports(args.reports)), hd, ours, args.verbose)
    if args.no_types:
        types = {}
    # Ownership has to be complete over THIS tree, which is what a rewrite
    # touches; the IDB's own types are not in it and would only over-refuse.
    owners = defaultdict(set)
    for name, defs in ours.items():
        for fields in defs.values():
            for _, m, _ in fields:
                owners[m].add(name)
    for name in nested_member_names():
        owners[name].add("<nested>")
    if args.safe_only:
        kept, dropped = 0, 0
        for our_t in list(members):
            safe = {m: n for m, n in members[our_t].items() if not (owners.get(m, set()) - {our_t})}
            dropped += len(members[our_t]) - len(safe)
            kept += len(safe)
            if safe:
                members[our_t] = safe
            else:
                del members[our_t]
        print(f"safe-only: {kept} members rewritable tree-wide, {dropped} deferred to the backlog")
    if args.exclude and args.exclude.exists():
        drop = {l.strip() for l in args.exclude.read_text().splitlines()
                if l.strip() and not l.startswith("#")}
        for our_t in list(members):
            members[our_t] = {m: n for m, n in members[our_t].items() if m not in drop}
            if not members[our_t]:
                del members[our_t]
        print(f"excluded {len(drop)} member names")
    for r in refused if args.plan else []:
        print("refused", r)
    for t, (hd_t, n) in sorted(types.items()):
        why = next((e for k, e in evidence.items() if k[:2] == ("type", t)), "")
        print(f"type {t} -> {hd_t}  ({n} functions; {why})")
    for t, picks in sorted(members.items()):
        hd_t = types[t][0] if t in types else t
        for m, new in picks.items():
            key = ("member", (t, m), (hd_t, new))
            print(f"member {t}.{m} -> {new}" + (f"  ({evidence[key]})" if key in evidence else ""))
    print(f"{len(types)} types, {sum(map(len, members.values()))} members, {len(refused)} refused")
    if args.apply and args.resolve:
        touched, counts = apply_resolved(types, members, ours)
        print(f"touched {len(touched)} files; {sum(counts.values())} uses placed by type")
        for old, n in counts.most_common(10):
            print(f"  {old}: {n}")
        if args.fix:
            n, stuck, rc = fix_single(members)
            for s in stuck:
                print("stuck", s)
            print(f"placed {n} more by elimination; {len(stuck)} stuck; ninja exit {rc}")
    elif args.apply:
        touched, deferred = apply(types, members, ours, owners)
        print(f"touched {len(touched)} files; {len(deferred)} member names left for the compiler: "
              + ", ".join(f"{o}->{'|'.join(n)}" for o, n in deferred.items()))
        if args.fix:
            fixed, given_up, rc = fix(deferred)
            for g in given_up:
                print("unresolved", g)
            if args.unresolved_out:
                names = sorted({g.split(": ")[1].split(" fits")[0] for g in given_up})
                args.unresolved_out.write_text("\n".join(names) + "\n" if names else "")
                print(f"wrote {len(names)} names to {args.unresolved_out}")
            print(f"fixed {fixed} uses; {len(given_up)} unresolved; ninja exit {rc}")


if __name__ == "__main__":
    main()
