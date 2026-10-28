#!/usr/bin/env python3
"""Cross-title body pairing must ignore relocation fields, not constants."""
from pathlib import Path
import tempfile
import unittest

import dds_bodies as bodies


class RelocatedBodies(unittest.TestCase):
    def parse(self, word, operand):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "function.s"
            raw = word.to_bytes(4, "little").hex().upper()
            path.write_text(f"glabel target\n/* 001000 00100000 {raw} */ {operand}\n")
            return bodies.parse_asm(path)["target"]

    def test_small_data_displacements_are_link_time_values(self):
        # Same instruction, different gp-relative symbol and displacement.
        a = self.parse(0x8F82FFF0, "lw $2, %gp_rel(donor_value)($28)")
        b = self.parse(0x8F820030, "lw $2, %gp_rel(target_value)($28)")
        self.assertEqual(a[0], b[0])
        self.assertEqual(a[1], [("gp_rel", "donor_value")])
        self.assertEqual(bodies.symbol_map({"syms": b[1]}, {"syms": a[1]}),
                         {"donor_value": ["target_value"]})

    def test_small_data_matching_preserves_registers(self):
        a = self.parse(0x8F820010, "lw $2, %gp_rel(value)($28)")
        b = self.parse(0x8F830010, "lw $3, %gp_rel(value)($28)")
        self.assertNotEqual(a[0], b[0])

    def test_plain_immediates_are_not_masked(self):
        a = self.parse(0x24020001, "addiu $2, $0, 1")
        b = self.parse(0x24020002, "addiu $2, $0, 2")
        self.assertNotEqual(a[0], b[0])

    def test_gp_relative_addends_remain_in_the_symbol_map(self):
        a = self.parse(0x8F820010, "lw $2, %gp_rel(values + 0x4)($28)")
        self.assertEqual(a[1], [("gp_rel", "values + 0x4")])


if __name__ == "__main__":
    unittest.main()
