#!/usr/bin/env python3
"""Write one record per invented type: our definition, the functions that use it,
and the HD bodies of the same functions with the candidate records they name.

A type name is what makes its members recoverable, so these are worth asking about
directly rather than through the per-function pairs of tools/hd_pairs.py."""

import argparse
import json
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from cparse import struct_fields  # noqa: E402
from hd_pairs import definitions, hd_members  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
HD = Path("/home/wsl/projects/smt3hd-reconstructed-sources/src")

CONVENTION = re.compile(r"_(?:t\d?|u|e|b|s|tag\d?)$")
INVENTED = re.compile(r"^[A-Z][a-z0-9]")
IDENT = re.compile(r"\b[A-Za-z_]\w*\b")
MAX_BODY = 2600


def clip(body):
    return body if len(body) <= MAX_BODY else body[:MAX_BODY] + "\n/* ...truncated */\n"


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--out", type=Path, required=True)
    ap.add_argument("--batch", type=int, default=3)
    ap.add_argument("--max-funcs", type=int, default=5)
    args = ap.parse_args()

    hdm = hd_members()
    ida = {}
    cur = None
    for line in (ROOT / "config" / "smt3" / "ida_members.txt").read_text().splitlines():
        if line.startswith("T "):
            cur = line.split()[1]
            ida[cur] = []
        elif line.startswith("M ") and cur:
            p = line.split(None, 4)
            ida[cur].append((p[3], p[4].strip() if len(p) > 4 else ""))
    corpus = {**ida, **{k: v for k, v in hdm.items() if v}}

    # Our invented types, with the file that defines each and its member list.
    invented = {}
    for path in sorted(list((ROOT / "include").rglob("*.h")) + list((ROOT / "src").rglob("*.c"))):
        for _, name, fields, line in struct_fields(path.read_text(errors="replace")):
            if name and INVENTED.match(name) and not CONVENTION.search(name) and name not in invented:
                invented[name] = (path, line,
                                  [(ty + "*" * stars, m + "".join(f"[{d}]" for d in dims))
                                   for ty, stars, m, dims in fields])
    hd_defs = {}
    for path in sorted(HD.glob("*.c")):
        for n, body in definitions(path).items():
            hd_defs.setdefault(n, (path.name, body))

    records = []
    for name, (path, line, fields) in sorted(invented.items()):
        users = []
        for src in sorted((ROOT / "src").rglob("*.c")):
            text = src.read_text(errors="replace")
            if name not in text:
                continue
            for fn, body in definitions(src).items():
                if re.search(rf"\b{re.escape(name)}\b", body) and fn in hd_defs:
                    users.append((fn, src, body))
        users.sort(key=lambda u: -len(re.findall(rf"\b{re.escape(name)}\b", u[2])))
        users = users[:args.max_funcs]
        if not users:
            continue
        cands = {}
        for fn, _, _ in users:
            for tok in IDENT.findall(hd_defs[fn][1]):
                for key in (tok, tok[:-2] if tok.endswith("_o") else tok):
                    if key in corpus and key not in invented:
                        cands[key] = corpus[key]
        records.append({"type": name, "file": path.relative_to(ROOT).as_posix(), "line": line,
                        "fields": fields, "users": users, "cands": cands})

    args.out.mkdir(parents=True, exist_ok=True)
    index, n = [], 0
    for i in range(0, len(records), args.batch):
        n += 1
        out = args.out / f"type{n:04d}.txt"
        with out.open("w") as f:
            for r in records[i:i + args.batch]:
                f.write(f"=== OUR TYPE {r['type']}  ({r['file']}:{r['line']})\n")
                f.write("\n".join(f"  {ty} {m}" for ty, m in r["fields"]) + "\n")
                for fn, src, body in r["users"]:
                    hd_file, hd_body = hd_defs[fn]
                    f.write(f"--- OURS {fn} ({src.relative_to(ROOT).as_posix()})\n{clip(body).rstrip()}\n")
                    f.write(f"--- HD {fn} ({hd_file})\n{clip(hd_body).rstrip()}\n")
                f.write(f"--- CANDIDATE RECORDS named in those HD bodies ({len(r['cands'])})\n")
                for t, ms in sorted(r["cands"].items()):
                    f.write(f"  {t}: " + ", ".join(f"{ty} {m}" for ty, m in ms[:24]) + "\n")
                f.write("\n")
        index.append({"batch": out.name, "types": [r["type"] for r in records[i:i + args.batch]]})
    (args.out / "index.json").write_text(json.dumps(index, indent=1))
    print(f"{len(records)} invented types with HD-shared users, in {n} batches under {args.out}")


if __name__ == "__main__":
    main()
