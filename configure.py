#!/usr/bin/env python3
"""Generate build.ninja and objdiff.json for the Nocturne decomp (see README.md).

    python configure.py [--no-split] [--force-split] [--clean]
"""

from __future__ import annotations

import argparse
import hashlib
import json
import os
import platform
import re
import shlex
import shutil
import stat
import subprocess
import sys
from pathlib import Path

from tools.ee_gcc_aslr import marker_is_valid
from tools.symbol_aliases import load_symbol_aliases, linker_aliases

ROOT = Path(__file__).resolve().parent


def load_versions() -> dict:
    """config/versions.json, or empty when it has not been written yet."""
    path = ROOT / "config" / "versions.json"
    return json.loads(path.read_text()) if path.exists() else {}


VERSIONS = load_versions()

TOOLS = Path("tools")
BIN = TOOLS / "bin"
EEGCC = TOOLS / "compilers" / "ee-gcc2.96"
CC1 = EEGCC / "lib" / "gcc-lib" / "ee" / "2.96-ee-001003-1" / "cc1"
EE_AS = EEGCC / "ee" / "bin" / "as"
AS = BIN / "mips-ps2-decompals-as"
LD = BIN / "mips-ps2-decompals-ld"
OBJCOPY = BIN / "mips-ps2-decompals-objcopy"
OBJDIFF = BIN / "objdiff-cli"

# What the ee-gcc 2.96 driver passes to cc1 (`ee-gcc -v`); cc1 preprocesses itself.
CC1_DEFINES = (
    "-D__GNUC__=2 -D__GNUC_MINOR__=96 -D__GNUC_PATCHLEVEL__=0 "
    "-Dmips -DMIPSEL -DR5900 -D_mips -D_MIPSEL -D_R5900 -D__ee__ "
    "-D__mips__ -D__MIPSEL__ -D__R5900__ -D__mips -D__MIPSEL -D__R5900 -D__OPTIMIZE__ "
    "-D__LANGUAGE_C -D_LANGUAGE_C -DLANGUAGE_C "
    "'-D__SIZE_TYPE__=unsigned int' '-D__PTRDIFF_TYPE__=int' -D__LONG_MAX__=9223372036854775807L "
    "-U__mips -D__mips=3 -D__mips64 -D__mips_eabi -D__mips_single_float"
)
# Retail game code: -O2, default small-data limit (-G8: .sdata/.sbss/.lit4 are $gp-relative).
CC1_FLAGS = "-quiet -O2"
INCLUDES = "-Iinclude -Isrc"
AS_FLAGS = "-EL -march=r5900 -mabi=eabi -G8 -Iinclude"
# Retail was assembled with -g: GNU as then leaves `.set reorder` code where cc1
# put it, so every delay slot cc1 did not fill keeps its `nop` (soft-double and
# libm call argument moves, mtc1/lwc1/cvt dependents, indexed la/memory macros).
EE_AS_FLAGS = "-EL -G8 -g -Iinclude"


def i386_prefix() -> str:
    """cc1/ee-as are 32-bit i386 binaries. Without a system /lib/ld-linux.so.2,
    point SMT3_I386_LIBDIR at a directory holding ld-linux.so.2 + libc.so.6."""
    return no_aslr_prefix() + i386_loader()


def no_aslr_prefix() -> str:
    """Fallback for an unpatched, manually installed ee-gcc 2.96.

    tools/download_tools.py patches the compiler's GGC arena, allowing normal
    ASLR.  Keep setarch support for an otherwise-compatible manual toolchain.
    """
    if marker_is_valid(ROOT / CC1):
        return ""
    if os.environ.get("SMT3_ALLOW_ASLR") or not shutil.which("setarch"):
        return ""
    return f"setarch {platform.machine()} -R "


def i386_loader() -> str:
    # The pinned glibc from tools/download_tools.py comes first: ee-gcc 2.96's
    # output depends on heap layout, so the system libc is only a fallback.
    libdir = os.environ.get("SMT3_I386_LIBDIR")
    if not libdir and (ROOT / "tools/glibc32/libc.so.6").exists():
        return "tools/glibc32/ld-linux.so.2 --library-path tools/glibc32 "  # ninja runs in ROOT
    if libdir:
        libdir = shlex.quote(str(Path(libdir).expanduser().resolve()))
        return f"{libdir}/ld-linux.so.2 --library-path {libdir} "
    if Path("/lib/ld-linux.so.2").exists():
        return ""
    sys.exit("32-bit loader missing: install 32-bit glibc (e.g. glibc.i686 / libc6:i386) "
             "or set SMT3_I386_LIBDIR=<dir with ld-linux.so.2 and libc.so.6>")


def unit_cflags(version: str) -> dict[str, str]:
    """Per-unit cc1 flags from config/<v>/cflags.txt (`<dir>/<unit>  <flags>  # evidence`).

    Some original files were built with different options; each entry records the
    evidence that proves it."""
    path = ROOT / "config" / version / "cflags.txt"
    out = {}
    if path.exists():
        for line in path.read_text().splitlines():
            line = line.split("#", 1)[0].strip()
            if line:
                unit, flags = line.split(None, 1)
                out[unit] = flags
    return out


def _regular_files(paths: list[Path]) -> list[Path]:
    """List existing regular files under paths without following symlinks."""
    files = []
    for path in paths:
        candidates = path.rglob("*") if path.is_dir() and not path.is_symlink() else (path,)
        for candidate in candidates:
            try:
                mode = candidate.lstat().st_mode
            except FileNotFoundError:
                continue
            if stat.S_ISREG(mode):
                files.append(candidate)
    return files


def snapshot_generated_files(paths: list[Path]) -> dict[Path, tuple[bytes, int, int]]:
    """Record content digests and nanosecond timestamps for generated files."""
    snapshot = {}
    for path in _regular_files(paths):
        try:
            metadata = path.stat(follow_symlinks=False)
            digest = hashlib.sha256(path.read_bytes()).digest()
        except FileNotFoundError:
            # A concurrently removed file is simply absent from the snapshot.
            continue
        snapshot[path] = (digest, metadata.st_atime_ns, metadata.st_mtime_ns)
    return snapshot


def restore_unchanged_timestamps(snapshot: dict[Path, tuple[bytes, int, int]]) -> None:
    """Restore timestamps only when the same regular file still has identical bytes."""
    for path, (digest, atime_ns, mtime_ns) in snapshot.items():
        try:
            if not stat.S_ISREG(path.lstat().st_mode):
                continue
            if hashlib.sha256(path.read_bytes()).digest() != digest:
                continue
            os.utime(path, ns=(atime_ns, mtime_ns), follow_symlinks=False)
        except FileNotFoundError:
            # Deleted outputs and outputs replaced during generation are untouched.
            continue


def generated_footprint(version: str) -> list[Path]:
    """Return splat and postprocessor outputs whose timestamps feed Ninja."""
    serial = VERSIONS[version]["serial"]
    config = ROOT / "config" / version
    return [
        ROOT / "asm" / version,
        ROOT / "assets" / version,
        *(ROOT / "src" / version).rglob("*.c"),
        ROOT / "build" / version / f"{serial}.ld",
        *config.glob("undefined_*_auto.txt"),
    ]


def run_splat(version: str, yaml: Path, force: bool) -> None:
    """Split when the config inputs changed since the last successful split."""
    inputs = [yaml, ROOT / "config" / version / "symbol_addrs.txt", ROOT / "config" / version / "reloc_addrs.txt"]
    digest = hashlib.sha1(b"".join(p.read_bytes() for p in inputs if p.exists())).hexdigest()
    stamp = ROOT / "build" / version / ".splat_stamp"
    if not force and stamp.exists() and stamp.read_text() == digest and (ROOT / "asm" / version).exists():
        return
    print(f"splat: {version}")
    snapshot = snapshot_generated_files(generated_footprint(version))
    # splat never deletes files: a unit or rodata split that moved leaves stale
    # asm behind (and include_rodata.py would include it twice).
    shutil.rmtree(ROOT / "asm" / version, ignore_errors=True)
    subprocess.run([sys.executable, "-m", "splat", "split", str(yaml)], cwd=ROOT, check=True)
    subprocess.run([sys.executable, "tools/resolve_jtbl_targets.py", f"asm/{version}"], cwd=ROOT, check=True)
    subprocess.run([sys.executable, "tools/include_rodata.py", version], cwd=ROOT, check=True)
    subprocess.run([sys.executable, "tools/include_sdata.py", version], cwd=ROOT, check=True)
    align_bss(ROOT / "build" / version / f"{VERSIONS[version]['serial']}.ld")
    provide_data_symbols(version)
    restore_unchanged_timestamps(snapshot)
    stamp.parent.mkdir(parents=True, exist_ok=True)
    stamp.write_text(digest)


def write_configure_build(n, configure_inputs: list[str], argv: list[str]) -> None:
    """Write the Ninja regeneration edge without persisting one-shot options."""
    generator_args = [arg for arg in argv if arg not in ("--force-split", "--no-split")]
    n.build(
        "build.ninja",
        "configure",
        implicit=configure_inputs,
        variables={"args": " ".join(generator_args)},
    )


def provide_data_symbols(version: str) -> None:
    """Give every D_XXXXXXXX the asm refers to its retail address as a fallback.

    Per-unit .lit4/.rodata is compiled by C objects as local labels, so a .data
    word pointing at such a constant has no global definition. PROVIDE only
    defines a symbol no object defines."""
    names = set()
    for path in (ROOT / "asm" / version).rglob("*.s"):
        names.update(re.findall(r"\bD_[0-9A-F]{8}\b", path.read_text()))
    path = ROOT / "config" / version / "undefined_syms_auto.txt"
    text = path.read_text() if path.exists() else ""
    path.write_text(text + "".join(f"PROVIDE({n} = 0x{n[2:]});\n" for n in sorted(names)))


def align_bss(ld_script: Path) -> None:
    """Sony's app.cmd starts .sbss and .bss on 128-byte boundaries; splat's
    script does not, so insert the alignment the retail layout depends on."""
    text = ld_script.read_text()
    for sym in ("main_SBSS_START", "main_BSS_START"):
        text = re.sub(rf"^(\s*)({sym} = \.;)", r"\1. = ALIGN(128);\n\1\2", text, count=1, flags=re.M)
    ld_script.write_text(text)


def provide_symbol_aliases(version: str) -> None:
    """Keep source-owned interior symbols relative to their real C owner."""
    aliases = load_symbol_aliases(ROOT, version)
    path = ROOT / "config" / version / "undefined_syms_auto.txt"
    if path.exists():
        text = path.read_text()
        updated = linker_aliases(text, aliases)
        if updated != text:
            path.write_text(updated)


LD_OBJECT = re.compile(r"^\s*(build/\S+\.o)\(")


def linker_objects(ld_script: Path) -> list[Path]:
    seen: dict[Path, None] = {}
    for line in ld_script.read_text().splitlines():
        m = LD_OBJECT.match(line)
        if m:
            seen.setdefault(Path(m.group(1)), None)
    return list(seen)


INCLUDE_DIRECTIVE = re.compile(
    r'\bINCLUDE_(ASM|RODATA|SDATA)\b'
    r'(?:\s|/\*.*?\*/|//[^\r\n]*(?:\r?\n|$))*'
    r'\(\s*[^,]+,\s*"([^"]+)"\s*,\s*(\w+)\s*\)',
    re.S,
)
INCLUDE_TOKEN = re.compile(r'\bINCLUDE_(ASM|RODATA|SDATA)\b')
C_NON_CODE = re.compile(
    r'//[^\r\n]*|/\*.*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'',
    re.S,
)


def splice_c_lines(text: str) -> tuple[str, list[int]]:
    """Apply C phase-2 line splicing and map output offsets to the source."""

    result = []
    source_offsets = []
    index = 0
    while index < len(text):
        if text[index] == "\\":
            if text.startswith("\r\n", index + 1):
                index += 3
                continue
            if index + 1 < len(text) and text[index + 1] in "\r\n":
                index += 2
                continue
        result.append(text[index])
        source_offsets.append(index)
        index += 1
    source_offsets.append(len(text))
    return "".join(result), source_offsets


def mask_c_non_code(text: str) -> str:
    """Blank comments and literals while preserving token offsets and lines."""

    return C_NON_CODE.sub(
        lambda match: "".join(
            character if character in "\r\n" else " "
            for character in match.group()
        ),
        text,
    )


def include_directives(
    text: str, *, context: str | None = None
) -> list[tuple[str, str, str]]:
    spliced, source_offsets = splice_c_lines(text)
    code = mask_c_non_code(spliced)
    matches = [
        match
        for match in INCLUDE_DIRECTIVE.finditer(spliced)
        if code.startswith("INCLUDE_", match.start())
    ]
    if context is not None:
        matched_starts = {match.start() for match in matches}
        unmatched = [
            match for match in INCLUDE_TOKEN.finditer(code)
            if match.start() not in matched_starts
        ]
        if unmatched:
            source_offset = source_offsets[unmatched[0].start()]
            line = text.count("\n", 0, source_offset) + 1
            raise SystemExit(
                f"{context}: unsupported INCLUDE_{unmatched[0].group(1)} "
                f"syntax at line {line}"
            )
    return [match.groups() for match in matches]


def source_for(obj: Path, version: str) -> tuple[str, Path]:
    """Map a linker-script object back to the input splat or the tree provides."""
    rel = obj.relative_to(Path("build") / version)
    stem = Path(str(rel)[: -len(".o")])
    for kind, suffix in (("c", ".c"), ("s", ".s")):
        candidate = Path(f"{stem}{suffix}")
        if (ROOT / candidate).exists():
            return kind, candidate
    if rel.parts[0] == "assets":
        return "bin", Path(f"{stem}.bin")
    raise SystemExit(f"{obj}: no source found (expected {stem}.c or {stem}.s); re-run splat")


def objdiff_progress_category(version: str, name: str) -> str:
    # splat exposes the VU1 microcode and its data as asm units, not EE code.
    if Path(name).name in ("vutext", "vudata"):
        return "vu1"
    # Whole functions Atlus wrote in assembly (crt0, VU0 loops) are `asm` segments
    # under hand/, not C targets; like the SDK they never enter the game-code totals.
    if name.startswith(f"asm/{version}/hand/"):
        return "handwritten"
    return "sdk" if "/sdk/" in name else "game"


def write_objdiff_reports(n, units: dict[str, list[dict]]) -> None:
    objdiff_objects = sorted({p for rows in units.values() for row in rows
                              for p in (row["target"], row["base"]) if p})
    n.rule("objdiff_report", f"{OBJDIFF} report generate -p $project -o $out",
           description="objdiff report $out")

    def report(out: str, project: str, objs: list[str]) -> None:
        project_config = "objdiff.json" if project == "." else f"{project}/objdiff.json"
        n.build(out, "objdiff_report", implicit=objs + [project_config],
                variables={"project": project})

    n.build("objdiff", "phony", objdiff_objects)
    report("report.json", ".", objdiff_objects)
    reports = []
    for version, rows in units.items():
        game_rows = [row for row in rows if objdiff_progress_category(version, row["name"]) == "game"]
        # Keep the full-binary audit separate from the primary game-code report.
        # objdiff computes every measure from each project's actual unit list.
        for selected, project, out in (
            (rows, f"build/{version}", f"build/{version}/report.all.json"),
            (game_rows, f"build/{version}/progress", f"build/{version}/report.json"),
        ):
            objs = sorted({p for row in selected for p in (row["target"], row["base"]) if p})
            report(out, project, objs)
            reports.append(out)
    n.build("report", "phony", ["report.json"] + reports)


def write_ninja(versions: list[str], args: argparse.Namespace) -> dict[str, list[dict]]:
    # Imported here so --help works before `pip install -r requirements.txt`.
    import ninja_syntax

    prefix = i386_prefix()
    n = ninja_syntax.Writer(open(ROOT / "build.ninja", "w"), width=120)
    n.comment("Generated by configure.py; do not edit.")
    n.variable("ninja_required_version", "1.10")
    n.newline()

    n.rule("as", f"{AS} {AS_FLAGS} -o $out $in", description="as $in")
    n.rule("bin", f"{OBJCOPY} -I binary -O elf32-littlemips -B mips:5900 $in $out", description="bin $in")
    n.rule("eeasm", f"{sys.executable} tools/eeas_compat.py $in $out", description="ee-as compat $in")
    n.rule(
        "cc",
        f"cpp -MM -MG -MF $out.d -MT $out -nostdinc {INCLUDES} $cdefs $in && "
        f"{prefix}{CC1} {CC1_DEFINES} {INCLUDES} $cdefs {CC1_FLAGS} $cflags $in -o $out.s && "
        f"{prefix}{EE_AS} {EE_AS_FLAGS} -o $out $out.s",
        description="cc $in",
        depfile="$out.d",
        deps="gcc",
    )
    n.rule(
        "ld",
        f"{LD} -EL -T $ldscript -T $undef_syms -T $undef_funcs -Map $map --no-check-sections -o $out",
        description="ld $out",
    )
    n.rule("objcopy", f"{OBJCOPY} -O binary $in $out", description="objcopy $out")
    n.rule("check", "sha1sum --quiet -c $in && touch $out", description="check $in")
    n.rule("configure", f"{sys.executable} configure.py $args", description="configure", generator=True)
    n.newline()

    units: dict[str, list[dict]] = {}
    defaults = []
    for version in versions:
        serial = VERSIONS[version]["serial"]
        yaml = Path("config") / version / f"{serial}.yaml"
        if not args.no_split:
            run_splat(version, ROOT / yaml, args.force_split)
        provide_symbol_aliases(version)
        ld_script = Path("build") / version / f"{serial}.ld"
        if not (ROOT / ld_script).exists():
            sys.exit(f"{ld_script} missing; run configure.py without --no-split")
        objects = linker_objects(ROOT / ld_script)
        units[version] = []
        n.comment(f"--- {version} ({serial}) ---")
        for obj in objects:
            kind, src = source_for(obj, version)
            if kind == "s":
                n.build(str(obj), "as", str(src), implicit=["include/macro.inc"])
                units[version].append({"name": str(src.with_suffix("")), "target": str(obj), "base": None})
            elif kind == "bin":
                n.build(str(obj), "bin", str(src))
            else:
                text = (ROOT / src).read_text(errors="replace")
                eeasm = []
                nonmatchings = Path("asm") / version / "nonmatchings"
                for _, folder, name in include_directives(text, context=str(src)):
                    asm = nonmatchings / folder / f"{name}.s"
                    out = Path("build") / "eeasm" / asm
                    n.build(str(out), "eeasm", str(asm), implicit=["tools/eeas_compat.py"])
                    eeasm.append(str(out))
                cdefs = f"'-DASM_ROOT=\"build/eeasm/{nonmatchings}/\"' -DVERSION_{version.upper()}"
                flags = unit_cflags(version).get(src.relative_to(Path("src") / version).with_suffix("").as_posix(), "")
                n.build(str(obj), "cc", str(src),
                        implicit=eeasm + ["include/macro.inc", f"config/{version}/cflags.txt"],
                        implicit_outputs=[str(obj) + ".s"],
                        variables={"cdefs": cdefs, "cflags": flags})
                # objdiff base: the same unit without its INCLUDE_ASM fallbacks, so only C counts.
                base = Path("build") / version / "base" / src.with_suffix(".o")
                n.build(str(base), "cc", str(src), implicit=[f"config/{version}/cflags.txt"],
                        implicit_outputs=[str(base) + ".s"],
                        variables={"cdefs": f"{cdefs} -DSKIP_ASM", "cflags": flags})
                # objdiff target: splat's full disassembly of this C unit, assembled as-is.
                full = Path("asm") / version / src.relative_to(Path("src") / version).with_suffix(".s")
                target = Path("build") / version / "target" / full.with_suffix(".o")
                if (ROOT / full).exists():
                    n.build(str(target), "as", str(full), implicit=["include/macro.inc"])
                units[version].append({"name": str(src.with_suffix("")), "target": str(target), "base": str(base)})
        elf = Path("build") / version / f"{serial}.elf"
        image = Path("build") / version / serial
        n.build(
            str(elf), "ld", [str(o) for o in objects],
            implicit=[str(ld_script), f"config/{version}/undefined_syms_auto.txt",
                      f"config/{version}/undefined_funcs_auto.txt"],
            implicit_outputs=[str(elf.with_suffix(".map"))],
            variables={
                "ldscript": str(ld_script),
                "undef_syms": f"config/{version}/undefined_syms_auto.txt",
                "undef_funcs": f"config/{version}/undefined_funcs_auto.txt",
                "map": str(elf.with_suffix(".map")),
            },
        )
        n.build(str(image), "objcopy", str(elf))
        stamp = Path("build") / version / f"{serial}.ok"
        n.build(str(stamp), "check", f"config/{version}/checksum.sha1", implicit=[str(image)])
        n.build(version, "phony", [str(stamp)])
        defaults.append(version)
        (ROOT / "config" / version / "checksum.sha1").write_text(f"{VERSIONS[version]['elf_sha1']}  {image}\n")
        n.newline()

    write_objdiff_reports(n, units)

    configure_inputs = ["configure.py", "tools/symbol_aliases.py", "config/versions.json"] + [
        f"config/{v}/{VERSIONS[v]['serial']}.yaml" for v in versions
    ] + [f"config/{v}/symbol_addrs.txt" for v in versions]
    for version in versions:
        alias_metadata = ROOT / "config" / version / "symbol_aliases.json"
        if alias_metadata.exists():
            configure_inputs.append(str(alias_metadata.relative_to(ROOT)))
    write_configure_build(n, configure_inputs, sys.argv[1:])
    n.default(defaults)
    n.close()
    return units


def write_objdiff(units: dict[str, list[dict]]) -> None:
    categories = [
        {"id": "game", "name": "Atlus game/engine"},
        {"id": "sdk", "name": "Sony SDK / C runtime"},
        {"id": "vu1", "name": "VU1 microcode (binary)"},
        {"id": "handwritten", "name": "Handwritten assembly (not C)"},
    ]
    config = {
        "$schema": "https://raw.githubusercontent.com/encounter/objdiff/main/config.schema.json",
        "custom_make": "ninja",
        "build_target": False,
        "build_base": True,
        "watch_patterns": ["*.c", "*.h", "*.s", "*.inc"],
        "units": [],
    }
    for version, rows in units.items():
        for row in rows:
            category = objdiff_progress_category(version, row["name"])
            unit = {
                "name": "/".join(Path(row["name"]).parts[1:]),  # drop asm/ or src/: "<version>/<unit>"
                "target_path": row["target"],
                "metadata": {"progress_categories": [version, category]},
            }
            if row["base"]:
                unit["base_path"] = row["base"]
            config["units"].append(unit)
    config["progress_categories"] = [{"id": v, "name": VERSIONS[v]["title"]} for v in units] + categories
    (ROOT / "objdiff.json").write_text(json.dumps(config, indent=2) + "\n")
    # Per-version configs for the per-version reports; objdiff resolves paths
    # relative to the config's directory.
    for version in units:
        sub = dict(config, units=[], progress_categories=categories)
        for unit in config["units"]:
            if unit["name"].split("/", 1)[0] != version:
                continue
            unit = dict(unit, name=unit["name"].split("/", 1)[1],
                        metadata={"progress_categories": [c for c in unit["metadata"]["progress_categories"]
                                                          if c != version]})
            for key in ("target_path", "base_path"):
                if key in unit:
                    unit[key] = str(Path("..", "..", unit[key]))
            sub["units"].append(unit)
        path = ROOT / "build" / version / "objdiff.json"
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(json.dumps(sub, indent=2) + "\n")
        # decomp.dev's headline uses the report's overall matching-code measure.
        # Give it a coherent game-only project, while keeping the full config
        # above available for SDK/VU diagnostics and report.all.json.
        primary = dict(sub, units=[], progress_categories=[c for c in categories if c["id"] == "game"])
        for unit in sub["units"]:
            if "game" not in unit["metadata"]["progress_categories"]:
                continue
            unit = dict(unit)
            for key in ("target_path", "base_path"):
                if key in unit:
                    unit[key] = str(Path("..", unit[key]))
            primary["units"].append(unit)
        path = ROOT / "build" / version / "progress" / "objdiff.json"
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(json.dumps(primary, indent=2) + "\n")


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("versions", nargs="*", help=f"any of {', '.join(VERSIONS) or 'config/versions.json'}")
    ap.add_argument("--no-split", action="store_true", help="do not run splat")
    ap.add_argument("--force-split", action="store_true", help="run splat even if its inputs are unchanged")
    ap.add_argument("--clean", action="store_true", help="remove generated asm/, assets/, build/ first")
    args = ap.parse_args()

    if not VERSIONS:
        ap.error("config/versions.json is missing or empty")
    versions = args.versions or [v for v in VERSIONS if (ROOT / "orig" / v / VERSIONS[v]["serial"]).exists()]
    for v in versions:
        if v not in VERSIONS:
            ap.error(f"unknown version {v}")
        if not (ROOT / "orig" / v / VERSIONS[v]["serial"]).exists():
            ap.error(f"{v}: orig/{v}/{VERSIONS[v]['serial']} missing; run tools/extract.py")
    if not versions:
        ap.error("no extracted versions; run tools/extract.py")
    for tool in (CC1, EE_AS, AS, LD, OBJCOPY):
        if not (ROOT / tool).exists():
            ap.error(f"{tool} missing; run tools/download_tools.py")
    if args.clean:
        for d in ("asm", "assets", "build"):
            shutil.rmtree(ROOT / d, ignore_errors=True)

    os.chdir(ROOT)
    units = write_ninja(versions, args)
    write_objdiff(units)
    print(f"configured {', '.join(versions)}; run ninja")


if __name__ == "__main__":
    main()
