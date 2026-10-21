#!/usr/bin/env python3
"""Fail when a new invented type or member name appears.

Two kinds that keep coming back: a type named off the convention (`SdfPool` where
the tree spells it `sdfPool_t`) and a member left `unk_18`. Every one that exists
today is in config/smt3/drift_baseline.txt, so this fails only on a new name.
Report and fail with no argument; `--update` after a deliberate fix."""

import argparse
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from cparse import structs  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
BASELINE = ROOT / "config" / "smt3" / "drift_baseline.txt"

# The tree spells a type `<module><Name>_t`: a lowercase module prefix and a suffix.
# `SdfPool` and `EffRecordPool` break both halves, which is what makes them findable.
CONVENTION = re.compile(r"_(?:t\d?|u|e|b|s|tag\d?)$")
INVENTED = re.compile(r"^[A-Z][a-z0-9]")

# A member the decompiler named after its offset rather than its job. The `unk`
# prefix has to be followed by the offset or nothing, so a real word that starts
# with it (`unknownReason`) is not swept up.
UNNAMED = re.compile(r"^(?:unk(?:nown)?(?:[_0-9]|$)|field_0x|dummy\d|unused\d)", re.I)

# Names that are correct as they are: newlib, the Sony SDK and the IDB's own
# spellings, which must not be renamed to fit the convention.
EXTERNAL = {
    "FILE", "_Bigint", "_glue", "_atexit", "__sbuf", "__siov", "__suio",
    "sceGsDispEnv", "sceSifDmaData", "sceDmaTag", "sceGifTag",
}


def sources():
    return sorted(list((ROOT / "src").rglob("*.c")) + list((ROOT / "include").rglob("*.h")))


def scan():
    """Every drift entry in the tree: {"type <name>"} and {"member <type>.<name>"}.

    A type defined in several units (a shared header inlined per translation unit)
    is one entry, not one per copy."""
    found = {}
    for path in sources():
        rel = path.relative_to(ROOT).as_posix()
        for _, name, members, line in structs(path.read_text(errors="replace")):
            if not name or name in EXTERNAL:
                continue
            if INVENTED.match(name) and not CONVENTION.search(name):
                found.setdefault(f"type {name}", f"{rel}:{line}")
            for member in members:
                if UNNAMED.match(member):
                    found.setdefault(f"member {name}.{member}", f"{rel}:{line}")
    return found


def load_baseline():
    if not BASELINE.exists():
        return set()
    return {line.split("#")[0].strip()
            for line in BASELINE.read_text().splitlines()
            if line.strip() and not line.startswith("#")} - {""}


def write_baseline(found):
    types = sorted(k for k in found if k.startswith("type "))
    members = sorted(k for k in found if k.startswith("member "))
    lines = [
        "# Invented type and member names that already existed when the drift check",
        "# landed, from tools/check_drift.py --update. The check fails on a name that",
        "# is NOT here, so this file only ever shrinks: fix one, then --update.",
        f"# {len(types)} types, {len(members)} members.",
        "",
    ]
    lines += types + [""] + members
    BASELINE.write_text("\n".join(lines) + "\n")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--update", action="store_true", help="rewrite the baseline")
    ap.add_argument("--quiet", action="store_true")
    args = ap.parse_args()

    found = scan()
    if args.update:
        write_baseline(found)
        print(f"baseline rewritten: {len(found)} entries")
        return 0

    baseline = load_baseline()
    new = sorted(set(found) - baseline)
    fixed = sorted(baseline - set(found))

    if not args.quiet:
        types = sum(1 for k in found if k.startswith("type "))
        print(f"{types} invented type names, {len(found) - types} unnamed members "
              f"({len(baseline)} in the baseline)")
        if fixed:
            print(f"{len(fixed)} baseline entries are now clean; "
                  f"run --update to tighten the baseline")

    if new:
        print(f"\n{len(new)} name(s) not in the baseline:")
        for key in new:
            print(f"  {key}   {found[key]}")
        print("\nLook the name up before inventing one:  python3 tools/hd_names.py <name>")
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
