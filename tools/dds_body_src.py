#!/usr/bin/env python3
"""Print a dds-decomp function definition by name, skipping its declarations.

    python3 tools/dds_body_src.py --dds-decomp ../dds-decomp <name> [<name>...]
"""

from __future__ import annotations

import argparse
import re
from pathlib import Path


def find(text, name):
    """The definition of `name` in `text`, or None if this file only declares it."""
    for m in re.finditer(r"(?<!\w)" + re.escape(name) + r"\s*\(", text):
        depth, i = 0, m.end() - 1
        while i < len(text):
            if text[i] == "(":
                depth += 1
            elif text[i] == ")":
                depth -= 1
                if depth == 0:
                    break
            i += 1
        j = i + 1
        while j < len(text) and text[j] in " \t\r\n":
            j += 1
        if j >= len(text) or text[j] != "{":
            continue
        start = text.rfind("\n\n", 0, m.start()) + 2
        depth, k = 0, j
        while k < len(text):
            if text[k] == "{":
                depth += 1
            elif text[k] == "}":
                depth -= 1
                if depth == 0:
                    return text[start:k + 1]
            k += 1
    return None


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
