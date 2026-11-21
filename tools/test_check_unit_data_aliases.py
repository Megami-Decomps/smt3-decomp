#!/usr/bin/env python3
"""Focused tests for non-owning .data aliases in the stock unit checker."""

import struct
import unittest
from dataclasses import replace

from check_unit import initialized_data_alias_problems, owned_rom_range
from symbol_aliases import SymbolAlias


UNIT = "field/waypoints"
START, ORIGIN = 0x100, 0x300000
SIZE = 0x20
PRIMARY = ("rows", 0, SIZE, 0x11, 1)  # global STT_OBJECT in .data
ALIAS = SymbolAlias("anchors", "rows", UNIT, ".data", 4, ORIGIN + 4, SIZE, 4, "u16")
UNDEFINED = ("anchors", 0, 4, 0x10, 0)
SYMS = {"rows": ORIGIN, "anchors": ORIGIN + 4}
YAML = """      - [0x100, .data, field/waypoints]
      - [0x120, data, unchanged_neighbor]
"""


def elf32(*, native=False, flags=3, kind=1, symbols=(), competing_data=False):
    """Small real little-endian ELF with .data, a symbol table, and strings."""
    strings = b"\0" + b"".join(name.encode() + b"\0" for name, *_ in symbols)
    symtab = bytes(16) + b"".join(
        struct.pack("<IIIBBH", strings.index(name.encode() + b"\0"),
                    value, size, info, 0, section)
        for name, value, size, info, section in symbols
    )
    names = b"\0.data\0.symtab\0.strtab\0.shstrtab\0"
    raw = bytearray(START + SIZE)
    null = (0,) * 10
    data = (1, kind, flags, ORIGIN if native else 0, START, SIZE, 0, 0, 4, 0)
    headers = [null, data]
    for name, payload, typ, link, info, width in (
            (".symtab", symtab, 2, 3, 1, 16),
            (".strtab", strings, 3, 0, 0, 0),
            (".shstrtab", names, 3, 0, 0, 0)):
        headers.append((names.index(name.encode()), typ, 0, 0, len(raw),
                        len(payload), link, info, 1, width))
        raw += payload
    if competing_data:
        headers.append(data)
    shoff = len(raw)
    raw += b"".join(struct.pack("<10I", *h) for h in headers)
    ident = b"\x7fELF\x01\x01\x01" + bytes(9)
    raw[:52] = struct.pack("<16sHHIIIIIHHHHHH", ident, 2 if native else 1,
                           8, 1, 0, 0, shoff, 0, 52, 32, 0, 40, len(headers), 4)
    return bytes(raw)


class InitializedDataAliasTests(unittest.TestCase):
    def check(self, *, yaml=YAML, aliases=(ALIAS,), syms=SYMS, retail=None,
              segs=None, skip=None, full=None, unit=UNIT):
        return initialized_data_alias_problems(
            unit, owned_rom_range(yaml, unit, ".data"), aliases, syms,
            elf32(native=True) if retail is None else retail,
            [(ORIGIN, START, SIZE)] if segs is None else segs,
            elf32(symbols=(PRIMARY, UNDEFINED)) if skip is None else skip,
            elf32(symbols=(PRIMARY, UNDEFINED)) if full is None else full)

    def test_native_primary_has_only_nonallocating_alias_references(self):
        second = SymbolAlias("switches", "rows", UNIT, ".data", 8,
                             ORIGIN + 8, SIZE, SIZE - 8, "u8")
        self.assertEqual(self.check(aliases=(ALIAS, second)), [])
        self.assertEqual(self.check(unit="field/reader",
                                   skip=elf32(symbols=(UNDEFINED,)),
                                   full=elf32(symbols=(UNDEFINED,))), [])

    def test_overlapping_views_require_valid_geometry_even_on_manual_objects(self):
        first = replace(ALIAS, record_bytes=4, alias_size=8)
        second = replace(first, name="switches", offset=8, address=ORIGIN + 8,
                         prefix_bytes=8, alias_size=4)
        syms = SYMS | {"switches": ORIGIN + 8}
        self.assertEqual(self.check(aliases=(first, second), syms=syms), [])
        for invalid in (replace(second, prefix_bytes=12), replace(second, prefix_bytes=True),
                        replace(second, record_bytes=None, prefix_bytes=0),
                        replace(second, owner_size=9), replace(second, alias_size=SIZE)):
            with self.subTest(invalid=invalid):
                self.assertTrue(self.check(aliases=(first, invalid), syms=syms))
        other_def = ("switches", 8, 4, 0x11, 1)
        false = elf32(symbols=(PRIMARY, UNDEFINED, other_def))
        self.assertTrue(self.check(aliases=(first, second), syms=syms, skip=false))
        self.assertTrue(self.check(aliases=(first, second), syms=syms, full=false))

    def test_ordinary_and_nobits_do_not_enter_elf_alias_checks(self):
        bss = SymbolAlias("tail", "bssRows", UNIT, ".bss", 4, ORIGIN + 4)
        for aliases in ((), (bss,)):
            self.assertEqual(self.check(aliases=aliases, retail=b"", segs=[],
                                        skip=b"", full=b""), [])

    def test_requires_one_correct_yaml_owner_and_complete_native_mapping(self):
        for yaml, segs in (
                ("", None), (YAML + YAML, None),
                (YAML.replace("0x120", "0x121"), None),
                (YAML, [(ORIGIN, START, SIZE - 1)]),
                (YAML, [(ORIGIN, START, SIZE // 2),
                        (ORIGIN + SIZE // 2, START + SIZE // 2, SIZE // 2)]),
                (YAML, [(ORIGIN, START, SIZE)] * 2),
                (YAML, [(ORIGIN + 4, START, SIZE)])):
            with self.subTest(yaml=yaml, segs=segs):
                self.assertTrue(self.check(yaml=yaml, segs=segs))

    def test_requires_actual_native_writable_allocated_progbits(self):
        for retail in (elf32(native=True, kind=8), elf32(native=True, flags=2),
                       elf32(native=True, flags=1), elf32(native=True, flags=7),
                       elf32(native=True, competing_data=True)):
            with self.subTest(retail=retail[18:24]):
                self.assertTrue(any("file-backed" in s for s in self.check(retail=retail)))

    def test_rejects_wrong_native_primary_or_alias_range(self):
        for alias, syms in (
                (ALIAS, {}), (ALIAS, {"rows": ORIGIN + 1}),
                (replace(ALIAS, owner_size=SIZE + 1), SYMS),
                (replace(ALIAS, alias_size=0), SYMS),
                (replace(ALIAS, alias_size=SIZE), SYMS),
                (replace(ALIAS, offset=0, address=ORIGIN), SYMS),
                (replace(ALIAS, address=ORIGIN + 8), SYMS)):
            with self.subTest(alias=alias, syms=syms):
                self.assertTrue(self.check(aliases=(alias,), syms=syms))

    def test_each_context_requires_exact_strong_primary(self):
        for field, label in (("skip", "standalone"), ("full", "full-unit")):
            for rows in (
                    (), (PRIMARY, PRIMARY),
                    (("rows", 1, SIZE, 0x11, 1),),
                    (("rows", 0, SIZE - 1, 0x11, 1),),
                    (("rows", 0, SIZE, 0x10, 1),),  # NOTYPE
                    (("rows", 0, SIZE, 0x01, 1),),  # local
                    (("rows", 0, SIZE, 0x21, 1),),  # weak
                    (("rows", 0, SIZE, 0x11, 0xFFF1),),  # ABS
                    (("rows", 0, SIZE, 0x11, 0),)):  # undefined
                with self.subTest(field=field, rows=rows):
                    problems = self.check(**{field: elf32(symbols=rows)})
                    self.assertTrue(any(label in s and "primary rows" in s for s in problems))

    def test_competing_alias_fails_even_if_zero_size_or_different_section(self):
        for field, label in (("skip", "standalone"), ("full", "full-unit")):
            for section in (1, 3, 0xFFF1, 0xFFF2):  # same, other, ABS, COMMON
                with self.subTest(field=field, section=section):
                    blob = elf32(symbols=(PRIMARY, ("anchors", 4, 0, 0x10, section)))
                    self.assertIn(f"{label}: competing emitted definition of .data alias anchors",
                                  self.check(**{field: blob}))
        blob = elf32(symbols=(("anchors", 0, 0, 0x10, 0xFFF1),))
        self.assertTrue(any("competing emitted" in s for s in self.check(
            unit="field/reader", skip=blob, full=blob)))

    def test_primary_in_wrong_unit_does_not_confer_ownership(self):
        self.assertTrue(any("outside its declared" in s for s in self.check(
            unit="field/reader")))

    def test_requires_actual_writable_file_section_in_each_object(self):
        for field in ("skip", "full"):
            for kwargs in ({"flags": 2}, {"kind": 8}, {"competing_data": True}):
                with self.subTest(field=field, kwargs=kwargs):
                    self.assertTrue(any("file-backed" in s for s in self.check(
                        **{field: elf32(symbols=(PRIMARY,), **kwargs)})))


if __name__ == "__main__":
    unittest.main()
