#!/usr/bin/env python3
"""Source coverage per version: functions and code bytes in C, and drafts.

    python3 tools/progress.py [smt3] [--markdown]

Every retail function of a game unit is either C in src/<v>/ or an
INCLUDE_ASM line. Sizes come from the split assembly (asm/<v>/, so run
configure.py first). Library code outside src/ is not counted, and the VU
microcode is reported on its own, since it is not EE code. This source inventory does not compile or verify matching;
use check_unit.py and the retail checksum build for that.

Functions still assembled are counted again on their own: a NON_MATCHING
body is compiled by builds defining NON_MATCHING. The normal matching build
uses the assembly fallback, so retained C earns no matching coverage credit
and is reported as a separate counter.
"""
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import vu_include  # noqa: E402
from check_nonmatching import draft_functions  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
INCLUDE = re.compile(r'^INCLUDE_ASM\([^\n]*\b(\w+)\);', re.M)
GLABEL = re.compile(r"^glabel (\w+)\n(.*?)^endlabel \1", re.M | re.S)
INSN = re.compile(r"^\s*/\* [0-9A-F]+ [0-9A-F]{8} [0-9A-F]{8} \*/", re.M)


def version_stats(version):
    funcs = c_funcs = draft_funcs = size = c_size = draft_size = 0
    drafts = draft_functions(version)
    for unit in (ROOT / "src" / version).rglob("*.c"):
        name = unit.relative_to(ROOT / "src" / version).with_suffix("").as_posix()
        # Match the primary report's game scope. SDK/runtime reconstructions
        # have their own category and must not move the game-code denominator.
        if name.startswith("sdk/"):
            continue
        full = ROOT / "asm" / version / f"{name}.s"
        if not full.exists():
            continue
        asm_funcs = set(INCLUDE.findall(unit.read_text()))
        unit_drafts = drafts.get(name, set())
        for fname, body in GLABEL.findall(full.read_text()):
            n = 4 * len(INSN.findall(body))
            funcs += 1
            size += n
            if fname not in asm_funcs:
                c_funcs += 1
                c_size += n
            elif fname in unit_drafts:
                draft_funcs += 1
                draft_size += n
    return funcs, c_funcs, draft_funcs, size, c_size, draft_size


def vu_line(version):
    """The microcode counters, or None when the retail image is not at hand."""
    try:
        return vu_include.coverage(version)
    except FileNotFoundError:
        return None


def main():
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    md = "--markdown" in sys.argv
    if md:
        print("| Version | Functions in C | Code bytes in C | Non-matching drafts | VU microcode |"
              "\n|---|---|---|---|---|")
    for v in args or ["smt3"]:
        f, cf, df, s, cs, ds = version_stats(v)
        if not f:
            print(f"{v}: no split assembly (run configure.py)", file=sys.stderr)
            continue
        # Only a function that is still assembled can have a draft, so the
        # drafts are a share of what is left rather than of the whole game.
        left, left_size = f - cf, s - cs
        vu = vu_line(v)
        vu_text = (f"{vu['instructions']:,} instructions, {vu['source_bytes']:,} / "
                   f"{vu['microcode_bytes']:,} bytes from source") if vu else "retail image missing"
        if md:
            print(f"| `{v}` | {cf:,} / {f:,} ({100 * cf / f:.1f}%) | {cs:,} / {s:,} ({100 * cs / s:.1f}%) "
                  f"| {df:,} / {left:,} ({100 * df / left if left else 0:.1f}%) | {vu_text} |")
        else:
            print(f"{v}: {cf}/{f} functions in C ({100 * cf / f:.1f}%), "
                  f"{cs}/{s} code bytes in C ({100 * cs / s:.1f}%)")
            print(f"{v}: {df}/{left} remaining functions have a non-matching draft "
                  f"({100 * df / left if left else 0:.1f}%), "
                  f"{ds}/{left_size} remaining code bytes "
                  f"({100 * ds / left_size if left_size else 0:.1f}%)")
            print(f"{v}: VU microcode {vu_text}")


if __name__ == "__main__":
    main()
