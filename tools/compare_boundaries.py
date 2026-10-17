#!/usr/bin/env python3
"""Score the applied unit boundaries against the in-binary boundary evidence.

    python3 tools/compare_boundaries.py smt3

Reads the applied `c` subsegments from the splat yaml and the rules
tools/find_boundaries.py applies, and writes nothing but --json.
"""

from __future__ import annotations

import argparse
import bisect
import collections
import json
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

import find_boundaries as fb

ROOT = Path(__file__).resolve().parent.parent
SUBSEGMENT = re.compile(r"^\s*-\s*\[\s*(0x[0-9A-Fa-f]+)\s*,\s*c\s*,\s*([^\]\s]+)\s*\]")
# The HD source map feeds both tu_candidates.tsv's hd columns and find_boundaries'
# hd-source rule, and 56 of the 60 applied starts sit on an HD run boundary, so an
# hd-source row at an applied start is the same witness twice. Only these rules read
# the binary alone.
BINARY_ONLY = {"dup-string", "core-edge", "dup-func"}
# A unit start the evidence misses by a function or two is still the same boundary
# seen through a different rule; past this the row is about something else.
NEAR = 2
# Order used for "the best row at this address".
RANK = {"strong": 3, "medium": 2, "weak": 1}


def applied_units(yaml_path: Path, named_only=True) -> list[tuple[int, str]]:
    """`c` subsegments as (vram, unit), in address order."""
    out = []
    for line in yaml_path.read_text().splitlines():
        if (m := SUBSEGMENT.match(line)) and not (named_only
                                                  and m.group(2).startswith("code_")):
            out.append((int(m.group(1), 16) + 0xFF000, m.group(2)))
    return sorted(out)


def candidates(tsv: Path) -> tuple[list[str], list[list[str]]]:
    head, rows = [], []
    for line in tsv.read_text().splitlines():
        if line.startswith("#"):
            head.append(line)
        elif line.startswith("start\t"):
            cols = line.split("\t")
        elif line.strip():
            rows.append(line.split("\t"))
    return head, [cols] + rows


# Applied boundaries a source other than tu_evidence.tsv argues against, and the
# reason. Set by review, not computed; docs/tu-recovery.md carries the evidence.
ACTIONS = {
    "file/fileManager": ("move-end", "end 0x00297F68 cuts the group "
                         "0x00297990-0x00298050 that shares \"/font/name_ent.fnt\"; "
                         "end at 0x00297928, the first nme function"),
    "effect/effEvent": ("narrow-end", "0x001B0858-0x001B0EF8 is a dds3*Spline* run; "
                        "end at 0x001B0858"),
    "sdf/sdfMovie": ("move-start", "the movie* run starts at 0x002C5080; the applied "
                     "start is 5 functions into it"),
    "basic/dds3CameraObjectBasic": ("widen-start", "DDS1 labels 0x001123D0-0x001124F8 "
                                    "for this unit and HD names nothing there; start "
                                    "at 0x001123D0"),
    "basic/dds3ModelRangeBasic": ("defer", "one function; HD puts dds3ModelRangeBasic.c "
                                  "at 0x00115F68, not at this DDS1 start"),
    "misc/l2d": ("defer", "the DDS1 label lands inside the dds3FontReq* run "
                 "0x001178F8-0x00117B08, which looks like one object"),
    "sdf/sdfThread": ("defer", "the thread block starts at 0x002A8598; apply there or "
                      "not at all"),
}
# HD file names that are not the unit name but are not a disagreement either.
HD_FIT = {"event/evtEventPack": "near", "mc/mcManager": "disagree"}
NEW_COLUMNS = ["disposition", "applied_start", "applied_end", "support", "support_kinds",
               "hd_fit", "witnesses", "action", "action_note"]


def hd_runs(b, hd_path: Path):
    """Maximal runs of consecutive functions HD maps to one source file."""
    if not hd_path.exists():
        return []
    hd = fb.hd_sources(hd_path)
    source = [hd.get(name) for name in b.names]
    runs, start = [], None
    for i, src in enumerate(source + [None]):
        if start is not None and (src is None or src != source[start]):
            runs.append((start, i - 1, source[start]))
            start = None
        if src is not None and start is None:
            start = i
    return runs


def hd_view(runs, index):
    """(source, run start index, run end index) for the HD run at this index."""
    for lo, hi, src in runs:
        if lo <= index <= hi:
            return src, lo, hi
    return None, None, None


def score_at(rows_by_index, index, inside):
    """(verdict, strength, kinds) for a boundary at this function index."""
    if index in inside:
        return "contradicted", "-", []
    here = rows_by_index.get(index, [])
    if not here:
        return "silent", "-", []
    kinds = sorted({r["kind"] for r in here})
    hard = BINARY_ONLY & set(kinds)
    best = max(RANK[r["strength"]] for r in here if r["kind"] in hard) if hard else 0
    if not hard:
        # Nothing but the HD map and the names it recovered.
        return "echo", "prefix" if "prefix" in kinds else "hd-only", kinds
    weight = sum(fb.WEIGHTS[k] for k in hard)
    if best == 3 and weight >= 4:
        level = "strong"
    elif best >= 2 or weight >= 4:
        level = "medium"
    else:
        level = "weak"
    return "supported", level, kinds


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("version", choices=list(fb.VERSIONS))
    ap.add_argument("--hd-symbols", type=Path, default=fb.DEFAULT_HD)
    ap.add_argument("--top", type=int, default=30, help="unreached boundaries to rank")
    ap.add_argument("--json", type=Path, help="dump the per-unit table here")
    ap.add_argument("--write-candidates", action="store_true",
                    help="rewrite tu_candidates.tsv's verdict columns in place")
    args = ap.parse_args()

    cfg = ROOT / "config" / args.version
    b = fb.Binary(args.version)
    rows, groups, inside = fb.evidence(b, args.hd_symbols)
    rows_by_index = collections.defaultdict(list)
    for row in rows:
        if row["strength"] != "refuted":
            rows_by_index[row["index"]].append(row)

    applied = applied_units(cfg / f"{fb.VERSIONS[args.version]['serial']}.yaml")
    by_unit = {unit: vram for vram, unit in applied}
    _head, cand = candidates(cfg / "tu_candidates.tsv")
    cand_start = {r[2]: int(r[0], 16) for r in cand[1:]}

    runs = hd_runs(b, args.hd_symbols)
    nexts = {v: n for (v, _), (n, _) in zip(applied_units(
        cfg / f"{fb.VERSIONS[args.version]['serial']}.yaml", named_only=False)[:-1],
        applied_units(cfg / f"{fb.VERSIONS[args.version]['serial']}.yaml",
                      named_only=False)[1:])}

    table = []
    for vram, unit in applied:
        index = bisect.bisect_left(b.starts, vram)
        verdict, level, kinds = score_at(rows_by_index, index, inside)
        src, rlo, rhi = hd_view(runs, index)
        base = unit.split("/")[-1].lower().replace("_", "")
        hd_name = (src or "").lower().removesuffix(".c").replace("_", "")
        end_index = bisect.bisect_left(b.starts, nexts.get(vram, b.starts[-1] + 4)) - 1
        hd_fit = ("none" if src is None else
                  ("name+start+end" if hd_name == base and rlo == index and rhi == end_index
                   else "name+start" if hd_name == base and rlo == index
                   else "name" if hd_name == base
                   else "start" if rlo == index else "no"))
        near = ""
        if verdict == "silent":
            for d in range(1, NEAR + 1):
                for j in (index - d, index + d):
                    if j in rows_by_index and not near:
                        near = (f"{'-' if j < index else '+'}{d}:"
                                f"{','.join(sorted({r['kind'] for r in rows_by_index[j]}))}")
        table.append({"unit": unit, "applied": vram, "index": index,
                      "end": nexts.get(vram), "candidate": cand_start.get(unit),
                      "verdict": verdict, "support": level, "kinds": kinds, "near": near,
                      "hd_source": src, "hd_fit": hd_fit,
                      "detail": "; ".join(r["detail"] for r in rows_by_index.get(index, []))})

    # Every subsegment edge, filler units included, against the one hard rule:
    # a same-unit group may not be cut.
    edges = {bisect.bisect_left(b.starts, v)
             for v, _ in applied_units(cfg / f"{fb.VERSIONS[args.version]['serial']}.yaml",
                                       named_only=False)}
    cuts = [(lo, hi, i) for lo, hi in groups for i in sorted(edges) if lo < i <= hi]
    print(f"{len(edges)} code subsegment edges, {len(cuts)} of them cut a same-unit group")
    for lo, hi, i in cuts:
        print(f"  cut 0x{b.starts[i]:08X} inside group 0x{b.starts[lo]:08X}"
              f"-0x{b.starts[hi]:08X}")

    # A duplicate literal proves two objects, so each window must hold a boundary.
    gaps = []
    for lo, hi, value, first, second in fb.dup_string_windows(b):
        if not any(lo < i <= hi for i in edges):
            gaps.append((hi - lo, lo, hi, value, first, second))
    gaps.sort()
    print(f"{len(gaps)} duplicate-literal windows hold no subsegment edge")
    for width, lo, hi, value, first, second in gaps:
        print(f"  0x{b.starts[lo + 1]:08X}-0x{b.starts[hi]:08X} ({width} positions) "
              f"{value.decode('ascii', 'replace')!r} at 0x{first:08X}/0x{second:08X}")

    counts = collections.Counter(r["verdict"] for r in table)
    print(f"{len(applied)} applied units: " +
          ", ".join(f"{v} {k}" for k, v in counts.most_common()))
    print("  hd fit: " + ", ".join(
        f"{v} {k}" for k, v in collections.Counter(r["hd_fit"] for r in table).most_common()))
    for r in table:
        print(f"  0x{r['applied']:08X} {r['unit']:34} {r['verdict']:10} {r['support']:8} "
              f"{r['hd_fit']:15} {','.join(r['kinds']) or r['near']}")

    # Boundaries the applied set does not use. A row beside an applied start is the
    # same boundary, so those are dropped too.
    used = set()
    for r in table:
        used.update(range(r["index"] - NEAR, r["index"] + NEAR + 1))
    scored = []
    for index, here in rows_by_index.items():
        if index in used:
            continue
        kinds = sorted({r["kind"] for r in here})
        exact = [r for r in here if r["window"] == 1]
        scored.append((sum(fb.WEIGHTS[k] for k in kinds), len(exact), index, kinds, here))
    scored.sort(reverse=True)
    print(f"\ntop {args.top} evidenced boundaries no applied unit starts at:")
    for weight, _n, index, kinds, here in scored[:args.top]:
        best = max(here, key=lambda r: RANK[r["strength"]])
        print(f"  0x{b.starts[index]:08X} {b.names[index]:32} w{weight:2d} "
              f"{best['strength']:6} {','.join(kinds):34} {best['detail']}")

    if args.write_candidates:
        tsv = cfg / "tu_candidates.tsv"
        head, cand = candidates(tsv)
        base = len(cand[0]) - len(NEW_COLUMNS) if cand[0][-1] == NEW_COLUMNS[-1] \
            else len(cand[0])
        by_unit = {r["unit"]: r for r in table}
        cut_at = {i for _lo, _hi, i in cuts}
        out = [cand[0][:base] + NEW_COLUMNS]
        for row in cand[1:]:
            row, unit = row[:base], row[2]
            action, note = ACTIONS.get(unit, ("keep", ""))
            r = by_unit.get(unit)
            if r is None:
                out.append(row + ["not-applied", "-", "-", "-", "-", "-", "dds1",
                                  action, note])
                continue
            fit = HD_FIT.get(unit, r["hd_fit"])
            support = r["verdict"]
            if r["index"] in cut_at or bisect.bisect_left(b.starts, r["end"]) in cut_at:
                support = "contradicted"
            witnesses = ["dds1"]
            if fit in ("name+start", "name+start+end", "near"):
                witnesses.append("hd")
            if support == "supported":
                witnesses.append("binary")
            contained = (r["applied"] <= int(row[0], 16)
                         and r["end"] > int(row[1], 16))
            out.append(row + ["applied" if contained else "superseded",
                              f"0x{r['applied']:08X}", f"0x{r['end']:08X}", support,
                              ",".join(r["kinds"]) or "-", fit, "+".join(witnesses),
                              action, note])
        tsv.write_text("\n".join(head) + "\n"
                       + "".join("\t".join(r) + "\n" for r in out))
        print(f"wrote {tsv.relative_to(ROOT)}")

    if args.json:
        args.json.write_text(json.dumps(
            {"applied": table,
             "unreached": [{"addr": b.starts[i], "name": b.names[i], "weight": w,
                            "kinds": k,
                            "details": [r["detail"] for r in h],
                            "strengths": [r["strength"] for r in h]}
                           for w, _n, i, k, h in scored],
             "unapplied_candidates": [r[2] for r in cand[1:] if r[2] not in by_unit]},
            indent=1))


if __name__ == "__main__":
    main()
