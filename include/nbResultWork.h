#ifndef SMT3_NB_RESULT_WORK_H
#define SMT3_NB_RESULT_WORK_H

#include "newdata.h"

/* Four native 0x34-byte rows; member names follow typeNbResultWork. */
typedef struct nbResultScore_t {
    s32 useflg;
    s32 exp_cnt;
    s32 l_lv;
    s32 lvupflg;
    s32 unitidx;
    datUnitWork_t *unit;
    s32 lvupanmflg;
    s32 lvupanmcnt;
    s32 lvuplvanmcnt;
    s32 ofsx;
    s32 stopcnt;
    s32 addexp;
    s32 resultadd;
} nbResultScore_t;

extern nbResultScore_t nbResultScoreTable[4];
typedef struct nbResultCaption_t {
    s32 flag;
    s32 x;
    s32 y;
    s32 life;
} nbResultCaption_t;

extern nbResultCaption_t nbResultCaptions[64];
#endif
