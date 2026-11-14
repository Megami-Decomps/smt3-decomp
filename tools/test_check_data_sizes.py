#!/usr/bin/env python3
"""Pin the jump table rule: a data symbol must end at or before the next jtbl_ label."""
import sys
import json
import tempfile
from unittest import mock
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import check_data_sizes as cds


class SymbolInventory(unittest.TestCase):
    def load(self, text):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "symbols.txt"
            path.write_text(text)
            return cds.load_syms(path)

    def test_duplicate_name_is_rejected(self):
        for first, second in (("size:4 type:u8", "size:64 type:u32"),
                              ("", "size:64"), ("size:4", "size:4")):
            with self.subTest(first=first, second=second), self.assertRaisesRegex(
                    ValueError, r":2: duplicate symbol value; first defined on line 1"):
                self.load(f"value = 0x00300000; // {first}\n"
                          f"value = 0x00300000; // {second}\n")

    def test_duplicate_at_other_address_is_rejected(self):
        with self.assertRaisesRegex(ValueError, "duplicate symbol value"):
            self.load("value = 0x00300000;\nvalue = 0x00400000;\n")

    def test_duplicate_overlays_are_explicitly_unsupported(self):
        with self.assertRaisesRegex(ValueError, "Duplicate-name inventories are unsupported"):
            self.load("value = 0x00300000; // allow_duplicated:true\n"
                      "value = 0x00400000; // allow_duplicated:true\n")

    def test_distinct_names_are_unchanged(self):
        symbols = self.load("first = 0x00300000; // size:4 type:u8\n"
                            "next = 0x00300010; // size:16\n")
        self.assertEqual(set(symbols), {"first", "next"})
        self.assertEqual(symbols["first"]["size"], 4)
        self.assertEqual(symbols["first"]["type"], "u8")

    def test_cli_rejects_duplicate_before_other_inputs(self):
        with mock.patch.object(sys, "argv", ["check_data_sizes"]), \
             mock.patch.object(cds, "load_syms", side_effect=ValueError("duplicate symbol x")), \
             mock.patch.object(cds, "load_elf") as elf, \
             mock.patch.object(cds, "load_word_addrs") as words, \
             mock.patch.object(sys, "stderr"):
            with self.assertRaises(SystemExit) as failure:
                cds.main()
        self.assertEqual(failure.exception.code, 2)
        elf.assert_not_called()
        words.assert_not_called()


class CSizeCache(unittest.TestCase):
    def cache(self, symbols, failed=None):
        data = {"symbols": symbols}
        if failed is not None:
            data["failed_units"] = failed
        tmp = tempfile.TemporaryDirectory()
        self.addCleanup(tmp.cleanup)
        path = Path(tmp.name) / "sizes.json"
        path.write_text(json.dumps(data))
        return str(path)

    def test_empty_failed_cache_is_rejected(self):
        with self.assertRaisesRegex(ValueError, "1 unit.*src/broken.c"):
            cds.load_csizes(self.cache({}, ["src/broken.c"]))

    def test_partial_failed_cache_is_rejected(self):
        with self.assertRaisesRegex(ValueError, "2 unit.*src/a.c, src/b.c"):
            cds.load_csizes(self.cache({"good": [{"size": 4}]},
                                      ["src/a.c", "src/b.c"]))

    def test_complete_cache_preserves_views(self):
        views = {"good": [{"size": 4, "unit": "src/good.c"}]}
        self.assertEqual(cds.load_csizes(self.cache(views, [])), views)

    def test_legacy_cache_remains_supported(self):
        self.assertEqual(cds.load_csizes(self.cache({})), {})

    def test_cli_rejects_before_scanning_inputs(self):
        path = self.cache({}, ["src/broken.c"])
        with mock.patch.object(sys, "argv", ["check_data_sizes", "--csizes", path]), \
             mock.patch.object(cds, "load_syms", return_value={}), \
             mock.patch.object(cds, "load_elf") as elf, \
             mock.patch.object(cds, "load_word_addrs") as words, \
             mock.patch.object(sys, "stderr") as stderr:
            with self.assertRaises(SystemExit) as failure:
                cds.main()
        self.assertEqual(failure.exception.code, 2)
        elf.assert_not_called()
        words.assert_not_called()
        self.assertIn("src/broken.c", "".join(c.args[0] for c in stderr.write.call_args_list))


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


class JumpTargets(unittest.TestCase):
    scan = AddressProvenance.scan

    def test_forward_jump_target_drops_linear_path_constant(self):
        accesses, _ = self.scan([
            (2 << 26) | (0x100010 >> 2), 0,
            (0xF << 26) | (2 << 16) | 0x32,
            (9 << 26) | (2 << 21) | (2 << 16) | 0x20,
            (0x23 << 26) | (2 << 21) | (3 << 16)])
        self.assertEqual(accesses, [])

    def test_backward_jump_target_drops_linear_path_constant(self):
        accesses, _ = self.scan([
            (0xF << 26) | (2 << 16) | 0x32,
            (9 << 26) | (2 << 21) | (2 << 16) | 0x20,
            (0x23 << 26) | (2 << 21) | (3 << 16),
            (2 << 26) | (0x100008 >> 2), 0])
        self.assertEqual(accesses, [])

    def test_jump_delay_slot_and_target_local_construction_survive(self):
        accesses, _ = self.scan([
            (0xF << 26) | (2 << 16) | 0x32,
            (9 << 26) | (2 << 21) | (2 << 16) | 0x20,
            (2 << 26) | (0x100010 >> 2),
            (0x23 << 26) | (2 << 21) | (3 << 16),
            (0xF << 26) | (2 << 16) | 0x34,
            (9 << 26) | (2 << 21) | (2 << 16) | 0x40,
            (0x23 << 26) | (2 << 21) | (3 << 16)])
        self.assertEqual([a[1] for a in accesses], [0x320020, 0x340040])


class LikelyDelaySlots(unittest.TestCase):
    scan = AddressProvenance.scan

    def branches(self):
        return ([(op << 26) | (4 << 21) | 5 for op in (0x14, 0x15, 0x16, 0x17)]
                + [(1 << 26) | (4 << 21) | (rt << 16) | 5 for rt in (2, 3, 18, 19)]
                + [(op << 26) | (8 << 21) | (rt << 16) | 5
                   for op in (0x10, 0x11, 0x12) for rt in (2, 3)])

    def test_annulled_address_creation_cannot_seed_fallthrough(self):
        for branch in self.branches():
            with self.subTest(branch=hex(branch)):
                accesses, _ = self.scan([branch,
                    (0xF << 26) | (2 << 16) | 0x32,
                    (0x23 << 26) | (2 << 21) | (3 << 16)])
                self.assertEqual(accesses, [])

    def test_annulled_overwrite_restores_pre_slot_address(self):
        # Use a callee-saved register to isolate annulment from call clobbers.
        for branch in self.branches():
            with self.subTest(branch=hex(branch)):
                accesses, _ = self.scan([
                    (0xF << 26) | (16 << 16) | 0x32, branch,
                    (0xF << 26) | (16 << 16) | 0x34,
                    (0x23 << 26) | (16 << 21) | (3 << 16)])
                self.assertEqual([a[1] for a in accesses], [0x320000])

    def test_link_likely_invalidates_previous_return_address(self):
        for rt in (18, 19):
            with self.subTest(rt=rt):
                accesses, _ = self.scan([
                    (0xF << 26) | (31 << 16) | 0x32,
                    (1 << 26) | (4 << 21) | (rt << 16) | 5,
                    (0x23 << 26) | (31 << 21) | (2 << 16),
                    (0x23 << 26) | (31 << 21) | (3 << 16)])
                self.assertEqual(accesses, [])

    def test_target_join_still_clears_restored_state(self):
        accesses, _ = self.scan([
            (0xF << 26) | (2 << 16) | 0x32,
            (0x14 << 26) | (4 << 21) | 1,
            (0xF << 26) | (2 << 16) | 0x34,
            (0x23 << 26) | (2 << 21) | (3 << 16)])
        self.assertEqual(accesses, [])

    def test_taken_slot_access_and_nonlikely_slot_writes_survive(self):
        accesses, _ = self.scan([
            (0xF << 26) | (2 << 16) | 0x32,
            (0x14 << 26) | (4 << 21) | 5,
            (0x23 << 26) | (2 << 21) | (2 << 16),
            (0x23 << 26) | (2 << 21) | (3 << 16)])
        self.assertEqual([a[1] for a in accesses], [0x320000, 0x320000])
        accesses, _ = self.scan([
            (4 << 26) | (4 << 21) | 5,
            (0xF << 26) | (2 << 16) | 0x34,
            (0x23 << 26) | (2 << 21) | (3 << 16)])
        self.assertEqual([a[1] for a in accesses], [0x340000])


class CallLinkProvenance(unittest.TestCase):
    scan = AddressProvenance.scan
    def test_jal_invalidates_old_link_before_delay_slot_and_after_return(self):
        for slot in (0, (0x23 << 26) | (31 << 21) | (3 << 16)):
            with self.subTest(slot=slot):
                accesses, _ = self.scan([
                    (0xF << 26) | (31 << 16) | 0x34,
                    (3 << 26) | (0x120000 >> 2), slot,
                    (0x23 << 26) | (31 << 21) | (3 << 16)])
                self.assertEqual(accesses, [])

    def test_jal_does_not_construct_address_from_old_link_in_slot(self):
        accesses, consts = self.scan([
            (0xF << 26) | (31 << 16) | 0x34,
            (3 << 26) | (0x120000 >> 2),
            (9 << 26) | (31 << 21) | (16 << 16) | 0x1234,
            (0x23 << 26) | (16 << 21) | (3 << 16)])
        self.assertEqual(accesses, [])
        self.assertEqual(consts, [])

    def test_noncall_preserves_link_and_jalr_kills_its_actual_link(self):
        accesses, _ = self.scan([
            (0xF << 26) | (31 << 16) | 0x34, 0,
            (0x23 << 26) | (31 << 21) | (3 << 16)])
        self.assertEqual([a[1] for a in accesses], [0x340000])
        for link in (16, 31):
            with self.subTest(link=link):
                accesses, _ = self.scan([
                    (0xF << 26) | (link << 16) | 0x34,
                    (25 << 21) | (link << 11) | 9,
                    (0x23 << 26) | (link << 21) | (3 << 16)])
                self.assertEqual(accesses, [])


class ConditionalCallProvenance(unittest.TestCase):
    scan = AddressProvenance.scan

    def branch(self, rt):
        return (1 << 26) | (4 << 21) | (rt << 16) | 8

    def test_all_link_forms_invalidate_old_ra_before_slot(self):
        for rt in (16, 17, 18, 19):
            with self.subTest(rt=rt):
                accesses, _ = self.scan([
                    (0xF << 26) | (31 << 16) | 0x34,
                    self.branch(rt),
                    (0x23 << 26) | (31 << 21) | (3 << 16),
                    (0x23 << 26) | (31 << 21) | (3 << 16)])
                self.assertEqual(accesses, [])

    def test_link_slot_reads_argument_but_return_cannot_reuse_it(self):
        for rt in (16, 17, 18, 19):
            with self.subTest(rt=rt):
                accesses, _ = self.scan([
                    (0xF << 26) | (5 << 16) | 0x34,
                    self.branch(rt),
                    (0x23 << 26) | (5 << 21) | (3 << 16),
                    (0x23 << 26) | (5 << 21) | (3 << 16)])
                self.assertEqual([a[0] for a in accesses], [0x100008])

    def test_saved_address_survives_link_return(self):
        for rt in (16, 17, 18, 19):
            with self.subTest(rt=rt):
                accesses, _ = self.scan([
                    (0xF << 26) | (16 << 16) | 0x34,
                    self.branch(rt), 0,
                    (0x23 << 26) | (16 << 21) | (3 << 16)])
                self.assertEqual([a[1] for a in accesses], [0x340000])

    def test_nonlink_regimm_preserves_caller_and_link_registers(self):
        for rt in (0, 1, 2, 3):
            for reg in (5, 31):
                with self.subTest(rt=rt, reg=reg):
                    accesses, _ = self.scan([
                        (0xF << 26) | (reg << 16) | 0x34,
                        self.branch(rt), 0,
                        (0x23 << 26) | (reg << 21) | (3 << 16)])
                    self.assertEqual([a[1] for a in accesses], [0x340000])


class MergeAccessSpans(unittest.TestCase):
    scan = AddressProvenance.scan
    cases = ((0x22, 0x26, 4), (0x1A, 0x1B, 8),
             (0x2A, 0x2E, 4), (0x2C, 0x2D, 8))

    def test_left_and_right_spans_at_every_alignment(self):
        for left, right, size in self.cases:
            for offset in range(size):
                for op, start, width in ((left, 0x340000, offset + 1),
                                         (right, 0x340000 + offset, size - offset)):
                    with self.subTest(op=op, offset=offset):
                        accesses, _ = self.scan([
                            (0xF << 26) | (16 << 16) | 0x34,
                            (9 << 26) | (16 << 21) | (16 << 16),
                            (op << 26) | (16 << 21) | (3 << 16) | offset])
                        self.assertEqual(accesses, [(0x100008, start, width, True,
                                                     0x340000, op in cds.STORE_W)])

    def test_merge_pairs_cover_exact_unaligned_block(self):
        for left, right, size in self.cases:
            for offset in range(size):
                with self.subTest(size=size, left=left, offset=offset):
                    accesses, _ = self.scan([
                        (0xF << 26) | (16 << 16) | 0x34,
                        (left << 26) | (16 << 21) | (3 << 16) | (offset + size - 1),
                        (right << 26) | (16 << 21) | (3 << 16) | offset])
                    touched = {a for _, start, width, *_ in accesses
                               for a in range(start, start + width)}
                    self.assertEqual(touched, set(range(0x340000 + offset,
                                                        0x340000 + offset + size)))

    def test_gp_negative_offsets_use_effective_address_alignment(self):
        import struct
        for left, right, size in self.cases:
            for op in (left, right):
                with self.subTest(op=op):
                    word = (op << 26) | (28 << 21) | (3 << 16) | 0xFFFF
                    data = struct.pack('<I', word)
                    accesses, _ = cds.scan_text(data, [dict(name='.text', off=0,
                        size=4, addr=0x100000)], {0x100000}, 0x340002)
                    start, width = ((0x340000, 2) if op == left
                                    else (0x340001, size - 1))
                    self.assertEqual(accesses, [(0x100000, start, width, False,
                                                 None, op in cds.STORE_W)])

    def test_merge_load_kills_base_destination_but_store_preserves_it(self):
        for left, right, size in self.cases:
            for op in (left, right):
                with self.subTest(op=op):
                    accesses, _ = self.scan([
                        (0xF << 26) | (16 << 16) | 0x34,
                        (op << 26) | (16 << 21) | (16 << 16) | 1,
                        (0x23 << 26) | (16 << 21) | (3 << 16)])
                    self.assertEqual(len(accesses), 2 if op in cds.STORE_W else 1)


class MergeExtentReports(unittest.TestCase):
    def report(self, words, start, size, other=None):
        import contextlib
        import io
        import struct
        data = struct.pack('<%dI' % len(words), *words)
        sections = [dict(name='.text', off=0, size=len(data), addr=0x100000),
                    dict(name='.bss', off=0, size=0x100, addr=0x340000, type=8)]
        symbols = {'object': dict(name='object', addr=start, size=size,
                                  type='u8', auto=False)}
        if other is not None:
            symbols['next'] = dict(name='next', addr=other, size=16, type='u8', auto=False)
        output = io.StringIO()
        with mock.patch.object(sys, 'argv', ['check_data_sizes', '--classes',
                                             'BEYOND,STRADDLE,IN_GAP']), \
             mock.patch.object(cds, 'load_syms', return_value=symbols), \
             mock.patch.object(cds, 'load_elf', return_value=(data, sections)), \
             mock.patch.object(cds, 'load_word_addrs', return_value=set()), \
             mock.patch.object(cds, 'load_jtbls', return_value=[]), \
             contextlib.redirect_stdout(output):
            cds.main()
        return output.getvalue()

    def setup(self, offset=0):
        return [(0xF << 26) | (16 << 16) | 0x34,
                (9 << 26) | (16 << 21) | (16 << 16) | offset]

    def test_right_span_reports_previously_missed_straddle(self):
        for op in (0x26, 0x1B, 0x2E, 0x2D):
            with self.subTest(op=op):
                report = self.report(self.setup() + [
                    (op << 26) | (16 << 21) | (3 << 16) | 1], 0x340000, 2)
                self.assertIn('STRADDLE', report)
                self.assertIn('object', report)

    def test_right_span_reports_beyond_base_into_next_owner(self):
        for op in (0x26, 0x1B, 0x2E, 0x2D):
            with self.subTest(op=op):
                report = self.report(self.setup() + [
                    (op << 26) | (16 << 21) | (3 << 16) | 4],
                    0x340000, 4, 0x340004)
                self.assertIn('BEYOND', report)
                self.assertIn('into next', report)

    def test_exact_unaligned_pairs_do_not_overrun_object(self):
        for left, right, size in MergeAccessSpans.cases:
            for offset in range(1, size):
                with self.subTest(left=left, offset=offset):
                    report = self.report(self.setup(offset) + [
                        (left << 26) | (16 << 21) | (3 << 16) | (size - 1),
                        (right << 26) | (16 << 21) | (3 << 16)],
                        0x340000 + offset, size)
                    self.assertNotIn('BEYOND', report)
                    self.assertNotIn('STRADDLE', report)
                    self.assertNotIn('IN_GAP', report)


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
