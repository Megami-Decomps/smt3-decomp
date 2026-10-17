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
            writer, [f"config/{VERSION}/{SERIAL}.yaml"], [VERSION, "--force-split", "--no-split"]
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
            ]
        }
        self.categories = ["game", "sdk", "vu1", "game", "handwritten"]
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
            [VERSION, "game", "sdk", "vu1", "handwritten"],
        )
        for unit, category in zip(self.combined["units"], self.categories):
            self.assertEqual(unit["metadata"]["progress_categories"], [VERSION, category])

    def test_per_version_config_rebases_paths_and_keeps_all_categories(self) -> None:
        self.assertEqual(
            [category["id"] for category in self.per_version["progress_categories"]],
            ["game", "sdk", "vu1", "handwritten"],
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

    def test_report_targets_use_matching_projects_and_object_dependencies(self) -> None:
        writer = Mock()
        configure.write_objdiff_reports(writer, self.units)
        builds = {call.args[0]: call for call in writer.build.call_args_list}
        all_objects = sorted({p for row in self.units[VERSION]
                              for p in (row["target"], row["base"]) if p})
        self.assertEqual(builds["objdiff"].args, ("objdiff", "phony", all_objects))
        self.assertEqual(builds["report.json"].kwargs, {
            "implicit": all_objects + ["objdiff.json"], "variables": {"project": "."},
        })
        expected_reports = {"report.json"}
        rows = self.units[VERSION]
        for selected, project, out in (
            (rows, f"build/{VERSION}", f"build/{VERSION}/report.all.json"),
            ([rows[index] for index in (0, 3)], f"build/{VERSION}/progress", f"build/{VERSION}/report.json"),
        ):
            expected_reports.add(out)
            objects = sorted({p for row in selected for p in (row["target"], row["base"]) if p})
            self.assertEqual(builds[out].args, (out, "objdiff_report"))
            self.assertEqual(builds[out].kwargs, {
                "implicit": objects + [f"{project}/objdiff.json"], "variables": {"project": project},
            })
        self.assertEqual(set(builds["report"].args[2]), expected_reports)
        writer.rule.assert_any_call(
            "objdiff_report", f"{configure.OBJDIFF} report generate -p $project -o $out",
            description="objdiff report $out"
        )


class ProgressLabelTests(unittest.TestCase):
    def test_text_labels_source_coverage(self) -> None:
        out = io.StringIO()
        with patch.object(sys, "argv", ["progress.py", VERSION]), \
                patch.object(progress, "version_stats", return_value=(4, 2, 100, 25)), \
                redirect_stdout(out):
            progress.main()
        self.assertEqual(
            out.getvalue(),
            f"{VERSION}: 2/4 functions in C (50.0%), 25/100 code bytes in C (25.0%)\n",
        )

    def test_markdown_keeps_source_coverage_labels_and_values(self) -> None:
        out = io.StringIO()
        with patch.object(sys, "argv", ["progress.py", VERSION, "--markdown"]), \
                patch.object(progress, "version_stats", return_value=(4, 2, 100, 25)), \
                redirect_stdout(out):
            progress.main()
        self.assertEqual(
            out.getvalue(),
            "| Version | Functions in C | Code bytes in C |\n|---|---|---|\n"
            f"| `{VERSION}` | 2 / 4 (50.0%) | 25 / 100 (25.0%) |\n",
        )


if __name__ == "__main__":
    unittest.main()
