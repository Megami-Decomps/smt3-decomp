#include "common.h"

f64 __floatdidf(s64 value)
{
    f64 high = (f64)(s32)(value >> 32);
    s32 low;
    f64 result;

    high *= 65536.0;
    high *= 65536.0;
    /* Keep the low word in the 64-bit working value before signed narrowing;
     * the native mask and conversion order are documented in sdk-matching.md. */
    value &= 0xFFFFFFFFULL;
    low = (s32)value;
    result = (f64)low;
    if (low < 0) {
        result += 4294967296.0;
    }
    return high + result;
}
