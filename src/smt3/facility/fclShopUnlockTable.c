#include "common.h"

/* Same six-byte item-category record as the consumer in fclShopInit. */
typedef struct
{
    u16 unk_00;
    s16 flag;
    u8 state[2];
} shpItemEntry_t;

shpItemEntry_t shpUnlockTable[7] = {
    { 0, -1, { 1, 1 } },
    { 1, -1, { 1, 1 } },
    { 2, 0x0930, { 2, 7 } },
    { 3, -1, { 3, 3 } },
    { 4, -1, { 4, 4 } },
    { 5, -1, { 5, 5 } },
    { 6, -1, { 6, 6 } },
};
