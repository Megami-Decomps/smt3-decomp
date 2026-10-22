#!/usr/bin/env python3
"""Resolve which struct type each `x->member` access reads.

A member name several structs share cannot be renamed by text alone: the member
belongs to the declared type of its object expression. This reads the
declarations in scope and answers per access, or says it cannot."""

import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from cparse import struct_fields  # noqa: E402

# A declaration occupies a whole statement: `TYPE *name;` or `TYPE name[2] = x;`
DECL = re.compile(r"^\s*(?:static\s+|const\s+|volatile\s+|register\s+|extern\s+)*"
                  r"(?:struct\s+|union\s+|enum\s+)?([A-Za-z_]\w*)(?:\s+|\s*\*+\s*)"
                  r"([A-Za-z_]\w*)\s*(?:\[[^\]]*\])*\s*(?:=[^;]*)?;")
PARAM = re.compile(r"(?:struct\s+|union\s+|const\s+)*([A-Za-z_]\w*)(?:\s+|\s*\*+\s*)"
                   r"([A-Za-z_]\w*)\s*(?:\[[^\]]*\])*$")
FUNC = re.compile(r"^[A-Za-z_][\w \t\*]*?\b\w+\s*\(([^;{}]*)\)\s*\{", re.M)
# An old-style definition declares its parameters between the header and the
# brace. Those lines sit at file scope, where several functions' `source`
# parameters collide and the last one wins, which resolves the others wrongly.
KNR = re.compile(r"^[A-Za-z_][\w \t\*]*?\b\w+\s*\(\s*[A-Za-z_][\w, \t]*\)\s*$((?:\s*\n(?!\s*\{)[^\n]*;)*)\s*\n\s*\{", re.M)
# The object expression ending just before a `.`/`->`. A cast types the base only
# when both are parenthesised: `->` binds tighter than a cast, so `(T *)r->plane`
# casts the member, not `r`, and taking T there resolved the wrong struct.
# The cast may wrap a whole expression, `((T *)base->member)->m`, as long as it
# holds no further parentheses: a call's result is not something to resolve.
PCHAIN = re.compile(r"\(\s*\(\s*(?:struct\s+|const\s+)*([A-Za-z_]\w*)\s*\*+\s*\)\s*&?\s*"
                    r"[^()]*\)((?:\s*\[[^\]]*\]|\s*(?:->|\.)\s*[A-Za-z_]\w*)*)$")
BCHAIN = re.compile(r"([A-Za-z_]\w*)((?:\s*\[[^\]]*\]|\s*(?:->|\.)\s*[A-Za-z_]\w*)*)$")
STEP = re.compile(r"(?:->|\.)\s*([A-Za-z_]\w*)")
# Resolved, but to something no rename can target.
UNKNOWN = "<unknown>"
# `return work;` and `goto done;` have the shape of a declaration.
NOT_A_TYPE = {"return", "goto", "break", "continue", "case", "default", "else",
              "do", "typedef", "sizeof", "if", "while", "for", "switch"}


# `cparse.struct_fields` skips a definition written on one line, so its members
# are invisible: `typedef struct { instanceMes_tag *mes; } itfMesInstance_t;`
ONELINE = re.compile(r"(?:typedef\s+)?(?:struct|union)\s+?(\w+)?\s*\{([^{}]*)\}\s*(\w+)?\s*;")
ALIAS = re.compile(r"^\s*typedef\s+(?:struct\s+|union\s+)?([A-Za-z_]\w*)\s+([A-Za-z_]\w*)\s*;", re.M)
MEMBER = re.compile(r"^\s*(?:const\s+|volatile\s+)*(?:struct\s+|union\s+|enum\s+)?"
                    r"([A-Za-z_]\w*)(?:\s+|\s*\*+\s*)([A-Za-z_]\w*)\s*(?:\[[^\]]*\])*\s*$")


def struct_members(root):
    """{type: {member: its type}} over the tree, so a chain can be followed."""
    out, aliases = {}, {}
    for path in sorted(list((root / "include").rglob("*.h")) + list((root / "src").rglob("*.c"))):
        text = path.read_text(errors="replace")
        for _, name, fields, _ in struct_fields(text):
            if name:
                out.setdefault(name, {}).update({m: ty for ty, _, m, _ in fields})
        bare = re.sub(r"/\*.*?\*/", " ", text, flags=re.S)
        for m in ONELINE.finditer(bare):
            name = m.group(3) or m.group(1)
            if not name:
                continue
            for decl in m.group(2).split(";"):
                d = MEMBER.match(decl)
                if d:
                    out.setdefault(name, {}).setdefault(d.group(2), d.group(1))
        for old, new in ALIAS.findall(bare):
            aliases[new] = old
    # `typedef EffBlurTemplateBody EffPCPTexturedBlurParams;` names one record twice.
    for _ in range(4):
        for new, old in aliases.items():
            if new not in out and old in out:
                out[new] = out[old]
    return out


def scopes(text):
    """[(start, end, {identifier: type})] per function body, K&R definitions included."""
    out = []
    for m in KNR.finditer(text):
        i, depth = text.index("{", m.end() - 1) + 1, 1
        while depth and i < len(text):
            depth += (text[i] == "{") - (text[i] == "}")
            i += 1
        names = {}
        for line in m.group(1).split("\n"):
            d = DECL.match(line)
            if d and d.group(1) not in NOT_A_TYPE:
                names[d.group(2)] = d.group(1)
        for line in text[m.end():i].split("\n"):
            d = DECL.match(line)
            if d and d.group(1) not in NOT_A_TYPE:
                names[d.group(2)] = d.group(1)
        out.append((m.start(), i, names))
    for m in FUNC.finditer(text):
        i, depth = m.end(), 1
        while depth and i < len(text):
            depth += (text[i] == "{") - (text[i] == "}")
            i += 1
        names = {}
        for part in m.group(1).split(","):
            p = PARAM.search(part.strip())
            if p:
                names[p.group(2)] = p.group(1)
        for line in text[m.end():i].split("\n"):
            d = DECL.match(line)
            if d and d.group(1) not in NOT_A_TYPE:
                names[d.group(2)] = d.group(1)
        out.append((m.start(), i, names))
    return out


def file_scope(text, scope_list=()):
    """Declarations at file scope, which a body falls back to.

    A K&R parameter list is at depth 0 too, so the spans of the scopes already
    found are excluded; otherwise one function's `source` types another's."""
    covered = [(s, e) for s, e, _ in scope_list]
    names, depth, pos = {}, 0, 0
    for line in text.split("\n"):
        if depth == 0 and not any(s <= pos < e for s, e in covered):
            d = DECL.match(line)
            if d and d.group(1) not in NOT_A_TYPE:
                names[d.group(2)] = d.group(1)
        depth = max(0, depth + line.count("{") - line.count("}"))
        pos += len(line) + 1
    return names


def owner_at(text, pos, names, members):
    """The type whose member the access at `pos` reads, or None if unresolvable.

    `pos` is the offset of the `.` or `->`. The chain is followed from its base,
    so `work->head.degreesA` resolves against the type of `head`."""
    head = text[text.rfind("\n", 0, pos) + 1:pos]
    m = PCHAIN.search(head)
    if m:
        ty, steps = m.group(1), m.group(2)
    else:
        m = BCHAIN.search(head)
        if not m:
            return None
        ty, steps = names.get(m.group(1)), m.group(2)
    if ty is None:
        return None
    for step in STEP.findall(steps or ""):
        table = members.get(ty)
        if table is None:
            # Not a record this tree declares, so it is not one being renamed either.
            return UNKNOWN
        if step not in table:
            # The prefix resolved to the wrong record, or the member sits in a
            # nested anonymous struct, which struct_fields does not record.
            return None
        ty = table[step].replace("struct ", "").strip()
    return ty


def accesses(text, member, scope_list, globals_, members):
    """[(punct_start, name_end, owner or None)] for every `.member` use."""
    out = []
    for m in re.finditer(r"(->|\.)\s*" + re.escape(member) + r"\b", text):
        names = dict(globals_)
        for start, end, local in scope_list:
            if start <= m.start() < end:
                names.update(local)
                break
        out.append((m.start(), m.end(), owner_at(text, m.start(), names, members)))
    return out
