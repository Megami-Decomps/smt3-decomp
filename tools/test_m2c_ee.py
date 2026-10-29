"""Exercise actual m2c caller/callee decoding of EE stack arguments."""
import importlib.util
import re
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
PARAMS = ", ".join(f"int {name}" for name in "abcdefghijkl")


@unittest.skipUnless(importlib.util.find_spec("m2c"), "optional m2c is not installed")
class EeStackArgumentsTests(unittest.TestCase):
    def decompile(self, context, assembly):
        with tempfile.TemporaryDirectory() as tmp:
            folder = Path(tmp)
            ctx = folder / "context.c"
            asm = folder / "fixture.s"
            ctx.write_text(context)
            asm.write_text(assembly)
            result = subprocess.run(
                [sys.executable, str(ROOT / "tools/m2c_ee.py"),
                 "-t", "mipsee-gcc-c", "--no-cache", "--context", str(ctx), str(asm)],
                capture_output=True, text=True, check=True)
            self.assertNotIn("M2C_ERROR", result.stdout)
            self.assertNotIn("extra?", result.stdout)
            return result.stdout

    def test_twelve_argument_call(self):
        instructions = [".text", "glabel caller", "addiu $sp, $sp, -48", "sd $ra, 32($sp)"]
        instructions += [f"li ${reg}, {reg - 3}" for reg in range(4, 12)]
        for index in range(4):
            instructions += [f"li $2, {index + 9}", f"sw $2, {8 * index}($sp)"]
        instructions += ["jal sink", "nop", "ld $ra, 32($sp)",
                         "jr $ra", "addiu $sp, $sp, 48"]
        text = self.decompile(f"void sink({PARAMS});\nvoid caller(void);\n",
                              "\n".join(instructions) + "\n")
        arguments = re.search(r"sink\(([^)]+)\);", text)[1]
        self.assertEqual([int(value.strip(), 0) for value in arguments.split(",")],
                         list(range(1, 13)))

    def test_incoming_stack_arguments(self):
        text = self.decompile(f"int receiver({PARAMS});\n", """.text
glabel receiver
lw $2, 0($sp)
lw $3, 24($sp)
addu $2, $2, $3
jr $ra
nop
""")
        self.assertIn("return i + l;", text)

    def test_raw_single_to_integer_conversion(self):
        for fd, word in ((0, 0x46006024), (1, 0x46006064)):
            with self.subTest(fd=fd):
                text = self.decompile("int convert(float value);\n", f""".text
glabel convert
.word 0x{word:08X} # cvt.w.s $f{fd}, $f12
mfc1 $2, $f{fd}
jr $ra
nop
""")
                self.assertIn("return (s32) value;", text)

    def test_conversion_word_in_data_is_preserved(self):
        sys.path.insert(0, str(ROOT / "tools"))
        from m2c_ee import normalize_ee_asm
        text = ".section .rodata\n.word 0x46006024\n.text\n.word 0x46006024\n"
        result = normalize_ee_asm(text)
        self.assertIn(".rodata\n.word 0x46006024", result)
        self.assertIn(".text\ncvt.w.s $f0, $f12", result)
        multiple = ".text\n.word 0x46006024, 0x12345678\n"
        self.assertEqual(normalize_ee_asm(multiple), multiple)
        literal = ".lit4\n.word 0x46006024\n"
        self.assertEqual(normalize_ee_asm(literal), literal)


if __name__ == "__main__":
    unittest.main()
