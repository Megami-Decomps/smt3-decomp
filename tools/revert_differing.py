#!/usr/bin/env python3
"""Put every C function check_unit.py reports as not matching back to INCLUDE_ASM.

    python3 tools/revert_differing.py src/smt3/<unit>.c [...] [--dry-run]

A unit must contain only C that matches: a near miss earns no credit and hides
that the unit is incomplete. Structs, prototypes and matching functions are
left alone. Run configure.py --force-split afterwards, since the set of
INCLUDE_ASM lines has changed.
"""
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent

# The labels check_unit.py prints for a function it cannot credit. "DIFF jump
# table of X" and the rodata/sdata forms name the owning function too, so the
# optional "<word> of" prefix is matched and discarded.
BAD = re.compile(
    r"^(?:DIFF|OVER|CONTEXT|PAD|SHARED)(?: (?:jump table|rodata|sdata) of)? (\w+)\b", re.M)

# A function definition's opening line: a name followed by a parameter list,
# not a call, a control statement or an INCLUDE_ macro.
OPEN = re.compile(r"^[A-Za-z_][\w \t*]*?\b([A-Za-z_]\w*)\s*\([^;]*\)\s*$")
KEYWORDS = {"if", "for", "while", "switch", "return", "sizeof", "else", "do"}


def spans(text):
    """Yield (name, start, end) for each top-level function definition."""
    lines = text.split("\n")
    i = 0
    while i < len(lines):
        m = OPEN.match(lines[i])
        if m and m.group(1) not in KEYWORDS and i + 1 < len(lines) and lines[i + 1].startswith("{"):
            start = i
            # The signature may span several lines above; walk back over them.
            while start > 0 and lines[start - 1].strip() not in ("", "}") \
                    and not lines[start - 1].rstrip().endswith((";", "{", "*/")) \
                    and not lines[start - 1].lstrip().startswith("#"):
                start -= 1
            depth, j = 0, i + 1
            while j < len(lines):
                depth += lines[j].count("{") - lines[j].count("}")
                if depth == 0:
                    break
                j += 1
            yield m.group(1), start, j
            i = j + 1
        else:
            i += 1


def revert(path: Path, dry_run=False) -> list[str]:
    unit = path.resolve().relative_to(ROOT / "src" / "smt3").with_suffix("").as_posix()
    out = subprocess.run([sys.executable, str(ROOT / "tools/check_unit.py"), str(path)],
                         capture_output=True, text=True).stdout
    bad = set(BAD.findall(out))
    if not bad:
        return []
    lines = path.read_text().split("\n")
    drop = []
    for name, start, end in spans("\n".join(lines)):
        if name in bad:
            drop.append((start, end, name))
    if not drop:
        return []
    for start, end, name in reversed(drop):
        lines[start:end + 1] = [f'INCLUDE_ASM(const s32, "{unit}", {name});']
    if not dry_run:
        path.write_text("\n".join(lines))
    return sorted(n for _, _, n in drop)


if __name__ == "__main__":
    args = [a for a in sys.argv[1:] if a != "--dry-run"]
    dry = "--dry-run" in sys.argv
    for arg in args:
        names = revert(Path(arg), dry)
        print(f"{arg}: {'would revert' if dry else 'reverted'} {len(names)}"
              + (f": {' '.join(names)}" if names else ""))
