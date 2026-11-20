"""Regression tests for explicit, non-allocating native data aliases."""

import json
import hashlib
from pathlib import Path
import struct
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import configure
from tools.symbol_aliases import linker_aliases, load_symbol_aliases


class AliasTests(unittest.TestCase):
    def setUp(self):
        tmp = tempfile.TemporaryDirectory()
        self.addCleanup(tmp.cleanup)
        self.root = Path(tmp.name)
        self.cfg = self.root / "config/smt3"
        self.cfg.mkdir(parents=True)
        self.native = (
            "owner = 0x303050; // size:0x64A0\n"
            "anchor = 0x303054; // type:u16 size:0x2\n"
            "actors = 0x3030F0; // type:u8 size:0x6400\n"
            "owner = 0x303050;\n"
        )

    def row(self, name="anchor", offset=4, **updates):
        row = dict({"name": name, "owner": "owner", "unit": "field/fldWap",
                    "section": ".data", "offset": offset}, **updates)
        if row["section"] == ".data":
            row.setdefault("owner_size", 0x64A0)
        return row

    def load(self, rows, native=None):
        (self.cfg / "symbol_addrs.txt").write_text(self.native if native is None else native)
        (self.cfg / "symbol_aliases.json").write_text(json.dumps(rows))
        return load_symbol_aliases(self.root, "smt3")

    def test_native_data_sizes_types_and_linker_idempotence(self):
        anchor, actors = self.load([self.row(), self.row("actors", 0xA0)])
        self.assertEqual((anchor.address, anchor.owner_size, anchor.alias_size, anchor.native_type),
                         (0x303054, 0x64A0, 2, "u16"))
        self.assertEqual((actors.address, actors.owner_size, actors.alias_size, actors.native_type),
                         (0x3030F0, 0x64A0, 0x6400, "u8"))
        generated = linker_aliases("unrelated = 0x123;\n", [anchor, actors])
        self.assertIn("PROVIDE(anchor = owner + 0x4);", generated)
        self.assertIn("PROVIDE(actors = owner + 0xA0);", generated)
        self.assertEqual(linker_aliases(generated, [anchor, actors]), generated)
        unsized_owner = self.native.replace("// size:0x64A0", "")
        self.assertEqual(self.load([self.row()], unsized_owner)[0].owner_size, 0x64A0)

    def test_reject_data_without_exact_native_metadata(self):
        for native in (
            self.native.replace("size:0x6400", ""),
            self.native.replace("type:u16", ""),
            self.native.replace("type:u16", "type:func"),
            self.native.replace("type:u16", "type:label"),
            self.native.replace("size:0x2", "size:0x0"),
            self.native.replace("size:0x2", "size:0x3"),
            self.native.replace("size:0x64A0", "size:0x649F"),
            self.native.replace("owner = 0x303050;\n", "owner = 0x303051;\n"),
            self.native.replace("owner = 0x303050;\n", "owner = 0x303050; // size:0x6490\n"),
            self.native.replace("size:0x64A0", "size:0x64A0 size:0x64A0"),
        ):
            with self.subTest(native=native), self.assertRaises(ValueError):
                self.load([self.row(), self.row("actors", 0xA0)], native)

    def test_wrong_offsets_duplicate_chain_or_competing_owner_rejected(self):
        for rows in (
            [self.row(offset=0)],
            [self.row(offset=5)],
            [self.row(offset=False)],
            [self.row(owner_size=False)],
            [self.row(owner_size=0x64A1)],
            [self.row(section=".rodata")],
            [self.row(), self.row()],
            [self.row(), self.row("actors", 0x9C, owner="anchor")],
            [self.row(), self.row("actors", 0xA0, unit="field/Other")],
        ):
            with self.subTest(rows=rows), self.assertRaises(ValueError):
                self.load(rows)

    def test_overlapping_distinct_alias_spans_rejected(self):
        native = self.native + "overlap = 0x303056; // type:u8 size:0x2\n"
        native = native.replace("type:u16 size:0x2", "type:u16 size:0x4")
        with self.assertRaisesRegex(ValueError, "overlapping"):
            self.load([self.row(), self.row("overlap", 6)], native)

    def test_script_collisions_do_not_shadow_relative_alias(self):
        aliases = self.load([self.row()])
        for old in ("anchor = 0x303054;\n", "PROVIDE(anchor = 0x303054);\n",
                    "PROVIDE_HIDDEN(anchor = other + 4);\n"):
            with self.subTest(old=old), self.assertRaisesRegex(ValueError, "competing"):
                linker_aliases(old, aliases)

    def test_nobits_existing_autolabel_contract_unchanged(self):
        aliases = self.load([self.row("D_00303054", section=".bss")], "owner = 0x303050;\n")
        self.assertEqual((aliases[0].address, aliases[0].alias_size), (0x303054, None))
        legacy = linker_aliases("PROVIDE(D_00303054 = 0x303054);\n", aliases)
        self.assertIn("PROVIDE(D_00303054 = owner + 0x4);", legacy)
        self.assertNotIn(" = 0x303054", legacy)

    @staticmethod
    def fixture_image(flags=3, kind=1):
        size, origin, start = 0x64A0, 0x303050, 0x80
        names = b"\0.data\0.shstrtab\0"
        strings_at = start + size
        sections_at = strings_at + len(names)
        output = bytearray(sections_at + 120)
        output[:6] = b"\x7fELF\x01\x01"
        struct.pack_into("<I", output, 32, sections_at)
        struct.pack_into("<HHH", output, 46, 40, 3, 2)
        output[strings_at:sections_at] = names
        struct.pack_into("<10I", output, sections_at+40, 1, kind, flags, origin, start, size, 0, 0, 4, 0)
        struct.pack_into("<10I", output, sections_at+80, 7, 3, 0, 0, strings_at, len(names), 0, 0, 1, 0)
        return output

    def test_configure_requires_real_section_and_no_other_script_definition(self):
        self.load([self.row()])
        generated = self.root / "build/smt3"
        generated.mkdir(parents=True)
        script = generated / "SLUS_209.11.ld"
        syms = self.cfg / "undefined_syms_auto.txt"
        funcs = self.cfg / "undefined_funcs_auto.txt"
        syms.touch()
        funcs.touch()
        obj = "build/smt3/src/smt3/field/fldWap.o"
        orig = self.root / "orig/smt3/SLUS_209.11"
        orig.parent.mkdir(parents=True)
        image = self.fixture_image()
        orig.write_bytes(image)
        yaml = self.cfg / "SLUS_209.11.yaml"
        layout = "segments:\n  - subsegments:\n      - [0x80, .data, field/fldWap]\n"
        yaml.write_text(layout + "      - [0x6520, data, next_data]\n")
        spec = dict(serial="SLUS_209.11", elf_size=len(image), elf_sha1=hashlib.sha1(image).hexdigest())
        with patch.object(configure, "ROOT", self.root), patch.dict(configure.VERSIONS, {"smt3": spec}):
            with self.assertRaisesRegex(ValueError, "real generated"):
                configure.provide_symbol_aliases("smt3")
            for content in (f"{obj}(.text);\n", f"{obj}(.data*);\n",
                            f"{obj}(.data);\n{obj}(.data);\n"):
                script.write_text(content)
                with self.assertRaisesRegex(ValueError, "exactly one"):
                    configure.provide_symbol_aliases("smt3")
            script.write_text(f"{obj}(.text);\n{obj}(.data);\n")
            funcs.write_text("anchor = 0x303054;\n")
            with self.assertRaisesRegex(ValueError, "competing"):
                configure.provide_symbol_aliases("smt3")
            funcs.write_text("")
            configure.provide_symbol_aliases("smt3")
            self.assertIn("PROVIDE(anchor = owner + 0x4);", syms.read_text())
            yaml.write_text(layout + "      - [0x651C, data, next_data]\n")
            with self.assertRaisesRegex(ValueError, "entire bounded"):
                configure.provide_symbol_aliases("smt3")
            yaml.write_text(layout + "      - [0x6520, data, next_data]\n")
            for bad in (self.fixture_image(flags=2), self.fixture_image(kind=8)):
                orig.write_bytes(bad)
                with self.assertRaisesRegex(ValueError, "pinned"):
                    configure.provide_symbol_aliases("smt3")
                spec["elf_sha1"] = hashlib.sha1(bad).hexdigest()
                with self.assertRaisesRegex(ValueError, "writable PROGBITS"):
                    configure.provide_symbol_aliases("smt3")


if __name__ == "__main__":
    unittest.main()
