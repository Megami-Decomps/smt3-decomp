#!/usr/bin/env python3
"""Pin the jump table rule: a data symbol must end at or before the next jtbl_ label."""
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import check_data_sizes as cds


class JtblCrossings(unittest.TestCase):
    def test_symbol_running_into_a_jtbl_is_reported(self):
        self.assertEqual(cds.jtbl_crossings([(0x1000, 0x1278)], [0x1450]),
                         [(0x1000, 0x2278, 0x1450)])

    def test_symbol_ending_at_the_jtbl_is_not(self):
        self.assertEqual(cds.jtbl_crossings([(0x1000, 0x450)], [0x1450]), [])

    def test_jtbl_at_the_symbol_start_is_not(self):
        self.assertEqual(cds.jtbl_crossings([(0x1450, 0x10)], [0x1450]), [])

    def test_missing_asm_refuses(self):
        import tempfile
        with tempfile.TemporaryDirectory() as d, self.assertRaises(SystemExit):
            cds.load_jtbls(d)


class WordAddrs(unittest.TestCase):
    def test_only_word_lines_count(self):
        import tempfile
        with tempfile.TemporaryDirectory() as d:
            (Path(d) / "a.s").write_text(
                "    /* 1 00360175 */ .byte 0x00\n"
                "    /* 2 00360178 */ .short 0x0046\n"
                "    /* 3 0036017C 30313200 */ .word D_00323130\n"
                "    /* 4 00360180 00010200 */ .word 0x00020100\n")
            self.assertEqual(cds.load_word_addrs(d), {0x36017C, 0x360180})

    def test_missing_asm_refuses(self):
        import tempfile
        with tempfile.TemporaryDirectory() as d, self.assertRaises(SystemExit):
            cds.load_word_addrs(d)


if __name__ == "__main__":
    unittest.main()
