#!/usr/bin/env python3
"""Resolve each truncated-address site to the declaration that truncates it.

tools/host64_probe.py trunc counts the sites: an address of host pointer width
stored into a 32-bit declaration. The caret gcc reports is the source
expression, not the destination, so this tool walks backwards from it to the
destination context and reports the declaration to widen:

  call:<function>:<index>   an argument passed to a 32-bit parameter
  return:<function>         a value returned through a 32-bit return type
  assign:<lvalue>           a value stored into a 32-bit object

Shapes it cannot settle are reported as unresolved rather than guessed.
Nothing here is part of the retail build.
"""

import argparse
import collections
import concurrent.futures
import json
import pathlib
import re
import subprocess
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent
HOST = ROOT / "tools" / "host64"

BASE = [
    "-m64", "-fsyntax-only", "-std=gnu99",
    "-include", str(HOST / "common.h"),
    "-include", str(HOST / "fpu.h"),
    "-include", str(HOST / "eeaddr.h"),
    "-I", str(ROOT / "include"), "-I", str(ROOT / "src"),
    "-DSKIP_ASM", "-DVERSION_SMT3", "-DNON_MATCHING", '-DASM_ROOT=""',
    "-Wno-all", "-Wno-extra", "-Werror=conversion", "-Wno-error=sign-conversion",
    "-fdiagnostics-format=json",
]
# The source type of the conversion, as gcc prints ee_addr_t's underlying
# type. Anchored on the source so a u64 or ps2Size_t destination cannot be
# counted here; see the traps in docs/host64.md.
MESSAGE = "conversion from ‘long long unsigned int’ to"

IDENT = re.compile(r"[A-Za-z_][A-Za-z_0-9]*\Z")


def blank_noncode(text):
    """Replace comments and string literals with spaces, keeping offsets."""
    out = list(text)
    i, n = 0, len(text)
    while i < n:
        c = text[i]
        if c == "/" and i + 1 < n and text[i + 1] == "/":
            while i < n and text[i] != "\n":
                out[i] = " "
                i += 1
        elif c == "/" and i + 1 < n and text[i + 1] == "*":
            while i < n and not (text[i] == "*" and i + 1 < n and text[i + 1] == "/"):
                if text[i] != "\n":
                    out[i] = " "
                i += 1
            for j in range(i, min(i + 2, n)):
                out[j] = " "
            i += 2
        elif c in "\"'":
            quote = c
            out[i] = " "
            i += 1
            while i < n and text[i] != quote:
                if text[i] == "\\":
                    out[i] = " "
                    i += 1
                if i < n and text[i] != "\n":
                    out[i] = " "
                i += 1
            if i < n:
                out[i] = " "
            i += 1
        else:
            i += 1
    return "".join(out)


def offset_of(text, line, column):
    pos = 0
    for _ in range(line - 1):
        pos = text.index("\n", pos) + 1
    return pos + column - 1


def resolve(code, offset):
    """Walk back from the source expression to its destination context."""
    depth = 0
    commas = 0
    i = offset - 1
    while i >= 0:
        c = code[i]
        if c in ")]":
            depth += 1
        elif c == "]":
            depth += 1
        elif c == "(":
            if depth == 0:
                j = i - 1
                while j >= 0 and code[j].isspace():
                    j -= 1
                m = IDENT.search(code[: j + 1])
                if not m:
                    return None
                name = m.group(0)
                if name in ("if", "while", "for", "switch", "return", "sizeof"):
                    return None
                return ("call", name, commas)
            depth -= 1
        elif c == "[":
            depth -= 1
        elif c == "," and depth == 0:
            commas += 1
        elif c == "{" and depth == 0:
            return None
        elif c == ";" and depth == 0:
            return None
        elif c == "=" and depth == 0:
            if i > 0 and code[i - 1] in "=!<>+-*/%&|^":
                return None
            if i + 1 < len(code) and code[i + 1] == "=":
                return None
            start = max(code.rfind(";", 0, i), code.rfind("{", 0, i),
                        code.rfind("}", 0, i), code.rfind(",", 0, i))
            lhs = " ".join(code[start + 1:i].split())
            return ("assign", lhs, 0)
        elif c == "?" and depth == 0:
            return None
        elif c == ":" and depth == 0:
            return None
        i -= 1
    return None


def resolve_return(code, offset):
    j = offset - 1
    while j >= 0 and code[j].isspace():
        j -= 1
    m = IDENT.search(code[: j + 1])
    if not m or m.group(0) != "return":
        return None
    # Enclosing definition: the nearest line starting in column 0 that is
    # followed by a brace-opening body above this point.
    head = code[:offset]
    best = None
    for mm in re.finditer(r"(?m)^([A-Za-z_][^\n;{}]*?)\(([^;{}]*)\)\s*\n?\{", head):
        best = mm
    if not best:
        return None
    decl = " ".join(best.group(1).split())
    name = decl.split()[-1].lstrip("*")
    return ("return", name, 0)


def sites(cc="gcc", jobs=0):
    units = sorted((ROOT / "src" / "smt3").rglob("*.c"))

    def run(path):
        out = subprocess.run([cc] + BASE + [str(path)],
                             capture_output=True, text=True)
        try:
            diags = json.loads(out.stderr)
        except ValueError:
            return []
        found = []
        for entry in diags:
            if entry.get("kind") != "error":
                continue
            if not entry.get("message", "").startswith(MESSAGE):
                continue
            caret = entry["locations"][0]["caret"]
            found.append((caret["file"], caret["line"], caret["column"]))
        return found

    rows = []
    with concurrent.futures.ThreadPoolExecutor(jobs or None) as pool:
        for found in pool.map(run, units):
            rows.extend(found)
    return rows


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("mode", choices=("inventory", "sites", "selfcheck"))
    ap.add_argument("--cc", default="gcc")
    ap.add_argument("--jobs", type=int, default=0)
    ap.add_argument("--key", help="sites mode: only this destination key")
    args = ap.parse_args()

    if args.mode == "selfcheck":
        return selfcheck()

    rows = sites(args.cc, args.jobs)
    cache = {}
    counts = collections.Counter()
    places = collections.defaultdict(list)
    for path, line, column in rows:
        if path not in cache:
            text = pathlib.Path(ROOT / path).read_text(errors="replace")
            cache[path] = (text, blank_noncode(text))
        text, code = cache[path]
        off = offset_of(text, line, column)
        got = resolve_return(code, off) or resolve(code, off)
        key = "unresolved" if got is None else f"{got[0]}:{got[1]}" + (
            f":{got[2]}" if got[0] == "call" else "")
        counts[key] += 1
        places[key].append(f"{path}:{line}:{column}")

    if args.mode == "sites":
        for key in ([args.key] if args.key else sorted(places)):
            for place in places.get(key, []):
                print(f"{key}\t{place}")
        return 0

    print(f"{len(rows)} truncated-address sites, "
          f"{len(counts) - (1 if 'unresolved' in counts else 0)} destinations")
    for key, count in counts.most_common():
        print(f"{count:5d}  {key}")
    return 0


SELFCHECK = r"""
void sink32(u32 a, u32 b);
void sinkp(void *p);
u32 retaddr(void *p) { return EE_ADDR(p); }
void positive(void *p, u32 *slot) {
    sink32(0, EE_ADDR(p));
    *slot = EE_ADDR(p);
}
void negative(void *p, u64 *wide) {
    sinkp(p);
    *wide = EE_ADDR(p);
    sink32(0, (u32)sizeof(*wide));
}
"""


def selfcheck():
    """A known positive and a known negative, through the real pipeline."""
    tmp = ROOT / "src" / "smt3" / "host64_selfcheck.c"
    tmp.write_text('#include "common.h"\n' + SELFCHECK)
    try:
        rows = sites()
    finally:
        tmp.unlink()
    mine = [r for r in rows if r[0].endswith("host64_selfcheck.c")]
    text = '#include "common.h"\n' + SELFCHECK
    code = blank_noncode(text)
    keys = []
    for path, line, column in mine:
        off = offset_of(text, line, column)
        got = resolve_return(code, off) or resolve(code, off)
        keys.append("unresolved" if got is None else
                    f"{got[0]}:{got[1]}" + (f":{got[2]}" if got[0] == "call" else ""))
    want = {"call:sink32:1", "assign:*slot", "return:retaddr"}
    got = set(keys)
    print(f"positive: want {sorted(want)}")
    print(f"found:    {sorted(got)}")
    bad = [k for k in keys if k == "unresolved"]
    ok = want <= got and not bad and len(keys) == 3
    print("negative: no site for the void * argument, the u64 store or the "
          f"sizeof cast ({len(keys)} sites total, expected 3)")
    print("selfcheck:", "pass" if ok else "FAIL")
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
