#ifndef FPU_H
#define FPU_H

/* NATIVE_FPU switches every helper below from MIPS COP1 inline asm to a
 * host-compilable replacement body, for a compiler that cannot assemble EE
 * instructions. The retail build never defines it, so it keeps taking the
 * asm exactly as before. The replacements reproduce the EE FPU's actual
 * single-precision behaviour rather than plain IEEE 754: a zero or denormal
 * bit pattern (exponent field 0) reads as a signed zero, and a maximum
 * exponent field (254 or 255) is NOT infinity or NaN: the EE has no such
 * reserved encoding, and reads it as an ordinary, very large finite value
 * through the plain (-1)^sign * (1+mantissa/2^23) * 2^(exp-127) formula,
 * extended through exponent 255.
 *
 * On real hardware, a divide-by-zero or an overflowing result lands on the
 * bit pattern 0x7FFFFFFF/0xFFFFFFFF (measured in the ps2autotests EE FPU and
 * vu0_macro suites). This header does NOT reproduce that bit pattern: on the
 * host, exponent field 255 with a nonzero mantissa is a quiet NaN, not a
 * large finite number, so storing those bits in a `float` and later doing
 * ordinary arithmetic on it (a multiply, say) would poison the computation
 * with a host NaN where the EE gets a finite result. Clamping to
 * +/-FLT_MAX instead (exponent 254, the largest exponent that still reads
 * as an ordinary finite value on the host) keeps arithmetic on the result
 * correct; the cost is that code which inspects the result's raw bits
 * instead of computing with it sees FLT_MAX's bits, not hardware's. See
 * docs/fpu-native-semantics.md for where that cost is paid and why it was
 * chosen anyway. An underflowing result flushes to a signed zero, which
 * matches hardware with no such tradeoff.
 *
 * This was first written from PCSX2's EE FPU interpreter (pcsx2/FPU.cpp),
 * which clamps a maximum-exponent operand to +/-FLT_MAX before computing and
 * clamps an overflowing result the same way. Real-hardware results (the
 * ps2autotests EE FPU suite) show PCSX2's operand clamp is not
 * hardware-accurate (see EeFpuLiteralValue below) on every site except
 * abs.s and cvt.w.s, even though its result clamp target (FLT_MAX) is kept
 * here, for the host-safety reason above rather than because it matches
 * hardware's own bit pattern. See docs/fpu-native-semantics.md. */
#ifdef NATIVE_FPU
#include <math.h>
#include <stdint.h>

#define EE_FPU_SIGN 0x80000000u
#define EE_FPU_EXP  0x7F800000u
#define EE_FPU_FMAX 0x7F7FFFFFu /* FLT_MAX's bit pattern */

typedef union { float f; uint32_t u; } eeFpuWord_t;

/* Read a 32-bit EE float bit pattern as a double, the literal way the
 * hardware does: exponent field 0 is a signed zero (no denormals), and any
 * other exponent (up to and including 255) is 2^(exp-127) times the
 * mantissa, with no reserved Inf/NaN encoding. A plain `float` local variable
 * cannot hold this for exponent 255, since the host FPU would treat that bit
 * pattern as infinity or NaN and propagate it through any arithmetic; this
 * stays in double and decodes the fields by hand instead. */
static inline double EeFpuLiteralValue(uint32_t bits)
{
    uint32_t exp = (bits & EE_FPU_EXP) >> 23;
    double value;
    if (exp == 0)
        value = 0.0;
    else
        value = ldexp(1.0 + (double)(bits & 0x007FFFFFu) / 8388608.0,
                       (int)exp - 127);
    return (bits & EE_FPU_SIGN) ? -value : value;
}

/* Encode a double back into a host-safe 32-bit float: clamp a magnitude that
 * would need exponent field 255 or past it to +/-FLT_MAX instead (hardware
 * would keep going through exponent 255 and then its own divide-by-zero
 * sentinel; see the file comment above for why this header stops one
 * exponent early), and flush a magnitude too small to represent to a
 * signed zero. The EE has no denormals, so there is nothing representable
 * between zero and the smallest normal value; a magnitude just below the
 * smallest normal only rounds up to it if the ordinary 23-bit mantissa
 * rounding below carries all the way out, the same rounding the normal
 * path already does one exponent higher. */
static inline uint32_t EeFpuEncodeExtended(double value)
{
    uint32_t sign;
    double mag, mant;
    int exp, biased;
    uint32_t mantissa_bits;

    if (value == 0.0)
        return signbit(value) ? EE_FPU_SIGN : 0u;

    sign = (value < 0.0) ? EE_FPU_SIGN : 0u;
    mag = fabs(value);
    mant = frexp(mag, &exp); /* mag == mant * 2^exp, 0.5 <= mant < 1 */
    biased = (exp - 1) + 127;
    if (biased >= 255)
        return sign | EE_FPU_FMAX;
    if (biased < 0)
        return sign;

    mantissa_bits = (uint32_t)nearbyint((mant * 2.0 - 1.0) * 8388608.0);
    if (mantissa_bits >= 8388608u) {
        mantissa_bits -= 8388608u;
        biased += 1;
        if (biased >= 255)
            return sign | EE_FPU_FMAX;
    }
    /* biased == 0 has no denormal storage on the EE, so it is only
     * reachable here through the carry above (mantissa_bits rounded all
     * the way up to the next exponent); anything that did not carry out
     * of this band flushes to zero, even though it is mathematically
     * closer to the smallest normal than to zero. */
    if (biased == 0)
        return sign;
    return sign | ((uint32_t)biased << 23) | mantissa_bits;
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
/* sqrt.s: a zero or denormal operand always returns POSITIVE zero, sign
 * dropped (measured on hardware; PCSX2's model keeps the operand's sign
 * here, which this file originally copied and real hardware contradicts).
 * A negative operand sets the FPU's Invalid flag (not modeled here: nothing
 * in this codebase reads FCR31 back through these helpers) and returns sqrt
 * of the magnitude, never a NaN, since the EE has no NaN representation.
 * A maximum-exponent operand is read at its literal extended value, not
 * clamped to Fmax; its square root always stays within the encodable range,
 * so no result clamp is needed. See docs/fpu-native-semantics.md. */
static inline float fsqrtf(float x)
{
    eeFpuWord_t in, out;
    in.f = x;
    if ((in.u & EE_FPU_EXP) == 0) {
        out.u = 0;
        return out.f;
    }
    out.u = EeFpuEncodeExtended(sqrt(fabs(EeFpuLiteralValue(in.u))));
    return out.f;
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
 * division and sqrt reproduce it rather than an approximation. On hardware,
 * a zero or denormal x (the sqrt operand, divide-by-zero) returns the bit
 * pattern 0x7FFFFFFF/0xFFFFFFFF, signed by the NUMERATOR's sign bit, not
 * x's, regardless of the numerator's own magnitude (measured in
 * sqrt.expected; PCSX2's model signs this by x and clamps to +/-Fmax
 * instead, which this file originally copied from PCSX2 before that was
 * checked against hardware). This header returns +/-FLT_MAX here instead of
 * those bits, for the same host-NaN-collision reason as EeFpuEncodeExtended
 * above: see the file comment. A negative x sets Invalid and uses
 * sqrt(|x|). Both operands are read at their literal extended value with no
 * clamp; an underflowing quotient flushes to a signed zero. See
 * docs/fpu-native-semantics.md. */
static inline float frsqrtf(float numerator, float x)
{
    eeFpuWord_t ft, fs, out;
    double root, num;
    ft.f = x;
    fs.f = numerator;
    if ((ft.u & EE_FPU_EXP) == 0) {
        out.u = (fs.u & EE_FPU_SIGN) ? (EE_FPU_SIGN | EE_FPU_FMAX) : EE_FPU_FMAX;
        return out.f;
    }
    root = sqrt(fabs(EeFpuLiteralValue(ft.u)));
    num = EeFpuLiteralValue(fs.u);
    out.u = EeFpuEncodeExtended(num / root);
    return out.f;
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
 * input passes through unchanged exactly as ABS_S leaves it. Confirmed
 * against the ps2autotests hardware corpus with no exceptions; this is one
 * of the two sites (with cvt.w.s) where PCSX2's model already matches real
 * hardware. */
static inline float ffabsf(float x)
{
    eeFpuWord_t w;
    w.f = x;
    w.u &= ~EE_FPU_SIGN;
    return w.f;
}
#endif

/* EE conversion truncates toward zero and saturates out-of-range inputs;
 * see docs/fpu-native-semantics.md. */
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
 * return already represents. Confirmed against the ps2autotests hardware
 * corpus (both convert.expected and edge.expected) with no exceptions: the
 * threshold is a plain magnitude check, so it saturates an exponent-255
 * operand correctly without needing to special-case it. This is one of the
 * two sites (with abs.s) where PCSX2's model already matches real
 * hardware. */
static inline int fcvtws(float value)
{
    eeFpuWord_t w;
    w.f = value;
    if ((w.u & EE_FPU_EXP) <= 0x4E800000u)
        return (int)value;
    return (w.u & EE_FPU_SIGN) ? INT32_MIN : INT32_MAX;
}
#endif

#endif /* FPU_H */
