#include "common.h"

extern char nbUnitBadStatusNameShock[8];
extern char nbUnitBadStatusNameFreeze[8];
extern char nbUnitBadStatusNameSleep[8];
extern char nbUnitBadStatusNamePanic[8];
extern char nbUnitBadStatusNameBind[8];
extern char nbUnitBadStatusNameClose[8];
extern char nbUnitBadStatusNamePoison[8];
extern char nbUnitBadStatusNameCharm[8];
extern char nbUnitBadStatusNamePara[8];
extern char nbUnitBadStatusNameFly[8];
extern char nbUnitBadStatusNameStone[8];
extern char nbUnitBadStatusNameDead[8];
extern char nbUnitBadStatusNameNone[8];

/* Native order for the bad-status index used by the unit debug display. */
const char *nbUnitBadStatusNames[13] = {
    nbUnitBadStatusNameShock,
    nbUnitBadStatusNameFreeze,
    nbUnitBadStatusNameSleep,
    nbUnitBadStatusNamePanic,
    nbUnitBadStatusNameBind,
    nbUnitBadStatusNameClose,
    nbUnitBadStatusNamePoison,
    nbUnitBadStatusNameCharm,
    nbUnitBadStatusNamePara,
    nbUnitBadStatusNameFly,
    nbUnitBadStatusNameStone,
    nbUnitBadStatusNameDead,
    nbUnitBadStatusNameNone,
};
