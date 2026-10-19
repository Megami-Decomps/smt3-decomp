#!/usr/bin/env python3
"""inline_strings.py must not mistake a record or float data for a string."""
import importlib.util
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
spec = importlib.util.spec_from_file_location("ins", ROOT / "tools/inline_strings.py")
ins = importlib.util.module_from_spec(spec)
spec.loader.exec_module(ins)

STRING = """.include "macro.inc"
.section .rodata, "a"
.align 3
dlabel D_00549040
    /* 44A040 00549040 */ .asciz "%s can't load.\\n"
    /* 25732063616E2774206C6F61642E0A00 */
.align 2
enddlabel D_00549040
"""

# A record that opens with a name field: one .asciz, then more data.
RECORD = """.section .rodata, "a"
dlabel D_0052F850
    /* 430850 0052F850 */ .asciz "hijri"
    /* 68696A7269000000 */
.align 2
    /* 430858 0052F858 */ .word 0x3F800000
.align 2
enddlabel D_0052F850
"""


class LiteralTests(unittest.TestCase):
    def test_printable_bytes_become_a_literal(self) -> None:
        self.assertEqual(ins.c_literal(b"abc\x00"), '"abc"')

    def test_newline_and_quote_are_escaped(self) -> None:
        self.assertEqual(ins.c_literal(b'a\nb"c\x00'), '"a\\nb\\"c"')

    def test_backslash_is_escaped(self) -> None:
        """cdrom0:\\SLPS_999.99;1 really has a backslash byte in retail."""
        self.assertEqual(ins.c_literal(b"a\\b\x00"), '"a\\\\b"')

    def test_data_with_no_terminator_is_rejected(self) -> None:
        self.assertIsNone(ins.c_literal(b"abc"))

    def test_non_text_bytes_are_rejected(self) -> None:
        self.assertIsNone(ins.c_literal(b"\x01\x02\x00"))

    def test_float_bytes_that_look_like_text_still_need_a_terminator(self) -> None:
        """0.7f repeated is '333?333?', printable but not a string."""
        self.assertIsNone(ins.c_literal(b"\x33\x33\x33\x3f" * 3))


class ScanTests(unittest.TestCase):
    def scan(self, text):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "asm").mkdir()
            (root / "asm/x.s").write_text(text)
            cwd = Path.cwd()
            import os

            os.chdir(root)
            try:
                return ins.scan_strings()
            finally:
                os.chdir(cwd)

    def test_a_lone_string_is_found(self) -> None:
        got = self.scan(STRING)
        self.assertEqual(got["D_00549040"], b"%s can't load.\n\x00")

    def test_a_record_opening_with_a_name_is_not_a_string(self) -> None:
        self.assertEqual(self.scan(RECORD), {})


class DeclarationTests(unittest.TestCase):
    def candidates(self, source, strings):
        with tempfile.NamedTemporaryFile("w", suffix=".c", delete=False) as handle:
            handle.write(source)
            path = handle.name
        return ins.candidates(path, strings)

    def test_a_char_declaration_qualifies(self) -> None:
        src = 'extern char D_00549040[];\nvoid f(void) { printf(D_00549040); }\n'
        got = self.candidates(src, {"D_00549040": b"hi\x00"})
        self.assertEqual(got, {"D_00549040": '"hi"'})

    def test_a_struct_declaration_does_not(self) -> None:
        """effEvent.c declares D_00320598 a params record; its bytes start '<'."""
        src = 'extern effBlurScatterParams_t D_00320598;\nvoid *f(void) { return &D_00320598; }\n'
        self.assertEqual(self.candidates(src, {"D_00320598": b"<\x00"}), {})

    def test_an_sdata_address_is_skipped(self) -> None:
        src = 'extern char D_0054F628[];\nvoid f(void) { printf(D_0054F628); }\n'
        self.assertEqual(self.candidates(src, {"D_0054F628": b"model\x00"}), {})


class ConvertTests(unittest.TestCase):
    def convert(self, source, picks):
        with tempfile.NamedTemporaryFile("w", suffix=".c", delete=False) as handle:
            handle.write(source)
            path = handle.name
        ins.convert(path, picks)
        return open(path).read()

    def test_a_shared_extern_keeps_its_other_declarators(self) -> None:
        src = "extern char D_1[], D_2[];\nvoid f(void) { g(D_1, D_2); }\n"
        got = self.convert(src, {"D_1": '"a"'})
        self.assertIn("extern char D_2[];", got)
        self.assertIn('g("a", D_2);', got)

    def test_a_semicolon_in_a_trailing_comment_does_not_break_the_line(self) -> None:
        """A comment holding "SLPS_999.99;1" once left the comment unterminated."""
        src = 'extern char D_1[], D_2[]; /* "cdrom0:\\\\SLPS_999.99;1" */\nvoid f(void) { g(D_1, D_2); }\n'
        got = self.convert(src, {"D_1": '"a"'})
        self.assertEqual(got.count("/*"), got.count("*/"), "comment left unterminated")
        self.assertIn("D_2[];", got)

    def test_the_directive_line_is_removed_not_rewritten(self) -> None:
        src = 'extern char D_1[];\nvoid f(void) { g(D_1); }\nINCLUDE_RODATA(const s32, "u", D_1);\n'
        got = self.convert(src, {"D_1": '"a"'})
        self.assertNotIn("INCLUDE_RODATA", got)
        self.assertNotIn("D_1", got)


if __name__ == "__main__":
    unittest.main()
