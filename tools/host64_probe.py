#!/usr/bin/env python3
"""Compile every src/smt3 unit for a 64-bit host and report what fails.

Two modes, because they answer different questions.

  accept  does an LP64 compiler accept the unit at all, warnings off. This is
          the same question tools/host64/*.h exist to answer, and the same
          method sdfport's own 32-bit probe used: one unit at a time,
          -fsyntax-only, no link.
  width   does the unit narrow or widen a pointer, with the pointer-width
          diagnostics promoted to errors. A unit that passes accept can still
          fail this, and that is the interesting failure: it compiles and is
          wrong.
  trunc   does the unit store an EE_ADDR result in a 32-bit declaration. This
          is what survives naming every cast: the address is no longer
          truncated by the cast, it is truncated by the declaration it lands
          in, which only widening that declaration can fix.

Nothing here is part of the retail build.
"""

import argparse
import concurrent.futures
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
]
# gcc 14 makes the two pointer/integer mismatch classes hard errors. They are
# the decompilation's own typing, the same class sdfport's 32-bit probe
# silences with -Wno-int-conversion, and width mode counts them on purpose,
# so accept mode demotes exactly those two and nothing else.
ACCEPT = ["-w", "-Wno-error=int-conversion",
          "-Wno-error=incompatible-pointer-types"]
# No -w here: -w outranks -Werror=<name> and silences the very diagnostics
# this mode exists to count, which is how a first attempt at this measurement
# reported zero pointer-width casts in a tree that has hundreds.
WIDTH = [
    "-Wno-all", "-Wno-extra",
    "-Werror=pointer-to-int-cast", "-Werror=int-to-pointer-cast",
    "-Werror=int-conversion", "-Werror=incompatible-pointer-types",
]

TRUNC = ["-Wno-all", "-Wno-extra", "-Werror=conversion",
         "-Wno-error=sign-conversion"]
# gcc prints a cast's result as its underlying type and a declared variable as
# its typedef name plus {aka ...}, so anchoring on `from '<type>' to` selects
# the EE_ADDR expansions and rejects u64 and size_t variables, which are
# the two classes that would otherwise be counted here. Keyed on ee_addr_t's
# spelling: `long unsigned int` would also match every sizeof.
TRUNC_RE = re.compile(r"error: conversion from .long long unsigned int. to")

CLASSES = [
    ("pointer narrowed to a 32-bit integer", r"cast from pointer to integer of different size"),
    ("32-bit integer widened to a pointer", r"cast to pointer from integer of different size"),
    ("pointer against integer without a cast", r"(initialization|assignment|passing argument|returning).*makes (pointer|integer) from"),
    ("incompatible pointer types", r"incompatible pointer type"),
    ("EE inline asm register or constraint", r"(unknown register name|impossible constraint|inconsistent operand)"),
    ("missing declaration", r"(undeclared|implicit declaration|unknown type name)"),
]


def units():
    return sorted(p for p in (ROOT / "src" / "smt3").rglob("*.c"))


def compile_one(cc, extra, path):
    out = subprocess.run([cc] + BASE + extra + [str(path)],
                         capture_output=True, text=True)
    return path, out.returncode, out.stderr


def classify(stderr):
    hits = set()
    for line in stderr.splitlines():
        # Errors only: a warning does not fail the unit, so counting one
        # would attribute a failure to the wrong class.
        if ": error:" not in line:
            continue
        for name, pattern in CLASSES:
            if re.search(pattern, line):
                hits.add(name)
                break
        else:
            hits.add("other")
    return hits


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("mode", choices=("accept", "width", "trunc"))
    ap.add_argument("--cc", default="gcc")
    ap.add_argument("--jobs", type=int, default=0)
    ap.add_argument("--list-failures", action="store_true")
    args = ap.parse_args()

    extra = {"accept": ACCEPT, "width": WIDTH, "trunc": TRUNC}[args.mode]
    all_units = units()
    failed = []
    counts = {}
    sites = 0
    with concurrent.futures.ThreadPoolExecutor(args.jobs or None) as pool:
        for path, rc, stderr in pool.map(
                lambda p: compile_one(args.cc, extra, p), all_units):
            if args.mode == "trunc":
                hits = len(TRUNC_RE.findall(stderr))
                sites += hits
                if hits:
                    failed.append(path)
                continue
            if rc == 0:
                continue
            failed.append(path)
            for name in classify(stderr):
                counts[name] = counts.get(name, 0) + 1

    if args.mode == "trunc":
        print(f"trunc: {sites} EE_ADDR results stored in a 32-bit declaration, "
              f"over {len(failed)} units ({args.cc} -m64)")
    else:
        print(f"{args.mode}: {len(all_units) - len(failed)}/{len(all_units)} "
              f"units compile ({args.cc} -m64)")
    for name, count in sorted(counts.items(), key=lambda kv: -kv[1]):
        print(f"  {count:4d} units: {name}")
    if args.list_failures:
        for path in failed:
            print(path.relative_to(ROOT))
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
