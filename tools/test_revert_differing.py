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


if __name__ == "__main__":
    unittest.main(verbosity=0)
