#include "common.h"

extern f32 __kernel_cosf(f32 x, f32 tail);
extern f32 __kernel_sinf(f32 x, f32 tail, s32 reduced);
extern s32 __ieee754_rem_pio2f(f32 x, f32 *remainder);

/* NON_MATCHING: native size; fifteen initial bit-extraction and
 * scheduling words differ; quadrant dispatch is exact. */
#ifdef NON_MATCHING
f32 cosf(f32 x)
{
    f32 y[2];
    s32 magnitude;
    s32 quadrant;

    {
        union { f32 value; u32 bits; } input;
        input.value = x;
        magnitude = input.bits;
    }
    magnitude &= 0x7FFFFFFF;
    if (magnitude <= 0x3F490FD8) {
        return __kernel_cosf(x, 0.0f);
    }
    quadrant = __ieee754_rem_pio2f(x, y);
    switch (quadrant & 3) {
    case 0: return __kernel_cosf(y[0], y[1]);
    case 1: return -__kernel_sinf(y[0], y[1], 1);
    case 2: return -__kernel_cosf(y[0], y[1]);
    default: return __kernel_sinf(y[0], y[1], 1);
    }
}
#else
INCLUDE_ASM(const s32, "sdk/libm", cosf);
#endif

/* NON_MATCHING: native size; fifteen initial bit-extraction and
 * scheduling words differ; quadrant dispatch is exact. */
#ifdef NON_MATCHING
f32 sinf(f32 x)
{
    f32 y[2];
    s32 magnitude;
    s32 quadrant;

    {
        union { f32 value; u32 bits; } input;
        input.value = x;
        magnitude = input.bits;
    }
    magnitude &= 0x7FFFFFFF;
    if (magnitude <= 0x3F490FD8) {
        return __kernel_sinf(x, 0.0f, 0);
    }
    quadrant = __ieee754_rem_pio2f(x, y);
    switch (quadrant & 3) {
    case 0: return __kernel_sinf(y[0], y[1], 1);
    case 1: return __kernel_cosf(y[0], y[1]);
    case 2: return -__kernel_sinf(y[0], y[1], 1);
    default: return -__kernel_cosf(y[0], y[1]);
    }
}
#else
INCLUDE_ASM(const s32, "sdk/libm", sinf);
#endif

INCLUDE_ASM(const s32, "sdk/libm", tanf);

INCLUDE_ASM(const s32, "sdk/libm", __ieee754_acosf);

INCLUDE_ASM(const s32, "sdk/libm", __ieee754_atan2f);

INCLUDE_ASM(const s32, "sdk/libm", __ieee754_rem_pio2f);

/* NON_MATCHING: 34 of 51 words differ (first at +0x4). */
#ifdef NON_MATCHING
f32 __ieee754_sqrtf(f32 x)
{
    union { f32 f; s32 i; } input;
    union { f32 f; s32 i; } output;
    s32 mantissa;
    s32 exponent;
    s32 partial;
    s32 root;
    s32 trial;
    u32 bit;

    input.f = x;
    mantissa = input.i;
    if ((u32)(mantissa & 0x7FFFFFFF) <= 0x007FFFFF) {
        return x;
    }
    if (mantissa < 0) {
        return (x - x) / (x - x);
    }
    exponent = (mantissa >> 23) - 127;
    mantissa = (mantissa & 0x007FFFFF) | 0x00800000;
    mantissa <<= exponent & 1;
    exponent >>= 1;
    mantissa <<= 1;
    partial = 0;
    root = 0;
    bit = 0x01000000;
    do {
        trial = partial + bit;
        if (trial <= mantissa) {
            mantissa -= trial;
            partial = trial + bit;
            root += bit;
        }
        bit >>= 1;
        mantissa <<= 1;
    } while (bit != 0);
    if (mantissa != 0) {
        root += root & 1;
    }
    output.i = (root >> 1) + 0x3F000000U + ((u32)exponent << 23);
    return output.f;
}
#else
INCLUDE_ASM(const s32, "sdk/libm", __ieee754_sqrtf);
#endif

INCLUDE_ASM(const s32, "sdk/libm", __kernel_cosf);

INCLUDE_ASM(const s32, "sdk/libm", __kernel_rem_pio2f);

INCLUDE_ASM(const s32, "sdk/libm", __kernel_sinf);

INCLUDE_ASM(const s32, "sdk/libm", __kernel_tanf);

INCLUDE_RODATA(const s32, "sdk/libm", D_0054AF60);

INCLUDE_RODATA(const s32, "sdk/libm", D_0054B274);

INCLUDE_RODATA(const s32, "sdk/libm", D_0054B348);

INCLUDE_RODATA(const s32, "sdk/libm", D_0054B358);

INCLUDE_RODATA(const s32, "sdk/libm", D_0054B3C0);

INCLUDE_RODATA(const s32, "sdk/libm", D_0054B3F8);

INCLUDE_ASM(const s32, "sdk/libm", atanf);

INCLUDE_ASM(const s32, "sdk/libm", fabsf);

#define GET_FLOAT_WORD(i, d)                    \
    do {                                        \
        union { f32 f; u32 u; } gf_u;             \
        gf_u.f = (d);                           \
        (i) = (s32)gf_u.u;                      \
    } while (0)

#define SET_FLOAT_WORD(d, i)                    \
    do {                                        \
        union { f32 f; u32 u; } sf_u;             \
        sf_u.u = (u32)(i);                      \
        (d) = sf_u.f;                           \
    } while (0)

f32 floorf(f32 x)
{
    s32 word;
    u32 magnitude;
    s32 exponent;
    u32 fractionMask;

    GET_FLOAT_WORD(word, x);
    magnitude = (u32)(word & 0x7FFFFFFF);
    exponent = (s32)(magnitude >> 23) - 127;

    if (exponent < 23) {
        if (exponent < 0) {
            if (0.0f < (x + 1.0e30f)) {
                if (word >= 0) {
                    word = 0;
                } else if (magnitude > 0x007FFFFFU) {
                    word = (s32)0xBF800000U;
                }
            }
        } else {
            fractionMask = 0x007FFFFF >> exponent;
            if ((word & fractionMask) == 0) {
                return x;
            }
            if (0.0f < (x + 1.0e30f)) {
                if (word < 0) {
                    word += (s32)0x00800000U >> exponent;
                }
                word &= ~fractionMask;
            }
        }
        SET_FLOAT_WORD(x, word);
        return x;
    }
    return x;
}

extern f32 copysignf(f32 magnitude, f32 sign);
f32 scalbnf(f32 x, s32 n)
{
    s32 word;
    f32 scale;
    s32 exponent;

    GET_FLOAT_WORD(word, x);
    exponent = (s32)(((u32)(word & 0x7FFFFFFF)) >> 23);
    if (((u32)(word & 0x7FFFFFFF)) <= 0x007FFFFFU) {
        return x;
    }
    exponent = (s32)((u32)exponent + (u32)n);

    if (exponent >= 0x100) {
        scale = 1.0e30f;
        return copysignf(scale, x) * scale;
    }
    if (exponent > 0) {
        SET_FLOAT_WORD(x, ((u32)word & 0x807FFFFFU) | ((u32)exponent << 23));
        return x;
    }

    if (n > 50000) {
        return copysignf(1.0e30f, x) * 1.0e30f;
    }
    return copysignf(1.0e-30f, x) * 1.0e-30f;
}

INCLUDE_ASM(const s32, "sdk/libm", copysignf);


#undef GET_FLOAT_WORD
#undef SET_FLOAT_WORD

INCLUDE_RODATA(const s32, "sdk/libm", D_0054B408);

INCLUDE_RODATA(const s32, "sdk/libm", D_0054B414);

INCLUDE_RODATA(const s32, "sdk/libm", D_0054B418);

