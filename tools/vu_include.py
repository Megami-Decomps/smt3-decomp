#!/usr/bin/env python3
"""Point splat's .vutext unit at the assembled microcode instead of the extracted blob.

    python3 tools/vu_include.py smt3 [--coverage]

Runs after splat, like the other asm fixups. The VIF headers, the debug font texture
and the data the packet unpacks stay bytes from the asset; every instruction comes
from src/<version>/sdf/vu, assembled by tools/vuas.py.
"""

import argparse
import importlib.util
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent

spec = importlib.util.spec_from_file_location("vutext", ROOT / "tools" / "vutext.py")
vutext = importlib.util.module_from_spec(spec)
spec.loader.exec_module(vutext)

# The source the build assembles for each VU, and where its payload lands in micro memory.
SOURCES = {0: "vu0_micro", 1: "vu1_micro"}


def pieces(elf_path):
    """[(what, start, length)] covering .vutext, each either asset bytes or a built payload."""
    elf = elf_path.read_bytes()
    secs = vutext.sections(elf)
    base, _, size = secs[".vutext"]
    out, at = [], 0
    for img in vutext.images(elf, secs):
        for src, micro, count in img["chunks"]:
            start = src - base
            if start > at:
                out.append(("asset", at, start - at))
            out.append((("vu%d" % img["vu"], micro), start, count * 8))
            at = start + count * 8
    if at < size:
        out.append(("asset", at, size - at))
    return out


def coverage(version, elf_path=vutext.DEFAULT_ELF):
    """What .vutext holds and how much of its microcode the build assembles from source.

    objdiff cannot measure this unit: splat gives the whole section one label, so
    the report would score 59,312 bytes of microcode, VIF headers and texture as
    one unmatched function.
    """
    written = {"vu%d" % vu: (ROOT / "src" / version / "sdf" / "vu" / f"{name}.vsm").exists()
               for vu, name in SOURCES.items()}
    section = micro = source = 0
    for what, _, length in pieces(elf_path):
        section += length
        if what == "asset":
            continue
        micro += length
        if written[what[0]]:
            source += length
    return {"section_bytes": section, "microcode_bytes": micro,
            "source_bytes": source, "instructions": source // 8}


def render(version, pieces_, asset, builds):
    lines = ['.include "macro.inc"', "", '.section .text, "ax"', "",
             "/* Written by tools/vu_include.py: the microcode is assembled from",
             " * src/%s/sdf/vu, and the VIF headers and packet data around it are" % version,
             " * bytes from the retail image. */", "",
             "glabel sdfDebugFontTex"]
    for what, start, length in pieces_:
        if what == "asset":
            lines.append('.incbin "%s", 0x%X, 0x%X' % (asset, start, length))
        else:
            vu, micro = what
            lines.append('.incbin "%s", 0x%X, 0x%X   /* %s micro 0x%04X */'
                         % (builds[vu], micro, length, vu, micro // 8))
    lines += ["endlabel sdfDebugFontTex", ""]
    return "\n".join(lines)


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("version")
    ap.add_argument("--elf", type=Path, default=vutext.DEFAULT_ELF)
    ap.add_argument("--coverage", action="store_true",
                    help="report the microcode counters instead of writing the unit")
    args = ap.parse_args(argv)

    if args.coverage:
        vu = coverage(args.version, args.elf)
        print("%(instructions)d instructions, %(source_bytes)d of %(microcode_bytes)d "
              "microcode bytes from source, in a %(section_bytes)d byte section" % vu)
        return 0

    unit = ROOT / "asm" / args.version / "data" / "vutext.s"
    if not unit.exists():
        sys.exit("%s missing; run splat first" % unit)
    asset = "assets/%s/vutext.textbin.bin" % args.version
    builds = {"vu%d" % vu: "build/%s/vu/%s.bin" % (args.version, name)
              for vu, name in SOURCES.items()}
    unit.write_text(render(args.version, pieces(args.elf), asset, builds))
    print("%s -> %d pieces" % (unit.relative_to(ROOT), len(pieces(args.elf))))
    return 0


if __name__ == "__main__":
    sys.exit(main())
