"""Synthetic fail-closed graph tests, no proprietary input or stock compiler."""
import io
import json
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

import ninja_syntax

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from tools import native_rodata_fill as fill
from tools.linker_inputs import linker_object_sections

VERSION = 'smt3'
SERIAL = 'SLUS_209.11'
PB = 'pb/pbDebugMode'
NEXT = 'model/mdlManager'


def cobj(unit):
    return Path(f'build/smt3/src/smt3/{unit}.o')


def write(root, rel, payload):
    out = root / rel
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(payload)
    return out


class NativeReadonlyFill(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name)
        self.pri = cobj(PB)
        self.nxt = cobj(NEXT)
        self.selectors = {
            self.pri: ['.text', '.rodata', '.lit4'],
            self.nxt: ['.text', '.rodata', '.sdata'],
        }
        self.ld_source = ''.join(f'{p}({sec});\n'
                                 for p, secs in self.selectors.items() for sec in secs)

    def test_duplicate_or_ambiguous_selectors_fail(self):
        for extra in ('', f'{self.pri}(.rodata);\n', f'{self.pri}(\n.rodata\n);',
                      f'/* {self.nxt}(.rodata); */', f'"{self.nxt}" (.rodata);'):
            script = write(self.root, 'build/smt3/SLUS_209.11.ld', self.ld_source + extra)
            actual = linker_object_sections(script)
            if extra:
                with self.assertRaisesRegex(ValueError, 'one unambiguous'):
                    fill.strict_selectors(actual, self.pri, self.nxt)
            else:
                fill.strict_selectors(actual, self.pri, self.nxt)

    def test_explicit_opts_only(self):
        cfg = fill.opt_path(VERSION)
        self.assertEqual(fill.options(self.root, VERSION), ())
        write(self.root, cfg, json.dumps([PB]))
        self.assertEqual(fill.options(self.root, VERSION), (PB,))
        for bad in ([PB, PB], [NEXT], [], {PB: True}, [1]):
            with self.subTest(opts=bad):
                write(self.root, cfg, json.dumps(bad))
                with self.assertRaisesRegex(ValueError, 'invalid native'):
                    fill.options(self.root, VERSION)

    def test_source_manifest_rejects_changed_parameters_or_stock_assembly(self):
        a = 'config/smt3/reloc_addrs.txt'
        b = fill.full_asm(VERSION, PB)
        write(self.root, a, 'old relocation')
        write(self.root, b, 'old generated assembly')
        with patch.object(fill, 'current_splat_inputs', return_value={a, str(b)}), \
                patch('configure.source_matching_ownership', return_value=[('path', [('scope', 'asm')])]):
            stamp = fill.split_stamp(VERSION)
            (self.root / stamp).parent.mkdir(parents=True)
            fill.record_split(self.root, VERSION, SERIAL, (PB,))
            fill.check_split(self.root, VERSION, SERIAL, (PB,))
            for p, old, new in ((a, 'old relocation', 'diff reloc'), (b, 'old generated assembly', 'diff asm')):
                write(self.root, p, new)
                with self.assertRaisesRegex(ValueError, 'stale'):
                    fill.check_split(self.root, VERSION, SERIAL, (PB,))
                write(self.root, p, old)
            with patch('configure.source_matching_ownership', return_value=[('path', [])]):
                with self.assertRaisesRegex(ValueError, 'stale'):
                    fill.check_split(self.root, VERSION, SERIAL, (PB,))

    @unittest.skipUnless(shutil.which('ninja'), 'needs Ninja graph reader')
    def test_existing_production_graph_does_not_depend_on_comparison_references(self):
        o = io.StringIO()
        n = ninja_syntax.Writer(o)
        n.rule('as', 'true')
        n.rule('ld', 'true')
        n.rule('native_rodata_fill', 'true')
        with patch.object(fill, 'current_splat_inputs', return_value={'config/smt3/reloc_addrs.txt'}):
            fill.write_reference_edge(n, self.root, VERSION, SERIAL, PB, self.selectors, (PB,))
        l = f'build/smt3/{SERIAL}'
        n.build(l + '.elf', 'ld', [str(self.pri), str(self.nxt)],
                implicit_outputs=[l + '.map', l + '.elf.proof.json'])
        n.build(l, 'phony', l + '.elf')
        n.build(l + '.ok', 'phony', l)
        n.build('smt3', 'phony', l + '.ok')
        ref = str(fill.target_obj(VERSION, PB))
        backup = str(fill.stock_obj(VERSION, PB))
        n.build('report', 'phony', ref)
        write(self.root, 'build.ninja', o.getvalue())
        graph = lambda target: subprocess.check_output(['ninja', '-t', 'graph', target],
                                                        cwd=self.root).decode()
        prod, report = graph('smt3'), graph('report')
        self.assertNotIn(ref, prod)
        self.assertNotIn(backup, prod)
        for edge in (ref, backup, l + '.elf.proof.json', l + '.ok',
                     'build/smt3/base/src/smt3/pb/pbDebugMode.o',
                     'config/smt3/reloc_addrs.txt'):
            self.assertIn(edge, report)


if __name__ == '__main__':
    unittest.main()
