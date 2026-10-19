#!/usr/bin/env python3
"""Disassemble a VU image into assembler source, from vu2c.py's decode tables.

  tools/vudis.py --vu 1 -o src/smt3/sdf/vu/vu1_micro.vsm
  tools/vudis.py --vu 1 --verify     assemble with dvp-as and diff against the ELF
"""

import argparse
import importlib.util
import os
import re
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent


def load(name):
    spec = importlib.util.spec_from_file_location(name, ROOT / "tools" / (name + ".py"))
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


vu2c = load("vu2c")
vutext = load("vutext")

# The original assembler, for --verify. Not needed to build: tools/vuas.py is.
DVP_AS = os.environ.get("DVP_AS", "dvp-as")

# Upper instructions whose destination is the accumulator rather than a register. The table name
# carries the broadcast letter, so the base name is what decides the shape.
ACC_DEST = {"MULA", "MADDA", "MSUBA", "ADDA", "SUBA", "OPMULA"}
# Upper instructions that take one source and write one register.
ONE_SOURCE = {"ABS", "FTOI0", "FTOI4", "FTOI12", "FTOI15", "ITOF0", "ITOF4", "ITOF12", "ITOF15"}
# A trailing letter selects the second source: a lane of vft, or the I or Q register.
SUFFIX = "xyzwiq"


def split_bc(name):
    """(base name, source suffix) for an upper instruction, or (name, "")."""
    if len(name) > 3 and name[-1] in SUFFIX:
        return name[:-1], name[-1]
    return name, ""


def upper_text(ins, flag=""):
    """The upper half as dvp-as writes it, with any flag between the mnemonic and the mask."""
    name, f = ins.uname, ins.uf
    if name is None or name == "NOP":
        return "NOP" + flag
    base, bc = split_bc(name)
    name += flag
    mask = "." + (f.mask or "")
    vfs, vft, vfd = "vf%02d" % f.fs, "vf%02d" % f.ft, "vf%02d" % f.fd
    third = {"i": "I", "q": "Q"}.get(bc, vft)
    if base.rstrip("i").rstrip("q") in ONE_SOURCE or base in ONE_SOURCE:
        return "%s%s %s, %s" % (name, mask, vft, vfs)
    if base == "CLIP":
        # CLIP always tests vft's w, and the assembler wants that written as the suffix. It
        # writes no register, but the mask bits still vary between instances and are kept.
        return "CLIPw%s%s %s, %s" % (flag, mask, vfs, vft)
    if base in ACC_DEST:
        return "%s%s ACC, %s, %s" % (name, mask, vfs, third)
    return "%s%s %s, %s, %s" % (name, mask, vfd, vfs, third)


def lower_text(ins, target):
    """The lower half as dvp-as writes it."""
    name, f = ins.lname, ins.lf
    if name is None:
        return "NOP"
    vfs, vft = "vf%02d" % f.fs, "vf%02d" % f.ft
    vis, vit, vid = "vi%02d" % f.is_, "vi%02d" % f.it, "vi%02d" % f.id_
    mask = "." + (f.mask or "")
    if name in ("NOP", "WAITQ", "WAITP"):
        return name
    if name == "LOI":
        return "LOI %.9g" % struct.unpack("<f", struct.pack("<I", f.code))[0]
    if name in ("LQ", "ILW", "ISW"):
        reg, imm = (vft, f.imm11) if name == "LQ" else (vit, f.imm11)
        return "%s%s %s, %d(%s)" % (name, mask, reg, imm, vis)
    if name == "SQ":
        return "SQ%s %s, %d(%s)" % (mask, vfs, f.imm11, vit)
    if name in ("LQI", "LQD"):
        step = "(%s++)" % vis if name == "LQI" else "(--%s)" % vis
        return "%s%s %s, %s" % (name, mask, vft, step)
    if name in ("SQI", "SQD"):
        step = "(%s++)" % vit if name == "SQI" else "(--%s)" % vit
        return "%s%s %s, %s" % (name, mask, vfs, step)
    if name in ("IADDIU", "ISUBIU"):
        return "%s %s, %s, %d" % (name, vit, vis, f.imm15)
    if name == "IADDI":
        return "IADDI %s, %s, %d" % (vit, vis, f.imm5)
    if name in ("IADD", "ISUB", "IAND", "IOR"):
        return "%s %s, %s, %s" % (name, vid, vis, vit)
    if name in vu2c.BRANCH_COND:
        where = target(ins)
        if name in ("IBEQ", "IBNE"):
            # The assembler takes the it register first, which is not the order a decode prints.
            return "%s %s, %s, %s" % (name, vit, vis, where)
        return "%s %s, %s" % (name, vis, where)
    if name == "B":
        return "B %s" % target(ins)
    if name == "BAL":
        return "BAL %s, %s" % (vit, target(ins))
    if name == "JR":
        return "JR %s" % vis
    if name == "JALR":
        return "JALR %s, %s" % (vit, vis)
    if name == "DIV":
        return "DIV Q, %s%s, %s%s" % (vfs, f.fsf, vft, f.ftf)
    if name == "RSQRT":
        return "RSQRT Q, %s%s, %s%s" % (vfs, f.fsf, vft, f.ftf)
    if name == "SQRT":
        return "SQRT Q, %s%s" % (vft, f.ftf)
    if name.startswith("E"):
        return "%s P, %s%s" % (name, vfs, f.fsf)
    if name in ("MOVE", "MR32"):
        return "%s%s %s, %s" % (name, mask, vft, vfs)
    if name == "MFIR":
        return "MFIR%s %s, %s" % (mask, vft, vis)
    if name == "MTIR":
        return "MTIR %s, %s%s" % (vit, vfs, f.fsf)
    if name == "MFP":
        return "MFP%s %s, P" % (mask, vft)
    if name in ("XTOP", "XITOP"):
        return "%s %s" % (name, vit)
    if name == "XGKICK":
        return "XGKICK %s" % vis
    if name == "FCGET":
        return "FCGET %s" % vit
    if name == "FCSET":
        return "FCSET 0x%x" % f.imm24
    if name in ("FCAND", "FCOR"):
        return "%s vi01, 0x%x" % (name, f.imm24)
    if name in ("FCEQ",):
        return "FCEQ vi01, 0x%x" % f.imm24
    if name in ("FMAND", "FMOR", "FMEQ"):
        return "%s %s, %s" % (name, vit, vis)
    if name in ("FSAND", "FSOR", "FSEQ", "FSSET"):
        return "%s %s, 0x%x" % (name, vit, f.imm12)
    raise ValueError("no syntax for lower %s at 0x%04X" % (name, ins.addr))


def flags(ins):
    """The bits the assembler writes as suffixes on the upper half."""
    out = ""
    # The I bit is not written: the assembler sets it from the LOI in the lower half.
    if ins.ebit:
        out += "[e]"
    if ins.upper_word & (1 << 29):
        out += "[m]"
    if ins.upper_word & (1 << 28):
        out += "[d]"
    if ins.upper_word & (1 << 27):
        out += "[t]"
    return out


def annotate(ins):
    """What the instruction does, as one pseudo-C statement per half.

    vu2c.py renders it from the same decode; control flow is left out, since the label in
    the operand already says where it goes.
    """
    out = list(vu2c.render_upper(ins.uname, ins.uf)[0]) if ins.uname else []
    if ins.ibit:
        out.append("I = %g;" % struct.unpack("<f", struct.pack("<I", ins.lower_word))[0])
    elif ins.lname and not ins.branch:
        out += list(vu2c.render_lower(ins.lname, ins.lf)[0])
    return " ".join(t for t in out if not t.startswith("/*"))


def routines(path):
    """[(lo, hi, name, what it is)] from the transcription's declared ranges."""
    if not path.exists():
        return []
    out, listing = [], False
    for line in path.read_text().splitlines():
        if "Ranges covered" in line:
            listing = True
            continue
        if not listing:
            continue
        m = re.match(r"\s*\*\s+0x([0-9A-Fa-f]{4})\.\.0x([0-9A-Fa-f]{4})\s+(\w+)\s*(.*)", line)
        if m:
            out.append((int(m.group(1), 16), int(m.group(2), 16), m.group(3), m.group(4).strip()))
        elif not line.strip(" *"):
            break
    return out


def decode(micro):
    return [vu2c.Ins(i // 8, *struct.unpack_from("<2I", micro, i)[::-1])
            for i in range(0, len(micro), 8)]


def render(micro, labels, header, ranges=()):
    """The whole image as dvp-as source."""
    code = decode(micro)
    wanted = {ins.target for ins in code if ins.target is not None}
    banner = {lo: (name, what) for lo, _, name, what in ranges}

    def label_at(addr):
        return labels.get(addr) or "L_%04X" % addr

    def target_at(ins):
        """The label, plus the wrap the hardware applies where the real displacement needs it.

        A branch displacement is 11 bits signed, so a target the assembler would reach with a
        larger number is encoded as the wrapped one; the term makes the assembler write the
        instruction's own bits rather than complain and truncate.
        """
        label, natural = label_at(ins.target), ins.target - ins.addr - 1
        if natural == ins.lf.imm11:
            return label
        return "%s%s%d" % (label, "+" if ins.lf.imm11 > natural else "-",
                           abs(ins.lf.imm11 - natural) * 8)

    out = [header, "\t.vu", ""]
    for ins in code:
        if ins.addr in banner:
            name, what = banner[ins.addr]
            hi = next(h for lo, h, n, _ in ranges if lo == ins.addr and n == name)
            out += ["", "; ---- %s, 0x%04X..0x%04X%s" % (name, ins.addr, hi,
                                                         ": " + what if what else "")]
        if ins.addr in wanted or ins.addr in labels:
            out.append("%s:" % label_at(ins.addr))
        upper = upper_text(ins, flags(ins))
        lower = lower_text(ins, target_at)
        note = annotate(ins)
        out.append("\t%-38s %-32s ; %04X %08X %08X%s"
                   % (upper, lower, ins.addr, ins.upper_word, ins.lower_word,
                      "  " + note if note else ""))
    return "\n".join(out) + "\n"


def assemble(text):
    """The .vutext dvp-as produces for this source."""
    with tempfile.TemporaryDirectory() as tmp:
        src, obj = Path(tmp) / "a.vsm", Path(tmp) / "a.o"
        src.write_text(text)
        done = subprocess.run([DVP_AS, "-o", str(obj), str(src)],
                              capture_output=True, text=True)
        if done.returncode:
            return None, done.stderr
        elf = obj.read_bytes()
        addr, off, size = vutext.sections(elf)[".vutext"]
        return elf[off:off + size], done.stderr


def verify(micro, text, limit=20):
    """Diff the assembled source against the image, instruction by instruction."""
    got, err = assemble(text)
    lines = [l for l in text.splitlines() if l.startswith("\t") and not l.startswith("\t.")]
    if got is None:
        print(err.strip()[:4000])
        return 1
    if err.strip():
        print(err.strip()[:2000])
    bad = 0
    if len(got) > len(micro) and got[len(micro):].strip(b"\0"):
        print("%d bytes of unexpected tail" % (len(got) - len(micro)))
        bad += 1
    for i in range(0, len(micro), 8):
        want, have = micro[i:i + 8], got[i:i + 8]
        if want == have:
            continue
        bad += 1
        if bad <= limit:
            wl, wu = struct.unpack("<2I", want) if len(want) == 8 else (0, 0)
            hl, hu = struct.unpack("<2I", have) if len(have) == 8 else (0, 0)
            print("0x%04X want %08X %08X got %08X %08X\n        %s"
                  % (i // 8, wu, wl, hu, hl,
                     lines[i // 8].strip() if i // 8 < len(lines) else "(missing)"))
    print("%d of %d instructions differ" % (bad, len(micro) // 8))
    return bad


def labels_for(vu, micro, names_path, symbols_path):
    """{address: label} from the routine names and the entry symbols."""
    if vu != 1:
        return {}
    labels = {}
    for at, name in vutext.routine_names(names_path).items():
        labels[at] = name
    base = 0x002F18E8
    for at, symbol in vutext.symbol_entries(symbols_path, base).items():
        labels.setdefault(at, symbol)
    return labels


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--vu", type=int, choices=(0, 1), default=1)
    ap.add_argument("--elf", type=Path, default=vutext.DEFAULT_ELF)
    ap.add_argument("--names", type=Path, default=ROOT / "docs" / "vu" / "vu1_names.txt")
    ap.add_argument("--symbols", type=Path,
                    default=ROOT / "config" / "smt3" / "symbol_addrs.txt")
    ap.add_argument("--transcription", type=Path,
                    default=ROOT / "docs" / "vu" / "vu1_annotated.c",
                    help="where the routine ranges and their one-line descriptions come from")
    ap.add_argument("-o", "--out", type=Path)
    ap.add_argument("--verify", action="store_true")
    args = ap.parse_args(argv)

    elf = args.elf.read_bytes()
    img = next(i for i in vutext.images(elf, vutext.sections(elf)) if i["vu"] == args.vu)
    micro = img["micro"]
    labels = labels_for(args.vu, micro, args.names, args.symbols)
    header = ("; VU%d microcode of SLUS_209.11, from .vutext's %s.\n"
              "; Generated by tools/vudis.py; sha1 of the image is %s."
              % (args.vu, img["name"], vutext.image_hash(micro)))
    text = render(micro, labels, header,
                  routines(args.transcription) if args.vu == 1 else [])
    if args.out:
        args.out.parent.mkdir(parents=True, exist_ok=True)
        args.out.write_text(text)
        print("%d instructions -> %s" % (len(micro) // 8, args.out))
    if args.verify or not args.out:
        return 1 if verify(micro, text) else 0
    return 0


if __name__ == "__main__":
    sys.exit(main())
