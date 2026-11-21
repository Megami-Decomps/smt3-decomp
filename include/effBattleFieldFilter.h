#ifndef EFF_BATTLE_FIELD_FILTER_H
#define EFF_BATTLE_FIELD_FILTER_H

#include "effect.h"

typedef struct effBTLFieldFilter_t {
    dds3Effect_Filter_t rect;
    u32 baseColor;
    u32 targetColor;
    u16 frame;
    u16 frameMax;
} effBTLFieldFilter_t;

extern effBTLFieldFilter_t effBTLFieldColorFilter;

#endif
