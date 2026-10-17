#ifndef NEWDATA_H
#define NEWDATA_H

#include "common.h"

typedef struct
{
    /* 0x00 */ s32 flag;
    /* 0x04 */ s32 flag2;
    /* 0x08 */ s32 encid;
    /* 0x0C */ s32 dbest;
} datBossPress_t;

/* Offsets through levelupparam are the ones datCalc's retail code uses
 * (param 0x16, skillparam 0x1C, levelupparam 0x7A). The fields after it are
 * not covered by any decompiled function yet, so they are not declared. */
typedef struct
{
    /* 0x00 */ s32 flag;
    /* 0x04 */ u16 id;
    /* 0x06 */ u16 hp;
    /* 0x08 */ u16 maxhp;
    /* 0x0A */ u16 mp;
    /* 0x0C */ u16 maxmp;
    /* 0x0E */ u16 badstatus;
    /* 0x10 */ u32 exp;
    /* 0x14 */ u16 level;
    /* 0x16 */ s8 param[6];
    /* 0x1C */ s8 skillparam[6];
    /* 0x22 */ u16 uniqueid;
    /* 0x24 */ u16 reserve1;
    /* 0x26 */ u16 reserve2;
    /* 0x28 */ char namecode[8];
    /* 0x30 */ s32 skillcnt;
    /* 0x34 */ u16 skill[24];
    /* 0x64 */ s16 nowcommand;
    /* 0x66 */ u16 nowindex;
    /* 0x68 */ s16 nowtarea;
    /* 0x6A */ s16 nowtform;
    /* 0x6C */ s16 prevcommand;
    /* 0x6E */ u16 previndex;
    /* 0x70 */ s16 prevtarea;
    /* 0x72 */ s16 prevtform;
    /* 0x74 */ s8 mitamaparam[6];
    /* 0x7A */ s8 levelupparam[6];
} datUnitWork_t;

#endif /* NEWDATA_H */
