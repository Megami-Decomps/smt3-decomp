#include "common.h"
#include "motion.h"
#include "model.h"
#include "event.h"
#include "nb.h"
#include "effBattle.h"
#include "ps2asm.h"

extern u32 effBTLPointLightProcess;

typedef struct
{
    u8 type;
    u8 unknown01;
    u8 unknown02;
    u8 unknown03;
    u32 unknown04;
    u32 unknown08;
    u32 unknown0C;
    u32 unknown10;
    u32 unknown14;
    u32 unknown18;
    u32 unknown1C;
    u32 unknown20;
} effBTLPointLightParam_t;

typedef struct
{
    u32 unknown00;
    u32 unknown04;
    u32 unknown08;
    u32 unknown0C;
    u32 unknown10;
    u32 unknown14;
    u32 unknown18;
} effBTLModelColorParam_t;

/* One light the battle effects place, 32 of them indexed by the formation id. */
typedef struct
{
    effBTLPointLightParam_t param;
    u32 unknown24;
    effBTLModelColorParam_t color;
    u32 unknown44;
} effBTLPointLight_t;

/* The unit position and model id a light starts from. */
typedef struct
{
    sdfVector4_t position;
    u32 unknown10;
    u32 unknown14;
    u8 reserve18[0x8];
} effBTLPointLightBase_t;

extern effBTLPointLight_t effBTLPointLight[32];

extern effBTLPointLightBase_t effBTLPointLightBase[32];

s32 effBTLModelGetID(nbFormation_t *pFormation);

u32 effBTLSkillGetInvokeParty(void);

u32 effBTLSkillGetTargetParty(void);

void effBTLPointLightModelSet(effBTLPointLightParam_t *pParam, nbFormation_t *pFormation);

void effBTLPointLightGroupSet(effBTLPointLightParam_t *pParam, u32 mask);

void effBTLPointLightStart(void);

void effBTLPointLightMain(void);

void effBTLPointLightEnd(void);

void effBTLModelColorMain(void);

void evtUnitLightOff(evtUnit_t *hUnit, s32 param);

void evtSetUnitColorRGB(evtUnit_t *hUnit, s32 param, u32 color);

void effBTLPointLightInit(void)
{
    effBTLPointLightProcess = 0;
}

void effBTLPointLightON(void)
{
    if (effBTLPointLightProcess == 0) {
        effBTLPointLightProcess = 1;
    } else {
        effBTLPointLightProcess = 2;
    }
}

void effBTLPointLightOFF(void)
{
    if (effBTLPointLightProcess != 0) {
        effBTLPointLightProcess = 3;
    }
}

void effBTLPointLightCompulsionOFF(void)
{
    nbFormation_t *pFormation;
    s32 i;

    pFormation = *(nbFormation_t **)nbGetMainProcessData();
    for (i = 0x1F; i >= 0; i--) {
        if ((pFormation->stat & 3) == 3 && pFormation->hunit != NULL) {
            evtUnitLightOff(pFormation->hunit, 0);
            evtSetUnitColorRGB(pFormation->hunit, 0xC, pFormation->modelcol);
        }
        pFormation++;
    }
    effBTLPointLightProcess = 0;
}

void effBTLPointLightModelSet(effBTLPointLightParam_t *pParam, nbFormation_t *pFormation)
{
    effBTLPointLight_t *pLight;
    s32 id;

    pLight = &effBTLPointLight[pFormation->formindex];
    pLight->param = *pParam;
    id = effBTLModelGetID(pFormation);
    if (id == 0x125 || id == 0x108) {
        pLight->param.unknown08 = 0x80FFB996;
        pLight->param.unknown0C = 0x80503214;
    }
    pLight->unknown24 = 0;
}

void effBTLPointLightGroupSet(effBTLPointLightParam_t *pParam, u32 mask)
{
    nbFormation_t *pFormation;
    s32 i;

    pFormation = *(nbFormation_t **)nbGetMainProcessData();
    for (i = 0x1F; i >= 0; i--) {
        if ((pFormation->stat & mask) != 0 && (pFormation->stat & 3) == 3) {
            effBTLPointLightModelSet(pParam, pFormation);
        }
        pFormation++;
    }
}

void effBTLPointLightSet(effBTLPointLightParam_t *pParam, nbFormation_t *pFormation)
{
    if (pParam->unknown10 == pParam->unknown18) {
        return;
    }
    switch (pParam->type) {
    case 0:
        effBTLPointLightModelSet(pParam, pFormation);
        break;
    case 1:
        effBTLPointLightGroupSet(pParam, effBTLSkillGetInvokeParty());
        break;
    case 2:
        effBTLPointLightGroupSet(pParam, effBTLSkillGetTargetParty());
        break;
    case 3:
        effBTLPointLightGroupSet(pParam, 0x700);
        break;
    }
}

void effBTLModelColorSet(effBTLModelColorParam_t *pParam, nbFormation_t *pFormation)
{
    effBTLPointLight_t *pLight;
    s32 id;

    id = effBTLModelGetID(pFormation);
    if (id != 0x125 && id != 0x108) {
        pLight = &effBTLPointLight[pFormation->formindex];
        pLight->color = *pParam;
        pLight->unknown44 = 0;
    }
}

void effBTLPointLightStart(void)
{
    nbFormation_t *pFormation;
    effBTLPointLightBase_t *pBase;
    evtUnit_t *hUnit;
    s32 i;

    memset(effBTLPointLight, -1, sizeof(effBTLPointLight));
    pFormation = *(nbFormation_t **)nbGetMainProcessData();
    pBase = effBTLPointLightBase;
    for (i = 0; i < 32; i++) {
        if ((pFormation->stat & 3) == 3) {
            hUnit = pFormation->hunit;
            if (hUnit != NULL) {
                SDF_VU0_COPYVECTOR(pBase, &hUnit->vec10);
                pBase->unknown10 = hUnit->color;
                pBase->unknown14 = hUnit->color50;
            }
        }
        pFormation++;
        pBase++;
    }
}

void effBTLPointLightEnd(void)
{
    nbFormation_t *pFormation;
    s32 i;

    pFormation = *(nbFormation_t **)nbGetMainProcessData();
    for (i = 0x1F; i >= 0; i--) {
        if ((pFormation->stat & 3) == 3 && pFormation->hunit != NULL) {
            evtUnitLightOff(pFormation->hunit, 0xC);
            evtSetUnitColorRGB(pFormation->hunit, 0xC, pFormation->modelcol);
        }
        pFormation++;
    }
}

INCLUDE_ASM(const s32, "effect/effBattleLight", effBTLPointLightMain);

INCLUDE_ASM(const s32, "effect/effBattleLight", effBTLModelColorMain);

void effBTLPointLightRun(void)
{
    switch (effBTLPointLightProcess) {
    case 0:
        break;
    case 1:
        effBTLPointLightStart();
        effBTLPointLightProcess = 2;
        break;
    case 2:
        effBTLPointLightMain();
        effBTLModelColorMain();
        break;
    case 3:
        effBTLPointLightEnd();
        effBTLPointLightProcess = 0;
        break;
    }
}
INCLUDE_SDATA(const s32, "effect/effBattleLight", effBTLPointLightProcess);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F0E4);

INCLUDE_SDATA(const s32, "effect/effBattleLight", effBTLFieldColorOn);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F0F1);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F0F4);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F0F8);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F100);

INCLUDE_SDATA(const s32, "effect/effBattleLight", effBTLCutCount);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F10C);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F110);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F118);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F120);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F128);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F130);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F134);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F135);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F138);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F140);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F148);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F150);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F158);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F160);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F168);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F170);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F178);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F180);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F188);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F18A);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F18B);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F18C);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F190);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F198);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F1A0);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F1A8);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F1B0);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F1B8);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F1C0);

INCLUDE_SDATA(const s32, "effect/effBattleLight", effBTLDebugCameraAllEdit);

INCLUDE_SDATA(const s32, "effect/effBattleLight", effBTLDebugCameraAllTable);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F1C3);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F1C8);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F1D0);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F1D8);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F1E0);

INCLUDE_SDATA(const s32, "effect/effBattleLight", D_0054F1E4);

