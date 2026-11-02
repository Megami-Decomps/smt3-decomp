#!/usr/bin/env python3
"""fpu.h's NATIVE_FPU bodies, compiled with the host's own C compiler, against
literal hardware results from the ps2autotests EE FPU corpus
(tests/cpu/ee_fpu/sqrt.expected, edge.expected, arithmetic.expected,
convert.expected), not against a from-scratch model of any emulator's
behaviour. Per uee's CONTRIBUTING.md, a real-hardware autotest outranks a
vector derived from an emulator's source, so hardware is the only authority
checked here. See docs/fpu-native-semantics.md for the PCSX2-vs-hardware
divergences this replaced, and for why rsqrt.s's divide-by-zero/overflow
rows below expect +/-FLT_MAX rather than the raw hardware bit pattern."""
import shutil
import subprocess
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent

SIGN = 0x80000000
EXP_MASK = 0x7F800000
FMAX_BITS = 0x7F7FFFFF

# (operand, hardware result), sourced from sqrt.expected's "sqrt:" section
# and edge.expected's "sqrt.s:" section.
SQRT_ROWS = [
    (0x00000000, 0x00000000),
    (0x80000000, 0x00000000),
    (0x3F800000, 0x3F800000),
    (0x40000000, 0x3FB504F3),
    (0x40400000, 0x3FDDB3D7),
    (0x7FFFFFFF, 0x5FB504F3),
    (0xFFFFFFFF, 0x5FB504F3),
    (0x7F800000, 0x5F800000),
    (0xFF800000, 0x5F800000),
    (0x41C80000, 0x40A00000),
    (0x00000000, 0x00000000),
    (0x80000000, 0x00000000),
    (0x3F800000, 0x3F800000),
    (0xBF800000, 0x3F800000),
    (0x3FFFFFFF, 0x3FB504F3),
    (0x7F800001, 0x5F800000),
    (0x00000001, 0x00000000),
    (0x7FFFFFFF, 0x5FB504F3),
    (0xFFFFFFFF, 0x5FB504F3),
    (0x00001337, 0x00000000),
    (0xDEADBEEF, 0x4F152108),
    (0x00000000, 0x00000000),
    (0x80000000, 0x00000000),
    (0x3F800000, 0x3F800000),
    (0xBF800000, 0x3F800000),
    (0x40000000, 0x3FB504F3),
    (0x3FFFFFFF, 0x3FB504F3),
    (0x3F800001, 0x3F800000),
    (0x33800000, 0x39800000),
    (0x00000001, 0x00000000),
    (0x007FFFFF, 0x00000000),
    (0x00800000, 0x20000000),
    (0x7F7FFFFF, 0x5F7FFFFF),
    (0xFF7FFFFF, 0x5F7FFFFF),
    (0x7F800000, 0x5F800000),
    (0xFF800000, 0x5F800000),
    (0x7FFFFFFF, 0x5FB504F3),
    (0xFFFFFFFF, 0x5FB504F3),
    (0x3EAAAAAB, 0x3F13CD3A),
    (0x40490FDB, 0x3FE2DFC4),
    (0x5F800000, 0x4F800000),
]

# (numerator, denominator, hardware result), sourced from sqrt.expected's
# "rsqrt:" table and edge.expected's 20x20 "rsqrt.s:" matrix.
RSQRT_ROWS = [
    (0x00000000, 0x00000000, 0x7FFFFFFF),
    (0x00000000, 0x80000000, 0x7FFFFFFF),
    (0x80000000, 0x00000000, 0xFFFFFFFF),
    (0x80000000, 0x80000000, 0xFFFFFFFF),
    (0x00000000, 0x3F800000, 0x00000000),
    (0x3F800000, 0x3F800000, 0x3F800000),
    (0x3F800000, 0x00000000, 0x7FFFFFFF),
    (0x40000000, 0x40000000, 0x3FB504F3),
    (0x40400000, 0x3F800000, 0x40400000),
    (0x40400000, 0x40400000, 0x3FDDB3D7),
    (0x7FFFFFFF, 0x7FFFFFFF, 0x5FB504F2),
    (0x7FFFFFFF, 0xFFFFFFFF, 0x5FB504F2),
    (0xFFFFFFFF, 0x7FFFFFFF, 0xDFB504F2),
    (0xFFFFFFFF, 0xFFFFFFFF, 0xDFB504F2),
    (0x7FFFFFFF, 0x00000000, 0x7FFFFFFF),
    (0x00000000, 0x7FFFFFFF, 0x00000000),
    (0x00000000, 0x7F800000, 0x00000000),
    (0x7F800000, 0x7F800000, 0x5F800000),
    (0xFF800000, 0x7F800000, 0xDF800000),
    (0x3F800000, 0x41C80000, 0x3E4CCCCD),
    (0x40A00000, 0x41C80000, 0x3F800000),
    (0x41C80000, 0x41C80000, 0x40A00000),
    (0x00000000, 0x00000000, 0x7FFFFFFF),
    (0x00000000, 0x80000000, 0x7FFFFFFF),
    (0x3F800000, 0x3F800000, 0x3F800000),
    (0x3F800000, 0xBF800000, 0x3F800000),
    (0x3FFFFFFF, 0x3F800000, 0x3FFFFFFF),
    (0x7F800001, 0x3F800000, 0x7F800001),
    (0x00000001, 0x3F800000, 0x00000000),
    (0x3F800000, 0x3FFFFFFF, 0x3F3504F3),
    (0x3F800000, 0x7F800001, 0x1F800000),
    (0x3F800000, 0x00000001, 0x7FFFFFFF),
    (0x7FFFFFFF, 0x3F800000, 0x7FFFFFFF),
    (0xFFFFFFFF, 0x3F800000, 0xFFFFFFFF),
    (0x3F800000, 0x7FFFFFFF, 0x1F3504F3),
    (0x3F800000, 0xFFFFFFFF, 0x1F3504F3),
    (0x7FFFFFFF, 0x7FFFFFFF, 0x5FB504F2),
    (0xFFFFFFFF, 0xFFFFFFFF, 0xDFB504F2),
    (0x00001337, 0xDEADBEEF, 0x00000000),
    (0x00000000, 0x00000000, 0x7FFFFFFF),
    (0x00000000, 0x80000000, 0x7FFFFFFF),
    (0x00000000, 0x3F800000, 0x00000000),
    (0x00000000, 0xBF800000, 0x00000000),
    (0x00000000, 0x40000000, 0x00000000),
    (0x00000000, 0x3FFFFFFF, 0x00000000),
    (0x00000000, 0x3F800001, 0x00000000),
    (0x00000000, 0x33800000, 0x00000000),
    (0x00000000, 0x00000001, 0x7FFFFFFF),
    (0x00000000, 0x007FFFFF, 0x7FFFFFFF),
    (0x00000000, 0x00800000, 0x00000000),
    (0x00000000, 0x7F7FFFFF, 0x00000000),
    (0x00000000, 0xFF7FFFFF, 0x00000000),
    (0x00000000, 0x7F800000, 0x00000000),
    (0x00000000, 0xFF800000, 0x00000000),
    (0x00000000, 0x7FFFFFFF, 0x00000000),
    (0x00000000, 0xFFFFFFFF, 0x00000000),
    (0x00000000, 0x3EAAAAAB, 0x00000000),
    (0x00000000, 0x40490FDB, 0x00000000),
    (0x00000000, 0x5F800000, 0x00000000),
    (0x80000000, 0x00000000, 0xFFFFFFFF),
    (0x80000000, 0x80000000, 0xFFFFFFFF),
    (0x80000000, 0x3F800000, 0x80000000),
    (0x80000000, 0xBF800000, 0x80000000),
    (0x80000000, 0x40000000, 0x80000000),
    (0x80000000, 0x3FFFFFFF, 0x80000000),
    (0x80000000, 0x3F800001, 0x80000000),
    (0x80000000, 0x33800000, 0x80000000),
    (0x80000000, 0x00000001, 0xFFFFFFFF),
    (0x80000000, 0x007FFFFF, 0xFFFFFFFF),
    (0x80000000, 0x00800000, 0x80000000),
    (0x80000000, 0x7F7FFFFF, 0x80000000),
    (0x80000000, 0xFF7FFFFF, 0x80000000),
    (0x80000000, 0x7F800000, 0x80000000),
    (0x80000000, 0xFF800000, 0x80000000),
    (0x80000000, 0x7FFFFFFF, 0x80000000),
    (0x80000000, 0xFFFFFFFF, 0x80000000),
    (0x80000000, 0x3EAAAAAB, 0x80000000),
    (0x80000000, 0x40490FDB, 0x80000000),
    (0x80000000, 0x5F800000, 0x80000000),
    (0x3F800000, 0x00000000, 0x7FFFFFFF),
    (0x3F800000, 0x80000000, 0x7FFFFFFF),
    (0x3F800000, 0x3F800000, 0x3F800000),
    (0x3F800000, 0xBF800000, 0x3F800000),
    (0x3F800000, 0x40000000, 0x3F3504F3),
    (0x3F800000, 0x3FFFFFFF, 0x3F3504F3),
    (0x3F800000, 0x3F800001, 0x3F800000),
    (0x3F800000, 0x33800000, 0x45800000),
    (0x3F800000, 0x00000001, 0x7FFFFFFF),
    (0x3F800000, 0x007FFFFF, 0x7FFFFFFF),
    (0x3F800000, 0x00800000, 0x5F000000),
    (0x3F800000, 0x7F7FFFFF, 0x1F800001),
    (0x3F800000, 0xFF7FFFFF, 0x1F800001),
    (0x3F800000, 0x7F800000, 0x1F800000),
    (0x3F800000, 0xFF800000, 0x1F800000),
    (0x3F800000, 0x7FFFFFFF, 0x1F3504F3),
    (0x3F800000, 0xFFFFFFFF, 0x1F3504F3),
    (0x3F800000, 0x3EAAAAAB, 0x3FDDB3D8),
    (0x3F800000, 0x40490FDB, 0x3F106EBB),
    (0x3F800000, 0x5F800000, 0x2F800000),
    (0xBF800000, 0x00000000, 0xFFFFFFFF),
    (0xBF800000, 0x80000000, 0xFFFFFFFF),
    (0xBF800000, 0x3F800000, 0xBF800000),
    (0xBF800000, 0xBF800000, 0xBF800000),
    (0xBF800000, 0x40000000, 0xBF3504F3),
    (0xBF800000, 0x3FFFFFFF, 0xBF3504F3),
    (0xBF800000, 0x3F800001, 0xBF800000),
    (0xBF800000, 0x33800000, 0xC5800000),
    (0xBF800000, 0x00000001, 0xFFFFFFFF),
    (0xBF800000, 0x007FFFFF, 0xFFFFFFFF),
    (0xBF800000, 0x00800000, 0xDF000000),
    (0xBF800000, 0x7F7FFFFF, 0x9F800001),
    (0xBF800000, 0xFF7FFFFF, 0x9F800001),
    (0xBF800000, 0x7F800000, 0x9F800000),
    (0xBF800000, 0xFF800000, 0x9F800000),
    (0xBF800000, 0x7FFFFFFF, 0x9F3504F3),
    (0xBF800000, 0xFFFFFFFF, 0x9F3504F3),
    (0xBF800000, 0x3EAAAAAB, 0xBFDDB3D8),
    (0xBF800000, 0x40490FDB, 0xBF106EBB),
    (0xBF800000, 0x5F800000, 0xAF800000),
    (0x40000000, 0x00000000, 0x7FFFFFFF),
    (0x40000000, 0x80000000, 0x7FFFFFFF),
    (0x40000000, 0x3F800000, 0x40000000),
    (0x40000000, 0xBF800000, 0x40000000),
    (0x40000000, 0x40000000, 0x3FB504F3),
    (0x40000000, 0x3FFFFFFF, 0x3FB504F3),
    (0x40000000, 0x3F800001, 0x40000000),
    (0x40000000, 0x33800000, 0x46000000),
    (0x40000000, 0x00000001, 0x7FFFFFFF),
    (0x40000000, 0x007FFFFF, 0x7FFFFFFF),
    (0x40000000, 0x00800000, 0x5F800000),
    (0x40000000, 0x7F7FFFFF, 0x20000001),
    (0x40000000, 0xFF7FFFFF, 0x20000001),
    (0x40000000, 0x7F800000, 0x20000000),
    (0x40000000, 0xFF800000, 0x20000000),
    (0x40000000, 0x7FFFFFFF, 0x1FB504F3),
    (0x40000000, 0xFFFFFFFF, 0x1FB504F3),
    (0x40000000, 0x3EAAAAAB, 0x405DB3D8),
    (0x40000000, 0x40490FDB, 0x3F906EBB),
    (0x40000000, 0x5F800000, 0x30000000),
    (0x3FFFFFFF, 0x00000000, 0x7FFFFFFF),
    (0x3FFFFFFF, 0x80000000, 0x7FFFFFFF),
    (0x3FFFFFFF, 0x3F800000, 0x3FFFFFFF),
    (0x3FFFFFFF, 0xBF800000, 0x3FFFFFFF),
    (0x3FFFFFFF, 0x40000000, 0x3FB504F2),
    (0x3FFFFFFF, 0x3FFFFFFF, 0x3FB504F2),
    (0x3FFFFFFF, 0x3F800001, 0x3FFFFFFF),
    (0x3FFFFFFF, 0x33800000, 0x45FFFFFF),
    (0x3FFFFFFF, 0x00000001, 0x7FFFFFFF),
    (0x3FFFFFFF, 0x007FFFFF, 0x7FFFFFFF),
    (0x3FFFFFFF, 0x00800000, 0x5F7FFFFF),
    (0x3FFFFFFF, 0x7F7FFFFF, 0x20000000),
    (0x3FFFFFFF, 0xFF7FFFFF, 0x20000000),
    (0x3FFFFFFF, 0x7F800000, 0x1FFFFFFF),
    (0x3FFFFFFF, 0xFF800000, 0x1FFFFFFF),
    (0x3FFFFFFF, 0x7FFFFFFF, 0x1FB504F2),
    (0x3FFFFFFF, 0xFFFFFFFF, 0x1FB504F2),
    (0x3FFFFFFF, 0x3EAAAAAB, 0x405DB3D6),
    (0x3FFFFFFF, 0x40490FDB, 0x3F906EBA),
    (0x3FFFFFFF, 0x5F800000, 0x2FFFFFFF),
    (0x3F800001, 0x00000000, 0x7FFFFFFF),
    (0x3F800001, 0x80000000, 0x7FFFFFFF),
    (0x3F800001, 0x3F800000, 0x3F800001),
    (0x3F800001, 0xBF800000, 0x3F800001),
    (0x3F800001, 0x40000000, 0x3F3504F5),
    (0x3F800001, 0x3FFFFFFF, 0x3F3504F5),
    (0x3F800001, 0x3F800001, 0x3F800001),
    (0x3F800001, 0x33800000, 0x45800001),
    (0x3F800001, 0x00000001, 0x7FFFFFFF),
    (0x3F800001, 0x007FFFFF, 0x7FFFFFFF),
    (0x3F800001, 0x00800000, 0x5F000001),
    (0x3F800001, 0x7F7FFFFF, 0x1F800001),
    (0x3F800001, 0xFF7FFFFF, 0x1F800001),
    (0x3F800001, 0x7F800000, 0x1F800001),
    (0x3F800001, 0xFF800000, 0x1F800001),
    (0x3F800001, 0x7FFFFFFF, 0x1F3504F5),
    (0x3F800001, 0xFFFFFFFF, 0x1F3504F5),
    (0x3F800001, 0x3EAAAAAB, 0x3FDDB3D9),
    (0x3F800001, 0x40490FDB, 0x3F106EBC),
    (0x3F800001, 0x5F800000, 0x2F800001),
    (0x33800000, 0x00000000, 0x7FFFFFFF),
    (0x33800000, 0x80000000, 0x7FFFFFFF),
    (0x33800000, 0x3F800000, 0x33800000),
    (0x33800000, 0xBF800000, 0x33800000),
    (0x33800000, 0x40000000, 0x333504F3),
    (0x33800000, 0x3FFFFFFF, 0x333504F3),
    (0x33800000, 0x3F800001, 0x33800000),
    (0x33800000, 0x33800000, 0x39800000),
    (0x33800000, 0x00000001, 0x7FFFFFFF),
    (0x33800000, 0x007FFFFF, 0x7FFFFFFF),
    (0x33800000, 0x00800000, 0x53000000),
    (0x33800000, 0x7F7FFFFF, 0x13800001),
    (0x33800000, 0xFF7FFFFF, 0x13800001),
    (0x33800000, 0x7F800000, 0x13800000),
    (0x33800000, 0xFF800000, 0x13800000),
    (0x33800000, 0x7FFFFFFF, 0x133504F3),
    (0x33800000, 0xFFFFFFFF, 0x133504F3),
    (0x33800000, 0x3EAAAAAB, 0x33DDB3D8),
    (0x33800000, 0x40490FDB, 0x33106EBB),
    (0x33800000, 0x5F800000, 0x23800000),
    (0x00000001, 0x00000000, 0x7FFFFFFF),
    (0x00000001, 0x80000000, 0x7FFFFFFF),
    (0x00000001, 0x3F800000, 0x00000000),
    (0x00000001, 0xBF800000, 0x00000000),
    (0x00000001, 0x40000000, 0x00000000),
    (0x00000001, 0x3FFFFFFF, 0x00000000),
    (0x00000001, 0x3F800001, 0x00000000),
    (0x00000001, 0x33800000, 0x00000000),
    (0x00000001, 0x00000001, 0x7FFFFFFF),
    (0x00000001, 0x007FFFFF, 0x7FFFFFFF),
    (0x00000001, 0x00800000, 0x00000000),
    (0x00000001, 0x7F7FFFFF, 0x00000000),
    (0x00000001, 0xFF7FFFFF, 0x00000000),
    (0x00000001, 0x7F800000, 0x00000000),
    (0x00000001, 0xFF800000, 0x00000000),
    (0x00000001, 0x7FFFFFFF, 0x00000000),
    (0x00000001, 0xFFFFFFFF, 0x00000000),
    (0x00000001, 0x3EAAAAAB, 0x00000000),
    (0x00000001, 0x40490FDB, 0x00000000),
    (0x00000001, 0x5F800000, 0x00000000),
    (0x007FFFFF, 0x00000000, 0x7FFFFFFF),
    (0x007FFFFF, 0x80000000, 0x7FFFFFFF),
    (0x007FFFFF, 0x3F800000, 0x00000000),
    (0x007FFFFF, 0xBF800000, 0x00000000),
    (0x007FFFFF, 0x40000000, 0x00000000),
    (0x007FFFFF, 0x3FFFFFFF, 0x00000000),
    (0x007FFFFF, 0x3F800001, 0x00000000),
    (0x007FFFFF, 0x33800000, 0x00000000),
    (0x007FFFFF, 0x00000001, 0x7FFFFFFF),
    (0x007FFFFF, 0x007FFFFF, 0x7FFFFFFF),
    (0x007FFFFF, 0x00800000, 0x00000000),
    (0x007FFFFF, 0x7F7FFFFF, 0x00000000),
    (0x007FFFFF, 0xFF7FFFFF, 0x00000000),
    (0x007FFFFF, 0x7F800000, 0x00000000),
    (0x007FFFFF, 0xFF800000, 0x00000000),
    (0x007FFFFF, 0x7FFFFFFF, 0x00000000),
    (0x007FFFFF, 0xFFFFFFFF, 0x00000000),
    (0x007FFFFF, 0x3EAAAAAB, 0x00000000),
    (0x007FFFFF, 0x40490FDB, 0x00000000),
    (0x007FFFFF, 0x5F800000, 0x00000000),
    (0x00800000, 0x00000000, 0x7FFFFFFF),
    (0x00800000, 0x80000000, 0x7FFFFFFF),
    (0x00800000, 0x3F800000, 0x00800000),
    (0x00800000, 0xBF800000, 0x00800000),
    (0x00800000, 0x40000000, 0x00000000),
    (0x00800000, 0x3FFFFFFF, 0x00000000),
    (0x00800000, 0x3F800001, 0x00800000),
    (0x00800000, 0x33800000, 0x06800000),
    (0x00800000, 0x00000001, 0x7FFFFFFF),
    (0x00800000, 0x007FFFFF, 0x7FFFFFFF),
    (0x00800000, 0x00800000, 0x20000000),
    (0x00800000, 0x7F7FFFFF, 0x00000000),
    (0x00800000, 0xFF7FFFFF, 0x00000000),
    (0x00800000, 0x7F800000, 0x00000000),
    (0x00800000, 0xFF800000, 0x00000000),
    (0x00800000, 0x7FFFFFFF, 0x00000000),
    (0x00800000, 0xFFFFFFFF, 0x00000000),
    (0x00800000, 0x3EAAAAAB, 0x00DDB3D8),
    (0x00800000, 0x40490FDB, 0x00000000),
    (0x00800000, 0x5F800000, 0x00000000),
    (0x7F7FFFFF, 0x00000000, 0x7FFFFFFF),
    (0x7F7FFFFF, 0x80000000, 0x7FFFFFFF),
    (0x7F7FFFFF, 0x3F800000, 0x7F7FFFFF),
    (0x7F7FFFFF, 0xBF800000, 0x7F7FFFFF),
    (0x7F7FFFFF, 0x40000000, 0x7F3504F2),
    (0x7F7FFFFF, 0x3FFFFFFF, 0x7F3504F2),
    (0x7F7FFFFF, 0x3F800001, 0x7F7FFFFF),
    (0x7F7FFFFF, 0x33800000, 0x7FFFFFFF),
    (0x7F7FFFFF, 0x00000001, 0x7FFFFFFF),
    (0x7F7FFFFF, 0x007FFFFF, 0x7FFFFFFF),
    (0x7F7FFFFF, 0x00800000, 0x7FFFFFFF),
    (0x7F7FFFFF, 0x7F7FFFFF, 0x5F800000),
    (0x7F7FFFFF, 0xFF7FFFFF, 0x5F800000),
    (0x7F7FFFFF, 0x7F800000, 0x5F7FFFFF),
    (0x7F7FFFFF, 0xFF800000, 0x5F7FFFFF),
    (0x7F7FFFFF, 0x7FFFFFFF, 0x5F3504F2),
    (0x7F7FFFFF, 0xFFFFFFFF, 0x5F3504F2),
    (0x7F7FFFFF, 0x3EAAAAAB, 0x7FDDB3D6),
    (0x7F7FFFFF, 0x40490FDB, 0x7F106EBA),
    (0x7F7FFFFF, 0x5F800000, 0x6F7FFFFF),
    (0xFF7FFFFF, 0x00000000, 0xFFFFFFFF),
    (0xFF7FFFFF, 0x80000000, 0xFFFFFFFF),
    (0xFF7FFFFF, 0x3F800000, 0xFF7FFFFF),
    (0xFF7FFFFF, 0xBF800000, 0xFF7FFFFF),
    (0xFF7FFFFF, 0x40000000, 0xFF3504F2),
    (0xFF7FFFFF, 0x3FFFFFFF, 0xFF3504F2),
    (0xFF7FFFFF, 0x3F800001, 0xFF7FFFFF),
    (0xFF7FFFFF, 0x33800000, 0xFFFFFFFF),
    (0xFF7FFFFF, 0x00000001, 0xFFFFFFFF),
    (0xFF7FFFFF, 0x007FFFFF, 0xFFFFFFFF),
    (0xFF7FFFFF, 0x00800000, 0xFFFFFFFF),
    (0xFF7FFFFF, 0x7F7FFFFF, 0xDF800000),
    (0xFF7FFFFF, 0xFF7FFFFF, 0xDF800000),
    (0xFF7FFFFF, 0x7F800000, 0xDF7FFFFF),
    (0xFF7FFFFF, 0xFF800000, 0xDF7FFFFF),
    (0xFF7FFFFF, 0x7FFFFFFF, 0xDF3504F2),
    (0xFF7FFFFF, 0xFFFFFFFF, 0xDF3504F2),
    (0xFF7FFFFF, 0x3EAAAAAB, 0xFFDDB3D6),
    (0xFF7FFFFF, 0x40490FDB, 0xFF106EBA),
    (0xFF7FFFFF, 0x5F800000, 0xEF7FFFFF),
    (0x7F800000, 0x00000000, 0x7FFFFFFF),
    (0x7F800000, 0x80000000, 0x7FFFFFFF),
    (0x7F800000, 0x3F800000, 0x7F800000),
    (0x7F800000, 0xBF800000, 0x7F800000),
    (0x7F800000, 0x40000000, 0x7F3504F3),
    (0x7F800000, 0x3FFFFFFF, 0x7F3504F3),
    (0x7F800000, 0x3F800001, 0x7F800000),
    (0x7F800000, 0x33800000, 0x7FFFFFFF),
    (0x7F800000, 0x00000001, 0x7FFFFFFF),
    (0x7F800000, 0x007FFFFF, 0x7FFFFFFF),
    (0x7F800000, 0x00800000, 0x7FFFFFFF),
    (0x7F800000, 0x7F7FFFFF, 0x5F800001),
    (0x7F800000, 0xFF7FFFFF, 0x5F800001),
    (0x7F800000, 0x7F800000, 0x5F800000),
    (0x7F800000, 0xFF800000, 0x5F800000),
    (0x7F800000, 0x7FFFFFFF, 0x5F3504F3),
    (0x7F800000, 0xFFFFFFFF, 0x5F3504F3),
    (0x7F800000, 0x3EAAAAAB, 0x7FDDB3D8),
    (0x7F800000, 0x40490FDB, 0x7F106EBB),
    (0x7F800000, 0x5F800000, 0x6F800000),
    (0xFF800000, 0x00000000, 0xFFFFFFFF),
    (0xFF800000, 0x80000000, 0xFFFFFFFF),
    (0xFF800000, 0x3F800000, 0xFF800000),
    (0xFF800000, 0xBF800000, 0xFF800000),
    (0xFF800000, 0x40000000, 0xFF3504F3),
    (0xFF800000, 0x3FFFFFFF, 0xFF3504F3),
    (0xFF800000, 0x3F800001, 0xFF800000),
    (0xFF800000, 0x33800000, 0xFFFFFFFF),
    (0xFF800000, 0x00000001, 0xFFFFFFFF),
    (0xFF800000, 0x007FFFFF, 0xFFFFFFFF),
    (0xFF800000, 0x00800000, 0xFFFFFFFF),
    (0xFF800000, 0x7F7FFFFF, 0xDF800001),
    (0xFF800000, 0xFF7FFFFF, 0xDF800001),
    (0xFF800000, 0x7F800000, 0xDF800000),
    (0xFF800000, 0xFF800000, 0xDF800000),
    (0xFF800000, 0x7FFFFFFF, 0xDF3504F3),
    (0xFF800000, 0xFFFFFFFF, 0xDF3504F3),
    (0xFF800000, 0x3EAAAAAB, 0xFFDDB3D8),
    (0xFF800000, 0x40490FDB, 0xFF106EBB),
    (0xFF800000, 0x5F800000, 0xEF800000),
    (0x7FFFFFFF, 0x00000000, 0x7FFFFFFF),
    (0x7FFFFFFF, 0x80000000, 0x7FFFFFFF),
    (0x7FFFFFFF, 0x3F800000, 0x7FFFFFFF),
    (0x7FFFFFFF, 0xBF800000, 0x7FFFFFFF),
    (0x7FFFFFFF, 0x40000000, 0x7FB504F2),
    (0x7FFFFFFF, 0x3FFFFFFF, 0x7FB504F2),
    (0x7FFFFFFF, 0x3F800001, 0x7FFFFFFF),
    (0x7FFFFFFF, 0x33800000, 0x7FFFFFFF),
    (0x7FFFFFFF, 0x00000001, 0x7FFFFFFF),
    (0x7FFFFFFF, 0x007FFFFF, 0x7FFFFFFF),
    (0x7FFFFFFF, 0x00800000, 0x7FFFFFFF),
    (0x7FFFFFFF, 0x7F7FFFFF, 0x60000000),
    (0x7FFFFFFF, 0xFF7FFFFF, 0x60000000),
    (0x7FFFFFFF, 0x7F800000, 0x5FFFFFFF),
    (0x7FFFFFFF, 0xFF800000, 0x5FFFFFFF),
    (0x7FFFFFFF, 0x7FFFFFFF, 0x5FB504F2),
    (0x7FFFFFFF, 0xFFFFFFFF, 0x5FB504F2),
    (0x7FFFFFFF, 0x3EAAAAAB, 0x7FFFFFFF),
    (0x7FFFFFFF, 0x40490FDB, 0x7F906EBA),
    (0x7FFFFFFF, 0x5F800000, 0x6FFFFFFF),
    (0xFFFFFFFF, 0x00000000, 0xFFFFFFFF),
    (0xFFFFFFFF, 0x80000000, 0xFFFFFFFF),
    (0xFFFFFFFF, 0x3F800000, 0xFFFFFFFF),
    (0xFFFFFFFF, 0xBF800000, 0xFFFFFFFF),
    (0xFFFFFFFF, 0x40000000, 0xFFB504F2),
    (0xFFFFFFFF, 0x3FFFFFFF, 0xFFB504F2),
    (0xFFFFFFFF, 0x3F800001, 0xFFFFFFFF),
    (0xFFFFFFFF, 0x33800000, 0xFFFFFFFF),
    (0xFFFFFFFF, 0x00000001, 0xFFFFFFFF),
    (0xFFFFFFFF, 0x007FFFFF, 0xFFFFFFFF),
    (0xFFFFFFFF, 0x00800000, 0xFFFFFFFF),
    (0xFFFFFFFF, 0x7F7FFFFF, 0xE0000000),
    (0xFFFFFFFF, 0xFF7FFFFF, 0xE0000000),
    (0xFFFFFFFF, 0x7F800000, 0xDFFFFFFF),
    (0xFFFFFFFF, 0xFF800000, 0xDFFFFFFF),
    (0xFFFFFFFF, 0x7FFFFFFF, 0xDFB504F2),
    (0xFFFFFFFF, 0xFFFFFFFF, 0xDFB504F2),
    (0xFFFFFFFF, 0x3EAAAAAB, 0xFFFFFFFF),
    (0xFFFFFFFF, 0x40490FDB, 0xFF906EBA),
    (0xFFFFFFFF, 0x5F800000, 0xEFFFFFFF),
    (0x3EAAAAAB, 0x00000000, 0x7FFFFFFF),
    (0x3EAAAAAB, 0x80000000, 0x7FFFFFFF),
    (0x3EAAAAAB, 0x3F800000, 0x3EAAAAAB),
    (0x3EAAAAAB, 0xBF800000, 0x3EAAAAAB),
    (0x3EAAAAAB, 0x40000000, 0x3E715BF0),
    (0x3EAAAAAB, 0x3FFFFFFF, 0x3E715BF0),
    (0x3EAAAAAB, 0x3F800001, 0x3EAAAAAB),
    (0x3EAAAAAB, 0x33800000, 0x44AAAAAB),
    (0x3EAAAAAB, 0x00000001, 0x7FFFFFFF),
    (0x3EAAAAAB, 0x007FFFFF, 0x7FFFFFFF),
    (0x3EAAAAAB, 0x00800000, 0x5E2AAAAB),
    (0x3EAAAAAB, 0x7F7FFFFF, 0x1EAAAAAC),
    (0x3EAAAAAB, 0xFF7FFFFF, 0x1EAAAAAC),
    (0x3EAAAAAB, 0x7F800000, 0x1EAAAAAB),
    (0x3EAAAAAB, 0xFF800000, 0x1EAAAAAB),
    (0x3EAAAAAB, 0x7FFFFFFF, 0x1E715BF0),
    (0x3EAAAAAB, 0xFFFFFFFF, 0x1E715BF0),
    (0x3EAAAAAB, 0x3EAAAAAB, 0x3F13CD3A),
    (0x3EAAAAAB, 0x40490FDB, 0x3E4093A4),
    (0x3EAAAAAB, 0x5F800000, 0x2EAAAAAB),
    (0x40490FDB, 0x00000000, 0x7FFFFFFF),
    (0x40490FDB, 0x80000000, 0x7FFFFFFF),
    (0x40490FDB, 0x3F800000, 0x40490FDB),
    (0x40490FDB, 0xBF800000, 0x40490FDB),
    (0x40490FDB, 0x40000000, 0x400E2C19),
    (0x40490FDB, 0x3FFFFFFF, 0x400E2C19),
    (0x40490FDB, 0x3F800001, 0x40490FDB),
    (0x40490FDB, 0x33800000, 0x46490FDB),
    (0x40490FDB, 0x00000001, 0x7FFFFFFF),
    (0x40490FDB, 0x007FFFFF, 0x7FFFFFFF),
    (0x40490FDB, 0x00800000, 0x5FC90FDB),
    (0x40490FDB, 0x7F7FFFFF, 0x20490FDC),
    (0x40490FDB, 0xFF7FFFFF, 0x20490FDC),
    (0x40490FDB, 0x7F800000, 0x20490FDB),
    (0x40490FDB, 0xFF800000, 0x20490FDB),
    (0x40490FDB, 0x7FFFFFFF, 0x200E2C19),
    (0x40490FDB, 0xFFFFFFFF, 0x200E2C19),
    (0x40490FDB, 0x3EAAAAAB, 0x40AE1FEF),
    (0x40490FDB, 0x40490FDB, 0x3FE2DFC5),
    (0x40490FDB, 0x5F800000, 0x30490FDB),
    (0x5F800000, 0x00000000, 0x7FFFFFFF),
    (0x5F800000, 0x80000000, 0x7FFFFFFF),
    (0x5F800000, 0x3F800000, 0x5F800000),
    (0x5F800000, 0xBF800000, 0x5F800000),
    (0x5F800000, 0x40000000, 0x5F3504F3),
    (0x5F800000, 0x3FFFFFFF, 0x5F3504F3),
    (0x5F800000, 0x3F800001, 0x5F800000),
    (0x5F800000, 0x33800000, 0x65800000),
    (0x5F800000, 0x00000001, 0x7FFFFFFF),
    (0x5F800000, 0x007FFFFF, 0x7FFFFFFF),
    (0x5F800000, 0x00800000, 0x7F000000),
    (0x5F800000, 0x7F7FFFFF, 0x3F800001),
    (0x5F800000, 0xFF7FFFFF, 0x3F800001),
    (0x5F800000, 0x7F800000, 0x3F800000),
    (0x5F800000, 0xFF800000, 0x3F800000),
    (0x5F800000, 0x7FFFFFFF, 0x3F3504F3),
    (0x5F800000, 0xFFFFFFFF, 0x3F3504F3),
    (0x5F800000, 0x3EAAAAAB, 0x5FDDB3D8),
    (0x5F800000, 0x40490FDB, 0x5F106EBB),
    (0x5F800000, 0x5F800000, 0x4F800000),
]

# (operand, hardware result), sourced from edge.expected's "abs.s:" section
# and arithmetic.expected's "abs:" section.
ABS_ROWS = [
    (0x00000000, 0x00000000),
    (0x80000000, 0x00000000),
    (0x3F800000, 0x3F800000),
    (0xBF800000, 0x3F800000),
    (0x40000000, 0x40000000),
    (0x3FFFFFFF, 0x3FFFFFFF),
    (0x3F800001, 0x3F800001),
    (0x33800000, 0x33800000),
    (0x00000001, 0x00000001),
    (0x007FFFFF, 0x007FFFFF),
    (0x00800000, 0x00800000),
    (0x7F7FFFFF, 0x7F7FFFFF),
    (0xFF7FFFFF, 0x7F7FFFFF),
    (0x7F800000, 0x7F800000),
    (0xFF800000, 0x7F800000),
    (0x7FFFFFFF, 0x7FFFFFFF),
    (0xFFFFFFFF, 0x7FFFFFFF),
    (0x3EAAAAAB, 0x3EAAAAAB),
    (0x40490FDB, 0x40490FDB),
    (0x5F800000, 0x5F800000),
    (0x00000000, 0x00000000),
    (0x80000000, 0x00000000),
    (0x3F800000, 0x3F800000),
    (0x40000000, 0x40000000),
    (0x40400000, 0x40400000),
    (0x7FFFFFFF, 0x7FFFFFFF),
    (0xFFFFFFFF, 0x7FFFFFFF),
    (0x7F800000, 0x7F800000),
    (0xFF800000, 0x7F800000),
    (0x00000000, 0x00000000),
    (0x3F800000, 0x3F800000),
    (0xBF800000, 0x3F800000),
    (0x3FFFFFFF, 0x3FFFFFFF),
    (0x7F800001, 0x7F800001),
    (0x00000001, 0x00000001),
    (0x7FFFFFFF, 0x7FFFFFFF),
    (0xFFFFFFFF, 0x7FFFFFFF),
    (0x00001337, 0x00001337),
    (0xDEADBEEF, 0x5EADBEEF),
]

# (operand, hardware result), sourced from edge.expected's "cvt.w.s:"
# section, plus convert.expected's ceffffff row, the non-saturating
# truncation case right at the saturation threshold.
CVT_ROWS = [
    (0x00000000, 0x00000000),
    (0x80000000, 0x00000000),
    (0x3F800000, 0x00000001),
    (0xBF800000, 0xFFFFFFFF),
    (0x40000000, 0x00000002),
    (0x3FFFFFFF, 0x00000001),
    (0x3F800001, 0x00000001),
    (0x33800000, 0x00000000),
    (0x00000001, 0x00000000),
    (0x007FFFFF, 0x00000000),
    (0x00800000, 0x00000000),
    (0x7F7FFFFF, 0x7FFFFFFF),
    (0xFF7FFFFF, 0x80000000),
    (0x7F800000, 0x7FFFFFFF),
    (0xFF800000, 0x80000000),
    (0x7FFFFFFF, 0x7FFFFFFF),
    (0xFFFFFFFF, 0x80000000),
    (0x3EAAAAAB, 0x00000000),
    (0x40490FDB, 0x00000003),
    (0x5F800000, 0x7FFFFFFF),
    (0xCEFFFFFF, 0x80000080),
]


def classify_rsqrt(den_bits, hw_result):
    """Which of frsqrtf's cases a (denominator, hardware result) pair falls
    into. "sentinel" is the divide-by-zero case (den is zero/denormal, so
    the EE's own sqrt(den) never runs); "overflow" is a quotient whose
    biased exponent would need to reach 255 to represent on the EE. Both
    land on hardware's 0x7FFFFFFF/0xFFFFFFFF bit pattern, and both are where
    this header deliberately returns +/-FLT_MAX instead; see
    docs/fpu-native-semantics.md. Anything else is a result this header
    computes the ordinary way, either an exact zero or a normal-range value
    with the measured <=1 ULP residual."""
    if (den_bits & EXP_MASK) == 0:
        return "sentinel"
    if (hw_result & EXP_MASK) == EXP_MASK:
        return "overflow"
    if (hw_result & ~SIGN) == 0:
        return "exact_zero"
    return "normal"


def expected_native_rsqrt(num_bits, den_bits, hw_result):
    """What this header's frsqrtf is defined to return, which is hardware's
    own value except in the two sentinel-shaped cases, where it substitutes
    +/-FLT_MAX for host safety (docs/fpu-native-semantics.md explains why)."""
    category = classify_rsqrt(den_bits, hw_result)
    if category == "sentinel":
        sign = num_bits & SIGN
        return sign | FMAX_BITS, category
    if category == "overflow":
        sign = hw_result & SIGN
        return sign | FMAX_BITS, category
    return hw_result, category


def classify_sqrt(bits):
    return "zero" if (bits & EXP_MASK) == 0 else "normal"


SMALLEST_NORMAL = 0x00800000  # the EE's smallest nonzero magnitude; no denormals exist below it


def within_one_ulp(native, expected):
    """True if native and expected agree on sign and differ by at most 1 in
    the raw magnitude bits: the measured, bounded residual from using host
    sqrt/division instead of the EE's own approximation algorithm (see
    docs/fpu-native-semantics.md, "The normal-range residual").

    Zero and the smallest normal are adjacent representable EE values (the
    EE has no denormals in between), so a genuine 1-ULP residual landing on
    that boundary looks like a full jump in raw magnitude bits, not a
    difference of 1; treat that specific jump as the same 1-ULP case too."""
    if (native & SIGN) != (expected & SIGN):
        return native == expected == 0 or native == expected
    native_mag = native & ~SIGN
    expected_mag = expected & ~SIGN
    if {native_mag, expected_mag} == {0, SMALLEST_NORMAL}:
        return True
    return abs(native_mag - expected_mag) <= 1


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
        "static const unsigned int sqrt_in[] = {",
    ]
    lines += ["    0x%08Xu," % bits for bits, _ in SQRT_ROWS]
    lines.append("};")
    lines.append("static const unsigned int rsqrt_num[] = {")
    lines += ["    0x%08Xu," % num for num, _, _ in RSQRT_ROWS]
    lines.append("};")
    lines.append("static const unsigned int rsqrt_den[] = {")
    lines += ["    0x%08Xu," % den for _, den, _ in RSQRT_ROWS]
    lines.append("};")
    lines.append("static const unsigned int abs_in[] = {")
    lines += ["    0x%08Xu," % bits for bits, _ in ABS_ROWS]
    lines.append("};")
    lines.append("static const unsigned int cvt_in[] = {")
    lines += ["    0x%08Xu," % bits for bits, _ in CVT_ROWS]
    lines.append("};")
    lines.append("int main(void) {")
    lines.append("    unsigned int i;")
    lines.append("    for (i = 0; i < sizeof(sqrt_in) / sizeof(sqrt_in[0]); i++) {")
    lines.append("        W in, out;")
    lines.append("        in.u = sqrt_in[i];")
    lines.append("        out.f = fsqrtf(in.f);")
    lines.append('        printf("SQRT %u %u\\n", in.u, out.u);')
    lines.append("    }")
    lines.append("    for (i = 0; i < sizeof(rsqrt_num) / sizeof(rsqrt_num[0]); i++) {")
    lines.append("        W num, den, out;")
    lines.append("        num.u = rsqrt_num[i];")
    lines.append("        den.u = rsqrt_den[i];")
    lines.append("        out.f = frsqrtf(num.f, den.f);")
    lines.append('        printf("RSQRT %u %u %u\\n", num.u, den.u, out.u);')
    lines.append("    }")
    lines.append("    for (i = 0; i < sizeof(abs_in) / sizeof(abs_in[0]); i++) {")
    lines.append("        W in, out;")
    lines.append("        in.u = abs_in[i];")
    lines.append("        out.f = ffabsf(in.f);")
    lines.append('        printf("ABS %u %u\\n", in.u, out.u);')
    lines.append("    }")
    lines.append("    for (i = 0; i < sizeof(cvt_in) / sizeof(cvt_in[0]); i++) {")
    lines.append("        W in;")
    lines.append("        int cv;")
    lines.append("        in.u = cvt_in[i];")
    lines.append("        cv = fcvtws(in.f);")
    lines.append('        printf("CVT %u %u\\n", in.u, (unsigned int)cv);')
    lines.append("    }")
    lines.append("    return 0;")
    lines.append("}")
    src = tmpdir / "fpu_native_program.c"
    src.write_text("\n".join(lines) + "\n")
    return src


class NativeFpuMatchesHardware(unittest.TestCase):
    """Differential check: the header's NATIVE_FPU bodies vs. literal
    ps2autotests hardware results."""

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
        lines = run_result.stdout.splitlines()
        sqrt_lines = [ln for ln in lines if ln.startswith("SQRT ")]
        rsqrt_lines = [ln for ln in lines if ln.startswith("RSQRT ")]
        abs_lines = [ln for ln in lines if ln.startswith("ABS ")]
        cvt_lines = [ln for ln in lines if ln.startswith("CVT ")]
        assert len(sqrt_lines) == len(SQRT_ROWS)
        assert len(rsqrt_lines) == len(RSQRT_ROWS)
        assert len(abs_lines) == len(ABS_ROWS)
        assert len(cvt_lines) == len(CVT_ROWS)

        cls.sqrt_results = []
        for (bits, hw), line in zip(SQRT_ROWS, sqrt_lines):
            _, in_u, native = line.split()
            native = int(native)
            assert int(in_u) == bits
            cls.sqrt_results.append((bits, hw, native))

        cls.rsqrt_results = []
        for (num, den, hw), line in zip(RSQRT_ROWS, rsqrt_lines):
            _, num_u, den_u, native = line.split()
            native = int(native)
            assert int(num_u) == num and int(den_u) == den
            cls.rsqrt_results.append((num, den, hw, native))

        cls.abs_results = []
        for (bits, hw), line in zip(ABS_ROWS, abs_lines):
            _, in_u, native = line.split()
            cls.abs_results.append((bits, hw, int(native)))

        cls.cvt_results = []
        for (bits, hw), line in zip(CVT_ROWS, cvt_lines):
            _, in_u, native = line.split()
            cls.cvt_results.append((bits, hw, int(native)))

    def test_no_compiler_warnings(self):
        self.assertEqual(self.warnings.strip(), "", self.warnings)

    def test_sqrt_matches_hardware(self):
        for bits, hw, native in self.sqrt_results:
            with self.subTest(op=hex(bits)):
                if classify_sqrt(bits) == "zero":
                    self.assertEqual(native, 0,
                                      "sqrt.s(0x%08X): native 0x%08X, hardware always drops sign to +0"
                                      % (bits, native))
                else:
                    self.assertTrue(within_one_ulp(native, hw),
                                     "sqrt.s(0x%08X): native 0x%08X vs hardware 0x%08X, more than 1 ULP apart"
                                     % (bits, native, hw))

    def test_rsqrt_matches_hardware_or_documented_fmax_substitution(self):
        for num, den, hw, native in self.rsqrt_results:
            expected, category = expected_native_rsqrt(num, den, hw)
            with self.subTest(num=hex(num), den=hex(den), category=category):
                if category in ("sentinel", "overflow", "exact_zero"):
                    self.assertEqual(native, expected,
                                      "rsqrt.s(0x%08X, 0x%08X) [%s]: native 0x%08X != expected 0x%08X "
                                      "(hardware itself reads 0x%08X; see docs/fpu-native-semantics.md)"
                                      % (num, den, category, native, expected, hw))
                else:
                    self.assertTrue(within_one_ulp(native, hw),
                                     "rsqrt.s(0x%08X, 0x%08X): native 0x%08X vs hardware 0x%08X, more than 1 ULP apart"
                                     % (num, den, native, hw))

    def test_abs_matches_hardware_exactly(self):
        for bits, hw, native in self.abs_results:
            with self.subTest(op=hex(bits)):
                self.assertEqual(native, hw,
                                  "abs.s(0x%08X): native 0x%08X != hardware 0x%08X"
                                  % (bits, native, hw))

    def test_cvtws_matches_hardware_exactly(self):
        for bits, hw, native in self.cvt_results:
            with self.subTest(op=hex(bits)):
                self.assertEqual(native, hw,
                                  "cvt.w.s(0x%08X): native %d != hardware %d"
                                  % (bits, native, hw))


if __name__ == "__main__":
    unittest.main()
