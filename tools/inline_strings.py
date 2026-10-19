#!/usr/bin/env python3
"""Turn `extern char D_<addr>[]` string placeholders into C literals.

    python3 tools/inline_strings.py --list          # candidates per file
    python3 tools/inline_strings.py --apply <file>  # convert one file

Decodes each string from the hex the splitter emits, not the assembler's
quoting. Skips .sdata, which gcc cannot emit a literal into. Verify with
tools/check_unit.py per file and then the full link: see docs/naming-drift.md.
"""
import re
import glob
import sys

SDATA = 0x54E800


def scan_strings():
    """label -> bytes, for each dlabel whose whole body is one string.

    A record can begin with a name field, so a leading .asciz proves nothing:
    dds3UnitPartTable_t starts with one and is 360 bytes of table. Only a symbol
    holding exactly one .asciz between dlabel and enddlabel is a string.
    """
    out = {}
    for path in glob.glob("asm/**/*.s", recursive=True):
        with open(path, errors="replace") as handle:
            lines = handle.read().splitlines()
        label, raw, extra = None, None, 0
        for line in lines:
            start = re.match(r"\s*dlabel\s+(\S+)", line)
            if start:
                label, raw, extra = start.group(1), None, 0
                continue
            if label is None:
                continue
            if re.match(r"\s*enddlabel\s+" + re.escape(label), line):
                if raw is not None and not extra:
                    out[label] = raw
                label = None
                continue
            stripped = line.strip()
            # The byte dump is a comment-only line, so read it before stripping comments.
            hexline = re.fullmatch(r"/\*\s*([0-9A-Fa-f]+)\s*\*/", stripped)
            if hexline:
                if raw == b"":
                    raw = bytes.fromhex(hexline.group(1))
                continue
            body = re.sub(r"^/\*.*?\*/", "", stripped).strip()
            if body.startswith(".asci"):
                if raw is None:
                    raw = b""
                else:
                    extra += 1
            elif body.startswith(".align") or not body:
                continue
            elif body.startswith(("nonmatching", "matching", ".section", ".include")):
                continue
            elif body.startswith("."):
                extra += 1
    return out


ESCAPES = {0x0A: "\\n", 0x09: "\\t", 0x0D: "\\r", 0x22: '\\"', 0x5C: "\\\\"}


def c_literal(raw):
    """A C literal for the bytes before the first NUL, or None if not text.

    Float data can disassemble as .asciz when its bytes happen to be printable,
    so anything outside printable ASCII plus the known escapes is rejected.
    """
    if b"\x00" not in raw:
        return None
    body = raw.split(b"\x00")[0]
    if not body:
        return None
    out = []
    for byte in body:
        if byte in ESCAPES:
            out.append(ESCAPES[byte])
        elif 0x20 <= byte < 0x7F:
            out.append(chr(byte))
        else:
            return None
    return '"' + "".join(out) + '"'


def candidates(path, strings):
    """label -> literal for every inlinable string this file references in code."""
    with open(path, errors="replace") as handle:
        text = handle.read()
    found = {}
    for line in text.splitlines():
        if "INCLUDE_" in line:
            continue
        for label in re.findall(r"D_00[0-9A-F]{6}", line):
            if label in found or label not in strings:
                continue
            if int(label[2:], 16) >= SDATA:
                continue
            # A zero-filled record whose first bytes are text disassembles as one
            # .asciz too, so trust the declaration: only a char object is a string.
            decl = re.search(r"^extern\s+([^;\n]*?)\b" + label + r"\b", text, re.M)
            if not decl or "char" not in decl.group(1):
                continue
            lit = c_literal(strings[label])
            if lit:
                found[label] = lit
    return found


def convert(path, picks):
    """Drop the directive and the extern by label, then rewrite only real uses.

    Order matters: substituting first would put a quoted literal inside the
    INCLUDE_RODATA directive, which the assembler then chokes on.
    """
    with open(path) as handle:
        lines = handle.read().splitlines(keepends=True)
    out = []
    for line in lines:
        if re.match(r"^INCLUDE_(RODATA|SDATA|DATA)\(", line) and any(
            re.search(r"\b" + label + r"\b", line) for label in picks
        ):
            continue
        if line.startswith("extern") and any(
            re.search(r"\b" + label + r"\b", line) for label in picks
        ):
            # Drop only the inlined declarators, and parse the code ahead of any
            # comment: a comment can hold a semicolon ("SLPS_999.99;1") and
            # cutting there leaves it unterminated, eating the next declaration.
            cut = min((line.find(c) for c in ("/*", "//") if c in line), default=len(line))
            code, comment = line[:cut], line[cut:].rstrip("\n")
            if ";" not in code:
                out.append(line)
                continue
            decls = re.split(r",\s*", code[: code.rindex(";")].split(None, 2)[-1])
            kept = [
                d for d in decls
                if not any(re.fullmatch(re.escape(label) + r"(\[\])?", d.strip()) for label in picks)
            ]
            if not kept:
                continue
            head = code[: code.rindex(";")].split(None, 2)[:2]
            rebuilt = " ".join(head) + " " + ", ".join(d.strip() for d in kept) + ";"
            out.append(rebuilt + ((" " + comment) if comment.strip() else "") + "\n")
            continue
        if "INCLUDE_" not in line:
            for label, lit in picks.items():
                line = re.sub(r"\b" + label + r"\b", lit, line)
        out.append(line)
    with open(path, "w") as handle:
        handle.write("".join(out))


def main():
    strings = scan_strings()
    if sys.argv[1] == "--list":
        for path in sorted(glob.glob("src/smt3/**/*.c", recursive=True)):
            picks = candidates(path, strings)
            if picks:
                with open(path, errors="replace") as handle:
                    asm = sum(1 for ln in handle if "INCLUDE_ASM" in ln)
                print(f"{len(picks)}\t{asm}\t{path}")
        return
    path = sys.argv[2]
    picks = candidates(path, strings)
    if not picks:
        print("no candidates")
        return
    convert(path, picks)
    print(f"inlined {len(picks)} strings in {path}")
    for label, lit in sorted(picks.items()):
        print(f"  {label} {lit}")


if __name__ == "__main__":
    main()
