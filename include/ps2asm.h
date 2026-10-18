#ifndef PS2ASM_H
#define PS2ASM_H

#include "common.h"

/* EE instructions ee-gcc 2.96 never selects from C: VU0 macro-mode COP2, the
 * MMI 128-bit integer ops, and the pipeline-1 integer multiply/divide.
 *
 * No macro parameter reaches the assembler. Opcodes, suffixes and every
 * vf/vi register are literal inside the macro body; parameters are C
 * expressions bound through operand constraints and substituted as %0, %1.
 * One macro per concrete operation, named after what the operation does.
 *
 * The engine's VU0 convention fixes the register roles: vf10 is the working
 * vector, vf11 the second operand, vf2 the broadcast scratch and vf28-vf31
 * the matrix bank. See include/README.md for what cc1 does with each
 * sequence. */

/* ---- quadword memory ---- */

/* The record keeps a 16-byte memory operand from decaying to a pointer. */
typedef struct {
    u32 words[4];
} Quad;
typedef struct {
    f32 components[4];
} QuadF32;

/* lq/sq through $2. Plain C `*(u128 *)dst = *(u128 *)src` gives the same pair
 * but lets the sq fall into a jr delay slot. */
#define PS2_COPY_QUAD(dst, src) __asm__ volatile ( \
    ".set noreorder\n\tlq $2, 0(%1)\n\tsq $2, 0(%0)\n\t.set reorder" \
    : : "r" (dst), "r" (src) : "$2", "memory")

/* sq $0: EE GCC materialises a C 128-bit zero in a register first. */
#define PS2_STORE_ZERO_QUAD(dst) __asm__ volatile ( \
    "sq $0, 0(%0)" : : "r" (dst) : "memory")

/* ---- quadword between memory and the VU0 register file ---- */

/* The _TO/_FROM forms take a C lvalue and let cc1 pick the addressing mode;
 * the plain forms take an address already in a register. */
#define VU0_LOAD_VF10(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r" (src))
#define VU0_LOAD_VF10_FROM(lvalue) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf10, %0\n\t.set reorder" : : "m" (lvalue))
#define VU0_STORE_VF10(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r" (dst) : "memory")
#define VU0_STORE_VF10_TO(lvalue) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf10, %0\n\t.set reorder" : "=m" (lvalue))

#define VU0_LOAD_VF11(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf11, 0(%0)\n\t.set reorder" : : "r" (src))
#define VU0_LOAD_VF11_FROM(lvalue) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf11, %0\n\t.set reorder" : : "m" (lvalue))
#define VU0_STORE_VF11(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf11, 0(%0)\n\t.set reorder" : : "r" (dst) : "memory")
#define VU0_STORE_VF11_TO(lvalue) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf11, %0\n\t.set reorder" : "=m" (lvalue))

/* Four quadwords between memory and the matrix bank vf28-vf31. */
#define VU0_LOAD_MATRIX(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf28, 0(%0)\n\tlqc2 vf29, 16(%0)\n\t" \
    "lqc2 vf30, 32(%0)\n\tlqc2 vf31, 48(%0)\n\t.set reorder" \
    : : "r" (src))
#define VU0_STORE_MATRIX(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf28, 0(%0)\n\tsqc2 vf29, 16(%0)\n\t" \
    "sqc2 vf30, 32(%0)\n\tsqc2 vf31, 48(%0)\n\t.set reorder" \
    : : "r" (dst) : "memory")

/* Preserve the current transform in the secondary matrix bank. */
#define VU0_COPY_MATRIX_TO_SECONDARY() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf24, vf28\n\tvmove.xyzw vf25, vf29\n\t" \
    "vmove.xyzw vf26, vf30\n\tvmove.xyzw vf27, vf31\n\t.set reorder")

/* ---- GPR <-> VU0 register ---- */

/* qmtc2/qmfc2 with the .ni (no interlock) suffix the SDK inlines use. */
#define VU0_VF10_FROM_GPR(r) __asm__ volatile ( \
    ".set noreorder\n\tqmtc2.ni %0, vf10\n\t.set reorder" : : "r" (r))
#define VU0_VF10_TO_GPR(r) __asm__ volatile ( \
    ".set noreorder\n\tqmfc2.ni %0, vf10\n\t.set reorder" : "=r" (r))
#define VU0_VF11_FROM_GPR(r) __asm__ volatile ( \
    ".set noreorder\n\tqmtc2.ni %0, vf11\n\t.set reorder" : : "r" (r))
#define VU0_VF11_TO_GPR(r) __asm__ volatile ( \
    ".set noreorder\n\tqmfc2.ni %0, vf11\n\t.set reorder" : "=r" (r))

/* ---- scaling by a scalar broadcast through vf2.x ---- */

/* Broadcast a C float into vf2.x and scale. The GPR the bits travel through is
 * the compiler's, not a fixed scratch: two consecutive scales in one function
 * use two different registers in retail code. */
#define VU0_SCALE_VF10(f) do { u32 ps2Bits; __asm__ volatile ( \
    ".set noreorder\n\tmfc1 %0, %1\n\tqmtc2.ni %0, vf2\n\t" \
    "vmulx.xyzw vf10, vf10, vf2x\n\t.set reorder" \
    : "=&r" (ps2Bits) : "f" (f)); } while (0)
#define VU0_SCALE_VF11(f) do { u32 ps2Bits; __asm__ volatile ( \
    ".set noreorder\n\tmfc1 %0, %1\n\tqmtc2.ni %0, vf2\n\t" \
    "vmulx.xyzw vf11, vf11, vf2x\n\t.set reorder" \
    : "=&r" (ps2Bits) : "f" (f)); } while (0)
/* The same scale from a GPR that already holds the binary32 bits. */
#define VU0_SCALE_VF10_BITS(r) __asm__ volatile ( \
    ".set noreorder\n\tqmtc2.ni %0, vf2\n\tvmulx.xyzw vf10, vf10, vf2x\n\t.set reorder" \
    : : "r" (r))
#define VU0_SCALE_VF11_BITS(r) __asm__ volatile ( \
    ".set noreorder\n\tqmtc2.ni %0, vf2\n\tvmulx.xyzw vf11, vf11, vf2x\n\t.set reorder" \
    : : "r" (r))
#define VU0_SET_VF2X(r) __asm__ volatile ( \
    ".set noreorder\n\tqmtc2.ni %0, vf2\n\t.set reorder" : : "r" (r))

/* ---- VU0 macro-mode arithmetic on the fixed working pair ---- */

#define VU0_COPY_VF10_TO_VF11() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf11, vf10\n\t.set reorder")
#define VU0_COPY_VF11_TO_VF10() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf10, vf11\n\t.set reorder")
/* vf12 is the spare the effect code parks a vector in across a call. */
#define VU0_COPY_VF10_TO_VF12() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf12, vf10\n\t.set reorder")
#define VU0_COPY_VF12_TO_VF10() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf10, vf12\n\t.set reorder")
#define VU0_COPY_VF12_TO_VF11() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf11, vf12\n\t.set reorder")

#define VU0_ADD_VF10_VF11() __asm__ volatile ( \
    ".set noreorder\n\tvadd.xyzw vf10, vf10, vf11\n\t.set reorder")
#define VU0_SUB_VF10_VF11() __asm__ volatile ( \
    ".set noreorder\n\tvsub.xyzw vf10, vf10, vf11\n\t.set reorder")
#define VU0_MUL_VF10_VF11() __asm__ volatile ( \
    ".set noreorder\n\tvmul.xyzw vf10, vf10, vf11\n\t.set reorder")
#define VU0_ADD_VF10_VF11_XYZ() __asm__ volatile ( \
    ".set noreorder\n\tvadd.xyz vf10, vf10, vf11\n\t.set reorder")
#define VU0_SUB_VF10_VF11_XYZ() __asm__ volatile ( \
    ".set noreorder\n\tvsub.xyz vf10, vf10, vf11\n\t.set reorder")
#define VU0_MUL_VF10_VF11_XYZ() __asm__ volatile ( \
    ".set noreorder\n\tvmul.xyz vf10, vf10, vf11\n\t.set reorder")

/* vf10.xyz = -vf10.xyz (vf0.xyz is zero), vf10.w = 0, vf10.w = 1. */
#define VU0_NEGATE_VF10_XYZ() __asm__ volatile ( \
    ".set noreorder\n\tvsub.xyz vf10, vf0, vf10\n\t.set reorder")
#define VU0_CLEAR_VF10_W() __asm__ volatile ( \
    ".set noreorder\n\tvmulx.w vf10, vf10, vf0x\n\t.set reorder")
#define VU0_SET_VF10_W_ONE() __asm__ volatile ( \
    ".set noreorder\n\tvmove.w vf10, vf0\n\t.set reorder")

/* Float <-> integer lane conversion with 0 and 4 fractional bits. */
#define VU0_ITOF0_VF10() __asm__ volatile ( \
    ".set noreorder\n\tvitof0.xyzw vf10, vf10\n\t.set reorder")
#define VU0_FTOI0_VF10() __asm__ volatile ( \
    ".set noreorder\n\tvftoi0.xyzw vf10, vf10\n\t.set reorder")
#define VU0_FTOI4_VF10() __asm__ volatile ( \
    ".set noreorder\n\tvftoi4.xyzw vf10, vf10\n\t.set reorder")

/* vf10 = matrix (vf28-vf31) * vf10: the core of libvu0 sceVu0ApplyMatrix.
 * _POINT takes the source w as 1; _ROTATE uses the 3x3 part only. */
#define VU0_APPLY_MATRIX_VF10() __asm__ volatile ( \
    ".set noreorder\n\tvmulax.xyzw ACC, vf28, vf10x\n\tvmadday.xyzw ACC, vf29, vf10y\n\t" \
    "vmaddaz.xyzw ACC, vf30, vf10z\n\tvmaddw.xyzw vf10, vf31, vf10w\n\t.set reorder")
#define VU0_TRANSFORM_POINT_VF10() __asm__ volatile ( \
    ".set noreorder\n\tvmulax.xyzw ACC, vf28, vf10x\n\tvmadday.xyzw ACC, vf29, vf10y\n\t" \
    "vmaddaz.xyzw ACC, vf30, vf10z\n\tvmaddw.xyzw vf10, vf31, vf0w\n\t.set reorder")
#define VU0_ROTATE_VF10() __asm__ volatile ( \
    ".set noreorder\n\tvmulax.xyzw ACC, vf28, vf10x\n\tvmadday.xyzw ACC, vf29, vf10y\n\t" \
    "vmaddz.xyzw vf10, vf30, vf10z\n\t.set reorder")

/* vf10 = vf10*(1 - t) + vf11*t. vf2 holds t broadcast and vf3.w holds 1 - t;
 * $2 carries the bits, and is deliberately not a clobber: the engine's own
 * sequence does not list it, and cc1 then picks $2 for a nearby address the
 * way retail does. The _BITS form takes a
 * GPR that already holds them, which is what retail emits when one blend
 * factor drives several lerps. */
#define VU0_LERP_VF10_VF11(t) __asm__ volatile ( \
    ".set noreorder\n\tmfc1 $2, %0\n\tqmtc2.ni $2, vf2\n\t" \
    "vsubx.w vf3, vf0, vf2x\n\tvmulax.xyzw ACC, vf11, vf2x\n\t" \
    "vmaddw.xyzw vf10, vf10, vf3w\n\t.set reorder" \
    : : "f" (t))
#define VU0_LERP_VF10_VF11_BITS(r) __asm__ volatile ( \
    ".set noreorder\n\tqmtc2.ni %0, vf2\n\t" \
    "vsubx.w vf3, vf0, vf2x\n\tvmulax.xyzw ACC, vf11, vf2x\n\t" \
    "vmaddw.xyzw vf10, vf10, vf3w\n\t.set reorder" \
    : : "r" (r))
/* binary32 bits of a float in a GPR (mfc1), for the _BITS forms. */
#define PS2_FLOAT_BITS(out, f) __asm__ volatile ( \
    ".set noreorder\n\tmfc1 %0, %1\n\t.set reorder" : "=r" (out) : "f" (f))

/* vf10 = vf10 x vf11 over xyz: the outer-product pair. */
#define VU0_CROSS_VF10_VF11() __asm__ volatile ( \
    ".set noreorder\n\tvopmula.xyz ACC, vf10, vf11\n\t" \
    "vopmsub.xyz vf10, vf11, vf10\n\t.set reorder")
/* out = vf10 . vf11 over xyz as a C float; $2 is scratch. */
#define VU0_DOT_VF10_VF11(out) __asm__ volatile ( \
    ".set noreorder\n\tvmul.xyz vf2, vf10, vf11\n\tvaddy.x vf2, vf2, vf2y\n\t" \
    "vaddz.x vf2, vf2, vf2z\n\tqmfc2.ni $2, vf2\n\tmtc1 $2, %0\n\t.set reorder" \
    : "=f" (out) : : "$2")
/* out = |vf10.xyz|, read back through the Q register and $2. */
#define VU0_LENGTH_VF10(out) __asm__ volatile ( \
    ".set noreorder\n\tvmul.xyz vf2, vf10, vf10\n\tvaddy.x vf2, vf2, vf2y\n\t" \
    "vaddz.x vf2, vf2, vf2z\n\tvsqrt Q, vf2x\n\tvwaitq\n\tcfc2.ni $2, $vi22\n\t" \
    "mtc1 $2, %0\n\t.set reorder" \
    : "=f" (out) : : "$2")
/* vf10 /= |vf10.xyz| (w untouched). */
#define VU0_NORMALIZE_VF10() __asm__ volatile ( \
    ".set noreorder\n\tvmul.xyz vf2, vf10, vf10\n\tvmulax.w ACC, vf0, vf2x\n\t" \
    "vmadday.w ACC, vf0, vf2y\n\tvmaddz.w vf2, vf0, vf2z\n\tvrsqrt Q, vf0w, vf2w\n\t" \
    "vwaitq\n\tvmulq.xyz vf10, vf10, Q\n\t.set reorder")
/* vf10 /= vf10.w, then w = 1: the perspective divide after a point transform. */
#define VU0_PERSPECTIVE_DIVIDE_VF10() __asm__ volatile ( \
    ".set noreorder\n\tvdiv Q, vf0w, vf10w\n\tvmove.w vf10, vf0\n\tvwaitq\n\t" \
    "vmulq.xyzw vf10, vf10, Q\n\t.set reorder")

/* Accumulate the xyz clip tests of a transformed vector against its own w,
 * then read or reset the accumulated flags in vi18. */
#define VU0_CLIP_VF10() __asm__ volatile ( \
    ".set noreorder\n\tvclipw.xyz vf10, vf10w\n\t.set reorder")
#define VU0_READ_CLIP_FLAGS(out) __asm__ volatile ( \
    ".set noreorder\n\tcfc2.ni %0, $vi18\n\t.set reorder" : "=r" (out))
#define VU0_WRITE_CLIP_FLAGS(r) __asm__ volatile ( \
    ".set noreorder\n\tctc2.ni %0, $vi18\n\t.set reorder" : : "r" (r))

/* ---- MMI 128-bit integer ops ---- */

/* cc1 rejects C operators on a 128-bit type ("unsupported wide integer
 * operation"), so every lane op is written out. Operands are u128/s128. */
#define MMI_PEXTLW(out, a, b) __asm__ volatile ("pextlw %0, %1, %2" : "=r" (out) : "r" (a), "r" (b))
#define MMI_PEXTUW(out, a, b) __asm__ volatile ("pextuw %0, %1, %2" : "=r" (out) : "r" (a), "r" (b))
#define MMI_PCPYLD(out, a, b) __asm__ volatile ("pcpyld %0, %1, %2" : "=r" (out) : "r" (a), "r" (b))
#define MMI_PCPYUD(out, a, b) __asm__ volatile ("pcpyud %0, %1, %2" : "=r" (out) : "r" (a), "r" (b))
#define MMI_PADDW(out, a, b) __asm__ volatile ("paddw %0, %1, %2" : "=r" (out) : "r" (a), "r" (b))
#define MMI_PSUBW(out, a, b) __asm__ volatile ("psubw %0, %1, %2" : "=r" (out) : "r" (a), "r" (b))
#define MMI_PAND(out, a, b) __asm__ volatile ("pand %0, %1, %2" : "=r" (out) : "r" (a), "r" (b))
#define MMI_POR(out, a, b) __asm__ volatile ("por %0, %1, %2" : "=r" (out) : "r" (a), "r" (b))

/* Leading sign bits minus one of a word; cc1 has no clz builtin. */
#define MMI_PLZCW(out, in) __asm__ volatile ("plzcw %0, %1" : "=r" (out) : "r" (in))

/* RGBA8888 word -> four float lanes in vf10: bytes to halfwords to words,
 * then vitof0. */
#define MMI_RGBA_TO_VF10(rgba) do { u32 ps2Lanes; __asm__ volatile ( \
    ".set noreorder\n\tpextlb %0, $0, %1\n\tpextlh %0, $0, %0\n\t" \
    "qmtc2.ni %0, vf10\n\tvitof0.xyzw vf10, vf10\n\t.set reorder" \
    : "=&r" (ps2Lanes) : "r" (rgba)); } while (0)
/* The SDK form, which reads the colour word back from its own 16-byte carrier
 * and scales the lanes by `unit`, binary32 bits in a GPR (0x3C000000 = 1/128).
 * $2 is the SDK's scratch and has to stay fixed: letting cc1 allocate it picks
 * a different register from retail. */
#define MMI_RGBA_UNPACK(src, unit) __asm__ volatile ( \
    ".set noreorder\n\tlw $2, 0(%0)\n\tpextlb $2, $0, $2\n\tpextlh $2, $0, $2\n\t" \
    "qmtc2.ni $2, vf10\n\tvitof0.xyzw vf10, vf10\n\tqmtc2.ni %1, vf2\n\t" \
    "vmulx.xyzw vf10, vf10, vf2x\n\t.set reorder" \
    : : "r" (src), "r" (unit) : "$2", "memory")
/* vf10 (floats) -> RGBA8888 word: scale through vf2x, vftoi0, then words to
 * halfwords to bytes. $3 carries the scale bits, as in the SDK sequence. */
#define MMI_RGBA_PACK(out, scale) __asm__ volatile ( \
    ".set noreorder\n\tmfc1 $3, %1\n\tqmtc2.ni $3, vf2\n\t" \
    "vmulx.xyzw vf10, vf10, vf2x\n\tvftoi0.xyzw vf10, vf10\n\tqmfc2.ni %0, vf10\n\t" \
    "ppach %0, $0, %0\n\tppacb %0, $0, %0\n\t.set reorder" \
    : "=r" (out) : "f" (scale) : "$3")
/* The same pack with the scale already in a GPR. */
#define MMI_RGBA_PACK_UNIT(out, unit) __asm__ volatile ( \
    ".set noreorder\n\tqmtc2.ni %1, vf2\n\tvmulx.xyzw vf10, vf10, vf2x\n\t" \
    "vftoi0.xyzw vf10, vf10\n\tqmfc2.ni %0, vf10\n\tppach %0, $0, %0\n\t" \
    "ppacb %0, $0, %0\n\t.set reorder" \
    : "=&r" (out) : "r" (unit))

/* Three unaligned floats at p -> vf10 (w undefined), and the reverse. cc1's
 * own unaligned accesses emit ldl before ldr, and cannot extract a quadword's
 * upper half with a C shift. $2/$3 are scratch. */
#define MMI_LOAD_VEC3_VF10(p) __asm__ volatile ( \
    ".set noreorder\n\tldr $2, 0(%0)\n\tldl $2, 7(%0)\n\tlw $3, 8(%0)\n\t" \
    "pcpyld $2, $3, $2\n\tqmtc2.ni $2, vf10\n\t.set reorder" \
    : : "r" (p) : "$2", "$3")
#define MMI_STORE_VEC3(dst, src) __asm__ volatile ( \
    ".set noreorder\n\tlq $3, 0(%1)\n\tpcpyud $2, $3, $0\n\tsdr $3, 0(%0)\n\t" \
    "sdl $3, 7(%0)\n\tsw $2, 8(%0)\n\t.set reorder" \
    : : "r" (dst), "r" (src) : "$2", "$3", "memory")

/* ---- pipeline-1 integer multiply and divide ---- */

/* cc1 only ever emits mult/div/mflo on pipeline 0, so a second multiply or
 * divide issued alongside one of those has to be written out. */
#define EE_MULT1(out, a, b) __asm__ volatile ( \
    ".set noreorder\n\tmult1 %0, %1, %2\n\t.set reorder" \
    : "=r" (out) : "r" (a), "r" (b))
#define EE_DIV1(quotient, a, b) __asm__ volatile ( \
    ".set noreorder\n\tdiv1 $0, %1, %2\n\tmflo1 %0\n\t.set reorder" \
    : "=r" (quotient) : "r" (a), "r" (b))
#define EE_DIVU1(quotient, a, b) __asm__ volatile ( \
    ".set noreorder\n\tdivu1 $0, %1, %2\n\tmflo1 %0\n\t.set reorder" \
    : "=r" (quotient) : "r" (a), "r" (b))
#define EE_REM1(remainder, a, b) __asm__ volatile ( \
    ".set noreorder\n\tdiv1 $0, %1, %2\n\tmfhi1 %0\n\t.set reorder" \
    : "=r" (remainder) : "r" (a), "r" (b))

/* ---- COP1 square root ---- */

/* sqrt.s exists on the EE, but cc1 2.96 expands a sqrtf call into one only
 * with -fno-math-errno, which the build does not pass. */
#define EE_SQRT_F32(out, in) __asm__ volatile ( \
    ".set noreorder\n\tsqrt.s %0, %1\n\t.set reorder" : "=f" (out) : "f" (in))

/* ---- barriers ---- */

#define EE_SYNC() __asm__ volatile ("sync")
#define EE_SYNC_P() __asm__ volatile ("sync.p")

/* Additional working-register operations used by the trail interpolator. */
#define VU0_ADD_VF10_VF12() __asm__ volatile ( \
    ".set noreorder\n\tvadd.xyzw vf10, vf10, vf12\n\t.set reorder")

#define VU0_ADD_VF12_VF10() __asm__ volatile ( \
    ".set noreorder\n\tvadd.xyzw vf12, vf12, vf10\n\t.set reorder")

/* SDK input-address-only store: audited VU pipelines only. Unlike the
 * normal store it does not invalidate values cached from unrelated memory. */
#define VU0_STORE_VF10_UNCLOBBERED(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" \
    : : "r" (dst))

/* Cell gradient vector operations. */
#define VU0_LOAD_VF11_MEMORY(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf11, 0(%0)\n\t.set reorder" \
    : : "r" (src) : "memory")

#define VU0_SUB_VF11_VF12() __asm__ volatile ( \
    ".set noreorder\n\tvsub.xyzw vf11, vf11, vf12\n\t.set reorder")

/* vf10 = (1 - t) * vf10 + t * vf11, with float bits passed in a GPR. */
#define VU0_LERP_VF10(t) __asm__ volatile ( \
    ".set noreorder\n\tqmtc2.ni %0, vf2\n\tvsubx.w vf3, vf0, vf2x\n\t" \
    "vmulax.xyzw ACC, vf11, vf2x\n\tvmaddw.xyzw vf10, vf10, vf3w\n\t.set reorder" \
    : : "r" (t))

#define VU0_LOAD_VF12(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf12, 0(%0)\n\t.set reorder" \
    : : "r" (src))

/* Ring geometry matrix and working-register operations. */

#define VU0_APPLY_MATRIX_VF11() __asm__ volatile ( \
    ".set noreorder\n\tvmulax.xyzw ACC, vf28, vf11x\n\tvmadday.xyzw ACC, vf29, vf11y\n\t" \
    "vmaddaz.xyzw ACC, vf30, vf11z\n\tvmaddw.xyzw vf11, vf31, vf11w\n\t.set reorder")

#define VU0_LOAD_MATRIX_B(src) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "lqc2 vf24, 0(%0)\n\t" \
    "lqc2 vf25, 16(%0)\n\t" \
    "lqc2 vf26, 32(%0)\n\t" \
    "lqc2 vf27, 48(%0)\n\t" \
    ".set reorder" \
    : : "r" (src) : "memory")

/* Store XYZ from an aligned quadword into the packed VIF stream. */
#define MMI_STORE_VEC3_VALUE(dst, value) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "pcpyud $2, %1, $0\n\t" \
    "sdr %1, 0(%0)\n\t" \
    "sdl %1, 7(%0)\n\t" \
    "sw $2, 8(%0)\n\t" \
    ".set reorder" \
    : : "r" (dst), "r" (value) : "$2", "memory")

#endif /* PS2ASM_H */
