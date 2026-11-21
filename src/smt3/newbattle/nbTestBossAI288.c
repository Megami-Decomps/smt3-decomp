#include "newdata.h"

SIZE_CHECK(nbTestBossAI288_size_must_be_0xA4, datDevilAIData_t, 0xA4);

/* Debug AI copied by SetBossDummy into devil ID 288. */
datDevilAIData_t D_00526DE0 = {
    .entry = {
        { { 0x00050001u, 0 }, 1 },
    },
    .table = {
        { { 100, 0x8000, 0 } },
        { { 100, 0xE00B, 0 } },
        { { 100, 0x0007, 0 } },
    },
    .death = { 0, 4 },
};
