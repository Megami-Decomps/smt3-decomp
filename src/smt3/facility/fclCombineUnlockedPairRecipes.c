#include "common.h"

typedef struct {
    u16 devilA;
    u16 devilB;
    u16 result;
} cmbPairEntry_t;

SIZE_CHECK(cmbPairRecipeSize, cmbPairEntry_t, 6);

cmbPairEntry_t cmbEntryPairTable[3] = {
    { 98, 150, 12 },
    { 63, 141, 140 },
    { 62, 140, 139 },
};
