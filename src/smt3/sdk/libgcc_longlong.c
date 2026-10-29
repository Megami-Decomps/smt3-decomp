#include "common.h"

INCLUDE_ASM(const s32, "sdk/libgcc_longlong", libc_2E52FC);

INCLUDE_ASM(const s32, "sdk/libgcc_longlong", __divdi3);

#ifdef NON_MATCHING
/* EE little-endian raw word view; arithmetic below converts each half to u32. */
typedef union {
    u64 value;
    struct { s32 low, high; } words;
} runtimeWide_t;

extern const u8 D_0054BF78[256];

static inline u32 LeadingZeroBits(u32 word)
{
    u32 shift;
    if (word < 0x10000) {
        shift = word < 0x100 ? 0 : 8;
    } else {
        shift = word < 0x1000000 ? 16 : 24;
    }
    return 32 - (D_0054BF78[word >> shift] + shift);
}

/* The historical longlong.h uses this multi-output arithmetic primitive.
 * A macro keeps quotient/remainder outputs scalar under this compiler.
 * Divide a two-word numerator by a normalized one-word divisor, estimating
 * and correcting each base-65536 quotient digit. */
#define DIVIDE_NORMALIZED(quotient, remainder, high, low, divisor) \
    do { \
        u32 divisorHigh = divisor >> 16; \
        u32 divisorLow = divisor & 0xFFFF; \
        u32 highDigit, lowDigit, highRemainder, lowRemainder, product; \
     \
        highDigit = high / divisorHigh; \
        highRemainder = high % divisorHigh; \
        product = highDigit * divisorLow; \
        highRemainder = (highRemainder << 16) | (low >> 16); \
        if (highRemainder < product) { \
            highDigit--; \
            highRemainder += divisor; \
            if (highRemainder >= divisor && highRemainder < product) { \
                highDigit--; \
                highRemainder += divisor; \
            } \
        } \
        highRemainder -= product; \
        lowDigit = highRemainder / divisorHigh; \
        lowRemainder = highRemainder % divisorHigh; \
        product = lowDigit * divisorLow; \
        lowRemainder = (lowRemainder << 16) | (low & 0xFFFF); \
        if (lowRemainder < product) { \
            lowDigit--; \
            lowRemainder += divisor; \
            if (lowRemainder >= divisor && lowRemainder < product) { \
                lowDigit--; \
                lowRemainder += divisor; \
            } \
        } \
        (remainder) = lowRemainder - product; \
        (quotient) = (highDigit << 16) | lowDigit; \
    } while (0)

#endif

#ifdef NON_MATCHING
/* Preserve the shared helper's optional remainder output. The wrapper
 * discards its quotient and returns the remainder written to its local. */
extern const u8 D_0054BE78[256];
extern const u8 D_0054C078[256];

static inline u32 RemainderLeadingZeroBits(u32 word, const u8 *bitLengths)
{
    u32 shift;
    if (word < 0x10000) {
        shift = word < 0x100 ? 0 : 8;
    } else {
        shift = word < 0x1000000 ? 16 : 24;
    }
    return 32 - (bitLengths[word >> shift] + shift);
}

static inline u64 DivideWithRemainder(u64 numerator, u64 denominator, u64 *remainder,
                                          const u8 *bitLengths)
{
    runtimeWide_t n, d, result, rest;
    u32 nHigh, nLow, dHigh, dLow, shift, inverse;
    u32 qHigh, qLow;

    n.value = numerator;
    d.value = denominator;
    dHigh = d.words.high;
    nHigh = n.words.high;
    dLow = d.words.low;
    nLow = n.words.low;
    if (dHigh == 0) {
        if (dLow > nHigh) {
            shift = RemainderLeadingZeroBits(dLow, bitLengths);
            if (shift != 0) {
                dLow <<= shift;
                nHigh = (nHigh << shift) | (nLow >> (32 - shift));
                nLow <<= shift;
            }
            DIVIDE_NORMALIZED(qLow, nHigh, nHigh, nLow, dLow);
            qHigh = 0;
        } else {
            if (dLow == 0) {
                dLow = 1 / dLow;
            }
            shift = RemainderLeadingZeroBits(dLow, bitLengths);
            if (shift == 0) {
                nHigh -= dLow;
                qHigh = 1;
            } else {
                u32 upper;
                inverse = 32 - shift;
                dLow <<= shift;
                upper = nHigh >> inverse;
                nHigh = (nHigh << shift) | (nLow >> inverse);
                nLow <<= shift;
                DIVIDE_NORMALIZED(qHigh, nHigh, upper, nHigh, dLow);
            }
            DIVIDE_NORMALIZED(qLow, nHigh, nHigh, nLow, dLow);
        }
        if (remainder != NULL) {
            rest.words.low = nHigh >> shift;
            rest.words.high = 0;
            *remainder = rest.value;
        }
    } else if (dHigh > nHigh) {
        qLow = 0;
        qHigh = 0;
        if (remainder != NULL) {
            rest.words.low = nLow;
            rest.words.high = nHigh;
            *remainder = rest.value;
        }
    } else {
        shift = RemainderLeadingZeroBits(dHigh, bitLengths);
        if (shift == 0) {
            if (nHigh > dHigh || nLow >= dLow) {
                u32 previousLow = nLow;
                qLow = 1;
                nLow -= dLow;
                nHigh = nHigh - dHigh - (previousLow < nLow);
            } else {
                qLow = 0;
            }
            if (remainder != NULL) {
                rest.words.low = nLow;
                rest.words.high = nHigh;
                *remainder = rest.value;
            }
        } else {
            u32 upper, productHigh, productLow;
            runtimeWide_t product;
            inverse = 32 - shift;
            dHigh = (dHigh << shift) | (dLow >> inverse);
            dLow <<= shift;
            upper = nHigh >> inverse;
            nHigh = (nHigh << shift) | (nLow >> inverse);
            nLow <<= shift;
            DIVIDE_NORMALIZED(qLow, nHigh, upper, nHigh, dHigh);
            product.value = (u64)qLow * dLow;
            productHigh = product.words.high;
            productLow = product.words.low;
            if (productHigh > nHigh ||
                (productHigh == nHigh && productLow > nLow)) {
                u32 previousLow = productLow;
                qLow--;
                productLow -= dLow;
                productHigh = productHigh - dHigh - (previousLow < productLow);
            }
            if (remainder != NULL) {
                u32 previousLow = nLow;
                nLow -= productLow;
                nHigh = nHigh - productHigh - (previousLow < nLow);
                rest.words.low = (nHigh << inverse) | (nLow >> shift);
                rest.words.high = nHigh >> shift;
                *remainder = rest.value;
            }
        }
        qHigh = 0;
    }
    result.words.low = qLow;
    result.words.high = qHigh;
    return result.value;
}

#endif

#ifdef NON_MATCHING
s64 __moddi3(s64 numerator, s64 denominator)
{
    runtimeWide_t n, d, remainder;
    s32 negative = 0;

    n.value = (u64)numerator;
    d.value = (u64)denominator;
    if (n.words.high < 0) {
        u32 negatedHigh = 0U - (u32)n.words.high;
        n.words.low = 0U - (u32)n.words.low;
        n.words.high = negatedHigh - ((u32)n.words.low != 0);
        negative = -1;
    }
    if (d.words.high < 0) {
        u32 negatedHigh = 0U - (u32)d.words.high;
        d.words.low = 0U - (u32)d.words.low;
        d.words.high = negatedHigh - ((u32)d.words.low != 0);
    }
    DivideWithRemainder(n.value, d.value, &remainder.value, D_0054BE78);
    if (negative != 0) {
        u32 negatedHigh = 0U - (u32)remainder.words.high;
        remainder.words.low = 0U - (u32)remainder.words.low;
        remainder.words.high = negatedHigh -
                               ((u32)remainder.words.low != 0);
    }
    return (s64)remainder.value;
}

#else
INCLUDE_ASM(const s32, "sdk/libgcc_longlong", __moddi3);
#endif

#ifdef NON_MATCHING
/* NON_MATCHING: 1424/1404 bytes; 74 of 351 native words match.
 * Widening-product word extraction and scalar allocation differ. */
u64 __udivdi3(u64 numerator, u64 denominator)
{
    runtimeWide_t n, d, result;
    u32 nHigh, nLow, dHigh, dLow, shift, inverse;
    u32 qHigh, qLow;
    n.value = numerator;
    d.value = denominator;
    dHigh = d.words.high;
    nHigh = n.words.high;
    dLow = d.words.low;
    nLow = n.words.low;
    if (dHigh == 0) {
        if (dLow > nHigh) {
            shift = LeadingZeroBits(dLow);
            if (shift != 0) {
                dLow <<= shift;
                nHigh = (nHigh << shift) | (nLow >> (32 - shift));
                nLow <<= shift;
            }
            DIVIDE_NORMALIZED(qLow, nHigh, nHigh, nLow, dLow);
            qHigh = 0;
        } else {
            if (dLow == 0) {
                /* Preserve the runtime's native divide-by-zero trap. */
                dLow = 1 / dLow;
            }
            shift = LeadingZeroBits(dLow);
            if (shift == 0) {
                nHigh -= dLow;
                qHigh = 1;
            } else {
                u32 upper;
                inverse = 32 - shift;
                dLow <<= shift;
                upper = nHigh >> inverse;
                nHigh = (nHigh << shift) | (nLow >> inverse);
                nLow <<= shift;
                DIVIDE_NORMALIZED(qHigh, nHigh, upper, nHigh, dLow);
            }
            DIVIDE_NORMALIZED(qLow, nHigh, nHigh, nLow, dLow);
        }
    } else if (dHigh > nHigh) {
        qLow = 0;
        qHigh = 0;
    } else {
        shift = LeadingZeroBits(dHigh);
        if (shift == 0) {
            if (nHigh > dHigh || nLow >= dLow) {
                qLow = 1;
            } else {
                qLow = 0;
            }
        } else {
            u32 upper, productHigh, productLow;
            runtimeWide_t product;
            inverse = 32 - shift;
            dHigh = (dHigh << shift) | (dLow >> inverse);
            dLow <<= shift;
            upper = nHigh >> inverse;
            nHigh = (nHigh << shift) | (nLow >> inverse);
            nLow <<= shift;
            DIVIDE_NORMALIZED(qLow, nHigh, upper, nHigh, dHigh);
            product.value = (u64)qLow * dLow;
            productHigh = product.words.high;
            productLow = product.words.low;
            if (productHigh > nHigh ||
                (productHigh == nHigh && productLow > nLow)) {
                qLow--;
            }
        }
        qHigh = 0;
    }
    result.words.low = qLow;
    result.words.high = qHigh;
    return result.value;
}

#else
INCLUDE_ASM(const s32, "sdk/libgcc_longlong", __udivdi3);
#endif

#ifdef NON_MATCHING
u64 __umoddi3(u64 numerator, u64 denominator)
{
    u64 remainder;
    DivideWithRemainder(numerator, denominator, &remainder, D_0054C078);
    return remainder;
}

#undef DIVIDE_NORMALIZED
#else
INCLUDE_ASM(const s32, "sdk/libgcc_longlong", __umoddi3);
#endif

INCLUDE_RODATA(const s32, "sdk/libgcc_longlong", D_0054BD78);

INCLUDE_RODATA(const s32, "sdk/libgcc_longlong", D_0054BE78);

INCLUDE_RODATA(const s32, "sdk/libgcc_longlong", D_0054BF78);

INCLUDE_RODATA(const s32, "sdk/libgcc_longlong", D_0054C078);

