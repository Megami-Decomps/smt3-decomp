#!/usr/bin/env python3
"""Carry SMT3HD type names to DDS through the function names both repos share.

A function named the same in both repos takes the same parameters, so each
position is a vote for a type pairing. Taken only when the votes agree and the
layouts do. Rules: docs/dds-names.md."""

from __future__ import annotations

import argparse
import re
import sys
from collections import Counter, defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]

SCALARS = {"void", "char", "short", "int", "long", "float", "double", "signed",
           "unsigned", "bool", "u8", "u16", "u32", "u64", "s8", "s16", "s32",
           "s64", "f32", "f64", "size_t", "va_list", "__int8", "__int16",
           "__int32", "__int64", "_BYTE", "_WORD", "_DWORD", "_QWORD"}
IDENT = re.compile(r"^[A-Za-z_]\w*$")
# A DDS header declares each struct's extent with a sizeof assertion.
SIZE_ASSERT = re.compile(
    r"typedef\s+char\s+(\w+)_size\s*\[\s*\(\s*sizeof\s*\(\s*(\w+)\s*\)\s*==\s*(0x[0-9A-Fa-f]+|\d+)")
TYPEDEF_TAG = re.compile(r"typedef\s+(struct|union|enum)\s+(\w+)\s*\{")
PROTO_DECL = re.compile(r"^\s*(?:extern\s+)?([A-Za-z_][\w \t\*]*?)\b(\w+)\s*\(([^;{)]*)\)\s*;", re.M)
PROTO_DEF = re.compile(
    r"^(?!static\b|extern\b|typedef\b)([A-Za-z_][\w \t\*]*?)\b(\w+)\s*\(([^;{)]*)\)\s*\{", re.M)
T_ROW = re.compile(r"^T (\S+) (\S+) (\S+)$")
# dds-decomp names its own types in CamelCase, so a lowercase candidate is a
# parameter name this parser mistook for a type.
DDS_TYPE = re.compile(r"^[A-Z]\w*$")
# IDA writes an owner into a nested type's name, which is not a name to carry.
ARTIFACT = re.compile(r"_(?:sdf|dds3|eff|fld|btl|evt|itf|mdl|scr|snd|par|bill|fr|nb|cmp|cmb)[A-Z0-9]")
# Below this the top candidate is not clearly the one, and a runner-up this
# close means the two types are used in the same positions.
MIN_VOTES = 4
DOMINANCE = 4


def split_params(text):
    """One parameter per entry, splitting only at top-level commas."""
    out, depth, current = [], 0, ""
    for ch in text:
        if ch in "([":
            depth += 1
        elif ch in ")]":
            depth -= 1
        if ch == "," and depth == 0:
            out.append(current)
            current = ""
        else:
            current += ch
    if current.strip():
        out.append(current)
    return [p.strip() for p in out if p.strip()]


def base_type(text):
    """(type name, pointer depth) for one parameter, or None when not a named type."""
    text = text.strip()
    if not text or text == "void" or "(" in text:
        return None
    text = re.sub(r"\b(const|volatile|struct|union|enum|register)\b", " ", text)
    stars = text.count("*")
    text = text.replace("*", " ")
    words = [w for w in re.split(r"[\s\[\]]+", text) if w]
    if not words:
        return None
    # The declarator name is dropped only when a type word precedes it.
    candidates = [w for w in words if w not in SCALARS]
    if len(words) > 1 and candidates and candidates[-1] == words[-1] and len(candidates) > 1:
        candidates = candidates[:-1]
    for word in candidates:
        if IDENT.match(word) and word not in SCALARS:
            return word, stars
    return None


def smt3_prototypes(path):
    """{function name: (return type, [(type, depth) or None per parameter])}."""
    out = {}
    for line in Path(path).read_text().splitlines():
        if line.startswith("#"):
            continue
        parts = line.split(None, 2)
        if len(parts) != 3:
            continue
        _addr, name, proto = parts
        m = re.match(rf"^(.*?)\s*\b{re.escape(name)}\s*\((.*)\)\s*$", proto.strip())
        if not m:
            continue
        out[name] = (base_type(m[1]), [base_type(p) for p in split_params(m[2])])
    return out


def smt3_sizes(path):
    sizes = {}
    for line in Path(path).read_text().splitlines():
        m = T_ROW.match(line)
        if m:
            sizes[m[1]] = (m[2], int(m[3]) if m[3].isdigit() else None)
    return sizes


def dds_types(include_dir):
    """{type name: declared size or None} for every type a DDS header defines."""
    sizes, declared = {}, set()
    for path in sorted(Path(include_dir).rglob("*.h")):
        text = path.read_text()
        for m in SIZE_ASSERT.finditer(text):
            if m[1] == m[2]:
                sizes[m[1]] = int(m[3], 0)
        declared |= {m[2] for m in TYPEDEF_TAG.finditer(text)}
        declared |= set(re.findall(r"^\s*\}\s*(\w+)\s*;", text, re.M))
        declared |= set(re.findall(r"typedef\s+(?:struct|union|enum)\s+(\w+)\s+\1\s*;", text))
    for name in declared:
        if DDS_TYPE.match(name):
            sizes.setdefault(name, None)
    return {k: v for k, v in sizes.items() if DDS_TYPE.match(k)}


def dds_prototypes(version, dds_decomp):
    """{function name: (return type, [param types])} from the headers and the C."""
    out = {}
    paths = list((dds_decomp / "include").rglob("*.h"))
    paths += list((dds_decomp / "src" / version).rglob("*.c"))
    for path in paths:
        text = path.read_text()
        for pattern in (PROTO_DECL, PROTO_DEF):
            for m in pattern.finditer(text):
                name = m[2]
                if name in SCALARS or not IDENT.match(name):
                    continue
                params = [base_type(p) for p in split_params(m[3])]
                ret = base_type(m[1])
                # A definition wins over a declaration, and the first wins otherwise.
                if pattern is PROTO_DEF or name not in out:
                    out[name] = (ret, params)
    return out


def collect_votes(smt3, dds, dds_type_sizes):
    """{dds type: Counter of smt3 type} over positions whose pointer depth agrees."""
    votes = defaultdict(Counter)
    functions = defaultdict(set)
    for name, (dds_ret, dds_params) in dds.items():
        proto = smt3.get(name)
        if proto is None:
            continue
        smt3_ret, smt3_params = proto
        if len(smt3_params) != len(dds_params):
            continue
        positions = list(zip(dds_params, smt3_params)) + [(dds_ret, smt3_ret)]
        for mine, theirs in positions:
            if not mine or not theirs or mine[1] != theirs[1]:
                continue
            if mine[0] not in dds_type_sizes:
                continue
            votes[mine[0]][theirs[0]] += 1
            functions[(mine[0], theirs[0])].add(name)
    return votes, functions


def decide(votes, functions, dds_type_sizes, smt3_size_rows, excluded, dds_names):
    rows, winner = [], {}
    for mine, counter in votes.items():
        (theirs, n), *rest = counter.most_common()
        runner_up = rest[0][1] if rest else 0
        status = "accept"
        kind, smt3_size = smt3_size_rows.get(theirs, (None, None))
        dds_size = dds_type_sizes.get(mine)
        if runner_up and n < runner_up * DOMINANCE:
            status = f"reject:votes-{n}-vs-{runner_up}"
        elif mine == theirs:
            status = "same"
        elif mine in excluded:
            status = "reject:excluded"
        elif kind is None:
            status = "reject:not-an-smt3-type"
        elif n < MIN_VOTES:
            status = f"reject:only-{n}-votes"
        elif ARTIFACT.search(theirs):
            status = "reject:owner-in-name"
        elif theirs in dds_names:
            status = "reject:name-taken-in-dds"
        elif dds_size is not None and smt3_size is not None and dds_size != smt3_size:
            status = f"reject:size-{dds_size:#x}-vs-{smt3_size:#x}"
        if status == "accept":
            winner[mine] = (theirs, n, sorted(functions[(mine, theirs)])[:3])
        rows.append((mine, theirs, status, n, dds_size, smt3_size))
    counts = Counter(t for t, _n, _f in winner.values())
    for mine, (theirs, n, funcs) in list(winner.items()):
        if counts[theirs] > 1:
            rows = [(a, b, "reject:two-dds-types-one-name" if a == mine and st == "accept" else st,
                     c, d, e) for a, b, st, c, d, e in rows]
            winner.pop(mine)
    return rows, winner


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("--dds-decomp", type=Path, required=True)
    ap.add_argument("--out-dir", type=Path, default=ROOT / "build" / "dds_names" / "out")
    args = ap.parse_args()

    smt3 = smt3_prototypes(ROOT / "config/smt3/ida_protos.txt")
    smt3_size_rows = smt3_sizes(ROOT / "config/smt3/ida_members.txt")
    dds_type_sizes = dds_types(args.dds_decomp / "include")
    # Any identifier a DDS header already declares as a type, CamelCase or not.
    dds_all_names = set()
    for path in (args.dds_decomp / "include").rglob("*.h"):
        text = path.read_text()
        dds_all_names |= {m[2] for m in TYPEDEF_TAG.finditer(text)}
        dds_all_names |= set(re.findall(r"^\s*\}\s*(\w+)\s*;", text, re.M))
        dds_all_names |= set(re.findall(r"typedef\s+[\w \t\*]+?(\w+)\s*;", text, re.M))
    excluded = set()
    path = args.dds_decomp / "config" / "smt3_type_exclude.txt"
    if path.exists():
        for line in path.read_text().splitlines():
            body, _, _reason = line.partition("#")
            if body.split():
                excluded.add(body.split()[0])

    args.out_dir.mkdir(parents=True, exist_ok=True)
    print(f"{len(smt3)} SMT3 prototypes, {len(dds_type_sizes)} DDS types "
          f"({sum(1 for v in dds_type_sizes.values() if v is not None)} with a declared size)")
    for version in ("dds1", "dds2"):
        dds = dds_prototypes(version, args.dds_decomp)
        shared = sum(1 for n in dds if n in smt3)
        votes, functions = collect_votes(smt3, dds, dds_type_sizes)
        rows, winner = decide(votes, functions, dds_type_sizes, smt3_size_rows, excluded,
                              dds_all_names)
        renames = [f"{mine}\t{theirs}" for mine, (theirs, _n, _f) in sorted(winner.items())]
        (args.out_dir / f"type_renames_{version}.tsv").write_text("\n".join(renames) + "\n")
        head = [f"# SMT3HD type names for {version}, voted through shared function names.",
                "# Generated by smt3-decomp tools/dds_type_names.py.",
                "# dds_type\tsmt3_type\tstatus\tvotes\tdds_size\tsmt3_size\tsample functions"]
        body = []
        for mine, theirs, status, n, dds_size, smt3_size in sorted(rows):
            sample = ", ".join(sorted(functions[(mine, theirs)])[:3])
            body.append(f"{mine}\t{theirs}\t{status}\t{n}\t"
                        f"{dds_size if dds_size is None else hex(dds_size)}\t"
                        f"{smt3_size if smt3_size is None else hex(smt3_size)}\t{sample}")
        (args.out_dir / f"type_pairs_{version}.tsv").write_text("\n".join(head + body) + "\n")
        counts = Counter(st for _m, _t, st, *_ in rows)
        print(f"{version}: {len(dds)} C prototypes, {shared} share a name  " +
              "  ".join(f"{k}={v}" for k, v in sorted(counts.items())))
    return 0


if __name__ == "__main__":
    sys.exit(main())
