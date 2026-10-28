#!/usr/bin/env python3
"""Guard the probe's known debugger-interference boundary."""
import ast
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]


class GdbHookTests(unittest.TestCase):
    def test_reorg_avoids_adjacent_eager_entry_breakpoint(self):
        # A structural guard complements, but cannot replace, live qualification.
        tree = ast.parse((ROOT / "tools/cc1probe_gdb.py").read_text())
        install = next(node for node in tree.body
                       if isinstance(node, ast.FunctionDef) and node.name == "install")
        symbols, handlers = set(), set()
        for node in ast.walk(install):
            if not (isinstance(node, ast.Call) and isinstance(node.func, ast.Name)
                    and node.func.id == "Hook" and len(node.args) >= 2):
                continue
            address, handler = node.args[:2]
            if isinstance(handler, ast.Name):
                handlers.add(handler.id)
            if (isinstance(address, ast.Call) and isinstance(address.func, ast.Name)
                    and address.func.id == "sym" and address.args
                    and isinstance(address.args[0], ast.Constant)):
                symbols.add(address.args[0].value)
        self.assertNotIn("fill_eager_delay_slots", symbols)
        self.assertNotIn("hk_fill_eager", handlers)
        self.assertIn("fill_slots_from_thread", symbols)
        self.assertTrue({"hk_fst_entry", "hk_fst_exit",
                         "hk_mtlr_entry", "hk_mtlr_exit"}.issubset(handlers))


if __name__ == "__main__":
    unittest.main()
