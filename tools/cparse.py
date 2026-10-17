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


def blocks(text):
    """Top-level items: each declaration, definition, INCLUDE_ASM or preprocessor line,
    with any comment lines directly above it. Several declarations on consecutive lines
    are separate items, so porting picks only the ones a function uses."""
    out, cur, depth, in_comment = [], [], 0, False

    def flush():
        if cur:
            out.append("\n".join(cur))
            cur.clear()

    for line in text.split("\n"):
        stripped = line.strip()
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


def declared_name(decl):
    """Identifier a top-level declaration introduces: the typedef/struct name for type
    definitions (whose bodies may hold function-pointer members), the name before '('
    for prototypes, else the last identifier before ';'."""
    code = re.sub(r"/\*.*?\*/|//[^\n]*", "", decl, flags=re.S).strip()
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
