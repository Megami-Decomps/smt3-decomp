#ifndef RESULT2_H
#define RESULT2_H

#include "common.h"

typedef struct
{
    s8 SkillCnt;
    u8 TargetLevel[0x19];
    u16 skill[25];
} rstSkillInfo_t;

#endif
