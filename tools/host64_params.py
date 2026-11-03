#!/usr/bin/env python3
"""Rewrite one parameter's or one return type's integer type, tree-wide.

The 32-bit integer types that hold a guest address are named ps2Addr_t and
ps2AddrS_t (include/common.h), so a host can widen them without editing the
decompilation. A function's parameter is declared in its definition and again
in every unit that calls it, so the edit has to reach all of them at once.

  param <function> <index> <type>    parameter <index>, counting from 0
  ret <function> <type>              the return type

A type of "auto" keeps each declaration's own signedness: u32 becomes
ps2Addr_t and s32 becomes ps2AddrS_t. The two differ in later arithmetic and
comparisons, so one function's parameter declared u32 in one unit and s32 in
another must stay that way in both.

Only a declaration can match: a parameter slot is rewritten solely when its
text is a bare integer type with an optional name, which a call argument never
is. Every rewrite is reported, so nothing lands unseen.
"""

import argparse
import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent
PARAM = re.compile(r"\A\s*(u32|s32|unsigned int|int)(\s+(?:const\s+)?[A-Za-z_][A-Za-z_0-9]*)?\s*\Z")


AUTO = {"u32": "ps2Addr_t", "unsigned int": "ps2Addr_t",
        "s32": "ps2AddrS_t", "int": "ps2AddrS_t"}


def pick(newtype, old):
    return AUTO[old] if newtype == "auto" else newtype


def units():
    for base in ("include", "src"):
        for pattern in ("*.c", "*.h"):
            yield from sorted((ROOT / base).rglob(pattern))


def split_args(text):
    parts, depth, start = [], 0, 0
    for i, c in enumerate(text):
        if c in "([":
            depth += 1
        elif c in ")]":
            depth -= 1
        elif c == "," and depth == 0:
            parts.append((start, i))
            start = i + 1
    parts.append((start, len(text)))
    return parts


def arg_list(text, open_paren):
    depth = 0
    for i in range(open_paren, len(text)):
        if text[i] == "(":
            depth += 1
        elif text[i] == ")":
            depth -= 1
            if depth == 0:
                return open_paren + 1, i
    return None


def rewrite_param(text, func, index, newtype):
    hits = []
    out = text
    pattern = re.compile(r"\b" + re.escape(func) + r"\s*\(")
    pos = 0
    while True:
        m = pattern.search(out, pos)
        if not m:
            break
        span = arg_list(out, m.end() - 1)
        pos = m.end()
        if not span:
            continue
        lo, hi = span
        args = out[lo:hi]
        parts = split_args(args)
        if index >= len(parts):
            continue
        a, b = parts[index]
        piece = args[a:b]
        pm = PARAM.match(piece)
        if not pm:
            continue
        new = piece[:pm.start(1)] + pick(newtype, pm.group(1)) + piece[pm.end(1):]
        out = out[:lo + a] + new + out[lo + b:]
        hits.append(piece.strip())
        pos = lo + a + len(new)
    return out, hits


def rewrite_ret(text, func, newtype):
    pattern = re.compile(
        r"(?m)^([ \t]*(?:extern[ \t]+|static[ \t]+)*)(u32|s32)([ \t]+\**[ \t]*)("
        + re.escape(func) + r")[ \t]*\(")
    hits = []

    def sub(m):
        hits.append(m.group(2))
        return m.group(1) + pick(newtype, m.group(2)) + m.group(3) + m.group(4) + "("

    return pattern.sub(sub, text), hits


def main():
    ap = argparse.ArgumentParser()
    sub = ap.add_subparsers(dest="mode", required=True)
    p = sub.add_parser("param")
    p.add_argument("func")
    p.add_argument("index", type=int)
    p.add_argument("type")
    r = sub.add_parser("ret")
    r.add_argument("func")
    r.add_argument("type")
    args = ap.parse_args()

    total = 0
    for path in units():
        text = path.read_text()
        if args.func not in text:
            continue
        if args.mode == "param":
            out, hits = rewrite_param(text, args.func, args.index, args.type)
        else:
            out, hits = rewrite_ret(text, args.func, args.type)
        if not hits:
            continue
        path.write_text(out)
        total += len(hits)
        print(f"{path.relative_to(ROOT)}: {len(hits)} ({', '.join(sorted(set(hits)))})")
    print(f"{total} declarations rewritten")
    return 0


if __name__ == "__main__":
    sys.exit(main())
