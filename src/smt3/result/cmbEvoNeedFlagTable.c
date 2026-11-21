#include "common.h"

typedef struct
{
    /* 0x0 */ u16 devilId;
    /* 0x2 */ u16 flag;
} cmbEvoFlagEntry_t;

/* Evolution prerequisites indexed by devil ID. */
cmbEvoFlagEntry_t cmbEvoNeedFlagTable[6] = {
    { 110, 2304 }, { 155, 2305 }, { 17, 2306 },
    { 141, 2307 }, { 161, 2308 }, { 105, 2312 },
};
