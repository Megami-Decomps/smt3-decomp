#ifndef FACILITY_H
#define FACILITY_H

#include "common.h"

typedef struct
{
    /* 0x00 */ u8 animState;
    /* 0x01 */ u8 reserved01[3];
    /* 0x04 */ s32 target;
    /* 0x08 */ u8 reserved08[0xC];
    /* 0x14 */ s32 phase;
    /* 0x18 */ s32 phaseStep;
    /* 0x1C */ s32 value;
} fclPanelAnimState_t;

typedef struct
{
    /* 0x00 */ fclPanelAnimState_t state;
    /* 0x20 */ void (*calcSizes)();
} fclPanelAnim_t;

typedef char fclPanelAnimStateSizeCheck[(sizeof(fclPanelAnimState_t) == 0x20) ? 1 : -1];

#endif
