#ifndef CMP_HEARTS_ICON_DATA_H
#define CMP_HEARTS_ICON_DATA_H

#include "common.h"

typedef struct
{
    /* 0x00 */ s8 id;
    /* 0x01 */ s8 type;
    /* 0x02 */ u8 unk_02[2];
    /* 0x04 */ s32 color;
} cmpHeartsIconTableEntry_t;

SIZE_CHECK(cmpHeartsIconEntrySize, cmpHeartsIconTableEntry_t, 8);

extern cmpHeartsIconTableEntry_t cmpHeartsIconTable[27];

#endif
