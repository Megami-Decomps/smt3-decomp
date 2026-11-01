#!/usr/bin/env python3
"""Reject an asm keyword written directly in a C unit.

    python3 tools/check_inline_asm.py src/smt3/sdf/sdfTex.c [--all]

Every form has a macro home: emitted EE instructions in include/ps2asm.h, a
compiler constraint or a symbol name in a FAKEMATCH_ASM_* macro in
include/common.h. Only C units are checked, since headers are where the macros
live; config/inline_asm_allow.txt grandfathers the units that predate the rule.
"""
import argparse
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
ALLOW_FILE = ROOT / "config" / "inline_asm_allow.txt"
MACRO_HOME = Path("include/ps2asm.h")
CONSTRAINT_HOME = Path("include/common.h")
ASM_KEYWORD = re.compile(r"\b(?:__asm__|__asm|asm)\b")


def load_allowlist() -> set[Path]:
    if not ALLOW_FILE.exists():
        return set()
    paths = set()
    for line in ALLOW_FILE.read_text().splitlines():
        line = line.split("#", 1)[0].strip()
        if line:
            paths.add(Path(line))
    return paths


def code_view(text: str) -> str:
    """The text with comments and literal bodies blanked, positions preserved."""
    view = list(text)
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
            for j in range(start + 1, i - 1):
                if text[j] != "\n":
                    view[j] = " "
        else:
            i += 1
    return "".join(view)


def violations(path: Path) -> list[int]:
    """The lines of the unit that spell an asm keyword themselves."""
    view = code_view((ROOT / path).read_text(errors="replace"))
    return [view.count("\n", 0, m.start()) + 1 for m in ASM_KEYWORD.finditer(view)]


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
        for line in violations(path):
            print(f"{path}:{line}: a C unit does not spell asm itself; emitted "
                  f"instructions go in {MACRO_HOME}, a compiler constraint or a "
                  f"symbol name in a FAKEMATCH_ASM_* macro in {CONSTRAINT_HOME} "
                  f"(docs/CONTRIBUTING.md)", file=sys.stderr)
            failed = True
    sys.exit(1 if failed else 0)


if __name__ == "__main__":
    main()
