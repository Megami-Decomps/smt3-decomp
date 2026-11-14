#!/usr/bin/env python3
"""Pin where an asm keyword is a violation, NON_MATCHING drafts included."""
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import check_inline_asm as cia

ROOT = Path(__file__).resolve().parent.parent


class Violations(unittest.TestCase):
    def check(self, body: str) -> list[int]:
        with tempfile.TemporaryDirectory() as tmp:
            unit = Path(tmp) / "unit.c"
            unit.write_text(body)
            return cia.violations(unit)

    def test_instruction_statement(self):
        self.assertEqual(self.check('void f(void) { __asm__ volatile ("nop"); }'), [1])

    def test_macro_defined_in_the_unit(self):
        body = ('#define COPY(dst) __asm__ volatile ( \\\n'
                '    "lq $2, 0(%0)" : : "r" (dst))\n')
        self.assertEqual(self.check(body), [1])

    def test_draft_behind_the_non_matching_guard(self):
        body = ('#ifdef NON_MATCHING\n'
                'void f(void) { __asm__ volatile ("sync"); }\n'
                '#else\n'
                'INCLUDE_ASM(const s32, "sdf/unit", f);\n'
                '#endif\n')
        self.assertEqual(self.check(body), [2])

    def test_empty_constraint_template(self):
        self.assertEqual(self.check('void f(void) { __asm__("" : "+r" (x)); }'), [1])

    def test_declaration_label_and_register_binding(self):
        body = ('extern void g(void) __asm__("retailName");\n'
                'void f(void) { register s32 x asm("$5") = 1; }\n')
        self.assertEqual(self.check(body), [1, 2])

    def test_the_macros_themselves_are_not_asm(self):
        body = ('extern void g(void) FAKEMATCH_ASM_NAME("retailName");\n'
                'void f(void) {\n'
                '    FAKEMATCH_ASM_REG(s32, x, "$5") = 1;\n'
                '    FAKEMATCH_ASM_LIVE(x);\n'
                '    EE_SPIN_NOP();\n'
                '}\n')
        self.assertEqual(self.check(body), [])

    def test_comments_and_strings_are_not_code(self):
        body = ('/* __asm__ volatile ("nop") stays out of the unit. */\n'
                'const char *s = "__asm__ volatile (\\"nop\\")";\n')
        self.assertEqual(self.check(body), [])


    def test_spliced_keywords_are_still_violations(self):
        for keyword in ('asm', '__asm', '__asm__'):
            for index in range(1, len(keyword)):
                with self.subTest(keyword=keyword, index=index):
                    body = 'void f(void) { ' + keyword[:index] + chr(92) + '\n' + keyword[index:] + '("nop"); }'
                    self.assertEqual(self.check(body), [1])

    def test_spliced_comment_stays_a_comment(self):
        self.assertEqual(self.check('// comment ' + chr(92) + '\nasm("nop");\n'), [])

    def test_splice_can_form_comment_delimiters(self):
        splice = chr(92) + '\n'
        self.assertEqual(self.check('/' + splice + '* asm("nop"); *' + splice + '/'), [])
        self.assertEqual(self.check('/' + splice + '/ asm("nop");\n'), [])

    def test_spliced_literal_stays_a_literal(self):
        self.assertEqual(self.check('const char *s = "as' + chr(92) + '\nm";'), [])

    def test_physical_line_numbers_after_multiple_splices(self):
        splice = chr(92) + '\n'
        body = '// ignored' + splice + '\n\nvoid f(void) { __as' + splice + 'm__("nop");\nasm("nop"); }'
        self.assertEqual(self.check(body), [4, 6])

    def test_crlf_splice(self):
        self.assertEqual(self.check('void f(void) { __as' + chr(92) + '\r\nm__("nop"); }'), [1])


class Allowlist(unittest.TestCase):
    def test_every_listed_unit_still_has_inline_asm(self):
        listed = cia.load_allowlist()
        # An empty allowlist is valid once every unit has migrated.
        for path in sorted(listed):
            with self.subTest(path=path):
                self.assertTrue((ROOT / path).exists(), "listed unit is gone")
                self.assertTrue(cia.violations(path), "nothing left to grandfather")

    def test_no_other_unit_spells_asm(self):
        listed = cia.load_allowlist()
        for unit in sorted(ROOT.glob("src/**/*.c")):
            path = unit.relative_to(ROOT)
            if path not in listed:
                with self.subTest(path=path):
                    self.assertEqual(cia.violations(path), [])


if __name__ == "__main__":
    unittest.main()
