#!/usr/bin/env python3
"""Assemble VU microcode source, in dvp-as syntax, into the words the VU runs.

  tools/vuas.py src/smt3/sdf/vu/vu1_micro.vsm -o build/smt3/vu/vu1_micro.bin
"""

import argparse
import importlib.util
import re
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent

spec = importlib.util.spec_from_file_location("vu2c", ROOT / "tools" / "vu2c.py")
vu2c = importlib.util.module_from_spec(spec)
spec.loader.exec_module(vu2c)

FIELDS = "xyzw"
# Where each half's fields sit, as vu2c's decoder reads them back out.
MASK_SHIFT = 21
BC_OF = {c: i for i, c in enumerate(FIELDS)}

UPPER_OPS = {name: op for op, name in enumerate(vu2c.UPPER_MAIN) if name}
# op 0x3C..0x3F take the fd field as a second opcode, so those forms have no fd register.
UPPER_FD_OPS = {name: (0x3C + i, sub)
                for i, table in enumerate(vu2c.UPPER_FD) for sub, name in table.items() if name}
LOWER_OPS = {name: op for op, name in vu2c.LOWER_MAIN.items() if name}
LOWER_SPECIAL_OPS = {name: sub for sub, name in vu2c.LOWER_SPECIAL.items()}
LOWER_T3_OPS = {name: (0x3C + i, sub)
                for i, table in enumerate(vu2c.LOWER_T3) for sub, name in table.items() if name}

# The assembler is case-insensitive; the decode tables spell a suffix in lower case.
CANON = {name.upper(): name for name in
         list(UPPER_OPS) + list(UPPER_FD_OPS) + list(LOWER_OPS)
         + list(LOWER_SPECIAL_OPS) + list(LOWER_T3_OPS) + ["NOP", "LOI"]}
# CLIP only ever tests vft's w, and the assembler wants that written as a suffix.
CANON["CLIPW"] = "CLIP"

ACC_DEST = {"MULA", "MADDA", "MSUBA", "ADDA", "SUBA", "OPMULA"}
ONE_SOURCE = {"ABS", "FTOI0", "FTOI4", "FTOI12", "FTOI15", "ITOF0", "ITOF4", "ITOF12", "ITOF15"}
SUFFIX = "xyzwiq"
FLAG_BIT = {"i": 31, "e": 30, "m": 29, "d": 28, "t": 27}
# A branch displacement is 11 bits, and the hardware wraps it inside micro memory.
MICRO_INSTRUCTIONS = 2048


class Error(Exception):
    pass


def split_name(token):
    """(name, flags, mask) for `MADDz[e].xyzw`."""
    m = re.fullmatch(r"([A-Za-z0-9]+)((?:\[[a-zA-Z]\])*)(?:\.([xyzwXYZW]+))?", token)
    if not m:
        raise Error("cannot read mnemonic %r" % token)
    name = CANON.get(m.group(1).upper())
    if name is None:
        raise Error("unknown instruction %r" % m.group(1))
    return name, re.findall(r"\[([a-zA-Z])\]", m.group(2)), (m.group(3) or "").lower()


def mask_bits(mask):
    return sum(1 << (24 - FIELDS.index(c)) for c in mask)


def reg(token, kind):
    m = re.fullmatch(r"v%s(\d+)([xyzw]?)" % kind, token, re.I)
    if not m:
        raise Error("expected a v%s register, got %r" % (kind, token))
    return int(m.group(1)), m.group(2).lower()


def base_of(name):
    """The name without its source suffix, which is what decides the operand shape."""
    if len(name) > 3 and name[-1].lower() in SUFFIX:
        return name[:-1]
    return name


def upper_operands(name):
    base = base_of(name)
    if name == "NOP":
        return 0
    if base in ONE_SOURCE or name in ONE_SOURCE or base == "CLIP":
        return 2
    return 3


def encode_upper(name, flags, mask, operands):
    if name in UPPER_FD_OPS:
        op, sub = UPPER_FD_OPS[name]
        word = op | (sub << 6)
    elif name in UPPER_OPS:
        word = UPPER_OPS[name]
        sub = None
    else:
        raise Error("unknown upper instruction %r" % name)
    word |= mask_bits(mask)
    for flag in flags:
        if flag.lower() not in FLAG_BIT:
            raise Error("unknown flag [%s]" % flag)
        word |= 1 << FLAG_BIT[flag.lower()]
    if name == "NOP":
        return word
    base = base_of(name)
    fd = fs = ft = 0
    if base in ONE_SOURCE or name in ONE_SOURCE or base == "CLIP":
        # NAME.mask vft, vfs for the converts, CLIPw.mask vfs, vft for the clip.
        first, second = operands
        if base == "CLIP":
            fs, ft = reg(first, "f")[0], reg(second, "f")[0]
        else:
            ft, fs = reg(first, "f")[0], reg(second, "f")[0]
    else:
        dest, src, third = operands
        fs = reg(src, "f")[0]
        if base in ACC_DEST:
            if dest.upper() != "ACC":
                raise Error("%s writes ACC, not %r" % (name, dest))
        else:
            fd = reg(dest, "f")[0]
        if third.upper() not in ("I", "Q"):
            ft = reg(third, "f")[0]
    return word | (ft << 16) | (fs << 11) | (fd << 6) if sub is None \
        else word | (ft << 16) | (fs << 11)


LOWER_SHAPE = {
    "NOP": 0, "WAITQ": 0, "WAITP": 0, "LOI": 1,
    "LQ": 2, "SQ": 2, "ILW": 2, "ISW": 2, "LQI": 2, "LQD": 2, "SQI": 2, "SQD": 2,
    "IADDIU": 3, "ISUBIU": 3, "IADDI": 3, "IADD": 3, "ISUB": 3, "IAND": 3, "IOR": 3,
    "IBEQ": 3, "IBNE": 3, "IBLTZ": 2, "IBGTZ": 2, "IBLEZ": 2, "IBGEZ": 2,
    "B": 1, "BAL": 2, "JR": 1, "JALR": 2,
    "DIV": 3, "RSQRT": 3, "SQRT": 2, "MOVE": 2, "MR32": 2, "MFIR": 2, "MTIR": 2, "MFP": 2,
    "XTOP": 1, "XITOP": 1, "XGKICK": 1, "FCGET": 1, "FCSET": 1, "FCAND": 2, "FCOR": 2,
    "FCEQ": 2, "FMAND": 2, "FMOR": 2, "FMEQ": 2, "FSAND": 2, "FSOR": 2, "FSEQ": 2,
    "FSSET": 2, "ESADD": 2, "ERSADD": 2, "ELENG": 2, "ERLENG": 2, "ESQRT": 2, "ERSQRT": 2,
    "ERCPR": 2, "ESUM": 2, "ESIN": 2, "EEXP": 2, "EATAN": 2,
}


def offset_operand(token):
    """(immediate, register) for `800(vi04)`, `(vi05++)` or `(--vi05)`."""
    m = re.fullmatch(r"(-?\d+)?\((--)?vi(\d+)(\+\+)?\)", token, re.I)
    if not m:
        raise Error("expected an address like 8(vi01), got %r" % token)
    return int(m.group(1) or 0), int(m.group(3))


def number(token):
    return int(token, 0)


def lower_word(name, mask, operands, at, labels):
    """The lower half's 32 bits."""
    def base_word():
        if name in LOWER_OPS:
            return LOWER_OPS[name] << 25
        if name in LOWER_SPECIAL_OPS:
            return (0x40 << 25) | LOWER_SPECIAL_OPS[name]
        if name in LOWER_T3_OPS:
            sub, which = LOWER_T3_OPS[name]
            return (0x40 << 25) | sub | (which << 6)
        raise Error("unknown lower instruction %r" % name)

    def target(token):
        """A branch displacement, in instructions, from a label and an optional byte term."""
        m = re.fullmatch(r"([A-Za-z_.$][\w.$]*)(?:([-+])(\d+))?", token)
        if not m:
            raise Error("expected a branch target, got %r" % token)
        if m.group(1) not in labels:
            raise Error("no label %r" % m.group(1))
        where = labels[m.group(1)]
        if m.group(2):
            where += int(m.group(3)) // 8 * (1 if m.group(2) == "+" else -1)
        return (where - at - 1) & 0x7FF

    if name == "NOP":
        # The assembler's NOP is a MOVE of vf00 into vf00, which is what the decoder reads back.
        return (0x40 << 25) | 0x3C | (12 << 6)
    if name == "LOI":
        # The immediate is the whole lower half; the upper's I bit is what marks it.
        return struct.unpack("<I", struct.pack("<f", float(operands[0])))[0]
    word = base_word()
    if name in ("WAITQ", "WAITP"):
        return word
    if name in ("LQ", "ILW", "ISW"):
        imm, vi = offset_operand(operands[1])
        rt = reg(operands[0], "f" if name == "LQ" else "i")[0]
        return word | mask_bits(mask) | (rt << 16) | (vi << 11) | (imm & 0x7FF)
    if name == "SQ":
        imm, vi = offset_operand(operands[1])
        return word | mask_bits(mask) | (reg(operands[0], "f")[0] << 11) | (vi << 16) \
            | (imm & 0x7FF)
    if name in ("LQI", "LQD"):
        _, vi = offset_operand(operands[1])
        return word | mask_bits(mask) | (reg(operands[0], "f")[0] << 16) | (vi << 11)
    if name in ("SQI", "SQD"):
        _, vi = offset_operand(operands[1])
        return word | mask_bits(mask) | (reg(operands[0], "f")[0] << 11) | (vi << 16)
    if name in ("IADDIU", "ISUBIU"):
        it, is_ = reg(operands[0], "i")[0], reg(operands[1], "i")[0]
        imm = number(operands[2]) & 0x7FFF
        return word | (it << 16) | (is_ << 11) | ((imm & 0x7800) << 10) | (imm & 0x7FF)
    if name == "IADDI":
        it, is_ = reg(operands[0], "i")[0], reg(operands[1], "i")[0]
        return word | (it << 16) | (is_ << 11) | ((number(operands[2]) & 0x1F) << 6)
    if name in ("IADD", "ISUB", "IAND", "IOR"):
        id_, is_, it = (reg(o, "i")[0] for o in operands)
        return word | (it << 16) | (is_ << 11) | (id_ << 6)
    if name in ("IBEQ", "IBNE"):
        it, is_ = reg(operands[0], "i")[0], reg(operands[1], "i")[0]
        return word | (it << 16) | (is_ << 11) | target(operands[2])
    if name in ("IBLTZ", "IBGTZ", "IBLEZ", "IBGEZ"):
        return word | (reg(operands[0], "i")[0] << 11) | target(operands[1])
    if name == "B":
        return word | target(operands[0])
    if name == "BAL":
        return word | (reg(operands[0], "i")[0] << 16) | target(operands[1])
    if name == "JR":
        return word | (reg(operands[0], "i")[0] << 11)
    if name == "JALR":
        return word | (reg(operands[0], "i")[0] << 16) | (reg(operands[1], "i")[0] << 11)
    if name in ("DIV", "RSQRT"):
        if operands[0].upper() != "Q":
            raise Error("%s writes Q, not %r" % (name, operands[0]))
        fs, fsf = reg(operands[1], "f")
        ft, ftf = reg(operands[2], "f")
        return word | (ft << 16) | (fs << 11) | (BC_OF[ftf] << 23) | (BC_OF[fsf] << 21)
    if name == "SQRT":
        ft, ftf = reg(operands[1], "f")
        return word | (ft << 16) | (BC_OF[ftf] << 23)
    if name in ("MOVE", "MR32"):
        return word | mask_bits(mask) | (reg(operands[0], "f")[0] << 16) \
            | (reg(operands[1], "f")[0] << 11)
    if name == "MFIR":
        return word | mask_bits(mask) | (reg(operands[0], "f")[0] << 16) \
            | (reg(operands[1], "i")[0] << 11)
    if name == "MTIR":
        fs, fsf = reg(operands[1], "f")
        return word | (reg(operands[0], "i")[0] << 16) | (fs << 11) | (BC_OF[fsf] << 21)
    if name == "MFP":
        return word | mask_bits(mask) | (reg(operands[0], "f")[0] << 16)
    if name in ("XTOP", "XITOP", "FCGET"):
        return word | (reg(operands[0], "i")[0] << 16)
    if name == "XGKICK":
        return word | (reg(operands[0], "i")[0] << 11)
    if name == "FCSET":
        return word | (number(operands[0]) & 0xFFFFFF)
    if name in ("FCAND", "FCOR", "FCEQ"):
        return word | (number(operands[1]) & 0xFFFFFF)
    if name in ("FMAND", "FMOR", "FMEQ"):
        return word | (reg(operands[0], "i")[0] << 16) | (reg(operands[1], "i")[0] << 11)
    if name in ("FSAND", "FSOR", "FSEQ", "FSSET"):
        imm = number(operands[1]) & 0xFFF
        return word | (reg(operands[0], "i")[0] << 16) | ((imm & 0x800) << 10) | (imm & 0x7FF)
    if name.startswith("E"):
        fs, fsf = reg(operands[1], "f")
        return word | (fs << 11) | (BC_OF[fsf] << 21)
    raise Error("no encoding for lower %r" % name)


def statements(text):
    """[(line number, label or None, tokens)] for every line that holds something."""
    out = []
    for number_, raw in enumerate(text.splitlines(), 1):
        line = raw.split(";")[0].rstrip()
        if not line.strip():
            continue
        label = None
        stripped = line.strip()
        if ":" in stripped:
            head, _, stripped = stripped.partition(":")
            label = head.strip()
        if stripped.startswith("."):          # a directive: .vu and friends carry no code
            stripped = ""
        tokens = [t for t in re.split(r"[,\s]+", stripped) if t]
        out.append((number_, label, tokens))
    return out


def assemble(text):
    """The instruction words this source assembles to."""
    rows, labels, at = [], {}, 0
    for number_, label, tokens in statements(text):
        if label:
            labels[label] = at
        if tokens:
            rows.append((number_, at, tokens))
            at += 1
    words = []
    for number_, addr, tokens in rows:
        try:
            name, flags, mask = split_name(tokens[0])
            count = upper_operands(name)
            upper_ops, rest = tokens[1:1 + count], tokens[1 + count:]
            if not rest:
                raise Error("no lower instruction on the line")
            lname, lflags, lmask = split_name(rest[0])
            if lflags:
                raise Error("a flag belongs on the upper half, not on %s" % lname)
            if lname not in LOWER_SHAPE:
                raise Error("unknown lower instruction %r" % lname)
            lower_ops = rest[1:1 + LOWER_SHAPE[lname]]
            if len(lower_ops) != LOWER_SHAPE[lname] or len(upper_ops) != count:
                raise Error("%s/%s wants %d and %d operands"
                            % (name, lname, count, LOWER_SHAPE[lname]))
            if len(rest) > 1 + LOWER_SHAPE[lname]:
                raise Error("extra operands: %s" % " ".join(rest[1 + LOWER_SHAPE[lname]:]))
            if lname == "LOI":
                flags = list(flags) + ["i"]
            upper = encode_upper(name, flags, mask, upper_ops)
            lower = lower_word(lname, lmask, lower_ops, addr, labels)
        except Error as exc:
            raise SystemExit("line %d: %s" % (number_, exc))
        words.append((upper, lower))
    return words


def to_bytes(words):
    return b"".join(struct.pack("<2I", lower, upper) for upper, lower in words)


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("source", type=Path)
    ap.add_argument("-o", "--out", type=Path, required=True)
    args = ap.parse_args(argv)
    data = to_bytes(assemble(args.source.read_text()))
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_bytes(data)
    return 0


if __name__ == "__main__":
    sys.exit(main())
