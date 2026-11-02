#ifndef FPU_H
#define FPU_H

/* NATIVE_FPU switches every helper below from MIPS COP1 inline asm to a
 * host-compilable replacement body, for a compiler that cannot assemble EE
 * instructions. The retail build never defines it, so it keeps taking the
 * asm exactly as before. The replacements reproduce the EE FPU's actual
 * single-precision behaviour rather than plain IEEE 754: no denormals (an
 * input or result with that bit pattern flushes to a same-signed zero) and
 * no infinities or NaNs (a maximum-exponent bit pattern saturates to
 * +/-Fmax, the largest finite float, on read, and an overflowing result
 * saturates the same way on write). Established from PCSX2's EE FPU
 * interpreter, pcsx2/FPU.cpp (fpuDouble, checkOverflow, checkUnderflow,
 * SQRT_S, RSQRT_S, ABS_S, CVT_W): the usual software reference for
 * undocumented corners of this hardware. Calling the host's plain IEEE
 * sqrtf/fabsf or a plain cast instead would be wrong on denormal, zero,
 * negative-root and out-of-range inputs. */
#ifdef NATIVE_FPU
#include <math.h>
#include <stdint.h>

#define EE_FPU_SIGN 0x80000000u
#define EE_FPU_EXP  0x7F800000u
#define EE_FPU_FMAX 0x7F7FFFFFu /* FLT_MAX's bit pattern */

typedef union { float f; uint32_t u; } EeFpuWord;

/* Flush a zero/denormal word to a same-signed zero and clamp a
 * maximum-exponent ("would be Inf/NaN") word to +/-Fmax; a normal value
 * passes through untouched. Matches PCSX2's fpuDouble(). */
static inline float EeFpuSanitize(uint32_t bits)
{
    EeFpuWord w;
    uint32_t exp = bits & EE_FPU_EXP;
    if (exp == 0)
        w.u = bits & EE_FPU_SIGN;
    else if (exp == EE_FPU_EXP)
        w.u = (bits & EE_FPU_SIGN) | EE_FPU_FMAX;
    else
        w.u = bits;
    return w.f;
}

/* Saturate a computed result the way DIV_S/RSQRT_S do: a bit pattern that
 * reads as +/-Inf saturates to +/-Fmax, and a denormal result flushes to a
 * same-signed zero. Matches PCSX2's checkOverflow()/checkUnderflow(). */
static inline float EeFpuClampResult(float value)
{
    EeFpuWord w;
    w.f = value;
    uint32_t exp = w.u & EE_FPU_EXP;
    if (exp == EE_FPU_EXP)
        w.u = (w.u & EE_FPU_SIGN) | EE_FPU_FMAX;
    else if (exp == 0 && (w.u & 0x007FFFFFu) != 0)
        w.u &= EE_FPU_SIGN;
    return w.f;
}
#endif /* NATIVE_FPU */

/* DDS's scalar FPU convention: ee-gcc 2.96 normally calls the errno-checking
 * sqrtf library entry. Even with -fno-math-errno its sqrt.s has hazard nops
 * absent from retail. Keep the bare instruction shared, rather than adding
 * per-unit wrappers. See DDS docs/idioms.md, "Square roots: sqrt.s".
 * SMT3 users include effPCPBlurScreenOffset and the quaternion routines. */
#ifndef NATIVE_FPU
static inline float fsqrtf(float x)
{
    float result;
    __asm__(".set noreorder\n\tsqrt.s %0, %1\n\t.set reorder"
            : "=f" (result) : "f" (x));
    return result;
}
#else
/* sqrt.s: a zero or denormal operand returns a same-signed zero. A negative
 * operand sets the FPU's Invalid flag (not modeled here: nothing in this
 * codebase reads FCR31 back through these helpers) and returns sqrt of the
 * magnitude, never a NaN, since the EE has no NaN representation. See
 * PCSX2's SQRT_S. No overflow/underflow clamp follows: sqrt of any value in
 * [FLT_MIN, Fmax] stays within that range. */
static inline float fsqrtf(float x)
{
    EeFpuWord in;
    in.f = x;
    if ((in.u & EE_FPU_EXP) == 0) {
        EeFpuWord zero;
        zero.u = in.u & EE_FPU_SIGN;
        return zero.f;
    }
    if (in.u & EE_FPU_SIGN)
        return sqrtf(fabsf(EeFpuSanitize(in.u)));
    return sqrtf(EeFpuSanitize(in.u));
}
#endif

/* EE numerator / sqrt(x): gcc does not combine a square root and division
 * into rsqrt.s. SMT3 LookAt uses this operation, as do the DDS quaternion
 * normalizers. */
#ifndef NATIVE_FPU
static inline float frsqrtf(float numerator, float x)
{
    float result;
    __asm__(".set noreorder\n\trsqrt.s %0, %1, %2\n\t.set reorder"
            : "=f" (result) : "f" (numerator), "f" (x));
    return result;
}
#else
/* rsqrt.s computes numerator / sqrt(|x|) at full precision; it is not a
 * fast approximate reciprocal square root like x86's rsqrtss, so a native
 * division and sqrtf reproduce it rather than an approximation. A zero or
 * denormal x saturates to +/-Fmax, signed by x's own sign bit (not the
 * numerator's) regardless of the numerator; a negative x sets Invalid and
 * uses sqrt(|x|); the quotient is then clamped the way any divide result
 * is. See PCSX2's RSQRT_S. */
static inline float frsqrtf(float numerator, float x)
{
    EeFpuWord ft;
    float root;
    EeFpuWord fs;
    ft.f = x;
    if ((ft.u & EE_FPU_EXP) == 0) {
        EeFpuWord sat;
        sat.u = (ft.u & EE_FPU_SIGN) | EE_FPU_FMAX;
        return sat.f;
    }
    root = (ft.u & EE_FPU_SIGN) ? sqrtf(fabsf(EeFpuSanitize(ft.u)))
                                 : sqrtf(EeFpuSanitize(ft.u));
    fs.f = numerator;
    return EeFpuClampResult(EeFpuSanitize(fs.u) / root);
}
#endif

/* Preserve the SDK inline-asm scheduling boundary around abs.s. Independent
 * constant loads and argument moves may cross builtin fabsf calls, but stay
 * ordered around this helper. */
#ifndef NATIVE_FPU
static inline float ffabsf(float x)
{
    float result;
    __asm__(".set noreorder\n\tabs.s %0, %1\n\t.set reorder"
            : "=f" (result) : "f" (x));
    return result;
}
#else
/* abs.s is a bare sign-bit clear, not an arithmetic absolute value: unlike
 * every other EE FPU op, it never flushes a denormal or clamps a
 * maximum-exponent bit pattern. A plain IEEE fabsf is bit-identical for a
 * normal value, but could differ under a host FPU mode that flushes
 * denormal operands (DAZ); mask the bit directly instead, so a denormal
 * input passes through unchanged exactly as ABS_S leaves it. See PCSX2's
 * ABS_S. */
static inline float ffabsf(float x)
{
    EeFpuWord w;
    w.f = x;
    w.u &= ~EE_FPU_SIGN;
    return w.f;
}
#endif

/* Convert using the EE FPU's current rounding mode. */
#ifndef NATIVE_FPU
static inline int fcvtws(float value)
{
    float bits;
    int result;
    __asm__ volatile ("cvt.w.s %0, %1" : "=f" (bits) : "f" (value));
    __asm__ volatile ("mfc1 %0, %1" : "=r" (result) : "f" (bits));
    return result;
}
#else
/* cvt.w.s always truncates toward zero, ignoring FCR31's rounding-mode
 * field entirely (unlike plain MIPS, where CVT.W.S obeys it); a plain C
 * float-to-int cast already truncates toward zero, so it reproduces that
 * part exactly. A magnitude at or above 2^31 saturates to INT32_MAX or
 * INT32_MIN by sign instead of wrapping, which also covers a bit pattern
 * that looks like +Inf/NaN (exponent field all ones). mfc1 then moves that
 * 32-bit result unchanged into the integer register, which the plain `int`
 * return already represents. See PCSX2's CVT_W. */
static inline int fcvtws(float value)
{
    EeFpuWord w;
    w.f = value;
    if ((w.u & EE_FPU_EXP) <= 0x4E800000u)
        return (int)value;
    return (w.u & EE_FPU_SIGN) ? INT32_MIN : INT32_MAX;
}
#endif

#endif /* FPU_H */
