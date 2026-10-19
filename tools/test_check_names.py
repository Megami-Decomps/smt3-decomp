#!/usr/bin/env python3
"""check_names.py has to keep a convention fix apart from real drift."""
import importlib.util
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
spec = importlib.util.spec_from_file_location("cn", ROOT / "tools/check_names.py")
cn = importlib.util.module_from_spec(spec)
spec.loader.exec_module(cn)

IDB = [
    ("effPCPMuzzleFlash_tag", "struct"),
    ("dds3MoverBasicWorkData_t", "struct"),
    ("sdfMemHandle_t", "struct"),
    ("itfpanel_t", "struct"),
    ("NB_KOUKA_TYPE", "enum"),
    ("dds3HermitianTrackPoly_tag", "struct"),
]


def classify(names):
    tree = {name: ["src/x.c"] for name in names}
    recovered, external, resuffixed, invented, near, unmatched = cn.classify(tree, IDB)
    return {
        "recovered": recovered,
        "external": external,
        "resuffixed": [pair[0] for pair in resuffixed],
        "invented": invented,
        "near": {pair[0]: pair[1] for pair in near},
        "unmatched": unmatched,
    }


class ClassifyTests(unittest.TestCase):
    def test_exact_idb_name_is_recovered(self) -> None:
        self.assertEqual(classify(["sdfMemHandle_t"])["recovered"], ["sdfMemHandle_t"])

    def test_t_for_ida_tag_is_the_convention_not_drift(self) -> None:
        got = classify(["effPCPMuzzleFlash_t"])
        self.assertEqual(got["resuffixed"], ["effPCPMuzzleFlash_t"])
        self.assertEqual(got["near"], {})

    def test_a_dropped_word_is_reported_as_drift(self) -> None:
        got = classify(["dds3MoverBasicWork_t"])
        self.assertEqual(got["near"], {"dds3MoverBasicWork_t": "dds3MoverBasicWorkData_t"})

    def test_case_only_difference_is_reported_as_drift(self) -> None:
        """A capital D where every other dds3 type is lowercase."""
        got = classify(["Dds3HermitianTrackPoly_t"])
        self.assertEqual(
            got["near"], {"Dds3HermitianTrackPoly_t": "dds3HermitianTrackPoly_tag"}
        )
        self.assertEqual(got["resuffixed"], [], "a case change is not a suffix change")

    def test_newlib_name_is_external_despite_breaking_the_convention(self) -> None:
        self.assertEqual(classify(["FILE"])["external"], ["FILE"])

    def test_off_convention_name_with_no_counterpart_is_invented(self) -> None:
        self.assertEqual(classify(["EffRecordPool"])["invented"], ["EffRecordPool"])

    def test_conforming_name_with_no_counterpart_is_not_called_invented(self) -> None:
        got = classify(["fldSomethingNobodyTyped_t"])
        self.assertEqual(got["unmatched"], ["fldSomethingNobodyTyped_t"])
        self.assertEqual(got["invented"], [])

    def test_enum_tag_is_suggested_only_for_an_all_caps_name(self) -> None:
        self.assertEqual(classify(["nbKouka_t"])["near"], {})
        self.assertEqual(classify(["NB_KOUKA_TYP"])["near"], {"NB_KOUKA_TYP": "NB_KOUKA_TYPE"})


class ParseTests(unittest.TestCase):
    def test_typedefs_are_found_for_struct_union_and_enum(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "src").mkdir()
            (root / "include").mkdir()
            (root / "src/a.c").write_text(
                "typedef struct { int a; } foo_t;\n"
                "typedef union { int a; } bar_u;\n"
                "typedef enum { X } baz_e;\n"
                "struct notatypedef { int a; };\n"
            )
            (root / "include/b.h").write_text("typedef struct { int a; } foo_t;\n")
            got = cn.tree_typedefs(root)
        self.assertEqual(sorted(got), ["bar_u", "baz_e", "foo_t"])
        self.assertEqual(len(got["foo_t"]), 2, "a type defined twice records both files")

    def test_load_idb_keeps_the_kind(self) -> None:
        with tempfile.NamedTemporaryFile("w", suffix=".txt", delete=False) as handle:
            handle.write("# comment\nfoo_t struct 16\nBAR enum 4\n")
            path = handle.name
        self.assertEqual(cn.load_idb(path), [("foo_t", "struct"), ("BAR", "enum")])


if __name__ == "__main__":
    unittest.main()
