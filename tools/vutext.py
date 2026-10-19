#!/usr/bin/env python3
"""Reconstruct the VU0 and VU1 micro memory images from the retail ELF's .vutext.

.vutext holds two VIF packets, `vu0MicroPacket` and `vu1MicroPacket`, which
`sdfCoreDrawInit` and `sdf3DLinkInitPacket` send to VIF0 and VIF1. Walking them
gives each MPG upload, the micro address it loads at, and the UNPACKs that seed
VU1 data memory. Writes the images as a vu2c.py dump so the listings in docs/vu
come from the ELF rather than from a capture.

  tools/vutext.py --packets               the VIF walk, one line per code
  tools/vutext.py --jsonl build/vu/images.jsonl
  tools/vutext.py --check                 every byte accounted for, against .DVP.ovlytab
"""

import argparse
import hashlib
import json
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
DEFAULT_ELF = ROOT / "orig" / "smt3" / "SLUS_209.11"

# The two packets, from the code that sends them: sdfCoreDrawInit DMAs
# vu0MicroPacket..vu1MicroPacket to channel 0, and sdf3DLinkInitPacket REFs
# vu1MicroPacket..D_002F5600 into the draw chain.
PACKETS = [("vu0MicroPacket", 0, 0x002F1350, 0x002F18E0),
           ("vu1MicroPacket", 1, 0x002F18E0, 0x002F5600)]

# sha1 of the reconstructed micro memory, first 16 hex digits. vu2c.py prints this
# as the program's identity; the VU1 one is the image a PCSX2 VU trace of the
# retail game captures, so a transcription of either applies to the other.
IMAGE_SHA1 = {0: "FA3335F3E7425E51", 1: "914D6629023EA313"}

UNPACK_FORMAT = {0x0: ("S-32", 1, 4), 0x1: ("S-16", 1, 2), 0x2: ("S-8", 1, 1),
                 0x4: ("V2-32", 2, 4), 0x5: ("V2-16", 2, 2), 0x6: ("V2-8", 2, 1),
                 0x8: ("V3-32", 3, 4), 0x9: ("V3-16", 3, 2), 0xA: ("V3-8", 3, 1),
                 0xC: ("V4-32", 4, 4), 0xD: ("V4-16", 4, 2), 0xE: ("V4-8", 4, 1),
                 0xF: ("V4-5", 1, 2)}

SIMPLE = {0x00: "NOP", 0x10: "FLUSHE", 0x11: "FLUSH", 0x13: "FLUSHA", 0x17: "MSCNT"}
WITH_IMM = {0x02: "OFFSET", 0x03: "BASE", 0x04: "ITOP", 0x05: "STMOD", 0x06: "MSKPATH3",
            0x07: "MARK", 0x14: "MSCAL", 0x15: "MSCALF"}


def sections(elf):
    """{name: (addr, offset, size)} from the section headers."""
    shoff = struct.unpack_from("<I", elf, 0x20)[0]
    shentsize, shnum, shstrndx = struct.unpack_from("<3H", elf, 0x2E)
    rows = [struct.unpack_from("<10I", elf, shoff + i * shentsize) for i in range(shnum)]
    strtab = rows[shstrndx][4]
    out = {}
    for name_off, _, _, addr, off, size, _, _, _, _ in rows:
        at = strtab + name_off
        out[elf[at:elf.index(b"\0", at)].decode()] = (addr, off, size)
    return out


def overlays(elf, secs):
    """[(name, source vram, micro byte address, size)] from the .DVP tables.

    dvp-as records one entry per MPG upload: where the microcode sits in the image
    and where it loads in micro memory. The .DVP.overlay[...] sections it points at
    are all zero in the retail file; only their sizes carry information.
    """
    _, taboff, tabsize = secs[".DVP.ovlytab"]
    _, stroff, strsize = secs[".DVP.ovlystrtab"]
    strtab = elf[stroff:stroff + strsize]
    sizes = [size for name, (_, _, size) in secs.items() if name.startswith(".DVP.overlay[")]
    out = []
    for i in range(tabsize // 12):
        name_off, vram, micro = struct.unpack_from("<3I", elf, taboff + i * 12)
        name = strtab[name_off:strtab.index(b"\0", name_off)].decode()
        out.append((name, vram, micro, sizes[i] if i < len(sizes) else None))
    return out


def vif_walk(elf, secs, start, end):
    """[(vram, word, text, payload)] for every VIF code in [start, end).

    Raises if a code runs past `end`, so a walk that reaches the end exactly is a
    proof that every byte of the packet is a code or a code's payload.
    """
    addr, off, _ = secs[".vutext"]
    out, at = [], start
    while at < end:
        word = struct.unpack_from("<I", elf, off + at - addr)[0]
        cmd, num, imm = (word >> 24) & 0x7F, (word >> 16) & 0xFF, word & 0xFFFF
        irq = (word >> 31) & 1
        payload = 0
        if cmd in SIMPLE:
            text = SIMPLE[cmd]
        elif cmd in WITH_IMM:
            text = "%s %d" % (WITH_IMM[cmd], imm)
        elif cmd == 0x01:
            text = "STCYCL cl=%d wl=%d" % (imm & 0xFF, imm >> 8)
        elif cmd == 0x20:
            text, payload = "STMASK", 4
        elif cmd in (0x30, 0x31):
            text, payload = ("STROW" if cmd == 0x30 else "STCOL"), 16
        elif cmd == 0x4A:
            text, payload = "MPG %d -> micro 0x%04X" % (num or 256, imm), (num or 256) * 8
        elif cmd in (0x50, 0x51):
            text = "%s %d qw" % ("DIRECT" if cmd == 0x50 else "DIRECTHL", imm or 65536)
            payload = (imm or 65536) * 16
        elif cmd >= 0x60:
            fmt, comps, bits = UNPACK_FORMAT[cmd & 0x0F]
            text = "UNPACK %s%s %d -> vu mem %d%s" % (
                fmt, "m" if cmd & 0x10 else "", num or 256, imm & 0x3FF,
                " unsigned" if (imm >> 14) & 1 else "")
            payload = (num or 256) * comps * bits
        else:
            raise ValueError("unknown VIF command 0x%02X at 0x%08X" % (cmd, at))
        if irq:
            text += " +IRQ"
        body = elf[off + at - addr + 4:][:payload]
        if at + 4 + payload > end:
            raise ValueError("%s at 0x%08X runs %d bytes past 0x%08X"
                             % (text, at, at + 4 + payload - end, end))
        out.append((at, word, text, body))
        at += 4 + payload
        at = (at + 3) & ~3
    if at != end:
        raise ValueError("walk ended at 0x%08X, not 0x%08X" % (at, end))
    return out


def images(elf, secs):
    """One entry per packet: the micro memory it uploads, and the data writes it makes."""
    out = []
    for name, vu, start, end in PACKETS:
        codes = vif_walk(elf, secs, start, end)
        micro, chunks, unpacks = bytearray(), [], []
        for at, _, text, body in codes:
            if text.startswith("MPG"):
                target = int(text.split("0x")[1], 16) * 8
                if target != len(micro):
                    raise ValueError("%s uploads micro 0x%04X with 0x%04X filled"
                                     % (name, target // 8, len(micro) // 8))
                chunks.append((at + 4, target, len(body) // 8))
                micro += body
            elif text.startswith("UNPACK"):
                unpacks.append((at, text, body))
        out.append({"name": name, "vu": vu, "start": start, "end": end, "codes": codes,
                    "micro": bytes(micro), "chunks": chunks, "unpacks": unpacks})
    return out


# The lower-instruction opcodes a dispatch stub is built from, as PCSX2 decodes them
# (bits 31..25), and the micro memory a branch target wraps in.
LOWER_IADDIU, LOWER_B, LOWER_BAL, LOWER_JR = 0x08, 0x20, 0x21, 0x24
MICRO_INSTRUCTIONS = {0: 512, 1: 2048}


def entry_table(micro, symbols, routines, size):
    """[(micro address, symbol, target, how, routine)] for the MSCAL entry stubs.

    EE code kicks the image with `MSCAL <micro address>`, and most entries are a stub
    that branches straight to its routine, so resolving the branch is what ties a name
    the EE side carries to a range of the listing. A stub that jumps through a register
    the VIF's ITOP feeds has no static target; the entry that is simply the routine's
    first instruction has none to resolve.
    """
    out, step = [], 0
    for at in sorted(symbols):
        target, how, const = None, "the routine itself", {}
        for k in range(at, min(at + 3, len(micro) // 8)):
            lower = struct.unpack_from("<I", micro, k * 8)[0]
            op = lower >> 25
            if op in (LOWER_B, LOWER_BAL):
                imm = lower & 0x7FF
                target = (k + 1 + (imm - 0x800 if imm & 0x400 else imm)) % size
                how = "BAL" if op == LOWER_BAL else "B"
                break
            if op == LOWER_IADDIU:
                # vi00 is always zero, so an IADDIU from it is the constant itself.
                if (lower >> 11) & 0x1F == 0:
                    const[(lower >> 16) & 0x1F] = ((lower >> 10) & 0x7800) | (lower & 0x7FF)
            elif op == LOWER_JR:
                target = const.get((lower >> 11) & 0x1F)
                how = "JR" if target is not None else "JR through VIF1_ITOP"
                break
        out.append((at, symbols[at], target, how, routines.get(target, "")))
        step += 1
    return out


def vutext_symbols(path):
    """{symbol: address} for the VU symbols in splat's symbol table."""
    out = {}
    for line in Path(path).read_text().splitlines():
        if not line.startswith(("vu0", "vu1")):
            continue
        name, _, rest = line.partition(" = ")
        out[name] = int(rest.split(";")[0], 16)
    return out


def symbol_entries(path, base):
    """{micro address: symbol} for the entry symbols inside the VU1 image."""
    return {(addr - base) // 8: name for name, addr in vutext_symbols(path).items()
            if name.startswith("vu1_entry") or name == "vu1_storeFog"}


def routine_names(path):
    """{micro address: routine} from a vu2c names file's `@<entry> <name>` lines."""
    out = {}
    for line in Path(path).read_text().splitlines():
        if line.startswith("@"):
            at, _, name = line[1:].partition(" ")
            out[int(at, 16)] = name.split("#")[0].strip()
    return out


def data_writes(img):
    """[(quadword, lane count, [words])] for every UNPACK in the packet.

    These run before the first MSCAL, so they are the data memory the microcode starts
    from: every fixed address the listing reads and nothing writes is one of them.
    """
    out = []
    for at, text, body in img["unpacks"]:
        fmt = text.split()[1]
        comps, width = UNPACK_FORMAT[
            next(k for k, v in UNPACK_FORMAT.items() if v[0] == fmt.rstrip("m"))][1:]
        quad = int(text.rsplit("mem ", 1)[1].split()[0])
        count = int(text.split()[2])
        for i in range(count):
            start = at + 4 + i * comps * width
            row = body[i * comps * width:(i + 1) * comps * width]
            words = [int.from_bytes(row[j * width:(j + 1) * width], "little")
                     for j in range(comps)]
            out.append({"quad": quad + i, "code": at, "format": fmt, "words": words,
                        "width": width, "start": start, "size": comps * width})
    return out


def as_float(word):
    """The word as a float, where that reading is plausible; None where it is not.

    An integer in a 32-bit lane reads as a denormal, and a float small enough to be one
    is not a constant any of this code uses, so the test separates the two.
    """
    value = struct.unpack("<f", struct.pack("<I", word))[0]
    if value == 0.0 or 1e-6 <= abs(value) < 1e9:
        return value
    return None


def image_hash(micro):
    return hashlib.sha1(micro).hexdigest()[:16].upper()


def as_dump(imgs):
    """The images as a vu2c.py dump: one `image` row each, instructions in micro order."""
    rows = [{"type": "header", "images": len(imgs),
             "instructions": max(len(i["micro"]) // 8 for i in imgs)}]
    for img in imgs:
        micro = img["micro"]
        code = [{"pc": "0x%04X" % (i // 8),
                 "upper": "%08X" % struct.unpack_from("<I", micro, i + 4)[0],
                 "lower": "%08X" % struct.unpack_from("<I", micro, i)[0]}
                for i in range(0, len(micro), 8)]
        rows.append({"type": "image", "hash": image_hash(micro), "addr": "0x0",
                     "words": len(micro) // 4, "instrs": len(code), "execs": 0,
                     "entries": [], "aliases": [], "source": img["name"], "code": code})
    return rows


def check(imgs, ovly):
    """The MPG uploads against .DVP.ovlytab, and each image against its recorded sha1."""
    bad = 0
    uploads = [(src, micro, count) for img in imgs for src, micro, count in img["chunks"]]
    if len(uploads) != len(ovly):
        print("%d MPG uploads, %d overlay entries" % (len(uploads), len(ovly)))
        bad += 1
    for (src, micro, count), (name, vram, at, size) in zip(uploads, ovly):
        if (src, micro) != (vram, at) or (size is not None and size != count * 8):
            print("upload 0x%08X -> micro 0x%04X, %d instructions, is not overlay %s "
                  "(0x%08X -> 0x%04X, %s bytes)"
                  % (src, micro // 8, count, name, vram, at // 8, size))
            bad += 1
    for img in imgs:
        got, want = image_hash(img["micro"]), IMAGE_SHA1.get(img["vu"])
        status = "ok" if got == want else "EXPECTED %s" % want
        print("vu%d %-16s micro 0x0000..0x%04X  %4d instructions  sha1 %s %s"
              % (img["vu"], img["name"], len(img["micro"]) // 8 - 1, len(img["micro"]) // 8,
                 got, status))
        bad += got != want
    return bad


def manifest(imgs, ovly):
    """The upload table, as markdown."""
    lines = ["| packet | source | overlay id | micro | instructions |", "|---|---|---|---|---|"]
    # dvp-as names an overlay `.DVP.overlay..<kind>.<id>.<count>.<index>`; the id is what repeats
    # across the chunks of one upload, and nothing in the name identifies a source file.
    by_src = {vram: name.split(".")[-3] for name, vram, _, _ in ovly}
    for img in imgs:
        for src, micro, count in img["chunks"]:
            lines.append("| `%s` | `0x%08X` | `%s` | `0x%04X..0x%04X` | %d |"
                         % (img["name"], src, by_src.get(src, "-"), micro // 8,
                            micro // 8 + count - 1, count))
    return "\n".join(lines)


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--elf", type=Path, default=DEFAULT_ELF)
    ap.add_argument("--jsonl", type=Path, help="write the images as a vu2c.py dump")
    ap.add_argument("--packets", action="store_true", help="print the VIF walk")
    ap.add_argument("--manifest", action="store_true", help="print the upload table")
    ap.add_argument("--entries", action="store_true",
                    help="the MSCAL entry stubs and the routine each reaches")
    ap.add_argument("--data", action="store_true",
                    help="the VU1 data memory the packet writes before the first MSCAL")
    ap.add_argument("--names", type=Path, default=ROOT / "docs" / "vu" / "vu1_names.txt")
    ap.add_argument("--symbols", type=Path,
                    default=ROOT / "config" / "smt3" / "symbol_addrs.txt")
    ap.add_argument("--check", action="store_true")
    args = ap.parse_args(argv)

    elf = args.elf.read_bytes()
    secs = sections(elf)
    ovly = overlays(elf, secs)
    imgs = images(elf, secs)

    if args.packets:
        for img in imgs:
            print("%s  0x%08X..0x%08X" % (img["name"], img["start"], img["end"]))
            for at, word, text, body in img["codes"]:
                print("  0x%08X  %08X  %s%s"
                      % (at, word, text, "  (%d bytes)" % len(body) if body else ""))
    if args.entries:
        vu1 = next(i for i in imgs if i["vu"] == 1)
        routines = routine_names(args.names) if args.names.exists() else {}
        base = vu1["chunks"][0][0]
        print("| MSCAL | symbol | reaches | routine |")
        print("|---|---|---|---|")
        for at, symbol, target, note, routine in entry_table(
                vu1["micro"], symbol_entries(args.symbols, base), routines,
                MICRO_INSTRUCTIONS[1]):
            where = "`0x%04X` (%s)" % (target, note) if target is not None else note
            print("| `0x%04X` | `%s` | %s | %s |"
                  % (at, symbol, where, "`%s`" % routine if routine else ""))
    if args.data:
        vu1 = next(i for i in imgs if i["vu"] == 1)
        patches = {name: addr for name, addr in vutext_symbols(args.symbols).items()
                   if name.startswith("vu1Tex0Slot")}
        print("| quadword | UNPACK at | format | words | note |")
        print("|---|---|---|---|---|")
        for row in data_writes(vu1):
            cells = []
            for word in row["words"]:
                if row["width"] != 4:
                    cells.append("%d" % word)
                    continue
                value = as_float(word)
                cells.append("`0x%08X`%s" % (word, " %g" % value if value is not None else ""))
            note = ", ".join(name for name, addr in sorted(patches.items())
                             if row["start"] <= addr < row["start"] + row["size"])
            print("| `%d` | `0x%08X` | %s | %s | %s |"
                  % (row["quad"], row["code"], row["format"], " ".join(cells),
                     "patched by `sdf3DInit` through `%s`" % note if note else ""))
    if args.manifest:
        print(manifest(imgs, ovly))
    if args.jsonl:
        args.jsonl.parent.mkdir(parents=True, exist_ok=True)
        with args.jsonl.open("w") as f:
            for row in as_dump(imgs):
                f.write(json.dumps(row, separators=(",", ":")) + "\n")
        print("%d images -> %s" % (len(imgs), args.jsonl))
    if args.check or not (args.packets or args.jsonl or args.manifest
                          or args.entries or args.data):
        return 1 if check(imgs, ovly) else 0
    return 0


if __name__ == "__main__":
    sys.exit(main())
