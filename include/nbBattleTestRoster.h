#ifndef NB_BATTLE_TEST_ROSTER_H
#define NB_BATTLE_TEST_ROSTER_H

#include "common.h"

/* The signed word terminates a list; the accepted ID is read as a halfword. */
typedef union {
    s32 value;
    struct { u16 low; u16 high; };
} nbTestSkillId_t;

SIZE_CHECK(nbTestSkillIdSize, nbTestSkillId_t, 4);

#endif
