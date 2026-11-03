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
WIDTH = [
    "-w",
    "-Werror=pointer-to-int-cast", "-Werror=int-to-pointer-cast",
    "-Werror=int-conversion", "-Werror=incompatible-pointer-types",
]

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
        if ": error:" not in line and ": warning:" not in line:
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
    ap.add_argument("mode", choices=("accept", "width"))
    ap.add_argument("--cc", default="gcc")
    ap.add_argument("--jobs", type=int, default=0)
    ap.add_argument("--list-failures", action="store_true")
    args = ap.parse_args()

    extra = ACCEPT if args.mode == "accept" else WIDTH
    all_units = units()
    failed = []
    counts = {}
    with concurrent.futures.ThreadPoolExecutor(args.jobs or None) as pool:
        for path, rc, stderr in pool.map(
                lambda p: compile_one(args.cc, extra, p), all_units):
            if rc == 0:
                continue
            failed.append(path)
            for name in classify(stderr):
                counts[name] = counts.get(name, 0) + 1

    print(f"{args.mode}: {len(all_units) - len(failed)}/{len(all_units)} units compile "
          f"({args.cc} -m64)")
    for name, count in sorted(counts.items(), key=lambda kv: -kv[1]):
        print(f"  {count:4d} units: {name}")
    if args.list_failures:
        for path in failed:
            print(path.relative_to(ROOT))
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
