#!/usr/bin/env python3
"""Check selected pinned EE compiler properties by raw object instructions.

Requires the same downloaded compiler and pyelftools as test_headers.py.
No retail executable or disassembler is needed.
"""
import os
from pathlib import Path
import struct
import subprocess
import tempfile
import unittest

from elftools.elf.elffile import ELFFile

ROOT = Path(__file__).resolve().parents[1]

# Ignore fs/fd but retain opcode, single format, unused ft and function bits.
CONVERSION_MASK = 0xFFFF003F
CVT_W_S = 0x46000024
TRUNC_W_S = 0x4600000D


class EeCodegenTests(unittest.TestCase):
    def test_conversion_opcode_mask(self):
        self.assertEqual(0x46006064 & CONVERSION_MASK, CVT_W_S)
        self.assertEqual(0x46000064 & CONVERSION_MASK, CVT_W_S)
        self.assertEqual(0x4600604D & CONVERSION_MASK, TRUNC_W_S)
        self.assertNotEqual(CVT_W_S, TRUNC_W_S)

    def test_ordinary_casts_select_cvt_w_s(self):
        # decompals objdump names function 0x24 "trunc.w.s". Check the bits,
        # not its mnemonic, and inspect each function rather than the whole
        # section so one passing cast cannot hide another's regression.
        source = """
int cast_signed(float f) { return (int)f; }
unsigned cast_unsigned(float f) { return (unsigned)f; }
int cast_scaled(float f) { return (int)(f * 2.0f); }
int cast_squared(float f) { return (int)(f * f); }
"""
        with tempfile.TemporaryDirectory() as tmp:
            folder = Path(tmp)
            src, obj = folder / "casts.c", folder / "casts.o"
            src.write_text(source)
            env = os.environ.copy()
            # A temporary source outside src/ uses the default game compiler.
            # Do not inherit a developer's per-unit experiment override.
            for key in ("SMT3_AS_UNIT", "SMT3_COMPILER", "SMT3_AS",
                        "SMT3_KEEP_S"):
                env.pop(key, None)
            done = subprocess.run(
                [str(ROOT / "tools/cc.sh"), str(src), "-o", str(obj)],
                cwd=ROOT, env=env, capture_output=True, text=True,
            )
            self.assertEqual(done.returncode, 0, done.stdout + done.stderr)
            with obj.open("rb") as stream:
                elf = ELFFile(stream)
                self.assertEqual(elf.elfclass, 32)
                self.assertEqual(elf["e_machine"], "EM_MIPS")
                self.assertEqual(elf["e_type"], "ET_REL")
                symbols = elf.get_section_by_name(".symtab")
                for name in ("cast_signed", "cast_unsigned", "cast_scaled",
                             "cast_squared"):
                    with self.subTest(function=name):
                        matches = symbols.get_symbol_by_name(name)
                        self.assertEqual(len(matches or []), 1)
                        symbol = matches[0]
                        self.assertEqual(symbol["st_info"]["type"], "STT_FUNC")
                        section = elf.get_section(symbol["st_shndx"])
                        self.assertTrue(section["sh_flags"] & 0x4)  # SHF_EXECINSTR
                        start, size = symbol["st_value"], symbol["st_size"]
                        self.assertGreater(size, 0)
                        self.assertEqual(size % 4, 0)
                        code = section.data()[start:start + size]
                        self.assertEqual(len(code), size)
                        order = "<" if elf.little_endian else ">"
                        words = struct.unpack(order + "I" * (size // 4), code)
                        opcodes = [word & CONVERSION_MASK for word in words]
                        self.assertIn(CVT_W_S, opcodes)
                        self.assertNotIn(TRUNC_W_S, opcodes)


if __name__ == "__main__":
    unittest.main()
