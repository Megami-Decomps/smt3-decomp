#!/usr/bin/env python3
"""Focused tests for relocation-aware unit verification."""

import contextlib
import io
import struct
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch

import check_unit
from symbol_aliases import SymbolAlias

VERSION = "smt3"
SERIAL = "SLUS_209.11"
GP = 0x00554170
RETAIL_PATH = f"orig/{VERSION}/{SERIAL}"

from check_unit import (
    address,
    asm_literal_users,
    bss_ownership_problems,
    bss_symbols,
    common_symbols,
    initialized_data_problems,
    nobits_ownership_problems,
    nobits_symbols,
    owned_bss_range,
    owned_bss_size,
    owned_nobits_range,
    owned_nobits_size,
    owns_exact_section_item,
    relocate_sdata_item,
    rodata_code_pointers,
    section_symbols,
    source_owned_item_size,
    symbols,
    trim_sdata_item,
    native_function_size,
    text_comparison_extent,
    text_residual_summary,
)


class FullContextRelocationTests(unittest.TestCase):
    def check(self, words, wants, relocs, syms=None, funcs=None, offset=0, size=None):
        return check_unit.full_context_relocation_diffs(
            struct.pack("<" + "I" * len(words), *words), offset,
            len(wants) * 4 if size is None else size,
            struct.pack("<" + "I" * len(wants), *wants), relocs,
            funcs or [], syms or {}, GP)

    def test_named_hi_lo_addend_and_carry(self):
        words = [0x3C020001, 0x24428000]
        relocs = {0: ("R_MIPS_HI16", "data"), 4: ("R_MIPS_LO16", "data")}
        wants = [0x3C020031, 0x24428000]
        self.assertFalse(self.check(words, wants, relocs, {"data": 0x300000}))
        self.assertEqual(len(self.check(words, wants, relocs, {"data": 0x310004})), 2)

    def test_named_gprel_uses_actual_provider(self):
        relocs = {0: ("R_MIPS_GPREL16", "data")}
        self.assertFalse(self.check([0x8F820004], [0x8F820014], relocs, {"data": GP + 16}))
        self.assertEqual(len(self.check([0x8F820004], [0x8F820014], relocs, {"data": GP + 20})), 1)

    def test_local_call_resolves_full_object_offsets(self):
        relocs = {0: ("R_MIPS_26", ".text")}
        funcs = [(0x20, 16, "provider")]
        self.assertFalse(self.check([0x0C000009], [0x0C000801], relocs, {"provider": 0x2000}, funcs))
        self.assertEqual(len(self.check([0x0C000009], [0x0C000801], relocs, {"provider": 0x3000}, funcs)), 1)

    def test_slice_does_not_check_neighbor_or_unresolved_data_section(self):
        self.assertFalse(self.check([0x08000000, 0x3C020000], [0x3C020020],
            {0: ("R_MIPS_26", "bad"), 4: ("R_MIPS_HI16", ".rodata")},
            {"bad": 0x3000}, offset=4))

    def test_unresolved_named_and_local_calls_fail_closed(self):
        for symbol in ("missing_provider", ".text"):
            result = self.check([0x08000000], [0x08000800],
                                {0: ("R_MIPS_26", symbol)})
            self.assertEqual(len(result), 1)
            self.assertIn("unresolved", result[0][3])

    def test_full_owned_small_data_uses_object_gp_bias(self):
        words = struct.pack("<I", 0x8F82C004)
        expected = struct.pack("<I", 0x8F820014)
        args = (words, 0, 4, expected, {0: ("R_MIPS_GPREL16", ".sdata+0x4000")}, [], {}, GP)
        self.assertFalse(check_unit.full_context_relocation_diffs(
            *args, section_bases={".sdata": GP + 16}, object_gp=0x4000))
        self.assertEqual(len(check_unit.full_context_relocation_diffs(
            *args, section_bases={".sdata": GP + 20}, object_gp=0x4000)), 1)

    def test_full_owned_rodata_resolves_actual_origin_and_offset(self):
        words = struct.pack("<II", 0x3C020000, 0x24420028)
        relocs = {0: ("R_MIPS_HI16", ".rodata"), 4: ("R_MIPS_LO16", ".rodata")}
        args = (words, 0, 8, struct.pack("<II", 0x3C020053, 0x2442DF78), relocs, [], {}, GP)
        self.assertFalse(check_unit.full_context_relocation_diffs(
            *args, section_bases={".rodata": 0x52DF50}))
        self.assertEqual(len(check_unit.full_context_relocation_diffs(
            *args, section_bases={".rodata": 0x52DF54})), 1)

    def test_wrong_instruction_shape_is_not_masked(self):
        self.assertEqual(len(self.check([0x08000000], [0x0C000800],
            {0: ("R_MIPS_26", "provider")}, {"provider": 0x2000})), 1)


class FullRodataRelocationTests(unittest.TestCase):
    def check(self, value, want, symbol=".text", kind="R_MIPS_32", **kwargs):
        return check_unit.full_rodata_relocation_problems(
            struct.pack("<I", value), {0: (kind, symbol)},
            [(0x20, 16, "target")], {"target": 0x2000, "data": 0x3000},
            kwargs.get("bases", {}), struct.pack("<I", want))

    def test_full_function_offset_and_interior(self):
        self.assertFalse(self.check(0x24, 0x2004))
        self.assertTrue(self.check(0x28, 0x2004))

    def test_named_provider_and_addend(self):
        self.assertFalse(self.check(4, 0x3004, "data"))
        self.assertTrue(self.check(8, 0x3004, "data"))

    def test_owned_section_pointer(self):
        self.assertFalse(self.check(4, 0x3004, ".rodata", bases={".rodata": 0x3000}))
        self.assertTrue(self.check(8, 0x3004, ".rodata", bases={".rodata": 0x3000}))

    def test_unknown_targets_fail_closed(self):
        for symbol in ("unknown", ".unknown", ".text"):
            self.assertTrue(self.check(0, 0x2000, symbol))

    def test_unsupported_relocation_fails_closed(self):
        self.assertTrue(self.check(0, 0x3000, "data", "R_MIPS_16"))

    def test_outside_emitted_and_owned_span_fail_closed(self):
        for off, data, retail in ((-4, b"1234", b"1234"),
                                  (4, b"1234", b"12345678"),
                                  (4, b"12345678", b"1234")):
            problems = check_unit.full_rodata_relocation_problems(
                data, {off: ("R_MIPS_32", "data")}, [], {}, {}, retail)
            self.assertIn("outside owned", problems[0])


class GeneratedRodataInputTests(unittest.TestCase):
    def test_absent_optional_file(self):
        with tempfile.TemporaryDirectory() as tmp:
            self.assertEqual(check_unit.generated_rodata_text(Path(tmp) / "missing", None), "")

    def test_missing_required_file(self):
        with tempfile.TemporaryDirectory() as tmp:
            with self.assertRaisesRegex(ValueError, "finish configure.py"):
                check_unit.generated_rodata_text(Path(tmp) / "missing", (0x10, 0x20))

    def test_empty_required_file(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "unit.rodata.s"
            for text in ("", " \n\t"):
                path.write_text(text)
                with self.subTest(text=text), self.assertRaisesRegex(ValueError, "missing or empty"):
                    check_unit.generated_rodata_text(path, (0x10, 0x20))

    def test_existing_file_preserved(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "unit.rodata.s"
            text = ".section .rodata\ndlabel D_00100010\n.word 0\n"
            path.write_text(text)
            for extent in (None, (0x10, 0x20)):
                self.assertEqual(check_unit.generated_rodata_text(path, extent), text)


class FocusedFunctionTests(unittest.TestCase):
    def run_checker(self, requested, emitted="target", full_emitted=None, *,
                    code=None, retail_code=None, text_relocs=None, extra_symbols="",
                    source_text="void target(void) {}\n", candidate=None, assembly=None,
                    native_inventory=None, full_nm=None, full_relocs=None, full_rodata=None, full_rodata_owned=True):
        """Exercise the CLI with synthetic objects, without invoking a compiler."""
        if code is None:
            code = struct.pack("<II", 0x03E00008, 0)
        with tempfile.TemporaryDirectory() as tmp, contextlib.ExitStack() as stack:
            root = Path(tmp)
            unit = root / "src/smt3/main/unit.c"
            unit.parent.mkdir(parents=True)
            unit.write_text(source_text)
            config = root / "config/smt3"
            config.mkdir(parents=True)
            (config / "symbol_addrs.txt").write_text(
                "target = 0x1000; // type:func\nalias = 0x1000; // type:func\n"
                + extra_symbols)
            (config / "SLUS_209.11.yaml").write_text("")
            original = root / RETAIL_PATH
            original.parent.mkdir(parents=True)
            original.write_bytes(code if retail_code is None else retail_code)
            argv = ["check_unit.py", str(unit)]
            if requested is not None:
                argv.extend(["--func", requested])
            if candidate is not None:
                alternate = root / "candidate.c"
                alternate.write_text(candidate)
                argv.extend(["--source", str(alternate)])
            if native_inventory is not None:
                native = root / "asm/smt3/main/unit.s"
                native.parent.mkdir(parents=True)
                native.write_text(native_inventory)
            stack.enter_context(patch.object(check_unit, "ROOT", root))
            stack.enter_context(patch.dict(check_unit.RETAIL, {VERSION: RETAIL_PATH}, clear=True))
            stack.enter_context(patch.dict(
                check_unit.VERSIONS, {VERSION: {"serial": SERIAL, "gp": hex(GP)}}, clear=True))
            stack.enter_context(patch.object(check_unit.sys, "argv", argv))
            stack.enter_context(patch("eeas_compat.lit4_range", return_value=(0, 0)))
            stack.enter_context(patch.object(check_unit, "load_segments", return_value=[]))
            stack.enter_context(patch.object(check_unit, "va_to_off", side_effect=lambda segments, addr: addr - 0x1000))
            stack.enter_context(patch.object(check_unit, "text_section", return_value=code))
            stack.enter_context(patch.object(check_unit, "sections", side_effect=lambda obj:
                (full_rodata[0], {".rodata": (0, len(full_rodata[0]))})
                if full_rodata is not None and obj.name == "full.o" else (b"", {})))
            if full_rodata is not None:
                stack.enter_context(patch.object(check_unit, "owned_rom_range", side_effect=
                    lambda yaml, unit, section: (len(code), len(code) + 4)
                    if section == ".rodata" and full_rodata_owned else None))
                stack.enter_context(patch.object(check_unit, "generated_rodata_text", return_value=""))
            stack.enter_context(patch.object(check_unit, "relocations", side_effect=lambda obj: {
                ".text": (full_relocs if obj.name == "full.o" and full_relocs is not None
                          else text_relocs or {}), ".rodata":
                    full_rodata[1] if full_rodata is not None and obj.name == "full.o" else {},
                    ".sdata": {}}))

            def object_output(*args):
                if "-S" not in args:
                    return ""
                if str(args[-1]).endswith("full.o") and full_nm is not None:
                    return full_nm
                names = full_emitted if str(args[-1]).endswith("full.o") and full_emitted is not None else emitted
                return "".join(f"0 {len(code):x} T {name}\n" for name in names.split())

            stack.enter_context(patch.object(check_unit, "run", side_effect=object_output))
            def compile_output(*args, **kwargs):
                keep = kwargs.get("env", {}).get("SMT3_KEEP_S")
                if keep and assembly is not None:
                    Path(keep).write_text(assembly)
                return SimpleNamespace(returncode=0, stderr="")

            stack.enter_context(patch.object(check_unit.subprocess, "run", side_effect=compile_output))
            output = stack.enter_context(contextlib.redirect_stdout(io.StringIO()))
            with self.assertRaises(SystemExit) as stopped:
                check_unit.main()
            return stopped.exception.code, output.getvalue()

    def test_empty_candidate_cannot_pass_whole_owner_audit(self):
        status, report = self.run_checker(
            None, emitted="", candidate="", native_inventory="glabel target\n")
        self.assertEqual(status, 1, report)
        self.assertIn("MISSING target: neither C nor INCLUDE_ASM", report)
        self.assertIn("0 match, 1 differ", report)

    def test_candidate_guarded_asm_fallback_retains_owner(self):
        candidate = ('#ifdef NON_MATCHING\nvoid target(void) {}\n#else\n'
                     'INCLUDE_ASM(s32, "smt3/main/unit", target);\n#endif\n')
        status, report = self.run_checker(
            None, emitted="", full_emitted="target", candidate=candidate, native_inventory="glabel target\n")
        self.assertEqual(status, 0, report)

    def test_inactive_source_definition_cannot_supply_native_owner(self):
        for candidate in (
            "/*\nvoid target(void) {}\n*/\n",
            "#if 0\nvoid target(void) {}\n#endif\n",
            "#ifdef NON_MATCHING\nvoid target(void) {}\n#endif\n",
        ):
            with self.subTest(candidate=candidate):
                status, report = self.run_checker(
                    None, emitted="", candidate=candidate,
                    native_inventory="glabel target\n")
                self.assertEqual(status, 1, report)
                self.assertIn("MISSING target: not emitted by the full-unit compile", report)

    def test_full_owner_emission_checked_even_if_skip_context_has_function(self):
        status, report = self.run_checker(
            None, emitted="target", full_emitted="",
            native_inventory="glabel target\n")
        self.assertEqual(status, 1, report)
        self.assertIn("MISSING target: not emitted by the full-unit compile", report)

    def test_full_owner_definition_retained(self):
        status, report = self.run_checker(
            None, emitted="target", native_inventory="glabel target\n")
        self.assertEqual(status, 0, report)

    def test_zero_size_assembly_entry_is_present_in_full_owner(self):
        status, report = self.run_checker(
            None, emitted="", full_nm="00000000 T target\n",
            candidate='INCLUDE_ASM(s32, "smt3/main/unit", target);\n',
            native_inventory="glabel target\n")
        self.assertEqual(status, 0, report)

    def test_nontext_symbol_cannot_supply_function_owner(self):
        for symbol in ("00000000 D target\n", "00000000 00000008 D target\n"):
            with self.subTest(symbol=symbol):
                status, report = self.run_checker(
                    None, emitted="", full_nm=symbol,
                    native_inventory="glabel target\n")
                self.assertEqual(status, 1, report)
                self.assertIn("not emitted by the full-unit compile", report)

    def test_focused_check_requires_full_context_emission(self):
        status, report = self.run_checker("target", full_emitted="")
        self.assertEqual(status, 1, report)
        self.assertIn("requested function not emitted by the full-unit compile", report)

    def test_missing_requested_name_fails_even_at_an_emitted_address(self):
        for requested in ("typo", "func_00001000", "alias"):
            with self.subTest(requested=requested):
                status, report = self.run_checker(requested)
                self.assertEqual(status, 1)
                self.assertIn(f"MISSING {requested}: requested function not emitted", report)
                self.assertIn("0 match, 1 differ", report)

    def test_empty_skip_asm_object_cannot_pass_a_focused_check(self):
        status, report = self.run_checker("target", emitted="")
        self.assertEqual(status, 1)
        self.assertIn("MISSING target", report)

    def test_wrong_full_context_call_is_not_hidden_by_matching_skip_call(self):
        for requested in (None, "target"):
            with self.subTest(requested=requested):
                status, report = self.run_checker(
                    requested, code=struct.pack("<II", 0x08000000, 0),
                    retail_code=struct.pack("<II", 0x08000800, 0),
                    text_relocs={0: ("R_MIPS_26", "right")},
                    full_relocs={0: ("R_MIPS_26", "wrong")},
                    extra_symbols="right = 0x2000;\nwrong = 0x3000;\n")
                self.assertEqual(status, 1)
                self.assertIn("CONTEXT RELOC target", report)
                self.assertIn("1 match, 1 differ", report)

    def test_full_context_switch_relocation_is_not_hidden_by_masking(self):
        code = struct.pack("<II", 0x03E00008, 0)
        for addend, expected_status in ((0, 0), (4, 1)):
            status, report = self.run_checker(None, code=code,
                retail_code=code + struct.pack("<I", 0x1000),
                full_rodata=(struct.pack("<I", addend), {0: ("R_MIPS_32", ".text")}))
            self.assertEqual(status, expected_status, report)
            if expected_status:
                self.assertIn("DATA CONTEXT full unit's .rodata R_MIPS_32 .text", report)

    def test_relocated_full_rodata_requires_owned_range(self):
        status, report = self.run_checker(None,
            full_rodata=(bytes(4), {0: ("R_MIPS_32", ".text")}),
            full_rodata_owned=False)
        self.assertEqual(status, 1, report)
        self.assertIn("relocated .rodata has no unambiguous YAML-owned range", report)

    def test_full_context_call_can_use_equivalent_symbol_alias(self):
        status, report = self.run_checker(
            None, code=struct.pack("<II", 0x08000000, 0),
            retail_code=struct.pack("<II", 0x08000800, 0),
            text_relocs={0: ("R_MIPS_26", "right")},
            full_relocs={0: ("R_MIPS_26", "equivalent")},
            extra_symbols="right = 0x2000;\nequivalent = 0x2000;\n")
        self.assertEqual(status, 0)
        self.assertNotIn("CONTEXT RELOC", report)

    def test_asm_only_function_does_not_count_as_emitted_c(self):
        status, report = self.run_checker("target", emitted="", full_emitted="target")
        self.assertEqual(status, 1)
        self.assertIn("0 match, 1 differ", report)

    def test_present_requested_name_and_emitted_alias_still_pass(self):
        for requested in ("target", "alias"):
            with self.subTest(requested=requested):
                status, report = self.run_checker(requested, emitted="target alias")
                self.assertEqual(status, 0)
                self.assertIn(f"OK   {requested}", report)
                self.assertIn("1 match, 0 differ", report)

    def test_whole_unit_keeps_existing_empty_and_nonempty_behavior(self):
        for emitted, matches in (("", 0), ("target", 1), ("target alias", 2)):
            with self.subTest(emitted=emitted):
                status, report = self.run_checker(None, emitted=emitted)
                self.assertEqual(status, 0)
                self.assertIn(f"{matches} match, 0 differ", report)

    def test_local_binding_reports_review_without_rejecting_exact_bytes(self):
        for spelling in ("asm", "__asm", "__asm__"):
            with self.subTest(spelling=spelling):
                source = f'void target(void) {{ register float speed {spelling}("$f1") = realSpeed; }}\n'
                status, report = self.run_checker(None, source_text=source)
                self.assertEqual(status, 0)
                self.assertIn("CONSTRAINT line 1: local register binding", report)
                self.assertIn("not semantic approval", report)
                self.assertNotIn("TRICK", report)

    def test_constraint_does_not_suppress_instruction_mismatch(self):
        source = 'void target(void) { register float speed __asm__("$f1") = realSpeed; }\n'
        status, report = self.run_checker(
            None, source_text=source, retail_code=struct.pack("<II", 0x03E00008, 1))
        self.assertEqual(status, 1)
        self.assertIn("DIFF target", report)
        self.assertIn("CONSTRAINT", report)

    def test_alternate_source_gets_the_same_policy_scan_as_the_compiler(self):
        status, report = self.run_checker(None, candidate="void target(void) { goto *destination; }\n")
        self.assertEqual(status, 1)
        self.assertIn("TRICK line 1: computed goto", report)

    def test_global_binding_is_not_a_local_constraint(self):
        source = 'register float speed __asm__("$f1");\nvoid target(void) {}\n'
        status, report = self.run_checker(None, source_text=source)
        self.assertEqual(status, 1)
        self.assertIn("global register binding", report)

    def test_empty_constraint_and_instruction_body_diagnostics_remain_distinct(self):
        source = 'void target(void) { register float speed asm("$f1") = realSpeed; }\n'
        assembly = ".ent target\n#APP\n#NO_APP\n#APP\n" + "nop\n" * 8 + "#NO_APP\njr $ra\n.end target\n"
        status, report = self.run_checker(None, source_text=source, assembly=assembly)
        self.assertEqual(status, 0)  # ASMBODY remains informational, as before.
        self.assertIn("CONSTRAINT target: 1 empty inline-asm region", report)
        self.assertIn("INLINEASM target: 1 nonempty inline-asm region", report)
        self.assertIn("ASMBODY target: 8 of 9 instructions", report)


    def test_backedge_hi16_uses_elf_relocation_order_and_shared_low(self):
        # ee-as puts both HI16 entries before their shared LO16, although the
        # second LUI lives after the ADDIU in the function's instruction stream.
        compiled = struct.pack("<III", 0x3C020000, 0x244216F0, 0x3C020000)
        retail = struct.pack("<III", 0x3C020030, 0x24428170, 0x3C020030)
        relocs = {0: ("R_MIPS_HI16", "drawBank"),
                  8: ("R_MIPS_HI16", "drawBank"),
                  4: ("R_MIPS_LO16", "drawBank")}
        status, report = self.run_checker(
            "target", code=compiled, retail_code=retail, text_relocs=relocs,
            extra_symbols="drawBank = 0x002F6A80;\n")
        self.assertEqual(status, 0)
        self.assertIn("1 match, 0 differ", report)

    def test_backedge_hi16_still_rejects_an_incorrect_high_field(self):
        compiled = struct.pack("<III", 0x3C020000, 0x244216F0, 0x3C020000)
        retail = struct.pack("<III", 0x3C020030, 0x24428170, 0x3C02002F)
        relocs = {0: ("R_MIPS_HI16", "drawBank"),
                  8: ("R_MIPS_HI16", "drawBank"),
                  4: ("R_MIPS_LO16", "drawBank")}
        status, report = self.run_checker(
            "target", code=compiled, retail_code=retail, text_relocs=relocs,
            extra_symbols="drawBank = 0x002F6A80;\n")
        self.assertEqual(status, 1)
        self.assertIn("DIFF target", report)
        self.assertIn("0 match, 2 differ", report)


class SourceReviewDiagnosticsTests(unittest.TestCase):
    def test_all_register_spellings_are_reported_locally_and_rejected_globally(self):
        for spelling in ("asm", "__asm", "__asm__"):
            declaration = f'register float speed {spelling}("$f1") = realSpeed;'
            with self.subTest(spelling=spelling):
                local = check_unit.source_review_diagnostics(f"void f(void) {{ {declaration} }}", "")
                self.assertFalse(any(fatal for fatal, _ in local))
                self.assertIn("local register binding", local[0][1])
                global_ = check_unit.source_review_diagnostics(declaration, "")
                self.assertTrue(global_[0][0])
                self.assertIn("global register binding", global_[0][1])

    def test_comments_strings_and_ordinary_registers_do_not_create_hits(self):
        source = '''
/* register float speed asm("$f1"); goto *destination; */
void f(void) {
    const char *comment = "/*";
    const char *text = "register float speed asm(\\\"$f1\\\"); goto *destination;";
    register int genuineScratch;
    // register float speed __asm__("$f1");
}
'''
        self.assertEqual(check_unit.source_review_diagnostics(source, ""), [])

    def test_quoted_comment_delimiter_does_not_hide_later_computed_goto(self):
        source = 'void f(void) { const char *text = "/*"; goto *destination; }'
        diagnostics = check_unit.source_review_diagnostics(source, "")
        self.assertEqual(len(diagnostics), 1)
        self.assertTrue(diagnostics[0][0])
        self.assertIn("computed goto", diagnostics[0][1])

    def test_label_tables_remain_fatal(self):
        diagnostics = check_unit.source_review_diagnostics(
            'void f(void) { void *labels[] = { &&one, &&two }; }', "")
        self.assertEqual(len(diagnostics), 2)
        self.assertTrue(all(fatal for fatal, _ in diagnostics))

    def test_inactive_nonmatching_draft_does_not_trigger_policy_diagnostics(self):
        source = '#ifdef NON_MATCHING\nvoid f(void) { goto *p; }\n#else\nvoid f(void) {}\n#endif\n'
        self.assertEqual(check_unit.source_review_diagnostics(source, ""), [])

    def test_diagnostics_keep_source_line_numbers_after_inactive_drafts(self):
        source = '#ifdef NON_MATCHING\nvoid f(void) { goto *p; }\n#else\nvoid f(void) { goto *q; }\n#endif\n'
        diagnostics = check_unit.source_review_diagnostics(source, "")
        self.assertEqual(diagnostics, [(True, "TRICK line 4: computed goto (write the switch)")])

    def test_empty_regions_are_diagnostics_not_semantic_approvals(self):
        assembly = '.ent f\n#APP\n# comment only\n\n#NO_APP\njr $ra\n.end f\n'
        self.assertEqual(check_unit.inline_asm_regions(assembly), {"f": (1, 0)})
        self.assertEqual(check_unit.inline_asm_share(assembly), {"f": (0, 1)})
        diagnostics = check_unit.source_review_diagnostics("void f(void) {}", assembly)
        self.assertEqual(len(diagnostics), 1)
        self.assertFalse(diagnostics[0][0])
        self.assertIn("not semantic approval", diagnostics[0][1])

    def test_raw_opcodes_and_directives_cannot_be_called_empty_constraints(self):
        source = 'void f(void) { register int value asm("$2") = input; }'
        for body in ('addiu $2,$2,1', '.word 0x24420001', '.byte 1,0,66,36', '.set noreorder'):
            with self.subTest(body=body):
                assembly = f'.ent f\n#APP\n{body}\n#NO_APP\njr $ra\n.end f\n'
                self.assertEqual(check_unit.inline_asm_regions(assembly), {"f": (0, 1)})
                messages = [message for _, message in check_unit.source_review_diagnostics(source, assembly)]
                self.assertTrue(any("not covered by the empty-constraint allowance" in message for message in messages))
                self.assertFalse(any(message.startswith("CONSTRAINT f:") for message in messages))

    def test_regions_are_kept_separate_by_function(self):
        assembly = '.ent f\n#APP\n#NO_APP\n.end f\n.ent g\n#APP\nnop\n#NO_APP\n.end g\n'
        self.assertEqual(check_unit.inline_asm_regions(assembly), {"f": (1, 0), "g": (0, 1)})


class TextResidualTests(unittest.TestCase):
    def test_agreeing_overrun_prefix_still_fails_the_complete_checker(self):
        compiled = struct.pack("<III", 0x03E00008, 0, 0x24020001)
        retail = struct.pack("<III", 0x03E00008, 0, 0xDEADBEEF)
        with tempfile.TemporaryDirectory() as tmp, contextlib.ExitStack() as stack:
            root = Path(tmp)
            unit = root / "src/smt3/main/unit.c"
            unit.parent.mkdir(parents=True)
            unit.write_text("void target(void) {}\n")
            config = root / "config/smt3"
            config.mkdir(parents=True)
            (config / "symbol_addrs.txt").write_text(
                "target = 0x1000; // type:func\nnext = 0x1008; // type:func\n")
            (config / "SLUS_209.11.yaml").write_text("")
            original = root / RETAIL_PATH
            original.parent.mkdir(parents=True)
            original.write_bytes(retail)
            assembly = root / "asm/smt3/nonmatchings/game/unit/target.s"
            assembly.parent.mkdir(parents=True)
            assembly.write_text("nonmatching target, 0x8\n")
            stack.enter_context(patch.object(check_unit, "ROOT", root))
            stack.enter_context(patch.dict(check_unit.RETAIL, {VERSION: RETAIL_PATH}, clear=True))
            stack.enter_context(patch.dict(
                check_unit.VERSIONS, {VERSION: {"serial": SERIAL, "gp": hex(GP)}}, clear=True))
            stack.enter_context(patch.object(check_unit.sys, "argv", ["check_unit.py", str(unit)]))
            stack.enter_context(patch("eeas_compat.lit4_range", return_value=(0, 0)))
            stack.enter_context(patch.object(check_unit, "load_segments", return_value=[]))
            stack.enter_context(patch.object(check_unit, "va_to_off", side_effect=lambda segments, addr: addr - 0x1000))
            stack.enter_context(patch.object(check_unit, "text_section", return_value=compiled))
            stack.enter_context(patch.object(check_unit, "sections", return_value=(b"", {})))
            stack.enter_context(patch.object(check_unit, "relocations", return_value={".text": {}, ".rodata": {}, ".sdata": {}}))
            stack.enter_context(patch.object(check_unit, "run", side_effect=lambda *args: "0 c T target\n" if "-S" in args else ""))
            stack.enter_context(patch.object(check_unit.subprocess, "run", return_value=SimpleNamespace(returncode=0, stderr="")))
            output = stack.enter_context(contextlib.redirect_stdout(io.StringIO()))
            with self.assertRaises(SystemExit) as stopped:
                check_unit.main()
            self.assertEqual(stopped.exception.code, 1)
            report = output.getvalue()
            self.assertIn("OVER target", report)
            self.assertIn("text prefix: 2 matching, 0 differing words", report)
            self.assertIn("0 match, 1 differ", report)
            self.assertNotIn("DIFF target", report)
            self.assertNotIn("OK   target", report)

    def test_nonzero_overrun_compares_only_the_native_prefix(self):
        text = struct.pack("<IIII", 0xFFFFFFFF, 0x03E00008, 0, 0x24020001)
        self.assertEqual(text_comparison_extent(text, 4, 12, 0x1000, 0x1008),
                         (8, True))
        # An agreeing prefix still reports excess code, not an exact function.
        self.assertEqual(text_residual_summary(12, 8, [], 8),
                         "text prefix: 2 matching, 0 differing words; 12 emitted bytes, "
                         "8 native bytes; 0 uncovered, 4 excess bytes")

    def test_zero_padding_preserves_the_original_comparison_path(self):
        text = struct.pack("<III", 0x03E00008, 0, 0)
        self.assertEqual(text_comparison_extent(text, 0, 12, 0x1000, 0x1008),
                         (12, False))
        self.assertEqual(text_comparison_extent(text, 0, 12, 0x1000, None),
                         (12, False))

    def test_repeated_relocation_diagnostics_do_not_double_count_a_word(self):
        diffs = [(0, 1, 2, "HI16 pair one"), (0, 1, 2, "HI16 pair two"),
                 (8, 3, 4, "outside the native function")]
        self.assertEqual(text_residual_summary(12, 12, diffs, 8),
                         "text prefix: 1 matching, 1 differing words; 12 emitted bytes, "
                         "8 native bytes; 0 uncovered, 4 excess bytes")

    def test_short_candidate_reports_uncovered_native_code(self):
        diffs = [(4, 1, 2, "")]
        self.assertEqual(text_residual_summary(8, 8, diffs, 12),
                         "text prefix: 1 matching, 1 differing words; 8 emitted bytes, "
                         "12 native bytes; 4 uncovered, 0 excess bytes")

    def test_missing_or_unaligned_extent_is_not_invented(self):
        self.assertIsNone(native_function_size("nonmatching other, 0x10\n", "target"))
        self.assertIsNone(native_function_size("nonmatching target, 0x11\n", "target"))
        self.assertEqual(native_function_size("nonmatching target, 0x25C\n", "target"), 604)
        self.assertEqual(text_residual_summary(8, 8, [(0, 1, 2, "")]),
                         "text prefix: 1 matching, 1 differing words; 8 emitted bytes")


class LinkerSymbolTests(unittest.TestCase):
    def symbols(self, rows):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / f"config/{VERSION}").mkdir(parents=True)
            (root / f"config/{VERSION}/symbol_addrs.txt").write_text(rows)
            with patch.object(check_unit, "ROOT", root), \
                    patch.dict(check_unit.VERSIONS,
                               {VERSION: {"serial": SERIAL, "gp": hex(GP)}}, clear=True):
                return symbols(VERSION)

    def test_linker_gp_resolves_to_the_retail_address(self):
        self.assertEqual(address("_gp", self.symbols("target = 0x1000;\n")), GP)

    def test_unprovided_linker_symbol_remains_unresolved(self):
        self.assertIsNone(address("_missing_linker_symbol", self.symbols("")))


class AsmLiteralSharingTests(unittest.TestCase):
    def test_candidate_replaces_self_but_preserves_remaining_asm_sharing(self):
        production = '\n'.join([
            'INCLUDE_ASM(s32, "main/unit", target);',
            'INCLUDE_ASM(s32, "main/unit", other);',
        ])
        candidate = production.replace(
            'INCLUDE_ASM(s32, "main/unit", target);',
            'const char ownLiteral[] = "own";\nvoid target(void) {}',
        )
        with tempfile.TemporaryDirectory() as tmp:
            asm_dir = Path(tmp)
            (asm_dir / "target.s").write_text(
                'lui $a0, %hi(ownLiteral)\naddiu $a1, $gp, %gp_rel(D_00300010)\n'
            )
            (asm_dir / "other.s").write_text(
                'addiu $a0, $gp, %gp_rel(D_00300010)\n'
            )
            syms = {"ownLiteral": 0x00300000}
            self.assertEqual(
                asm_literal_users(production, asm_dir, syms, 0x00300000),
                ["target"],
            )
            self.assertEqual(
                asm_literal_users(candidate, asm_dir, syms, 0x00300000), [],
            )
            self.assertEqual(
                asm_literal_users(candidate, asm_dir, syms, 0x00300010),
                ["other"],
            )


class RodataCodePointerTests(unittest.TestCase):
    def test_mixed_callback_relocations_and_next_table_boundary(self):
        data = struct.pack("<IIII", 0x24, 4, 0x28, 0x2C)
        relocs = {0: ("R_MIPS_32", ".text"),
                  4: ("R_MIPS_32", "asmCallback"),
                  8: ("R_MIPS_32", ".text"),
                  12: ("R_MIPS_32", ".text")}
        funcs = [(0x20, 0x10, "localCallback")]
        syms = {"localCallback": 0x101000, "asmCallback": 0x202000}
        self.assertEqual(rodata_code_pointers(
            data, 0, 12, relocs, funcs, syms, {0x101000, 0x202000}),
            [(0, 0x101004), (4, 0x202004), (8, 0x101008)])

    def test_data_symbol_does_not_extend_code_table(self):
        data = struct.pack("<II", 0x24, 0)
        relocs = {0: ("R_MIPS_32", ".text"),
                  4: ("R_MIPS_32", "dataTable")}
        self.assertEqual(rodata_code_pointers(
            data, 0, 8, relocs, [(0x20, 0x10, "callback")],
            {"callback": 0x101000, "dataTable": 0x303000}, {0x101000}),
            [(0, 0x101004)])

    def test_unresolved_or_unsupported_code_pointer_remains_invalid(self):
        data = struct.pack("<II", 0x40, 0x24)
        relocs = {0: ("R_MIPS_32", ".text"),
                  4: ("R_MIPS_HI16", ".text")}
        self.assertEqual(rodata_code_pointers(
            data, 0, 8, relocs, [(0x20, 0x10, "callback")],
            {"callback": 0x101000}, {0x101000}), [(0, None), (4, None)])


class FocusedRodataTests(unittest.TestCase):
    def check_tables(self, *, func=None, wrong_entry=False, extra_entry=False,
                     neighbor_as_built=False):
        # Skipping an ASM function packs these C tables together in the object,
        # but retail keeps an unrelated table between them. The selected table
        # must end at the next compiled reference, not the end of .rodata or
        # a guessed retail length.
        entries = [0, 4, 8, 0 if wrong_entry else 12]
        if extra_entry:
            entries.append(8)
        neighbor_offset = len(entries) * 4
        entries.extend([16, 20, 24, 28])
        rodata = struct.pack(f"<{len(entries)}I", *entries)
        compiled = struct.pack("<8I", 0x3C020000, 0x24420000, 0x03E00008, 0,
                               0x3C020000, 0x24420000 + neighbor_offset,
                               0x03E00008, 0)
        full_text = bytearray(compiled)
        struct.pack_into("<I", full_text, 20, 0x24420020)
        full_rodata = rodata[:16] + struct.pack("<4I", *([0x12345678] * 4)) + rodata[neighbor_offset:]
        if neighbor_as_built:
            struct.pack_into("<I", full_text, 28, 0x24020001)
        retail = bytearray(0x1040)
        struct.pack_into("<4I", retail, 0, 0x3C020000, 0x24422000, 0x03E00008, 0)
        struct.pack_into("<4I", retail, 0x40, 0x3C020000, 0x24422020,
                         0x03E00008, 0x24020001 if neighbor_as_built else 0)
        struct.pack_into("<12I", retail, 0x1000,
                         0x1000, 0x1004, 0x1008, 0x100C,
                         *([0x12345678] * 4),
                         0x1040, 0x1044, 0x1048, 0x104C)
        relocs = {
            ".text": {0: ("R_MIPS_HI16", ".rodata"),
                      4: ("R_MIPS_LO16", ".rodata"),
                      16: ("R_MIPS_HI16", ".rodata"),
                      20: ("R_MIPS_LO16", ".rodata")},
            ".rodata": {off: ("R_MIPS_32", ".text")
                        for off in range(0, len(rodata), 4)},
            ".sdata": {}, ".data": {},
        }
        with tempfile.TemporaryDirectory() as tmp, contextlib.ExitStack() as stack:
            root = Path(tmp)
            unit = root / "src/smt3/main/unit.c"
            unit.parent.mkdir(parents=True)
            unit.write_text("void target(void) {}\nvoid neighbor(void) {}\n")
            config = root / "config/smt3"
            config.mkdir(parents=True)
            (config / "symbol_addrs.txt").write_text(
                "target = 0x1000; // type:func\nneighbor = 0x1040; // type:func\n")
            (config / "SLUS_209.11.yaml").write_text("      - [0x1000, .rodata, main/unit]\n      - [0x1030, .rodata, main/next]\n")
            original = root / RETAIL_PATH
            original.parent.mkdir(parents=True)
            original.write_bytes(retail)
            assembly = root / "asm/smt3/data/main/unit.rodata.s"
            assembly.parent.mkdir(parents=True)
            assembly.write_text("dlabel jtbl_00002000\ndlabel jtbl_00002010\n"
                                "dlabel jtbl_00002020\ndlabel jtbl_00002030\n")
            argv = ["check_unit.py", str(unit)]
            if func:
                argv.extend(["--func", func])
            stack.enter_context(patch.object(check_unit, "ROOT", root))
            stack.enter_context(patch.dict(check_unit.RETAIL, {VERSION: RETAIL_PATH}, clear=True))
            stack.enter_context(patch.dict(
                check_unit.VERSIONS, {VERSION: {"serial": SERIAL, "gp": hex(GP)}}, clear=True))
            stack.enter_context(patch.object(check_unit.sys, "argv", argv))
            stack.enter_context(patch("eeas_compat.lit4_range", return_value=(0, 0)))
            stack.enter_context(patch.object(check_unit, "load_segments", return_value=[]))
            stack.enter_context(patch.object(check_unit, "va_to_off", side_effect=lambda segments, addr: addr - 0x1000))
            stack.enter_context(patch.object(check_unit, "text_section", side_effect=lambda obj: full_text if obj.name == "full.o" else compiled))
            stack.enter_context(patch.object(check_unit, "sections", side_effect=lambda obj: (full_rodata, {".rodata": (0, len(full_rodata))}) if obj.name == "full.o" else (rodata, {".rodata": (0, len(rodata))})))
            stack.enter_context(patch.object(check_unit, "relocations", side_effect=lambda obj: {**relocs, ".rodata": {off: ("R_MIPS_32", ".text") for off in list(range(0,16,4))+list(range(32,48,4))}} if obj.name == "full.o" else relocs))
            stack.enter_context(patch.object(check_unit, "owns_rodata", return_value=True))
            stack.enter_context(patch.object(check_unit, "run", side_effect=lambda *args: "0 10 T target\n10 10 T neighbor\n" if "-S" in args else ""))
            stack.enter_context(patch.object(check_unit.subprocess, "run", return_value=SimpleNamespace(returncode=0, stderr="")))
            output = stack.enter_context(contextlib.redirect_stdout(io.StringIO()))
            with self.assertRaises(SystemExit) as stopped:
                check_unit.main()
            return stopped.exception.code, output.getvalue()

    def test_focus_preserves_adjacent_c_table_boundaries(self):
        for func in (None, "target", "neighbor"):
            with self.subTest(func=func):
                code, report = self.check_tables(func=func)
                self.assertEqual(code, 0, report)
                self.assertIn(f"{1 if func else 2} match, 0 differ", report)
                if func == "target":
                    self.assertNotIn("neighbor", report)

    def test_filtered_as_built_neighbor_still_ends_the_table(self):
        for func in (None, "target"):
            with self.subTest(func=func):
                code, report = self.check_tables(func=func, neighbor_as_built=True)
                self.assertEqual(code, 0, report)

    def test_focus_still_rejects_incorrect_selected_table_entry(self):
        code, report = self.check_tables(func="target", wrong_entry=True)
        self.assertEqual(code, 1, report)
        self.assertIn("DIFF jump table of target", report)
        self.assertIn("1 of 4 entries differ", report)

    def test_focus_does_not_truncate_excess_entries_to_retail_length(self):
        code, report = self.check_tables(func="target", extra_entry=True)
        self.assertEqual(code, 1, report)
        self.assertIn("DIFF jump table of target", report)
        self.assertIn("1 of 5 entries differ", report)


class SdataRelocationTests(unittest.TestCase):
    def test_resolves_local_text_and_external_pointers(self):
        item = struct.pack("<II", 0x24, 4)
        relocs = {
            0x10: ("R_MIPS_32", ".text"),
            0x14: ("R_MIPS_32", "externalCallback"),
        }
        funcs = [(0x20, 0x10, "localCallback")]
        syms = {"localCallback": 0x00101000, "externalCallback": 0x00202000}

        linked, problems = relocate_sdata_item(item, 0x10, relocs, funcs, syms)

        self.assertEqual(problems, [])
        self.assertEqual(struct.unpack("<II", linked), (0x00101004, 0x00202004))

    def test_rejects_unresolved_local_text_target(self):
        linked, problems = relocate_sdata_item(
            struct.pack("<I", 0x40), 0, {0: ("R_MIPS_32", ".text")},
            [(0x20, 0x10, "localCallback")], {"localCallback": 0x00101000})

        self.assertEqual(linked, struct.pack("<I", 0x40))
        self.assertEqual(problems, ["unresolved R_MIPS_32 .text at +0x0"])

    def test_rejects_unsupported_relocation(self):
        linked, problems = relocate_sdata_item(
            struct.pack("<I", 0), 0, {0: ("R_MIPS_GPREL16", "global")}, [],
            {"global": 0x00303000})

        self.assertEqual(linked, struct.pack("<I", 0))
        self.assertEqual(problems, ["unsupported R_MIPS_GPREL16 at +0x0"])

    def test_relocation_words_are_not_trimmed_as_zero_padding(self):
        item = b"\0" * 12
        relocs = {0x10: ("R_MIPS_32", ".text"), 0x14: ("R_MIPS_32", ".text")}

        self.assertEqual(trim_sdata_item(item, 0x10, relocs), b"\0" * 8)


class OwnedBssTests(unittest.TestCase):
    YAML = """\
segments:
  - name: main
    subsegments:
      - { start: 0x1000, type: .bss, vram: 0x003DC658, name: main/unit }
      - { start: 0x1000, type: bss, vram: 0x003DC698, name: bss }
"""

    def test_dict_form_unit_owns_exact_span(self):
        self.assertEqual(
            owned_bss_range(self.YAML, "main/unit"),
            (0x003DC658, 0x40),
        )
        self.assertEqual(owned_bss_size(self.YAML, "main/unit"), 0x40)

    def test_sbss_uses_the_same_exact_symbol_contract(self):
        sbss = self.YAML.replace(".bss", ".sbss", 1).replace(
            "main/unit", "main/thread"
        )
        self.assertEqual(
            owned_nobits_range(sbss, "main/thread", ".sbss"),
            (0x003DC658, 0x40),
        )
        self.assertEqual(
            owned_nobits_size(sbss, "main/thread", ".sbss"), 0x40
        )
        emitted = [(0x00, 0x40, "sdfTrackedThreadState", True)]
        self.assertEqual(
            nobits_ownership_problems(
                sbss, "main/thread", ".sbss", 0x40, emitted,
                {"sdfTrackedThreadState": 0x003DC658},
            ),
            [],
        )
        self.assertIsNone(
            owned_nobits_size(sbss, "main/thread", ".bss")
        )

    def test_rejects_non_nobits_section(self):
        with self.assertRaisesRegex(ValueError, "unsupported NOBITS section"):
            owned_nobits_range(self.YAML, "main/unit", ".data")
        with self.assertRaisesRegex(ValueError, "unsupported NOBITS section"):
            nobits_symbols("", ".data")

    def ownership_problems(self, emitted, syms=None, section_size=0x40):
        if syms is None:
            syms = {
                "unitHead": 0x003DC658,
                "fileManagerSlots": 0x003DC668,
            }
        return bss_ownership_problems(
            self.YAML, "main/unit", section_size, emitted, syms
        )

    def test_accepts_exact_named_object_layout(self):
        emitted = [
            (0x00, 0x10, "unitHead", True),
            (0x10, 0x30, "fileManagerSlots", True),
        ]
        self.assertEqual(self.ownership_problems(emitted), [])

    def test_explicit_native_extent_excludes_alignment_gap(self):
        emitted = [(0, 0xC, "unitHead", True),
                   (0x10, 0x30, "fileManagerSlots", True)]
        syms = {"unitHead": 0x003DC658,
                "fileManagerSlots": 0x003DC668}
        self.assertEqual(nobits_ownership_problems(
            self.YAML, "main/unit", ".bss", 0x40, emitted, syms,
            explicit_sizes={"unitHead": 0xC}), [])
        self.assertTrue(nobits_ownership_problems(
            self.YAML, "main/unit", ".bss", 0x40, emitted, syms))

    def test_explicit_native_extent_must_fit_its_address_interval(self):
        emitted = [(0, 0x10, "unitHead", True),
                   (0x10, 0x30, "fileManagerSlots", True)]
        syms = {"unitHead": 0x003DC658,
                "fileManagerSlots": 0x003DC668}
        for size in (0, -1, 0x11):
            with self.subTest(size=size):
                self.assertTrue(nobits_ownership_problems(
                    self.YAML, "main/unit", ".bss", 0x40,
                    emitted, syms, explicit_sizes={"unitHead": size}))

    def test_same_address_names_share_one_explicit_extent(self):
        emitted = [(0, 0xC, "unitHead", True),
                   (0, 0xC, "headAlias", True),
                   (0x10, 0x30, "fileManagerSlots", True)]
        syms = {"unitHead": 0x003DC658, "headAlias": 0x003DC658,
                "fileManagerSlots": 0x003DC668}
        self.assertEqual(nobits_ownership_problems(
            self.YAML, "main/unit", ".bss", 0x40, emitted, syms,
            explicit_sizes={"unitHead": 0xC}), [])
        self.assertTrue(nobits_ownership_problems(
            self.YAML, "main/unit", ".bss", 0x40, emitted, syms,
            explicit_sizes={"unitHead": 0xC, "headAlias": 0x10}))

    def test_interior_alias_cannot_enter_alignment_gap(self):
        emitted = [(0, 0xC, "unitHead", True),
                   (0x10, 0x30, "fileManagerSlots", True)]
        syms = {"unitHead": 0x003DC658,
                "fileManagerSlots": 0x003DC668}
        alias = SymbolAlias("gapAlias", "unitHead", "main/unit",
                            ".bss", 0xC, 0x003DC664)
        self.assertTrue(nobits_ownership_problems(
            self.YAML, "main/unit", ".bss", 0x40, emitted, syms,
            (alias,), {"unitHead": 0xC}))

    def test_absolute_linker_gp_does_not_split_owned_objects(self):
        emitted = [
            (0x00, 0x10, "unitHead", True),
            (0x10, 0x30, "fileManagerSlots", True),
        ]
        syms = {
            "unitHead": 0x003DC658,
            "fileManagerSlots": 0x003DC668,
            "_gp": 0x003DC678,
        }
        self.assertEqual(self.ownership_problems(emitted, syms), [])

    def test_rejects_same_size_wrong_name(self):
        emitted = [(0x00, 0x40, "wrongManagerWork", True)]
        problems = self.ownership_problems(
            emitted, {"unitWork": 0x003DC658}
        )
        self.assertIn("extra emitted symbol wrongManagerWork", problems)
        self.assertIn(
            "missing retail symbol unitWork at +0x0 size 0x40", problems
        )

    def test_rejects_wrong_symbol_order(self):
        emitted = [
            (0x00, 0x30, "fileManagerSlots", True),
            (0x30, 0x10, "unitHead", True),
        ]
        problems = self.ownership_problems(emitted)
        self.assertIn(
            "fileManagerSlots is at +0x0, retail inventory requires +0x10",
            problems,
        )
        self.assertIn(
            "unitHead is at +0x30, retail inventory requires +0x0",
            problems,
        )

    def test_rejects_wrong_offset_and_individual_size(self):
        wrong_offset = [
            (0x00, 0x14, "unitHead", True),
            (0x14, 0x2C, "fileManagerSlots", True),
        ]
        problems = self.ownership_problems(wrong_offset)
        self.assertIn(
            "unitHead has size 0x14, retail inventory requires 0x10",
            problems,
        )
        self.assertIn(
            "fileManagerSlots is at +0x14, retail inventory requires +0x10",
            problems,
        )

        wrong_size = [
            (0x00, 0x0C, "unitHead", True),
            (0x10, 0x30, "fileManagerSlots", True),
        ]
        problems = self.ownership_problems(wrong_size)
        self.assertIn(
            "unitHead has size 0xC, retail inventory requires 0x10",
            problems,
        )

    def test_rejects_non_object_and_wrong_retail_address(self):
        emitted = [(0x00, 0x40, "unitWork", False)]
        problems = self.ownership_problems(
            emitted, {"unitWork": 0x003DC65C}
        )
        self.assertIn("unitWork is not STT_OBJECT", problems)
        self.assertIn(
            "retail inventory begins at +0x4, expected +0x0",
            problems,
        )
        self.assertIn(
            "unitWork is at +0x0, retail inventory requires +0x4",
            problems,
        )

    def test_rejects_anonymous_padding_overlap_and_section_size(self):
        gap = [(0x04, 0x3C, "unitWork", True)]
        problems = self.ownership_problems(
            gap, {"unitWork": 0x003DC65C}
        )
        self.assertIn(
            "retail inventory begins at +0x4, expected +0x0",
            problems,
        )

        overlap = [
            (0x00, 0x20, "unitHead", True),
            (0x10, 0x30, "fileManagerSlots", True),
        ]
        problems = self.ownership_problems(overlap)
        self.assertIn(
            "unitHead has size 0x20, retail inventory requires 0x10",
            problems,
        )
        self.assertIn(
            "section size 0x44, retail span 0x40",
            self.ownership_problems(
                [(0x00, 0x40, "unitWork", True)],
                {"unitWork": 0x003DC658},
                section_size=0x44,
            ),
        )

    def test_rejects_omitted_interior_retail_symbol(self):
        syms = {
            "unitHead": 0x003DC658,
            "fileManagerInterior": 0x003DC668,
            "fileManagerTail": 0x003DC678,
        }
        emitted = [
            (0x00, 0x20, "unitHead", True),
            (0x20, 0x20, "fileManagerTail", True),
        ]

        problems = self.ownership_problems(emitted, syms)

        self.assertIn(
            "unitHead has size 0x20, retail inventory requires 0x10",
            problems,
        )
        self.assertIn(
            "missing retail symbol fileManagerInterior at +0x10 size 0x10",
            problems,
        )

    def test_requires_aliases_one_for_one(self):
        syms = {
            "unitHead": 0x003DC658,
            "unitHeadAlias": 0x003DC658,
            "fileManagerSlots": 0x003DC668,
        }
        exact = [
            (0x00, 0x10, "unitHeadAlias", True),
            (0x10, 0x30, "fileManagerSlots", True),
            (0x00, 0x10, "unitHead", True),
        ]
        self.assertEqual(self.ownership_problems(exact, syms), [])

        missing_alias = exact[1:]
        self.assertIn(
            "missing retail symbol unitHeadAlias at +0x0 size 0x10",
            self.ownership_problems(missing_alias, syms),
        )

        extra_alias = [*exact, (0x00, 0x10, "inventedAlias", True)]
        self.assertIn(
            "extra emitted symbol inventedAlias",
            self.ownership_problems(extra_alias, syms),
        )

    def test_rejects_duplicate_emitted_name_and_empty_inventory(self):
        duplicate = [
            (0x00, 0x10, "unitHead", True),
            (0x00, 0x10, "unitHead", True),
            (0x10, 0x30, "fileManagerSlots", True),
        ]
        self.assertIn(
            "duplicate symbol unitHead",
            self.ownership_problems(duplicate),
        )
        self.assertIn(
            "retail span has no symbol_addrs inventory",
            self.ownership_problems([], {}),
        )

    def test_paired_nobits_inventories_remain_accepted(self):
        syms = {"unitWork": 0x003DC658, "unitRequests": 0x003DC698}
        self.assertEqual(
            bss_ownership_problems(
                self.YAML, "main/unit", 0x40,
                [(0, 0x40, "unitWork", True)], syms,
            ),
            [],
        )
        yaml_text = self.YAML.replace(".bss", ".sbss", 1).replace("0x003DC698", "0x003DC660")
        syms = {"unitState": 0x003DC658, "unitHead": 0x003DC65C, "unitNext": 0x003DC660}
        self.assertEqual(
            nobits_ownership_problems(
                yaml_text, "main/unit", ".sbss", 8,
                [(0, 4, "unitState", True), (4, 4, "unitHead", True)], syms,
            ),
            [],
        )

    def test_parses_bss_object_type_from_objdump(self):
        table = """\
SYMBOL TABLE:
00000000 l    d  .bss  00000000 .bss
00000000 g     O .bss  00000010 unitHead
00000010 l       .bss  00000030 fileManagerSlots
00000000 g     O .data 00000004 initialized
"""
        self.assertEqual(
            bss_symbols(table),
            [
                (0x00, 0x10, "unitHead", True),
                (0x10, 0x30, "fileManagerSlots", False),
            ],
        )

    def test_parses_sbss_symbols_independently(self):
        table = """\
SYMBOL TABLE:
00000000 l    d  .sbss 00000000 .sbss
00000000 g     O .sbss 00000008 sdfTrackedThreadState
00000000 g     O .bss  00000040 unitWork
"""
        self.assertEqual(
            nobits_symbols(table, ".sbss"),
            [(0x00, 0x08, "sdfTrackedThreadState", True)],
        )

    def test_list_form_or_wrong_unit_does_not_claim_bss(self):
        list_form = """\
segments:
  - [0x1000, .bss, main/unit]
  - { start: 0x1000, type: bss, vram: 0x003DC698, name: bss }
"""
        self.assertIsNone(owned_bss_size(list_form, "main/unit"))
        self.assertIsNone(owned_bss_size(self.YAML, "file/anotherUnit"))
        self.assertIsNone(owned_bss_size(
            self.YAML.replace("type: .bss", "type: bss", 1),
            "main/unit",
        ))

    def test_requires_immediate_explicit_next_vram_boundary(self):
        missing_boundary = """\
segments:
  - name: main
    subsegments:
      - { start: 0x1000, type: .bss, vram: 0x003DC658, name: main/unit }
      - [0x1000, bin, trailer]
      - { start: 0x1000, type: bss, vram: 0x003DC698, name: bss }
"""
        self.assertIsNone(
            owned_bss_size(missing_boundary, "main/unit")
        )

        crossed_parent = """\
segments:
  - name: main
    subsegments:
      - { start: 0x1000, type: .bss, vram: 0x003DC658, name: main/unit }
  - name: another
    subsegments:
      - { start: 0x2000, type: bss, vram: 0x003DC698, name: bss }
"""
        self.assertIsNone(owned_bss_size(crossed_parent, "main/unit"))

    def test_rejects_duplicate_or_nonadvancing_claims(self):
        duplicate = self.YAML + """\
      - { start: 0x1000, type: .bss, vram: 0x003DC700, name: main/unit }
      - { start: 0x1000, type: bss, vram: 0x003DC740, name: bss }
"""
        backwards = self.YAML.replace("0x003DC698", "0x003DC650")
        self.assertIsNone(owned_bss_size(duplicate, "main/unit"))
        self.assertIsNone(owned_bss_size(backwards, "main/unit"))

    def test_common_symbols_are_kept_out_of_owned_bss(self):
        nm = """\
00000004 00000040 c unitWork
00000000 00000040 B explicitFileManagerWork
00000080 00000008 C ordinaryCommon
"""
        self.assertEqual(
            common_symbols(nm),
            [("unitWork", 0x40), ("ordinaryCommon", 8)],
        )


class SectionOwnershipTests(unittest.TestCase):
    SYMBOLS = """\
00000000 g     O .sdata 00000008 D_003BC7D8
00000008 g     O .sdata 00000008 D_003BC7E0
00000000 g     O .data  00000008 unrelated
00000000         *UND*  00000040 unitWork
"""

    def test_parses_only_requested_section(self):
        self.assertEqual(
            section_symbols(self.SYMBOLS, ".sdata"),
            [(0, 8, "D_003BC7D8"), (8, 8, "D_003BC7E0")],
        )

    def test_requires_exact_extent_and_retail_address(self):
        definitions = section_symbols(self.SYMBOLS, ".sdata")
        syms = {"D_003BC7D8": 0x003BC7D8, "D_003BC7E0": 0x003BC7E0}

        self.assertTrue(owns_exact_section_item(
            definitions, syms, 0, 8, 0x003BC7D8))
        self.assertFalse(owns_exact_section_item(
            definitions, syms, 0, 4, 0x003BC7D8))
        self.assertFalse(owns_exact_section_item(
            definitions, syms, 0, 8, 0x003BC7E0))
        self.assertEqual(source_owned_item_size(
            definitions, syms, 0, 0x003BC7D8), 8)
        self.assertIsNone(source_owned_item_size(
            definitions, syms, 0, 0x003BC7E0))


class InitializedDataTests(unittest.TestCase):
    ORIGIN = 0x00400000
    RANGE = (0x1000, 0x1008)

    def problems(self, *, section_range=RANGE, section_origin=ORIGIN,
                 retail=b"\x01\x02\x03\x04\x05\x06\x07\x08",
                 data=b"\x01\x02\x03\x04\x05\x06\x07\x08",
                 definitions=((0, 8, "consoleUv"),), relocs=None,
                 full_data=None, full_definitions=None, full_relocs=None,
                 syms=None, section_bases=None):
        if full_data is None:
            full_data = data
        if full_definitions is None:
            full_definitions = definitions
        if relocs is None:
            relocs = {}
        if full_relocs is None:
            full_relocs = relocs
        if syms is None:
            syms = {"consoleUv": self.ORIGIN}
        return initialized_data_problems(
            section_range, section_origin, retail,
            data, list(definitions), relocs, [],
            full_data, list(full_definitions), full_relocs, [], syms,
            section_bases or {})

    def test_accepts_exact_object_and_native_payload(self):
        self.assertEqual(self.problems(), [])

    def test_resolves_native_supported_data_relocation(self):
        native_pointer = struct.pack("<I", self.ORIGIN)
        self.assertEqual(self.problems(
            retail=native_pointer + b"\0" * 4, data=b"\0" * 4,
            definitions=((0, 4, "consoleUv"),),
            relocs={0: ("R_MIPS_32", ".data")},
            full_data=b"\0" * 4, full_relocs={0: ("R_MIPS_32", ".data")},
            syms={"consoleUv": self.ORIGIN},
            section_bases={".data": self.ORIGIN}), [])

    def test_rejects_unowned_or_ambiguous_yaml_extent(self):
        self.assertTrue(any("no unambiguous YAML-owned" in p
                            for p in self.problems(section_range=None)))

    def test_rejects_unknown_or_misplaced_object(self):
        unknown = self.problems(syms={})
        self.assertTrue(any("no unambiguous native symbol" in p for p in unknown))
        misplaced = self.problems(
            definitions=((4, 4, "consoleUv"),),
            full_definitions=((4, 4, "consoleUv"),),
            data=b"\0" * 4 + b"\x05\x06\x07\x08",
            full_data=b"\0" * 4 + b"\x05\x06\x07\x08")
        self.assertTrue(any("does not match native address" in p for p in misplaced))

    def test_rejects_ambiguous_or_overlapping_object_inventory(self):
        duplicate = self.problems(
            definitions=((0, 4, "consoleUv"), (4, 4, "consoleUv")),
            full_definitions=((0, 4, "consoleUv"), (4, 4, "consoleUv")))
        self.assertTrue(any("duplicate STT_OBJECT names" in p for p in duplicate))
        overlap = self.problems(
            definitions=((0, 8, "consoleUv"), (0, 4, "otherUv")),
            full_definitions=((0, 8, "consoleUv"), (0, 4, "otherUv")),
            syms={"consoleUv": self.ORIGIN, "otherUv": self.ORIGIN})
        self.assertTrue(any("overlapping STT_OBJECT" in p for p in overlap))

    def test_rejects_overlong_or_different_payload(self):
        overlong = self.problems(
            data=b"\x01" * 12, full_data=b"\x01" * 12,
            definitions=((0, 12, "consoleUv"),),
            full_definitions=((0, 12, "consoleUv"),))
        self.assertTrue(any("exceeds its 0x8-byte" in p for p in overlong))
        changed = self.problems(data=b"\x09\x02\x03\x04\x05\x06\x07\x08")
        self.assertTrue(any("bytes/relocations differ" in p for p in changed))

    def test_rejects_unowned_bytes_and_unresolvable_relocations(self):
        unowned = self.problems(
            definitions=((0, 4, "consoleUv"),),
            full_definitions=((0, 4, "consoleUv"),),
            data=b"\x01\x02\x03\x04\x00\x00\x00\x09",
            full_data=b"\x01\x02\x03\x04\x00\x00\x00\x09")
        self.assertTrue(any("unowned nonzero byte" in p for p in unowned))
        unresolved = self.problems(
            data=b"\0" * 8,
            relocs={0: ("R_MIPS_32", "missingTable")},
            full_data=b"\0" * 8,
            full_relocs={0: ("R_MIPS_32", "missingTable")})
        self.assertTrue(any("unresolved R_MIPS_32" in p for p in unresolved))
        unsupported = self.problems(
            relocs={0: ("R_MIPS_26", "consoleUv")},
            full_relocs={0: ("R_MIPS_26", "consoleUv")})
        self.assertTrue(any("unsupported R_MIPS_26" in p for p in unsupported))

    def test_requires_same_full_unit_object_extent(self):
        problems = self.problems(
            full_definitions=((0, 4, "consoleUv"),),
            full_data=b"\x01\x02\x03\x04\0\0\0\0")
        self.assertTrue(any("STT_OBJECT inventories differ" in p for p in problems))


class CompilerLocalFunctionAliasTests(unittest.TestCase):
    def aliases(self, funcs=None, metadata=None, syms=None, function_names=None):
        return check_unit.compiler_local_function_aliases(
            "00000000 l F .text 00000008 helper.0\n" if metadata is None else metadata,
            [(0, 8, "helper.0")] if funcs is None else funcs,
            {"helper": 0x1000} if syms is None else syms,
            {"helper"} if function_names is None else function_names)

    def test_unique_local_function_gets_exact_canonical_address(self):
        self.assertEqual(self.aliases(), {"helper.0": 0x1000})
        self.assertIsNone(check_unit.address("helper.0", {"helper": 0x1000}))

    def test_explicit_suffixed_symbol_wins(self):
        syms = {"helper": 0x1000, "helper.0": 0x2000}
        self.assertEqual(self.aliases(syms=syms), {})
        self.assertEqual(check_unit.address("helper.0", syms), 0x2000)

    def test_plain_peer_other_suffix_and_duplicate_are_ambiguous(self):
        for peer in ((8, 8, "helper"), (8, 8, "helper.1"), (0, 8, "helper.0")):
            with self.subTest(peer=peer):
                self.assertEqual(self.aliases(funcs=[(0, 8, "helper.0"), peer]), {})

    def test_zero_size_definition_peers_are_ambiguous_even_when_nm_omits_them(self):
        for peer in ("helper", "helper.1"):
            for flags in ("l F", "g F"):
                with self.subTest(peer=peer, flags=flags):
                    self.assertEqual(self.aliases(metadata=(
                        "0 l F .text 8 helper.0\n"
                        f"8 {flags} .text 0 {peer}\n")), {})

    def test_exact_suffixed_address_survives_zero_size_peer(self):
        syms = {"helper": 0x1000, "helper.0": 0x2000}
        self.assertEqual(self.aliases(
            metadata="0 l F .text 8 helper.0\n8 g F .text 0 helper\n",
            syms=syms), {})
        self.assertEqual(check_unit.address("helper.0", syms), 0x2000)

    def test_requires_an_explicit_retail_function_stem(self):
        for syms, names in (({}, {"helper"}), ({"helper": 0x1000}, set())):
            with self.subTest(syms=syms, names=names):
                self.assertEqual(self.aliases(syms=syms, function_names=names), {})

    def test_requires_local_function_type_text_section_and_matching_extent(self):
        for row in (
            "", "0 g F .text 8 helper.0", "0 l O .text 8 helper.0",
            "0 l F .data 8 helper.0", "4 l F .text 8 helper.0",
            "0 l F .text 4 helper.0", "0 l F .text 0 helper.0",
            "0 l F .text 8 helper.0\n0 l F .text 8 helper.0",
        ):
            with self.subTest(row=row):
                self.assertEqual(self.aliases(metadata=row), {})

    def test_nonnumeric_and_compound_clone_suffixes_remain_unresolved(self):
        for name in ("helper.isra.0", "helper.clone", "helper.", "helper.0x1"):
            with self.subTest(name=name):
                self.assertEqual(self.aliases(
                    funcs=[(0, 8, name)], metadata=f"0 l F .text 8 {name}"), {})


class NestedFunctionCheckerTests(unittest.TestCase):
    def run_checker(self, requested=None, *, helper_word=0x03E00008,
                    full_helper_word=None, retail_call=0x0C000400,
                    metadata_flags="l F", full_name="helper.0",
                    full_flags="l F", extra_full=(), extra_skip=(),
                    explicit_address=None, candidate_source=None, native_inventory=None):
        """Run the complete checker with fake objects; never invoke cc.sh."""
        compiled = struct.pack("<IIII", helper_word, 0, 0x0C000000, 0)
        full_code = struct.pack("<IIII", helper_word if full_helper_word is None
                                else full_helper_word, 0, 0x0C000000, 0)
        retail = struct.pack("<IIII", 0x03E00008, 0, retail_call, 0)
        skip_funcs = [(0, 8, "helper.0"), (8, 8, "caller"), *extra_skip]
        full_funcs = [(0, 8, full_name), (8, 8, "caller"), *extra_full]
        with tempfile.TemporaryDirectory() as tmp, contextlib.ExitStack() as stack:
            root = Path(tmp)
            unit = root / "src/smt3/main/unit.c"
            unit.parent.mkdir(parents=True)
            unit.write_text("void caller(void) {\n    void helper(void) {}\n}\n")
            config = root / "config/smt3"
            config.mkdir(parents=True)
            declarations = "helper = 0x1000; // type:func\ncaller = 0x1008; // type:func\n"
            if explicit_address is not None:
                declarations += f"helper.0 = 0x{explicit_address:X}; // type:func\n"
            (config / "symbol_addrs.txt").write_text(declarations)
            (config / "SLUS_209.11.yaml").write_text("")
            original = root / check_unit.RETAIL["smt3"]
            original.parent.mkdir(parents=True)
            original.write_bytes(retail)
            argv = ["check_unit.py", str(unit)]
            if candidate_source is not None:
                candidate = root / "candidate.c"
                candidate.write_text(candidate_source)
                argv.extend(["--source", str(candidate)])
            if native_inventory is not None:
                assembly = root / "asm/smt3/main/unit.s"
                assembly.parent.mkdir(parents=True)
                assembly.write_text(native_inventory)
            if requested is not None:
                argv.extend(["--func", requested])
            stack.enter_context(patch.object(check_unit, "ROOT", root))
            stack.enter_context(patch.object(check_unit.sys, "argv", argv))
            stack.enter_context(patch("eeas_compat.lit4_range", return_value=(0, 0)))
            stack.enter_context(patch.object(check_unit, "load_segments", return_value=[]))
            stack.enter_context(patch.object(check_unit, "va_to_off", side_effect=lambda segments, addr: addr - 0x1000))
            stack.enter_context(patch.object(check_unit, "text_section", side_effect=lambda obj: full_code if str(obj).endswith("full.o") else compiled))
            stack.enter_context(patch.object(check_unit, "sections", return_value=(b"", {})))
            stack.enter_context(patch.object(check_unit, "relocations", return_value={
                ".text": {8: ("R_MIPS_26", ".text")}, ".rodata": {}, ".sdata": {}}))

            def object_output(*args):
                full = str(args[-1]).endswith("full.o")
                functions = full_funcs if full else skip_funcs
                if "-S" in args:
                    # Real nm -S omits the size column for zero-size symbols.
                    return "".join(
                        f"{offset:x} " + (f"{size:x} " if size else "")
                        + f"{'T' if name == 'caller' else 't'} {name}\n"
                        for offset, size, name in functions)
                if "-t" in args:
                    flags = full_flags if full else metadata_flags
                    return "".join(f"{offset:x} {'g F' if name == 'caller' else flags} .text {size:x} {name}\n"
                                   for offset, size, name in functions)
                return ""

            stack.enter_context(patch.object(check_unit, "run", side_effect=object_output))
            stack.enter_context(patch.object(check_unit.subprocess, "run", return_value=SimpleNamespace(returncode=0, stderr="")))
            output = stack.enter_context(contextlib.redirect_stdout(io.StringIO()))
            with self.assertRaises(SystemExit) as stopped:
                check_unit.main()
            return stopped.exception.code, output.getvalue()

    def test_whole_unit_resolves_nested_body_and_local_call(self):
        status, report = self.run_checker()
        self.assertEqual(status, 0, report)
        self.assertIn("OK   helper.0 @ 0x00001000", report)
        self.assertIn("OK   caller @ 0x00001008", report)
        self.assertIn("2 match, 0 differ", report)

    def test_qualified_local_alias_supplies_full_native_inventory(self):
        status, report = self.run_checker(native_inventory="glabel helper\nglabel caller\n")
        self.assertEqual(status, 0, report)
        self.assertIn("2 match, 0 differ", report)

    def test_source_override_missing_owner_is_not_hidden_by_canonical_source(self):
        status, report = self.run_checker(
            candidate_source="", native_inventory="glabel caller\n")
        self.assertEqual(status, 1, report)
        self.assertIn("MISSING caller: neither C nor INCLUDE_ASM", report)

    def test_source_override_audits_candidate_order(self):
        status, report = self.run_checker(candidate_source=(
            "void caller(void)\n{\n}\nvoid helper(void)\n{\n}\n"))
        self.assertEqual(status, 1, report)
        self.assertIn("ORDER helper", report)

    def test_source_override_audits_candidate_stale_names(self):
        status, report = self.run_checker(candidate_source=(
            "void caller(void) {}\n/* func_00001000 */\n"))
        self.assertEqual(status, 1, report)
        self.assertIn("STALE func_00001000", report)

    def test_focused_candidate_need_not_retain_unselected_owner(self):
        status, report = self.run_checker(
            "caller", candidate_source="void caller(void) {}\n",
            native_inventory="glabel helper\nglabel caller\n")
        self.assertEqual(status, 0, report)

    def test_focused_selection_still_requires_exact_emitted_name(self):
        status, report = self.run_checker("helper.0")
        self.assertEqual(status, 0, report)
        self.assertIn("1 match, 0 differ", report)
        status, report = self.run_checker("helper")
        self.assertEqual(status, 1, report)
        self.assertIn("MISSING helper: requested function not emitted", report)

    def test_real_helper_difference_is_not_masked(self):
        status, report = self.run_checker(helper_word=0x03E00009)
        self.assertEqual(status, 1, report)
        self.assertIn("DIFF helper.0", report)
        self.assertIn("1 match, 1 differ", report)

    def test_real_call_target_difference_is_not_masked(self):
        status, report = self.run_checker(retail_call=0x0C000402)
        self.assertEqual(status, 1, report)
        self.assertIn("DIFF caller", report)
        self.assertIn("retail calls a different function", report)

    def test_full_unit_helper_difference_keeps_context_failure(self):
        status, report = self.run_checker(full_helper_word=0x03E00009)
        self.assertEqual(status, 1, report)
        self.assertIn("CONTEXT helper.0", report)
        self.assertIn("full unit vs retail: 1 of 2 words differ", report)

    def test_both_objects_must_prove_unique_local_identity(self):
        for arguments in (
            {"metadata_flags": "g F"}, {"metadata_flags": "l O"},
            {"full_flags": "g F"}, {"full_name": "helper.1"},
            {"extra_full": [(0, 8, "helper.1")]},
            {"extra_skip": [(0, 8, "helper.1")]},
            {"extra_full": [(0, 8, "helper")]},
            {"extra_skip": [(0, 8, "helper")]},
        ):
            with self.subTest(arguments=arguments):
                status, report = self.run_checker(**arguments)
                self.assertEqual(status, 1, report)
                self.assertIn("helper.0: no address", report)

    def test_zero_size_peer_in_either_object_blocks_alias_and_call_resolution(self):
        for context in ("extra_skip", "extra_full"):
            for peer in ("helper", "helper.1"):
                with self.subTest(context=context, peer=peer):
                    status, report = self.run_checker(**{context: [(8, 0, peer)]})
                    self.assertEqual(status, 1, report)
                    self.assertIn("helper.0: no address", report)
                    self.assertIn("DIFF caller", report)
                    self.assertIn("CONTEXT RELOC caller", report)
                    self.assertIn("0 match, 3 differ", report)

    def test_explicit_symbol_mapping_is_not_overridden_by_stem(self):
        status, report = self.run_checker(explicit_address=0x1008)
        self.assertEqual(status, 1, report)
        self.assertIn("DIFF helper.0 @ 0x00001008", report)


class DefaultGuardScan(unittest.TestCase):
    def test_fix_bugs_c_is_excluded_but_retail_fallback_is_kept(self):
        text = "#ifdef FIX_BUGS\nvoid f(void) {}\n#else\nINCLUDE_ASM(x, \"u\", f);\n#endif"
        result = check_unit.drop_nonmatching(text)
        self.assertNotIn("void f", result)
        self.assertIn("INCLUDE_ASM", result)

    def test_nested_else_does_not_end_disabled_outer_branch(self):
        text = "#ifdef NON_MATCHING\n#if INNER\nfirst\n#else\nsecond\n#endif\nlast\n#else\nretail\n#endif"
        self.assertEqual(check_unit.drop_nonmatching(text), "retail")

    def test_known_ifndef_keeps_default_and_discards_else(self):
        text = "#ifndef FIX_BUGS\nretail\n#else\ncorrected\n#endif"
        self.assertEqual(check_unit.drop_nonmatching(text), "retail")

    def test_unknown_conditions_do_not_hide_duplicate_definitions(self):
        text = "#if UNKNOWN\nvoid f(void) {}\n#else\nvoid f(void) {}\n#endif"
        self.assertEqual(check_unit.drop_nonmatching(text).count("void f"), 2)

    def test_commented_directive_cannot_hide_live_fallback(self):
        text = '/*\n#ifdef FIX_BUGS\n*/\nINCLUDE_ASM(x, "u", f);'
        self.assertIn("INCLUDE_ASM", check_unit.drop_nonmatching(text))

    def test_commented_endif_cannot_expose_disabled_c(self):
        text = "#ifdef FIX_BUGS\n/*\n#endif\n*/\nvoid f(void) {}\n#else\nretail\n#endif"
        self.assertEqual(check_unit.drop_nonmatching(text), "retail")

    def test_nested_guard_preserves_line_numbers(self):
        text = "first\n#ifdef FIX_BUGS\n#if INNER\nhidden\n#endif\n#else\nretail\n#endif\nlast"
        result = check_unit.drop_nonmatching(text, preserve_lines=True)
        self.assertEqual(len(result.splitlines()), len(text.splitlines()))
        self.assertEqual(result.splitlines()[6], "retail")
        self.assertNotIn("hidden", result)


class OwnedRodataBoundaryTests(unittest.TestCase):
    def check_owned_neighbor(self, *, object_offset=48, wrong_object=False,
                             nonzero_gap=False, wrong_entry=False, omit_symbol=False):
        # Nine jump entries end at36. A verified16-byte C object at48
        # supplies the alignment gap, even when only another TU references it.
        compiled = struct.pack('<4I', 0x3C020000, 0x24420000, 0x03E00008, 0)
        native = struct.pack('<4I', 0x3C020010, 0x24420100, 0x03E00008, 0)
        table = [0, 4, 8, 12, 0, 4, 8, 12, 0]
        data = bytearray(64)
        struct.pack_into('<9I', data, 0, *table)
        payload = b'OWNED CONSTANT\0\0'
        data[object_offset:object_offset+16] = payload
        if wrong_object:
            data[object_offset] ^= 1
        if nonzero_gap:
            data[40] = 1
        if wrong_entry:
            struct.pack_into('<I', data, 0, 4)
        retail = bytearray(0x1200)
        retail[0x1000:0x1010] = native
        struct.pack_into('<9I', retail, 0x1100, *(0x100000+x for x in table))
        retail[0x1130:0x1140] = payload
        relocs = {'.text': {0: ('R_MIPS_HI16', '.rodata'), 4: ('R_MIPS_LO16', '.rodata')},
                  '.rodata': {x: ('R_MIPS_32', '.text') for x in range(0,36,4)},
                  '.sdata': {}, '.data': {}}
        with tempfile.TemporaryDirectory() as tmp, contextlib.ExitStack() as stack:
            root = Path(tmp)
            unit = root/'src/smt3/main/unit.c'; unit.parent.mkdir(parents=True)
            unit.write_text('void target(void) {}\nconst char title[16] = "OWNED CONSTANT";\n')
            cfg = root/'config/smt3'; cfg.mkdir(parents=True)
            (cfg/'symbol_addrs.txt').write_text('target = 0x100000; // type:func\ntitle = 0x100130;\n')
            (cfg/'SLUS_209.11.yaml').write_text('      - [0x1100, .rodata, main/unit]\n      - [0x1140, .rodata, main/next]\n')
            original = root/RETAIL_PATH; original.parent.mkdir(parents=True); original.write_bytes(retail)
            assembly = root/'asm/smt3/data/main/unit.rodata.s'; assembly.parent.mkdir(parents=True)
            assembly.write_text('dlabel jtbl_00100100\ndlabel title\n')
            def run(*args):
                if '-S' in args:
                    return '0 10 T target\n'
                if '-t' in args and not omit_symbol:
                    return f'{object_offset:08x} g O .rodata 00000010 title\n'
                return ''
            stack.enter_context(patch.object(check_unit, 'ROOT', root))
            stack.enter_context(patch.dict(check_unit.RETAIL, {VERSION: RETAIL_PATH}, clear=True))
            stack.enter_context(patch.dict(check_unit.VERSIONS, {VERSION: {'serial': SERIAL, 'gp': hex(GP)}}, clear=True))
            stack.enter_context(patch.object(check_unit.sys, 'argv', ['check_unit.py', str(unit)]))
            stack.enter_context(patch('eeas_compat.lit4_range', return_value=(0, 0)))
            stack.enter_context(patch.object(check_unit, 'load_segments', return_value=[]))
            stack.enter_context(patch.object(check_unit, 'va_to_off', side_effect=lambda segs, addr: addr-0xFF000))
            stack.enter_context(patch.object(check_unit, 'text_section', return_value=compiled))
            stack.enter_context(patch.object(check_unit, 'sections', return_value=(data, {'.rodata': (0,len(data))})))
            stack.enter_context(patch.object(check_unit, 'relocations', return_value=relocs))
            stack.enter_context(patch.object(check_unit, 'owns_rodata', return_value=True))
            stack.enter_context(patch.object(check_unit, 'run', side_effect=run))
            stack.enter_context(patch.object(check_unit.subprocess, 'run', return_value=SimpleNamespace(returncode=0, stderr='')))
            output = stack.enter_context(contextlib.redirect_stdout(io.StringIO()))
            with self.assertRaises(SystemExit) as stopped:
                check_unit.main()
            return stopped.exception.code, output.getvalue()

    def test_verified_owned_neighbor_supplies_alignment_gap(self):
        code, report = self.check_owned_neighbor()
        self.assertEqual(code, 0, report)
        self.assertIn('1 match, 0 differ', report)

    def test_invalid_owned_neighbors_do_not_hide_padding_or_data_errors(self):
        for args in ({'object_offset':40}, {'wrong_object':True}, {'nonzero_gap':True},
                     {'wrong_entry':True}, {'omit_symbol':True}):
            with self.subTest(args=args):
                code, report = self.check_owned_neighbor(**args)
                self.assertEqual(code, 1, report)


class InteriorOwnedContextTests(unittest.TestCase):
    def check_interior(self, *, object_offset=24, wrong_object=False,
                             nonzero_gap=False, wrong_entry=False, omit_symbol=False, full_offset=24, full_size=8, skip_size=8, full_missing=False, relocated=False, full_relocated=False, native_nonzero=False, full_relocation=None, full_data_size=None):
        # Six entries end at24; verified zero object spans24..32.
        # Raw retail labels omit the interior object but include nextData at32.
        compiled = struct.pack('<4I', 0x3C020000, 0x24420000, 0x03E00008, 0)
        native = struct.pack('<4I', 0x3C020010, 0x24420100, 0x03E00008, 0)
        table = [0, 4, 8, 12, 0, 4]
        data = bytearray(40)
        struct.pack_into('<6I', data, 0, *table)
        payload = bytes(8)
        data[object_offset:object_offset+8] = payload
        if wrong_object:
            data[object_offset] ^= 1
        if nonzero_gap:
            data[28] = 1
        if wrong_entry:
            struct.pack_into('<I', data, 0, 4)
        retail = bytearray(0x1200)
        retail[0x1000:0x1010] = native
        struct.pack_into('<6I', retail, 0x1100, *(0x100000+x for x in table))
        retail[0x1118:0x1120] = payload
        data[32:40] = retail[0x1120:0x1128] = b'NEXTDATA'
        if native_nonzero:
            retail[0x111c] = 1
        relocs = {'.text': {0: ('R_MIPS_HI16', '.rodata'), 4: ('R_MIPS_LO16', '.rodata')},
                  '.rodata': {x: ('R_MIPS_32', '.text') for x in range(0,24,4)},
                  '.sdata': {}, '.data': {}}
        if relocated:
            relocs['.rodata'][24] = ('R_MIPS_32', 'unknown')
        with tempfile.TemporaryDirectory() as tmp, contextlib.ExitStack() as stack:
            root = Path(tmp)
            unit = root/'src/smt3/main/unit.c'; unit.parent.mkdir(parents=True)
            unit.write_text('void target(void) {}\nconst char title[8] = {0};\nconst char nextData[8] = "NEXTDATA";\n')
            cfg = root/'config/smt3'; cfg.mkdir(parents=True)
            (cfg/'symbol_addrs.txt').write_text('target = 0x100000; // type:func\ntitle = 0x100118;\nnextData = 0x100120;\n')
            (cfg/'SLUS_209.11.yaml').write_text('      - [0x1100, .rodata, main/unit]\n      - [0x1128, .rodata, main/next]\n')
            original = root/RETAIL_PATH; original.parent.mkdir(parents=True); original.write_bytes(retail)
            assembly = root/'asm/smt3/data/main/unit.rodata.s'; assembly.parent.mkdir(parents=True)
            assembly.write_text('dlabel jtbl_00100100\ndlabel nextData\n')
            def run(*args):
                if '-S' in args:
                    return '0 10 T target\n'
                if '-t' in args and not omit_symbol:
                    full = str(args[-1]).endswith('full.o')
                    off, extent = (full_offset, full_size) if full else (object_offset, skip_size)
                    head = '' if full and full_missing else f'{off:08x} g O .rodata {extent:08x} title\n'
                    return head + '00000020 g O .rodata 00000008 nextData\n'
                return ''
            stack.enter_context(patch.object(check_unit, 'ROOT', root))
            stack.enter_context(patch.dict(check_unit.RETAIL, {VERSION: RETAIL_PATH}, clear=True))
            stack.enter_context(patch.dict(check_unit.VERSIONS, {VERSION: {'serial': SERIAL, 'gp': hex(GP)}}, clear=True))
            stack.enter_context(patch.object(check_unit.sys, 'argv', ['check_unit.py', str(unit)]))
            stack.enter_context(patch('eeas_compat.lit4_range', return_value=(0, 0)))
            stack.enter_context(patch.object(check_unit, 'load_segments', return_value=[]))
            stack.enter_context(patch.object(check_unit, 'va_to_off', side_effect=lambda segs, addr: addr-0xFF000))
            stack.enter_context(patch.object(check_unit, 'text_section', return_value=compiled))
            stack.enter_context(patch.object(check_unit, 'sections', return_value=(data, {'.rodata': (0,len(data))})))
            if full_data_size is not None:
                def object_sections(obj):
                    blob = data[:full_data_size] if str(obj).endswith('full.o') else data
                    return blob, {'.rodata': (0, len(blob))}
                stack.enter_context(patch.object(check_unit, 'sections', side_effect=object_sections))
            def object_relocs(obj):
                if full_relocation and str(obj).endswith('full.o'):
                    offset, kind = full_relocation
                    return {**relocs, '.rodata': {**relocs['.rodata'], offset: (kind, 'target')}}
                if full_relocated and str(obj).endswith('full.o'):
                    return {**relocs, '.rodata': {**relocs['.rodata'], 24: ('R_MIPS_32', 'unknown')}}
                return relocs
            stack.enter_context(patch.object(check_unit, 'relocations', side_effect=object_relocs))
            stack.enter_context(patch.object(check_unit, 'owns_rodata', return_value=True))
            stack.enter_context(patch.object(check_unit, 'run', side_effect=run))
            stack.enter_context(patch.object(check_unit.subprocess, 'run', return_value=SimpleNamespace(returncode=0, stderr='')))
            output = stack.enter_context(contextlib.redirect_stdout(io.StringIO()))
            with self.assertRaises(SystemExit) as stopped:
                check_unit.main()
            return stopped.exception.code, output.getvalue()

    def test_real_context_interior(self):
        code, report = self.check_interior()
        self.assertEqual(code, 0, report)

    def test_bad_context_interiors(self):
        for args in ({'full_offset':28}, {'full_size':4}, {'skip_size':4},
                     {'full_missing':True}, {'object_offset':28},
                     {'wrong_object':True}, {'nonzero_gap':True},
                     {'wrong_entry':True}, {'omit_symbol':True}, {'relocated':True},
                     {'full_relocated':True}, {'native_nonzero':True}):
            with self.subTest(args=args):
                code, report = self.check_interior(**args)
                self.assertEqual(code, 1, report)


class InteriorOwnedGapTests(unittest.TestCase):
    def proof(self, **changes):
        args = dict(data=bytes(40), relocs={}, starts=[0,24,32,40],
                    owned=[(24,0x1018,8)], table_off=0, native_addr=0x1000,
                    size=24, expected=32, native_gap=bytes(8))
        args.update(changes)
        return check_unit.verified_zero_gap(**args)

    def test_verified_interior_zero_object(self):
        self.assertTrue(self.proof())

    def test_invalid_interiors_fail_closed(self):
        for change in (
            dict(owned=[]), dict(owned=[(24,0x1020,8)]),
            dict(owned=[(24,0x1018,16)]), dict(owned=[(20,0x1014,12)]),
            dict(owned=[(24,0x1018,8),(28,0x101c,4)]),
            dict(owned=[(16,0x1010,8)]), dict(owned=[(24,0x1018,0)]),
            dict(starts=[0,24,28,32,40]), dict(starts=[0,24,40]),
            dict(relocs={24: ('R_MIPS_32','symbol')}),
            dict(data=bytes(27)+b'X'+bytes(12)),
            dict(native_gap=bytes(7)+b'X'), dict(native_gap=bytes(7)),
            dict(expected=40),
        ):
            with self.subTest(change=change):
                self.assertFalse(self.proof(**change))

    def test_empty_gap_and_ordinary_padding_still_work(self):
        self.assertTrue(self.proof(starts=[0,32,40],owned=[]))
        self.assertTrue(self.proof(starts=[0,24,40],owned=[],expected=24,native_gap=b''))


class RelocationFreeIntervalTests(unittest.TestCase):
    def test_fixed_fields_match_interval_oracle(self):
        for kind, width in (('R_MIPS_32', 4), ('R_MIPS_64', 8)):
            for offset in range(-1, 18):
                for begin in range(17):
                    for end in range(begin, 17):
                        with self.subTest(kind=kind, offset=offset, begin=begin, end=end):
                            valid = 0 <= offset and offset + width <= 16
                            overlap = begin < end and offset < end and begin < offset + width
                            self.assertEqual(check_unit.relocation_free_interval(
                                {offset: (kind, 'symbol')}, begin, end, 16), valid and not overlap)

    def test_unknown_and_malformed_fields_never_prove_a_gap(self):
        for relocs in ({0: ('R_MIPS_UNKNOWN', 'symbol')}, {0: ('R_MIPS_NONE', 'symbol')},
                       {-1: ('R_MIPS_32', 'symbol')}, {13: ('R_MIPS_32', 'symbol')},
                       {9: ('R_MIPS_64', 'symbol')}, {False: ('R_MIPS_32', 'symbol')},
                       {0: ('R_MIPS_32', '')}, {0: ['R_MIPS_32', 'symbol']},
                       {0: ('R_MIPS_32',)}, {0: (None, 'symbol')}):
            for begin, end in ((0, 0), (8, 12), (16, 16)):
                with self.subTest(relocs=relocs, begin=begin, end=end):
                    self.assertFalse(check_unit.relocation_free_interval(relocs, begin, end, 16))

    def test_interval_bounds_and_exact_adjacency(self):
        for args in ((-1, 0, 16), (0, 17, 16), (5, 4, 16), (0, 0, -1),
                     (False, 0, 16), (0, 0.0, 16), (0, 0, True)):
            with self.subTest(args=args):
                self.assertFalse(check_unit.relocation_free_interval({}, *args))
        for begin, end in ((0, 8), (12, 16), (10, 10)):
            self.assertTrue(check_unit.relocation_free_interval({8: ('R_MIPS_32', 'x')}, begin, end, 16))


class SharedNamedRodataProofTests(unittest.TestCase):
    @staticmethod
    def row(offset, size, name='named', flags='g O'):
        return f'{offset:08x} {flags} .rodata {size:08x} {name}\n'

    def fixture(self, size=16, skip_off=8, full_off=24, origin=0x1000):
        payload = bytes((index % 251 + 1 for index in range(size - 4))) + bytes(4)
        skip = bytes(skip_off) + payload + bytes(8)
        full = bytes(full_off) + payload + bytes(8)
        retail = bytes(0x80) + full
        addr = origin + full_off
        return dict(offset=skip_off, native_addr=addr, owned_range=(0x80, len(retail)),
                    symbol_text=f'named = 0x{addr:X}; // size:0x{size:X}\nnext = 0x{addr+size:X};\n',
                    skip_data=skip, skip_symbols=self.row(skip_off, size), skip_relocs={},
                    full_data=full, full_symbols=self.row(full_off, size), full_relocs={},
                    retail=retail, segs=[(origin, 0x80, len(full))])

    def proof(self, **changes):
        args = self.fixture()
        args.update(changes)
        return check_unit.shared_named_rodata_object(**args)

    def test_explicit_size_or_complete_native_interval(self):
        self.assertTrue(self.proof())
        self.assertTrue(self.proof(symbol_text='named = 0x1018;\nnext = 0x1028;\n'))
        self.assertTrue(self.proof(skip_symbols=self.row(8,16,flags='l O')))
        self.assertTrue(self.proof(symbol_text='named = 0x1018; // size:0x10\n'))

    def test_intended_native_shapes_without_claiming_a_real_build(self):
        for size, skip_off, full_off, origin in ((200,0x208,0x3E0,0x54B550),
                                               (16,0x28,0x38,0x531EE0)):
            with self.subTest(size=size):
                args = self.fixture(size,skip_off,full_off,origin)
                self.assertTrue(check_unit.shared_named_rodata_object(**args))
        args = self.fixture(24,0x38,0x48,0x531EE0)
        args['symbol_text'] = 'named = 0x531F28;\nnext = 0x5320F8;\n'
        self.assertFalse(check_unit.shared_named_rodata_object(**args))

    def test_native_address_size_owner_and_alias_controls(self):
        for changes in (
            dict(symbol_text='next = 0x1028;\n'),
            dict(symbol_text='named = 0x1019; // size:0x10\n'),
            dict(symbol_text='named = 0x1018;\n'),
            dict(symbol_text='named = 0x1018;\nnext = 0x1030;\n'),
            dict(symbol_text='named = 0x1018; // size:8\nnext = 0x1028;\n'),
            dict(symbol_text='named = 0x1018; // size:24\nnext = 0x1028;\n'),
            dict(symbol_text='named = 0x1018; // size:0x10 size:0x10\n'),
            dict(symbol_text='named = 0x1018; // size:invalid\n'),
            dict(symbol_text='named = 0x1018; // size:-1\n'),
            dict(symbol_text='named = 0x1018; // size:0x10\nnamed = 0x1018;\n'),
            dict(symbol_text='named = 0x1018; // size:0x10\nalias = 0x1018;\n'),
            dict(symbol_text='named = 0x1018; // size:0x10\nalias = 0x101C;\n'),
            dict(symbol_text='named = 0x1018; // size:0x10\nprior = 0x1010; // size:16\n'),
            dict(owned_range=None), dict(owned_range=(0x90,0xB0)),
            dict(owned_range=(0x80,0xA0)), dict(owned_range=(-1,0xB0)),
            dict(owned_range=(0x80,0x1000)), dict(native_addr=0x1019), dict(segs=[]),
        ):
            with self.subTest(changes=changes):
                self.assertFalse(self.proof(**changes))

    def test_full_skip_uniqueness_and_types(self):
        for side, offset in (('skip',8),('full',24)):
            for raw in ('', self.row(offset,8), self.row(offset,24),
                        self.row(offset+1,16), self.row(offset,16,flags='g'),
                        self.row(offset,16,flags='w O'), self.row(offset,0),
                        self.row(offset,16)+self.row(offset,16),
                        self.row(offset,16)+self.row(offset,0,'alias','l'),
                        self.row(offset,16)+self.row(offset+4,4,'interior'),
                        self.row(offset,16)+self.row(offset-4,8,'prior'),
                        self.row(offset,16)+self.row(1,0,'named'),
                        self.row(offset,16)+'malformed .rodata row\n'):
                with self.subTest(side=side,raw=raw):
                    self.assertFalse(self.proof(**{side+'_symbols':raw}))
            self.assertTrue(self.proof(**{side+'_symbols':self.row(offset,16)+self.row(offset+16,0,'adjacent','l')}))

    def test_all_bytes_and_tail_bytes_are_required(self):
        args = self.fixture()
        for field, start in (('skip_data',8),('full_data',24),('retail',0x98)):
            for delta in range(16):
                data = bytearray(args[field]); data[start+delta] ^= 1
                with self.subTest(field=field,delta=delta):
                    self.assertFalse(self.proof(**{field:bytes(data)}))
            self.assertFalse(self.proof(**{field:args[field][:start+15]}))

    def test_complete_native_mapping_rejects_holes_and_overlaps(self):
        valid = self.fixture()['segs'][0]
        for segs in (
            [(0x1000,0x80,0x20),(0x1024,0xA4,0xC)],  # mapped endpoints, interior hole
            [(0x1000,0x80,0x20),(0x1020,0xA0,0x10)],  # contiguous but split proof
            [valid,valid],
            [valid,(0x101C,0x9C,4)],  # same-offset overlapping segment
            [valid,(0x101C,0xAC,4)],  # conflicting overlapping mapping
            [(0x1019,0x99,0x17)], [(0x1000,0x80,0x27)],
            [(0x1000,0x80,0x100)], [(0x1000,-1,0x30)],
            [valid,(0x2000,0,False)], [valid,(0x2000,0)],
        ):
            with self.subTest(segs=segs):
                self.assertFalse(self.proof(segs=segs))
        self.assertTrue(self.proof(segs=[valid,(0x1028,0xA8,8)]))
        self.assertTrue(self.proof(segs=[valid,(0x1010,0x90,8)]))
        self.assertTrue(self.proof(segs=[(0x1018,0x98,16)]))

    def test_auto_address_names_do_not_grant_named_object_proof(self):
        for name in ('D_00001018','D_00001018.1','jtbl_00001018','func_00001018'):
            args=self.fixture()
            for key in ('symbol_text','skip_symbols','full_symbols'):
                args[key]=args[key].replace('named',name)
            with self.subTest(name=name):
                self.assertFalse(check_unit.shared_named_rodata_object(**args))

    def test_relocation_partial_overlap_unknown_footprints_and_adjacency(self):
        for side, start in (('skip',8),('full',24)):
            for offset,kind in ((start,'R_MIPS_32'),(start-4,'R_MIPS_64'),
                                (start+15,'R_MIPS_32'),(0,'R_MIPS_UNKNOWN'),
                                (-1,'R_MIPS_32')):
                with self.subTest(side=side,offset=offset,kind=kind):
                    self.assertFalse(self.proof(**{side+'_relocs':{offset:(kind,'x')}}))
            for offset in (start-4,start+16):
                self.assertTrue(self.proof(**{side+'_relocs':{offset:('R_MIPS_32','x')}}))


class SharedNamedRodataContextTests(unittest.TestCase):
    def check_shared(self, **changes):
        payload = b'NAMED OBJECT' + bytes(4)
        f = dict(skip=bytes(8)+payload, full=bytes(24)+payload,
                 skip_raw='00000008 g O .rodata 00000010 named\n',
                 full_raw='00000018 g O .rodata 00000010 named\n',
                 symbol_text='target = 0x100000; // type:func\nnamed = 0x100118;\nnext = 0x100128;\n',
                 yaml='      - [0x1100, .rodata, main/unit]\n      - [0x1128, .rodata, main/next]\n',
                 skip_relocs={}, full_relocs={}, failed_full=False)
        f.update(changes)
        compiled = struct.pack('<4I',0x3C020000,0x24420008,0x03E00008,0)
        full_code = struct.pack('<4I',0x3C020000,0x24420018,0x03E00008,0)
        retail = bytearray(0x1200)
        struct.pack_into('<4I',retail,0x1000,0x3C020010,0x24420118,0x03E00008,0)
        retail[0x1118:0x1128] = payload
        with tempfile.TemporaryDirectory() as tmp, contextlib.ExitStack() as stack:
            root=Path(tmp); unit=root/'src/smt3/main/unit.c'; unit.parent.mkdir(parents=True)
            unit.write_text('void target(void) {}\nconst char named[16] = "NAMED OBJECT";\nINCLUDE_ASM(const s32, "main/unit", consumer);\n')
            cfg=root/'config/smt3'; cfg.mkdir(parents=True)
            (cfg/'symbol_addrs.txt').write_text(f['symbol_text'])
            (cfg/'SLUS_209.11.yaml').write_text(f['yaml'])
            original=root/RETAIL_PATH; original.parent.mkdir(parents=True); original.write_bytes(retail)
            assembly=root/'asm/smt3/data/main/unit.rodata.s'; assembly.parent.mkdir(parents=True)
            assembly.write_text('dlabel named\ndlabel next\n')
            consumer=root/'asm/smt3/nonmatchings/main/unit/consumer.s'; consumer.parent.mkdir(parents=True)
            consumer.write_text('lui $2,%hi(named)\naddiu $2,$2,%lo(named)\n')
            def run(*args):
                if '-S' in args: return '0 10 T target\n'
                if '-t' in args: return f['full_raw' if str(args[-1]).endswith('full.o') else 'skip_raw']
                return ''
            def relocations(obj):
                return {'.text':{0:('R_MIPS_HI16','.rodata'),4:('R_MIPS_LO16','.rodata')},
                        '.rodata':f['full_relocs' if obj.name=='full.o' else 'skip_relocs'],'.data':{},'.sdata':{}}
            def sections(obj):
                data=f['full' if obj.name=='full.o' else 'skip']
                return data,{'.rodata':(0,len(data))}
            stack.enter_context(patch.object(check_unit,'ROOT',root))
            stack.enter_context(patch.dict(check_unit.RETAIL,{VERSION:RETAIL_PATH},clear=True))
            stack.enter_context(patch.dict(check_unit.VERSIONS,{VERSION:{'serial':SERIAL,'gp':hex(GP)}},clear=True))
            stack.enter_context(patch.object(check_unit.sys,'argv',['check_unit.py',str(unit)]))
            stack.enter_context(patch('eeas_compat.lit4_range',return_value=(0,0)))
            stack.enter_context(patch.object(check_unit,'load_segments',return_value=[(0x100000,0x1000,0x200)]))
            stack.enter_context(patch.object(check_unit,'text_section',side_effect=lambda obj:full_code if obj.name=='full.o' else compiled))
            stack.enter_context(patch.object(check_unit,'sections',side_effect=sections))
            stack.enter_context(patch.object(check_unit,'relocations',side_effect=relocations))
            stack.enter_context(patch.object(check_unit,'owns_rodata',return_value=True))
            stack.enter_context(patch.object(check_unit,'run',side_effect=run))
            calls=[SimpleNamespace(returncode=0,stderr=''),SimpleNamespace(returncode=int(f['failed_full']),stderr='fixture failure')]
            stack.enter_context(patch.object(check_unit.subprocess,'run',side_effect=calls))
            output=stack.enter_context(contextlib.redirect_stdout(io.StringIO()))
            stack.enter_context(contextlib.redirect_stderr(io.StringIO()))
            with self.assertRaises(SystemExit) as stopped: check_unit.main()
            return stopped.exception.code,output.getvalue()

    def test_complete_same_named_object_satisfies_shared_only(self):
        code,report=self.check_shared()
        self.assertEqual(code,0,report)
        self.assertIn('1 match, 0 differ',report)
        self.assertNotIn('SHARED',report)

    def test_missing_proof_keeps_shared_or_existing_gate_failure(self):
        for changes in (
            dict(skip_raw=''),dict(full_raw=''),dict(failed_full=True),
            dict(skip_raw='00000008 g O .rodata 00000008 named\n'),
            dict(full_raw='00000018 g O .rodata 00000008 named\n'),
            dict(full_raw='00000018 g O .rodata 00000010 named\n00000018 l .rodata 00000000 alias\n'),
            dict(skip=bytes(8)+b'NAMED OBJECT'+bytes(3)+b'X'),
            dict(full=bytes(24)+b'NAMED OBJECT'+bytes(3)+b'X'),
            dict(symbol_text='target = 0x100000; // type:func\nnamed = 0x100118;\nnext = 0x100138;\n'),
            dict(symbol_text='target = 0x100000; // type:func\nnamed = 0x100118; // size:8\nnext = 0x100128;\n'),
            dict(yaml=''),dict(yaml='      - [0x1110, .rodata, main/unit]\n      - [0x1128, .rodata, main/next]\n'),
            dict(skip_relocs={4:('R_MIPS_64','unknown')}),
            dict(full_relocs={20:('R_MIPS_64','unknown')}),
            dict(skip_relocs={0:('R_MIPS_UNKNOWN','unknown')}),
            dict(full_relocs={0:('R_MIPS_UNKNOWN','unknown')}),
        ):
            with self.subTest(changes=changes):
                code,report=self.check_shared(**changes)
                self.assertEqual(code,1,report)


class InteriorRelocationFootprintTests(unittest.TestCase):
    def test_full_partial_field_rejects_the_prior_false_positive(self):
        helper=InteriorOwnedContextTests()
        for field in ((20,'R_MIPS_64'),(23,'R_MIPS_32'),(0,'R_MIPS_UNKNOWN')):
            with self.subTest(field=field):
                code,report=helper.check_interior(full_relocation=field)
                self.assertEqual(code,1,report)
                self.assertIn('PAD jump table',report)

    def test_full_adjacency_preserves_the_existing_positive(self):
        code,report=InteriorOwnedContextTests().check_interior(full_relocation=(20,'R_MIPS_32'))
        self.assertEqual(code,0,report)

    def test_skip_partial_field_and_unknown_width_cannot_prove_a_zero_gap(self):
        helper=InteriorOwnedGapTests()
        for offset,kind in ((20,'R_MIPS_64'),(23,'R_MIPS_32'),(0,'R_MIPS_UNKNOWN')):
            with self.subTest(offset=offset,kind=kind):
                self.assertFalse(helper.proof(relocs={offset:(kind,'x')}))
        self.assertTrue(helper.proof(relocs={20:('R_MIPS_32','x'),32:('R_MIPS_64','x')}))

    def test_full_gap_must_fit_the_emitted_section(self):
        code,report=InteriorOwnedContextTests().check_interior(full_data_size=28)
        self.assertEqual(code,1,report)
        self.assertIn('PAD jump table',report)

    def test_invalid_skip_bounds_and_empty_interval_fields(self):
        helper=InteriorOwnedGapTests()
        for changes in (dict(table_off=-1),dict(size=-1),dict(size=-4,expected=-4,native_gap=b''),
                        dict(expected=41),dict(expected=23),dict(size=41)):
            with self.subTest(changes=changes):
                self.assertFalse(helper.proof(**changes))
        self.assertTrue(helper.proof(starts=[0,24,40],owned=[],expected=24,native_gap=b'',
                                     relocs={20:('R_MIPS_64','x')}))
        self.assertFalse(helper.proof(starts=[0,24,40],owned=[],expected=24,native_gap=b'',
                                      relocs={38:('R_MIPS_32','x')}))


class NativeObjectMappingTests(unittest.TestCase):
    def test_single_segment_matches_bounded_interval_oracle(self):
        for addr in range(7):
            for size in range(1,7):
                for start in range(7):
                    for extent in range(7):
                        for file_off in (0,4):
                            fits = file_off + extent <= 8 and start <= addr and addr + size <= start + extent
                            want = file_off + addr - start if fits else None
                            with self.subTest(addr=addr,size=size,start=start,extent=extent,file_off=file_off):
                                self.assertEqual(check_unit.native_object_offset(
                                    [(start,file_off,extent)],addr,size,8),want)

    def test_invalid_object_bounds_withhold_mapping(self):
        for args in ((-1,1,8),(0,0,8),(0,-1,8),(0,1,-1),(False,1,8),(0,True,8),(0,1,8.0)):
            with self.subTest(args=args):
                self.assertIsNone(check_unit.native_object_offset([(0,0,8)],*args))


if __name__ == "__main__":
    unittest.main()
