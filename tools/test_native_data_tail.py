import tempfile
import unittest
from pathlib import Path

from native_data_tail import (HEADER, check_primary, complete_reference, name_records,
                              opt_ins, pad_assets, verify_reloc_overrides, yaml_inputs)

NAME = "table"
START = 0x100
ADDR = 0x100100
HEAD = "\n\n".join(HEADER) + "\n\n"


def data_source(data, style="byte", operand=None):
    width = {"byte": 1, "short": 2, "word": 4}[style]
    rows = []
    for offset in range(0, len(data) & ~3, width):
        chunk = data[offset:offset + width]
        right = operand if operand is not None else hex(int.from_bytes(chunk, "little"))
        rows.append(f"    /* {START+offset:06X} {ADDR+offset:08X} {chunk.hex()} */ .{style} {right}\n")
    if not rows:
        return HEAD
    return HEAD + f"nonmatching {NAME}\n\ndlabel {NAME}\n" + "".join(rows) + f"enddlabel {NAME}\n"


def patch(source, payload, names=None):
    return complete_reference(source, NAME, payload, START, ADDR, names or {})


class NativeDataTail(unittest.TestCase):
    def test_real_tail_widths_and_no_word_control(self):
        for size in (2, 13, 26, 27, 32):
            for tail_value in (0x00, 0x86):
                with self.subTest(size=size, tail_value=tail_value):
                    count = size % 4
                    prefix = bytes(range(size - count))
                    tail = bytes([tail_value]) * count
                    original = data_source(prefix)
                    out = patch(original, prefix + tail)
                    expected = "".join(f"    /* {START+i:06X} {ADDR+i:08X} */ .byte 0x{tail_value:02X}\n"
                                       for i in range(len(prefix), size))
                    if not prefix:
                        original += f"\nnonmatching {NAME}\n\ndlabel {NAME}\nenddlabel {NAME}\n"
                    self.assertEqual(out, original.replace(f"enddlabel {NAME}\n", expected + f"enddlabel {NAME}\n"))
                    self.assertEqual(out.count(f"enddlabel {NAME}\n"), 1)

    def test_prefix_pointer_addend_is_literal_and_checked(self):
        pointer = (0x1234 + 0x28).to_bytes(4, "little")
        original = data_source(pointer, "word", "pointer + 0x28")
        names = {"pointer": (0x1234, None), "neighbor": (0x7654, 16)}
        out = patch(original, pointer + b"\x01\xFE", names)
        self.assertIn(" .word pointer + 0x28\n", out)
        self.assertNotIn("neighbor", out)
        self.assertIn(".byte 0xFE\nenddlabel table", out)
        with self.assertRaisesRegex(ValueError, "addend"):
            patch(original.replace(" + 0x28", " - 0x28"), pointer + b"\x01\xFE", names)
        with self.assertRaisesRegex(ValueError, "unresolved"):
            patch(original, pointer + b"\x01\xFE")

    def test_short_and_negative_literal(self):
        p = b"\xFF\xFF" * 2
        old = data_source(p, "short", "-1")
        self.assertEqual(patch(old, p + b"\x07").count(" .short -1"), 2)

    def test_wrong_prefix_and_unbounded_or_duplicate_inventory(self):
        p = bytes(range(12))
        old = data_source(p)
        variants = [old.replace("000100", "000101", 1),
                    old.replace(" .byte 0x0", " .byte 0x42", 1),
                    old.replace("00 */", "01 */", 1),
                    old.replace("enddlabel table", "dlabel other\nenddlabel table"),
                    old.replace('.section .data, "wa"', '.section .rodata, "a"'),
                    old.replace(next(x + "\n" for x in old.splitlines() if " .byte " in x), "", 1)]
        for a in variants:
            with self.subTest(variant=a), self.assertRaises(ValueError):
                patch(a, p + b"\x09")

    def test_exact_primary_disjoint_from_pointer_neighbor(self):
        good = name_records("table = 0x1000; // type:u8 size:0x1A\n"
                            "neighbor = 0x101A; // type:u32 size:0x8\n")
        check_primary(good, "table", 0x1000, 26)
        for number in ("0X1018", "4120"):
            with self.assertRaisesRegex(ValueError, "interior"):
                bad = name_records(f"table = 0x1000; // size:26\ninside = {number};\n")
                check_primary(bad, "table", 0x1000, 26)
        with self.assertRaisesRegex(ValueError, "unsupported"):
            name_records("an unrecognized active symbol line\n")
        for bad in ({"table": (0x1000, 24)}, {"table": (0x1000, None)},
                    good | {"interior": (0x1018, 4)}, good | {"earlier": (0x0FF0, 32)}):
            with self.subTest(records=bad), self.assertRaises(ValueError):
                check_primary(bad, "table", 0x1000, 26)

    def test_installed_relocation_grammar_and_both_span_ends(self):
        for text in (
            "reloc:MIPS_32 symbol:pointer rom:0x7C",   # non-leading ROM at the first omitted byte
            "rom:124 reloc:MIPS_32 symbol:pointer",     # decimal
            "rom:0X7C reloc:MIPS_32 symbol:pointer",    # capital prefix
            "rom:0x64 reloc:MIPS_32 symbol:pointer",    # first included byte
            "rom:0x7D reloc:MIPS_32 symbol:pointer",    # last included byte
            "rom:0x63 reloc:MIPS_32 symbol:pointer",    # crosses the left end
        ):
            with self.subTest(text=text), self.assertRaisesRegex(ValueError, "intersects"):
                verify_reloc_overrides([text], 100, 126)
        verify_reloc_overrides(["rom:96 reloc:MIPS_32 symbol:pointer // outside",
                                "symbol:pointer rom:126 reloc:MIPS_32 addend:0x10"], 100, 126)
        for text in ("rom:0x80 kind:MIPS_32 symbol:pointer", "symbol:pointer",
                     "rom:0x80 rom:0x81 reloc:MIPS_32 symbol:pointer",
                     "rom:0x80 reloc:unrecognized symbol:pointer"):
            with self.subTest(text=text), self.assertRaises(ValueError):
                verify_reloc_overrides([text], 100, 126)

    def test_all_yaml_relocation_paths_and_real_adjacent_pad(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            cfg = root / "config/v"
            cfg.mkdir(parents=True)
            src = root / "asm/v/data/pad.s"
            src.parent.mkdir(parents=True)
            pad = root / "assets/v/pad.databin.bin"
            pad.parent.mkdir(parents=True)
            pad.write_bytes(bytes(6))
            listing = ('.include "macro.inc"\n\n.section .data, "wa"\n\n'
                       'nonmatching __pad_databin\n\ndlabel __pad_databin\n'
                       f'.incbin "{pad}"\nenddlabel __pad_databin\n')
            src.write_text(listing)
            a, b = cfg / "a.txt", cfg / "b.txt"
            a.write_text("// empty\n")
            b.write_text("symbol:pointer reloc:MIPS_32 rom:125\n")
            y = cfg / "demo.yaml"
            y.write_text("""options:
  base_path: ../..
  asset_path: assets/v
  symbol_addrs_path: [config/v/symbol_addrs.txt]
  reloc_addrs_path: [config/v/a.txt, config/v/b.txt]
segments:
  - name: main
    subsegments:
      - [0x64, .data, a/example]
      - [0x7E, databin, pad]
      - [0x84, data, following]
""")
            paths = yaml_inputs(root, "v", "demo")[2]
            self.assertEqual(paths, [a, b])
            with self.assertRaisesRegex(ValueError, "intersects"):
                verify_reloc_overrides((p.read_text() for p in paths), 100, 126)
            self.assertEqual(pad_assets(root, "v", "demo", {"a/example"}), {
                src.relative_to(root): pad.relative_to(root)})
            src.write_text(listing.replace(str(pad), "bogus.bin"))
            with self.assertRaisesRegex(ValueError, "incbin"):
                pad_assets(root, "v", "demo", {"a/example"})
            src.write_text(listing.replace("enddlabel", '    .incbin "extra.bin"\nenddlabel'))
            with self.assertRaisesRegex(ValueError, "incbin"):
                pad_assets(root, "v", "demo", {"a/example"})
            a.unlink()
            with self.assertRaisesRegex(ValueError, "missing"):
                yaml_inputs(root, "v", "demo")

    def test_ledger_rejects_duplicate_and_unsafe(self):
        with tempfile.TemporaryDirectory() as tmp:
            f = Path(tmp) / "ledger"
            self.assertEqual(opt_ins(f), {})
            for line in ('{"a/b":"one","a/b":"two"}', '{"a/b":"one","q/z":"one"}',
                         '{"../a":"one"}', '{"a/b":"$not_valid"}', '[]'):
                f.write_text(line)
                with self.subTest(ledger=line), self.assertRaises(ValueError):
                    opt_ins(f)
            f.write_text('{"a/b":"one"}')
            self.assertEqual(opt_ins(f), {"a/b": "one"})


if __name__ == "__main__":
    unittest.main()
