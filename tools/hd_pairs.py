#!/usr/bin/env python3
"""Write the function pairs an agent reads to align our names with SMT3HD's.

One record per function defined in both trees whose body touches an invented type,
an `unk_` member or a member HD's copy of the type lacks: both definitions, our
struct definitions and HD's member lists. Batched under --out, with an index."""

import argparse
import json
import re
import sys
from collections import defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from cparse import struct_fields  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
HD = Path("/home/wsl/projects/smt3hd-reconstructed-sources/src")

SIG = re.compile(r"^([A-Za-z_][\w \t\*]*?)\b(\w+)\s*\(([^;{}]*)\)\s*\{", re.M)
MEMBER = re.compile(r"(?:->|\.)\s*([A-Za-z_]\w*)")
UNNAMED = re.compile(r"^(?:unk(?:nown)?(?:[_0-9]|$)|field_0x|dummy\d|unused\d)", re.I)
IDENT = re.compile(r"\b[A-Za-z_]\w*\b")


def definitions(path):
    text = path.read_text(errors="replace")
    out = {}
    for m in SIG.finditer(text):
        i, depth = m.end(), 1
        while depth and i < len(text):
            depth += (text[i] == "{") - (text[i] == "}")
            i += 1
        out.setdefault(m.group(2), text[m.start():i])
    return out


def hd_members():
    """name -> [(type, member)] from config/smt3/hd_types.txt."""
    out, cur = {}, None
    for line in (ROOT / "config" / "smt3" / "hd_types.txt").read_text().splitlines():
        if line.startswith("T "):
            cur = line.split()[1]
            out[cur] = []
        elif line.startswith("M ") and cur:
            p = line.split(None, 4)
            out[cur].append((p[3], p[4].strip() if len(p) > 4 else ""))
    return out


def our_structs(root):
    """name -> (file, [(type, member)]) for every struct the tree defines."""
    out = {}
    for path in sorted(list((root / "include").rglob("*.h")) + list((root / "src").rglob("*.c"))):
        for kind, name, fields, _ in struct_fields(path.read_text(errors="replace")):
            if name and name not in out:
                out[name] = (path.relative_to(root).as_posix(),
                             [(ty + "*" * stars, name + "".join(f"[{d}]" for d in dims))
                              for ty, stars, name, dims in fields])
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--root", type=Path, default=ROOT)
    ap.add_argument("--out", type=Path, required=True)
    ap.add_argument("--batch", type=int, default=10)
    ap.add_argument("--only", help="regex on our file path")
    args = ap.parse_args()

    base = (ROOT / "config" / "smt3" / "drift_baseline.txt").read_text().splitlines()
    invented = {l.split()[1] for l in base if l.startswith("type ")}
    hdm = hd_members()
    hd_member_names = {m for ms in hdm.values() for _, m in ms}
    ours = our_structs(args.root)
    ida = {line.split()[0] for line in (ROOT / "config" / "smt3" / "ida_types.txt").read_text().splitlines()
           if line and not line.startswith("#")}

    drifted = set()
    for t, (_, fields) in ours.items():
        if t in hdm:
            theirs = {m for _, m in hdm[t]}
            drifted |= {m for _, m in fields if m not in theirs and not m.startswith("pad")}
    drifted -= hd_member_names  # a name some other HD type has is ambiguous on its own

    hd_defs = {}
    for path in sorted(HD.glob("*.c")):
        for n, body in definitions(path).items():
            hd_defs.setdefault(n, (path.name, body))

    records = []
    for path in sorted((args.root / "src").rglob("*.c")):
        rel = path.relative_to(args.root).as_posix()
        if args.only and not re.search(args.only, rel):
            continue
        for name, body in definitions(path).items():
            if name not in hd_defs:
                continue
            idents = set(IDENT.findall(body))
            mems = set(MEMBER.findall(body))
            reasons = []
            if idents & invented:
                reasons.append("invented type: " + ", ".join(sorted(idents & invented)))
            if any(UNNAMED.match(m) for m in mems):
                reasons.append("unnamed members: " + ", ".join(sorted(m for m in mems if UNNAMED.match(m))))
            if mems & drifted:
                reasons.append("members HD lacks: " + ", ".join(sorted(mems & drifted)))
            foreign = {t for t in idents if t in ours and t not in hdm and t not in ida and t not in invented}
            if foreign:
                reasons.append("our type absent from HD: " + ", ".join(sorted(foreign)))
            if not reasons:
                continue
            hd_file, hd_body = hd_defs[name]
            hd_types = {t[:-2] if t.endswith("_o") else t
                        for t in IDENT.findall(hd_body) if t in hdm or (t.endswith("_o") and t[:-2] in hdm)}
            our_types = {t for t in idents if t in ours}
            records.append({
                "function": name, "file": rel, "hd_file": hd_file, "reasons": reasons,
                "ours": body, "hd": hd_body,
                "our_structs": {t: ours[t] for t in sorted(our_types)},
                "hd_structs": {t: hdm[t] for t in sorted(our_types | hd_types) if t in hdm},
            })

    args.out.mkdir(parents=True, exist_ok=True)
    by_file = defaultdict(list)
    for r in records:
        by_file[r["file"]].append(r)
    index, n = [], 0
    for rel, recs in sorted(by_file.items()):
        for i in range(0, len(recs), args.batch):
            chunk = recs[i:i + args.batch]
            n += 1
            out = args.out / f"batch{n:04d}.txt"
            with out.open("w") as f:
                for r in chunk:
                    f.write(f"=== FUNCTION {r['function']}  ({r['file']}; HD: {r['hd_file']})\n")
                    f.write("why: " + "; ".join(r["reasons"]) + "\n")
                    f.write("--- OURS\n" + r["ours"].rstrip() + "\n")
                    f.write("--- HD\n" + r["hd"].rstrip() + "\n")
                    for t, (where, fields) in r["our_structs"].items():
                        f.write(f"--- OUR STRUCT {t} ({where})\n")
                        f.write("\n".join(f"  {ty} {m}" for ty, m in fields) + "\n")
                    for t, fields in r["hd_structs"].items():
                        f.write(f"--- HD STRUCT {t}\n")
                        f.write("\n".join(f"  {ty} {m}" for ty, m in fields) + "\n")
                    f.write("\n")
            index.append({"batch": out.name, "file": rel,
                          "functions": [r["function"] for r in chunk]})
    (args.out / "index.json").write_text(json.dumps(index, indent=1))
    print(f"{len(records)} functions in {n} batches under {args.out}")


if __name__ == "__main__":
    main()
