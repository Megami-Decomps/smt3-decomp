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


class CoverCrossings(unittest.TestCase):
    def test_extent_over_another_symbol_is_reported(self):
        self.assertEqual(cds.cover_crossings([(0x1000, 0x7620)], [0x1000, 0x100C, 0x1090]),
                         [(0x1000, 0x8620, 0x100C), (0x1000, 0x8620, 0x1090)])

    def test_extent_ending_at_the_next_symbol_is_not(self):
        self.assertEqual(cds.cover_crossings([(0x1000, 0xC)], [0x1000, 0x100C]), [])


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


class AddressProvenance(unittest.TestCase):
    def scan(self, words):
        import struct
        data = struct.pack("<%dI" % len(words), *words)
        return cds.scan_text(data, [dict(name=".text", off=0, size=len(data),
                                       addr=0x100000)], {0x100000}, None)

    def test_same_register_high_low_construction(self):
        for opcode, low, expected in ((9, 0x1234, 0x321234),
                                      (9, 0xFFFF, 0x31FFFF),
                                      (0x19, 0x1234, 0x321234),
                                      (0xD, 0xFFFF, 0x32FFFF)):
            with self.subTest(opcode=opcode, low=low):
                _, consts = self.scan([(0xF << 26) | (2 << 16) | 0x32,
                    (opcode << 26) | (2 << 21) | (2 << 16) | low])
                self.assertEqual(consts, [(0x100004, expected, True)])

    def test_distinct_destination_has_same_provenance(self):
        for destination in (2, 3):
            _, consts = self.scan([(0xF << 26) | (2 << 16) | 0x32,
                (9 << 26) | (2 << 21) | (destination << 16) | 0x1234])
            self.assertEqual(consts, [(0x100004, 0x321234, True)])

    def test_adjustment_of_completed_address_is_not_new_high_pair(self):
        _, consts = self.scan([(0xF << 26) | (2 << 16) | 0x32,
            (9 << 26) | (2 << 21) | (2 << 16) | 0x1234,
            (9 << 26) | (2 << 21) | (2 << 16) | 4])
        self.assertEqual(consts, [(0x100004, 0x321234, True),
                                (0x100008, 0x321238, False)])

    def test_move_preserves_high_provenance_before_self_update(self):
        _, consts = self.scan([(0xF << 26) | (2 << 16) | 0x32,
            (2 << 21) | (3 << 11) | 0x21,
            (9 << 26) | (3 << 21) | (3 << 16) | 0x1234])
        self.assertEqual(consts, [(0x100008, 0x321234, True)])


class ZeroRegister(unittest.TestCase):
    scan = AddressProvenance.scan
    def test_discarded_address_does_not_seed_access_or_const(self):
        for opcode in (9, 0x19, 0xD):
            with self.subTest(opcode=opcode):
                accesses, consts = self.scan([
                    (0xF << 26) | (2 << 16) | 0x32,
                    (opcode << 26) | (2 << 21) | 0x1234,
                    (0x23 << 26) | (3 << 16),
                    (9 << 26) | (4 << 16) | 8,
                    (0x23 << 26) | (4 << 21) | (5 << 16)])
                self.assertEqual((accesses, consts), ([], []))

    def test_discarded_move_does_not_seed_access(self):
        for funct in (0x21, 0x2D, 0x25):
            with self.subTest(funct=funct):
                accesses, consts = self.scan([
                    (0xF << 26) | (2 << 16) | 0x32,
                    (2 << 21) | funct,
                    (0x23 << 26) | (3 << 16)])
                self.assertEqual((accesses, consts), ([], []))

    def test_discarded_gp_address_does_not_seed_const(self):
        import struct
        for opcode in (9, 0x19):
            with self.subTest(opcode=opcode):
                words = [(opcode << 26) | (28 << 21) | 4,
                         (0x23 << 26) | (3 << 16)]
                data = struct.pack('<2I', *words)
                result = cds.scan_text(data, [dict(name='.text', off=0,
                    size=len(data), addr=0x100000)], {0x100000}, 0x550000)
                self.assertEqual(result, ([], []))

    def test_real_register_source_survives_discarded_write(self):
        accesses, consts = self.scan([
            (0xF << 26) | (2 << 16) | 0x32,
            (9 << 26) | (2 << 21) | 0x1234,
            (9 << 26) | (2 << 21) | (2 << 16) | 0x20,
            (0x23 << 26) | (2 << 21) | (3 << 16)])
        self.assertEqual(consts, [(0x100008, 0x320020, True)])
        self.assertEqual(accesses, [(0x10000C, 0x320020, 4, True,
                                    0x320020, False)])


class Controls(unittest.TestCase):
    def test_controls_pass_on_the_real_checks(self):
        cds.run_controls()

    def test_controls_fail_when_a_check_is_muted(self):
        real = cds.count_address_like
        cds.count_address_like = lambda *a: 0
        try:
            with self.assertRaises(SystemExit):
                cds.run_controls()
        finally:
            cds.count_address_like = real
        real = cds.cover_crossings
        cds.cover_crossings = lambda *a: []
        try:
            with self.assertRaises(SystemExit):
                cds.run_controls()
        finally:
            cds.cover_crossings = real


if __name__ == "__main__":
    unittest.main()
