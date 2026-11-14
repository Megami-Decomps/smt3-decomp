#!/usr/bin/env python3
"""List data symbols whose size is wrong in splat's view, the C, or the retail code's use.

Three views per symbol:
  splat  size:/type: in symbol_addrs.txt, else the gap to the next symbol;
  C      sizeof from tools/c_data_sizes.py (the cc1 answer, per unit);
  code   the extent retail text reaches from a base address, decoded from the ELF
         (constants built by lui/addiu/ori/gp, tracked through registers within a function,
         cleared at branch targets and calls).
Classes:
  BEYOND   code reads/writes past the end of the symbol it took the address of
           (strong when the base is exactly a symbol start, else weak)
  IN_GAP   an access lands in the unowned gap after a sized symbol
  STRADDLE an access starts in one symbol and ends in the next
  CSIZE    sizeof in C differs from the splat extent (or C views disagree)
  WORDS    narrow (1/2 byte) accesses to a symbol splat emits as words, with address-like words
  JTBL     a data symbol's extent runs over the start of a jump table (jtbl_ label)
  COVER    a sized symbol's extent runs over the start of another named symbol
  TAIL     gap after a sized symbol that no code or data pointer references
Usage: check_data_sizes.py [--symbols FILE] [--elf FILE] [--csizes JSON] [--only A,B]
"""
import argparse, json, re, struct, sys, bisect
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
DATA_SECS = {".data", ".rodata", ".lit4", ".lit8", ".sdata", ".sbss", ".bss", ".vudata"}
NARROW_OK = {"u8", "s8", "u16", "s16", "char", "asciz", "ascii", "short", "string", "u8*"}
SYM = re.compile(r"^(\w+)\s*=\s*0x([0-9A-Fa-f]+);\s*(?://\s*(.*))?$")


def load_elf(path):
    b = Path(path).read_bytes()
    shoff, = struct.unpack_from("<I", b, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from("<HHH", b, 0x2E)
    raw = [struct.unpack_from("<10I", b, shoff + i * shentsize) for i in range(shnum)]
    stroff = raw[shstrndx][4]
    secs = []
    for name, typ, flags, addr, off, size, *_ in raw:
        n = b[stroff + name:b.index(b"\0", stroff + name)].decode()
        secs.append(dict(name=n, type=typ, flags=flags, addr=addr, off=off, size=size))
    return b, secs


def load_syms(path):
    syms = {}
    for line in Path(path).read_text().splitlines():
        m = SYM.match(line.strip())
        if not m:
            continue
        name, addr, tail = m.group(1), int(m.group(2), 16), m.group(3) or ""
        size = re.search(r"size:(0x[0-9A-Fa-f]+|\d+)", tail)
        typ = re.search(r"type:(\w+)", tail)
        syms[name] = dict(name=name, addr=addr, size=int(size.group(1), 0) if size else None,
                          type=typ.group(1) if typ else None, auto=False)
    return syms


def sx16(v):
    return v - 0x10000 if v & 0x8000 else v


LOAD_W = {0x20: 1, 0x21: 2, 0x23: 4, 0x24: 1, 0x25: 2, 0x27: 4, 0x31: 4, 0x37: 8, 0x1E: 16,
          0x32: 4, 0x36: 16, 0x1A: 1, 0x1B: 1, 0x22: 1, 0x26: 1}
STORE_W = {0x28: 1, 0x29: 2, 0x2B: 4, 0x39: 4, 0x3F: 8, 0x1F: 16, 0x3A: 4, 0x3E: 16,
           0x2A: 1, 0x2E: 1, 0x2C: 1, 0x2D: 1}
CALLER_SAVED = list(range(1, 16)) + [24, 25]


def scan_text(b, secs, func_starts, gp):
    """Yield (pc, base, addr, width, via_reg, exact_addr) for each memory access with a known constant address,
    and ('const', value) events for addiu/ori built addresses."""
    accesses, consts = [], []
    for s in secs:
        if s["name"] != ".text":
            continue
        n = s["size"] // 4
        words = struct.unpack_from("<%dI" % n, b, s["off"])
        base_pc = s["addr"]
        # branch targets and function boundaries clear the state
        clear = set(func_starts)
        for i, w in enumerate(words):
            op = w >> 26
            if op in (4, 5, 6, 7, 0x14, 0x15, 0x16, 0x17, 1) or (op in (0x10, 0x11, 0x12) and (w >> 21) & 31 == 8):
                clear.add(base_pc + (i + 1) * 4 + (sx16(w & 0xFFFF) << 2))
        regs = {}
        pending_call = False
        jump_seen = 0  # 1 on a j/jr's delay slot, 2 on the first instruction after it
        for i, w in enumerate(words):
            pc = base_pc + i * 4
            if pc in clear:
                regs = {}
            if jump_seen == 2:
                regs = {}
                jump_seen = 0
            elif jump_seen == 1:
                jump_seen = 2
            if w >> 26 == 2 or (w >> 26 == 0 and (w & 63) == 8):
                jump_seen = 1
            op, rs, rt, imm = w >> 26, (w >> 21) & 31, (w >> 16) & 31, w & 0xFFFF
            rd = (w >> 11) & 31
            kill = []
            if op == 0x0F:
                regs[rt] = ((imm << 16) & 0xFFFFFFFF, "hi", pc)
                regs.pop(0, None)
            elif op in (0x09, 0x0D, 0x19) and rs in regs:
                # Capture provenance before writing rt: address construction often
                # uses the same register for both the high half and result.
                v, kind, _ = regs[rs]
                v = (v + sx16(imm)) & 0xFFFFFFFF if op != 0x0D else v | imm
                regs[rt] = (v, "addr", pc)
                if rt != 0:
                    consts.append((pc, v, kind == "hi"))
            elif op in (0x09, 0x19) and rs == 28 and gp:
                v = (gp + sx16(imm)) & 0xFFFFFFFF
                regs[rt] = (v, "addr", pc)
                if rt != 0:
                    consts.append((pc, v, True))
            elif op in LOAD_W or op in STORE_W:
                width = LOAD_W.get(op) or STORE_W.get(op)
                if rs in regs:
                    v, kind, _ = regs[rs]
                    accesses.append((pc, (v + sx16(imm)) & 0xFFFFFFFF, width, kind == "addr", v, op in STORE_W))
                elif rs == 28 and gp:
                    accesses.append((pc, (gp + sx16(imm)) & 0xFFFFFFFF, width, False, None, op in STORE_W))
                if op in LOAD_W and op not in (0x31, 0x32, 0x36):
                    kill.append(rt)
            elif op == 0:
                funct = w & 63
                if funct in (0x21, 0x2D, 0x25) and rt == 0 and rs in regs:  # move
                    regs[rd] = regs[rs]
                elif funct == 0x08:
                    regs = {} if False else regs
                    kill = []
                else:
                    kill.append(rd)
                if funct == 9:
                    pending_call = True
            elif op == 3:
                pending_call = True
            elif op in (0x08, 0x0A, 0x0B, 0x0C, 0x0E, 0x18, 0x09, 0x0D, 0x19):
                kill.append(rt)
            elif op in (0x10, 0x11, 0x12):
                if rs in (0, 1, 2, 3):
                    kill.append(rt)
            elif op == 0x1C:
                kill += [rd, rt]
            # GPR zero discards every write, including arithmetic and moves.
            # Never let a discarded address seed later accesses or symbols.
            regs.pop(0, None)
            for r in kill:
                regs.pop(r, None)
            if pending_call and op not in (3,) and not (op == 0 and (w & 63) == 9):
                # the instruction after jal/jalr is its delay slot; kill after it
                for r in CALLER_SAVED:
                    regs.pop(r, None)
                pending_call = False
            elif pending_call:
                pass
    return accesses, consts


def load_jtbls(asm_root):
    """Addresses of every `dlabel jtbl_XXXXXXXX` under asm_root; exits if there are none."""
    pat = re.compile(r"^dlabel jtbl_([0-9A-F]{8})\b", re.M)
    found = set()
    for f in Path(asm_root).rglob("*.s"):
        found.update(int(m, 16) for m in pat.findall(f.read_text(errors="replace")))
    if not found:
        sys.exit("no jtbl_ labels under %s, so jump tables cannot be checked (configure first)" % asm_root)
    return sorted(found)


WORD_LINE = re.compile(r"^\s*/\* \w+ ([0-9A-F]{8}) [0-9A-F]{8} \*/ \.word ", re.M)


def scan_word_addrs(asm_root):
    """Addresses splat emits as `.word` under asm_root; only those can be relocated."""
    found = set()
    for f in Path(asm_root).rglob("*.s"):
        found.update(int(m, 16) for m in WORD_LINE.findall(f.read_text(errors="replace")))
    return found


def load_word_addrs(asm_root):
    """scan_word_addrs, but exits when there is none."""
    found = scan_word_addrs(asm_root)
    if not found:
        sys.exit("no .word lines under %s, so byte and short data cannot be told from words (configure first)" % asm_root)
    return found


def count_address_like(base, n, word_addrs, word_at, sec_of):
    """Words in [base, base+4n) that splat emits as `.word` and that read as an in-image address."""
    return sum(1 for k in range(n) if base + 4 * k in word_addrs
               and (word_at(base + 4 * k) or 0) >= 0x100000 and sec_of(word_at(base + 4 * k)))


def run_controls():
    """Refuse to report if the checks cannot tell a halfword pair from an address word, or miss a cover.

    Runs on a synthesised asm fixture in a temp dir, so it needs no configured tree."""
    import tempfile
    word_at = lambda a: {0x1000: 0x00330000}.get(a)
    sec_of = lambda v: v if 0x100000 <= v <= 0x5A0330 else None
    shorts = "    /* 1 00001000 */ .short 0x0000\n    /* 2 00001002 */ .short 0x0033\n"
    word = "    /* 1 00001000 00003300 */ .word 0x00330000\n"
    problems = []
    with tempfile.TemporaryDirectory() as d:
        (Path(d) / "a.s").write_text(shorts)
        got = count_address_like(0x1000, 1, scan_word_addrs(d), word_at, sec_of)
        if got != 0:
            problems.append("a .short pair was counted as %d address word(s)" % got)
        (Path(d) / "a.s").write_text(word)
        got = count_address_like(0x1000, 1, scan_word_addrs(d), word_at, sec_of)
        if got != 1:
            problems.append("an address-like .word was counted as %d, not 1" % got)
    if cover_crossings([(0x1000, 0x20)], [0x1000, 0x1010]) != [(0x1000, 0x1020, 0x1010)]:
        problems.append("a symbol over another symbol's start was not reported")
    if cover_crossings([(0x1000, 0x10)], [0x1000, 0x1010]):
        problems.append("a symbol ending at the next start was reported")
    if problems:
        sys.exit("check_data_sizes controls failed, so no result can be trusted: " + "; ".join(problems))


def jtbl_crossings(extents, jtbls):
    """(addr, end, jtbl) for each extent (addr, size) that covers a jtbl start after its own start."""
    out = []
    for addr, size in extents:
        for j in jtbls:
            if addr < j < addr + size:
                out.append((addr, addr + size, j))
    return out


def cover_crossings(extents, starts):
    """(addr, end, other) for each extent (addr, size) that covers the start of another symbol."""
    return [(addr, addr + size, o) for addr, size in extents for o in starts if addr < o < addr + size]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--symbols", default=str(ROOT / "config/smt3/symbol_addrs.txt"))
    ap.add_argument("--elf", default=str(ROOT / "orig/smt3/SLUS_209.11"))
    ap.add_argument("--csizes", default=None)
    ap.add_argument("--only", default=None, help="comma list of symbol names to report")
    ap.add_argument("--classes", default=None)
    ap.add_argument("--dump-universe", default=None, help="write the symbol universe and reference sets as JSON")
    a = ap.parse_args()

    run_controls()
    word_addrs = load_word_addrs(ROOT / "asm")
    b, secs = load_elf(a.elf)
    dsecs = [s for s in secs if s["name"] in DATA_SECS and s["size"]]
    dsecs.sort(key=lambda s: s["addr"])
    sec_starts = [s["addr"] for s in dsecs]

    def sec_of(addr):
        i = bisect.bisect_right(sec_starts, addr) - 1
        if i >= 0 and addr < dsecs[i]["addr"] + dsecs[i]["size"]:
            return dsecs[i]
        return None

    def word_at(addr):
        s = sec_of(addr)
        if not s or s["type"] == 8:
            return None
        o = s["off"] + addr - s["addr"]
        return struct.unpack_from("<I", b, o)[0] if addr + 4 <= s["addr"] + s["size"] else None

    syms = load_syms(a.symbols)
    funcs = {s["addr"] for s in syms.values() if s["type"] == "func" or (not sec_of(s["addr"]))}
    gp = None
    for s in secs:
        if s["name"] == ".reginfo" and s["size"] >= 24:
            gp = struct.unpack_from("<I", b, s["off"] + 20)[0]
    accesses, consts = scan_text(b, secs, funcs, gp)

    # symbol universe: symbol_addrs data symbols plus code-referenced addresses with no symbol
    uni = {s["addr"]: s for s in syms.values() if sec_of(s["addr"])}
    code_refs = set()
    for pc, v, exact in consts:
        if sec_of(v):
            code_refs.add(v)
            if v not in uni and exact:
                uni[v] = dict(name="D_%08X" % v, addr=v, size=None, type=None, auto=True)
    for pc, A, w, exact, base, st in accesses:
        if base is not None and exact and sec_of(base) and base not in uni:
            uni[base] = dict(name="D_%08X" % base, addr=base, size=None, type=None, auto=True)
    starts = sorted(uni)

    def next_start(addr):
        i = bisect.bisect_right(starts, addr)
        sec = sec_of(addr)
        end = sec["addr"] + sec["size"] if sec else addr
        return min(starts[i], end) if i < len(starts) else end

    def extent(s):
        gapend = next_start(s["addr"])
        return s["size"] if s["size"] is not None else gapend - s["addr"]

    def owner(addr):
        i = bisect.bisect_right(starts, addr) - 1
        return uni[starts[i]] if i >= 0 and sec_of(starts[i]) is sec_of(addr) else None

    data_refs = set()
    for s in dsecs:
        if s["type"] == 8:
            continue
        o, n = s["off"], s["size"] // 4
        for i, v in enumerate(struct.unpack_from("<%dI" % n, b, o)):
            if v >= 0x100000 and sec_of(v):
                data_refs.add(v)
    # also anything between sections: data pointing into text is irrelevant here

    if a.dump_universe:
        json.dump(dict(uni=[dict(u, end=next_start(u["addr"])) for u in uni.values()],
                       code_refs=sorted(code_refs), data_refs=sorted(data_refs)), open(a.dump_universe, "w"))

    findings = []
    only_dbg = set(a.only.split(',')) if a.only else None

    def add(cls, s, msg, strength=""):
        findings.append((cls, s["name"], s["addr"], strength, msg))

    narrow, widths = {}, {}
    for pc, A, w, exact, base, st in accesses:
        o = owner(A)
        if not sec_of(A) or o is None:
            continue
        end_o = o["addr"] + extent(o)
        if A >= end_o:
            # starts past the owner's declared extent (inside a gap)
            if base is not None and exact and base in uni:
                bo = uni[base]
                add("BEYOND", bo, "pc %08X %s +0x%X w%d reaches 0x%X, extent 0x%X" %
                    (pc, "store" if st else "load", A - base, w, A + w - bo["addr"], extent(bo)),
                    "strong" if (not bo["auto"] or True) else "weak")
            else:
                add("IN_GAP", o, "pc %08X access %08X w%d is 0x%X past extent 0x%X" %
                    (pc, A, w, A - end_o, extent(o)))
        elif A + w > end_o:
            add("STRADDLE", o, "pc %08X access %08X w%d crosses end 0x%X" % (pc, A, w, end_o))
        elif base is not None and exact and base in uni:
            bo = uni[base]
            if A + w - bo["addr"] > extent(bo):
                add("BEYOND", bo, "pc %08X +0x%X w%d reaches 0x%X, extent 0x%X (into %s)" %
                    (pc, A - base, w, A + w - bo["addr"], extent(bo), o["name"]), "strong")
        if w < 4 and base is not None and exact and base in uni:
            narrow.setdefault(base, []).append(w)
    # WORDS
    for base, ws in narrow.items():
        s = uni[base]
        if s["type"] and s["type"] in NARROW_OK:
            continue
        ext = extent(s)
        n = ext // 4
        like = count_address_like(base, n, word_addrs, word_at, sec_of)
        if like:
            add("WORDS", s, "type %s, %d narrow accesses (min width %d), %d of %d words look like addresses" %
                (s["type"], len(ws), min(ws), like, n))
    # WORDS2: untyped symbol whose address-like words point at nothing code or a symbol ever names
    known = {x["addr"] for x in syms.values()} | code_refs | set(uni)
    uni2 = dict(uni)
    for v in data_refs:
        if v not in uni2 and sec_of(v) and sec_of(v)["name"] in (".data", ".rodata"):
            uni2[v] = dict(name="D_%08X" % v, addr=v, size=None, type=None, auto=True)
    starts2 = sorted(uni2)
    def extent2(s):
        i = bisect.bisect_right(starts2, s["addr"])
        sec = sec_of(s["addr"])
        end = min(starts2[i], sec["addr"] + sec["size"]) if i < len(starts2) else sec["addr"] + sec["size"]
        return s["size"] if s["size"] is not None else end - s["addr"]
    textsec = [x for x in secs if x["name"] == ".text"]
    def in_text(v):
        return any(t["addr"] <= v < t["addr"] + t["size"] for t in textsec)
    for addr in starts2:
        s = uni2[addr]
        if s["type"] and s["type"] not in ("u32", "s32", "u64", "s64"):
            continue
        sec = sec_of(addr)
        if sec["name"] not in (".data", ".rodata", ".sdata", ".lit4") or sec["type"] == 8:
            continue
        ext = extent2(s)
        n = ext // 4
        if n < 2:
            continue
        like = bad = nz = 0
        raw = [(word_at(addr + 4 * k) or 0) for k in range(n)]
        nz = sum(1 for v in raw if v)
        if nz and sum(1 for v in raw if all(0x20 <= (v >> sh & 255) < 0x7f or (v >> sh & 255) == 0 for sh in (0, 8, 16, 24))) * 2 > nz:
            continue
        for k, v in enumerate(raw):
            if addr + 4 * k not in word_addrs:
                continue
            if v >= 0x100000 and (sec_of(v) or in_text(v)):
                like += 1
                if v not in known and not (in_text(v) and v % 4 == 0):
                    bad += 1
        if only_dbg and s["name"] in only_dbg:
            print("DBG", s["name"], hex(ext), "n", n, "nz", nz, "like", like, "bad", bad, file=sys.stderr)
        if bad >= 4 and bad * 5 >= like * 4 and bad * 5 >= nz:
            add("WORDS", s, "type %s, %d of %d words look like addresses, %d of those name no symbol or code reference (u8/u16 data?)" %
                (s["type"], like, n, bad))
    # TAIL / gaps
    for addr in starts:
        s = uni[addr]
        if s["size"] is None:
            continue
        gend = next_start(addr)
        gap = gend - (addr + s["size"])
        if gap <= 0:
            continue
        gb = [(word_at(addr + s["size"] + k) or 0) for k in range(0, gap - gap % 4, 4)]
        if gap < 16 and not any(gb) and gap % 4 == 0:
            continue
        gs = addr + s["size"]
        refd = gs in code_refs or gs in data_refs or gs in uni
        if refd or not any(gb[:4]):
            continue
        add("TAIL", s, "size 0x%X, 0x%X bytes before %s, %d nonzero words, gap start is referenced by nothing" %
            (s["size"], gap, owner(gend)["name"] if owner(gend) else "?", sum(1 for x in gb if x)))
    # CSIZE
    if a.csizes:
        cs = json.load(open(a.csizes))["symbols"]
        for addr in starts:
            s = uni[addr]
            if s["name"] not in cs:
                continue
            views = cs[s["name"]]
            sizes = sorted({v["size"] for v in views})
            hi = sizes[-1]
            gap = next_start(addr) - addr
            if len(sizes) > 1:
                add("CSIZE", s, "C views disagree: %s" % ", ".join("%s=%d" % (v["unit"].split("/")[-1], v["size"]) for v in views))
            if s["size"] is not None and hi > s["size"]:
                if hi > gap:
                    add("CSIZE", s, "C sizeof 0x%X > splat size 0x%X and > gap 0x%X to %s (C too big, or a symbol between is missing)" %
                        (hi, s["size"], gap, owner(next_start(addr)) and owner(next_start(addr))["name"]))
                else:
                    add("CSIZE", s, "C sizeof 0x%X > splat size 0x%X, gap is 0x%X (splat size too small)" % (hi, s["size"], gap))
            elif s["size"] is not None and sizes[0] < s["size"] - 15:
                add("CSIZE", s, "C sizeof 0x%X < splat size 0x%X (C too small)" % (sizes[0], s["size"]))
            elif s["size"] is None and hi > gap:
                add("CSIZE", s, "C sizeof 0x%X > gap 0x%X to next symbol, no splat size" % (hi, gap))
            elif s["size"] is None and sizes[0] < gap - 15:
                add("CSIZE", s, "C sizeof 0x%X < gap 0x%X to next symbol, no splat size" % (sizes[0], gap))
    jtbls = load_jtbls(ROOT / "asm")
    for u in uni.values():
        if u.get("size") is None:
            continue
        crossed = jtbl_crossings([(u["addr"], extent(u))], jtbls)
        if crossed:
            add("JTBL", u, "extent ends 0x%X, over %d jtbl_ label(s) from jtbl_%08X" %
                (crossed[0][1], len(crossed), crossed[0][2]))
    named = sorted(u["addr"] for u in uni.values() if not u["auto"])
    for u in uni.values():
        if u.get("size") is None:
            continue
        covered = cover_crossings([(u["addr"], extent(u))], named)
        if covered:
            add("COVER", u, "extent ends 0x%X, over %d symbol(s) from %s" %
                (covered[0][1], len(covered), owner(covered[0][2])["name"]))
    # dedupe
    seen, out = set(), []
    for f in findings:
        k = (f[0], f[1], f[4].split(" ", 2)[-1] if f[0] != "BEYOND" else f[4])
        if f[0] in ("BEYOND", "IN_GAP", "STRADDLE") and (f[0], f[1]) in seen:
            continue
        seen.add((f[0], f[1])) if f[0] in ("BEYOND", "IN_GAP", "STRADDLE") else None
        out.append(f)
    only = set(a.only.split(",")) if a.only else None
    cls = set(a.classes.split(",")) if a.classes else None
    counts = {}
    for f in sorted(out, key=lambda f: (f[0], f[2])):
        if only and f[1] not in only or cls and f[0] not in cls:
            continue
        counts[f[0]] = counts.get(f[0], 0) + 1
        print("%-8s %08X %-28s %s" % (f[0], f[2], f[1], f[4]))
    print("# counts (symbols with a finding of each class): " + ", ".join("%s=%d" % kv for kv in sorted(counts.items())), file=sys.stderr)


if __name__ == "__main__":
    main()
