#!/usr/bin/env python3
"""Pin the draft accounting, which has misread the tree in both directions."""
import re
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import check_nonmatching as cn


class DraftNames(unittest.TestCase):
    def test_one_block_can_hold_several_drafts(self) -> None:
        """Reading only the first name hid four drafted functions in sdfVu1."""
        text = ('#ifdef NON_MATCHING\n'
                'void a(void) {}\n'
                'void b(void) {}\n'
                '#else\n'
                'INCLUDE_ASM(const s32, "u", a);\n'
                'INCLUDE_ASM(const s32, "u", b);\n'
                '#endif\n')
        self.assertEqual(cn.draft_names(text), ["a", "b"])

    def test_a_guarded_helper_is_not_a_draft(self) -> None:
        """No #else means no asm fallback, so nothing is being drafted."""
        self.assertEqual(cn.draft_names('#ifdef NON_MATCHING\nstatic int h;\n#endif\n'), [])

    def test_separate_blocks_each_count(self) -> None:
        text = ('#ifdef NON_MATCHING\nvoid a(void) {}\n#else\n'
                'INCLUDE_ASM(const s32, "u", a);\n#endif\n'
                '#ifdef NON_MATCHING\nvoid b(void) {}\n#else\n'
                'INCLUDE_ASM(const s32, "u", b);\n#endif\n')
        self.assertEqual(cn.draft_names(text), ["a", "b"])

    def test_a_plain_include_asm_outside_a_guard_is_not_a_draft(self) -> None:
        self.assertEqual(cn.draft_names('INCLUDE_ASM(const s32, "u", a);\n'), [])


class TreeState(unittest.TestCase):
    def test_every_draft_compiles_and_no_entry_is_stale(self) -> None:
        """The tree as it stands passes both directions of the check."""
        self.assertEqual(cn.stale_entries(), 0)

    def test_drafts_are_found_in_the_tree(self) -> None:
        """A parser that silently matched nothing would pass every other test."""
        self.assertGreater(sum(len(v) for v in cn.draft_functions().values()), 100)


if __name__ == "__main__":
    unittest.main(verbosity=0)
