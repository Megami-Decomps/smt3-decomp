#!/usr/bin/env python3
"""Regression checks for the frame-counter survey's unit-step tokens."""
import importlib.util
from pathlib import Path
import tempfile
import unittest

SPEC = importlib.util.spec_from_file_location("frame_timer_sites", Path(__file__).with_name("frame_timer_sites.py"))
SURVEY = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(SURVEY)


class UnitStepTests(unittest.TestCase):
    def scan(self, statement):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "fixture.c"
            path.write_text(statement + "\n")
            return list(SURVEY.scan(path))

    def test_supported_unit_literals(self):
        for value in ("1", "1.0", "1.0f"):
            for operator in ("+=", "-="):
                with self.subTest(value=value, operator=operator):
                    self.assertEqual(len(self.scan(f"work->timer {operator} {value};")), 1)

    def test_increment_and_decrement(self):
        for statement in ("work->timer++;", "--work->timer;", "work->timer--;", "++work->timer;"):
            with self.subTest(statement=statement):
                self.assertEqual(len(self.scan(statement)), 1)

    def test_nonunit_tokens_do_not_match_prefix(self):
        for value in ("1.5f", "1.25", "10", "11", "1e3", "1.0e3", "1UL", "1.0foo"):
            for operator in ("+=", "-="):
                with self.subTest(value=value, operator=operator):
                    self.assertEqual(self.scan(f"work->timer {operator} {value};"), [])

    def test_existing_survey_filters(self):
        for statement in ("local++;", "for (work->timer = 0; work->timer < 4; work->timer++) {}", "// work->timer += 1;"):
            with self.subTest(statement=statement):
                self.assertEqual(self.scan(statement), [])


if __name__ == "__main__":
    unittest.main()
