#!/usr/bin/env python3
"""Tests for tools/perm_unit.py: what a candidate is spliced into and what is dumped."""

from __future__ import annotations

import sys
import unittest
from pathlib import Path
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parent))

import perm_unit  # noqa: E402

UNIT = """\
#include "common.h"

extern s32 helper(s32);

static inline s32 clamp(s32 a) {
    return a < 0 ? 0 : a;
}

void func_001001D8(void) {
    helper(1);
}

INCLUDE_ASM(const s32, "code_001001D8", dds3SoftReset);

void func_00100200(void) {
    helper(2);
}
"""

DRAFT = """\
extern s32 g_flag;

void dds3SoftReset(void) {
    g_flag = clamp(helper(0));
}
"""


class SpliceTests(unittest.TestCase):
    def test_candidate_replaces_the_include_asm_line(self) -> None:
        template, full, include_asm = perm_unit.splice(UNIT, DRAFT, "dds3SoftReset")
        self.assertEqual(include_asm, 'INCLUDE_ASM(const s32, "code_001001D8", dds3SoftReset);')
        # The draft's declarations stay; only its definition becomes the hole.
        self.assertIn("extern s32 g_flag;\n\n@@PERM@@\n", template)
        self.assertNotIn("INCLUDE_ASM", template)
        self.assertNotIn("@@PERM@@", full)
        self.assertIn("void dds3SoftReset(void) {", full)
        # Neighbours keep their order around the spliced function.
        self.assertLess(full.index("func_001001D8"), full.index("dds3SoftReset"))
        self.assertLess(full.index("dds3SoftReset"), full.index("func_00100200"))

    def test_a_function_that_is_already_c_is_refused(self) -> None:
        with self.assertRaises(SystemExit):
            perm_unit.splice(UNIT, DRAFT, "func_00100200")

    def test_find_def_skips_a_prototype(self) -> None:
        text = "void f(void);\n\nvoid f(void) {\n    return;\n}\n"
        start, end = perm_unit.find_def(text, "f")
        self.assertEqual(text[start:end], "void f(void) {\n    return;\n}\n")


class PrototypeOnlyTests(unittest.TestCase):
    def test_unused_bodies_go_but_inline_helpers_and_callees_stay(self) -> None:
        template, full, _ = perm_unit.splice(UNIT, DRAFT, "dds3SoftReset")
        body = full[slice(*perm_unit.find_def(full, "dds3SoftReset"))]
        fast = perm_unit.prototypes_only(template, body)
        self.assertIn("void func_001001D8(void);", fast)
        self.assertIn("void func_00100200(void);", fast)
        self.assertNotIn("helper(1)", fast)
        # An inline helper, and anything the body calls, must keep its body.
        self.assertIn("return a < 0 ? 0 : a;", fast)
        self.assertIn("@@PERM@@", fast)


class HideAsmTests(unittest.TestCase):
    def test_inline_asm_becomes_a_b64literal_pragma(self) -> None:
        src = 'void f(void) {\n    __asm__ volatile ("qmtc2 $0, $vf1" : : "r" (0));\n}\n'
        out = perm_unit.hide_asm(src)
        self.assertNotIn("__asm__", out)
        self.assertIn("#pragma _permuter b64literal ", out)
        import base64
        blob = out.split("b64literal ")[1].split("\n")[0]
        self.assertIn("qmtc2 $0, $vf1", base64.b64decode(blob).decode())


class StripOtherCodeTests(unittest.TestCase):
    ASM = """\
\t.extern g_flag, 4
\t.ent other
other:
\tjr\t$31
\t.end other
\t.include "build/eeasm/asm/smt3/nonmatchings/code_001001D8/func_00100200.s"
\t.include "build/eeasm/asm/smt3/nonmatchings/code_001001D8/jtbl_00200000.s"
\t.ent target
target:
\tjr\t$31
\t.end target
"""

    def test_only_the_target_function_and_shared_data_survive(self) -> None:
        out = perm_unit.strip_other_code(self.ASM, "target")
        self.assertIn(".ent target", out)
        self.assertIn(".extern g_flag, 4", out)
        self.assertNotIn("other", out)
        self.assertNotIn("func_00100200.s", out)
        # Jump tables and data the function itself needs are still included.
        self.assertIn("jtbl_00200000.s", out)


class DumpTests(unittest.TestCase):
    OBJDUMP = """\

f.o:     file format elf32-tradlittlemips

Disassembly of section .text:

00000000 <target>:
   0:\t0c000000 \tjal\t0 <target>
\t\t\t0: R_MIPS_26\thelper
   4:\t3c020000 \tlui\t$2,0x0
\t\t\t4: R_MIPS_HI16\tg_flag
   8:\t8f838018 \tlw\t$3,-32744(gp)
   c:\t8fa40010 \tlw\t$4,16(sp)
"""

    def dump(self):
        with patch("perm_unit.subprocess.run") as run:
            run.return_value.stdout = self.OBJDUMP
            return perm_unit.dump(Path("f.o"))

    def test_relocations_are_folded_into_their_instruction(self) -> None:
        out = self.dump()
        self.assertIn("   0:\t0c000000 \tjal\thelper", out)
        self.assertIn("   4:\t3c020000 \tlui\t$2,<sym>", out)
        self.assertFalse([line for line in out if "R_MIPS" in line])

    def test_gp_offsets_normalize_but_stack_offsets_stay_visible(self) -> None:
        out = self.dump()
        self.assertIn("   8:\t8f838018 \tlw\t$3,<sym>(gp)", out)
        # SP is upper-cased so the scorer's stack wildcards cannot hide a
        # frame or local-offset difference.
        self.assertIn("   c:\t8fa40010 \tlw\t$4,16(SP)", out)


if __name__ == "__main__":
    unittest.main()
