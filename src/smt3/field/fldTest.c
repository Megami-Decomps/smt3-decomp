#include "common.h"
#include "basic.h"
#include "kernel.h"
#include "libsdf.h"
#include "ps2asm.h"
#include "newdata.h"
#include "field.h"
#include "event.h"
#include "model.h"
#include "motion.h"

extern s32 fldDoor_ElvTakasaToFloor(s32 takasa);

extern dds3BasicArrayHandle_t *dds3SceneBasicGetList(dds3Basic_t *pBasic, s32 type);

extern s32 dds3SetFirstBasicArray(dds3BasicArrayHandle_t *hndl);

extern dds3Basic_t *dds3GetBasicArray(dds3BasicArrayHandle_t *hndl);

extern s32 dds3SetNextBasicArray(dds3BasicArrayHandle_t *hndl);

extern void dds3FreeBasicArrayHandle(dds3BasicArrayHandle_t *hndl);

extern s32 dds3ObjectBaseCheckFlag(dds3Basic_t *pObjectBasic, u32 flag);

extern void rsetOtOfs(u32 otofs);

extern void fldDrwZSave(void);

extern void fldDrwZLoad(void);

extern f32 R360_LR(f32 a, f32 b);

extern sdfVector4_t fldCamTarget;

extern s32 fldGb __attribute__((section(".data")));

extern f32 sdfAtan2(f32 aY, f32 aX);

extern dds3Basic_t *dds3GetControlScene(void);

extern void dds3GetPosPoseSet(dds3Basic_t *pBasic);

extern void fldRunDanteShutdown(void);

typedef struct {
    u32 firstOffset;
    u32 cellCount;
    u32 secondOffset;
} fldRunMapHeader_t;

extern void dds3ObjectBaseOnFlag(dds3Basic_t *basic, s32 flag);

extern s16 fldRun_MAP_OX;

extern s16 fldRun_MAP_OZ;

extern s16 fldRun_MAP_BX;

extern s16 fldRun_MAP_BZ;

extern s32 fldRunAiTimer __attribute__((section(".bss")));

extern s32 evtBitCheck(s32 no);

extern void fldSetElvCameraScene(s32 dir);

typedef struct {
    u16 sizeX;
    u16 sizeZ;
    u32 cellsOffset;
} fldRunMapGrid_t;

extern void fldMakeFilePath(char *pathstr, s32 field, s32 area);

extern s32 sprintf(char *text, const char *format, ...);

extern sdfMemHandle_t *sdfDevLoadFileEx(char *path, void *destination, void *size);

extern void dds3SetPosPoseSet(dds3Basic_t *pBasic, void *pos);

extern void *dds3ObjectBaseGetModelHandle(dds3Basic_t *pObjectBasic);

extern void dds3ObjectBaseOffFlag(dds3Basic_t *pObjectBasic, u32 flag);

extern void mdlSingleMotion(void *aHandle, s32 aGroup, s32 aNumber);

extern s32 FroFtoI(f32 fx, f32 fz, u16 *rx, u16 *rz);

extern u16 fldRunDanteStartX[2];

extern void fldRunDanteCollision(void);

extern void fldRunDanteAi(void);

extern void fldRunDanteSetModel(void);

extern void fldRunDanteSe(void);

extern void fldRunEffTask(void);

extern void fldRunTamaSe(void);

extern void fldRunDanteStanceSet(s32 stance);

extern s32 p2sdLoadCheckMidi(u32 MidiNo);

extern s32 p2sdLoadSMGFile(u32 MidiNo);

extern void sdfDrawSync(void);

extern s32 nbGetEncountOn(void);

extern void rsetTest(s32, s32, s32, s32, s32, s32, s32, s32);

extern void rsetAlpha(s32 mode);

extern void fldSkyDrawFilter(void);

extern void fldDrawFrameSprite(void);

extern u32 fldDbg_BlurOn[];

extern u32 dds3AllocObjectId(void);

extern dds3Basic_t *dds3AddUnitObjectBasic(s32 serialID, void *aPos, void *aRot);

extern void dds3SetUnitObjectId(dds3Basic_t *basic, const char *name);

extern void dds3ObjectBaseMakeUnit(dds3Basic_t *pObjectBasic, s32 major, s32 minor);

extern f32 sdfSin(f32 aRadius);

extern f32 sdfCos(f32 aRadius);

extern void fldFindDoorEvent(void *event);

extern void evtBitOn(s32 no);

extern s32 fldJDataHandle __attribute__((section(".data")));

extern void fldRunTamaFree(void);

extern void p2sdFadeOutBgm(u32 arg);

extern void p2sdStopBGM(u32 arg);

/* One loadable field effect: the resource handle trio the loader fills, then
 * the live effect or part handle and the placement the task applies. */
typedef struct {
    sdfMemHandle_t *mhFile;
    void *ptr;
    s32 size;
    void *pcf;
    void *d3p;
    s32 cnt;
    f32 x;
    f32 y;
    f32 z;
    f32 r;
} fldTest_FLDRUN_EFF_t;

typedef struct {
    fldTest_FLDRUN_EFF_t *pEff;
    s32 time;
    s32 effmode;
    s32 rotmode;
    f32 off_cam_range_min;
    f32 off_cam_range_max;
} fldTest_FLDRUN_EFFTBL_t;

extern fldTest_FLDRUN_EFFTBL_t *D_002FBEF0[];

extern char D_002FBD40[];

extern void FromRotation(f32 aAngle);

extern void dds3SetRotPoseSet(dds3Basic_t *pBasic, const sdfVector4_t *rot);

extern dds3Basic_t *D_0054EAB0;

extern dds3Basic_t *fldRunTama2Basic;

extern dds3Basic_t *fldRunTama3Basic;

extern s32 D_0054EABC;

extern sdfVector4_t D_00531148;

extern sdfVector4_t D_00531040;

extern char D_00531050[], D_00531060[], D_00531070[];

extern char D_00531080[], D_00531098[], D_005310B0[], D_005310C8[];

extern char D_005310E0[], D_005310F8[], D_00531118[], D_00531130[];

extern fldTest_FLDRUN_EFF_t fldDspark01File;

extern fldTest_FLDRUN_EFF_t fldDsmoke2File;

extern fldTest_FLDRUN_EFF_t fldDspark02File;

extern fldTest_FLDRUN_EFF_t fldGun02aFile;

extern fldTest_FLDRUN_EFF_t fldGun01a0File;

extern fldTest_FLDRUN_EFF_t fldMuzzleB010File;

extern fldTest_FLDRUN_EFF_t fldDsmoke1File;

extern fldTest_FLDRUN_EFF_t fldDflash1File;

extern void Mult(void);

typedef struct {
    f32 f[10];
} fldTamaSeFrame_t;

typedef struct {
    s16 v[10];
} fldTamaSeVol_t;

typedef struct {
    u8 unk00[0x1C];
    f32 frame;
} fldMotionCtrl_t;

typedef struct {
    u8 unk00[0x1C];
    fldMotionCtrl_t *motion;
} fldModel_t;

extern fldTamaSeFrame_t D_00531158;

extern fldTamaSeFrame_t D_00531180;

extern fldTamaSeFrame_t D_005311A8;

extern fldTamaSeVol_t D_005311D0;

extern s32 fldRunDanteDistToTarget(f32 x, f32 y, f32 z);

extern void frSetFontDrawOT(s32 OtType);

extern void fldKanjiDraw(s32 x, s32 y, u32 rgba, const char *moji);

extern void p2sdPlaySE(s32 id, s32 vol, s32 pan);

extern s32 fldProcCampChk(void);

extern s32 fldEncountTestFlag(u32 flag);

extern void dds3EffectDelete(void *pData);

extern void dds3EffectDraw(void *pData);

extern s32 dds3EffectGetState(void *hEffect);

extern void *dds3EffectLoad(void *pD3P);

extern void dds3EffectSetPosition(void *pData, sdfVector4_t *pPos);

extern void dds3EffectSetRotateMatrix(void *pData, sdfMatrix4_t *pMatrix);

extern void *dds3EffectPartCreate(u16 type, void *pData);

extern void dds3EffectPartDelete(void *hEffPart);

extern void dds3EffectPartDraw(void *hEffPart);

extern void dds3EffectPartSetPosition(void *hEffPart, sdfVector4_t *pPos);

extern void dds3EffectPartSetRotateMatrix(void *hEffPart, sdfMatrix4_t *pMatrix);

extern void SetRotateMatrixY(f32 aAngle);

typedef struct {
    char name[8];
    s16 ix;
    s16 iz;
    f32 angle;
} fldDanteDoor_t;

/* Dante's chase work block. Only the fields the chase code reaches are known. */
typedef struct
{
    s32 status;
    f32 x;
    f32 y;
    f32 z;
    f32 turn;
    f32 oldframe;
    f32 nowframe;
    s16 ix;
    s16 iz;
    u16 targetIx;
    u16 targetIz;
    u16 tikamiti;
    s16 wkmode;
    fldMapCell_t *routeCell;
    void *RouteDataTop;
    s32 now;
    s32 goal;
    s32 nextframe;
    f32 angleToPlayer;
    f32 speed;
    s32 cnt_ex;
    s32 gunlight_anime;
    u32 flag;
    s32 update_flag;
    s32 bgm_mode;
    f32 d_p_len;
    f32 d_p_dir;
    f32 d_c_dir;
    s32 stance;
    fldDanteDoor_t *pDoor;
    u16 mapIxOld;
    u16 mapIzOld;
    s32 old_status;
} fldTest_FLDRUN_DANTE_t;

extern char D_00530F30[];

extern fldTest_FLDRUN_DANTE_t fldRun_Dante;

extern s32 fldRunDanteHandle __attribute__((section(".bss")));

extern s32 fldRunDanteSe_asise;

extern s32 fldElvState;

extern s32 fldElvFloorTarget;

extern s32 gFldElvFloorNow;

extern s32 fldElvFloorFrom;

extern s32 fldElvFloorNum;

extern s32 fldElvFloorTo;

extern s32 fldElvFloorDisp;

extern sdfMemHandle_t *fldRunMapData;

extern fldRunMapHeader_t *fldRunMapHeader;

extern fldMapCell_t *fldRunMapCellData;

extern void *pFroMapHead;

extern void *fldRunMapScale;

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, dds3ProcessFunc_t drawExit,
                                         dds3ProcessFunc_t deleteExit, void *p);

extern s32 dds3KillProcessName(char *name, s32 shutdowntimer);

extern void sdfMemFreeDelayedDirect(sdfMemHandle_t *aHandle);

extern s32 fldAutoMapChkMode(void);

extern void fldClearElvCameraScene(void);

extern void fldRunDanteGetDistAngle(f32 *dist, f32 *angle);

extern s32 ooyTestDrawProc(dds3ProcessID_t *aProcId);

s32 ooyTestDrawProc(dds3ProcessID_t *aProcId)
{
    if (nbGetEncountOn() != 0) {
        return 0;
    }
    if (fldProcCampChk() == 1) {
        return 0;
    }
    rsetOtOfs(0x47);
    rsetTest(1, 0, 0x81, 3, 0, 0, 1, 1);
    rsetAlpha(0);
    fldSkyDrawFilter();
    if (fldDbg_BlurOn[0] != 0) {
        if (fldAutoMapChkMode() == 0) {
            rsetOtOfs(0x47);
        } else {
            rsetOtOfs(0x51);
        }
        fldDrawFrameSprite();
    }
    rsetOtOfs(0x27);
    rsetTest(1, 5, 0x80, 1, 0, 0, 1, 2);
    rsetAlpha(0);
    rsetOtOfs(0x2E);
    rsetTest(1, 5, 0x80, 1, 0, 0, 1, 2);
    rsetAlpha(0);
    return 0;
}

void InitOoyTest(void)
{
    dds3StartProcess(D_00530F30, DDS3_USERDRAW_PRIORITY, 0, 0, ooyTestDrawProc, NULL, NULL);
}

void KillOoyTest(void)
{
    dds3KillProcessName(D_00530F30, 1);
}

void fldElvGetFloorDigits(s32 x, s32 y, s32 floor)
{
    char text[32];
    s32 i;
    s32 digit;

    frSetFontDrawOT(0x48);
    if (floor > 1000) {
        char *roof = D_002FBD40;

        fldKanjiDraw(x, y / 2, -0x80, roof + 0x30);
        return;
    }
    i = 0;
    if (floor < 0) {
        text[0] = 'B';
        i = 1;
        floor = -floor;
    }
    digit = floor / 10;
    if (digit >= 1 && digit <= 9) {
        text[i] = digit + '0';
        i++;
    }
    digit = floor % 10;
    if (digit < 10) {
        text[i] = digit + '0';
        i++;
    }
    text[i] = 'F';
    text[i + 1] = '\0';
    fldKanjiDraw(x, y / 2, -0x80, text);
}

void fldElvSetFloor(s32 start, s32 end) {
    s32 first;
    s32 endEntry;

    first = fldDoor_ElvTakasaToFloor(start);
    fldElvState = 0;
    fldElvFloorFrom = first;
    if (first > 1000) {
        gFldElvFloorNow = first % 1000;
    } else {
        gFldElvFloorNow = first;
    }
    if (first > 1000) {
        fldElvFloorNum = first;
        fldElvFloorFrom = first % 1000;
    } else {
        fldElvFloorNum = -999;
    }

    endEntry = fldDoor_ElvTakasaToFloor(end);
    fldElvFloorTo = endEntry;
    if (endEntry > 1000) {
        fldElvFloorDisp = endEntry;
        fldElvFloorTo = endEntry % 1000;
    } else {
        fldElvFloorDisp = -999;
    }
    if (start == end) {
        fldElvFloorTarget = -999;
    } else {
        fldElvFloorTarget = 0;
    }
    if (gFldElvFloorNow != -999 && fldElvFloorTarget != -999) {
        fldElvFloorTarget++;
        if (fldElvFloorFrom < fldElvFloorTo) {
            fldSetElvCameraScene(1);
        } else {
            fldSetElvCameraScene(-1);
        }
    }
}

void fldElvReset(void)
{
    fldElvState = 0;
    gFldElvFloorNow = -999;
    fldElvFloorFrom = -999;
    fldElvFloorNum = -999;
    fldElvFloorTo = -999;
    fldElvFloorDisp = -999;
    fldElvFloorTarget = -999;
    fldClearElvCameraScene();
}

s32 fldElvCheckEnd(void)
{
    if (fldElvState == 1) {
        return 2;
    }
    if (fldElvFloorTarget != -999 && gFldElvFloorNow == fldElvFloorTo) {
        if (fldElvFloorFrom < gFldElvFloorNow) {
            fldSetElvCameraScene(2);
        } else {
            fldSetElvCameraScene(-2);
        }
        return 1;
    }
    return 0;
}

#ifdef NON_MATCHING
extern volatile s8 dds3PadButtonState[];

void fldElvDrawFloorNum(void)
{
    s32 delays[6] = { 50, 45, 30, 20, 15, 15 };
    volatile s32 *timer = &fldElvFloorTarget;
    volatile s32 *target = &fldElvFloorTo;
    volatile s32 *from = &fldElvFloorFrom;
    volatile s32 *displayFloor = &fldElvFloorDisp;
    volatile s32 *state = &fldElvState;
    s32 floor = gFldElvFloorNow;
    s32 timerValue;
    s32 targetFloor;
    s32 fromDistance;
    s32 toDistance;
    s32 distance;
    s32 display;
    s8 button;

    if (floor == -999) {
        return;
    }
    timerValue = *timer;
    targetFloor = *target;
    if (timerValue != -999) {
        *timer = (s32)((u32)timerValue + 1);
        toDistance = (s32)((u32)targetFloor - (u32)floor);
        fromDistance = (s32)((u32)*from - (u32)floor);
        if (toDistance < 0) {
            toDistance = (s32)(0U - (u32)toDistance);
        }
        if (fromDistance < 0) {
            fromDistance = (s32)(0U - (u32)fromDistance);
        }
        distance = fromDistance < toDistance ? fromDistance : toDistance;
        if (distance >= 6) {
            distance = 5;
        }
        if (delays[distance] < *timer) {
            *timer = 0;
            if (floor < targetFloor) {
                floor = (s32)((u32)floor + 1);
                if (floor == 0) {
                    floor++;
                }
                gFldElvFloorNow = floor;
            } else if (targetFloor < floor) {
                floor = (s32)((u32)floor - 1);
                if (floor == 0) {
                    floor--;
                }
                gFldElvFloorNow = floor;
            }
        }
    }
    button = dds3PadButtonState[0x23];
    display = *displayFloor;
    if (button < 0) {
        *state = 1;
        display = *displayFloor;
    }
    if (display < 1001) {
        display = fldElvFloorNum;
        if (display < 1001 || floor != display % 1000) {
            display = floor;
        }
    } else if (floor != display % 1000) {
        display = floor;
    }
    fldElvGetFloorDigits(380, 26, display);
}
#else
INCLUDE_RODATA(const s32, "field/fldTest", D_00530F30);

INCLUDE_ASM(const s32, "field/fldTest", fldElvDrawFloorNum);
#endif

void drawSceneBasicList(void) {
    s32 hasEligibleBackground = 0;
    dds3BasicArrayHandle_t *objectChain;
    dds3Basic_t *worldObject;

    objectChain = dds3SceneBasicGetList(dds3GetControlScene(), 5);
    if (objectChain != 0) {
        if (dds3SetFirstBasicArray((dds3BasicArrayHandle_t *)objectChain) != 0) {
            do {
                worldObject = (dds3Basic_t *)dds3GetBasicArray((dds3BasicArrayHandle_t *)objectChain);
                if (dds3ObjectBaseCheckFlag(worldObject, 0x200) != 0) {
                    if (dds3ObjectBaseCheckFlag(worldObject, 1) == 0) {
                        hasEligibleBackground = 1;

                    }
                }
            } while (dds3SetNextBasicArray((dds3BasicArrayHandle_t *)objectChain) != 0);
        }
        dds3FreeBasicArrayHandle(objectChain);
    }
    if (fldGb == 1) {
        hasEligibleBackground = 0;
    }
    if (hasEligibleBackground != 0) {
        rsetOtOfs(0x24);
        fldDrwZSave();
        rsetOtOfs(0x26);
        fldDrwZLoad();
    }
}

INCLUDE_ASM(const s32, "field/fldTest", fldSdwDraw);

void fldRunDanteClear(void)
{
    fldRunDanteHandle = -1;
}

#ifdef NON_MATCHING
extern void *memset(void *a, s32 b, u32 c);
#endif

void fldRunTamaInit(void)
{
    fldTest_FLDRUN_EFF_t *res;
    sdfVector4_t pos;
    sdfVector4_t rot;
    sdfVector4_t axis;

    memset(&pos, 0, sizeof(pos));
    pos.w = 1.0f;
    memset(&rot, 0, sizeof(rot));
    rot.w = 1.0f;
    axis = D_00531040;
    SDF_VU0_LOADVECTOR(&axis);
    FromRotation(3.1415926f);
    SDF_VU0_LOADVECTOR_B(&rot);
    Mult();
    SDF_VU0_STOREVECTOR_NOMEM(&rot);
    D_0054EAB0 = dds3AddUnitObjectBasic(dds3AllocObjectId(), &pos, &rot);
    dds3SetUnitObjectId(D_0054EAB0, D_00531050);
    dds3ObjectBaseMakeUnit(D_0054EAB0, 2, 0x11);
    dds3ObjectBaseOnFlag(D_0054EAB0, 1);
    dds3ObjectBaseOnFlag(D_0054EAB0, 0x100);
    fldRunTama2Basic = dds3AddUnitObjectBasic(dds3AllocObjectId(), &pos, &rot);
    dds3SetUnitObjectId(fldRunTama2Basic, D_00531060);
    dds3ObjectBaseMakeUnit(fldRunTama2Basic, 2, 0x12);
    dds3ObjectBaseOnFlag(fldRunTama2Basic, 1);
    dds3ObjectBaseOnFlag(fldRunTama2Basic, 0x100);
    fldRunTama3Basic = dds3AddUnitObjectBasic(dds3AllocObjectId(), &pos, &rot);
    dds3SetUnitObjectId(fldRunTama3Basic, D_00531070);
    dds3ObjectBaseMakeUnit(fldRunTama3Basic, 2, 0x13);
    dds3ObjectBaseOnFlag(fldRunTama3Basic, 1);
    dds3ObjectBaseOnFlag(fldRunTama3Basic, 0x100);
    res = &fldDspark01File;
    res->mhFile = NULL;
    res->ptr = NULL;
    res->size = 0;
    res->pcf = NULL;
    res->d3p = NULL;
    res->cnt = 0;
    res->x = 0.0f;
    res->y = 0.0f;
    res->z = 0.0f;
    res->mhFile = sdfDevLoadFileEx(D_00531080, &res->ptr, &res->size);
    res = &fldDsmoke2File;
    res->mhFile = NULL;
    res->ptr = NULL;
    res->size = 0;
    res->pcf = NULL;
    res->d3p = NULL;
    res->cnt = 0;
    res->x = 0.0f;
    res->y = 0.0f;
    res->z = 0.0f;
    res->r = 0.0f;
    res->mhFile = sdfDevLoadFileEx(D_00531098, &res->ptr, &res->size);
    res = &fldDspark02File;
    res->mhFile = NULL;
    res->ptr = NULL;
    res->size = 0;
    res->pcf = NULL;
    res->d3p = NULL;
    res->cnt = 0;
    res->x = 0.0f;
    res->y = 0.0f;
    res->z = 0.0f;
    res->r = 0.0f;
    res->mhFile = sdfDevLoadFileEx(D_005310B0, &res->ptr, &res->size);
    res = &fldGun02aFile;
    res->mhFile = NULL;
    res->ptr = NULL;
    res->size = 0;
    res->pcf = NULL;
    res->d3p = NULL;
    res->cnt = 0;
    res->x = 0.0f;
    res->y = 0.0f;
    res->z = 0.0f;
    res->r = 0.0f;
    res->mhFile = sdfDevLoadFileEx(D_005310C8, &res->ptr, &res->size);
    res = &fldGun01a0File;
    res->mhFile = NULL;
    res->ptr = NULL;
    res->size = 0;
    res->pcf = NULL;
    res->d3p = NULL;
    res->cnt = 0;
    res->x = 0.0f;
    res->y = 0.0f;
    res->z = 0.0f;
    res->r = 0.0f;
    res->mhFile = sdfDevLoadFileEx(D_005310E0, &res->ptr, &res->size);
    res = &fldMuzzleB010File;
    res->mhFile = NULL;
    res->ptr = NULL;
    res->size = 0;
    res->pcf = NULL;
    res->d3p = NULL;
    res->cnt = 0;
    res->x = 0.0f;
    res->y = 0.0f;
    res->z = 0.0f;
    res->r = 0.0f;
    res->mhFile = sdfDevLoadFileEx(D_005310F8, &res->ptr, &res->size);
    res = &fldDsmoke1File;
    res->mhFile = NULL;
    res->ptr = NULL;
    res->size = 0;
    res->pcf = NULL;
    res->d3p = NULL;
    res->cnt = 0;
    res->x = 0.0f;
    res->y = 0.0f;
    res->z = 0.0f;
    res->r = 0.0f;
    res->mhFile = sdfDevLoadFileEx(D_00531118, &res->ptr, &res->size);
    res = &fldDflash1File;
    res->mhFile = NULL;
    res->ptr = NULL;
    res->size = 0;
    res->pcf = NULL;
    res->d3p = NULL;
    res->cnt = 0;
    res->x = 0.0f;
    res->y = 0.0f;
    res->z = 0.0f;
    res->r = 0.0f;
    res->mhFile = sdfDevLoadFileEx(D_00531130, &res->ptr, &res->size);
}

/* NON_MATCHING: 27 of 47 words differ (first at +0x2C). */
#ifdef NON_MATCHING
void fldRunTamaFree(void)
{
    fldTest_FLDRUN_EFFTBL_t *entry;
    fldTest_FLDRUN_EFF_t *res;
    void *handle;
    s32 i;

    for (i = 0; i < 2; i++) {
        for (entry = D_002FBEF0[i]; (res = entry->pEff) != NULL; entry++) {
            if (entry->effmode == 0) {
                handle = res->d3p;
                if (handle != NULL) {
                    dds3EffectDelete(handle);
                    res->d3p = NULL;
                }
            } else {
                handle = res->pcf;
                if (handle != NULL) {
                    dds3EffectPartDelete(handle);
                    res->pcf = NULL;
                }
            }
            handle = res->mhFile;
            if (handle != NULL) {
                sdfMemFreeDelayedDirect(handle);
                res->mhFile = NULL;
            }
        }
    }
}
#else
INCLUDE_ASM(const s32, "field/fldTest", fldRunTamaFree);
#endif

void fldRunEffSet(s32 id, f32 x, f32 y, f32 z, f32 r)
{
    fldTest_FLDRUN_EFFTBL_t *entry = D_002FBEF0[id];
    fldTest_FLDRUN_DANTE_t *dante = &fldRun_Dante;
    fldTest_FLDRUN_EFF_t *res;
    void *handle;

    for (;;) {
        res = entry->pEff;
        if (res == NULL) {
            return;
        }
        if (dante->d_c_dir > entry->off_cam_range_min && dante->d_c_dir < entry->off_cam_range_max) {
            if (entry->effmode == 0) {
                handle = res->d3p;
                if (handle != NULL) {
                    dds3EffectDelete(handle);
                    res->d3p = NULL;
                }
                res->d3p = dds3EffectLoad(res->ptr);
                res->cnt = entry->time;
            } else {
                handle = res->pcf;
                if (handle != NULL) {
                    dds3EffectPartDelete(handle);
                    res->pcf = NULL;
                }
                res->pcf = dds3EffectPartCreate(6, res->ptr);
                res->cnt = entry->time;
            }
            res->x = x;
            res->y = y;
            res->z = z;
            res->r = r;
        }
        entry++;
    }
}

/* NON_MATCHING: 115 of 136 words differ (first at +0x10). */
#ifdef NON_MATCHING
void fldRunEffTask(void)
{
    sdfVector4_t pos;
    sdfMatrix4_t matrix;
    fldTest_FLDRUN_EFFTBL_t *entry;
    fldTest_FLDRUN_EFF_t *res;
    s32 i;

    memset(&pos, 0, sizeof(pos));
    pos.w = 1.0f;

    for (i = 0; i < 2; i++) {
        for (entry = D_002FBEF0[i]; (res = entry->pEff) != NULL; entry++) {
            if (entry->effmode == 0) {
                if (res->d3p == NULL) {
                    continue;
                }
                if (dds3EffectGetState(res->d3p) == 0) {
                    dds3EffectDelete(res->d3p);
                    res->d3p = NULL;
                    continue;
                }
                pos.x = res->x;
                pos.y = res->y;
                pos.z = res->z;
                dds3EffectSetPosition(res->d3p, &pos);
                if (entry->rotmode == 1) {
                    SetRotateMatrixY((90.0f - res->r) * 3.14f / 180.0f);
                    SDF_VU0_STOREMATRIX(&matrix);
                    dds3EffectSetRotateMatrix(res->d3p, &matrix);
                }
                dds3EffectDraw(res->d3p);
            } else {
                if (res->pcf == NULL) {
                    continue;
                }
                if (res->cnt < 0) {
                    dds3EffectPartDelete(res->pcf);
                    res->pcf = NULL;
                    continue;
                }
                res->cnt = res->cnt - 1;
                pos.x = res->x;
                pos.y = res->y;
                pos.z = res->z;
                dds3EffectPartSetPosition(res->pcf, &pos);
                if (entry->rotmode == 1) {
                    SetRotateMatrixY((90.0f - res->r) * 3.14f / 180.0f);
                    SDF_VU0_STOREMATRIX(&matrix);
                    dds3EffectPartSetRotateMatrix(res->pcf, &matrix);
                }
                dds3EffectPartDraw(res->pcf);
            }
        }
    }
}
#else
INCLUDE_ASM(const s32, "field/fldTest", fldRunEffTask);
#endif

void fldRunTamaSet(f32 x, f32 y, f32 z, f32 r)
{
    sdfVector4_t pos;
    sdfVector4_t rot;
    sdfVector4_t axis;

    memset(&pos, 0, sizeof(pos));
    pos.w = 1.0f;
    memset(&rot, 0, sizeof(rot));
    rot.w = 1.0f;
    axis = D_00531148;
    pos.x = x;
    pos.y = y;
    pos.z = z;
    SDF_VU0_LOADVECTOR(&axis);
    r = r * 3.14f;
    FromRotation(r / 180.0f);
    SDF_VU0_STOREVECTOR(&rot);
    switch (D_0054EABC) {
    case 0:
        dds3ObjectBaseOffFlag(D_0054EAB0, 1);
        mdlSingleMotion(dds3ObjectBaseGetModelHandle(D_0054EAB0), 0, 0);
        dds3SetRotPoseSet(D_0054EAB0, &rot);
        dds3SetPosPoseSet(D_0054EAB0, &pos);
        break;
    case 1:
        dds3ObjectBaseOffFlag(fldRunTama2Basic, 1);
        mdlSingleMotion(dds3ObjectBaseGetModelHandle(fldRunTama2Basic), 0, 0);
        dds3SetRotPoseSet(fldRunTama2Basic, &rot);
        dds3SetPosPoseSet(fldRunTama2Basic, &pos);
        break;
    case 2:
        dds3ObjectBaseOffFlag(fldRunTama3Basic, 1);
        mdlSingleMotion(dds3ObjectBaseGetModelHandle(fldRunTama3Basic), 0, 0);
        dds3SetRotPoseSet(fldRunTama3Basic, &rot);
        dds3SetPosPoseSet(fldRunTama3Basic, &pos);
        break;
    }
    D_0054EABC = D_0054EABC + 1;
    D_0054EABC = D_0054EABC % 3;
}

/* NON_MATCHING: 165 of 230 words differ (first at +0x34). */
#ifdef NON_MATCHING
void fldRunTamaSe(void)
{
    sdfVector4_t pos;
    fldTamaSeFrame_t frame1;
    fldTamaSeFrame_t frame2;
    fldTamaSeFrame_t frame3;
    fldTamaSeVol_t vol;
    fldModel_t *model;
    f32 now;
    s32 i;

    memset(&pos, 0, sizeof(pos));
    pos.w = 1.0f;
    frame1 = D_00531158;
    frame2 = D_00531180;
    frame3 = D_005311A8;
    vol = D_005311D0;
    model = dds3ObjectBaseGetModelHandle(D_0054EAB0);
    now = model->motion->frame;
    for (i = 0; i < 10; i++) {
        if (frame1.f[i] - 0.5f < now && now < frame1.f[i] + 0.5f) {
            dds3GetPosPoseSet(D_0054EAB0);
            SDF_VU0_STOREVECTOR_MEMOPS(pos);
            p2sdPlaySE(0x6F0006,
                       fldRunDanteDistToTarget(pos.x, pos.y, pos.z) * vol.v[i] / 128 / 2, 0x3F);
            break;
        }
    }
    model = dds3ObjectBaseGetModelHandle(fldRunTama2Basic);
    now = model->motion->frame;
    for (i = 0; i < 10; i++) {
        if (frame2.f[i] - 0.5f < now && now < frame2.f[i] + 0.5f) {
            dds3GetPosPoseSet(fldRunTama2Basic);
            SDF_VU0_STOREVECTOR_MEMOPS(pos);
            p2sdPlaySE(0x6F0006,
                       fldRunDanteDistToTarget(pos.x, pos.y, pos.z) * vol.v[i] / 128 / 2, 0x3F);
            break;
        }
    }
    model = dds3ObjectBaseGetModelHandle(fldRunTama3Basic);
    now = model->motion->frame;
    for (i = 0; i < 10; i++) {
        if (frame3.f[i] - 0.5f < now && now < frame3.f[i] + 0.5f) {
            dds3GetPosPoseSet(fldRunTama3Basic);
            SDF_VU0_STOREVECTOR_MEMOPS(pos);
            p2sdPlaySE(0x6F0006,
                       fldRunDanteDistToTarget(pos.x, pos.y, pos.z) * vol.v[i] / 128 / 2, 0x3F);
            break;
        }
    }
}
#else
INCLUDE_ASM(const s32, "field/fldTest", fldRunTamaSe);
#endif

INCLUDE_RODATA(const s32, "field/fldTest", D_00531040);

INCLUDE_RODATA(const s32, "field/fldTest", D_00531050);

INCLUDE_RODATA(const s32, "field/fldTest", D_00531060);

INCLUDE_RODATA(const s32, "field/fldTest", D_00531070);

INCLUDE_RODATA(const s32, "field/fldTest", D_00531080);

INCLUDE_RODATA(const s32, "field/fldTest", D_00531098);

INCLUDE_RODATA(const s32, "field/fldTest", D_005310B0);

INCLUDE_RODATA(const s32, "field/fldTest", D_005310C8);

INCLUDE_RODATA(const s32, "field/fldTest", D_005310E0);

INCLUDE_RODATA(const s32, "field/fldTest", D_005310F8);

INCLUDE_RODATA(const s32, "field/fldTest", D_00531118);

INCLUDE_RODATA(const s32, "field/fldTest", D_00531130);

INCLUDE_RODATA(const s32, "field/fldTest", D_00531148);

INCLUDE_RODATA(const s32, "field/fldTest", D_00531158);

INCLUDE_RODATA(const s32, "field/fldTest", D_00531180);

INCLUDE_RODATA(const s32, "field/fldTest", D_005311A8);

INCLUDE_RODATA(const s32, "field/fldTest", D_005311D0);

void fldMakeFroFileName(void) {
    char directory[0x20];
    char path[0x40];
    fldRunMapHeader_t *resource;
    u32 base;
    u32 first;
    u32 second;
    u32 third;

    fldMakeFilePath(directory, fldGlobalBase.area, fldGlobalBase.floor + 1);
    sprintf(path, "%sF%03d_%03d.FRO", directory, fldGlobalBase.area, fldGlobalBase.floor + 1);
    fldRunMapData = sdfDevLoadFileEx(path, &resource, 0);
    base = (u32)resource;
    first = base + resource->firstOffset;
    second = base + resource->secondOffset;
    third = base + ((fldRunMapGrid_t *)second)->cellsOffset;
    fldRunMapHeader = (fldRunMapHeader_t *)base;
    fldRunMapCellData = (fldMapCell_t *)first;
    pFroMapHead = (fldRunMapGrid_t *)second;
    fldRunMapScale = (void *)third;
}

void fldFreeRunMapData(void)
{
    if (fldRunMapData != NULL) {
        sdfMemFreeDelayedDirect(fldRunMapData);
        fldRunMapData = NULL;
        fldRunMapHeader = NULL;
        fldRunMapCellData = NULL;
        pFroMapHead = NULL;
        fldRunMapScale = NULL;
    }
}

s32 FroFtoI(f32 fx, f32 fz, u16 *rx, u16 *rz)
{
    f32 ox = fldRun_MAP_OX;
    f32 oz = fldRun_MAP_OZ;
    f32 best = 10000.0f;
    s16 bestX = 0;
    s16 bestZ = 0;
    s16 i;
    s16 j;
    s32 offset;
    f32 dx;
    f32 dz;
    f32 d;

    for (j = 0; j < ((fldRunMapGrid_t *)pFroMapHead)->sizeZ; j++) {
        for (i = 0; i < ((fldRunMapGrid_t *)pFroMapHead)->sizeX; i++) {
            offset = (((fldRunMapGrid_t *)pFroMapHead)->sizeX * j + i) * 2;
            if (((u8 *)fldRunMapScale)[offset] != 0) {
                dx = fx - (ox + (f32)fldRun_MAP_BX * (f32)i);
                dz = fz - (oz + (f32)-fldRun_MAP_BZ * (f32)j);
                d = sdfSqrt(dx * dx + dz * dz);
                if (d < best) {
                    best = d;
                    bestZ = j;
                    bestX = i;
                }
            }
        }
    }
    *rx = bestX;
    *rz = bestZ;
    if (bestX == 0 && bestZ == 0) {
        return -1;
    }
    return 0;
}

void fldMapIndexToPos(s16 ix, s16 iz, f32 *px, f32 *pz)
{
    f32 ox = fldRun_MAP_OX;
    f32 oz = fldRun_MAP_OZ;

    *px = ox + (f32)fldRun_MAP_BX * (f32)ix;
    *pz = oz + (f32)-fldRun_MAP_BZ * (f32)iz;
}

fldMapCell_t *fldFindMapCell(s16 x, s16 y, s16 z, s16 w)
{
    fldMapCell_t *p = fldRunMapCellData;
    u32 i;

    for (i = 0; i < fldRunMapHeader->cellCount; i++, p++) {
        if (p->x == (u8)x && p->y == (u8)y && p->z == (u8)z && p->w == (u8)w) {
            return p;
        }
    }
    return NULL;
}

void fldRunDanteGetDistAngle(f32 *pDist, f32 *pAngle)
{
    f32 dx;
    f32 dz;
    f32 dist;
    f32 angle;

    dx = fldGlobalBase.x - fldRun_Dante.x;
    dz = fldGlobalBase.z - fldRun_Dante.z;
    dist = sdfSqrt(dx * dx + dz * dz);
    if (dist < 1.0f) {
        angle = 0.0f;
    } else {
        angle = sdfAtan2(dx, dz) * 180.0f / 3.14f;
    }
    *pDist = dist;
    *pAngle = angle;
}

void fldGetDistAngleTo(f32 *pDist, f32 *pAngle, f32 x, f32 z)
{
    f32 dx;
    f32 dz;
    f32 dist;
    f32 angle;

    dx = x - fldRun_Dante.x;
    dz = z - fldRun_Dante.z;
    dist = sdfSqrt(dx * dx + dz * dz);
    if (dist < 1.0f) {
        angle = 0.0f;
    } else {
        angle = sdfAtan2(dx, dz) * 180.0f / 3.14f;
    }
    *pDist = dist;
    *pAngle = angle;
}

f32 fldTurnToward(f32 cur, f32 target, f32 speed, f32 minStep) {
    f32 delta = R360_LR(cur, target);
    f32 step = delta / speed;

    if (step < 0.0f) {
        step = -step;
    }
    if (step < minStep) {
        step = minStep;
    }
    if ((delta >= 0.0f && delta <= step) || (delta <= 0.0f && -step <= delta)) {
        delta = target;
    } else if (delta < 0.0f) {
        delta = cur - step;
    } else {
        delta = cur + step;
    }
    return delta;
}

#ifdef NON_MATCHING
extern s32 printf(const char *format, ...);
extern u32 dds3GetRandA(void *name);
extern s32 fldHitChkEnemy(const f32 *position, const f32 *target, f32 *hitPosition);
extern char D_0054EAC0[];
extern char D_0054EAC8[];

void fldRunAiCng(s32 mode)
{
    volatile f32 distance = fldRun_Dante.d_p_len;
    volatile f32 angle = fldRun_Dante.d_p_dir;
    fldMapCell_t *cell;
    fldRoutePoint_t *route;
    f32 targetX;
    f32 targetZ;

    fldRun_Dante.cnt_ex = 0;
    fldRun_Dante.tikamiti = 0;
    if (mode == 0) {
        printf("AI:[ NULL ]\n");
        fldRun_Dante.status = 0;
        fldRun_Dante.speed = 0.0f;
        fldRun_Dante.wkmode = 0;
        fldRun_Dante.nextframe = 0;
    } else if (mode == 1) {
        printf("AI:[ STND ]\n");
        fldRun_Dante.status = mode;
        fldRun_Dante.nextframe = 30;
        fldRun_Dante.update_flag = 3;
        fldRun_Dante.flag |= 2;
        fldRun_Dante.wkmode = 0;
        fldRun_Dante.speed = 0.0f;
    } else if (mode == 2) {
        printf("AI:[ WAIT ]\n");
        fldRun_Dante.status = mode;
        fldRun_Dante.nextframe = 30;
        fldRun_Dante.update_flag = 3;
        fldRun_Dante.flag |= 2;
        fldRun_Dante.wkmode = 0;
        fldRun_Dante.speed = 0.0f;
    } else if (mode == 9) {
        printf("AI:[ DOIN ]\n");
        fldRun_Dante.status = mode;
        fldRun_Dante.update_flag = 2;
        fldRun_Dante.nextframe = 80;
        fldRun_Dante.speed = 10.0f;
        fldRun_Dante.flag |= 2;
    } else if (mode == 6) {
        printf("AI:[ PRUN ]\n");
        fldRun_Dante.status = mode;
        fldRun_Dante.wkmode = 0;
        FroFtoI(fldRun_Dante.x, fldRun_Dante.z, (u16 *)&fldRun_Dante.ix, (u16 *)&fldRun_Dante.iz);
        cell = fldFindMapCell(fldRun_Dante.ix, fldRun_Dante.iz, (s16)fldRunDanteStartX[0], (s16)fldRunDanteStartX[1]);
        fldRun_Dante.routeCell = cell;
        fldRun_Dante.targetIx = fldRunDanteStartX[0];
        fldRun_Dante.targetIz = fldRunDanteStartX[1];
        if (cell != NULL) {
            fldRun_Dante.now = 0;
            route = (fldRoutePoint_t *)((u8 *)fldRunMapHeader + cell->routeOffset);
            fldRun_Dante.speed = 20.0f;
            fldRun_Dante.nextframe = 30;
            fldRun_Dante.goal = cell->routeCount - 1;
            fldRun_Dante.RouteDataTop = route;
            fldRun_Dante.angleToPlayer = route->angle;
        } else {
            printf(D_0054EAC0);
            fldRun_Dante.RouteDataTop = NULL;
            fldRun_Dante.speed = 0.0f;
            fldRun_Dante.nextframe = 20;
            fldRun_Dante.now = 0;
            fldRun_Dante.goal = 0;
            fldRun_Dante.angleToPlayer = 0.0f;
        }
        fldRun_Dante.update_flag = 0;
        fldRun_Dante.flag |= 2;
    } else if (mode == 5) {
        printf("AI:[ PWAK ]\n");
        fldRun_Dante.status = mode;
        fldRun_Dante.wkmode = 1;
        FroFtoI(fldRun_Dante.x, fldRun_Dante.z, (u16 *)&fldRun_Dante.ix, (u16 *)&fldRun_Dante.iz);
        FroFtoI(fldGlobalBase.x, fldGlobalBase.z, &fldRunDanteStartX[0], &fldRunDanteStartX[1]);
        cell = fldFindMapCell(fldRun_Dante.ix, fldRun_Dante.iz, (s16)fldRunDanteStartX[0], (s16)fldRunDanteStartX[1]);
        fldRun_Dante.routeCell = cell;
        fldRun_Dante.targetIx = fldRunDanteStartX[0];
        fldRun_Dante.targetIz = fldRunDanteStartX[1];
        if (cell != NULL) {
            fldRun_Dante.now = 0;
            route = (fldRoutePoint_t *)((u8 *)fldRunMapHeader + cell->routeOffset);
            fldRun_Dante.speed = 5.0f;
            fldRun_Dante.nextframe = 120;
            fldRun_Dante.goal = cell->routeCount - 1;
            fldRun_Dante.RouteDataTop = route;
            fldRun_Dante.angleToPlayer = route->angle;
        } else {
            fldRun_Dante.speed = 0.0f;
            fldRun_Dante.nextframe = 20;
            fldRun_Dante.RouteDataTop = NULL;
            fldRun_Dante.now = 0;
            fldRun_Dante.goal = 0;
            fldRun_Dante.angleToPlayer = 0.0f;
        }
        fldRun_Dante.update_flag = 2;
        fldRun_Dante.flag |= 2;
    } else if (mode == 3) {
        u32 delay;
        printf("AI:[ SOBA ]\n");
        fldRun_Dante.status = mode;
        delay = (dds3GetRandA(NULL) >> 3) % 40;
        fldRun_Dante.update_flag = 0;
        fldRun_Dante.speed = 25.0f;
        fldRun_Dante.flag |= 2;
        fldRun_Dante.nextframe = delay + 40;
    } else if (mode == 4) {
        printf("AI:[ PATR ]\n");
        fldRun_Dante.status = 4;
        FroFtoI(fldRun_Dante.x, fldRun_Dante.z, (u16 *)&fldRun_Dante.ix, (u16 *)&fldRun_Dante.iz);
        FroFtoI(fldGlobalBase.x, fldGlobalBase.z, &fldRunDanteStartX[0], &fldRunDanteStartX[1]);
        cell = fldFindMapCell(fldRun_Dante.ix, fldRun_Dante.iz, (s16)fldRunDanteStartX[0], (s16)fldRunDanteStartX[1]);
        fldRun_Dante.routeCell = cell;
        fldRun_Dante.targetIx = fldRunDanteStartX[0];
        fldRun_Dante.targetIz = fldRunDanteStartX[1];
        if (cell != NULL) {
            fldRoutePoint_t *routeBase;
            f32 d;
            f32 a;

            fldRun_Dante.now = 0;
            fldRun_Dante.nextframe = 0;
            fldRun_Dante.goal = cell->routeCount - 1;
            fldRun_Dante.speed = fldRun_Dante.wkmode != 0 ? 6.0f : 20.0f;
            routeBase = (fldRoutePoint_t *)((u8 *)fldRunMapHeader + fldRun_Dante.routeCell->routeOffset);
            route = routeBase;
            if (fldRun_Dante.routeCell->routeCount >= 2) {
                f32 position[4];
                f32 targetPos[4];
                f32 hitPosition[4];

                route++;
                fldMapIndexToPos(route->x, route->z, &targetX, &targetZ);
                position[0] = fldRun_Dante.x;
                position[1] = 0.0f;
                position[2] = fldRun_Dante.z;
                position[3] = 1.0f;
                targetPos[0] = targetX;
                targetPos[1] = 0.0f;
                targetPos[2] = targetZ;
                targetPos[3] = 1.0f;
                if (fldHitChkEnemy(position, targetPos, hitPosition) != -1) {
                    route = routeBase;
                } else {
                    printf(D_0054EAC8);
                    fldRun_Dante.now++;
                }
            }
            fldMapIndexToPos(route->x, route->z, &targetX, &targetZ);
            fldGetDistAngleTo(&d, &a, targetX, targetZ);
            distance = d;
            angle = a;
            fldRun_Dante.angleToPlayer = angle;
            fldRun_Dante.nextframe = (s32)(distance / fldRun_Dante.speed);
        } else {
            fldRun_Dante.nextframe = 20;
            fldRun_Dante.now = 0;
            fldRun_Dante.goal = 0;
            fldRun_Dante.angleToPlayer = 0.0f;
            fldRun_Dante.speed = 0.0f;
            if (fldRun_Dante.ix == (s16)fldRunDanteStartX[0] && fldRun_Dante.iz == (s16)fldRunDanteStartX[1]) {
                f32 d;
                f32 a;
                fldMapIndexToPos(fldRun_Dante.ix, fldRun_Dante.iz, &targetX, &targetZ);
                fldGetDistAngleTo(&d, &a, targetX, targetZ);
                distance = d;
                angle = a;
                fldRun_Dante.speed = fldRun_Dante.wkmode != 0 ? 6.0f : 20.0f;
                fldRun_Dante.angleToPlayer = angle;
                fldRun_Dante.nextframe = (s32)(distance / fldRun_Dante.speed);
            }
        }
        fldRun_Dante.update_flag = fldRun_Dante.wkmode != 0 ? 2 : 0;
        fldRun_Dante.flag |= 2;
    } else if (mode == 7) {
        u32 delay;
        printf("AI:[ GUN1 ]\n");
        fldRun_Dante.status = mode;
        fldRun_Dante.wkmode = 0;
        delay = (dds3GetRandA(NULL) >> 3) % 60;
        fldRun_Dante.update_flag = 1;
        fldRun_Dante.speed = 8.0f;
        fldRun_Dante.flag |= 2;
        fldRun_Dante.nextframe = delay + 50;
    } else if (mode == 8) {
        printf("AI:[ GUN2 ]\n");
        fldRun_Dante.status = mode;
        fldRun_Dante.update_flag = 4;
        fldRun_Dante.nextframe = 90;
        fldRun_Dante.flag |= 2;
        fldRun_Dante.wkmode = 0;
        fldRun_Dante.speed = 0.0f;
    }
}
#else
INCLUDE_ASM(const s32, "field/fldTest", fldRunAiCng);
#endif

void func_0011C790(void) {
}

void func_0011C798(void)
{
    if (fldRun_Dante.nextframe > 0) {
        fldRun_Dante.nextframe--;
    }
}

void func_0011C7B8(void)
{
    if (fldRun_Dante.nextframe > 0) {
        fldRun_Dante.nextframe--;
    }
    if (fldRun_Dante.bgm_mode != 0) {
        fldRun_Dante.bgm_mode = 0;
        fldRun_Dante.flag |= 0x4;
    }
}

#ifdef NON_MATCHING
extern dds3Basic_t *dds3SceneBasicSearchNameAndType(dds3Basic_t *scene, s32 type, const char *name);
#endif

void fldRunDanteDoin(void)
{
    void **basic;

    if (fldRun_Dante.nextframe > 0) {
        fldRun_Dante.nextframe--;
    }
    if (fldRun_Dante.nextframe == 0x4E && fldRun_Dante.pDoor != NULL) {
        basic = dds3SceneBasicSearchNameAndType(dds3GetControlScene(), 0x11, fldRun_Dante.pDoor->name);
        fldFindDoorEvent(basic[1]);
    }
    if (fldRun_Dante.nextframe >= 0x15) {
        fldRun_Dante.x += sdfSin(fldRun_Dante.angleToPlayer * 3.14f / 180.0f) * fldRun_Dante.speed;
        fldRun_Dante.z += sdfCos(fldRun_Dante.angleToPlayer * 3.14f / 180.0f) * fldRun_Dante.speed;
    } else if (fldRun_Dante.nextframe == 0x14) {
        fldRun_Dante.update_flag = 3;
        fldRun_Dante.flag |= 2;
    }
    FroFtoI(fldRun_Dante.x, fldRun_Dante.z, &fldRun_Dante.ix, &fldRun_Dante.iz);
}

extern s32 fldHitChkEnemy(const f32 *position, const f32 *target, f32 *hitPosition);
extern void fldRunAiCng(s32 mode);
extern s32 printf(const char *format, ...);

void fldRunDanteMove(void)
{
    f32 originX = (f32)fldRun_MAP_OX;
    f32 originZ = (f32)fldRun_MAP_OZ;
    f32 targetX;
    f32 targetZ;
    f32 distance;
    f32 angle;
    f32 position[4];
    f32 target[4];
    f32 hitPosition[3];
    u8 *routeEntry;

    if (fldRun_Dante.cnt_ex < 300) {
        fldRun_Dante.cnt_ex++;
    }
    if (fldRun_Dante.nextframe > 0) {
        fldRun_Dante.nextframe--;
    }

    routeEntry = fldRun_Dante.RouteDataTop;
    if (routeEntry != NULL && fldRun_Dante.goal >= fldRun_Dante.now + 2) {
        routeEntry += fldRun_Dante.now * 4 + 8;
        fldMapIndexToPos(routeEntry[0], routeEntry[1], &targetX, &targetZ);

        position[0] = fldRun_Dante.x;
        position[1] = 0.0f;
        position[2] = fldRun_Dante.z;
        position[3] = 1.0f;
        target[0] = targetX;
        target[1] = 0.0f;
        target[2] = targetZ;
        target[3] = 1.0f;
        if (fldHitChkEnemy(position, target, hitPosition) == -1) {
            printf("PRUN_TIKAMITI!!!\n");
            fldRun_Dante.tikamiti = 1;
            fldMapIndexToPos(routeEntry[0], routeEntry[1], &targetX, &targetZ);
            fldGetDistAngleTo(&distance, &angle, targetX, targetZ);
            {
                s32 frames = (s32)(distance / fldRun_Dante.speed);
                fldRun_Dante.now++;
                fldRun_Dante.angleToPlayer = angle;
                fldRun_Dante.nextframe = frames;
            }
        }
    }

    if (fldRun_Dante.nextframe <= 0) {
        if (fldRun_Dante.now < fldRun_Dante.goal) {
            fldRun_Dante.now++;
            fldRun_Dante.nextframe = 30;
        }

        routeEntry = fldRun_Dante.RouteDataTop;
        if (routeEntry != NULL) {
            routeEntry += fldRun_Dante.now * 4;
            fldRun_Dante.ix = routeEntry[0];
            fldRun_Dante.iz = routeEntry[1];
            fldRun_Dante.x = originX + (f32)routeEntry[0] * (f32)fldRun_MAP_BX;
            fldRun_Dante.z = originZ + (f32)routeEntry[1] * (f32)-fldRun_MAP_BZ;
        }

        if ((s16)fldRunDanteStartX[0] == fldRun_Dante.ix &&
            (s16)fldRunDanteStartX[1] == fldRun_Dante.iz) {
            fldRun_Dante.nextframe = 20;
            fldRun_Dante.routeCell = NULL;
            fldRun_Dante.RouteDataTop = NULL;
            fldRun_Dante.now = 0;
            fldRun_Dante.goal = 0;
            /* Stop after resetting the route. Qualified cc1 sched1 traces
             * (hooked/unhooked output equal) give this final address use a
             * reg_weight of -1 versus 0 for the other reset stores, selecting
             * it first and preserving the native store order. */
            fldRun_Dante.speed = 0.0f;
        } else {
            fldRunAiCng(6);
        }
    } else {
        f32 angleForSin = fldRun_Dante.angleToPlayer;
        f32 sinValue = sdfSin(angleForSin * 3.14f / 180.0f);
        f32 speed = fldRun_Dante.speed;
        f32 angleForCos = fldRun_Dante.angleToPlayer;
        f32 x = fldRun_Dante.x;
        f32 z;
        f32 cosSpeed;
        f32 cosValue;

        fldRun_Dante.x = x + sinValue * speed;
        cosValue = sdfCos(angleForCos * 3.14f / 180.0f);
        cosSpeed = fldRun_Dante.speed;
        z = fldRun_Dante.z;
        fldRun_Dante.z = z + cosValue * cosSpeed;
        fldRun_Dante.turn = fldTurnToward(fldRun_Dante.turn, 180.0f - fldRun_Dante.angleToPlayer, 10.0f, 1.0f);
    }

    fldRunDanteGetDistAngle(&distance, &angle);
    if (distance < 3000.0f) {
        if (fldRun_Dante.bgm_mode != 1) {
            fldRun_Dante.bgm_mode = 1;
            fldRun_Dante.flag |= 4;
        }
    } else if (fldRun_Dante.bgm_mode != 0) {
        fldRun_Dante.bgm_mode = 0;
        fldRun_Dante.flag |= 4;
    }
}


extern f64 fptodp(f32 value);

void fldRunDantePwak(void)
{
    f32 originX = (f32)fldRun_MAP_OX;
    f32 originZ = (f32)fldRun_MAP_OZ;
    f32 targetX;
    f32 targetZ;
    f32 distance;
    f32 angle;
    f32 position[4];
    f32 target[4];
    f32 hitPosition[3];
    u8 *routeEntry;

    if (fldRun_Dante.cnt_ex < 300) {
        fldRun_Dante.cnt_ex++;
    }
    if (fldRun_Dante.nextframe > 0) {
        fldRun_Dante.nextframe--;
    }

    routeEntry = fldRun_Dante.RouteDataTop;
    if (routeEntry != NULL && fldRun_Dante.goal >= fldRun_Dante.now + 2) {
        routeEntry += fldRun_Dante.now * 4 + 8;
        fldMapIndexToPos(routeEntry[0], routeEntry[1], &targetX, &targetZ);

        position[0] = fldRun_Dante.x;
        position[1] = 0.0f;
        position[2] = fldRun_Dante.z;
        position[3] = 1.0f;
        target[0] = targetX;
        target[1] = 0.0f;
        target[2] = targetZ;
        target[3] = 1.0f;
        if (fldHitChkEnemy(position, target, hitPosition) == -1) {
            printf("TIKAMITI!!!\n");
            fldRun_Dante.tikamiti = 1;
            fldMapIndexToPos(routeEntry[0], routeEntry[1], &targetX, &targetZ);
            fldGetDistAngleTo(&distance, &angle, targetX, targetZ);
            {
                s32 frames = (s32)(distance / fldRun_Dante.speed);
                fldRun_Dante.now++;
                fldRun_Dante.angleToPlayer = angle;
                fldRun_Dante.nextframe = frames;
            }

            printf("d:%f,%f, path:%f,%f\n", fldRun_Dante.x, fldRun_Dante.z, targetX, targetZ);
            printf("path_i:%d,%d  d_i:%d,%d  dir:%f\n", routeEntry[0], routeEntry[1],
                   fldRun_Dante.ix, fldRun_Dante.iz, fldRun_Dante.angleToPlayer);
        }
    }

    if (fldRun_Dante.nextframe <= 0) {
        if (fldRun_Dante.now < fldRun_Dante.goal) {
            fldRun_Dante.now++;
            fldRun_Dante.nextframe = 120;
        }

        routeEntry = (u8 *)fldRun_Dante.RouteDataTop;
        if (routeEntry != NULL) {
            routeEntry += fldRun_Dante.now * 4;
            fldRun_Dante.ix = routeEntry[0];
            fldRun_Dante.iz = routeEntry[1];
            fldRun_Dante.x = originX + (f32)routeEntry[0] * (f32)fldRun_MAP_BX;
            fldRun_Dante.z = originZ + (f32)routeEntry[1] * (f32)-fldRun_MAP_BZ;
        }

        if ((s16)fldRunDanteStartX[0] == fldRun_Dante.ix &&
            (s16)fldRunDanteStartX[1] == fldRun_Dante.iz) {
            fldRun_Dante.nextframe = 20;
            fldRun_Dante.routeCell = NULL;
            fldRun_Dante.RouteDataTop = NULL;
            fldRun_Dante.now = 0;
            fldRun_Dante.goal = 0;
            fldRun_Dante.speed = 0.0f;
        } else {
            fldRunAiCng(5);
        }
    } else {
        f32 angleForSin = fldRun_Dante.angleToPlayer;
        f32 sinValue = sdfSin(angleForSin * 3.14f / 180.0f);
        f32 speed = fldRun_Dante.speed;
        f32 angleForCos = fldRun_Dante.angleToPlayer;
        f32 x = fldRun_Dante.x;
        f32 z;
        f32 cosSpeed;
        f32 cosValue;

        fldRun_Dante.x = x + sinValue * speed;
        cosValue = sdfCos(angleForCos * 3.14f / 180.0f);
        cosSpeed = fldRun_Dante.speed;
        z = fldRun_Dante.z;
        fldRun_Dante.z = z + cosValue * cosSpeed;
        fldRun_Dante.turn = fldTurnToward(fldRun_Dante.turn,
                                   180.0f - fldRun_Dante.angleToPlayer,
                                   15.0f, 1.0f);
    }

    fldRunDanteGetDistAngle(&distance, &angle);
    if (distance < 3000.0f) {
        if (fldRun_Dante.bgm_mode != 1) {
            fldRun_Dante.bgm_mode = 1;
            fldRun_Dante.flag |= 4;
        }
    } else if (fldRun_Dante.bgm_mode != 0) {
        fldRun_Dante.bgm_mode = 0;
        fldRun_Dante.flag |= 4;
    }
}


void fldRunDanteSoba(void)
{
    f32 dist;

    if (fldRun_Dante.nextframe > 0) {
        fldRun_Dante.nextframe--;
    }
    dist = fldRun_Dante.d_p_len;
    fldRun_Dante.angleToPlayer = fldRun_Dante.d_p_dir;
    fldRun_Dante.turn =
        fldTurnToward(fldRun_Dante.turn, 180.0f - fldRun_Dante.d_p_dir, 10.0f, 1.0f);
    if (dist > 100.0f) {
        fldRun_Dante.x += sdfSin(fldRun_Dante.angleToPlayer * 3.14f / 180.0f) * fldRun_Dante.speed;
        fldRun_Dante.z += sdfCos(fldRun_Dante.angleToPlayer * 3.14f / 180.0f) * fldRun_Dante.speed;
    }
    FroFtoI(fldRun_Dante.x, fldRun_Dante.z, &fldRun_Dante.ix, &fldRun_Dante.iz);
    if (dist < 3600.0f) {
        if (fldRun_Dante.bgm_mode != 1) {
            fldRun_Dante.bgm_mode = 1;
            fldRun_Dante.flag |= 4;
        }
    } else if (fldRun_Dante.bgm_mode != 0) {
        fldRun_Dante.bgm_mode = 0;
        fldRun_Dante.flag |= 4;
    }
}

/* NON_MATCHING: 178 of 180 words match, native 720-byte size; timer load order. */
#ifdef NON_MATCHING
void fldRunDantePatr(void)
{
    fldMapCell_t *cell;
    u8 *routeBase;
    u8 *routeEntry;
    f32 position[4];
    f32 target[4];
    f32 hitPosition[3];
    f32 targetX;
    f32 targetZ;
    f32 distance;
    f32 angle;

    if (fldRun_Dante.cnt_ex < 300) {
        fldRun_Dante.cnt_ex++;
    }
    if (fldRun_Dante.nextframe > 0) {
        fldRun_Dante.nextframe--;
    }

    FroFtoI(fldRun_Dante.x, fldRun_Dante.z, (u16 *)&fldRun_Dante.ix, (u16 *)&fldRun_Dante.iz);
    FroFtoI(fldGlobalBase.x, fldGlobalBase.z, &fldRunDanteStartX[0],
            &fldRunDanteStartX[1]);
    cell = fldFindMapCell(fldRun_Dante.ix, fldRun_Dante.iz,
                          (s16)fldRunDanteStartX[0],
                          (s16)fldRunDanteStartX[1]);
    if (cell != NULL) {
        printf("ReCalcOK\n");
        fldRun_Dante.routeCell = cell;
        fldRun_Dante.now = 0;
        fldRun_Dante.nextframe = 0;
        fldRun_Dante.goal = (s32)(cell->routeCount - 1);
        fldRun_Dante.targetIx = fldRunDanteStartX[0];
        fldRun_Dante.targetIz = fldRunDanteStartX[1];

        routeBase = (u8 *)fldRunMapHeader + cell->routeOffset;
        routeEntry = routeBase;
        if (cell->routeCount >= 2) {
            routeEntry += 4;
            fldMapIndexToPos(routeEntry[0], routeEntry[1], &targetX, &targetZ);
            position[0] = fldRun_Dante.x;
            position[1] = 0.0f;
            position[2] = fldRun_Dante.z;
            position[3] = 1.0f;
            target[0] = targetX;
            target[1] = 0.0f;
            target[2] = targetZ;
            target[3] = 1.0f;
            if (fldHitChkEnemy(position, target, hitPosition) != -1) {
                routeEntry = routeBase;
            } else {
                printf("Re1KO\n");
                fldRun_Dante.now++;
                fldRun_Dante.tikamiti = 1;
            }
        }

        fldMapIndexToPos(routeEntry[0], routeEntry[1], &targetX, &targetZ);
        fldGetDistAngleTo(&distance, &angle, targetX, targetZ);
        {
            f32 frames = distance / fldRun_Dante.speed;
            fldRun_Dante.angleToPlayer = angle;
            fldRun_Dante.nextframe = (s32)frames;
        }
    }

    fldRun_Dante.turn = fldTurnToward(fldRun_Dante.turn, 180.0f - fldRun_Dante.angleToPlayer,
                               15.0f, 1.0f);
    {
        f32 angleForSin = fldRun_Dante.angleToPlayer;
        f32 sinValue = sdfSin(angleForSin * 3.14f / 180.0f);
        f32 speed = fldRun_Dante.speed;
        f32 angleForCos = fldRun_Dante.angleToPlayer;
        f32 x = fldRun_Dante.x;
        f32 z;
        f32 cosSpeed;
        f32 cosValue;

        fldRun_Dante.x = x + sinValue * speed;
        cosValue = sdfCos(angleForCos * 3.14f / 180.0f);
        cosSpeed = fldRun_Dante.speed;
        z = fldRun_Dante.z;
        fldRun_Dante.z = z + cosValue * cosSpeed;
    }
    FroFtoI(fldRun_Dante.x, fldRun_Dante.z, (u16 *)&fldRun_Dante.ix, (u16 *)&fldRun_Dante.iz);

    if (fldRun_Dante.bgm_mode != 1) {
        fldRun_Dante.bgm_mode = 1;
        fldRun_Dante.flag |= 4;
    }
}

#else
INCLUDE_ASM(const s32, "field/fldTest", fldRunDantePatr);
#endif

extern fldDanteGunState_t fldRunDanteGunFlag;
extern dds3Basic_t *D_0054EAF0;
extern s32 fldPlayerStepDist;
extern u32 dds3GetRandA(void *name);
extern void datPartyDamageFld(s32 mode);
extern void fldRunEffSet(s32 id, f32 x, f32 y, f32 z, f32 r);
extern evtUnit_t *dds3GetUnitHandleObjectBasic(dds3Basic_t *basic);
extern s32 sdfModelGetMarkPos(sdfModelRootNode_t *root, s32 mark);

#ifdef NON_MATCHING
void fldRunDanteGun1(void)
{
    f32 frames[4] = { 2.0f, 10.0f, 19.0f, 28.0f };
    s32 marks[4] = { 20, 21, 20, 21 };
    sdfVector4_t pos __attribute__((aligned(16)));
    f32 distance;
    f32 oldframe;
    f32 nowframe;
    s32 i;

    if (fldRun_Dante.cnt_ex < 300) {
        fldRun_Dante.cnt_ex++;
    }
    if (fldRun_Dante.nextframe > 0) {
        fldRun_Dante.nextframe--;
    }
    fldRun_Dante.angleToPlayer = fldRun_Dante.d_p_dir;
    distance = fldRun_Dante.d_p_len;
    fldRun_Dante.turn = fldTurnToward(fldRun_Dante.turn, 180.0f - fldRun_Dante.d_p_dir, 10.0f, 1.0f);
    if (distance > 100.0f) {
        f32 pi = 3.1399999f;
        fldRun_Dante.x += sdfSin(fldRun_Dante.angleToPlayer * pi / 180.0f) * fldRun_Dante.speed;
        fldRun_Dante.z += sdfCos(fldRun_Dante.angleToPlayer * pi / 180.0f) * fldRun_Dante.speed;
    }
    if (fldRun_Dante.flag & 2) {
        oldframe = 0.0f;
        nowframe = 0.0f;
    } else {
        oldframe = fldRun_Dante.oldframe;
        nowframe = fldRun_Dante.nowframe;
    }
    for (i = 0; i < 4; i++) {
        if (oldframe < frames[i] && frames[i] <= nowframe) {
            s32 hit = 0;
            u32 roll;
            s32 mark;

            fldRun_Dante.gunlight_anime = 8;
            fldRun_Dante.flag |= 8;
            if (fldRunDanteGunFlag.counter != 0) {
                continue;
            }
            roll = (dds3GetRandA(NULL) >> 3) % 100;
            if (!(fldRun_Dante.flag & 1)) {
                if (distance > 2400.0f && roll < 2) {
                    hit = 1;
                } else if (distance > 1800.0f && roll < 5) {
                    hit = 1;
                } else if (distance > 1200.0f && roll < 20) {
                    hit = 1;
                } else if (distance > 600.0f && roll < 30) {
                    hit = 1;
                } else if (roll < 50) {
                    hit = 1;
                }
            }
            if (hit) {
                s32 offset;

                fldGlobalBase.word78 = 1;
                fldPlayerStepDist = 8;
                datPartyDamageFld(3);
                offset = (s32)((dds3GetRandA(NULL) >> 3) % 40) - 20;
                pos.x = fldGlobalBase.x + (f32)offset;
                offset = (s32)((dds3GetRandA(NULL) >> 3) % 40) - 20;
                pos.y = fldGlobalBase.y - 140.0f + (f32)offset;
                offset = (s32)((dds3GetRandA(NULL) >> 3) % 40) - 20;
                pos.z = fldGlobalBase.z + (f32)offset;
                fldRunEffSet(0, pos.x, pos.y, pos.z, 0.0f);
            }
            mark = marks[i];
            if (mark == 20) {
                fldRun_Dante.flag |= 0x10;
            }
            if (sdfModelGetMarkPos(dds3GetUnitHandleObjectBasic(D_0054EAF0)->owner->inner, mark) != 0) {
                SDF_VU0_STOREVECTOR(&pos);
                fldRunEffSet(1, pos.x, pos.y, pos.z, fldRun_Dante.turn);
            }
        }
    }
    FroFtoI(fldRun_Dante.x, fldRun_Dante.z, (u16 *)&fldRun_Dante.ix, (u16 *)&fldRun_Dante.iz);
    if (fldRun_Dante.bgm_mode != 1) {
        fldRun_Dante.bgm_mode = 1;
        fldRun_Dante.flag |= 4;
    }
}
#else
INCLUDE_ASM(const s32, "field/fldTest", fldRunDanteGun1);
#endif

#ifdef NON_MATCHING
void fldRunDanteGun2(void)
{
    f32 frames[6] = { 31.0f, 37.0f, 44.0f, 51.0f, 59.0f, 65.0f };
    s32 marks[6] = { 20, 21, 20, 21, 20, 21 };
    sdfVector4_t pos __attribute__((aligned(16)));
    f32 distance;
    f32 oldframe;
    f32 nowframe;
    s32 i;

    if (fldRun_Dante.cnt_ex < 300) {
        fldRun_Dante.cnt_ex++;
    }
    if (fldRun_Dante.nextframe > 0) {
        fldRun_Dante.nextframe--;
    }
    fldRun_Dante.angleToPlayer = fldRun_Dante.d_p_dir;
    distance = fldRun_Dante.d_p_len;
    fldRun_Dante.turn = fldTurnToward(fldRun_Dante.turn, 180.0f - fldRun_Dante.d_p_dir, 10.0f, 1.0f);
    if (fldRun_Dante.flag & 2) {
        oldframe = 0.0f;
        nowframe = 0.0f;
    } else {
        oldframe = fldRun_Dante.oldframe;
        nowframe = fldRun_Dante.nowframe;
    }
    for (i = 0; i < 6; i++) {
        if (oldframe < frames[i] && frames[i] <= nowframe) {
            s32 hit = 0;
            u32 roll;
            s32 mark;

            fldRun_Dante.gunlight_anime = 8;
            fldRun_Dante.flag |= 8;
            if (fldRunDanteGunFlag.counter != 0) {
                continue;
            }
            roll = (dds3GetRandA(NULL) >> 3) % 100;
            if (!(fldRun_Dante.flag & 1)) {
                if (distance > 2400.0f && roll < 2) {
                    hit = 1;
                } else if (distance > 1800.0f && roll < 5) {
                    hit = 1;
                } else if (distance > 1200.0f && roll < 20) {
                    hit = 1;
                } else if (distance > 600.0f && roll < 30) {
                    hit = 1;
                } else if (roll < 50) {
                    hit = 1;
                }
            }
            if (hit) {
                s32 offset;

                fldGlobalBase.word78 = 1;
                fldPlayerStepDist = 8;
                datPartyDamageFld(3);
                offset = (s32)((dds3GetRandA(NULL) >> 3) % 40) - 20;
                pos.x = fldGlobalBase.x + (f32)offset;
                offset = (s32)((dds3GetRandA(NULL) >> 3) % 40) - 20;
                pos.y = fldGlobalBase.y - 140.0f + (f32)offset;
                offset = (s32)((dds3GetRandA(NULL) >> 3) % 40) - 20;
                pos.z = fldGlobalBase.z + (f32)offset;
                fldRunEffSet(0, pos.x, pos.y, pos.z, 0.0f);
            }
            mark = marks[i];
            if (mark == 20) {
                fldRun_Dante.flag |= 0x10;
            }
            if (sdfModelGetMarkPos(dds3GetUnitHandleObjectBasic(D_0054EAF0)->owner->inner, mark) != 0) {
                SDF_VU0_STOREVECTOR(&pos);
                fldRunEffSet(1, pos.x, pos.y, pos.z, fldRun_Dante.turn);
            }
        }
    }
    FroFtoI(fldRun_Dante.x, fldRun_Dante.z, (u16 *)&fldRun_Dante.ix, (u16 *)&fldRun_Dante.iz);
    if (fldRun_Dante.bgm_mode != 1) {
        fldRun_Dante.bgm_mode = 1;
        fldRun_Dante.flag |= 4;
    }
}
#else
INCLUDE_ASM(const s32, "field/fldTest", fldRunDanteGun2);
#endif

#ifdef NON_MATCHING
extern sdfVector4_t aViewEye;
extern sdfVector4_t aViewTarget;

void fldRunDanteCollision(void)
{
    f32 position[4];
    f32 target[4];
    f32 hitPosition[4];
    f32 dx;
    f32 dz;
    f32 viewAngle = 0.0f;
    f32 difference;
    s32 hit;
    s32 status;

    fldRun_Dante.flag = 0;
    fldRunDanteGetDistAngle(&fldRun_Dante.d_p_len, &fldRun_Dante.d_p_dir);
    dz = aViewTarget.z - aViewEye.z;
    dx = aViewTarget.x - aViewEye.x;
    if (!(sdfSqrt(dx * dx + dz * dz) < 1.0f)) {
        viewAngle = sdfAtan2(dx, dz) * 180.0f / 3.1399999f;
    }
    difference = R360_LR(180.0f - viewAngle, fldRun_Dante.turn);
    if (difference < 0.0f) {
        difference = -difference;
    }
    fldRun_Dante.d_c_dir = difference;
    status = fldRun_Dante.status;
    if (status == 0) {
        return;
    }
    if (status == 1 && fldRun_Dante.nextframe > 0) {
        return;
    }
    if (status == 2) {
        if (fldRun_Dante.nextframe <= 0) {
            fldRunAiCng(9);
        }
        return;
    }
    if (status == 9) {
        if (fldRun_Dante.nextframe <= 0) {
            fldRunAiCng(6);
        }
        return;
    }
    position[0] = fldRun_Dante.x;
    position[1] = 0.0f;
    position[2] = fldRun_Dante.z;
    position[3] = 1.0f;
    target[0] = fldGlobalBase.x;
    target[1] = 0.0f;
    target[2] = fldGlobalBase.z;
    target[3] = 1.0f;
    hit = fldHitChkEnemy(position, target, hitPosition);
    if (hit != -1) {
        fldRun_Dante.flag |= 1;
    }
    if (fldRun_Dante.stance == 4) {
        if (fldRun_Dante.nextframe <= 0) {
            if ((fldRun_Dante.flag & 1) || (dds3GetRandA(NULL) >> 3) % 100 >= 30) {
                if (fldRun_Dante.status != 1) {
                    fldRunAiCng(1);
                }
            } else if (fldRun_Dante.status != 8) {
                fldRunAiCng(8);
            }
        }
        return;
    }
    if (fldRun_Dante.stance == 9) {
        if (fldRun_Dante.nextframe <= 0 && fldRun_Dante.status == 1) {
            if (!(fldRun_Dante.flag & 1) && fldRun_Dante.d_p_len < 900.0f) {
                fldRunAiCng(3);
            }
            return;
        }
        if (fldGlobalBase.floor == 7 && (u32)(fldRunDanteStartX[0] - 5) < 3 &&
            (s16)fldRunDanteStartX[1] < 4 && fldRun_Dante.iz >= 6) {
            fldRun_Dante.ix = 6;
            fldRun_Dante.iz = 8;
            fldRun_Dante.angleToPlayer = 180.0f;
            fldRun_Dante.turn = 0.0f;
            fldMapIndexToPos(6, 8, &fldRun_Dante.x, &fldRun_Dante.z);
            fldRunAiCng(1);
            fldRunDanteSetModel();
            if (fldRun_Dante.bgm_mode != 0) {
                fldRun_Dante.bgm_mode = 0;
                fldRun_Dante.flag |= 4;
            }
            return;
        }
    }
    if (hit == -1) {
        s32 choice;

        if (fldRun_Dante.status == 6 && fldRun_Dante.nextframe > 0 && fldRun_Dante.cnt_ex >= 6) {
            fldRun_Dante.nextframe = 0;
        }
        if (fldRun_Dante.status == 5) {
            fldRun_Dante.nextframe = 0;
        }
        if (fldRun_Dante.status == 4 && (fldRun_Dante.cnt_ex >= 3 || fldRun_Dante.nextframe < 3)) {
            fldRun_Dante.nextframe = 0;
        }
        if (fldRun_Dante.nextframe > 0) {
            return;
        }
        choice = (dds3GetRandA(NULL) >> 3) % 100;
        if (fldRun_Dante.stance == 7) {
            choice += 50;
        }
        if (fldRun_Dante.stance == 9) {
            choice += 50;
        }
        if (choice < 80) {
            if (fldRun_Dante.status != 3) {
                fldRunAiCng(3);
            }
        } else {
            if (fldRun_Dante.d_p_len < 900.0f) {
                choice = (dds3GetRandA(NULL) >> 3) % 100;
                if (fldRun_Dante.stance == 7 || fldRun_Dante.stance == 9) {
                    choice = 100;
                }
                if (choice < 80) {
                    if (fldRun_Dante.status != 8) {
                        fldRunAiCng(8);
                    }
                    return;
                }
            }
            if (fldRun_Dante.status != 7) {
                fldRunAiCng(7);
            }
        }
        return;
    }
    status = fldRun_Dante.status;
    if ((status == 7 && fldRun_Dante.cnt_ex < 15) ||
        (status == 8 && fldRun_Dante.cnt_ex < 30)) {
        return;
    }
    if (status == 4) {
        if (fldRun_Dante.nextframe <= 0) {
            fldRunAiCng(6);
        } else if ((s16)fldRun_Dante.targetIx != (s16)fldRunDanteStartX[0] ||
                   (s16)fldRun_Dante.targetIz != (s16)fldRunDanteStartX[1]) {
            fldRunAiCng(4);
        }
    } else if ((s16)fldRun_Dante.targetIx != (s16)fldRunDanteStartX[0] ||
               (s16)fldRun_Dante.targetIz != (s16)fldRunDanteStartX[1]) {
        if (status == 5 && fldRun_Dante.tikamiti == 0) {
            fldRunAiCng(5);
        } else if (status == 6 && fldRun_Dante.tikamiti == 0) {
            fldRunAiCng(6);
        } else {
            fldRunAiCng(4);
        }
    } else if (status != 5 && status != 6) {
        fldRunAiCng(4);
    }
}
#else
INCLUDE_ASM(const s32, "field/fldTest", fldRunDanteCollision);
#endif

INCLUDE_RODATA(const s32, "field/fldTest", D_00531318);

INCLUDE_RODATA(const s32, "field/fldTest", D_00531328);

INCLUDE_RODATA(const s32, "field/fldTest", D_00531338);

INCLUDE_RODATA(const s32, "field/fldTest", D_00531350);

void fldRunDanteAi(void)
{
    switch (fldRun_Dante.status) {
    case 0:
        func_0011C790();
        return;
    case 1:
        func_0011C798();
        return;
    case 2:
        func_0011C7B8();
        return;
    case 9:
        fldRunDanteDoin();
        return;
    case 5:
        fldRunDantePwak();
        return;
    case 3:
        fldRunDanteSoba();
        return;
    case 4:
        fldRunDantePatr();
        return;
    case 7:
        fldRunDanteGun1();
        return;
    case 8:
        fldRunDanteGun2();
        return;
    default:
        do {
            fldRunDanteMove();
        } while (0);
        break;
    }
}

void fldRunDanteLoadBgm(void)
{
    if (p2sdLoadCheckMidi(0x820000) == 0) {
        p2sdLoadSMGFile(0x820000);
    }
    while (p2sdLoadCheckMidi(0x820000) != 1) {
        sdfDrawSync();
    }
    if (p2sdLoadCheckMidi(0x830000) == 0) {
        p2sdLoadSMGFile(0x830000);
    }
    while (p2sdLoadCheckMidi(0x830000) != 1) {
        sdfDrawSync();
    }
    if (p2sdLoadCheckMidi(0x6F0000) == 0) {
        p2sdLoadSMGFile(0x6F0000);
    }
    while (p2sdLoadCheckMidi(0x6F0000) != 1) {
        sdfDrawSync();
    }
    p2sdStopBGM(0x820000);
}

void fldRunDanteSwapBgm(s32 toDante)
{
    if (toDante == 0) {
        p2sdFadeOutBgm(0x6B0001);
        p2sdStopBGM(0x820000);
    } else {
        p2sdFadeOutBgm(0x820000);
        p2sdStopBGM(0x6B0001);
    }
}

#ifdef NON_MATCHING
extern void evtBitOff(s32 no);
#endif

void fldRunDanteInit(void)
{
    fldMakeFroFileName();
    fldRunDanteLoadBgm();
    fldRunTamaInit();
    fldRun_MAP_BX = 600;
    fldRun_MAP_BZ = 600;
    switch (fldGlobalBase.floor) {
    case 3:
        fldRun_MAP_OX = 0xF6E;
        fldRun_MAP_OZ = 0xC1C;
        fldRun_Dante.ix = 3;
        fldRun_Dante.iz = 10;
        break;
    case 4:
        fldRun_MAP_OX = 0xC8;
        fldRun_MAP_OZ = 0x64;
        fldRun_Dante.ix = 7;
        fldRun_Dante.iz = 0;
        fldRunDanteStartX[0] = 3;
        fldRunDanteStartX[1] = 2;
        break;
    case 5:
        fldRun_MAP_OX = 0xCE4;
        fldRun_MAP_OZ = -0x834;
        fldRun_Dante.ix = 0;
        fldRun_Dante.iz = 0;
        break;
    case 7:
        fldRun_MAP_OX = -0x1482;
        fldRun_MAP_OZ = 0xFD2;
        fldRun_Dante.ix = 0;
        fldRun_Dante.iz = 0;
        break;
    case 8:
        fldRun_MAP_OX = -0xDAC;
        fldRun_MAP_OZ = -0xB54;
        fldRun_Dante.ix = 0;
        fldRun_Dante.iz = 0;
        break;
    case 9:
        fldRun_MAP_OX = 0x12C;
        fldRun_MAP_OZ = 0x834;
        fldRun_Dante.ix = 0;
        fldRun_Dante.iz = 0;
        break;
    case 10:
        fldRun_MAP_OX = -0xA8C;
        fldRun_MAP_OZ = -0xA8C;
        fldRun_Dante.ix = 0;
        fldRun_Dante.iz = 0;
        break;
    }
    fldRun_Dante.nextframe = -20;
    fldRun_Dante.wkmode = 0;
    fldRun_Dante.now = 0;
    fldRun_Dante.goal = 0;
    fldRun_Dante.y = 0.0f;
    fldRun_Dante.angleToPlayer = 0.0f;
    fldRun_Dante.speed = 0.0f;
    fldRun_Dante.gunlight_anime = 0;
    fldRun_Dante.flag = 0;
    fldRun_Dante.bgm_mode = 0;
    fldRun_Dante.stance = 0;
    fldRun_Dante.d_c_dir = 0.0f;
    fldRun_Dante.x = fldRun_MAP_OX + (f32)fldRun_MAP_BX * (f32)fldRun_Dante.ix;
    fldRun_Dante.z = fldRun_MAP_OZ + (f32)-fldRun_MAP_BZ * (f32)fldRun_Dante.iz;
    switch (fldGlobalBase.floor) {
    case 3:
        fldRunDanteStanceSet(6);
        evtBitOn(0xAF4);
        break;
    case 4:
        fldRunDanteStanceSet(2);
        evtBitOn(0xAF4);
        break;
    case 5:
        fldRunDanteStanceSet(3);
        evtBitOn(0xAF4);
        break;
    case 7:
    case 8:
    case 9:
    case 10:
        fldRunAiCng(0);
        evtBitOff(0xAF4);
        break;
    default:
        fldRunAiCng(2);
        evtBitOff(0xAF4);
        break;
    }
    fldRunDanteSetModel();
}

void fldRunDanteShutdown(void)
{
    fldFreeRunMapData();
    fldRunTamaFree();
    if (fldJDataHandle != 0) {
        fldRun_Dante.old_status = fldRun_Dante.status;
    } else {
        fldRunDanteHandle = -1;
    }
}

#ifdef NON_MATCHING
extern void fldDrawPoly3(s32 c0, s32 c1, s32 c2,
                         f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1,
                         f32 x2, f32 y2, f32 z2);
extern void fldDrawPoly4(s32 c0, f32 x0, f32 y0, f32 z0,
                         s32 c1, f32 x1, f32 y1, f32 z1,
                         s32 c2, f32 x2, f32 y2, f32 z2,
                         s32 c3, f32 x3, f32 y3, f32 z3);

void fldRunDanteDrawShadow(s32 alpha, f32 x, f32 y, f32 z, f32 radius)
{
    s32 i;
    u32 edgeColor;
    u32 centerColor;
    f32 innerRadius;

    if (alpha == 0) {
        return;
    }
    rsetOtOfs(0x2E);
    rsetAlpha(1);
    edgeColor = ((u32)alpha << 24) | 0x181818;
    centerColor = ((u32)alpha << 24) | 0x202020;
    innerRadius = radius / 3.0f;

    for (i = 0; i < 360; i += 10) {
        f32 angle = (f32)((i + 360) % 360) * 3.1399999f / 180.0f;
        f32 outerX = sdfSin(angle) * radius;
        f32 outerZ = sdfCos(angle) * radius;
        f32 prevAngle = (f32)((i + 350) % 360) * 3.1399999f / 180.0f;
        f32 prevOuterX = sdfSin(prevAngle) * radius;
        f32 prevOuterZ = sdfCos(prevAngle) * radius;
        f32 innerX = sdfSin(angle) * innerRadius;
        f32 innerZ = sdfCos(angle) * innerRadius;
        f32 prevInnerX = sdfSin(prevAngle) * innerRadius;
        f32 prevInnerZ = sdfCos(prevAngle) * innerRadius;
        f32 height = y - 2.0f;

        innerX = x + innerX;
        innerZ = z + innerZ;
        prevInnerX = x + prevInnerX;
        prevInnerZ = z + prevInnerZ;
        fldDrawPoly3(centerColor, edgeColor, edgeColor,
                     x + 0.0f, height, z + 0.0f,
                     innerX, height, innerZ, prevInnerX, height, prevInnerZ);
        fldDrawPoly4(edgeColor, innerX, height, innerZ,
                     0, x + outerX, height, z + outerZ,
                     0, x + prevOuterX, height, z + prevOuterZ,
                     edgeColor, prevInnerX, height, prevInnerZ);
    }
    rsetAlpha(0);
}
#else
INCLUDE_ASM(const s32, "field/fldTest", fldRunDanteDrawShadow);
#endif

void fldRunDanteFacePlayer(void)
{
    f32 dist;
    f32 angle;

    fldRunDanteGetDistAngle(&dist, &angle);
    fldRun_Dante.angleToPlayer = angle;
    fldRun_Dante.turn = 180.0f - angle;
}

#ifdef NON_MATCHING
extern void *memset(void *dst, int value, unsigned int size);
extern void FromRotation(f32 angle);
extern void dds3OnPoseSetFlag(dds3PoseSet_t *pose, u32 flag);
extern void dds3OffPoseSetFlag(dds3PoseSet_t *pose, u32 flag);
extern void mdlLoopMotion(mdlObj_t *handle, s32 group, s32 number);
extern void mdlSmoothLoopMotion(mdlObj_t *handle, s32 group, s32 number, f32 before, f32 after);
extern void dds3DrawUnitObjectShadow(s32 fade, f32 x, f32 y, f32 z, f32 radius);
extern void fldRunDanteDrawShadow(s32 alpha, f32 x, f32 y, f32 z, f32 radius);
extern void aDrawBoxF(s32 x, s32 y, s32 w, s32 h, u32 r, u32 g, u32 b, u32 a);

void fldRunDanteSetModel(void)
{
    mdlObj_t *model = dds3ObjectBaseGetModelHandle(D_0054EAF0);
    sdfVector4_t pos __attribute__((aligned(16)));
    sdfVector4_t previous __attribute__((aligned(16)));
    sdfVector4_t rotation __attribute__((aligned(16)));
    f32 distance;
    f32 angle;

    memset(&pos, 0, sizeof(pos));
    pos.w = 1.0f;
    memset(&previous, 0, sizeof(previous));
    previous.w = 1.0f;
    {
        sdfVector4_t axis __attribute__((aligned(16))) = { 0.0f, 1.0f, 0.0f, 1.0f };

        memset(&rotation, 0, sizeof(rotation));
        rotation.w = 1.0f;
        pos.x = fldRun_Dante.x;
        pos.y = 0.0f;
        pos.z = fldRun_Dante.z;
        dds3SetPosPoseSet(D_0054EAF0, &pos);
        SDF_VU0_LOADVECTOR(&axis);
        FromRotation(fldRun_Dante.turn * 3.1399999f / 180.0f);
        SDF_VU0_STOREVECTOR(&rotation);
    }
    dds3SetRotPoseSet(D_0054EAF0, &rotation);
    if (fldRun_Dante.flag & 0x40) {
        dds3PoseSet_t *pose;

        dds3OnPoseSetFlag(D_0054EAF0->poseset, 8);
        pose = D_0054EAF0->poseset;
        SDF_VU0_COPYVECTOR(&previous, &pose->pos2);
        if (sdfFabs(previous.x - pos.x) < 1.0f) {
            pose->pos2.x = pos.x;
        } else {
            pose->pos2.x = previous.x + (pos.x - previous.x) / 3.0f;
        }
        if (sdfFabs(previous.y - pos.y) < 1.0f) {
            D_0054EAF0->poseset->pos2.y = pos.y;
        } else {
            D_0054EAF0->poseset->pos2.y = previous.y + (pos.y - previous.y) / 3.0f;
        }
        if (sdfFabs(previous.z - pos.z) < 1.0f) {
            D_0054EAF0->poseset->pos2.z = pos.z;
        } else {
            D_0054EAF0->poseset->pos2.z = previous.z + (pos.z - previous.z) / 3.0f;
        }
        D_0054EAF0->poseset->pos2.w = 1.0f;
    } else {
        dds3OffPoseSetFlag(D_0054EAF0->poseset, 8);
    }
    if (fldRun_Dante.flag & 0x20) {
        mdlLoopMotion(model, 0, fldRun_Dante.update_flag);
    } else if (fldRun_Dante.flag & 2) {
        s32 motion = fldRun_Dante.update_flag;
        s32 change = 1;

        if (motion == 0 || (motion >= 2 && motion < 4)) {
            change = motion != model->current.h.motionIndex;
        }
        if (change) {
            model->first->speed = 1.0f;
            if (motion == 1) {
                mdlSmoothLoopMotion(model, 0, 1, 1.0f, 4.0f);
            } else if (motion == 4) {
                mdlSmoothLoopMotion(model, 0, 4, 3.0f, 4.0f);
            } else if (motion == 3) {
                mdlSmoothLoopMotion(model, 0, 3, 8.0f, 8.0f);
            } else {
                mdlSmoothLoopMotion(model, 0, motion, 6.0f, 6.0f);
            }
        }
    }
    {
        u32 flag = fldRun_Dante.flag;
        f32 frame = model->first->frame;
        f32 oldframe = fldRun_Dante.nowframe;

        fldRun_Dante.nowframe = frame;
        fldRun_Dante.oldframe = oldframe;
        if (flag & 2) {
            fldRun_Dante.oldframe = frame;
        }
    }
    if (sdfModelGetMarkPos(dds3GetUnitHandleObjectBasic(D_0054EAF0)->owner->inner, 0) != 0) {
        SDF_VU0_STOREVECTOR(&pos);
        dds3DrawUnitObjectShadow(0x80, pos.x, fldRun_Dante.y, pos.z, 70.0f);
    }
    if (fldRun_Dante.gunlight_anime > 0) {
        s32 count = fldRun_Dante.gunlight_anime;
        s32 alpha = (s32)((u32)count * 14U);
        s32 intensity = 16;

        if (count < 4) {
            alpha >>= 1;
        }
        fldRunDanteDrawShadow(alpha, fldRun_Dante.x, fldRun_Dante.y, fldRun_Dante.z, 700.0f);
        fldRun_Dante.gunlight_anime--;
        fldRunDanteGetDistAngle(&distance, &angle);
        if (distance < 1800.0f) {
            intensity = (s32)((1800.0f - distance) * 0.017777777f) + 16;
        }
        alpha = (s32)((u32)intensity * (u32)fldRun_Dante.gunlight_anime) / 8;
        rsetOtOfs(0x47);
        rsetAlpha(1);
        rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
        aDrawBoxF(0, 0, 0x200, 0xF4, 0x30, 0x30, 0x30, alpha);
        rsetAlpha(0);
    }
    if (fldRun_Dante.flag & 0x10) {
        f32 pi = 3.1399999f;

        pos.x = fldRun_Dante.x;
        pos.y = -10.0f;
        pos.z = fldRun_Dante.z;
        pos.x += sdfSin(fldRun_Dante.angleToPlayer * pi / 180.0f) * 100.0f;
        pos.z += sdfCos(fldRun_Dante.angleToPlayer * pi / 180.0f) * 100.0f;
        fldRunTamaSet(pos.x, pos.y, pos.z, fldRun_Dante.turn);
    }
}
#else
INCLUDE_ASM(const s32, "field/fldTest", fldRunDanteSetModel);
#endif

s32 fldRunDanteDistToTarget(f32 x, f32 y, f32 z)
{
    f32 dx;
    f32 dy;
    f32 dz;
    f32 dist;
    s32 v;

    dx = fldCamTarget.x - x;
    dy = fldCamTarget.y - y;
    dz = fldCamTarget.z - z;
    dist = sdfSqrt(dx * dx + dy * dy + dz * dz);
    dist -= 500.0f;
    if (dist > 5400.0f) {
        dist = 5400.0f;
    }
    if (dist < 0.0f) {
        dist = 0.0f;
    }
    v = ((5400 - (s32)dist) * 127) / 5400;
    if (v > 127) {
        v = 127;
    }
    if (v < 0) {
        v = 0;
    }
    return v;
}

#ifdef NON_MATCHING
extern u32 dds3GetRandA(void *name);
#endif

void fldRunDanteSe(void)
{
    s32 seFlag = 0;
    s32 walkFlag = 0;
    s32 vol;
    f32 prev;
    f32 now;

    now = fldRun_Dante.nowframe;
    prev = fldRun_Dante.oldframe;
    vol = fldRunDanteDistToTarget(fldRun_Dante.x, fldRun_Dante.y, fldRun_Dante.z);
    switch (fldRun_Dante.update_flag) {
    case 0:
        if (prev < 5.0f && 5.0f <= now) {
            seFlag = 1;
        }
        if (prev < 15.0f && 15.0f <= now) {
            seFlag = 1;
        }
        break;
    case 2:
        if (prev < 5.0f && 5.0f <= now) {
            seFlag = 1;
        }
        if (prev < 24.0f && 24.0f <= now) {
            seFlag = 1;
        }
        break;
    case 1:
        if (prev < 3.0f && 3.0f <= now) {
            seFlag = 1;
        }
        if (prev < 18.0f && 18.0f <= now) {
            seFlag = 1;
        }
        if (fldRun_Dante.flag & 2) {
            walkFlag = 1;
        }
        break;
    case 4:
        if (prev < 17.0f && 17.0f <= now) {
            walkFlag = 1;
        }
        break;
    }
    if (seFlag != 0) {
        p2sdPlaySE(0x6F0001 + fldRunDanteSe_asise, vol, 0x3F);
        fldRunDanteSe_asise =
            (fldRunDanteSe_asise + (s32)(((u32)dds3GetRandA(0) >> 4) & 1) + 1) % 4;
    }
    if (walkFlag != 0) {
        p2sdPlaySE(0x6F000A, vol * 30 / 100, 0x3F);
    }
    if (fldRun_Dante.flag & 8) {
        p2sdPlaySE(0x6F0005, vol * 30 / 100, 0x3F);
    }
    if (fldRun_Dante.flag & 4) {
        fldRunDanteSwapBgm(fldRun_Dante.bgm_mode);
    }
}

void fldRunView(void)
{
    if (evtBitCheck(0xAF3) == 0) {
        return;
    }
    if (evtBitCheck(0xAF4) == 0) {
        return;
    }
    if (fldEncountTestFlag(0x40) == 0) {
        return;
    }
    fldRunDanteCollision();
    fldRunDanteAi();
    fldRunDanteSetModel();
    fldRunDanteSe();
    fldRunEffTask();
    fldRunTamaSe();
    FroFtoI(fldGlobalBase.x, fldGlobalBase.z, &fldRunDanteStartX[0], &fldRunDanteStartX[1]);
    fldRun_Dante.mapIxOld = fldRun_Dante.ix;
    fldRun_Dante.mapIzOld = fldRun_Dante.iz;
}

s32 fldRunEncountChk(void)
{
    f32 dist;
    f32 angle;

    if (evtBitCheck(0xAF3) == 0) {
        return 0;
    }
    if (fldRunAiTimer & 1) {
        return 0;
    }
    fldRunDanteGetDistAngle(&dist, &angle);
    if (dist < 250.0f) {
        return 1;
    }
    return 0;
}

#ifdef NON_MATCHING
extern fldDanteDoor_t D_002FBF00, D_002FBF10, D_002FBF20;
extern fldDanteDoor_t D_002FBF40, D_002FBF50, D_002FBF60;
extern char D_0054EAE0[];
extern s32 fldGetRestoreMode(void);

static inline void fldRunDanteDoorPosition(void)
{
    if (fldRun_Dante.pDoor != NULL) {
        fldRun_Dante.ix = fldRun_Dante.pDoor->ix;
        fldRun_Dante.angleToPlayer = fldRun_Dante.pDoor->angle;
        fldRun_Dante.iz = fldRun_Dante.pDoor->iz;
    }
    fldRun_Dante.turn = 180.0f - fldRun_Dante.angleToPlayer;
    fldMapIndexToPos(fldRun_Dante.ix, fldRun_Dante.iz, &fldRun_Dante.x, &fldRun_Dante.z);
    fldRunAiCng(2);
    fldRunDanteSetModel();
}

static inline void fldRunDanteSpecialDoor(void)
{
    fldRun_Dante.angleToPlayer = D_002FBF00.angle;
    fldRun_Dante.ix = D_002FBF00.ix;
    fldRun_Dante.iz = D_002FBF00.iz;
    fldRun_Dante.turn = 180.0f - D_002FBF00.angle;
    fldRun_Dante.pDoor = NULL;
    fldMapIndexToPos(fldRun_Dante.ix, fldRun_Dante.iz, &fldRun_Dante.x, &fldRun_Dante.z);
    fldRun_Dante.flag |= 0x20;
    fldRunAiCng(1);
    fldRunDanteSetModel();
    fldRun_Dante.stance = 9;
}

void fldRunDanteStanceSet(s32 stance)
{
    sdfVector4_t position;
    s32 restore;
    s32 mode;

    memset(&position, 0, sizeof(position));
    position.w = 1.0f;
    printf("DanteStanceSet( %d )\n", stance);
    fldRun_Dante.flag = 0x40;
    fldRun_Dante.stance = stance;

    switch (stance) {
    case 1:
    case 4:
    case 5:
    case 7:
    case 104:
        dds3GetPosPoseSet(D_0054EAF0);
        SDF_VU0_STOREVECTOR(&position);
        fldRun_Dante.x = position.x;
        fldRun_Dante.z = position.z;
        FroFtoI(position.x, position.z, (u16 *)&fldRun_Dante.ix, (u16 *)&fldRun_Dante.iz);
        if (stance == 5) {
            FroFtoI(fldGlobalBase.x, fldGlobalBase.z, &fldRunDanteStartX[0], &fldRunDanteStartX[1]);
        }
        mode = stance == 1 || stance == 5 ? 6 : stance == 7 ? 5 : 1;
        fldRunAiCng(mode);
        fldRunDanteFacePlayer();
        fldRunDanteSetModel();
        break;
    case 2:
        fldRunDanteFacePlayer();
        printf("fldGetRestoreMode:%d  old_area:%d\n", 0, fldGlobalBase.previousFloor + 1);
        if (fldGlobalBase.previousFloor == 4 && fldRun_Dante.old_status != 1) {
            fldRun_Dante.ix = fldRun_Dante.mapIxOld;
            fldRun_Dante.iz = fldRun_Dante.mapIzOld;
            fldMapIndexToPos(fldRun_Dante.ix, fldRun_Dante.iz, &fldRun_Dante.x, &fldRun_Dante.z);
            fldRunAiCng(6);
            fldRunDanteSetModel();
            printf(D_0054EAE0);
            break;
        }
        if (evtBitCheck(0x7B1) && evtBitCheck(0x7B2)) {
            fldRunDanteSpecialDoor();
        } else {
            if (fldGlobalBase.previousFloor == 3 || !(fldGlobalBase.z < -4000.0f)) {
                fldRun_Dante.pDoor = &D_002FBF20;
            } else {
                fldRun_Dante.pDoor = &D_002FBF10;
            }
            fldRunDanteDoorPosition();
        }
        break;
    case 3:
        restore = fldGetRestoreMode();
        printf("fldGetRestoreMode:%d  old_area:%d\n", restore, fldGlobalBase.previousFloor + 1);
        fldRunDanteFacePlayer();
        if (restore == 1) {
            fldRunAiCng(2);
            fldRunDanteSetModel();
            break;
        }
        fldRun_Dante.pDoor = fldGlobalBase.z < -4000.0f ? &D_002FBF40 : &D_002FBF50;
        fldRunDanteDoorPosition();
        break;
    case 6:
        restore = fldGetRestoreMode();
        fldRunDanteFacePlayer();
        printf("fldGetRestoreMode:%d  old_area:%d\n", restore, fldGlobalBase.previousFloor + 1);
        if (restore == 1) {
            fldRunAiCng(2);
            fldRunDanteSetModel();
            break;
        }
        fldRun_Dante.pDoor = &D_002FBF60;
        fldRunDanteDoorPosition();
        break;
    case 9:
        if (fldGlobalBase.previousFloor == 7 && fldRun_Dante.old_status != 1) {
            fldRun_Dante.ix = fldRun_Dante.mapIxOld;
            fldRun_Dante.iz = fldRun_Dante.mapIzOld;
            fldMapIndexToPos(fldRun_Dante.ix, fldRun_Dante.iz, &fldRun_Dante.x, &fldRun_Dante.z);
            fldRunAiCng(6);
        } else {
            dds3GetPosPoseSet(D_0054EAF0);
            SDF_VU0_STOREVECTOR(&position);
            fldRun_Dante.z = position.z;
            fldRun_Dante.x = position.x;
            FroFtoI(position.x, position.z, (u16 *)&fldRun_Dante.ix, (u16 *)&fldRun_Dante.iz);
            fldRun_Dante.turn = 0.0f;
            fldRun_Dante.angleToPlayer = 180.0f;
            fldRunAiCng(1);
        }
        fldRunDanteSetModel();
        break;
    case 10:
        fldRunDanteSpecialDoor();
        break;
    case 11:
    case 12:
        fldRun_Dante.pDoor = &D_002FBF20;
        fldRun_Dante.ix = D_002FBF20.ix;
        fldRun_Dante.iz = D_002FBF20.iz;
        fldRun_Dante.turn = 180.0f - D_002FBF20.angle;
        fldRun_Dante.angleToPlayer = D_002FBF20.angle;
        fldMapIndexToPos(fldRun_Dante.ix, fldRun_Dante.iz, &fldRun_Dante.x, &fldRun_Dante.z);
        fldRunAiCng(2);
        if (stance == 11) {
            evtBitOff(0xAF4);
        }
        fldRunDanteSetModel();
        break;
    }
}
#else
INCLUDE_RODATA(const s32, "field/fldTest", D_005313E0);

INCLUDE_ASM(const s32, "field/fldTest", fldRunDanteStanceSet);
#endif
INCLUDE_SDATA(const s32, "field/fldTest", gFldElvFloorNow);

INCLUDE_SDATA(const s32, "field/fldTest", fldElvFloorFrom);

INCLUDE_SDATA(const s32, "field/fldTest", fldElvFloorNum);

INCLUDE_SDATA(const s32, "field/fldTest", fldElvFloorTo);

INCLUDE_SDATA(const s32, "field/fldTest", fldElvFloorDisp);

INCLUDE_SDATA(const s32, "field/fldTest", fldElvFloorTarget);

INCLUDE_SDATA(const s32, "field/fldTest", fldElvState);

INCLUDE_SDATA(const s32, "field/fldTest", fldShadowWork);

INCLUDE_SDATA(const s32, "field/fldTest", D_0054EA8C);

INCLUDE_SDATA(const s32, "field/fldTest", D_0054EA90);

INCLUDE_SDATA(const s32, "field/fldTest", fldRun_MAP_OX);

INCLUDE_SDATA(const s32, "field/fldTest", fldRun_MAP_OZ);

INCLUDE_SDATA(const s32, "field/fldTest", fldRun_MAP_BX);

INCLUDE_SDATA(const s32, "field/fldTest", fldRun_MAP_BZ);

INCLUDE_SDATA(const s32, "field/fldTest", fldRunMapData);

INCLUDE_SDATA(const s32, "field/fldTest", fldRunMapHeader);

INCLUDE_SDATA(const s32, "field/fldTest", fldRunMapCellData);

INCLUDE_SDATA(const s32, "field/fldTest", pFroMapHead);

INCLUDE_SDATA(const s32, "field/fldTest", fldRunMapScale);

INCLUDE_SDATA(const s32, "field/fldTest", D_0054EAB0);

INCLUDE_SDATA(const s32, "field/fldTest", fldRunTama2Basic);

INCLUDE_SDATA(const s32, "field/fldTest", fldRunTama3Basic);

INCLUDE_SDATA(const s32, "field/fldTest", D_0054EABC);

INCLUDE_SDATA(const s32, "field/fldTest", D_0054EAC0);

INCLUDE_SDATA(const s32, "field/fldTest", D_0054EAC8);

INCLUDE_SDATA(const s32, "field/fldTest", D_0054EAD0);

INCLUDE_SDATA(const s32, "field/fldTest", fldRunDanteSe_asise);

INCLUDE_SDATA(const s32, "field/fldTest", D_0054EADC);

INCLUDE_SDATA(const s32, "field/fldTest", D_0054EAE0);

INCLUDE_SDATA(const s32, "field/fldTest", D_0054EAE8);

INCLUDE_SDATA(const s32, "field/fldTest", fldPlayerUnit);

INCLUDE_SDATA(const s32, "field/fldTest", D_0054EAF0);

