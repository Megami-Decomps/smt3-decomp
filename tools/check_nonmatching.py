#!/usr/bin/env python3
"""Compile the NON_MATCHING drafts.

    python3 tools/check_nonmatching.py [--jobs 1..8] [src/smt3/<unit>.c ...]

Optional bounded parallelism runs independent units with isolated temporary
assembly files. Default 1 preserves resource use; --jobs 4 is useful on machines
with spare CPU/memory. Controls always finish before workers start; reports
retain input order regardless of completion order. No matching build may
regenerate shared inputs while these compiler processes are active.

A draft kept behind `#ifdef NON_MATCHING` is never compiled by the normal
build, so a struct rename or a changed signature elsewhere can break it
silently. This compiles every unit that has one with NON_MATCHING and
SKIP_ASM defined, which selects the drafts and drops the asm fallbacks.
SMT3_I386_LIBDIR overrides the pinned loader directory, as in configure.py.

A failure means the draft needs updating, not that the repo is broken: the
real build does not use it. Exit status is non-zero so CI still says so.
"""
import argparse
from concurrent.futures import ThreadPoolExecutor
import glob
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
# A draft is a guarded block whose #else holds the asm fallback; the
# INCLUDE_ASM names there name the functions the drafts are for. One block can
# carry several, so every name in the #else counts: reading only the first made
# four drafted functions in sdf/sdfVu1 look absent.
DIRECTIVE = re.compile(r'^\s*#\s*(ifdef|ifndef|if|elif|else|endif)\b(.*)$')
COMMENTS = re.compile(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|/\*.*?\*/|//[^\n]*', re.S)
ASM_NAME = re.compile(r'INCLUDE_ASM\([^,]+,\s*"[^"]+"\s*,\s*(\w+)\s*\)')


def draft_names(text: str) -> list[str]:
    """Every function drafted in this unit, across all guarded blocks."""
    # Phase-2 line splicing precedes token and comment recognition. Match the
    # compilation-mode scanner so a continued guard, fallback name, or comment
    # cannot silently change the progress inventory.
    text = re.sub(r"\\\r?\n", "", text)
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


def conditional_names(text: str) -> set[str]:
    """Conservative mode references outside comments and string literals.

    C permits spaces between # and ifdef, #if expressions, and macro aliases
    such as #define BUG FIX_BUGS followed by #if BUG. Scan all identifier tokens
    rather than trying to evaluate the preprocessor: extra checks are safe,
    but silently dropping a mode can hide invalid drafts. Splice logical lines
    before stripping comments, as the preprocessor does.
    """
    text = re.sub(r"\\\r?\n", "", text)
    text = COMMENTS.sub(lambda m: re.sub(r"[^\n]", " ", m.group()), text)
    return set(re.findall(r"\b[A-Za-z_]\w*\b", text))


def units(args):
    if args:
        return [Path(a) for a in args]
    return [Path(p) for p in sorted(glob.glob(str(ROOT / "src/smt3/**/*.c"), recursive=True))
            if conditional_names(Path(p).read_text()) & {"NON_MATCHING", "FIX_BUGS"}]


def draft_functions(version: str = "smt3") -> dict[str, set[str]]:
    """Map each unit, relative to src/<version> and without .c, to its drafts."""
    src = ROOT / "src" / version
    drafts = {}
    for path in sorted(src.rglob("*.c")):
        names = set(draft_names(path.read_text()))
        if names:
            drafts[path.relative_to(src).with_suffix("").as_posix()] = names
    return drafts


def compile_unit(path: Path, fix_bugs: bool = False, asm_out: str = "/dev/null") -> str:
    """Return cc1's diagnostics, empty when the draft compiles.

    fix_bugs adds -DFIX_BUGS, which selects the corrections the matching build
    never compiles either.
    """
    cc1 = ROOT / "tools/compilers/ee-gcc2.96/lib/gcc-lib/ee/2.96-ee-001003-1/cc1"
    libdir = Path(os.environ.get("SMT3_I386_LIBDIR") or ROOT / "tools/glibc32")
    try:
        out = subprocess.run(
            [str(libdir / "ld-linux.so.2"), "--library-path",
             str(libdir), str(cc1),
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
             "-quiet", "-O2", str(path), "-o", asm_out],
            capture_output=True, text=True, cwd=ROOT)
    except OSError as error:
        return f"could not start compiler: {error}"
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


SKIP_ASM_WORDS = re.compile(r"^\s*\.(?:file|section|align|ent|end|frame|mask|fmask|set|size|type|ident)\b")


def symbol_extents() -> list[tuple[int, int]]:
    """Extents from effective first definitions, including unsized shadows.

    Hand-curated rows precede generated rows. A shadowed generated address must
    not become a fictitious symbol start that exempts an unresolved D_ operand.
    Keep metadata from the same definition, as check_unit.symbol_sizes does.
    """
    extents = []
    seen = set()
    for line in (ROOT / "config/smt3/symbol_addrs.txt").read_text().splitlines():
        row = re.match(r"^\s*(\S+)\s*=\s*0x([0-9A-Fa-f]+)\s*;", line)
        if not row or row.group(1) in seen:
            continue
        seen.add(row.group(1))
        size = re.search(r"\bsize:([^\s/]+)", line)
        if size:
            extents.append((int(row.group(2), 16), int(size.group(1), 0)))
    if not extents:
        sys.exit("config/smt3/symbol_addrs.txt has no sized symbols, so extents cannot be read")
    return extents


ASM_STRING = re.compile(r'"(?:\\.|[^"\\])*"')
DATA_NAME = re.compile(r"\bD_[0-9A-F]{8}\b")
ASM_DEFINITION = re.compile(
    r"^\s*(?:([.$A-Za-z_][.$A-Za-z_0-9]*)\s*(?::|=)|"
    r"\.(?:comm|lcomm|equ|equiv|eqv|set)\s+([.$A-Za-z_][.$A-Za-z_0-9]*)\s*,)")
ASM_DECLARATION = re.compile(r"^\s*\.(?:globl|global|weak|local|extern)\b")


def undefined_data(asm: str, extents: list[tuple[int, int]], source: str = "") -> list[str]:
    """Unresolved D_ operands covered by a sized symbol rather than starting one.

    Exempt only definitions in the active compiler output, including SYMBOL_AT
    and SYMBOL_AT_REGION assignments. Source comments and disabled branches
    cannot prove a definition. The source argument is retained for callers.
    """
    used, defined = set(), set()
    for line in asm.splitlines():
        # A symbol-looking string or comment is data, not a relocation.
        line = ASM_STRING.sub('""', line).split("#", 1)[0]
        definition = ASM_DEFINITION.match(line)
        if definition:
            defined.add(next(n for n in definition.groups() if n))
            # Include assignment RHS operands, even for a .set directive.
            line = line[definition.end():]
        elif SKIP_ASM_WORDS.match(line) or ASM_DECLARATION.match(line):
            continue
        used.update(DATA_NAME.findall(line))
    starts = {a for a, _ in extents}
    return sorted(n for n in used - defined
                  if (a := int(n[2:], 16)) not in starts
                  and any(s < a < s + z for s, z in extents))


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
        unit = Path(tmp) / "dropped_symbol.c"
        unit.write_text("extern int D_0FFFFFF0[];\nint f(void)\n{\n    return D_0FFFFFF0[0];\n}\n")
        asm = Path(tmp) / "dropped_symbol.s"
        if compile_unit(unit, False, str(asm)) or \
                undefined_data(asm.read_text(), [(0x0FFFFFE0, 0x40)]) != ["D_0FFFFFF0"]:
            sys.exit("control dropped_symbol.c: an undefined D_ name was not "
                     "reported, so the symbol check cannot be trusted")


def check_draft_unit(path, known):
    """Return failures and report lines for one unit without shared output files."""
    text = path.read_text()
    names = draft_names(text)
    rel = path.relative_to(ROOT) if path.is_absolute() else path
    bad, lines = 0, []
    for fix_bugs in (False, True):
        # Headers may carry corrected branches even when this unit never
        # spells FIX_BUGS. Check both modes for every selected draft unit.
        label = "NON_MATCHING + FIX_BUGS" if fix_bugs else "NON_MATCHING"
        with tempfile.TemporaryDirectory(prefix="smt3-draft-") as tmp:
            asm = Path(tmp) / "unit.s"
            err = compile_unit(path, fix_bugs, str(asm))
            missing = [] if err else undefined_data(asm.read_text(), known, text)
        if missing:
            bad += 1
            lines.append(f"{rel}: undefined with {label}: {' '.join(missing)}")
        elif err:
            bad += 1
            lines.append(f"{rel}: {len(names)} draft(s), DOES NOT COMPILE with {label}")
            lines.append("\n".join("    " + l for l in err.split("\n")[:12]))
        else:
            lines.append(f"{rel}: {len(names)} draft(s) compile with {label}")
    return bad, lines


def worker_count(value):
    try:
        count = int(value)
    except ValueError:
        raise argparse.ArgumentTypeError("jobs must be an integer from 1 to 8")
    if not 1 <= count <= 8:
        raise argparse.ArgumentTypeError("jobs must be an integer from 1 to 8")
    return count


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--jobs", type=worker_count, default=1,
                        help="maximum concurrent unit compiles (1..8, default 1)")
    parser.add_argument("paths", nargs="*")
    args = parser.parse_args(argv)
    todo = units(args.paths)
    if not todo:
        print("no NON_MATCHING drafts")
        return 0
    control()
    known = symbol_extents()
    bad = 0
    def check(path):
        return check_draft_unit(path, known)
    # map yields input order, so slow units cannot reorder diagnostics. Keep
    # the default serial path identical in resource use and exception behavior.
    if args.jobs == 1:
        results = map(check, todo)
        for failures, lines in results:
            bad += failures
            for line in lines:
                print(line)
    else:
        with ThreadPoolExecutor(max_workers=args.jobs) as pool:
            for failures, lines in pool.map(check, todo):
                bad += failures
                for line in lines:
                    print(line)
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
