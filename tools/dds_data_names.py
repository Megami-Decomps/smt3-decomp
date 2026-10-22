#!/usr/bin/env python3
"""Carry SMT3HD data symbol names to DDS through identical function pairs.

Where two paired functions have the same masked code, the instruction at a given
index addresses the same object in both, so each reference is a vote. Writes
data_<v>.tsv for dds-decomp's names.py. Rules: docs/dds-names.md."""

from __future__ import annotations

import argparse
import json
import re
import struct
import sys
from collections import Counter, defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
DELTA = 0xFF000
GP = 28
TEXT_START = 0x00100000
# Immediate users that complete a lui or address off $gp into a data address.
LO_OPS = frozenset((0x09, 0x19, 0x1E, 0x1F, 0x20, 0x21, 0x23, 0x24, 0x25, 0x27, 0x28,
                    0x29, 0x2B, 0x31, 0x35, 0x37, 0x39, 0x3D, 0x3F))
PLACEHOLDER = re.compile(r"^(D|jpt|jtbl|sub|func|loc|unk|byte|word|dword|qword|flt|dbl|asc|stru|algn|off|seg)_")
ADDRESS_TAG = re.compile(r"_[0-9A-Fa-f]{5,}$")
IDENT = re.compile(r"^[A-Za-z_]\w*$")
ROW = re.compile(r"^\s*(\S+)\s*=\s*0x([0-9A-Fa-f]+)\s*;(.*)$")
SIZE = re.compile(r"size:(0x[0-9A-Fa-f]+|\d+)")
# A name with no capital is a compiler runtime or libc object the linker resolves
# by name, as in the function pass.
RUNTIME = re.compile(r"^_|^[a-z0-9_]+$")


def gp_value(image):
    """$gp out of the MIPS register-info section, which both ELFs carry."""
    shoff = struct.unpack_from("<I", image, 0x20)[0]
    shnum, _shstrndx = struct.unpack_from("<HH", image, 0x30)
    for i in range(shnum):
        sh = struct.unpack_from("<10I", image, shoff + i * 40)
        if sh[1] == 0x70000006:
            return struct.unpack_from("<I", image, sh[4] + 0x14)[0]
    raise SystemExit("no .reginfo section")


def data_refs(image, addr, size, gp):
    """{instruction index: data address} for one function's resolvable references."""
    out = {}
    live = {}
    for i in range(0, size, 4):
        word = struct.unpack_from("<I", image, addr - DELTA + i)[0]
        op, rs, rt, imm = word >> 26, (word >> 21) & 31, (word >> 16) & 31, word & 0xFFFF
        signed = imm - 0x10000 if imm & 0x8000 else imm
        if op == 0x0F:
            live[rt] = imm << 16
        elif op in LO_OPS and rs == GP:
            out[i] = (gp + signed) & 0xFFFFFFFF
        elif op in LO_OPS and rs in live:
            out[i] = (live[rs] + signed) & 0xFFFFFFFF
        elif op == 0x0D and rs in live:
            out[i] = (live[rs] + imm) & 0xFFFFFFFF
    return out


def read_symbols(path, text_end):
    """{addr: (name, size or None)} for every data row, and the set of all addresses."""
    data, every = {}, set()
    for line in Path(path).read_text().splitlines():
        m = ROW.match(line)
        if not m:
            continue
        addr, attrs = int(m[2], 16), m[3]
        every.add(addr)
        if "type:func" in attrs or addr < text_end:
            continue
        size = SIZE.search(attrs)
        data[addr] = (m[1], int(size[1], 0) if size else None)
    return data, every


def read_pairs(path):
    out = {}
    for line in Path(path).read_text().splitlines():
        if not line or line.startswith("#") or line.startswith("kind\t"):
            continue
        kind, a, _an, _asz, b, _bsz, _d, tier, ev = line.split("\t")
        if kind == "pair" and tier == "high":
            out[int(a, 16)] = int(b, 16)
    return out


def read_excludes(path):
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


def vote(smt3_image, dds_image, cache_dir, pairs, gp_smt3, gp_dds):
    """{smt3 data addr: Counter of dds data addr} over pairs with identical code."""
    rows = json.loads((Path(cache_dir) / "from.json").read_text())
    smt3 = {r[0]: r for r in rows}
    dds = {r[0]: r for r in json.loads((Path(cache_dir) / "to.json").read_text())}
    votes = defaultdict(Counter)
    used = 0
    for a, b in pairs.items():
        fa, fb = smt3.get(a), dds.get(b)
        if not fa or not fb or fa[1] != fb[1] or fa[3] != fb[3]:
            # Different length or masked code: the indices would not line up.
            continue
        used += 1
        ra = data_refs(smt3_image, a, fa[1], gp_smt3)
        rb = data_refs(dds_image, b, fb[1], gp_dds)
        for i, target in ra.items():
            if i in rb:
                votes[target][rb[i]] += 1
    return votes, used


class Game:
    def __init__(self, version, pairs_path, cache_dir, dds_decomp, smt3_syms, smt3_elf, text_ends):
        self.version = version
        self.dds_elf = dds_decomp / ("orig/%s/%s" % (version, {"dds1": "SLUS_209.74",
                                                              "dds2": "SLUS_211.52"}[version]))
        self.symbols, self.every = read_symbols(
            dds_decomp / "config" / version / "symbol_addrs.txt", text_ends[version])
        self.by_name = {}
        for addr, (name, _size) in self.symbols.items():
            self.by_name.setdefault(name, addr)
        self.kinds = read_sources(dds_decomp / "config" / version / "name_sources.txt")
        self.excluded = read_excludes(
            dds_decomp / "config" / "smt3_data_exclude.txt").get(version, {})
        smt3_image = Path(smt3_elf).read_bytes()
        dds_image = self.dds_elf.read_bytes()
        self.votes, self.pairs_used = vote(
            smt3_image, dds_image, cache_dir, read_pairs(pairs_path),
            gp_value(smt3_image), gp_value(dds_image))
        self.smt3 = smt3_syms
        self.rows = []
        self.accepted = {}

    def decide(self):
        """One pass over the votes, applying the evidence rules in order."""
        winner = {}
        for target, counter in self.votes.items():
            (addr, n), *rest = counter.most_common()
            smt3_name, _smt3_size = self.smt3.get(target, (None, None))
            status = "accept"
            if smt3_name is None:
                continue          # not a symbol on the SMT3 side, so nothing to carry
            if rest:
                status = "reject:conflicting-votes"
            elif PLACEHOLDER.match(smt3_name) or ADDRESS_TAG.search(smt3_name) \
                    or not IDENT.match(smt3_name):
                status = "reject:smt3-placeholder"
            elif addr not in self.symbols:
                status = ("reject:inside-another-object" if addr in self.every
                          else "reject:not-a-dds-symbol")
            elif addr in self.excluded:
                status = "reject:excluded"
            elif self.kinds.get(addr) == "evidence":
                status = "reject:dds-evidence"
            elif RUNTIME.match(self.symbols[addr][0]) and not PLACEHOLDER.match(self.symbols[addr][0]):
                status = "reject:dds-runtime-name"
            elif self.symbols[addr][0] == smt3_name:
                status = "same"
            elif not self.sizes_agree(target, addr):
                status = "reject:extent-differs"
            if status == "accept":
                if addr in winner:
                    status = "reject:two-smt3-symbols"
                else:
                    winner[addr] = (target, smt3_name, n)
            self.rows.append((addr, self.symbols.get(addr, ("?", None))[0], target,
                              smt3_name, status, n))
        counts = Counter(name for _t, name, _n in winner.values())
        for addr, (target, name, n) in list(winner.items()):
            other = self.by_name.get(name)
            if counts[name] > 1:
                self.restate(addr, "reject:duplicate-target")
                winner.pop(addr)
            elif other is not None and other != addr and other not in winner:
                self.restate(addr, f"reject:collision-with-0x{other:08X}")
                winner.pop(addr)
        self.accepted = winner

    def restate(self, addr, status):
        self.rows = [(a, dn, t, sn, status if a == addr and st == "accept" else st, n)
                     for a, dn, t, sn, st, n in self.rows]

    def sizes_agree(self, smt3_addr, dds_addr):
        """Both sides' declared extents must match when both are known."""
        smt3_size = self.smt3[smt3_addr][1]
        dds_size = self.symbols[dds_addr][1]
        return smt3_size is None or dds_size is None or smt3_size == dds_size

    def write(self, out_dir):
        v = self.version
        names = []
        for addr, (target, name, n) in sorted(self.accepted.items()):
            old = self.symbols[addr][0]
            names.append(f"0x{addr:08X}\t{name}\tinferred\tSMT3HD data name via "
                         f"SLUS_209.11 0x{target:08X}, {n} agreeing references; was {old}")
        (out_dir / f"data_{v}.tsv").write_text("\n".join(names) + "\n")
        renames = [f"{self.symbols[a][0]}\t{n}" for a, (_t, n, _c) in sorted(self.accepted.items())
                   if not PLACEHOLDER.match(self.symbols[a][0])]
        (out_dir / f"data_renames_{v}.tsv").write_text("\n".join(renames) + "\n")
        head = [f"# SMT3HD data names for {v}, voted through identical function pairs.",
                "# Generated by smt3-decomp tools/dds_data_names.py.",
                "# dds_addr\tdds_name\tsmt3_addr\tsmt3_name\tstatus\tvotes"]
        body = [f"0x{a:08X}\t{dn}\t0x{t:08X}\t{sn}\t{st}\t{n}"
                for a, dn, t, sn, st, n in sorted(self.rows)]
        (out_dir / f"data_pairs_{v}.tsv").write_text("\n".join(head + body) + "\n")
        return Counter(st for *_, st, _n in self.rows)


def twin_check(g1, g2, dds1_dds2_votes):
    """Drop a pair of DDS1 and DDS2 objects that would take different names."""
    dropped = 0
    for addr, (target, name, _n) in list(g1.accepted.items()):
        for other, (other_target, other_name, _m) in list(g2.accepted.items()):
            if other_target == target and other_name != name:
                g1.restate(addr, "reject:twin-conflict")
                g2.restate(other, "reject:twin-conflict")
                g1.accepted.pop(addr, None)
                g2.accepted.pop(other, None)
                dropped += 1
    return dropped


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("--dds1", nargs=2, metavar=("PAIRS", "CACHE"), required=True)
    ap.add_argument("--dds2", nargs=2, metavar=("PAIRS", "CACHE"), required=True)
    ap.add_argument("--dds-decomp", type=Path, required=True)
    ap.add_argument("--smt3-elf", default=str(ROOT / "orig/smt3/SLUS_209.11"))
    ap.add_argument("--smt3-text-end", type=lambda s: int(s, 0), default=0x002E6E44)
    ap.add_argument("--out-dir", type=Path, default=ROOT / "build" / "dds_names" / "out")
    args = ap.parse_args()

    smt3_syms, _every = read_symbols(ROOT / "config/smt3/symbol_addrs.txt", args.smt3_text_end)
    text_ends = {"dds1": 0x00315B94, "dds2": 0x00370B7C}
    games = [Game(v, p, c, args.dds_decomp, smt3_syms, args.smt3_elf, text_ends)
             for v, (p, c) in (("dds1", args.dds1), ("dds2", args.dds2))]
    for g in games:
        g.decide()
    dropped = twin_check(games[0], games[1], None)
    args.out_dir.mkdir(parents=True, exist_ok=True)
    for g in games:
        counts = g.write(args.out_dir)
        print(f"{g.version}: {g.pairs_used} identical pairs  " +
              "  ".join(f"{k}={n}" for k, n in sorted(counts.items())))
    if dropped:
        print(f"{dropped} twin conflicts dropped on both sides")
    print(f"wrote data_*.tsv to {args.out_dir}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
