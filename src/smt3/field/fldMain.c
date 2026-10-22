#include "common.h"
#include "basic.h"
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
    s32 active;
    s32 count;
    u8 unk08[0x8];
    dds3Basic_t *handle;
} fldWave_t;

extern fldWave_t fldWaveBuff[];

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

typedef struct {
    u8 safeUnitPoint[0x30];
    u32 flag;
    s32 mode;
    u32 sequenceWord38;
    u32 sequenceWord3C;
    char name[16];
    s32 stage;
    s32 kind;
    s32 enabled;
    s32 restore;
    u16 code;
    u16 sequenceMode;
    s32 link;
    u8 unk68[8];
    char detail[16];
    char note[16];
    u32 options;
    u8 unk94[12];
} fldSceneParam_t;

typedef struct {
    s32 gunFlag;                /* 0x00 */
    s32 word04;
    s32 word08;
    s32 word0C;
    s32 word10;
    s32 word14;
    s32 word18;
    s32 word1C;
    s32 word20;
    s32 word24;
    s32 word28;
    s32 skyTable;               /* 0x2C */
    s32 word30;
    s32 filter;                 /* 0x34 */
    s32 word38;
    s32 word3C;
    s32 word40;
    s32 enabled;                /* 0x44 */
    u32 unk48;
    s32 counter;                /* 0x4C */
    s32 word50;
    s32 word54;
    u32 unk58;
    s32 word5C;
} fldDanteGunWork_t;

extern fldDanteGunWork_t fldRunDanteGunFlag;

extern u8 fldDoorExitFlag;

extern s8 fldDoorExitMode[];

extern char pointResName[];

extern char D_00531690[], D_00531950[];

extern void fclClearTerminalState(void);

extern sdfMemHandle_t *sdfDevLoadFileEx(char *path, void *destination, void *size);

extern s32 fldWmapIsEnabled(void);

extern s32 dds3ConfigChanged(void);

extern void dds3ConfigSaveBackup(void);

extern void dds3ConfigRestoreBackup(void);

extern void dds3SetPosPoseSet(dds3Basic_t *pBasic, void *pos);

extern void *dds3ObjectBaseGetModelHandle(dds3Basic_t *pObjectBasic);

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

typedef struct
{
    dds3Basic_t *handle;
    dds3Basic_t *handle2;
    s8 moveFlag;
    s8 rotFlag;
} fldUnitControl_t;

typedef struct
{
    sdfVector4_t wideCameraOffset;
    sdfVector4_t wideTargetOffset;
    sdfVector4_t narrowCameraOffset;
    sdfVector4_t narrowTargetOffset;
    sdfVector4_t q0;
    sdfVector4_t q1;
    f32 rotFrame;
    f32 rotFrameMax;
    f32 t0;
    f32 t1;
    f32 lenFrame;
    f32 lenFrameMax;
    dds3Basic_t *handle;
} fldFollowCameraControl_t;

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
    record.options = 1;
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

void fldInitAllValue(void)
{
    u8 *p = (u8 *)&fldSceneParam;

    SDF_VU0_STOREVECTOR_ZERO(p);
    SDF_VU0_STOREVECTOR_ZERO(p + 0x10);
    memset(fldGetUnitControl(), 0, 0xC);
    memset(fldGetFollowCameraControl(), 0, 0x7C);
}

void fldSetSceneParam2(fldSceneParam_t *pParam, s32 loadField, s32 loadArea, const char *pointResName) {
    if (dds3SearchProcessName(D_00531690) != NULL) {
        if (fldGlobalBase.area == loadField) {
            fldGlobalBase.reuseAreaResources = 1;
        } else {
            fclClearTerminalState();
        }
    }
    pParam->flag = 0;
    pParam->sequenceWord3C = 0;
    pParam->sequenceWord38 = 0;
    strcpy(pParam->name, pointResName);
    pParam->stage = loadField;
    pParam->enabled = 1;
    pParam->restore = 2;
    pParam->kind = loadArea;
    pParam->code = 0;
    pParam->sequenceMode = 0;
    pParam->link = 0;
    memset(pParam->detail, 0, sizeof(pParam->detail));
    memset(pParam->note, 0, sizeof(pParam->note));
    pParam->options = 0;
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
    pParam->sequenceWord3C = 0;
    pParam->sequenceWord38 = 0;
    strcpy(pParam->name, pointResName);
    pParam->stage = loadField;
    pParam->enabled = 1;
    pParam->restore = 3;
    pParam->kind = loadArea;
    pParam->code = 0;
    pParam->sequenceMode = 0;
    pParam->link = 0;
    memset(pParam->detail, 0, sizeof(pParam->detail));
    memset(pParam->note, 0, sizeof(pParam->note));
    pParam->options = 0;
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
    pParam->sequenceWord3C = 0;
    pParam->sequenceWord38 = 0;
    strcpy(pParam->name, pointResName);
    pParam->stage = loadField;
    pParam->enabled = 1;
    pParam->kind = loadArea;
    pParam->code = bgm;
    pParam->link = camMode;
    pParam->restore = 2;
    pParam->sequenceMode = 0;
    strcpy(pParam->detail, camName);
    memset(pParam->note, 0, sizeof(pParam->note));
    pParam->options = 0;
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
    pParam->sequenceWord3C = 0;
    pParam->sequenceWord38 = 0;
    strcpy(pParam->name, pointResName);
    pParam->stage = loadField;
    pParam->enabled = 1;
    pParam->restore = 2;
    pParam->kind = loadArea;
    pParam->code = bgm;
    pParam->sequenceMode = 0;
    pParam->link = 0;
    memset(pParam->detail, 0, sizeof(pParam->detail));
    strcpy(pParam->note, FoceScript);
    pParam->options = 0;
}

void fldSetSceneParamLoad(fldSceneParam_t *pParam, s32 loadField, s32 loadArea, const char *pointResName,
                    s32 bgm, s32 camMode, const char *camName) {
    pParam->flag = 0;
    pParam->sequenceWord3C = 0;
    pParam->sequenceWord38 = 0;
    strcpy(pParam->name, pointResName);
    pParam->enabled = 1;
    pParam->stage = loadField;
    pParam->kind = loadArea;
    pParam->code = bgm;
    pParam->link = camMode;
    pParam->restore = 2;
    pParam->sequenceMode = 0;
    strcpy(pParam->detail, camName);
    memset(pParam->note, 0, sizeof(pParam->note));
    pParam->options = 0;
}

s32 fldGetRestoreMode(void)
{
    if (fldSceneParam.mode == 1) {
        return 1;
    }
    if (fldSceneParam.restore == 3) {
        return 3;
    }
    if (fldSceneParam.restore == 4) {
        return 4;
    }
    if (fldSceneParam.restore == 5) {
        return 5;
    }
    if (fldSceneParam.restore == 6) {
        return 2;
    }
    return fldSceneParam.mode != 0 ? 2 : 0;
}

INCLUDE_ASM(const s32, "field/fldMain", fldReadyScene);

void fldPlayerALoad(void)
{
    if (fldPlayerAnimData == NULL) {
        fldPlayerAnimData = sdfDevLoadFileEx(D_005316B0, &gfldPlayerPtr, &gfldPlayerSizeBk);
    }
}

void fldPlayerAFree(void)
{
    if (fldPlayerAnimData != NULL) {
        sdfMemFreeDirect(fldPlayerAnimData);
        fldPlayerAnimData = NULL;
    }
}

void fldJDataLoad(void)
{
    void *src;
    void *dst;

    if (fldGlobalBase.area == 11 || fldGlobalBase.area == 14) {
        if (evtBitCheck(0x8B0) != 0) {
            mhfldPlayerFile = sdfDevLoadFileEx(D_005316D0, &gfldPlayerPtr, &gfldPlayerSize);
        } else {
            mhfldPlayerFile = sdfDevLoadFileEx(D_005316F0, &gfldPlayerPtr, &gfldPlayerSize);
        }
        mdlPackDecode(2, 0, 0x101, gfldPlayerPtr, gfldPlayerSize);
    } else if (fldGlobalBase.area < 10) {
        mhfldPlayerFile = sdfDevLoadFileEx(D_00531710, &gfldPlayerPtr, &gfldPlayerSize);
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
        mhfldAnaFile = sdfDevLoadFileEx(D_00531728, &gfldAnaPtr, &gfldAnaSize);
        mdlPackDecode(2, 5, 0x101, gfldAnaPtr, gfldAnaSize);
    }
    if (fldGlobalBase.area == 22) {
        mhfldWaveFile = sdfDevLoadFileEx(D_00531740, &gfldWavePtr, &gfldWaveSize);
        mdlPackDecode(2, 0x1B, 0x101, gfldWavePtr, gfldWaveSize);
    } else if (fldGlobalBase.area == 45) {
        mhfldWaveFile = sdfDevLoadFileEx(D_00531758, &gfldWavePtr, &gfldWaveSize);
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

INCLUDE_ASM(const s32, "field/fldMain", fldSetUnit);

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
        fldWaveBuff[i].active = 0;
        fldWaveBuff[i].count = 0;
        fldWaveBuff[i].handle = NULL;
        fldWaveBuff[i].handle = dds3AddUnitObjectBasic(dds3AllocObjectId(), pos, rot);
        dds3SetUnitObjectId(fldWaveBuff[i].handle, D_00531820);
        dds3ObjectBaseMakeUnit(fldWaveBuff[i].handle, 2, 0x1B);
        dds3ObjectBaseOnFlag(fldWaveBuff[i].handle, 1);
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
        if (fldWaveBuff[i].active != 0) {
            continue;
        }
        fldWaveBuff[i].active = 1;
        fldWaveBuff[i].count = 0;
        pos[0] = x;
        pos[1] = y;
        pos[2] = z;
        dds3SetPosPoseSet(fldWaveBuff[i].handle, pos);
        mdlSingleMotion(dds3ObjectBaseGetModelHandle(fldWaveBuff[i].handle), 0, 0);
        dds3ObjectBaseOffFlag(fldWaveBuff[i].handle, 1);
        mdlSingleMotion(dds3ObjectBaseGetModelHandle(fldWaveBuff[i].handle), 0, 0);
        return;
    }
}

void fldWaveTask(void)
{
    fldWave_t *p;
    s32 i;

    if (mhfldWaveFile == NULL) {
        return;
    }
    p = fldWaveBuff;
    for (i = 0x27; i >= 0; i--, p++) {
        if (p->active == 0) {
            continue;
        }
        if (++p->count >= 0x47) {
            dds3ObjectBaseOnFlag(p->handle, 1);
            p->active = 0;
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

