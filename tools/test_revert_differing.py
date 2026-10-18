#!/usr/bin/env python3
"""revert_differing.py has to recognise both brace styles used in this tree."""
import importlib.util
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
spec = importlib.util.spec_from_file_location("rd", ROOT / "tools/revert_differing.py")
rd = importlib.util.module_from_spec(spec)
spec.loader.exec_module(rd)

SAME_LINE = """#include "common.h"

void fldDrawInit(void) {
    gDrawReady = 1;
}

s32 fldGetFlag(s32 i) {
    if (i < 0) {
        return 0;
    }
    return gFlags[i];
}
"""

NEXT_LINE = """#include "common.h"

void fldDrawInit(void)
{
    gDrawReady = 1;
}

s32 fldGetFlag(s32 i)
{
    return gFlags[i];
}
"""

MULTILINE_SIG = """#include "common.h"

void DrwSprite2D(s32 x, s32 y, s32 w, s32 h,
                 s32 color) {
    gColor = color;
}
"""


class TestSpans(unittest.TestCase):
    def names(self, text):
        return [n for n, _, _ in rd.spans(text)]

    def test_same_line_brace(self):
        self.assertEqual(self.names(SAME_LINE), ["fldDrawInit", "fldGetFlag"])

    def test_next_line_brace(self):
        self.assertEqual(self.names(NEXT_LINE), ["fldDrawInit", "fldGetFlag"])

    def test_multiline_signature(self):
        self.assertEqual(self.names(MULTILINE_SIG), ["DrwSprite2D"])

    def test_span_covers_the_whole_body(self):
        lines = SAME_LINE.split("\n")
        for name, start, end in rd.spans(SAME_LINE):
            if name == "fldGetFlag":
                self.assertIn("fldGetFlag", lines[start])
                self.assertEqual(lines[end], "}")
                self.assertIn("return gFlags[i];", "\n".join(lines[start:end + 1]))

    def test_control_statements_are_not_definitions(self):
        self.assertEqual(self.names("""
void f(void) {
    if (x) {
        while (y) {
        }
    }
}
"""), ["f"])

    def test_real_units_are_recognised(self):
        """Both styles appear in the tree; a miss here means silent no-ops."""
        for unit in ("src/smt3/field/fldDraw.c", "src/smt3/newdata/datCalc.c"):
            path = ROOT / unit
            if not path.exists():
                continue
            text = path.read_text()
            openers = sum(1 for l in text.split("\n")
                          if l[:1].isalpha() and l.rstrip().endswith(") {"))
            if openers:
                self.assertGreaterEqual(len(self.names(text)), openers,
                                        f"{unit}: recognised fewer than its same-line definitions")




DIFFERING = """#include "common.h"

void fldDrawInit(void) {
    gDrawReady = 1;
}
"""


class TestWrap(unittest.TestCase):
    def test_draft_is_kept_behind_the_fallback(self):
        body = ["void f(void) {", "    g();", "}"]
        out = rd.wrap(body, "field/fldDraw", "f", "12 of 694 words differ")
        self.assertTrue(out[0].startswith("/* NON_MATCHING: 12 of 694 words differ"))
        i = out.index("#ifdef NON_MATCHING")
        self.assertEqual(out[i + 1:i + 4], body)
        self.assertEqual(out[i + 4], "#else")
        self.assertEqual(out[i + 5], 'INCLUDE_ASM(const s32, "field/fldDraw", f);')
        self.assertEqual(out[i + 6], "#endif")

    def test_the_annotation_is_one_line(self):
        """The label says how close it is; the reasoning goes in the commit."""
        out = rd.wrap(["void f(void) {", "}"], "u", "f", "3 of 9 words differ")
        self.assertEqual(out.index("#ifdef NON_MATCHING"), 1)
        self.assertEqual(out[0], "/* NON_MATCHING: 3 of 9 words differ. */")

    def test_an_unmeasured_draft_says_so(self):
        out = rd.wrap(["void f(void) {", "}"], "u", "f")
        self.assertIn("difference not recorded", out[0])

    def test_the_draft_is_not_a_second_definition(self):
        """check_unit has to read the wrapped form as asm only."""
        import importlib.util
        spec = importlib.util.spec_from_file_location("cu", ROOT / "tools/check_unit.py")
        cu = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(cu)
        text = "\n".join(["#include \"common.h\""] + rd.wrap(
            ["void f(void) {", "    g();", "}"], "field/fldDraw", "f"))
        stripped = cu.drop_nonmatching(text)
        self.assertNotIn("void f(void)", stripped)
        self.assertNotIn("g();", stripped)
        self.assertIn('INCLUDE_ASM(const s32, "field/fldDraw", f);', stripped)
        self.assertNotIn("#ifdef NON_MATCHING", stripped)


if __name__ == "__main__":
    unittest.main(verbosity=0)
