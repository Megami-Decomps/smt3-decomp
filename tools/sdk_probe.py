#!/usr/bin/env python3
"""Compile a candidate C file and compare its functions to the retail bytes.

    python tools/sdk_probe.py smt3 probe.c [--toolchain ee-gcc2.9-991111-01]

A standalone loop for the SDK region: no linking, no build, so several of these
can run at once. docs/sdk-matching.md has the method.
"""
from __future__ import annotations

import argparse
import shlex
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
from sdk_objects import mask, object_funcs, retail_funcs  # noqa: E402

from configure import TOOLCHAINS, i386_prefix  # noqa: E402

REGION = (0x2C81A8, 0x2E6E50)
ROOT = Path(__file__).resolve().parent.parent


def compile_probe(source: Path, toolchain: str, out: Path, extra: list[str]) -> None:
    chain = TOOLCHAINS[toolchain]
    prefix = i386_prefix().split()
    asm = out.with_suffix(".s")
    run = prefix + [str(ROOT / chain["cc1"]), *shlex.split(chain["defines"]),
                    "-Iinclude", "-Isrc", *shlex.split(chain["flags"]), *extra,
                    str(source), "-o", str(asm)]
    subprocess.run(run, cwd=ROOT, check=True)
    subprocess.run(prefix + [str(ROOT / chain["as"]), *shlex.split(chain["as_flags"]),
                             "-o", str(out), str(asm)], cwd=ROOT, check=True)


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("version")
    ap.add_argument("source", type=Path)
    ap.add_argument("--toolchain", default="ee-gcc2.9-991111-01", choices=sorted(TOOLCHAINS))
    ap.add_argument("--flag", action="append", default=[], help="extra cc1 flag")
    ap.add_argument("--words", type=int, default=16)
    args = ap.parse_args()

    retail = {name: code for _, name, code in retail_funcs(args.version, *REGION)}
    with tempfile.TemporaryDirectory(prefix="sdk_probe.") as tmp:
        obj = Path(tmp) / "probe.o"
        compile_probe(args.source.resolve(), args.toolchain, obj, args.flag)
        funcs = object_funcs(obj)

    if not funcs:
        sys.exit("the probe defines no functions")
    bad = 0
    for name, code in funcs:
        if name not in retail:
            print(f"{name:26} not a function in the SDK region")
            bad += 1
            continue
        want = retail[name]
        fill = len(want) - len(code)
        if mask(code) == mask(want[:len(code)]) and (fill == 0 or set(want[len(code):]) <= {0}):
            note = "" if fill == 0 else f" + {fill} alignment fill"
            print(f"{name:26} match  {len(code)} bytes{note}")
            continue
        bad += 1
        print(f"{name:26} DIFF   built {len(code)}, retail span {len(want)}")
        shown = 0
        for i in range(0, min(len(code), len(want)), 4):
            if mask(code[i:i + 4]) != mask(want[i:i + 4]) and shown < args.words:
                (a,) = struct.unpack_from("<I", code, i)
                (b,) = struct.unpack_from("<I", want, i)
                print(f"    +{i:03X}  built {a:08X}  retail {b:08X}")
                shown += 1
    sys.exit(1 if bad else 0)


if __name__ == "__main__":
    main()
