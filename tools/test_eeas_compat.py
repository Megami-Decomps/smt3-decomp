"""Native padding at and within a translation unit's rodata boundary."""
import sys
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parent))

import eeas_compat  # noqa: E402
from eeas_compat import realign_rodata  # noqa: E402


class RodataAlignmentTests(unittest.TestCase):
    def convert(self, address, unit_start, preceding=bytes(8)):
        elf = bytearray(0x100)
        offset = address - 0x1000
        elf[offset - 8:offset] = preceding
        pool = SimpleNamespace(
            elf=bytes(elf), segs=None,
            va_to_off=lambda segments, addr: addr - 0x1000,
        )
        text = (
            ".section .rodata\n.align 3\nnonmatching table\n\n"
            f"dlabel table\n    /* 000020 {address:08X} 01000000 */ .word 1\n"
        )
        return realign_rodata(text, pool, unit_start)

    def test_first_block_after_native_padding(self):
        # A preceding C object may omit padding carried by its old asm blob.
        self.assertIn(".align 4", self.convert(0x1020, 0x1020))

    def test_interior_block_after_native_padding(self):
        self.assertIn(".align 4", self.convert(0x1040, 0x1020))

    def test_nonzero_predecessor_keeps_alignment(self):
        self.assertIn(".align 3", self.convert(0x1020, 0x1020, b"\x01" + bytes(7)))

    def test_eight_aligned_unit_keeps_alignment(self):
        self.assertIn(".align 3", self.convert(0x1040, 0x1028))

    def test_block_before_unit_keeps_alignment(self):
        self.assertIn(".align 3", self.convert(0x1020, 0x1040))


class VersionDetectionTests(unittest.TestCase):
    """The version comes from the path and names config/versions.json's keys: the
    .lit4 pool and the rodata realignment are skipped without it."""

    def version(self, relative):
        with tempfile.TemporaryDirectory() as tmp:
            src = Path(tmp) / relative
            src.parent.mkdir(parents=True, exist_ok=True)
            src.write_text("")
            argv = ["eeas_compat.py", str(src), str(Path(tmp) / "out.s")]
            with patch.object(eeas_compat, "Pool") as pool, \
                    patch.object(sys, "argv", argv):
                eeas_compat.main()
            return pool.call_args.args[0] if pool.call_args else None

    def test_a_split_asm_path_selects_its_version(self):
        self.assertEqual(
            self.version("asm/smt3/nonmatchings/code_001001D8/dds3FirstInit.s"),
            "smt3",
        )

    def test_a_path_outside_the_version_tree_has_no_pool(self):
        self.assertIsNone(self.version("scratch/experiment.s"))


if __name__ == "__main__":
    unittest.main()
