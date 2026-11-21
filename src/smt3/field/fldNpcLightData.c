#include "fldNpcLightData.h"

#define DEFAULT_NPC_LIGHT(mode) { { 110, 110, 110 }, { 50, 50, 50 }, 0xF0, (mode) }

fldNpcLightRow_t fldNpcLightTable[32] __attribute__((aligned(16))) = {
    DEFAULT_NPC_LIGHT(1),
    [1 ... 31] = DEFAULT_NPC_LIGHT(0),
};
