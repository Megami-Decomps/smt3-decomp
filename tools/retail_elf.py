"""Retail ELF paths and PT_LOAD address/offset mapping."""
import json
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parent.parent


def load_versions():
    path = ROOT / "config" / "versions.json"
    return json.loads(path.read_text()) if path.exists() else {}


VERSIONS = load_versions()
RETAIL = {version: f"orig/{version}/{info['serial']}" for version, info in VERSIONS.items()}


def load_segments(elf):
    """[(vaddr, offset, filesz)] for every PT_LOAD of an ELF32 image."""
    phoff = struct.unpack_from("<I", elf, 0x1C)[0]
    count = struct.unpack_from("<H", elf, 0x2C)[0]
    segs = []
    for i in range(count):
        p_type, p_off, p_vaddr, _, p_filesz = struct.unpack_from("<5I", elf, phoff + i * 32)
        if p_type == 1:
            segs.append((p_vaddr, p_off, p_filesz))
    return segs


def va_to_off(segs, va):
    for vaddr, off, size in segs:
        if vaddr <= va < vaddr + size:
            return off + va - vaddr
    return None


def off_to_va(segs, off):
    for vaddr, foff, size in segs:
        if foff <= off < foff + size:
            return vaddr + off - foff
    return None
