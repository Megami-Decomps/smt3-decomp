#!/usr/bin/env python3
"""Collect translation-unit boundary evidence from SLUS_209.11 itself.

    python3 tools/find_boundaries.py smt3

Writes config/<v>/tu_evidence.tsv, whose comment header lists the rules. Reads
the split asm, the retail ELF and the HD per-function source map; not the IDB.
"""

from __future__ import annotations

import argparse
import bisect
import collections
import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
_VERSIONS_PATH = ROOT / "config" / "versions.json"
VERSIONS = json.loads(_VERSIONS_PATH.read_text()) if _VERSIONS_PATH.exists() else {}
DEFAULT_HD = Path("/home/wsl/projects/smt3hd-reconstructed-sources/symbols.txt")

GLABEL = re.compile(r"^glabel (\w+)")
INSN = re.compile(r"/\* [0-9A-F]+ ([0-9A-F]{8}) ([0-9A-F]{8}) \*/\s+(\S+)")
# A %lo/%gp_rel reference to a data symbol, with the pc of the instruction making it.
REF = re.compile(r"/\* [0-9A-F]+ ([0-9A-F]{8}) [0-9A-F]{8} \*/.*?"
                 r"%(?:lo|gp_rel)\(([A-Za-z_]\w*)(?:\s*\+\s*(0x[0-9A-F]+|\d+))?\)")
HEX_SUFFIX = re.compile(r"_([0-9A-F]{8})$")
SKIP_DIRS = {"data", "nonmatchings", "matchings", "sdk"}
SECTION_FILE = re.compile(r"\.(rodata|lit4|sdata|data|bss|sbss)\.s$")
LABEL = re.compile(r"^(?:dlabel|glabel|jlabel) (\w+)\n\s*/\* [0-9A-F]+ ([0-9A-F]{8})", re.M)
PREFIX = re.compile(r"^([a-z][a-z0-9]{2,5})(?=[A-Z_])")
# A prefix this rare is one name's spelling, not a module.
MIN_PREFIX_USE = 20
# Smallest function allowed to count as a duplicate: shorter ones collide by chance.
MIN_DUP_FUNC = 0x40
# Sections an object emits as one run, from the ELF section headers.
SECTIONS = {".rodata": (0x0052DC80, 0x0054C178), ".lit4": (0x0054C180, 0x0054E7D4),
            ".sdata": (0x0054E800, 0x00551B55)}
STRENGTHS = ["strong", "medium", "weak", "refuted"]
KINDS = ["dup-string", "core-edge", "hd-source", "prefix", "dup-func"]


class Binary:
    """Functions, data symbols and data references, read out of the split asm."""

    def __init__(self, version: str):
        serial = VERSIONS[version]["serial"]
        self.elf = (ROOT / "orig" / version / serial).read_bytes()
        self.delta = 0xFF000  # vram - file offset, constant across the image
        code = self._code_files(version)
        self.funcs = sorted(f for path in code for f in self._functions(path.read_text()))
        self.starts = [addr for addr, _ in self.funcs]
        self.names = [name for _, name in self.funcs]
        self.symbols = self._data_symbols(version)
        if not self.symbols or not self.funcs:
            raise SystemExit(f"asm/{version} holds no split asm to read")
        self.data = sorted(self.symbols)
        self.by_name = {name: addr for addr, name in sorted(self.symbols.items())}
        self.owners = collections.defaultdict(set)
        for path in code:
            self._references(path.read_text())

    @staticmethod
    def _code_files(version: str) -> list[Path]:
        """The unit asm, whatever the split's shape. nonmatchings/ and matchings/
        repeat the same code, and data/ holds no code."""
        asm = ROOT / "asm" / version
        return sorted(p for p in asm.rglob("*.s")
                      if not (SKIP_DIRS & set(p.relative_to(asm).parts))
                      and not SECTION_FILE.search(p.name))

    @staticmethod
    def _functions(text: str) -> list[tuple[int, str]]:
        out, pending = [], None
        for line in text.splitlines():
            if m := GLABEL.match(line):
                pending = m.group(1)
            elif pending and (m := INSN.search(line)):
                out.append((int(m.group(1), 16), pending))
                pending = None
        return out

    @staticmethod
    def _data_symbols(version: str) -> dict[int, str]:
        out = {}
        for path in sorted(ROOT.joinpath("asm", version).rglob("*.s")):
            if SECTION_FILE.search(path.name):
                for name, addr in LABEL.findall(path.read_text()):
                    out[int(addr, 16)] = name
        return out

    def _references(self, text: str) -> None:
        """Record, per data symbol, the functions whose code names it."""
        for m in REF.finditer(text):
            # D_0054E744 carries its address; a name from symbol_addrs.txt does not,
            # so those are resolved through the labels the data asm defines.
            hexed = HEX_SUFFIX.search(m.group(2))
            base = int(hexed.group(1), 16) if hexed else self.by_name.get(m.group(2))
            if base is None:
                continue
            target = base + (int(m.group(3), 0) if m.group(3) else 0)
            if self.section(target) is None:
                continue
            func = bisect.bisect_right(self.starts, int(m.group(1), 16)) - 1
            self.owners[self.data[bisect.bisect_right(self.data, target) - 1]].add(func)

    def section(self, addr: int) -> str | None:
        for name, (lo, hi) in SECTIONS.items():
            if lo <= addr < hi:
                return name
        return None

    def func_end(self, i: int) -> int:
        return self.starts[i + 1] if i + 1 < len(self.starts) else self.starts[i]

    def string(self, addr: int) -> bytes | None:
        """The .rodata literal at addr, or None if the bytes are not one.

        A literal ends before the next symbol and the rest of its slot is NUL
        padding; anything else is a table that happens to start with text."""
        if self.section(addr) != ".rodata":
            return None
        off = addr - self.delta
        end = self.elf.find(b"\0", off)
        if end < 0:
            return None
        i = bisect.bisect_right(self.data, addr)
        limit = (self.data[i] - self.delta) if i < len(self.data) else None
        if limit is not None and (end >= limit or any(self.elf[end:limit])):
            return None
        s = self.elf[off:end]
        if len(s) < 2 or not all(32 <= c < 127 or c in (9, 10, 13) for c in s):
            return None
        return s if any(chr(c).isalnum() for c in s) else None

    def kind_of(self, addr: int) -> str:
        if self.section(addr) == ".lit4":
            return "lit4"
        if self.symbols[addr].startswith("jtbl_"):
            return "jtbl"
        return "string" if self.string(addr) else "other"


def same_unit_groups(b: Binary) -> list[tuple[int, int]]:
    """Function intervals proven to be one unit, merged.

    Only literals, jump tables and .lit4 words count: a plain data symbol can be
    a global another object refers to, and with those in, one group swallows 9673
    of the 9694 functions."""
    intervals = []
    for addr, users in sorted(b.owners.items()):
        if len(users) > 1 and b.kind_of(addr) != "other":
            intervals.append((min(users), max(users)))
    for lo, hi in SECTIONS.values():
        highest = -1
        for addr in b.data:
            if not (lo <= addr < hi) or addr not in b.owners:
                continue
            if b.kind_of(addr) not in ("string", "jtbl"):
                continue
            users = b.owners[addr]
            if highest > min(users):
                intervals.append((min(users), highest))
            highest = max(highest, max(users))
    groups: list[list[int]] = []
    for lo, hi in sorted(intervals):
        if groups and lo <= groups[-1][1]:
            groups[-1][1] = max(groups[-1][1], hi)
        else:
            groups.append([lo, hi])
    return [(lo, hi) for lo, hi in groups if hi > lo]


def dup_string_windows(b: Binary):
    """(first owner, last owner, literal, address, address) per pair of copies.

    A copy no code reaches still lands between the owners of the referenced
    symbols around it, because the section follows text order."""
    by_value = collections.defaultdict(list)
    for addr in b.data:
        if s := b.string(addr):
            by_value[s].append(addr)
    owned = [(a, min(b.owners[a])) for a in b.data
             if a in b.owners and b.section(a) == ".rodata"]
    addrs = [a for a, _ in owned]

    def bracket(addr, after):
        i = bisect.bisect_left(addrs, addr) if after else bisect.bisect_right(addrs, addr) - 1
        return owned[i][1] if 0 <= i < len(owned) else None

    out = []
    for value, copies in sorted(by_value.items()):
        if len(copies) < 2:
            continue
        copies.sort()
        for first, second in zip(copies, copies[1:]):
            lo, hi = bracket(first, False), bracket(second, True)
            if lo is None or hi is None or hi <= lo:
                continue
            out.append((lo, hi, value, first, second))
    return out


def hd_sources(path: Path) -> dict[str, str]:
    """HD function name -> source basename, dropping names HD maps to two files."""
    out: dict[str, str | None] = {}
    for line in path.read_text().splitlines():
        parts = line.split()
        if line.startswith("#") or len(parts) < 3:
            continue
        name, source = parts[1], parts[2]
        if name in out and out[name] != source:
            out[name] = None
        else:
            out.setdefault(name, source)
    return {k: v for k, v in out.items() if v}


def run_length(values: list, i: int, step: int) -> int:
    """How many consecutive entries from i, moving by step, hold values[i]."""
    want, n = values[i], 0
    while 0 <= i < len(values) and values[i] == want:
        n += 1
        i += step
    return n


def duplicate_functions(b: Binary) -> list[tuple[int, int, int]]:
    """(first index, second index, size) for byte-identical functions."""
    seen = collections.defaultdict(list)
    for i in range(len(b.starts)):
        size = b.func_end(i) - b.starts[i]
        if size >= MIN_DUP_FUNC:
            seen[b.elf[b.starts[i] - b.delta:b.func_end(i) - b.delta]].append(i)
    out = []
    for code, group in sorted(seen.items()):
        for first, second in zip(sorted(group), sorted(group)[1:]):
            out.append((first, second, len(code)))
    return sorted(out)


def dds1_ranges(path: Path, starts: list[int]) -> list[tuple[int, int, str]]:
    """DDS1-derived candidate ranges as (first index, last index, unit)."""
    out = []
    for line in path.read_text().splitlines():
        parts = line.split("\t")
        if line.startswith("#") or line.startswith("start\t") or len(parts) < 3:
            continue
        lo, hi = int(parts[0], 16), int(parts[1], 16)
        out.append((bisect.bisect_left(starts, lo),
                    bisect.bisect_right(starts, hi) - 1, parts[2]))
    return sorted(out)


def evidence(b: Binary, hd_path: Path):
    """Every row, before the DDS1 comparison: (index, kind, strength, window, detail)."""
    groups = same_unit_groups(b)
    inside = set()
    for lo, hi in groups:
        inside.update(range(lo + 1, hi + 1))
    rows = []

    def add(index, kind, strength, window, detail):
        if 0 < index < len(b.starts):
            rows.append({"index": index, "kind": kind, "strength": strength,
                         "window": window, "detail": detail})

    for lo, hi, value, first, second in dup_string_windows(b):
        width = sum(1 for i in range(lo + 1, hi + 1) if i not in inside)
        add(hi, "dup-string", "strong" if width == 1 else "medium", width,
            f"{value.decode('ascii', 'replace')!r} at 0x{first:08X} and 0x{second:08X}; "
            f"boundary at or after 0x{b.starts[lo + 1]:08X}")
    for lo, hi in groups:
        size = hi - lo + 1
        add(lo, "core-edge", "medium" if size >= 4 else "weak", 1,
            f"first of a {size}-function same-unit group ending 0x{b.starts[hi]:08X}")
        add(hi + 1, "core-edge", "medium" if size >= 4 else "weak", 1,
            f"after a {size}-function same-unit group starting 0x{b.starts[lo]:08X}")

    if hd_path.exists():
        hd = hd_sources(hd_path)
        source = [hd.get(name) for name in b.names]
        for i in range(1, len(source)):
            if not (source[i] and source[i - 1]) or source[i] == source[i - 1]:
                continue
            before, after = run_length(source, i - 1, -1), run_length(source, i, 1)
            add(i, "hd-source", "medium" if min(before, after) >= 3 else "weak", 1,
                f"HD source {source[i - 1]} -> {source[i]}, {before} then {after} functions")

    prefix = [m.group(1) if (m := PREFIX.match(name)) else None for name in b.names]
    common = {p for p, n in collections.Counter(prefix).items() if p and n >= MIN_PREFIX_USE}
    prefix = [p if p in common else None for p in prefix]
    for i in range(1, len(prefix)):
        if not (prefix[i] and prefix[i - 1]) or prefix[i] == prefix[i - 1]:
            continue
        before, after = run_length(prefix, i - 1, -1), run_length(prefix, i, 1)
        if min(before, after) < 2:
            continue
        add(i, "prefix", "weak", 1,
            f"name prefix {prefix[i - 1]} -> {prefix[i]}, {before} then {after} functions")

    for first, second, size in duplicate_functions(b):
        width = sum(1 for i in range(first + 1, second + 1) if i not in inside)
        add(second, "dup-func", "weak", width,
            f"0x{size:X} bytes identical to 0x{b.starts[first]:08X} ({b.names[first]})")

    # A row inside a proven same-unit group cannot be a boundary, whatever produced it.
    for row in rows:
        if row["index"] in inside:
            row["strength"] = "refuted"
    # One kind is one witness, two kinds at one address are two, so each lifts the other.
    kinds_at = collections.defaultdict(set)
    for row in rows:
        if row["strength"] != "refuted":
            kinds_at[row["index"]].add(row["kind"])
    for row in rows:
        if (row["strength"] in ("medium", "weak") and row["window"] == 1
                and len(kinds_at[row["index"]]) > 1):
            row["strength"] = "strong" if row["strength"] == "medium" else "medium"
    rows.sort(key=lambda r: (r["index"], r["kind"]))
    return rows, groups, inside


def compare(rows, ranges):
    """Fill in each row's dds1 column; return the per-range verdicts."""
    starts_at = {lo: unit for lo, _, unit in ranges}
    window, held, previous = {}, {}, None
    for lo, hi, unit in ranges:
        earliest = max(lo - NEAR, 0 if previous is None else previous + 1)
        for i in range(earliest, lo):
            window[i] = unit
        for i in range(lo + 1, hi + 1):
            held[i] = unit
        previous = hi
    for row in rows:
        i = row["index"]
        if i in starts_at:
            row["dds1"] = "start"
        elif i in window:
            row["dds1"] = f"window:{window[i]}"
        elif i in held:
            row["dds1"] = f"inside:{held[i]}"
        else:
            row["dds1"] = "-"
    support = collections.defaultdict(collections.Counter)
    for row in rows:
        if row["strength"] == "refuted":
            continue
        if row["dds1"] == "start":
            support[starts_at[row["index"]]][row["strength"]] += 1
        elif row["dds1"].startswith("window:"):
            support[row["dds1"].split(":", 1)[1]][row["strength"]] += 1
    return support


# A range's first labelled function is a few functions into the file, because only
# a third of the functions paired; past this many the row is about something else.
NEAR = 8


# What a kind is worth when several land on one address: a duplicate literal is a
# rule, a prefix change is a hint.
WEIGHTS = {"dup-string": 4, "hd-source": 2, "core-edge": 2, "dup-func": 1, "prefix": 1}


def missed(rows, count):
    """The best-attested addresses no DDS1 range starts at or just after."""
    kinds = collections.defaultdict(collections.Counter)
    for row in rows:
        if row["strength"] == "refuted" or row["dds1"].startswith(("start", "window")):
            continue
        kinds[row["index"]][row["kind"]] += 1
    # Distinct kinds, not repeated rows: ten copies of one literal are still one rule.
    score = {i: sum(WEIGHTS[k] for k in seen) for i, seen in kinds.items()}
    best = sorted(((-score[i], -sum(kinds[i].values()), i) for i in score))[:count]
    return [(-v, i, set(kinds[i])) for v, _, i in best]


PREAMBLE = [
    "Translation-unit boundary evidence found inside SLUS_209.11 itself.",
    "NOT a splat config. Produced by tools/find_boundaries.py from the split asm,",
    "the retail ELF and the HD reconstruction's per-function source map.",
    "config/smt3/tu_candidates.tsv is an independent source: it carries DDS1 unit",
    "names over byte-identical function pairs. Only the dds1 column here looks at",
    "it, so a row agreeing with it has two witnesses rather than one.",
    "",
    "The rule the rest leans on: ee-gcc emits a string literal, a jump table and a",
    ".lit4 word into the object that uses it, and the linker lays each object's",
    "data out in text order. Two functions naming the same such symbol are one",
    "unit, and so are two whose references invert that address order, with",
    "everything between them. A boundary can lie at such a group's edge, never",
    "inside one, so a row inside a group reads as refuted.",
    "",
    "Rules tried that produce no rows, because they are at chance against the DDS1",
    "range starts:",
    "  .rodata/.lit4/.sdata ownership runs. Data is contiguous across objects too,",
    "  so the function where ownership moves on says nothing: 918/541/292 such",
    "  positions, 6/5/0 on a range start, 5.9/3.5/1.9 expected by chance.",
    "  Duplicate .lit4 words. ee-as does not merge equal constants inside one",
    "  object here: 224 values appear twice under a single function.",
    "  Call locality. Call edges shorter than 10, 20 or 40 functions cross a range",
    "  start as often as any other position.",
    "  tools/find_nosibcall.py finds no run without a sibling call and",
    "  tools/find_fragments.py no false function start, so neither bounds anything.",
]
COLUMNS = [
    ("addr", "candidate boundary vram: the function that would start the next unit"),
    ("kind", " / ".join(KINDS)),
    ("strength", "strong / medium / weak, or refuted: inside a proven same-unit group"),
    ("window", "positions the boundary could take, the last of them addr (1 = exact)"),
    ("detail", "what was observed"),
    ("dds1", f"start: a tu_candidates.tsv range starts here. window:<unit>: up to"
             f" {NEAR} functions before that range's start, where its real start can"
             " still be. inside:<unit>: contradicts that range. -: no range nearby"),
]


def header() -> str:
    lines = [f"# {line}" for line in PREAMBLE]
    lines += ["#", "# Regenerate: python3 tools/find_boundaries.py smt3",
              "#", "# Columns, tab separated:"]
    width = max(len(name) for name, _ in COLUMNS)
    lines += [f"#   {name:<{width}}  {what}" for name, what in COLUMNS]
    return "\n".join(lines) + "\n"


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("version", choices=list(VERSIONS))
    ap.add_argument("--hd-symbols", type=Path, default=DEFAULT_HD,
                    help="smt3hd-reconstructed-sources symbols.txt")
    ap.add_argument("--dds1", type=Path, help="tu_candidates.tsv (default: config/<v>/)")
    ap.add_argument("--output", type=Path, help="default: config/<v>/tu_evidence.tsv")
    ap.add_argument("--dry-run", action="store_true", help="report counts, write nothing")
    args = ap.parse_args()
    dds1 = args.dds1 or ROOT / "config" / args.version / "tu_candidates.tsv"
    output = args.output or ROOT / "config" / args.version / "tu_evidence.tsv"

    b = Binary(args.version)
    rows, groups, inside = evidence(b, args.hd_symbols)
    ranges = dds1_ranges(dds1, b.starts) if dds1.exists() else []
    support = compare(rows, ranges)

    print(f"{len(b.starts)} functions, {len(b.symbols)} data symbols, "
          f"{len(b.owners)} of them named by code")
    print(f"{len(groups)} same-unit groups over {sum(h - l + 1 for l, h in groups)} "
          f"functions; {len(inside)} positions no boundary can take")
    for kind in KINDS:
        counts = collections.Counter(r["strength"] for r in rows if r["kind"] == kind)
        print(f"  {kind:11} {sum(counts.values()):5d}  " +
              " ".join(f"{s} {counts[s]}" for s in STRENGTHS if counts[s]))
    kept = [r for r in rows if r["strength"] != "refuted"]
    print(f"{len(kept)} rows at {len({r['index'] for r in kept})} addresses, "
          f"{sum(1 for r in rows if r['strength'] == 'refuted')} refuted")
    for level in ("strong", "medium", "weak"):
        at = [r for r in kept if r["strength"] == level]
        verdict = collections.Counter(r["dds1"].split(":")[0] for r in at)
        print(f"  {level:6} {len(at):5d} rows: {verdict['start']} on a DDS1 range start, "
              f"{verdict['window']} in a range's start window, {verdict['inside']} inside "
              f"a range, {verdict['-']} elsewhere")
    for level in ("strong", "medium", "weak"):
        blind = [u for _, _, u in ranges
                 if not any(s in support[u] for s in STRENGTHS[:STRENGTHS.index(level) + 1])]
        print(f"{len(blind)} of {len(ranges)} DDS1 ranges have no {level} or better row at "
              f"or before their start")
    print("strongest boundaries tu_candidates.tsv does not have:")
    for score, index, kinds in missed(rows, 10):
        print(f"  0x{b.starts[index]:08X} {b.names[index]:34} score {score:2d}  "
              f"{', '.join(sorted(kinds))}")
    if args.dry_run:
        return
    out = [header(), "\t".join(name for name, _ in COLUMNS) + "\n"]
    out += ["\t".join([f"0x{b.starts[r['index']]:08X}", r["kind"], r["strength"],
                       str(r["window"]), r["detail"], r["dds1"]]) + "\n" for r in rows]
    output.write_text("".join(out))
    print(f"wrote {output.relative_to(ROOT)}")


if __name__ == "__main__":
    main()
