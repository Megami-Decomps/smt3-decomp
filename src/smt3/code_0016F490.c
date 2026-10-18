#include "common.h"
#include "motion.h"
#include "model.h"
#include "event.h"
#include "nb.h"
#include "effBattle.h"
#include "ps2asm.h"

extern u128 D_0031DF60;
extern u128 D_0031DF70;
extern u128 D_0031DF80;

typedef struct
{
    u16 type;
    void *pHandle;
} effPartHandle_t;

typedef struct
{
    u32 kateg;
    u32 param;
    void *pHandle;
} dds3EffectPCP_t;

typedef struct
{
    u32 size;
    u32 offset;
    u32 type;
    u32 reserve;
} dds3EffectPCPData_Part_t;

typedef struct
{
    u32 kateg;
    u32 param;
    u32 part;
    u32 reserve;
    dds3EffectPCPData_Part_t pcpData[1];
} dds3EffectPCPData_t;

typedef struct
{
    void *(*create)(void *);
    void (*draw)(void *);
    void (*destroy)(void *);
    void *(*copy)(void *);
    void (*setPosition)(void *, void *);
    void (*setScale)(void *, f32);
    void (*setRotateMatrix)(void *, void *);
    void (*setColor)(void *, u32);
    void (*setDirection)(void *, void *);
    void (*setArea)(void *, f32);
} dds3EffectFunc_t;


void mdlSetPosition(mdlObj_t *aHandle);
void mdlSetRotate(mdlObj_t *aHandle);
void mdlSetScale(mdlObj_t *aHandle);
void mdlSetColor(mdlObj_t *aHandle, u32 aColor);
void mdlLoopMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber);

extern dds3EffectFunc_t dds3EffectFuncTable[7];
extern void *D_002F69B8[4];
extern s32 D_0054F1E4;
extern s32 effBTLCutCount;
extern u32 effBTLCutMaxTable[4];

/* Header of a packed model an effect part carries, followed by the pack data. */
typedef struct
{
    u32 size;
    u8 reserve[0xC];
    u8 data[1];
} dds3EffectPartModelData_t;

void *dds3BillboardCreate(s32 kind, u32 data);
void mdlPackDecode(s32 group, s32 id, s32 mode, const u8 *data, s32 size);
void mdlLink(mdlObj_t *hModel, void **surfaces);
void mdlFree(mdlObj_t *hModel);
void effBTLSkillStart(s32 id, s32 param);
void effBTLSkillEnd(void);
extern dds3EffectFunc_t EffectPCPFuncTable[];
extern void **D_0031DDD0[];

void *sdfChipAllocDirect(s32 aSize);
void sdfChipFreeDirect(void *aPtr);
void dds3AdminiCallSequence(s32 sequence, void *data, s32 datasize, s32 restore);
void dds3BillboardSetScale(void *hBillboard, f32 x, f32 y);
s32 mdlGetMajorNum(mdlObj_t *aHandle);
s32 mdlGetMinorNum(mdlObj_t *aHandle);
mdlObj_t *mdlLoad(u32 aMajor, u32 aMinor);
void dds3EffectPartModelInit(mdlObj_t *hModel);
effPartHandle_t *dds3EffectPartCreate(u16 type, void *pData);
u32 dds3EffectPCPDataGetKateg(dds3EffectPCPData_t *hPCPData);
u32 dds3EffectPCPDataGetParam(dds3EffectPCPData_t *hPCPData);
u32 dds3EffectPCPDataGetType(dds3EffectPCPData_t *hPCPData, u32 number);
void *dds3EffectPCPDataGetData(dds3EffectPCPData_t *hPCPData, u32 number);


typedef struct
{
    u8 unknown00[0x20];
    f32 unknown20;
} effPCPThunderSpherePolyLowParam_t;

/* One entry of an effBTLSystem_*_PartList table. */
typedef struct
{
    u16 type;
    char *fileName;
    void *pData;
    u16 refCount;
    effPartHandle_t *hPart;
    effBTL_t *hBTLEffect;
    sdfMemHandle_t *hMem;
} effBTLSystem_Part_t;

extern effBTLSystem_Part_t effBTLSystem_Freeze_PartList[];
extern effBTLSystem_Part_t effBTLSystem_Sleep_PartList[];
extern effBTLSystem_Part_t effBTLSystem_Panic_PartList[];
extern effBTLSystem_Part_t effBTLSystem_Bind_PartList[];
extern effBTLSystem_Part_t effBTLSystem_Poison_PartList[];
extern effBTLSystem_Part_t effBTLSystem_Charm_PartList[];
extern effBTLSystem_Part_t effBTLSystem_Curse_PartList[];
extern effBTLSystem_Part_t effBTLSystem_CurseCAMP_PartList[];
extern effBTLSystem_Part_t effBTLSystem_Stone_Dead_PartList[];
extern effBTLSystem_Part_t effBTLSystem_Hansya_PartList[];
extern effBTLSystem_Part_t effBTLSystem_Kyusyuu_PartList[];
extern effBTLSystem_Part_t effBTLSystem_Return_PartList[];
extern effBTLSystem_Part_t effBTLSystem_Attack_PartList[];
extern effBTLSystem_Part_t effBTLSystem_AttackC_PartList[];
extern effBTLSystem_Part_t effBTLSystem_Encount_PartList[];
extern effBTLSystem_Part_t effBTLSystem_Nego_Kaihuku_PartList[];
extern effBTLSystem_Part_t effBTLSystem_Nego_Kyusyu_PartList[];
extern effBTLSystem_Part_t effBTLSystem_Nego_Stone_PartList[];

extern u8 effBTLSystem_Shock_ThunderSpherePolyLowParam[];
extern u8 effBTLSystem_Bind_ThunderSpherePolyLowParam[];
extern u8 effBTLSystem_Paralyze_ThunderSpherePolyLowParam[];

/* One effect family of the effBTLSystem table, indexed by the family id. */
typedef struct
{
    void *(*create)(nbFormation_t *);
    void (*destroy)(void *);
    void (*destroy2)(void *);
    void (*setColor)(void *, u32);
    void (*draw)(void *);
    u32 (*getLength)(void *);
    void (*dataLoad)(void);
    void (*dataFree)(void);
    void (*partHandleCreate)(void);
    void (*partHandleDelete)(void);
    u32 rgba;
    s8 active;
    u8 reserve2D[3];
    s32 unknown30;
    effBTLSystem_Part_t *pPartList;
} effBTLSystemFunc_t;

typedef struct
{
    s32 family;
    void *hEffect;
    nbFormation_t *pFormation;
} effBTLSystemHandle_t;

extern effBTLSystemFunc_t D_0031BF00[];

void effBTLSystemCut1DataFree(void);
void effBTLSystemCut1PartHandleCreate(void);
void effBTLSystemCut1PartHandleDelete(void);

/* One shared cut effect of the effBTLSystemCut1 table. */
typedef struct
{
    char *fileName;
    effBTL_t *hBTLEffect;
    s32 refCount;
    void *pData;
    sdfMemHandle_t *hMem;
} effBTLSystemCut1Data_t;

extern effBTLSystemCut1Data_t effBTLSystemCut1Data[];

void *effBTLCutGetUserData(void *hCut);
void effBTLCutSetUserData(void *hCut, void *pUserData);

extern u32 effBTLPointLightProcess;
extern s8 effBTLFieldColorOn;
extern s8 D_0054F0F1;
extern s32 D_0054F0F4;
extern u8 effBTLDebugCameraAllEdit;
extern u8 effBTLDebugCameraAllTable;
extern u8 D_0054F1C3;
extern s8 effBTLSkillActive;
extern void *nbHelperViewer;
extern u32 dds3DebugSign;
extern u8 D_0054F1DF;
extern s32 D_0054F130;
extern s8 D_0054F134;
extern u8 D_0031D000[];
extern void *D_00551DA0;
extern void *D_00551DA4;
void dds3EffectFileListWindowDelete(void *window);
void dds3EffectFileListDelete(void *list);
extern char D_0054F100[];
extern char effBedNameBuf[];

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


void *dds3EffectFileListCreate(const char *path, s32 max);
void *dds3EffectFileListWindowCreate(void *hFileList);
void dds3EffectFileListWindowSetPos(void *hWindow, s32 x, s32 y);
void dds3EffectFileListWindowSetOt(void *hWindow, s32 ot);
void dds3EffectFileListWindowSetPadWait(void *hWindow, s32 wait);
void dds3EffectFileListWindowSetWindowColor(void *hWindow, u32 color, u32 frameColor);
void func_001750D0(s32 param);
void *dblurUpdateHandle(void *hBlur);

/* A part whose type is this one is an effBTL cut, not a dds3 effect part. */
#define EFFBTL_PART_TYPE_BTL 0xFFFD

/* Effect handle shared by the families that hold one part and a frame count. */
typedef struct
{
    nbFormation_t *pFormation;
    u32 frame;
    effPartHandle_t *hPart;
} effBTLSysPart_t;

void sdfMemFreeDirect(sdfMemHandle_t *hMem);
void effBTLModelGetMarkPosition2(nbFormation_t *pFormation, s32 mark);
void effBTLModelGetMarkPosition3(mdlObj_t *hModel, s32 mark);
void mdlSetClutColor(mdlObj_t *hModel, u32 color, f32 ratio);
void *effBTLBindCreateTrack(void *pParam);
void effPCPThunderSpherePolyLowDelete(void *hTrack);
void effPCPThunderSpherePolyLowDraw(void *hTrack);
void effPCPThunderSpherePolyLowSetPosition(void *hTrack, sdfVector4_t *pPosition);
void effPCPThunderSpherePolyLowSetScale(void *hTrack, f32 scale);
void effPCPThunderSpherePolyLowSetColor(void *hTrack, u32 color);
effPCPThunderSpherePolyLowParam_t *effPCPThunderSpherePolyLowGetParam(void *hTrack);

/* Effect handle of the families that hold one effPCP track and a frame count. */
typedef struct
{
    nbFormation_t *pFormation;
    u32 frame;
    void *hTrack;
} effBTLSysTrack_t;

/* Effect handle of the families that only recolour the unit model. */
typedef struct
{
    nbFormation_t *pFormation;
    u32 frame;
} effBTLSysFrame_t;

/* Effect handle of the Poison family, which keeps the unit model it marks. */
typedef struct
{
    nbFormation_t *pFormation;
    u32 frame;
    effPartHandle_t *hPart;
    mdlObj_t *hModel;
} effBTLSysPoison_t;

/* Effect handle of the families that hold two parts. */
typedef struct
{
    nbFormation_t *pFormation;
    u32 frame;
    effPartHandle_t *hPart;
    effPartHandle_t *hPart2;
} effBTLSysPart2_t;

/* Effect handle of the Close family: two parts plus the unit model. */
typedef struct
{
    nbFormation_t *pFormation;
    u32 frame;
    effPartHandle_t *hPart;
    effPartHandle_t *hPart2;
    mdlObj_t *hModel;
} effBTLSysClose_t;

void dds3SpPoly_RollRingSetEndless(void *hRollRing);

/* Effect handle of the families that play one battle cut. */
typedef struct
{
    nbFormation_t *pFormation;
    u32 frame;
    void *hCut;
} effBTLSysCut_t;

s32 effBTLSkillGetID(void);

/* Effect handle of the Hansya family, two parts behind its own work area. */
typedef struct
{
    u8 reserve00[0xE0];
    nbFormation_t *pFormation;
    u32 frame;
    effPartHandle_t *hPart;
    effPartHandle_t *hPart2;
} effBTLSysHansya_t;

/* Effect handle of the Kyusyuu family, four parts. */
typedef struct
{
    nbFormation_t *pFormation;
    u32 frame;
    effPartHandle_t *hPart;
    effPartHandle_t *hPart2;
    effPartHandle_t *hPart3;
    effPartHandle_t *hPart4;
} effBTLSysPart4_t;

/* Effect handle of the Attack family, six parts. */
typedef struct
{
    nbFormation_t *pFormation;
    u32 frame;
    effPartHandle_t *hPart;
    effPartHandle_t *hPart2;
    effPartHandle_t *hPart3;
    effPartHandle_t *hPart4;
    effPartHandle_t *hPart5;
    effPartHandle_t *hPart6;
} effBTLSysPart6_t;

/* Effect handle of the AttackC family, the Attack parts plus six of its own. */
typedef struct
{
    nbFormation_t *pFormation;
    u32 frame;
    effPartHandle_t *hPart;
    effPartHandle_t *hPart2;
    effPartHandle_t *hPart3;
    effPartHandle_t *hPart4;
    effPartHandle_t *hPart5;
    effPartHandle_t *hPart6;
    effPartHandle_t *hPart7;
    effPartHandle_t *hPart8;
    effPartHandle_t *hPart9;
    effPartHandle_t *hPart10;
    effPartHandle_t *hPart11;
    effPartHandle_t *hPart12;
} effBTLSysPart12_t;

/* Effect handle of the Stone_Dead family, which holds three parts. */
typedef struct
{
    nbFormation_t *pFormation;
    u32 frame;
    effPartHandle_t *hPart;
    effPartHandle_t *hPart2;
    effPartHandle_t *hPart3;
} effBTLSysPart3_t;

void effBTLCutDraw(void *hCut);
u32 effBTLCutGetCutEndFrame(void *hCut);
void effBTLModelHide(nbFormation_t *pFormation);
void mdlSingleMotion(void *hModel, s32 motion, s32 flag);
void effBTLSystem_Part_DataLoad(effBTLSystem_Part_t *pPartList, u32 count);
void effBTLSystem_Part_DataFree(effBTLSystem_Part_t *pPartList, u32 count);
void effBTLSystem_Part_HandleCreate(effBTLSystem_Part_t *pPartList, u32 count);
void effBTLSystem_Part_HandleDelete(effBTLSystem_Part_t *pPartList, u32 count);
void *dds3EffectPartGetHandle(effPartHandle_t *hEffPart);
u16 dds3EffectPartGetType(effPartHandle_t *hEffPart);
u32 dds3EffectPCPDataGetSize(dds3EffectPCPData_t *hPCPData, u32 number);
void dds3EffectPartDelete(effPartHandle_t *hEffPart);
effPartHandle_t *dds3EffectPartCopy(effPartHandle_t *hEffPart);
void dds3EffectPartDraw(effPartHandle_t *hEffPart);
void dds3EffectPartSetPosition(effPartHandle_t *hEffPart, sdfVector4_t *pPosition);
void dds3EffectPartSetScale(effPartHandle_t *hEffPart, f32 scale);
void dds3EffectPartSetColor(effPartHandle_t *hEffPart, u32 color);
void effBTLSystem_SetUnitColorRGB(nbFormation_t *pFormation, s32 frame, u32 rgb);
f32 effBTLSystem_GetScalePtr(nbFormation_t *pFormation);
f32 effBTLPartGetScalePtr(nbFormation_t *pFormation);
f32 effBTLPartCalcScale(nbFormation_t *pFormation);

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
                SDF_VU0__COPYVECTOR(pBase, &hUnit->vec10);
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

INCLUDE_ASM(const s32, "code_0016F490", effBTLPointLightMain);

INCLUDE_ASM(const s32, "code_0016F490", effBTLModelColorMain);

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

void effBTLSystem_SetUnitColorRGB(nbFormation_t *pFormation, s32 frame, u32 rgb)
{
    if (pFormation->hunit != NULL) {
        evtSetUnitColorRGB(pFormation->hunit, frame, rgb);
        pFormation->modelcol = (pFormation->modelcol & 0xFF000000) | (rgb & 0xFFFFFF);
    }
}

f32 effBTLSystem_GetScalePtr(nbFormation_t *pFormation)
{
    if (pFormation->hunit != NULL) {
        return effBTLPartGetScalePtr(pFormation);
    } else {
        return effBTLPartCalcScale(pFormation);
    }
}

INCLUDE_ASM(const s32, "code_0016F490", effBTLSystem_Part_DataLoad);

void effBTLSystem_Part_DataFree(effBTLSystem_Part_t *pPartList, u32 count)
{
    u32 i;

    for (i = 0; i < count; i++) {
        sdfMemFreeDirect(pPartList->hMem);
        pPartList++;
    }
}

void effBTLSystem_Part_HandleCreate(effBTLSystem_Part_t *pPartList, u32 count)
{
    u32 i;

    for (i = 0; i < count; i++) {
        if (pPartList->refCount == 0) {
            if (pPartList->type != EFFBTL_PART_TYPE_BTL) {
                pPartList->hPart = dds3EffectPartCreate(pPartList->type, pPartList->pData);
            } else {
                pPartList->hBTLEffect = effBTLCreate(pPartList->pData);
            }
        }
        pPartList->refCount++;
        pPartList++;
    }
}

void effBTLSystem_Part_HandleDelete(effBTLSystem_Part_t *pPartList, u32 count)
{
    u32 i;

    for (i = 0; i < count; i++) {
        if (--pPartList->refCount == 0) {
            if (pPartList->type != EFFBTL_PART_TYPE_BTL) {
                dds3EffectPartDelete(pPartList->hPart);
            } else {
                effBTLDelete(pPartList->hBTLEffect);
            }
        }
        pPartList++;
    }
}

effBTLSysTrack_t *effBTLSys_Shock_Create(nbFormation_t *pFormation)
{
    effBTLSysTrack_t *hEffect;
    effPCPThunderSpherePolyLowParam_t *pParam;

    hEffect = sdfChipAllocDirect(sizeof(effBTLSysTrack_t));
    hEffect->pFormation = pFormation;
    hEffect->frame = 0;
    hEffect->hTrack = effBTLBindCreateTrack(effBTLSystem_Shock_ThunderSpherePolyLowParam);
    effPCPThunderSpherePolyLowSetScale(hEffect->hTrack, effBTLSystem_GetScalePtr(hEffect->pFormation));
    pParam = effPCPThunderSpherePolyLowGetParam(hEffect->hTrack);
    pParam->unknown20 = hEffect->pFormation->h * hEffect->pFormation->scl * 0.85f;
    return hEffect;
}

void effBTLSys_Shock_Delete(effBTLSysTrack_t *hEffect)
{
    effBTLSystem_SetUnitColorRGB(hEffect->pFormation, 4, 0x80808080);
    effPCPThunderSpherePolyLowDelete(hEffect->hTrack);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Shock_Delete2(effBTLSysTrack_t *hEffect)
{
    effPCPThunderSpherePolyLowDelete(hEffect->hTrack);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Shock_SetColor(effBTLSysTrack_t *hEffect, u32 color)
{
    effPCPThunderSpherePolyLowSetColor(hEffect->hTrack, color);
}

void effBTLSys_Shock_Draw(effBTLSysTrack_t *hEffect)
{
    sdfVector4_t position;

    if (hEffect->frame == 0) {
        effBTLSystem_SetUnitColorRGB(hEffect->pFormation, 4, 0x80FFCDCD);
    }
    effBTLModelGetMarkPosition2(hEffect->pFormation, 0);
    VU0_STORE_VF10_TO(position);
    effPCPThunderSpherePolyLowSetPosition(hEffect->hTrack, &position);
    effPCPThunderSpherePolyLowDraw(hEffect->hTrack);
    hEffect->frame++;
}

u32 effBTLSys_Shock_GetLength(void *hEffect)
{
    return 0;
}

void effBTLSys_Shock_DataLoad(void)
{
    effBTLSystem_Part_DataLoad(effBTLSystem_Freeze_PartList, 0);
}

void effBTLSys_Shock_DataFree(void)
{
    effBTLSystem_Part_DataFree(effBTLSystem_Freeze_PartList, 0);
}

void effBTLSys_Shock_PartHandleCreate(void)
{
    effBTLSystem_Part_HandleCreate(effBTLSystem_Freeze_PartList, 0);
}

void effBTLSys_Shock_PartHandleDelete(void)
{
    effBTLSystem_Part_HandleDelete(effBTLSystem_Freeze_PartList, 0);
}

effBTLSysPart_t *effBTLSys_Freeze_Create(nbFormation_t *pFormation)
{
    effBTLSysPart_t *hEffect;

    hEffect = sdfChipAllocDirect(sizeof(effBTLSysPart_t));
    hEffect->pFormation = pFormation;
    hEffect->frame = 0;
    hEffect->hPart = dds3EffectPartCopy(effBTLSystem_Freeze_PartList[0].hPart);
    dds3EffectPartSetScale(hEffect->hPart, effBTLSystem_GetScalePtr(hEffect->pFormation));
    return hEffect;
}

void effBTLSys_Freeze_Delete(effBTLSysPart_t *hEffect)
{
    hEffect->pFormation->hunit->owner->flags &= ~2;
    effBTLSystem_SetUnitColorRGB(hEffect->pFormation, 0, 0x80808080);
    mdlSetClutColor(hEffect->pFormation->hunit->owner, 0x808080, 0.0f);
    dds3EffectPartDelete(hEffect->hPart);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Freeze_Delete2(effBTLSysPart_t *hEffect)
{
    dds3EffectPartDelete(hEffect->hPart);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Freeze_SetColor(effBTLSysPart_t *hEffect, u32 color)
{
    dds3EffectPartSetColor(hEffect->hPart, color);
}

void effBTLSys_Freeze_Draw(effBTLSysPart_t *hEffect)
{
    sdfVector4_t position;

    if (hEffect->frame == 0) {
        effBTLSystem_SetUnitColorRGB(hEffect->pFormation, 0, 0x80FFF8BC);
        mdlSetClutColor(hEffect->pFormation->hunit->owner, 0x80FFC39B, 1.0f);
        hEffect->pFormation->hunit->owner->flags |= 2;
    }
    effBTLModelGetMarkPosition2(hEffect->pFormation, 0);
    VU0_STORE_VF10_TO(position);
    dds3EffectPartSetPosition(hEffect->hPart, &position);
    dds3EffectPartDraw(hEffect->hPart);
    hEffect->frame++;
}

u32 effBTLSys_Freeze_GetLength(void *hEffect)
{
    return 0;
}

void effBTLSys_Freeze_DataLoad(void)
{
    effBTLSystem_Part_DataLoad(effBTLSystem_Freeze_PartList, 1);
}

void effBTLSys_Freeze_DataFree(void)
{
    effBTLSystem_Part_DataFree(effBTLSystem_Freeze_PartList, 1);
}

void effBTLSys_Freeze_PartHandleCreate(void)
{
    effBTLSystem_Part_HandleCreate(effBTLSystem_Freeze_PartList, 1);
}

void effBTLSys_Freeze_PartHandleDelete(void)
{
    effBTLSystem_Part_HandleDelete(effBTLSystem_Freeze_PartList, 1);
}

effBTLSysPart_t *effBTLSys_Sleep_Create(nbFormation_t *pFormation)
{
    effBTLSysPart_t *hEffect;

    hEffect = sdfChipAllocDirect(sizeof(effBTLSysPart_t));
    hEffect->pFormation = pFormation;
    hEffect->frame = 0;
    hEffect->hPart = dds3EffectPartCopy(effBTLSystem_Sleep_PartList[0].hPart);
    dds3EffectPartSetScale(hEffect->hPart, effBTLSystem_GetScalePtr(hEffect->pFormation));
    return hEffect;
}

void effBTLSys_Sleep_Delete(effBTLSysPart_t *hEffect)
{
    effBTLSystem_SetUnitColorRGB(hEffect->pFormation, 4, 0x80808080);
    dds3EffectPartDelete(hEffect->hPart);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Sleep_Delete2(effBTLSysPart_t *hEffect)
{
    dds3EffectPartDelete(hEffect->hPart);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Sleep_SetColor(effBTLSysPart_t *hEffect, u32 color)
{
    dds3EffectPartSetColor(hEffect->hPart, color);
}

void effBTLSys_Sleep_Draw(effBTLSysPart_t *hEffect)
{
    sdfVector4_t position;

    if (hEffect->frame == 0) {
        effBTLSystem_SetUnitColorRGB(hEffect->pFormation, 4, 0x80FFB97D);
    }
    effBTLModelGetMarkPosition2(hEffect->pFormation, 2);
    VU0_STORE_VF10_TO(position);
    dds3EffectPartSetPosition(hEffect->hPart, &position);
    dds3EffectPartDraw(hEffect->hPart);
    hEffect->frame++;
}

u32 effBTLSys_Sleep_GetLength(void *hEffect)
{
    return 0;
}

void effBTLSys_Sleep_DataLoad(void)
{
    effBTLSystem_Part_DataLoad(effBTLSystem_Sleep_PartList, 1);
}

void effBTLSys_Sleep_DataFree(void)
{
    effBTLSystem_Part_DataFree(effBTLSystem_Sleep_PartList, 1);
}

void effBTLSys_Sleep_PartHandleCreate(void)
{
    effBTLSystem_Part_HandleCreate(effBTLSystem_Sleep_PartList, 1);
}

void effBTLSys_Sleep_PartHandleDelete(void)
{
    effBTLSystem_Part_HandleDelete(effBTLSystem_Sleep_PartList, 1);
}

effBTLSysPart_t *effBTLSys_Panic_Create(nbFormation_t *pFormation)
{
    effBTLSysPart_t *hEffect;

    hEffect = sdfChipAllocDirect(sizeof(effBTLSysPart_t));
    hEffect->pFormation = pFormation;
    hEffect->frame = 0;
    hEffect->hPart = dds3EffectPartCopy(effBTLSystem_Panic_PartList[0].hPart);
    dds3EffectPartSetScale(hEffect->hPart, effBTLSystem_GetScalePtr(hEffect->pFormation));
    return hEffect;
}

void effBTLSys_Panic_Delete(effBTLSysPart_t *hEffect)
{
    effBTLSystem_SetUnitColorRGB(hEffect->pFormation, 4, 0x80808080);
    dds3EffectPartDelete(hEffect->hPart);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Panic_Delete2(effBTLSysPart_t *hEffect)
{
    dds3EffectPartDelete(hEffect->hPart);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Panic_SetColor(effBTLSysPart_t *hEffect, u32 color)
{
    dds3EffectPartSetColor(hEffect->hPart, color);
}

void effBTLSys_Panic_Draw(effBTLSysPart_t *hEffect)
{
    sdfVector4_t position;

    if (hEffect->frame == 0) {
        effBTLSystem_SetUnitColorRGB(hEffect->pFormation, 4, 0x8058B8FF);
    }
    effBTLModelGetMarkPosition2(hEffect->pFormation, 2);
    VU0_STORE_VF10_TO(position);
    dds3EffectPartSetPosition(hEffect->hPart, &position);
    dds3EffectPartDraw(hEffect->hPart);
    hEffect->frame++;
}

u32 effBTLSys_Panic_GetLength(void *hEffect)
{
    return 0;
}

void effBTLSys_Panic_DataLoad(void)
{
    effBTLSystem_Part_DataLoad(effBTLSystem_Panic_PartList, 1);
}

void effBTLSys_Panic_DataFree(void)
{
    effBTLSystem_Part_DataFree(effBTLSystem_Panic_PartList, 1);
}

void effBTLSys_Panic_PartHandleCreate(void)
{
    effBTLSystem_Part_HandleCreate(effBTLSystem_Panic_PartList, 1);
}

void effBTLSys_Panic_PartHandleDelete(void)
{
    effBTLSystem_Part_HandleDelete(effBTLSystem_Panic_PartList, 1);
}

effBTLSysTrack_t *effBTLSys_Bind_Create(nbFormation_t *pFormation)
{
    effBTLSysTrack_t *hEffect;

    hEffect = sdfChipAllocDirect(sizeof(effBTLSysTrack_t));
    hEffect->pFormation = pFormation;
    hEffect->frame = 0;
    hEffect->hTrack = effBTLBindCreateTrack(effBTLSystem_Bind_ThunderSpherePolyLowParam);
    effPCPThunderSpherePolyLowSetScale(hEffect->hTrack, effBTLSystem_GetScalePtr(hEffect->pFormation));
    return hEffect;
}

void effBTLSys_Bind_Delete(effBTLSysTrack_t *hEffect)
{
    effBTLSystem_SetUnitColorRGB(hEffect->pFormation, 4, 0x80808080);
    effPCPThunderSpherePolyLowDelete(hEffect->hTrack);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Bind_Delete2(effBTLSysTrack_t *hEffect)
{
    effPCPThunderSpherePolyLowDelete(hEffect->hTrack);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Bind_SetColor(effBTLSysTrack_t *hEffect, u32 color)
{
    effPCPThunderSpherePolyLowSetColor(hEffect->hTrack, color);
}

void effBTLSys_Bind_Draw(effBTLSysTrack_t *hEffect)
{
    sdfVector4_t position;

    if (hEffect->frame == 0) {
        effBTLSystem_SetUnitColorRGB(hEffect->pFormation, 4, 0x8080FFD7);
    }
    effBTLModelGetMarkPosition2(hEffect->pFormation, 0);
    VU0_STORE_VF10_TO(position);
    effPCPThunderSpherePolyLowSetPosition(hEffect->hTrack, &position);
    effPCPThunderSpherePolyLowDraw(hEffect->hTrack);
    hEffect->frame++;
}

u32 effBTLSys_Bind_GetLength(void *hEffect)
{
    return 0;
}

void effBTLSys_Bind_DataLoad(void)
{
    effBTLSystem_Part_DataLoad(effBTLSystem_Bind_PartList, 0);
}

void effBTLSys_Bind_DataFree(void)
{
    effBTLSystem_Part_DataFree(effBTLSystem_Bind_PartList, 0);
}

void effBTLSys_Bind_PartHandleCreate(void)
{
    effBTLSystem_Part_HandleCreate(effBTLSystem_Bind_PartList, 0);
}

void effBTLSys_Bind_PartHandleDelete(void)
{
    effBTLSystem_Part_HandleDelete(effBTLSystem_Bind_PartList, 0);
}

effBTLSysClose_t *effBTLSys_Close_Create(nbFormation_t *pFormation)
{
    effBTLSysClose_t *hEffect;
    f32 scale;

    hEffect = sdfChipAllocDirect(sizeof(effBTLSysClose_t));
    hEffect->pFormation = pFormation;
    hEffect->frame = 0;
    if (pFormation->hunit != NULL) {
        hEffect->hModel = pFormation->hunit->owner;
    } else {
        hEffect->hModel = NULL;
    }
    hEffect->hPart = dds3EffectPartCopy(effBTLSystem_Bind_PartList[0].hPart);
    hEffect->hPart2 = dds3EffectPartCopy(effBTLSystem_Bind_PartList[1].hPart);
    scale = effBTLSystem_GetScalePtr(hEffect->pFormation);
    dds3EffectPartSetScale(hEffect->hPart, scale);
    dds3EffectPartSetScale(hEffect->hPart2, scale);
    return hEffect;
}

void effBTLSys_Close_Delete(effBTLSysClose_t *hEffect)
{
    effBTLSystem_SetUnitColorRGB(hEffect->pFormation, 4, 0x80808080);
    dds3EffectPartDelete(hEffect->hPart2);
    dds3EffectPartDelete(hEffect->hPart);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Close_Delete2(effBTLSysClose_t *hEffect)
{
    dds3EffectPartDelete(hEffect->hPart);
    dds3EffectPartDelete(hEffect->hPart2);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Close_SetColor(effBTLSysClose_t *hEffect, u32 color)
{
    dds3EffectPartSetColor(hEffect->hPart, color);
    dds3EffectPartSetColor(hEffect->hPart2, color);
}

void effBTLSys_Close_Draw(effBTLSysClose_t *hEffect)
{
    sdfVector4_t position;

    if (hEffect->frame == 0) {
        effBTLSystem_SetUnitColorRGB(hEffect->pFormation, 4, 0x80FFCDCD);
    }
    effBTLModelGetMarkPosition3(hEffect->hModel, 2);
    VU0_STORE_VF10_TO(position);
    dds3EffectPartSetPosition(hEffect->hPart, &position);
    dds3EffectPartSetPosition(hEffect->hPart2, &position);
    dds3SpPoly_RollRingSetEndless(((dds3EffectPCP_t *)dds3EffectPartGetHandle(hEffect->hPart))->pHandle);
    dds3SpPoly_RollRingSetEndless(((dds3EffectPCP_t *)dds3EffectPartGetHandle(hEffect->hPart2))->pHandle);
    dds3EffectPartDraw(hEffect->hPart);
    dds3EffectPartDraw(hEffect->hPart2);
    hEffect->frame++;
}

u32 effBTLSys_Close_GetLength(void *hEffect)
{
    return 0;
}

void effBTLSys_Close_DataLoad(void)
{
    effBTLSystem_Part_DataLoad(effBTLSystem_Bind_PartList, 2);
}

void effBTLSys_Close_DataFree(void)
{
    effBTLSystem_Part_DataFree(effBTLSystem_Bind_PartList, 2);
}

void effBTLSys_Close_PartHandleCreate(void)
{
    effBTLSystem_Part_HandleCreate(effBTLSystem_Bind_PartList, 2);
}

void effBTLSys_Close_PartHandleDelete(void)
{
    effBTLSystem_Part_HandleDelete(effBTLSystem_Bind_PartList, 2);
}

effBTLSysPoison_t *effBTLSys_Poison_Create(nbFormation_t *pFormation)
{
    effBTLSysPoison_t *hEffect;

    hEffect = sdfChipAllocDirect(sizeof(effBTLSysPoison_t));
    hEffect->pFormation = pFormation;
    hEffect->frame = 0;
    if (pFormation->hunit != NULL) {
        hEffect->hModel = pFormation->hunit->owner;
    } else {
        hEffect->hModel = NULL;
    }
    hEffect->hPart = dds3EffectPartCopy(effBTLSystem_Poison_PartList[0].hPart);
    dds3EffectPartSetScale(hEffect->hPart, effBTLSystem_GetScalePtr(hEffect->pFormation));
    return hEffect;
}

void effBTLSys_Poison_Delete(effBTLSysPoison_t *hEffect)
{
    effBTLSystem_SetUnitColorRGB(hEffect->pFormation, 4, 0x80808080);
    dds3EffectPartDelete(hEffect->hPart);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Poison_Delete2(effBTLSysPoison_t *hEffect)
{
    dds3EffectPartDelete(hEffect->hPart);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Poison_SetColor(effBTLSysPoison_t *hEffect, u32 color)
{
    dds3EffectPartSetColor(hEffect->hPart, color);
}

void effBTLSys_Poison_Draw(effBTLSysPoison_t *hEffect)
{
    sdfVector4_t position;

    if (hEffect->frame == 0) {
        effBTLSystem_SetUnitColorRGB(hEffect->pFormation, 4, 0x80A53A6C);
    }
    effBTLModelGetMarkPosition3(hEffect->hModel, 2);
    VU0_STORE_VF10_TO(position);
    dds3EffectPartSetPosition(hEffect->hPart, &position);
    dds3EffectPartDraw(hEffect->hPart);
    hEffect->frame++;
}

u32 effBTLSys_Poison_GetLength(void *hEffect)
{
    return 0;
}

void effBTLSys_Poison_DataLoad(void)
{
    effBTLSystem_Part_DataLoad(effBTLSystem_Poison_PartList, 1);
}

void effBTLSys_Poison_DataFree(void)
{
    effBTLSystem_Part_DataFree(effBTLSystem_Poison_PartList, 1);
}

void effBTLSys_Poison_PartHandleCreate(void)
{
    effBTLSystem_Part_HandleCreate(effBTLSystem_Poison_PartList, 1);
}

void effBTLSys_Poison_PartHandleDelete(void)
{
    effBTLSystem_Part_HandleDelete(effBTLSystem_Poison_PartList, 1);
}

effBTLSysPart2_t *effBTLSys_Charm_Create(nbFormation_t *pFormation)
{
    effBTLSysPart2_t *hEffect;
    f32 scale;

    hEffect = sdfChipAllocDirect(sizeof(effBTLSysPart2_t));
    hEffect->pFormation = pFormation;
    hEffect->frame = 0;
    hEffect->hPart = dds3EffectPartCopy(effBTLSystem_Charm_PartList[0].hPart);
    hEffect->hPart2 = dds3EffectPartCopy(effBTLSystem_Charm_PartList[1].hPart);
    scale = effBTLSystem_GetScalePtr(hEffect->pFormation);
    dds3EffectPartSetScale(hEffect->hPart, scale);
    dds3EffectPartSetScale(hEffect->hPart2, scale);
    return hEffect;
}

void effBTLSys_Charm_Delete(effBTLSysPart2_t *hEffect)
{
    effBTLSystem_SetUnitColorRGB(hEffect->pFormation, 4, 0x80808080);
    dds3EffectPartDelete(hEffect->hPart);
    dds3EffectPartDelete(hEffect->hPart2);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Charm_Delete2(effBTLSysPart2_t *hEffect)
{
    dds3EffectPartDelete(hEffect->hPart);
    dds3EffectPartDelete(hEffect->hPart2);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Charm_SetColor(effBTLSysPart2_t *hEffect, u32 color)
{
    dds3EffectPartSetColor(hEffect->hPart, color);
    dds3EffectPartSetColor(hEffect->hPart2, color);
}

void effBTLSys_Charm_Draw(effBTLSysPart2_t *hEffect)
{
    sdfVector4_t position;

    if (hEffect->frame == 0) {
        effBTLSystem_SetUnitColorRGB(hEffect->pFormation, 4, 0x808064FF);
    }
    effBTLModelGetMarkPosition2(hEffect->pFormation, 2);
    VU0_STORE_VF10_TO(position);
    dds3EffectPartSetPosition(hEffect->hPart, &position);
    dds3EffectPartSetPosition(hEffect->hPart2, &position);
    dds3EffectPartDraw(hEffect->hPart);
    dds3EffectPartDraw(hEffect->hPart2);
    hEffect->frame++;
}

u32 effBTLSys_Charm_GetLength(void *hEffect)
{
    return 0;
}

void effBTLSys_Charm_DataLoad(void)
{
    effBTLSystem_Part_DataLoad(effBTLSystem_Charm_PartList, 2);
}

void effBTLSys_Charm_DataFree(void)
{
    effBTLSystem_Part_DataFree(effBTLSystem_Charm_PartList, 2);
}

void effBTLSys_Charm_PartHandleCreate(void)
{
    effBTLSystem_Part_HandleCreate(effBTLSystem_Charm_PartList, 2);
}

void effBTLSys_Charm_PartHandleDelete(void)
{
    effBTLSystem_Part_HandleDelete(effBTLSystem_Charm_PartList, 2);
}

effBTLSysTrack_t *effBTLSys_Paralyze_Create(nbFormation_t *pFormation)
{
    effBTLSysTrack_t *hEffect;
    effPCPThunderSpherePolyLowParam_t *pParam;

    hEffect = sdfChipAllocDirect(sizeof(effBTLSysTrack_t));
    hEffect->pFormation = pFormation;
    hEffect->frame = 0;
    hEffect->hTrack = effBTLBindCreateTrack(effBTLSystem_Paralyze_ThunderSpherePolyLowParam);
    effPCPThunderSpherePolyLowSetScale(hEffect->hTrack, effBTLSystem_GetScalePtr(hEffect->pFormation));
    pParam = effPCPThunderSpherePolyLowGetParam(hEffect->hTrack);
    pParam->unknown20 = hEffect->pFormation->h * hEffect->pFormation->scl * 0.85f;
    return hEffect;
}

void effBTLSys_Paralyze_Delete(effBTLSysTrack_t *hEffect)
{
    effBTLSystem_SetUnitColorRGB(hEffect->pFormation, 4, 0x80808080);
    effPCPThunderSpherePolyLowDelete(hEffect->hTrack);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Paralyze_Delete2(effBTLSysTrack_t *hEffect)
{
    effPCPThunderSpherePolyLowDelete(hEffect->hTrack);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Paralyze_SetColor(effBTLSysTrack_t *hEffect, u32 color)
{
    effPCPThunderSpherePolyLowSetColor(hEffect->hTrack, color);
}

void effBTLSys_Paralyze_Draw(effBTLSysTrack_t *hEffect)
{
    sdfVector4_t position;

    if (hEffect->frame == 0) {
        effBTLSystem_SetUnitColorRGB(hEffect->pFormation, 4, 0x80589BFF);
    }
    effBTLModelGetMarkPosition2(hEffect->pFormation, 0);
    VU0_STORE_VF10_TO(position);
    effPCPThunderSpherePolyLowSetPosition(hEffect->hTrack, &position);
    effPCPThunderSpherePolyLowDraw(hEffect->hTrack);
    hEffect->frame++;
}

u32 effBTLSys_Paralyze_GetLength(void *hEffect)
{
    return 0;
}

void effBTLSys_Paralyze_DataLoad(void)
{
    effBTLSystem_Part_DataLoad(effBTLSystem_Curse_PartList, 0);
}

void effBTLSys_Paralyze_DataFree(void)
{
    effBTLSystem_Part_DataFree(effBTLSystem_Curse_PartList, 0);
}

void effBTLSys_Paralyze_PartHandleCreate(void)
{
    effBTLSystem_Part_HandleCreate(effBTLSystem_Curse_PartList, 0);
}

void effBTLSys_Paralyze_PartHandleDelete(void)
{
    effBTLSystem_Part_HandleDelete(effBTLSystem_Curse_PartList, 0);
}

effBTLSysFrame_t *effBTLSys_Fly_Create(nbFormation_t *pFormation)
{
    effBTLSysFrame_t *hEffect;

    hEffect = sdfChipAllocDirect(sizeof(effBTLSysFrame_t));
    hEffect->pFormation = pFormation;
    hEffect->frame = 0;
    return hEffect;
}

void effBTLSys_Fly_Delete(effBTLSysFrame_t *hEffect)
{
    effBTLSystem_SetUnitColorRGB(hEffect->pFormation, 4, 0x80808080);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Fly_Delete2(effBTLSysFrame_t *hEffect)
{
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Fly_SetColor(void) {
}

void effBTLSys_Fly_Draw(effBTLSysFrame_t *hEffect)
{
    if (hEffect->frame == 0) {
        effBTLSystem_SetUnitColorRGB(hEffect->pFormation, 4, 0x80FFCDCD);
    }
    hEffect->frame++;
}

u32 effBTLSys_Fly_GetLength(void *hEffect)
{
    return 0;
}

void effBTLSys_Fly_DataLoad(void)
{
    effBTLSystem_Part_DataLoad(effBTLSystem_Curse_PartList, 0);
}

void effBTLSys_Fly_DataFree(void)
{
    effBTLSystem_Part_DataFree(effBTLSystem_Curse_PartList, 0);
}

void effBTLSys_Fly_PartHandleCreate(void)
{
    effBTLSystem_Part_HandleCreate(effBTLSystem_Curse_PartList, 0);
}

void effBTLSys_Fly_PartHandleDelete(void)
{
    effBTLSystem_Part_HandleDelete(effBTLSystem_Curse_PartList, 0);
}

effBTLSysFrame_t *effBTLSys_Stone_Create(nbFormation_t *pFormation)
{
    effBTLSysFrame_t *hEffect;

    hEffect = sdfChipAllocDirect(sizeof(effBTLSysFrame_t));
    hEffect->pFormation = pFormation;
    hEffect->frame = 0;
    return hEffect;
}

void effBTLSys_Stone_Delete(effBTLSysFrame_t *hEffect)
{
    mdlObj_t *hModel = hEffect->pFormation->hunit->owner;

    hModel->flags &= ~2;
    mdlSetClutColor(hModel, 0x808080, 0.0f);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Stone_Delete2(effBTLSysFrame_t *hEffect)
{
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Stone_SetColor(void) {
}

void effBTLSys_Stone_Draw(effBTLSysFrame_t *hEffect)
{
    if (hEffect->frame == 0) {
        mdlSetClutColor(hEffect->pFormation->hunit->owner, 0x80FFEBE1, 1.0f);
        hEffect->pFormation->hunit->owner->flags |= 2;
    }
    hEffect->frame++;
}

u32 effBTLSys_Stone_GetLength(void *hEffect)
{
    return 0;
}

void effBTLSys_Stone_DataLoad(void)
{
    effBTLSystem_Part_DataLoad(effBTLSystem_Curse_PartList, 0);
}

void effBTLSys_Stone_DataFree(void)
{
    effBTLSystem_Part_DataFree(effBTLSystem_Curse_PartList, 0);
}

void effBTLSys_Stone_PartHandleCreate(void)
{
    effBTLSystem_Part_HandleCreate(effBTLSystem_Curse_PartList, 0);
}

void effBTLSys_Stone_PartHandleDelete(void)
{
    effBTLSystem_Part_HandleDelete(effBTLSystem_Curse_PartList, 0);
}

effBTLSysPart_t *effBTLSys_Curse_Create(nbFormation_t *pFormation)
{
    effBTLSysPart_t *hEffect;

    hEffect = sdfChipAllocDirect(sizeof(effBTLSysPart_t));
    hEffect->pFormation = pFormation;
    hEffect->frame = 0;
    hEffect->hPart = dds3EffectPartCopy(effBTLSystem_Curse_PartList[0].hPart);
    dds3EffectPartSetScale(hEffect->hPart, effBTLSystem_GetScalePtr(hEffect->pFormation));
    return hEffect;
}

void effBTLSys_Curse_Delete(effBTLSysPart_t *hEffect)
{
    dds3EffectPartDelete(hEffect->hPart);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Curse_Delete2(effBTLSysPart_t *hEffect)
{
    dds3EffectPartDelete(hEffect->hPart);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Curse_SetColor(effBTLSysPart_t *hEffect, u32 color)
{
    dds3EffectPartSetColor(hEffect->hPart, color);
}

void effBTLSys_Curse_Draw(effBTLSysPart_t *hEffect)
{
    dds3EffectPartSetPosition(hEffect->hPart, &hEffect->pFormation->pos);
    dds3EffectPartDraw(hEffect->hPart);
    hEffect->frame++;
}

u32 effBTLSys_Curse_GetLength(void *hEffect)
{
    return 0;
}

void effBTLSys_Curse_DataLoad(void)
{
    effBTLSystem_Part_DataLoad(effBTLSystem_Curse_PartList, 1);
}

void effBTLSys_Curse_DataFree(void)
{
    effBTLSystem_Part_DataFree(effBTLSystem_Curse_PartList, 1);
}

void effBTLSys_Curse_PartHandleCreate(void)
{
    effBTLSystem_Part_HandleCreate(effBTLSystem_Curse_PartList, 1);
}

void effBTLSys_Curse_PartHandleDelete(void)
{
    effBTLSystem_Part_HandleDelete(effBTLSystem_Curse_PartList, 1);
}

effBTLSysPart2_t *effBTLSys_CurseCAMP_Create(nbFormation_t *pFormation)
{
    effBTLSysPart2_t *hEffect;
    f32 scale;

    hEffect = sdfChipAllocDirect(sizeof(effBTLSysPart2_t));
    hEffect->pFormation = pFormation;
    hEffect->frame = 0;
    hEffect->hPart = dds3EffectPartCopy(effBTLSystem_CurseCAMP_PartList[0].hPart);
    hEffect->hPart2 = dds3EffectPartCopy(effBTLSystem_CurseCAMP_PartList[1].hPart);
    scale = effBTLSystem_GetScalePtr(hEffect->pFormation);
    dds3EffectPartSetScale(hEffect->hPart, scale);
    dds3EffectPartSetScale(hEffect->hPart2, scale);
    return hEffect;
}

void effBTLSys_CurseCAMP_Delete(effBTLSysPart2_t *hEffect)
{
    dds3EffectPartDelete(hEffect->hPart);
    dds3EffectPartDelete(hEffect->hPart2);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_CurseCAMP_Delete2(effBTLSysPart2_t *hEffect)
{
    dds3EffectPartDelete(hEffect->hPart);
    dds3EffectPartDelete(hEffect->hPart2);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_CurseCAMP_SetColor(effBTLSysPart2_t *hEffect, u32 color)
{
    dds3EffectPartSetColor(hEffect->hPart, color);
    dds3EffectPartSetColor(hEffect->hPart2, color);
}

void effBTLSys_CurseCAMP_Draw(effBTLSysPart2_t *hEffect)
{
    sdfVector4_t position;

    dds3EffectPartSetPosition(hEffect->hPart, &hEffect->pFormation->pos);
    position.x = hEffect->pFormation->pos.x;
    position.y = hEffect->pFormation->pos.y - 100.0f;
    position.z = hEffect->pFormation->pos.z;
    dds3EffectPartSetPosition(hEffect->hPart2, &position);
    dds3EffectPartDraw(hEffect->hPart);
    dds3EffectPartDraw(hEffect->hPart2);
    hEffect->frame++;
}

u32 effBTLSys_CurseCAMP_GetLength(void *hEffect)
{
    return 0;
}

void effBTLSys_CurseCAMP_DataLoad(void)
{
    effBTLSystem_Part_DataLoad(effBTLSystem_CurseCAMP_PartList, 2);
}

void effBTLSys_CurseCAMP_DataFree(void)
{
    effBTLSystem_Part_DataFree(effBTLSystem_CurseCAMP_PartList, 2);
}

void effBTLSys_CurseCAMP_PartHandleCreate(void)
{
    effBTLSystem_Part_HandleCreate(effBTLSystem_CurseCAMP_PartList, 2);
}

void effBTLSys_CurseCAMP_PartHandleDelete(void)
{
    effBTLSystem_Part_HandleDelete(effBTLSystem_CurseCAMP_PartList, 2);
}

effBTLSysPart3_t *effBTLSys_Stone_Dead_Create(nbFormation_t *pFormation)
{
    effBTLSysPart3_t *hEffect;
    f32 scale;

    hEffect = sdfChipAllocDirect(sizeof(effBTLSysPart3_t));
    hEffect->pFormation = pFormation;
    hEffect->frame = 0;
    hEffect->hPart = dds3EffectPartCopy(effBTLSystem_Stone_Dead_PartList[0].hPart);
    mdlSingleMotion(dds3EffectPartGetHandle(hEffect->hPart), 0, 0);
    hEffect->hPart2 = dds3EffectPartCopy(effBTLSystem_Stone_Dead_PartList[1].hPart);
    hEffect->hPart3 = dds3EffectPartCopy(effBTLSystem_Stone_Dead_PartList[2].hPart);
    scale = effBTLSystem_GetScalePtr(hEffect->pFormation);
    dds3EffectPartSetScale(hEffect->hPart, scale);
    dds3EffectPartSetScale(hEffect->hPart2, scale);
    dds3EffectPartSetScale(hEffect->hPart3, scale);
    return hEffect;
}

void effBTLSys_Stone_Dead_Delete(effBTLSysPart3_t *hEffect)
{
    mdlSetClutColor(hEffect->pFormation->hunit->owner, 0x808080, 0.0f);
    dds3EffectPartDelete(hEffect->hPart3);
    dds3EffectPartDelete(hEffect->hPart2);
    dds3EffectPartDelete(hEffect->hPart);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Stone_Dead_Delete2(effBTLSysPart3_t *hEffect)
{
    dds3EffectPartDelete(hEffect->hPart3);
    dds3EffectPartDelete(hEffect->hPart2);
    dds3EffectPartDelete(hEffect->hPart);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Stone_Dead_SetColor(void) {
}

void effBTLSys_Stone_Dead_Draw(effBTLSysPart3_t *hEffect)
{
    sdfVector4_t position;

    if (hEffect->frame == 0) {
        effBTLModelHide(hEffect->pFormation);
        mdlSetClutColor(hEffect->pFormation->hunit->owner, 0x80FFEBE1, 1.0f);
        hEffect->pFormation->hunit->owner->flags |= 2;
    }
    effBTLModelGetMarkPosition2(hEffect->pFormation, 0);
    VU0_STORE_VF10_TO(position);
    dds3EffectPartSetPosition(hEffect->hPart3, &position);
    dds3EffectPartSetPosition(hEffect->hPart2, &position);
    dds3EffectPartSetPosition(hEffect->hPart, &position);
    dds3EffectPartDraw(hEffect->hPart3);
    dds3EffectPartDraw(hEffect->hPart2);
    if (hEffect->frame >= 2) {
        dds3EffectPartDraw(hEffect->hPart);
    }
    hEffect->frame++;
}

u32 effBTLSys_Stone_Dead_GetLength(void *hEffect)
{
    return 50;
}

void effBTLSys_Stone_Dead_DataLoad(void)
{
    effBTLSystem_Part_DataLoad(effBTLSystem_Stone_Dead_PartList, 3);
}

void effBTLSys_Stone_Dead_DataFree(void)
{
    effBTLSystem_Part_DataFree(effBTLSystem_Stone_Dead_PartList, 3);
}

void effBTLSys_Stone_Dead_PartHandleCreate(void)
{
    effBTLSystem_Part_HandleCreate(effBTLSystem_Stone_Dead_PartList, 3);
}

void effBTLSys_Stone_Dead_PartHandleDelete(void)
{
    effBTLSystem_Part_HandleDelete(effBTLSystem_Stone_Dead_PartList, 3);
}

effBTLSysHansya_t *effBTLSys_Hansya_Create(nbFormation_t *pFormation)
{
    effBTLSysHansya_t *hEffect;
    f32 scale;

    hEffect = sdfChipAllocDirect(sizeof(effBTLSysHansya_t));
    hEffect->pFormation = pFormation;
    hEffect->frame = 0;
    hEffect->hPart = dds3EffectPartCopy(effBTLSystem_Hansya_PartList[0].hPart);
    mdlLoopMotion(dds3EffectPartGetHandle(hEffect->hPart), 0, 0);
    if (datSkillTable.normal[effBTLSkillGetID()].koukatype == 0) {
        hEffect->hPart2 = dds3EffectPartCopy(effBTLSystem_Hansya_PartList[1].hPart);
    } else {
        hEffect->hPart2 = dds3EffectPartCopy(effBTLSystem_Hansya_PartList[2].hPart);
    }
    scale = effBTLSystem_GetScalePtr(hEffect->pFormation);
    dds3EffectPartSetScale(hEffect->hPart, scale);
    dds3EffectPartSetScale(hEffect->hPart2, scale);
    return hEffect;
}

void effBTLSys_Hansya_Delete(effBTLSysHansya_t *hEffect)
{
    dds3EffectPartDelete(hEffect->hPart2);
    dds3EffectPartDelete(hEffect->hPart);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Hansya_Delete2(effBTLSysHansya_t *hEffect)
{
    dds3EffectPartDelete(hEffect->hPart2);
    dds3EffectPartDelete(hEffect->hPart);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Hansya_SetColor(void) {
}

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Hansya_Draw);

u32 effBTLSys_Hansya_GetLength(void *hEffect)
{
    return 15;
}

void effBTLSys_Hansya_DataLoad(void)
{
    effBTLSystem_Part_DataLoad(effBTLSystem_Hansya_PartList, 3);
}

void effBTLSys_Hansya_DataFree(void)
{
    effBTLSystem_Part_DataFree(effBTLSystem_Hansya_PartList, 3);
}

void effBTLSys_Hansya_PartHandleCreate(void)
{
    effBTLSystem_Part_HandleCreate(effBTLSystem_Hansya_PartList, 3);
}

void effBTLSys_Hansya_PartHandleDelete(void)
{
    effBTLSystem_Part_HandleDelete(effBTLSystem_Hansya_PartList, 3);
}

effBTLSysPart4_t *effBTLSys_Kyusyuu_Create(nbFormation_t *pFormation)
{
    effBTLSysPart4_t *hEffect;
    f32 scale;

    hEffect = sdfChipAllocDirect(sizeof(effBTLSysPart4_t));
    hEffect->pFormation = pFormation;
    hEffect->frame = 0;
    hEffect->hPart = dds3EffectPartCopy(effBTLSystem_Kyusyuu_PartList[0].hPart);
    hEffect->hPart2 = dds3EffectPartCopy(effBTLSystem_Kyusyuu_PartList[0].hPart);
    hEffect->hPart3 = dds3EffectPartCopy(effBTLSystem_Kyusyuu_PartList[0].hPart);
    hEffect->hPart4 = dds3EffectPartCopy(effBTLSystem_Kyusyuu_PartList[1].hPart);
    scale = effBTLSystem_GetScalePtr(hEffect->pFormation);
    dds3EffectPartSetScale(hEffect->hPart, scale);
    dds3EffectPartSetScale(hEffect->hPart2, scale);
    dds3EffectPartSetScale(hEffect->hPart3, scale);
    dds3EffectPartSetScale(hEffect->hPart4, scale);
    return hEffect;
}

void effBTLSys_Kyusyuu_Delete(effBTLSysPart4_t *hEffect)
{
    dds3EffectPartDelete(hEffect->hPart4);
    dds3EffectPartDelete(hEffect->hPart3);
    dds3EffectPartDelete(hEffect->hPart2);
    dds3EffectPartDelete(hEffect->hPart);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Kyusyuu_Delete2(effBTLSysPart4_t *hEffect)
{
    dds3EffectPartDelete(hEffect->hPart4);
    dds3EffectPartDelete(hEffect->hPart3);
    dds3EffectPartDelete(hEffect->hPart2);
    dds3EffectPartDelete(hEffect->hPart);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Kyusyuu_SetColor(void) {
}

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Kyusyuu_Draw);

u32 effBTLSys_Kyusyuu_GetLength(void *hEffect)
{
    return 25;
}

void effBTLSys_Kyusyuu_DataLoad(void)
{
    effBTLSystem_Part_DataLoad(effBTLSystem_Kyusyuu_PartList, 2);
}

void effBTLSys_Kyusyuu_DataFree(void)
{
    effBTLSystem_Part_DataFree(effBTLSystem_Kyusyuu_PartList, 2);
}

void effBTLSys_Kyusyuu_PartHandleCreate(void)
{
    effBTLSystem_Part_HandleCreate(effBTLSystem_Kyusyuu_PartList, 2);
}

void effBTLSys_Kyusyuu_PartHandleDelete(void)
{
    effBTLSystem_Part_HandleDelete(effBTLSystem_Kyusyuu_PartList, 2);
}

effBTLSysCut_t *effBTLSys_Return_Create(nbFormation_t *pFormation)
{
    effBTLSysCut_t *hEffect;

    hEffect = sdfChipAllocDirect(sizeof(effBTLSysCut_t));
    hEffect->pFormation = pFormation;
    hEffect->frame = 0;
    hEffect->hCut = effBTLCutCreate(effBTLSystem_Return_PartList[0].hBTLEffect, 3, pFormation, 0);
    return hEffect;
}

void effBTLSys_Return_Delete(effBTLSysCut_t *hEffect)
{
    effBTLCutDelete(hEffect->hCut);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Return_Delete2(effBTLSysCut_t *hEffect)
{
    effBTLCutDelete(hEffect->hCut);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Return_SetColor(void) {
}

void effBTLSys_Return_Draw(effBTLSysCut_t *hEffect)
{
    effBTLCutDraw(hEffect->hCut);
    hEffect->frame++;
}

u32 effBTLSys_Return_GetLength(effBTLSysCut_t *hEffect)
{
    return effBTLCutGetCutEndFrame(hEffect->hCut);
}

void effBTLSys_Return_DataLoad(void)
{
    effBTLSystem_Part_DataLoad(effBTLSystem_Return_PartList, 1);
}

void effBTLSys_Return_DataFree(void)
{
    effBTLSystem_Part_DataFree(effBTLSystem_Return_PartList, 1);
}

void effBTLSys_Return_PartHandleCreate(void)
{
    effBTLSystem_Part_HandleCreate(effBTLSystem_Return_PartList, 1);
}

void effBTLSys_Return_PartHandleDelete(void)
{
    effBTLSystem_Part_HandleDelete(effBTLSystem_Return_PartList, 1);
}

effBTLSysPart6_t *effBTLSys_Attack_Create(nbFormation_t *pFormation)
{
    effBTLSysPart6_t *hEffect;

    hEffect = sdfChipAllocDirect(sizeof(effBTLSysPart6_t));
    hEffect->pFormation = pFormation;
    hEffect->frame = 0;
    hEffect->hPart = dds3EffectPartCopy(effBTLSystem_Attack_PartList[0].hPart);
    hEffect->hPart2 = dds3EffectPartCopy(effBTLSystem_Attack_PartList[1].hPart);
    hEffect->hPart3 = dds3EffectPartCopy(effBTLSystem_Attack_PartList[2].hPart);
    hEffect->hPart4 = dds3EffectPartCopy(effBTLSystem_Attack_PartList[3].hPart);
    hEffect->hPart5 = dds3EffectPartCopy(effBTLSystem_Attack_PartList[4].hPart);
    hEffect->hPart6 = dds3EffectPartCopy(effBTLSystem_Attack_PartList[5].hPart);
    return hEffect;
}

void effBTLSys_Attack_Delete(effBTLSysPart6_t *hEffect)
{
    dds3EffectPartDelete(hEffect->hPart6);
    dds3EffectPartDelete(hEffect->hPart5);
    dds3EffectPartDelete(hEffect->hPart4);
    dds3EffectPartDelete(hEffect->hPart3);
    dds3EffectPartDelete(hEffect->hPart2);
    dds3EffectPartDelete(hEffect->hPart);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Attack_Delete2(effBTLSysPart6_t *hEffect)
{
    dds3EffectPartDelete(hEffect->hPart6);
    dds3EffectPartDelete(hEffect->hPart5);
    dds3EffectPartDelete(hEffect->hPart4);
    dds3EffectPartDelete(hEffect->hPart3);
    dds3EffectPartDelete(hEffect->hPart2);
    dds3EffectPartDelete(hEffect->hPart);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Attack_SetColor(void) {
}

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Attack_Draw);

u32 effBTLSys_Attack_GetLength(void *hEffect)
{
    return 20;
}

void effBTLSys_Attack_DataLoad(void)
{
    effBTLSystem_Part_DataLoad(effBTLSystem_Attack_PartList, 6);
}

void effBTLSys_Attack_DataFree(void)
{
    effBTLSystem_Part_DataFree(effBTLSystem_Attack_PartList, 6);
}

void effBTLSys_Attack_PartHandleCreate(void)
{
    effBTLSystem_Part_HandleCreate(effBTLSystem_Attack_PartList, 6);
}

void effBTLSys_Attack_PartHandleDelete(void)
{
    effBTLSystem_Part_HandleDelete(effBTLSystem_Attack_PartList, 6);
}

effBTLSysPart12_t *effBTLSys_AttackC_Create(nbFormation_t *pFormation)
{
    effBTLSysPart12_t *hEffect;

    hEffect = sdfChipAllocDirect(sizeof(effBTLSysPart12_t));
    hEffect->pFormation = pFormation;
    hEffect->frame = 0;
    hEffect->hPart = dds3EffectPartCopy(effBTLSystem_Attack_PartList[0].hPart);
    hEffect->hPart2 = dds3EffectPartCopy(effBTLSystem_Attack_PartList[1].hPart);
    hEffect->hPart3 = dds3EffectPartCopy(effBTLSystem_Attack_PartList[2].hPart);
    hEffect->hPart4 = dds3EffectPartCopy(effBTLSystem_Attack_PartList[3].hPart);
    hEffect->hPart5 = dds3EffectPartCopy(effBTLSystem_Attack_PartList[4].hPart);
    hEffect->hPart6 = dds3EffectPartCopy(effBTLSystem_Attack_PartList[5].hPart);
    hEffect->hPart7 = dds3EffectPartCopy(effBTLSystem_AttackC_PartList[0].hPart);
    hEffect->hPart8 = dds3EffectPartCopy(effBTLSystem_AttackC_PartList[1].hPart);
    hEffect->hPart9 = dds3EffectPartCopy(effBTLSystem_AttackC_PartList[2].hPart);
    hEffect->hPart10 = dds3EffectPartCopy(effBTLSystem_AttackC_PartList[3].hPart);
    hEffect->hPart11 = dds3EffectPartCopy(effBTLSystem_AttackC_PartList[4].hPart);
    hEffect->hPart12 = dds3EffectPartCopy(effBTLSystem_AttackC_PartList[5].hPart);
    return hEffect;
}

void effBTLSys_AttackC_Delete(effBTLSysPart12_t *hEffect)
{
    dds3EffectPartDelete(hEffect->hPart12);
    dds3EffectPartDelete(hEffect->hPart11);
    dds3EffectPartDelete(hEffect->hPart10);
    dds3EffectPartDelete(hEffect->hPart9);
    dds3EffectPartDelete(hEffect->hPart8);
    dds3EffectPartDelete(hEffect->hPart7);
    dds3EffectPartDelete(hEffect->hPart6);
    dds3EffectPartDelete(hEffect->hPart5);
    dds3EffectPartDelete(hEffect->hPart4);
    dds3EffectPartDelete(hEffect->hPart3);
    dds3EffectPartDelete(hEffect->hPart2);
    dds3EffectPartDelete(hEffect->hPart);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_AttackC_Delete2(effBTLSysPart12_t *hEffect)
{
    dds3EffectPartDelete(hEffect->hPart12);
    dds3EffectPartDelete(hEffect->hPart11);
    dds3EffectPartDelete(hEffect->hPart10);
    dds3EffectPartDelete(hEffect->hPart9);
    dds3EffectPartDelete(hEffect->hPart8);
    dds3EffectPartDelete(hEffect->hPart7);
    dds3EffectPartDelete(hEffect->hPart6);
    dds3EffectPartDelete(hEffect->hPart5);
    dds3EffectPartDelete(hEffect->hPart4);
    dds3EffectPartDelete(hEffect->hPart3);
    dds3EffectPartDelete(hEffect->hPart2);
    dds3EffectPartDelete(hEffect->hPart);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_AttackC_SetColor(void) {
}

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_AttackC_Draw);

u32 effBTLSys_AttackC_GetLength(void *hEffect)
{
    return 20;
}

void effBTLSys_AttackC_DataLoad(void)
{
    effBTLSystem_Part_DataLoad(effBTLSystem_AttackC_PartList, 6);
}

void effBTLSys_AttackC_DataFree(void)
{
    effBTLSystem_Part_DataFree(effBTLSystem_AttackC_PartList, 6);
}

void effBTLSys_AttackC_PartHandleCreate(void)
{
    effBTLSystem_Part_HandleCreate(effBTLSystem_AttackC_PartList, 6);
}

void effBTLSys_AttackC_PartHandleDelete(void)
{
    effBTLSystem_Part_HandleDelete(effBTLSystem_AttackC_PartList, 6);
}

effBTLSysCut_t *effBTLSys_Encount_Create(nbFormation_t *pFormation)
{
    effBTLSysCut_t *hEffect;

    hEffect = sdfChipAllocDirect(sizeof(effBTLSysCut_t));
    hEffect->pFormation = pFormation;
    hEffect->frame = 0;
    hEffect->hCut = effBTLCutCreate(effBTLSystem_Encount_PartList[0].hBTLEffect, 3, pFormation, 0);
    return hEffect;
}

void effBTLSys_Encount_Delete(effBTLSysCut_t *hEffect)
{
    effBTLCutDelete(hEffect->hCut);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Encount_Delete2(effBTLSysCut_t *hEffect)
{
    effBTLCutDelete(hEffect->hCut);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Encount_Draw(effBTLSysCut_t *hEffect)
{
    effBTLCutDraw(hEffect->hCut);
    hEffect->frame++;
}

void effBTLSys_Encount_SetColor(void) {
}

u32 effBTLSys_Encount_GetLength(effBTLSysCut_t *hEffect)
{
    return effBTLCutGetCutEndFrame(hEffect->hCut);
}

void effBTLSys_Encount_DataLoad(void)
{
    effBTLSystem_Part_DataLoad(effBTLSystem_Encount_PartList, 1);
}

void effBTLSys_Encount_DataFree(void)
{
    effBTLSystem_Part_DataFree(effBTLSystem_Encount_PartList, 1);
}

void effBTLSys_Encount_PartHandleCreate(void)
{
    effBTLSystem_Part_HandleCreate(effBTLSystem_Encount_PartList, 1);
}

void effBTLSys_Encount_PartHandleDelete(void)
{
    effBTLSystem_Part_HandleDelete(effBTLSystem_Encount_PartList, 1);
}

effBTLSysCut_t *effBTLSys_Nego_Kaihuku_Create(nbFormation_t *pFormation)
{
    effBTLSysCut_t *hEffect;

    hEffect = sdfChipAllocDirect(sizeof(effBTLSysCut_t));
    hEffect->pFormation = pFormation;
    hEffect->frame = 0;
    hEffect->hCut = effBTLCutCreate(effBTLSystem_Nego_Kaihuku_PartList[0].hBTLEffect, 3, pFormation, 0);
    return hEffect;
}

void effBTLSys_Nego_Kaihuku_Delete(effBTLSysCut_t *hEffect)
{
    effBTLCutDelete(hEffect->hCut);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Nego_Kaihuku_Delete2(effBTLSysCut_t *hEffect)
{
    effBTLCutDelete(hEffect->hCut);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Nego_Kaihuku_Draw(effBTLSysCut_t *hEffect)
{
    effBTLCutDraw(hEffect->hCut);
    hEffect->frame++;
}

u32 effBTLSys_Nego_Kaihuku_GetLength(effBTLSysCut_t *hEffect)
{
    return effBTLCutGetCutEndFrame(hEffect->hCut);
}

void effBTLSys_Nego_Kaihuku_DataLoad(void)
{
    effBTLSystem_Part_DataLoad(effBTLSystem_Nego_Kaihuku_PartList, 1);
}

void effBTLSys_Nego_Kaihuku_DataFree(void)
{
    effBTLSystem_Part_DataFree(effBTLSystem_Nego_Kaihuku_PartList, 1);
}

void effBTLSys_Nego_Kaihuku_PartHandleCreate(void)
{
    effBTLSystem_Part_HandleCreate(effBTLSystem_Nego_Kaihuku_PartList, 1);
}

void effBTLSys_Nego_Kaihuku_PartHandleDelete(void)
{
    effBTLSystem_Part_HandleDelete(effBTLSystem_Nego_Kaihuku_PartList, 1);
}

effBTLSysCut_t *effBTLSys_Nego_Kyusyu_Create(nbFormation_t *pFormation)
{
    effBTLSysCut_t *hEffect;

    hEffect = sdfChipAllocDirect(sizeof(effBTLSysCut_t));
    hEffect->pFormation = pFormation;
    hEffect->frame = 0;
    hEffect->hCut = effBTLCutCreate(effBTLSystem_Nego_Kyusyu_PartList[0].hBTLEffect, 3, pFormation, 0);
    return hEffect;
}

void effBTLSys_Nego_Kyusyu_Delete(effBTLSysCut_t *hEffect)
{
    effBTLCutDelete(hEffect->hCut);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Nego_Kyusyu_Delete2(effBTLSysCut_t *hEffect)
{
    effBTLCutDelete(hEffect->hCut);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Nego_Kyusyu_Draw(effBTLSysCut_t *hEffect)
{
    effBTLCutDraw(hEffect->hCut);
    hEffect->frame++;
}

u32 effBTLSys_Nego_Kyusyu_GetLength(effBTLSysCut_t *hEffect)
{
    return effBTLCutGetCutEndFrame(hEffect->hCut);
}

void effBTLSys_Nego_Kyusyu_DataLoad(void)
{
    effBTLSystem_Part_DataLoad(effBTLSystem_Nego_Kyusyu_PartList, 1);
}

void effBTLSys_Nego_Kyusyu_DataFree(void)
{
    effBTLSystem_Part_DataFree(effBTLSystem_Nego_Kyusyu_PartList, 1);
}

void effBTLSys_Nego_Kyusyu_PartHandleCreate(void)
{
    effBTLSystem_Part_HandleCreate(effBTLSystem_Nego_Kyusyu_PartList, 1);
}

void effBTLSys_Nego_Kyusyu_PartHandleDelete(void)
{
    effBTLSystem_Part_HandleDelete(effBTLSystem_Nego_Kyusyu_PartList, 1);
}

effBTLSysCut_t *effBTLSys_Nego_Stone_Create(nbFormation_t *pFormation)
{
    effBTLSysCut_t *hEffect;

    hEffect = sdfChipAllocDirect(sizeof(effBTLSysCut_t));
    hEffect->pFormation = pFormation;
    hEffect->frame = 0;
    hEffect->hCut = effBTLCutCreate(effBTLSystem_Nego_Stone_PartList[0].hBTLEffect, 3, pFormation, 0);
    return hEffect;
}

void effBTLSys_Nego_Stone_Delete(effBTLSysCut_t *hEffect)
{
    effBTLCutDelete(hEffect->hCut);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Nego_Stone_Delete2(effBTLSysCut_t *hEffect)
{
    effBTLCutDelete(hEffect->hCut);
    sdfChipFreeDirect(hEffect);
}

void effBTLSys_Nego_Stone_Draw(effBTLSysCut_t *hEffect)
{
    effBTLCutDraw(hEffect->hCut);
    hEffect->frame++;
}

u32 effBTLSys_Nego_Stone_GetLength(effBTLSysCut_t *hEffect)
{
    return effBTLCutGetCutEndFrame(hEffect->hCut);
}

void effBTLSys_Nego_Stone_DataLoad(void)
{
    effBTLSystem_Part_DataLoad(effBTLSystem_Nego_Stone_PartList, 1);
}

void effBTLSys_Nego_Stone_DataFree(void)
{
    effBTLSystem_Part_DataFree(effBTLSystem_Nego_Stone_PartList, 1);
}

void effBTLSys_Nego_Stone_PartHandleCreate(void)
{
    effBTLSystem_Part_HandleCreate(effBTLSystem_Nego_Stone_PartList, 1);
}

void effBTLSys_Nego_Stone_PartHandleDelete(void)
{
    effBTLSystem_Part_HandleDelete(effBTLSystem_Nego_Stone_PartList, 1);
}

effBTLSystemHandle_t *effBTLSystemCreate(nbFormation_t *pFormation, s32 family)
{
    effBTLSystemHandle_t *hEffect;

    hEffect = sdfChipAllocDirect(sizeof(effBTLSystemHandle_t));
    hEffect->family = family;
    hEffect->pFormation = pFormation;
    if (D_0031BF00[family].create != NULL) {
        hEffect->hEffect = D_0031BF00[family].create(pFormation);
    }
    return hEffect;
}

void effBTLSystemDelete(effBTLSystemHandle_t *hEffect)
{
    if (D_0031BF00[hEffect->family].destroy != NULL) {
        D_0031BF00[hEffect->family].destroy(hEffect->hEffect);
    }
    sdfChipFreeDirect(hEffect);
}

void effBTLSystemDelete2(effBTLSystemHandle_t *hEffect)
{
    if (D_0031BF00[hEffect->family].destroy2 != NULL) {
        D_0031BF00[hEffect->family].destroy2(hEffect->hEffect);
    }
    sdfChipFreeDirect(hEffect);
}

INCLUDE_ASM(const s32, "code_0016F490", effBTLSystemDraw);

u32 effBTLSystemGetLength(effBTLSystemHandle_t *hEffect)
{
    if (D_0031BF00[hEffect->family].getLength != NULL) {
        return D_0031BF00[hEffect->family].getLength(hEffect->hEffect);
    }
    return 0;
}

s32 effBTLSystemGetID(effBTLSystemHandle_t *hEffect)
{
    return hEffect->family;
}

u32 effBTLSystemGetRGBA(s32 family)
{
    return D_0031BF00[family].rgba;
}

void effBTLSystemCut1DataFree(void)
{
    u32 i;

    for (i = 0; i < 0x19; i++) {
        if (effBTLSystemCut1Data[i].hMem != NULL) {
            sdfMemFreeDirect(effBTLSystemCut1Data[i].hMem);
        }
        if (effBTLSystemCut1Data[i].hBTLEffect != NULL) {
            effBTLDelete(effBTLSystemCut1Data[i].hBTLEffect);
            effBTLSystemCut1Data[i].hBTLEffect = NULL;
            effBTLSystemCut1Data[i].refCount = 0;
        }
    }
}

INCLUDE_RODATA(const s32, "code_0016F490", D_00533318);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533338);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533350);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533368);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533380);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533398);

INCLUDE_RODATA(const s32, "code_0016F490", D_005333B0);

INCLUDE_RODATA(const s32, "code_0016F490", D_005333D0);

INCLUDE_RODATA(const s32, "code_0016F490", D_005333E8);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533400);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533418);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533438);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533450);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533468);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533488);

INCLUDE_RODATA(const s32, "code_0016F490", D_005334A8);

INCLUDE_RODATA(const s32, "code_0016F490", D_005334C0);

INCLUDE_RODATA(const s32, "code_0016F490", D_005334D0);

INCLUDE_RODATA(const s32, "code_0016F490", D_005334F0);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533510);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533520);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533538);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533558);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533578);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533590);

INCLUDE_RODATA(const s32, "code_0016F490", D_005335A8);

INCLUDE_RODATA(const s32, "code_0016F490", D_005335C0);

INCLUDE_RODATA(const s32, "code_0016F490", D_005335E0);

INCLUDE_RODATA(const s32, "code_0016F490", D_005335F0);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533608);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533620);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533638);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533650);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533668);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533680);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533690);

INCLUDE_RODATA(const s32, "code_0016F490", D_005336A8);

INCLUDE_RODATA(const s32, "code_0016F490", D_005336C8);

INCLUDE_RODATA(const s32, "code_0016F490", D_005336E8);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533708);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533720);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533738);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533750);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533768);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533780);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533798);

INCLUDE_RODATA(const s32, "code_0016F490", D_005337B0);

INCLUDE_RODATA(const s32, "code_0016F490", D_005337C8);

INCLUDE_RODATA(const s32, "code_0016F490", D_005337E0);

INCLUDE_RODATA(const s32, "code_0016F490", D_005337F8);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533810);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533828);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533840);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533858);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533878);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533890);

INCLUDE_RODATA(const s32, "code_0016F490", D_005338A8);

INCLUDE_RODATA(const s32, "code_0016F490", D_005338C0);

INCLUDE_RODATA(const s32, "code_0016F490", D_005338D8);

INCLUDE_RODATA(const s32, "code_0016F490", D_005338F8);

void *effBTLSystemCut1Create(nbFormation_t *pFormation, s32 cut)
{
    effBTLSystemCut1Data_t *pData;
    void *hCut;

loop:
    pData = &effBTLSystemCut1Data[cut];
    if (pData->pData == NULL) {
        cut = 1;
        goto loop;
    }
    if (pData->refCount == 0) {
        pData->hBTLEffect = effBTLCreate(pData->pData);
        printf("effBTLSys:effBTLCreate[%d]\n", cut);
    }
    pData->refCount++;
    hCut = effBTLCutCreate(pData->hBTLEffect, 0, pFormation, 0);
    printf("effBTLSys:effBTLCutCreate\n");
    effBTLCutSetUserData(hCut, pData);
    return hCut;
}

void effBTLSystemCut1Draw(void *hCut)
{
    effBTLCutDraw(hCut);
}

void effBTLSystemCut1Delete(void *hCut)
{
    effBTLSystemCut1Data_t *pData;

    pData = effBTLCutGetUserData(hCut);
    effBTLCutDelete(hCut);
    printf("effBTLSys:effBTLCutDelete\n");
    pData->refCount--;
    if (pData->refCount == 0) {
        effBTLDelete(pData->hBTLEffect);
        pData->hBTLEffect = NULL;
        printf("effBTLSys:effBTLDelete\n");
    }
}

void effBTLSystemCut1PartHandleCreate(void) {
}

void effBTLSystemCut1PartHandleDelete(void)
{
    u32 i;

    for (i = 0; i < 0x19; i++) {
        if (effBTLSystemCut1Data[i].hBTLEffect != NULL) {
            effBTLDelete(effBTLSystemCut1Data[i].hBTLEffect);
            effBTLSystemCut1Data[i].hBTLEffect = NULL;
            effBTLSystemCut1Data[i].refCount = 0;
        }
    }
}

void effBTLEncountStart(void)
{
    effBTLSkillStart(0x200, 0);
}

void effBTLEncountEnd(void)
{
    effBTLSkillEnd();
}

INCLUDE_ASM(const s32, "code_0016F490", effBTLSystemInit);

void effBTLSystemEnd(void)
{
    s32 i;

    effBTLSystemCut1DataFree();
    for (i = 0; i <= 0x1D; i++) {
        if (D_0031BF00[i].dataFree != NULL) {
            D_0031BF00[i].dataFree();
        }
    }
    printf("effBTLSys:DataFree\n");
}

void effBTLSystemPartHandleCreate(void)
{
    s32 i;

    effBTLSystemCut1PartHandleCreate();
    for (i = 0; i <= 0x1D; i++) {
        if (D_0031BF00[i].active != 0 && D_0031BF00[i].partHandleCreate != NULL) {
            D_0031BF00[i].partHandleCreate();
        }
    }
    printf("effBTLSys:PartHandleCreate\n");
}

void effBTLSystemPartHandleDelete(void)
{
    s32 i;

    effBTLSystemCut1PartHandleDelete();
    for (i = 0; i <= 0x1D; i++) {
        if (D_0031BF00[i].active != 0 && D_0031BF00[i].partHandleDelete != NULL) {
            D_0031BF00[i].partHandleDelete();
        }
    }
    printf("effBTLSys:PartHandleDelete\n");
}

INCLUDE_ASM(const s32, "code_0016F490", effCAMPBadCreate);

INCLUDE_ASM(const s32, "code_0016F490", effCAMPBadDraw);

INCLUDE_ASM(const s32, "code_0016F490", effCAMPBadDelete);

void func_00174858(void) {
}

void func_00174860(void) {
}

void func_00174868(void) {
}

void func_00174870(void) {
}

void func_00174878(void) {
}

void effBTLSetFieldColor(s32 on)
{
    effBTLFieldColorOn = on;
}

s32 eff_174888(void)
{
    return D_0054F0F1;
}

s32 effBTLFieldColorIsOn(void)
{
    return effBTLFieldColorOn;
}

s32 eff_174898(void)
{
    return D_0054F0F4;
}

char *effMakeBedFileName(u32 id)
{
    if (id < 0x10) {
        sprintf(effBedNameBuf, "%s00%X.BED", D_0054F100, id);
    } else if (id < 0x100) {
        sprintf(effBedNameBuf, "%s0%X.BED", D_0054F100, id);
    } else {
        sprintf(effBedNameBuf, "%s%X.BED", D_0054F100, id);
    }
    return effBedNameBuf;
}

void effBTLSkillSetActive(void)
{
    effBTLSkillActive = TRUE;
}

void effBTLSkillClearActive(void)
{
    effBTLSkillActive = FALSE;
}

void effBTLCutReset(void)
{
    effBTLCutCount = 0;
    memset(effBTLCutMaxTable, 0, sizeof(effBTLCutMaxTable));
}

void effBTLCutSetMax(u8 cut, u32 max)
{
    if (effBTLCutMaxTable[cut] < max) {
        effBTLCutMaxTable[cut] = max;
    }
}

INCLUDE_ASM(const s32, "code_0016F490", effBTLCutDebugPrint);

INCLUDE_ASM(const s32, "code_0016F490", func_00174AB8);

INCLUDE_ASM(const s32, "code_0016F490", func_00174BB0);

INCLUDE_ASM(const s32, "code_0016F490", func_00174C50);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533A40);

char *effMakeBedPath(char *path)
{
    D_00551DA0 = dds3EffectFileListCreate("/efftool/bed/", 8);
    D_00551DA4 = dds3EffectFileListWindowCreate(D_00551DA0);
    dds3EffectFileListWindowSetPos(D_00551DA4, 0xF0, 0xD0);
    dds3EffectFileListWindowSetOt(D_00551DA4, 1);
    dds3EffectFileListWindowSetPadWait(D_00551DA4, 0x53);
    dds3EffectFileListWindowSetWindowColor(D_00551DA4, 0x80806020, 0x80000000);
    return path;
}

INCLUDE_ASM(const s32, "code_0016F490", eff_174D38);

void eff_174D98(void)
{
    dds3EffectFileListWindowDelete(D_00551DA4);
    dds3EffectFileListDelete(D_00551DA0);
}

INCLUDE_ASM(const s32, "code_0016F490", func_00174DC0);

INCLUDE_ASM(const s32, "code_0016F490", eff_174E88);

INCLUDE_ASM(const s32, "code_0016F490", func_00175030);

INCLUDE_ASM(const s32, "code_0016F490", eff_175070);

INCLUDE_ASM(const s32, "code_0016F490", func_001750D0);

u32 effBTLDebug_CameraAllEditCheck(void)
{
    return effBTLDebugCameraAllEdit;
}

u32 effBTLDebug_CameraAllTableCheck(void)
{
    return effBTLDebugCameraAllTable;
}

u32 eff_175328(void)
{
    return D_0054F1C3;
}

void nbSetHelperViewer(void *pViewer)
{
    nbHelperViewer = pViewer;
}

INCLUDE_ASM(const s32, "code_0016F490", nb_175338);

INCLUDE_ASM(const s32, "code_0016F490", nb_175440);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533A60);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533A70);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533A80);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533A90);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533AA0);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533AB0);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533AC0);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533AD0);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533AE0);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533AF0);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533B00);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533B10);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533B20);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533B30);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533B40);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533B50);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533B60);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533B70);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533B80);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533B90);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533BA0);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533BB0);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533BC0);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533BD0);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533BE0);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533BF0);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533C00);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533C10);

INCLUDE_ASM(const s32, "code_0016F490", effBTLDebugPrintDistance);

void eff_175768(void)
{
    if (dds3DebugSign != 0) {
        func_00174858();
        effBTLSkillActive = FALSE;
        D_0054F1DF = 0;
        D_0054F130 = 0;
        nbHelperViewer = D_0031D000;
        D_0054F134 = 0;
    }
}

void eff_1757A8(void)
{
    if (dds3DebugSign != 0) {
        func_00174868();
    }
}

INCLUDE_ASM(const s32, "code_0016F490", eff_1757D8);

INCLUDE_ASM(const s32, "code_0016F490", dds3_175878);

void InitModelCollisionEditorProcess(void)
{
    dds3AdminiCallSequence(0, 0, 0, 0);
}

void *dds3EffectPartGetHandle(effPartHandle_t *hEffPart)
{
    return hEffPart->pHandle;
}

u16 dds3EffectPartGetType(effPartHandle_t *hEffPart)
{
    return hEffPart->type;
}

effPartHandle_t *dds3EffectPartCreate(u16 type, void *pData)
{
    effPartHandle_t *hEffPart;

    hEffPart = sdfChipAllocDirect(sizeof(effPartHandle_t));
    hEffPart->type = type;
    hEffPart->pHandle = dds3EffectFuncTable[type].create(pData);
    return hEffPart;
}

void dds3EffectPartDelete(effPartHandle_t *hEffPart)
{
    dds3EffectFuncTable[hEffPart->type].destroy(hEffPart->pHandle);
    sdfChipFreeDirect(hEffPart);
}

void dds3EffectPartDraw(effPartHandle_t *hEffPart)
{
    dds3EffectFuncTable[hEffPart->type].draw(hEffPart->pHandle);
}

effPartHandle_t *dds3EffectPartCopy(effPartHandle_t *hEffPartSrc)
{
    effPartHandle_t *hEffPart;

    hEffPart = sdfChipAllocDirect(sizeof(effPartHandle_t));
    hEffPart->type = hEffPartSrc->type;
    hEffPart->pHandle = dds3EffectFuncTable[hEffPartSrc->type].copy(hEffPartSrc->pHandle);
    return hEffPart;
}

void dds3EffectPartSetPosition(effPartHandle_t *hEffPart, sdfVector4_t *pPosition)
{
    if (dds3EffectFuncTable[hEffPart->type].setPosition != NULL) {
        dds3EffectFuncTable[hEffPart->type].setPosition(hEffPart->pHandle, pPosition);
    }
}

void dds3EffectPartSetScale(effPartHandle_t *hEffPart, f32 scale)
{
    if (dds3EffectFuncTable[hEffPart->type].setScale != NULL) {
        dds3EffectFuncTable[hEffPart->type].setScale(hEffPart->pHandle, scale);
    }
}

void dds3EffectPartSetRotateMatrix(effPartHandle_t *hEffPart, void *pMatrix)
{
    if (dds3EffectFuncTable[hEffPart->type].setRotateMatrix != NULL) {
        dds3EffectFuncTable[hEffPart->type].setRotateMatrix(hEffPart->pHandle, pMatrix);
    }
}

void dds3EffectPartSetColor(effPartHandle_t *hEffPart, u32 color)
{
    if (dds3EffectFuncTable[hEffPart->type].setColor != NULL) {
        dds3EffectFuncTable[hEffPart->type].setColor(hEffPart->pHandle, color);
    }
}

void dds3EffectPartSetDirection(effPartHandle_t *hEffPart, void *pMatrix)
{
    if (dds3EffectFuncTable[hEffPart->type].setDirection != NULL) {
        dds3EffectFuncTable[hEffPart->type].setDirection(hEffPart->pHandle, pMatrix);
    }
}

void dds3EffectPartSetArea(effPartHandle_t *hEffPart, f32 area)
{
    if (dds3EffectFuncTable[hEffPart->type].setArea != NULL) {
        dds3EffectFuncTable[hEffPart->type].setArea(hEffPart->pHandle, area);
    }
}

void *dds3EffectPartTMXBillboardCreate(u32 data)
{
    return dds3BillboardCreate(0, data);
}

void *dds3EffectPartP2ABillboardCreate(u32 data)
{
    return dds3BillboardCreate(1, data);
}

void dds3EffectPartBillboardScale(void *hBillboard, f32 scale)
{
    dds3BillboardSetScale(hBillboard, scale, scale);
}

void dds3EffectPartModelInit(mdlObj_t *hModel)
{
    SDF_VU0__LOADVECTOR(&D_0031DF60);
    mdlSetPosition(hModel);
    SDF_VU0__LOADVECTOR(&D_0031DF70);
    mdlSetRotate(hModel);
    SDF_VU0__LOADVECTOR(&D_0031DF80);
    mdlSetScale(hModel);
    mdlSetColor(hModel, 0x80808080);
    if (hModel->first != NULL) {
        mdlLoopMotion(hModel, 0, 0);
        hModel->first->speed = 1.0f;
    }
    hModel->flags &= ~1;
}

mdlObj_t *dds3EffectPartModelCreate(dds3EffectPartModelData_t *pData)
{
    mdlObj_t *hModel;

    mdlPackDecode(7, D_0054F1E4, 0x101, pData->data, pData->size);
    hModel = mdlLoad(7, D_0054F1E4);
    dds3EffectPartModelInit(hModel);
    D_0054F1E4++;
    return hModel;
}

void dds3EffectPartModelDraw(mdlObj_t *hModel)
{
    mdlLink(hModel, D_002F69B8);
}

void dds3EffectPartModelDelete(mdlObj_t *hModel)
{
    mdlFree(hModel);
}

mdlObj_t *dds3EffectPartModelCopy(void *hData)
{
    s32 major;
    s32 minor;
    mdlObj_t *hModel;

    major = mdlGetMajorNum(hData);
    minor = mdlGetMinorNum(hData);
    hModel = mdlLoad(major, minor);
    dds3EffectPartModelInit(hModel);
    return hModel;
}

void dds3EffectPartModelSetPosition(mdlObj_t *hModel, sdfVector4_t *pPosition)
{
    SDF_VU0__LOADVECTOR(pPosition);
    mdlSetPosition(hModel);
}

void dds3EffectPartModelSetScale(void *hData, f32 scale)
{
    f32 v[3];

    v[0] = v[1] = v[2] = scale;
    VU0_LOAD_VF10_FROM(v[0]);
    mdlSetScale(hData);
}

void dds3EffectPartModelSetRotateMatrix(mdlObj_t *hModel, sdfMatrix4_t *pMatrix)
{
    SDF_VU0__LOADMATRIX(pMatrix);
    VU0_STORE_VF28(&hModel->inner->tm.m[0]);
    VU0_STORE_VF29(&hModel->inner->tm.m[4]);
    VU0_STORE_VF30(&hModel->inner->tm.m[8]);
}

void dds3EffectPartModelSetColor(mdlObj_t *hModel, u32 color)
{
    mdlSetColor(hModel, color);
}

dds3EffectPCP_t *dds3EffectPCPCreate(dds3EffectPCPData_t *hPCPData)
{
    dds3EffectPCP_t *hPCP;

    hPCP = sdfChipAllocDirect(sizeof(dds3EffectPCP_t));
    hPCP->kateg = dds3EffectPCPDataGetKateg(hPCPData);
    hPCP->param = dds3EffectPCPDataGetParam(hPCPData);
    if (EffectPCPFuncTable[hPCP->kateg].copy == NULL) {
        hPCP->pHandle = EffectPCPFuncTable[hPCP->kateg].create(D_0031DDD0[hPCP->kateg][hPCP->param]);
    } else {
        hPCP->pHandle = EffectPCPFuncTable[hPCP->kateg].create(hPCPData);
    }
    return hPCP;
}

void dds3EffectPCPDelete(dds3EffectPCP_t *hPCP)
{
    EffectPCPFuncTable[hPCP->kateg].destroy(hPCP->pHandle);
    sdfChipFreeDirect(hPCP);
}

void dds3EffectPCPDraw(dds3EffectPCP_t *hPCP)
{
    EffectPCPFuncTable[hPCP->kateg].draw(hPCP->pHandle);
}

dds3EffectPCP_t *dds3EffectPCPCopy(dds3EffectPCP_t *hPCPSrc)
{
    dds3EffectPCP_t *hPCP;

    hPCP = sdfChipAllocDirect(sizeof(dds3EffectPCP_t));
    hPCP->kateg = hPCPSrc->kateg;
    hPCP->param = hPCPSrc->param;
    if (EffectPCPFuncTable[hPCP->kateg].copy == NULL) {
        hPCP->pHandle = EffectPCPFuncTable[hPCP->kateg].create(D_0031DDD0[hPCP->kateg][hPCP->param]);
    } else {
        hPCP->pHandle = EffectPCPFuncTable[hPCP->kateg].copy(hPCPSrc->pHandle);
    }
    return hPCP;
}

void dds3EffectPCPSetPosition(dds3EffectPCP_t *hPCP, void *pPos)
{
    if (EffectPCPFuncTable[hPCP->kateg].setPosition != NULL) {
        EffectPCPFuncTable[hPCP->kateg].setPosition(hPCP->pHandle, pPos);
    }
}

void dds3EffectPCPSetScale(dds3EffectPCP_t *hPCP, f32 scale)
{
    if (EffectPCPFuncTable[hPCP->kateg].setScale != NULL) {
        EffectPCPFuncTable[hPCP->kateg].setScale(hPCP->pHandle, scale);
    }
}

void dds3EffectPCPSetRotateMatrix(dds3EffectPCP_t *hPCP, void *pMatrix)
{
    if (EffectPCPFuncTable[hPCP->kateg].setRotateMatrix != NULL) {
        EffectPCPFuncTable[hPCP->kateg].setRotateMatrix(hPCP->pHandle, pMatrix);
    }
}

void dds3EffectPCPSetDirection(dds3EffectPCP_t *hPCP, void *pMatrix)
{
    if (EffectPCPFuncTable[hPCP->kateg].setDirection != NULL) {
        EffectPCPFuncTable[hPCP->kateg].setDirection(hPCP->pHandle, pMatrix);
    }
}

void dds3EffectPCPSetColor(dds3EffectPCP_t *hPCP, u32 abgr)
{
    if (EffectPCPFuncTable[hPCP->kateg].setColor != NULL) {
        EffectPCPFuncTable[hPCP->kateg].setColor(hPCP->pHandle, abgr);
    }
}

void dds3EffectPCPSetArea(dds3EffectPCP_t *hPCP, f32 area)
{
    if (EffectPCPFuncTable[hPCP->kateg].setArea != NULL) {
        EffectPCPFuncTable[hPCP->kateg].setArea(hPCP->pHandle, area);
    }
}

u32 dds3EffectPCPDataGetKateg(dds3EffectPCPData_t *hPCPData)
{
    return hPCPData->kateg;
}

u32 dds3EffectPCPDataGetParam(dds3EffectPCPData_t *hPCPData)
{
    return hPCPData->param;
}

void *dds3EffectPCPDataGetData(dds3EffectPCPData_t *hPCPData, u32 number)
{
    dds3EffectPCPData_Part_t *part = &hPCPData->pcpData[number];

    return (u8 *)hPCPData + part->offset;
}

u32 dds3EffectPCPDataGetSize(dds3EffectPCPData_t *hPCPData, u32 number)
{
    dds3EffectPCPData_Part_t *part = &hPCPData->pcpData[number];

    return part->size;
}

u32 dds3EffectPCPDataGetType(dds3EffectPCPData_t *hPCPData, u32 number)
{
    dds3EffectPCPData_Part_t *part = &hPCPData->pcpData[number];

    return part->type;
}

effPartHandle_t *dds3EffectPCPDataPartCreate(dds3EffectPCPData_t *hPCPData, u32 number)
{
    u16 type;
    void *pData;

    type = (u16)dds3EffectPCPDataGetType(hPCPData, number);
    pData = dds3EffectPCPDataGetData(hPCPData, number);
    return dds3EffectPartCreate(type, pData);
}

INCLUDE_RODATA(const s32, "code_0016F490", D_00533C40);

INCLUDE_SDATA(const s32, "code_0016F490", effBTLPointLightProcess);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F0E4);

INCLUDE_SDATA(const s32, "code_0016F490", effBTLFieldColorOn);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F0F1);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F0F4);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F0F8);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F100);

INCLUDE_SDATA(const s32, "code_0016F490", effBTLCutCount);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F10C);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F110);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F118);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F120);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F128);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F130);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F134);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F135);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F138);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F140);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F148);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F150);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F158);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F160);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F168);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F170);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F178);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F180);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F188);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F18A);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F18B);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F18C);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F190);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F198);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F1A0);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F1A8);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F1B0);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F1B8);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F1C0);

INCLUDE_SDATA(const s32, "code_0016F490", effBTLDebugCameraAllEdit);

INCLUDE_SDATA(const s32, "code_0016F490", effBTLDebugCameraAllTable);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F1C3);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F1C8);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F1D0);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F1D8);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F1E0);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F1E4);

