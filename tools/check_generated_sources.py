#!/usr/bin/env python3
"""Fail if configure.py rewrites any tracked source under src/.

    python3 tools/check_generated_sources.py

configure.py re-splits and runs tools/include_rodata.py and tools/include_sdata.py,
which rewrite the INCLUDE_RODATA/INCLUDE_SDATA lines in src/**/*.c. The tree is
meant to be committed in their output, so a change that moves the split leaves
sources dirty until those lines are committed. Needs orig/smt3/SLUS_209.11.

The check compares src/ before and after a forced configure, so uncommitted work
does not matter. A control then adds a stale INCLUDE_RODATA line in an isolated copy and
requires the same comparison to flag it, without mutating the working sources.
"""
import hashlib
import shutil
import subprocess
import tempfile
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
STALE = 'INCLUDE_RODATA(const s32, "sdk/libc", D_DEADBEEF);\n'
STALE_UNIT = Path("src/smt3/sdk/libc.c")


def snapshot(root: Path) -> dict[str, str]:
    return {
        str(p.relative_to(root)): hashlib.sha1(p.read_bytes()).hexdigest()
        for p in sorted((root / "src").rglob("*")) if p.is_file()
    }


def configure_changes(root: Path) -> list[str]:
    """Files under src/ that a forced configure changed."""
    before = snapshot(root)
    subprocess.run([str(Path(sys.executable).absolute()), "configure.py", "--force-split"], cwd=root, check=True)
    after = snapshot(root)
    return sorted(n for n in before.keys() | after.keys() if before.get(n) != after.get(n))


def control_changes(root: Path) -> list[str]:
    """Run the destructive negative control in a disposable, independent tree.

    Copies, not hard links or directory symlinks: configure rewrites sources and
    generated config. Even an uncatchable termination must not leave the caller's
    source tree with the deliberately invalid sentinel.
    """
    with tempfile.TemporaryDirectory(prefix="smt3-generated-control-") as tmp:
        control = Path(tmp)
        shutil.copy2(root / "configure.py", control / "configure.py")
        for name in ("src", "config", "include", "tools", "orig"):
            shutil.copytree(root / name, control / name)
        unit = control / STALE_UNIT
        unit.write_bytes(unit.read_bytes() + STALE.encode())
        return configure_changes(control)


def main() -> int:
    changed = configure_changes(ROOT)
    if changed:
        print("configure.py rewrote tracked sources; commit the regenerated "
              "INCLUDE_RODATA/INCLUDE_SDATA lines:", file=sys.stderr)
        for name in changed:
            print(f"  {name}", file=sys.stderr)
        return 1

    # Negative control: a stale line must be seen as a change, or the check is mute.
    flagged = control_changes(ROOT)
    if flagged != [str(STALE_UNIT)]:
        print(f"control failed: a stale INCLUDE_RODATA line in {STALE_UNIT.name} "
              f"was reported as {flagged}", file=sys.stderr)
        return 2
    print("sources unchanged by configure; control flagged a stale line")
    return 0


if __name__ == "__main__":
    sys.exit(main())
