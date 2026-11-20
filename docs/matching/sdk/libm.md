# Math runtime matching notes

## __ieee754_sqrtf (sdk/libm, 204 bytes)

51 of 51 words match (204 bytes) at
`0x002D0118`. The configured compiler for this unit is ee-gcc 2.96.
This is SDK code, not game-code progress.

The routine returns the original float when the magnitude word is at most
`0x007FFFFF`, preserving signed zeros and subnormal input words. Other
negative words take the native subtract-then-divide path. Positive words
with an exponent field of 255 go through the integer algorithm, not a
separate infinity or NaN branch.

The restoring integer square root begins with the hidden mantissa bit and
doubles for an odd unbiased exponent. When the signed remainder is at least
the current trial, it subtracts the trial and updates the partial sum and
root. The trial bit halves from `0x01000000` to zero. A nonzero final
remainder applies the native odd-root correction; packing combines the root,
signed exponent and binary32 bias.

Localized register bindings and empty constraints preserve the native homes
of actual live masks, mantissa, exponent, trial state and final packed
components. The split mask construction is the real high/low construction
used by the magnitude test and fraction extraction. The hidden bit is used
in normalization, and the final binary32 conversion uses the actual packed
word. These constraints are reconstruction choices, not a claim about the
original C spelling.
