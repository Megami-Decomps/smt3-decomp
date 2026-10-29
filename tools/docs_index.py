#!/usr/bin/env python3
"""Rebuild the searchable indexes for the levers and draft pages.

    python3 tools/docs_index.py            # rewrite both INDEX.tsv files
    python3 tools/docs_index.py --check    # fail if either is stale

docs/levers/ and docs/nonmatching/ are too long to read end to end, so each
carries an INDEX.tsv: one row per lever or draft, with the mnemonics it
concerns and the page and line to open. Grep the index, then read the range.
"""
import argparse
import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent
LEVERS = ROOT / "docs/levers"
DRAFTS = ROOT / "docs/nonmatching"

# Mnemonics and register names worth searching for. A lever is usually looked
# up by the instruction retail shows, so those are the index's keywords.
MNEMONIC = re.compile(
    r"\b(b(?:eq|ne|gez|gtz|lez|ltz)z?l?|j|jal|jr|jalr"
    r"|l[bhwd]u?|s[bhwd]|l[wd]l|l[wd]r|s[wd]l|s[wd]r|lui|li"
    r"|addi?u?|subu?|mult1?|multu|div1?|divu|move|movz|movn"
    r"|slt|slti|sltu|sltiu|and|andi|x?or|x?ori|nor|sll|srl|sra|sllv|srlv|srav"
    r"|dsll32|dsra32|daddu|dsubu|seq|sne"
    r"|abs\.s|add\.s|sub\.s|mul\.s|div\.s|neg\.s|mov\.s|c\.(?:eq|lt|le)\.s"
    r"|cvt\.[sw]\.[sw]|mtc1|mfc1|lwc1|swc1|trunc\.w\.s"
    r"|lqc2|sqc2|qmtc2|qmfc2|cfc2|ctc2|v(?:add|sub|mul|madd|msub|move|iadd"
    r"|itof0|ftoi0|ftoi4|rsqrt|sqrt|waitq|nop|mulq|maddq|clipw)[a-z.]*"
    r"|plzcw|pext[lu]?[bhw]|ppac[bhw5]|pextlb|pextlh|nop)\b")
REGISTER = re.compile(r"\$(?:[0-9]{1,2}|f[0-9]{1,2}|gp|sp|ra|at|vf[0-9]{1,2}|Q)\b")


CODE_SPAN = re.compile(r"`([^`]+)`")


def keywords(text: str) -> str:
    """The mnemonics and registers an entry mentions, deduped and sorted.

    Only code spans are scanned. Prose shares words with the instruction set
    ("and", "or", "move", "div"), so reading the whole entry fills the index
    with noise that then matches every grep.
    """
    found = set()
    for span in CODE_SPAN.findall(text):
        for pat in (MNEMONIC, REGISTER):
            found.update(m.group(0) for m in pat.finditer(span))
    return " ".join(sorted(found))


def one_line(text: str, limit: int = 160) -> str:
    """The entry's first sentence, flattened to fit one TSV cell."""
    body = re.sub(r"\s+", " ", re.sub(r"`", "", text)).strip()
    body = body.lstrip("- ").strip()
    cut = re.split(r"(?<=[.;:]) ", body)[0]
    if len(cut) > limit:
        cut = cut[: limit - 1].rsplit(" ", 1)[0] + "…"
    return cut


def lever_rows() -> list[tuple[str, ...]]:
    rows = []
    for page in sorted(LEVERS.glob("*.md")):
        lines = page.read_text().split("\n")
        # An entry is a table row, a top-level bullet, or an H3; its body runs
        # to the next one of those.
        starts = [
            i for i, l in enumerate(lines)
            if l.startswith("- ") or l.startswith("### ")
            or (l.startswith("| ") and not re.match(r"^\|[\s:-]+\|", l)
                and not l.startswith("| Retail shows"))
        ]
        for n, i in enumerate(starts):
            end = starts[n + 1] if n + 1 < len(starts) else len(lines)
            body = "\n".join(lines[i:end])
            rows.append((
                page.relative_to(ROOT).as_posix(),
                str(i + 1),
                keywords(body),
                one_line(body),
            ))
    return rows


def draft_rows() -> list[tuple[str, ...]]:
    rows = []
    for page in sorted(DRAFTS.rglob("*.md")):
        lines = page.read_text().split("\n")
        heads = [i for i, l in enumerate(lines) if l.startswith("## ")]
        for n, i in enumerate(heads):
            end = heads[n + 1] if n + 1 < len(heads) else len(lines)
            # One entry may name several siblings sharing a residual:
            # "## a, b and c (unit, 0x10 each)". The unit is the first field of
            # the parenthetical, however many names precede it.
            m = re.match(r"^## (.+?)(?: \(([^,]+), ([^)]+)\))?$", lines[i])
            if not m:
                continue
            body = "\n".join(lines[i + 1:end])
            # "N of M words match/differ" is the convention. The bare "N of M
            # words" is ambiguous and the preamble forbids it, so capture it as
            # written: a cell with no verb is the lint.
            d = (re.search(r"\b([\w-]+ of [\w-]+ words?[^.;]{0,40}?"
                           r"(?:differ|match)\w*)", body)
                 or re.search(r"\b([\w-]+ of [\w-]+ words?)\b", body))
            rows.append((
                m.group(1),
                m.group(2) or "",
                m.group(3) or "",
                re.sub(r"\s+", " ", d.group(1)) if d else "",
                page.relative_to(ROOT).as_posix(),
                str(i + 1),
            ))
    return rows


def lever_readme() -> str:
    """A browsable list of the section pages, longest first."""
    rows = []
    for page in sorted(LEVERS.glob("*.md")):
        if page.name == "README.md":
            continue
        lines = page.read_text().split("\n")
        title = lines[0].lstrip("# ").strip() if lines else page.stem
        n = sum(1 for r in lever_rows() if r[0].endswith("/" + page.name))
        rows.append((n, page.name, title))
    rows.sort(reverse=True)
    body = ["# Matching levers by section", "",
            "Generated by `tools/docs_index.py`. `INDEX.tsv` beside this file is",
            "the searchable form: grep it for the mnemonic you are looking at and",
            "open the page and line it names.", "",
            "| Levers | Page | Section |", "|---|---|---|"]
    body += [f"| {n} | [{name}]({name}) | {title} |" for n, name, title in rows]
    return "\n".join(body) + "\n"


def draft_readme() -> str:
    """A browsable list of the unit pages, most drafts first."""
    counts = {}
    for _fn, unit, _s, _d, page, _l in draft_rows():
        counts.setdefault(page, [unit, 0])[1] += 1
    rows = sorted(((n, page, unit) for page, (unit, n) in counts.items()),
                  reverse=True)
    body = ["# Non-matching drafts by unit", "",
            "Generated by `tools/docs_index.py`. `INDEX.tsv` beside this file is",
            "the searchable form: one row per draft with its distance and the",
            "page and line to open. Conventions are in",
            "[../nonmatching.md](../nonmatching.md).", "",
            "| Drafts | Unit | Page |", "|---|---|---|"]
    for n, page, unit in rows:
        rel = page.split("docs/nonmatching/", 1)[1]
        body.append(f"| {n} | {unit} | [{rel}]({rel}) |")
    return "\n".join(body) + "\n"


PAGES = {
    LEVERS / "README.md": lever_readme,
    DRAFTS / "README.md": draft_readme,
}

TABLES = {
    LEVERS / "INDEX.tsv": ("page\tline\tmnemonics\ttell", lever_rows),
    DRAFTS / "INDEX.tsv": ("function\tunit\tsize\tdistance\tpage\tline", draft_rows),
}


def render(header: str, rows: list[tuple[str, ...]]) -> str:
    return "\n".join([header] + ["\t".join(r) for r in rows]) + "\n"


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--check", action="store_true",
                    help="exit non-zero if an index is stale")
    args = ap.parse_args()
    stale = 0
    for path, build_page in PAGES.items():
        want = build_page()
        have = path.read_text() if path.exists() else ""
        if args.check:
            if want != have:
                print(f"{path.relative_to(ROOT)} is stale; run tools/docs_index.py")
                stale += 1
        elif want != have:
            path.write_text(want)
            print(f"{path.relative_to(ROOT)}: rewritten")
    for path, (header, build) in TABLES.items():
        want = render(header, build())
        have = path.read_text() if path.exists() else ""
        if args.check:
            if want != have:
                print(f"{path.relative_to(ROOT)} is stale; run tools/docs_index.py")
                stale += 1
        elif want != have:
            path.write_text(want)
            print(f"{path.relative_to(ROOT)}: {want.count(chr(10)) - 1} rows")
        else:
            print(f"{path.relative_to(ROOT)}: unchanged")
    vague = [r for r in draft_rows()
             if r[3] and "match" not in r[3] and "differ" not in r[3]]
    if vague:
        print(f"{len(vague)} draft(s) write the ambiguous bare \"N of M words\"; "
              "the convention is to say match or differ")
    return 1 if stale else 0


if __name__ == "__main__":
    sys.exit(main())
