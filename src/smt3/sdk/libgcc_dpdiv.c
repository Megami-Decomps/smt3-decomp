#include "common.h"
#include "sdk_libgcc.h"

typedef struct fp_number_type_df {
    s32 class;
    s32 sign;
    s32 normal_exp;
    long fraction;
} fp_number_type_df;

extern void sdfUnpackDouble(const double *value, fp_number_type_df *parts);
extern f64 __pack_df(const fp_number_type_df *parts);
extern const fp_number_type_df D_0054B460;

static inline s32 dfIsNaN(const fp_number_type_df *number)
{
    return (u32)number->class < 2;
}

static inline s32 dfIsZero(const fp_number_type_df *number)
{
    return number->class == 2;
}

static inline s32 dfIsInfinity(const fp_number_type_df *number)
{
    return number->class == 4;
}

/* Parts-level division is an actual fp-bit helper; see sdk-matching.md. */
static inline const fp_number_type_df *dfDivideParts(fp_number_type_df *numerator,
                                                    fp_number_type_df *denominator)
{
    u64 remainder;
    u64 divisor;
    u64 quotient;
    u64 bit;

    if (dfIsNaN(numerator)) return numerator;
    if (dfIsNaN(denominator)) return denominator;
    numerator->sign ^= denominator->sign;
    if (dfIsInfinity(numerator) || dfIsZero(numerator)) {
        if (numerator->class == denominator->class) return &D_0054B460;
        return numerator;
    }
    if (dfIsInfinity(denominator)) {
        numerator->fraction = 0;
        numerator->normal_exp = 0;
        return numerator;
    }
    if (dfIsZero(denominator)) {
        numerator->class = 4;
        return numerator;
    }
    numerator->normal_exp -= denominator->normal_exp;
    remainder = (u64)numerator->fraction;
    divisor = (u64)denominator->fraction;
    if (remainder < divisor) {
        numerator->normal_exp--;
        remainder <<= 1;
    }
    bit = 1ULL << 60;
    quotient = 0;
    while (bit != 0) {
        u64 nextRemainder;

        if (remainder >= divisor) {
            remainder -= divisor;
            quotient |= bit;
        }
        nextRemainder = remainder << 1;
        bit >>= 1;
        remainder = nextRemainder;
    }
    numerator->fraction = quotient;
    return numerator;
}

f64 dpdiv(f64 a, f64 b)
{
    fp_number_type_df numerator;
    fp_number_type_df denominator;
    const fp_number_type_df *result;

    sdfUnpackDouble(&a, &numerator);
    sdfUnpackDouble(&b, &denominator);
    result = dfDivideParts(&numerator, &denominator);
    return __pack_df(result);
}
