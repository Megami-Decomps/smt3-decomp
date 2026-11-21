#ifndef PB_LEVEL_TABLE_H
#define PB_LEVEL_TABLE_H

#include "common.h"

typedef union
{
    struct { s16 clearBit, unk02; } header;
    struct { s16 number, label; } stage;
} pbLevelSlot_t;

SIZE_CHECK(pbLevelSlotSize, pbLevelSlot_t, 4);

extern pbLevelSlot_t *pbLevelTable[3];

#endif
