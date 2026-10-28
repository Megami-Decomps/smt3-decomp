#include "common.h"
#include "motion.h"
#include "model.h"
#include "event.h"
#include "nb.h"
#include "effBattle.h"
#include "ps2asm.h"
#include "draw3d.h"

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

void mdlLoopMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber);

extern s32 effBTLCutCount;

extern u32 effBTLCutMaxTable[4];

void effBTLSkillStart(s32 id, s32 stat);

void effBTLSkillEnd(void);

void *sdfChipAllocDirect(s32 aSize);

void sdfChipFreeDirect(void *aPtr);

extern void *sdfFileOpen(const char *path);
extern u32 sdfFileLength(void *handle);
extern sdfMemHandle_t *sdfMemAllocDirect(s32 size);
extern void *sdfMemLock(sdfMemHandle_t *handle);
extern s32 sdfFileRead(void *handle, void *dst, s32 size);
extern void sdfFileClose(void *handle);
extern void *sdfDevLoadFileEx(const char *path, u32 *data, u32 *size);
extern s32 printf(const char *format, ...);
extern char D_00533300[];

void dds3AdminiCallSequence(s32 sequence, void *data, s32 datasize, s32 restore);

effPartHandle_t *dds3EffectPartCreate(u16 type, void *pData);

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
} effBattleSystem_effBTLSystem_Part_tag;

extern effBattleSystem_effBTLSystem_Part_tag effBTLSystem_Freeze_PartList[];

extern effBattleSystem_effBTLSystem_Part_tag effBTLSystem_Sleep_PartList[];

extern effBattleSystem_effBTLSystem_Part_tag effBTLSystem_Panic_PartList[];

extern effBattleSystem_effBTLSystem_Part_tag effBTLSystem_Bind_PartList[];

extern effBattleSystem_effBTLSystem_Part_tag effBTLSystem_Poison_PartList[];

extern effBattleSystem_effBTLSystem_Part_tag effBTLSystem_Charm_PartList[];

extern effBattleSystem_effBTLSystem_Part_tag effBTLSystem_Curse_PartList[];

extern effBattleSystem_effBTLSystem_Part_tag effBTLSystem_CurseCAMP_PartList[];

extern effBattleSystem_effBTLSystem_Part_tag effBTLSystem_Stone_Dead_PartList[];

extern effBattleSystem_effBTLSystem_Part_tag effBTLSystem_Hansya_PartList[];

extern effBattleSystem_effBTLSystem_Part_tag effBTLSystem_Kyusyuu_PartList[];

extern effBattleSystem_effBTLSystem_Part_tag effBTLSystem_Return_PartList[];

extern effBattleSystem_effBTLSystem_Part_tag effBTLSystem_Attack_PartList[];

extern effBattleSystem_effBTLSystem_Part_tag effBTLSystem_AttackC_PartList[];

extern effBattleSystem_effBTLSystem_Part_tag effBTLSystem_Encount_PartList[];

extern effBattleSystem_effBTLSystem_Part_tag effBTLSystem_Nego_Kaihuku_PartList[];

extern effBattleSystem_effBTLSystem_Part_tag effBTLSystem_Nego_Kyusyu_PartList[];

extern effBattleSystem_effBTLSystem_Part_tag effBTLSystem_Nego_Stone_PartList[];

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
    effBattleSystem_effBTLSystem_Part_tag *pPartList;
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
    s32 use;
    void *pData;
    sdfMemHandle_t *hMem;
} effBattleSystem_effBTLSystem_Cut1_tag;

extern effBattleSystem_effBTLSystem_Cut1_tag effBTLSystemCut1Data[];

void *effBTLCutGetUserData(void *hBTLCutEffect);

void effBTLCutSetUserData(void *hBTLCutEffect, void *pData);

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

void dds3EffectFileListWindowDelete(void *a);

void dds3EffectFileListDelete(void *a);

extern char D_0054F100[];

extern char effBedNameBuf[];

void evtSetUnitColorRGB(evtUnit_t *h, s32 frame, u32 rgb);

void *dds3EffectFileListCreate(const char *dir, s32 type);

void *dds3EffectFileListWindowCreate(void *a);

void dds3EffectFileListWindowSetPos(void *a, s32 b, s32 c);

void dds3EffectFileListWindowSetOt(void *hEffFLWin, s32 ot);

void dds3EffectFileListWindowSetPadWait(void *hEffFLWin, s32 wait);

void dds3EffectFileListWindowSetWindowColor(void *hEffFLWin, u32 frame_c, u32 bg_c);

void func_001750D0(s32 param);

#define EFFBTL_PART_TYPE_BTL 0xFFFD

/* Effect handle shared by the families that hold one part and a frame count. */
typedef struct
{
    nbFormation_t *pFormation;
    u32 frame;
    effPartHandle_t *hPart;
} effBattleSystem_effBTLSystem_Curse_tag;

void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

void effBTLModelGetMarkPosition2(nbFormation_t *pFormation, s32 mark);

void effBTLModelGetMarkPosition3(mdlObj_t *hModel, s32 mark);

void mdlSetClutColor(mdlObj_t *aHandle, u32 aColor, f32 aRatio);

void *effBTLBindCreateTrack(void *pParam);

void effPCPThunderSpherePolyLowDelete(void *hData);

void effPCPThunderSpherePolyLowDraw(void *hData);

void effPCPThunderSpherePolyLowSetPosition(void *hData, sdfVector4_t *pPos);

void effPCPThunderSpherePolyLowSetScale(void *hData, f32 scale);

void effPCPThunderSpherePolyLowSetColor(void *hData, u32 abgr);

effPCPThunderSpherePolyLowParam_t *effPCPThunderSpherePolyLowGetParam(void *pThunderPoly);

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
    effPartHandle_t *hPartD3P;
    mdlObj_t *hModel;
} effBattleSystem_effBTLSystem_Poison_tag;

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
    effPartHandle_t *hPartD3P0;
    effPartHandle_t *hPartD3P1;
    mdlObj_t *hModel;
} effBattleSystem_effBTLSystem_Close_tag;

void dds3SpPoly_RollRingSetEndless(void *pRollRing);

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
} effBattleSystem_effBTLSystem_Hansya_tag;

/* Effect handle of the Kyusyuu family, four parts. */
typedef struct
{
    nbFormation_t *pFormation;
    u32 frame;
    effPartHandle_t *hPart;
    effPartHandle_t *hPart2;
    effPartHandle_t *hPart3;
    effPartHandle_t *hPart4;
} effBattleSystem_effBTLSystem_Kyusyuu_tag;

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
} effBattleSystem_effBTLSystem_Attack_tag;

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
} effBattleSystem_effBTLSystem_AttackC_tag;

/* Effect handle of the Stone_Dead family, which holds three parts. */
typedef struct
{
    nbFormation_t *pFormation;
    u32 frame;
    effPartHandle_t *hPart;
    effPartHandle_t *hPart2;
    effPartHandle_t *hPart3;
} effBattleSystem_effBTLSystem_Stone_Dead_tag;

void effBTLCutDraw(void *hBTLCutEffect);

u32 effBTLCutGetCutEndFrame(void *hBTLCutEffect);

void effBTLModelHide(nbFormation_t *pFormation);

void mdlSingleMotion(void *aHandle, s32 aGroup, s32 aNumber);

void effBTLSystem_Part_DataLoad(effBattleSystem_effBTLSystem_Part_tag *pPartList, u32 count);

void effBTLSystem_Part_DataFree(effBattleSystem_effBTLSystem_Part_tag *pPartList, u32 count);

void effBTLSystem_Part_HandleCreate(effBattleSystem_effBTLSystem_Part_tag *pPartList, u32 count);

void effBTLSystem_Part_HandleDelete(effBattleSystem_effBTLSystem_Part_tag *pPartList, u32 count);

void *dds3EffectPartGetHandle(effPartHandle_t *hEffPart);

void dds3EffectPartDelete(effPartHandle_t *hEffPart);

effPartHandle_t *dds3EffectPartCopy(effPartHandle_t *hEffPartSrc);

void dds3EffectPartDraw(effPartHandle_t *hEffPart);

void dds3EffectPartSetPosition(effPartHandle_t *hEffPart, sdfVector4_t *pPos);

void dds3EffectPartSetScale(effPartHandle_t *hEffPart, f32 scale);

void dds3EffectPartSetColor(effPartHandle_t *hEffPart, u32 abgr);

void effBTLSystem_SetUnitColorRGB(nbFormation_t *pFormation, s32 frame, u32 bgr);

f32 effBTLSystem_GetScalePtr(nbFormation_t *pFormation);

f32 effBTLPartGetScalePtr(nbFormation_t *pFormation);

f32 effBTLPartCalcScale(nbFormation_t *pFormation);

void effBTLSystem_SetUnitColorRGB(nbFormation_t *pFormation, s32 frame, u32 bgr)
{
    if (pFormation->hunit != NULL) {
        evtSetUnitColorRGB(pFormation->hunit, frame, bgr);
        pFormation->modelcol = (pFormation->modelcol & 0xFF000000) | (bgr & 0xFFFFFF);
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

void effBTLSystem_Part_DataLoad(effBattleSystem_effBTLSystem_Part_tag *pPartList, u32 count)
{
    u32 i;

    for (i = 0; i < count; i++) {
        if (pPartList->type == 3) {
            void *file = sdfFileOpen(pPartList->fileName);
            u32 fileSize = sdfFileLength(file);
            /* Retail copies the 16-byte header with only its length initialized. */
            struct {
                u32 fileSize;
                u8 reserved[0xC];
            } header;

            header.fileSize = fileSize;
            pPartList->hMem = sdfMemAllocDirect(fileSize + 0x10);
            pPartList->pData = sdfMemLock(pPartList->hMem);
            memcpy(pPartList->pData, &header, sizeof(header));
            sdfFileRead(file, (u8 *)pPartList->pData + 0x10, header.fileSize);
            sdfFileClose(file);
        } else {
            pPartList->hMem = sdfDevLoadFileEx(pPartList->fileName, (u32 *)&pPartList->pData, NULL);
        }
        pPartList->refCount = 0;
        printf(D_00533300, pPartList->fileName);
        pPartList++;
    }
}

void effBTLSystem_Part_DataFree(effBattleSystem_effBTLSystem_Part_tag *pPartList, u32 count)
{
    u32 i;

    for (i = 0; i < count; i++) {
        sdfMemFreeDirect(pPartList->hMem);
        pPartList++;
    }
}

void effBTLSystem_Part_HandleCreate(effBattleSystem_effBTLSystem_Part_tag *pPartList, u32 count)
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

void effBTLSystem_Part_HandleDelete(effBattleSystem_effBTLSystem_Part_tag *pPartList, u32 count)
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

void effBTLSys_Shock_Delete(effBTLSysTrack_t *pData)
{
    effBTLSystem_SetUnitColorRGB(pData->pFormation, 4, 0x80808080);
    effPCPThunderSpherePolyLowDelete(pData->hTrack);
    sdfChipFreeDirect(pData);
}

void effBTLSys_Shock_Delete2(effBTLSysTrack_t *pData)
{
    effPCPThunderSpherePolyLowDelete(pData->hTrack);
    sdfChipFreeDirect(pData);
}

void effBTLSys_Shock_SetColor(effBTLSysTrack_t *pData, u32 abgr)
{
    effPCPThunderSpherePolyLowSetColor(pData->hTrack, abgr);
}

void effBTLSys_Shock_Draw(effBTLSysTrack_t *pData)
{
    sdfVector4_t position;

    if (pData->frame == 0) {
        effBTLSystem_SetUnitColorRGB(pData->pFormation, 4, 0x80FFCDCD);
    }
    effBTLModelGetMarkPosition2(pData->pFormation, 0);
    SDF_VU0_STOREVECTOR_MEMOPS(position);
    effPCPThunderSpherePolyLowSetPosition(pData->hTrack, &position);
    effPCPThunderSpherePolyLowDraw(pData->hTrack);
    pData->frame++;
}

u32 effBTLSys_Shock_GetLength(void *pData)
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

effBattleSystem_effBTLSystem_Curse_tag *effBTLSys_Freeze_Create(nbFormation_t *pFormation)
{
    effBattleSystem_effBTLSystem_Curse_tag *hEffect;

    hEffect = sdfChipAllocDirect(sizeof(effBattleSystem_effBTLSystem_Curse_tag));
    hEffect->pFormation = pFormation;
    hEffect->frame = 0;
    hEffect->hPart = dds3EffectPartCopy(effBTLSystem_Freeze_PartList[0].hPart);
    dds3EffectPartSetScale(hEffect->hPart, effBTLSystem_GetScalePtr(hEffect->pFormation));
    return hEffect;
}

void effBTLSys_Freeze_Delete(effBattleSystem_effBTLSystem_Curse_tag *hData)
{
    hData->pFormation->hunit->owner->flags &= ~2;
    effBTLSystem_SetUnitColorRGB(hData->pFormation, 0, 0x80808080);
    mdlSetClutColor(hData->pFormation->hunit->owner, 0x808080, 0.0f);
    dds3EffectPartDelete(hData->hPart);
    sdfChipFreeDirect(hData);
}

void effBTLSys_Freeze_Delete2(effBattleSystem_effBTLSystem_Curse_tag *hData)
{
    dds3EffectPartDelete(hData->hPart);
    sdfChipFreeDirect(hData);
}

void effBTLSys_Freeze_SetColor(effBattleSystem_effBTLSystem_Curse_tag *hData, u32 abgr)
{
    dds3EffectPartSetColor(hData->hPart, abgr);
}

void effBTLSys_Freeze_Draw(effBattleSystem_effBTLSystem_Curse_tag *hData)
{
    sdfVector4_t position;

    if (hData->frame == 0) {
        effBTLSystem_SetUnitColorRGB(hData->pFormation, 0, 0x80FFF8BC);
        mdlSetClutColor(hData->pFormation->hunit->owner, 0x80FFC39B, 1.0f);
        hData->pFormation->hunit->owner->flags |= 2;
    }
    effBTLModelGetMarkPosition2(hData->pFormation, 0);
    SDF_VU0_STOREVECTOR_MEMOPS(position);
    dds3EffectPartSetPosition(hData->hPart, &position);
    dds3EffectPartDraw(hData->hPart);
    hData->frame++;
}

u32 effBTLSys_Freeze_GetLength(void *hData)
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

effBattleSystem_effBTLSystem_Curse_tag *effBTLSys_Sleep_Create(nbFormation_t *pFormation)
{
    effBattleSystem_effBTLSystem_Curse_tag *hEffect;

    hEffect = sdfChipAllocDirect(sizeof(effBattleSystem_effBTLSystem_Curse_tag));
    hEffect->pFormation = pFormation;
    hEffect->frame = 0;
    hEffect->hPart = dds3EffectPartCopy(effBTLSystem_Sleep_PartList[0].hPart);
    dds3EffectPartSetScale(hEffect->hPart, effBTLSystem_GetScalePtr(hEffect->pFormation));
    return hEffect;
}

void effBTLSys_Sleep_Delete(effBattleSystem_effBTLSystem_Curse_tag *hData)
{
    effBTLSystem_SetUnitColorRGB(hData->pFormation, 4, 0x80808080);
    dds3EffectPartDelete(hData->hPart);
    sdfChipFreeDirect(hData);
}

void effBTLSys_Sleep_Delete2(effBattleSystem_effBTLSystem_Curse_tag *hData)
{
    dds3EffectPartDelete(hData->hPart);
    sdfChipFreeDirect(hData);
}

void effBTLSys_Sleep_SetColor(effBattleSystem_effBTLSystem_Curse_tag *hData, u32 abgr)
{
    dds3EffectPartSetColor(hData->hPart, abgr);
}

void effBTLSys_Sleep_Draw(effBattleSystem_effBTLSystem_Curse_tag *hData)
{
    sdfVector4_t position;

    if (hData->frame == 0) {
        effBTLSystem_SetUnitColorRGB(hData->pFormation, 4, 0x80FFB97D);
    }
    effBTLModelGetMarkPosition2(hData->pFormation, 2);
    SDF_VU0_STOREVECTOR_MEMOPS(position);
    dds3EffectPartSetPosition(hData->hPart, &position);
    dds3EffectPartDraw(hData->hPart);
    hData->frame++;
}

u32 effBTLSys_Sleep_GetLength(void *hData)
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

effBattleSystem_effBTLSystem_Curse_tag *effBTLSys_Panic_Create(nbFormation_t *pFormation)
{
    effBattleSystem_effBTLSystem_Curse_tag *hEffect;

    hEffect = sdfChipAllocDirect(sizeof(effBattleSystem_effBTLSystem_Curse_tag));
    hEffect->pFormation = pFormation;
    hEffect->frame = 0;
    hEffect->hPart = dds3EffectPartCopy(effBTLSystem_Panic_PartList[0].hPart);
    dds3EffectPartSetScale(hEffect->hPart, effBTLSystem_GetScalePtr(hEffect->pFormation));
    return hEffect;
}

void effBTLSys_Panic_Delete(effBattleSystem_effBTLSystem_Curse_tag *hData)
{
    effBTLSystem_SetUnitColorRGB(hData->pFormation, 4, 0x80808080);
    dds3EffectPartDelete(hData->hPart);
    sdfChipFreeDirect(hData);
}

void effBTLSys_Panic_Delete2(effBattleSystem_effBTLSystem_Curse_tag *hData)
{
    dds3EffectPartDelete(hData->hPart);
    sdfChipFreeDirect(hData);
}

void effBTLSys_Panic_SetColor(effBattleSystem_effBTLSystem_Curse_tag *hData, u32 abgr)
{
    dds3EffectPartSetColor(hData->hPart, abgr);
}

void effBTLSys_Panic_Draw(effBattleSystem_effBTLSystem_Curse_tag *hData)
{
    sdfVector4_t position;

    if (hData->frame == 0) {
        effBTLSystem_SetUnitColorRGB(hData->pFormation, 4, 0x8058B8FF);
    }
    effBTLModelGetMarkPosition2(hData->pFormation, 2);
    SDF_VU0_STOREVECTOR_MEMOPS(position);
    dds3EffectPartSetPosition(hData->hPart, &position);
    dds3EffectPartDraw(hData->hPart);
    hData->frame++;
}

u32 effBTLSys_Panic_GetLength(void *hData)
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

void effBTLSys_Bind_Delete(effBTLSysTrack_t *hData)
{
    effBTLSystem_SetUnitColorRGB(hData->pFormation, 4, 0x80808080);
    effPCPThunderSpherePolyLowDelete(hData->hTrack);
    sdfChipFreeDirect(hData);
}

void effBTLSys_Bind_Delete2(effBTLSysTrack_t *hData)
{
    effPCPThunderSpherePolyLowDelete(hData->hTrack);
    sdfChipFreeDirect(hData);
}

void effBTLSys_Bind_SetColor(effBTLSysTrack_t *hData, u32 abgr)
{
    effPCPThunderSpherePolyLowSetColor(hData->hTrack, abgr);
}

void effBTLSys_Bind_Draw(effBTLSysTrack_t *hData)
{
    sdfVector4_t position;

    if (hData->frame == 0) {
        effBTLSystem_SetUnitColorRGB(hData->pFormation, 4, 0x8080FFD7);
    }
    effBTLModelGetMarkPosition2(hData->pFormation, 0);
    SDF_VU0_STOREVECTOR_MEMOPS(position);
    effPCPThunderSpherePolyLowSetPosition(hData->hTrack, &position);
    effPCPThunderSpherePolyLowDraw(hData->hTrack);
    hData->frame++;
}

u32 effBTLSys_Bind_GetLength(void *hData)
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

effBattleSystem_effBTLSystem_Close_tag *effBTLSys_Close_Create(nbFormation_t *pFormation)
{
    effBattleSystem_effBTLSystem_Close_tag *hEffect;
    f32 scale;

    hEffect = sdfChipAllocDirect(sizeof(effBattleSystem_effBTLSystem_Close_tag));
    hEffect->pFormation = pFormation;
    hEffect->frame = 0;
    if (pFormation->hunit != NULL) {
        hEffect->hModel = pFormation->hunit->owner;
    } else {
        hEffect->hModel = NULL;
    }
    hEffect->hPartD3P0 = dds3EffectPartCopy(effBTLSystem_Bind_PartList[0].hPart);
    hEffect->hPartD3P1 = dds3EffectPartCopy(effBTLSystem_Bind_PartList[1].hPart);
    scale = effBTLSystem_GetScalePtr(hEffect->pFormation);
    dds3EffectPartSetScale(hEffect->hPartD3P0, scale);
    dds3EffectPartSetScale(hEffect->hPartD3P1, scale);
    return hEffect;
}

void effBTLSys_Close_Delete(effBattleSystem_effBTLSystem_Close_tag *hData)
{
    effBTLSystem_SetUnitColorRGB(hData->pFormation, 4, 0x80808080);
    dds3EffectPartDelete(hData->hPartD3P1);
    dds3EffectPartDelete(hData->hPartD3P0);
    sdfChipFreeDirect(hData);
}

void effBTLSys_Close_Delete2(effBattleSystem_effBTLSystem_Close_tag *hData)
{
    dds3EffectPartDelete(hData->hPartD3P0);
    dds3EffectPartDelete(hData->hPartD3P1);
    sdfChipFreeDirect(hData);
}

void effBTLSys_Close_SetColor(effBattleSystem_effBTLSystem_Close_tag *hData, u32 abgr)
{
    dds3EffectPartSetColor(hData->hPartD3P0, abgr);
    dds3EffectPartSetColor(hData->hPartD3P1, abgr);
}

void effBTLSys_Close_Draw(effBattleSystem_effBTLSystem_Close_tag *hData)
{
    sdfVector4_t position;

    if (hData->frame == 0) {
        effBTLSystem_SetUnitColorRGB(hData->pFormation, 4, 0x80FFCDCD);
    }
    effBTLModelGetMarkPosition3(hData->hModel, 2);
    SDF_VU0_STOREVECTOR_MEMOPS(position);
    dds3EffectPartSetPosition(hData->hPartD3P0, &position);
    dds3EffectPartSetPosition(hData->hPartD3P1, &position);
    dds3SpPoly_RollRingSetEndless(((dds3EffectPCP_t *)dds3EffectPartGetHandle(hData->hPartD3P0))->pHandle);
    dds3SpPoly_RollRingSetEndless(((dds3EffectPCP_t *)dds3EffectPartGetHandle(hData->hPartD3P1))->pHandle);
    dds3EffectPartDraw(hData->hPartD3P0);
    dds3EffectPartDraw(hData->hPartD3P1);
    hData->frame++;
}

u32 effBTLSys_Close_GetLength(void *hData)
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

effBattleSystem_effBTLSystem_Poison_tag *effBTLSys_Poison_Create(nbFormation_t *pFormation)
{
    effBattleSystem_effBTLSystem_Poison_tag *hEffect;

    hEffect = sdfChipAllocDirect(sizeof(effBattleSystem_effBTLSystem_Poison_tag));
    hEffect->pFormation = pFormation;
    hEffect->frame = 0;
    if (pFormation->hunit != NULL) {
        hEffect->hModel = pFormation->hunit->owner;
    } else {
        hEffect->hModel = NULL;
    }
    hEffect->hPartD3P = dds3EffectPartCopy(effBTLSystem_Poison_PartList[0].hPart);
    dds3EffectPartSetScale(hEffect->hPartD3P, effBTLSystem_GetScalePtr(hEffect->pFormation));
    return hEffect;
}

void effBTLSys_Poison_Delete(effBattleSystem_effBTLSystem_Poison_tag *hData)
{
    effBTLSystem_SetUnitColorRGB(hData->pFormation, 4, 0x80808080);
    dds3EffectPartDelete(hData->hPartD3P);
    sdfChipFreeDirect(hData);
}

void effBTLSys_Poison_Delete2(effBattleSystem_effBTLSystem_Poison_tag *hData)
{
    dds3EffectPartDelete(hData->hPartD3P);
    sdfChipFreeDirect(hData);
}

void effBTLSys_Poison_SetColor(effBattleSystem_effBTLSystem_Poison_tag *hData, u32 abgr)
{
    dds3EffectPartSetColor(hData->hPartD3P, abgr);
}

void effBTLSys_Poison_Draw(effBattleSystem_effBTLSystem_Poison_tag *hData)
{
    sdfVector4_t position;

    if (hData->frame == 0) {
        effBTLSystem_SetUnitColorRGB(hData->pFormation, 4, 0x80A53A6C);
    }
    effBTLModelGetMarkPosition3(hData->hModel, 2);
    SDF_VU0_STOREVECTOR_MEMOPS(position);
    dds3EffectPartSetPosition(hData->hPartD3P, &position);
    dds3EffectPartDraw(hData->hPartD3P);
    hData->frame++;
}

u32 effBTLSys_Poison_GetLength(void *hData)
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

void effBTLSys_Charm_Delete(effBTLSysPart2_t *hData)
{
    effBTLSystem_SetUnitColorRGB(hData->pFormation, 4, 0x80808080);
    dds3EffectPartDelete(hData->hPart);
    dds3EffectPartDelete(hData->hPart2);
    sdfChipFreeDirect(hData);
}

void effBTLSys_Charm_Delete2(effBTLSysPart2_t *hData)
{
    dds3EffectPartDelete(hData->hPart);
    dds3EffectPartDelete(hData->hPart2);
    sdfChipFreeDirect(hData);
}

void effBTLSys_Charm_SetColor(effBTLSysPart2_t *hData, u32 abgr)
{
    dds3EffectPartSetColor(hData->hPart, abgr);
    dds3EffectPartSetColor(hData->hPart2, abgr);
}

void effBTLSys_Charm_Draw(effBTLSysPart2_t *hData)
{
    sdfVector4_t position;

    if (hData->frame == 0) {
        effBTLSystem_SetUnitColorRGB(hData->pFormation, 4, 0x808064FF);
    }
    effBTLModelGetMarkPosition2(hData->pFormation, 2);
    SDF_VU0_STOREVECTOR_MEMOPS(position);
    dds3EffectPartSetPosition(hData->hPart, &position);
    dds3EffectPartSetPosition(hData->hPart2, &position);
    dds3EffectPartDraw(hData->hPart);
    dds3EffectPartDraw(hData->hPart2);
    hData->frame++;
}

u32 effBTLSys_Charm_GetLength(void *hData)
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

void effBTLSys_Paralyze_Delete(effBTLSysTrack_t *hData)
{
    effBTLSystem_SetUnitColorRGB(hData->pFormation, 4, 0x80808080);
    effPCPThunderSpherePolyLowDelete(hData->hTrack);
    sdfChipFreeDirect(hData);
}

void effBTLSys_Paralyze_Delete2(effBTLSysTrack_t *hData)
{
    effPCPThunderSpherePolyLowDelete(hData->hTrack);
    sdfChipFreeDirect(hData);
}

void effBTLSys_Paralyze_SetColor(effBTLSysTrack_t *hData, u32 abgr)
{
    effPCPThunderSpherePolyLowSetColor(hData->hTrack, abgr);
}

void effBTLSys_Paralyze_Draw(effBTLSysTrack_t *hData)
{
    sdfVector4_t position;

    if (hData->frame == 0) {
        effBTLSystem_SetUnitColorRGB(hData->pFormation, 4, 0x80589BFF);
    }
    effBTLModelGetMarkPosition2(hData->pFormation, 0);
    SDF_VU0_STOREVECTOR_MEMOPS(position);
    effPCPThunderSpherePolyLowSetPosition(hData->hTrack, &position);
    effPCPThunderSpherePolyLowDraw(hData->hTrack);
    hData->frame++;
}

u32 effBTLSys_Paralyze_GetLength(void *hData)
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

void effBTLSys_Fly_Delete(effBTLSysFrame_t *hData)
{
    effBTLSystem_SetUnitColorRGB(hData->pFormation, 4, 0x80808080);
    sdfChipFreeDirect(hData);
}

void effBTLSys_Fly_Delete2(effBTLSysFrame_t *hData)
{
    sdfChipFreeDirect(hData);
}

void effBTLSys_Fly_SetColor(void) {
}

void effBTLSys_Fly_Draw(effBTLSysFrame_t *hData)
{
    if (hData->frame == 0) {
        effBTLSystem_SetUnitColorRGB(hData->pFormation, 4, 0x80FFCDCD);
    }
    hData->frame++;
}

u32 effBTLSys_Fly_GetLength(void *hData)
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

void effBTLSys_Stone_Delete(effBTLSysFrame_t *hData)
{
    mdlObj_t *hModel = hData->pFormation->hunit->owner;

    hModel->flags &= ~2;
    mdlSetClutColor(hModel, 0x808080, 0.0f);
    sdfChipFreeDirect(hData);
}

void effBTLSys_Stone_Delete2(effBTLSysFrame_t *hData)
{
    sdfChipFreeDirect(hData);
}

void effBTLSys_Stone_SetColor(void) {
}

void effBTLSys_Stone_Draw(effBTLSysFrame_t *hData)
{
    if (hData->frame == 0) {
        mdlSetClutColor(hData->pFormation->hunit->owner, 0x80FFEBE1, 1.0f);
        hData->pFormation->hunit->owner->flags |= 2;
    }
    hData->frame++;
}

u32 effBTLSys_Stone_GetLength(void *hData)
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

effBattleSystem_effBTLSystem_Curse_tag *effBTLSys_Curse_Create(nbFormation_t *pFormation)
{
    effBattleSystem_effBTLSystem_Curse_tag *hEffect;

    hEffect = sdfChipAllocDirect(sizeof(effBattleSystem_effBTLSystem_Curse_tag));
    hEffect->pFormation = pFormation;
    hEffect->frame = 0;
    hEffect->hPart = dds3EffectPartCopy(effBTLSystem_Curse_PartList[0].hPart);
    dds3EffectPartSetScale(hEffect->hPart, effBTLSystem_GetScalePtr(hEffect->pFormation));
    return hEffect;
}

void effBTLSys_Curse_Delete(effBattleSystem_effBTLSystem_Curse_tag *hData)
{
    dds3EffectPartDelete(hData->hPart);
    sdfChipFreeDirect(hData);
}

void effBTLSys_Curse_Delete2(effBattleSystem_effBTLSystem_Curse_tag *hData)
{
    dds3EffectPartDelete(hData->hPart);
    sdfChipFreeDirect(hData);
}

void effBTLSys_Curse_SetColor(effBattleSystem_effBTLSystem_Curse_tag *hData, u32 abgr)
{
    dds3EffectPartSetColor(hData->hPart, abgr);
}

void effBTLSys_Curse_Draw(effBattleSystem_effBTLSystem_Curse_tag *hData)
{
    dds3EffectPartSetPosition(hData->hPart, &hData->pFormation->pos);
    dds3EffectPartDraw(hData->hPart);
    hData->frame++;
}

u32 effBTLSys_Curse_GetLength(void *hData)
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

void effBTLSys_CurseCAMP_Delete(effBTLSysPart2_t *hData)
{
    dds3EffectPartDelete(hData->hPart);
    dds3EffectPartDelete(hData->hPart2);
    sdfChipFreeDirect(hData);
}

void effBTLSys_CurseCAMP_Delete2(effBTLSysPart2_t *hData)
{
    dds3EffectPartDelete(hData->hPart);
    dds3EffectPartDelete(hData->hPart2);
    sdfChipFreeDirect(hData);
}

void effBTLSys_CurseCAMP_SetColor(effBTLSysPart2_t *hData, u32 abgr)
{
    dds3EffectPartSetColor(hData->hPart, abgr);
    dds3EffectPartSetColor(hData->hPart2, abgr);
}

void effBTLSys_CurseCAMP_Draw(effBTLSysPart2_t *hData)
{
    sdfVector4_t position;

    dds3EffectPartSetPosition(hData->hPart, &hData->pFormation->pos);
    position.x = hData->pFormation->pos.x;
    position.y = hData->pFormation->pos.y - 100.0f;
    position.z = hData->pFormation->pos.z;
    dds3EffectPartSetPosition(hData->hPart2, &position);
    dds3EffectPartDraw(hData->hPart);
    dds3EffectPartDraw(hData->hPart2);
    hData->frame++;
}

u32 effBTLSys_CurseCAMP_GetLength(void *hData)
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

effBattleSystem_effBTLSystem_Stone_Dead_tag *effBTLSys_Stone_Dead_Create(nbFormation_t *pFormation)
{
    effBattleSystem_effBTLSystem_Stone_Dead_tag *hEffect;
    f32 scale;

    hEffect = sdfChipAllocDirect(sizeof(effBattleSystem_effBTLSystem_Stone_Dead_tag));
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

void effBTLSys_Stone_Dead_Delete(effBattleSystem_effBTLSystem_Stone_Dead_tag *hData)
{
    mdlSetClutColor(hData->pFormation->hunit->owner, 0x808080, 0.0f);
    dds3EffectPartDelete(hData->hPart3);
    dds3EffectPartDelete(hData->hPart2);
    dds3EffectPartDelete(hData->hPart);
    sdfChipFreeDirect(hData);
}

void effBTLSys_Stone_Dead_Delete2(effBattleSystem_effBTLSystem_Stone_Dead_tag *hData)
{
    dds3EffectPartDelete(hData->hPart3);
    dds3EffectPartDelete(hData->hPart2);
    dds3EffectPartDelete(hData->hPart);
    sdfChipFreeDirect(hData);
}

void effBTLSys_Stone_Dead_SetColor(void) {
}

void effBTLSys_Stone_Dead_Draw(effBattleSystem_effBTLSystem_Stone_Dead_tag *hData)
{
    sdfVector4_t position;

    if (hData->frame == 0) {
        effBTLModelHide(hData->pFormation);
        mdlSetClutColor(hData->pFormation->hunit->owner, 0x80FFEBE1, 1.0f);
        hData->pFormation->hunit->owner->flags |= 2;
    }
    effBTLModelGetMarkPosition2(hData->pFormation, 0);
    SDF_VU0_STOREVECTOR_MEMOPS(position);
    dds3EffectPartSetPosition(hData->hPart3, &position);
    dds3EffectPartSetPosition(hData->hPart2, &position);
    dds3EffectPartSetPosition(hData->hPart, &position);
    dds3EffectPartDraw(hData->hPart3);
    dds3EffectPartDraw(hData->hPart2);
    if (hData->frame >= 2) {
        dds3EffectPartDraw(hData->hPart);
    }
    hData->frame++;
}

u32 effBTLSys_Stone_Dead_GetLength(void *hData)
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

effBattleSystem_effBTLSystem_Hansya_tag *effBTLSys_Hansya_Create(nbFormation_t *pFormation)
{
    effBattleSystem_effBTLSystem_Hansya_tag *hEffect;
    f32 scale;

    hEffect = sdfChipAllocDirect(sizeof(effBattleSystem_effBTLSystem_Hansya_tag));
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

void effBTLSys_Hansya_Delete(effBattleSystem_effBTLSystem_Hansya_tag *hData)
{
    dds3EffectPartDelete(hData->hPart2);
    dds3EffectPartDelete(hData->hPart);
    sdfChipFreeDirect(hData);
}

void effBTLSys_Hansya_Delete2(effBattleSystem_effBTLSystem_Hansya_tag *hData)
{
    dds3EffectPartDelete(hData->hPart2);
    dds3EffectPartDelete(hData->hPart);
    sdfChipFreeDirect(hData);
}

void effBTLSys_Hansya_SetColor(void) {
}

INCLUDE_ASM(const s32, "effect/effBattleSystem", effBTLSys_Hansya_Draw);

u32 effBTLSys_Hansya_GetLength(void *hData)
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

effBattleSystem_effBTLSystem_Kyusyuu_tag *effBTLSys_Kyusyuu_Create(nbFormation_t *pFormation)
{
    effBattleSystem_effBTLSystem_Kyusyuu_tag *hEffect;
    f32 scale;

    hEffect = sdfChipAllocDirect(sizeof(effBattleSystem_effBTLSystem_Kyusyuu_tag));
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

void effBTLSys_Kyusyuu_Delete(effBattleSystem_effBTLSystem_Kyusyuu_tag *hData)
{
    dds3EffectPartDelete(hData->hPart4);
    dds3EffectPartDelete(hData->hPart3);
    dds3EffectPartDelete(hData->hPart2);
    dds3EffectPartDelete(hData->hPart);
    sdfChipFreeDirect(hData);
}

void effBTLSys_Kyusyuu_Delete2(effBattleSystem_effBTLSystem_Kyusyuu_tag *hData)
{
    dds3EffectPartDelete(hData->hPart4);
    dds3EffectPartDelete(hData->hPart3);
    dds3EffectPartDelete(hData->hPart2);
    dds3EffectPartDelete(hData->hPart);
    sdfChipFreeDirect(hData);
}

void effBTLSys_Kyusyuu_SetColor(void) {
}

INCLUDE_ASM(const s32, "effect/effBattleSystem", effBTLSys_Kyusyuu_Draw);

u32 effBTLSys_Kyusyuu_GetLength(void *hData)
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

void effBTLSys_Return_Delete(effBTLSysCut_t *hData)
{
    effBTLCutDelete(hData->hCut);
    sdfChipFreeDirect(hData);
}

void effBTLSys_Return_Delete2(effBTLSysCut_t *hData)
{
    effBTLCutDelete(hData->hCut);
    sdfChipFreeDirect(hData);
}

void effBTLSys_Return_SetColor(void) {
}

void effBTLSys_Return_Draw(effBTLSysCut_t *hData)
{
    effBTLCutDraw(hData->hCut);
    hData->frame++;
}

u32 effBTLSys_Return_GetLength(effBTLSysCut_t *hData)
{
    return effBTLCutGetCutEndFrame(hData->hCut);
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

effBattleSystem_effBTLSystem_Attack_tag *effBTLSys_Attack_Create(nbFormation_t *pFormation)
{
    effBattleSystem_effBTLSystem_Attack_tag *hEffect;

    hEffect = sdfChipAllocDirect(sizeof(effBattleSystem_effBTLSystem_Attack_tag));
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

void effBTLSys_Attack_Delete(effBattleSystem_effBTLSystem_Attack_tag *hData)
{
    dds3EffectPartDelete(hData->hPart6);
    dds3EffectPartDelete(hData->hPart5);
    dds3EffectPartDelete(hData->hPart4);
    dds3EffectPartDelete(hData->hPart3);
    dds3EffectPartDelete(hData->hPart2);
    dds3EffectPartDelete(hData->hPart);
    sdfChipFreeDirect(hData);
}

void effBTLSys_Attack_Delete2(effBattleSystem_effBTLSystem_Attack_tag *hData)
{
    dds3EffectPartDelete(hData->hPart6);
    dds3EffectPartDelete(hData->hPart5);
    dds3EffectPartDelete(hData->hPart4);
    dds3EffectPartDelete(hData->hPart3);
    dds3EffectPartDelete(hData->hPart2);
    dds3EffectPartDelete(hData->hPart);
    sdfChipFreeDirect(hData);
}

void effBTLSys_Attack_SetColor(void) {
}

INCLUDE_ASM(const s32, "effect/effBattleSystem", effBTLSys_Attack_Draw);

u32 effBTLSys_Attack_GetLength(void *hData)
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

effBattleSystem_effBTLSystem_AttackC_tag *effBTLSys_AttackC_Create(nbFormation_t *pFormation)
{
    effBattleSystem_effBTLSystem_AttackC_tag *hEffect;

    hEffect = sdfChipAllocDirect(sizeof(effBattleSystem_effBTLSystem_AttackC_tag));
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

void effBTLSys_AttackC_Delete(effBattleSystem_effBTLSystem_AttackC_tag *hData)
{
    dds3EffectPartDelete(hData->hPart12);
    dds3EffectPartDelete(hData->hPart11);
    dds3EffectPartDelete(hData->hPart10);
    dds3EffectPartDelete(hData->hPart9);
    dds3EffectPartDelete(hData->hPart8);
    dds3EffectPartDelete(hData->hPart7);
    dds3EffectPartDelete(hData->hPart6);
    dds3EffectPartDelete(hData->hPart5);
    dds3EffectPartDelete(hData->hPart4);
    dds3EffectPartDelete(hData->hPart3);
    dds3EffectPartDelete(hData->hPart2);
    dds3EffectPartDelete(hData->hPart);
    sdfChipFreeDirect(hData);
}

void effBTLSys_AttackC_Delete2(effBattleSystem_effBTLSystem_AttackC_tag *hData)
{
    dds3EffectPartDelete(hData->hPart12);
    dds3EffectPartDelete(hData->hPart11);
    dds3EffectPartDelete(hData->hPart10);
    dds3EffectPartDelete(hData->hPart9);
    dds3EffectPartDelete(hData->hPart8);
    dds3EffectPartDelete(hData->hPart7);
    dds3EffectPartDelete(hData->hPart6);
    dds3EffectPartDelete(hData->hPart5);
    dds3EffectPartDelete(hData->hPart4);
    dds3EffectPartDelete(hData->hPart3);
    dds3EffectPartDelete(hData->hPart2);
    dds3EffectPartDelete(hData->hPart);
    sdfChipFreeDirect(hData);
}

void effBTLSys_AttackC_SetColor(void) {
}

INCLUDE_ASM(const s32, "effect/effBattleSystem", effBTLSys_AttackC_Draw);

u32 effBTLSys_AttackC_GetLength(void *hData)
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

void effBTLSys_Encount_Delete(effBTLSysCut_t *pData)
{
    effBTLCutDelete(pData->hCut);
    sdfChipFreeDirect(pData);
}

void effBTLSys_Encount_Delete2(effBTLSysCut_t *pData)
{
    effBTLCutDelete(pData->hCut);
    sdfChipFreeDirect(pData);
}

void effBTLSys_Encount_Draw(effBTLSysCut_t *pData)
{
    effBTLCutDraw(pData->hCut);
    pData->frame++;
}

void effBTLSys_Encount_SetColor(void) {
}

u32 effBTLSys_Encount_GetLength(effBTLSysCut_t *pData)
{
    return effBTLCutGetCutEndFrame(pData->hCut);
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

void effBTLSys_Nego_Kaihuku_Delete(effBTLSysCut_t *hData)
{
    effBTLCutDelete(hData->hCut);
    sdfChipFreeDirect(hData);
}

void effBTLSys_Nego_Kaihuku_Delete2(effBTLSysCut_t *hData)
{
    effBTLCutDelete(hData->hCut);
    sdfChipFreeDirect(hData);
}

void effBTLSys_Nego_Kaihuku_Draw(effBTLSysCut_t *hData)
{
    effBTLCutDraw(hData->hCut);
    hData->frame++;
}

u32 effBTLSys_Nego_Kaihuku_GetLength(effBTLSysCut_t *hData)
{
    return effBTLCutGetCutEndFrame(hData->hCut);
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

void effBTLSys_Nego_Kyusyu_Delete(effBTLSysCut_t *hData)
{
    effBTLCutDelete(hData->hCut);
    sdfChipFreeDirect(hData);
}

void effBTLSys_Nego_Kyusyu_Delete2(effBTLSysCut_t *hData)
{
    effBTLCutDelete(hData->hCut);
    sdfChipFreeDirect(hData);
}

void effBTLSys_Nego_Kyusyu_Draw(effBTLSysCut_t *hData)
{
    effBTLCutDraw(hData->hCut);
    hData->frame++;
}

u32 effBTLSys_Nego_Kyusyu_GetLength(effBTLSysCut_t *hData)
{
    return effBTLCutGetCutEndFrame(hData->hCut);
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

void effBTLSys_Nego_Stone_Delete(effBTLSysCut_t *hData)
{
    effBTLCutDelete(hData->hCut);
    sdfChipFreeDirect(hData);
}

void effBTLSys_Nego_Stone_Delete2(effBTLSysCut_t *hData)
{
    effBTLCutDelete(hData->hCut);
    sdfChipFreeDirect(hData);
}

void effBTLSys_Nego_Stone_Draw(effBTLSysCut_t *hData)
{
    effBTLCutDraw(hData->hCut);
    hData->frame++;
}

u32 effBTLSys_Nego_Stone_GetLength(effBTLSysCut_t *hData)
{
    return effBTLCutGetCutEndFrame(hData->hCut);
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

effBTLSystemHandle_t *effBTLSystemCreate(nbFormation_t *pFormation, s32 id)
{
    effBTLSystemHandle_t *hEffect;

    hEffect = sdfChipAllocDirect(sizeof(effBTLSystemHandle_t));
    hEffect->family = id;
    hEffect->pFormation = pFormation;
    if (D_0031BF00[id].create != NULL) {
        hEffect->hEffect = D_0031BF00[id].create(pFormation);
    }
    return hEffect;
}

void effBTLSystemDelete(effBTLSystemHandle_t *hEffBTLSys)
{
    if (D_0031BF00[hEffBTLSys->family].destroy != NULL) {
        D_0031BF00[hEffBTLSys->family].destroy(hEffBTLSys->hEffect);
    }
    sdfChipFreeDirect(hEffBTLSys);
}

void effBTLSystemDelete2(effBTLSystemHandle_t *hEffBTLSys)
{
    if (D_0031BF00[hEffBTLSys->family].destroy2 != NULL) {
        D_0031BF00[hEffBTLSys->family].destroy2(hEffBTLSys->hEffect);
    }
    sdfChipFreeDirect(hEffBTLSys);
}

void effBTLSystemDraw(effBTLSystemHandle_t *hEffBTLSys)
{
    s32 family = hEffBTLSys->family;
    evtUnit_t *unit;
    u32 color;

    if (family != 0x14) {
        if ((((nbFormation_t *)hEffBTLSys->pFormation)->stat & 1) == 0) {
            return;
        }
        unit = ((nbFormation_t *)hEffBTLSys->pFormation)->hunit;
        if (unit == NULL) {
            return;
        }
        if (family != 0xE && (unit->owner->flags & 1) != 0) {
            return;
        }
    }

    if (D_0031BF00[family].setColor != NULL &&
        (((nbFormation_t *)hEffBTLSys->pFormation)->stat & 2) != 0 &&
        ((nbFormation_t *)hEffBTLSys->pFormation)->hunit->owner != NULL) {
        color = (mdlGetColor(((nbFormation_t *)hEffBTLSys->pFormation)->hunit->owner) &
                 0xFF000000) |
            0x00808080;
        D_0031BF00[family].setColor(hEffBTLSys->hEffect, color);
    }

    if (D_0031BF00[family].draw != NULL) {
        D_0031BF00[family].draw(hEffBTLSys->hEffect);
    }
}

u32 effBTLSystemGetLength(effBTLSystemHandle_t *hEffBTLSys)
{
    if (D_0031BF00[hEffBTLSys->family].getLength != NULL) {
        return D_0031BF00[hEffBTLSys->family].getLength(hEffBTLSys->hEffect);
    }
    return 0;
}

s32 effBTLSystemGetID(effBTLSystemHandle_t *hEffBTLSys)
{
    return hEffBTLSys->family;
}

u32 effBTLSystemGetRGBA(s32 id)
{
    return D_0031BF00[id].rgba;
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
            effBTLSystemCut1Data[i].use = 0;
        }
    }
}

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533300);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533318);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533338);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533350);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533368);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533380);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533398);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_005333B0);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_005333D0);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_005333E8);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533400);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533418);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533438);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533450);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533468);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533488);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_005334A8);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_005334C0);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_005334D0);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_005334F0);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533510);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533520);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533538);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533558);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533578);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533590);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_005335A8);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_005335C0);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_005335E0);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_005335F0);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533608);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533620);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533638);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533650);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533668);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533680);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533690);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_005336A8);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_005336C8);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_005336E8);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533708);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533720);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533738);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533750);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533768);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533780);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533798);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_005337B0);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_005337C8);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_005337E0);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_005337F8);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533810);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533828);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533840);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533858);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533878);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533890);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_005338A8);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_005338C0);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_005338D8);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_005338F8);

void *effBTLSystemCut1Create(nbFormation_t *pFormation, s32 index)
{
    effBattleSystem_effBTLSystem_Cut1_tag *pData;
    void *hCut;

loop:
    pData = &effBTLSystemCut1Data[index];
    if (pData->pData == NULL) {
        index = 1;
        goto loop;
    }
    if (pData->use == 0) {
        pData->hBTLEffect = effBTLCreate(pData->pData);
        printf("effBTLSys:effBTLCreate[%d]\n", index);
    }
    pData->use++;
    hCut = effBTLCutCreate(pData->hBTLEffect, 0, pFormation, 0);
    printf("effBTLSys:effBTLCutCreate\n");
    effBTLCutSetUserData(hCut, pData);
    return hCut;
}

void effBTLSystemCut1Draw(void *hBTLCutEffect)
{
    effBTLCutDraw(hBTLCutEffect);
}

void effBTLSystemCut1Delete(void *hBTLCutEffect)
{
    effBattleSystem_effBTLSystem_Cut1_tag *pData;

    pData = effBTLCutGetUserData(hBTLCutEffect);
    effBTLCutDelete(hBTLCutEffect);
    printf("effBTLSys:effBTLCutDelete\n");
    pData->use--;
    if (pData->use == 0) {
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
            effBTLSystemCut1Data[i].use = 0;
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

INCLUDE_ASM(const s32, "effect/effBattleSystem", effBTLSystemInit);

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

typedef struct
{
    sdfVector4_t pos;
    sdfVector4_t rot;
    sdfVector4_t center;
    f32 r;
    f32 h;
    f32 scl;
    mdlObj_t *hModel;
} effCAMPBadParams_t;

typedef struct
{
    nbFormation_t formation;
    effCAMPBadParams_t params;
    effBTLSystemHandle_t *hEffBTLSys;
    u32 id;
    s32 frame;
} effBattleSystem_effCAMPBad_tag;

extern void mdlSetColor(mdlObj_t *model, u32 color);
extern u32 effColorBlend(u32 color1, u32 color2, f32 ratio);

void *effCAMPBadCreate(effCAMPBadParams_t *params, s32 id)
{
    effBattleSystem_effCAMPBad_tag *work;
    work = sdfChipAllocDirect(0x13C);
    memcpy(&work->params, params, sizeof(work->params));
    work->id = id;
    work->frame = 0;
    memset(&work->formation, 0, sizeof(work->formation));
    SDF_VU0_COPYVECTOR(&work->formation.pos, &work->params.pos);
    SDF_VU0_COPYVECTOR(&work->formation.rot, &work->params.rot);
    SDF_VU0_COPYVECTOR(&work->formation.center, &work->params.center);
    work->formation.r = work->params.r;
    work->formation.h = work->params.h;
    work->formation.scl = work->params.scl;
    if (D_0031BF00[work->id].partHandleCreate != NULL) {
        D_0031BF00[work->id].partHandleCreate();
    }
    work->hEffBTLSys = effBTLSystemCreate(&work->formation, work->id);
    switch (work->id) {
    case 7:
        ((effBattleSystem_effBTLSystem_Poison_tag *)work->hEffBTLSys->hEffect)->hModel = work->params.hModel;
        break;
    case 6:
        ((effBattleSystem_effBTLSystem_Close_tag *)work->hEffBTLSys->hEffect)->hModel = work->params.hModel;
        break;
    }
    return work;
}

void effCAMPBadDraw(effBattleSystem_effCAMPBad_tag *work)
{
    u32 color;

    if (D_0031BF00[work->id].draw != NULL) {
        D_0031BF00[work->id].draw(work->hEffBTLSys->hEffect);
    }
    if (work->params.hModel != NULL) {
        color = D_0031BF00[work->id].rgba;
        if (work->frame < 8) {
            color = effColorBlend(0x80808080, color, work->frame * 0.125f);
        }
        mdlSetColor(work->params.hModel, color);
    }
    work->frame++;
}

void effCAMPBadDelete(effBattleSystem_effCAMPBad_tag *work)
{
    effBTLSystemDelete(work->hEffBTLSys);
    if (work->params.hModel != NULL) {
        mdlSetColor(work->params.hModel, 0x80808080);
    }
    if (D_0031BF00[work->id].partHandleDelete != NULL) {
        D_0031BF00[work->id].partHandleDelete();
    }
    sdfChipFreeDirect(work);
}

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

extern void effDebugStringDraw(s32 x, s32 y, s32 commandIndex, const char *text);
extern char *D_0054F0F8;
extern char D_0054F110[];
extern char D_0054F118[];
extern char D_0054F120[];
extern char D_0054F128[];

void effBTLCutDebugPrint(void)
{
    char text[0x70];
    nbMainProcessData_t *mainData;

    if (effBTLSkillActive == 0) {
        return;
    }
    mainData = nbGetMainProcessData();
    if (mainData->debug != 0) {
        sprintf(text, D_0054F110, effBTLCutCount);
        effDebugStringDraw(0x20, 0x166, 0, text);
        sprintf(text, D_0054F118, effBTLCutMaxTable[0]);
        effDebugStringDraw(0x20, 0x172, 0, text);
        sprintf(text, D_0054F120, effBTLCutMaxTable[1]);
        effDebugStringDraw(0x20, 0x17E, 0, text);
        sprintf(text, D_0054F128, effBTLCutMaxTable[2]);
        effDebugStringDraw(0x20, 0x18A, 0, text);
        effDebugStringDraw(0x20, 0x1A2, 0,
            effBTLFieldColorIsOn() ? D_0054F0F8 : effBedNameBuf);
    }
    effBTLCutCount++;
}


extern s32 effBTLModelGetMarkPosition(nbFormation_t *formation, s32 mark);
extern void effDebugPointDraw2(sdfVector4_t *point, u32 color);
extern void effDebugWireframeSphereDraw(sdfVector4_t *center, u32 color, f32 radius);
extern void effDebugPointDraw(sdfVector4_t *point);

void func_00174AB8(u32 color1, u32 color3)
{
    nbFormation_t *formation;
    sdfVector4_t center;
    s32 i;

    formation = nbGetMainProcessData()->form;
    for (i = 31; i >= 0; i--, formation++) {
        if ((formation->stat & 3) == 3) {
            if (effBTLModelGetMarkPosition(formation, 1) != 0) {
                SDF_VU0_STOREVECTOR(&center);
                effDebugPointDraw2(&center, color1);
                effDebugWireframeSphereDraw(&center, color1,
                    formation->oneshotr * formation->scl);
            }
            if (effBTLModelGetMarkPosition(formation, 3) != 0) {
                SDF_VU0_STOREVECTOR(&center);
                effDebugPointDraw2(&center, color3);
                effDebugWireframeSphereDraw(&center, color3,
                    formation->allshotr * formation->scl);
            }
            effDebugPointDraw(&center);
        }
    }
}

extern void effBTLModelGetSphereCenter(nbFormation_t *formation);
extern void effDebugWireframeCylinderDraw(sdfVector4_t *center, u32 color, f32 radius, f32 height);
extern void effDebugPointDraw(sdfVector4_t *point);

void func_00174BB0(u32 color)
{
    sdfVector4_t center;
    nbFormation_t *formation;
    s32 i;

    formation = nbGetMainProcessData()->form;
    for (i = 31; i >= 0; i--, formation++) {
        if ((formation->stat & 3) == 3) {
            effBTLModelGetSphereCenter(formation);
            SDF_VU0_STOREVECTOR(&center);
            effDebugWireframeCylinderDraw(&center, color,
                formation->r * formation->scl, formation->h * formation->scl);
            effDebugPointDraw(&center);
        }
    }
}

extern f32 effBTLModelGroupGetSphere2(u32 group);
extern void effDebugWireframeSphereDraw(sdfVector4_t *center, u32 color, f32 radius);
extern void effDebugPointDraw(sdfVector4_t *point);

void func_00174C50(u32 color)
{
    sdfVector4_t center;
    f32 radius;

    radius = effBTLModelGroupGetSphere2(0x100);
    SDF_VU0_STOREVECTOR(&center);
    effDebugWireframeSphereDraw(&center, color, radius);
    effDebugPointDraw(&center);
    radius = effBTLModelGroupGetSphere2(0x200);
    SDF_VU0_STOREVECTOR(&center);
    effDebugWireframeSphereDraw(&center, color, radius);
    effDebugPointDraw(&center);
}

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533A40);

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

extern void dds3EffectFileListWindowDraw(void *window);
extern u32 dds3EffectFileListWindowGetState(void *window);
extern u32 dds3EffectFileListWindowGetFilename(void *window, char *filename);
extern void *func_001AA310(void *viewer);
extern char D_0056AA38[];
extern char *D_0054F0F8;

void eff_174D38(void)
{
    dds3EffectFileListWindowDraw(D_00551DA4);
    if (dds3EffectFileListWindowGetState(D_00551DA4) == 1) {
        dds3EffectFileListWindowGetFilename(D_00551DA4, D_0056AA38);
        D_0054F0F8 = D_0056AA38;
        nbHelperViewer = func_001AA310(nbHelperViewer);
    }
}

void eff_174D98(void)
{
    dds3EffectFileListWindowDelete(D_00551DA4);
    dds3EffectFileListDelete(D_00551DA0);
}

INCLUDE_ASM(const s32, "effect/effBattleSystem", func_00174DC0);

INCLUDE_ASM(const s32, "effect/effBattleSystem", eff_174E88);

typedef struct {
    sdf3DLight_t *parallel[4];
    sdfVector4_t ambient;
    sdf3DFogParam_t fog;
} dds3LightState_t;

extern dds3LightState_t dds3LightParam;
PS2_SYMBOL_AT(sdfVector4_t, dds3AmbientColor, dds3LightParam, 0x10);
extern sdfVector4_t D_0056AC40;
extern sdfVector4_t D_0056AC50;
extern u8 D_0054F18A;
extern u8 D_0054F18B;
extern void dds3UnlinkSceneLight(dds3Basic_t *light);

s32 func_00175030(void)
{
    SDF_VU0_COPYVECTOR(&D_0056AC40, dds3LightParam.parallel[0]);
    SDF_VU0_COPYVECTOR(&D_0056AC50, &dds3AmbientColor);
    D_0054F18B = 1;
    return 0;
}

void eff_175070(void)
{
    if (D_0054F18A == 0) {
        dds3UnlinkSceneLight(nbGetMainProcessData()->light[1]);
        SDF_VU0_COPYVECTOR(dds3LightParam.parallel[0], &D_0056AC40);
        SDF_VU0_COPYVECTOR(&dds3AmbientColor, &D_0056AC50);
        D_0054F18B = 0;
    }
}

typedef struct fldResourceLight_t {
    u32 resv;
    u32 Flag;
    u32 AnimeType;
    f32 InsideR;
    f32 OutsideR;
    f32 Soften;
    f32 Bias;
    f32 DiffuseR;
    f32 DiffuseG;
    f32 DiffuseB;
    f32 AmbientR;
    f32 AmbientG;
    f32 AmbientB;
} fldResourceLight_t;

extern u8 D_0054F18B;
extern u32 D_0054F18C;
extern u32 D_0054F190;
extern fldResourceLight_t D_0056AC60;
extern dds3Effect_Filter_t D_0031CA90;
extern void dds3SetLightObjectResoruce(dds3Basic_t *basic, fldResourceLight_t *resource);
extern void dds3LinkSceneLight(dds3Basic_t *basic, s32 frame);
extern void dds3EffectFilter(dds3Effect_Filter_t *params);

void func_001750D0(s32 param)
{
    nbMainProcessData_t *mainData;
    u32 color;
    u32 redChannel;
    u32 greenChannel;
    u32 blueChannel;
    f32 red;
    f32 green;
    f32 blue;
    f32 scaleRed;
    f32 scaleGreen;
    f32 scaleBlue;

    if (D_0054F18B != 0) {
        color = D_0054F18C;
        blueChannel = (color >> 16) & 0xFF;
        blue = (f32)blueChannel / 255.0f;
        greenChannel = (color >> 8) & 0xFF;
        green = (f32)greenChannel / 255.0f;
        redChannel = color & 0xFF;
        red = (f32)redChannel / 255.0f;

        color = D_0054F190;
        blueChannel = (color >> 16) & 0xFF;
        scaleBlue = (f32)blueChannel * 0.0078125f;
        greenChannel = (color >> 8) & 0xFF;
        scaleGreen = (f32)greenChannel * 0.0078125f;
        redChannel = color & 0xFF;
        scaleRed = (f32)redChannel * 0.0078125f;

        D_0056AC60.DiffuseR = red * scaleRed;
        D_0056AC60.DiffuseG = green * scaleGreen;
        D_0056AC60.DiffuseB = blue * scaleBlue;
        D_0056AC60.AmbientR = red * scaleRed;
        D_0056AC60.AmbientG = green * scaleGreen;
        D_0056AC60.AmbientB = blue * scaleBlue;

        mainData = nbGetMainProcessData();
        dds3SetLightObjectResoruce(mainData->light[1], &D_0056AC60);
        dds3LinkSceneLight(mainData->light[1], 0);
        dds3EffectFilter(&D_0031CA90);
    }
}

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

INCLUDE_ASM(const s32, "effect/effBattleSystem", nb_175338);

INCLUDE_ASM(const s32, "effect/effBattleSystem", nb_175440);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533A60);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533A70);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533A80);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533A90);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533AA0);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533AB0);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533AC0);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533AD0);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533AE0);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533AF0);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533B00);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533B10);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533B20);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533B30);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533B40);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533B50);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533B60);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533B70);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533B80);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533B90);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533BA0);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533BB0);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533BC0);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533BD0);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533BE0);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533BF0);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533C00);

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533C10);

typedef struct
{
    char *label;
    u8 type;
    u8 pad5[3];
    void *value;
    void *value2;
    s32 unk16;
    s32 unk20;
} effDebugMenuItem_t;

extern effDebugMenuItem_t D_0031CF88[5];
extern char D_0056A920[0x20];
extern char D_0056A940[0x20];
extern f32 effBTLModelGroupGetSphereOld(u32 group);
extern f64 sdfFloatToULong(f32 value);
EQUATE_FPTODP();

void effBTLDebugPrintDistance(void)
{
    sprintf(D_0056A920, "PLAYER:%-4.3f", effBTLModelGroupGetSphereOld(0x100) * 0.02f);
    sprintf(D_0056A940, " ENEMY:%-4.3f", effBTLModelGroupGetSphereOld(0x200) * 0.02f);
    D_0031CF88[3].label = D_0056A920;
    D_0031CF88[4].label = D_0056A940;
}

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

extern s8 dds3PadBitTable[];
extern void *dblurUpdateHandle(void *viewer);

void eff_1757D8(void)
{
    nbMainProcessData_t *work;

    if (dds3DebugSign != 0) {
        if (dds3PadBitTable[14] < 0) {
            D_0054F1DF ^= 1;
            work = nbGetMainProcessData();
            if (D_0054F1DF != 0) {
                work->stat |= 0x200000;
            } else {
                work->stat &= ~0x200000;
            }
        }
        if (D_0054F1DF != 0) {
            nbHelperViewer = dblurUpdateHandle(nbHelperViewer);
        }
        func_001750D0(0);
    }
}

extern u8 D_0054F1BE;
extern u8 D_0054F1BF;
extern u8 D_0054F1C0;
extern s8 dds3PadButtonState[];
extern void effBTLCutDebugPrint(void);
extern void effDebugAllocDrawTag(void);
extern void effDebugDrawMenu(void *menu);
extern void effDebugLinkDrawTag(void);
extern void nbCameraSetTarget(nbFormation_t *target);

void dds3_175878(void)
{
    nbMainProcessData_t *mainData;

    if (dds3DebugSign == 0) {
        return;
    }
    func_00174860();
    effBTLCutDebugPrint();
    if (D_0054F1BF != 0) {
        func_00174AB8(0x10604020, 0x10204060);
    }
    if (D_0054F1BE != 0) {
        func_00174BB0(0x80303030);
    }
    if (D_0054F1C0 != 0) {
        func_00174C50(0x10206040);
    }
    if (D_0054F1DF != 0) {
        effDebugAllocDrawTag();
        effDebugDrawMenu(nbHelperViewer);
        effDebugLinkDrawTag();
    }

    mainData = nbGetMainProcessData();
    if ((mainData->stat & 0x400000) == 0) {
        return;
    }
    if (dds3PadButtonState[0x2B] >= 0) {
        return;
    }
    if ((mainData->form[4].stat & 3) != 3) {
        return;
    }
    nbCameraSetTarget(&mainData->form[4]);
    /* Retail keeps jal plus the epilogue. Qualified ee-gcc traces (ordinary
     * and probed output identical) retain this own-pointer reset after call
     * insn 216 in cfg, then delete reset 220 in life. It therefore blocks
     * sibling-call formation before disappearing; see CONTRIBUTING.md. */
    mainData = NULL;
}

void InitModelCollisionEditorProcess(void)
{
    dds3AdminiCallSequence(0, 0, 0, 0);
}

INCLUDE_RODATA(const s32, "effect/effBattleSystem", D_00533C40);

