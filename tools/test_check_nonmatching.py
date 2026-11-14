#!/usr/bin/env python3
"""Pin the draft accounting, which has misread the tree in both directions."""
import contextlib
import errno
import os
import io
import re
import subprocess
import sys
import unittest
import tempfile
from pathlib import Path
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parent))
import check_nonmatching as cn
import check_irx_nonmatching as irx


class ConditionalSelection(unittest.TestCase):
    def test_whitespace_guards(self):
        self.assertIn("NON_MATCHING", cn.conditional_names(" #  ifdef\tNON_MATCHING\n"))

    def test_expression_and_elif_guards(self):
        self.assertEqual(cn.conditional_names("#if defined(NON_MATCHING)\n#elif !defined(FIX_BUGS)\n"),
                         {"if", "elif", "defined", "NON_MATCHING", "FIX_BUGS"})

    def test_comments_and_strings_do_not_select_modes(self):
        self.assertFalse(cn.conditional_names('/* #ifdef NON_MATCHING */\nconst char *s = "FIX_BUGS";\n// #if FIX_BUGS\n') & {"FIX_BUGS", "NON_MATCHING"})

    def test_identifier_boundaries_and_nonconditional_directives(self):
        self.assertNotIn("NON_MATCHING", cn.conditional_names("#define FIX_BUGS 1\n#if NOT_NON_MATCHING\n"))

    def test_macro_aliases_preserve_modes(self):
        for mode in ("FIX_BUGS", "NON_MATCHING"):
            self.assertIn(mode, cn.conditional_names("#define ALIAS " + mode + "\n#if ALIAS\n"))

    def test_non_directive_mode_use_is_conservatively_selected(self):
        self.assertIn("FIX_BUGS", cn.conditional_names("int fixed = FIX_BUGS;"))

    def test_alias_reference_runs_both_compiler_modes(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            unit = root / "unit.c"
            unit.write_text("#define APPLY FIX_BUGS\n#if APPLY\nint fixed;\n#endif\n")
            modes = []
            def compile(path, mode, output):
                modes.append(mode)
                Path(output).write_text("")
                return ""
            with patch.object(cn, "ROOT", root), patch.object(cn, "compile_unit", side_effect=compile):
                self.assertEqual(cn.check_draft_unit(unit, [])[0], 0)
            self.assertEqual(modes, [False, True])

    def test_spliced_guard_and_continued_comment(self):
        slash = chr(92)
        self.assertIn("NON_MATCHING", cn.conditional_names("#if defined(" + slash + "\nNON_MATCHING)\n"))
        self.assertEqual(cn.conditional_names("// ignored " + slash + "\n#ifdef FIX_BUGS\n"), set())

    def test_unit_selection_uses_condition_tokens(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            src = root / "src/smt3"
            src.mkdir(parents=True)
            for name, content in [("a.c", "# ifdef NON_MATCHING\n"),
                                  ("b.c", "#if defined(FIX_BUGS)\n"),
                                  ("c.c", "/* #ifdef NON_MATCHING */")]:
                (src / name).write_text(content)
            with patch.object(cn, "ROOT", root):
                self.assertEqual(cn.units([]), [src / "a.c", src / "b.c"])
                self.assertEqual(cn.units(["explicit.c"]), [Path("explicit.c")])


class ParallelDraftChecks(unittest.TestCase):
    def test_worker_bound_is_explicit(self):
        for value in ("1", "4", "8"):
            self.assertEqual(cn.worker_count(value), int(value))
        for value in ("0", "-1", "9", "many", "1.5"):
            with self.assertRaises(cn.argparse.ArgumentTypeError):
                cn.worker_count(value)

    def test_controls_finish_before_any_work_and_reports_stay_ordered(self):
        from threading import Event, Lock
        release = Event()
        controlled = []
        completed = []
        lock = Lock()
        paths = [Path("slow.c"), Path("fast.c"), Path("last.c")]
        def work(path, known):
            self.assertEqual(controlled, [True])
            self.assertEqual(known, [(8, 32)])
            if path == paths[0]:
                self.assertTrue(release.wait(5), "second worker did not start")
            with lock:
                completed.append(path)
            if path != paths[0]:
                release.set()
            return (1 if path == paths[1] else 0), [path.name]
        with patch.object(cn, "units", return_value=paths), \
                patch.object(cn, "control", side_effect=lambda: controlled.append(True)), \
                patch.object(cn, "symbol_extents", return_value=[(8, 32)]), \
                patch.object(cn, "check_draft_unit", side_effect=work), \
                contextlib.redirect_stdout(io.StringIO()) as out:
            self.assertEqual(cn.main(["--jobs", "2"]), 1)
        self.assertEqual(out.getvalue(), "slow.c\nfast.c\nlast.c\n")
        self.assertNotEqual(completed[0], paths[0])

    def test_serial_default_and_positionals_are_unchanged(self):
        with patch.object(cn, "units", return_value=[Path("a.c")]) as units, \
                patch.object(cn, "control") as control, \
                patch.object(cn, "symbol_extents", return_value=[]), \
                patch.object(cn, "check_draft_unit", return_value=(0, ["ok"])) as work, \
                patch.object(cn, "ThreadPoolExecutor") as pool, \
                contextlib.redirect_stdout(io.StringIO()) as out:
            self.assertEqual(cn.main(["src/smt3/a.c"]), 0)
            units.assert_called_once_with(["src/smt3/a.c"])
            control.assert_called_once_with()
            work.assert_called_once_with(Path("a.c"), [])
            pool.assert_not_called()
        self.assertEqual(out.getvalue(), "ok\n")

    def test_failed_control_cannot_launch_workers(self):
        with patch.object(cn, "units", return_value=[Path("a.c")]), \
                patch.object(cn, "control", side_effect=SystemExit("bad control")), \
                patch.object(cn, "ThreadPoolExecutor") as pool, \
                patch.object(cn, "check_draft_unit") as work:
            with self.assertRaises(SystemExit):
                cn.main(["--jobs", "4"])
            pool.assert_not_called()
            work.assert_not_called()

    def test_empty_selection_does_not_launch_controls(self):
        with patch.object(cn, "units", return_value=[]), \
                patch.object(cn, "control") as control, \
                contextlib.redirect_stdout(io.StringIO()) as out:
            self.assertEqual(cn.main(["--jobs", "4"]), 0)
            control.assert_not_called()
        self.assertEqual(out.getvalue(), "no NON_MATCHING drafts\n")

    def test_each_mode_owns_and_cleans_its_temporary_directory(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            unit = root / "unit.c"
            unit.write_text("#ifdef NON_MATCHING\nint f(void){return 1;}\n#else\n"
                            'INCLUDE_ASM(int, "u", f);\n#endif\n#ifdef FIX_BUGS\n#endif\n')
            outputs, modes = [], []
            def compile(path, mode, asm):
                outputs.append(Path(asm))
                modes.append(mode)
                Path(asm).write_text("lw $2,%lo(D_00000010)($3)\n")
                return "unit.c:4: failure" if mode else ""
            with patch.object(cn, "ROOT", root), patch.object(cn, "compile_unit", side_effect=compile):
                count, lines = cn.check_draft_unit(unit, [(8,32)])
            self.assertEqual(count, 2)
            self.assertEqual(modes, [False, True])
            self.assertNotEqual(outputs[0].parent, outputs[1].parent)
            self.assertTrue(all(not p.parent.exists() for p in outputs))
            self.assertIn("undefined with NON_MATCHING: D_00000010", lines[0])
            self.assertIn("DOES NOT COMPILE with NON_MATCHING + FIX_BUGS", lines[1])
            self.assertEqual(lines[2], "    unit.c:4: failure")

    def test_exception_cleans_temporary_assembly(self):
        with tempfile.TemporaryDirectory() as directory:
            root=Path(directory);unit=root/"unit.c";unit.write_text("int f(void){}")
            outputs=[]
            def compile(path, mode, asm):
                outputs.append(Path(asm));Path(asm).write_text("partial")
                raise RuntimeError("failed")
            with patch.object(cn, "ROOT", root), patch.object(cn, "compile_unit", side_effect=compile):
                with self.assertRaises(RuntimeError):
                    cn.check_draft_unit(unit, [])
            self.assertFalse(outputs[0].parent.exists())


class EffectiveSymbolExtents(unittest.TestCase):
    def extents(self, source):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            config = root / "config/smt3"
            config.mkdir(parents=True)
            (config / "symbol_addrs.txt").write_text(source)
            with patch.object(cn, "ROOT", root):
                return cn.symbol_extents()

    def test_shadowed_start_cannot_hide_interior_reference(self):
        known = self.extents("owner = 0x8; // size:0x20\n"
                             "owner = 0x10; // size:0x4\n")
        self.assertEqual(known, [(8, 32)])
        self.assertEqual(cn.undefined_data(".word D_00000010\n", known),
                         ["D_00000010"])

    def test_unsized_first_definition_shadows_generated_extent(self):
        known = self.extents("owner = 0x100;\nowner = 0x8; // size:0x20\n"
                             "other = 0x200; // size:0x10\n")
        self.assertEqual(known, [(0x200, 16)])
        self.assertEqual(cn.undefined_data(".word D_00000010\n", known), [])

    def test_same_address_first_size_wins(self):
        self.assertEqual(self.extents("owner = 0x8; // size:0x8\n"
                                      "owner = 0x8; // size:0x20\n"), [(8, 8)])

    def test_distinct_alias_names_retain_their_extents(self):
        self.assertEqual(self.extents("owner = 0x8; // size:0x20\n"
                                      "part = 0x10; // size:0x4\n"),
                         [(8, 32), (16, 4)])

    def test_spacing_decimal_sizes_and_comments(self):
        self.assertEqual(self.extents("// fake = 0x10; // size:0x4\n"
                                      "  owner = 0x8 ; // size:32\n"), [(8, 32)])

    def test_malformed_effective_size_fails_closed(self):
        with self.assertRaises(ValueError):
            self.extents("owner = 0x8; // size:not_a_size\n")

    def test_shadowed_invalid_size_is_not_read(self):
        self.assertEqual(self.extents("owner = 0x8; // size:0x20\n"
                                      "owner = 0x10; // size:bad\n"), [(8, 32)])

    def test_no_effective_sized_symbols_fails_closed(self):
        with self.assertRaises(SystemExit):
            self.extents("owner = 0x8;\nowner = 0x10; // size:0x20\n")


class SwallowedLabels(unittest.TestCase):
    asm = "\tlw\t$2,%lo(D_00000010)($3)\n"

    def test_label_inside_a_sized_symbol_is_reported(self):
        self.assertEqual(cn.undefined_data(self.asm, [(0x8, 0x20)]), ["D_00000010"])

    def test_label_that_starts_a_symbol_or_a_gap_is_not(self):
        self.assertEqual(cn.undefined_data(self.asm, [(0x10, 4)]), [])
        self.assertEqual(cn.undefined_data(self.asm, [(0x8, 0x8)]), [])

    def test_symbol_at_alias_is_not(self):
        src = "SYMBOL_AT(sdfGrOt_t, D_00000010, owner, 0x8);"
        self.assertEqual(cn.undefined_data("D_00000010 = owner + 0x8\n" + self.asm, [(0x8, 0x20)], src), [])


class ActiveAssembly(unittest.TestCase):
    extent = [(8, 32)]
    use = '\tlw $2,%lo(D_00000010)($3)\n'
    def missing(self, asm=None, source=''):
        return cn.undefined_data(self.use if asm is None else asm, self.extent, source)
    def test_real_reference_remains_rejected(self):
        self.assertEqual(self.missing(), ['D_00000010'])
    def test_comment_cannot_define_alias(self):
        self.assertEqual(self.missing(source='/* SYMBOL_AT(int, D_00000010, owner, 8); */'), ['D_00000010'])
    def test_disabled_define_cannot_define_alias(self):
        self.assertEqual(self.missing(source='#if 0\n#define D_00000010 owner\n#endif'), ['D_00000010'])
    def test_dropped_include_cannot_define_symbol(self):
        self.assertEqual(self.missing(source='INCLUDE_RODATA(int, "u", D_00000010);'), ['D_00000010'])
    def test_active_assignment_needs_no_source_exception(self):
        self.assertEqual(self.missing('D_00000010 = owner + 8\n' + self.use), [])
    def test_local_label_is_definition(self):
        self.assertEqual(self.missing('D_00000010:\n.word 0\n' + self.use), [])
    def test_common_is_definition(self):
        for directive in ['comm','lcomm']:
            self.assertEqual(self.missing('.'+directive+' D_00000010,4,4\n'+self.use), [])
    def test_literal_and_comments_are_not_references(self):
        self.assertEqual(self.missing('.ascii "D_00000010\\\\\\\" # data"\n# D_00000010\n'), [])
    def test_declarations_are_not_references_or_definitions(self):
        for directive in ['globl','global','weak','local','extern']:
            prefix='.'+directive+' D_00000010\n'
            self.assertEqual(self.missing(prefix), [])
            self.assertEqual(self.missing(prefix+self.use), ['D_00000010'])
    def test_alias_rhs_still_checked(self):
        for assignment in ['D_00000020 = D_00000010 + 8', '.set D_00000020,D_00000010+8', '.equ D_00000020,D_00000010+8', '.set other,D_00000010+8']:
            self.assertEqual(self.missing(assignment), ['D_00000010'])
    def test_words_keep_reference(self):
        self.assertEqual(self.missing('.word D_00000010\n'), ['D_00000010'])


class CompilerFailures(unittest.TestCase):
    def ee_result(self, status, stderr="", stdout=""):
        result = subprocess.CompletedProcess([], status, stdout, stderr)
        with patch.object(cn.subprocess, "run", return_value=result):
            return cn.compile_unit(Path("src/smt3/unit.c"))

    def test_ee_loader_uses_override_or_default(self):
        for override in (None, "", "/tmp/loader path"):
            with self.subTest(override=override), patch.dict(os.environ, {}, clear=True):
                if override is not None:
                    os.environ["SMT3_I386_LIBDIR"] = override
                with patch.object(cn.subprocess, "run", return_value=
                                  subprocess.CompletedProcess([], 0, "", "")) as run:
                    self.assertEqual(cn.compile_unit(Path("unit.c")), "")
                libdir = Path(override) if override else cn.ROOT / "tools/glibc32"
                self.assertEqual(run.call_args.args[0][:3],
                                 [str(libdir / "ld-linux.so.2"), "--library-path",
                                  str(libdir)])

    def test_ee_loader_launch_errors_fail_closed(self):
        for code in (errno.ENOENT, errno.EACCES, errno.ENOEXEC):
            error = OSError(code, os.strerror(code), "ld-linux.so.2")
            with self.subTest(code=code), patch.object(cn.subprocess, "run",
                                                      side_effect=error):
                diagnostic = cn.compile_unit(Path("unit.c"))
                self.assertIn("could not start compiler", diagnostic)
                self.assertIn(str(error), diagnostic)

    def irx_result(self, status, stderr="", stdout=""):
        result = subprocess.CompletedProcess([], status, stdout, stderr)
        with patch.object(irx.irx_config, "load", return_value={}), \
                patch.object(irx.irx_config, "path", return_value=Path("unit.c")), \
                patch.object(irx.subprocess, "run", return_value=result), \
                contextlib.redirect_stdout(io.StringIO()):
            return irx.check("test", Path("shim"))

    def test_ee_warning_with_a_successful_exit_is_allowed(self):
        self.assertEqual(self.ee_result(0, "unit.c:3: warning: unused variable\n"), "")

    def test_ee_failure_without_a_source_diagnostic_is_reported(self):
        for stderr in ("", "cc1: internal compiler error\n",
                       "unit.c:3: warning: unused variable\n"):
            with self.subTest(stderr=stderr):
                self.assertIn("status 1", self.ee_result(1, stderr))

    def test_ee_signal_and_loader_diagnostic_are_preserved(self):
        diagnostic = "ld-linux.so.2: cannot load shared object\n"
        self.assertIn(diagnostic.strip(), self.ee_result(-11, diagnostic))

    def test_ee_source_error_is_preserved(self):
        diagnostic = "unit.c:3: syntax error"
        self.assertEqual(self.ee_result(1, diagnostic), diagnostic)

    def test_irx_failure_without_an_error_diagnostic_is_reported(self):
        for stderr in ("", "cc1: terminated\n", "unit.c:3: warning: unused variable\n"):
            with self.subTest(stderr=stderr):
                self.assertGreater(self.irx_result(1, stderr), 0)

    def test_irx_warning_with_a_successful_exit_is_allowed(self):
        self.assertEqual(self.irx_result(0, "unit.c:3: warning: unused variable\n"), 0)


class FixBugsControl(unittest.TestCase):
    def test_control_passes_with_the_real_compiler(self):
        cn.control()


class DraftNames(unittest.TestCase):
    def test_nested_fix_bugs_does_not_hide_outer_fallback(self) -> None:
        text = ('#ifdef NON_MATCHING\nvoid a(void) {\n'
                '#ifdef FIX_BUGS\nfixed();\n#else\nretail();\n#endif\n}\n'
                '#else\nINCLUDE_ASM(const s32, "u", a);\n#endif\n')
        self.assertEqual(cn.draft_names(text), ["a"])

    def test_nested_conditionals_and_elif_keep_the_outer_depth(self) -> None:
        text = ('#if PLATFORM\n#ifdef NON_MATCHING\n'
                '#if FIRST\n#ifndef SECOND\nint value;\n#endif\n'
                '#elif THIRD\nint other;\n#else\nint last;\n#endif\n'
                '#else\nINCLUDE_ASM(const s32, "u", a);\n#endif\n#endif\n')
        self.assertEqual(cn.draft_names(text), ["a"])

    def test_nested_fallback_keeps_all_names_once(self) -> None:
        text = ('#ifdef NON_MATCHING\nvoid a(void) {}\n#else\n'
                '#if FIRST\nINCLUDE_ASM(const s32, "u", a);\n#else\n'
                'INCLUDE_ASM(const s32, "u", b);\n#endif\n'
                '#ifdef NON_MATCHING\nvoid helper(void) {}\n#else\n'
                'INCLUDE_ASM(const s32, "u", c);\n#endif\n#endif\n')
        self.assertEqual(cn.draft_names(text), ["a", "b", "c"])

    def test_comments_do_not_add_names_or_change_nesting(self) -> None:
        text = ('/* #ifdef NON_MATCHING\n#else\n'
                'INCLUDE_ASM(const s32, "u", fake);\n#endif */\n'
                '#ifdef NON_MATCHING /* draft */\n'
                'const char *url = "https://example.test/#endif";\n'
                '/*\n#endif\n*/\n#else\n'
                '// INCLUDE_ASM(const s32, "u", fake);\n'
                'INCLUDE_ASM(const s32, "u", real);\n#endif\n')
        self.assertEqual(cn.draft_names(text), ["real"])

    def test_multiline_fallback_and_non_draft_else_are_distinct(self) -> None:
        text = ('#ifdef NON_MATCHING\n'
                '#if FIX_BUGS\nint a;\n#else\nint b;\n#endif\n#endif\n'
                '#ifdef NON_MATCHING\nvoid real(void) {}\n#else\n'
                'INCLUDE_ASM(\nconst s32,\n"u",\nreal\n);\n#endif\n')
        self.assertEqual(cn.draft_names(text), ["real"])

    def test_one_block_can_hold_several_drafts(self) -> None:
        """Reading only the first name hid four drafted functions in sdfVu1."""
        text = ('#ifdef NON_MATCHING\n'
                'void a(void) {}\n'
                'void b(void) {}\n'
                '#else\n'
                'INCLUDE_ASM(const s32, "u", a);\n'
                'INCLUDE_ASM(const s32, "u", b);\n'
                '#endif\n')
        self.assertEqual(cn.draft_names(text), ["a", "b"])

    def test_a_guarded_helper_is_not_a_draft(self) -> None:
        """No #else means no asm fallback, so nothing is being drafted."""
        self.assertEqual(cn.draft_names('#ifdef NON_MATCHING\nstatic int h;\n#endif\n'), [])

    def test_separate_blocks_each_count(self) -> None:
        text = ('#ifdef NON_MATCHING\nvoid a(void) {}\n#else\n'
                'INCLUDE_ASM(const s32, "u", a);\n#endif\n'
                '#ifdef NON_MATCHING\nvoid b(void) {}\n#else\n'
                'INCLUDE_ASM(const s32, "u", b);\n#endif\n')
        self.assertEqual(cn.draft_names(text), ["a", "b"])

    def test_a_plain_include_asm_outside_a_guard_is_not_a_draft(self) -> None:
        self.assertEqual(cn.draft_names('INCLUDE_ASM(const s32, "u", a);\n'), [])


class TreeState(unittest.TestCase):
    def test_drafts_are_found_in_the_tree(self) -> None:
        """A parser that silently matched nothing would pass every other test."""
        self.assertGreater(sum(len(v) for v in cn.draft_functions().values()), 100)


@unittest.skipUnless((cn.ROOT / "tools/compilers/ee-gcc2.96").is_dir(), "ee-gcc not installed")
class ImplicitDeclarationTests(unittest.TestCase):
    def test_retained_c_requires_a_declaration_before_use(self):
        with tempfile.TemporaryDirectory() as directory:
            source = Path(directory) / "unit.c"
            source.write_text('#ifdef NON_MATCHING\n'
                              'int retained(void) { return undeclared(1); }\n'
                              '#else\nint matching(void) { return 0; }\n#endif\n')
            self.assertIn("implicit declaration", cn.compile_unit(source))
            normal = subprocess.run([str(cn.ROOT / "tools/cc.sh"), "-DSKIP_ASM",
                                     str(source), "-o", str(Path(directory) / "unit.o")],
                                    cwd=cn.ROOT, capture_output=True, text=True)
            self.assertEqual(normal.returncode, 0, normal.stderr)

    def test_early_prototype_allows_the_retained_call(self):
        with tempfile.TemporaryDirectory() as directory:
            source = Path(directory) / "unit.c"
            source.write_text('extern int declared(int);\n'
                              'int retained(void) { return declared(1); }\n')
            self.assertEqual(cn.compile_unit(source), "")


if __name__ == "__main__":
    unittest.main(verbosity=0)
