#ifndef FLD_NPC_OVERRIDE_H
#define FLD_NPC_OVERRIDE_H

#include "common.h"

typedef struct {
    s8 field;
    s8 floor;
    char name[14];
    u8 unused;
    s8 modelMajor;
    u8 modelMinor;
    s8 motion;
    s8 hitSize;
    s8 camera;
    s16 eventBit;
} fldNpcOverride_t;

enum { FLD_NPC_OVERRIDE_COUNT = 448 };

extern fldNpcOverride_t fldNpcOverrides[FLD_NPC_OVERRIDE_COUNT];

#endif
