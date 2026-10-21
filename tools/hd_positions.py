#!/usr/bin/env python3
"""Name an invented type from the same position of the same function in SMT3HD.

Position means something once the arity agrees, as it does for 5463 of our functions:
parameter i of ours is then parameter i of HD's, so the type HD names there is ours.
Two agreeing positions is the bar; one is reported."""

import argparse
import re
import sys
from collections import defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from cparse import struct_fields  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
DEFAULT_HD = Path("/home/wsl/projects/smt3hd-reconstructed-sources")

SIG = re.compile(r"^([A-Za-z_][\w \t\*]*?)\b(\w+)\s*\(([^;{]*)\)\s*\{", re.M)
CONVENTION = re.compile(r"_(?:t\d?|u|e|b|s|tag\d?)$")
INVENTED = re.compile(r"^[A-Z][a-z0-9]")
# il2cpp leaves these behind on its own types; they are not names to adopt.
ARTIFACT = re.compile(r"(_o|_array|_StaticFields|_c|_Fields)$")


def split_params(text):
    text = text.strip()
    if not text or text == "void":
        return []
    out, depth, cur = [], 0, ""
    for ch in text:
        if ch == "," and depth == 0:
            out.append(cur)
            cur = ""
            continue
        depth += (ch == "(") - (ch == ")")
        cur += ch
    out.append(cur)
    return out


def signatures(paths):
    out = {}
    for path in paths:
        for m in SIG.finditer(path.read_text(errors="replace")):
            name = m.group(2)
            if name not in out:
                out[name] = (m.group(1).strip(), split_params(m.group(3)), path)
    return out


def proposals(hd):
    """invented type -> {candidate: {"function:position"}}, over same-arity functions."""
    ours = signatures(sorted((ROOT / "src").rglob("*.c")))
    theirs = signatures(sorted((hd / "src").glob("*.c")))
    pairs = defaultdict(lambda: defaultdict(set))
    for func in sorted(set(ours) & set(theirs)):
        oret, oparams, _ = ours[func]
        hret, hparams, _ = theirs[func]
        if len(oparams) != len(hparams):
            continue
        slots = [("return", oret, hret)]
        slots += [(f"p{i}", o, h) for i, (o, h) in enumerate(zip(oparams, hparams))]
        for where, mine, hd_text in slots:
            onames = [w for w in re.findall(r"[A-Za-z_]\w*", mine)
                      if INVENTED.match(w) and not CONVENTION.search(w)]
            hnames = [w for w in re.findall(r"[A-Za-z_]\w*", hd_text)
                      if CONVENTION.search(w) and not ARTIFACT.search(w)]
            # One type on each side, or the position does not say which maps to which.
            if len(onames) == 1 and len(hnames) == 1:
                pairs[onames[0]][hnames[0]].add(f"{func}:{where}")
    return pairs


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--hd", type=Path, default=DEFAULT_HD)
    ap.add_argument("--min-positions", type=int, default=2)
    ap.add_argument("--apply", action="store_true")
    args = ap.parse_args()

    taken = set()
    for path in sorted(list((ROOT / "src").rglob("*.c"))
                       + list((ROOT / "include").rglob("*.h"))):
        for _, name, _, _ in struct_fields(path.read_text(errors="replace")):
            if name:
                taken.add(name)

    pairs = proposals(args.hd)
    forced, split, weak = [], [], []
    for name, cands in sorted(pairs.items()):
        if len(cands) > 1:
            split.append((name, cands))
            continue
        target, where = next(iter(cands.items()))
        if target in taken:
            continue  # the tree already uses that name for something
        if len(where) >= args.min_positions:
            forced.append((name, target, sorted(where)))
        else:
            weak.append((name, target, sorted(where)))

    # Two of our types cannot be one IDB type.
    claims = defaultdict(list)
    for name, target, _ in forced:
        claims[target].append(name)
    contested = {t for t, who in claims.items() if len(who) > 1}
    if contested:
        print(f"{len(contested)} contested target(s), dropped:")
        for t in sorted(contested):
            print(f"    {t:<34} <- {', '.join(sorted(claims[t]))}")
        print()
    forced = [row for row in forced if row[1] not in contested]

    # A positional match says which type the function handles; it cannot say our
    # struct is the same record. Where ours is bigger than the IDB's, something is
    # wrong with one of them, so the name is reported rather than taken.
    import ida_members
    from ida_fill import ida_sizes, layout, project_sizes

    sizes = project_sizes(ida_sizes())
    idb = ida_members.load(ROOT / "config" / "smt3" / "ida_members.txt")
    fields_of = {}
    for path in sorted(list((ROOT / "src").rglob("*.c"))
                       + list((ROOT / "include").rglob("*.h"))):
        for _, name, fields, _ in struct_fields(path.read_text(errors="replace")):
            if name and name not in fields_of:
                fields_of[name] = fields

    def corroborates(name, target):
        """None if there is nothing to check, else whether our struct and the IDB's
        look like the same record.

        Size alone is the wrong test. PcpScatterInstanceA is 0x18c against
        effPCPWhirlwindPoly_tag's 0x7c purely because the IDB holds its param by
        pointer and its part inline where ours is the reverse; both still have the
        same seven members in the same roles, and one member type already agrees
        exactly. So a size that fits counts, and so does an equal member count with
        a member type in common."""
        if target not in idb or name not in fields_of:
            return None
        ours_fields = fields_of[name]
        _, isize, imembers = idb[target]
        rows = layout(ours_fields, sizes)
        if rows is not None:
            total = (rows[-1][0] + rows[-1][1]) if rows else 0
            if total <= isize:
                return True
        # Member count in order is the signal, not size: pointer-against-inline is a
        # layout disagreement, not a different record.
        return len(ours_fields) == len(imembers)

    def contained_types(fields):
        out = set()
        for text, _, _, _ in fields:
            for word in re.findall(r"[A-Za-z_]\w*", text):
                if CONVENTION.search(word):
                    out.add(word)
        return out

    def containment_ok(name, target):
        """Does our struct hold the types the IDB says this one holds?

        A work struct holds its own family's part type, and that settles a case the
        shape cannot: effPCPWwSpherePolyPart_tag and effPCPHamaCardSpherePart_tag are
        both 32 bytes. Two renames went to the wrong struct of a pair before this
        check existed, each holding a record the IDB assigns to the other."""
        if target not in idb or name not in fields_of:
            return None
        theirs = {re.sub(r"[\s*\[\]0-9]+", "", t) for _, _, t, _ in idb[target][2]}
        theirs = {t for t in theirs if CONVENTION.search(t)}
        mine = contained_types(fields_of[name])
        parts = {t for t in theirs if t.endswith(("Part_tag", "Part_t"))}
        if not parts:
            return None
        # Where the IDB says this type embeds a part record, ours must not instead
        # embed a different family's part.
        ours_parts = {t for t in mine if t.endswith(("Part_tag", "Part_t"))}
        if ours_parts:
            return bool(ours_parts & parts)
        # Our particle may not be named yet, but its size still has to agree:
        # PcpScatterPlainInstance holds a 40-byte record where effPCPWwSpherePoly_tag
        # embeds a 32-byte part, so it is a different family whatever it is called.
        want = {idb[p][1] for p in parts if p in idb}
        held = set()
        for text, stars, _, _ in fields_of[name]:
            base = re.sub(r"\b(struct|union|const)\b", " ", text).strip()
            if base in sizes and (stars or base not in idb):
                held.add(sizes[base])
        if not want or not held:
            return None
        return bool(want & held)

    bad = [row for row in forced
           if corroborates(row[0], row[1]) is False
           or containment_ok(row[0], row[1]) is False]
    forced = [row for row in forced if row not in bad]
    oversize = bad

    print(f"{len(forced)} invented type(s) named by {args.min_positions}+ agreeing "
          f"positions, with a size the IDB can hold:")
    for name, target, where in forced:
        print(f"    {name:<30} -> {target:<34} {len(where)} fn: "
              f"{', '.join(where[:3])}")
    if oversize:
        print(f"\n{len(oversize)} more are named by position but our struct is larger "
              f"than the IDB's, so they are not taken:")
        for name, target, where in oversize:
            print(f"    {name:<30} ?  {target:<34} {len(where)} fn")
    print(f"\n{len(split)} get conflicting candidates, so none is taken:")
    for name, cands in split[:8]:
        print(f"    {name:<30} ?  {', '.join(sorted(cands)[:4])}")
    print(f"\n{len(weak)} rest on a single position")

    if args.apply:
        done = 0
        for name, target, _ in forced:
            touched = 0
            for path in sorted(list((ROOT / "src").rglob("*.c"))
                               + list((ROOT / "include").rglob("*.h"))):
                text = path.read_text()
                swapped, n = re.subn(rf"\b{re.escape(name)}\b", target, text)
                if n:
                    path.write_text(swapped)
                    touched += 1
            print(f"  {name} -> {target}   {touched} file(s)")
            done += 1
        print(f"\n{done} renamed; relink and verify the checksum")
    return 0


if __name__ == "__main__":
    sys.exit(main())
