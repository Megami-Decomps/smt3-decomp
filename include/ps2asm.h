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

/* lq/sq through $2. Plain C `*(u128 *)dst = *(u128 *)src` gives the same pair
 * but lets the sq fall into a jr delay slot. */
#define SDF_VU0_COPYVECTOR(dst, src) __asm__ volatile ( \
    ".set noreorder\n\tlq $2, 0(%1)\n\tsq $2, 0(%0)\n\t.set reorder" \
    : : "r" (dst), "r" (src) : "$2", "memory")

/* Copy exactly four 16-byte-aligned f32 elements with explicit memory
 * operands, without a global memory clobber. Addresses must have no evaluation
 * side effects because each is used by both an address and memory operand;
 * source and destination may alias. */
typedef struct {
    f32 components[4];
} ps2asm_float_vector4_t;
#define SDF_VU0_COPYVECTOR_F32(dst, src) __asm__ volatile ( \
    ".set noreorder\n\tlq $2, 0(%2)\n\tsq $2, 0(%1)\n\t.set reorder" \
    : "=m" (*(ps2asm_float_vector4_t *)(dst)) \
    : "r" (dst), "r" (src), "m" (*(const ps2asm_float_vector4_t *)(src)) \
    : "$2")

/* sq $0: EE GCC materialises a C 128-bit zero in a register first. */
#define PS2_STORE_ZERO_QUAD(dst) __asm__ volatile ( \
    "sq $0, 0(%0)" : : "r" (dst) : "memory")

/* ---- quadword between memory and the VU0 register file ---- */

/* The _TO/_FROM forms take a C lvalue and let cc1 pick the addressing mode;
 * the plain forms take an address already in a register. */
#define SDF_VU0_LOADVECTOR(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r" (src))
#define SDF_VU0_LOADVECTOR_MEMOPS(lvalue) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf10, %0\n\t.set reorder" : : "m" (lvalue))
#define SDF_VU0_STOREVECTOR(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r" (dst) : "memory")
#define SDF_VU0_STOREVECTOR_MEMOPS(lvalue) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf10, %0\n\t.set reorder" : "=m" (lvalue))
/* vf0 is the constant (0,0,0,1), so this clears xyz and sets w to one. */
#define SDF_VU0_STOREVECTOR_ZERO(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf0, 0(%0)\n\t.set reorder" : : "r" (dst) : "memory")

#define SDF_VU0_LOADVECTOR_B(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf11, 0(%0)\n\t.set reorder" : : "r" (src))
#define SDF_VU0_LOADVECTOR_B_MEMOPS(lvalue) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf11, %0\n\t.set reorder" : : "m" (lvalue))
#define SDF_VU0_STOREVECTOR_B(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf11, 0(%0)\n\t.set reorder" : : "r" (dst) : "memory")
#define SDF_VU0_STOREVECTOR_B_MEMOPS(lvalue) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf11, %0\n\t.set reorder" : "=m" (lvalue))

/* Four quadwords between memory and the matrix bank vf28-vf31. */
#define SDF_VU0_LOADMATRIX(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf28, 0(%0)\n\tlqc2 vf29, 16(%0)\n\t" \
    "lqc2 vf30, 32(%0)\n\tlqc2 vf31, 48(%0)\n\t.set reorder" \
    : : "r" (src))
#define SDF_VU0_STOREMATRIX(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf28, 0(%0)\n\tsqc2 vf29, 16(%0)\n\t" \
    "sqc2 vf30, 32(%0)\n\tsqc2 vf31, 48(%0)\n\t.set reorder" \
    : : "r" (dst) : "memory")

/* Preserve the current transform in the secondary matrix bank. */
#define SDF_VU0_COPYMATRIX_A_TO_B() __asm__ volatile ( \
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
#define SDF_VU0_SCALEVECTOR(f) do { u32 ps2Bits; __asm__ volatile ( \
    ".set noreorder\n\tmfc1 %0, %1\n\tqmtc2.ni %0, vf2\n\t" \
    "vmulx.xyzw vf10, vf10, vf2x\n\t.set reorder" \
    : "=&r" (ps2Bits) : "f" (f)); } while (0)
#define SDF_VU0_SCALEVECTOR_B(f) do { u32 ps2Bits; __asm__ volatile ( \
    ".set noreorder\n\tmfc1 %0, %1\n\tqmtc2.ni %0, vf2\n\t" \
    "vmulx.xyzw vf11, vf11, vf2x\n\t.set reorder" \
    : "=&r" (ps2Bits) : "f" (f)); } while (0)
/* The same scale from a GPR that already holds the binary32 bits. */
#define SDF_VU0_SCALEVECTOR_BITS(r) __asm__ volatile ( \
    ".set noreorder\n\tqmtc2.ni %0, vf2\n\tvmulx.xyzw vf10, vf10, vf2x\n\t.set reorder" \
    : : "r" (r))
#define SDF_VU0_SCALEVECTOR_B_BITS(r) __asm__ volatile ( \
    ".set noreorder\n\tqmtc2.ni %0, vf2\n\tvmulx.xyzw vf11, vf11, vf2x\n\t.set reorder" \
    : : "r" (r))
#define VU0_SET_VF2X(r) __asm__ volatile ( \
    ".set noreorder\n\tqmtc2.ni %0, vf2\n\t.set reorder" : : "r" (r))

/* ---- VU0 macro-mode arithmetic on the fixed working pair ---- */

#define SDF_VU0_COPYVECTOR_A_TO_B() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf11, vf10\n\t.set reorder")
#define SDF_VU0_COPYVECTOR_B_TO_A() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf10, vf11\n\t.set reorder")
/* vf12 is the spare the effect code parks a vector in across a call. */
#define SDF_VU0_COPYVECTOR_A_TO_C() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf12, vf10\n\t.set reorder")
#define SDF_VU0_COPYVECTOR_C_TO_A() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf10, vf12\n\t.set reorder")
#define SDF_VU0_COPYVECTOR_C_TO_B() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf11, vf12\n\t.set reorder")

#define SDF_VU0_ADDVECTOR() __asm__ volatile ( \
    ".set noreorder\n\tvadd.xyzw vf10, vf10, vf11\n\t.set reorder")
#define SDF_VU0_SUBVECTOR() __asm__ volatile ( \
    ".set noreorder\n\tvsub.xyzw vf10, vf10, vf11\n\t.set reorder")
#define SDF_VU0_MULVECTOR() __asm__ volatile ( \
    ".set noreorder\n\tvmul.xyzw vf10, vf10, vf11\n\t.set reorder")
#define VU0_ADD_VF10_VF11_XYZ() __asm__ volatile ( \
    ".set noreorder\n\tvadd.xyz vf10, vf10, vf11\n\t.set reorder")
#define VU0_SUB_VF10_VF11_XYZ() __asm__ volatile ( \
    ".set noreorder\n\tvsub.xyz vf10, vf10, vf11\n\t.set reorder")
#define VU0_MUL_VF10_VF11_XYZ() __asm__ volatile ( \
    ".set noreorder\n\tvmul.xyz vf10, vf10, vf11\n\t.set reorder")

/* vf10.xyz = -vf10.xyz (vf0.xyz is zero), vf10.w = 0, vf10.w = 1. */
#define SDF_VU0_NEGVECTOR_XYZ() __asm__ volatile ( \
    ".set noreorder\n\tvsub.xyz vf10, vf0, vf10\n\t.set reorder")
#define SDF_VU0_VECTOR_W_SET_ZERO() __asm__ volatile ( \
    ".set noreorder\n\tvmulx.w vf10, vf10, vf0x\n\t.set reorder")
#define SDF_VU0_VECTOR_W_SET_ONE() __asm__ volatile ( \
    ".set noreorder\n\tvmove.w vf10, vf0\n\t.set reorder")

/* Float <-> integer lane conversion with 0 and 4 fractional bits. */
#define SDF_VU0_ITOF0() __asm__ volatile ( \
    ".set noreorder\n\tvitof0.xyzw vf10, vf10\n\t.set reorder")
#define SDF_VU0_FTOI0() __asm__ volatile ( \
    ".set noreorder\n\tvftoi0.xyzw vf10, vf10\n\t.set reorder")
#define SDF_VU0_FTOI4() __asm__ volatile ( \
    ".set noreorder\n\tvftoi4.xyzw vf10, vf10\n\t.set reorder")

/* vf10 = matrix (vf28-vf31) * vf10: the core of libvu0 sceVu0ApplyMatrix.
 * _POINT takes the source w as 1; _ROTATE uses the 3x3 part only. */
#define SDF_VU0_APPLYMATRIX() __asm__ volatile ( \
    ".set noreorder\n\tvmulax.xyzw ACC, vf28, vf10x\n\tvmadday.xyzw ACC, vf29, vf10y\n\t" \
    "vmaddaz.xyzw ACC, vf30, vf10z\n\tvmaddw.xyzw vf10, vf31, vf10w\n\t.set reorder")
#define SDF_VU0_APPLYMATRIX_XYZ1() __asm__ volatile ( \
    ".set noreorder\n\tvmulax.xyzw ACC, vf28, vf10x\n\tvmadday.xyzw ACC, vf29, vf10y\n\t" \
    "vmaddaz.xyzw ACC, vf30, vf10z\n\tvmaddw.xyzw vf10, vf31, vf0w\n\t.set reorder")
#define SDF_VU0_APPLYMATRIX_XYZ() __asm__ volatile ( \
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
#define SDF_VU0_INTERPOLATE(r) __asm__ volatile ( \
    ".set noreorder\n\tqmtc2.ni %0, vf2\n\t" \
    "vsubx.w vf3, vf0, vf2x\n\tvmulax.xyzw ACC, vf11, vf2x\n\t" \
    "vmaddw.xyzw vf10, vf10, vf3w\n\t.set reorder" \
    : : "r" (r))
/* binary32 bits of a float in a GPR (mfc1), for the _BITS forms. */
#define PS2_FLOAT_BITS(out, f) __asm__ volatile ( \
    ".set noreorder\n\tmfc1 %0, %1\n\t.set reorder" : "=r" (out) : "f" (f))

/* vf10 = vf10 x vf11 over xyz: the outer-product pair. */
#define SDF_VU0_OUTERPRODUCT() __asm__ volatile ( \
    ".set noreorder\n\tvopmula.xyz ACC, vf10, vf11\n\t" \
    "vopmsub.xyz vf10, vf11, vf10\n\t.set reorder")
/* out = vf10 . vf11 over xyz as a C float; $2 is scratch. */
#define SDF_VU0_INNERPRODUCT(out) __asm__ volatile ( \
    ".set noreorder\n\tvmul.xyz vf2, vf10, vf11\n\tvaddy.x vf2, vf2, vf2y\n\t" \
    "vaddz.x vf2, vf2, vf2z\n\tqmfc2.ni $2, vf2\n\tmtc1 $2, %0\n\t.set reorder" \
    : "=f" (out) : : "$2")
/* out = |vf10.xyz|, read back through the Q register and $2. */
#define SDF_VU0_GETLENGTH(out) __asm__ volatile ( \
    ".set noreorder\n\tvmul.xyz vf2, vf10, vf10\n\tvaddy.x vf2, vf2, vf2y\n\t" \
    "vaddz.x vf2, vf2, vf2z\n\tvsqrt Q, vf2x\n\tvwaitq\n\tcfc2.ni $2, $vi22\n\t" \
    "mtc1 $2, %0\n\t.set reorder" \
    : "=f" (out) : : "$2")
/* vf10 /= |vf10.xyz| (w untouched). */
#define SDF_VU0_NORMALIZE() __asm__ volatile ( \
    ".set noreorder\n\tvmul.xyz vf2, vf10, vf10\n\tvmulax.w ACC, vf0, vf2x\n\t" \
    "vmadday.w ACC, vf0, vf2y\n\tvmaddz.w vf2, vf0, vf2z\n\tvrsqrt Q, vf0w, vf2w\n\t" \
    "vwaitq\n\tvmulq.xyz vf10, vf10, Q\n\t.set reorder")
/* vf10 /= vf10.w, then w = 1: the perspective divide after a point transform. */
#define SDF_VU0_PERSVECTOR() __asm__ volatile ( \
    ".set noreorder\n\tvdiv Q, vf0w, vf10w\n\tvmove.w vf10, vf0\n\tvwaitq\n\t" \
    "vmulq.xyzw vf10, vf10, Q\n\t.set reorder")

/* Accumulate the xyz clip tests of a transformed vector against its own w,
 * then read or reset the accumulated flags in vi18. */
#define SDF_VU0_CLIPTEST() __asm__ volatile ( \
    ".set noreorder\n\tvclipw.xyz vf10, vf10w\n\t.set reorder")
#define SDF_VU0_GETCLIPFLAG(out) __asm__ volatile ( \
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

/* Bit length of a non-negative word: the index of its highest set bit plus one.
 * 31 - plzcw is the whole operation, so the 31 arrives through the one addi
 * cc1 never selects. Passed dim - 1 it gives log2 of a power-of-two dimension. */
#define PS2_WORD_BIT_LENGTH(x) ({ s32 ps2Bits = (x); s32 ps2Lzc; __asm__ ( \
    ".set noreorder\n\tplzcw %1, %0\n\tandi %1, %1, 0xFF\n\t" \
    "addi %0, $0, 0x1F\n\tsubu %0, %0, %1\n\t.set reorder" \
    : "+r" (ps2Bits), "=&r" (ps2Lzc)); ps2Bits; })

/* RGBA8888 word -> four float lanes in vf10: bytes to halfwords to words,
 * then vitof0. */
#define SDF_VU0_dds3ColorItoF(rgba) do { u32 ps2Lanes; __asm__ volatile ( \
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
#define SDF_VU0_dds3ColorFtoI(out, scale) __asm__ volatile ( \
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
#define SDF_VU0_LOADVECTOR_UNALIGN(p) __asm__ volatile ( \
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

/* The engine's own names. The HD sources keep both as real functions in
 * sdfMath.c, sdfSqrt as `return sqrtf(aValue);` and sdfFabs as
 * `return fabsf(c);`, so a call site here reads the way the original did.
 * Value forms, and not volatile: they have an output and no side effect. */
#define sdfSqrt(in) ({ \
    f32 _sqrtOut; \
    __asm__ (".set noreorder\n\tsqrt.s %0, %1\n\t.set reorder" \
        : "=f" (_sqrtOut) : "f" (in)); \
    _sqrtOut; })
#define sdfFabs(in) ({ \
    f32 _fabsOut; \
    __asm__ (".set noreorder\n\tabs.s %0, %1\n\t.set reorder" \
        : "=f" (_fabsOut) : "f" (in)); \
    _fabsOut; })

/* ---- barriers ---- */

#define EE_SYNC() __asm__ volatile ("sync")
#define EE_SYNC_P() __asm__ volatile ("sync.p")

/* Additional working-register operations used by the trail interpolator. */
#define SDF_VU0_ADDVECTOR_A_C() __asm__ volatile ( \
    ".set noreorder\n\tvadd.xyzw vf10, vf10, vf12\n\t.set reorder")

#define SDF_VU0_ADDVECTOR_C_A() __asm__ volatile ( \
    ".set noreorder\n\tvadd.xyzw vf12, vf12, vf10\n\t.set reorder")

/* SDK input-address-only store: audited VU pipelines only. Unlike the
 * normal store it does not invalidate values cached from unrelated memory. */
#define SDF_VU0_STOREVECTOR_NOMEM(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" \
    : : "r" (dst))

/* The load, the store and the quadword copy with the address as a read-write
 * operand, so cc1 gives each asm its own copy of it instead of sharing one
 * pseudo. Needed where several VU0 accesses to one frame vector sit inside a
 * loop: the shared pseudo becomes a loop invariant in a callee-saved register,
 * where retail rematerialises `addiu rd, $29, N` at every access. */
#define SDF_VU0_LOADVECTOR_RWADDR(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" \
    : "+r" (src))
#define SDF_VU0_STOREVECTOR_RWADDR(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" \
    : "+r" (dst))
#define SDF_VU0_COPYVECTOR_RWSRC(dst, src) __asm__ volatile ( \
    ".set noreorder\n\tlq $2, 0(%0)\n\tsq $2, 0(%1)\n\t.set reorder" \
    : "+r" (src) : "r" (dst) : "$2", "memory")

/* Cell gradient vector operations. */
#define SDF_VU0_LOADVECTOR_B_BARRIER(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf11, 0(%0)\n\t.set reorder" \
    : : "r" (src) : "memory")

#define SDF_VU0_SUBVECTOR_B_C() __asm__ volatile ( \
    ".set noreorder\n\tvsub.xyzw vf11, vf11, vf12\n\t.set reorder")

#define SDF_VU0_ADDVECTOR_B_C() __asm__ volatile ( \
    ".set noreorder\n\tvadd.xyzw vf11, vf11, vf12\n\t.set reorder")


#define SDF_VU0_LOADVECTOR_C(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf12, 0(%0)\n\t.set reorder" \
    : : "r" (src))

/* Ring geometry matrix and working-register operations. */

#define SDF_VU0_APPLYMATRIX_B() __asm__ volatile ( \
    ".set noreorder\n\tvmulax.xyzw ACC, vf28, vf11x\n\tvmadday.xyzw ACC, vf29, vf11y\n\t" \
    "vmaddaz.xyzw ACC, vf30, vf11z\n\tvmaddw.xyzw vf11, vf31, vf11w\n\t.set reorder")

#define SDF_VU0_LOADMATRIX_B(src) __asm__ volatile ( \
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
#define SDF_VU0_LOADIDENTITYVECTOR() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf10, vf0\n\t.set reorder")


#define SDF_VU0_GETVECTOR_X(out) __asm__ volatile ( \
    ".set noreorder\n\tqmfc2.ni $2, vf10\n\tmtc1 $2, %0\n\t.set reorder" \
    : "=f" (out) : : "$2")

#define SDF_VU0_STOREMATRIX_NOMEM(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf28, 0(%0)\n\tsqc2 vf29, 16(%0)\n\tsqc2 vf30, 32(%0)\n\t" \
    "sqc2 vf31, 48(%0)\n\t.set reorder" \
    : : "r" (dst))
/* Same stores with the address as a read-write operand, so cc1 gives the asm
 * its own copy of it instead of sharing one pseudo with a later use. Needed
 * for a frame address whose other use is a call argument in the same loop:
 * the shared form becomes a loop invariant and takes a callee-saved register,
 * where this one is rematerialised at each use. */
#define SDF_VU0_STOREMATRIX_RWADDR(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf28, 0(%0)\n\tsqc2 vf29, 16(%0)\n\tsqc2 vf30, 32(%0)\n\t" \
    "sqc2 vf31, 48(%0)\n\t.set reorder" \
    : "+r" (dst))
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
#define SDF_VU0_STOREIDENTITY(dst) __asm__ volatile ( \
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

#define SDF_VU0_GETVECTOR_W(out) __asm__ volatile ( \
    ".set noreorder\n\tvaddw.x vf2, vf0, vf10w\n\tqmfc2.ni $2, vf2\n\tmtc1 $2, %0\n\t.set reorder" \
    : "=f" (out) : : "$2")
#define SDF_VU0_GETVECTOR_Y(out) __asm__ volatile ( \
    ".set noreorder\n\tqmfc2.ni $2, vf10\n\tprot3w $2, $2\n\tmtc1 $2, %0\n\t.set reorder" \
    : "=f" (out) : : "$2")
#define SDF_VU0_GETVECTOR_Z(out) __asm__ volatile ( \
    ".set noreorder\n\tqmfc2.ni $2, vf10\n\tpexew $2, $2\n\tmtc1 $2, %0\n\t.set reorder" \
    : "=f" (out) : : "$2")

/* vf10.w and vf10.z set from a C float, vf0 supplying the one. */

#define SDF_VU0_SETVECTOR_W(f) __asm__ volatile ( \
    ".set noreorder\n\tmfc1 $2, %0\n\tqmtc2.ni $2, vf2\n\t" \
    "vmulx.w vf10, vf0, vf2x\n\t.set reorder" \
    : : "f" (f) : "$2")
#define SDF_VU0_SETVECTOR_Z(value) __asm__ volatile ( \
    ".set noreorder\n\tmfc1 $2, %0\n\tqmtc2.ni $2, vf2\n\t" \
    "vaddx.z vf10, vf0, vf2x\n\t.set reorder" : : "f" (value) : "$2")

#define SDF_VU0_STORE_ROW0(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf28, 0(%0)\n\t.set reorder" : : "r" (dst) : "memory")
#define SDF_VU0_STORE_ROW1(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf29, 0(%0)\n\t.set reorder" : : "r" (dst) : "memory")
#define SDF_VU0_STORE_ROW2(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf30, 0(%0)\n\t.set reorder" : : "r" (dst) : "memory")

#define SDF_VU0_SUBVECTOR_B_A() __asm__ volatile ( \
    ".set noreorder\n\tvsub.xyzw vf11, vf11, vf10\n\t.set reorder")

/* An identity matrix in the bank: vmr32 rotates the one in vf0.w into
 * place for each row. */
#define SDF_VU0_LOADIDENTITY() __asm__ volatile ( \
    ".set noreorder\n\tvsub.xyzw vf28, vf0, vf0\n\tvmr32.xyzw vf30, vf0\n\t" \
    "vmove.xyzw vf31, vf0\n\tvaddw.x vf28, vf28, vf0w\n\tvmr32.xyzw vf29, vf30\n\t.set reorder")


/* ---- fixed-register forms, one name per register pair ---- */

#define SDF_VU0_LOADVECTOR_MTXCOL(src) __asm__ volatile ( \
    ".set noreorder\n\tlw $2, 0(%0)\n\tlw $3, 0x10(%0)\n\t" \
    "lw $4, 0x20(%0)\n\tlw $5, 0x30(%0)\n\t" \
    "pextlw $2, $3, $2\n\tpextlw $4, $5, $4\n\t" \
    "pcpyld $2, $4, $2\n\tqmtc2.ni $2, vf10\n\t.set reorder" \
    : : "r" (src) : "$2", "$3", "$4", "$5", "memory")
#define SDF_VU0_LOAD_ROW3(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf31, 0(%0)\n\t.set reorder" \
    : : "r" (src))
#define SDF_VU0_LOAD_ROW0_BARRIER(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf28, 0(%0)\n\t.set reorder" \
    : : "r" (src) : "memory")
#define SDF_VU0_LOAD_ROW1_BARRIER(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf29, 0(%0)\n\t.set reorder" \
    : : "r" (src) : "memory")
#define SDF_VU0_LOAD_ROW2_BARRIER(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf30, 0(%0)\n\t.set reorder" \
    : : "r" (src) : "memory")
#define SDF_VU0_LOAD_ROW3_BARRIER(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf31, 0(%0)\n\t.set reorder" \
    : : "r" (src) : "memory")
#define SDF_VU0_RESET_ROW3() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf31, vf0\n\t.set reorder")
#define SDF_VU0_SCALE_XYZ() __asm__ volatile ( \
    ".set noreorder\n\tvmulx.xyzw vf28, vf28, vf10x\n\tvmuly.xyzw vf29, vf29, vf10y\n\t" \
    "vmulz.xyzw vf30, vf30, vf10z\n\t.set reorder")
#define VU0_SET_VF10_Y_CLEAR_W(f) __asm__ volatile ( \
    ".set noreorder\n\tmfc1 $2, %0\n\tqmtc2.ni $2, vf2\n\tvaddx.y vf10, vf0, vf2x\n\t" \
    "vmulx.w vf10, vf10, vf0x\n\t.set reorder" \
    : : "f" (f))
#define SDF_VU0_MATRIX_ROW3_W_SET_ONE() __asm__ volatile ( \
    ".set noreorder\n\tvmove.w vf31, vf0\n\t.set reorder")

#define SDF_VU0_SET_ROW0() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf28, vf10\n\t.set reorder")
#define SDF_VU0_SET_ROW1() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf29, vf10\n\t.set reorder")
#define SDF_VU0_SET_ROW2() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf30, vf10\n\t.set reorder")
#define SDF_VU0_GET_ROW2() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf10, vf30\n\t.set reorder")
#define SDF_VU0_LOADVECTOR_BARRIER(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" \
    : : "r" (src) : "memory")
#define SDF_VU0_SETVECTOR_X(value) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "mfc1 $2, %0\n\t" \
    "qmtc2.ni $2, $vf2\n\t" \
    "vaddx.x vf10, vf0, vf2x\n\t" \
    ".set reorder" \
    : : "f" (value) : "$2")
#define SDF_VU0_SETVECTOR_Y(value) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "mfc1 $2, %0\n\t" \
    "qmtc2.ni $2, $vf2\n\t" \
    "vaddx.y vf10, vf0, vf2x\n\t" \
    ".set reorder" \
    : : "f" (value) : "$2")
#define SDF_VU0_STOREIDENTITYVECTOR(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf0, 0(%0)\n\t.set reorder" \
    : : "r" (dst) : "memory")
#define SDF_VU0_STOREVECTOR_B_NOMEM(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf11, 0(%0)\n\t.set reorder" \
    : : "r" (dst))
#define SDF_VU0_SETSCALE1VECTOR() __asm__ volatile ( \
    ".set noreorder\n\tvaddw.xyz vf10, vf0, vf0w\n\t" \
    "vmulx.w vf10, vf0, vf0x\n\t.set reorder" : : : "memory")

/* The extended form: an empty operand list, not a basic asm with no colons.
 * cc1 2.96 schedules the two differently, and mdlSetBlur only matches with
 * this spelling. */
#define SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf11, vf10\n\t.set reorder" : :)
#define SDF_VU0_SUBVECTOR_EXTENDED() __asm__ volatile ( \
    ".set noreorder\n\tvsub.xyzw vf10, vf10, vf11\n\t.set reorder" : :)

/* ---- operations that were written out at a call site ---- */

#define SDF_VU0_SET_ROW3() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf31, vf10\n\t.set reorder")
#define VU0_ADD_VF31_VF10_XYZ() __asm__ volatile ( \
    ".set noreorder\n\tvadd.xyz vf31, vf31, vf10\n\t.set reorder")

/* The extended form, as the trail builder spells it. */
#define SDF_VU0_ADDVECTOR_B_A() __asm__ volatile ( \
    ".set noreorder\n\tvadd.xyzw vf11, vf11, vf10\n\t.set reorder" : :)

#define SDF_VU0_COPYVECTOR_B_TO_C() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf12, vf11\n\t.set reorder")

/* An sq offset is a literal, so the unrolled clear needs one name per step. */
#define PS2_STORE_ZERO_QUAD_AT16(dst) __asm__ volatile ( \
    "sq $0, 0x10(%0)" : : "r" (dst) : "memory")
#define PS2_STORE_ZERO_QUAD_AT32(dst) __asm__ volatile ( \
    "sq $0, 0x20(%0)" : : "r" (dst) : "memory")
#define PS2_STORE_ZERO_QUAD_AT48(dst) __asm__ volatile ( \
    "sq $0, 0x30(%0)" : : "r" (dst) : "memory")

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


/* ---- operations recovered in one unit each ---- */

/* from basic/dds3EffectObjectBasic.c */
#define SDF_VU0_TRANSLATE() __asm__ volatile ( \
    ".set noreorder\n\tvmulax.xyzw ACC, vf28, vf10x\n\t" \
    "vmadday.xyzw ACC, vf29, vf10y\n\tvmaddaz.xyzw ACC, vf30, vf10z\n\t" \
    "vmaddw.xyzw vf31, vf31, vf10w\n\t.set reorder")

/* from basic/dds3PathBasic.c */
#define SDF_VU0_SCALEVECTOR_V0_BARRIER(value) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "mfc1 $2, %0\n\t" \
    "qmtc2.ni $2, $vf2\n\t" \
    "vmulx.xyzw vf10, vf10, vf2x\n\t" \
    ".set reorder" \
    : : "f" (value) : "$2", "memory")
#define SDF_VU0_SCALEVECTOR_V0(value) __asm__ volatile ( \
    ".set noreorder\n\tmfc1 $2, %0\n\tqmtc2.ni $2, vf2\n\t" \
    "vmulx.xyzw vf10, vf10, vf2x\n\t.set reorder" : : "f" (value))

/* from code_001AC0A8.c */
#define SDF_VU0_LOADMATRIX_B_MEMOPS(src) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "lqc2 vf24, 0(%0)\n\t" \
    "lqc2 vf25, 16(%0)\n\t" \
    "lqc2 vf26, 32(%0)\n\t" \
    "lqc2 vf27, 48(%0)\n\t" \
    ".set reorder" \
    : : "r" (src), "m" (*(const effDebugMatrix_t *)(src)))

/* from code_001DEA58.c */
#define SDF_VU0_SUBVECTOR_A_C() __asm__ volatile ( \
    ".set noreorder\n\tvsub.xyzw vf10, vf10, vf12\n\t.set reorder")

/* from newbattle/nbStageProcess.c */
#define SDF_VU0_COPYMATRIX_B_TO_A() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf28, vf24\n\tvmove.xyzw vf29, vf25\n\t" \
    "vmove.xyzw vf30, vf26\n\tvmove.xyzw vf31, vf27\n\t.set reorder")

/* from code_002B5CD0.c */
#define SDF_VU0_STOREMATRIX_B(dst) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "sqc2 vf24, 0(%0)\n\t" \
    "sqc2 vf25, 16(%0)\n\t" \
    "sqc2 vf26, 32(%0)\n\t" \
    "sqc2 vf27, 48(%0)\n\t" \
    ".set reorder" \
    : : "r" (dst) : "memory")
#define SDF_VU0_COPYMATRIX_A_TO_C() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf20, vf28\n\tvmove.xyzw vf21, vf29\n\t" \
    "vmove.xyzw vf22, vf30\n\tvmove.xyzw vf23, vf31\n\t.set reorder")
#define SDF_VU0_COPYMATRIX_B_TO_C() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf20, vf24\n\tvmove.xyzw vf21, vf25\n\t" \
    "vmove.xyzw vf22, vf26\n\tvmove.xyzw vf23, vf27\n\t.set reorder")
#define SDF_VU0_COPYMATRIX_C_TO_A() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf28, vf20\n\tvmove.xyzw vf29, vf21\n\t" \
    "vmove.xyzw vf30, vf22\n\tvmove.xyzw vf31, vf23\n\t.set reorder")
#define SDF_VU0_COPYMATRIX_C_TO_B() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf24, vf20\n\tvmove.xyzw vf25, vf21\n\t" \
    "vmove.xyzw vf26, vf22\n\tvmove.xyzw vf27, vf23\n\t.set reorder")
#define SDF_VU0_LOADIDENTITY_B() __asm__ volatile ( \
    ".set noreorder\n\tvsub.xyzw vf24, vf0, vf0\n\tvmr32.xyzw vf26, vf0\n\t" \
    "vmove.xyzw vf27, vf0\n\tvaddw.x vf24, vf24, vf0w\n\tvmr32.xyzw vf25, vf26\n\t.set reorder")
/* The three rotated basis rows saved across a nested packet build. */
#define VU0_STORE_VF24_VF25_VF26(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf24, 0(%0)\n\tsqc2 vf25, 16(%0)\n\t" \
    "sqc2 vf26, 32(%0)\n\t.set reorder" \
    : : "r" (dst))
#define VU0_LOAD_VF24_VF25_VF26(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf24, 0(%0)\n\tlqc2 vf25, 16(%0)\n\t" \
    "lqc2 vf26, 32(%0)\n\t.set reorder" \
    : : "r" (src))
#define VU0_MATRIX4_MUL_BANK_B_LEFT() __asm__ volatile ( \
    ".set noreorder\n\t" \
    "vmulax.xyzw ACC, vf24, vf28x\n\tvmadday.xyzw ACC, vf25, vf28y\n\t" \
    "vmaddaz.xyzw ACC, vf26, vf28z\n\tvmaddw.xyzw vf28, vf27, vf28w\n\t" \
    "vmulax.xyzw ACC, vf24, vf29x\n\tvmadday.xyzw ACC, vf25, vf29y\n\t" \
    "vmaddaz.xyzw ACC, vf26, vf29z\n\tvmaddw.xyzw vf29, vf27, vf29w\n\t" \
    "vmulax.xyzw ACC, vf24, vf30x\n\tvmadday.xyzw ACC, vf25, vf30y\n\t" \
    "vmaddaz.xyzw ACC, vf26, vf30z\n\tvmaddw.xyzw vf30, vf27, vf30w\n\t" \
    "vmulax.xyzw ACC, vf24, vf31x\n\tvmadday.xyzw ACC, vf25, vf31y\n\t" \
    "vmaddaz.xyzw ACC, vf26, vf31z\n\tvmaddw.xyzw vf31, vf27, vf31w\n\t" \
    ".set reorder" \
    : : : "memory")
#define VU0_MATRIX4_MUL_BANK_C_RIGHT() __asm__ volatile ( \
    ".set noreorder\n\t" \
    "vmulax.xyzw ACC, vf28, vf20x\n\tvmadday.xyzw ACC, vf29, vf20y\n\t" \
    "vmaddaz.xyzw ACC, vf30, vf20z\n\tvmaddw.xyzw vf2, vf31, vf20w\n\t" \
    "vmulax.xyzw ACC, vf28, vf21x\n\tvmadday.xyzw ACC, vf29, vf21y\n\t" \
    "vmaddaz.xyzw ACC, vf30, vf21z\n\tvmaddw.xyzw vf3, vf31, vf21w\n\t" \
    "vmulax.xyzw ACC, vf28, vf22x\n\tvmadday.xyzw ACC, vf29, vf22y\n\t" \
    "vmaddaz.xyzw ACC, vf30, vf22z\n\tvmaddw.xyzw vf4, vf31, vf22w\n\t" \
    "vmulax.xyzw ACC, vf28, vf23x\n\tvmadday.xyzw ACC, vf29, vf23y\n\t" \
    "vmaddaz.xyzw ACC, vf30, vf23z\n\tvmaddw.xyzw vf31, vf31, vf23w\n\t" \
    "vmove.xyzw vf28, vf2\n\tvmove.xyzw vf29, vf3\n\tvmove.xyzw vf30, vf4\n\t" \
    ".set reorder")
#define VU0_MATRIX4_MUL_BANK_C_LEFT() __asm__ volatile ( \
    ".set noreorder\n\t" \
    "vmulax.xyzw ACC, vf20, vf28x\n\tvmadday.xyzw ACC, vf21, vf28y\n\t" \
    "vmaddaz.xyzw ACC, vf22, vf28z\n\tvmaddw.xyzw vf28, vf23, vf28w\n\t" \
    "vmulax.xyzw ACC, vf20, vf29x\n\tvmadday.xyzw ACC, vf21, vf29y\n\t" \
    "vmaddaz.xyzw ACC, vf22, vf29z\n\tvmaddw.xyzw vf29, vf23, vf29w\n\t" \
    "vmulax.xyzw ACC, vf20, vf30x\n\tvmadday.xyzw ACC, vf21, vf30y\n\t" \
    "vmaddaz.xyzw ACC, vf22, vf30z\n\tvmaddw.xyzw vf30, vf23, vf30w\n\t" \
    "vmulax.xyzw ACC, vf20, vf31x\n\tvmadday.xyzw ACC, vf21, vf31y\n\t" \
    "vmaddaz.xyzw ACC, vf22, vf31z\n\tvmaddw.xyzw vf31, vf23, vf31w\n\t" \
    ".set reorder")
#define SDF_VU0_TRANSMATRIX() __asm__ volatile ( \
    ".set noreorder\n\t" \
    "qmfc2.ni $8, vf28\n\tqmfc2.ni $9, vf29\n\tqmfc2.ni $10, vf30\n\tqmfc2.ni $11, vf31\n\t" \
    "pextlw $12, $9, $8\n\tpextuw $13, $9, $8\n\tpextlw $14, $11, $10\n\tpextuw $15, $11, $10\n\t" \
    "pcpyld $8, $14, $12\n\tpcpyud $9, $12, $14\n\tpcpyld $10, $15, $13\n\tpcpyud $11, $13, $15\n\t" \
    "qmtc2.ni $8, vf28\n\tqmtc2.ni $9, vf29\n\tqmtc2.ni $10, vf30\n\tqmtc2.ni $11, vf31\n\t" \
    ".set reorder\n")
#define VU0_MATRIX4_INVERT_RIGID() __asm__ volatile ( \
    ".set noreorder\n\t" \
    "qmfc2.ni $8, vf28\n\tqmfc2.ni $9, vf29\n\tqmfc2.ni $10, vf30\n\tqmfc2.ni $11, vf0\n\t" \
    "pextlw $12, $9, $8\n\tpextuw $13, $9, $8\n\tpextlw $14, $11, $10\n\tpextuw $15, $11, $10\n\t" \
    "pcpyld $8, $14, $12\n\tpcpyud $9, $12, $14\n\tpcpyld $10, $15, $13\n\t" \
    "vmove.w vf31, vf0\n\t" \
    "qmtc2.ni $8, vf28\n\tqmtc2.ni $9, vf29\n\tqmtc2.ni $10, vf30\n\t" \
    "vsuba.xyz ACC, vf0, vf0\n\tvmsubax.xyz ACC, vf28, vf31x\n\t" \
    "vmsubay.xyz ACC, vf29, vf31y\n\tvmsubz.xyz vf31, vf30, vf31z\n\t" \
    ".set reorder\n")
#define SDF_VU0_GET_ROW3() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf10, vf31\n\t.set reorder")
#define VU0_ROTATE_BASIS_AND_CACHE(vectors, matrix, cache) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "lqc2 vf2, 0x40(%0)\n\tlqc2 vf5, 0x10(%1)\n\t" \
    "lqc2 vf6, 0x20(%1)\n\tlqc2 vf7, 0x30(%1)\n\t" \
    "lqc2 vf3, 0x50(%0)\n\tlqc2 vf4, 0x60(%0)\n\t" \
    "vmulax.xyz ACC, vf5, vf2x\n\tvmadday.xyz ACC, vf6, vf2y\n\tvmaddz.xyz vf24, vf7, vf2z\n\t" \
    "vmulax.xyz ACC, vf5, vf3x\n\tvmadday.xyz ACC, vf6, vf3y\n\tvmaddz.xyz vf25, vf7, vf3z\n\t" \
    "vmulax.xyz ACC, vf5, vf4x\n\tvmadday.xyz ACC, vf6, vf4y\n\tvmaddz.xyz vf26, vf7, vf4z\n\t" \
    "sqc2 vf2, 0x0(%2)\n\tsqc2 vf3, 0x10(%2)\n\tsqc2 vf4, 0x20(%2)\n\t.set reorder" \
    : : "r" (vectors), "r" (matrix), "r" (cache) : "memory")
#define SDF_VU0_COPYVECTOR_MEMOPS(dst, src) __asm__ volatile ( \
    ".set noreorder\n\tlq $2, 0(%2)\n\tsq $2, 0(%1)\n\t.set reorder" \
    : "=m" (*(u128 *)(dst)) \
    : "r" (dst), "r" (src), "m" (*(const u128 *)(src)) \
    : "$2")
#define EE_LOAD_UNALIGNED_WORD(out, src) __asm__ volatile ( \
    "lwr %0, 0(%1)\n\t" \
    "lwl %0, 3(%1)" \
    : "=&r"(out) : "r"(src))
#define VU0_MATRIX4_INVERT_SCALED() __asm__ volatile ( \
    ".set noreorder\n\t" \
    "vmula.xyz ACC, vf28, vf28\n\tvmadda.xyz ACC, vf29, vf29\n\tvmadd.xyz vf2, vf30, vf30\n\t" \
    "vaddw.xyz vf3, vf0, vf0w\n\t" \
    "qmfc2.ni $8, vf28\n\tqmfc2.ni $9, vf29\n\tqmfc2.ni $10, vf30\n\tqmfc2.ni $11, vf0\n\t" \
    "vdiv Q, vf0w, vf2x\n\t" \
    "pextlw $12, $9, $8\n\tpextuw $13, $9, $8\n\tpextlw $14, $11, $10\n\tpextuw $15, $11, $10\n\t" \
    "pcpyld $8, $14, $12\n\tpcpyud $9, $12, $14\n\tpcpyld $10, $15, $13\n\t" \
    "vwaitq\n\tvmulq.x vf3, vf3, Q\n\t" \
    "vdiv Q, vf0w, vf2y\n\t" \
    "qmtc2.ni $8, vf28\n\tqmtc2.ni $9, vf29\n\tqmtc2.ni $10, vf30\n\t" \
    "vwaitq\n\tvmulq.y vf3, vf3, Q\n\t" \
    "vdiv Q, vf0w, vf2z\n\t" \
    "vwaitq\n\tvmulq.z vf3, vf3, Q\n\t" \
    "vmul.xyz vf28, vf28, vf3\n\tvmul.xyz vf29, vf29, vf3\n\tvmul.xyz vf30, vf30, vf3\n\t" \
    "vmula.xyz ACC, vf0, vf0\n\tvmsubax.xyz ACC, vf28, vf31x\n\t" \
    "vmsubay.xyz ACC, vf29, vf31y\n\tvmsubz.xyz vf31, vf30, vf31z\n\t" \
    ".set reorder\n")
#define VU0_MATRIX4_MUL_BANK_B_RIGHT() __asm__ volatile ( \
    ".set noreorder\n\t" \
    "vmulax.xyzw ACC, vf28, vf24x\n\tvmadday.xyzw ACC, vf29, vf24y\n\t" \
    "vmaddaz.xyzw ACC, vf30, vf24z\n\tvmaddw.xyzw vf2, vf31, vf24w\n\t" \
    "vmulax.xyzw ACC, vf28, vf25x\n\tvmadday.xyzw ACC, vf29, vf25y\n\t" \
    "vmaddaz.xyzw ACC, vf30, vf25z\n\tvmaddw.xyzw vf3, vf31, vf25w\n\t" \
    "vmulax.xyzw ACC, vf28, vf26x\n\tvmadday.xyzw ACC, vf29, vf26y\n\t" \
    "vmaddaz.xyzw ACC, vf30, vf26z\n\tvmaddw.xyzw vf4, vf31, vf26w\n\t" \
    "vmulax.xyzw ACC, vf28, vf27x\n\tvmadday.xyzw ACC, vf29, vf27y\n\t" \
    "vmaddaz.xyzw ACC, vf30, vf27z\n\tvmaddw.xyzw vf31, vf31, vf27w\n\t" \
    "vmove.xyzw vf28, vf2\n\tvmove.xyzw vf29, vf3\n\tvmove.xyzw vf30, vf4\n\t" \
    ".set reorder")
#define VU0_SET_CLIP_PLANES(bias, eye, projection) __asm__ volatile ( \
    ".set noreorder\n\tqmtc2.ni %0, vf2\n\t" \
    "lqc2 vf21, 0(%1)\n\tlqc2 vf22, 0x70(%2)\n\t" \
    "lqc2 vf23, 0x80(%2)\n\tvaddz.x vf23, vf23, vf2z\n\t.set reorder" \
    : : "r" (bias), "r" (eye), "r" (projection))
#define VU0_STORE_MATRIX_AND_UNIT_ROWS(dst) __asm__ volatile ( \
    ".set noreorder\n\t.set noat\n\t" \
    "lui $1, 0x3F80\n\t" \
    "mtc1 $1, $f0\n\t" \
    "vmula.xyz ACC, vf28, vf28\n\t" \
    "vmadda.xyz ACC, vf29, vf29\n\t" \
    "vmadd.xyz vf2, vf30, vf30\n\t" \
    "vrsqrt Q, vf0w, vf2y\n\t" \
    "qmfc2.ni $2, vf2\n\t" \
    "mtc1 $2, $f2\n\t" \
    "rsqrt.s $f2, $f0, $f2\n\t" \
    "sqc2 vf28, 0x0(%0)\n\t" \
    "sqc2 vf29, 0x10(%0)\n\t" \
    "sqc2 vf30, 0x20(%0)\n\t" \
    "sqc2 vf31, 0x30(%0)\n\t" \
    "vwaitq\n\t" \
    "vaddq.y vf3, vf0, Q\n\t" \
    "vrsqrt Q, vf0w, vf2z\n\t" \
    "mfc1 $2, $f2\n\t" \
    "qmtc2.ni $2, vf4\n\t" \
    "vmove.x vf3, vf4\n\t" \
    "vwaitq\n\t" \
    "vaddq.z vf3, vf0, Q\n\t" \
    "vmul.xyz vf4, vf28, vf3\n\t" \
    "vmul.xyz vf5, vf29, vf3\n\t" \
    "vmul.xyz vf6, vf30, vf3\n\t" \
    "sqc2 vf4, 0x40(%0)\n\t" \
    "sqc2 vf5, 0x50(%0)\n\t" \
    "sqc2 vf6, 0x60(%0)\n\t" \
    ".set at\n\t.set reorder" \
    : : "r" (dst) : "$1", "$2", "$f0", "$f2", "memory")
#define EE_MMI_RGBA_MULTIPLY_VECTOR(out, a, b, unit) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "pextlb $2, $0, %1\n\t" \
    "pextlb $3, $0, %2\n\t" \
    "pextlh $2, $0, $2\n\t" \
    "pextlh $3, $0, $3\n\t" \
    "qmtc2.ni $2, vf2\n\t" \
    "qmtc2.ni $3, vf3\n\t" \
    "qmtc2.ni %3, vf4\n\t" \
    "vitof0.xyzw vf2, vf2\n\t" \
    "vitof0.xyzw vf3, vf3\n\t" \
    "vmul.xyzw vf2, vf2, vf3\n\t" \
    "vmulx.xyzw vf2, vf2, vf4x\n\t" \
    "qmfc2.ni %0, vf2\n\t" \
    ".set reorder" \
    : "=r" (out) : "r" (a), "r" (b), "r" (unit) : "$2", "$3")
#define VU0_ROTATE_BASIS_X(c, s) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "mfc1 $3, %0\n\tmfc1 $2, %1\n\tqmtc2.ni $3, $vf3\n\tqmtc2.ni $2, $vf2\n\t" \
    "vmove.xyzw $vf4, $vf29\n\t" \
    "vmulax.xyzw ACC, $vf29, $vf3x\n\tvmaddx.xyzw $vf29, $vf30, $vf2x\n\t" \
    "vmulax.xyzw ACC, $vf30, $vf3x\n\tvmsubx.xyzw $vf30, $vf4, $vf2x\n\t" \
    ".set reorder\n" \
    : : "f" (c), "f" (s) : "$2", "$3")
#define VU0_ROTATE_BASIS_Y(c, s) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "mfc1 $3, %0\n\tmfc1 $2, %1\n\tqmtc2.ni $3, $vf3\n\tqmtc2.ni $2, $vf2\n\t" \
    "vmove.xyzw $vf4, $vf28\n\t" \
    "vmulax.xyzw ACC, $vf28, $vf3x\n\tvmsubx.xyzw $vf28, $vf30, $vf2x\n\t" \
    "vmulax.xyzw ACC, $vf4, $vf2x\n\tvmaddx.xyzw $vf30, $vf30, $vf3x\n\t" \
    ".set reorder\n" \
    : : "f" (c), "f" (s) : "$2", "$3")
#define VU0_ROTATE_BASIS_Z(c, s) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "mfc1 $3, %0\n\tmfc1 $2, %1\n\tqmtc2.ni $3, $vf3\n\tqmtc2.ni $2, $vf2\n\t" \
    "vmove.xyzw $vf4, $vf28\n\t" \
    "vmulax.xyzw ACC, $vf28, $vf3x\n\tvmaddx.xyzw $vf28, $vf29, $vf2x\n\t" \
    "vmulax.xyzw ACC, $vf29, $vf3x\n\tvmsubx.xyzw $vf29, $vf4, $vf2x\n\t" \
    ".set reorder\n" \
    : : "f" (c), "f" (s) : "$2", "$3")
#define VU0_TRIANGLE_FACING(out, a, b, c) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf2, 0(%1)\n\tlqc2 vf3, 0(%2)\n\tlqc2 vf4, 0(%3)\n\t" \
    "vsub.xyz vf2, vf2, vf3\n\tvsub.xyz vf4, vf4, vf3\n\tvsub.xyz vf3, vf21, vf3\n\t" \
    "vopmula.xyz ACC, vf2, vf4\n\tvopmsub.xyz vf2, vf4, vf2\n\t" \
    "vmul.xyz vf2, vf2, vf3\n\tvadday.x ACC, vf2, vf2y\n\tvmaddz.x vf2, vf1, vf2z\n\t" \
    "qmfc2.ni $2, vf2\n\tmtc1 $2, %0\n\t.set reorder" \
    : "=f" (out) : "r" (a), "r" (b), "r" (c) : "$2")
#define VU0_BLEND_NODE_XY(node, first, second) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "lqc2 vf2, 0x40(%0)\n\tlqc2 vf8, 0x30(%1)\n\tlqc2 vf9, 0x30(%2)\n\t" \
    "vmulaw.xy ACC, vf8, vf0w\n\tvmaddaw.xy ACC, vf9, vf2w\n\tvmsubw.xy vf15, vf8, vf2w\n\t" \
    "sqc2 vf15, 0x30(%0)\n\t.set reorder" \
    : : "r" (node), "r" (first), "r" (second) : "memory")
#define VU0_BLEND_NODE_VECTORS(node, first, second) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "lqc2 vf2, 0x40(%0)\n\tlqc2 vf8, 0x30(%1)\n\tlqc2 vf9, 0x30(%2)\n\t" \
    "lqc2 vf10, 0x20(%1)\n\tlqc2 vf11, 0x20(%2)\n\t" \
    "vmulaw.xy ACC, vf8, vf0w\n\tvmaddaw.xy ACC, vf9, vf2w\n\tvmsubw.xy vf15, vf8, vf2w\n\t" \
    "vmulaw.xyzw ACC, vf10, vf0w\n\tvmaddaw.xyzw ACC, vf11, vf2w\n\tvmsubw.xyzw vf16, vf10, vf2w\n\t" \
    "sqc2 vf15, 0x30(%0)\n\tsqc2 vf16, 0x20(%0)\n\t.set reorder" \
    : : "r" (node), "r" (first), "r" (second) : "memory")
#define VU0_INTERPOLATE_CLIP_VERTEX(out, first, second) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "vaddx.w vf2, vf0, vf0x\n\tlbu $2, 0x50(%1)\n\tlqc2 vf4, 0x10(%1)\n\tlqc2 vf5, 0x10(%2)\n\t" \
    "andi $2, $2, 0x1\n\tbne $0, $2, 1f\n\tlqc2 vf6, 0x0(%1)\n\tvsubw.w vf2, vf1, vf0w\n" \
    "1:\n\tlqc2 vf7, 0x0(%2)\n\tlqc2 vf8, 0x30(%1)\n\tlqc2 vf9, 0x30(%2)\n\t" \
    "vsubw.w vf12, vf2, vf4w\n\tvsubw.w vf13, vf5, vf4w\n\tvsub.xyzw vf14, vf7, vf6\n\t" \
    "vdiv Q, vf12w, vf13w\n\tlqc2 vf10, 0x20(%1)\n\tvsub.xyzw vf15, vf9, vf8\n\t" \
    "vmulaw.xyzw ACC, vf6, vf0w\n\tsqc2 vf10, 0x20(%0)\n\tvwaitq\n\tcfc2.ni $2, $vi22\n\t" \
    "vmaddq.xyzw vf14, vf14, Q\n\tvmulaw.xyzw ACC, vf8, vf0w\n\tvmaddq.xy vf15, vf15, Q\n\t" \
    "sqc2 vf14, 0x0(%0)\n\tsw $2, 0x4C(%0)\n\tsqc2 vf15, 0x30(%0)\n\tsqc2 vf2, 0x10(%0)\n\t" \
    ".set reorder" \
    : : "r" (out), "r" (first), "r" (second) : "$2", "memory")
#define VU0_BUILD_PACKED_COLOR_TRANSFORM(dst, reference, packedColor, firstColor, secondColor, blend) \
__asm__ volatile ( \
        ".set noreorder\n\t" \
        "pextlb $2, $0, %3\n\t" \
        "pextlb $3, $0, %4\n\t" \
        "pextlb $4, $0, %2\n\t" \
        "pextlh $2, $0, $2\n\t" \
        "pextlh $3, $0, $3\n\t" \
        "pextlh $4, $0, $4\n\t" \
        "qmtc2.ni $2, vf2\n\t" \
        "qmtc2.ni $3, vf3\n\t" \
        "qmtc2.ni $4, vf4\n\t" \
        "qmtc2.ni %5, vf5\n\t" \
        "qmtc2.ni %6, vf6\n\t" \
        "qmtc2.ni %7, vf7\n\t" \
        "vitof0.xyzw vf2, vf2\n\t" \
        "vitof0.xyzw vf3, vf3\n\t" \
        "vitof0.xyzw vf4, vf4\n\t" \
        "lqc2 vf8, 0x40(%1)\n\t" \
        "lqc2 vf9, 0x80(%1)\n\t" \
        "vmulx.xyzw vf2, vf2, vf6x\n\t" \
        "vmulx.xyzw vf3, vf3, vf6x\n\t" \
        "vmove.w vf9, vf0\n\t" \
        "vmulx.w vf8, vf8, vf0x\n\t" \
        "vmulw.w vf4, vf4, vf2w\n\t" \
        "vmul.xyz vf9, vf3, vf9\n\t" \
        "vmul.xyz vf8, vf2, vf8\n\t" \
        "vmulaw.xyz ACC, vf9, vf0w\n\t" \
        "vmaddax.xyz ACC, vf8, vf5x\n\t" \
        "vmsubx.xyz vf9, vf9, vf5x\n\t" \
        "vmul.xyzw vf8, vf8, vf4\n\t" \
        "vmul.xyzw vf9, vf9, vf4\n\t" \
        "vminix.xyzw vf8, vf8, vf7x\n\t" \
        "vsub.xyz vf8, vf8, vf9\n\t" \
        "sqc2 vf9, 0x10(%0)\n\t" \
        "sqc2 vf8, 0x20(%0)\n\t" \
        ".set reorder" \
        : : "r"(dst), "r"(reference), "r"(packedColor), \
            "r"(firstColor), "r"(secondColor), "r"(blend), \
            "r"(0.0078125f), "r"(0x437F0000) \
        : "$2", "$3", "$4", "memory")

/* from code_002C6F38.c */
/* The wire-model clipper keeps the segment in vf10/vf11, its colours in
 * vf8/vf9, the viewport scale and offset in vf14/vf15 and the plane in vf2. */
#define VU0_LOAD_VF14(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf14, 0(%0)\n\t.set reorder" \
    : : "r" (src))
#define VU0_LOAD_VF15_AT16(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf15, 16(%0)\n\t.set reorder" \
    : : "r" (src))
#define VU0_TRANSFORM_POINT_VF11() __asm__ volatile ( \
    ".set noreorder\n\tvmulax.xyzw ACC, vf28, vf11x\n\tvmadday.xyzw ACC, vf29, vf11y\n\t" \
    "vmaddaz.xyzw ACC, vf30, vf11z\n\tvmaddw.xyzw vf11, vf31, vf0w\n\t.set reorder")
#define SDF_VU0_CLIPTEST_B() __asm__ volatile ( \
    ".set noreorder\n\tvclipw.xyz vf11, vf11w\n\t.set reorder")
#define SDF_VU0_CLIPTESTWAIT(out) __asm__ volatile ( \
    ".set noreorder\n\tvnop\n\tvnop\n\tvnop\n\tvnop\n\tvnop\n\tcfc2.ni %0, $vi18\n\t.set reorder" \
    : "=r" (out))
#define VU0_STORE_VF8_AT0(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf8, 0(%0)\n\t.set reorder" \
    : : "r" (dst))
#define VU0_STORE_VF10_AT16(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf10, 16(%0)\n\t.set reorder" \
    : : "r" (dst))
#define VU0_STORE_VF9_AT32(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf9, 32(%0)\n\t.set reorder" \
    : : "r" (dst))
#define VU0_STORE_VF11_AT48(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf11, 48(%0)\n\t.set reorder" \
    : : "r" (dst))
#define VU0_POSITIVE_CLIP_PLANE_VF2() __asm__ volatile ( \
    ".set noreorder\n\tvaddw.xyz vf2, vf0, vf0w\n\t" \
    "vmulw.w vf2, vf0, vf0w\n\t.set reorder")
#define VU0_NEGATIVE_CLIP_PLANE_VF2() __asm__ volatile ( \
    ".set noreorder\n\tvsubw.xyzw vf2, vf0, vf0w\n\t" \
    "vsubw.w vf2, vf0, vf0w\n\t.set reorder")
#define VU0_CLIP_SEGMENT_PREP_FIRST() __asm__ volatile ( \
    ".set noreorder\n\t" \
    "vmulw.xyzw vf3, vf2, vf10w\n\t" \
    "vmulw.xyzw vf4, vf2, vf11w\n\t" \
    "vsub.xyzw vf5, vf11, vf10\n\t" \
    "vsub.xyzw vf6, vf9, vf8\n\t" \
    "vsub.xyzw vf3, vf10, vf3\n\t" \
    "vsub.xyzw vf4, vf11, vf4\n\t" \
    "vsub.xyzw vf4, vf4, vf3\n\t.set reorder")
#define VU0_CLIP_SEGMENT_PREP_SECOND() __asm__ volatile ( \
    ".set noreorder\n\t" \
    "vmulw.xyzw vf3, vf2, vf11w\n\t" \
    "vmulw.xyzw vf4, vf2, vf10w\n\t" \
    "vsub.xyzw vf5, vf10, vf11\n\t" \
    "vsub.xyzw vf6, vf8, vf9\n\t" \
    "vsub.xyzw vf3, vf11, vf3\n\t" \
    "vsub.xyzw vf4, vf10, vf4\n\t" \
    "vsub.xyzw vf4, vf4, vf3\n\t.set reorder")
#define VU0_CLIP_RATIO_X() __asm__ volatile ( \
    ".set noreorder\n\tvnop\n\tvnop\n\tvdiv Q, vf3x, vf4x\n\t.set reorder")
#define VU0_CLIP_RATIO_Y() __asm__ volatile ( \
    ".set noreorder\n\tvnop\n\tvnop\n\tvdiv Q, vf3y, vf4y\n\t.set reorder")
#define VU0_CLIP_RATIO_Z() __asm__ volatile ( \
    ".set noreorder\n\tvnop\n\tvnop\n\tvdiv Q, vf3z, vf4z\n\t.set reorder")
#define VU0_PROJECT_COLORED_SEGMENT() __asm__ volatile ( \
    ".set noreorder\n\t" \
    "vdiv Q, vf0w, vf10w\n\t" \
    "vmul.xyz vf10, vf10, vf14\n\t" \
    "vmulaw.xyz ACC, vf15, vf0w\n\t" \
    "vmulx.w vf10, vf10, vf0x\n\t" \
    "vftoi0.xyzw vf8, vf8\n\tvwaitq\n\t" \
    "vmaddq.xyz vf10, vf10, Q\n\t" \
    "vdiv Q, vf0w, vf11w\n\t" \
    "vmul.xyz vf11, vf11, vf14\n\t" \
    "vmulx.w vf11, vf11, vf0x\n\t" \
    "vftoi4.xyzw vf10, vf10\n\t" \
    "vftoi0.xyzw vf9, vf9\n\tvwaitq\n\t" \
    "vmaddq.xyz vf11, vf11, Q\n\t" \
    "vftoi4.xyzw vf11, vf11\n\t.set reorder")
/* The unpack with the colour word read through an input operand instead of a
 * memory clobber, so a surrounding loop can still hoist unrelated loads, and
 * with the source operand first: the hoisted address and the hoisted unit
 * constant come out in operand order. */
#define MMI_RGBA_UNPACK_NOMEM(src, unit) __asm__ volatile ( \
    ".set noreorder\n\tlw $2, 0(%0)\n\tpextlb $2, $0, $2\n\tpextlh $2, $0, $2\n\t" \
    "qmtc2.ni $2, vf10\n\tvitof0.xyzw vf10, vf10\n\tqmtc2.ni %1, vf2\n\t" \
    "vmulx.xyzw vf10, vf10, vf2x\n\t.set reorder" \
    : : "r" (src), "r" (unit), "m" (*(const u32 *)(src)) : "$2")
#define EE_MMI_RGBA_UNPACK_READONLY(src, unit) __asm__ volatile ( \
    ".set noreorder\n" \
    "lw $2, 0(%1)\n" \
    "pextlb $2, $0, $2\n" \
    "pextlh $2, $0, $2\n" \
    "qmtc2.ni $2, vf10\n" \
    "vitof0.xyzw vf10, vf10\n" \
    "qmtc2.ni %0, vf2\n" \
    "vmulx.xyzw vf10, vf10, vf2x\n" \
    ".set reorder" \
    : : "r"(unit), "r"(src), "m"(*(const u32 *)(src)) : "$2")
#define EE_MMI_RGBA_TO_VF8(rgba) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "pextlb $2, $0, %0\n\t" \
    "pextlh $2, $0, $2\n\t" \
    "qmtc2.ni $2, vf8\n\t" \
    "vitof0.xyzw vf8, vf8\n\t" \
    ".set reorder" \
    : : "r" (rgba) : "$2")
#define EE_MMI_RGBA_TO_VF9(rgba) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "pextlb $2, $0, %0\n\t" \
    "pextlh $2, $0, $2\n\t" \
    "qmtc2.ni $2, vf9\n\t" \
    "vitof0.xyzw vf9, vf9\n\t" \
    ".set reorder" \
    : : "r" (rgba) : "$2")
#define VU0_CLIP_SEGMENT_UPDATE_FIRST() __asm__ volatile ( \
    ".set noreorder\n\tvwaitq\n\tvaddq.x vf2, vf0, Q\n\tvabs.x vf2, vf2\n\t" \
    "vmulaw.xyzw ACC, vf10, vf0w\n\t" \
    "vmaddx.xyzw vf10, vf5, vf2x\n\t" \
    "vmulaw.xyzw ACC, vf8, vf0w\n\t" \
    "vmaddx.xyzw vf8, vf6, vf2x\n\t" \
    "vclipw.xyz vf10, vf10w\n\t.set reorder")
#define VU0_CLIP_SEGMENT_UPDATE_SECOND() __asm__ volatile ( \
    ".set noreorder\n\tvwaitq\n\tvaddq.x vf2, vf0, Q\n\tvabs.x vf2, vf2\n\t" \
    "vmulaw.xyzw ACC, vf11, vf0w\n\t" \
    "vmaddx.xyzw vf11, vf5, vf2x\n\t" \
    "vmulaw.xyzw ACC, vf9, vf0w\n\t" \
    "vmaddx.xyzw vf9, vf6, vf2x\n\t" \
    "vclipw.xyz vf11, vf11w\n\t.set reorder")

/* from effect/effPCPScatter.c */
/* Load-normalize-store of one packed vector in a single block ("+m" operand
 * with a memory clobber). */
#define VU0_NORMALIZE_PACKED_VECTOR(v) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf10, %0\n\tvmul.xyz vf2, vf10, vf10\n\tvmulax.w ACC, vf0, vf2x\n\t" \
    "vmadday.w ACC, vf0, vf2y\n\tvmaddz.w vf2, vf0, vf2z\n\tvrsqrt Q, vf0w, vf2w\n\t" \
    "vwaitq\n\tvmulq.xyz vf10, vf10, Q\n\tsqc2 vf10, %0\n\t.set reorder" \
    : "+m" (v) :: "memory")

/* from effect/parManager.c */
#define SDF_VU0_STOREVECTOR_C(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 " "vf12" ", 0(%0)\n\t.set reorder" \
    : : "r" (dst) : "memory")

/* from kernel/dds3KernelDraw.c */
/* Concrete SDK register transfers used by the quaternion and projection paths.
 * VF2 is the scalar/swap scratch; C float transfers use the SDK's $2 scratch. */
#define SDF_VU0_SWAPVECTOR_A_B() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf2, vf10\n\tvmove.xyzw vf10, vf11\n\t" \
    "vmove.xyzw vf11, vf2\n\t.set reorder")
#define SDF_VU0_SWAPVECTOR_A_C() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf2, vf10\n\tvmove.xyzw vf10, vf12\n\t" \
    "vmove.xyzw vf12, vf2\n\t.set reorder")
#define MMI_RGBA_PACK_255(out) __asm__ volatile ( \
    ".set noreorder\n\tmfc1 $2, %1\n\tqmtc2.ni $2, vf2\n\t" \
    "vmulx.xyzw vf10, vf10, vf2x\n\tvftoi0.xyzw vf10, vf10\n\t" \
    "qmfc2.ni %0, vf10\n\tppach %0, $0, %0\n\tppacb %0, $0, %0\n\t.set reorder" \
    : "=r" (out) : "f" (255.0f))

/* Light and ambient colors scale by 128, and their scratch register is $3. */
#define MMI_RGBA_PACK_128(out) __asm__ volatile ( \
    ".set noreorder\n\tmfc1 $3, %1\n\tqmtc2.ni $3, vf2\n\t" \
    "vmulx.xyzw vf10, vf10, vf2x\n\tvftoi0.xyzw vf10, vf10\n\t" \
    "qmfc2.ni %0, vf10\n\tppach %0, $0, %0\n\tppacb %0, $0, %0\n\t.set reorder" \
    : "=r" (out) : "f" (128.0f))

/* The same pack by 128 through the $2 scratch, as event/evtCommand uses. */
#define MMI_RGBA_PACK_128_S2(out) __asm__ volatile ( \
    ".set noreorder\n\tmfc1 $2, %1\n\tqmtc2.ni $2, vf2\n\t" \
    "vmulx.xyzw vf10, vf10, vf2x\n\tvftoi0.xyzw vf10, vf10\n\t" \
    "qmfc2.ni %0, vf10\n\tppach %0, $0, %0\n\tppacb %0, $0, %0\n\t.set reorder" \
    : "=r" (out) : "f" (128.0f))

/* from script/scrCommonCommand.c */
#define SDF_VU0_SETVECTOR_X_BITS(r) __asm__ volatile ( \
    ".set noreorder\n\tqmtc2 %0, $vf2\n\tvaddx.x $vf10, $vf0, $vf2x\n\t.set reorder" \
    : : "r" (r))
#define SDF_VU0_SETVECTOR_Y_BITS(r) __asm__ volatile ( \
    ".set noreorder\n\tqmtc2 %0, $vf2\n\tvaddx.y $vf10, $vf0, $vf2x\n\t.set reorder" \
    : : "r" (r))
#define SDF_VU0_SETVECTOR_Z_BITS(r) __asm__ volatile ( \
    ".set noreorder\n\tqmtc2 %0, $vf2\n\tvaddx.z $vf10, $vf0, $vf2x\n\t.set reorder" \
    : : "r" (r))

/* from sdf/sdfModel.c */
#define SDF_VU0_SCALE_XYZ_REV() __asm__ volatile ( \
    ".set noreorder\n\tvmul.xyz vf28, vf28, vf10\n\tvmul.xyz vf29, vf29, vf10\n\t" \
    "vmul.xyz vf30, vf30, vf10\n\t.set reorder" \
    : : : "memory")
#define SDF_VU0_STOREMATRIX_MEMOPS(a, b, c, d) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf28, %0\n\tsqc2 vf29, %1\n\tsqc2 vf30, %2\n\tsqc2 vf31, %3\n\t" \
    ".set reorder" \
    : : "m" (a), "m" (b), "m" (c), "m" (d) : "memory")
#define VU0_CLIP_TEST_BOX_CORNERS(flags, bounds) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "ldr $2, 0(%1)\n\tldl $2, 7(%1)\n\tlw $3, 8(%1)\n\t" \
    "ldr $6, 0xC(%1)\n\tldl $6, 0x13(%1)\n\tlw $7, 0x14(%1)\n\t" \
    "pcpyld $2, $3, $2\n\tpcpyld $6, $7, $6\n\tqmtc2.ni $2, vf2\n\tqmtc2.ni $6, vf3\n\t" \
    "vmulax.xyzw ACC, vf28, vf2x\n\tvmadday.xyzw ACC, vf29, vf2y\n\tvmaddaz.xyzw ACC, vf30, vf2z\n\tvmaddw.xyzw vf4, vf31, vf0w\n\t" \
    "vmulax.xyzw ACC, vf28, vf3x\n\tvmadday.xyzw ACC, vf29, vf2y\n\tvmaddaz.xyzw ACC, vf30, vf2z\n\tvmaddw.xyzw vf5, vf31, vf0w\n\t" \
    "vclipw.xyz vf4, vf4w\n\t" \
    "vmulax.xyzw ACC, vf28, vf2x\n\tvmadday.xyzw ACC, vf29, vf3y\n\tvmaddaz.xyzw ACC, vf30, vf2z\n\tvmaddw.xyzw vf4, vf31, vf0w\n\t" \
    "vclipw.xyz vf5, vf5w\n\t" \
    "vmulax.xyzw ACC, vf28, vf3x\n\tvmadday.xyzw ACC, vf29, vf3y\n\tvmaddaz.xyzw ACC, vf30, vf2z\n\tvmaddw.xyzw vf5, vf31, vf0w\n\t" \
    "vclipw.xyz vf4, vf4w\n\t" \
    "vmulax.xyzw ACC, vf28, vf2x\n\tvmadday.xyzw ACC, vf29, vf2y\n\tvmaddaz.xyzw ACC, vf30, vf3z\n\tvmaddw.xyzw vf4, vf31, vf0w\n\t" \
    "vclipw.xyz vf5, vf5w\n\t" \
    "vmulax.xyzw ACC, vf28, vf3x\n\tvmadday.xyzw ACC, vf29, vf2y\n\tvmaddaz.xyzw ACC, vf30, vf3z\n\tvmaddw.xyzw vf5, vf31, vf0w\n\t" \
    "vnop\n\tcfc2.ni $2, $vi18\n\t" \
    "vclipw.xyz vf4, vf4w\n\t" \
    "vmulax.xyzw ACC, vf28, vf2x\n\tvmadday.xyzw ACC, vf29, vf3y\n\tvmaddaz.xyzw ACC, vf30, vf3z\n\tvmaddw.xyzw vf4, vf31, vf0w\n\t" \
    "vclipw.xyz vf5, vf5w\n\t" \
    "vmulax.xyzw ACC, vf28, vf3x\n\tvmadday.xyzw ACC, vf29, vf3y\n\tvmaddaz.xyzw ACC, vf30, vf3z\n\tvmaddw.xyzw vf5, vf31, vf0w\n\t" \
    "vclipw.xyz vf4, vf4w\n\t" \
    "vnop\n\tvnop\n\t" \
    "vclipw.xyz vf5, vf5w\n\t" \
    "vnop\n\tvnop\n\tvnop\n\tvnop\n\tvnop\n\tcfc2.ni $3, $vi18\n\t" \
    "and %0, $2, $3\n\t.set reorder" \
    : "=r" (flags) : "r" (bounds) : "$2", "$3", "$6", "$7", "memory")

/* from sdf/sdfMotion.c */
/* SDK packed-key and VU interpolation sequences used by model channels. */
#define VU0_LERP_VF10_W(t) __asm__ volatile ( \
    ".set noreorder\n\tmfc1 $2, %0\n\tqmtc2.ni $2, vf2\n\tvsubx.w vf3, vf0, vf2x\n\t" \
    "vmulax.xyzw ACC, vf11, vf2x\n\tvmaddw.xyzw vf10, vf10, vf3w\n\t" \
    "vmove.w vf10, vf0\n\t.set reorder" \
    : : "f" (t))
#define VU0_LERP_VF10_COPY(t) __asm__ volatile ( \
    ".set noreorder\n\tmfc1 $2, %0\n\tqmtc2.ni $2, vf2\n\tvsubx.w vf3, vf0, vf2x\n\t" \
    "vmulax.xyzw ACC, vf11, vf2x\n\tvmaddw.xyzw vf10, vf10, vf3w\n\t" \
    "vmove.xyzw vf11, vf10\n\t.set reorder" \
    : : "f" (t))
/* Retail repeats the first pointer and transfers the raw z word to vf3;
 * preserve these two SDK sequence defects (SMT3 0x002B4258, DDS1 twin). */
#define VU0_LERP_VEC3_KEYS(first, second, t) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "qmtc2.ni %2, vf4\n\t" \
    "ldr $2, 0(%0)\n\t" \
    "ldl $2, 7(%0)\n\t" \
    "lw $3, 8(%0)\n\t" \
    "ldr $4, 0(%0)\n\t" \
    "ldl $4, 7(%0)\n\t" \
    "lw $5, 8(%0)\n\t" \
    "pcpyld $2, $3, $2\n\t" \
    "pcpyld $4, $5, $4\n\t" \
    "vsubx.w vf5, vf0, vf4x\n\t" \
    "vmulx.xyzw vf10, vf0, vf0x\n\t" \
    "qmtc2.ni $2, vf2\n\t" \
    "qmtc2.ni $3, vf3\n\t" \
    "vmulaw.xyz ACC, vf2, vf5w\n\t" \
    "vmaddx.xyz vf10, vf3, vf4x\n\t" \
    ".set reorder" \
    : : "r" (first), "r" (second), "r" (t) : "$2", "$3", "$4", "$5")
#define SDF_VU0_LOADVECTOR_UNALIGN_B(p) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "ldr $2, 0(%0)\n\t" \
    "ldl $2, 7(%0)\n\t" \
    "lw $3, 8(%0)\n\t" \
    "pcpyld $2, $3, $2\n\t" \
    "qmtc2.ni $2, " "vf11" "\n\t" \
    ".set reorder" \
    : : "r"(p) : "$2", "$3")
#define SDF_VU0_LOADSHORTVECTOR(p) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "ldr $2, 0(%0)\n\t" \
    "ldl $2, 7(%0)\n\t" \
    "pextlh $2, $2, $0\n\t" \
    "psraw $2, $2, 16\n\t" \
    "qmtc2.ni $2, " "vf10" "\n\t" \
    "vitof12.xyzw " "vf10" ", " "vf10" "\n\t" \
    ".set reorder" \
    : : "r"(p) : "$2")
#define SDF_VU0_LOADSHORTVECTOR_B(p) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "ldr $2, 0(%0)\n\t" \
    "ldl $2, 7(%0)\n\t" \
    "pextlh $2, $2, $0\n\t" \
    "psraw $2, $2, 16\n\t" \
    "qmtc2.ni $2, " "vf11" "\n\t" \
    "vitof12.xyzw " "vf11" ", " "vf11" "\n\t" \
    ".set reorder" \
    : : "r"(p) : "$2")
#define SDF_VU0_STORE_ROW0_NOMEM(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 " "vf28" ", 0(%0)\n\t.set reorder" \
    : : "r" (dst))
#define SDF_VU0_STORE_ROW1_NOMEM(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 " "vf29" ", 0(%0)\n\t.set reorder" \
    : : "r" (dst))
#define SDF_VU0_STORE_ROW2_NOMEM(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 " "vf30" ", 0(%0)\n\t.set reorder" \
    : : "r" (dst))
/* out = a + (b - a) * t per RGBA byte lane, rounded through vf3x. `half` is
 * 0.5f in a general register. */
#define VU0_RGBA_LERP(out, a, b, t, half) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "qmtc2.ni %4, vf3\n\t" \
    "mfc1 $8, %3\n\t" \
    "pextlb $4, $0, %1\n\t" \
    "pextlb $5, $0, %2\n\t" \
    "vsub.xyzw vf2, vf0, vf0\n\t" \
    "qmtc2.ni $8, vf4\n\t" \
    "pextlh $4, $0, $4\n\t" \
    "pextlh $5, $0, $5\n\t" \
    "vaddax.xyzw ACC, vf2, vf3x\n\t" \
    "vsubx.w vf5, vf0, vf4x\n\t" \
    "qmtc2.ni $4, vf2\n\t" \
    "qmtc2.ni $5, vf3\n\t" \
    "vsubx.w vf5, vf0, vf4x\n\t" \
    "vitof0.xyzw vf2, vf2\n\t" \
    "vitof0.xyzw vf3, vf3\n\t" \
    "vmaddaw.xyzw ACC, vf2, vf5w\n\t" \
    "vmaddx.xyzw vf5, vf3, vf4x\n\t" \
    "vftoi0.xyzw vf4, vf5\n\t" \
    "qmfc2.ni $4, vf4\n\t" \
    "ppach $5, $0, $4\n\t" \
    "ppacb %0, $0, $5\n\t" \
    ".set reorder" \
    : "=r" (out) : "r" (a), "r" (b), "f" (t), "r" (half) : "$4", "$5", "$6", "$7", "$8")

/* The scalar reaches vf2.x through $2 rather than a register cc1 picks,
 * which is what these callers' retail allocation needs. */

#define SDF_VU0_SCALEVECTOR_B_V0(f) __asm__ volatile ( \
    ".set noreorder\n\tmfc1 $2, %0\n\tqmtc2.ni $2, vf2\n\t" \
    "vmulx.xyzw vf11, vf11, vf2x\n\t.set reorder" \
    : : "f" (f))
#define SDF_VU0_SETVECTOR_Y_V0(f) __asm__ volatile ( \
    ".set noreorder\n\tmfc1 $2, %0\n\tqmtc2.ni $2, vf2\n\t" \
    "vaddx.y vf10, vf0, vf2x\n\t.set reorder" \
    : : "f" (f))
/* ---- sequences that units wrote out in place ---- */

/* ---- sequences that units wrote out in place ---- */

/* From src/smt3/code_002BD340.c. */
#define VU0_SEQ_MULT() __asm__ volatile ( \
    ".set noreorder\n" \
    "\tvmul.xyzw vf2, vf10, vf11\n" \
    "\tvopmula.xyz ACC, vf10, vf11\n" \
    "\tvmaddaw.xyz ACC, vf11, vf10\n" \
    "\tvmaddaw.xyz ACC, vf10, vf11\n" \
    "\tvopmsub.xyz vf10, vf11, vf10\n" \
    "\tvmulaw.w ACC, vf10, vf11\n" \
    "\tvmsubax.w ACC, vf0, vf2\n" \
    "\tvmsubay.w ACC, vf0, vf2\n" \
    "\tvmsubz.w vf10, vf0, vf2\n" \
    "\t.set reorder\n" \
    : : : "memory")

/* From src/smt3/code_002BD340.c. */
#define VU0_SEQ_INVERSE() __asm__ volatile ( \
    ".set noreorder\n" \
    "\tvmul.xyzw vf2, vf10, vf10\n" \
    "\tvaddax.w ACC, vf2, vf2x\n" \
    "\tvmadday.w ACC, vf0, vf2y\n" \
    "\tvmaddz.w vf3, vf0, vf2z\n" \
    "\tvmove.w vf2, vf10\n" \
    "\tvdiv Q, vf0w, vf3w\n" \
    "\tvsub.xyz vf2, vf0, vf10\n" \
    "\tvwaitq\n" \
    "\tvmulq.xyzw vf10, vf2, Q\n" \
    "\t.set reorder")

/* From src/smt3/code_002BD340.c. */
#define VU0_SEQ_NORMALIZE() __asm__ volatile ( \
    ".set noreorder\n" \
    "\tvmul.xyzw vf2, vf10, vf10\n" \
    "\tvaddax.w ACC, vf2, vf2\n" \
    "\tvmadday.w ACC, vf0, vf2\n" \
    "\tvmaddz.w vf3, vf0, vf2\n" \
    "\tvrsqrt Q, vf0w, vf3w\n" \
    "\tvwaitq\n" \
    "\tvmulq.xyzw vf10, vf10, Q\n" \
    "\t.set reorder")

/* From src/smt3/code_002BD340.c. */
#define VU0_SEQ_SDF_2_C0510(componentMetric) __asm__ volatile ( \
    ".set noreorder\n" \
    "\tvaddw.xyz vf1, vf0, vf0w\n" \
    "\tvmul.xyzw vf2, vf10, vf10\n" \
    "\tvmulay.x ACC, vf2, vf2y\n" \
    "\tvmaddaz.x ACC, vf1, vf2z\n" \
    "\tvmaddw.x vf2, vf1, vf2w\n" \
    "\tqmfc2.ni $2, vf2\n" \
    "\tmtc1 $2, %0\n" \
    "\t.set reorder" \
    : "=f" (componentMetric))

/* From src/smt3/code_002BD340.c. */
#define VU0_SEQ_FROM_ROTATION(trigValue) __asm__ volatile ( \
    ".set noreorder\n" \
    "\tmfc1 $2, %0\n" \
    "\tqmtc2.ni $2, vf2\n" \
    "\tvmulx.w vf10, vf0, vf2x\n" \
    "\t.set reorder" \
    : : "f" (trigValue))

/* From src/smt3/code_002BD340.c. */
#define VU0_SEQ_FROM_ROTATION_B(trigValue) __asm__ volatile ( \
    ".set noreorder\n" \
    "\tmfc1 $2, %0\n" \
    "\tqmtc2.ni $2, vf2\n" \
    "\tvmulx.w vf11, vf0, vf2x\n" \
    "\t.set reorder" \
    : : "f" (trigValue))

/* From src/smt3/code_002BD340.c. */
#define VU0_SEQ_TO_MATRIX() __asm__ volatile ( \
    ".set noreorder\n" \
    "\tvaddw.xyz vf1, vf0, vf0w\n" \
    "\tvadd.xyzw vf2, vf10, vf10\n" \
    "\tvmulx.w vf28, vf0, vf0x\n" \
    "\tvmulx.w vf29, vf0, vf0x\n" \
    "\tvmulx.w vf30, vf0, vf0x\n" \
    "\tvmul.xyzw vf3, vf10, vf2\n" \
    "\tvmuly.xyzw vf4, vf10, vf2y\n" \
    "\tvmulz.xyzw vf5, vf10, vf2z\n" \
    "\tvmulx.xyzw vf6, vf10, vf2x\n" \
    "\tvaddaw.xyz ACC, vf0, vf0w\n" \
    "\tvmsubay.x ACC, vf1, vf3y\n" \
    "\tvmsubz.x vf28, vf1, vf3z\n" \
    "\tvmsubax.y ACC, vf1, vf3x\n" \
    "\tvmsubz.y vf29, vf1, vf3z\n" \
    "\tvmsubax.z ACC, vf1, vf3x\n" \
    "\tvmsuby.z vf30, vf1, vf3y\n" \
    "\tvmulax.y ACC, vf1, vf4x\n" \
    "\tvmsubw.y vf28, vf1, vf5w\n" \
    "\tvaddw.x vf29, vf4, vf5w\n" \
    "\tvsubw.x vf30, vf5, vf4w\n" \
    "\tvmulax.z ACC, vf1, vf5x\n" \
    "\tvmaddw.z vf28, vf1, vf4w\n" \
    "\tvmulay.z ACC, vf1, vf5y\n" \
    "\tvmsubw.z vf29, vf1, vf6w\n" \
    "\tvaddw.y vf30, vf5, vf6w\n" \
    "\tvmove.xyzw vf31, vf0\n" \
    "\t.set reorder" \
    : : : "memory")

/* From src/smt3/code_002BD340.c. */
#define VU0_SEQ_EULER_ANGLE_AXIS_Y(rotatedZ, rotatedX) __asm__ volatile ( \
    ".set noreorder\n" \
    "\tqmfc2.ni $2, vf10\n" \
    "\tpexew $2, $2\n" \
    "\tmtc1 $2, %0\n" \
    "\tqmfc2.ni $2, vf10\n" \
    "\tmtc1 $2, %1\n" \
    "\t.set reorder" \
    : "=f" (rotatedZ), "=f" (rotatedX) : : "$2", "memory")

/* ---- sequences that units wrote out in place ---- */

/* From src/smt3/model/mdlManager.c. */
#define VU0_SEQ_MDL_PACK_DEV_SLOT_COLOR(age, unit) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "qmtc2.ni %1, vf2\n\t" \
    "vitof0.xyzw vf2, vf2\n\t" \
    "vmulaw.xyzw ACC, vf10, vf0w\n\t" \
    "vmaddx.xyzw vf2, vf11, vf2x\n\t" \
    "vmul.xyzw vf10, vf2, vf12\n\t" \
    "qmtc2.ni %2, vf2\n\t" \
    "vmulx.xyzw vf10, vf10, vf2x\n\t" \
    "vftoi0.xyzw vf10, vf10\n\t" \
    "qmfc2.ni %0, vf10\n\t" \
    "ppach %0, $0, %0\n\t" \
    "ppacb %0, $0, %0\n\t" \
    ".set reorder" \
    : "=&r"(rgba[0]) \
    : "r"(age), "r"(unit))

/* ee-gcc 2.96 promotes a float argument through its own runtime helper, which
 * it calls fptodp; retail resolved that name to the routine the IDB calls
 * sdfFloatToULong. The equate is an assembler alias to an extern, so .globl
 * cannot share it: every unit that passes a float to a varargs call needs its
 * own, after declaring sdfFloatToULong. */
#define EQUATE_FPTODP() __asm__("fptodp = sdfFloatToULong")

#endif /* PS2ASM_H */
