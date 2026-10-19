#!/usr/bin/env python3
"""Put every C function check_unit.py reports as not matching behind its fallback.

    python3 tools/revert_differing.py src/smt3/<unit>.c [...] [--dry-run] [--discard]

A unit must build from asm for anything that does not match: a near miss earns
no credit and hides that the unit is incomplete. The draft is kept rather than
thrown away, wrapped as

    #ifdef NON_MATCHING
    <the draft>
    #else
    INCLUDE_ASM(const s32, "unit/path", name);
    #endif

so the normal build takes the asm and the next person starts from the draft
instead of from scratch. One line above it says how close it is and what
differs, "12 of 694 words, three spill slots rotated". The rest goes in
docs/nonmatching/<unit>.md: the remaining difference and the shapes already
ruled out, which tools/check_nonmatching.py requires. --discard deletes the
draft instead, for one not worth keeping.

Structs, prototypes and matching functions are left alone. Run
configure.py --force-split afterwards, since the set of INCLUDE_ASM lines has
changed, and tools/check_nonmatching.py to confirm the draft still compiles
and is written up.
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
# The measured difference, so the kept draft can say how close it is.
DETAIL = re.compile(r"^(?:DIFF|OVER|CONTEXT|PAD|SHARED)[^\n]*?\b(\w+)\b[^\n]*?: (.+)$", re.M)

# A function definition's opening line: a name followed by a parameter list,
# not a call, a control statement or an INCLUDE_ macro. Both brace styles
# occur in this tree, the opening brace on the same line or on the next.
OPEN = re.compile(r"^[A-Za-z_][\w \t*]*?\b([A-Za-z_]\w*)\s*\([^;]*\)\s*\{?\s*$")
KEYWORDS = {"if", "for", "while", "switch", "return", "sizeof", "else", "do"}


def spans(text):
    """Yield (name, start, end) for each top-level function definition.

    Handles both brace styles in this tree and a signature wrapped over
    several lines: a definition missed here is reverted silently, so the
    scanner balances the parameter list rather than matching one line.
    """
    lines = text.split("\n")
    i = 0
    while i < len(lines):
        line = lines[i]
        if not line[:1].isalpha() and line[:1] != "_":
            i += 1
            continue
        # Join continuation lines until the parameter list closes.
        head, j = line, i
        while head.count("(") > head.count(")") and j + 1 < len(lines):
            j += 1
            head += " " + lines[j].strip()
        m = OPEN.match(" ".join(head.split()))
        if not m or m.group(1) in KEYWORDS:
            i += 1
            continue
        brace_here = head.rstrip().endswith("{")
        brace_next = j + 1 < len(lines) and lines[j + 1].startswith("{")
        if not (brace_here or brace_next):
            i += 1
            continue
        start = i
        # The return type may sit on its own line above the name.
        while start > 0 and lines[start - 1].strip() not in ("", "}") \
                and not lines[start - 1].rstrip().endswith((";", "{", "*/")) \
                and not lines[start - 1].lstrip().startswith("#"):
            start -= 1
        depth, k = 0, j if brace_here else j + 1
        while k < len(lines):
            depth += lines[k].count("{") - lines[k].count("}")
            if depth == 0:
                break
            k += 1
        yield m.group(1), start, k
        i = k + 1


def wrap(body: list[str], unit: str, name: str, detail: str = "") -> list[str]:
    """The draft kept behind its asm fallback, labelled with what still differs."""
    note = f"/* NON_MATCHING: {detail or 'difference not recorded'}. */"
    return ([note, "#ifdef NON_MATCHING"] + body + ["#else",
            f'INCLUDE_ASM(const s32, "{unit}", {name});', "#endif"])


def revert(path: Path, dry_run=False, discard=False) -> list[str]:
    unit = path.resolve().relative_to(ROOT / "src" / "smt3").with_suffix("").as_posix()
    out = subprocess.run([sys.executable, str(ROOT / "tools/check_unit.py"), str(path)],
                         capture_output=True, text=True).stdout
    bad = set(BAD.findall(out))
    detail = {n: d.strip() for n, d in DETAIL.findall(out)}
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
        if discard:
            lines[start:end + 1] = [f'INCLUDE_ASM(const s32, "{unit}", {name});']
        else:
            lines[start:end + 1] = wrap(lines[start:end + 1], unit, name,
                                        detail.get(name, ""))
    if not dry_run:
        path.write_text("\n".join(lines))
    return sorted(n for _, _, n in drop)


if __name__ == "__main__":
    flags = {"--dry-run", "--discard"}
    args = [a for a in sys.argv[1:] if a not in flags]
    dry = "--dry-run" in sys.argv
    discard = "--discard" in sys.argv
    verb = "would " if dry else ""
    verb += "discarded" if discard else "kept behind the fallback"
    for arg in args:
        names = revert(Path(arg), dry, discard)
        print(f"{arg}: {len(names)} {verb}"
              + (f": {' '.join(names)}" if names else ""))
