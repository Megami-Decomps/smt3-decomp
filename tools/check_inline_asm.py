#!/usr/bin/env python3
"""Reject instruction-emitting inline asm written directly in a source file.

    python3 tools/check_inline_asm.py src/smt3/sdf/sdfTex.c [--all]

Emitted EE instructions belong in one named macro in include/ps2asm.h. An empty
constraint template and a declaration's asm label or register binding emit
nothing and pass. Only C units are checked, since headers are where the macros
live; config/inline_asm_allow.txt grandfathers the units that predate the rule.
"""
import argparse
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
ALLOW_FILE = ROOT / "config" / "inline_asm_allow.txt"
MACRO_HOME = Path("include/ps2asm.h")
ASM_KEYWORD = re.compile(r"\b(?:__asm__|__asm|asm)\b")
VOLATILE = re.compile(r"\s*(?:volatile|__volatile__)?\s*")


def load_allowlist() -> set[Path]:
    if not ALLOW_FILE.exists():
        return set()
    paths = set()
    for line in ALLOW_FILE.read_text().splitlines():
        line = line.split("#", 1)[0].strip()
        if line:
            paths.add(Path(line))
    return paths


def code_view(text: str) -> tuple[str, dict[int, str]]:
    """Blank comments and literal bodies; return the view and the literals by offset.

    Positions are preserved, so an offset in the view indexes the original text.
    """
    view = list(text)
    literals: dict[int, str] = {}
    i, n = 0, len(text)
    while i < n:
        c = text[i]
        two = text[i:i + 2]
        if two == "//":
            while i < n and text[i] != "\n":
                view[i] = " "
                i += 1
        elif two == "/*":
            end = text.find("*/", i + 2)
            end = n if end < 0 else end + 2
            for j in range(i, end):
                if text[j] != "\n":
                    view[j] = " "
            i = end
        elif c in "\"'":
            start = i
            i += 1
            while i < n and text[i] != c:
                i += 2 if text[i] == "\\" else 1
            i = min(i + 1, n)
            literals[start] = text[start + 1:i - 1]
            for j in range(start + 1, i - 1):
                if text[j] != "\n":
                    view[j] = " "
        else:
            i += 1
    return "".join(view), literals


def in_directive(view: str, pos: int) -> bool:
    """True if pos sits on a preprocessor directive, following line continuations."""
    line_start = view.rfind("\n", 0, pos) + 1
    while True:
        if view[line_start:pos].lstrip().startswith("#"):
            return True
        if line_start == 0:
            return False
        prev_end = line_start - 1
        if view[:prev_end].rstrip(" \t").endswith("\\"):
            pos = line_start
            line_start = view.rfind("\n", 0, prev_end) + 1
            continue
        return False


def statement_prefix(view: str, pos: int) -> str:
    """The code between the enclosing statement's start and pos.

    A preprocessor directive is a boundary too: an asm after `#ifdef
    NON_MATCHING` starts a statement, it does not continue a declaration.
    """
    start = max(view.rfind(ch, 0, pos) for ch in ";{}") + 1
    lines = view[start:pos].split("\n")
    keep = 0
    for i, line in enumerate(lines):
        if line.lstrip().startswith("#"):
            keep = i + 1
            while keep < len(lines) and lines[keep - 1].rstrip().endswith("\\"):
                keep += 1
    return "\n".join(lines[keep:]).strip()


def template(view: str, literals: dict[int, str], pos: int) -> str | None:
    """The asm template at pos: the literals before the first colon, concatenated.

    None when the operand list holds anything but string literals, which means a
    macro reaches the assembler and the template is not verifiably empty.
    """
    open_paren = VOLATILE.match(view, pos).end()
    if open_paren >= len(view) or view[open_paren] != "(":
        return None
    i = open_paren + 1
    parts = []
    while i < len(view):
        c = view[i]
        if c in " \t\r\n":
            i += 1
        elif c == "\\" and view[i + 1:i + 2] == "\n":
            i += 2
        elif c == '"':
            parts.append(literals.get(i, "x"))
            i = view.find('"', i + 1) + 1
        elif c in ":)":
            return "".join(parts)
        else:
            return None
    return None


def violations(path: Path) -> list[tuple[int, str]]:
    text = (ROOT / path).read_text(errors="replace")
    view, literals = code_view(text)
    found = []
    for match in ASM_KEYWORD.finditer(view):
        body = template(view, literals, match.end())
        if body == "":
            continue
        line = view.count("\n", 0, match.start()) + 1
        if in_directive(view, match.start()):
            found.append((line, "asm macro defined in the unit"))
        elif not statement_prefix(view, match.start()):
            found.append((line, "inline asm statement"))
    return found


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("sources", nargs="*", type=Path)
    parser.add_argument("--all", action="store_true", help="check every C unit under src/")
    args = parser.parse_args()

    sources = [p if not p.is_absolute() else p.relative_to(ROOT) for p in args.sources]
    if args.all:
        sources += [p.relative_to(ROOT) for p in sorted(ROOT.glob("src/**/*.c"))]
    allowed = load_allowlist()

    failed = False
    for path in dict.fromkeys(sources):
        if path in allowed:
            continue
        for line, what in violations(path):
            print(f"{path}:{line}: {what}; put the operation in {MACRO_HOME} "
                  f"(docs/CONTRIBUTING.md)", file=sys.stderr)
            failed = True
    sys.exit(1 if failed else 0)


if __name__ == "__main__":
    main()
