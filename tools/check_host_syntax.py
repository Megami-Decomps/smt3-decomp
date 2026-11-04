#!/usr/bin/env python3
"""Syntax-check every NON_MATCHING and FIX_BUGS body with a host clang.

    python3 tools/check_host_syntax.py --host-include <dir>   (or $HOST_INCLUDE)

The host include dir is the port's include/ (its include/host shadows the EE
headers). Without it the EE headers do not parse, so this refuses to run. Each
unit is checked with -DNON_MATCHING -DFIX_BUGS and with -DNON_MATCHING alone.
"""
import argparse
import os
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent


def clang(unit: Path, host: Path, fix_bugs: bool) -> str:
    cmd = ["clang", "-fsyntax-only", "-m32", "-std=gnu90", "-w",
           "-Wno-int-conversion", "-Wno-incompatible-pointer-types", "-Wno-return-mismatch",
           "-DVERSION_SMT3", "-DSKIP_ASM", "-DNON_MATCHING", '-DASM_ROOT=""',
           *(["-DFIX_BUGS"] if fix_bugs else []),
           f"-I{host}/host", f"-I{host}", f"-I{ROOT}/include", f"-I{ROOT}/src", str(unit)]
    out = subprocess.run(cmd, capture_output=True, text=True)
    errors = [l for l in out.stderr.splitlines() if "error:" in l]
    if out.returncode and not errors:
        errors = [f"clang exited {out.returncode}: {out.stderr.strip()[:200]}"]
    return "\n".join(errors[:3])


def control(host: Path) -> None:
    body = "int f(int a)\n{\n#ifdef FIX_BUGS\n    if (a) {\n        a++;\n%s#endif\n    return a;\n}\n"
    with tempfile.TemporaryDirectory() as tmp:
        for text, name, must_fail in ((body % "", "dropped.c", True),
                                      (body % "    }\n", "whole.c", False)):
            unit = Path(tmp) / name
            unit.write_text(text)
            if bool(clang(unit, host, True)) != must_fail:
                sys.exit(f"control {name} gave the wrong answer, so this check cannot be trusted")


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--host-include", default=os.environ.get("HOST_INCLUDE"))
    ap.add_argument("--exclude", action="append", default=[], metavar="UNIT=REASON",
                    help="skip a unit (path relative to src/smt3) and print why")
    ap.add_argument("units", nargs="*")
    args = ap.parse_args()
    if not args.host_include or not (Path(args.host_include) / "host").is_dir():
        sys.exit("cannot look: pass --host-include <port>/include (needs a host/ directory)")
    host = Path(args.host_include).resolve()
    control(host)
    units = [Path(u) for u in args.units] or sorted(
        p for p in (ROOT / "src/smt3").rglob("*.c")
        if "NON_MATCHING" in (t := p.read_text()) or "FIX_BUGS" in t)
    excluded = {}
    for item in args.exclude:
        name, _, reason = item.partition("=")
        if not reason or not (ROOT / "src/smt3" / name).is_file():
            sys.exit(f"--exclude {item!r}: needs UNIT=REASON naming a unit under src/smt3")
        excluded[ROOT / "src/smt3" / name] = reason
    for unit, reason in excluded.items():
        print(f"{unit.relative_to(ROOT)}: excluded, {reason}")
    units = [u for u in units if u.resolve() not in {e.resolve() for e in excluded}]
    bad = 0
    for unit in units:
        for fix_bugs in (True, False):
            err = clang(unit, host, fix_bugs)
            if err:
                bad += 1
                print(f"{unit}: FAILS ({'with' if fix_bugs else 'without'} FIX_BUGS)\n    " + err.replace("\n", "\n    "))
    print(f"{len(units)} unit(s), {bad} failing pass(es)")
    return 1 if bad else 0


sys.exit(main())
