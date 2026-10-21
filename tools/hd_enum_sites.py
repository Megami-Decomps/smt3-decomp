#!/usr/bin/env python3
"""Find switches whose case values match exactly one HD enum.

A literal is safe to replace only where the code shows which enum it belongs to, and
a switch is that evidence. A case set fitting several enums, or a 0..3 run that fits
dozens, proves nothing and is reported as ambiguous."""

import argparse
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
ENUMS = ROOT / "config" / "smt3" / "hd_enums.txt"

SWITCH = re.compile(r"\bswitch\s*\(([^)]*)\)\s*\{", re.S)
CASE = re.compile(r"\bcase\s+(-?(?:0[xX])?[0-9A-Fa-f]+)\s*:")
MIN_CASES = 4


def load_enums():
    out, cur = [], None
    for line in ENUMS.read_text().splitlines():
        if line.startswith("E "):
            p = line.split()
            cur = (p[1], p[2], [], len(p) > 4 and p[4] == "merged")
            out.append(cur)
        elif line.startswith("V ") and cur:
            p = line.split(None, 2)
            cur[2].append((p[2], int(p[1])))
    return out


def switch_bodies(text):
    """[(selector, [case values], line)] for each switch, to its closing brace."""
    out = []
    for m in SWITCH.finditer(text):
        depth, i = 1, m.end()
        while i < len(text) and depth:
            depth += (text[i] == "{") - (text[i] == "}")
            i += 1
        body = text[m.end():i]
        values = []
        for c in CASE.finditer(body):
            try:
                values.append(int(c.group(1), 0))
            except ValueError:
                pass
        if values:
            out.append((m.group(1).strip(), values, text[:m.start()].count("\n") + 1))
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--min-cases", type=int, default=MIN_CASES)
    ap.add_argument("--all", action="store_true")
    args = ap.parse_args()

    enums = load_enums()
    sure, unsure, none = [], [], 0
    for path in sorted((ROOT / "src").rglob("*.c")):
        text = path.read_text(errors="replace")
        for selector, values, line in switch_bodies(text):
            uniq = set(values)
            if len(uniq) < args.min_cases:
                continue
            # The selector's own name is what says which enum this is. Values alone
            # do not: `switch (button)` fits DDS3_PATHBASIC_FLAG perfectly and means
            # nothing, because a four-value set sits inside dozens of enums.
            words = set(re.findall(r"[A-Za-z_]\w*", selector.lower()))
            hits = []
            for name, src, members, merged in enums:
                if merged:
                    continue
                vals = {v for _, v in members}
                if not uniq <= vals:
                    continue
                if len(uniq) < max(args.min_cases, len(vals) * 0.5):
                    continue
                parts = {w for w in name.lower().split("_") if len(w) > 2}
                if not (parts & words):
                    continue
                hits.append((name, src, len(vals)))
            rel = path.relative_to(ROOT).as_posix()
            if len(hits) == 1:
                sure.append((rel, line, selector, sorted(uniq), hits[0]))
            elif hits:
                unsure.append((rel, line, selector, sorted(uniq), hits))
            else:
                none += 1

    print(f"{len(sure)} switch(es) whose cases fit exactly one HD enum:")
    for rel, line, sel, vals, (name, src, total) in sure:
        print(f"    {rel}:{line}  switch ({sel[:40]})")
        print(f"        {len(vals)} cases of {name} ({total} members, {src})")
    print(f"\n{len(unsure)} fit several:")
    for rel, line, sel, vals, hits in (unsure if args.all else unsure[:8]):
        print(f"    {rel}:{line}  {len(vals)} cases  ?  "
              f"{', '.join(n for n, _, _ in hits[:4])}")
    print(f"\n{none} fit none")
    return 0


if __name__ == "__main__":
    sys.exit(main())
