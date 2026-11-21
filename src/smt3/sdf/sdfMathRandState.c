#include "sdf_math_state.h"

SIZE_CHECK(sdfMathConstantsSize, sdfMathConstants_t, 0x28);
SIZE_CHECK(sdfDefaultRandStateSize, sdfRandCB_t, 0x10);

/* LookAt loads the first four floats with lqc2. Clock seeding also reads
 * this object through its halfword view, including the leading zero term. */
sdfMathConstants_t sdfMathConstants __attribute__((aligned(16))) = {
    {
        { 0.0f, 1.0f, 0.0f, 0.0f },
        { 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30 },
    },
};

/* The default generator is mutated by sdfRandomize and dds3GetRandA.
 * Its initial zero state is file-backed at the end of this .data owner. */
sdfRandCB_t sdfDefaultRandCB = {
    { 0, 0, 0, 0 },
};
