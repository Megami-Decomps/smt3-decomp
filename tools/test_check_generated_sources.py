#!/usr/bin/env python3
"""Generated-source checker controls must never corrupt working sources."""
import importlib.util
from pathlib import Path
import os
import shutil
import signal
import subprocess
import sys
import time
import tempfile
import unittest
from unittest import mock

spec = importlib.util.spec_from_file_location(
    "check_generated_sources", Path(__file__).with_name("check_generated_sources.py"))
check = importlib.util.module_from_spec(spec)
spec.loader.exec_module(check)


class IsolatedControlTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name)
        for name in ("src", "config", "include", "tools", "orig"):
            (self.root / name).mkdir()
        self.unit = self.root / check.STALE_UNIT
        self.unit.parent.mkdir(parents=True)
        self.unit.write_bytes(b"/* user's uncommitted source */\n")
        (self.root / "configure.py").write_text("# fixture\n")
        self.original = self.unit.read_bytes()

    def visit_control(self, control):
        self.assertNotEqual(control, self.root)
        unit = control / check.STALE_UNIT
        self.assertEqual(unit.read_bytes(), self.original + check.STALE.encode())
        self.assertEqual(self.unit.read_bytes(), self.original)
        self.assertFalse(unit.samefile(self.unit))
        return unit

    def test_control_uses_independent_copy_and_cleans_up(self):
        visited = []
        def configure(control):
            unit = self.visit_control(control)
            visited.append(control)
            unit.write_bytes(self.original)
            return [str(check.STALE_UNIT)]
        with mock.patch.object(check, "configure_changes", side_effect=configure):
            self.assertEqual(check.control_changes(self.root), [str(check.STALE_UNIT)])
        self.assertFalse(visited[0].exists())
        self.assertEqual(self.unit.read_bytes(), self.original)

    def test_interrupt_cannot_leave_sentinel_in_working_tree(self):
        def configure(control):
            self.visit_control(control)
            raise KeyboardInterrupt
        with mock.patch.object(check, "configure_changes", side_effect=configure):
            with self.assertRaises(KeyboardInterrupt):
                check.control_changes(self.root)
        self.assertEqual(self.unit.read_bytes(), self.original)

    def test_configure_failure_preserves_working_tree(self):
        def configure(control):
            self.visit_control(control)
            raise subprocess.CalledProcessError(1, ["configure.py"])
        with mock.patch.object(check, "configure_changes", side_effect=configure):
            with self.assertRaises(subprocess.CalledProcessError):
                check.control_changes(self.root)
        self.assertEqual(self.unit.read_bytes(), self.original)

    def test_real_subprocess_detects_only_control_change(self):
        (self.root / "configure.py").write_text(
            "from pathlib import Path\n"
            f"p = Path({str(check.STALE_UNIT)!r})\n"
            f"p.write_bytes(p.read_bytes().replace({check.STALE.encode()!r}, b''))\n")
        self.assertEqual(check.configure_changes(self.root), [])
        self.assertEqual(check.control_changes(self.root), [str(check.STALE_UNIT)])
        self.assertEqual(self.unit.read_bytes(), self.original)

    @unittest.skipUnless(hasattr(signal, "SIGKILL"), "requires uncatchable termination")
    def test_sigkill_cannot_leave_sentinel_in_working_tree(self):
        ready = self.root / "ready"
        script = (
            "import importlib.util, pathlib, time\n"
            f"s = importlib.util.spec_from_file_location('checker', {str(Path(check.__file__).resolve())!r})\n"
            "m = importlib.util.module_from_spec(s); s.loader.exec_module(m)\n"
            "def stopped(control):\n"
            f"    p = pathlib.Path({str(ready)!r})\n"
            "    q = p.with_suffix('.tmp'); q.write_text(str(control)); q.replace(p)\n"
            "    time.sleep(60)\n"
            "m.configure_changes = stopped\n"
            f"m.control_changes(pathlib.Path({str(self.root)!r}))\n")
        process = subprocess.Popen([sys.executable, "-c", script])
        control = None
        try:
            deadline = time.monotonic() + 10
            while not ready.exists() and process.poll() is None and time.monotonic() < deadline:
                time.sleep(0.01)
            self.assertTrue(ready.exists(), "control never reached injected sentinel")
            control = Path(ready.read_text())
            self.visit_control(control)
            os.kill(process.pid, signal.SIGKILL)
            process.wait(timeout=10)
            self.assertEqual(self.unit.read_bytes(), self.original)
            self.assertTrue((control / check.STALE_UNIT).read_bytes().endswith(check.STALE.encode()))
        finally:
            if process.poll() is None:
                process.kill()
            process.wait(timeout=10)
            if control is not None:
                shutil.rmtree(control)

    def test_relative_interpreter_is_absolute_without_resolving_symlinks(self):
        link = self.root / "venv-python"
        link.symlink_to(Path(sys.executable).absolute())
        executable = os.path.relpath(link)
        with mock.patch.object(check.sys, "executable", executable), \
             mock.patch.object(check.subprocess, "run") as run:
            self.assertEqual(check.configure_changes(self.root), [])
        run.assert_called_once_with(
            [str(Path(executable).absolute()), "configure.py", "--force-split"],
            cwd=self.root, check=True)

    def test_main_still_rejects_rewritten_working_sources(self):
        with mock.patch.object(check, "configure_changes", return_value=["src/a.c"]), \
             mock.patch.object(check, "control_changes") as control:
            self.assertEqual(check.main(), 1)
            control.assert_not_called()

    def test_main_rejects_muted_negative_control(self):
        with mock.patch.object(check, "configure_changes", return_value=[]), \
             mock.patch.object(check, "control_changes", return_value=[]):
            self.assertEqual(check.main(), 2)


if __name__ == "__main__":
    unittest.main()
