#!/usr/bin/env python3
"""Tests for tools/find_similar.py, the matched-function template search."""
import pathlib
import sys
import unittest

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))
import find_similar as fs  # noqa: E402

ROOT = pathlib.Path(__file__).resolve().parent.parent


def seq(*words):
    """A function record carrying just the sequences the comparisons read."""
    return {"seq": [list(words), [], [], [w.split()[0] for w in words]],
            "n": len(words), "calls": [], "state": "asm", "name": "x", "unit": "u",
            "addr": 0}


class OperandTests(unittest.TestCase):
    def test_registers_and_immediates_are_separated(self) -> None:
        self.assertEqual(fs.operands("v0,0x10(a0)"), (["v0", "a0"], [0x10]))

    def test_float_and_vector_registers_count_as_registers(self) -> None:
        regs, imms = fs.operands("$f20,$vf10xyzw")
        self.assertEqual(regs, ["$f20", "$vf10xyzw"])
        self.assertEqual(imms, [])

    def test_a_negative_immediate_keeps_its_sign(self) -> None:
        self.assertEqual(fs.operands("sp,sp,-96"), (["sp", "sp"], [-96]))

    def test_the_symbol_objdump_appends_is_not_an_operand(self) -> None:
        self.assertEqual(fs.operands("0x285d60 <ChkAiHojo>"), ([], [0x285D60]))


class TokenTests(unittest.TestCase):
    """The tiers must coarsen in one direction and nothing else."""

    def test_the_four_tiers_drop_what_they_say_they_drop(self) -> None:
        got = fs.tokens(0x1000, "lw", "v0,0x10(a0)", 0x100000, 0x2E6E50)
        self.assertEqual(got, ("lw v0,a0 16", "lw v0,a0", "lw  16", "lw"))

    def test_a_call_target_is_wildcarded_at_every_tier(self) -> None:
        a = fs.tokens(0x100000, "jal", "0x200000 <foo>", 0x100000, 0x2E6E50)
        b = fs.tokens(0x100000, "jal", "0x210000 <bar>", 0x100000, 0x2E6E50)
        self.assertEqual(a, b)

    def test_a_branch_target_becomes_an_offset_so_two_copies_agree(self) -> None:
        a = fs.tokens(0x100000, "beqz", "a0,0x100020", 0x100000, 0x2E6E50)
        b = fs.tokens(0x200000, "beqz", "a0,0x200020", 0x100000, 0x2E6E50)
        self.assertEqual(a, b)
        self.assertIn("32", a[0])

    def test_a_target_outside_text_is_left_alone(self) -> None:
        """A %lo immediate that happens to sit last is data, not a branch."""
        got = fs.tokens(0x100000, "bltz", "a0,0x900000", 0x100000, 0x2E6E50)
        self.assertEqual(got[0], "bltz a0 9437184")


class DifferenceTests(unittest.TestCase):
    def test_equal_sequences_have_no_differences(self) -> None:
        a, b = seq("lw v0,a0 16", "jr ra "), seq("lw v0,a0 16", "jr ra ")
        self.assertEqual(fs.differences(a, b), [])

    def test_a_changed_immediate_is_reported_once(self) -> None:
        a, b = seq("lw v0,a0 16"), seq("lw v0,a0 32")
        got = fs.differences(a, b)
        self.assertEqual(len(got), 1)
        self.assertIn("16", got[0])
        self.assertIn("32", got[0])

    def test_a_differing_callee_is_reported(self) -> None:
        a, b = seq("jal  "), seq("jal  ")
        a["calls"], b["calls"] = ["ChkAiHojoMax"], ["ChkAiHojo"]
        got = fs.differences(a, b)
        self.assertEqual(len(got), 1)
        self.assertEqual(got[0].split(), ["call", "ChkAiHojoMax", "template", "ChkAiHojo"])

    def test_an_extra_instruction_shows_against_a_dash(self) -> None:
        a, b = seq("lw v0,a0 16", "nop "), seq("lw v0,a0 16")
        self.assertEqual(len(fs.differences(a, b)), 1)
        self.assertIn("template -", fs.differences(a, b)[0])


class FamilyTests(unittest.TestCase):
    def setUp(self) -> None:
        def f(name, state):
            return {"name": name, "state": state, "unit": "u", "n": 8, "addr": 0}
        self.funcs = {n: f(n, s) for n, s in (
            ("effBTLModelLook", "c"), ("effBTLModelLook2", "asm"),
            ("effBTLModelHide", "c"), ("nbPnl_rsetTex1", "c"), ("func_00206308", "asm"))}

    def test_a_longer_name_with_the_same_stem_is_family(self) -> None:
        got = [g["name"] for g in fs.family(self.funcs, "effBTLModelLook2")]
        self.assertIn("effBTLModelLook", got)

    def test_a_short_shared_prefix_is_not_family(self) -> None:
        got = [g["name"] for g in fs.family(self.funcs, "effBTLModelLook2")]
        self.assertNotIn("nbPnl_rsetTex1", got)

    def test_only_matched_functions_are_offered(self) -> None:
        self.funcs["effBTLModelHide"]["state"] = "asm"
        self.assertEqual([g["name"] for g in fs.family(self.funcs, "effBTLModelLook2")],
                         ["effBTLModelLook"])

    def test_a_splat_placeholder_name_has_no_family(self) -> None:
        self.assertEqual(fs.family(self.funcs, "func_00206308"), [])


class ClusterTests(unittest.TestCase):
    def test_functions_sharing_a_tier_hash_group_together(self) -> None:
        funcs = {n: {"name": n, "hash": h, "state": "asm", "n": 8, "addr": a, "unit": "u"}
                 for n, h, a in (("a", ["1", "1", "1", "1"], 0),
                                 ("b", ["2", "1", "1", "1"], 4),
                                 ("c", ["3", "3", "3", "3"], 8))}
        self.assertEqual([[f["name"] for f in g] for g in fs.clusters(funcs, 0)], [])
        self.assertEqual([[f["name"] for f in g] for g in fs.clusters(funcs, 1)], [["a", "b"]])


@unittest.skipUnless((ROOT / "asm" / "smt3").exists(), "needs configure.py --force-split")
class CorpusTests(unittest.TestCase):
    """The one invariant worth paying for: the corpus covers the split asm exactly."""

    def test_every_function_of_every_unit_is_covered(self) -> None:
        funcs = fs.corpus("smt3")
        inv = fs.inventory("smt3")
        self.assertEqual(len(funcs), len(inv))
        for f in inv:
            got = funcs[f["name"]]
            self.assertEqual(len(got["seq"][0]), f["n"], f["name"])
            self.assertEqual(got["addr"], f["addr"], f["name"])

    def test_a_tier_is_never_finer_than_the_one_above_it(self) -> None:
        for f in fs.corpus("smt3").values():
            for k in range(3):
                self.assertEqual(len(f["seq"][k]), len(f["seq"][k + 1]), f["name"])


if __name__ == "__main__":
    unittest.main()
