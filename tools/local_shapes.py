#!/usr/bin/env python3
"""Give a coarse copy of a record the member list another copy already has.

A record declared in several units does not always carry the same reading. One
file names all 25 members of `datDevilFormat_t`; another declares `flag` and
`unk_04[0x38]` over the same 60 bytes. Those placeholders are not unknown, they
are just not written down here, so naming them needs no inference at all: copy
the richer member list over the poorer one.

The substitution is only made where it cannot change the layout:

* the two readings cover the same extent,
* every placeholder run of the poor copy is exactly tiled by members of the rich
  copy, so nothing moves,
* every named member of the poor copy sits at the same offset and width in the
  rich copy, and
* the file does not mention the placeholders being replaced, since a read of one
  would have to be rewritten and that is a reading, not a copy.

    python3 tools/local_shapes.py            # what it would do
    python3 tools/local_shapes.py --apply
"""

import argparse
import re
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from cparse import struct_fields  # noqa: E402
from ida_fill import ida_sizes, layout, project_sizes  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
UNNAMED = re.compile(r"^(?:unk(?:nown)?(?:[_0-9]|[0-9A-Fa-f]+$|$)|field_0x|dummy\d|unused\d)", re.I)


def shapes(sizes):
    """name -> {layout: [(path, kind, first line, member lines)]}."""
    out = {}
    for path in sorted(list((ROOT / "src").rglob("*.c")) + list((ROOT / "include").rglob("*.h"))):
        text = path.read_text(errors="replace")
        for kind, name, fields, line in struct_fields(text):
            if not name or kind == "union":
                continue
            rows = layout(fields, sizes)
            if rows:
                out.setdefault(name, {}).setdefault(tuple(rows), []).append((path, line))
    return out


def hidden_bytes(shape):
    return sum(s for _, s, m in shape if UNNAMED.match(m))


def tiles(poor, rich):
    """Does the rich reading explain every placeholder of the poor one?"""
    if poor[-1][0] + poor[-1][1] != rich[-1][0] + rich[-1][1]:
        return False
    byoff = {(o, s): m for o, s, m in rich}
    covered = {}
    for o, s, m in rich:
        covered.setdefault(o, []).append((o, s, m))
    for o, s, m in poor:
        if not UNNAMED.match(m):
            # a named member of ours has to be the same member over there
            if byoff.get((o, s)) != m:
                return False
            continue
        inside = [r for r in rich if o <= r[0] < o + s]
        if not inside or inside[0][0] != o:
            return False
        if inside[-1][0] + inside[-1][1] != o + s:
            return False
    return True


def body(text, start_line):
    """The source span of the struct whose header is at start_line (1-based)."""
    lines = text.split("\n")
    i = start_line - 1
    while i < len(lines) and "{" not in lines[i]:
        i += 1
    depth, j = 0, i
    while j < len(lines):
        depth += lines[j].count("{") - lines[j].count("}")
        if depth == 0 and j > i:
            break
        j += 1
    return i, j


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--apply", action="store_true")
    ap.add_argument("types", nargs="*")
    args = ap.parse_args()

    sizes = project_sizes(ida_sizes())
    found = shapes(sizes)
    plans, refused = [], []
    for name, by_shape in sorted(found.items()):
        if args.types and name not in args.types:
            continue
        if len(by_shape) < 2:
            continue
        ranked = sorted(by_shape.items(), key=lambda kv: hidden_bytes(kv[0]))
        rich, richwhere = ranked[0]
        for poor, places in ranked[1:]:
            if hidden_bytes(poor) <= hidden_bytes(rich):
                continue
            if not tiles(poor, rich):
                refused.append((name, "the readings do not tile"))
                continue
            gained = [m for _, _, m in poor if UNNAMED.match(m)]
            for path, line in places:
                text = path.read_text()
                i, j = body(text, line)
                used = [g for g in gained
                        if re.search(r"(?:->|\.)\s*" + re.escape(g) + r"\b", text)]
                if used:
                    refused.append((f"{name} in {path.name}", f"reads {', '.join(used[:3])}"))
                    continue
                rich_path, rich_line = richwhere[0]
                plans.append((name, path, i, j, rich_path, rich_line, len(gained),
                              sum(1 for _, _, m in rich if not UNNAMED.match(m))
                              - sum(1 for _, _, m in poor if not UNNAMED.match(m))))

    for name, path, i, j, rich_path, rich_line, n, gain in plans:
        print(f"{name:30s} {path.relative_to(ROOT)!s:42s} "
              f"+{gain} names, {n} placeholders gone, from {rich_path.name}")
    for what, why in refused:
        print(f"# refused {what}: {why}", file=sys.stderr)
    if not args.apply:
        print(f"# {len(plans)} substitutions available", file=sys.stderr)
        return

    # one file at a time, so a copy that will not compile can be put back
    done = 0
    for name, path, i, j, rich_path, rich_line, n, gain in plans:
        ri, rj = body(rich_path.read_text(), rich_line)
        members = rich_path.read_text().split("\n")[ri + 1:rj]
        lines = path.read_text().split("\n")
        before = "\n".join(lines)
        lines[i + 1:j] = members
        path.write_text("\n".join(lines))
        # plain `ninja`: a target given as an absolute path is not a target it
        # knows, and reading that refusal as a compile failure is how this
        # reported 0 of 18 while every substitution was in fact sound.
        if subprocess.run(["ninja"], cwd=ROOT, capture_output=True).returncode:
            path.write_text(before)
            print(f"# reverted {name} in {path.name}: it does not compile there",
                  file=sys.stderr)
            continue
        done += 1
    print(f"# {done} of {len(plans)} substitutions written", file=sys.stderr)


if __name__ == "__main__":
    main()
