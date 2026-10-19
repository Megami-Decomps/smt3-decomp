#!/usr/bin/env python3
"""Tests for tools/progress_site.py: history, objdiff's sparse measures, VU microcode."""

from __future__ import annotations

import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

import progress_site  # noqa: E402


def entry(commit: str, date: str, code: int) -> dict:
    return {"commit": commit, "date": date, "matched_code": code, "total_code": 1000,
            "matched_functions": 0, "total_functions": 10}


class HistoryTests(unittest.TestCase):
    def test_a_rerun_replaces_its_commit_instead_of_adding_a_point(self) -> None:
        history = [entry("a", "2026-10-01T00:00:00+00:00", 10),
                   entry("b", "2026-10-02T00:00:00+00:00", 20)]
        merged = progress_site.merge_history(history, entry("b", "2026-10-02T00:00:00+00:00", 25))
        self.assertEqual([(e["commit"], e["matched_code"]) for e in merged], [("a", 10), ("b", 25)])

    def test_points_are_ordered_by_date_across_time_zones(self) -> None:
        history = [entry("late", "2026-10-02T01:00:00+02:00", 20)]
        # 01:00+02:00 on the 2nd is 23:00 UTC on the 1st, before 23:30 UTC.
        merged = progress_site.merge_history(history, entry("later", "2026-10-01T23:30:00+00:00", 30))
        self.assertEqual([e["commit"] for e in merged], ["late", "later"])


class SparseMeasureTests(unittest.TestCase):
    # objdiff omits zero-valued fields and writes byte counts as strings.
    REPORT = {
        "measures": {"total_code": "2000", "matched_code": "500", "total_functions": 4,
                     "matched_functions": 1, "total_units": 2},
        "units": [
            {"name": "kernel/done", "measures": {"total_code": "500", "matched_code": "500",
                                                 "total_functions": 1, "matched_functions": 1},
             "functions": [{"name": "doneFunc", "size": "500", "fuzzy_match_percent": 100.0}]},
            {"name": "code_00100000", "measures": {"total_code": "1500", "total_functions": 3},
             "functions": [{"name": "todoFunc", "size": "1500"}]},
        ],
    }
    AUDIT = {
        "measures": {"total_code": "2468", "matched_code": "500", "total_functions": 8,
                     "matched_functions": 1, "total_units": 4},
        "categories": [
            {"id": "game", "name": "Atlus game/engine", "measures": REPORT["measures"]},
            {"id": "data", "name": "Split data segments", "measures": {"total_units": 1}},
        ],
    }

    def test_missing_measures_count_as_zero(self) -> None:
        self.assertEqual(progress_site.totals(self.REPORT["units"][1]["measures"]),
                         {"matched_code": 0, "total_code": 1500,
                          "matched_functions": 0, "total_functions": 3})
        self.assertEqual(progress_site.badge("code bytes", 500, 2000)["message"], "25.00%")
        self.assertEqual(progress_site.badge("functions", 0, 0)["message"], "0.00%")

    def test_page_marks_only_matched_functions_and_skips_uncompared_categories(self) -> None:
        page = progress_site.render("smt3", self.REPORT, self.AUDIT, [], None, "0" * 40,
                                    "2026-10-18T00:00:00+00:00")
        self.assertIn('<li class="ok"><span>doneFunc</span>', page)
        self.assertIn('<li class="todo"><span>todoFunc</span>', page)
        self.assertIn("<td>Split data segments</td><td class=\"num\">1</td><td>not compared", page)
        self.assertIn("500 / 2,000 bytes", page)


class DraftTests(unittest.TestCase):
    REPORT = SparseMeasureTests.REPORT

    def page(self, drafts: dict) -> str:
        return progress_site.render("smt3", self.REPORT, SparseMeasureTests.AUDIT, [], None,
                                    "0" * 40, "2026-10-18T00:00:00+00:00", drafts)

    def test_a_draft_is_counted_apart_from_matched_functions(self) -> None:
        page = self.page({"code_00100000": {"todoFunc"}})
        self.assertIn("<h2>Non-matching drafts</h2>", page)
        self.assertIn('<p class="big">1</p>', page)
        self.assertIn("of 3 unmatched functions", page)
        self.assertIn('<li class="draft"><span>todoFunc</span>', page)

    def test_drafts_outside_the_published_scope_are_not_counted(self) -> None:
        page = self.page({"elsewhere/unit": {"otherFunc"}})
        self.assertIn('<p class="big">0</p>', page)
        self.assertNotIn('class="draft"', page)


class VuMicrocodeTests(unittest.TestCase):
    AUDIT = {
        "measures": SparseMeasureTests.AUDIT["measures"],
        "categories": SparseMeasureTests.AUDIT["categories"] + [
            # objdiff sees one unmatched label spanning all of .vutext.
            {"id": "vu1", "name": "VU0/VU1 microcode",
             "measures": {"total_code": "59312", "total_functions": 1, "total_units": 2}},
        ],
    }
    COVERAGE = {"section_bytes": 59312, "microcode_bytes": 16488,
                "source_bytes": 16488, "instructions": 2061}

    def page(self, vu: dict | None) -> str:
        return progress_site.render("smt3", SparseMeasureTests.REPORT, self.AUDIT, [], None,
                                    "0" * 40, "2026-10-18T00:00:00+00:00", None, vu)

    def test_assembled_microcode_is_credited_against_the_whole_section(self) -> None:
        page = self.page(self.COVERAGE)
        self.assertIn("<h2>VU microcode</h2>", page)
        self.assertIn("2,061 instructions assembled from", page)
        self.assertIn('<td class="num">16,488 / 59,312</td><td class="num">&ndash;</td>', page)
        self.assertIn("other 42,824 bytes are VIF headers", page)

    def test_without_the_retail_image_the_row_stays_objdiff_s(self) -> None:
        page = self.page(None)
        self.assertNotIn("<h2>VU microcode</h2>", page)
        self.assertIn('<td class="num">0 / 59,312</td>', page)


if __name__ == "__main__":
    unittest.main()
