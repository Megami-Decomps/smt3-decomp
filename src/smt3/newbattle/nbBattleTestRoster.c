#include "nbBattleTestRoster.h"

s32 test_devilIds[4] = { -1, -1, -1, -1 };

/* Each skill row occupies 0x78 bytes; the dummy setup tests for a
 * negative word and uses the low halfword of each accepted skill ID. */
nbTestSkillId_t test_devilSkills[90] = {
    { 5 }, { 42 }, { 98 },
    [30] = { 23 }, { 63 }, { 125 },
    [60] = { 12 }, { 27 }, { 41 },
};

nbTestSkillId_t test_pskill[14] = {
    { 131 }, { 144 }, { 136 }, { -1 }, { -1 }, { -1 }, { -1 },
    { -1 }, { -1 }, { -1 }, { -1 }, { -1 }, { 0 }, { 0 },
};
