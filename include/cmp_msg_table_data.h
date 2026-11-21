#ifndef CMP_MSG_TABLE_DATA_H
#define CMP_MSG_TABLE_DATA_H

#include "common.h"

typedef struct
{
    /* 0x00 */ u16 id;
    /* 0x02 */ u16 unk_02;
    /* 0x04 */ u16 unk_04;
} cmpMsgEntry_t;

extern cmpMsgEntry_t cmpItemMsgTable[];
extern cmpMsgEntry_t cmpSkillMsgTable[];

#endif
