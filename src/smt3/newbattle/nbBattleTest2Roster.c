#include "nbBattleTestRoster.h"

s32 test2_devilIds[4] = { 194, 192, 193, -1 };

/* Each skill row occupies 0x78 bytes; the dummy setup tests for a
 * negative word and uses the low halfword of each accepted skill ID. */
nbTestSkillId_t test2_devilSkills[120] = {
    { 323 }, { 412 }, { 413 }, { 414 }, { 73 }, { 345 }, { -1 },
    [30] = { 409 }, { 410 }, { 411 }, { 73 }, { 345 }, { -1 },
    [60] = { 409 }, { 410 }, { 411 }, { 73 }, { 345 }, { -1 },
    [90] = { -1 },
};

nbTestSkillId_t test2_dvllist[4] = {
    { 290 }, { -1 }, { 0 }, { 0 },
};
