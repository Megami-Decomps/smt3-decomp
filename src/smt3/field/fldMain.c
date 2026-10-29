#include "common.h"
#include "basic.h"
#include "dds3obj.h"
#include "model.h"
#include "kernel.h"
#include "libsdf.h"
#include "ps2asm.h"
#include "newdata.h"
#include "field.h"

extern dds3Basic_t *dds3GetControlScene(void);

extern void dds3GetPosPoseSet(dds3Basic_t *pBasic);

extern void dds3GetRotPoseSet(dds3Basic_t *pBasic);

extern void fldRunDanteShutdown(void);

typedef struct {
    s32 use;
    s32 cnt;
    u8 unk08[0x8];
    dds3Basic_t *pBasic;
} fldWave_tag;

extern fldWave_tag fldWaveBuff[];

extern void *mhfldWaveFile;

extern void *mhfldPlayerFile;

extern void *mhfldAnaFile;

extern s32 gFldRndEncBtl;

extern void dds3ObjectBaseOnFlag(dds3Basic_t *basic, s32 flag);

extern void fldPlayerAFree(void);

extern void fldLoadNextFile_SetId_BtlRet(s32 field, s32 area);

extern s32 gFldAnahoriAkumaIdx;

extern s32 gFldAnahoriAkumaIdxMax;

extern s32 evtBitCheck(s32 no);



extern fldDanteGunState_t fldRunDanteGunFlag;

extern u8 fldDoorExitFlag;

extern s8 fldDoorExitMode[];

extern char pointResName[];

extern char D_00531690[], D_00531950[];

extern void fclClearTerminalState(void);


extern s32 fldWmapIsEnabled(void);

extern s32 dds3ConfigChanged(void);

extern void dds3ConfigSaveBackup(void);

extern void dds3ConfigRestoreBackup(void);



extern mdlObj_t *dds3ObjectBaseGetModelHandle(dds3Basic_t *pObjectBasic);

extern void dds3ObjectBaseOffFlag(dds3Basic_t *pObjectBasic, u32 flag);

extern void mdlSingleMotion(void *aHandle, s32 aGroup, s32 aNumber);

extern void dds3GlobalWorkUserInit2(s32 mode);

extern dds3Basic_t *fldAnaBasic;

extern char D_00531950[];

extern void datDelDevil(s32 id);

extern u32 dds3AllocObjectId(void);

extern dds3Basic_t *dds3AddUnitObjectBasic(s32 serialID, void *aPos, void *aRot);

extern void dds3SetUnitObjectId(dds3Basic_t *basic, const char *name);

extern void dds3ObjectBaseMakeUnit(dds3Basic_t *pObjectBasic, s32 major, s32 minor);

extern char D_00531820[];

extern char D_005316B0[];

extern char D_005316D0[], D_005316F0[], D_00531710[], D_00531728[], D_00531740[], D_00531758[];

extern s32 gfldPlayerSize;

extern void *gfldAnaPtr;

extern s32 gfldAnaSize;

extern void *gfldWavePtr;

extern s32 gfldWaveSize;

extern void mdlPackDecode(s32 major, s32 minor, s32 regFlags, void *buff, s32 leng);

extern void *sdfMemAllocDirect(s32 aSize);

extern void *sdfMemGetPtr(sdfMemHandle_t *aHandle);

extern void *gfldPlayerPtr;

extern s32 gfldPlayerSizeBk;

extern s32 slGetExitType(void);

extern void dds3AdminiCallSequence(s32 sequence, void *data, s32 datasize, s32 restore);

extern void fldGbInit_System(s32 mode);

extern void evtBitOn(s32 no);

extern void fldSetNextField(s32 field, s32 parameter);

void fldSetSceneParam2(fldSceneParam_t *pParam, s32 loadField, s32 loadArea, const char *pointResName);

extern dds3ProcessID_t *dds3SearchProcessName(char *namestr);

extern fldSceneParam_t fldSceneParam;

extern u8 D_002FC480[];

extern s32 gFldNowDZone;

extern s32 fldEncountCheckCount __attribute__((section(".data")));

extern s32 fldJDataHandle __attribute__((section(".data")));

extern void fldEsutoMaClear(void);

extern void fldRiberaMaClear(void);

extern void fldRifutoMaClear(void);

extern void fldLightMaClear(void);

extern s32 fldLightMaIdxCalc(void);

extern void fldSePlay(s32 seid);

extern void fldSkySetTblLightMa(s32 idx, s32 hokan);

extern void fldDrawInit(void);

extern void fldInitAllValue(void);

extern void fldSe_Init(void);

extern void sdfRandomize(void *aCB, u32 aSeed);

extern void fldInitPacLoader(void);

extern void fldSkyDefaultLoad(void);

extern void fldAutoMapLoad(void);

extern void fldAllTblLoad(void);

extern void dds3ObjectBaseSetPrimaryControl(dds3Basic_t *basic, s32 (*func)(dds3PoseSet_t *, dds3Basic_t *));

extern void fldEncountClearFlag(u32 flag);

extern char D_0054EB40[];

extern s32 fldUnitPad(dds3PoseSet_t *pUpdatePoseSet, dds3Basic_t *pBasic);

extern void fldPlayerCalc_Hit(void);

extern void fldHitChkUnit(sdfVector4_t *phys_pos);

extern sdfVector4_t D_00531830;

extern sdfVector4_t D_00531840;

extern dds3Basic_t *dds3AddCameraObjectBasicTypeLookAt(u32 id, sdfVector4_t *campos, sdfVector4_t *tm, sdfVector4_t *lookat);

extern void dds3CameraObjectBasicGetCameraTM(dds3Basic_t *basic, sdfVector4_t *tm);

extern void dds3GetRadiusPoseSet(dds3Basic_t *basic, f32 radius);

extern void dds3SetActiveCameraSceneBasic(dds3Basic_t *pSceneBasic, dds3Basic_t *pCameraBasic);

extern void fldStatusSet(void);

extern s32 fldChkVortex(void);

extern void fldEncountSetFlag(u32 flag);

extern void fldReleaseUnitPad(void);

extern sdfGrOt_t *D_002F6A18[4];

extern void sdfModelOtInit(void *aModelOt, sdfGrOt_t *aOt0, sdfGrOt_t *aOt1,
                           sdfGrOt_t *aOt2, sdfGrOt_t *aOt3);




extern sdfGrOt_t sdfGrOtTable[];

extern fldUnitControl_t fldUnitControl;

extern fldFollowCameraControl_t fldFollowCameraControl;

extern fldUnitControl_t *fldGetUnitControl(void);

extern fldFollowCameraControl_t *fldGetFollowCameraControl(void);

extern s32 gFldAnahoriAkumaIdx;

extern s32 gFldAnahoriAkumaIdxMax;

extern sdfMemHandle_t *fldPlayerAnimData;

extern s32 D_002FC3D8 __attribute__((section(".data")));

extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern void sdfMemFreeDelayedDirect(sdfMemHandle_t *aHandle);

extern void fldEveHit_End(void);

extern void fldShutoff(void);

extern void evtFreeStage(void);

extern void fldProcShutDown(void);

void KillOoyTest(void);

void fldDbgWmapStart(void)
{
    s32 field;

    if (DDS3_GBWK->transition != 0) {
        dds3GlobalWorkUserInit2(1);
    } else {
        dds3GlobalWorkUserInit2(0);
        if (fldWmapIsEnabled() == 0) {
            evtBitOn(0xAF2);
        }
    }
    if (dds3ConfigChanged() == 0) {
        dds3ConfigSaveBackup();
    } else {
        dds3ConfigRestoreBackup();
    }
    evtBitOn(0xC0F);
    fldRunDanteGunFlag.enabled = 1;
    field = 0x259;
    fldRunDanteGunFlag.counter = 0;
    fldSetNextField(0xB, 0x259);
    dds3AdminiCallSequence(7, &field, 4, 0);
}

void fldMainLoadStart(void) {
    fldSceneParam_t record;
    u32 mode;
    u32 *modep = &mode;

    if (slGetExitType() == 1) {
        return;
    }
    if (DDS3_GBWK->transition != 0) {
        mode = 1;
        dds3AdminiCallSequence(0x29, modep, 4, 0);
        return;
    }
    fldGbInit_System(1);
    fldRunDanteGunFlag.counter = 0;
    fldRunDanteGunFlag.enabled = 1;
    evtBitOn(0xC0F);
    fldSetNextField(0, 0);
    fldSetSceneParam2(&record, 1, 1, pointResName);
    record.loadMode = 1;
    dds3AdminiCallSequence(6, &record, 0xA0, 0);
}

void fldFirstInit(void)
{
    fldDrawInit();
    fldInitAllValue();
    fldSe_Init();
    sdfRandomize(D_002FC480, 0x1E240);
    fldInitPacLoader();
    fldSkyDefaultLoad();
    fldAutoMapLoad();
    fldAllTblLoad();
}

#ifdef NON_MATCHING
extern void *memset(void *a, s32 b, u32 c);
#endif

void fldInitAllValue(void)
{
    u8 *p = (u8 *)&fldSceneParam;

    SDF_VU0_STOREVECTOR_ZERO(p);
    SDF_VU0_STOREVECTOR_ZERO(p + 0x10);
    memset(fldGetUnitControl(), 0, 0xC);
    memset(fldGetFollowCameraControl(), 0, 0x7C);
}

#ifdef NON_MATCHING
extern char * strcpy(char *, const char *);
#endif

void fldSetSceneParam2(fldSceneParam_t *pParam, s32 loadField, s32 loadArea, const char *pointResName) {
    if (dds3SearchProcessName(D_00531690) != NULL) {
        if (fldGlobalBase.area == loadField) {
            fldGlobalBase.reuseAreaResources = 1;
        } else {
            fclClearTerminalState();
        }
    }
    pParam->flag = 0;
    pParam->pointResrcID = 0;
    pParam->sequenceWord38 = 0;
    strcpy(pParam->pointResName, pointResName);
    pParam->loadField = loadField;
    pParam->loadScript = 1;
    pParam->callmode = 2;
    pParam->loadArea = loadArea;
    pParam->bgm = 0;
    pParam->bgmMode = 0;
    pParam->camMode = 0;
    memset(pParam->camName, 0, sizeof(pParam->camName));
    memset(pParam->FoceScript, 0, sizeof(pParam->FoceScript));
    pParam->loadMode = 0;
    fldDoorExitFlag = 0;
    fldDoorExitMode[0] = 0;
}

void fldSetSceneParam3(fldSceneParam_t *pParam, s32 loadField, s32 loadArea, const char *pointResName) {
    if (dds3SearchProcessName(D_00531690) != NULL) {
        if (fldGlobalBase.area == loadField) {
            fldGlobalBase.reuseAreaResources = 1;
        } else {
            fclClearTerminalState();
        }
    }
    pParam->flag = 0;
    pParam->pointResrcID = 0;
    pParam->sequenceWord38 = 0;
    strcpy(pParam->pointResName, pointResName);
    pParam->loadField = loadField;
    pParam->loadScript = 1;
    pParam->callmode = 3;
    pParam->loadArea = loadArea;
    pParam->bgm = 0;
    pParam->bgmMode = 0;
    pParam->camMode = 0;
    memset(pParam->camName, 0, sizeof(pParam->camName));
    memset(pParam->FoceScript, 0, sizeof(pParam->FoceScript));
    pParam->loadMode = 0;
}

void fldSetSceneParam4(fldSceneParam_t *pParam, s32 loadField, s32 loadArea, const char *pointResName,
                    s32 bgm, s32 camMode, const char *camName) {
    if (dds3SearchProcessName(D_00531690) != NULL) {
        if (fldGlobalBase.area == loadField) {
            fldGlobalBase.reuseAreaResources = 1;
        } else {
            fclClearTerminalState();
        }
    }
    pParam->flag = 0;
    pParam->pointResrcID = 0;
    pParam->sequenceWord38 = 0;
    strcpy(pParam->pointResName, pointResName);
    pParam->loadField = loadField;
    pParam->loadScript = 1;
    pParam->loadArea = loadArea;
    pParam->bgm = bgm;
    pParam->camMode = camMode;
    pParam->callmode = 2;
    pParam->bgmMode = 0;
    strcpy(pParam->camName, camName);
    memset(pParam->FoceScript, 0, sizeof(pParam->FoceScript));
    pParam->loadMode = 0;
}

void fldSetSceneParam5(fldSceneParam_t *pParam, s32 loadField, s32 loadArea, const char *pointResName,
                    s32 bgm, const char *FoceScript) {
    if (dds3SearchProcessName(D_00531690) != NULL) {
        if (fldGlobalBase.area == loadField) {
            fldGlobalBase.reuseAreaResources = 1;
        } else {
            fclClearTerminalState();
        }
    }
    pParam->flag = 0;
    pParam->pointResrcID = 0;
    pParam->sequenceWord38 = 0;
    strcpy(pParam->pointResName, pointResName);
    pParam->loadField = loadField;
    pParam->loadScript = 1;
    pParam->callmode = 2;
    pParam->loadArea = loadArea;
    pParam->bgm = bgm;
    pParam->bgmMode = 0;
    pParam->camMode = 0;
    memset(pParam->camName, 0, sizeof(pParam->camName));
    strcpy(pParam->FoceScript, FoceScript);
    pParam->loadMode = 0;
}

void fldSetSceneParamLoad(fldSceneParam_t *pParam, s32 loadField, s32 loadArea, const char *pointResName,
                    s32 bgm, s32 camMode, const char *camName) {
    pParam->flag = 0;
    pParam->pointResrcID = 0;
    pParam->sequenceWord38 = 0;
    strcpy(pParam->pointResName, pointResName);
    pParam->loadScript = 1;
    pParam->loadField = loadField;
    pParam->loadArea = loadArea;
    pParam->bgm = bgm;
    pParam->camMode = camMode;
    pParam->callmode = 2;
    pParam->bgmMode = 0;
    strcpy(pParam->camName, camName);
    memset(pParam->FoceScript, 0, sizeof(pParam->FoceScript));
    pParam->loadMode = 0;
}

s32 fldGetRestoreMode(void)
{
    if (fldSceneParam.mode == 1) {
        return 1;
    }
    if (fldSceneParam.callmode == 3) {
        return 3;
    }
    if (fldSceneParam.callmode == 4) {
        return 4;
    }
    if (fldSceneParam.callmode == 5) {
        return 5;
    }
    if (fldSceneParam.callmode == 6) {
        return 2;
    }
    return fldSceneParam.mode != 0 ? 2 : 0;
}

#ifdef NON_MATCHING
typedef struct {
    s32 eventFlag;
    s16 normal[32];
    s16 alternate[32];
} fldEncounterFloorTable_t;

typedef struct __attribute__((packed, aligned(1))) {
    u64 part[2];
} fldPackedVector_t;

extern fldEncounterFloorTable_t D_003D51E0[];
extern const fldPackedVector_t D_005316A0;
extern dds3Basic_t *dds3SceneBasicSearchNameAndType(dds3Basic_t *scene, s32 type, const char *name);
extern fldResourcePoint_t *dds3PointBasicGetResource(dds3Basic_t *basic);
extern void dds3SetActiveScene(dds3Basic_t *scene);
extern s32 evtLoadArea(s32 field, s32 area);
extern void fldWap_Load(s32 terminalType, s32 terminalId, fldSceneParam_t *param);
extern void fldSetUnit(fldResourcePoint_t *point);
extern void fldRegistUnitPad(void);
extern void fldSetUnitFollowCamera(void);
extern void fldTitleMiniStart(s32 field);
extern void nbBgmFadeOut(void);
extern void resetCamNormal(void);
extern void resetCamNormal2(void);
extern void resetCamTgtPlayer(void);
extern void fldCamSetChase(void);
extern void FromRotation(f32 angle);
extern void Mult(void);
extern s32 gfldPlayerDamegeCnt;

void fldReadyScene(void)
{
    QWORD_ALIGNED fldResourcePoint_t defaultPoint;
    QWORD_ALIGNED fldResourcePoint_t adjustedPoint;
    QWORD_ALIGNED union {
        sdfVector4_t vector;
        fldPackedVector_t packed;
    } rotation;
    fldResourcePoint_t *point = NULL;
    fldEncounterFloorTable_t *encounters;
    dds3Basic_t *basic;
    s32 reused;
    s32 previous;
    s32 area;
    s32 showTitle;
    s32 eventSet;
    s32 cameraMode;

    memset(&defaultPoint, 0, sizeof(defaultPoint));
    defaultPoint.pos.w = 1.0f;
    defaultPoint.rot.w = 1.0f;
    memset(&adjustedPoint, 0, sizeof(adjustedPoint));
    adjustedPoint.rot.w = 1.0f;
    adjustedPoint.pos.w = 1.0f;
    rotation.packed = D_005316A0;
    if (fldSceneParam.loadMode == 1)
        fldWap_Load(DDS3_GBWK->previewX, DDS3_GBWK->previewY, &fldSceneParam);
    reused = fldGlobalBase.reuseAreaResources;
    evtLoadArea(fldSceneParam.loadField, fldSceneParam.loadArea);
    dds3SetActiveScene(NULL);
    if (fldSceneParam.mode == 0) {
        basic = dds3SceneBasicSearchNameAndType(dds3GetControlScene(), 0x11, fldSceneParam.pointResName);
        point = basic != NULL ? dds3PointBasicGetResource(basic) : &defaultPoint;
        if (fldSceneParam.callmode == 3) {
            adjustedPoint.pos.x = point->pos.x;
            adjustedPoint.pos.y = point->pos.y;
            adjustedPoint.pos.z = point->pos.z;
            SDF_VU0_LOADVECTOR(&rotation.vector);
            FromRotation(3.141592503f);
            SDF_VU0_LOADVECTOR_B(&fldSceneParam.safeUnit.rot);
            Mult();
            SDF_VU0_STOREVECTOR(&fldSceneParam.safeUnit.rot);
            adjustedPoint.rot.x = fldSceneParam.safeUnit.rot.x;
            adjustedPoint.rot.y = fldSceneParam.safeUnit.rot.y;
            adjustedPoint.rot.z = fldSceneParam.safeUnit.rot.z;
            adjustedPoint.rot.w = fldSceneParam.safeUnit.rot.w;
            point = &adjustedPoint;
        }
        if (fldSceneParam.loadField < 46) {
            encounters = &D_003D51E0[fldSceneParam.loadField];
            eventSet = 0;
            if (encounters->eventFlag != 0) eventSet = evtBitCheck(encounters->eventFlag) != 0;
            if (eventSet == 0)
                fldGlobalBase.wordBC = D_003D51E0[fldSceneParam.loadField].normal[fldSceneParam.loadArea];
            else
                fldGlobalBase.wordBC = D_003D51E0[fldSceneParam.loadField].alternate[fldSceneParam.loadArea];
        }
        if (((u32)fldGlobalBase.area - 11) < 35 && fldGlobalBase.area != 39 &&
            fldGetRestoreMode() != 4 && reused == 0) {
            if (fldGetRestoreMode() == 5) {
                if (fldGlobalBase.word10 != fldGlobalBase.area) fldTitleMiniStart(fldGlobalBase.area);
            } else {
                if (fldGetRestoreMode() == 2) {
                    previous = fldGlobalBase.word10;
                    showTitle = 0;
                    switch (previous) {
                    case 40:
                        area = fldGlobalBase.area;
                        showTitle = ((u32)area - 41) < 5;
                        break;
                    case 41:
                        area = fldGlobalBase.area;
                        showTitle = area == 40 || area == 42;
                        break;
                    case 42:
                        area = fldGlobalBase.area;
                        showTitle = area == 40 || area == 41 || area == 43;
                        break;
                    case 43:
                        area = fldGlobalBase.area;
                        showTitle = area == 40 || area == 42 || area == 44;
                        break;
                    case 44:
                        area = fldGlobalBase.area;
                        showTitle = area == 40 || area == 43 || area == 45;
                        break;
                    case 45:
                        area = fldGlobalBase.area;
                        showTitle = area == 40 || area == 44;
                        break;
                    }
                    if (showTitle) fldTitleMiniStart(fldGlobalBase.area);
                }
                if (fldGetRestoreMode() != 2) {
                    previous = fldGlobalBase.word10;
                    area = fldGlobalBase.area;
                    if (!((previous == 32 && (area == 36 || area == 37)) ||
                          (previous == 36 && (area == 32 || area == 37)) ||
                          (previous == 37 && (area == 32 || area == 36))))
                        fldTitleMiniStart(area);
                }
            }
        }
    } else if (fldSceneParam.mode == 1) {
        point = &fldSceneParam.safeUnit;
        SDF_VU0_LOADVECTOR(&rotation.vector);
        FromRotation(3.141592503f);
        SDF_VU0_LOADVECTOR_B(&fldSceneParam.safeUnit.rot);
        Mult();
        SDF_VU0_STOREVECTOR(&fldSceneParam.safeUnit.rot);
        nbBgmFadeOut();
    }
    fldSetUnit(point);
    fldRegistUnitPad();
    fldSetUnitFollowCamera();
    if (fldSceneParam.mode == 0) {
        cameraMode = fldSceneParam.camMode;
        if (cameraMode == 1) {
            basic = dds3SceneBasicSearchNameAndType(dds3GetControlScene(), 4, fldSceneParam.camName);
            if (basic != NULL) {
                fldGlobalBase.mode = cameraMode;
                fldGlobalBase.campos.x = basic->poseset->pos.x;
                fldGlobalBase.campos.y = basic->poseset->pos.y;
                fldGlobalBase.campos.z = basic->poseset->pos.z;
                resetCamTgtPlayer();
            } else {
                fldGlobalBase.mode = 0;
                if (fldGlobalBase.area < 10) fldCamSetChase();
                else resetCamNormal();
            }
        } else {
            fldGlobalBase.mode = cameraMode == 2 ? 3 : 0;
            if (fldSceneParam.callmode == 3) resetCamNormal2();
            else if (fldGlobalBase.area < 10) fldCamSetChase();
            else resetCamNormal();
        }
    }
    gfldPlayerDamegeCnt = 0;
}
#else
INCLUDE_ASM(const s32, "field/fldMain", fldReadyScene);
#endif

void fldPlayerALoad(void)
{
    if (fldPlayerAnimData == NULL) {
        fldPlayerAnimData = sdfDevLoadFileEx(D_005316B0, (u32 *)&gfldPlayerPtr, &gfldPlayerSizeBk);
    }
}

void fldPlayerAFree(void)
{
    if (fldPlayerAnimData != NULL) {
        sdfMemFreeDirect(fldPlayerAnimData);
        fldPlayerAnimData = NULL;
    }
}

#ifdef NON_MATCHING
extern void *memcpy(void *a, const void *b, u32 c);
#endif

void fldJDataLoad(void)
{
    void *src;
    void *dst;

    if (fldGlobalBase.area == 11 || fldGlobalBase.area == 14) {
        if (evtBitCheck(0x8B0) != 0) {
            mhfldPlayerFile = sdfDevLoadFileEx(D_005316D0, (u32 *)&gfldPlayerPtr, &gfldPlayerSize);
        } else {
            mhfldPlayerFile = sdfDevLoadFileEx(D_005316F0, (u32 *)&gfldPlayerPtr, &gfldPlayerSize);
        }
        mdlPackDecode(2, 0, 0x101, gfldPlayerPtr, gfldPlayerSize);
    } else if (fldGlobalBase.area < 10) {
        mhfldPlayerFile = sdfDevLoadFileEx(D_00531710, (u32 *)&gfldPlayerPtr, &gfldPlayerSize);
        mdlPackDecode(2, 4, 0x101, gfldPlayerPtr, gfldPlayerSize);
    } else {
        fldPlayerALoad();
        gfldPlayerSize = gfldPlayerSizeBk;
        mhfldPlayerFile = sdfMemAllocDirect(gfldPlayerSize);
        src = sdfMemGetPtr(fldPlayerAnimData);
        dst = sdfMemGetPtr(mhfldPlayerFile);
        memcpy(dst, src, gfldPlayerSize);
        gfldPlayerPtr = dst;
        mdlPackDecode(2, 0, 0x101, dst, gfldPlayerSize);
        sdfMemFreeDelayedDirect(mhfldPlayerFile);
        mhfldPlayerFile = NULL;
    }
    if (fldGlobalBase.area == 21 || fldGlobalBase.area == 26 || fldGlobalBase.area >= 36) {
        mhfldAnaFile = sdfDevLoadFileEx(D_00531728, (u32 *)&gfldAnaPtr, &gfldAnaSize);
        mdlPackDecode(2, 5, 0x101, gfldAnaPtr, gfldAnaSize);
    }
    if (fldGlobalBase.area == 22) {
        mhfldWaveFile = sdfDevLoadFileEx(D_00531740, (u32 *)&gfldWavePtr, &gfldWaveSize);
        mdlPackDecode(2, 0x1B, 0x101, gfldWavePtr, gfldWaveSize);
    } else if (fldGlobalBase.area == 45) {
        mhfldWaveFile = sdfDevLoadFileEx(D_00531758, (u32 *)&gfldWavePtr, &gfldWaveSize);
        mdlPackDecode(2, 0x1B, 0x101, gfldWavePtr, gfldWaveSize);
    }
}

void fldJDataClear(void)
{
    if (mhfldPlayerFile != NULL) {
        sdfMemFreeDelayedDirect((sdfMemHandle_t *)mhfldPlayerFile);
        mhfldPlayerFile = NULL;
    }
    if (mhfldAnaFile != NULL) {
        sdfMemFreeDelayedDirect((sdfMemHandle_t *)mhfldAnaFile);
        mhfldAnaFile = NULL;
    }
    if (mhfldWaveFile != NULL) {
        sdfMemFreeDelayedDirect((sdfMemHandle_t *)mhfldWaveFile);
        mhfldWaveFile = NULL;
    }
    if (fldJDataHandle == 0 && gFldRndEncBtl == 0) {
        fldPlayerAFree();
        fldLoadNextFile_SetId_BtlRet(0, 0);
    }
}

void fldShutoff(void)
{
    dds3Basic_t *handle;
    u8 *p;

    if (fldGetUnitControl()->handle != NULL) {
        handle = fldGetUnitControl()->handle;
        if (dds3GetControlScene() != NULL) {
            dds3GetPosPoseSet(handle);
            p = (u8 *)&fldSceneParam;
            SDF_VU0_STOREVECTOR(p);
            dds3GetRotPoseSet(handle);
            SDF_VU0_STOREVECTOR(p + 0x10);
        }
        fldGetUnitControl()->handle = NULL;
        fldGetUnitControl()->handle2 = NULL;
        fldGetFollowCameraControl()->handle = NULL;
        fldJDataClear();
        fldRunDanteShutdown();
    }
}

void fldAllShutDown(void)
{
    fldEveHit_End();
    KillOoyTest();
    fldShutoff();
    evtFreeStage();
    fldProcShutDown();
    D_002FC3D8 = 0;
}

fldUnitControl_t *fldGetUnitControl(void)
{
    return &fldUnitControl;
}

fldFollowCameraControl_t *fldGetFollowCameraControl(void)
{
    return &fldFollowCameraControl;
}

dds3Basic_t *fldGetUnitFollowCamera(void)
{
    return fldGetFollowCameraControl()->handle;
}

void fldRegistUnitPad(void)
{
    sdfVector4_t pos;
    fldUnitControl_t *ctrl;

    ctrl = fldGetUnitControl();
    if (ctrl->handle != NULL) {
        fldEncountSetFlag(0x40);
        dds3ObjectBaseSetPrimaryControl(ctrl->handle, fldUnitPad);
    }
    fldGlobalBase.word5C = 4;
    if (fldGlobalBase.menuOpenFlag < 4) {
        fldGlobalBase.menuOpenFlag = 4;
    }
    if (fldGlobalBase.encountState < 5) {
        fldGlobalBase.encountState = 5;
    }
    fldStatusSet();
    fldGlobalBase.hitMatterIndex = -1;
    fldPlayerCalc_Hit();
    dds3GetPosPoseSet(ctrl->handle);
    SDF_VU0_STOREVECTOR_NOMEM(&pos);
    if (fldGlobalBase.area < 0xA) {
        pos.y = pos.y + 20.0f;
    } else {
        pos.y = pos.y + 10.0f;
    }
    fldHitChkUnit(&pos);
}

void fldReleaseUnitPad(void)
{
    fldUnitControl_t *ctl = fldGetUnitControl();

    if (ctl->handle != NULL) {
        fldEncountClearFlag(0x40);
        dds3ObjectBaseSetPrimaryControl(ctl->handle, 0);
    }
    fldEncountCheckCount = 4;
}

INCLUDE_RODATA(const s32, "field/fldMain", D_00531690);

INCLUDE_RODATA(const s32, "field/fldMain", D_005316A0);

INCLUDE_RODATA(const s32, "field/fldMain", D_005316B0);

INCLUDE_RODATA(const s32, "field/fldMain", D_005316D0);

INCLUDE_RODATA(const s32, "field/fldMain", D_005316F0);

INCLUDE_RODATA(const s32, "field/fldMain", D_00531710);

INCLUDE_RODATA(const s32, "field/fldMain", D_00531728);

INCLUDE_RODATA(const s32, "field/fldMain", D_00531740);

INCLUDE_RODATA(const s32, "field/fldMain", D_00531758);

INCLUDE_RODATA(const s32, "field/fldMain", D_00531770);

INCLUDE_RODATA(const s32, "field/fldMain", D_00531780);

INCLUDE_RODATA(const s32, "field/fldMain", D_00531790);

INCLUDE_RODATA(const s32, "field/fldMain", D_005317A0);

INCLUDE_RODATA(const s32, "field/fldMain", D_005317B0);

#ifdef NON_MATCHING
extern const fldPackedVector_t D_00531770, D_00531780, D_00531790, D_005317A0, D_005317B0;
extern s32 D_002FC41C;
extern s32 l_UseCollision;
extern s32 fldCamUnit[];
extern dds3Basic_t *D_0054EAE8, *D_0054EAF0, *fldPlayerUnit;
extern char D_0054EB38[];
extern void *mhfldAnaFile;
extern void evtBitOff(s32 no);
extern dds3Basic_t *dds3SetActiveUnitSceneBasic(dds3Basic_t *scene, dds3Basic_t *unit);
extern void dds3UnitObjectBasicSetOt(dds3Basic_t *unit, s32 index);
extern void dds3SetSclPoseSet(dds3Basic_t *unit, const sdfVector4_t *scale);
extern void mdlLoopMotion(mdlObj_t *model, s32 group, s32 number);
extern void sdfModelSetFaceDivParm(sdfModelRootNode_t *model, f32 soften, f32 bias);
extern void fldJDataLoad(void);
extern void fldRunDanteInit(void);
extern void fldRunDanteClear(void);
extern s32 p2sdStopBGM(s32 id);
extern void fldSkySetTbl(s32 index, s32 blend);
extern void fldSkySetTableAnime(s32 type);
extern void fldSkySetTblLightMaDZ(s32 index);
extern s32 fldLightMaChk(void);
extern void fldLightMaStart(void);
extern s32 fldPlayerFastSet(void);
extern void fldWaveInit(void);
extern dds3Basic_t *dds3AddPointBasic(u32 serialID, fldResourcePoint_t *data, char *name);

void fldSetUnit(fldResourcePoint_t *point)
{
    QWORD_ALIGNED sdfVector4_t zeroPosition;
    QWORD_ALIGNED sdfVector4_t rotation;
    QWORD_ALIGNED union { sdfVector4_t vector; fldPackedVector_t packed; } axis;
    QWORD_ALIGNED union { sdfVector4_t vector; fldPackedVector_t packed; } scale;
    QWORD_ALIGNED union { sdfVector4_t vector; fldPackedVector_t packed; } mirrorScale;
    QWORD_ALIGNED union { sdfVector4_t vector; fldPackedVector_t packed; } dantePosition;
    QWORD_ALIGNED union { sdfVector4_t vector; fldPackedVector_t packed; } danteScale;
    mdlObj_t *model;
    s32 area;
    s32 floor;
    s32 mirror;
    s32 oldDante;
    s32 enableDante;
    s32 lightZone;
    s32 lightCheck;

    memset(&zeroPosition, 0, sizeof(zeroPosition));
    zeroPosition.w = 1.0f;
    memset(&rotation, 0, sizeof(rotation));
    rotation.w = 1.0f;
    axis.packed = D_00531770;
    scale.packed = D_00531780;
    mirrorScale.packed = D_00531790;
    dantePosition.packed = D_005317A0;
    danteScale.packed = D_005317B0;
    SDF_VU0_LOADVECTOR(&axis.vector);
    FromRotation(3.141592503f);
    SDF_VU0_LOADVECTOR_B(&point->rot);
    Mult();
    SDF_VU0_STOREVECTOR(&rotation);
    fldUnitControl.handle = dds3AddUnitObjectBasic(dds3AllocObjectId(), point, &rotation);
    dds3SetUnitObjectId(fldUnitControl.handle, "PLAYER_UNIT");
    dds3SetActiveUnitSceneBasic(dds3GetControlScene(), fldUnitControl.handle);
    D_0054EAE8 = fldUnitControl.handle;
    if (l_UseCollision != 0) dds3ObjectBaseOffFlag(fldUnitControl.handle, 0x20);
    if (fldGlobalBase.area < 10) {
        fldJDataLoad();
        dds3ObjectBaseMakeUnit(fldUnitControl.handle, 2, 4);
        if (fldGlobalBase.area != 1) dds3ObjectBaseOnFlag(fldUnitControl.handle, 0x400);
        else dds3UnitObjectBasicSetOt(fldUnitControl.handle, 7);
    } else {
        fldJDataLoad();
        dds3ObjectBaseMakeUnit(fldUnitControl.handle, 2, D_002FC41C);
    }
    dds3SetSclPoseSet(fldUnitControl.handle, &scale.vector);
    model = dds3ObjectBaseGetModelHandle(fldUnitControl.handle);
    if (fldGlobalBase.area < 10) {
        dds3GetRadiusPoseSet(fldUnitControl.handle, 30.0f);
        mdlLoopMotion(model, 0, 0);
    } else {
        dds3GetRadiusPoseSet(fldUnitControl.handle, 90.0f);
        mdlLoopMotion(model, 0, 0);
        sdfModelSetFaceDivParm(model->inner, 15.0f, 0.0f);
    }
    fldPlayerUnit = NULL;
    area = fldGlobalBase.area;
    mirror = 0;
    if (area == 18 || area == 28 || area == 30) {
        mirror = 1;
    } else if (area == 33) {
        floor = fldGlobalBase.floor;
        mirror = floor == 0 || floor == 6 || floor == 23 || floor == 27;
    } else if (area == 17) {
        floor = fldGlobalBase.floor;
        mirror = floor == 1 || floor == 2 || floor == 3;
    } else if (area == 14) {
        mirror = fldGlobalBase.floor == 16;
    } else if (area == 15) {
        floor = fldGlobalBase.floor;
        mirror = floor == 16 || floor == 9;
    } else if (area == 34) {
        mirror = fldGlobalBase.floor == 0;
    }
    if (mirror) {
        SDF_VU0_LOADVECTOR(&axis.vector);
        FromRotation(3.141592503f);
        SDF_VU0_LOADVECTOR_B(&point->rot);
        Mult();
        SDF_VU0_STOREVECTOR(&rotation);
        fldUnitControl.handle2 = dds3AddUnitObjectBasic(dds3AllocObjectId(), point, &rotation);
        dds3SetUnitObjectId(fldUnitControl.handle2, "PLAYER_UNIT2");
        fldPlayerUnit = fldUnitControl.handle2;
        dds3ObjectBaseMakeUnit(fldUnitControl.handle2, 2, D_002FC41C);
        dds3SetSclPoseSet(fldUnitControl.handle2, &mirrorScale.vector);
        model = dds3ObjectBaseGetModelHandle(fldUnitControl.handle2);
        dds3GetRadiusPoseSet(fldUnitControl.handle, 90.0f);
        mdlLoopMotion(model, 0, 0);
        sdfModelSetFaceDivParm(model->inner, 15.0f, 0.0f);
        model->inner->flags |= 8;
        dds3ObjectBaseOnFlag(fldUnitControl.handle, 0x400);
    } else {
        dds3ObjectBaseOnFlag(fldUnitControl.handle, 0x200);
    }
    D_0054EAF0 = NULL;
    oldDante = evtBitCheck(0xAF3);
    evtBitOff(0xAF3);
    evtBitOff(0xAF4);
    if (fldGlobalBase.area == 43 && evtBitCheck(0x105) == 0) {
        enableDante = 0;
        switch (fldGlobalBase.floor) {
        case 3: enableDante = evtBitCheck(0x7B9) == 0; break;
        case 4: enableDante = evtBitCheck(0x7BA) == 0; break;
        case 5:
            if (evtBitCheck(0x7BA) == 0) {
                if (evtBitCheck(0x7B1) == 0) enableDante = 1;
                else enableDante = evtBitCheck(0x7B2) == 0;
            }
            break;
        case 7: case 8: case 9: case 10:
            enableDante = evtBitCheck(0x7BB) == 0;
            break;
        }
        if (enableDante) {
            evtBitOn(0xAF3);
            evtBitOn(0xAF4);
            fldCamUnit[0] = 24;
        }
    }
    if (evtBitCheck(0xAF3) != 0) {
        SDF_VU0_LOADVECTOR(&axis.vector);
        FromRotation(4.712388515f);
        SDF_VU0_LOADVECTOR_B(&point->rot);
        Mult();
        SDF_VU0_STOREVECTOR(&rotation);
        D_0054EAF0 = dds3AddUnitObjectBasic(dds3AllocObjectId(), point, &rotation);
        dds3SetUnitObjectId(D_0054EAF0, "DANTE_UNIT");
        dds3ObjectBaseMakeUnit(D_0054EAF0, 2, 7);
        dds3SetSclPoseSet(D_0054EAF0, &danteScale.vector);
        model = dds3ObjectBaseGetModelHandle(D_0054EAF0);
        dds3GetRadiusPoseSet(D_0054EAF0, 90.0f);
        mdlLoopMotion(model, 0, 3);
        sdfModelSetFaceDivParm(model->inner, 15.0f, 0.0f);
        dds3ObjectBaseOffFlag(D_0054EAF0, 1);
        dds3SetPosPoseSet(D_0054EAF0, &dantePosition.vector);
        fldRunDanteInit();
    } else {
        fldRunDanteClear();
        if (oldDante != 0) p2sdStopBGM(0x820000);
    }
    if (fldGlobalBase.area < 200) {
        fldSkySetTblLightMa(0, 0);
        fldSkySetTbl(fldRunDanteGunFlag.skyTable, 0);
        fldSkySetTableAnime(fldRunDanteGunFlag.word30);
        lightCheck = fldLightMaChk();
        lightZone = gFldNowDZone;
        if (lightCheck == 1) {
            if (gFldNowDZone == 0) {
                fldGlobalBase.lightMaIdx = 0;
                fldSkySetTblLightMa(0, 0);
                fldSkySetTbl(fldRunDanteGunFlag.skyTable, 0);
                fldSkySetTableAnime(fldRunDanteGunFlag.word30);
            } else {
                fldLightMaStart();
            }
            lightZone = gFldNowDZone;
        } else if (lightZone == 0) {
            fldGlobalBase.lightMaIdx = 0;
        }
        fldSkySetTblLightMaDZ(lightZone);
    }
    fldPlayerFastSet();
    if (mhfldAnaFile != NULL) {
        zeroPosition.x = 0.0f;
        rotation.w = 1.0f;
        zeroPosition.y = 0.0f;
        zeroPosition.z = 0.0f;
        zeroPosition.w = 1.0f;
        rotation.x = 0.0f;
        rotation.y = 0.0f;
        rotation.z = 0.0f;
        fldAnaBasic = dds3AddUnitObjectBasic(dds3AllocObjectId(), &zeroPosition, &rotation);
        dds3ObjectBaseMakeUnit(fldAnaBasic, 2, 5);
        dds3ObjectBaseOnFlag(fldAnaBasic, 1);
        dds3ObjectBaseOnFlag(fldAnaBasic, 0x100);
        dds3SetUnitObjectId(fldAnaBasic, "ANA_UNIT");
    }
    if (fldGlobalBase.area >= 11) fldWaveInit();
    dds3AddPointBasic(dds3AllocObjectId(), &fldPlayerPosRotWork, D_0054EB38);
}
#else
INCLUDE_ASM(const s32, "field/fldMain", fldSetUnit);
#endif

void fldAnaStart(f32 x, f32 y, f32 z)
{
    f32 pos[4];

    memset(pos, 0, sizeof(pos));
    pos[3] = 1.0f;
    if (mhfldAnaFile != NULL) {
        pos[0] = x;
        pos[1] = y;
        pos[2] = z;
        dds3SetPosPoseSet(fldAnaBasic, pos);
        mdlSingleMotion(dds3ObjectBaseGetModelHandle(fldAnaBasic), 0, 0);
        dds3ObjectBaseOffFlag(fldAnaBasic, 1);
        fldSePlay(0x26);
    }
}

void fldWaveInit(void)
{
    f32 pos[4];
    f32 rot[4];
    s32 i;

    memset(pos, 0, sizeof(pos));
    pos[3] = 1.0f;
    memset(rot, 0, sizeof(rot));
    rot[3] = 1.0f;
    if (mhfldWaveFile == NULL) {
        return;
    }
    for (i = 0; i < 0x28; i++) {
        fldWaveBuff[i].use = 0;
        fldWaveBuff[i].cnt = 0;
        fldWaveBuff[i].pBasic = NULL;
        fldWaveBuff[i].pBasic = dds3AddUnitObjectBasic(dds3AllocObjectId(), pos, rot);
        dds3SetUnitObjectId(fldWaveBuff[i].pBasic, D_00531820);
        dds3ObjectBaseMakeUnit(fldWaveBuff[i].pBasic, 2, 0x1B);
        dds3ObjectBaseOnFlag(fldWaveBuff[i].pBasic, 1);
    }
}

void fldWaveStart(f32 x, f32 y, f32 z)
{
    f32 pos[4];
    s32 i;

    memset(pos, 0, sizeof(pos));
    pos[3] = 1.0f;
    if (mhfldWaveFile == NULL) {
        return;
    }
    for (i = 0; i < 0x28; i++) {
        if (fldWaveBuff[i].use != 0) {
            continue;
        }
        fldWaveBuff[i].use = 1;
        fldWaveBuff[i].cnt = 0;
        pos[0] = x;
        pos[1] = y;
        pos[2] = z;
        dds3SetPosPoseSet(fldWaveBuff[i].pBasic, pos);
        mdlSingleMotion(dds3ObjectBaseGetModelHandle(fldWaveBuff[i].pBasic), 0, 0);
        dds3ObjectBaseOffFlag(fldWaveBuff[i].pBasic, 1);
        mdlSingleMotion(dds3ObjectBaseGetModelHandle(fldWaveBuff[i].pBasic), 0, 0);
        return;
    }
}

void fldWaveTask(void)
{
    fldWave_tag *p;
    s32 i;

    if (mhfldWaveFile == NULL) {
        return;
    }
    p = fldWaveBuff;
    for (i = 0x27; i >= 0; i--, p++) {
        if (p->use == 0) {
            continue;
        }
        if (++p->cnt >= 0x47) {
            dds3ObjectBaseOnFlag(p->pBasic, 1);
            p->use = 0;
        }
    }
}

void fldSetUnitFollowCamera(void)
{
    sdfVector4_t lookat;
    sdfVector4_t campos;
    sdfVector4_t tm;
    fldFollowCameraControl_t *ctrl;

    lookat = D_00531830;
    memset(&campos, 0, sizeof(campos));
    campos.w = 1.0f;
    tm = D_00531840;
    ctrl = &fldFollowCameraControl;
    ctrl->handle =
        dds3AddCameraObjectBasicTypeLookAt(dds3AllocObjectId(), &campos, &tm, &lookat);
    dds3SetUnitObjectId(ctrl->handle, D_0054EB40);
    dds3CameraObjectBasicGetCameraTM(ctrl->handle, &tm);
    dds3GetRadiusPoseSet(ctrl->handle, 2.0f);
    dds3SetActiveCameraSceneBasic(dds3GetControlScene(), ctrl->handle);
}

s32 fldUnitPad(dds3PoseSet_t *pUpdatePoseSet, dds3Basic_t *pBasic)
{
    return 0;
}

INCLUDE_RODATA(const s32, "field/fldMain", D_00531820);

INCLUDE_RODATA(const s32, "field/fldMain", D_00531830);

INCLUDE_RODATA(const s32, "field/fldMain", D_00531840);

s32 fldChkVortex(void)
{
    s32 list[3] = { 1, 11, 14 };
    u32 i;

    for (i = 0; i < 3; i++) {
        if (fldGlobalBase.area == list[i]) {
            return 0;
        }
    }
    if ((u32)(fldGlobalBase.area - 0x259) < 0x10) {
        return 0;
    }
    return 1;
}

s32 fldLightMaIdxCalc(void)
{
    s32 idx = 0x64;

    if (fldGlobalBase.area == 0x15) {
        idx = 0x66;
        if (fldGlobalBase.floor == 0x02) {
            idx = 0x64;
        }
        if (fldGlobalBase.floor == 0x17) {
            idx = 0x65;
        }
    }
    if (fldGlobalBase.area == 0x1A) {
        idx = 0x66;
        if (fldGlobalBase.floor == 0x12) {
            idx = 0x65;
        }
        if (fldGlobalBase.floor == 0x13) {
            idx = 0x65;
        }
        if (fldGlobalBase.floor == 0x15) {
            idx = 0x65;
        }
    }
    if (fldGlobalBase.area == 0x1D) {
        idx = 0x66;
        if (fldGlobalBase.floor == 0x01) {
            idx = 0x64;
        }
        if (fldGlobalBase.floor == 0x09) {
            idx = 0x65;
        }
        if (fldGlobalBase.floor == 0x0A) {
            idx = 0x65;
        }
    }
    if (fldGlobalBase.area == 0x25) {
        if (fldGlobalBase.floor == 0x0F) {
            idx = 0x64;
        }
    }
    return idx;
}

void fldLightMaClearMsg(void)
{
    if ((DDS3_GBWK->maFlags & 0x8) != 0) {
        fldLightMaClear();
        fldGlobalBase.consumedFlags |= 0x1;
    }
}

void fldLightMaClear(void)
{
    DDS3_GBWK->maFlags &= ~0x8;
    fldGlobalBase.consumedFlags &= ~0x1;
}

void fldLightMaStart(void)
{
    if (gFldNowDZone != 0) {
        if ((DDS3_GBWK->maFlags & 0x8) == 0) {
            fldSePlay(0x29);
        }
        fldGlobalBase.lightMaIdx = fldLightMaIdxCalc();
        DDS3_GBWK->maFlags |= 0x8;
        fldGlobalBase.consumedFlags &= ~0x1;
        fldSkySetTblLightMa(fldGlobalBase.lightMaIdx, 0);
    }
}

s32 fldLightMaChk(void)
{
    if ((DDS3_GBWK->maFlags & 0x8) != 0) {
        return 1;
    }
    return gFldNowDZone != 0 ? 0 : -1;
}

void fldLightMaTask(void)
{
    if (gFldNowDZone == 0) {
        if (fldGlobalBase.lightMaIdx != 0) {
            fldGlobalBase.lightMaIdx = 0;
            fldSkySetTblLightMa(0, 0);
        }
    } else if (DDS3_GBWK->maFlags & 8) {
        if (fldGlobalBase.lightMaIdx == 0) {
            fldGlobalBase.lightMaIdx = fldLightMaIdxCalc();
            fldSkySetTblLightMa(fldGlobalBase.lightMaIdx, 0x14);
        }
    } else if (fldGlobalBase.lightMaIdx != 0) {
        fldGlobalBase.lightMaIdx = 0;
        fldSkySetTblLightMa(0, 0x14);
        fldSePlay(0x2A);
    }
}

void fldRifutoMaClearMsg(void)
{
    if ((DDS3_GBWK->maFlags & 0x4) != 0) {
        fldRifutoMaClear();
        fldGlobalBase.consumedFlags |= 0x2;
    }
}

void fldRifutoMaClear(void)
{
    DDS3_GBWK->maFlags &= ~0x4;
    fldGlobalBase.consumedFlags &= ~0x2;
}

void fldRifutoMaStart(void)
{
    DDS3_GBWK->maFlags |= 0x4;
    fldGlobalBase.consumedFlags &= ~0x2;
}

s32 fldRifutoMaChk(void)
{
    if (DDS3_GBWK->maFlags & 0x4) {
        return 1;
    }
    return 0;
}

void fldRiberaMaClearMsg(void)
{
    if ((DDS3_GBWK->maFlags & 0x2) != 0) {
        fldRiberaMaClear();
        fldGlobalBase.consumedFlags |= 0x4;
    }
}

void fldRiberaMaClear(void)
{
    DDS3_GBWK->maFlags &= ~0x2;
    fldGlobalBase.consumedFlags &= ~0x4;
}

void fldRiberaMaStart(void)
{
    DDS3_GBWK->maFlags = (DDS3_GBWK->maFlags | 0x2) & ~0x1;
    fldGlobalBase.consumedFlags &= ~0x4;
}

s32 fldRiberaMaChk(void)
{
    if (DDS3_GBWK->maFlags & 0x2) {
        return 1;
    }
    return 0;
}

void fldEsutoMaClearMsg(void)
{
    if ((DDS3_GBWK->maFlags & 0x1) != 0) {
        fldEsutoMaClear();
        fldGlobalBase.consumedFlags |= 0x8;
    }
}

void fldEsutoMaClear(void)
{
    DDS3_GBWK->maFlags &= ~0x1;
    fldGlobalBase.consumedFlags &= ~0x8;
}

void fldEsutoMaStart(void)
{
    DDS3_GBWK->maFlags = (DDS3_GBWK->maFlags | 0x1) & ~0x2;
    fldGlobalBase.consumedFlags &= ~0x8;
}

s32 fldEsutoMaChk(void)
{
    if (DDS3_GBWK->maFlags & 0x1) {
        return 1;
    }
    return 0;
}

/* NON_MATCHING: 10 of 30 words differ (first at +0x2C). */
#ifdef NON_MATCHING
void fldAnahoriSelectInit(void)
{
    s32 n = 0;
    s32 i;
    u32 flag;

    gFldAnahoriAkumaIdx = 0;
    gFldAnahoriAkumaIdxMax = 0;
    for (i = 0; i < 16; i++) {
        flag = DDS3_GBWK->akuma[i].flag;
        if (!(flag & 1)) {
            continue;
        }
        if (flag & 4) {
            continue;
        }
        if (flag & 0x10) {
            continue;
        }
        if (flag & 0x800) {
            continue;
        }
        if (DDS3_GBWK->akuma[i].hp != 0) {
            n++;
        }
    }
    gFldAnahoriAkumaIdxMax = n;
}
#else
INCLUDE_ASM(const s32, "field/fldMain", fldAnahoriSelectInit);
#endif

s32 fldAnahoriSelectGet(void)
{
    s32 n = 0;
    s32 i;
    u32 flag;

    if (gFldAnahoriAkumaIdxMax == 0) {
        return 0;
    }
    for (i = 0; i < 16; i++) {
        flag = DDS3_GBWK->akuma[i].flag;
        if (!(flag & 1)) {
            continue;
        }
        if (flag & 4) {
            continue;
        }
        if (flag & 0x10) {
            continue;
        }
        if (flag & 0x800) {
            continue;
        }
        if (DDS3_GBWK->akuma[i].hp == 0) {
            continue;
        }
        if (gFldAnahoriAkumaIdx == n) {
            return DDS3_GBWK->akuma[i].devilId;
        }
        n++;
    }
    return 0;
}

void fldAnahoriSelectNext(void)
{
    s32 idx = gFldAnahoriAkumaIdx + 1;

    if (idx >= gFldAnahoriAkumaIdxMax) {
        idx = 0;
    }
    gFldAnahoriAkumaIdx = idx;
}

void fldAnahoriSelectDel(void)
{
    s32 n = 0;
    s32 i;
    u32 flag;

    for (i = 0; i < 16; i++) {
        flag = DDS3_GBWK->akuma[i].flag;
        if (!(flag & 1)) {
            continue;
        }
        if (flag & 4) {
            continue;
        }
        if (flag & 0x10) {
            continue;
        }
        if (flag & 0x800) {
            continue;
        }
        if (DDS3_GBWK->akuma[i].hp == 0) {
            continue;
        }
        if (gFldAnahoriAkumaIdx == n) {
            if (DDS3_GBWK->akuma[i].level >= 0x15) {
                evtBitOn(0x504);
            }
            datDelDevil(DDS3_GBWK->akuma[i].devilId);
            return;
        }
        n++;
    }
}

s32 fldOtInit(void)
{
    if ((fldGlobalBase.area == 0x2A && fldGlobalBase.floor == 0xF) ||
        (fldGlobalBase.area == 0x2B && fldGlobalBase.floor == 0x8) ||
        (fldGlobalBase.area == 0x2B && fldGlobalBase.floor == 0x9) ||
        (fldGlobalBase.area == 0x2B && fldGlobalBase.floor == 0xA)) {
        sdfModelOtInit(D_002F6A18, &sdfGrOtTable[34], &sdfGrOtTable[40], &sdfGrOtTable[42],
                       &sdfGrOtTable[41]);
        D_002F6A18[1] = &sdfGrOtTable[46];
    } else if (fldGlobalBase.area == 0x20 || fldGlobalBase.area == 0x24 ||
               fldGlobalBase.area == 0x25) {
        sdfModelOtInit(D_002F6A18, &sdfGrOtTable[34], &sdfGrOtTable[40], &sdfGrOtTable[42],
                       &sdfGrOtTable[41]);
        D_002F6A18[1] = &sdfGrOtTable[35];
    } else {
        sdfModelOtInit(D_002F6A18, &sdfGrOtTable[34], &sdfGrOtTable[40], &sdfGrOtTable[42],
                       &sdfGrOtTable[41]);
    }
}
INCLUDE_SDATA(const s32, "field/fldMain", l_UseCollision);

INCLUDE_SDATA(const s32, "field/fldMain", gFldNowDZone);

INCLUDE_SDATA(const s32, "field/fldMain", fldPlayerAnimData);

INCLUDE_SDATA(const s32, "field/fldMain", mhfldPlayerFile);

INCLUDE_SDATA(const s32, "field/fldMain", gfldPlayerPtr);

INCLUDE_SDATA(const s32, "field/fldMain", gfldPlayerSize);

INCLUDE_SDATA(const s32, "field/fldMain", gfldPlayerSizeBk);

INCLUDE_SDATA(const s32, "field/fldMain", mhfldAnaFile);

INCLUDE_SDATA(const s32, "field/fldMain", gfldAnaPtr);

INCLUDE_SDATA(const s32, "field/fldMain", gfldAnaSize);

INCLUDE_SDATA(const s32, "field/fldMain", fldAnaBasic);

INCLUDE_SDATA(const s32, "field/fldMain", mhfldWaveFile);

INCLUDE_SDATA(const s32, "field/fldMain", gfldWavePtr);

INCLUDE_SDATA(const s32, "field/fldMain", gfldWaveSize);

INCLUDE_SDATA(const s32, "field/fldMain", pointResName);

INCLUDE_SDATA(const s32, "field/fldMain", D_0054EB38);

INCLUDE_SDATA(const s32, "field/fldMain", D_0054EB40);

INCLUDE_SDATA(const s32, "field/fldMain", gFldAnahoriAkumaIdx);

INCLUDE_SDATA(const s32, "field/fldMain", gFldAnahoriAkumaIdxMax);

