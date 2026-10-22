#!/usr/bin/env python3
"""Rename function parameters to the names the IDB prototype carries.

The IDB's parameter names came with the HD symbol recovery, so they are the
original ones. For every C definition whose arity equals the IDB prototype's,
each differing parameter is renamed throughout that one function, all at once.
A function is skipped when a new name already occurs in it as something other
than a member, or when the IDB name is a dummy. Renaming changes no codegen, so
the image must still match afterwards."""

import argparse
import keyword
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import ida_protos  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]

SIG = re.compile(r"^([A-Za-z_][\w \t\*]*?)\b(\w+)\s*\(([^;{}]*)\)\s*(?:\{|;)", re.M)
DUMMY = re.compile(r"^(?:a\d+|arg\d*|argc|argv|this|__\w*)$")
C_KEYWORDS = {
    "auto", "break", "case", "char", "const", "continue", "default", "do", "double",
    "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register",
    "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef",
    "union", "unsigned", "void", "volatile", "while", "inline", "asm",
}
QUALIFIERS = {"const", "volatile", "struct", "union", "enum", "unsigned", "signed", "register"}
STRINGS_COMMENTS = re.compile(r'/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'', re.S)


def param_names(params):
    """Names in a parameter list, None for an unnamed slot; [] for `void`/empty."""
    params = params.strip()
    if params in ("", "void"):
        return []
    out = []
    for part in params.split(","):
        part = re.sub(r"\[[^\]]*\]", " ", part).strip()
        if "(" in part:          # a function-pointer parameter: name is inside
            m = re.search(r"\(\s*\*\s*(\w+)\s*\)", part)
            out.append(m.group(1) if m else None)
            continue
        words = [w for w in re.findall(r"[A-Za-z_]\w*", part) if w not in QUALIFIERS]
        if not words or "$" in part:
            out.append(None)     # IDA's `_$A1` stands for a slot it could not name
        elif len(words) == 1:
            out.append(None)     # a bare type, `u8 *` or `dds3Basic_t *`
        elif words[-1] in C_KEYWORDS or words[-1] in ("__fastcall", "__cdecl"):
            out.append(None)
        else:
            out.append(words[-1])
    return out


def rename_identifiers(text, mapping):
    """Replace whole identifiers per mapping, outside strings, comments and member
    accesses, all substitutions at once."""
    pat = re.compile(r"(->|\.)?(\s*)\b(" + "|".join(map(re.escape, mapping)) + r")\b")

    def swap(mm):
        return mm.group(0) if mm.group(1) else mm.group(2) + mapping[mm.group(3)]

    out, pos = [], 0
    for m in STRINGS_COMMENTS.finditer(text):
        out.append(pat.sub(swap, text[pos:m.start()]))
        out.append(m.group(0))
        pos = m.end()
    out.append(pat.sub(swap, text[pos:]))
    return "".join(out)


def identifiers(text):
    """Identifiers in code positions (not member accesses, strings or comments)."""
    code = STRINGS_COMMENTS.sub(" ", text)
    return set(m.group(2) for m in re.finditer(r"(->|\.)?\s*\b([A-Za-z_]\w*)\b", code)
               if not m.group(1))


def function_items(text):
    """(name, start, end, params, is_definition) for every definition and declaration."""
    for m in SIG.finditer(text):
        is_def = m.group(0).rstrip().endswith("{")
        if is_def:
            i, depth = m.end(), 1
            while depth and i < len(text):
                depth += (text[i] == "{") - (text[i] == "}")
                i += 1
            yield m.group(2), m.start(), i, m.group(3), True
        else:
            yield m.group(2), m.start(), m.end(), m.group(3), False


def plan(path, protos, stats, skips):
    text = path.read_text()
    edits = []
    for name, start, end, params, is_def in function_items(text):
        if name not in protos:
            continue
        proto = protos[name][1]
        m = re.match(r".*?\b" + re.escape(name) + r"\s*\((.*)\)\s*$", proto)
        if not m:
            continue
        ours, theirs = param_names(params), param_names(m.group(1))
        if len(ours) != len(theirs) or not ours:
            if is_def and ours and len(ours) != len(theirs):
                stats["arity"] += 1
            continue
        mapping = {}
        for o, t in zip(ours, theirs):
            if o and t and o != t and not DUMMY.match(t) and t not in C_KEYWORDS:
                mapping[o] = t
        if not mapping:
            continue
        body = text[start:end]
        used = identifiers(body) - set(mapping)
        clash = set(mapping.values()) & used
        if clash:
            if is_def:
                skips.append(f"{path}:{name}: {sorted(clash)} already used")
            continue
        edits.append((start, end, rename_identifiers(body, mapping), is_def, name, mapping))
    return text, edits


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--root", type=Path, default=ROOT)
    ap.add_argument("--apply", action="store_true")
    ap.add_argument("--only", help="regex on the file path")
    args = ap.parse_args()
    protos = ida_protos.load(ROOT / "config" / "smt3" / "ida_protos.txt")
    stats = {"functions": 0, "params": 0, "decls": 0, "arity": 0}
    skips = []
    files = sorted(list((args.root / "src").rglob("*.c")) + list((args.root / "include").rglob("*.h")))
    for path in files:
        if args.only and not re.search(args.only, path.as_posix()):
            continue
        text, edits = plan(path, protos, stats, skips)
        if not edits:
            continue
        for _, _, _, is_def, name, mapping in edits:
            if is_def:
                stats["functions"] += 1
                stats["params"] += len(mapping)
                print(f"{path.relative_to(args.root)}:{name}: " +
                      ", ".join(f"{o}->{t}" for o, t in mapping.items()))
            else:
                stats["decls"] += 1
        if args.apply:
            for start, end, new, *_ in sorted(edits, reverse=True):
                text = text[:start] + new + text[end:]
            path.write_text(text)
    for s in skips:
        print("skip", s)
    print(stats, f"{len(skips)} skipped")


if __name__ == "__main__":
    main()
