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


class DueGateTests(unittest.TestCase):
    def done(self, source):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "fixture.c"
            path.write_text(source)
            return {name: done for _, name, done, _ in SURVEY.scan(path)}

    def test_gated_counter_converted_and_ungated_open(self):
        marks = self.done(
            "void f(void)\n{\n"
            "    work->gated++;\n"
            "    if (SDF_FRAME_DUE(work->age)) {\n"
            "        if (work->a) {\n            work->inner++;\n        }\n"
            "        work->gated2--;\n"
            "    }\n"
            "    work->ungated++;\n}\n")
        self.assertEqual(marks, {"gated": False, "inner": True, "gated2": True, "ungated": False})

    def test_compound_condition(self):
        marks = self.done("if (work->on && SDF_FRAME_DUE(work->age)) {\n    work->t++;\n}\nwork->u++;\n")
        self.assertEqual(marks, {"t": True, "u": False})

    def test_negated_form_gates_the_else(self):
        marks = self.done(
            "if (!SDF_FRAME_DUE(work->age)) {\n    work->skipped++;\n"
            "} else if (work->x) {\n    work->a++;\n} else {\n    work->b++;\n}\n"
            "work->after++;\n")
        self.assertEqual(marks, {"skipped": False, "a": True, "b": True, "after": False})

    def test_else_of_a_later_if_is_not_gated(self):
        marks = self.done(
            "if (!SDF_FRAME_DUE(work->age)) {\n} else {\n    work->a++;\n}\n"
            "if (work->y) {\n    work->p++;\n} else {\n    work->q++;\n}\n")
        self.assertEqual(marks, {"a": True, "p": False, "q": False})

    def test_uses_count_tick_and_due(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "fixture.c"
            path.write_text("a = SDF_FRAME_TICK(x);\nif (SDF_FRAME_DUE(x)) {}\n/* SDF_FRAME_DUE(y) */\n")
            counts = SURVEY.uses(path)
        self.assertEqual((counts["tick"], counts["due"]), (1, 1))


if __name__ == "__main__":
    unittest.main()
