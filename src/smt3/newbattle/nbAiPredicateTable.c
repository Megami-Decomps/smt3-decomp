#include "common.h"

struct nbActionProcessData_t;

typedef s32 (*nbAiPredicate_t)(struct nbActionProcessData_t *, s32);

extern s32 Chk_BA_286248(struct nbActionProcessData_t *, s32);
extern s32 Chk_CHKCNT_2855D8(struct nbActionProcessData_t *, s32);
extern s32 Chk_DMGCNT_285578(struct nbActionProcessData_t *, s32);
extern s32 Chk_ENALLBAD(struct nbActionProcessData_t *, s32);
extern s32 Chk_ENBAD(struct nbActionProcessData_t *, s32);
extern s32 Chk_ENCNT(struct nbActionProcessData_t *, s32);
extern s32 Chk_ENDARK(struct nbActionProcessData_t *, s32);
extern s32 Chk_ENHAMAMHK(struct nbActionProcessData_t *, s32);
extern s32 Chk_ENHOJO(struct nbActionProcessData_t *, s32);
extern s32 Chk_ENHOJOMAX(struct nbActionProcessData_t *, s32);
extern s32 Chk_ENID(struct nbActionProcessData_t *, s32);
extern s32 Chk_ENLIGHT(struct nbActionProcessData_t *, s32);
extern s32 Chk_FRBAD(struct nbActionProcessData_t *, s32);
extern s32 Chk_FRCNT(struct nbActionProcessData_t *, s32);
extern s32 Chk_FRHOJO(struct nbActionProcessData_t *, s32);
extern s32 Chk_FRHOJOMAX(struct nbActionProcessData_t *, s32);
extern s32 Chk_FRHP(struct nbActionProcessData_t *, s32);
extern s32 Chk_FRID(struct nbActionProcessData_t *, s32);
extern s32 Chk_MYBAD(struct nbActionProcessData_t *, s32);
extern s32 Chk_MYHP(struct nbActionProcessData_t *, s32);
extern s32 Chk_MYMP(struct nbActionProcessData_t *, s32);
extern s32 Chk_PLV(struct nbActionProcessData_t *, s32);
extern s32 Chk_POW_2862D0(struct nbActionProcessData_t *, s32);
extern s32 Chk_PPARTYLV(struct nbActionProcessData_t *, s32);
extern s32 Chk_SENSEI_286530(struct nbActionProcessData_t *, s32);
extern s32 Chk_SKILLNOMP(struct nbActionProcessData_t *, s32);
extern s32 Chk_TURN(struct nbActionProcessData_t *, s32);

/* Indexed by the high halfword of the AI condition. Condition 0 succeeds
 * before the dispatch, so it has no predicate. */
nbAiPredicate_t D_004F0788[29] = {
    0,
    Chk_MYHP,
    Chk_FRHP,
    Chk_DMGCNT_285578,
    Chk_CHKCNT_2855D8,
    Chk_FRCNT,
    Chk_ENCNT,
    Chk_MYBAD,
    Chk_FRBAD,
    Chk_ENBAD,
    Chk_ENALLBAD,
    Chk_ENID,
    Chk_FRID,
    Chk_ENHOJO,
    Chk_FRHOJO,
    Chk_BA_286248,
    Chk_ENLIGHT,
    Chk_ENDARK,
    Chk_SENSEI_286530,
    Chk_PLV,
    Chk_PPARTYLV,
    Chk_CHKCNT_2855D8,
    Chk_TURN,
    Chk_POW_2862D0,
    Chk_MYMP,
    Chk_ENHOJOMAX,
    Chk_FRHOJOMAX,
    Chk_ENHAMAMHK,
    Chk_SKILLNOMP,
};
