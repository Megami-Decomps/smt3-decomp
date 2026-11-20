#include "common.h"

typedef struct {
    u16 devilId;
    u16 selector[2];
    u8 percent;
} cmbEvoTableEntry_t;

/* Devil ID, two parameter selectors, and evolution percentage. */
cmbEvoTableEntry_t cmbEvoTable[4] = {
    { 0x2B, { 1, 5 }, 120 },
    { 0x28, { 6, 4 }, 120 },
    { 0x2A, { 3, 6 }, 120 },
    { 0x29, { 4, 5 }, 120 },
};
