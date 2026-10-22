#!/usr/bin/env python3
"""Turn SMT3-to-DDS function pairings into name tables dds-decomp can apply.

Reads three tools/pair_build.py runs (SMT3 to DDS1, SMT3 to DDS2, DDS1 to DDS2)
with their caches and writes names_<v>.tsv for dds-decomp's tools/names.py plus
smt3_pairs_<v>.tsv, one row per candidate with its status. Rules: docs/dds-names.md."""

from __future__ import annotations

import argparse
import hashlib
import json
import re
import struct
import sys
from collections import Counter, defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
TEXT_START = 0x00100000
# vram minus file offset, the same in all three retail ELFs.
DELTA = 0xFF000
GP = 28
# Immediate users that complete a lui or $gp into an address (%lo and %gp_rel).
LO_OPS = frozenset((0x09, 0x19, 0x1E, 0x1F, 0x20, 0x21, 0x23, 0x24, 0x25, 0x27, 0x28,
                    0x29, 0x2B, 0x31, 0x35, 0x37, 0x39, 0x3D, 0x3F))
# Below this a body is too short to be unique: 391 SMT3 functions share one 8-byte body.
MIN_SIZE = 16

PLACEHOLDER = re.compile(
    r"^(sub|nullsub|func|loc|locret|def|unk|j_sub|jpt|code|D)_[0-9A-Za-z]+$")
# HD names that are only a module tag plus an address: fcl_213998, dds3_2263E8,
# moviePlayFile_27041744. Not names, and they break the no-numbers convention.
ADDRESS_TAG = re.compile(r"_[0-9A-Fa-f]{5,}$")
IDENT = re.compile(r"^[A-Za-z_]\w*$")
# Compiler markers the HD recovery turned into function names, and IDA's suffix
# for a repeated name (kCopy_1, ctime_4).
JUNK = re.compile(r"^gcc2_compiled|_[0-9]+$")
# dds-decomp names with no capital are compiler runtime and libc routines (dpcmp,
# dptofp, memcpy) that the linker resolves by name; the SDK named them, not us.
RUNTIME = re.compile(r"^_|^[a-z0-9_]+$")
ROW = re.compile(r"^\s*(\S+)\s*=\s*0x([0-9A-Fa-f]+)\s*;(.*)$")


def reloc_hash(code):
    """Hash with only link-time fields zeroed: j/jal targets, lui halves, and the
    immediate of an instruction that completes a lui or addresses off $gp.

    pair_build.py masks every immediate, which makes the members of a function
    family (bodies differing in an allocation size or a constant) collide. Keeping
    the constants that are not addresses tells most of them apart."""
    words = bytearray()
    live = set()
    for i in range(0, len(code) - len(code) % 4, 4):
        w = struct.unpack_from("<I", code, i)[0]
        op, rs, rt = w >> 26, (w >> 21) & 31, (w >> 16) & 31
        if op in (2, 3):
            w &= 0xFC000000
        elif op == 0x0F:
            w &= 0xFFFF0000
            live.add(rt)
        elif op in LO_OPS and (rs == GP or rs in live):
            w &= 0xFFFF0000
            if rs in live and op in (0x09, 0x19):
                live.discard(rs)
        elif op == 0x0D and rs in live:
            w &= 0xFFFF0000
            live.discard(rs)
        words += struct.pack("<I", w)
    return hashlib.sha1(bytes(words)).hexdigest()[:16]


def read_pairs(path):
    """pair_build.py output as {from_addr: (to_addr, tier, evidence)}."""
    out = {}
    for line in Path(path).read_text().splitlines():
        if not line or line.startswith("#") or line.startswith("kind\t"):
            continue
        kind, a, _an, _asz, b, _bsz, _d, tier, ev = line.split("\t")
        if kind.startswith("pair"):
            out[int(a, 16)] = (int(b, 16), tier, ev)
    return out


def read_cache(cache_dir, from_elf, to_elf):
    """pair_build.py's cached inventories: {addr: (size, masked, raw, name, reloc)}."""
    def load(name, elf):
        image = Path(elf).read_bytes()
        out = {}
        for addr, size, _insns, masked, raw, fname in json.loads((Path(cache_dir) / name).read_text()):
            code = image[addr - DELTA:addr - DELTA + size] if masked else b""
            out[addr] = (size, masked, raw, fname, reloc_hash(code) if code else "")
        return out
    return load("from.json", from_elf), load("to.json", to_elf)


FIELD = {"masked": 1, "raw": 2, "reloc": 4}


def unique_index(side, field):
    """{hash: addr} for hashes carried by exactly one function of at least MIN_SIZE."""
    by = defaultdict(list)
    for addr, row in side.items():
        h = row[FIELD[field]]
        if h and row[0] >= MIN_SIZE:
            by[h].append(addr)
    return {h: addrs[0] for h, addrs in by.items() if len(addrs) == 1}, by


def read_symbols(path):
    rows = {}
    for line in Path(path).read_text().splitlines():
        if m := ROW.match(line):
            rows.setdefault(int(m[2], 16), m[1])
    return rows


def read_excludes(path):
    """dds-decomp's config/smt3_names_exclude.txt: {version: {addr: reason}}."""
    out = defaultdict(dict)
    if path.exists():
        for line in path.read_text().splitlines():
            body, _, reason = line.partition("#")
            if body.split():
                version, addr = body.split()[:2]
                out[version][int(addr, 16)] = reason.strip()
    return out


def read_sources(path):
    kinds = {}
    if path.exists():
        for line in path.read_text().splitlines():
            if line.strip() and not line.startswith("#"):
                addr, _name, kind, *_ = line.split("\t")
                kinds[int(addr, 16)] = kind
    return kinds


class Game:
    def __init__(self, version, pairs_path, cache_dir, smt3_elf, dds_elf, dds_decomp):
        self.version = version
        self.excluded = read_excludes(dds_decomp / "config" / "smt3_names_exclude.txt").get(version, {})
        self.prefixes = set()
        self.pairs = read_pairs(pairs_path)
        self.smt3, self.dds = read_cache(cache_dir, smt3_elf, dds_elf)
        self.symbols = read_symbols(dds_decomp / "config" / version / "symbol_addrs.txt")
        self.by_name = {}
        for addr, name in self.symbols.items():
            self.by_name.setdefault(name, addr)
        self.kinds = read_sources(dds_decomp / "config" / version / "name_sources.txt")
        self.rows = []       # (dds_addr, dds_name, smt3_addr, smt3_name, status, evidence)
        self.accepted = {}   # dds_addr -> (smt3_addr, smt3_name, evidence)

    def in_batch(self, b):
        """True when this address belongs to the batch being generated."""
        if not self.prefixes:
            return True
        name = self.symbols.get(b) or ""
        m = re.match(r"^_*([A-Za-z][a-z0-9]*)", name)
        return (m.group(1) if m else "") in self.prefixes

    def candidates(self, smt3_text_end):
        """Every SMT3 function with a usable pair, after the hash rules."""
        _, masked_all = unique_index(self.smt3, "masked")
        raw_u, _ = unique_index(self.smt3, "raw")
        reloc_u, _ = unique_index(self.smt3, "reloc")
        _, dds_masked_all = unique_index(self.dds, "masked")
        dds_raw_u, _ = unique_index(self.dds, "raw")
        dds_reloc_u, _ = unique_index(self.dds, "reloc")
        forced = {a: dds_raw_u[h] for h, a in raw_u.items() if h in dds_raw_u}
        proven = {a: dds_reloc_u[h] for h, a in reloc_u.items() if h in dds_reloc_u}
        out = {}
        for a, (size, masked, raw, name, reloc) in sorted(self.smt3.items()):
            if not TEXT_START <= a < smt3_text_end:
                continue
            if a in forced:
                out[a] = (forced[a], "byte-identical")
                continue
            if a in proven:
                out[a] = (proven[a], "reloc-identical")
                continue
            pair = self.pairs.get(a)
            if pair is None:
                continue
            b, tier, ev = pair
            if tier != "high":
                out[a] = (b, f"reject:tier-{tier}")
                continue
            if len(masked_all.get(masked, ())) != 1 or len(dds_masked_all.get(masked, ())) != 1:
                out[a] = (b, "shared-hash")
                continue
            out[a] = (b, ev)
        # A shared-hash pair is kept only inside a run: the proven pair before or after
        # it in SMT3 address order has the same delta, so its unit was linked as one.
        keys = sorted(out)
        delta = {a: out[a][0] - a for a in keys if not out[a][1].startswith("reject:")}
        strong = {a for a in delta if out[a][1] != "shared-hash"}
        for i, a in enumerate(keys):
            if out[a][1] != "shared-hash":
                continue
            prev = next((keys[j] for j in range(i - 1, -1, -1) if keys[j] in strong), None)
            nxt = next((keys[j] for j in range(i + 1, len(keys)) if keys[j] in strong), None)
            if (prev is not None and delta[prev] == delta[a]) or (nxt is not None and delta[nxt] == delta[a]):
                out[a] = (out[a][0], "shared-hash-in-run")
            else:
                out[a] = (out[a][0], "reject:shared-hash-off-run")
        return out

    def decide(self, smt3_text_end, dropped):
        taken = {}
        for a, (b, ev) in self.candidates(smt3_text_end).items():
            smt3_name = self.smt3[a][3]
            # The repo is the target; the DDS IDB may already carry other names.
            dds_name = self.symbols.get(b) or (self.dds[b][3] if b in self.dds else "?")
            status = "accept"
            if ev.startswith("reject:"):
                status = ev
            elif dds_name == smt3_name:
                status = "same"
            elif a in dropped:
                status = "reject:transitivity"
            elif PLACEHOLDER.match(smt3_name):
                status = "reject:smt3-placeholder"
            elif ADDRESS_TAG.search(smt3_name) or not IDENT.match(smt3_name) or JUNK.match(smt3_name):
                status = "reject:address-tag"
            elif RUNTIME.match(dds_name) and not PLACEHOLDER.match(dds_name):
                status = "reject:dds-runtime-name"
            elif self.kinds.get(b) == "evidence":
                status = "reject:dds-evidence"
            elif b not in self.symbols:
                status = "reject:not-in-dds-symbols"
            elif b in self.excluded:
                status = "reject:excluded"
            elif not self.in_batch(b):
                status = "skip:other-batch"
            elif b in taken:
                status = "reject:two-smt3-functions"
            if status == "accept":
                taken[b] = (a, smt3_name, ev)
            self.rows.append((b, dds_name, a, smt3_name, status, ev))
        # A new name that is already in use at an address this batch does not rename
        # would be refused by names.py; a name used twice in the batch is a wrong pair.
        counts = Counter(name for _a, name, _ev in taken.values())
        for b, (a, name, ev) in list(taken.items()):
            other = self.by_name.get(name)
            if counts[name] > 1:
                self.drop(b, "reject:duplicate-target")
                taken.pop(b)
            elif other is not None and other != b and other not in taken:
                self.drop(b, f"reject:collision-with-0x{other:08X}")
                taken.pop(b)
        self.accepted = taken

    def drop(self, b, status):
        self.rows = [(bb, dn, a, sn, status if bb == b and st == "accept" else st, ev)
                     for bb, dn, a, sn, st, ev in self.rows]

    def ordered(self):
        """Accepted rows as (parking rows, final rows), each in applicable order.

        names.py refuses a name another address still holds and validates row by
        row, so a row waits for the one that frees its name. A cycle has no such
        order, and is broken by parking one address on a temporary name. The two
        groups must be applied in separate names.py runs: it rewrites C references
        in one simultaneous pass, which permutes names correctly but cannot create
        a temporary name and consume it again in the same run."""
        pending = dict(sorted(self.accepted.items()))
        holder = {self.symbols[b]: b for b in pending if b in self.symbols}
        pre, out = [], []
        while pending:
            ready = [b for b, (_a, name, _ev) in pending.items()
                     if holder.get(name) in (None, b) or holder[name] not in pending]
            if not ready:
                b = next(iter(pending))
                pre.append((b, f"tmp_{b:08X}", "cycle"))
                holder.pop(self.symbols[b], None)
                continue
            for b in ready:
                out.append((b, *pending.pop(b)[1:]))
        return pre, out

    def write(self, out_dir):
        v = self.version
        pre, final = self.ordered()
        rows = lambda group: [
            f"0x{b:08X}\t{name}\tinferred\t" + (
                "parked while a rename cycle resolves" if ev == "cycle" else
                f"SMT3HD name via SLUS_209.11 0x{self.accepted[b][0]:08X}, {ev}; was "
                + (self.symbols.get(b) or (self.dds[b][3] if b in self.dds else "?")))
            for b, name, ev in group]
        (out_dir / f"names_{v}.tsv").write_text("\n".join(rows(final)) + "\n")
        path = out_dir / f"names_{v}_pre.tsv"
        if pre:
            path.write_text("\n".join(rows(pre)) + "\n")
        elif path.exists():
            path.unlink()
        # The net old-to-new map, for the headers, docs and config that names.py
        # leaves alone (dds-decomp tools/rename_refs.py).
        renames = []
        for b, (_a, name, _ev) in sorted(self.accepted.items()):
            old = self.symbols.get(b)
            if old and old != name and not PLACEHOLDER.match(old):
                renames.append(f"{old}\t{name}")
        (out_dir / f"renames_{v}.tsv").write_text("\n".join(renames) + "\n")
        head = [
            f"# SMT3HD function names for {v}, from SLUS_209.11 through code pairing.",
            "# Generated by smt3-decomp tools/dds_names.py; one row per candidate.",
            "# dds_addr\tdds_name\tsmt3_addr\tsmt3_name\tstatus\tevidence",
        ]
        body = [f"0x{b:08X}\t{dn}\t0x{a:08X}\t{sn}\t{st}\t{ev}"
                for b, dn, a, sn, st, ev in sorted(self.rows)]
        (out_dir / f"smt3_pairs_{v}.tsv").write_text("\n".join(head + body) + "\n")
        return Counter(st for *_, st, _ev in self.rows)


STRONG = ("unique-masked-hash", "masked-hash-in-gap", "byte-identical", "reloc-identical")


def propagate_twins(g1, g2, dds1_dds2_pairs):
    """Give a DDS1<->DDS2 twin the name its sibling got, or drop both on a conflict.

    include/ is shared by both games, so a declaration renamed for one must hold
    for the other. Only pairs the DDS1-to-DDS2 masked alignment calls high on a
    strong hash carry a name."""
    twins = {b1: b2 for b1, (b2, tier, ev) in dds1_dds2_pairs.items()
             if tier == "high" and ev.split("+")[0] in STRONG}
    back = {b2: b1 for b1, b2 in twins.items()}
    carried = Counter()
    for src, dst, fwd in ((g1, g2, twins), (g2, g1, back)):
        for b, (a, name, ev) in list(src.accepted.items()):
            t = fwd.get(b)
            if t is None:
                continue
            got = dst.accepted.get(t)
            if got is not None:
                if got[1] != name:
                    src.drop(b, "reject:twin-conflict")
                    dst.drop(t, "reject:twin-conflict")
                    src.accepted.pop(b, None)
                    dst.accepted.pop(t, None)
                continue
            if not dst.in_batch(t):
                continue
            current = dst.symbols.get(t) or (dst.dds[t][3] if t in dst.dds else "?")
            other = dst.by_name.get(name)
            if dst.kinds.get(t) == "evidence":
                # The twin keeps a name the binary itself carries; one name for both.
                src.drop(b, "reject:twin-evidence")
                src.accepted.pop(b, None)
                continue
            if current == name or (RUNTIME.match(current) and not PLACEHOLDER.match(current)):
                continue
            if other is not None and other != t and other not in dst.accepted:
                continue
            dst.accepted[t] = (a, name, f"twin-of-{src.version}-0x{b:08X}")
            dst.rows.append((t, current, a, name, "accept", f"twin-of-{src.version}-0x{b:08X}"))
            carried[dst.version] += 1
    # include/ is shared, so a function the two games call by one inferred name
    # is one declaration; when only one side paired, the other follows the name.
    for src, dst in ((g1, g2), (g2, g1)):
        for b, (a, name, ev) in list(src.accepted.items()):
            old = src.symbols.get(b)
            t = dst.by_name.get(old) if old else None
            if t is None or t in dst.accepted or dst.kinds.get(t) == "evidence":
                continue
            if t not in dst.symbols or RUNTIME.match(old) or not dst.in_batch(t):
                continue
            other = dst.by_name.get(name)
            if other is not None and other != t and other not in dst.accepted:
                continue
            if any(n == name for _a, n, _e in dst.accepted.values()):
                continue
            dst.accepted[t] = (a, name, f"same-old-name-as-{src.version}-0x{b:08X}")
            dst.rows.append((t, old, a, name, "accept", f"same-old-name-as-{src.version}-0x{b:08X}"))
            carried[dst.version] += 1
    return carried


def transitivity_drops(g1, g2, dds1_dds2_pairs, smt3_text_end):
    """SMT3 functions whose DDS2 counterpart differs by the route taken."""
    c1 = g1.candidates(smt3_text_end)
    c2 = g2.candidates(smt3_text_end)
    dropped = set()
    for a, (b1, ev1) in c1.items():
        if ev1.startswith("reject:") or a not in c2 or c2[a][1].startswith("reject:"):
            continue
        via = dds1_dds2_pairs.get(b1)
        if via and via[1] == "high" and via[0] != c2[a][0]:
            dropped.add(a)
    return dropped


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("--dds1", nargs=2, metavar=("PAIRS", "CACHE"), required=True)
    ap.add_argument("--dds2", nargs=2, metavar=("PAIRS", "CACHE"), required=True)
    ap.add_argument("--dds1-dds2", metavar="PAIRS", required=True)
    ap.add_argument("--dds-decomp", type=Path, required=True,
                    help="dds-decomp checkout; its orig/ holds the two DDS ELFs")
    ap.add_argument("--smt3-elf", default=str(ROOT / "orig/smt3/SLUS_209.11"))
    ap.add_argument("--prefix", nargs="*", default=[], metavar="P",
                    help="only rename functions whose current dds-decomp name starts "
                         "with one of these module prefixes, for one batch at a time")
    ap.add_argument("--list-prefixes", action="store_true",
                    help="print how many renames each current module prefix carries")
    ap.add_argument("--smt3-text-end", type=lambda s: int(s, 0), default=0x002E6E44)
    ap.add_argument("--out-dir", type=Path, default=ROOT / "build" / "dds_names")
    args = ap.parse_args()

    g1 = Game("dds1", args.dds1[0], args.dds1[1], args.smt3_elf,
              args.dds_decomp / "orig/dds1/SLUS_209.74", args.dds_decomp)
    g2 = Game("dds2", args.dds2[0], args.dds2[1], args.smt3_elf,
              args.dds_decomp / "orig/dds2/SLUS_211.52", args.dds_decomp)
    for g in (g1, g2):
        g.prefixes = set(args.prefix)
    dropped = transitivity_drops(g1, g2, read_pairs(args.dds1_dds2), args.smt3_text_end)
    if args.list_prefixes:
        counts = Counter()
        for g in (g1, g2):
            g.prefixes = set()
            g.decide(args.smt3_text_end, dropped)
            for b in g.accepted:
                name = g.symbols.get(b) or ""
                m = re.match(r"^_*([A-Za-z][a-z0-9]*)", name)
                counts[(m.group(1) if m else "?")] += 1
        for prefix, n in counts.most_common():
            print(f"{prefix:>12}  {n}")
        print(f"{len(counts)} prefixes, {sum(counts.values())} renames over both games")
        return 0
    args.out_dir.mkdir(parents=True, exist_ok=True)
    for g in (g1, g2):
        g.decide(args.smt3_text_end, dropped)
    carried = propagate_twins(g1, g2, read_pairs(args.dds1_dds2))
    for g in (g1, g2):
        counts = g.write(args.out_dir)
        print(f"{g.version}: " + "  ".join(f"{k}={n}" for k, n in sorted(counts.items()))
              + f"  (carried from twin: {carried[g.version]})")
    print(f"wrote names_*.tsv and smt3_pairs_*.tsv to {args.out_dir}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
