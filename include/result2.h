#ifndef RESULT2_H
#define RESULT2_H

#include "common.h"

typedef struct {
    u8 level;
    u8 kind;
    u16 value;
} fclSkillParam_t;

typedef struct
{
    s8 SkillCnt;
    u8 TargetLevel[0x19];
    u16 skill[25];
} rstSkillInfo_t;

#endif
