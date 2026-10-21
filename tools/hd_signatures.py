#!/usr/bin/env python3
"""Name an invented type from the HD signatures of the functions that use it.

Our function and HD's share a name, so the types in HD's signature are the ones ours
handles. One signature proves nothing, arity and pointer depth both differing; a type
agreed across the whole family of functions using ours is the lead worth reading."""

import argparse
import re
import sys
from collections import Counter, defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from cparse import struct_fields  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
DEFAULT_HD = Path("/home/wsl/projects/smt3hd-reconstructed-sources")

CONVENTION = re.compile(r"_(?:t\d?|u|e|b|s|tag\d?)$")
INVENTED = re.compile(r"^[A-Z][a-z0-9]")
SIG = re.compile(r"^([A-Za-z_][\w \t\*]*?)\b(\w+)\s*\(([^;{]*)\)\s*\{", re.M)

# Types that say nothing about identity: every signature is full of them.
GENERIC = {
    "void", "s8", "u8", "s16", "u16", "s32", "u32", "s64", "u64", "f32", "f64",
    "bool", "char", "int", "short", "long", "float", "double", "unsigned", "signed",
    "const", "struct", "union", "enum", "sdfVector4_t", "sdfVector3_t", "sdfVector2_t",
    "sdfMatrix4_t", "sdfColor_t", "sdfMemHandle_t",
}


def declarator_type(decl):
    """The type a declarator names, dropping the parameter's own name.

    `effPCPRingPolyExp2_tag **pPCPRingPolyExp2` is the type plus a name, and taking
    every identifier makes the name `hData` out-vote every real type in the file."""
    decl = re.sub(r"\[[^\]]*\]", " ", decl).strip()
    if not decl:
        return []
    words = re.findall(r"[A-Za-z_]\w*", decl)
    if not words:
        return []
    # A bare `void *` or `s32` has no name to drop; one with a name ends in it, and a
    # pointer declarator always does.
    if len(words) > 1 and (("*" in decl) or words[-1].islower() or words[-1][0].islower()):
        words = words[:-1]
    return [w for w in words if w not in GENERIC]


def signature_types(ret, params):
    out = []
    out += [w for w in re.findall(r"[A-Za-z_]\w*", ret) if w not in GENERIC]
    for part in params.split(","):
        out += declarator_type(part)
    return out


def invented_types():
    out = {}
    for p in sorted(list((ROOT / "src").rglob("*.c")) + list((ROOT / "include").rglob("*.h"))):
        for _, name, _, line in struct_fields(p.read_text(errors="replace")):
            if name and INVENTED.match(name) and not CONVENTION.search(name) \
                    and name not in out:
                out[name] = (p.relative_to(ROOT).as_posix(), line)
    return out


def hd_signatures(hd):
    out = {}
    for p in sorted((hd / "src").glob("*.c")):
        for m in SIG.finditer(p.read_text(errors="replace")):
            out.setdefault(m.group(2), (m.group(1).strip(), m.group(3), p.name))
    return out


def users(invented):
    """invented type -> {function names whose signature mentions it}."""
    out = defaultdict(set)
    names = set(invented)
    for p in sorted((ROOT / "src").rglob("*.c")):
        for m in SIG.finditer(p.read_text(errors="replace")):
            whole = f"{m.group(1)} {m.group(3)}"
            for name in names:
                if re.search(rf"\b{re.escape(name)}\b", whole):
                    out[name].add(m.group(2))
    return out


def layout_check(rows):
    """Does our struct have the same size and member places as the voted IDB type?

    Two independent directions agreeing, the function family and the layout, is what
    turns a lead into something worth applying."""
    import ida_members
    from ida_fill import ida_sizes, layout

    sizes = ida_sizes()
    idb = ida_members.load(ROOT / "config" / "smt3" / "ida_members.txt")
    ours = {}
    for p in sorted(list((ROOT / "src").rglob("*.c")) + list((ROOT / "include").rglob("*.h"))):
        for _, name, fields, _ in struct_fields(p.read_text(errors="replace")):
            if name and name not in ours:
                ours[name] = fields

    out = {}
    for name, top, *_ in rows:
        if top not in idb or name not in ours:
            out[name] = "no IDB layout to compare"
            continue
        mine = layout(ours[name], sizes)
        if mine is None:
            out[name] = "we cannot size our own struct"
            continue
        _, isize, imembers = idb[top]
        total = (mine[-1][0] + mine[-1][1]) if mine else 0
        theirs = [(o, s) for o, s, _, _ in imembers]
        if [(o, s) for o, s, _ in mine] == theirs and total <= isize:
            out[name] = "layout agrees exactly"
        elif total == isize:
            out[name] = f"same size {isize:#x}, members differ"
        else:
            out[name] = f"size {total:#x} vs {isize:#x}"
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--hd", type=Path, default=DEFAULT_HD)
    ap.add_argument("--min-votes", type=int, default=3,
                    help="functions that must agree (default 3)")
    ap.add_argument("--all", action="store_true")
    ap.add_argument("--check-layout", action="store_true",
                    help="corroborate each vote against the IDB layout")
    ap.add_argument("--apply", action="store_true",
                    help="rename only the votes the layout corroborates exactly")
    args = ap.parse_args()
    if args.apply:
        args.check_layout = True

    invented = invented_types()
    hd = hd_signatures(args.hd)
    used_by = users(invented)

    strong, weak, none = [], [], 0
    for name, (where, line) in sorted(invented.items()):
        funcs = sorted(used_by.get(name, ()))
        shared = [f for f in funcs if f in hd]
        if not shared:
            none += 1
            continue
        # A candidate only counts for a function whose own name carries its stem.
        # effPCPMisc.c holds dozens of effects, so without this the types of every
        # effect in the file vote for each other: effPCPBurstWork collected 24 votes
        # for effPCPEyeThunder_tag purely by sharing a file.
        votes = Counter()
        for f in shared:
            ret, params, _ = hd[f]
            for t in set(signature_types(ret, params)):
                stem = re.sub(r"_(?:tag\d?|t\d?|s|u|e)$", "", t)
                if stem and (f.startswith(stem) or stem.startswith(f)):
                    votes[t] += 1
        if not votes:
            none += 1
            continue
        top, count = votes.most_common(1)[0]
        runner = votes.most_common(2)[1][1] if len(votes) > 1 else 0
        row = (name, top, count, len(shared), runner, where, line)
        if count >= args.min_votes and count > runner:
            strong.append(row)
        else:
            weak.append(row)

    shapes = layout_check(strong) if args.check_layout else {}

    print(f"{len(strong)} invented type(s) with a type agreed by {args.min_votes}+ "
          f"of their functions, ahead of any other:")
    for name, top, count, total, runner, where, line in strong:
        note = f"   {shapes[name]}" if name in shapes else ""
        print(f"    {name:<30} ?= {top:<34} {count}/{total} funcs"
              f" (next {runner})  {where}:{line}{note}")
    if shapes:
        agree = sum(1 for v in shapes.values() if v.startswith("layout agrees"))
        print(f"\n{agree} of {len(strong)} are corroborated by the layout as well")

    if args.apply:
        done = 0
        for name, top, *_ in strong:
            if not shapes.get(name, "").startswith("layout agrees"):
                continue
            if any(re.search(rf"\b{re.escape(top)}\b", p.read_text(errors="replace"))
                   for p in (ROOT / "src").rglob("*.c")):
                print(f"  skip {name}: {top} already appears in the tree")
                continue
            touched = 0
            for p in sorted(list((ROOT / "src").rglob("*.c"))
                            + list((ROOT / "include").rglob("*.h"))):
                text = p.read_text()
                swapped, n = re.subn(rf"\b{re.escape(name)}\b", top, text)
                if n:
                    p.write_text(swapped)
                    touched += 1
            print(f"  {name} -> {top}   {touched} file(s)")
            done += 1
        print(f"\n{done} type(s) renamed; relink and verify the checksum")
    print(f"\n{len(weak)} with a weaker or tied vote:")
    for name, top, count, total, runner, where, line in (weak if args.all else weak[:10]):
        print(f"    {name:<30} ?  {top:<34} {count}/{total} (next {runner})")
    print(f"\n{none} are used by no function HD also defines")
    return 0


if __name__ == "__main__":
    sys.exit(main())
