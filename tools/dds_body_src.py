#!/usr/bin/env python3
"""Print a dds-decomp function definition by name, skipping its declarations.

    python3 tools/dds_body_src.py --dds-decomp ../dds-decomp <name> [<name>...]
"""

from __future__ import annotations

import argparse
import re
from pathlib import Path


def code_view(text):
    # Preserve offsets while hiding comments and literals, so call-shaped text
    # in either cannot look like a function definition or affect brace pairing.
    return re.sub(
        r'/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'',
        lambda match: re.sub(r"[^\n]", " ", match.group()),
        text,
        flags=re.S,
    )


def definitions(text, code=None):
    """Return function definitions in one source file, excluding declarations."""
    if code is None:
        code = code_view(text)
    found = {}
    candidate = re.compile(r"^[\w \t*\n]*?(\w+)[ \t]*(?:\n[ \t]*)?\(", re.M)
    for m in candidate.finditer(code):
        name = m.group(1)
        if not code[m.start():m.start(1)].strip():
            continue
        depth, i = 0, m.end() - 1
        while i < len(code):
            if code[i] == "(":
                depth += 1
            elif code[i] == ")":
                depth -= 1
                if depth == 0:
                    break
            i += 1
        j = i + 1
        while j < len(code) and code[j] in " \t\r\n":
            j += 1
        if j >= len(code) or code[j] != "{":
            continue
        start = text.rfind("\n\n", 0, m.start()) + 2
        depth, k = 0, j
        while k < len(code):
            if code[k] == "{":
                depth += 1
            elif code[k] == "}":
                depth -= 1
                if depth == 0:
                    found.setdefault(name, text[start:k + 1])
                    break
            k += 1
    return found


def find(text, name):
    """The definition of `name` in `text`, or None if this file only declares it."""
    return definitions(text).get(name)


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--dds-decomp", default="../dds-decomp", type=Path)
    ap.add_argument("names", nargs="+")
    args = ap.parse_args()

    texts = {p: p.read_text(errors="replace")
             for p in sorted((args.dds_decomp / "src").rglob("*.c"))}
    for name in args.names:
        for path, text in texts.items():
            body = find(text, name)
            if body:
                print(f"/* ==== {name}  {path} ==== */\n{body}\n")
                break
        else:
            print(f"/* ==== {name}: no definition found ==== */\n")


if __name__ == "__main__":
    main()
