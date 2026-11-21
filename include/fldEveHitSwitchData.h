#ifndef FLD_EVE_HIT_SWITCH_DATA_H
#define FLD_EVE_HIT_SWITCH_DATA_H

#include "common.h"

/* gFldEveHitOnOffTbl stride, 20 bytes. */
typedef struct {
    s16 area;
    s16 floor;
    s16 flag;
    char name[14];
} fldEveHitSwitch_t;

extern fldEveHitSwitch_t gFldEveHitOnOffTbl[32];

#endif
