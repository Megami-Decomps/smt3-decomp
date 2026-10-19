#!/usr/bin/env python3
"""Report type names in the tree that disagree with the IDB or the conventions.

Reads config/smt3/ida_types.txt (see tools/ida_types.py) and every typedef in
src/ and include/. Prints counts and the worst offenders; a report, not a test.
"""

import argparse
import re
import sys
from collections import Counter, defaultdict
from difflib import get_close_matches
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
DEFAULT_TYPES = ROOT / "config" / "smt3" / "ida_types.txt"

# `} name;` closing a typedef. Two spellings, and the anchors matter: an indented
# `} name;` is a nested struct member, not a type, so the multi-line form needs the
# brace at column 0 and the one-line form needs its own `typedef` keyword.
TYPEDEF_END = re.compile(r"^\}\s*([A-Za-z_]\w*)\s*;", re.M)
TYPEDEF_ONE_LINE = re.compile(r"^\s*typedef\s+(?:struct|union|enum)\b.*\}\s*([A-Za-z_]\w*)\s*;", re.M)

# docs/CONTRIBUTING.md: struct types take a _t suffix. _u and _e are the repo's
# spellings for a union and an enum; _tag and _s are IDA's, kept where recovered.
CONFORMING = re.compile(r"_[tue]$")

# Names that are correct despite breaking the convention, with why. newlib and the
# Sony SDK own their spellings; the rest are the IDB's own and must not be "fixed".
EXTERNAL = {
    "FILE": "newlib",
    "_Bigint": "newlib",
    "_glue": "newlib",
    "_atexit": "newlib",
    "__sbuf": "newlib",
    "__siov": "newlib",
    "__suio": "newlib",
    "sceGsDispEnv": "Sony SDK",
}

# A tree name and an IDB name can differ only in the suffix spelling and still be
# the same type, so compare stems. The leading `header_` qualifier is IDA's, from
# a type declared inside a translation unit.
QUALIFIER = re.compile(r"^[a-zA-Z0-9]+_(?=[a-z]{2,4}[A-Z])")
SUFFIX = re.compile(r"(_t\d?|_tag\d?|_s|_u|_e|_b)$")

NEAR_CUTOFF = 0.82


def stem(name):
    return SUFFIX.sub("", QUALIFIER.sub("", name)).lower()


def load_idb(path):
    """Return [(name, kind)] from an ida_types.txt."""
    rows = []
    for line in Path(path).read_text().splitlines():
        if line and not line.startswith("#"):
            parts = line.split()
            rows.append((parts[0], parts[1]))
    return rows


def tree_typedefs(root):
    """Map each typedef name to the files that define it."""
    where = defaultdict(list)
    sources = sorted(
        list((root / "src").rglob("*.c"))
        + list((root / "include").rglob("*.h"))
        + list((root / "src").rglob("*.h"))
    )
    for path in sources:
        text = path.read_text(errors="replace")
        for pattern in (TYPEDEF_END, TYPEDEF_ONE_LINE):
            for name in pattern.findall(text):
                rel = str(path.relative_to(root))
                if rel not in where[name]:
                    where[name].append(rel)
    return where


def count_sites(root, names):
    """How many times each name appears across src/ and include/."""
    if not names:
        return {}
    wanted = set(names)
    hits = Counter()
    word = re.compile(r"\b(" + "|".join(re.escape(n) for n in sorted(wanted)) + r")\b")
    for path in list((root / "src").rglob("*.c")) + list((root / "include").rglob("*.h")):
        for match in word.finditer(path.read_text(errors="replace")):
            hits[match.group(1)] += 1
    return hits


def classify(tree, idb):
    exact = {name for name, _ in idb}
    # An IDB enum tag is a poor suggestion for a record name, so suggest one only
    # for an all-caps tree name. Anything else would read as a layout claim.
    by_stem, enum_stems = {}, {}
    for name, kind in idb:
        target = enum_stems if kind == "enum" else by_stem
        target.setdefault(stem(name), name)

    recovered, external, invented, near, unmatched, resuffixed = [], [], [], [], [], []
    for name in sorted(tree):
        if name in exact:
            recovered.append(name)
            continue
        if name in EXTERNAL:
            external.append(name)
            continue
        pool = dict(by_stem)
        if name.isupper():
            pool.update(enum_stems)
        key = stem(name)
        if key in pool:
            target = pool[key]
            # The stems agree and the IDB spelling is IDA's `_tag`/`_s`: the tree is
            # the same type under the repo's own `_t`, which is the convention, not drift.
            if SUFFIX.sub("", name) == SUFFIX.sub("", target):
                resuffixed.append((name, target))
            else:
                near.append((name, target))
            continue
        close = get_close_matches(key, list(pool), n=1, cutoff=NEAR_CUTOFF)
        if close:
            near.append((name, pool[close[0]]))
        elif CONFORMING.search(name):
            unmatched.append(name)
        else:
            invented.append(name)
    return recovered, external, resuffixed, invented, near, unmatched


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--types", type=Path, default=DEFAULT_TYPES)
    parser.add_argument("--root", type=Path, default=ROOT)
    parser.add_argument("--top", type=int, default=15, help="rows to print per section")
    args = parser.parse_args()

    if not args.types.exists():
        raise SystemExit(f"{args.types} missing; run tools/ida_types.py with the IDB open")

    idb = load_idb(args.types)
    tree = tree_typedefs(args.root)
    recovered, external, resuffixed, invented, near, unmatched = classify(tree, idb)

    total = len(tree)
    print(f"{total} typedefs in the tree, {len(idb)} named types in the IDB")
    print(f"  {len(recovered):4d} match an IDB name exactly")
    print(f"  {len(resuffixed):4d} match but for the suffix, which is the convention")
    print(f"  {len(external):4d} external names, correct as they are")
    print(f"  {len(near):4d} differ from a close IDB name (candidate drift)")
    print(f"  {len(invented):4d} break the _t convention with no IDB counterpart")
    print(f"  {len(unmatched):4d} conform but the IDB never typed them")

    sites = count_sites(args.root, [n for n, _ in near] + invented)

    print(f"\ncandidate drift, widest first (showing {args.top}):")
    for name, target in sorted(near, key=lambda pair: -sites.get(pair[0], 0))[: args.top]:
        print(f"  {name:38s} -> {target:40s} {sites.get(name, 0)} sites")
    print("  a close name is a lead, not proof: confirm the layout before renaming")

    print(f"\nno IDB counterpart and off-convention, widest first (showing {args.top}):")
    for name in sorted(invented, key=lambda n: -sites.get(n, 0))[: args.top]:
        files = tree[name]
        dup = f", {len(files)} copies" if len(files) > 1 else ""
        print(f"  {name:38s} {sites.get(name, 0)} sites{dup}")

    dups = {name: files for name, files in tree.items() if len(files) > 1}
    print(f"\n{len(dups)} types defined in more than one file, widest first:")
    for name, files in sorted(dups.items(), key=lambda pair: -len(pair[1]))[: args.top]:
        print(f"  {name:38s} {len(files)} copies")
    return 0


if __name__ == "__main__":
    sys.exit(main())
