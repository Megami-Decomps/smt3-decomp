#!/usr/bin/env python3
"""List per-tick counter steps in src/smt3, the candidates for SDF_FRAMES.

A candidate is a line stepping a struct member or a g-prefixed global by one
(++, --, += 1, -= 1, += 1.0f), outside a for header. It counts as converted when
the same file names that counter on a line using an SDF_FRAME macro. sdk/ is
Sony's and is skipped. Prints one line per site and a total per directory.

A step count misses most converted work, which is a duration scaled where it
is read, a per-tick step or a decay. So each total also counts the macro uses
by kind: duration (SDF_FRAMES, SDF_FRAMES_F), step and decay.
"""
import argparse
import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent / "src" / "smt3"
NAME = r"[A-Za-z_]\w*(?:\[[^\]]*\])?(?:(?:->|\.)[A-Za-z_]\w*(?:\[[^\]]*\])?)*"
STEP = re.compile(
    r"(?P<a>" + NAME + r")\s*(?:\+\+|--|[+-]=\s*1(?:\.0f?)?(?![\w.]))"
    r"|(?:\+\+|--)\s*(?P<b>" + NAME + r")"
)
MACRO = re.compile(r"SDF_FRAME")
USE = re.compile(r"\bSDF_(FRAMES_F|FRAMES|FRAME_STEP|FRAME_DECAY)\s*\(")
KIND = {"FRAMES": "duration", "FRAMES_F": "duration", "FRAME_STEP": "step", "FRAME_DECAY": "decay"}


def counter_of(expr):
    """The member or global name a step moves, or None for a local."""
    leaf = re.split(r"->|\.", expr)[-1]
    leaf = re.sub(r"\[.*", "", leaf)
    if re.search(r"->|\.", expr) or re.match(r"g[A-Z]|dds3|sdf|[a-z]+[A-Z]\w*(Cnt|Count|Timer|Time|Wait)$", leaf):
        return leaf
    return None


def uses(path):
    """Count of SDF_FRAME macro uses by kind, outside comments and directives."""
    text = re.sub(r"/\*.*?\*/|//[^\n]*", "", path.read_text(errors="replace"), flags=re.S)
    counts = {"duration": 0, "step": 0, "decay": 0}
    for line in text.splitlines():
        if line.lstrip().startswith("#"):
            continue
        for m in USE.finditer(line):
            counts[KIND[m.group(1)]] += 1
    return counts


def scan(path):
    lines = path.read_text(errors="replace").splitlines()
    scaled = {w for l in lines if MACRO.search(l) for w in re.findall(r"\w+", l)}
    for i, line in enumerate(lines):
        code = re.sub(r"/\*.*?\*/|//.*", "", line)
        if re.match(r"\s*for\s*\(", code) or code.startswith("#"):
            continue
        for m in STEP.finditer(code):
            name = counter_of(m.group("a") or m.group("b"))
            if name is None:
                continue
            yield i + 1, name, name in scaled, line.strip()


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--all", action="store_true", help="also list converted sites")
    ap.add_argument("dirs", nargs="*")
    args = ap.parse_args()
    totals = {}
    kinds = {}
    for path in sorted(ROOT.rglob("*.c")):
        rel = path.relative_to(ROOT)
        top = rel.parts[0] if len(rel.parts) > 1 else "."
        if top == "sdk" or (args.dirs and top not in args.dirs):
            continue
        k = kinds.setdefault(top, {"duration": 0, "step": 0, "decay": 0})
        for kind, n in uses(path).items():
            k[kind] += n
        totals.setdefault(top, [0, 0])
        for lineno, name, done, text in scan(path):
            t = totals.setdefault(top, [0, 0])
            t[1 if done else 0] += 1
            if args.all or not done:
                print(f"{'done' if done else 'open'} {rel}:{lineno} {name}: {text}")
    def fmt(k):
        return f"{k['duration']} durations, {k['step']} steps, {k['decay']} decays"
    for top, (open_, done) in sorted(totals.items(), key=lambda kv: -kv[1][0]):
        print(f"# {top}: {open_} open, {done} converted; uses: {fmt(kinds[top])}", file=sys.stderr)
    all_k = {kind: sum(k[kind] for k in kinds.values()) for kind in ("duration", "step", "decay")}
    print(f"# total: {sum(t[0] for t in totals.values())} open, "
          f"{sum(t[1] for t in totals.values())} converted; uses: {fmt(all_k)}", file=sys.stderr)


if __name__ == "__main__":
    main()
