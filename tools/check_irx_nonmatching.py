#!/usr/bin/env python3
"""Compile IRX C with NON_MATCHING, with and without FIX_BUGS.

A syntax check, not a match.

    python tools/check_irx_nonmatching.py [MODULE ...]

The normal build takes the assembly for a guarded function and never compiles
its C, so a draft can carry an error indefinitely.
"""
from __future__ import annotations

import argparse
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(Path(__file__).resolve().parent))
import irx_config  # noqa: E402

# The build's own irx.h pulls in the assembly and needs ASM_ROOT; here the point
# is to compile the C, so INCLUDE_ASM goes away and va_list comes from the host.
SHIM = """#ifndef IRX_H
#define IRX_H
#include <stdarg.h>
#include <stddef.h>
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed char s8;
typedef short s16;
typedef int s32;
typedef unsigned long long u64;
typedef long long s64;
#define INCLUDE_ASM(MODULE, NAME)
#define ALIGN_TEXT(n)
#define IRX_IMPORT(LIB, NAME, ORDINAL)
#define IRX_IMPORT_TABLE(LIB, MAJOR, MINOR)
#define IRX_IMPORT_TABLE_END
#endif
"""


def check(module: str, shim: Path) -> int:
    cfg = irx_config.load(module)
    src = irx_config.path(cfg, "src")
    # Check both modes unconditionally, including FIX_BUGS uses in headers.
    # A corrected branch is otherwise as invisible to CI as an assembly-guarded
    # draft. This does not alter the production compiler or matching build.
    errors = 0
    for fix_bugs in (False, True):
        label = "NON_MATCHING + FIX_BUGS" if fix_bugs else "NON_MATCHING"
        # IRX drafts use pre-C23 non-prototype declarations. GCC 15 defaults to
        # C23, where f() means f(void); retain the legacy GNU C dialect.
        # Match the EE draft gate: a typo or missing callee declaration must
        # fail even on hosts that still treat implicit declarations as warnings.
        # Other warnings and legacy explicit non-prototypes remain permitted.
        out = subprocess.run(
            ["gcc", "-std=gnu17", "-m32", "-fsyntax-only", "-DNON_MATCHING",
             *(["-DFIX_BUGS"] if fix_bugs else []),
             "-Werror=implicit-function-declaration",
             "-include", str(shim / "irx.h"),
             "-I", str(ROOT / "include" / "irx"), str(src)],
            capture_output=True, text=True, cwd=ROOT)
        bad = [l for l in out.stderr.splitlines() if "error:" in l]
        if out.returncode and not bad:
            bad = [f"compiler exited with status {out.returncode}"]
            detail = (out.stderr or out.stdout).strip()
            if detail:
                bad.append(detail)
        print(f"{module} ({label}): {'ok' if not bad else str(len(bad)) + ' error(s)'}")
        for line in bad[:20]:
            print("   " + line)
        errors += len(bad)
    return errors



def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("modules", nargs="*")
    args = ap.parse_args()
    with tempfile.TemporaryDirectory() as tmp:
        shim = Path(tmp)
        (shim / "irx.h").write_text(SHIM)
        bad = sum(check(m, shim) for m in args.modules or irx_config.modules())
    raise SystemExit(1 if bad else 0)


if __name__ == "__main__":
    main()
