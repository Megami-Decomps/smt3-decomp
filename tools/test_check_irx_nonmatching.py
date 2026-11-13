#!/usr/bin/env python3
"""Regression checks for host-only IRX draft validation."""
import contextlib
import io
import subprocess
import unittest
from pathlib import Path
from unittest.mock import patch
import check_irx_nonmatching as checker


class IRXDialectTests(unittest.TestCase):
    def test_pre_c23_dialect_and_checks_remain_explicit(self):
        with patch.object(checker.irx_config, 'load', return_value={}), \
             patch.object(checker.irx_config, 'path', return_value=Path('draft.c')), \
             patch.object(checker.subprocess, 'run', return_value=subprocess.CompletedProcess([], 0, '', '')) as run, \
             contextlib.redirect_stdout(io.StringIO()):
            self.assertEqual(checker.check('TEST', Path('/tmp')), 0)
        command = run.call_args.args[0]
        for flag in ('-std=gnu17', '-m32', '-fsyntax-only', '-DNON_MATCHING'):
            self.assertIn(flag, command)

    def test_errors_are_still_rejected(self):
        with patch.object(checker.irx_config, 'load', return_value={}), \
             patch.object(checker.irx_config, 'path', return_value=Path('draft.c')), \
             patch.object(checker.subprocess, 'run', return_value=subprocess.CompletedProcess([], 1, '', 'draft.c:1: error: invalid syntax')), \
             contextlib.redirect_stdout(io.StringIO()):
            self.assertEqual(checker.check('TEST', Path('/tmp')), 1)


if __name__ == '__main__':
    unittest.main()
