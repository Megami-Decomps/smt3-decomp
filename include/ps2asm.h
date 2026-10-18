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
#define SDF_VU0__COPYVECTOR(dst, src) __asm__ volatile ( \
    ".set noreorder\n\tlq $2, 0(%1)\n\tsq $2, 0(%0)\n\t.set reorder" \
    : : "r" (dst), "r" (src) : "$2", "memory")

/* sq $0: EE GCC materialises a C 128-bit zero in a register first. */
#define PS2_STORE_ZERO_QUAD(dst) __asm__ volatile ( \
    "sq $0, 0(%0)" : : "r" (dst) : "memory")

/* ---- quadword between memory and the VU0 register file ---- */

/* The _TO/_FROM forms take a C lvalue and let cc1 pick the addressing mode;
 * the plain forms take an address already in a register. */
#define SDF_VU0__LOADVECTOR(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r" (src))
#define VU0_LOAD_VF10_FROM(lvalue) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf10, %0\n\t.set reorder" : : "m" (lvalue))
#define SDF_VU0__STOREVECTOR(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r" (dst) : "memory")
#define VU0_STORE_VF10_TO(lvalue) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf10, %0\n\t.set reorder" : "=m" (lvalue))

#define SDF_VU0__LOADVECTOR_B(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf11, 0(%0)\n\t.set reorder" : : "r" (src))
#define VU0_LOAD_VF11_FROM(lvalue) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf11, %0\n\t.set reorder" : : "m" (lvalue))
#define SDF_VU0__STOREVECTOR_B(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf11, 0(%0)\n\t.set reorder" : : "r" (dst) : "memory")
#define VU0_STORE_VF11_TO(lvalue) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf11, %0\n\t.set reorder" : "=m" (lvalue))

/* Four quadwords between memory and the matrix bank vf28-vf31. */
#define SDF_VU0__LOADMATRIX(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf28, 0(%0)\n\tlqc2 vf29, 16(%0)\n\t" \
    "lqc2 vf30, 32(%0)\n\tlqc2 vf31, 48(%0)\n\t.set reorder" \
    : : "r" (src))
#define SDF_VU0__STOREMATRIX(dst) __asm__ volatile ( \
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
#define SDF_VU0__SCALEVECTOR(f) do { u32 ps2Bits; __asm__ volatile ( \
    ".set noreorder\n\tmfc1 %0, %1\n\tqmtc2.ni %0, vf2\n\t" \
    "vmulx.xyzw vf10, vf10, vf2x\n\t.set reorder" \
    : "=&r" (ps2Bits) : "f" (f)); } while (0)
#define SDF_VU0__SCALEVECTOR_B(f) do { u32 ps2Bits; __asm__ volatile ( \
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

#define SDF_VU0__COPYVECTOR_A_TO_B() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf11, vf10\n\t.set reorder")
#define SDF_VU0__COPYVECTOR_B_TO_A() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf10, vf11\n\t.set reorder")
/* vf12 is the spare the effect code parks a vector in across a call. */
#define SDF_VU0__COPYVECTOR_A_TO_C() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf12, vf10\n\t.set reorder")
#define SDF_VU0__COPYVECTOR_C_TO_A() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf10, vf12\n\t.set reorder")
#define SDF_VU0__COPYVECTOR_C_TO_B() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf11, vf12\n\t.set reorder")

#define SDF_VU0__ADDVECTOR() __asm__ volatile ( \
    ".set noreorder\n\tvadd.xyzw vf10, vf10, vf11\n\t.set reorder")
#define SDF_VU0__SUBVECTOR() __asm__ volatile ( \
    ".set noreorder\n\tvsub.xyzw vf10, vf10, vf11\n\t.set reorder")
#define SDF_VU0__MULVECTOR() __asm__ volatile ( \
    ".set noreorder\n\tvmul.xyzw vf10, vf10, vf11\n\t.set reorder")
#define VU0_ADD_VF10_VF11_XYZ() __asm__ volatile ( \
    ".set noreorder\n\tvadd.xyz vf10, vf10, vf11\n\t.set reorder")
#define VU0_SUB_VF10_VF11_XYZ() __asm__ volatile ( \
    ".set noreorder\n\tvsub.xyz vf10, vf10, vf11\n\t.set reorder")
#define VU0_MUL_VF10_VF11_XYZ() __asm__ volatile ( \
    ".set noreorder\n\tvmul.xyz vf10, vf10, vf11\n\t.set reorder")

/* vf10.xyz = -vf10.xyz (vf0.xyz is zero), vf10.w = 0, vf10.w = 1. */
#define SDF_VU0__NEGVECTOR_XYZ() __asm__ volatile ( \
    ".set noreorder\n\tvsub.xyz vf10, vf0, vf10\n\t.set reorder")
#define SDF_VU0__VECTOR_W_SET_ZERO() __asm__ volatile ( \
    ".set noreorder\n\tvmulx.w vf10, vf10, vf0x\n\t.set reorder")
#define SDF_VU0__VECTOR_W_SET_ONE() __asm__ volatile ( \
    ".set noreorder\n\tvmove.w vf10, vf0\n\t.set reorder")

/* Float <-> integer lane conversion with 0 and 4 fractional bits. */
#define SDF_VU0__ITOF0() __asm__ volatile ( \
    ".set noreorder\n\tvitof0.xyzw vf10, vf10\n\t.set reorder")
#define SDF_VU0__FTOI0() __asm__ volatile ( \
    ".set noreorder\n\tvftoi0.xyzw vf10, vf10\n\t.set reorder")
#define SDF_VU0__FTOI4() __asm__ volatile ( \
    ".set noreorder\n\tvftoi4.xyzw vf10, vf10\n\t.set reorder")

/* vf10 = matrix (vf28-vf31) * vf10: the core of libvu0 sceVu0ApplyMatrix.
 * _POINT takes the source w as 1; _ROTATE uses the 3x3 part only. */
#define SDF_VU0__APPLYMATRIX() __asm__ volatile ( \
    ".set noreorder\n\tvmulax.xyzw ACC, vf28, vf10x\n\tvmadday.xyzw ACC, vf29, vf10y\n\t" \
    "vmaddaz.xyzw ACC, vf30, vf10z\n\tvmaddw.xyzw vf10, vf31, vf10w\n\t.set reorder")
#define SDF_VU0__APPLYMATRIX_XYZ1() __asm__ volatile ( \
    ".set noreorder\n\tvmulax.xyzw ACC, vf28, vf10x\n\tvmadday.xyzw ACC, vf29, vf10y\n\t" \
    "vmaddaz.xyzw ACC, vf30, vf10z\n\tvmaddw.xyzw vf10, vf31, vf0w\n\t.set reorder")
#define SDF_VU0__APPLYMATRIX_XYZ() __asm__ volatile ( \
    ".set noreorder\n\tvmulax.xyzw ACC, vf28, vf10x\n\tvmadday.xyzw ACC, vf29, vf10y\n\t" \
    "vmaddz.xyzw vf10, vf30, vf10z\n\t.set reorder")

/* vf10 = vf10 x vf11 over xyz: the outer-product pair. */
#define SDF_VU0__OUTERPRODUCT() __asm__ volatile ( \
    ".set noreorder\n\tvopmula.xyz ACC, vf10, vf11\n\t" \
    "vopmsub.xyz vf10, vf11, vf10\n\t.set reorder")
/* out = vf10 . vf11 over xyz as a C float; $2 is scratch. */
#define SDF_VU0__INNERPRODUCT(out) __asm__ volatile ( \
    ".set noreorder\n\tvmul.xyz vf2, vf10, vf11\n\tvaddy.x vf2, vf2, vf2y\n\t" \
    "vaddz.x vf2, vf2, vf2z\n\tqmfc2.ni $2, vf2\n\tmtc1 $2, %0\n\t.set reorder" \
    : "=f" (out) : : "$2")
/* out = |vf10.xyz|, read back through the Q register and $2. */
#define SDF_VU0__GETLENGTH(out) __asm__ volatile ( \
    ".set noreorder\n\tvmul.xyz vf2, vf10, vf10\n\tvaddy.x vf2, vf2, vf2y\n\t" \
    "vaddz.x vf2, vf2, vf2z\n\tvsqrt Q, vf2x\n\tvwaitq\n\tcfc2.ni $2, $vi22\n\t" \
    "mtc1 $2, %0\n\t.set reorder" \
    : "=f" (out) : : "$2")
/* vf10 /= |vf10.xyz| (w untouched). */
#define SDF_VU0__NORMALIZE() __asm__ volatile ( \
    ".set noreorder\n\tvmul.xyz vf2, vf10, vf10\n\tvmulax.w ACC, vf0, vf2x\n\t" \
    "vmadday.w ACC, vf0, vf2y\n\tvmaddz.w vf2, vf0, vf2z\n\tvrsqrt Q, vf0w, vf2w\n\t" \
    "vwaitq\n\tvmulq.xyz vf10, vf10, Q\n\t.set reorder")
/* vf10 /= vf10.w, then w = 1: the perspective divide after a point transform. */
#define SDF_VU0__PERSVECTOR() __asm__ volatile ( \
    ".set noreorder\n\tvdiv Q, vf0w, vf10w\n\tvmove.w vf10, vf0\n\tvwaitq\n\t" \
    "vmulq.xyzw vf10, vf10, Q\n\t.set reorder")

/* Accumulate the xyz clip tests of a transformed vector against its own w,
 * then read or reset the accumulated flags in vi18. */
#define SDF_VU0__CLIPTEST() __asm__ volatile ( \
    ".set noreorder\n\tvclipw.xyz vf10, vf10w\n\t.set reorder")
#define SDF_VU0__GETCLIPFLAG(out) __asm__ volatile ( \
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
#define SDF_VU0__dds3ColorItoF(rgba) do { u32 ps2Lanes; __asm__ volatile ( \
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
#define SDF_VU0__dds3ColorFtoI(out, scale) __asm__ volatile ( \
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
#define SDF_VU0__LOADVECTOR_UNALIGN(p) __asm__ volatile ( \
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
#define SDF_VU0__ADDVECTOR_A_C() __asm__ volatile ( \
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

#define SDF_VU0__SUBVECTOR_B_C() __asm__ volatile ( \
    ".set noreorder\n\tvsub.xyzw vf11, vf11, vf12\n\t.set reorder")

/* vf10 = (1 - t) * vf10 + t * vf11, with float bits passed in a GPR. */
#define SDF_VU0__INTERPOLATE(t) __asm__ volatile ( \
    ".set noreorder\n\tqmtc2.ni %0, vf2\n\tvsubx.w vf3, vf0, vf2x\n\t" \
    "vmulax.xyzw ACC, vf11, vf2x\n\tvmaddw.xyzw vf10, vf10, vf3w\n\t.set reorder" \
    : : "r" (t))

#define SDF_VU0__LOADVECTOR_C(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf12, 0(%0)\n\t.set reorder" \
    : : "r" (src))

/* Ring geometry matrix and working-register operations. */

#define SDF_VU0__APPLYMATRIX_B() __asm__ volatile ( \
    ".set noreorder\n\tvmulax.xyzw ACC, vf28, vf11x\n\tvmadday.xyzw ACC, vf29, vf11y\n\t" \
    "vmaddaz.xyzw ACC, vf30, vf11z\n\tvmaddw.xyzw vf11, vf31, vf11w\n\t.set reorder")

#define SDF_VU0__LOADMATRIX_B(src) __asm__ volatile ( \
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

/* ---- operations taken on a register named at the call site ---- */

/* The vf register is a literal token in the macro body, so one definition
 * covers every register the units use. Units that need a different operand
 * constraint for the same operation keep their own definition and say why. */


/* vf10 = vf0, so xyz go to zero and w to one. */
#define SDF_VU0__LOADIDENTITYVECTOR() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf10, vf0\n\t.set reorder")


#define SDF_VU0__GETVECTOR_X(out) __asm__ volatile ( \
    ".set noreorder\n\tqmfc2.ni $2, vf10\n\tmtc1 $2, %0\n\t.set reorder" \
    : "=f" (out) : : "$2")
#define VU0_STORE_VF10_NOMEM(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" \
    : : "r" (dst))

#define VU0_STORE_MATRIX_UNCLOBBERED(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf28, 0(%0)\n\tsqc2 vf29, 16(%0)\n\tsqc2 vf30, 32(%0)\n\t" \
    "sqc2 vf31, 48(%0)\n\t.set reorder" \
    : : "r" (dst))
/* Memory to memory through the matrix bank, which it leaves loaded. */
#define VU0_COPY_MATRIX(dst, src) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "lqc2 vf28, 0(%1)\n\tlqc2 vf29, 16(%1)\n\t" \
    "lqc2 vf30, 32(%1)\n\tlqc2 vf31, 48(%1)\n\t" \
    "sqc2 vf28, 0(%0)\n\tsqc2 vf29, 16(%0)\n\t" \
    "sqc2 vf30, 32(%0)\n\tsqc2 vf31, 48(%0)\n\t" \
    ".set reorder" \
    : : "r" (dst), "r" (src) : "memory")

/* An identity matrix built in GPRs from vf0 and stored, with no VU0
 * arithmetic: pextuw walks the one in vf0.w down the four rows. */
#define SDF_VU0__STOREIDENTITY(dst) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "qmfc2.ni $5, $vf0\n\t" \
    "pextuw $4, $0, $5\n\tpextuw $2, $0, $4\n\tpextuw $3, $4, $0\n\t" \
    "sq $2, 0(%0)\n\tsq $3, 0x10(%0)\n\t" \
    "sq $4, 0x20(%0)\n\tsq $5, 0x30(%0)\n\t" \
    ".set reorder" \
    : : "r" (dst) : "$2", "$3", "$4", "$5", "memory")

/* The same unpack as MMI_RGBA_UNPACK with the operands spelled the way the
 * event and model units write them. */
#define EE_MMI_RGBA_UNPACK(src, unit) __asm__ volatile ( \
    ".set noreorder\n" \
    "lw $2, 0(%0)\n" \
    "pextlb $2, $0, $2\n" \
    "pextlh $2, $0, $2\n" \
    "qmtc2.ni $2, vf10\n" \
    "vitof0.xyzw vf10, vf10\n" \
    "qmtc2.ni %1, vf2\n" \
    "vmulx.xyzw vf10, vf10, vf2x\n" \
    ".set reorder" \
    : : "r"(src), "r"(unit) : "$2", "memory")


/* ---- lane reads and writes through $2, and the single-register stores ---- */

#define SDF_VU0__GETVECTOR_W(out) __asm__ volatile ( \
    ".set noreorder\n\tvaddw.x vf2, vf0, vf10w\n\tqmfc2.ni $2, vf2\n\tmtc1 $2, %0\n\t.set reorder" \
    : "=f" (out) : : "$2")
#define SDF_VU0__GETVECTOR_Y(out) __asm__ volatile ( \
    ".set noreorder\n\tqmfc2.ni $2, vf10\n\tprot3w $2, $2\n\tmtc1 $2, %0\n\t.set reorder" \
    : "=f" (out) : : "$2")
#define SDF_VU0__GETVECTOR_Z(out) __asm__ volatile ( \
    ".set noreorder\n\tqmfc2.ni $2, vf10\n\tpexew $2, $2\n\tmtc1 $2, %0\n\t.set reorder" \
    : "=f" (out) : : "$2")

/* vf10.w and vf10.z set from a C float, vf0 supplying the one. */

#define SDF_VU0__SETVECTOR_W(f) __asm__ volatile ( \
    ".set noreorder\n\tmfc1 $2, %0\n\tqmtc2.ni $2, vf2\n\t" \
    "vmulx.w vf10, vf0, vf2x\n\t.set reorder" \
    : : "f" (f) : "$2")
#define SDF_VU0__SETVECTOR_Z(value) __asm__ volatile ( \
    ".set noreorder\n\tmfc1 $2, %0\n\tqmtc2.ni $2, vf2\n\t" \
    "vaddx.z vf10, vf0, vf2x\n\t.set reorder" : : "f" (value) : "$2")

#define VU0_STORE_VF28(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf28, 0(%0)\n\t.set reorder" : : "r" (dst) : "memory")
#define VU0_STORE_VF29(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf29, 0(%0)\n\t.set reorder" : : "r" (dst) : "memory")
#define VU0_STORE_VF30(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf30, 0(%0)\n\t.set reorder" : : "r" (dst) : "memory")

#define SDF_VU0__SUBVECTOR_B_A() __asm__ volatile ( \
    ".set noreorder\n\tvsub.xyzw vf11, vf11, vf10\n\t.set reorder")

/* An identity matrix in the bank: vmr32 rotates the one in vf0.w into
 * place for each row. */
#define SDF_VU0__LOADIDENTITY() __asm__ volatile ( \
    ".set noreorder\n\tvsub.xyzw vf28, vf0, vf0\n\tvmr32.xyzw vf30, vf0\n\t" \
    "vmove.xyzw vf31, vf0\n\tvaddw.x vf28, vf28, vf0w\n\tvmr32.xyzw vf29, vf30\n\t.set reorder")


/* ---- fixed-register forms, one name per register pair ---- */

#define SDF_VU0__LOADVECTOR_MTXCOL(src) __asm__ volatile ( \
    ".set noreorder\n\tlw $2, 0(%0)\n\tlw $3, 0x10(%0)\n\t" \
    "lw $4, 0x20(%0)\n\tlw $5, 0x30(%0)\n\t" \
    "pextlw $2, $3, $2\n\tpextlw $4, $5, $4\n\t" \
    "pcpyld $2, $4, $2\n\tqmtc2.ni $2, vf10\n\t.set reorder" \
    : : "r" (src) : "$2", "$3", "$4", "$5", "memory")
#define VU0_LOAD_VF31(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf31, 0(%0)\n\t.set reorder" \
    : : "r" (src))
#define VU0_LOAD_VF28_BARRIER(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf28, 0(%0)\n\t.set reorder" \
    : : "r" (src) : "memory")
#define VU0_LOAD_VF29_BARRIER(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf29, 0(%0)\n\t.set reorder" \
    : : "r" (src) : "memory")
#define VU0_LOAD_VF30_BARRIER(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf30, 0(%0)\n\t.set reorder" \
    : : "r" (src) : "memory")
#define VU0_LOAD_VF31_BARRIER(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf31, 0(%0)\n\t.set reorder" \
    : : "r" (src) : "memory")
#define VU0_COPY_VF28_TO_VF24() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf24, vf28\n\t.set reorder")
#define VU0_COPY_VF29_TO_VF25() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf25, vf29\n\t.set reorder")
#define VU0_COPY_VF30_TO_VF26() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf26, vf30\n\t.set reorder")
#define VU0_COPY_VF31_TO_VF27() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf27, vf31\n\t.set reorder")
#define VU0_COPY_VF0_TO_VF31() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf31, vf0\n\t.set reorder")
#define VU0_SCALE_MATRIX_ROWS_VF10() __asm__ volatile ( \
    ".set noreorder\n\tvmulx.xyzw vf28, vf28, vf10x\n\tvmuly.xyzw vf29, vf29, vf10y\n\t" \
    "vmulz.xyzw vf30, vf30, vf10z\n\t.set reorder")
#define VU0_SET_VF10_Y_CLEAR_W(f) __asm__ volatile ( \
    ".set noreorder\n\tmfc1 $2, %0\n\tqmtc2.ni $2, vf2\n\tvaddx.y vf10, vf0, vf2x\n\t" \
    "vmulx.w vf10, vf10, vf0x\n\t.set reorder" \
    : : "f" (f))
#define VU0_SET_VF31_W_ONE() __asm__ volatile ( \
    ".set noreorder\n\tvmove.w vf31, vf0\n\t.set reorder")

#define VU0_COPY_VF10_TO_VF2() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw " "vf2" ", " "vf10" "\n\t.set reorder")
#define VU0_COPY_VF10_TO_VF28() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf28, vf10\n\t.set reorder")
#define VU0_COPY_VF10_TO_VF29() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf29, vf10\n\t.set reorder")
#define VU0_COPY_VF10_TO_VF30() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf30, vf10\n\t.set reorder")
#define VU0_COPY_VF2_TO_VF12() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw " "vf12" ", " "vf2" "\n\t.set reorder")
#define VU0_COPY_VF30_TO_VF10() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf10, vf30\n\t.set reorder")
#define VU0_LOAD_VF10_BARRIER(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" \
    : : "r" (src) : "memory")
#define SDF_VU0__SETVECTOR_X(value) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "mfc1 $2, %0\n\t" \
    "qmtc2.ni $2, $vf2\n\t" \
    "vaddx.x vf10, vf0, vf2x\n\t" \
    ".set reorder" \
    : : "f" (value) : "$2")
#define SDF_VU0__SETVECTOR_Y(value) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "mfc1 $2, %0\n\t" \
    "qmtc2.ni $2, $vf2\n\t" \
    "vaddx.y vf10, vf0, vf2x\n\t" \
    ".set reorder" \
    : : "f" (value) : "$2")
#define VU0_STORE_VF0(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf0, 0(%0)\n\t.set reorder" \
    : : "r" (dst) : "memory")
#define VU0_STORE_VF11_NOMEM(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf11, 0(%0)\n\t.set reorder" \
    : : "r" (dst))
#define VU0_UNIT_SCALE() __asm__ volatile ( \
    ".set noreorder\n\tvaddw.xyz vf10, vf0, vf0w\n\t" \
    "vmulx.w vf10, vf0, vf0x\n\t.set reorder" : : : "memory")

/* The extended form: an empty operand list, not a basic asm with no colons.
 * cc1 2.96 schedules the two differently, and mdlSetBlur only matches with
 * this spelling. */
#define VU0_COPY_VF10_TO_VF11_EXTENDED() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf11, vf10\n\t.set reorder" : :)
#define VU0_SUB_VF10_VF11_EXTENDED() __asm__ volatile ( \
    ".set noreorder\n\tvsub.xyzw vf10, vf10, vf11\n\t.set reorder" : :)

/* ---- operations that were written out at a call site ---- */

#define VU0_COPY_VF10_TO_VF31() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf31, vf10\n\t.set reorder")
#define VU0_ADD_VF31_VF10_XYZ() __asm__ volatile ( \
    ".set noreorder\n\tvadd.xyz vf31, vf31, vf10\n\t.set reorder")

/* The extended form, as the trail builder spells it. */
#define SDF_VU0__ADDVECTOR_B_A() __asm__ volatile ( \
    ".set noreorder\n\tvadd.xyzw vf11, vf11, vf10\n\t.set reorder" : :)

#define SDF_VU0__COPYVECTOR_B_TO_C() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf12, vf11\n\t.set reorder")

/* An sq offset is a literal, so the unrolled clear needs one name per step. */
#define PS2_STORE_ZERO_QUAD_AT16(dst) __asm__ volatile ( \
    "sq $0, 0x10(%0)" : : "r" (dst) : "memory")
#define PS2_STORE_ZERO_QUAD_AT32(dst) __asm__ volatile ( \
    "sq $0, 0x20(%0)" : : "r" (dst) : "memory")
#define PS2_STORE_ZERO_QUAD_AT48(dst) __asm__ volatile ( \
    "sq $0, 0x30(%0)" : : "r" (dst) : "memory")

/* COP1 absolute value; cc1 2.96 calls fabsf instead of emitting it. Not
 * volatile, like the fpu.h helpers: it has an output and no side effect. */
#define EE_ABS_F32(out, in) __asm__ ( \
    ".set noreorder\n\tabs.s %0, %1\n\t.set reorder" \
    : "=f" (out) : "f" (in))

/* Pack an RGBA8888 word down to RGBA5551. */
#define MMI_PEXT5(out, in) __asm__ volatile ("pext5 %0, %1" : "=r" (out) : "r" (in))

/* The barrier forms: same instruction, but cc1 may not keep memory values
 * cached across them. */
#define EE_SYNC_EI() __asm__ volatile("sync\n\tei")
#define EE_SYNC_BARRIER() __asm__ volatile ("sync" : : : "memory")
#define EE_SYNC_EI_BARRIER() __asm__ volatile ( \
    ".set noreorder\n\tsync\n\tei\n\t.set reorder" : : : "memory")

/* ---- quaternion operations shared by the blend routines ---- */

/* vf10 . vf11 over all four lanes, read back as a C float. vf1.xyz is set to
 * one first because vf0.x is a hardwired zero and cannot carry the y and z
 * terms. */
#define VU0_QUAT_DOT_VF10_VF11(out) __asm__ volatile ( \
    ".set noreorder\n" \
    "\tvaddw.xyz vf1, vf0, vf0w\n" \
    "\tvmul.xyzw vf2, vf10, vf11\n" \
    "\tvadday.x ACC, vf2, vf2y\n" \
    "\tvmaddaz.x ACC, vf1, vf2z\n" \
    "\tvmaddw.x vf2, vf1, vf2w\n" \
    "\tqmfc2.ni $2, vf2\n" \
    "\tmtc1 $2, %0\n" \
    "\t.set reorder" \
    : "=f" (out))

/* vf12 = -vf11, taking the long way: the accumulator is zeroed by vf0.x and
 * vf11 is then subtracted from it. */
#define VU0_NEGATE_VF11_TO_VF12() __asm__ volatile ( \
    ".set noreorder\n" \
    "\tvmulax.xyzw ACC, vf0, vf0x\n" \
    "\tvmsubw.xyzw vf12, vf11, vf0w\n" \
    "\t.set reorder")

/* The two blend weights into vf2.x and vf3.x. */
#define VU0_SET_WEIGHTS_VF2X_VF3X(first, second) __asm__ volatile ( \
    ".set noreorder\n" \
    "\tmfc1 $2, %0\n" \
    "\tmfc1 $3, %1\n" \
    "\tqmtc2.ni $2, vf2\n" \
    "\tqmtc2.ni $3, vf3\n" \
    "\t.set reorder" \
    : : "f" (first), "f" (second))

/* vf10 = vf10 * vf2.x + vf12 * vf3.x. */
#define VU0_WEIGHTED_SUM_VF10_VF12() __asm__ volatile ( \
    ".set noreorder\n" \
    "\tvmulax.xyzw ACC, vf10, vf2x\n" \
    "\tvmaddx.xyzw vf10, vf12, vf3x\n" \
    "\t.set reorder")

#endif /* PS2ASM_H */
