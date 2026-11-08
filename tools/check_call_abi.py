#!/usr/bin/env python3
"""Find calls whose callee's parameter list differs from the type it is called through.

    python3 tools/check_call_abi.py --host-include <port>/include --sdfport-include <sdfport>/include
"""
import argparse
import bisect
import collections
import json
import os
import re
import subprocess
import sys
import tempfile
from concurrent.futures import ProcessPoolExecutor
from functools import partial
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
KINDS = """MIPS passes float arguments in $f12+ and integers in $a0+, independently, so
f(f32, T *) and f(T *, f32) match while an i386 caller swaps their stack slots.
Each unit is parsed by a host clang (-m32, NON_MATCHING and FIX_BUGS, the port's
headers), and every way a function reaches a call is compared by i386 cdecl
argument class (int or pointer, float, double, long long, struct by size):
  decl      a declaration against the definition, or prototypes with no definition
  table     a .word in asm/smt3/data against every C type that reads its slot: the
            declared element or field type and each cast of that variable or field
  cast      a conversion between function pointer types in C
  erased    a function converted to void * or an integer, traced to the function
            pointer parameter or record field it ends up in
  implicit  a call through an unprototyped declaration
A callee parameter that is never used is not compared. Exit 1 on a finding,
2 if it could not look."""
DEFINES = ["VERSION_SMT3", "SKIP_ASM", "NON_MATCHING", "FIX_BUGS"]

QUALS = re.compile(r"\b(const|volatile|restrict|__restrict|__restrict__|register)\b")
ATTR = re.compile(r"__attribute__\s*\(\(.*?\)\)")
INTS = {"char", "signed char", "unsigned char", "short", "unsigned short", "int", "unsigned int",
        "long", "unsigned long", "_Bool", "short int", "unsigned short int", "long int",
        "unsigned long int", "signed", "unsigned", "signed int", "signed short", "signed long"}
SIZES = {"char": 1, "signed char": 1, "unsigned char": 1, "_Bool": 1, "short": 2, "unsigned short": 2,
         "short int": 2, "unsigned short int": 2, "signed short": 2, "float": 4, "double": 8,
         "long long": 8, "unsigned long long": 8, "long long int": 8, "unsigned long long int": 8}


def split_top(s):
    out, depth, cur = [], 0, ""
    for ch in s:
        depth += ch in "([{"
        depth -= ch in ")]}"
        if ch == "," and depth == 0:
            out.append(cur.strip())
            cur = ""
        else:
            cur += ch
    return out + [cur.strip()]


def last_group(s):
    """(before, inside) for the parenthesised group that ends s."""
    s = s.rstrip()
    if not s.endswith(")"):
        return None
    depth = 0
    for i in range(len(s) - 1, -1, -1):
        depth += s[i] == ")"
        if s[i] == "(":
            depth -= 1
            if depth == 0:
                return s[:i].rstrip(), s[i + 1:-1]
    return None


def count(t):
    """Element count of an array type string: an int, None for [], 1 for a scalar."""
    n = 1
    for d in re.findall(r"\[(\d*)\]", t):
        if not d:
            return None
        n *= int(d)
    return n


class Types:
    """Type strings from the AST, resolved through the unit's typedefs to i386 argument classes."""

    def __init__(self):
        self.tdef = {}
        self.records = {}

    def resolve(self, s, depth=0):
        s = re.sub(r"\s+", " ", ATTR.sub("", QUALS.sub("", s))).strip()
        if depth < 20 and s in self.tdef:
            return self.resolve(self.tdef[s], depth + 1)
        m = re.match(r"^(\w+)\s*((?:\[\d*\])+)$", s)
        if depth < 20 and m and m.group(1) in self.tdef:
            elem, dims = self.resolve(m.group(1), depth + 1), m.group(2)
            if "(*)" in elem:
                return elem.replace("(*)", "(*" + dims + ")", 1)
            if "(" not in elem and "[" not in elem:
                return elem + " " + dims
        return s

    def cls(self, s):
        """I int or pointer, F float, D double, L long long, V void, S:name@size, ?:type unknown."""
        s = self.resolve(s)
        if "*" in s or "[" in s or "(" in s:
            return "I"
        if s == "float":
            return "F"
        if s in ("double", "long double"):
            return "D"
        if "long long" in s:
            return "L"
        if s == "void":
            return "V"
        if s.startswith(("struct ", "union ")):
            l = self.layout(s)
            return "S:" + s.split(" ", 1)[1] + "@" + (str(l[0]) if l else "?")
        if s.startswith("enum ") or s in INTS:
            return "I"
        return "?:" + s

    def layout(self, s, depth=0):
        """(size, align) of a type on i386 with EE-sized fields, or None."""
        s = self.resolve(s)
        m = re.match(r"^(.*?)\s*\[(\d+)\]$", s)
        if m:
            inner = self.layout(m.group(1), depth + 1)
            return inner and (inner[0] * int(m.group(2)), inner[1])
        if "*" in s or "(" in s:
            return (4, 4)
        if s in SIZES:
            return (SIZES[s], min(SIZES[s], 4))
        if s in INTS or s.startswith("enum "):
            return (4, 4)
        if s.startswith(("struct ", "union ")) and s in self.records and depth < 10:
            off, align, size = 0, 1, 0
            for _, ft in self.records[s]:
                l = self.layout(ft, depth + 1)
                if not l:
                    return None
                if s.startswith("union "):
                    size = max(size, l[0])
                else:
                    off = (off + l[1] - 1) // l[1] * l[1] + l[0]
                    size = off
                align = max(align, l[1])
            return ((size + align - 1) // align * align, align)
        return None

    def sig(self, ret, params):
        ps = split_top(params) if params.strip() else []
        variadic = bool(ps) and ps[-1] == "..."
        ps = ps[:-1] if variadic else ps
        proto = bool(params.strip())
        if ps == ["void"]:
            ps = []
        return {"ret": self.cls(ret), "params": [self.cls(p) for p in ps],
                "variadic": variadic, "proto": proto, "text": f"{ret} ({params})"}

    def fn_sig(self, s):
        """Signature of a function type 'R (A, B)'."""
        g = last_group(self.resolve(s))
        return g and self.sig(*g)

    def fp_sig(self, s):
        """Signature a pointer-to-function type (or an array of them) calls with, else None."""
        g = last_group(self.resolve(s))
        h = g and last_group(g[0])
        if not h or not h[1].startswith("*") or "(" in h[1] or "(" in h[0]:
            return None
        return self.sig(h[0], g[1])


def key(s):
    return (s["ret"], tuple(s["params"]), s["variadic"], s["proto"])


class Walker:
    """Collects one unit's call facts from its clang JSON AST."""

    def __init__(self):
        self.t = Types()
        self.records = self.t.records
        self.file = self.line = None
        self.funcs, self.vars, self.casts, self.erased, self.reads, self.implicit = [], [], [], [], [], []

    def upd(self, loc):
        if isinstance(loc, dict):
            for k in ("spellingLoc", "expansionLoc"):
                self.upd(loc.get(k))
            self.file = loc.get("file", self.file)
            self.line = loc.get("line", self.line)

    @staticmethod
    def qt(node):
        t = node.get("type", {})
        return t.get("desugaredQualType", t.get("qualType", ""))

    def walk(self, n, parents):
        # The JSON dump names a file or line only when it changes, so every
        # location is read in document order.
        self.upd(n.get("loc"))
        r = n.get("range") or {}
        self.upd(r.get("begin"))
        here = f"{self.file}:{self.line}"
        self.upd(r.get("end"))
        k = n.get("kind")
        if k == "TypedefDecl":
            owned = (n.get("inner") or [{}])[0].get("ownedTagDecl")
            t = n.get("type", {})
            if owned and "@" + owned["id"] in self.records:
                tag = t.get("qualType", "struct " + n["name"])
                self.records.setdefault(tag, self.records["@" + owned["id"]])
                self.t.tdef[n["name"]] = tag
            else:
                d = t.get("desugaredQualType")
                self.t.tdef[n["name"]] = d if d and d != n["name"] else t.get("qualType", "")
        elif k == "RecordDecl" and n.get("completeDefinition"):
            fields = [(f.get("name", ""), self.qt(f)) for f in n.get("inner", []) if f.get("kind") == "FieldDecl"]
            if n.get("name"):
                self.records[n.get("tagUsed", "struct") + " " + n["name"]] = fields
            self.records["@" + n["id"]] = fields
        elif k == "FunctionDecl":
            self.function(n, here)
        elif k == "VarDecl" and not any(p.get("kind") == "FunctionDecl" for p in parents):
            self.vars.append((n["name"], self.qt(n)))
        elif k in ("ImplicitCastExpr", "CStyleCastExpr"):
            self.cast(n, parents, here)
        elif k == "CallExpr":
            self.call(n, here)
        for c in n.get("inner", []):
            if isinstance(c, dict):
                self.walk(c, parents + [n])

    def function(self, n, here):
        parms = [c for c in n.get("inner", []) if c.get("kind") == "ParmVarDecl"]
        sig = self.t.fn_sig(n["type"]["qualType"])
        if sig is None:
            return
        if parms:
            sig["params"] = [self.t.cls(self.qt(p)) for p in parms]
        body = [c for c in n.get("inner", []) if c.get("kind") == "CompoundStmt"]
        flows = {}
        if body:
            sig["used"] = [bool(p.get("isUsed") or p.get("isReferenced")) for p in parms]
            ids = {p["id"]: i for i, p in enumerate(parms)}
            stack = list(body)
            while stack:
                e = stack.pop()
                if e.get("kind") == "BinaryOperator" and e.get("opcode") == "=":
                    rhs = self.strip(e["inner"][1])
                    pid = rhs and rhs.get("kind") == "DeclRefExpr" and rhs.get("referencedDecl", {}).get("id")
                    src = self.source(e["inner"][0]) if pid in ids else None
                    if src and "member" in src:
                        flows.setdefault(str(ids[pid]), []).append([src["record"], src["member"]])
                stack.extend(c for c in e.get("inner", []) if isinstance(c, dict))
        self.funcs.append({"name": n["name"], "sig": sig, "where": here, "def": bool(body),
                           "implicit": bool(n.get("isImplicit")), "static": n.get("storageClass") == "static",
                           "pfp": [self.t.fp_sig(self.qt(p)) for p in parms], "flows": flows})

    @staticmethod
    def strip(e):
        while e and e.get("kind") in ("ImplicitCastExpr", "ParenExpr", "CStyleCastExpr"):
            e = (e.get("inner") or [None])[0]
        return e

    def func_ref(self, e):
        """The function an expression names, through decays, casts, parens and &."""
        while e and e.get("kind") in ("ImplicitCastExpr", "ParenExpr", "UnaryOperator", "CStyleCastExpr"):
            if e.get("kind") == "UnaryOperator" and e.get("opcode") != "&":
                return None
            e = (e.get("inner") or [None])[0]
        if e and e.get("kind") == "DeclRefExpr" and e.get("referencedDecl", {}).get("kind") == "FunctionDecl":
            return e["referencedDecl"]["name"]
        return None

    def source(self, e):
        """The slot an expression reads: {var} or {record, member}."""
        e = self.strip(e)
        if not e:
            return None
        k = e.get("kind")
        if k == "MemberExpr":
            bt = self.t.resolve(self.qt(e["inner"][0]))
            if e.get("isArrow"):
                bt = self.t.resolve(bt.rstrip("* "))
            return {"record": bt, "member": e.get("name")}
        if k == "ArraySubscriptExpr" or (k == "UnaryOperator" and e.get("opcode") == "*"):
            return self.source((e.get("inner") or [None])[0])
        if k == "DeclRefExpr" and e.get("referencedDecl", {}).get("kind") == "VarDecl":
            return {"var": e["referencedDecl"]["name"]}
        return None

    def cast(self, n, parents, here):
        if n.get("castKind") in ("BuiltinFnToFnPtr", "FunctionToPointerDecay", "LValueToRValue", "ArrayToPointerDecay"):
            return
        sub = (n.get("inner") or [None])[0]
        if not sub:
            return
        to_t, from_t = self.qt(n), self.qt(sub)
        to_s, from_s = self.t.fp_sig(to_t), self.t.fp_sig(from_t)
        fn = self.func_ref(sub)
        if to_s and from_s and key(to_s) != key(from_s):
            self.casts.append({"where": here, "func": fn, "from": from_s, "to": to_s})
        elif from_s and not to_s and fn:
            e = {"where": here, "func": fn, "to": to_t}
            child = n
            for p in reversed(parents):
                if p.get("kind") in ("ImplicitCastExpr", "ParenExpr", "CStyleCastExpr"):
                    child = p
                    continue
                if p.get("kind") == "CallExpr":
                    args = p.get("inner", [])
                    idx = next((i for i, a in enumerate(args) if a is child), 0)
                    if idx > 0 and self.func_ref(args[0]):
                        e["call"], e["arg"] = self.func_ref(args[0]), idx - 1
                elif p.get("kind") == "BinaryOperator" and p.get("opcode") == "=":
                    src = self.source(p["inner"][0])
                    if src and "member" in src:
                        e["record"], e["member"] = src["record"], src["member"]
                break
            self.erased.append(e)
        if to_s and not fn:
            src = self.source(sub)
            if src:
                src.update(where=here, to=to_s)
                self.reads.append(src)

    def call(self, n, here):
        inner = n.get("inner", [])
        e = self.strip(inner[0]) if inner else None
        if e and e.get("kind") == "DeclRefExpr" and e.get("referencedDecl", {}).get("kind") == "FunctionDecl":
            ft = self.t.fn_sig(e["referencedDecl"].get("type", {}).get("qualType", ""))
            if ft and not ft["proto"]:
                self.implicit.append({"where": here, "func": e["referencedDecl"]["name"], "ret": ft["ret"],
                                      "args": [self.t.cls(self.qt(a)) for a in inner[1:]]})

    def record_info(self, rec):
        fields, off = [], 0
        for name, ft in self.records[rec]:
            l = self.t.layout(ft)
            off = None if l is None or off is None else (off + l[1] - 1) // l[1] * l[1]
            fields.append({"name": name, "sig": self.t.fp_sig(ft), "offset": off})
            if off is not None:
                off += l[0]
        whole = self.t.layout(rec)
        return {"record": rec, "size": whole and whole[0], "fields": fields}

    def facts(self, unit):
        record_fp = {}
        for rec, fields in self.records.items():
            fp = {name: self.t.fp_sig(ft) for name, ft in fields if self.t.fp_sig(ft)}
            if fp and not rec.startswith("@"):
                record_fp[rec] = fp
        objects = {}
        for name, vt in self.vars:
            t = self.t.resolve(vt)
            o = {"count": count(t)}
            sig = self.t.fp_sig(vt)
            base = re.sub(r"\[\d*\]", "", t).strip()
            if sig:
                o.update(kind="fp", sig=sig, size=4)
            elif base.endswith("*") and self.t.resolve(base[:-1].strip()) in self.records:
                o.update(kind="ptr", size=4, target=self.record_info(self.t.resolve(base[:-1].strip())))
            elif self.t.resolve(base) in self.records:
                o.update(kind="record", **self.record_info(self.t.resolve(base)))
            else:
                l = self.t.layout(base)
                o.update(kind="scalar", size=l and l[0])
            objects[name] = o
        return {"unit": unit, "funcs": self.funcs, "objects": objects, "record_fp": record_fp,
                "casts": self.casts, "erased": self.erased, "reads": self.reads, "implicit": self.implicit}


def clang_cmd(unit, host, sdfport):
    return ["clang", "-fsyntax-only", "-m32", "-std=gnu90", "-w",
            "-Wno-error=incompatible-function-pointer-types", "-Wno-int-conversion",
            "-Wno-incompatible-pointer-types", "-Wno-return-mismatch",
            *[f"-D{d}" for d in DEFINES], '-DASM_ROOT=""',
            "-include", str(host / "host/common.h"), "-include", str(host / "host/eeaddr.h"),
            f"-I{host}/host", f"-I{sdfport}", f"-I{sdfport}/host", f"-I{ROOT}/include", f"-I{ROOT}/src",
            "-Xclang", "-ast-dump=json", str(unit)]


def extract(unit, host, sdfport):
    """(facts, error text): error text is clang's first errors, which leave the AST partial."""
    out = subprocess.run(clang_cmd(unit, host, sdfport), capture_output=True, text=True, cwd=ROOT)
    if not out.stdout.strip():
        raise RuntimeError(f"{unit}: clang gave no AST: {out.stderr.strip()[:300]}")
    w = Walker()
    for top in json.loads(out.stdout).get("inner", []):
        w.walk(top, [])
    errors = [l for l in out.stderr.splitlines() if "error:" in l]
    return w.facts(str(unit)), "\n".join(errors[:2])


def size(c):
    if c in ("L", "D"):
        return 8
    if c.startswith("S:"):
        n = c.rsplit("@", 1)[1]
        return (int(n) + 3) // 4 * 4 if n != "?" else None
    return 4


def slots(params):
    """(offset, class) of each argument on the i386 stack; offset None past an unknown size."""
    out, off = [], 0
    for c in params:
        out.append((off, c))
        off = None if off is None or size(c) is None else off + size(c)
    return out


def mismatch(caller, callee):
    """Why calling callee through caller's type goes wrong under i386 cdecl; empty if it does not."""
    why = []
    cr, ce = caller["ret"], callee["ret"]
    # A void callee leaves eax unset and a long long one puts its low word there:
    # an int caller reads either the way a MIPS caller reads v0.
    if cr == "V":
        if ce in ("F", "D"):
            why.append(f"returns {ce} on the x87 stack, which a void caller never pops")
    elif cr != ce and not (cr == "I" and ce in ("V", "L")):
        why.append(f"returns {ce}, caller reads {cr}")
    used = callee.get("used") or [True] * len(callee["params"])
    if not caller["proto"]:
        bad = [i for i, c in enumerate(callee["params"]) if c == "F" and used[i]]
        if bad:
            why.append(f"unprototyped call passes float as double, callee param {bad[0]} is F")
        return why
    have = dict(slots(caller["params"]))
    for i, (off, c) in enumerate(slots(callee["params"])):
        if not used[i]:
            continue
        if off is None:
            why.append(f"param {i}: offset unknown")
        elif off not in have:
            if not caller["variadic"]:
                why.append(f"param {i} ({c}) at +{off}: caller passes nothing there")
        elif have[off] != c:
            why.append(f"param {i} at +{off}: caller passes {have[off]}, callee reads {c}")
    return why


def data_words():
    """{label: address} and {address: (word, label)} for every .word in the data split."""
    labels, words, label = {}, {}, None
    for path in sorted((ROOT / "asm/smt3/data").rglob("*.s")):
        for line in open(path):
            m = re.match(r"\s*dlabel (\w+)", line)
            if m:
                label = m.group(1)
                continue
            m = re.match(r"\s*/\* [0-9A-F]+ ([0-9A-F]{8}) [0-9A-F]* \*/\s*\.(\w+)\s+(.*)$", line)
            if m and label:
                addr = int(m.group(1), 16)
                labels.setdefault(label, addr)
                if m.group(2) == "word":
                    words[addr] = (m.group(3).strip(), label)
    return labels, words


def analyse(units, labels, words):
    """(findings, coverage notes)."""
    defs, protos, statics, full = {}, collections.defaultdict(list), {}, {}
    for u in units:
        for f in u["funcs"]:
            k = (u["unit"], f["name"]) if f["static"] else f["name"]
            if f["def"] or k not in full:
                full[k] = f
            if f["implicit"]:
                continue
            if f["static"]:
                statics[k] = f["sig"]
                continue
            protos[f["name"]].append((f["sig"], f["where"], f["def"]))
            if f["def"]:
                defs[f["name"]] = (f["sig"], f["where"])

    def callee(name, unit=None):
        if (unit, name) in statics:
            return statics[(unit, name)]
        if name in defs:
            return defs[name][0]
        return next((s for s, _, _ in protos.get(name, []) if s["proto"]), None)

    findings, notes = set(), []

    def report(kind, where, text, why):
        findings.add((kind, where, f"{text}: {'; '.join(why)}"))

    for name, decls in protos.items():
        if name in defs:
            ref, rwhere = defs[name]
            for sig, where, isdef in decls:
                why = [] if isdef else mismatch(sig, ref)
                if why:
                    report("decl", where, f"{name}: declared {sig['text']}, defined {ref['text']} at {rwhere}", why)
        else:
            first, fwhere, _ = decls[0]
            for sig, where, _ in decls[1:]:
                why = mismatch(sig, first) + mismatch(first, sig)
                if why:
                    report("decl", where, f"{name}: declared {sig['text']}, and {first['text']} at {fwhere}", why)

    var_views, field_views = collections.defaultdict(list), collections.defaultdict(list)
    for u in units:
        for r in u["reads"]:
            if "member" in r:
                field_views[(r["record"], r["member"])].append((r["to"], r["where"]))
            else:
                var_views[r["var"]].append((r["to"], r["where"]))
    record_fp = {}
    for u in units:
        for rec, fp in u["record_fp"].items():
            record_fp.setdefault(rec, fp)

    def field_slots(rec, member, label):
        out = [(record_fp[rec][member], label)] if member in record_fp.get(rec, {}) else []
        return out + [(v, f"{label} cast at {w}") for v, w in field_views.get((rec, member), [])]

    # Data: a word is read through every C object whose extent covers it. An
    # object declared [] extends to the next C object past its first element,
    # and an object reached through a pointer table takes the pointee's layout.
    objects = {}
    for u in units:
        for name, o in u["objects"].items():
            if name not in objects or (not objects[name].get("size") and o.get("size")):
                objects[name] = o
    placed = {n: o for n, o in objects.items() if n in labels and o.get("size")}
    for name, o in list(placed.items()):
        if o["kind"] == "ptr" and o["target"]["size"]:
            for i in range(o["count"] if o["count"] is not None else 64):
                w = words.get(labels[name] + 4 * i)
                if not w:
                    break
                if w[0] in labels and w[0] not in placed:
                    placed[w[0]] = dict(o["target"], kind="record", count=1)
    by_start = sorted((labels[n], n) for n in placed)
    starts = [a for a, _ in by_start]
    extent = {}
    for i, (a, n) in enumerate(by_start):
        o = placed[n]
        if o["count"] is not None:
            extent[n] = a + o["count"] * o["size"]
        else:
            extent[n] = next((b for b, _ in by_start[i + 1:] if b >= a + o["size"]), a + o["size"])
    known = set(protos) | {n for _, n in statics}
    for line in open(ROOT / "config/smt3/symbol_addrs.txt"):
        if "type:func" in line:
            known.add(line.split("=")[0].strip())
    checked, blind = 0, collections.Counter()
    for addr, (fname, lab) in sorted(words.items()):
        if fname not in known:
            continue
        views, owners = [], []
        for a, n in by_start[:bisect.bisect_right(starts, addr)][::-1][:400]:
            if addr >= extent[n]:
                continue
            o, off = placed[n], addr - a
            owners.append(n)
            if o["kind"] == "fp":
                views.append((o["sig"], f"{n}[{off // 4}]"))
            if o["kind"] == "record":
                f = next((f for f in o["fields"] if f["offset"] == off % o["size"]), None)
                if f:
                    slot = f"{n}[{off // o['size']}].{f['name']}"
                    views += ([(f["sig"], slot)] if f["sig"] else []) + field_slots(o["record"], f["name"], slot)
            else:
                views += [(v, f"{n}[{off // o['size']}] cast at {w}") for v, w in var_views.get(n, [])]
        got = callee(fname)
        if not views or not got:
            blind[(lab, f"{fname} has no prototype" if views else
                   f"inside {', '.join(owners)}, which C never reads as a function" if owners else
                   "no C object covers it")] += 1
            continue
        checked += 1
        for want, slot in views:
            why = mismatch(want, got)
            if why:
                report("table", f"{lab}+{addr - labels[lab]:#x}", f"{slot} = {fname}: called as {want['text']}, is {got['text']}", why)
    notes.append(f"{checked} function words in data compared, {sum(blind.values())} not:")
    notes += [f"  {lab}: {n} word(s), {why}" for (lab, why), n in sorted(blind.items())]

    for u in units:
        for c in u["casts"]:
            src = (c["func"] and callee(c["func"], u["unit"])) or c["from"]
            why = mismatch(c["to"], src)
            if why:
                report("cast", c["where"], f"{c['func'] or 'a pointer'} as {c['to']['text']}, is {src['text']}", why)

    traced, untraced = 0, collections.Counter()
    for u in units:
        for e in u["erased"]:
            got = callee(e["func"], u["unit"])
            views = field_slots(e["record"], e["member"], f"{e['record']}.{e['member']}") if "record" in e else []
            if "call" in e:
                t = full.get((u["unit"], e["call"])) or full.get(e["call"])
                if t and e["arg"] < len(t["pfp"]):
                    via = f"{e['call']} arg {e['arg']}"
                    views += [(t["pfp"][e["arg"]], via)] if t["pfp"][e["arg"]] else []
                    for rec, mem in t["flows"].get(str(e["arg"]), []):
                        views += field_slots(rec, mem, f"{via} -> {rec}.{mem}")
            if not views or not got:
                untraced[e.get("call", "a value of type " + e["to"])] += 1
                continue
            traced += 1
            for want, via in views:
                why = mismatch(want, got)
                if why:
                    report("erased", e["where"], f"{e['func']} via {via}: called as {want['text']}, is {got['text']}", why)
    notes.append(f"{traced} functions converted to void * or an integer traced to a typed call, "
                 f"{sum(untraced.values())} not:")
    notes += [f"  {n} passed to or as {k}" for k, n in untraced.most_common()]

    for u in units:
        for c in u["implicit"]:
            got = callee(c["func"], u["unit"])
            if got and got["proto"]:
                args = ["D" if a == "F" else a for a in c["args"]]
                caller = {"ret": c["ret"], "params": args, "variadic": False, "proto": True}
                why = mismatch(caller, got)
                if why:
                    report("implicit", c["where"], f"{c['func']} called unprototyped, is {got['text']}", why)

    unknown = {c for u in units for f in u["funcs"] for c in f["sig"]["params"] + [f["sig"]["ret"]] if c.startswith("?")}
    if unknown:
        notes.append(f"unresolved argument types, compared by name: {', '.join(sorted(unknown))}")
    return sorted(findings), notes


CONTROL = """typedef struct { int x; } T;
void good(T *p, float f) { p->x = f; }
void swapped(float f, T *p) { p->x = f; }
void (*table[])(T *, float) = { good, (void (*)(T *, float))swapped };
"""


def control(host, sdfport):
    with tempfile.TemporaryDirectory() as tmp:
        unit = Path(tmp) / "control.c"
        unit.write_text(CONTROL)
        facts, _ = extract(unit, host, sdfport)
    found, _ = analyse([facts], {}, {})
    if [f[2].split(" ")[0] for f in found] != ["swapped"]:
        sys.exit(f"control gave {found}, so this check cannot be trusted")


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0], epilog=KINDS,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--host-include", default=os.environ.get("HOST_INCLUDE"), help="the port's include/")
    ap.add_argument("--sdfport-include", default=os.environ.get("SDFPORT_INCLUDE"), help="sdfport's include/")
    ap.add_argument("--jobs", type=int, default=max(1, (os.cpu_count() or 2) // 2))
    ap.add_argument("-v", "--verbose", action="store_true", help="list what could not be compared")
    ap.add_argument("units", nargs="*", help="default: every unit under src/smt3")
    args = ap.parse_args()
    if not args.host_include or not (Path(args.host_include) / "host/common.h").is_file():
        sys.exit("cannot look: pass --host-include <port>/include (needs host/common.h)")
    if not args.sdfport_include or not Path(args.sdfport_include).is_dir():
        sys.exit("cannot look: pass --sdfport-include <sdfport>/include")
    host, sdfport = Path(args.host_include).resolve(), Path(args.sdfport_include).resolve()
    control(host, sdfport)
    units = [Path(u).resolve().relative_to(ROOT) for u in args.units] or \
        sorted(p.relative_to(ROOT) for p in (ROOT / "src/smt3").rglob("*.c"))
    try:
        with ProcessPoolExecutor(args.jobs) as pool:
            results = list(pool.map(partial(extract, host=host, sdfport=sdfport), units))
    except RuntimeError as e:
        print(f"cannot look: {e}", file=sys.stderr)
        return 2
    broken = [(facts["unit"], err) for facts, err in results if err]
    findings, notes = analyse([facts for facts, _ in results], *data_words())
    for kind, where, text in findings:
        print(f"{kind}: {where}: {text}")
    for unit, err in broken:
        print(f"note: {unit} has compile errors, its AST is partial: {err.splitlines()[0]}", file=sys.stderr)
    for line in notes if args.verbose else notes[:1] + [n for n in notes if not n.startswith("  ")][1:]:
        print(line, file=sys.stderr)
    print(f"{len(units)} unit(s), {len(findings)} finding(s)")
    return 1 if findings else 0


sys.setrecursionlimit(100000)
if __name__ == "__main__":
    sys.exit(main())
