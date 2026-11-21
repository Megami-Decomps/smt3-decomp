#include "nbBattleTestRoster.h"

/* No passive skills: the first signed word ends the list. */
nbTestSkillId_t nbTest8PassiveSkills[1] __attribute__((section(".data"))) = {{-1}};
