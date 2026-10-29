#!/usr/bin/env python3
"""Run m2c with EE stack slots and raw float-to-word instructions recovered.

Upstream mipsee currently advances known trailing arguments by four bytes.
Adapt that model locally; already correct eight-byte layouts pass through.
Also expose CVT.W.S encodings the disassembler leaves as text-section words.
The installed dependency and input assembly are never modified.
"""
from dataclasses import replace
import io
import re


def install_ee_stack_slots() -> None:
    from m2c.arch_mips import MipseeArch

    original = MipseeArch.function_abi
    if getattr(original, "_smt3_stack_slots", False):
        return

    def function_abi(self, fn_sig, likely_regs, *, for_call):
        abi = original(self, fn_sig, likely_regs, for_call=for_call)
        stack = [slot for slot in abi.arg_slots
                 if slot.loc.reg is None and slot.loc.offset is not None]
        if len(stack) < 2:
            return abi
        offsets = [slot.loc.offset for slot in stack]
        if offsets == list(range(0, 8 * len(stack), 8)):
            return abi
        if offsets != list(range(0, 4 * len(stack), 4)):
            raise ValueError(f"unsupported m2c EE stack argument layout: {offsets}")
        if any(slot.type.get_size_bits() not in (8, 16, 32, 64) for slot in stack):
            raise ValueError("m2c EE stack-slot adaptation requires scalar arguments")
        for index, slot in enumerate(stack):
            slot.loc = replace(slot.loc, offset=8 * index)
        return abi

    function_abi._smt3_stack_slots = True
    MipseeArch.function_abi = function_abi


def normalize_ee_asm(text: str) -> str:
    """Expose raw single-to-word conversions as instructions, only in text."""
    in_text = True
    lines = []
    for line in text.splitlines(keepends=True):
        section = re.match(r"\s*\.section\s+([.\w]+)", line)
        short = re.match(r"\s*\.(text|data|rdata|rodata|sdata|bss|sbss|lit4|lit8)\b", line)
        if section or short:
            name = section[1] if section else "." + short[1]
            in_text = name.startswith(".text")
        word = re.match(r"([ \t]*(?:/\*.*?\*/[ \t]*)?)\.word[ \t]+"
            r"(0x[0-9A-Fa-f]{8})[ \t]*(?:(?:#|/\*).*)?(?:\r?\n)?$", line)
        if in_text and word:
            value = int(word[2], 16)
            if value & 0xFFFF003F == 0x46000024:
                fd, fs = (value >> 6) & 31, (value >> 11) & 31
                line = f"{word[1]}cvt.w.s $f{fd}, $f{fs}\n"
        lines.append(line)
    return "".join(lines)


def install_ee_instruction_reader() -> None:
    import m2c.main as driver
    from m2c.arch_mips import MipseeArch

    original = driver.parse_file
    if getattr(original, "_smt3_raw_conversions", False):
        return

    def parse_file(stream, arch, options):
        if not isinstance(arch, MipseeArch):
            return original(stream, arch, options)
        normalized = io.StringIO(normalize_ee_asm(stream.read()))
        normalized.name = stream.name
        return original(normalized, arch, options)

    parse_file._smt3_raw_conversions = True
    driver.parse_file = parse_file


def main() -> None:
    install_ee_stack_slots()
    install_ee_instruction_reader()
    from m2c.main import main as m2c_main
    m2c_main()


if __name__ == "__main__":
    main()
