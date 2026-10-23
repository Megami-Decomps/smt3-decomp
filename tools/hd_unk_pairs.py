#!/usr/bin/env python3
"""Write one record per type holding unnamed members: our definition, the HD record
it is, and the bodies of the functions that read those members in both trees.

tools/hd_members.py names an unk_ member only where the gap between two shared names
forces it. Everything else needs the function bodies read side by side, which is what
these batches are for."""

import argparse
import json
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hd_resolve  # noqa: E402
from cparse import struct_fields  # noqa: E402
from hd_align import load_type_map  # noqa: E402
from hd_pairs import definitions  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
HD = Path("/home/wsl/projects/smt3hd-reconstructed-sources")
UNNAMED = re.compile(r"^(?:unk(?:nown)?(?:[_0-9]|$)|field_0x|dummy\d|unused\d)", re.I)
MAX_BODY = 2400
MANAGED = {}


def clip(body, limit=MAX_BODY):
    return body if len(body) <= limit else body[:limit] + "\n/* ...truncated */\n"


def hd_records():
    """name -> [(offset, size, type, member)] from the HD exports.

    The remaster's own metadata carries records the reconstruction's headers leave
    out; its offsets are the managed 64-bit layout, so the batch says so."""
    out = {}
    for name, managed in (("hd_types.txt", False), ("hd_il2cpp_types.txt", True)):
        path = ROOT / "config" / "smt3" / name
        if not path.exists():
            continue
        cur = None
        for line in path.read_text().splitlines():
            if line.startswith("T "):
                key = line.split()[1]
                cur = None if out.get(key) else key
                if cur:
                    out[cur] = []
                    MANAGED[cur] = managed
            elif line.startswith("M ") and cur:
                p = line.split(None, 4)
                out[cur].append((p[1], p[2], p[3], p[4].strip() if len(p) > 4 else ""))
    return {k: v for k, v in out.items() if v}


def our_definitions():
    """name -> (file, line, source text of the definition, [members])."""
    out = {}
    for path in sorted(list((ROOT / "include").rglob("*.h")) + list((ROOT / "src").rglob("*.c"))):
        text = path.read_text(errors="replace")
        lines = text.split("\n")
        for _, name, fields, line in struct_fields(text):
            if not name or name in out:
                continue
            depth, j = 0, line - 1
            while j < len(lines):
                depth += lines[j].count("{") - lines[j].count("}")
                if depth == 0 and j > line - 1:
                    break
                j += 1
            out[name] = (path, line, "\n".join(lines[line - 1:j + 1]),
                         [m for _, _, m, _ in fields])
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--out", type=Path, required=True)
    ap.add_argument("--batch", type=int, default=2)
    ap.add_argument("--max-funcs", type=int, default=4)
    ap.add_argument("--map", type=Path, default=ROOT / "config" / "smt3" / "hd_type_map.txt")
    ap.add_argument("--allow-unpaired", action="store_true",
                    help="also batch types whose readers the remaster does not define, "
                         "where the record's member list is the only evidence")
    args = ap.parse_args()

    hd = hd_records()
    aliases = load_type_map(args.map)
    ours = our_definitions()

    hd_defs = {}
    for path in sorted((HD / "src").glob("*.c")):
        for fn, body in definitions(path).items():
            hd_defs.setdefault(fn, (path.name, body))

    # A member name alone does not say whose member it is, so each reader is
    # confirmed by resolving the access to the type that declares it.
    our_members = hd_resolve.struct_members(ROOT)
    our_funcs = hd_resolve.function_types(ROOT)
    our_defs = []
    for path in sorted((ROOT / "src").rglob("*.c")):
        text = path.read_text(errors="replace")
        scope_list = hd_resolve.scopes(text)
        globals_ = hd_resolve.file_scope(text, scope_list)
        for fn, body in definitions(path).items():
            start = text.index(body)
            owners = {}
            for m in re.finditer(r"(?:->|\.)\s*([A-Za-z_]\w*)\b", body):
                names = dict(globals_)
                for s, e, local in scope_list:
                    if s <= start + m.start() < e:
                        names.update(local)
                        break
                ty = hd_resolve.owner_at(text, start + m.start(), names,
                                         our_members, our_funcs)
                if ty:
                    owners.setdefault(ty, set()).add(m.group(1))
            our_defs.append((fn, path, body, owners))

    records = []
    for name in sorted(ours):
        path, line, text, members = ours[name]
        unk = [m for m in members if UNNAMED.match(m)]
        hd_name = name if name in hd else aliases.get(name)
        if not unk or not hd_name or hd_name not in hd:
            continue
        users = []
        for fn, src, body, owners in our_defs:
            hits = owners.get(name, set()) & set(unk)
            if hits and (fn in hd_defs or args.allow_unpaired):
                users.append((len(hits), fn, src, body))
        users.sort(reverse=True, key=lambda u: u[0])
        if not users:
            continue
        records.append({"type": name, "hd": hd_name,
                        "file": path.relative_to(ROOT).as_posix(), "line": line,
                        "text": text, "unk": unk, "users": users[:args.max_funcs]})

    args.out.mkdir(parents=True, exist_ok=True)
    index, n = [], 0
    for i in range(0, len(records), args.batch):
        n += 1
        out = args.out / f"unk{n:04d}.txt"
        with out.open("w") as f:
            for r in records[i:i + args.batch]:
                f.write(f"=== OUR TYPE {r['type']}  ({r['file']}:{r['line']})"
                        f"  == HD RECORD {r['hd']}\n{clip(r['text'], 3000)}\n")
                note = ("  (offsets are the remaster's 64-bit layout, not the PS2's)"
                        if MANAGED.get(r["hd"]) else "")
                f.write(f"--- HD {r['hd']} MEMBERS (offset size type name){note}\n")
                for off, size, ty, m in hd[r["hd"]]:
                    f.write(f"  {off:>6} {size:>4} {ty} {m}\n")
                for _, fn, src, body in r["users"]:
                    f.write(f"--- OURS {fn} ({src.relative_to(ROOT).as_posix()})\n"
                            f"{clip(body).rstrip()}\n")
                    if fn in hd_defs:
                        hd_file, hd_body = hd_defs[fn]
                        f.write(f"--- HD {fn} ({hd_file})\n{clip(hd_body).rstrip()}\n")
                    else:
                        f.write(f"--- HD {fn}: the remaster has no function of this name\n")
                f.write("\n")
        index.append({"batch": out.name,
                      "types": [(r["type"], r["hd"], len(r["unk"])) for r in records[i:i + args.batch]]})
    (args.out / "index.json").write_text(json.dumps(index, indent=1))
    unk_total = sum(len(r["unk"]) for r in records)
    print(f"{len(records)} types with {unk_total} unnamed members and a readable HD "
          f"counterpart, in {n} batches under {args.out}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
