#!/usr/bin/env python3
"""Tests for tools/check_drift.py and the struct parser it reads the tree with."""

from __future__ import annotations

import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

import check_drift  # noqa: E402
from cparse import member_names, structs  # noqa: E402


class MemberNameTests(unittest.TestCase):
    def test_several_declarators_on_one_line(self) -> None:
        self.assertEqual(member_names(" s32 a, b, c"), ["a", "b", "c"])

    def test_an_array_is_named_without_its_extent(self) -> None:
        self.assertEqual(member_names(" u8 Name[24]"), ["Name"])

    def test_an_inline_struct_takes_the_name_after_its_brace(self) -> None:
        self.assertEqual(member_names(" struct { u32 inner; } nest"), ["nest"])

    def test_a_line_that_only_opens_a_definition_declares_nothing(self) -> None:
        self.assertEqual(member_names(" struct {"), [])

    def test_a_function_pointer_is_named_not_its_parameters(self) -> None:
        self.assertEqual(member_names(" void (*update)(struct SdfPool *, s32)"), ["update"])

    def test_a_bitfield_drops_its_width(self) -> None:
        self.assertEqual(member_names(" u16 flags : 3"), ["flags"])

    def test_a_comment_between_type_and_name_is_ignored(self) -> None:
        self.assertEqual(member_names(" u32 /* 0x10 */ color"), ["color"])


class StructTests(unittest.TestCase):
    SOURCE = """\
typedef struct X {
    u8 Name[24];
    s32 a, b;
    struct { u32 inner; } nest;
} X;

union U {
    f32 m[6];
    u64 words[3];
};

struct Tagged {
    u32 only;
};
"""

    def test_each_definition_with_its_members(self) -> None:
        got = {name: members for _, name, members, _ in structs(self.SOURCE)}
        self.assertEqual(got["X"], ["Name", "a", "b", "nest"])
        self.assertEqual(got["U"], ["m", "words"])
        self.assertEqual(got["Tagged"], ["only"])

    def test_a_nested_definitions_members_do_not_leak_outward(self) -> None:
        got = {name: members for _, name, members, _ in structs(self.SOURCE)}
        self.assertNotIn("inner", got["X"])

    def test_the_kind_is_reported(self) -> None:
        kinds = {name: kind for kind, name, _, _ in structs(self.SOURCE)}
        self.assertEqual(kinds["U"], "union")
        self.assertEqual(kinds["X"], "struct")

    BRACE_STYLES = """\
struct NextLine
{
    u8 Name[24];
};

typedef struct QWORD_ALIGNED
{
    u32 reg;
} Aligned_t;

struct OneLine { u32 a; };

typedef struct { f32 x, y; } sdfVector2_t;

typedef struct Fwd Fwd;
struct Opaque;
"""

    def test_a_brace_on_its_own_line_still_opens_a_definition(self) -> None:
        """The tree writes 1135 of its definitions this way; missing them made the
        drift check blind to more than half the tree."""
        got = {name: members for _, name, members, _ in structs(self.BRACE_STYLES)}
        self.assertEqual(got["NextLine"], ["Name"])

    def test_a_macro_between_the_keyword_and_the_brace(self) -> None:
        got = {name: members for _, name, members, _ in structs(self.BRACE_STYLES)}
        self.assertEqual(got["Aligned_t"], ["reg"])

    def test_a_definition_closed_on_its_opening_line(self) -> None:
        got = {name: members for _, name, members, _ in structs(self.BRACE_STYLES)}
        self.assertEqual(got["OneLine"], ["a"])
        self.assertEqual(got["sdfVector2_t"], ["x", "y"])

    def test_a_forward_declaration_defines_nothing(self) -> None:
        got = {name for _, name, _, _ in structs(self.BRACE_STYLES)}
        self.assertNotIn("Fwd", got)
        self.assertNotIn("Opaque", got)


class ConventionTests(unittest.TestCase):
    def invented(self, name):
        return bool(check_drift.INVENTED.match(name)
                    and not check_drift.CONVENTION.search(name))

    def test_a_camel_case_type_with_no_suffix_is_drift(self) -> None:
        for name in ("SdfPool", "EffRecordPool", "Dds3Thing", "VuWork"):
            self.assertTrue(self.invented(name), name)

    def test_the_trees_own_spelling_is_not_drift(self) -> None:
        for name in ("sdfMatPacket_t", "dds3Basic_t", "scrStackData_u",
                     "sdfSortOtNode_tag", "beHeader_s", "NB_PHASE"):
            self.assertFalse(self.invented(name), name)

    def test_a_suffixed_camel_case_name_is_accepted(self) -> None:
        """IDA recovered these spellings; they are not ours to re-style."""
        self.assertFalse(self.invented("Sprite3D_t"))
        self.assertFalse(self.invented("BillTable_t"))

    def test_an_offset_named_member_is_drift(self) -> None:
        for m in ("unk_18", "unk0x4", "UNK_8", "field_0x10", "dummy1", "unused2"):
            self.assertTrue(check_drift.UNNAMED.match(m), m)

    def test_a_named_member_is_not_drift(self) -> None:
        for m in ("flag", "serialID", "pad", "next", "unknownReason"):
            self.assertIsNone(check_drift.UNNAMED.match(m), m)


class BaselineTests(unittest.TestCase):
    def test_the_tree_as_it_stands_has_no_drift_outside_the_baseline(self) -> None:
        """The whole point of the baseline: today's tree passes, a new name fails."""
        found, baseline = check_drift.scan(), check_drift.load_baseline()
        new = sorted(set(found) - baseline)
        self.assertEqual(new, [], f"new drift: {new[:10]}")

    def test_the_baseline_holds_both_kinds(self) -> None:
        baseline = check_drift.load_baseline()
        self.assertTrue(any(k.startswith("type ") for k in baseline))
        self.assertTrue(any(k.startswith("member ") for k in baseline))

    def test_an_external_name_is_never_reported(self) -> None:
        found = check_drift.scan()
        for name in ("FILE", "_Bigint", "__sbuf"):
            self.assertNotIn(f"type {name}", found)


if __name__ == "__main__":
    unittest.main()
