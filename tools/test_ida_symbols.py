"""Curated symbol corrections must survive regeneration of the IDB export."""
from pathlib import Path
import tempfile
import unittest

from ida_symbols import BEGIN, END, Symbol, render, splice


class CuratedSymbolsTests(unittest.TestCase):
    def test_reassigned_name_does_not_return_at_its_old_address(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "symbols.txt"
            path.write_text(
                'packParts = 0x00101000; // type:func size:0x20\n'
                'convert = 0x00102000; // type:func size:0x40\n'
                + BEGIN + '\n' + END + '\n// tail\n')
            result = splice(path, render([
                Symbol(0x101000, 'convert', 'func', 0x20),
                Symbol(0x102000, 'oldConversionName', 'func', 0x40),
                Symbol(0x103000, 'unrelated', 'func', 0x10),
            ]))
            self.assertNotIn('oldConversionName', result)
            self.assertEqual(result.count('convert ='), 1)
            self.assertIn('convert = 0x00102000;', result)
            self.assertIn('unrelated = 0x00103000;', result)
            self.assertIn('// .text functions (1)', result)
            self.assertTrue(result.endswith('// tail\n'))

    def test_unmodified_export_remains_stable(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "symbols.txt"
            block = render([Symbol(0x101000, 'functionName', 'func', 0x20),
                            Symbol(0x2F5680, 'dataName', 'u8', 4)])
            first = splice(path, block)
            path.write_text(first)
            self.assertEqual(splice(path, block), first)


if __name__ == '__main__':
    unittest.main()
