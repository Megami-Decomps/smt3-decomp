#!/usr/bin/env python3
"""Tests for tools/docs_index.py, the levers and drafts index."""
import pathlib
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))
import docs_index  # noqa: E402

ROOT = pathlib.Path(__file__).resolve().parent.parent


class KeywordTests(unittest.TestCase):
    def test_prose_words_that_are_also_mnemonics_are_not_indexed(self) -> None:
        """"and"/"or"/"move" in prose would otherwise match every grep."""
        self.assertEqual(docs_index.keywords("the base and the index, or move it"), "")

    def test_code_spans_are_indexed(self) -> None:
        self.assertEqual(docs_index.keywords("retail has `beql` not `beq`"), "beq beql")

    def test_registers_and_float_ops_are_indexed(self) -> None:
        got = docs_index.keywords("`mov.s $f20, $f0` after the `abs.s`")
        self.assertEqual(got, "$f0 $f20 abs.s mov.s")

    def test_a_mnemonic_is_listed_once(self) -> None:
        self.assertEqual(docs_index.keywords("`sll` then `sll` again"), "sll")


class SummaryTests(unittest.TestCase):
    def test_first_sentence_only(self) -> None:
        got = docs_index.one_line("- A `u8` read is alias set 0. The rest is detail.")
        self.assertEqual(got, "A u8 read is alias set 0.")

    def test_long_summaries_are_cut_on_a_word(self) -> None:
        got = docs_index.one_line("- " + "word " * 80)
        self.assertLessEqual(len(got), 160)
        self.assertTrue(got.endswith("…"))


class DraftRowTests(unittest.TestCase):
    def setUp(self) -> None:
        self.dir = ROOT / "docs/nonmatching"

    def test_every_draft_page_entry_is_indexed(self) -> None:
        pages = sorted(self.dir.rglob("*.md"))
        headings = sum(
            sum(1 for l in p.read_text().split("\n") if l.startswith("## "))
            for p in pages)
        self.assertEqual(len(docs_index.draft_rows()), headings)

    def test_unit_and_line_point_at_the_entry(self) -> None:
        for fn, unit, _size, _d, page, line in docs_index.draft_rows()[:40]:
            text = (ROOT / page).read_text().split("\n")[int(line) - 1]
            self.assertTrue(text.startswith("## "), page)
            self.assertIn(fn, text)

    def test_wrapped_distance_stays_in_one_tsv_cell(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            root = pathlib.Path(directory)
            drafts = root / "docs/nonmatching"
            drafts.mkdir(parents=True)
            page = drafts / "example.md"
            for separator in ("\n", "\n\t"):
                with self.subTest(separator=separator):
                    page.write_text(
                        "## example (sdk/math, 0x28C)\n\n"
                        f"51 of 155 words{separator}match; 32 bytes uncovered.\n")
                    with patch.object(docs_index, "ROOT", root), \
                            patch.object(docs_index, "DRAFTS", drafts):
                        rows = docs_index.draft_rows()
                    self.assertEqual(len(rows), 1)
                    self.assertEqual(rows[0][3], "51 of 155 words match")
                    serialized = "\t".join(rows[0])
                    self.assertEqual(len(serialized.splitlines()), 1)
                    self.assertEqual(len(serialized.split("\t")), 6)


class IndexFreshnessTests(unittest.TestCase):
    def test_the_committed_indexes_are_current(self) -> None:
        """--check is what CI runs; keep the committed tables in step."""
        with patch("sys.argv", ["docs_index.py", "--check"]):
            self.assertEqual(docs_index.main(), 0,
                             "run python3 tools/docs_index.py")


if __name__ == "__main__":
    unittest.main()
