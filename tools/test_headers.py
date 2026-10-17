#!/usr/bin/env python3
"""Compile tools/header_layout.c, so include/'s sizes and offsets stay checked.

    python3 tools/test_headers.py
"""
import re
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
CHECK = ROOT / "tools" / "header_layout.c"
CC = ROOT / "tools" / "cc.sh"


def compile_source(text):
    with tempfile.TemporaryDirectory() as tmp:
        src = Path(tmp) / "layout.c"
        src.write_text(text)
        done = subprocess.run(
            [str(CC), "-Wall", str(src), "-o", str(Path(tmp) / "layout.o")],
            cwd=ROOT, capture_output=True, text=True,
        )
        return done.returncode, done.stdout + done.stderr


def main():
    if not (ROOT / "tools" / "compilers" / "ee-gcc2.96").is_dir():
        sys.exit("cc1 missing; run tools/download_tools.py")
    text = CHECK.read_text()
    claims = len(re.findall(r"^(?:SZ|OFF)\(", text, re.M))
    if claims < 100:
        sys.exit(f"{CHECK.name} has only {claims} claims; expected the full set")

    code, output = compile_source(text)
    if code != 0:
        sys.exit(f"header_layout.c does not compile:\n{output}")

    # A wrong number must fail, or the claims are passing vacuously.
    broken = re.sub(r"^SZ\((\w+), (\w+), (\d+)\);", r"SZ(\1, \2, \g<3>1);", text, count=1, flags=re.M)
    if broken == text:
        sys.exit("could not build the negative control")
    code, _ = compile_source(broken)
    if code == 0:
        sys.exit("a wrong size still compiles; the assertions are vacuous")

    print(f"OK ({claims} size and offset claims)")


if __name__ == "__main__":
    main()
