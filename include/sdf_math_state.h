#ifndef SDF_MATH_STATE_H
#define SDF_MATH_STATE_H

#include "common.h"

typedef union sdfMathConstants_t {
    /* Literal storage: the LookAt fallback followed by the clock terms. */
    struct {
        f32 lookAtFallback[4];
        u16 clockMonthTerms[12];
    } initial;
    f32 quaternion[4];
    u16 monthDays[20];
} sdfMathConstants_t;

typedef struct sdfRandCB_t {
    u32 v[4];
} sdfRandCB_t;

extern sdfMathConstants_t sdfMathConstants;
extern sdfRandCB_t sdfDefaultRandCB;

#endif
