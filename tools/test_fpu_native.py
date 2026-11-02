#!/usr/bin/env python3
"""fpu.h's NATIVE_FPU bodies, compiled with the host's own C compiler,
against a from-scratch Python model of the same documented EE FPU behaviour
(PCSX2's pcsx2/FPU.cpp). A mismatch is a bug in one of the two, not a check
against hand-typed expected numbers."""
import math
import shutil
import struct
import subprocess
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent

SIGN = 0x80000000
EXP_MASK = 0x7F800000
FMAX_BITS = 0x7F7FFFFF


def to_bits(value):
    return struct.unpack("<I", struct.pack("<f", value))[0]


def from_bits(bits):
    return struct.unpack("<f", struct.pack("<I", bits & 0xFFFFFFFF))[0]


def narrow(value):
    """Round a Python (double-precision) value to the nearest float32,
    saturating to a signed infinity rather than raising on overflow."""
    try:
        return struct.unpack("<f", struct.pack("<f", value))[0]
    except OverflowError:
        return math.inf if value > 0 else -math.inf


def sanitize(bits):
    """PCSX2's fpuDouble(): flush a zero/denormal word to a same-signed
    zero, clamp a maximum-exponent word to +/-Fmax, pass a normal value
    through unchanged."""
    exp = bits & EXP_MASK
    if exp == 0:
        return from_bits(bits & SIGN)
    if exp == EXP_MASK:
        return from_bits((bits & SIGN) | FMAX_BITS)
    return from_bits(bits)


def clamp_result(value):
    """PCSX2's checkOverflow()/checkUnderflow(), applied to a computed
    result: an Inf-looking result saturates to +/-Fmax, a denormal result
    flushes to a same-signed zero."""
    bits = to_bits(value)
    exp = bits & EXP_MASK
    if exp == EXP_MASK:
        return from_bits((bits & SIGN) | FMAX_BITS)
    if exp == 0 and (bits & 0x007FFFFF) != 0:
        return from_bits(bits & SIGN)
    return value


def model_sqrt(bits):
    if (bits & EXP_MASK) == 0:
        return to_bits(from_bits(bits & SIGN))
    if bits & SIGN:
        return to_bits(narrow(math.sqrt(abs(sanitize(bits)))))
    return to_bits(narrow(math.sqrt(sanitize(bits))))


def model_rsqrt(num_bits, ft_bits):
    if (ft_bits & EXP_MASK) == 0:
        return to_bits(from_bits((ft_bits & SIGN) | FMAX_BITS))
    if ft_bits & SIGN:
        root = narrow(math.sqrt(abs(sanitize(ft_bits))))
    else:
        root = narrow(math.sqrt(sanitize(ft_bits)))
    num = sanitize(num_bits)
    return to_bits(clamp_result(narrow(num / root)))


def model_abs(bits):
    return bits & ~SIGN & 0xFFFFFFFF


def model_cvtws(bits):
    value = from_bits(bits)
    if (bits & EXP_MASK) <= 0x4E800000:
        truncated = math.trunc(value)  # C-style truncation toward zero
        return truncated
    return -0x80000000 if (bits & SIGN) else 0x7FFFFFFF


# (label, raw bit pattern) shared by sqrt.s, abs.s and cvt.w.s -- one
# operand each, so one table covers all three.
WORDS = [
    ("+0", 0x00000000),
    ("-0", 0x80000000),
    ("smallest +denormal", 0x00000001),
    ("smallest -denormal", 0x80000001),
    ("largest +denormal", 0x007FFFFF),
    ("largest -denormal", 0x807FFFFF),
    ("+1.0", 0x3F800000),
    ("-1.0", 0xBF800000),
    ("+0.1", 0x3DCCCCCD),
    ("-0.1", 0xBDCCCCCD),
    ("+2.0", 0x40000000),
    ("-4.0", 0xC0800000),
    ("+FLT_MAX", 0x7F7FFFFF),
    ("-FLT_MAX", 0xFF7FFFFF),
    ("+Inf bit pattern", 0x7F800000),
    ("-Inf bit pattern", 0xFF800000),
    ("+NaN bit pattern", 0x7FC00000),
    ("-NaN bit pattern", 0xFFC00000),
    ("just below 2^31", 0x4EFFFFFF),
    ("just above threshold (2^31 exactly)", 0x4F000000),
]

# (label, numerator, denominator) for rsqrt.s; converted to bit patterns
# before reaching either side, so a decimal literal's host rounding never
# differs between the C program and the Python model.
RSQRT_CASES = [
    ("normal/normal", 1.0, 4.0),
    ("normal/negative", 1.0, -4.0),
    ("normal/+0", 1.0, 0.0),
    ("normal/-0", 1.0, -0.0),
    ("normal/+denormal", 1.0, from_bits(0x00000001)),
    ("normal/-denormal", 1.0, from_bits(0x80000001)),
    ("scaled", 2.5, 9.0),
    ("normal/+Inf-pattern", 1.0, from_bits(0x7F800000)),
    ("normal/-Inf-pattern", 1.0, from_bits(0xFF800000)),
    ("zero numerator", 0.0, 4.0),
    ("negative numerator", -3.0, 9.0),
    ("overflowing quotient", from_bits(0x7F7FFFFF), 1e-30),
    ("underflowing quotient", from_bits(0x00800000), 16.0),
]


def find_cc():
    for name in ("cc", "clang", "gcc"):
        path = shutil.which(name)
        if path:
            return path
    return None


def build_program(tmpdir):
    lines = [
        "#define NATIVE_FPU",
        '#include "%s"' % (ROOT / "include" / "fpu.h"),
        "#include <stdio.h>",
        "typedef union { float f; unsigned int u; } W;",
        "static const unsigned int words[] = {",
    ]
    lines += ["    0x%08Xu," % bits for _, bits in WORDS]
    lines.append("};")
    lines.append("static const unsigned int rsqrt_num[] = {")
    lines += ["    0x%08Xu," % to_bits(num) for _, num, _ in RSQRT_CASES]
    lines.append("};")
    lines.append("static const unsigned int rsqrt_den[] = {")
    lines += ["    0x%08Xu," % to_bits(den) for _, _, den in RSQRT_CASES]
    lines.append("};")
    lines.append("int main(void) {")
    lines.append("    unsigned int i;")
    lines.append("    for (i = 0; i < sizeof(words) / sizeof(words[0]); i++) {")
    lines.append("        W in, sq, ab;")
    lines.append("        int cv;")
    lines.append("        in.u = words[i];")
    lines.append("        sq.f = fsqrtf(in.f);")
    lines.append("        ab.f = ffabsf(in.f);")
    lines.append("        cv = fcvtws(in.f);")
    lines.append('        printf("W %u %u %u %d\\n", in.u, sq.u, ab.u, cv);')
    lines.append("    }")
    lines.append("    for (i = 0; i < sizeof(rsqrt_num) / sizeof(rsqrt_num[0]); i++) {")
    lines.append("        W num, den, rs;")
    lines.append("        num.u = rsqrt_num[i];")
    lines.append("        den.u = rsqrt_den[i];")
    lines.append("        rs.f = frsqrtf(num.f, den.f);")
    lines.append('        printf("R %u %u %u\\n", num.u, den.u, rs.u);')
    lines.append("    }")
    lines.append("    return 0;")
    lines.append("}")
    src = tmpdir / "fpu_native_program.c"
    src.write_text("\n".join(lines) + "\n")
    return src


class NativeFpuMatchesEeModel(unittest.TestCase):
    """Differential check: the header's NATIVE_FPU bodies vs. an independent
    Python model of the same documented EE FPU behaviour."""

    @classmethod
    def setUpClass(cls):
        cc = find_cc()
        if cc is None:
            raise unittest.SkipTest("no host C compiler found (cc/clang/gcc)")
        cls.tmpdir = Path(tempfile.mkdtemp(prefix="fpu_native_test_"))
        src = build_program(cls.tmpdir)
        exe = cls.tmpdir / "fpu_native_program"
        compile_result = subprocess.run(
            [cc, "-std=c99", "-Wall", "-Wextra", "-O2", "-o", str(exe), str(src), "-lm"],
            capture_output=True, text=True)
        if compile_result.returncode != 0:
            raise AssertionError("NATIVE_FPU program failed to compile:\n" + compile_result.stderr)
        cls.warnings = compile_result.stderr
        run_result = subprocess.run([str(exe)], capture_output=True, text=True, check=True)
        cls.sqrt_rows = []
        cls.abs_rows = []
        cls.cvt_rows = []
        cls.rsqrt_rows = []
        word_lines = [ln for ln in run_result.stdout.splitlines() if ln.startswith("W ")]
        rsqrt_lines = [ln for ln in run_result.stdout.splitlines() if ln.startswith("R ")]
        assert len(word_lines) == len(WORDS), (len(word_lines), len(WORDS))
        assert len(rsqrt_lines) == len(RSQRT_CASES), (len(rsqrt_lines), len(RSQRT_CASES))
        for (label, bits), line in zip(WORDS, word_lines):
            _, in_u, sq_u, ab_u, cv = line.split()
            in_u, sq_u, ab_u, cv = int(in_u), int(sq_u), int(ab_u), int(cv)
            assert in_u == bits
            cls.sqrt_rows.append((label, bits, sq_u, model_sqrt(bits)))
            cls.abs_rows.append((label, bits, ab_u, model_abs(bits)))
            cls.cvt_rows.append((label, bits, cv, model_cvtws(bits)))
        for (label, num, den), line in zip(RSQRT_CASES, rsqrt_lines):
            _, num_u, den_u, rs_u = line.split()
            num_u, den_u, rs_u = int(num_u), int(den_u), int(rs_u)
            cls.rsqrt_rows.append((label, num_u, den_u, rs_u, model_rsqrt(num_u, den_u)))

    def test_no_compiler_warnings(self):
        self.assertEqual(self.warnings.strip(), "", self.warnings)

    def test_sqrt_matches_model(self):
        for label, bits, native, model in self.sqrt_rows:
            with self.subTest(site="sqrt.s", case=label):
                self.assertEqual(native, model,
                                  "sqrt.s(0x%08X) [%s]: native 0x%08X != model 0x%08X"
                                  % (bits, label, native, model))

    def test_abs_matches_model(self):
        for label, bits, native, model in self.abs_rows:
            with self.subTest(site="abs.s", case=label):
                self.assertEqual(native, model,
                                  "abs.s(0x%08X) [%s]: native 0x%08X != model 0x%08X"
                                  % (bits, label, native, model))

    def test_cvtws_matches_model(self):
        for label, bits, native, model in self.cvt_rows:
            with self.subTest(site="cvt.w.s+mfc1", case=label):
                self.assertEqual(native, model,
                                  "cvt.w.s(0x%08X) [%s]: native %d != model %d"
                                  % (bits, label, native, model))

    def test_rsqrt_matches_model(self):
        for label, num, den, native, model in self.rsqrt_rows:
            with self.subTest(site="rsqrt.s", case=label):
                self.assertEqual(native, model,
                                  "rsqrt.s(0x%08X, 0x%08X) [%s]: native 0x%08X != model 0x%08X"
                                  % (num, den, label, native, model))

    @classmethod
    def tearDownClass(cls):
        print("\n%-34s %-10s %-12s %-12s" % ("case", "input", "native", "model"))
        for label, bits, native, model in cls.sqrt_rows:
            print("sqrt.s  %-26s 0x%08X 0x%08X   0x%08X %s"
                  % (label, bits, native, model, "" if native == model else "MISMATCH"))
        for label, bits, native, model in cls.abs_rows:
            print("abs.s   %-26s 0x%08X 0x%08X   0x%08X %s"
                  % (label, bits, native, model, "" if native == model else "MISMATCH"))
        for label, bits, native, model in cls.cvt_rows:
            print("cvt.w.s %-26s 0x%08X %-12d %-12d %s"
                  % (label, bits, native, model, "" if native == model else "MISMATCH"))
        for label, num, den, native, model in cls.rsqrt_rows:
            print("rsqrt.s %-26s n=0x%08X d=0x%08X 0x%08X 0x%08X %s"
                  % (label, num, den, native, model, "" if native == model else "MISMATCH"))


if __name__ == "__main__":
    unittest.main()
