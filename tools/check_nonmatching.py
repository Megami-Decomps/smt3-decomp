#!/usr/bin/env python3
"""Compile the NON_MATCHING drafts so they cannot rot unnoticed.

    python3 tools/check_nonmatching.py [src/smt3/<unit>.c ...]

A draft kept behind `#ifdef NON_MATCHING` is never compiled by the normal
build, so a struct rename or a changed signature elsewhere can break it
silently. This compiles every unit that has one with NON_MATCHING and
SKIP_ASM defined, which selects the drafts and drops the asm fallbacks.

A failure means the draft needs updating, not that the repo is broken: the
real build does not use it. Exit status is non-zero so CI still says so.
"""
import glob
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
GUARD = "#ifdef NON_MATCHING"


def units(args):
    if args:
        return [Path(a) for a in args]
    return [Path(p) for p in sorted(glob.glob(str(ROOT / "src/smt3/**/*.c"), recursive=True))
            if GUARD in Path(p).read_text()]


def compile_unit(path: Path) -> str:
    """Return cc1's diagnostics, empty when the draft compiles."""
    cc1 = ROOT / "tools/compilers/ee-gcc2.96/lib/gcc-lib/ee/2.96-ee-001003-1/cc1"
    out = subprocess.run(
        [str(ROOT / "tools/glibc32/ld-linux.so.2"), "--library-path",
         str(ROOT / "tools/glibc32"), str(cc1),
         "-D__GNUC__=2", "-D__GNUC_MINOR__=96", "-Dmips", "-DMIPSEL", "-DR5900",
         "-D__ee__", "-D__mips__", "-D__MIPSEL__", "-D__R5900__",
         "-D__SIZE_TYPE__=unsigned int", "-D__PTRDIFF_TYPE__=int",
         "-D__LONG_MAX__=9223372036854775807L",
         "-U__mips", "-D__mips=3", "-D__mips64", "-D__mips_eabi",
         "-D__mips_single_float",
         "-I" + str(ROOT / "include"), "-I" + str(ROOT / "src"),
         "-DVERSION_SMT3", "-DSKIP_ASM", "-DNON_MATCHING",
         "-quiet", "-O2", str(path), "-o", "/dev/null"],
        capture_output=True, text=True, cwd=ROOT)
    return "\n".join(l for l in out.stderr.split("\n")
                     if l.strip() and "warning:" not in l)


if __name__ == "__main__":
    todo = units(sys.argv[1:])
    if not todo:
        print("no NON_MATCHING drafts")
        sys.exit(0)
    bad = 0
    for path in todo:
        n = path.read_text().count(GUARD)
        err = compile_unit(path)
        rel = path.relative_to(ROOT) if path.is_absolute() else path
        if err:
            bad += 1
            print(f"{rel}: {n} draft(s), DOES NOT COMPILE")
            print("\n".join("    " + l for l in err.split("\n")[:12]))
        else:
            print(f"{rel}: {n} draft(s) compile")
    sys.exit(1 if bad else 0)
