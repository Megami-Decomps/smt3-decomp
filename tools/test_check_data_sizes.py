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


if __name__ == "__main__":
    unittest.main()
