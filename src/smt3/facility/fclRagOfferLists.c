#include "common.h"

/* Four fixed offers per menu; the display indexes each pair's first byte. */
u8 ragList2Fixed[8] __attribute__((section(".data"), aligned(8))) = { 1, 0, 6, 0, 11, 0, 16, 0 };
u8 ragList3Fixed[8] __attribute__((section(".data"), aligned(8))) = { 1, 0, 6, 0, 11, 0, 16, 0 };

/* Heart list indices for the two native offer sets. */
u8 ragHeartsIndexTable[2][32] = {
    { 1, 2, 3, 4, 5, 6, 7, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
