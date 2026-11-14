#!/usr/bin/env python3
"""Regression checks for host-only IRX draft validation."""
import contextlib
import io
import subprocess
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch
import check_irx_nonmatching as checker


class IRXDialectTests(unittest.TestCase):
    def run_mock(self, results):
        with patch.object(checker.irx_config, 'load', return_value={}), \
             patch.object(checker.irx_config, 'path', return_value=Path('draft.c')), \
             patch.object(checker.subprocess, 'run', side_effect=results) as run, \
             contextlib.redirect_stdout(io.StringIO()) as output:
            errors = checker.check('TEST', Path('/tmp'))
        return errors, [c.args[0] for c in run.call_args_list], output.getvalue()

    def result(self, code=0, error=''):
        return subprocess.CompletedProcess([], code, '', error)

    def test_pre_c23_dialect_and_checks_remain_explicit(self):
        errors, commands, output = self.run_mock([self.result(), self.result()])
        self.assertEqual(errors, 0)
        self.assertEqual(len(commands), 2)
        for command in commands:
            for flag in ('-std=gnu17', '-m32', '-fsyntax-only', '-DNON_MATCHING',
                         '-Werror=implicit-function-declaration'):
                self.assertIn(flag, command)
        self.assertNotIn('-DFIX_BUGS', commands[0])
        self.assertIn('-DFIX_BUGS', commands[1])
        self.assertIn('TEST (NON_MATCHING): ok', output)
        self.assertIn('TEST (NON_MATCHING + FIX_BUGS): ok', output)

    def test_errors_in_either_mode_are_rejected_and_labeled(self):
        for index in (0, 1):
            with self.subTest(index=index):
                results = [self.result(), self.result()]
                results[index] = self.result(1, 'draft.c:1: error: invalid syntax')
                errors, commands, output = self.run_mock(results)
                self.assertEqual(errors, 1)
                self.assertEqual(len(commands), 2)
                mode = 'NON_MATCHING + FIX_BUGS' if index else 'NON_MATCHING'
                self.assertIn(f'TEST ({mode}): 1 error(s)', output)

    def test_both_errors_are_accumulated(self):
        errors, _, _ = self.run_mock([self.result(1, 'error: bad')] * 2)
        self.assertEqual(errors, 2)

    def test_fix_mode_nondiagnostic_failure_is_rejected(self):
        errors, _, output = self.run_mock([self.result(), self.result(2, 'compiler failed')])
        self.assertGreater(errors, 0)
        self.assertIn('compiler exited with status 2', output)

    def test_successful_warnings_stay_nonfatal_in_both_modes(self):
        errors, _, _ = self.run_mock([self.result(0, 'warning: unused')] * 2)
        self.assertEqual(errors, 0)


class RealCompilerModes(unittest.TestCase):
    def compile_source(self, source, header=None):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / 'irx.h').write_text(checker.SHIM)
            path = root / 'draft.c'
            path.write_text(source)
            if header is not None:
                (root / 'mode.h').write_text(header)
            with patch.object(checker.irx_config, 'load', return_value={}), \
                 patch.object(checker.irx_config, 'path', return_value=path), \
                 contextlib.redirect_stdout(io.StringIO()) as output:
                errors = checker.check('CONTROL', root)
            return errors, output.getvalue()

    def test_broken_fixed_branch_cannot_hide_behind_good_retail_branch(self):
        errors, output = self.compile_source('#ifdef FIX_BUGS\nint f(void) { return ; broken\n#else\nint f(void) { return 1; }\n#endif\n')
        self.assertGreater(errors, 0)
        self.assertIn('CONTROL (NON_MATCHING): ok', output)
        self.assertNotIn('CONTROL (NON_MATCHING + FIX_BUGS): ok', output)

    def test_header_only_fixed_branch_is_also_checked(self):
        errors, _ = self.compile_source('#include "mode.h"\nint f(void) { return 1; }\n',
                                       '#ifdef FIX_BUGS\n#error broken header correction\n#endif\n')
        self.assertGreater(errors, 0)

    def test_undeclared_callee_fails_in_either_mode(self):
        for condition, good_mode in (("FIX_BUGS", "NON_MATCHING"),
                                     ("!defined(FIX_BUGS)", "NON_MATCHING + FIX_BUGS")):
            with self.subTest(condition=condition):
                errors, output = self.compile_source(
                    f"#if {condition}\n"
                    "int f(void) { return missing_callee(7); }\n"
                    "#else\nint f(void) { return 7; }\n#endif\n")
                self.assertGreater(errors, 0)
                self.assertIn(f"CONTROL ({good_mode}): ok", output)

    def test_warning_default_compiler_does_not_accept_missing_callee(self):
        # GCC 14 made this error-by-default. Its -fpermissive option restores
        # the warning disposition used by older hosts; the explicit -Werror
        # must still reject it. Exercise the real compiler, not canned stderr.
        real_run = subprocess.run

        def warning_default(command, **kwargs):
            return real_run([command[0], '-fpermissive', *command[1:]], **kwargs)

        with patch.object(checker.subprocess, 'run', side_effect=warning_default):
            errors, output = self.compile_source(
                'int f(void) { return missing_callee(7); }\n')
        self.assertGreater(errors, 0)
        self.assertNotIn(': ok', output)

    def test_header_undeclared_callee_fails(self):
        errors, _ = self.compile_source(
            '#include "mode.h"\nint f(void) { return helper(); }\n',
            'static int helper(void) { return missing_callee(); }\n')
        self.assertGreater(errors, 0)

    def test_declared_callees_keep_legacy_nonprototype_support(self):
        for declaration in ('extern int callee();', 'extern int callee(int);'):
            with self.subTest(declaration=declaration):
                errors, _ = self.compile_source(
                    declaration + '\nint f(void) { return callee(7); }\n')
                self.assertEqual(errors, 0)

    def test_valid_branches_pass(self):
        errors, _ = self.compile_source('int f(void) {\n#ifdef FIX_BUGS\nreturn 2;\n#else\nreturn 1;\n#endif\n}\n')
        self.assertEqual(errors, 0)


if __name__ == '__main__':
    unittest.main()
