#!/usr/bin/env python3
"""Find and convert the pointer-width cast sites tools/host64_probe.py counts.

gcc locates them: the caret of -Werror=pointer-to-int-cast and
-Werror=int-to-pointer-cast sits on the offending cast's open paren, and gcc
issues it from the operand's type, which a text scan cannot see. 412 of the
narrowing sites are spelled (s32), so keying on the cast text misses them.

  list     one line per site: file:line:col kind
  convert  rewrite them: EE_ADDR / EE_ADDR_S narrowing, EE_ADDR_AS widening
  verify   un-expand the three macros in the working tree and in HEAD and
           compare, which proves the conversion moved no token but the names

Nothing here is part of the retail build.
"""

import argparse
import concurrent.futures
import json
import pathlib
import re
import subprocess
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent
HOST = ROOT / "tools" / "host64"

FLAGS = [
    "-m64", "-fsyntax-only", "-std=gnu99", "-fdiagnostics-format=json",
    "-include", str(HOST / "common.h"),
    "-include", str(HOST / "fpu.h"),
    "-include", str(HOST / "eeaddr.h"),
    "-I", str(ROOT / "include"), "-I", str(ROOT / "src"),
    "-DSKIP_ASM", "-DVERSION_SMT3", "-DNON_MATCHING", '-DASM_ROOT=""',
    "-Wno-all", "-Wno-extra",
    "-Werror=pointer-to-int-cast", "-Werror=int-to-pointer-cast",
]

NARROW = "cast from pointer to integer of different size"
WIDEN = "cast to pointer from integer of different size"


def units():
    return sorted(p for p in (ROOT / "src" / "smt3").rglob("*.c"))


def sites_in(path):
    """Every pointer-width cast gcc reports while compiling this unit."""
    out = subprocess.run(["gcc"] + FLAGS + [str(path)],
                         capture_output=True, text=True)
    try:
        diags = json.loads(out.stderr or "[]")
    except json.JSONDecodeError:
        # A unit that fails before codegen emits plain text alongside the JSON.
        start = out.stderr.find("[{")
        if start < 0:
            return []
        try:
            diags = json.loads(out.stderr[start:])
        except json.JSONDecodeError:
            return []
    found = []
    for d in diags:
        kind = {NARROW: "narrow", WIDEN: "widen"}.get(d.get("message"))
        if not kind or not d.get("locations"):
            continue
        caret = d["locations"][0].get("caret", {})
        if "file" not in caret:
            continue
        found.append((caret["file"], caret["line"], caret["column"], kind))
    return found


def collect(paths):
    seen = {}
    with concurrent.futures.ThreadPoolExecutor() as pool:
        for found in pool.map(sites_in, paths):
            for file, line, col, kind in found:
                seen[(file, line, col)] = kind
    return seen


# A cast binds to a unary-expression and no further. Wrapping more is a bug
# the checksum cannot catch: it turns a byte offset into pointer arithmetic.
PREFIX = re.compile(r"\s*(?:[-+!~*&]|\+\+|--)*\s*")
NAME = re.compile(r'[A-Za-z_][A-Za-z0-9_]*|0[xX][0-9a-fA-F]+|[0-9]+|"(?:[^"\\]|\\.)*"')
CAST = re.compile(r"\(\s*([A-Za-z_][A-Za-z0-9_ ]*?)\s*(\**)\s*\)")


def line_starts(text):
    starts = [0]
    for i, ch in enumerate(text):
        if ch == "\n":
            starts.append(i + 1)
    return starts


def balanced(text, i):
    """Index just past the group starting at text[i] in ( [ {."""
    pairs = {"(": ")", "[": "]", "{": "}"}
    depth = 0
    while i < len(text):
        if text[i] in pairs:
            depth += 1
        elif text[i] in ")]}":
            depth -= 1
            if depth == 0:
                return i + 1
        i += 1
    raise ValueError("unbalanced")


def operand_end(text, i):
    """Index just past the unary-expression starting at text[i]."""
    i = PREFIX.match(text, i).end()
    if i >= len(text):
        raise ValueError("no operand")
    if text[i] == "(":
        i = balanced(text, i)
    else:
        m = NAME.match(text, i)
        if not m:
            raise ValueError("no operand")
        i = m.end()
    while i < len(text):
        if text[i] in "([":
            i = balanced(text, i)
        elif text[i] == "." or text[i : i + 2] == "->":
            j = i + (1 if text[i] == "." else 2)
            m = NAME.match(text, j)
            if not m:
                break
            i = m.end()
        elif text[i : i + 2] in ("++", "--"):
            i += 2
        else:
            break
    return i


def convert_at(text, i, kind):
    """The file text with the cast at index i rewritten. None if skipped."""
    if i >= len(text) or text[i] != "(":
        return None, "caret is not on a cast paren"
    m = CAST.match(text, i)
    if not m:
        return None, "cast type not a plain name"
    base, stars = m.group(1).strip(), m.group(2)
    try:
        end = operand_end(text, m.end())
    except ValueError as exc:
        return None, str(exc)
    operand = text[m.end() : end].strip()
    if not operand:
        return None, "empty operand"
    if operand.startswith("&(("):
        return None, "field offset, correct on a host as written"
    if kind == "narrow":
        if stars:
            return None, "narrowing cast is not to a plain integer"
        macro = {"u32": "EE_ADDR", "s32": "EE_ADDR_S"}.get(base)
        if macro is None:
            return None, f"narrowing cast spelled ({base})"
        new = f"{macro}({operand})"
    else:
        target = f"{base} {stars}" if stars else base
        new = f"EE_ADDR_AS({target}, {operand})"
    return text[:i] + new + text[end:], None


def convert(paths, kinds, report):
    sites = collect(paths)
    wanted = {}
    for (file, line, col), kind in sites.items():
        if kind not in kinds:
            continue
        p = pathlib.Path(file)
        if not p.is_relative_to(ROOT / "src"):
            continue
        wanted.setdefault(p, []).append((line, col, kind))
    done = skipped = 0
    for path, hits in sorted(wanted.items()):
        text = path.read_text()
        starts = line_starts(text)
        # Last site first, so every earlier site keeps its offset.
        for line, col, kind in sorted(hits, reverse=True):
            new, why = convert_at(text, starts[line - 1] + col - 1, kind)
            if new is None:
                skipped += 1
                report.append(f"skip {path.relative_to(ROOT)}:{line}:{col} {why}")
                continue
            text = new
            done += 1
        path.write_text(text)
    return done, skipped


UNEXPAND = {"EE_ADDR": "(u32)", "EE_ADDR_S": "(s32)", "EE_ADDR_PTR": "(void *)(u32)"}


def unexpand(text):
    """Every EE_ADDR* call rewritten back to the cast it expands to."""
    out = []
    i = 0
    while i < len(text):
        m = re.compile(r"EE_ADDR(?:_S|_PTR|_AS)?\b").search(text, i)
        if not m:
            out.append(text[i:])
            break
        out.append(text[i : m.start()])
        j = PREFIX.match(text, m.end()).end()
        if j >= len(text) or text[j] != "(":
            out.append(text[m.start() : m.end()])
            i = m.end()
            continue
        end = balanced(text, j)
        inner = text[j + 1 : end - 1]
        if m.group(0) == "EE_ADDR_AS":
            depth = 0
            for k, ch in enumerate(inner):
                if ch in "([":
                    depth += 1
                elif ch in ")]":
                    depth -= 1
                elif ch == "," and depth == 0:
                    out.append(f"({inner[:k].strip()}){inner[k + 1:].strip()}")
                    break
            else:
                out.append(text[m.start() : end])
        else:
            out.append(UNEXPAND[m.group(0)] + inner)
        i = end
    return "".join(out)


def unexpand_all(text):
    """unexpand to a fixed point, because the macros nest."""
    for _ in range(8):
        once = unexpand(text)
        if once == text:
            return text
        text = once
    raise ValueError("unexpand did not settle")


def verify(paths):
    bad = 0
    for path in paths:
        rel = path.relative_to(ROOT)
        head = subprocess.run(["git", "-C", str(ROOT), "show", f"HEAD:{rel}"],
                              capture_output=True, text=True)
        if head.returncode:
            print(f"{rel}: not in HEAD")
            bad += 1
            continue
        a = "".join(unexpand_all(head.stdout).split())
        b = "".join(unexpand_all(path.read_text()).split())
        if a != b:
            n = next(k for k in range(min(len(a), len(b)) + 1)
                     if k == len(a) or k == len(b) or a[k] != b[k])
            print(f"{rel}: differs at {n}\n  HEAD {a[max(0, n - 60):n + 60]}"
                  f"\n  tree {b[max(0, n - 60):n + 60]}")
            bad += 1
    print(f"verify: {len(paths) - bad}/{len(paths)} files token-identical to HEAD")
    return 1 if bad else 0


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("mode", choices=("list", "convert", "verify"))
    ap.add_argument("--kind", choices=("narrow", "widen", "both"), default="both")
    ap.add_argument("paths", nargs="*")
    args = ap.parse_args()

    paths = []
    for arg in args.paths:
        q = pathlib.Path(arg).resolve()
        paths.extend(sorted(q.rglob("*.c")) if q.is_dir() else [q])
    paths = paths or units()
    kinds = ("narrow", "widen") if args.kind == "both" else (args.kind,)

    if args.mode == "verify":
        return verify(paths)

    if args.mode == "list":
        sites = collect(paths)
        for (file, line, col), kind in sorted(sites.items()):
            if kind in kinds:
                print(f"{pathlib.Path(file).relative_to(ROOT)}:{line}:{col} {kind}")
        print(f"{sum(1 for k in sites.values() if k in kinds)} sites, "
              f"{len({f for (f, _, _), k in sites.items() if k in kinds})} files",
              file=sys.stderr)
        return 0

    report = []
    done, skipped = convert(paths, kinds, report)
    for line in report:
        print(line)
    print(f"converted {done}, skipped {skipped}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
