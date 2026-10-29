#!/usr/bin/env python3
"""Regression tests for source discovery and progress configuration/labels."""

from __future__ import annotations

import io
import json
import os
import sys
import tempfile
import types
import unittest
from contextlib import redirect_stdout
from pathlib import Path
from unittest.mock import Mock, patch


ROOT = Path(__file__).resolve().parents[1]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

# Source-discovery helpers do not use Ninja, and CI runs these tests before
# installing build dependencies.
sys.modules.setdefault("ninja_syntax", types.ModuleType("ninja_syntax"))

import configure  # noqa: E402
from tools import progress  # noqa: E402

VERSION = "smt3"
SERIAL = "SLUS_209.11"


class ConfigureTests(unittest.TestCase):
    def test_one_shot_force_split_is_not_persisted_and_cached_split_is_reused(self) -> None:
        writer = Mock()
        configure.write_configure_build(
            writer, [f"config/{VERSION}/{SERIAL}.yaml"], [VERSION, "--force-split", "--no-split", "--clean"]
        )
        self.assertEqual(
            writer.build.call_args,
            (("build.ninja", "configure"), {
                "implicit": [f"config/{VERSION}/{SERIAL}.yaml"],
                "variables": {"args": VERSION},
            }),
        )

        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            config = root / f"config/{VERSION}"
            build = root / f"build/{VERSION}"
            config.mkdir(parents=True)
            build.mkdir(parents=True)
            yaml = config / f"{SERIAL}.yaml"
            yaml.write_text(f"version: {VERSION}\n")
            (config / "symbol_addrs.txt").write_text("symbol = 0x1;\n")
            (config / "reloc_addrs.txt").write_text("reloc = 0x2;\n")
            (build / f"{SERIAL}.ld").write_text("main_SBSS_START = .;\nmain_BSS_START = .;\n")

            def run_forced_step(command, **kwargs):
                if "splat" in command:
                    (root / f"asm/{VERSION}").mkdir(parents=True, exist_ok=True)

            with patch.object(configure, "ROOT", root), \
                    patch.dict(configure.VERSIONS, {VERSION: {"serial": SERIAL}}), \
                    patch.object(configure.subprocess, "run", side_effect=run_forced_step) as run:
                configure.run_splat(VERSION, yaml, force=True)
                forced_run_count = run.call_count
                self.assertGreater(forced_run_count, 0)

                # Ninja's regeneration command omits the one-shot flag. With
                # the stamp written by the forced run, its ordinary configure
                # pass follows the real cache gate and does not split again.
                configure.run_splat(VERSION, yaml, force=False)
                self.assertEqual(run.call_count, forced_run_count)

                # A later symbol rename must refresh generated references even
                # when the previous explicit invocation skipped splitting.
                (config / "symbol_addrs.txt").write_text("renamed_symbol = 0x1;\n")
                configure.run_splat(VERSION, yaml, force=False)
                self.assertGreater(run.call_count, forced_run_count)

    def test_no_split_does_not_disable_future_symbol_regeneration(self) -> None:
        writer = Mock()
        configure.write_configure_build(writer, [], [VERSION, "--no-split"])
        self.assertEqual(writer.build.call_args.kwargs["variables"]["args"], VERSION)

    def test_clean_is_not_repeated_on_source_triggered_regeneration(self) -> None:
        writer = Mock()
        configure.write_configure_build(
            writer, [f"src/{VERSION}/game/test.c"], [VERSION, "--clean", "other"]
        )
        self.assertEqual(
            writer.build.call_args.kwargs["variables"]["args"], f"{VERSION} other"
        )

    def test_bss_alignment_is_inserted_once_per_symbol(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            script = Path(tmp) / f"{SERIAL}.ld"
            script.write_text(
                "    main_SBSS_START = .;\n"
                "    main_BSS_START = .;\n"
            )
            configure.align_bss(script)
            text = script.read_text()
            self.assertEqual(text.count(". = ALIGN(128);"), 2)
            self.assertLess(text.index("ALIGN(128)"), text.index("main_SBSS_START"))

    def test_linker_objects_keep_script_order_without_duplicates(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            script = Path(tmp) / f"{SERIAL}.ld"
            script.write_text(
                f"    build/{VERSION}/main/code_00100000.o(.text)\n"
                f"    build/{VERSION}/main/code_00100000.o(.rodata)\n"
                f"    build/{VERSION}/hand/crt0.o(.text)\n"
            )
            self.assertEqual(
                configure.linker_objects(script),
                [
                    Path(f"build/{VERSION}/main/code_00100000.o"),
                    Path(f"build/{VERSION}/hand/crt0.o"),
                ],
            )

    def test_unit_cflags_drop_comments_and_keep_flags(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            config = root / f"config/{VERSION}"
            config.mkdir(parents=True)
            (config / "cflags.txt").write_text(
                "# evidence table\n"
                "main/code_00100000  -fno-optimize-sibling-calls  # 12 tail calls\n"
                "\n"
            )
            with patch.object(configure, "ROOT", root):
                self.assertEqual(
                    configure.unit_cflags(VERSION),
                    {"main/code_00100000": "-fno-optimize-sibling-calls"},
                )

    def test_unit_asflags_keep_assembler_options_separate(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            config = root / f"config/{VERSION}"
            config.mkdir(parents=True)
            with patch.object(configure, "ROOT", root):
                self.assertEqual(configure.unit_asflags(VERSION), {})
                (config / "asflags.txt").write_text(
                    "# assembler evidence\n"
                    "  sdk/libm  -G0  # immediate float constants\n\n"
                )
                self.assertEqual(configure.unit_asflags(VERSION), {"sdk/libm": "-G0"})
                self.assertEqual(configure.unit_cflags(VERSION), {})

    def test_source_for_prefers_c_then_asm_then_assets(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / f"src/{VERSION}/main").mkdir(parents=True)
            (root / f"src/{VERSION}/main/code_00100000.c").touch()
            (root / f"asm/{VERSION}/hand").mkdir(parents=True)
            (root / f"asm/{VERSION}/hand/crt0.s").touch()
            with patch.object(configure, "ROOT", root):
                self.assertEqual(
                    configure.source_for(Path(f"build/{VERSION}/src/{VERSION}/main/code_00100000.o"), VERSION),
                    ("c", Path(f"src/{VERSION}/main/code_00100000.c")),
                )
                self.assertEqual(
                    configure.source_for(Path(f"build/{VERSION}/asm/{VERSION}/hand/crt0.o"), VERSION),
                    ("s", Path(f"asm/{VERSION}/hand/crt0.s")),
                )
                self.assertEqual(
                    configure.source_for(Path(f"build/{VERSION}/assets/{VERSION}/vu/prog.o"), VERSION),
                    ("bin", Path(f"assets/{VERSION}/vu/prog.bin")),
                )
                with self.assertRaisesRegex(SystemExit, "no source found"):
                    configure.source_for(Path(f"build/{VERSION}/src/{VERSION}/main/missing.o"), VERSION)

    def test_timestamp_restore_only_reverts_unchanged_existing_files(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            unchanged = root / "unchanged.s"
            changed = root / "changed.s"
            deleted = root / "deleted.s"
            new = root / "new.s"
            unchanged.write_bytes(b"same")
            changed.write_bytes(b"before")
            deleted.write_bytes(b"gone")
            old_times = (1_600_000_000_123_456_789, 1_600_000_001_987_654_321)
            for path in (unchanged, changed, deleted):
                os.utime(path, ns=old_times)

            snapshot = configure.snapshot_generated_files([root])
            unchanged.unlink()
            unchanged.write_bytes(b"same")
            changed.write_bytes(b"after")
            deleted.unlink()
            new.write_bytes(b"new")
            fresh_times = (1_700_000_000_111_222_333, 1_700_000_001_444_555_666)
            for path in (unchanged, changed, new):
                os.utime(path, ns=fresh_times)

            configure.restore_unchanged_timestamps(snapshot)

            self.assertEqual(
                (unchanged.stat().st_atime_ns, unchanged.stat().st_mtime_ns), old_times
            )
            self.assertEqual(changed.stat().st_mtime_ns, fresh_times[1])
            self.assertEqual(new.stat().st_mtime_ns, fresh_times[1])
            self.assertFalse(deleted.exists())

    def test_timestamp_snapshot_skips_symlinks(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            target = root / "target.s"
            link = root / "link.s"
            target.write_bytes(b"target")
            link.symlink_to(target)

            snapshot = configure.snapshot_generated_files([link])

            self.assertEqual(snapshot, {})
            self.assertTrue(link.is_symlink())

    def test_timestamp_restore_does_not_follow_replacement_symlink(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            path = root / "generated.s"
            target = root / "target.s"
            path.write_bytes(b"same")
            target.write_bytes(b"same")
            snapshot = configure.snapshot_generated_files([path])
            path.unlink()
            path.symlink_to(target)
            link_times = (1_700_000_000_111_222_333, 1_700_000_001_444_555_666)
            os.utime(path, ns=link_times, follow_symlinks=False)
            target_mtime = target.stat().st_mtime_ns

            configure.restore_unchanged_timestamps(snapshot)

            self.assertTrue(path.is_symlink())
            self.assertEqual(path.lstat().st_mtime_ns, link_times[1])
            self.assertEqual(target.stat().st_mtime_ns, target_mtime)

    def test_include_directive_accepts_leading_and_internal_comments(self) -> None:
        source = """\
/* retained table */ INCLUDE_RODATA /* generated */ (
    const s32, "main/code_00100000", table
);
"""
        self.assertEqual(
            configure.include_directives(source, context="test"),
            [("RODATA", "main/code_00100000", "table")],
        )

    def test_include_directive_accepts_whitespace_before_parenthesis(self) -> None:
        source = 'INCLUDE_ASM \n (const s32, "main/code_00100000", fallback);'
        self.assertEqual(
            configure.include_directives(source, context="test"),
            [("ASM", "main/code_00100000", "fallback")],
        )

    def test_include_identifier_honors_c_line_splicing(self) -> None:
        source = """\
INCLUDE_\\
RODATA(const s32, "main/code_00100000", table);
INCLUDE\\
_ASM(const s32, "main/code_00100000", fallback);
"""
        self.assertEqual(
            configure.include_directives(source, context="test"),
            [
                ("RODATA", "main/code_00100000", "table"),
                ("ASM", "main/code_00100000", "fallback"),
            ],
        )

    def test_include_tokens_in_comments_and_literals_are_ignored(self) -> None:
        source = """\
// INCLUDE_RODATA(const s32, "hidden", table);
const char *text = "INCLUDE_SDATA(hidden)";
/* INCLUDE_ASM(hidden) */
"""
        self.assertEqual(configure.include_directives(source, context="test"), [])

    def test_malformed_include_token_is_rejected(self) -> None:
        with self.assertRaisesRegex(SystemExit, "unsupported INCLUDE_ASM syntax"):
            configure.include_directives(
                'INCLUDE_ASM + (const s32, "main/code_00100000", fallback);',
                context="test",
            )

    def test_a_units_malformed_include_stops_configuration(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / f"src/{VERSION}").mkdir(parents=True)
            (root / f"src/{VERSION}/unit.c").write_text(
                'INCLUDE_ASM + (const s32, "unit", fallback);\n'
            )
            build = root / f"build/{VERSION}"
            build.mkdir(parents=True)
            (root / f"config/{VERSION}").mkdir(parents=True)
            (build / f"{SERIAL}.ld").write_text(
                f"    build/{VERSION}/src/{VERSION}/unit.o(.text)\n"
            )
            args = types.SimpleNamespace(no_split=True, force_split=False)
            # Enough version metadata that only the malformed directive can fail.
            version_info = {"serial": SERIAL, "title": "Nocturne", "elf_sha1": "0" * 40}
            with patch.object(configure, "ROOT", root), \
                    patch.dict(configure.VERSIONS, {VERSION: version_info}), \
                    patch.object(configure, "i386_prefix", return_value=""), \
                    patch("ninja_syntax.Writer", Mock(), create=True):
                with self.assertRaisesRegex(
                    SystemExit, rf"src/{VERSION}/unit\.c: unsupported INCLUDE_ASM syntax"
                ):
                    configure.write_ninja([VERSION], args)


    def test_a_failed_configure_leaves_build_ninja_untouched(self) -> None:
        """A partial graph is worse than none: ninja reads a truncated file as complete."""
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / f"src/{VERSION}").mkdir(parents=True)
            (root / f"src/{VERSION}/unit.c").write_text(
                'INCLUDE_ASM + (const s32, "unit", fallback);\n'
            )
            build = root / f"build/{VERSION}"
            build.mkdir(parents=True)
            (root / f"config/{VERSION}").mkdir(parents=True)
            (build / f"{SERIAL}.ld").write_text(
                f"    build/{VERSION}/src/{VERSION}/unit.o(.text)\n"
            )
            (root / "build.ninja").write_text("the previous graph\n")
            args = types.SimpleNamespace(no_split=True, force_split=False)
            version_info = {"serial": SERIAL, "title": "Nocturne", "elf_sha1": "0" * 40}
            with patch.object(configure, "ROOT", root), \
                    patch.dict(configure.VERSIONS, {VERSION: version_info}), \
                    patch.object(configure, "i386_prefix", return_value=""), \
                    patch("ninja_syntax.Writer", Mock(), create=True):
                with self.assertRaises(SystemExit):
                    configure.write_ninja([VERSION], args)
            self.assertEqual((root / "build.ninja").read_text(), "the previous graph\n")


class SplatOwnershipTests(unittest.TestCase):
    def setUp(self) -> None:
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        self.root = Path(temporary.name)
        root_patch = patch.object(configure, "ROOT", self.root)
        root_patch.start()
        self.addCleanup(root_patch.stop)
        self.config = self.root / f"config/{VERSION}"
        self.build = self.root / f"build/{VERSION}"
        self.source = self.root / f"src/{VERSION}/game/test.c"
        for path in (self.config, self.build, self.source.parent):
            path.mkdir(parents=True)
        self.yaml = self.config / f"{SERIAL}.yaml"
        self.yaml.write_text("test config\n")
        (self.config / "symbol_addrs.txt").write_text("test symbols\n")
        (self.config / "reloc_addrs.txt").write_text("test relocs\n")
        (self.build / f"{SERIAL}.ld").write_text("")
        self.fallback = 'INCLUDE_ASM(const s32, "game/test", useLiteral);\n'
        self.source.write_text(self.fallback)

    def test_ownership_ignores_bodies_comments_and_generated_data_directives(self) -> None:
        before = configure.splat_digest(VERSION, self.yaml)
        self.source.write_text(
            "// INCLUDE_ASM(const s32, \"fake\", fake);\n"
            + self.fallback
            + "int another(void) { return 2; }\n"
            + 'INCLUDE_RODATA(const s32, "game/test", D_0054A960);\n'
            + 'INCLUDE_SDATA(const s32, "game/test", D_00550000);\n'
            + 'const char *comment = "INCLUDE_ASM(fake)";\n'
        )
        self.assertEqual(configure.splat_digest(VERSION, self.yaml), before)
        self.source.write_text(self.source.read_text().replace("return 2", "return 3"))
        self.assertEqual(configure.splat_digest(VERSION, self.yaml), before)
        self.source.write_text(self.source.read_text().replace(self.fallback, ""))
        self.assertNotEqual(configure.splat_digest(VERSION, self.yaml), before)

    def test_ownership_tracks_source_path_and_include_target(self) -> None:
        before = configure.splat_digest(VERSION, self.yaml)
        self.source.write_text(self.fallback.replace("game/test", "game/other"))
        self.assertNotEqual(configure.splat_digest(VERSION, self.yaml), before)
        self.source.write_text(self.fallback)
        self.source.rename(self.source.with_name("other.c"))
        self.assertNotEqual(configure.splat_digest(VERSION, self.yaml), before)

    def test_promotion_splits_once_and_generated_rodata_does_not_loop(self) -> None:
        asm = self.root / f"asm/{VERSION}/nonmatchings/game/test"
        standalone = asm / "D_0054A960.s"
        splits = []

        def generate(command, **kwargs):
            if "splat" not in command:
                return
            splits.append(command)
            asm.mkdir(parents=True)
            if "INCLUDE_ASM" in self.source.read_text():
                (asm / "useLiteral.s").write_text("function with embedded rodata\n")
            else:
                standalone.write_text("standalone rodata\n")
                # Model include_rodata's source output, which must not become
                # another split input.
                self.source.write_text(
                    self.source.read_text()
                    + 'INCLUDE_RODATA(const s32, "game/test", D_0054A960);\n'
                )

        with patch.object(configure.subprocess, "run", side_effect=generate):
            configure.run_splat(VERSION, self.yaml, force=False)
            self.assertEqual(len(splits), 1)
            self.assertFalse(standalone.exists())
            configure.run_splat(VERSION, self.yaml, force=False)
            self.assertEqual(len(splits), 1)
            self.source.write_text("void useLiteral(void) { use(D_0054A960); }\n")
            configure.run_splat(VERSION, self.yaml, force=False)
            self.assertEqual(len(splits), 2)
            self.assertTrue(standalone.exists())
            self.assertFalse((asm / "useLiteral.s").exists())
            configure.run_splat(VERSION, self.yaml, force=False)
            self.assertEqual(len(splits), 2)
            self.source.write_text(self.fallback)
            configure.run_splat(VERSION, self.yaml, force=False)
            self.assertEqual(len(splits), 3)
            self.assertFalse(standalone.exists())

    def test_first_split_records_newly_generated_source_ownership(self) -> None:
        self.source.unlink()
        splits = []

        def generate(command, **kwargs):
            if "splat" in command:
                splits.append(command)
                (self.root / f"asm/{VERSION}").mkdir(parents=True)
                self.source.write_text(self.fallback)

        with patch.object(configure.subprocess, "run", side_effect=generate):
            configure.run_splat(VERSION, self.yaml, force=False)
            configure.run_splat(VERSION, self.yaml, force=False)
        self.assertEqual(len(splits), 1)

    def test_relocation_edit_invalidates_split(self) -> None:
        before = configure.splat_digest(VERSION, self.yaml)
        (self.config / "reloc_addrs.txt").write_text("changed relocs\n")
        self.assertNotEqual(configure.splat_digest(VERSION, self.yaml), before)

    def test_ninja_regeneration_depends_on_sources_and_relocations(self) -> None:
        (self.build / f"{SERIAL}.ld").write_text(
            f"    build/{VERSION}/src/{VERSION}/game/test.o(.text)\n"
        )
        (self.config / "asflags.txt").write_text("game/test -G0 # native literals\n")
        writer = Mock()

        def make_writer(stream, **kwargs):
            writer.close.side_effect = stream.close
            return writer

        args = types.SimpleNamespace(no_split=True, force_split=False)
        version_info = {"serial": SERIAL, "title": "Nocturne", "elf_sha1": "0" * 40}
        with patch.dict(configure.VERSIONS, {VERSION: version_info}), \
                patch.object(configure, "i386_prefix", return_value=""), \
                patch("ninja_syntax.Writer", side_effect=make_writer, create=True):
            configure.write_ninja([VERSION], args)
        regeneration = next(
            call for call in writer.build.call_args_list
            if call.args[:2] == ("build.ninja", "configure")
        )
        inputs = regeneration.kwargs["implicit"]
        self.assertIn(f"src/{VERSION}/game/test.c", inputs)
        self.assertIn(f"config/{VERSION}/reloc_addrs.txt", inputs)
        for name in ("asflags.txt", "cflags.txt", "compilers.txt"):
            self.assertIn(f"config/{VERSION}/{name}", inputs)
        compile_edges = [call for call in writer.build.call_args_list
                         if len(call.args) > 1 and call.args[1] == "cc"]
        self.assertEqual(len(compile_edges), 2)  # production and SKIP_ASM base
        defaults = configure.TOOLCHAINS[configure.DEFAULT_TOOLCHAIN]["as_flags"]
        for edge in compile_edges:
            self.assertIn(f"config/{VERSION}/asflags.txt", edge.kwargs["implicit"])
            self.assertEqual(edge.kwargs["variables"]["ccasflags"], defaults + " -G0")
            self.assertNotIn("-G0", edge.kwargs["variables"]["cflags"])


class ObjdiffProgressTests(unittest.TestCase):
    def setUp(self) -> None:
        self.units = {
            VERSION: [
                {
                    "name": f"src/{VERSION}/main/example",
                    "target": f"build/{VERSION}/target/main/example.o",
                    "base": f"build/{VERSION}/base/main/example.o",
                },
                {
                    "name": f"asm/{VERSION}/sdk/libc",
                    "target": f"build/{VERSION}/sdk/libc.o",
                    "base": None,
                },
                {
                    "name": f"asm/{VERSION}/vutext",
                    "target": f"build/{VERSION}/vutext.o",
                    "base": None,
                },
                {
                    "name": f"asm/{VERSION}/main/unmatched",
                    "target": f"build/{VERSION}/main/unmatched.o",
                    "base": None,
                },
                {
                    "name": f"asm/{VERSION}/hand/vu/example",
                    "target": f"build/{VERSION}/hand/vu/example.o",
                    "base": None,
                },
                {
                    "name": f"asm/{VERSION}/data/data.data",
                    "target": f"build/{VERSION}/data/data.data.o",
                    "base": None,
                },
            ]
        }
        self.categories = ["game", "sdk", "vu1", "game", "handwritten", "data"]
        with tempfile.TemporaryDirectory() as tmp, \
                patch.object(configure, "ROOT", Path(tmp)), \
                patch.dict(configure.VERSIONS,
                           {VERSION: {"serial": SERIAL, "title": "Nocturne (USA)"}}):
            configure.write_objdiff(self.units)
            self.combined = json.loads((Path(tmp) / "objdiff.json").read_text())
            self.per_version = json.loads(
                (Path(tmp) / "build" / VERSION / "objdiff.json").read_text())
            self.primary = json.loads(
                (Path(tmp) / "build" / VERSION / "progress" / "objdiff.json").read_text())

    def test_combined_config_retains_every_unit_and_object_path(self) -> None:
        rows = self.units[VERSION]
        self.assertEqual(len(self.combined["units"]), len(rows))
        for actual, row in zip(self.combined["units"], rows):
            self.assertEqual(actual["name"], row["name"].split("/", 1)[1])
            self.assertEqual(actual["target_path"], row["target"])
            self.assertEqual(actual.get("base_path"), row["base"])

    def test_categories_separate_vu1_sdk_handwritten_and_game_code(self) -> None:
        self.assertEqual(
            [category["id"] for category in self.combined["progress_categories"]],
            [VERSION, "game", "sdk", "vu1", "handwritten", "data"],
        )
        for unit, category in zip(self.combined["units"], self.categories):
            self.assertEqual(unit["metadata"]["progress_categories"], [VERSION, category])

    def test_per_version_config_rebases_paths_and_keeps_all_categories(self) -> None:
        self.assertEqual(
            [category["id"] for category in self.per_version["progress_categories"]],
            ["game", "sdk", "vu1", "handwritten", "data"],
        )
        for actual, row, category in zip(
            self.per_version["units"], self.units[VERSION], self.categories
        ):
            self.assertEqual(actual["name"], row["name"].split("/", 2)[2])
            self.assertEqual(actual["target_path"], "../../" + row["target"])
            self.assertEqual(
                actual.get("base_path"), "../../" + row["base"] if row["base"] else None
            )
            self.assertEqual(actual["metadata"], {"progress_categories": [category]})

    def test_primary_config_keeps_game_units_and_excludes_sdk_and_vu1(self) -> None:
        self.assertEqual(self.primary["progress_categories"],
                         [{"id": "game", "name": "Atlus game/engine"}])
        expected = [self.units[VERSION][index] for index in (0, 3)]
        self.assertEqual(len(self.primary["units"]), len(expected))
        for actual, row in zip(self.primary["units"], expected):
            self.assertEqual(actual["name"], row["name"].split("/", 2)[2])
            self.assertEqual(actual["target_path"], "../../../" + row["target"])
            self.assertEqual(
                actual.get("base_path"), "../../../" + row["base"] if row["base"] else None
            )

    def test_split_data_segments_are_not_game_code(self) -> None:
        for name in (f"asm/{VERSION}/data/data.data", f"asm/{VERSION}/data/bss.bss"):
            self.assertEqual(configure.objdiff_progress_category(VERSION, name), "data")
        for name in (f"asm/{VERSION}/data/vutext", f"asm/{VERSION}/data/vudata"):
            self.assertEqual(configure.objdiff_progress_category(VERSION, name), "vu1")
        self.assertNotIn("data", [unit["name"] for unit in self.primary["units"]])

    def test_categories_without_units_are_dropped(self) -> None:
        units = {VERSION: [row for row in self.units[VERSION] if "/sdk/" not in row["name"]]}
        with tempfile.TemporaryDirectory() as tmp, \
                patch.object(configure, "ROOT", Path(tmp)), \
                patch.dict(configure.VERSIONS,
                           {VERSION: {"serial": SERIAL, "title": "Nocturne (USA)"}}):
            configure.write_objdiff(units)
            combined = json.loads((Path(tmp) / "objdiff.json").read_text())
            per_version = json.loads(
                (Path(tmp) / "build" / VERSION / "objdiff.json").read_text())
        for config in (combined, per_version):
            self.assertNotIn("sdk", [c["id"] for c in config["progress_categories"]])

    def test_report_targets_use_matching_projects_and_object_dependencies(self) -> None:
        writer = Mock()
        configure.write_objdiff_reports(writer, self.units)
        builds = {call.args[0]: call for call in writer.build.call_args_list}
        all_objects = sorted({p for row in self.units[VERSION]
                              for p in (row["target"], row["base"]) if p})
        self.assertEqual(builds["objdiff"].args, ("objdiff", "phony", all_objects))
        self.assertEqual(builds["report.json.raw"].kwargs, {
            "implicit": all_objects + ["objdiff.json"], "variables": {"project": "."},
        })
        self.assertEqual(builds["report.json"].args,
                         ("report.json", "production_report", "report.json.raw"))
        self.assertEqual(builds["report.json"].kwargs["variables"],
                         {"scope": "all", "versions": f"--version {VERSION}"})
        expected_reports = {"report.json"}
        rows = self.units[VERSION]
        for selected, project, out in (
            (rows, f"build/{VERSION}", f"build/{VERSION}/report.all.json"),
            ([rows[index] for index in (0, 3)], f"build/{VERSION}/progress", f"build/{VERSION}/report.json"),
        ):
            expected_reports.add(out)
            objects = sorted({p for row in selected for p in (row["target"], row["base"]) if p})
            self.assertEqual(builds[f"{out}.raw"].args, (f"{out}.raw", "objdiff_report"))
            self.assertEqual(builds[f"{out}.raw"].kwargs, {
                "implicit": objects + [f"{project}/objdiff.json"], "variables": {"project": project},
            })
            self.assertEqual(builds[out].args, (out, "production_report", f"{out}.raw"))
            self.assertEqual(builds[out].kwargs["variables"],
                             {"scope": VERSION, "versions": f"--version {VERSION}"})
        self.assertEqual(set(builds["report"].args[2]), expected_reports)
        writer.rule.assert_any_call(
            "objdiff_report", f"{configure.OBJDIFF} report generate -p $project -o $out",
            description="objdiff report $out"
        )

    def test_reconciliation_depends_on_the_checked_build_and_its_receipts(self) -> None:
        writer = Mock()
        configure.write_objdiff_reports(writer, self.units)
        reconciled = [call for call in writer.build.call_args_list
                      if call.args[1] == "production_report"]
        self.assertEqual(len(reconciled), 3)
        for call in reconciled:
            dependencies = call.kwargs["implicit"]
            self.assertEqual(call.kwargs["implicit_outputs"],
                             [f"{call.args[0]}.proof.json"])
            for dependency in (
                "tools/production_report.py",
                f"build/{VERSION}/{SERIAL}.ok",
                f"build/{VERSION}/{SERIAL}.elf.proof.json",
                f"config/{VERSION}/symbol_addrs.txt",
                f"orig/{VERSION}/{SERIAL}",
            ):
                self.assertIn(dependency, dependencies)


class ProgressScopeTests(unittest.TestCase):
    def test_sdk_sources_and_drafts_do_not_enter_game_coverage(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            for unit, function in (("kernel/unit", "game_function"), ("sdk/libc", "sdk_function")):
                src = root / f"src/{VERSION}/{unit}.c"
                asm = root / f"asm/{VERSION}/{unit}.s"
                src.parent.mkdir(parents=True, exist_ok=True)
                asm.parent.mkdir(parents=True, exist_ok=True)
                src.write_text(f'INCLUDE_ASM(const s32, "{unit}", {function});\n')
                asm.write_text(f"glabel {function}\n"
                               "    /* 000000 00100000 0800E003 */ jr $ra\n"
                               "    /* 000004 00100004 00000000 */ nop\n"
                               f"endlabel {function}\n")
            with patch.object(progress, "ROOT", root), \
                    patch.object(progress, "draft_functions", return_value={"sdk/libc": {"sdk_function"}}):
                self.assertEqual(progress.version_stats(VERSION), (1, 0, 0, 8, 0, 0))


class ProgressLabelTests(unittest.TestCase):
    # 4 functions and 100 bytes, 2 functions and 25 bytes of them in C, and a
    # draft for one of the two functions that are left.
    STATS = (4, 2, 1, 100, 25, 40)
    VU = {"section_bytes": 800, "microcode_bytes": 400, "source_bytes": 400, "instructions": 50}

    def run_progress(self, *argv, stats=None, vu=...):
        out = io.StringIO()
        with patch.object(sys, "argv", ["progress.py", VERSION, *argv]), \
                patch.object(progress, "version_stats", return_value=stats or self.STATS), \
                patch.object(progress, "vu_line", return_value=self.VU if vu is ... else vu), \
                redirect_stdout(out):
            progress.main()
        return out.getvalue()

    def test_text_labels_source_coverage_and_drafts(self) -> None:
        self.assertEqual(
            self.run_progress(),
            f"{VERSION}: 2/4 functions in C (50.0%), 25/100 code bytes in C (25.0%)\n"
            f"{VERSION}: 1/2 remaining functions have a non-matching draft (50.0%), "
            "40/75 remaining code bytes (53.3%)\n"
            f"{VERSION}: VU microcode 50 instructions, 400 / 400 bytes from source\n",
        )

    def test_markdown_keeps_source_coverage_labels_and_values(self) -> None:
        self.assertEqual(
            self.run_progress("--markdown"),
            "| Version | Functions in C | Code bytes in C | Non-matching drafts | VU microcode |"
            "\n|---|---|---|---|---|\n"
            f"| `{VERSION}` | 2 / 4 (50.0%) | 25 / 100 (25.0%) | 1 / 2 (50.0%) "
            "| 50 instructions, 400 / 400 bytes from source |\n",
        )

    def test_without_the_retail_image_the_microcode_counters_say_so(self) -> None:
        self.assertIn(f"{VERSION}: VU microcode retail image missing", self.run_progress(vu=None))

    def test_a_finished_version_reports_no_remaining_share(self) -> None:
        self.assertIn("0/0 remaining functions have a non-matching draft (0.0%)",
                      self.run_progress(stats=(4, 4, 0, 100, 100, 0)))


if __name__ == "__main__":
    unittest.main()
