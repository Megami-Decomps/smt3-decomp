#include "common.h"

extern char nbBadStatusShock[8];
extern char nbBadStatusFreeze[8];
extern char nbBadStatusSleep[8];
extern char nbBadStatusPanic[8];
extern char nbBadStatusBind[8];
extern char nbBadStatusClose[8];
extern char nbBadStatusPoison[8];
extern char nbBadStatusCharm[8];
extern const char nbBadStatusParalyze[16];
extern char nbBadStatusFly[8];
extern char nbBadStatusStone[8];
extern char nbBadStatusDead[8];

/* Names in status bit order, as scanned by the debug display. */
const char *nbBadStatusNames[12] = {
    nbBadStatusShock,
    nbBadStatusFreeze,
    nbBadStatusSleep,
    nbBadStatusPanic,
    nbBadStatusBind,
    nbBadStatusClose,
    nbBadStatusPoison,
    nbBadStatusCharm,
    nbBadStatusParalyze,
    nbBadStatusFly,
    nbBadStatusStone,
    nbBadStatusDead,
};
