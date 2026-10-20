#!/usr/bin/env python3
"""Map the SDK region of .text to the archive object each function came from.

    python tools/sdk_objects.py smt3 --sdk DIR [--sdk DIR ...] [-o FILE]

Each --sdk is an installed SCE SDK tree. docs/sdk-matching.md has the method.
"""
from __future__ import annotations

import argparse
import collections
import hashlib
import re
import struct
import subprocess
import tempfile
from pathlib import Path

from elftools.elf.elffile import ELFFile

import retail_elf

ROOT = Path(__file__).resolve().parent.parent
VERSIONS = retail_elf.VERSIONS
AR = ROOT / "tools" / "bin" / "mips-ps2-decompals-ar"

# The game links hard-float, so the soft-float multilib of libgcc is left out:
# it defines the same names and would make every libgcc name ambiguous.
ARCHIVE_GLOBS = (
    "ee/lib/*.a",
    "ee/gcc/ee/lib/*.a",
    "ee/gcc/lib/gcc-lib/ee/*/libgcc.a",
)
# Archives that are copies of another: a name-only hit in one is reported there.
ALIAS = {"libg.a": "libc.a", "libidt.a": "libc.a", "libmx.a": "libm.a"}
SYM = re.compile(r"\s*([A-Za-z_][A-Za-z0-9_]*)\s*=\s*(0x[0-9A-Fa-f]+)\s*;")
UPSTREAM = re.compile(rb"src/(?:newlib|gcc)/[A-Za-z0-9_./+-]+\.(?:c|cc|S|s)")
# Opcodes (bits 31..26) whose low 16 bits can hold a relocation.
IMM_OPS = frozenset(range(0x08, 0x10)) | frozenset(range(0x20, 0x30)) | {0x0F, 0x31, 0x37, 0x39, 0x3F}
NAME_FLOOR = 0x10
BYTE_FLOOR = 0x20


def mask(code: bytes) -> bytes:
    """Clear every field a relocation can change, leaving the opcode shape."""
    out = bytearray()
    for i in range(0, len(code) - 3, 4):
        (word,) = struct.unpack_from("<I", code, i)
        op = word >> 26
        if op in (0x02, 0x03):
            word &= 0xFC000000
        elif op in IMM_OPS:
            word &= 0xFFFF0000
        out += struct.pack("<I", word)
    return bytes(out)


def fhash(code: bytes) -> str:
    return hashlib.sha1(mask(code)).hexdigest()[:16]


def object_funcs(path: Path) -> list[tuple[str, bytes]]:
    """[(name, bytes)] for the .text functions of one object, in address order."""
    with path.open("rb") as fp:
        try:
            elf = ELFFile(fp)
        except Exception:
            return []
        text = {i: s for i, s in enumerate(elf.iter_sections()) if s.name == ".text"}
        if not text:
            return []
        data = {i: s.data() for i, s in text.items()}
        out = []
        for sec in elf.iter_sections():
            if sec.header["sh_type"] != "SHT_SYMTAB":
                continue
            for sym in sec.iter_symbols():
                h = sym.entry
                if h["st_info"]["type"] != "STT_FUNC" or not h["st_size"]:
                    continue
                if h["st_shndx"] not in data:
                    continue
                blob = data[h["st_shndx"]][h["st_value"]:h["st_value"] + h["st_size"]]
                out.append((sym.name, h["st_value"], blob))
        out.sort(key=lambda t: t[1])
    return [(n, b) for n, _, b in out]


def index_archives(sdks: list[Path], workdir: Path):
    """(by_hash, by_name, upstream) over every archive in every SDK given."""
    by_hash: dict[str, list] = collections.defaultdict(list)
    by_name: dict[str, list] = collections.defaultdict(list)
    upstream: dict[tuple[str, str], str] = {}
    for sdk in sdks:
        for glob in ARCHIVE_GLOBS:
            for archive in sorted(sdk.glob(glob)):
                lib = archive.name
                if "gcc-lib" in archive.parts:
                    lib = f"libgcc[{archive.parent.name}]"
                out = workdir / sdk.name / archive.parent.name / archive.name
                out.mkdir(parents=True, exist_ok=True)
                try:
                    subprocess.run([str(AR), "x", str(archive.resolve())], cwd=out,
                                   check=True, stdout=subprocess.DEVNULL,
                                   stderr=subprocess.DEVNULL)
                except (OSError, subprocess.CalledProcessError):
                    continue
                for member in sorted(p for p in out.iterdir() if p.is_file()):
                    funcs = object_funcs(member)
                    if not funcs:
                        continue
                    key = (lib, member.name)
                    found = UPSTREAM.search(member.read_bytes())
                    if found and key not in upstream:
                        upstream[key] = found.group().decode()
                    for name, blob in funcs:
                        by_hash[fhash(blob)].append((sdk.name, lib, member.name, len(blob)))
                        by_name[name].append((sdk.name, lib, member.name, len(blob)))
    return by_hash, by_name, upstream


def retail_funcs(version: str, lo: int, hi: int):
    """[(addr, name, bytes)] for the symbols in [lo, hi), sized to the next one."""
    image = (ROOT / retail_elf.RETAIL[version]).read_bytes()
    segs = retail_elf.load_segments(image)
    syms = sorted({(int(m.group(2), 16), m.group(1))
                   for m in (SYM.match(line) for line
                             in (ROOT / "config" / version / "symbol_addrs.txt").read_text().splitlines())
                   if m})
    addrs = sorted({a for a, _ in syms})
    out = []
    for addr, name in syms:
        if not lo <= addr < hi:
            continue
        end = min(next((a for a in addrs if a > addr), hi), hi)
        lo_off, hi_off = (retail_elf.va_to_off(segs, x) for x in (addr, end))
        out.append((addr, name, image[lo_off:hi_off]))
    return out


def pick(hits, prefer: str):
    """Collapse hits to one (lib, member) when they agree, preferring one SDK."""
    best = [h for h in hits if h[0] == prefer] or hits
    libs = {ALIAS.get(h[1], h[1]) for h in best}
    if len(libs) != 1:
        return None
    members = {h[2] for h in best}
    sdks = sorted({h[0] for h in best})
    return (libs.pop(), members.pop() if len(members) == 1 else "?", ",".join(sdks))


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("version", choices=sorted(VERSIONS))
    ap.add_argument("--sdk", type=Path, action="append", required=True,
                    help="installed SCE SDK tree (holding ee/lib); repeatable")
    ap.add_argument("--range", default="0x2C81A8:0x2E6E50",
                    help="vram range to map (default: the SDK region of .text)")
    ap.add_argument("--prefer", default="296",
                    help="SDK directory name to trust when several match")
    ap.add_argument("-o", "--out", type=Path,
                    help="output TSV (default config/<version>/sdk_objects.tsv)")
    args = ap.parse_args()
    lo, hi = (int(x, 16) for x in args.range.split(":"))
    out = args.out or ROOT / "config" / args.version / "sdk_objects.tsv"

    with tempfile.TemporaryDirectory(prefix="sdk_objects.") as tmp:
        by_hash, by_name, upstream = index_archives(args.sdk, Path(tmp))
        rows = []
        for addr, name, code in retail_funcs(args.version, lo, hi):
            size = len(code)
            owner, how = None, "none"
            if size >= BYTE_FLOOR:
                hits = [h for h in by_hash.get(fhash(code), ()) if h[3] == size]
                if hits:
                    owner, how = pick(hits, args.prefer), "bytes"
            if owner is None and size >= NAME_FLOOR:
                hits = by_name.get(name, ())
                if hits:
                    owner, how = pick(hits, args.prefer), "name"
            if owner is None:
                how = "none"
                lib, member, sdks = "-", "-", "-"
            else:
                lib, member, sdks = owner
            rows.append((addr, name, size, lib, member, how, sdks,
                         upstream.get((lib, member), "-")))

    header = [
        "# Which archive object each function in the SDK region of .text came from.",
        "# NOT a splat config and not an input to the build. Produced by",
        "# tools/sdk_objects.py from installed SCE SDK trees; see docs/sdk-matching.md.",
        "#",
        "# evidence: bytes = same length and same relocation-masked instruction words",
        "#           name  = the recovered name is defined in exactly one member",
        "#           none  = neither; the object is still unidentified",
        "# upstream: the newlib or gcc source file the member was built from, where",
        "#           the member's debug paths name one.",
        "#",
        "addr\tname\tsize\tlib\tmember\tevidence\tsdk\tupstream",
    ]
    body = [f"{a:08X}\t{n}\t{s}\t{lib}\t{mem}\t{how}\t{sdks}\t{up}"
            for a, n, s, lib, mem, how, sdks, up in rows]
    out.write_text("\n".join(header + body) + "\n")

    agg = collections.defaultdict(lambda: [0, 0, collections.Counter()])
    for _, _, size, lib, _, how, _, _ in rows:
        agg[lib][0] += 1
        agg[lib][1] += size
        agg[lib][2][how] += 1
    total = sum(v[1] for v in agg.values())
    named = total - agg["-"][1]
    print(f"{out}: {len(rows)} functions, {total} bytes")
    for lib, (count, size, how) in sorted(agg.items(), key=lambda kv: -kv[1][1]):
        print(f"  {lib:26} {count:4} funcs {size:7} bytes  {dict(how)}")
    print(f"  attributed {named} of {total} bytes ({100 * named / total:.1f}%)")


if __name__ == "__main__":
    main()
