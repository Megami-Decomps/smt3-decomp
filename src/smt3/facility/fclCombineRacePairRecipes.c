#include "common.h"

typedef struct {
    u16 devilId;
    u8 raceA;
    u8 raceB;
    u16 result;
} cmbRacePairEntry_t;

SIZE_CHECK(cmbRacePairEntrySize, cmbRacePairEntry_t, 6);

cmbRacePairEntry_t cmbRacePairTable[2] = {
    { 0x8A, 0x0B, 0x13, 0xC1 },
    { 0x8A, 0x18, 0x13, 0xC1 },
};
