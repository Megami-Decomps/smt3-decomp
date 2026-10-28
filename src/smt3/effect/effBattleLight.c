#include "common.h"
#include "motion.h"
#include "model.h"
#include "event.h"
#include "nb.h"
#include "effBattle.h"
#include "ps2asm.h"

extern u32 effBTLPointLightProcess;
extern u32 effColorBlend(u32 first, u32 second, f32 rate);
extern u32 effBTLPartGetScalePtrSub(s32 selector);
extern void evtUnitLightOn(evtUnit_t *unit);
extern void evtSetUnitLightAndAmbient(evtUnit_t *unit, s32 frame, u32 diffuse, u32 ambient);
extern void evtSetUnitLightDir(evtUnit_t *unit, s32 frame);
extern void effBTLPartGetPointLightPtr(nbFormation_t *formation,
                                     effBTLAimParams_t *position, sdfVector4_t *direction);

typedef struct
{
    u8 type;
    u8 unknown01;
    u8 unknown02;
    u8 unknown03;
    u32 unknown04;
    u32 diffuse;
    u32 ambient;
    u32 color_anim;
    u32 unknown14;
    u32 unknown18;
    u32 unknown1C;
    u32 unknown20;
} effBTL_SkillLight_t;

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
    effBTL_SkillLight_t param;
    u32 light_frame;
    effBTLModelColorParam_t model;
    u32 model_frame;
} effBattleLight_effBTLPointLight_tag;

/* The unit position and model id a light starts from. */
typedef struct
{
    sdfVector4_t position;
    u32 unknown10;
    u32 unknown14;
    u8 reserve18[0x8];
} effBattleLight_effBTLPointLight_Base_tag;

extern effBattleLight_effBTLPointLight_tag effBTLPointLight[32];

extern effBattleLight_effBTLPointLight_Base_tag effBTLPointLightBase[32];

s32 effBTLModelGetID(nbFormation_t *pFormation);

u32 effBTLSkillGetInvokeParty(void);

u32 effBTLSkillGetTargetParty(void);

void effBTLPointLightModelSet(effBTL_SkillLight_t *pLight, nbFormation_t *pFormation);

void effBTLPointLightGroupSet(effBTL_SkillLight_t *pLight, u32 group);

void effBTLPointLightStart(void);

void effBTLPointLightMain(void);

void effBTLPointLightEnd(void);

void effBTLModelColorMain(void);

void evtUnitLightOff(evtUnit_t *h, s32 frame);

void evtSetUnitColorRGB(evtUnit_t *h, s32 frame, u32 rgb);

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

void effBTLPointLightModelSet(effBTL_SkillLight_t *pParam, nbFormation_t *pFormation)
{
    effBattleLight_effBTLPointLight_tag *pLight;
    s32 id;

    pLight = &effBTLPointLight[pFormation->formindex];
    pLight->param = *pParam;
    id = effBTLModelGetID(pFormation);
    if (id == 0x125 || id == 0x108) {
        pLight->param.diffuse = 0x80FFB996;
        pLight->param.ambient = 0x80503214;
    }
    pLight->light_frame = 0;
}

void effBTLPointLightGroupSet(effBTL_SkillLight_t *pLight, u32 group)
{
    nbFormation_t *pFormation;
    s32 i;

    pFormation = *(nbFormation_t **)nbGetMainProcessData();
    for (i = 0x1F; i >= 0; i--) {
        if ((pFormation->stat & group) != 0 && (pFormation->stat & 3) == 3) {
            effBTLPointLightModelSet(pLight, pFormation);
        }
        pFormation++;
    }
}

void effBTLPointLightSet(effBTL_SkillLight_t *pLight, nbFormation_t *pFormation)
{
    if (pLight->color_anim == pLight->unknown18) {
        return;
    }
    switch (pLight->type) {
    case 0:
        effBTLPointLightModelSet(pLight, pFormation);
        break;
    case 1:
        effBTLPointLightGroupSet(pLight, effBTLSkillGetInvokeParty());
        break;
    case 2:
        effBTLPointLightGroupSet(pLight, effBTLSkillGetTargetParty());
        break;
    case 3:
        effBTLPointLightGroupSet(pLight, 0x700);
        break;
    }
}

void effBTLModelColorSet(effBTLModelColorParam_t *pModel, nbFormation_t *pFormation)
{
    effBattleLight_effBTLPointLight_tag *pLight;
    s32 id;

    id = effBTLModelGetID(pFormation);
    if (id != 0x125 && id != 0x108) {
        pLight = &effBTLPointLight[pFormation->formindex];
        pLight->model = *pModel;
        pLight->model_frame = 0;
    }
}

void effBTLPointLightStart(void)
{
    nbFormation_t *pFormation;
    effBattleLight_effBTLPointLight_Base_tag *pBase;
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

void effBTLPointLightMain(void)
{
    nbFormation_t *formation;
    effBattleLight_effBTLPointLight_tag *light;
    effBattleLight_effBTLPointLight_Base_tag *base;
    evtUnit_t *unit;
    sdfVector4_t direction;
    u32 frame;
    u32 start;
    u32 end;
    u32 first;
    u32 second;
    u32 secondStart;
    u32 i;

    formation = nbGetMainProcessData()->form;
    light = effBTLPointLight;
    base = effBTLPointLightBase;
    for (i = 0; i < 32; i++) {
        if ((formation->stat & 3) == 3 && (s32)light->light_frame >= 0) {
            frame = light->light_frame;
            start = light->param.color_anim;
            end = light->param.unknown18;
            first = effBTLPartGetScalePtrSub(light->param.unknown14);
            second = effBTLPartGetScalePtrSub(light->param.unknown1C);
            if (start < end && end >= second) {
                secondStart = end - second;
                if (start < secondStart) {
                    unit = formation->hunit;
                    if (unit != NULL) {
                        if (frame == start) {
                            evtUnitLightOn(unit);
                            evtSetUnitLightAndAmbient(unit, first, light->param.diffuse,
                                                      light->param.ambient);
                            effBTLPartGetPointLightPtr(formation, (effBTLAimParams_t *)&light->param,
                                                      &direction);
                            SDF_VU0_LOADVECTOR(&direction);
                            evtSetUnitLightDir(unit, first);
                        }
                        if (frame == secondStart) {
                            evtSetUnitLightAndAmbient(unit, second, base->unknown10, base->unknown14);
                            SDF_VU0_LOADVECTOR(&base->position);
                            evtSetUnitLightDir(unit, second);
                        }
                        light->light_frame++;
                    }
                }
            }
        }
        formation++;
        light++;
        base++;
    }
}

void effBTLModelColorMain(void)
{
    nbFormation_t *formation;
    effBattleLight_effBTLPointLight_tag *light;
    u32 frame;
    u32 start;
    u32 end;
    u32 secondStart;
    u32 first;
    u32 second;
    u32 color;
    u32 i;

    formation = nbGetMainProcessData()->form;
    light = effBTLPointLight;
    for (i = 0; i < 32; i++) {
        if ((formation->stat & 3) == 3 && (s32)light->model_frame >= 0) {
            frame = light->model_frame;
            start = light->model.unknown04;
            end = light->model.unknown0C;
            first = effBTLPartGetScalePtrSub(light->model.unknown08);
            second = effBTLPartGetScalePtrSub(light->model.unknown10);
            if (start < end && end >= second) {
                secondStart = end - second;
                if (start < secondStart && formation->hunit != NULL) {
                    if (frame == start) {
                        if ((formation->modelcol & 0xFFFFFF) != 0x808080) {
                            color = effColorBlend(light->model.unknown00, formation->modelcol, 0.5f);
                            evtSetUnitColorRGB(formation->hunit, first, color);
                        } else {
                            evtSetUnitColorRGB(formation->hunit, first, light->model.unknown00);
                        }
                    }
                    if (frame == secondStart) {
                        evtSetUnitColorRGB(formation->hunit, second, formation->modelcol);
                    }
                    light->model_frame++;
                }
            }
        }
        formation++;
        light++;
    }
}

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

