#ifndef DATCALC_H
#define DATCALC_H

#include "newdata.h"
#include "result2.h"

typedef struct {
    u8 head[6];
    fclSkillParam_t entry[24];
} datLevelUpRecord_t;

extern datLevelUpRecord_t datLevelUpTable[];

/* paratype indexes datUnitWork_t::param[6]; the retail code uses 0, 2, 3 and 5
 * and no source names them, so they stay plain integers. */
s32 datGetDefPow(datUnitWork_t *work);
s32 datAddHp(datUnitWork_t *work, s32 hp);
s32 datGetBaseParam(datUnitWork_t *work, s32 paratype);
s32 datGetParam(datUnitWork_t *work, s32 paratype);
s32 datBossRashChk(s32 eid);

extern datBossPress_t datBossPress[40];

#endif /* DATCALC_H */
