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

extern f32 func_002CF760(f32 x);
extern f32 __ieee754_atan2f(f32 y, f32 x);

f32 __ieee754_acosf(f32 x)
{
    return func_002CF760(x);
}

f32 func_002CF748(f32 y, f32 x)
{
    return __ieee754_atan2f(y, x);
}

extern f32 __ieee754_sqrtf(f32 x);

f32 func_002CF760(f32 x)
{
    f32 z, p, q, r, w, s, c, df;
    s32 hx, ix;

    GET_FLOAT_WORD(hx, x);
    ix = hx & 0x7FFFFFFF;

    if (ix == 0x3F800000) {
        if (hx > 0) return 0.0f;
        return 3.141592502597e+00f;
    } else if (ix > 0x3F800000) {
        return (x - x) / (x - x);
    }
    if (ix <= 0x3EFFFFFF) {
        if (ix <= 0x23000000) return 1.570796251299e+00f;
        z = x * x;
        p = z * (1.666666567327e-01f
            + z * (-3.255657851699e-01f
            + z * (2.012125253679e-01f
            + z * (-4.005553573374e-02f
            + z * (7.915350142874e-04f
            + z * 3.479330916892e-05f)))));
        q = 1.0f + z * (-2.403394699099e+00f
            + z * (2.020945549013e+00f
            + z * (-6.882839202888e-01f
            + z * 7.703814655550e-02f)));
        r = p / q;
        return 1.570796251299e+00f - (x - (7.549789415869e-08f - x * r));
    } else if (hx < 0) {
        z = (1.0f + x) * 0.5f;
        p = z * (1.666666567327e-01f
            + z * (-3.255657851699e-01f
            + z * (2.012125253679e-01f
            + z * (-4.005553573374e-02f
            + z * (7.915350142874e-04f
            + z * 3.479330916892e-05f)))));
        q = 1.0f + z * (-2.403394699099e+00f
            + z * (2.020945549013e+00f
            + z * (-6.882839202888e-01f
            + z * 7.703814655550e-02f)));
        s = __ieee754_sqrtf(z);
        r = p / q;
        w = r * s - 7.549789415869e-08f;
        return 3.141592502597e+00f - 2.0f * (s + w);
    } else {
        s32 word;
        z = (1.0f - x) * 0.5f;
        s = __ieee754_sqrtf(z);
        df = s;
        GET_FLOAT_WORD(word, df);
        SET_FLOAT_WORD(df, word & 0xFFFFF000);
        c = (z - df * df) / (s + df);
        p = z * (1.666666567327e-01f
            + z * (-3.255657851699e-01f
            + z * (2.012125253679e-01f
            + z * (-4.005553573374e-02f
            + z * (7.915350142874e-04f
            + z * 3.479330916892e-05f)))));
        q = 1.0f + z * (-2.403394699099e+00f
            + z * (2.020945549013e+00f
            + z * (-6.882839202888e-01f
            + z * 7.703814655550e-02f)));
        r = p / q;
        w = r * s + c;
        return 2.0f * (df + w);
    }
}

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

extern const s32 D_0054AF60[198];
extern const s32 D_0054B278[32];
extern s32 __kernel_rem_pio2f(f32 *x, f32 *y, s32 e0, s32 nx, s32 precision,
                              const s32 *twoOverPi);

s32 __ieee754_rem_pio2f(f32 x, f32 *y)
{
    f32 z, w, t, r, fn;
    f32 tx[3];
    s32 i, j, n, ix, hx;
    s32 e0, nx;
    u32 high;

    GET_FLOAT_WORD(hx, x);
    ix = hx & 0x7FFFFFFF;
    if (ix <= 0x3F490FD8) {
        y[0] = x;
        y[1] = 0.0f;
        return 0;
    }

    if (ix <= 0x4016CBE3) {
        if (hx > 0) {
            z = x - 0x1.921f000000000p+0f;
            if ((ix & 0xFFFFFFF0) != 0x3FC90FD0) {
                y[0] = z - 0x1.6a88840000000p-17f;
                y[1] = (z - y[0]) - 0x1.6a88840000000p-17f;
            } else {
                z -= 0x1.6a87fe0000000p-17f;
                y[0] = z - 0x1.0b46100000000p-34f;
                y[1] = (z - y[0]) - 0x1.0b46100000000p-34f;
            }
            return 1;
        }
        z = x + 0x1.921f000000000p+0f;
        if ((ix & 0xFFFFFFF0) != 0x3FC90FD0) {
            y[0] = z + 0x1.6a88840000000p-17f;
            y[1] = (z - y[0]) + 0x1.6a88840000000p-17f;
        } else {
            z += 0x1.6a87fe0000000p-17f;
            y[0] = z + 0x1.0b46100000000p-34f;
            y[1] = (z - y[0]) + 0x1.0b46100000000p-34f;
        }
        return -1;
    }

    if (ix <= 0x43490F80) {
        t = fabsf(x);
        n = (s32)(t * 0x1.45f3080000000p-1f + 0x1.0000000000000p-1f);
        fn = (f32)n;
        r = t - fn * 0x1.921f000000000p+0f;
        w = fn * 0x1.6a88840000000p-17f;
        if (n < 32 && (ix & 0xFFFFFF00) != (u32)D_0054B278[n - 1]) {
            y[0] = r - w;
        } else {
            j = ix >> 23;
            y[0] = r - w;
            GET_FLOAT_WORD(high, y[0]);
            i = j - ((high >> 23) & 0xFF);
            if (i > 8) {
                t = r;
                w = fn * 0x1.6a87fe0000000p-17f;
                r = t - w;
                w = fn * 0x1.0b46100000000p-34f - ((t - r) - w);
                y[0] = r - w;
                GET_FLOAT_WORD(high, y[0]);
                i = j - ((high >> 23) & 0xFF);
                if (i > 25) {
                    t = r;
                    w = fn * 0x1.0b46000000000p-34f;
                    r = t - w;
                    w = fn * 0x1.1a62620000000p-54f - ((t - r) - w);
                    y[0] = r - w;
                }
            }
        }
        y[1] = (r - y[0]) - w;
        if (hx < 0) {
            y[0] = -y[0];
            y[1] = -y[1];
            return -n;
        }
        return n;
    }

    e0 = (ix >> 23) - 134;
    SET_FLOAT_WORD(z, ix - (e0 << 23));
    for (i = 0; i < 2; i++) {
        tx[i] = (f32)(s32)z;
        z = (z - tx[i]) * 0x1.0000000000000p+8f;
    }
    tx[2] = z;
    nx = 3;
    while (tx[nx - 1] == 0.0f) {
        nx--;
    }
    n = __kernel_rem_pio2f(tx, y, e0, nx, 2, D_0054AF60);
    if (hx < 0) {
        y[0] = -y[0];
        y[1] = -y[1];
        return -n;
    }
    return n;
}

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

f32 __kernel_cosf(f32 x, f32 tail)
{
    s32 magnitude;
    f32 z;
    f32 r;
    f32 qx;
    f32 hz;
    f32 a;

    GET_FLOAT_WORD(magnitude, x);
    magnitude &= 0x7FFFFFFF;
    if (magnitude <= 0x31FFFFFF) {
        if ((s32)x == 0) {
            return 1.0f;
        }
    }
    z = x * x;
    r = z * (0x1.555554p-5f + z * (-0x1.6c16c0p-10f +
        z * (0x1.a01a00p-16f + z * (-0x1.27e4f6p-22f +
        z * (0x1.1ee9eap-29f + z * -0x1.8fae9cp-37f)))));
    if (magnitude <= 0x3E999999) {
        return 1.0f - (0.5f * z - (z * r - x * tail));
    }
    if (magnitude > 0x3F480000) {
        qx = 0.28125f;
    } else {
        SET_FLOAT_WORD(qx, magnitude - 0x01000000);
    }
    hz = 0.5f * z - qx;
    a = 1.0f - qx;
    return a - (hz - (z * r - x * tail));
}

extern f32 floorf(f32 x);
extern f32 scalbnf(f32 x, s32 n);
extern const s32 D_0054B348[];
/* Readonly fragment also contains neighboring math constants. */
extern const f32 D_0054B358[];

s32 __kernel_rem_pio2f(f32 *x, f32 *y, s32 e0, s32 nx, s32 precision,
                       const s32 *ipio2)
{
    s32 jz, jx, jv, jp, jk, carry, n, iq[20], i, j, k, m, q0, ih;
    f32 z, fw, f[20], fq[20], q[20];

    jk = D_0054B348[precision];
    jp = jk;
    jx = nx - 1;
    jv = (e0 - 3) / 8;
    if (jv < 0) jv = 0;
    q0 = e0 - 8 * (jv + 1);

    j = jv - jx;
    m = jx + jk;
    for (i = 0; i <= m; i++, j++) {
        f[i] = j < 0 ? 0.0f : (f32)ipio2[j];
    }
    for (i = 0; i <= jk; i++) {
        fw = 0.0f;
        for (j = 0; j <= jx; j++) {
            fw += x[j] * f[jx + i - j];
        }
        q[i] = fw;
    }

    jz = jk;
recompute:
    z = q[jz];
    for (i = 0, j = jz; j > 0; i++, j--) {
        fw = (f32)(s32)(0x1.000000p-8f * z);
        iq[i] = (s32)(z - 256.0f * fw);
        z = q[j - 1] + fw;
    }

    z = scalbnf(z, q0);
    z -= 8.0f * floorf(z * 0.125f);
    n = (s32)z;
    z -= (f32)n;
    ih = 0;
    if (q0 > 0) {
        i = iq[jz - 1] >> (8 - q0);
        n += i;
        iq[jz - 1] -= i << (8 - q0);
        ih = iq[jz - 1] >> (7 - q0);
    } else if (q0 == 0) {
        ih = iq[jz - 1] >> 8;
    } else if (z >= 0.5f) {
        ih = 2;
    }

    if (ih > 0) {
        n++;
        carry = 0;
        for (i = 0; i < jz; i++) {
            j = iq[i];
            if (carry == 0) {
                if (j != 0) {
                    carry = 1;
                    iq[i] = 256 - j;
                }
            } else {
                iq[i] = 255 - j;
            }
        }
        if (q0 > 0) {
            switch (q0) {
            case 1: iq[jz - 1] &= 0x7F; break;
            case 2: iq[jz - 1] &= 0x3F; break;
            }
        }
        if (ih == 2) {
            z = 1.0f - z;
            if (carry != 0) z -= scalbnf(1.0f, q0);
        }
    }

    if (z == 0.0f) {
        j = 0;
        for (i = jz - 1; i >= jk; i--) j |= iq[i];
        if (j == 0) {
            for (k = 1; iq[jk - k] == 0; k++) {
            }
            for (i = jz + 1; i <= jz + k; i++) {
                f[jx + i] = (f32)ipio2[jv + i];
                fw = 0.0f;
                for (j = 0; j <= jx; j++) {
                    fw += x[j] * f[jx + i - j];
                }
                q[i] = fw;
            }
            jz += k;
            goto recompute;
        }
    }

    if (z == 0.0f) {
        jz--;
        q0 -= 8;
        while (iq[jz] == 0) {
            jz--;
            q0 -= 8;
        }
    } else {
        z = scalbnf(z, -q0);
        if (z >= 256.0f) {
            fw = (f32)(s32)(0x1.000000p-8f * z);
            iq[jz] = (s32)(z - 256.0f * fw);
            jz++;
            q0 += 8;
            iq[jz] = (s32)fw;
        } else {
            iq[jz] = (s32)z;
        }
    }

    fw = scalbnf(1.0f, q0);
    for (i = jz; i >= 0; i--) {
        q[i] = fw * (f32)iq[i];
        fw *= 0x1.000000p-8f;
    }
    for (i = jz; i >= 0; i--) {
        fw = 0.0f;
        for (k = 0; k <= jp && k <= jz - i; k++) {
            fw += D_0054B358[k] * q[i + k];
        }
        fq[jz - i] = fw;
    }

    switch (precision) {
    case 0:
        fw = 0.0f;
        for (i = jz; i >= 0; i--) fw += fq[i];
        y[0] = ih == 0 ? fw : -fw;
        break;
    case 1:
    case 2:
        fw = 0.0f;
        for (i = jz; i >= 0; i--) fw += fq[i];
        y[0] = ih == 0 ? fw : -fw;
        fw = fq[0] - fw;
        for (i = 1; i <= jz; i++) fw += fq[i];
        y[1] = ih == 0 ? fw : -fw;
        break;
    case 3:
        for (i = jz; i > 0; i--) {
            fw = fq[i - 1] + fq[i];
            fq[i] += fq[i - 1] - fw;
            fq[i - 1] = fw;
        }
        for (i = jz; i > 1; i--) {
            fw = fq[i - 1] + fq[i];
            fq[i] += fq[i - 1] - fw;
            fq[i - 1] = fw;
        }
        fw = 0.0f;
        for (i = jz; i >= 2; i--) fw += fq[i];
        if (ih == 0) {
            y[0] = fq[0];
            y[1] = fq[1];
            y[2] = fw;
        } else {
            y[0] = -fq[0];
            y[1] = -fq[1];
            y[2] = -fw;
        }
        break;
    }
    return n & 7;
}

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

INCLUDE_RODATA(const s32, "sdk/libm", D_0054B278);

INCLUDE_RODATA(const s32, "sdk/libm", D_0054B348);

INCLUDE_RODATA(const s32, "sdk/libm", D_0054B358);

INCLUDE_RODATA(const s32, "sdk/libm", D_0054B3C0);

INCLUDE_RODATA(const s32, "sdk/libm", D_0054B3F8);

INCLUDE_RODATA(const s32, "sdk/libm", D_0054B404);
extern const f32 D_0054B3F8[4];
extern const f32 D_0054B404;
extern const f32 D_0054B408[4];
extern const f32 D_0054B414;
extern const f32 D_0054B418[11];
f32 atanf(f32 x)
{
    s32 inputWord;
    s32 magnitude;
    s32 reduction;
    f32 z;
    f32 w;
    f32 even;
    f32 odd;
    f32 result;

    GET_FLOAT_WORD(inputWord, x);
    magnitude = inputWord & 0x7FFFFFFF;
    if (magnitude > 0x507FFFFF) {
        if (inputWord > 0) {
            return D_0054B404 + D_0054B414;
        }
        return -D_0054B404 - D_0054B414;
    }

    if (magnitude <= 0x3EDFFFFF) {
        if (magnitude < 0x31000000 && 1.0f < x + 1.0e30f) {
            return x;
        }
        reduction = -1;
    } else {
        x = fabsf(x);
        if (magnitude <= 0x3F97FFFF) {
            if (magnitude <= 0x3F2FFFFF) {
                reduction = 0;
                x = (2.0f * x - 1.0f) / (x + 2.0f);
            } else {
                reduction = 1;
                x = (x - 1.0f) / (x + 1.0f);
            }
        } else if (magnitude <= 0x401BFFFF) {
            reduction = 2;
            x = (x - 1.5f) / (1.5f * x + 1.0f);
        } else {
            reduction = 3;
            x = -1.0f / x;
        }
    }

    z = x * x;
    w = z * z;
    even = z * (D_0054B418[0] + w * (D_0054B418[2] + w * (D_0054B418[4] +
                  w * (D_0054B418[6] + w * (D_0054B418[8] +
                  w * D_0054B418[10])))));
    odd = w * (D_0054B418[1] + w * (D_0054B418[3] + w * (D_0054B418[5] +
                 w * (D_0054B418[7] + w * D_0054B418[9]))));

    if (reduction < 0) {
        return x - x * (even + odd);
    }
    result = D_0054B3F8[reduction] - ((x * (even + odd) - D_0054B408[reduction]) - x);
    if (inputWord < 0) {
        result = -result;
    }
    return result;
}

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

