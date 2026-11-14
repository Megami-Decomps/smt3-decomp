#!/usr/bin/env python3
"""Count what the HD exports newly name: members, defines and enum constants.

Members are compared between two hd_types.txt files (--old, default git HEAD).
Constants are found only inside functions defined in both trees: where HD's body
uses a define or enum member and ours has its value as a literal. Only a lead, since
a value can coincide; a rename needs the surrounding statements to agree."""

import argparse
import re
import struct
import subprocess
import sys
from collections import Counter, defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from cparse import struct_fields  # noqa: E402
from hd_align import load_type_map  # noqa: E402
from hd_pairs import definitions  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
HD = Path("/home/wsl/projects/smt3hd-reconstructed-sources")
UNNAMED = re.compile(r"^(?:unk(?:nown)?(?:[_0-9]|[0-9A-Fa-f]+$|$)|field_0x|dummy\d|unused\d)", re.I)
INT = re.compile(r"(?<![\w.])(-?(?:0[xX][0-9A-Fa-f]+|\d+))[uUlL]*(?![\w.])")
FLOAT = re.compile(r"(?<![\w.])(-?\d+\.\d*(?:[eE][-+]?\d+)?)f?(?![\w.])")
IDENT = re.compile(r"\b[A-Za-z_]\w*\b")


def hd_members(text):
    out, cur = {}, None
    for line in text.splitlines():
        if line.startswith("T "):
            cur = line.split()[1]
            out[cur] = []
        elif line.startswith("M ") and cur:
            out[cur].append(line.split(None, 4)[4].strip())
    return out


def our_structs():
    out = {}
    for path in sorted(list((ROOT / "include").rglob("*.h")) + list((ROOT / "src").rglob("*.c"))):
        for kind, name, fields, _ in struct_fields(path.read_text(errors="replace")):
            if name and name not in out:
                out[name] = (path.relative_to(ROOT), [f[2] for f in fields])
    return out


def member_counts(hd, ours, tmap):
    inv, unk, paired = [], [], 0
    for name, (path, names) in ours.items():
        target = tmap.get(name, name)
        if target not in hd:
            continue
        paired += 1
        for m in names:
            if UNNAMED.match(m):
                unk.append((name, m))
            elif m not in hd[target]:
                inv.append((name, m))
    return paired, inv, unk


def literal_value(tok):
    try:
        return int(tok, 0)
    except ValueError:
        return int(tok, 10)


def all_defs(root, glob):
    out = {}
    for path in sorted(root.rglob(glob)):
        for n, body in definitions(path).items():
            out.setdefault(n, (path, body))
    return out


def value_only(defines, kinds, enums):
    """Names whose value is unique across all HD defines and enum members and at least
    256 in magnitude, against literals of that value in our units. HD's bodies are
    preprocessed, so no body proves the pairing; a unit whose module matches the
    header's is the strongest context available."""
    rows = defaultdict(list)
    for n, v in defines.items():
        if n in kinds and len(n) >= 4:
            rows[literal_value(INT.fullmatch(v.strip()).group(1))].append((n, "define"))
    for n, (v, e) in enums.items():
        if len(n) >= 4:
            rows[v].append((n, "enum"))
    unique = {v: r[0] for v, r in rows.items() if len(r) == 1 and abs(v) >= 256}
    src = {}
    for line in (ROOT / "config/smt3/hd_defines.txt").read_text().splitlines():
        p = line.split(None, 3)
        if p[0] == "D":
            src[p[1]] = p[2]
    count = Counter()
    sites = defaultdict(list)
    for path in sorted((ROOT / "src").rglob("*.c")):
        text = re.sub(r"/\*.*?\*/|//[^\n]*", "", path.read_text(errors="replace"), flags=re.S)
        text = "\n".join(l for l in text.splitlines() if not l.lstrip().startswith("#"))
        text = re.sub(r'"(?:\\.|[^"\\])*"', '""', text)
        for m in INT.finditer(text):
            v = literal_value(m.group(1))
            if v in unique:
                name, kind = unique[v]
                stem = src.get(name, "").removesuffix(".h").lower()
                inmod = bool(stem) and (stem in path.stem.lower() or path.stem.lower().startswith(stem[:3]))
                count[(kind, inmod)] += 1
                if len(sites[(kind, inmod)]) < 4:
                    sites[(kind, inmod)].append(f"{path.relative_to(ROOT)}: {m.group(1)} -> {name}")
    print(f"values unique across defines and enum members, |v| >= 256: {len(unique)}")
    for key in sorted(count):
        print(f"  {key[0]:6} {'in module' if key[1] else 'elsewhere':9} {count[key]:6} literal sites")
        for t in sites[key]:
            print("      " + t)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--old", help="git rev of the old exports (default HEAD)", default="HEAD")
    ap.add_argument("--show", type=int, default=3, help="example sites per kind")
    args = ap.parse_args()

    new_text = (ROOT / "config/smt3/hd_types.txt").read_text()
    old_text = subprocess.run(["git", "show", f"{args.old}:config/smt3/hd_types.txt"],
                              cwd=ROOT, capture_output=True, text=True, check=True).stdout
    new, old = hd_members(new_text), hd_members(old_text)
    ours = our_structs()
    try:
        tmap = load_type_map(ROOT / "config/smt3/hd_type_map.txt")
    except Exception:
        tmap = {}
    print(f"HD types: old {len(old)}, new {len(new)}; added {len(set(new) - set(old))}, "
          f"dropped {len(set(old) - set(new))}")
    for label, hd in (("old", old), ("new", new)):
        paired, inv, unk = member_counts(hd, ours, tmap)
        print(f"{label}: {paired} of {len(ours)} our structs have an HD record; "
              f"{len(inv)} invented members, {len(unk)} unnamed members")
    _, inv_o, unk_o = member_counts(old, ours, tmap)
    _, inv_n, unk_n = member_counts(new, ours, tmap)
    print(f"invented members resolved by the refresh: {len(set(inv_o) - set(inv_n))}, "
          f"newly invented: {len(set(inv_n) - set(inv_o))}; unnamed in a newly paired type: "
          f"{len(set(unk_n) - set(unk_o))}")

    defines, enums = {}, {}
    for line in (ROOT / "config/smt3/hd_defines.txt").read_text().splitlines():
        if line.startswith("D "):
            p = line.split(None, 3)
            if len(p) == 4:
                defines[p[1]] = p[3]
    cur = None
    for line in (ROOT / "config/smt3/hd_enums.txt").read_text().splitlines():
        if line.startswith("E "):
            cur = line.split()[1]
        elif line.startswith("V "):
            _, v, n = line.split()
            enums[n] = (int(v), cur)
    by_val = defaultdict(list)
    kinds = {}
    for n, v in defines.items():
        m = INT.fullmatch(v.strip())
        if m:
            by_val[literal_value(m.group(1))].append(n)
            kinds[n] = "define"
    fl = {}
    for n, v in defines.items():
        m = FLOAT.fullmatch(v.strip())
        if m:
            fl[n] = float(m.group(1))
    print(f"defines: {len(defines)}; integer-valued {sum(1 for k in kinds.values())}, "
          f"float-valued {len(fl)}, other (strings, expressions) {len(defines) - len(kinds) - len(fl)}")

    hd_defs = all_defs(HD / "src", "*.c")
    our_defs = all_defs(ROOT / "src", "*.c")
    both = sorted(set(hd_defs) & set(our_defs))
    print(f"functions defined in both trees: {len(both)}")
    hits = defaultdict(list)
    for fn in both:
        hbody, obody = hd_defs[fn][1], our_defs[fn][1]
        otoks = Counter()
        for m in INT.finditer(obody):
            otoks[literal_value(m.group(1))] += 1
        used = [t for t in set(IDENT.findall(hbody))]
        for t in used:
            if len(t) < 4:
                continue  # U, D, L, R and similar collide with ordinary identifiers
            if t in defines and t in kinds:
                v = literal_value(INT.fullmatch(defines[t].strip()).group(1))
                src = "define"
            elif t in enums:
                v, src = enums[t][0], "enum"
            else:
                continue
            if otoks.get(v):
                same = [n for n in set(used) if (n in kinds and literal_value(INT.fullmatch(defines[n].strip()).group(1)) == v) or (n in enums and enums[n][0] == v)]
                big = abs(v) >= 16
                key = (src, "big" if big else "small", "unique" if len(same) == 1 else "ambiguous")
                hits[key].append((fn, t, v, otoks[v], our_defs[fn][0].relative_to(ROOT)))
    print("literal sites with a name in HD's body of the same function (functions, sites):")
    for key in sorted(hits):
        rows = hits[key]
        print(f"  {key[0]:6} {key[1]:5} {key[2]:9} {len(rows):5} pairs {sum(r[3] for r in rows):6} sites")
        for fn, t, v, n, path in rows[: args.show]:
            print(f"      {path}  {fn}: {v} -> {t} ({n}x)")
    value_only(defines, kinds, enums)
    nf = 0
    for fn in both:
        hbody, obody = hd_defs[fn][1], our_defs[fn][0] and our_defs[fn][1]
        ofl = {float(m.group(1)) for m in FLOAT.finditer(obody)}
        ofl |= {struct.unpack("<I", struct.pack("<f", f))[0] for f in ofl}
        for t in set(IDENT.findall(hbody)):
            if t in kinds and len(t) >= 4:
                v = literal_value(INT.fullmatch(defines[t].strip()).group(1))
                if v >= 0x10000 and v in ofl:
                    nf += 1
    print(f"float-bit defines (>= 0x10000) whose bits or float value appear in our same-named function: {nf}")


if __name__ == "__main__":
    main()
