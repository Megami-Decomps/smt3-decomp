#include "common.h"

typedef struct {
    u16 devilA;
    u16 devilB;
    u16 result;
} cmbPairEntry_t;

SIZE_CHECK(cmbPairRecipeSize, cmbPairEntry_t, 6);

cmbPairEntry_t cmbPairTable[6] = {
    { 36, 37, 40 },
    { 36, 38, 43 },
    { 36, 39, 41 },
    { 37, 38, 41 },
    { 37, 39, 43 },
    { 38, 39, 42 },
};
