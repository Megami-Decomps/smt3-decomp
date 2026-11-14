#!/usr/bin/env python3
"""Failed preprocessing must not look like an empty, successfully probed unit."""
import contextlib
import io
import json
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest
from unittest import mock

sys.path.insert(0, str(Path(__file__).resolve().parent))
import c_data_sizes as cds


class PreprocessingFailures(unittest.TestCase):
    def test_failed_empty_output_marks_unit_failed(self):
        result = subprocess.CompletedProcess([], 1, '', 'missing header')
        with mock.patch.object(cds.subprocess, 'run', return_value=result), \
                mock.patch.object(cds, 'compile_probe', return_value='') as compile_probe:
            self.assertEqual(cds.one('broken.c'), ('broken.c', {'__failed__': 1}))
            compile_probe.assert_not_called()

    def test_failed_partial_output_is_not_probed(self):
        result = subprocess.CompletedProcess([], 1, 'extern int partial;\n', '#error')
        with mock.patch.object(cds.subprocess, 'run', return_value=result), \
                mock.patch.object(cds, 'compile_probe', return_value='') as compile_probe:
            self.assertEqual(cds.one('broken.c'), ('broken.c', {'__failed__': 1}))
            compile_probe.assert_not_called()

    def test_successful_empty_output_is_not_a_failure(self):
        result = subprocess.CompletedProcess([], 0, '', '')
        with mock.patch.object(cds.subprocess, 'run', return_value=result):
            self.assertEqual(cds.one('empty.c'), ('empty.c', {}))

    def test_successful_declarations_still_probe(self):
        result = subprocess.CompletedProcess([], 0, 'extern int value;\n', '')
        with mock.patch.object(cds.subprocess, 'run', return_value=result), \
                mock.patch.object(cds, 'compile_probe', return_value='__sz_value:\n\t.word 4\n'):
            self.assertEqual(cds.one('valid.c'), ('valid.c', {'value': {
                'unit': 'valid.c', 'decl': 'extern int value', 'local': False, 'size': 4}}))

    @unittest.skipUnless(shutil.which('gcc'), 'gcc is required for preprocessing fixture')
    def test_missing_include_is_listed_in_json(self):
        # Real gcc exits unsuccessfully before emitting any declarations here.
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / 'src/smt3').mkdir(parents=True)
            (root / 'src/smt3/broken.c').write_text('#include "missing_audit_header.h"\n')
            out = root / 'sizes.json'
            with mock.patch.object(cds, 'ROOT', root), \
                    mock.patch.object(sys, 'argv', ['c_data_sizes.py', str(out), '-j', '1']), \
                    contextlib.redirect_stderr(io.StringIO()) as stderr:
                cds.main()
            self.assertEqual(json.loads(out.read_text()), {
                'symbols': {}, 'failed_units': ['src/smt3/broken.c']})
            self.assertIn('1/1 units failed to probe', stderr.getvalue())


if __name__ == '__main__':
    unittest.main()
