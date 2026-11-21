#include "common.h"

extern char nbDebugPhaseNameNon[8];
extern char nbDebugPhaseNameComm[8];
extern char nbDebugPhaseNameAi[8];
extern char nbDebugPhaseNameAiscr[8];
extern char nbDebugPhaseNameBad[8];
extern char nbDebugPhaseNameTarsel[8];
extern char nbDebugPhaseNameStksel[8];
extern char nbDebugPhaseNameReady[8];
extern char nbDebugPhaseNameSkill[8];
extern char nbDebugPhaseNameNego[8];
extern const char datDebugNegoResultName[16];
extern char nbDebugPhaseNameBadwait[8];
extern char nbDebugPhaseNameWait[8];
extern char nbDebugPhaseNameEscape[8];
extern char nbDebugPhaseNameReturn[8];
extern const char datDebugInvisibleName[16];
extern char nbDebugPhaseNameAuto[8];
extern char nbDebugPhaseNameAutotar[8];
extern char nbDebugPhaseNameCheck1[8];
extern const char datDebugDeadScriptName[16];
extern char nbDebugPhaseNameCheck3[8];
extern char nbDebugPhaseNameAto[8];
extern char nbDebugPhaseNameEnd[8];

const char *nbDebugPhaseNameTable[23] = {
    nbDebugPhaseNameNon,
    nbDebugPhaseNameComm,
    nbDebugPhaseNameAi,
    nbDebugPhaseNameAiscr,
    nbDebugPhaseNameBad,
    nbDebugPhaseNameTarsel,
    nbDebugPhaseNameStksel,
    nbDebugPhaseNameReady,
    nbDebugPhaseNameSkill,
    nbDebugPhaseNameNego,
    datDebugNegoResultName,
    nbDebugPhaseNameBadwait,
    nbDebugPhaseNameWait,
    nbDebugPhaseNameEscape,
    nbDebugPhaseNameReturn,
    datDebugInvisibleName,
    nbDebugPhaseNameAuto,
    nbDebugPhaseNameAutotar,
    nbDebugPhaseNameCheck1,
    datDebugDeadScriptName,
    nbDebugPhaseNameCheck3,
    nbDebugPhaseNameAto,
    nbDebugPhaseNameEnd,
};
