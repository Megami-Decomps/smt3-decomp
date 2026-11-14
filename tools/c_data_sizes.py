#!/usr/bin/env python3
"""Probe the C side of every data symbol: sizeof of each file-scope extern, per unit.

Appends `const unsigned int __sz_<name> = sizeof(<name>);` to a copy of each unit and
compiles it with the build's cc1, so the answer is the compiler's, not a guess.
Writes a JSON cache: {name: [{"unit", "decl", "size", "elem"}]}.
Used by check_data_sizes.py.  Usage: c_data_sizes.py OUT.json [-j N]
"""
import json, os, re, subprocess, sys, tempfile
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CC1 = ["tools/glibc32/ld-linux.so.2", "--library-path", "tools/glibc32",
       "tools/compilers/ee-gcc2.96/lib/gcc-lib/ee/2.96-ee-001003-1/cc1"]
DEFS = ("-D__GNUC__=2 -D__GNUC_MINOR__=96 -D__GNUC_PATCHLEVEL__=0 -Dmips -DMIPSEL -DR5900 "
        "-D_mips -D_MIPSEL -D_R5900 -D__ee__ -D__mips__ -D__MIPSEL__ -D__R5900__ -D__mips "
        "-D__MIPSEL -D__R5900 -D__OPTIMIZE__ -D__LANGUAGE_C -D_LANGUAGE_C -DLANGUAGE_C "
        "-D__mips_eabi -Iinclude -Isrc -DVERSION_SMT3").split()
ASM_ROOT = "-DASM_ROOT=\"build/eeasm/asm/smt3/nonmatchings/\""


def strip_attr(text):
    while True:
        i = text.find("__attribute__")
        if i < 0:
            return text
        j, depth = text.index("(", i), 0
        for k in range(j, len(text)):
            depth += (text[k] == "(") - (text[k] == ")")
            if depth == 0:
                break
        text = text[:i] + text[k + 1:]


DEFN = re.compile(r"^[A-Za-z_][\w \t\*]*?\b([A-Za-z_]\w*)\s*((?:\[[^\]]*\]\s*)*)\s*=")
DECL = re.compile(r"^extern\s+(.*?)\b([A-Za-z_]\w*)\s*((?:\[[^\]]*\]\s*)*)$")


def externs(unit):
    cmd = ["gcc", "-E", "-P", "-undef", "-nostdinc"] + DEFS + [ASM_ROOT, unit]
    result = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True)
    # A failed preprocessor can emit empty or partial output. Neither is a
    # complete declaration inventory; keep the unit in failed_units instead.
    if result.returncode != 0:
        return None
    out = result.stdout
    found = {}
    for raw in out.splitlines():
        line = raw.strip()
        local = raw[:1] in " \t"
        if line.startswith("extern ") and line.endswith(";"):
            body = strip_attr(line[:-1]).strip()
            if "(" in body or "," in body:
                continue
            m = DECL.match(body)
            if m:
                found[m.group(2)] = (body, m.group(3).strip(), local)
        elif not local and not line.startswith(("typedef", "static", "extern", "struct", "union", "enum", "}", "#")):
            m = DEFN.match(strip_attr(line))
            if m and m.group(1) not in found:
                found[m.group(1)] = ("definition " + line.split("=")[0].strip(), m.group(2).strip(), False)
    return found


def compile_probe(unit, names, decls, tmp):
    for _ in range(4):
        src = (ROOT / unit).read_text(errors="replace")
        lines = []
        for n in names:
            if decls[n][2]:
                lines.append(decls[n][0] + ";")
        for n in names:
            if decls[n][1].startswith("[]"):
                continue
            lines.append("const unsigned int __sz_%s = sizeof(%s);" % (n, n))
            if decls[n][1]:
                lines.append("const unsigned int __esz_%s = sizeof(%s[0]);" % (n, n))
        probe = Path(tmp) / "probe.c"
        # probe lines come last so error line numbers map to the line list
        probe.write_text(src + "\n" + "\n".join(lines) + "\n")
        base = src.count("\n") + 2
        r = subprocess.run(CC1 + DEFS + [ASM_ROOT, "-D__SIZE_TYPE__=unsigned int",
                           "-D__PTRDIFF_TYPE__=int", "-quiet", "-O0", str(probe), "-o", str(Path(tmp) / "probe.s")],
                           cwd=ROOT, capture_output=True, text=True)
        bad = set()
        for m in re.finditer(r"probe\.c:(\d+):", r.stderr):
            ln = int(m.group(1)) - base
            if 0 <= ln < len(lines):
                mm = re.search(r"__e?sz_(\w+)", lines[ln]) or re.search(r"(\w+)\s*(?:\[[^\]]*\]\s*)*;$", lines[ln])
                if mm:
                    bad.add(mm.group(1))
        if r.returncode == 0 and not bad:
            return (Path(tmp) / "probe.s").read_text()
        if not bad:
            return None
        names = [n for n in names if n not in bad]
    return None


def one(unit):
    decls = externs(unit)
    if decls is None:
        return unit, {"__failed__": 1}
    if not decls:
        return unit, {}
    with tempfile.TemporaryDirectory() as tmp:
        asm = compile_probe(unit, sorted(decls), decls, tmp)
    out = {}
    if asm is None:
        return unit, {"__failed__": 1}
    for m in re.finditer(r"^__(e?sz)_(\w+):\n\s*\.word\s+(\d+)", asm, re.M):
        kind, name, val = m.groups()
        out.setdefault(name, {"unit": unit, "decl": decls[name][0], "local": decls[name][2]})[
            "elem" if kind == "esz" else "size"] = int(val)
    return unit, out


def main():
    out = sys.argv[1]
    jobs = int(sys.argv[sys.argv.index("-j") + 1]) if "-j" in sys.argv else os.cpu_count()
    units = sorted(str(p.relative_to(ROOT)) for p in (ROOT / "src/smt3").rglob("*.c"))
    res, failed = {}, []
    with ThreadPoolExecutor(jobs) as ex:
        for unit, d in ex.map(one, units):
            if "__failed__" in d:
                failed.append(unit)
                continue
            for name, v in d.items():
                if "size" in v:
                    res.setdefault(name, []).append(v)
    Path(out).write_text(json.dumps({"symbols": res, "failed_units": failed}, indent=0))
    print("%d symbols, %d/%d units failed to probe" % (len(res), len(failed), len(units)), file=sys.stderr)


if __name__ == "__main__":
    main()
