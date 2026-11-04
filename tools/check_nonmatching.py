#!/usr/bin/env python3
"""Compile the NON_MATCHING drafts.

    python3 tools/check_nonmatching.py [src/smt3/<unit>.c ...]

A draft kept behind `#ifdef NON_MATCHING` is never compiled by the normal
build, so a struct rename or a changed signature elsewhere can break it
silently. This compiles every unit that has one with NON_MATCHING and
SKIP_ASM defined, which selects the drafts and drops the asm fallbacks.

A failure means the draft needs updating, not that the repo is broken: the
real build does not use it. Exit status is non-zero so CI still says so.
"""
import glob
import re
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
GUARD = "#ifdef NON_MATCHING"
# A draft is a guarded block whose #else holds the asm fallback; the
# INCLUDE_ASM names there name the functions the drafts are for. One block can
# carry several, so every name in the #else counts: reading only the first made
# four drafted functions in sdf/sdfVu1 look absent.
DIRECTIVE = re.compile(r'^\s*#\s*(ifdef|ifndef|if|elif|else|endif)\b(.*)$')
COMMENTS = re.compile(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|/\*.*?\*/|//[^\n]*', re.S)
ASM_NAME = re.compile(r'INCLUDE_ASM\([^,]+,\s*"[^"]+"\s*,\s*(\w+)\s*\)')


def draft_names(text: str) -> list[str]:
    """Every function drafted in this unit, across all guarded blocks."""
    # Keep directive nesting separate from draft selection: FIX_BUGS and other
    # conditionals inside a draft must not consume its outer #else/#endif.
    text = COMMENTS.sub(lambda m: re.sub(r"[^\n]", " ", m.group())
                        if m.group().startswith(("/*", "//")) else m.group(), text)
    stack = []
    fallback = []
    for line in text.splitlines(keepends=True):
        directive = DIRECTIVE.match(line)
        if directive:
            kind, argument = directive.groups()
            if kind in ("if", "ifdef", "ifndef"):
                stack.append([kind == "ifdef" and argument.strip() == "NON_MATCHING",
                              False])
            elif stack:
                if kind == "endif":
                    stack.pop()
                elif kind == "else":
                    stack[-1][1] = True
                elif kind == "elif":
                    stack[-1][1] = False
            # Prevent text on either side of a directive joining into a macro.
            fallback.append("\n")
        elif any(guard and in_else for guard, in_else in stack):
            fallback.append(line)
        else:
            fallback.append("\n")
    return ASM_NAME.findall("".join(fallback))


def units(args):
    if args:
        return [Path(a) for a in args]
    return [Path(p) for p in sorted(glob.glob(str(ROOT / "src/smt3/**/*.c"), recursive=True))
            if GUARD in (t := Path(p).read_text()) or "FIX_BUGS" in t]


def draft_functions(version: str = "smt3") -> dict[str, set[str]]:
    """Map each unit, relative to src/<version> and without .c, to its drafts."""
    src = ROOT / "src" / version
    drafts = {}
    for path in sorted(src.rglob("*.c")):
        names = set(draft_names(path.read_text()))
        if names:
            drafts[path.relative_to(src).with_suffix("").as_posix()] = names
    return drafts


def compile_unit(path: Path, fix_bugs: bool = False) -> str:
    """Return cc1's diagnostics, empty when the draft compiles.

    fix_bugs adds -DFIX_BUGS, which selects the corrections the matching build
    never compiles either.
    """
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
         *(["-DFIX_BUGS"] if fix_bugs else []),
         # Old GCC needs this spelling; -Werror=... is silently ignored.
         "-Werror-implicit-function-declaration",
         "-quiet", "-O2", str(path), "-o", "/dev/null"],
        capture_output=True, text=True, cwd=ROOT)
    # cc1 brackets its diagnostics with context lines that carry no line
    # number ("file: In function `x':", "file: At top level:"). Keeping those
    # made any unit that merely warns look like a failure.
    diagnostics = "\n".join(l for l in out.stderr.split("\n")
                            if re.search(r":\d+: ", l) and "warning:" not in l)
    if out.returncode and not diagnostics:
        detail = (out.stderr or out.stdout).strip()
        diagnostics = f"compiler exited with status {out.returncode}"
        if detail:
            diagnostics += "\n" + detail
    return diagnostics


def control() -> None:
    """A dropped brace inside a FIX_BUGS block must fail, a whole one must pass."""
    body = "int f(int a)\n{\n#ifdef FIX_BUGS\n    if (a) {\n        a++;\n%s#endif\n    return a;\n}\n"
    with tempfile.TemporaryDirectory() as tmp:
        for text, name, must_fail in ((body % "", "dropped.c", True),
                                      (body % "    }\n", "whole.c", False)):
            unit = Path(tmp) / name
            unit.write_text(text)
            failed = bool(compile_unit(unit, True))
            if failed != must_fail:
                sys.exit(f"control {name}: expected "
                         f"{'failure' if must_fail else 'success'} from cc1, "
                         f"so this check cannot be trusted")


if __name__ == "__main__":
    todo = units(sys.argv[1:])
    if not todo:
        print("no NON_MATCHING drafts")
        sys.exit(0)
    control()
    bad = 0
    for path in todo:
        text = path.read_text()
        names = draft_names(text)
        rel = path.relative_to(ROOT) if path.is_absolute() else path
        for fix_bugs in (False, True):
            if fix_bugs and "FIX_BUGS" not in text:
                continue
            label = "NON_MATCHING + FIX_BUGS" if fix_bugs else "NON_MATCHING"
            err = compile_unit(path, fix_bugs)
            if err:
                bad += 1
                print(f"{rel}: {len(names)} draft(s), DOES NOT COMPILE with {label}")
                print("\n".join("    " + l for l in err.split("\n")[:12]))
            else:
                print(f"{rel}: {len(names)} draft(s) compile with {label}")
    sys.exit(1 if bad else 0)
