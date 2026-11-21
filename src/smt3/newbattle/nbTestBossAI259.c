#include "newdata.h"

SIZE_CHECK(nbTestBossAIData_size_must_be_0xA4, datDevilAIData_t, 0xA4);

/* Complete debug AI presets copied by SetBossDummy. */
/* Consecutive presets for SetBossDummy; second address: D_00526FCC. */
datDevilAIData_t D_00526F28[2] = {
    {
        .aiscript = 0, .ailevel = 0,
        .entry = {
            { { 0x0u, 0x0u }, 0 },
            { { 0x0u, 0x0u }, 0 },
            { { 0x0u, 0x0u }, 0 },
        },
        .table = {
            { { 100, 0x8000, 0x0u }, { 0, 0x0000, 0x0u }, { 0, 0x0000, 0x0u }, { 0, 0x0000, 0x0u }, { 0, 0x0000, 0x0u } },
            { { 100, 0xE007, 0x0u }, { 0, 0x0000, 0x0u }, { 0, 0x0000, 0x0u }, { 0, 0x0000, 0x0u }, { 0, 0x0000, 0x0u } },
            { { 100, 0x0007, 0x0u }, { 0, 0x0000, 0x0u }, { 0, 0x0000, 0x0u }, { 0, 0x0000, 0x0u }, { 0, 0x0000, 0x0u } },
        },
        .death = { 0, 8 },
    },
    {
        .entry = {
            { { 0x160000u, 0 }, 1 },
        },
        .table = {
            { { 100, 0x8000, 0 } },
            { { 100, 0xE005, 0 } },
            { { 100, 0x0007, 0 } },
        },
    },
};
