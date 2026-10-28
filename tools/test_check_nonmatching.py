#!/usr/bin/env python3
"""Pin the draft accounting, which has misread the tree in both directions."""
import contextlib
import io
import re
import subprocess
import sys
import unittest
from pathlib import Path
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parent))
import check_nonmatching as cn
import check_irx_nonmatching as irx


class CompilerFailures(unittest.TestCase):
    def ee_result(self, status, stderr="", stdout=""):
        result = subprocess.CompletedProcess([], status, stdout, stderr)
        with patch.object(cn.subprocess, "run", return_value=result):
            return cn.compile_unit(Path("src/smt3/unit.c"))

    def irx_result(self, status, stderr="", stdout=""):
        result = subprocess.CompletedProcess([], status, stdout, stderr)
        with patch.object(irx.irx_config, "load", return_value={}), \
                patch.object(irx.irx_config, "path", return_value=Path("unit.c")), \
                patch.object(irx.subprocess, "run", return_value=result), \
                contextlib.redirect_stdout(io.StringIO()):
            return irx.check("test", Path("shim"))

    def test_ee_warning_with_a_successful_exit_is_allowed(self):
        self.assertEqual(self.ee_result(0, "unit.c:3: warning: unused variable\n"), "")

    def test_ee_failure_without_a_source_diagnostic_is_reported(self):
        for stderr in ("", "cc1: internal compiler error\n",
                       "unit.c:3: warning: unused variable\n"):
            with self.subTest(stderr=stderr):
                self.assertIn("status 1", self.ee_result(1, stderr))

    def test_ee_signal_and_loader_diagnostic_are_preserved(self):
        diagnostic = "ld-linux.so.2: cannot load shared object\n"
        self.assertIn(diagnostic.strip(), self.ee_result(-11, diagnostic))

    def test_ee_source_error_is_preserved(self):
        diagnostic = "unit.c:3: syntax error"
        self.assertEqual(self.ee_result(1, diagnostic), diagnostic)

    def test_irx_failure_without_an_error_diagnostic_is_reported(self):
        for stderr in ("", "cc1: terminated\n", "unit.c:3: warning: unused variable\n"):
            with self.subTest(stderr=stderr):
                self.assertGreater(self.irx_result(1, stderr), 0)

    def test_irx_warning_with_a_successful_exit_is_allowed(self):
        self.assertEqual(self.irx_result(0, "unit.c:3: warning: unused variable\n"), 0)


class DraftNames(unittest.TestCase):
    def test_one_block_can_hold_several_drafts(self) -> None:
        """Reading only the first name hid four drafted functions in sdfVu1."""
        text = ('#ifdef NON_MATCHING\n'
                'void a(void) {}\n'
                'void b(void) {}\n'
                '#else\n'
                'INCLUDE_ASM(const s32, "u", a);\n'
                'INCLUDE_ASM(const s32, "u", b);\n'
                '#endif\n')
        self.assertEqual(cn.draft_names(text), ["a", "b"])

    def test_a_guarded_helper_is_not_a_draft(self) -> None:
        """No #else means no asm fallback, so nothing is being drafted."""
        self.assertEqual(cn.draft_names('#ifdef NON_MATCHING\nstatic int h;\n#endif\n'), [])

    def test_separate_blocks_each_count(self) -> None:
        text = ('#ifdef NON_MATCHING\nvoid a(void) {}\n#else\n'
                'INCLUDE_ASM(const s32, "u", a);\n#endif\n'
                '#ifdef NON_MATCHING\nvoid b(void) {}\n#else\n'
                'INCLUDE_ASM(const s32, "u", b);\n#endif\n')
        self.assertEqual(cn.draft_names(text), ["a", "b"])

    def test_a_plain_include_asm_outside_a_guard_is_not_a_draft(self) -> None:
        self.assertEqual(cn.draft_names('INCLUDE_ASM(const s32, "u", a);\n'), [])


class TreeState(unittest.TestCase):
    def test_every_draft_compiles_and_no_entry_is_stale(self) -> None:
        """The tree as it stands passes both directions of the check."""
        self.assertEqual(cn.stale_entries(), 0)

    def test_drafts_are_found_in_the_tree(self) -> None:
        """A parser that silently matched nothing would pass every other test."""
        self.assertGreater(sum(len(v) for v in cn.draft_functions().values()), 100)


if __name__ == "__main__":
    unittest.main(verbosity=0)
