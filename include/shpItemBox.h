#ifndef SHP_ITEM_BOX_H
#define SHP_ITEM_BOX_H

#include "common.h"

typedef struct
{
    u8 weight;
    u8 reserved;
    u16 item;
} shpItemBoxEntry_t;

SIZE_CHECK(shpItemBoxEntrySize, shpItemBoxEntry_t, 4);

#endif
