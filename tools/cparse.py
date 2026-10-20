"""C text helpers: top-level items, the name a declaration introduces."""
import re

TOKENS = re.compile(r'/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|\b[A-Za-z_]\w*\b', re.S)
DEF = re.compile(r"^[A-Za-z_][\w\s\*]*?\b(\w+)\s*\([^;]*$", re.M)
KNR_HEADER = re.compile(r"^[A-Za-z_][\w \t\*]*\b\w+\s*\(\s*\w+(\s*,\s*\w+)*\s*\)\s*$")


def first_code_line(lines):
    """First line of an item outside its leading comment: the header of a
    commented old-style (K&R) definition."""
    in_comment = False
    for line in lines:
        stripped = line.strip()
        if in_comment:
            in_comment = "*/" not in stripped
        elif stripped.startswith("/*"):
            in_comment = "*/" not in stripped
        elif stripped and not stripped.startswith("//"):
            return line
    return ""


COND_OPEN = re.compile(r"^#\s*(if|ifdef|ifndef)\b")
COND_CLOSE = re.compile(r"^#\s*endif\b")


def blocks(text):
    """Top-level items: each declaration, definition, INCLUDE_ASM or preprocessor line,
    with any comment lines directly above it. Several declarations on consecutive lines
    are separate items, so porting picks only the ones a function uses.

    A `#if`/`#else`/`#endif` region is one item, body and all. Splitting it would
    hand a part an `#else` with no `#ifdef`, and a NON_MATCHING draft has to move
    to whichever part its function lands in."""
    out, cur, depth, in_comment, cond = [], [], 0, False, 0

    def flush():
        if cur:
            out.append("\n".join(cur))
            cur.clear()

    for line in text.split("\n"):
        stripped = line.strip()
        if cond:
            # Inside a conditional region nothing ends the item but its own #endif,
            # so blank lines and the brace rules below are skipped.
            cur.append(line)
            if COND_OPEN.match(stripped):
                cond += 1
            elif COND_CLOSE.match(stripped):
                cond -= 1
                if not cond:
                    flush()
            continue
        if depth == 0 and not in_comment and COND_OPEN.match(stripped):
            # Keep any comment already gathered: it introduces the region.
            cur.append(line)
            cond = 1
            continue
        if depth == 0 and not in_comment and not stripped:
            flush()
            continue
        if depth == 0 and not in_comment and stripped.startswith("#"):
            flush()
            out.append(line)
            continue
        cur.append(line)
        # The line's code outside comments; a block comment can span lines, and
        # its text (`...permit it;`) must not end the item.
        code, rest = "", stripped
        while rest:
            if in_comment:
                end = rest.find("*/")
                if end < 0:
                    rest = ""
                else:
                    in_comment, rest = False, rest[end + 2:]
            else:
                start, line_comment = rest.find("/*"), rest.find("//")
                if line_comment >= 0 and (start < 0 or line_comment < start):
                    code, rest = code + rest[:line_comment], ""
                elif start >= 0:
                    code, in_comment, rest = code + rest[:start], True, rest[start + 2:]
                else:
                    code, rest = code + rest, ""
        code = code.strip()
        depth += code.count("{") - code.count("}")
        # An old-style (K&R) definition declares its parameters between the
        # header and the body: those `type name;` lines belong to the function.
        knr_params = depth == 0 and code.endswith(";") and KNR_HEADER.match(first_code_line(cur)) is not None \
            and "{" not in "".join(cur)
        if depth == 0 and not in_comment and code.endswith((";", "}")) and not knr_params:
            flush()
    flush()
    return out


SUFFIXED = re.compile(r"(?:__attribute__|__asm__|asm)\s*\(")


def strip_attributes(code):
    """Drop a declarator's __attribute__((...)) and asm("...") suffixes.

    Their parentheses otherwise read as a prototype's parameter list, so
    `extern s32 fldGb __attribute__((section(".data")));` looks like it declares
    `__attribute__` and `extern T *cmbGlobalWork asm("DDS3_GBWK");` like `asm`.
    A bare `__asm__(...)` statement has nothing before it and is left alone.
    """
    while (m := SUFFIXED.search(code)) is not None:
        if not code[: m.start()].strip():
            break
        depth, k = 0, m.end() - 1
        while k < len(code):
            depth += (code[k] == "(") - (code[k] == ")")
            k += 1
            if not depth:
                break
        code = code[: m.start()] + code[k:]
    return code


def enum_members(code):
    """Member names of an enum definition, which are what its users name.

    An anonymous enum introduces no type name at all, so without these the
    definition cannot be matched to the part that uses a member."""
    m = re.search(r"\benum\b[^{]*\{(.*)\}", code, re.S)
    if not m:
        return set()
    out = set()
    for item in m.group(1).split(","):
        item = item.split("=")[0]
        names = re.findall(r"[A-Za-z_]\w*", item)
        if names:
            out.add(names[0])
    return out


def declared_names(decl):
    """Every identifier a declaration introduces, so a multi-declarator extern is
    found by any of its declarators rather than only the last, and an anonymous
    enum by any of its members."""
    code = strip_attributes(re.sub(r"/\*.*?\*/|//[^\n]*", "", decl, flags=re.S)).strip()
    if "enum" in code and "{" in code:
        name = declared_name(decl)
        return enum_members(code) | ({name} if name else set())
    # A #define has no declarator list, and its value's own characters can look
    # like one: `#define X 0xFFFF` otherwise ends up naming `xFFFF`.
    if code.startswith("#") or code.startswith(("typedef", "struct", "union", "enum")) \
            or "{" in code or "(" in code:
        name = declared_name(decl)
        return {name} if name else set()
    out = set()
    for part in code.rsplit(";", 1)[0].split(","):
        part = re.sub(r"\[[^\]]*\]|=.*", "", part, flags=re.S)
        names = re.findall(r"[A-Za-z_]\w*", part)
        if names:
            out.add(names[-1])
    return out


def declared_name(decl):
    """Identifier a top-level declaration introduces: the typedef/struct name for type
    definitions (whose bodies may hold function-pointer members), the name before '('
    for prototypes, else the last identifier before ';'."""
    code = strip_attributes(re.sub(r"/\*.*?\*/|//[^\n]*", "", decl, flags=re.S)).strip()
    if m := re.match(r"#define\s+(\w+)", code):
        return m.group(1)
    if code.startswith(("typedef", "struct", "union", "enum")):
        if "{" not in code and (m := re.search(r"\(\s*\*\s*(\w+)\s*\)\s*\(", code)):
            return m.group(1)  # typedef void (*Fn)(...);
        tail = code.rsplit("}", 1)[-1] if "}" in code else code
        names = re.findall(r"[A-Za-z_]\w*", tail.rsplit(";", 1)[0])
        if names:
            return names[-1]
        m = re.match(r"(?:typedef\s+)?(?:struct|union|enum)\s+(\w+)", code)
        return m.group(1) if m else None
    head = code.split("(", 1)[0] if "(" in code else code.rsplit(";", 1)[0]
    head = re.sub(r"\[[^\]]*\]|=.*", "", head, flags=re.S)  # `D_X[0x18]`, `x = {...}`
    names = re.findall(r"[A-Za-z_]\w*", head)
    return names[-1] if names else None
