#!/usr/bin/env python3
"""Name an invented type from the IDB's prototype at the same slot.

Our signatures say where a type sits: the return, or parameter n. The IDB has a
prototype for nearly every function, so the type it names in that slot is the
answer, with no layout comparison at all. This reads the invented names out of
the drift baseline, finds the signatures that mention them, and prints what the
IDB calls each slot.

    python3 tools/ida_slot_types.py                 # every invented type
    python3 tools/ida_slot_types.py SoundMixer      # just these
"""

import os
import re
import sys
from pathlib import Path

sys.path.insert(0, os.path.expanduser("~/.claude/skills/ida-instances/scripts"))
import ida  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
MODULE = "SLUS_209.11"
# A definition or a declaration: return type, name, arguments, then `;` or `{`.
SIG = re.compile(r"^([A-Za-z_][\w \*]*?)\b(\w+)\s*\(([^;{]*)\)\s*[;{]", re.M)
NOT_A_CALL = {"if", "while", "for", "switch", "sizeof", "return", "do"}
ARGS = re.compile(r"\(([^)]*)\)\s*$")


def port():
    for inst in ida.discover_instances(ida.parse_ports("13337-13437")):
        if inst.get("module") == MODULE or MODULE in str(inst.get("path", "")):
            return inst["port"]
    raise SystemExit(f"{MODULE} is not open")


def invented():
    return [l.split()[1] for l in
            (ROOT / "config" / "smt3" / "drift_baseline.txt").read_text().splitlines()
            if l.startswith("type ")]


def slots(types):
    """{type: {(function, slot)}}, slot 0 being the return."""
    out = {}
    pats = {t: re.compile(r"\b" + re.escape(t) + r"\b") for t in types}
    for path in (ROOT / "src").rglob("*.c"):
        for m in SIG.finditer(path.read_text(errors="replace")):
            ret, fn, args = m.group(1), m.group(2), m.group(3)
            if fn in NOT_A_CALL:
                continue
            parts = [a.strip() for a in args.split(",")]
            for t, pat in pats.items():
                if pat.search(ret):
                    out.setdefault(t, set()).add((fn, 0))
                for i, arg in enumerate(parts):
                    if pat.search(arg):
                        out.setdefault(t, set()).add((fn, i + 1))
    return out


def prototypes(p, names):
    out = {}
    for i in range(0, len(names), 60):
        chunk = names[i:i + 60]
        found = {e["query"]: e["fn"]["addr"]
                 for e in ida.call_tool(p, "lookup_funcs", {"queries": chunk})["result"]
                 if e.get("fn")}
        if not found:
            continue
        byname = {f["name"]: f["prototype"] for f in ida.call_tool(
            p, "export_funcs",
            {"addrs": list(found.values()), "format": "prototypes"})["functions"]}
        out.update({q: byname[q] for q in found if q in byname})
    return out


def main():
    types = sys.argv[1:] or invented()
    where = slots(types)
    p = port()
    protos = prototypes(p, sorted({fn for v in where.values() for fn, _ in v}))
    for t in sorted(where):
        rows = []
        for fn, slot in sorted(where[t]):
            proto = protos.get(fn)
            if not proto:
                continue
            if slot == 0:
                rows.append(f"{fn} returns {proto.split('(')[0].strip()}")
                continue
            m = ARGS.search(proto)
            if not m:
                continue
            args = [a.strip() for a in m.group(1).split(",")]
            if slot <= len(args):
                rows.append(f"{fn} arg{slot}: {args[slot - 1]}")
        if rows:
            print(f"{t}")
            for row in rows:
                print(f"    {row}")


if __name__ == "__main__":
    main()
