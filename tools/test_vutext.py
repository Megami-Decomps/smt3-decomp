#!/usr/bin/env python3
"""vutext.py has to walk a VIF packet exactly and refuse one that does not add up."""
import importlib.util
import struct
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
spec = importlib.util.spec_from_file_location("vutext", ROOT / "tools/vutext.py")
vutext = importlib.util.module_from_spec(spec)
spec.loader.exec_module(vutext)

spec = importlib.util.spec_from_file_location("vu_include", ROOT / "tools/vu_include.py")
vu_include = importlib.util.module_from_spec(spec)
spec.loader.exec_module(vu_include)

RETAIL = ROOT / "orig" / "smt3" / "SLUS_209.11"


def vif(cmd, num, imm):
    return struct.pack("<I", (cmd << 24) | (num << 16) | imm)


def fake_sections(packet, vram=0x1000):
    """A .vutext of just this packet, as sections() would report it."""
    return {".vutext": (vram, 0, len(packet))}


class Walk(unittest.TestCase):
    def test_mpg_payload_is_the_instructions(self):
        body = bytes(range(16))
        packet = vif(0x00, 0, 0) + vif(0x4A, 2, 0) + body
        codes = vutext.vif_walk(packet, fake_sections(packet), 0x1000, 0x1000 + len(packet))
        self.assertEqual([c[2] for c in codes], ["NOP", "MPG 2 -> micro 0x0000"])
        self.assertEqual(codes[1][3], body)

    def test_num_zero_is_256_instructions(self):
        packet = vif(0x4A, 0, 0) + bytes(256 * 8)
        codes = vutext.vif_walk(packet, fake_sections(packet), 0x1000, 0x1000 + len(packet))
        self.assertEqual(codes[0][2], "MPG 256 -> micro 0x0000")

    def test_payload_past_the_end_is_an_error(self):
        packet = vif(0x4A, 2, 0) + bytes(8)
        with self.assertRaises(ValueError):
            vutext.vif_walk(packet, fake_sections(packet), 0x1000, 0x1000 + len(packet))

    def test_a_packet_that_does_not_end_on_a_code_is_an_error(self):
        # Landing past the end rather than on it means a code boundary was wrong somewhere.
        packet = vif(0x00, 0, 0)
        with self.assertRaises(ValueError):
            vutext.vif_walk(packet, fake_sections(packet), 0x1000, 0x1002)

    def test_unknown_command_is_an_error(self):
        packet = vif(0x3F, 0, 0)
        with self.assertRaises(ValueError):
            vutext.vif_walk(packet, fake_sections(packet), 0x1000, 0x1000 + len(packet))

    def test_unpack_payload_is_sized_by_format(self):
        packet = vif(0x6C, 2, 0x43EA) + bytes(2 * 4 * 4)
        codes = vutext.vif_walk(packet, fake_sections(packet), 0x1000, 0x1000 + len(packet))
        self.assertEqual(codes[0][2], "UNPACK V4-32 2 -> vu mem 1002 unsigned")


class Stubs(unittest.TestCase):
    def stub(self, *lowers):
        micro = b"".join(struct.pack("<2I", low, 0x000002FF) for low in lowers)
        return vutext.entry_table(micro, {0: "entry"}, {}, 2048)[0]

    def test_branch_target_is_relative_to_the_next_instruction(self):
        at, _, target, how, _ = self.stub(0x40000018, 0, 0)
        self.assertEqual((at, target, how), (0, 0x19, "B"))

    def test_a_backward_branch_wraps_in_micro_memory(self):
        # 0x420F040E at 0x0010 is the BAL into drawEntry2: 11-bit signed, so it wraps to 0x041F.
        nop = struct.pack("<2I", 0x8000033C, 0x000002FF)
        micro = nop * 0x10 + struct.pack("<2I", 0x420F040E, 0x000002FF) + nop
        row = vutext.entry_table(micro, {0x10: "entryRamp"}, {0x41F: "drawEntry2"}, 2048)[0]
        self.assertEqual(row[2:], (0x41F, "BAL", "drawEntry2"))

    def test_iaddiu_then_jr_resolves_through_the_register(self):
        _, _, target, how, _ = self.stub(0x10010708, 0x48000800, 0)
        self.assertEqual((target, how), (0x708, "JR"))

    def test_a_jump_through_itop_has_no_static_target(self):
        _, _, target, how, _ = self.stub(0x800106BD, 0x48000800, 0)
        self.assertIsNone(target)
        self.assertEqual(how, "JR through VIF1_ITOP")


@unittest.skipUnless(RETAIL.exists(), "needs the retail ELF in orig/smt3")
class Retail(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.elf = RETAIL.read_bytes()
        cls.secs = vutext.sections(cls.elf)
        cls.imgs = vutext.images(cls.elf, cls.secs)

    def test_both_images_reconstruct_to_their_recorded_hash(self):
        for img in self.imgs:
            self.assertEqual(vutext.image_hash(img["micro"]), vutext.IMAGE_SHA1[img["vu"]])

    def test_the_uploads_agree_with_the_dvp_overlay_table(self):
        self.assertEqual(vutext.check(self.imgs, vutext.overlays(self.elf, self.secs)), 0)

    def test_the_vu1_image_is_1884_instructions(self):
        vu1 = next(i for i in self.imgs if i["vu"] == 1)
        self.assertEqual(len(vu1["micro"]) // 8, 1884)
        self.assertEqual(len(vu1["chunks"]), 8)

    def test_coverage_splits_vutext_into_microcode_and_asset_bytes(self):
        vu = vu_include.coverage("smt3", RETAIL)
        self.assertEqual(vu["section_bytes"], self.secs[".vutext"][2])
        self.assertEqual(vu["microcode_bytes"],
                         sum(len(i["micro"]) for i in self.imgs))
        # Both listings are in the tree, so the build assembles every instruction.
        self.assertEqual(vu["source_bytes"], vu["microcode_bytes"])
        self.assertEqual(vu["instructions"], vu["source_bytes"] // 8)


if __name__ == "__main__":
    unittest.main()
