#!/usr/bin/env python3
"""Pin what counts as inline asm in a unit, NON_MATCHING drafts included."""
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import check_inline_asm as cia

ROOT = Path(__file__).resolve().parent.parent


class Violations(unittest.TestCase):
    def check(self, body: str) -> list[str]:
        with tempfile.TemporaryDirectory() as tmp:
            unit = Path(tmp) / "unit.c"
            unit.write_text(body)
            return [what for _, what in cia.violations(unit)]

    def test_instruction_statement(self):
        self.assertEqual(self.check('void f(void) { __asm__ volatile ("nop"); }'),
                         ["inline asm statement"])

    def test_macro_defined_in_the_unit(self):
        body = ('#define COPY(dst) __asm__ volatile ( \\\n'
                '    "lq $2, 0(%0)" : : "r" (dst))\n')
        self.assertEqual(self.check(body), ["asm macro defined in the unit"])

    def test_draft_behind_the_non_matching_guard(self):
        body = ('#ifdef NON_MATCHING\n'
                'void f(void) { __asm__ volatile ("sync"); }\n'
                '#else\n'
                'INCLUDE_ASM(const s32, "sdf/unit", f);\n'
                '#endif\n')
        self.assertEqual(self.check(body), ["inline asm statement"])

    def test_statement_after_a_directive_is_not_a_declaration(self):
        for prefix in ("#ifdef NON_MATCHING\n", "#endif\n", "#include \"x.h\"\n"):
            with self.subTest(prefix=prefix):
                self.assertEqual(self.check(prefix + '__asm__ volatile ("nop");\n'),
                                 ["inline asm statement"])

    def test_declaration_after_a_directive_keeps_its_label(self):
        body = '#include "common.h"\nextern void g(void) __asm__("retailName");\n'
        self.assertEqual(self.check(body), [])

    def test_empty_constraint_template(self):
        for form in ('__asm__("" : "+r" (x));', '__asm__ volatile ("" : : "m" (x));',
                     '__asm__("" : "+r" (x), "+r" (y));'):
            with self.subTest(form=form):
                self.assertEqual(self.check("void f(void) { " + form + " }"), [])

    def test_declaration_label_and_register_binding(self):
        body = ('extern void g(void) __asm__("retailName");\n'
                'void f(void) { register s32 x asm("$5") = 1; }\n')
        self.assertEqual(self.check(body), [])

    def test_comments_and_strings_are_not_code(self):
        body = ('/* __asm__ volatile ("nop") stays out of the unit. */\n'
                'const char *s = "__asm__ volatile (\\"nop\\")";\n')
        self.assertEqual(self.check(body), [])


class Allowlist(unittest.TestCase):
    def test_every_listed_unit_still_has_inline_asm(self):
        listed = cia.load_allowlist()
        self.assertTrue(listed)
        for path in sorted(listed):
            with self.subTest(path=path):
                self.assertTrue((ROOT / path).exists(), "listed unit is gone")
                self.assertTrue(cia.violations(path), "nothing left to grandfather")

    def test_no_other_unit_has_inline_asm(self):
        listed = cia.load_allowlist()
        for unit in sorted(ROOT.glob("src/**/*.c")):
            path = unit.relative_to(ROOT)
            if path not in listed:
                with self.subTest(path=path):
                    self.assertEqual(cia.violations(path), [])


if __name__ == "__main__":
    unittest.main()
