#include "common.h"

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

extern f32 __kernel_cosf(f32 x, f32 tail);
extern f32 __kernel_sinf(f32 x, f32 tail, s32 reduced);
extern f32 __kernel_tanf(f32 x, f32 tail, s32 reciprocal);
extern s32 __ieee754_rem_pio2f(f32 x, f32 *remainder);

f32 cosf(f32 x)
{
    f32 y[2];
    s32 magnitude;
    s32 quadrant;

    GET_FLOAT_WORD(magnitude, x);
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

f32 sinf(f32 x)
{
    f32 y[2];
    s32 magnitude;
    s32 quadrant;

    GET_FLOAT_WORD(magnitude, x);
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

f32 tanf(f32 x)
{
    f32 y[2];
    s32 magnitude;
    s32 quadrant;

    GET_FLOAT_WORD(magnitude, x);
    magnitude &= 0x7FFFFFFF;
    if (magnitude <= 0x3F490FDA) {
        return __kernel_tanf(x, 0.0f, 1);
    }
    quadrant = __ieee754_rem_pio2f(x, y);
    return __kernel_tanf(y[0], y[1], 1 - ((quadrant & 1) << 1));
}

INCLUDE_ASM(const s32, "sdk/libm", __ieee754_acosf);

extern f32 fabsf(f32 x);
extern f32 atanf(f32 x);

f32 __ieee754_atan2f(f32 y, f32 x)
{
    s32 hx;
    s32 hy;
    s32 ix;
    s32 iy;
    s32 quadrant;
    s32 exponentDifference;
    f32 angle;

    GET_FLOAT_WORD(hx, x);
    ix = hx & 0x7FFFFFFF;
    GET_FLOAT_WORD(hy, y);
    iy = hy & 0x7FFFFFFF;

    if (hx == 0x3F800000) {
        return atanf(y);
    }

    quadrant = (s32)(((u32)hy >> 31) | ((hx >> 30) & 2));
    if (iy <= 0x007FFFFF) {
        switch (quadrant) {
        case 0:
        case 1:
            return y;
        case 2:
            return 3.1415926f;
        case 3:
            return -3.1415926f;
        }
    }

    if (ix <= 0x007FFFFF) {
        return hy < 0
                   ? -1.5707963f
                   : 1.5707963f;
    }

    exponentDifference = (iy - ix) >> 23;
    if (exponentDifference >= 61) {
        angle = 1.5707963f;
    } else if (hx < 0 && exponentDifference < -60) {
        angle = 0.0f;
    } else {
        angle = atanf(fabsf(y / x));
    }

    switch (quadrant) {
    case 0:
        return angle;
    case 1: {
        s32 signWord;
        GET_FLOAT_WORD(signWord, angle);
        SET_FLOAT_WORD(angle, (u32)signWord ^ 0x80000000U);
        return angle;
    }
    case 2:
        return 3.1415926f - (angle - 1.5099578831723193e-7f);
    default:
        return (angle - 1.5099578831723193e-7f) - 3.1415926f;
    }
}

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

f32 __kernel_sinf(f32 x, f32 y, s32 useTail)
{
    s32 magnitude;
    f32 z, v, r;

    GET_FLOAT_WORD(magnitude, x);
    magnitude &= 0x7FFFFFFF;
    if (magnitude < 0x32000000) {
        if ((s32)x == 0)
            return x;
    }
    z = x * x;
    v = z * x;
    r = 0x1.1111120000000p-7f + z * (-0x1.a01a020000000p-13f + z * (0x1.71de340000000p-19f + z * (-0x1.ae5e680000000p-26f + z * 0x1.5d93a40000000p-33f)));
    if (useTail == 0)
        return x + v * (-0x1.5555540000000p-3f + z * r);
    return x - ((z * (0.5f * y - v * r) - y) - v * -0x1.5555540000000p-3f);
}

extern const f32 D_0054B3C0[13];

f32 __kernel_tanf(f32 x, f32 y, s32 selector)
{
    s32 originalWord;
    s32 magnitude;
    s32 reciprocalWord;
    s32 resultWord;
    f32 z, w, r, v, s, a, t;

    GET_FLOAT_WORD(originalWord, x);
    magnitude = originalWord & 0x7FFFFFFF;
    if (magnitude < 0x31800000) {
        if ((s32)x == 0) {
            if (((u32)magnitude | ((u32)selector + 1U)) == 0) {
                return 1.0f / fabsf(x);
            }
            return selector == 1 ? x : -1.0f / x;
        }
    }
    if (magnitude >= 0x3F2CA140) {
        if (originalWord < 0) {
            x = -x;
            y = -y;
        }
        z = 0x1.921fb40000000p-1f - x;
        w = 0x1.4442ce0000000p-25f - y;
        x = z + w;
        y = 0.0f;
    }
    z = x * x;
    w = z * z;
    r = D_0054B3C0[1] + w * (D_0054B3C0[3] + w *
        (D_0054B3C0[5] + w * (D_0054B3C0[7] + w *
        (D_0054B3C0[9] + w * D_0054B3C0[11]))));
    v = z * (D_0054B3C0[2] + w * (D_0054B3C0[4] + w *
        (D_0054B3C0[6] + w * (D_0054B3C0[8] + w *
        (D_0054B3C0[10] + w * D_0054B3C0[12])))));
    s = z * x;
    r = y + z * (s * (r + v) + y);
    r += D_0054B3C0[0] * s;
    w = x + r;
    if (magnitude >= 0x3F2CA140) {
        v = (f32)selector;
        return (f32)(1 - ((originalWord >> 30) & 2)) *
            (v - 2.0f * (x - ((w * w) / (w + v) - r)));
    }
    if (selector == 1) {
        return w;
    }
    /* Truncate the working tangent value before reciprocal compensation. */
    z = w;
    GET_FLOAT_WORD(resultWord, z);
    SET_FLOAT_WORD(z, resultWord & 0xFFFFF000);
    v = r - (z - x);
    a = -1.0f / w;
    GET_FLOAT_WORD(reciprocalWord, a);
    SET_FLOAT_WORD(t, reciprocalWord & 0xFFFFF000);
    s = t * z + 1.0f;
    return t + a * (s + t * v);
}


INCLUDE_RODATA(const s32, "sdk/libm", D_0054AF60);

INCLUDE_RODATA(const s32, "sdk/libm", D_0054B274);

INCLUDE_RODATA(const s32, "sdk/libm", D_0054B348);

INCLUDE_RODATA(const s32, "sdk/libm", D_0054B358);

INCLUDE_RODATA(const s32, "sdk/libm", D_0054B3C0);

INCLUDE_RODATA(const s32, "sdk/libm", D_0054B3F8);

INCLUDE_ASM(const s32, "sdk/libm", atanf);

f32 fabsf(f32 x)
{
    s32 word;
    GET_FLOAT_WORD(word, x);
    SET_FLOAT_WORD(x, (u32)word & 0x7FFFFFFFU);
    return x;
}

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

f32 copysignf(f32 magnitude, f32 sign)
{
    s32 magnitudeWord;
    s32 signWord;
    GET_FLOAT_WORD(magnitudeWord, magnitude);
    GET_FLOAT_WORD(signWord, sign);
    SET_FLOAT_WORD(magnitude, ((u32)magnitudeWord & 0x7FFFFFFFU) |
                             ((u32)signWord & 0x80000000U));
    return magnitude;
}


#undef GET_FLOAT_WORD
#undef SET_FLOAT_WORD

INCLUDE_RODATA(const s32, "sdk/libm", D_0054B408);

INCLUDE_RODATA(const s32, "sdk/libm", D_0054B414);

INCLUDE_RODATA(const s32, "sdk/libm", D_0054B418);

