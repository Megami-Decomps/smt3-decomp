#ifndef SDK_REENT_AUX_H
#define SDK_REENT_AUX_H

#include "sdk_stdio_record.h"

/* The EE stores each bigint limb little-endian, low halfword first. */
typedef union sdkBigintLimb_t {
    u32 word;
    u16 half[2];
} sdkBigintLimb_t;

typedef struct _Bigint {
    struct _Bigint *next;
    s32 k;
    s32 maxwds;
    s32 sign;
    s32 wds;
    /* _Balloc reserves maxwds limbs after this header. Both views share
     * the SDK's trailing-array convention and preserve the record layout. */
    union {
        u32 x[1];
        sdkBigintLimb_t limb[1];
    };
} _Bigint;

typedef struct _glue {
    struct _glue *next;
    s32 niobs;
    FILE *iobs;
} _glue;

#endif
