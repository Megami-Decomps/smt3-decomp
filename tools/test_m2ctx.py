"""Coverage for the m2c context flattening."""

import sys
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parent))

import m2ctx  # noqa: E402


class ContextTests(unittest.TestCase):
    def context(self, source_text: str, header_text: str) -> str:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "include").mkdir()
            (root / "include" / "unit.h").write_text(header_text)
            (root / "src" / "smt3" / "main").mkdir(parents=True)
            source = root / "src" / "smt3" / "main" / "code_00100000.c"
            source.write_text(source_text)
            with patch.object(m2ctx, "ROOT", root):
                return m2ctx.context(source)

    def test_only_declarations_survive(self) -> None:
        text = self.context(
            '#include "unit.h"\n\nint hidden(void) { return 1; }\n',
            "struct Unit { int flags; };\nint declared(void);\n",
        )
        self.assertIn("int declared(void);", text)
        self.assertIn("struct Unit", text)
        self.assertNotIn("hidden", text)

    def test_versions_reads_the_repository_configuration(self) -> None:
        # The -DVERSION_<V> define is driven by these keys, not a hardcoded list.
        self.assertIn("smt3", m2ctx.versions())

    def test_version_define_selects_the_native_layout(self) -> None:
        header = (
            "#ifdef VERSION_SMT3\n"
            "struct Unit { int smt3; };\n"
            "#else\n"
            "struct Unit { int other; };\n"
            "#endif\n"
        )
        with patch.object(m2ctx, "versions", return_value=["smt3"]):
            text = self.context('#include "unit.h"\n', header)
        self.assertIn("int smt3;", text)
        self.assertNotIn("int other;", text)


if __name__ == "__main__":
    unittest.main()
