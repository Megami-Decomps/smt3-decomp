#!/usr/bin/env python3
"""The microcode listings must assemble to the bytes the retail image holds, exactly."""
import importlib.util
import os
import shutil
import struct
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent


def load(name):
    spec = importlib.util.spec_from_file_location(name, ROOT / "tools" / (name + ".py"))
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


vuas = load("vuas")
vudis = load("vudis")
vutext = load("vutext")

RETAIL = ROOT / "orig" / "smt3" / "SLUS_209.11"
SOURCES = {0: ROOT / "src/smt3/sdf/vu/vu0_micro.vsm", 1: ROOT / "src/smt3/sdf/vu/vu1_micro.vsm"}


def words(text):
    return vuas.assemble(text)


class Encode(unittest.TestCase):
    def test_a_pair_encodes_as_the_hardware_has_it(self):
        # micro 0x0000 of the VU0 image.
        self.assertEqual(words("\tMULw.xyz vf05, vf03, vf02\tMOVE.xyzw vf06, vf02\n"),
                         [(0x01C2195B, 0x81E6133C)])

    def test_nop_pair(self):
        self.assertEqual(words("\tNOP\tNOP\n"), [(0x000002FF, 0x8000033C)])

    def test_the_end_flag_sets_its_bit(self):
        upper, _ = words("\tNOP[e]\tNOP\n")[0]
        self.assertTrue(upper & (1 << 30))

    def test_loi_sets_the_i_bit_and_carries_the_float(self):
        upper, lower = words("\tNOP\tLOI 0.5\n")[0]
        self.assertTrue(upper & (1 << 31))
        self.assertEqual(struct.unpack("<f", struct.pack("<I", lower))[0], 0.5)

    def test_a_branch_is_relative_to_the_next_instruction(self):
        text = "\tNOP\tB there\n\tNOP\tNOP\nthere:\n\tNOP\tNOP\n"
        self.assertEqual(words(text)[0][1] & 0x7FF, 1)

    def test_a_wrapped_branch_takes_the_byte_term(self):
        # The form vudis writes where the real displacement does not fit in 11 bits.
        text = "here:\n" + "\tNOP\tNOP\n" * 2 + "\tNOP\tB here-16384\n"
        self.assertEqual(words(text)[2][1] & 0x7FF, (-3 - 2048) & 0x7FF)

    def test_an_unknown_mnemonic_is_refused(self):
        with self.assertRaises(SystemExit):
            words("\tFROBNICATE.xyzw vf01, vf02, vf03\tNOP\n")

    def test_a_line_without_a_lower_half_is_refused(self):
        with self.assertRaises(SystemExit):
            words("\tMULw.xyz vf05, vf03, vf02\n")

    def test_an_extra_operand_is_refused(self):
        with self.assertRaises(SystemExit):
            words("\tNOP\tNOP vi01\n")


@unittest.skipUnless(RETAIL.exists(), "needs the retail ELF in orig/smt3")
class Retail(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        elf = RETAIL.read_bytes()
        cls.images = {i["vu"]: i["micro"]
                      for i in vutext.images(elf, vutext.sections(elf))}

    def test_the_committed_listings_assemble_to_the_image(self):
        for vu, path in SOURCES.items():
            with self.subTest(vu=vu):
                got = vuas.to_bytes(vuas.assemble(path.read_text()))
                self.assertEqual(got, self.images[vu])

    def test_a_fresh_disassembly_round_trips(self):
        for vu, micro in self.images.items():
            with self.subTest(vu=vu):
                text = vudis.render(micro, {}, "; test")
                self.assertEqual(vuas.to_bytes(vuas.assemble(text)), micro)

    @unittest.skipUnless(shutil.which(vudis.DVP_AS) or os.path.exists(vudis.DVP_AS),
                         "dvp-as is not installed")
    def test_dvp_as_agrees_with_ours(self):
        """The original assembler reads the same source and writes the same bytes."""
        for vu, path in SOURCES.items():
            with self.subTest(vu=vu):
                got, err = vudis.assemble(path.read_text())
                self.assertIsNotNone(got, err)
                self.assertEqual(got[:len(self.images[vu])], self.images[vu])


if __name__ == "__main__":
    unittest.main()
