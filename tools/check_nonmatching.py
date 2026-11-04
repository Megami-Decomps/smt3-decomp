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


def compile_unit(path: Path, fix_bugs: bool = False, asm_out: str = "/dev/null") -> str:
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
         "-quiet", "-O2", str(path), "-o", asm_out],
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


SKIP_ASM_WORDS = re.compile(r"^\s*\.(?:file|section|align|ent|end|frame|mask|fmask|set|size|type|ident)\b")


def symbol_extents() -> list[tuple[int, int]]:
    """(address, size) for every sized symbol in symbol_addrs.txt."""
    extents = []
    for line in (ROOT / "config/smt3/symbol_addrs.txt").read_text().splitlines():
        m = re.match(r"\w+\s*=\s*0x([0-9A-Fa-f]+);.*\bsize:0x([0-9A-Fa-f]+)", line)
        if m:
            extents.append((int(m.group(1), 16), int(m.group(2), 16)))
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


if __name__ == "__main__":
    todo = units(sys.argv[1:])
    if not todo:
        print("no NON_MATCHING drafts")
        sys.exit(0)
    control()
    known = symbol_extents()
    bad = 0
    for path in todo:
        text = path.read_text()
        names = draft_names(text)
        rel = path.relative_to(ROOT) if path.is_absolute() else path
        for fix_bugs in (False, True):
            if fix_bugs and "FIX_BUGS" not in text:
                continue
            label = "NON_MATCHING + FIX_BUGS" if fix_bugs else "NON_MATCHING"
            asm = tempfile.NamedTemporaryFile(suffix=".s", delete=False)
            asm.close()
            err = compile_unit(path, fix_bugs, asm.name)
            missing = [] if err else undefined_data(Path(asm.name).read_text(), known, text)
            Path(asm.name).unlink()
            if missing:
                bad += 1
                print(f"{rel}: undefined with {label}: {' '.join(missing)}")
            elif err:
                bad += 1
                print(f"{rel}: {len(names)} draft(s), DOES NOT COMPILE with {label}")
                print("\n".join("    " + l for l in err.split("\n")[:12]))
            else:
                print(f"{rel}: {len(names)} draft(s) compile with {label}")
    sys.exit(1 if bad else 0)
