#!/usr/bin/env python3
"""Guard debugger-free preflight and the known hook-interference boundary."""
import ast
import hashlib
import json
import shutil
import subprocess
import sys
import tempfile
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


class NohookPreflightTests(unittest.TestCase):
    """Execute the real wrapper with synthetic compiler inputs and no GDB."""

    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix="c1p-test-")
        self.addCleanup(self.temp.cleanup)
        self.base = Path(self.temp.name)
        self.repo = self.base / "isolated-repository"
        self.tools = self.repo / "tools"
        self.tools.mkdir(parents=True)
        self.config = self.repo / "config/smt3/compilers.txt"
        self.config.parent.mkdir(parents=True)
        self.config.write_text("kernel/probe ee-gcc2.96\n")
        self.compiler = self.tools / "compilers/ee-gcc2.96/lib/gcc-lib/ee/2.96-ee-001003-1/cc1"
        self.compiler.parent.mkdir(parents=True)
        self.compiler.write_bytes(b"synthetic compiler fixture, never executed\n")
        # Only the isolated wrapper copy gets a synthetic pin. Real sha256sum
        # still checks it, including the mismatched-hash regression below.
        wrapper = (ROOT / "tools/cc1probe.sh").read_text()
        pin = "d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1"
        self.assertEqual(wrapper.count("want=" + pin), 1)
        wrapper = wrapper.replace("want=" + pin,
                                  "want=" + hashlib.sha256(self.compiler.read_bytes()).hexdigest())
        self.wrapper = self.tools / "cc1probe.sh"
        self.wrapper.write_text(wrapper)
        self.source = self.repo / "candidate.c"
        self.source.write_text("void fixture(void) {}\n")
        self.out = self.base / "output"
        self.calls = self.base / "calls.jsonl"
        # An allowlist, rather than the host PATH with a fake gdb prepended,
        # guarantees command -v gdb fails even on machines with GDB installed.
        bin_dir = self.base / "bin"
        bin_dir.mkdir()
        for command in ("dirname", "mkdir", "awk", "sha256sum", "cut", "rm",
                        "sha1sum", "seq", "ln", "readlink", "sed", "cmp", "mv", "cp"):
            executable = shutil.which(command)
            self.assertIsNotNone(executable, command)
            (bin_dir / command).symlink_to(executable)
        self.env = {
            "PATH": str(bin_dir), "TMPDIR": str(self.base / "tmp"),
            "CC1PROBE_MIRROR_BASE": str(self.base / "m") + "/",
            "FIXTURE_CALLS": str(self.calls),
        }
        driver = self.tools / "cc.sh"
        driver.write_text("#!" + sys.executable + "\n" + r"""
import json
import os
from pathlib import Path
import sys
args = sys.argv[1:]
log = Path(os.environ["FIXTURE_CALLS"])
prior = len(log.read_text().splitlines()) if log.exists() else 0
with log.open("a") as f:
    f.write(json.dumps({"args": args, "version": os.environ.get("SMT3_VERSION"),
                        "unit": os.environ.get("SMT3_AS_UNIT"),
                        "keep_s": os.environ.get("SMT3_KEEP_S"),
                        "libdir": os.environ.get("SMT3_I386_LIBDIR")}) + "\n")
if os.environ.get("FIXTURE_FAIL_CALL") == str(prior + 1):
    sys.exit(19)
assembly = "fixture assembly\n"
if "-da" in args and os.environ.get("FIXTURE_DIFFERENT_DUMPS"):
    assembly = "different dumped assembly\n"
Path(os.environ["SMT3_KEEP_S"]).write_text(assembly)
Path(args[args.index("-o") + 1]).write_bytes(b"fixture object\n")
if "-da" in args:
    Path(args[args.index("-dumpbase") + 1] + ".01.rtl").write_text("fixture RTL\n")
""")
        driver.chmod(0o755)

    def run_probe(self, *options, extra_env=None):
        return subprocess.run(
            ["/bin/bash", str(self.wrapper), str(self.source),
             "--as-unit", "src/smt3/kernel/probe.c", "--out", str(self.out), *options],
            env={**self.env, **(extra_env or {})}, capture_output=True, text=True,
            timeout=15,
        )

    def read_calls(self):
        return [json.loads(line) for line in self.calls.read_text().splitlines()]

    def assert_early_failure(self, result, code, diagnostic):
        self.assertEqual(result.returncode, code, result.stdout + result.stderr)
        self.assertIn(diagnostic, result.stderr)
        self.assertFalse(self.calls.exists())
        self.assertFalse(self.out.exists())

    def assert_no_hook_artifacts(self):
        for name in ("probe.gdb", "loader", "decisions.log", "decisions.raw", "gdb.stdout"):
            self.assertFalse((self.out / name).exists(), name)

    def test_default_requires_gdb_before_compile_or_output_creation(self):
        self.assert_early_failure(self.run_probe(), 3, "gdb (with Python) is required")

    def test_nohook_without_gdb_forwards_flags_and_collects_dumps(self):
        flags = ["-O2", "-DFIXTURE=two words"]
        result = self.run_probe("--nohook", "--cflag", flags[0], "--cflag", flags[1])
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        calls = self.read_calls()
        self.assertEqual(len(calls), 2)
        for call, name, prefix in zip(calls, ("candidate", "plain"),
                                      (["-da", "-dumpbase", str(self.out / "rtl")], [])):
            self.assertEqual(call["args"], prefix + flags + [str(self.source), "-o",
                             str(self.out / "baseline" / ("cc.o" if name == "candidate" else "plain.o"))])
            self.assertEqual(call["version"], "smt3")
            self.assertEqual(call["unit"], "src/smt3/kernel/probe.c")
            self.assertEqual(call["keep_s"], str(self.out / "baseline" / (name + ".s")))
            self.assertIsNone(call["libdir"])
        self.assertEqual((self.out / "candidate.s").read_text(), "fixture assembly\n")
        self.assertEqual((self.out / "rtl.01.rtl").read_text(), "fixture RTL\n")
        self.assert_no_hook_artifacts()

    def test_nohook_no_dumps_compiles_once_without_rtl(self):
        result = self.run_probe("--nohook", "--no-dumps", "--cflag", "-O2")
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        calls = self.read_calls()
        self.assertEqual(len(calls), 1)
        self.assertEqual(calls[0]["args"], ["-O2", str(self.source), "-o",
                                          str(self.out / "baseline/cc.o")])
        self.assertEqual((self.out / "candidate.s").read_text(), "fixture assembly\n")
        self.assertFalse(list(self.out.rglob("*.rtl")))
        self.assert_no_hook_artifacts()

    def test_nohook_still_rejects_unknown_version(self):
        self.assert_early_failure(self.run_probe("--nohook", "--version", "other"),
                                  2, "unknown version: other")

    def test_nohook_still_rejects_other_configured_compiler(self):
        self.config.write_text("kernel/probe ee-gcc2.9\n")
        self.assert_early_failure(self.run_probe("--nohook"), 3, "selects ee-gcc2.9")

    def test_nohook_still_rejects_missing_compiler(self):
        self.compiler.unlink()
        self.assert_early_failure(self.run_probe("--nohook"), 3, "no compiler at")

    def test_nohook_still_rejects_bad_compiler_hash(self):
        self.compiler.write_bytes(b"wrong compiler\n")
        self.assert_early_failure(self.run_probe("--nohook"), 3, "cc1 sha256 mismatch")

    def test_nohook_preserves_compile_failure_exits(self):
        for call_number, diagnostic in (("1", "baseline tools/cc.sh failed rc=19"),
                                        ("2", "plain tools/cc.sh failed")):
            with self.subTest(call_number=call_number):
                self.calls.unlink(missing_ok=True)
                result = self.run_probe("--nohook", extra_env={"FIXTURE_FAIL_CALL": call_number})
                self.assertEqual(result.returncode, 4, result.stdout + result.stderr)
                self.assertIn(diagnostic, result.stdout + result.stderr)
                self.assertEqual(len(self.read_calls()), int(call_number))
                self.assert_no_hook_artifacts()

    def test_nohook_success_does_not_certify_dump_plain_parity(self):
        result = self.run_probe("--nohook", extra_env={"FIXTURE_DIFFERENT_DUMPS": "1"})
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertNotEqual((self.out / "candidate.s").read_bytes(),
                            (self.out / "baseline/plain.s").read_bytes())
        self.assertNotIn("verify: OK", result.stdout + result.stderr)
        self.assert_no_hook_artifacts()


if __name__ == "__main__":
    unittest.main()
