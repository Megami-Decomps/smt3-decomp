#include "common.h"

u64 __fixunsdfdi(f64 value)
{
    u64 result;

    if (value < 0.0) {
        return 0;
    }
    result = (u64)(u32)(value * (1.0 / 4294967296.0)) << 32;
    /* Consume the input into its remainder; see the qualified allocation
     * evidence in docs/sdk-matching.md. */
    value -= (f64)result;
    if (value < 0.0) {
        result -= (u32)(0.0 - value);
    } else {
        result += (u32)value;
    }
    return result;
}
