#!/usr/bin/env python3
"""Carry dds-decomp's DDS1 unit names onto SLUS_209.11 through paired functions.

Pairs both IDBs' functions on size and masked code, carries each paired DDS1 function's
unit name from dds-decomp's config/dds1/SLUS_209.74.yaml over, writes tu_candidates.tsv."""

import argparse
import base64
import json
import os
import re
import sys
import zlib
from bisect import bisect_left, bisect_right
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
DEFAULT_OUTPUT = ROOT / "config" / "smt3" / "tu_candidates.tsv"
DEFAULT_HD = Path("/home/wsl/projects/smt3hd-reconstructed-sources/symbols.txt")
DEFAULT_SMT3_INSTANCE = "13339"
DEFAULT_DDS1_INSTANCE = "13340"
IDA_SCRIPTS = Path(
    os.environ.get("IDA_SKILL_SCRIPTS", Path.home() / ".claude/skills/ida-instances/scripts")
)

# dds-decomp links DDS1's main segment at this delta from the file offsets its yaml uses.
DDS1_VRAM_DELTA = 0xFF000
# Trivial stubs match by chance, so dds-decomp's projection kept a 16-byte floor.
MIN_SIZE = 16
# Units dds-decomp itself marked as likely mispairs in its own docs/tu-names.md.
DROPPED_UNITS = {
    "newbattle/nbSound",
    "newbattle/nbMisc",
    "newbattle/nbAiScript",
    "effect/effBattleSystem",
}
# Placeholder units, not real file names.
PLACEHOLDER_UNIT = re.compile(r"^game/code_[0-9A-Fa-f]+$")

YAML_SUBSEGMENT = re.compile(r"^\s*-\s*\[\s*(0x[0-9A-Fa-f]+)\s*,\s*c\s*,\s*([^\]\s]+)\s*\]")

IDA_PAYLOAD = r'''
def export():
    # py_eval execs this with split globals and locals, so the imports have to live
    # inside the function for its body to see them.
    import base64, hashlib, json, struct, zlib
    import ida_bytes, ida_funcs, idautils, idc

    # Opcodes whose low 16 bits can hold a relocated value (lui, addiu, ori, loads,
    # stores, lq/sq, ...). Masking them lets two differently linked builds compare equal.
    IMM_OPS = frozenset((0x0F, 0x09, 0x0C, 0x0D, 0x19, 0x1E, 0x1F, 0x20, 0x21, 0x23,
                         0x24, 0x25, 0x27, 0x28, 0x29, 0x2B, 0x2C, 0x2D, 0x31, 0x34,
                         0x35, 0x37, 0x39, 0x3C, 0x3D, 0x3F))

    rows = []
    for ea in sorted(idautils.Functions()):
        func = ida_funcs.get_func(ea)
        if func is None or len(list(idautils.Chunks(ea))) != 1:
            # A split function has no single extent to hash.
            continue
        size = func.end_ea - ea
        if size % 4:
            continue
        code = ida_bytes.get_bytes(ea, size)
        if code is None or len(code) != size:
            continue
        words = bytearray()
        for i in range(0, size, 4):
            w = struct.unpack_from("<I", code, i)[0]
            op = w >> 26
            if op in (2, 3):
                w &= 0xFC000000          # j / jal: target is a link-time address
            elif op in IMM_OPS:
                w &= 0xFFFF0000          # keep opcode and registers, drop the immediate
            words += struct.pack("<I", w)
        digest = hashlib.sha1(bytes(words)).hexdigest()[:16]
        rows.append([ea, size, digest, idc.get_func_name(ea)])

    blob = json.dumps(rows, separators=(",", ":"))
    return base64.b64encode(zlib.compress(blob.encode(), 9)).decode()


FUNC_BLOB = export()
len(FUNC_BLOB)
'''

# The MCP transport truncates a reply above roughly 20 KB, so the blob is read in pages.
PAGE = 16384


def eval_in_ida(ida, port, code, new_locals):
    out = ida.call_tool(port, "py_eval", {"code": code, "new_locals": new_locals})
    if out.get("stderr"):
        raise SystemExit(f"IDAPython failed on port {port}:\n{out['stderr']}")
    return out.get("result")


def collect(instance, ports, cache):
    """Every single-chunk function in one IDB as [addr, size, masked hash, name]."""
    if cache and cache.exists():
        return json.loads(cache.read_text())
    sys.path.insert(0, str(IDA_SCRIPTS))
    import ida

    instances = ida.get_instances(ida.parse_ports(ports), refresh=False)
    port = ida.resolve_target(instance, instances)
    total = int(eval_in_ida(ida, port, IDA_PAYLOAD, True))
    pages = []
    for start in range(0, total, PAGE):
        page = eval_in_ida(ida, port, f"FUNC_BLOB[{start}:{start + PAGE}]", False)
        if len(page) != min(PAGE, total - start):
            raise SystemExit(f"short page at {start}: got {len(page)} characters")
        pages.append(page)
    rows = json.loads(zlib.decompress(base64.b64decode("".join(pages))))
    if cache:
        cache.parent.mkdir(parents=True, exist_ok=True)
        cache.write_text(json.dumps(rows))
    return rows


def pair(smt3, dds1, min_size=MIN_SIZE):
    """One-to-one pairing on (size, masked hash), monotone in address order.

    Keys unique on both sides anchor it, cut to their longest increasing run; a shared
    key pairs only when counts match and each pair sits in the same anchor gap."""
    def index(rows):
        by_key = defaultdict(list)
        for addr, size, digest, _name in rows:
            if size >= min_size:
                by_key[(size, digest)].append(addr)
        return by_key

    a_keys, b_keys = index(smt3), index(dds1)
    shared = sorted(set(a_keys) & set(b_keys))

    raw = sorted(
        (a_keys[k][0], b_keys[k][0]) for k in shared if len(a_keys[k]) == 1 == len(b_keys[k])
    )
    anchors = longest_increasing(raw)
    a_anchor = [a for a, _ in anchors]
    b_anchor = [b for _, b in anchors]

    def slot(addr, anchor):
        return bisect_left(anchor, addr)

    pairs = dict(anchors)
    ambiguous = 0
    for key in shared:
        xs, ys = sorted(a_keys[key]), sorted(b_keys[key])
        if len(xs) == 1 == len(ys):
            continue
        if len(xs) != len(ys):
            ambiguous += len(xs)
            continue
        if any(slot(x, a_anchor) != slot(y, b_anchor) for x, y in zip(xs, ys)):
            ambiguous += len(xs)
            continue
        for x, y in zip(xs, ys):
            if x not in pairs:
                pairs[x] = y
    return pairs, dict(raw), len(anchors), ambiguous


def longest_increasing(pairs):
    """Keep the longest run of pairs rising on both sides (patience sort on the b side)."""
    tails, tail_at, prev = [], [], [-1] * len(pairs)
    for i, (_a, b) in enumerate(pairs):
        j = bisect_right(tails, b)
        if j == len(tails):
            tails.append(b)
            tail_at.append(i)
        else:
            tails[j] = b
            tail_at[j] = i
        prev[i] = tail_at[j - 1] if j else -1
    out = []
    i = tail_at[-1] if tail_at else -1
    while i >= 0:
        out.append(pairs[i])
        i = prev[i]
    return out[::-1]


def read_units(yaml_path):
    """dds-decomp's `c` subsegments as sorted (dds1 vram, unit name).

    Placeholder and dropped names stay in so a function inside one reads as unlabelled
    rather than missing; `usable` filters them."""
    rows = []
    for line in Path(yaml_path).read_text().splitlines():
        m = YAML_SUBSEGMENT.match(line)
        if m:
            rows.append((int(m.group(1), 16) + DDS1_VRAM_DELTA, m.group(2)))
    if not rows:
        raise SystemExit(f"no `c` subsegments in {yaml_path}")
    rows.sort()
    return rows


def unit_of(units, starts, addr):
    i = bisect_right(starts, addr) - 1
    return units[i][1] if i >= 0 else None


def usable(name):
    return name is not None and name not in DROPPED_UNITS and not PLACEHOLDER_UNIT.match(name)


def candidates(labels, unordered, smt3_addrs):
    """Labelled SMT3 functions -> per-unit ranges, counting contradictions.

    A span runs from a unit's lowest to its highest labelled function. Contradictions
    come from `unordered`: a monotone pairing cannot contradict a span by construction."""
    spans = defaultdict(list)
    for addr, name in labels.items():
        spans[name].append(addr)
    rows = []
    for name, addrs in spans.items():
        addrs.sort()
        lo, hi = addrs[0], addrs[-1]
        inside = smt3_addrs[bisect_left(smt3_addrs, lo):bisect_right(smt3_addrs, hi)]
        contradict = sorted(
            a for a in inside if unordered.get(a) not in (None, name)
        )
        rows.append({
            "unit": name,
            "start": lo,
            "end": hi,
            "labelled": len(addrs),
            "functions": len(inside),
            "contradictions": len(contradict),
            "first_contradiction": contradict[0] if contradict else None,
            "contradicting_units": sorted({unordered[a] for a in contradict}),
        })
    rows.sort(key=lambda r: r["start"])
    return rows


def read_hd_sources(path):
    """HD symbol name -> source basename, dropping names HD maps to several files."""
    out = {}
    for line in Path(path).read_text().splitlines():
        if line.startswith("#") or not line.strip():
            continue
        parts = line.split()
        if len(parts) < 3:
            continue
        name, source = parts[1], parts[2]
        if name in out and out[name] != source:
            out[name] = None
        else:
            out.setdefault(name, source)
    return {k: v for k, v in out.items() if v}


def normalize(name):
    """HD and Nocturne spell the same file sdf_PacDecode.c / sdfPacDecode, file_Manager.c
    / fileManager, so separators and case carry no information."""
    return re.sub(r"[^a-z0-9]", "", name.lower().removesuffix(".c"))


def same_file(unit, source):
    a, b = normalize(unit.rsplit("/", 1)[-1]), normalize(source)
    if a == b:
        return "agree"
    # HD sometimes drops the module prefix: EventPack.c for evtEventPack.
    if len(b) >= 4 and (a.endswith(b) or b.endswith(a)):
        return "near"
    return ""


def hd_verdict(unit, names, hd):
    """Compare a candidate's SMT3 function names against the HD source names they carry."""
    seen = defaultdict(int)
    for name in names:
        source = hd.get(name)
        if source:
            seen[source] += 1
    if not seen:
        return "no-hd", "", 0, 0, 0
    total = sum(seen.values())
    top = max(seen, key=lambda k: (seen[k], k))
    hits = {verdict: sum(n for s, n in seen.items() if same_file(unit, s) == verdict)
            for verdict in ("agree", "near")}
    agree = hits["agree"] + hits["near"]
    if not agree:
        return "disagree", top, 0, total, len(seen)
    verdict = "near" if hits["agree"] == 0 else "agree"
    return (verdict if agree == total else "mixed"), top, agree, total, len(seen)


PREAMBLE = [
    "Candidate SMT3 (SLUS_209.11) translation-unit ranges. NOT a splat config.",
    "Produced by tools/pair_dds1.py: SLUS_209.11 functions paired one-to-one against",
    "byte-identical SLUS_209.74 (DDS1) functions with relocations and jump targets",
    "masked, then dds-decomp's DDS1 unit names carried back along those pairs. The",
    "names are dds-decomp's, projected there from a Nocturne Dec 26 2002 debug build's",
    "__FILE__ strings. Method and limits: docs/tu-recovery.md.",
]
# Column name -> what the column holds, rendered into the output's comment header.
COLUMNS = [
    ("start", "first SMT3 vram labelled for this unit"),
    ("end", "last SMT3 vram labelled for this unit (a function start, not its end)"),
    ("unit", "dds-decomp unit name (directory/file, no extension)"),
    ("labelled", "SMT3 functions in [start, end] carrying this label"),
    ("functions", "all SMT3 functions in [start, end]"),
    ("contradictions", "functions in that span labelled for a different unit"),
    ("status", "candidate (no contradictions) or rejected"),
    ("hd", "agree / near / mixed / disagree / no-hd, from SMT3 HD source names"),
    ("hd_source", "the HD source file most of the span's functions came from"),
    ("hd_agree", "of the span's HD-known functions, how many name this unit"),
    ("hd_known", "how many of the span's functions have an HD source name"),
    ("hd_files", "distinct HD source files in the span; above 1 the range is too wide"),
    ("note", "first contradicting address and the units it belongs to"),
]
FIELDS = [name for name, _ in COLUMNS]


def header():
    lines = [f"# {line}" for line in PREAMBLE] + ["#", "# Columns, tab separated:"]
    width = max(len(name) for name in FIELDS)
    lines += [f"#   {name:<{width}}  {what}" for name, what in COLUMNS]
    return "\n".join(lines) + "\n"



def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--instance", default=DEFAULT_SMT3_INSTANCE,
                    help="SLUS_209.11 IDA instance (port or name)")
    ap.add_argument("--dds1-instance", default=DEFAULT_DDS1_INSTANCE,
                    help="SLUS_209.74 IDA instance (port or name)")
    ap.add_argument("--dds-yaml", type=Path,
                    default=Path(os.environ.get("DDS_DECOMP", "../dds-decomp"))
                    / "config/dds1/SLUS_209.74.yaml",
                    help="dds-decomp config/dds1/SLUS_209.74.yaml ($DDS_DECOMP)")
    ap.add_argument("--hd-symbols", type=Path, default=DEFAULT_HD,
                    help="smt3hd-reconstructed-sources symbols.txt")
    ap.add_argument("--min-size", type=int, default=MIN_SIZE,
                    help=f"smallest function allowed to pair (default {MIN_SIZE})")
    ap.add_argument("--cache-dir", type=Path, help="reuse per-IDB function dumps from here")
    ap.add_argument("--output", type=Path, default=DEFAULT_OUTPUT)
    ap.add_argument("--dry-run", action="store_true", help="report counts, write nothing")
    args = ap.parse_args()

    def cache(stem):
        return args.cache_dir / f"{stem}.json" if args.cache_dir else None

    smt3 = collect(args.instance, os.environ.get("IDA_MCP_PORTS", "13337-13437"), cache("smt3"))
    dds1 = collect(args.dds1_instance, os.environ.get("IDA_MCP_PORTS", "13337-13437"),
                   cache("dds1"))
    print(f"SLUS_209.11: {len(smt3)} functions   SLUS_209.74: {len(dds1)} functions")

    pairs, raw, anchors, ambiguous = pair(smt3, dds1, args.min_size)
    print(f"paired {len(pairs)}: {anchors} of {len(raw)} unique-hash anchors kept by the"
          f" monotone cut, {ambiguous} functions dropped as ambiguous")

    units = read_units(args.dds_yaml)
    starts = [a for a, _ in units]

    def label(mapping):
        out = {}
        for smt3_addr, dds1_addr in mapping.items():
            name = unit_of(units, starts, dds1_addr)
            if usable(name):
                out[smt3_addr] = name
        return out

    labels, unordered = label(pairs), label(raw)
    print(f"{len(labels)} labels transferred over {len(set(labels.values()))} units"
          f" ({sum(1 for _, n in units if usable(n))} named units in the dds-decomp yaml)")

    smt3_names = {addr: name for addr, _size, _digest, name in smt3}
    smt3_addrs = sorted(smt3_names)
    hd = read_hd_sources(args.hd_symbols) if args.hd_symbols.exists() else {}

    rows = candidates(labels, unordered, smt3_addrs)
    out = []
    for row in rows:
        span = smt3_addrs[bisect_left(smt3_addrs, row["start"]):
                          bisect_right(smt3_addrs, row["end"])]
        verdict, source, agree, known, files = hd_verdict(
            row["unit"], [smt3_names[a] for a in span], hd)
        note = ""
        if row["contradictions"]:
            note = (f"first at 0x{row['first_contradiction']:08X}; "
                    f"{','.join(row['contradicting_units'])}")
        out.append({
            "start": f"0x{row['start']:08X}",
            "end": f"0x{row['end']:08X}",
            "unit": row["unit"],
            "labelled": row["labelled"],
            "functions": row["functions"],
            "contradictions": row["contradictions"],
            "status": "rejected" if row["contradictions"] else "candidate",
            "hd": verdict,
            "hd_source": source,
            "hd_agree": agree,
            "hd_known": known,
            "hd_files": files,
            "note": note,
        })

    kept = [r for r in out if r["status"] == "candidate"]
    print(f"{len(kept)} candidate ranges, {len(out) - len(kept)} rejected for contradictions")
    for verdict in ("agree", "near", "mixed", "disagree", "no-hd"):
        count = sum(1 for r in kept if r["hd"] == verdict)
        if count:
            print(f"  {verdict}: {count}")
    if args.dry_run:
        return
    args.output.parent.mkdir(parents=True, exist_ok=True)
    body = "\n".join("\t".join(str(r[c]) for c in FIELDS) for r in out)
    args.output.write_text(header() + "\t".join(FIELDS) + "\n" + body + "\n")
    print(f"wrote {args.output}")


if __name__ == "__main__":
    main()
