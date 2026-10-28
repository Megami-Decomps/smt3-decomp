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


class CurrentDonorNames(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.dds = Path(self.tmp.name)
        for ver in ("dds1", "dds2"):
            (self.dds / "config" / ver).mkdir(parents=True)
            (self.dds / "config" / ver / "symbol_addrs.txt").write_text("")
            (self.dds / "src" / ver).mkdir(parents=True)
            (self.dds / "asm" / ver / "matchings").mkdir(parents=True)

    def tearDown(self):
        self.tmp.cleanup()

    def add_donor(self, *, asm_name="old_name", current_name="renamed",
                  source="s32 renamed(void) { return 1; }", address="00100000",
                  symbol_row=None):
        cfg = self.dds / "config/dds1/symbol_addrs.txt"
        row = symbol_row or f"{current_name} = 0x{address}; // type:func"
        cfg.write_text(row + "\n")
        src = self.dds / "src/dds1/test.c"
        src.write_text(source)
        asm = self.dds / "asm/dds1/matchings/test.s"
        words = "".join(
            f"    /* {0x1000 + i * 4:06X} {int(address, 16) + i * 4:08X} 24020001 */ addiu $2, $0, 1\n"
            for i in range(16)
        )
        asm.write_text(f"glabel {asm_name}\n{words}")
        return src

    def donors(self):
        return bodies.dds_donors(self.dds)

    def test_resolves_old_asm_label_to_current_c_definition(self):
        src = self.add_donor()
        donors = self.donors()
        rows = next(iter(donors.values()))
        self.assertEqual(rows[0]["fn"], "renamed")
        self.assertEqual(rows[0]["asm_fn"], "old_name")
        self.assertEqual(rows[0]["src"], str(src.relative_to(self.dds)))

    def test_excludes_current_include_asm_name(self):
        self.add_donor(source='INCLUDE_ASM(const s32, "test", renamed);')
        self.assertEqual(self.donors(), {})

    def test_excludes_prototype_without_definition(self):
        self.add_donor(source="s32 renamed(void);")
        self.assertEqual(self.donors(), {})

    def test_excludes_definition_shaped_comment(self):
        self.add_donor(source="/* s32 renamed(void) { return 1; } */")
        self.assertEqual(self.donors(), {})

    def test_excludes_asm_without_exact_function_address(self):
        self.add_donor(symbol_row="other = 0x00100004; // type:func")
        self.assertEqual(self.donors(), {})

    def test_same_name_c_definition_remains_a_donor(self):
        self.add_donor(asm_name="renamed", current_name="renamed")
        rows = next(iter(self.donors().values()))
        self.assertEqual(rows[0]["fn"], "renamed")

    def test_multiline_definition_is_indexed(self):
        self.add_donor(source="s32\nrenamed(\n    void\n) { return 1; }")
        rows = next(iter(self.donors().values()))
        self.assertEqual(rows[0]["fn"], "renamed")

    def test_non_function_symbol_at_address_is_not_a_donor(self):
        self.add_donor(symbol_row="renamed = 0x00100000; // type:u32")
        self.assertEqual(self.donors(), {})


if __name__ == "__main__":
    unittest.main()
