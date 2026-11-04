#!/usr/bin/env python3
"""Fail if configure.py rewrites any tracked source under src/.

    python3 tools/check_generated_sources.py

configure.py re-splits and runs tools/include_rodata.py and tools/include_sdata.py,
which rewrite the INCLUDE_RODATA/INCLUDE_SDATA lines in src/**/*.c. The tree is
meant to be committed in their output, so a change that moves the split leaves
sources dirty until those lines are committed. Needs orig/smt3/SLUS_209.11.

The check compares src/ before and after a forced configure, so uncommitted work
does not matter. A control then adds a stale INCLUDE_RODATA line to a source and
requires the same comparison to flag it.
"""
import hashlib
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
STALE = 'INCLUDE_RODATA(const s32, "sdk/libc", D_DEADBEEF);\n'
STALE_UNIT = ROOT / "src" / "smt3" / "sdk" / "libc.c"


def snapshot() -> dict[str, str]:
    return {
        str(p.relative_to(ROOT)): hashlib.sha1(p.read_bytes()).hexdigest()
        for p in sorted((ROOT / "src").rglob("*")) if p.is_file()
    }


def configure_changes() -> list[str]:
    """Files under src/ that a forced configure changed."""
    before = snapshot()
    subprocess.run([sys.executable, "configure.py", "--force-split"], cwd=ROOT, check=True)
    after = snapshot()
    return sorted(n for n in before.keys() | after.keys() if before.get(n) != after.get(n))


def main() -> int:
    changed = configure_changes()
    if changed:
        print("configure.py rewrote tracked sources; commit the regenerated "
              "INCLUDE_RODATA/INCLUDE_SDATA lines:", file=sys.stderr)
        for name in changed:
            print(f"  {name}", file=sys.stderr)
        return 1

    # Negative control: a stale line must be seen as a change, or the check is mute.
    original = STALE_UNIT.read_bytes()
    try:
        STALE_UNIT.write_bytes(original + STALE.encode())
        flagged = configure_changes()
    finally:
        STALE_UNIT.write_bytes(original)
    if flagged != [str(STALE_UNIT.relative_to(ROOT))]:
        print(f"control failed: a stale INCLUDE_RODATA line in {STALE_UNIT.name} "
              f"was reported as {flagged}", file=sys.stderr)
        return 2
    print("sources unchanged by configure; control flagged a stale line")
    return 0


if __name__ == "__main__":
    sys.exit(main())
