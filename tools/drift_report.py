#!/usr/bin/env python3
"""Classify every invented type by what evidence exists for naming it.

Running the methods as a report rather than a rename says which of the remaining
names are reachable and which have no candidate from any direction, so the backlog is
a list with reasons instead of a number."""

import argparse
import re
import subprocess
import sys
from collections import defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import ida_members  # noqa: E402
from cparse import struct_fields  # noqa: E402
from ida_fill import ida_sizes, layout, project_sizes  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "docs" / "type-backlog.md"

CONVENTION = re.compile(r"_(?:t\d?|u|e|b|s|tag\d?)$")
INVENTED = re.compile(r"^[A-Z][a-z0-9]")
EXTERNAL = {"FILE", "_Bigint", "_glue", "_atexit", "__sbuf", "__siov", "__suio",
            "sceGsDispEnv", "sceSifDmaData", "sceDmaTag", "sceGifTag"}


def tool_candidates(script, args):
    """Parse a `name -> target` or `name ?= target` listing out of a tool's report."""
    out = defaultdict(set)
    done = subprocess.run([sys.executable, str(ROOT / "tools" / script)] + args,
                          cwd=ROOT, capture_output=True, text=True, timeout=1800)
    for line in done.stdout.splitlines():
        m = re.match(r"^\s+(\w+)\s+(?:->|\?=|\?)\s+(.*)$", line)
        if not m:
            continue
        # A report line trails counts and provenance after the candidate, so take
        # identifiers only up to the first field that is not one.
        for cand in re.split(r"[,\s]+", m.group(2).strip()):
            if not re.fullmatch(r"[A-Za-z_]\w*", cand or ""):
                break
            if not CONVENTION.search(cand):
                break  # a target always carries the suffix; "6" or "members" does not
            out[m.group(1)].add(cand)
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--write", action="store_true", help=f"write {OUT.name}")
    args = ap.parse_args()

    idb = ida_members.load(ROOT / "config" / "smt3" / "ida_members.txt")
    sizes = project_sizes(ida_sizes())

    invented = {}
    for path in sorted((ROOT / "src").rglob("*.c")):
        for kind, name, fields, line in struct_fields(path.read_text(errors="replace")):
            if not name or name in EXTERNAL or name in invented:
                continue
            if INVENTED.match(name) and not CONVENTION.search(name):
                rows = layout(fields, sizes)
                invented[name] = (kind, len(fields), rows,
                                  path.relative_to(ROOT).as_posix(), line)
    for path in sorted((ROOT / "include").rglob("*.h")):
        for kind, name, fields, line in struct_fields(path.read_text(errors="replace")):
            if name and name not in invented and INVENTED.match(name) \
                    and not CONVENTION.search(name) and name not in EXTERNAL:
                invented[name] = (kind, len(fields), layout(fields, sizes),
                                  path.relative_to(ROOT).as_posix(), line)

    sources = {
        "position": tool_candidates("hd_positions.py", ["--min-positions", "1"]),
        "unit layout": tool_candidates("hd_unit_types.py", ["--all"]),
        "member slot": tool_candidates("hd_member_types.py", []),
        "neighbour": tool_candidates("hd_neighbours.py", ["--span", "8"]),
        "duplicate": tool_candidates("merge_duplicates.py", []),
        "signature vote": tool_candidates("hd_signatures.py", ["--all"]),
    }

    rows = []
    for name, (kind, count, lay, rel, line) in sorted(invented.items()):
        got = {src: sorted(c[name]) for src, c in sources.items() if name in c}
        uniform = bool(lay) and len({s for _, s, _ in lay}) == 1
        rows.append((name, kind, count, rel, line, got, uniform, lay is not None))

    reachable = [r for r in rows if r[5]]
    agreed = [r for r in reachable if len({c for cs in r[5].values() for c in cs}) == 1]
    nothing = [r for r in rows if not r[5]]

    print(f"{len(rows)} invented types")
    print(f"  {len(agreed)} have one candidate and every method that spoke agrees")
    print(f"  {len(reachable) - len(agreed)} have candidates that disagree")
    print(f"  {len(nothing)} have no candidate from any method")
    unsized = sum(1 for r in rows if not r[7])
    uniform = sum(1 for r in rows if r[6])
    print(f"  of all of them, {unsized} cannot be sized here and {uniform} are a "
          f"uniform run of same-size members, which identifies nothing")

    if agreed:
        print("\nreachable, one agreed candidate:")
        for name, _, count, rel, line, got, _, _ in agreed:
            cand = next(iter(next(iter(got.values()))))
            print(f"  {name:<30} ?= {cand:<32} {', '.join(got)}  {rel}:{line}")

    if args.write:
        lines = [
            "# Invented type backlog",
            "",
            "From `tools/drift_report.py --write`. Every type still carrying an",
            "invented name, with whatever evidence exists for the real one. The",
            "methods and their refusals are described in [type recovery](type-recovery.md).",
            "",
            f"{len(rows)} types: {len(agreed)} with a single agreed candidate,",
            f"{len(reachable) - len(agreed)} with candidates that disagree, and",
            f"{len(nothing)} with no candidate from any method.",
            "",
            "## One agreed candidate",
            "",
            "Worth reading the code for first, and agreement is a lead rather than",
            "proof. `BillRenderPair` is the standing counterexample: two layout",
            "methods agree on `dds3MultiBillResource_tag` and both are wrong, since it",
            "duplicates `include/billboard.h`'s `billRenderPair_t` member for member.",
            "Methods reading the same shape can agree and still be mistaken.",
            "",
            "| type | candidate | from | where |",
            "|---|---|---|---|",
        ]
        for name, _, _, rel, line, got, _, _ in agreed:
            cand = next(iter(next(iter(got.values()))))
            lines.append(f"| `{name}` | `{cand}` | {', '.join(sorted(got))} "
                         f"| {rel}:{line} |")
        lines += ["", "## Candidates that disagree", "",
                  "Two methods reading the same shape can agree and both be wrong, so a",
                  "disagreement here is the useful signal, not noise.", "",
                  "| type | candidates | where |", "|---|---|---|"]
        for name, _, _, rel, line, got, _, _ in reachable:
            if name in {r[0] for r in agreed}:
                continue
            cands = sorted({c for cs in got.values() for c in cs})
            lines.append(f"| `{name}` | {', '.join(f'`{c}`' for c in cands[:5])} "
                         f"| {rel}:{line} |")
        lines += ["", "## No candidate from any method", "",
                  "Nothing in the IDB or the HD corpus has this shape in a place this",
                  "type could occupy. Some of these the originals never named either.",
                  ""]
        for name, _, count, rel, line, _, uni, sized in nothing:
            why = []
            if not sized:
                why.append("cannot be sized here")
            if uni:
                why.append("uniform member sizes")
            note = f" ({'; '.join(why)})" if why else ""
            lines.append(f"- `{name}`, {count} members, {rel}:{line}{note}")
        OUT.write_text("\n".join(lines) + "\n")
        print(f"\nwrote {OUT}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
