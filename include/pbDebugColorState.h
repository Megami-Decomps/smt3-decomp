#ifndef PB_DEBUG_COLOR_STATE_H
#define PB_DEBUG_COLOR_STATE_H

#include "common.h"

typedef struct { u8 value; u8 upperBytes[3]; } pbColorByte_t;

typedef union
{
    struct { s32 red, green, blue, alpha; };
    s32 channels[4];
    pbColorByte_t channelBytes[4];
} pbColor_t;

extern pbColor_t D_00326C50;

#endif
