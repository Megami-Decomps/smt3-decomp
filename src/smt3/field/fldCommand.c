#include "common.h"
#include "basic.h"
#include "model.h"
#include "newdata.h"
#include "scr.h"
#include "field.h"
#include "ps2asm.h"

typedef struct
{
    f32 x;
    f32 y;
    f32 z;
} fldMapPoint_t;

typedef struct
{
    char *name;
    u32 nodeIndex;
    void *icons;
    u32 iconCount;
    s32 floor;
    fldMapPoint_t *pointA;
    fldMapPoint_t *pointB;
} fldMapItem_t;

typedef struct
{
    char *name;
    fldMapItem_t *items;
    u32 count;
    void *model;
    fldMapPoint_t *pos;
} fldMapRoom_t;

typedef struct
{
    s32 gunFlag;
    u8 unk04[0x28];
    s32 skyTable;              /* 0x2C */
    u8 unk30[4];
    s32 filter;                /* 0x34 */
} fldEnvironmentWork_t;

typedef struct
{
    void *resourceState;
    u32 roomNumber;
} fldMatterWork_t;

typedef struct
{
    s32 unk00;
    s32 value;
} fldDanteParam_t;

typedef struct fldUnit_t fldUnit_t;

typedef struct fldInputPanel_t fldInputPanel_t;

enum sl_slot_flag
{
    SL_SLOT_FLAG_CAN_READ_SAVE = 1,
    SL_SLOT_FLAG_DIRECTORY_READY = 2,
    SL_SLOT_FLAG_DELETE_BEFORE_WRITE = 8
};

extern s32 gFldSe3DNum;

extern s32 D_002FC020[];

extern u32 fldSeLastBgm;

extern s32 D_0054EE3C;

extern s32 fldBgmReloadStep;

extern s32 MidiNo;

extern s32 fldBgmLoadStep;

extern s32 fldBgmSmgAddr;

extern sdfMemHandle_t *fldAutoMapBuf;

extern fldMapRoom_t *fldAutoMapHeader;

extern s32 fldAutoMapData;

extern u32 AutoMapSeq;

extern s32 fldAutoMapField;

extern s32 fldAutoMapBBox;

extern s32 fldTakaraCount;

extern s32 fldGb[];

extern sdfTexHandle_t *fldUiTexture;

extern s32 fldNpcModelTable;

extern sdfTexHandle_t *fldTitleMiniTex;

extern sdfTexHandle_t *fldTitleMiniWork;

extern s32 fldSnowActive;

extern s32 slProcShutDownFlg;

extern s32 gSlWindowCnt;

extern s32 gslInfoCode;

extern s32 slChooseSaveIndex;

extern s32 slExitWaitCnt;

extern s32 gslMode;

extern u32 gslErrorCode;

extern u32 slQueryYesNoMsg;

extern u32 gslDrawFlagFile;

extern s32 gslCheckCardWait;

extern u32 gSlNowCardCheck;

extern u32 gslDrawFlagBg;

extern u32 baseIconFileHandle;

extern u32 gslErrorKeyFlag;

extern s32 slFileSelError;

extern u32 gSlEndingMode;

extern s32 gslAnimCnt;

extern u32 gslExitType;

extern s32 slQueryYesNoCursor;

extern s32 slQueryYesNoKeyFlag;

extern u32 gSlExitMode;

extern s32 scrGetIntPara(s32);

fldInputPanel_t *fldInputPanelSetMessage(s32 choice);

fldInputPanel_t *fldInputPanelSetDraw(s32 isPending);

extern void fldReleaseUnitPad(void);

dds3Basic_t *dds3GetActiveScene(void);

dds3Basic_t *dds3GetActiveUnitSceneBasic(dds3Basic_t *world);

extern void fldPlayerMotReset(void);

extern s32 dds3ObjectBaseSetPrimaryControl(dds3Basic_t *object, void *update);

extern void fldRegistUnitPad(void);

extern s32 fldCamMode[];

dds3Basic_t *fldGetUnitFollowCamera(void);

dds3Basic_t *dds3SetActiveCameraSceneBasic(dds3Basic_t *world, dds3Basic_t *camera);

extern void dds3CameraObjectBasicGetTgtCam(dds3Basic_t *camera, f32 *worldEyeOut,
                                                       f32 *targetPositionOut);

extern void fldCamGetPhokanParam(void);

extern f32 fldCamPhokanSelX[];

extern f32 D_002FC9F0[];

extern s32 fldCamPhokanState;

extern s32 fldCamPHokanMode;

dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *world, u32 key, s32 kind);

char *scrGetStrPara(s32 idx);

dds3Basic_t *dds3GetControlScene(void);

dds3Basic_t *dds3SceneBasicSearchName(dds3Basic_t *world, const u8 *name);

s32 printf(const char *format, ...);

extern char D_0054F0A8[];

extern char *strstr(const char *, const char *);

extern s32 fldCamUnit[];

extern fldEnvironmentWork_t fldRunDanteGunFlag;

extern void fldSkySetFilter(s32 area, s32 value, s32 duration);

extern void fldSetFilterAnime(s32);

extern void fldSetModelFlag(s32 area, s32 slotIndex, s32 bit, s32 enabled);

dds3BasicArrayHandle_t *dds3SceneBasicGetList(dds3Basic_t *object, s32 kind);

u16 dds3GetBasicArrayCount(const dds3BasicArrayHandle_t *object);

u32 dds3SetFirstBasicArray(dds3BasicArrayHandle_t *object);

u32 dds3GetBasicArray(const dds3BasicArrayHandle_t *object);

void dds3MatterObjectBasicSetAlpMode(dds3Basic_t *object, u32 value);

u32 dds3SetNextBasicArray(dds3BasicArrayHandle_t *object);

void dds3FreeBasicArrayHandle(dds3BasicArrayHandle_t *node);

extern void fldSetAnimeFlag(s32 area, s32 slotIndex, s32 bit, s32 enabled);

void dds3ObjectBaseMatterAnimeSpeed(dds3Basic_t *object, u32 requestedMode);

extern s32 fldArea[];

extern void fldSetNpcFlag(s32 world, s32 stage, s32 room, s32 value);

extern void fldGbSetGimic(s32 world, s32 stage, s32 index, s32 value);

extern void fldSetAtariFlag(s32 area, s32 slotIndex, s32 bit, s32 enabled);

extern s32 fldSetModelFlag2(s32 area, s32 slotIndex, s32 bit, s32 enabled);

extern s32 fldSetAnimeFlag2(s32 area, s32 slotIndex, s32 bit, s32 enabled);

extern void fldGetAnimeId2(s32 area, s32 slotIndex, s32 valueIndex, s32 value);

extern void fldAutoMapOpenBlock2(s32 area, s32 floor, s32 flagNumber);

extern void fldAutoMapCloseBlock2(s32 area, s32 floor, s32 flagNumber);

u32 fldGetRestoreMode(void);

scrProcessWork_s *scrGetScriptHandle(void);

extern s32 fldFindEveRngIndex(u32);

extern s32 fldDoor_GetHasigoTbl(s32);

extern s32 fldDoor_GetElvTbl(s32);

extern s32 fldElvCheckEnd(void);

extern s32 fldWapGetDoor(s32 value);

extern void scrSetIntReturnValue(s32 value);

extern s32 fldDoor_ChkWarp(void);

extern s32 fldInfNext(s32 param0, s32 param1);

extern s32 fldInfGet(s32 actorId, s32 attribute);

extern s32 fldInfChkFastPanel(s32 index);

extern void fldInfToWAP(void);

void fldBgmLoad(void);

extern void fldSePlay(s32 soundId);

extern void fldSeStop(s32 soundId);

extern void fldMotionSePlay(s32 group, s32 sound);

extern s32 fldEncountRate[];

extern const char *fldEveHitResName_Find(u32 key);

extern void fldDoor_ExitCamera(const char *entry);

extern s32 fldDoor_GetWarpEff(void);

extern fldUnit_t *evtGetUnitHandle(s32 param);

extern void fldNpcLightSet(fldUnit_t *unit);

void fldAnahoriSelectInit(void);

extern u32 fldAnahoriSelectGet(void);

void fldAnahoriSelectNext(void);

void fldAnahoriSelectDel(void);

extern fldDanteParam_t *D_0054EAF0;

extern void fldRunDanteStanceSet(s32);

extern s32 (*proc_func_ans)(dds3ProcessID_t *process);

extern s32 fldInputPanelCheckKey(void);

extern void fldEncountSetFlag(s32 flags);

extern void fldEncountClearFlag(s32 flags);

extern char *strcpy(char *dst, const char *src);

extern void fldSkySetTbl(s32 table, s32 durationMode);

extern s32 fldGetTakaraOpen(s32 room);

extern void fldSetTakaraOpen(s32 room);

extern void fldDoor_ElvExit(s32 selection);

extern void fldElvSetFloor(s32 first, s32 second);

extern void fldElvReset(void);

extern s32 fldEveHitResID_Find(u32 process);

extern void fldDoorRun_FindStart(s32 scene);

extern void fldBgmContinue(void);

extern void fldMotionSeStop(s32 group, s32 sound);

extern void fldTitleStart(s32 field, s32 option);

extern f32 D_0054EEA0;

extern void evtBitOn(s32 bit);

extern void evtBitOff(s32 bit);

typedef struct
{
    f32 x;
    f32 y;
    f32 z;
    s16 flg;
    u8 spdx;
    u8 spdy;
    s16 life;
    s16 alp;
} fldSeedWork_t;

extern fldSeedWork_t *gFldSeedBuff;

extern sdfMemHandle_t *fldSnowSeedBuf;

extern s32 gFldKyouseiTalk;

extern void fldFreeNextFile(void);

extern void fldDoor_WarpEx(s32 index);

extern char D_00532AA0[];

extern s32 fldNpcModelFreeIdx;

extern dds3Basic_t *fldNpcFreeCount;

extern s32 fldNpcCamTarget;

extern s32 slDDS3To31Flg;

extern s32 version_convert_ver;

extern u32 D_0054F06C;

extern u32 D_0054F070;

extern char D_00532A88[];

extern void fldDoor_Warp(const char *entry);

extern sdfTexHandle_t *fldEATexture;

extern sdfTexHandle_t *fldVLTexture;

extern sdfTexHandle_t *fldVLBlurHandle;

extern sdfTexHandle_t *fldSnowBlurHandle;

extern void *D_0054EED0;

extern void *D_0054EEDC;

extern sdfTexHandle_t *D_0054EF98;

extern sdfTexHandle_t *D_0054EF9C;

extern sdfTexHandle_t *D_0054EFA0;

extern sdfTexHandle_t *D_0054EFA4;

extern s32 gFldBgmBit;

extern void *fldDmgBlurHandle;

extern void *fldNpcBlurHandle;

extern void fldBgmPlay(void);

extern s32 fldTakaraSlotCount;

extern s32 fldNpcLoadIdx;

extern f32 slNextPageDraw_fc;

extern sdfTexHandle_t *slProcPtr;

extern s32 evtGetAgeOfMoon(void);

extern s32 dds3GetRandIntA(s32 low, s32 high);

extern s32 D_00316EC8[];

extern char D_00532AC8[];

extern char D_00532980[];

extern void resetCamTgtPlayer(void);

extern s32 gSlSelCnt;

extern s32 gFldSnowLife;

extern s32 gFldSnowMode;

extern f32 gFldSnowX;

extern f32 gFldSnowY;

extern f32 gFldSnowZ;

extern f32 gFldSnowYD;

extern f32 gFldSnowSpd;

extern f32 gFldSnowW;

extern s32 fldSeSmgId;

extern s32 fldEveHit_ChkDir(dds3Basic_t *unit, dds3Basic_t *hit);

extern s32 fldEveHit_ChkDir2(dds3Basic_t *unit, dds3Basic_t *hit);

typedef struct
{
    s32 type;
    s16 id;
    s16 num;
    s32 unk08;
    s32 unk0C;
} fldTakaraEntry_t;

extern sdfMemHandle_t *fldNpcModelCount;

extern sdfMemHandle_t *D_0054EF1C;

extern sdfMemHandle_t *D_0054EF20;

extern sdfMemHandle_t *D_0054EF24;

extern sdfTexHandle_t *hEffPart;

extern void *fldItemBoxCount;

extern fldTakaraEntry_t fldTakaraTable[];

extern void fldLoadNextFile(s32 a, s32 b);

extern void fldDoor_WarpCash(const char *name);

extern char D_00532A58[];


void fldBgmFadeOut(void);

void fldBgmStop(void);

void fldNpcHojiRefresh(void);

void fldPauseBgm(void);

s32 fldWaitBgmReload(void);

s32 fldCommand_ACT_WIN_DSP(void) {
    dds3Basic_t *scene = dds3GetActiveScene();
    dds3Basic_t *unit = dds3GetActiveUnitSceneBasic(scene);
    dds3Basic_t *hit;

    if (unit == NULL) {
        scrSetIntReturnValue(0);
        return 1;
    }
    hit = dds3SearchSceneBasic(
        scene, fldEveHitResID_Find((u32)scrGetScriptHandle()->ProcessID), 0x11);
    if (hit == NULL) {
        scrSetIntReturnValue(0);
        return 1;
    }
    if (fldGb[0] >= 0xB && fldEveHit_ChkDir(unit, hit) == 0) {
        scrSetIntReturnValue(0);
        return 1;
    }
    fldInputPanelSetMessage(scrGetIntPara(0));
    fldInputPanelSetDraw(1);
    switch (fldInputPanelCheckKey()) {
    case 1:
        scrSetIntReturnValue(1);
        return 1;
    case -1:
        scrSetIntReturnValue(-1);
        return 1;
    }
    scrSetIntReturnValue(0);
    return 1;
}

s32 fldCommand_ACT_WIN_DSP2(void) {
    dds3Basic_t *scene = dds3GetActiveScene();
    dds3Basic_t *unit = dds3GetActiveUnitSceneBasic(scene);
    dds3Basic_t *hit;

    if (unit == NULL) {
        scrSetIntReturnValue(0);
        return 1;
    }
    hit = dds3SearchSceneBasic(
        scene, fldEveHitResID_Find((u32)scrGetScriptHandle()->ProcessID), 0x11);
    if (hit == NULL) {
        scrSetIntReturnValue(0);
        return 1;
    }
    if (fldGb[0] >= 0xB && fldEveHit_ChkDir2(unit, hit) == 0) {
        scrSetIntReturnValue(0);
        return 1;
    }
    fldInputPanelSetMessage(scrGetIntPara(0));
    fldInputPanelSetDraw(1);
    switch (fldInputPanelCheckKey()) {
    case 1:
        scrSetIntReturnValue(1);
        return 1;
    case -1:
        scrSetIntReturnValue(-1);
        return 1;
    }
    scrSetIntReturnValue(0);
    return 1;
}

s32 fldCommand_ACT_WIN_DSP3(void) {
    s32 result;

    fldInputPanelSetMessage(scrGetIntPara(0));
    fldInputPanelSetDraw(1);
    result = fldInputPanelCheckKey();
    if (result == -1) {
        scrSetIntReturnValue(-1);
        return 1;
    }
    if (result == 1) {
        scrSetIntReturnValue(1);
        return 1;
    }
    scrSetIntReturnValue(0);
    return 1;
}


/* NON_MATCHING: 32 of 42 words, exact size; the first exit block is placed last. */
#ifdef NON_MATCHING
s32 fldCommand_DIR_HIT_CNTR(void) {
    dds3Basic_t *scene;
    dds3Basic_t *unit;
    dds3Basic_t *hit;

    if (fldGb[0] < 0xA) {
        scrSetIntReturnValue(1);
        return 1;
    }
    scene = dds3GetActiveScene();
    unit = dds3GetActiveUnitSceneBasic(scene);
    if (unit == NULL) {
        scrSetIntReturnValue(0);
        return 1;
    }
    hit = dds3SearchSceneBasic(
        scene, fldEveHitResID_Find((u32)scrGetScriptHandle()->ProcessID), 0x11);
    if (hit == NULL) {
        scrSetIntReturnValue(0);
        return 1;
    }
    if (fldEveHit_ChkDir(unit, hit) == 0) {
        scrSetIntReturnValue(0);
        return 1;
    }
    scrSetIntReturnValue(1);
    return 1;
}
#else
INCLUDE_ASM(const s32, "field/fldCommand", fldCommand_DIR_HIT_CNTR);
#endif


s32 fldCommand_PAD_DISABLE(void) {
    dds3Basic_t *object;

    fldReleaseUnitPad();
    object = dds3GetActiveUnitSceneBasic((dds3Basic_t *)dds3GetActiveScene());
    if (object == 0) {
        return 1;
    }
    fldPlayerMotReset();
    return 1;
}

s32 fldCommand_PAD_ENABLE(void) {
    dds3Basic_t *object;

    object = dds3GetActiveUnitSceneBasic((dds3Basic_t *)dds3GetActiveScene());
    if (object == 0) {
        return 1;
    }
    dds3ObjectBaseSetPrimaryControl(object, 0);
    fldPlayerMotReset();
    fldRegistUnitPad();
    return 1;
}

u32 fldCommand_ENCOUNT_ON(void) {
    fldEncountSetFlag(0x10);
    return 1;
}

u32 fldCommand_ENCOUNT_OFF(void) {
    fldEncountClearFlag(0x10);
    return 1;
}

s32 fldCommand_SET_FIX_CAMERA(void) {
    dds3Basic_t *scene = dds3GetControlScene();
    dds3Basic_t *basic = dds3SearchSceneBasic(scene, scrGetIntPara(0), 4);

    if (basic == NULL) {
        printf(D_00532980);
        return 1;
    }
    fldGlobalBase.mode = 1;
    fldGlobalBase.campos.x = basic->poseset->pos.x;
    fldGlobalBase.campos.y = basic->poseset->pos.y;
    fldGlobalBase.campos.z = basic->poseset->pos.z;
    resetCamTgtPlayer();
    return 1;
}

INCLUDE_ASM(const s32, "field/fldCommand", fldCommand_SET_CHASE_CAMERA);

s32 func_00158548(void) {
    fldCamMode[0] = 3;
    return 1;
}

INCLUDE_ASM(const s32, "field/fldCommand", fldCommand_CAM_DIR);

s32 fldCommand_CAM_SEL_PHOKAN(void) {
    f32 pos[4];
    f32 rot[4];
    dds3Basic_t *target;
    dds3Basic_t *object;
    dds3Basic_t *world;

    if (scrGetIntPara(0) == -1) {
        target = fldGetUnitFollowCamera();
        if (target == NULL) {
            return 1;
        }
        object = dds3SetActiveCameraSceneBasic(dds3GetActiveScene(), target);
        if (object == NULL) {
            return 1;
        }
        dds3CameraObjectBasicGetTgtCam(object, pos, rot);
        fldCamGetPhokanParam();
        fldCamPhokanSelX[0] = pos[0];
        fldCamPhokanSelX[1] = pos[1];
        fldCamPhokanSelX[2] = pos[2];
        D_002FC9F0[0] = rot[0];
        D_002FC9F0[1] = rot[1];
        D_002FC9F0[2] = rot[2];
        fldCamPhokanState = 0;
        fldCamPHokanMode = scrGetIntPara(1);
        fldCamMode[0] = 4;
    } else {
        world = dds3GetActiveScene();
        target = dds3SearchSceneBasic(world, scrGetIntPara(0), 4);
        if (target == NULL) {
            return 1;
        }
        dds3SetActiveCameraSceneBasic(dds3GetActiveScene(), target);
    }
    return 1;
}

s32 fldCommand_SET_NPC_CAMERA(void) {
    char *name;

    name = scrGetStrPara(0);
    fldGlobalBase.mode = 5;
    strcpy(fldGlobalBase.requestedSceneName, name);
    return 1;
}

INCLUDE_RODATA(const s32, "field/fldCommand", D_00532980);

INCLUDE_ASM(const s32, "field/fldCommand", fldCommand_SET_ALLPANEL);

void fldPanelSet(void) {
}

s32 fldCommand_SET_CASH_F(void) {
    fldAreaWork_t *work = &fldGlobalBase;
    s32 area;

    if (work->area < 10) {
        return 1;
    }
    area = scrGetIntPara(0);
    if (area == 0) {
        area = work->area;
    }
    fldLoadNextFile(area, scrGetIntPara(1));
    return 1;
}

s32 fldCommand_FREE_CASH_F(void) {
    if (fldGb[0] < 10) {
        return 1;
    }
    fldFreeNextFile();
    return 1;
}

s32 fldCommand_SEARCH_ID(const char *name) {
    dds3Basic_t *entry = dds3SceneBasicSearchName(dds3GetControlScene(), name);
    if (entry != 0) {
        return entry->serialID;
    }
    printf("field SEARCH_ID NotFound:[%s]\n", name);
    return -1;
}

s32 fldCommand_GetHojiID(char *name) {
    s32 index;
    if (strstr(name, D_0054F0A8) == 0) return 0;
    for (index = 0; index < 32; index++) {
        if (name[index] == '\0') {
            if (index < 4) return 0;
            return (name[index - 2] - '0') * 10 + (name[index - 1] - '0');
        }
    }
    return 0;
}

s32 fldCommand_SET_CAMTBL(void) {
    s32 value;

    value = scrGetIntPara(0);
    fldCamUnit[0] = value;
    return 1;
}

s32 fldCommand_FLD_KANKYO(void) {
    s32 durationMode;

    fldRunDanteGunFlag.skyTable = scrGetIntPara(0);
    durationMode = scrGetIntPara(1);
    fldSkySetTbl(fldRunDanteGunFlag.skyTable, durationMode);
    return 1;
}

s32 fldCommand_FLD_FILTER(void) {
    s32 second;
    s32 third;
    fldRunDanteGunFlag.filter = scrGetIntPara(0) & 0xFF;
    second = scrGetIntPara(1);
    third = scrGetIntPara(2);
    fldSkySetFilter(fldRunDanteGunFlag.filter, second, third);
    fldSetFilterAnime(scrGetIntPara(3));
    return 1;
}

s32 fldCommand_FLD_MODEL_ON(void) {
    s32 world;
    s32 stage;
    s32 mode;
    char *name;
    s32 room;
    dds3BasicArrayHandle_t *list;
    dds3Basic_t *node;

    world = scrGetIntPara(0);
    if (world == 0) {
        world = fldGlobalBase.area;
    }
    stage = scrGetIntPara(1);
    if (stage == 0) {
        stage = fldGlobalBase.floor + 1;
    }
    mode = scrGetIntPara(3);
    name = scrGetStrPara(2);
    if (name == NULL) {
        for (room = 0; room < 16; room++) {
            fldSetModelFlag(world, stage, room, 0);
        }
    } else {
        room = fldCommand_GetHojiID(name);
        fldSetModelFlag(world, stage, room, 0);
    }
    if (world == fldGlobalBase.area && stage == fldGlobalBase.floor + 1) {
        list = dds3SceneBasicGetList(dds3GetControlScene(), 6);
        if (dds3GetBasicArrayCount((dds3BasicArrayHandle_t *)list) != 0) {
            dds3SetFirstBasicArray((dds3BasicArrayHandle_t *)list);
            do {
                node = (dds3Basic_t *)(u32)dds3GetBasicArray((dds3BasicArrayHandle_t *)list);
                if (((fldMatterWork_t *)node->workdata)->roomNumber == (u32)room) {
                    switch (mode) {
                    case 0:
                        dds3MatterObjectBasicSetAlpMode(node, 1);
                        break;
                    case 1:
                        dds3MatterObjectBasicSetAlpMode(node, 5);
                        break;
                    case 2:
                        dds3MatterObjectBasicSetAlpMode(node, 7);
                        break;
                    }
                }
            } while (dds3SetNextBasicArray((dds3BasicArrayHandle_t *)list) != 0);
            dds3FreeBasicArrayHandle(list);
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "field/fldCommand", fldCommand_FLD_MODEL_OFF);

s32 fldCommand_FLD_ANIME_ON(void) {
    s32 world;
    s32 stage;
    char *name;
    void *object;
    s32 id;
    s32 i;

    world = scrGetIntPara(0);
    if (world == 0) {
        world = fldGlobalBase.area;
    }
    stage = scrGetIntPara(1);
    if (stage == 0) {
        stage = fldGlobalBase.floor + 1;
    }
    name = scrGetStrPara(2);
    if (name == NULL) {
        for (i = 0; i < 16; i++) {
            fldSetAnimeFlag(world, stage, i, 0);
        }
    } else {
        fldSetAnimeFlag(world, stage, fldCommand_GetHojiID(name), 0);
    }
    if (world == fldGlobalBase.area && stage == fldGlobalBase.floor + 1) {
        id = fldCommand_SEARCH_ID(name);
        if (id != -1) {
            object = dds3SearchSceneBasic(dds3GetControlScene(), id, 6);
            switch (scrGetIntPara(3)) {
            case 0:
                dds3ObjectBaseMatterAnimeSpeed(object, 0);
                break;
            case 1:
                dds3ObjectBaseMatterAnimeSpeed(object, 1);
                break;
            case 2:
                dds3ObjectBaseMatterAnimeSpeed(object, 2);
                break;
            case 3:
                dds3ObjectBaseMatterAnimeSpeed(object, 6);
                break;
            }
        }
    }
    return 1;
}

s32 fldCommand_FLD_ANIME_OFF(void) {
    s32 world;
    s32 stage;
    char *name;
    void *object;
    s32 id;
    s32 i;

    world = scrGetIntPara(0);
    if (world == 0) {
        world = fldGlobalBase.area;
    }
    stage = scrGetIntPara(1);
    if (stage == 0) {
        stage = fldGlobalBase.floor + 1;
    }
    name = scrGetStrPara(2);
    if (name == NULL) {
        for (i = 0; i < 16; i++) {
            fldSetAnimeFlag(world, stage, i, 1);
        }
    } else {
        fldSetAnimeFlag(world, stage, fldCommand_GetHojiID(name), 1);
    }
    if (world == fldGlobalBase.area && stage == fldGlobalBase.floor + 1) {
        id = fldCommand_SEARCH_ID(name);
        if (id != -1) {
            object = dds3SearchSceneBasic(dds3GetControlScene(), id, 6);
            switch (scrGetIntPara(3)) {
            case 0:
                dds3ObjectBaseMatterAnimeSpeed(object, 3);
                break;
            case 1:
                dds3ObjectBaseMatterAnimeSpeed(object, 4);
                break;
            case 2:
                dds3ObjectBaseMatterAnimeSpeed(object, 5);
                break;
            }
        }
    }
    return 1;
}

s32 fldCommand_FLD_ATARI_ON(void) {
    s32 world;
    s32 stage;
    char *name;
    s32 i;

    world = scrGetIntPara(0);
    if (world == 0) {
        world = fldGb[0];
    }
    stage = scrGetIntPara(1);
    if (stage == 0) {
        stage = fldArea[0] + 1;
    }
    name = scrGetStrPara(2);
    if (name == NULL) {
        for (i = 0; i < 16; i++) {
            fldSetAtariFlag(world, stage, i, 0);
        }
    } else {
        fldSetAtariFlag(world, stage, fldCommand_GetHojiID(name), 0);
    }
    return 1;
}

s32 fldCommand_FLD_ATARI_OFF(void) {
    s32 world;
    s32 stage;
    char *name;
    s32 i;

    world = scrGetIntPara(0);
    if (world == 0) {
        world = fldGb[0];
    }
    stage = scrGetIntPara(1);
    if (stage == 0) {
        stage = fldArea[0] + 1;
    }
    name = scrGetStrPara(2);
    if (name == NULL) {
        for (i = 0; i < 16; i++) {
            fldSetAtariFlag(world, stage, i, 1);
        }
    } else {
        fldSetAtariFlag(world, stage, fldCommand_GetHojiID(name), 1);
    }
    return 1;
}

s32 fldCommand_FLD_MODEL_ON2(void) {
    s32 area = scrGetIntPara(0);
    s32 floor, target;
    if (area == 0) area = fldGb[0];
    floor = scrGetIntPara(1);
    if (floor == 0) floor = fldArea[0] + 1;
    target = scrGetIntPara(2);
    if (target == 0) return 1;
    fldSetModelFlag2(area, floor, target, 0);
    return 1;
}

s32 fldCommand_FLD_MODEL_OFF2(void) {
    s32 area = scrGetIntPara(0);
    s32 floor, target;
    if (area == 0) area = fldGb[0];
    floor = scrGetIntPara(1);
    if (floor == 0) floor = fldArea[0] + 1;
    target = scrGetIntPara(2);
    if (target == 0) return 1;
    fldSetModelFlag2(area, floor, target, 1);
    return 1;
}

s32 fldCommand_FLD_ANIME_ON2(void) {
    s32 area = scrGetIntPara(0);
    s32 floor, target;
    if (area == 0) area = fldGb[0];
    floor = scrGetIntPara(1);
    if (floor == 0) floor = fldArea[0] + 1;
    target = scrGetIntPara(2);
    if (target == 0) return 1;
    fldSetAnimeFlag2(area, floor, target, 0);
    return 1;
}

s32 fldCommand_FLD_ANIME_OFF2(void) {
    s32 area = scrGetIntPara(0);
    s32 floor, target;
    if (area == 0) area = fldGb[0];
    floor = scrGetIntPara(1);
    if (floor == 0) floor = fldArea[0] + 1;
    target = scrGetIntPara(2);
    if (target == 0) return 1;
    fldSetAnimeFlag2(area, floor, target, 1);
    return 1;
}

s32 fldCommand_FLD_ANIME_ID2(void) {
    s32 world;
    s32 stage;
    s32 slot;

    world = scrGetIntPara(0);
    if (world == 0) {
        world = fldGb[0];
    }
    stage = scrGetIntPara(1);
    if (stage == 0) {
        stage = fldArea[0] + 1;
    }
    slot = scrGetIntPara(2);
    if (slot == 0) {
        return 1;
    }
    fldGetAnimeId2(world, stage, slot, scrGetIntPara(3));
    return 1;
}

s32 fldCommand_FLD_MAP_ON(void) {
    s32 area = scrGetIntPara(0);
    s32 floor, flagNumber;
    if (area == 0) area = fldGb[0];
    floor = scrGetIntPara(1);
    if (floor == 0) floor = fldArea[0] + 1;
    flagNumber = scrGetIntPara(2);
    if (flagNumber == 0) return 1;
    fldAutoMapOpenBlock2(area, floor, flagNumber);
    return 1;
}

s32 fldCommand_FLD_MAP_OFF(void) {
    s32 area = scrGetIntPara(0);
    s32 floor, flagNumber;
    if (area == 0) area = fldGb[0];
    floor = scrGetIntPara(1);
    if (floor == 0) floor = fldArea[0] + 1;
    flagNumber = scrGetIntPara(2);
    if (flagNumber == 0) return 1;
    fldAutoMapCloseBlock2(area, floor, flagNumber);
    return 1;
}


/* NON_MATCHING: 74 of 75 words, exact size; retail ends on a bnel, this a bne. */
#ifdef NON_MATCHING
s32 fldCommand_FLD_NPC_ON(void) {
    s32 world;
    s32 stage;
    char *name;
    s32 room;

    printf("CMD NPC_ON(%d,%d,%s)\n", scrGetIntPara(0), scrGetIntPara(1), scrGetStrPara(2));
    world = scrGetIntPara(0);
    if (world == 0) {
        world = fldGlobalBase.area;
    }
    stage = scrGetIntPara(1);
    if (stage == 0) {
        stage = fldGlobalBase.floor + 1;
    }
    name = scrGetStrPara(2);
    if (name == NULL) {
        for (room = 0; room < 16; room++) {
            fldSetNpcFlag(world, stage, room, 0);
        }
    } else {
        room = fldCommand_GetHojiID(name);
        fldSetNpcFlag(world, stage, room, 0);
    }
    if (world == fldGlobalBase.area) {
        if (stage == fldGlobalBase.floor + 1) {
            fldNpcHojiRefresh();
        }
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "field/fldCommand", fldCommand_FLD_NPC_ON);
#endif


/* NON_MATCHING: 74 of 75 words, exact size; retail ends on a bnel, this a bne. */
#ifdef NON_MATCHING
s32 fldCommand_FLD_NPC_OFF(void) {
    s32 world;
    s32 stage;
    char *name;
    s32 room;

    printf("CMD NPC_OFF(%d,%d,%s)\n", scrGetIntPara(0), scrGetIntPara(1), scrGetStrPara(2));
    world = scrGetIntPara(0);
    if (world == 0) {
        world = fldGlobalBase.area;
    }
    stage = scrGetIntPara(1);
    if (stage == 0) {
        stage = fldGlobalBase.floor + 1;
    }
    name = scrGetStrPara(2);
    if (name == NULL) {
        for (room = 0; room < 16; room++) {
            fldSetNpcFlag(world, stage, room, 1);
        }
    } else {
        room = fldCommand_GetHojiID(name);
        fldSetNpcFlag(world, stage, room, 1);
    }
    if (world == fldGlobalBase.area && stage == fldGlobalBase.floor + 1) {
        fldNpcHojiRefresh();
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "field/fldCommand", fldCommand_FLD_NPC_OFF);
#endif


s32 fldCommand_FLD_GIMIC_HOJI(void) {
    s32 world;
    s32 stage;
    s32 index;

    printf("CALL GIMIC_HOJI %d %d %d %d\n", scrGetIntPara(0), scrGetIntPara(1),
           scrGetIntPara(2), scrGetIntPara(3));
    world = scrGetIntPara(0);
    if (world == 0) {
        world = fldGb[0];
    }
    stage = scrGetIntPara(1);
    if (stage == 0) {
        stage = fldArea[0] + 1;
    }
    index = scrGetIntPara(2);
    if (index == 0) {
        for (index = 0; index < 16; index++) {
            fldGbSetGimic(world, stage, index, scrGetIntPara(3));
        }
    } else {
        fldGbSetGimic(world, stage, index, scrGetIntPara(3));
    }
    return 1;
}

s32 fldCommand_FLD_CHK_BTL(void) {
    if (fldGetRestoreMode() == 1) {
        scrSetIntReturnValue(1);
        return 1;
    }
    scrSetIntReturnValue(0);
    return 1;
}

s32 fldCommand_FLD_CHK_ANA(void) {
    if (fldGetRestoreMode() == 3) {
        scrSetIntReturnValue(1);
        return 1;
    }
    scrSetIntReturnValue(0);
    return 1;
}

s32 fldCommand_FLD_CHK_SISETU(void) {
    if (fldGetRestoreMode() == 4) {
        scrSetIntReturnValue(1);
        return 1;
    }
    scrSetIntReturnValue(0);
    return 1;
}

s32 fldCommand_FLD_CHK_EVE(void) {
    if (fldGetRestoreMode() == 2) {
        scrSetIntReturnValue(1);
        return 1;
    }
    scrSetIntReturnValue(0);
    return 1;
}

INCLUDE_RODATA(const s32, "field/fldCommand", D_00532A58);

s32 fldCommand_FLD_GET_TAKARA_TBL(void) {
    s32 index = fldFindEveRngIndex((u32)scrGetScriptHandle()->ProcessID);

    switch (scrGetIntPara(0)) {
    case 0:
        printf(D_00532A58, fldTakaraTable[index].type, fldTakaraTable[index].id,
               fldTakaraTable[index].num);
        scrSetIntReturnValue(fldTakaraTable[index].type);
        break;
    case 1:
        scrSetIntReturnValue(fldTakaraTable[index].id);
        break;
    case 2:
        scrSetIntReturnValue(fldTakaraTable[index].num);
        break;
    case 3:
        scrSetIntReturnValue(fldTakaraTable[index].unk08);
        break;
    case 4:
        scrSetIntReturnValue(fldTakaraTable[index].unk0C);
        break;
    }
    return 1;
}

u32 fldCommand_FLD_SET_TAKARA_OPEN(void) {
    scrProcessWork_s *script;
    s32 room;

    script = scrGetScriptHandle();
    room = fldFindEveRngIndex((u32)script->ProcessID);
    fldSetTakaraOpen(room);
    return 1;
}

u32 fldCommand_FLD_CHK_TAKARA_OPEN(void) {
    if (fldGetTakaraOpen(fldFindEveRngIndex((u32)scrGetScriptHandle()->ProcessID)) != 0) {
        scrSetIntReturnValue(1);
    } else {
        scrSetIntReturnValue(0);
    }
    return 1;
}

s32 fldCommand_FLD_WARP_CALL_FIELD(void) {
    const char *entry;

    printf(D_00532A88);
    if (fldDoor_ChkWarp() != 0) {
        fldDoor_Warp(NULL);
        return 1;
    }
    entry = fldEveHitResName_Find((u32)scrGetScriptHandle()->ProcessID);
    if (entry != NULL) {
        fldDoor_Warp(entry);
    }
    return 1;
}

s32 fldCommand_FLD_CALL_WAPEX(void) {
    printf(D_00532AA0);
    fldDoor_WarpEx(scrGetIntPara(0));
    return 1;
}

s32 fldCommand_FLD_WARP_CASH_F(void) {
    dds3Basic_t *scene = dds3GetActiveScene();
    dds3Basic_t *unit = dds3GetActiveUnitSceneBasic(scene);
    scrProcessWork_s *command;
    s32 id;
    const char *name;
    dds3Basic_t *hit;

    if (unit == NULL) {
        return 1;
    }
    command = scrGetScriptHandle();
    id = fldEveHitResID_Find((u32)command->ProcessID);
    name = fldEveHitResName_Find((u32)command->ProcessID);
    hit = dds3SearchSceneBasic(scene, id, 0x11);
    if (hit == NULL) {
        return 1;
    }
    if (fldEveHit_ChkDir(unit, hit) == 0 || name == NULL) {
        return 1;
    }
    fldDoor_WarpCash(name);
    return 1;
}

u32 fldCommand_FLD_WARP_HASIGO_GETTBL(void) {
    s32 motionEntry;

    motionEntry = scrGetIntPara(0);
    motionEntry = fldDoor_GetHasigoTbl(motionEntry);
    scrSetIntReturnValue(motionEntry);
    return 1;
}

u32 fldCommand_FLD_WARP_ELV_GETTBL(void) {
    s32 rowValue;

    rowValue = scrGetIntPara(0);
    rowValue = fldDoor_GetElvTbl(rowValue);
    scrSetIntReturnValue(rowValue);
    return 1;
}

u32 fldCommand_FLD_WARP_ELV_EXIT(void) {
    s32 sceneRoomSelection;

    sceneRoomSelection = scrGetIntPara(0);
    fldDoor_ElvExit(sceneRoomSelection);
    return 1;
}

u32 fldCommand_FLD_ELV_FLOORSTART(void) {
    s32 first;
    s32 second;

    first = scrGetIntPara(0);
    second = scrGetIntPara(1);
    fldElvSetFloor(first, second);
    return 1;
}

u32 fldCommand_FLD_ELV_FLOOREND(void) {
    fldElvReset();
    return 1;
}

u32 fldCommand_FLD_ELV_ENDCHK(void) {
    s32 result;

    result = fldElvCheckEnd();
    scrSetIntReturnValue(result);
    return 1;
}

s32 fldCommand_FLD_WARP_GET_DOOR(void) {
    scrSetIntReturnValue(fldWapGetDoor(scrGetIntPara(0)));
    return 1;
}

u32 fldCommand_FLD_WARP_RUN_START(void) {
    s32 scene;
    if (fldDoor_ChkWarp()) {
        scene = 0;
    } else {
        scene = fldEveHitResID_Find((u32)scrGetScriptHandle()->ProcessID);
    }
    fldDoorRun_FindStart(scene);
    return 1;
}

s32 fldCommand_FLD_INF_NEXT(void) {
    s32 param0 = scrGetIntPara(0);
    s32 param1 = scrGetIntPara(1);

    scrSetIntReturnValue(fldInfNext(param0, param1));
    return 1;
}

s32 fldCommand_FLD_INF_GET(void) {
    s32 actorId = scrGetIntPara(0);
    s32 attribute = scrGetIntPara(1);

    scrSetIntReturnValue(fldInfGet(actorId, attribute));
    return 1;
}

void fldCommand_FLD_INF_GETPANEL(void) {
    scrSetIntReturnValue(fldInfChkFastPanel(1));
}

s32 fldCommand_FLD_INF_GOWAP(void) {
    fldInfToWAP();
    return 1;
}

u32 fldCommand_FLD_BGM_RELOAD(void) {
    fldBgmLoad();
    return 1;
}

s32 fldCommand_FLD_BGM_RELOAD_WAIT(void)
{
    return fldWaitBgmReload() != 0;
}

INCLUDE_RODATA(const s32, "field/fldCommand", D_00532A88);

INCLUDE_RODATA(const s32, "field/fldCommand", D_00532AA0);

s32 fldCommand_FLD_BGM_PLAY(void) {
    s32 track = scrGetIntPara(0);

    D_002FC020[0] = track;
    printf("FLD_BGM_PLAY:%d\n", track);
    fldBgmPlay();
    return 1;
}

s32 fldCommand_FLD_BGM_CONTINUE(void) {
    D_002FC020[0] = scrGetIntPara(0);
    fldBgmContinue();
    return 1;
}

s32 fldCommand_FLD_BGM_PAUSE(void) {
    fldPauseBgm();
    return 1;
}

s32 fldCommand_FLD_BGM_STOP(void) {
    fldBgmStop();
    return 1;
}

s32 fldCommand_FLD_BGM_FADE(void) {
    fldBgmFadeOut();
    return 1;
}

s32 fldCommand_FLD_SE_PLAY(void) {
    fldSePlay(scrGetIntPara(0));
    return 1;
}

s32 fldCommand_FLD_SE_STOP(void) {
    fldSeStop(scrGetIntPara(0));
    return 1;
}

s32 fldCommand_FLD_MSE_PLAY(void)
{
    s32 sequenceGroup;

    sequenceGroup = scrGetIntPara(0);
    fldMotionSePlay(sequenceGroup, 0);
    return 1;
}

s32 fldCommand_FLD_MSE_PLAY2(void) {
    s32 sequenceGroup = scrGetIntPara(0);
    s32 sequenceIndex = scrGetIntPara(1);

    fldMotionSePlay(sequenceGroup, sequenceIndex);
    return 1;
}

u32 fldCommand_FLD_MSE_STOP2(void) {
    s32 sequenceGroup;
    s32 sequenceIndex;

    sequenceGroup = scrGetIntPara(0);
    sequenceIndex = scrGetIntPara(1);
    fldMotionSeStop(sequenceGroup, sequenceIndex);
    return 1;
}

u32 fldCommand_FLD_TITLE_START(void) {
    return 1;
}

u32 fldCommand_FLD_TITLE_START2(void) {
    s32 fieldId;
    s32 titleArg;

    fieldId = scrGetIntPara(0);
    titleArg = scrGetIntPara(1);
    fldTitleStart(fieldId, titleArg);
    return 1;
}

s32 fldCommand_FLD_SET_ENCOUNT(void) {
    s32 value;

    value = scrGetIntPara(0);
    fldEncountRate[0] = value;
    return 1;
}

s32 fldCommand_FLD_EXIT_CAMERA(void) {
    scrProcessWork_s *command = scrGetScriptHandle();
    const char *record = fldEveHitResName_Find((u32)command->ProcessID);

    if (record != NULL) {
        fldDoor_ExitCamera(record);
    }
    return 1;
}

s32 fldCommand_FLD_WARP_GET_WARPEFF(void) {
    scrSetIntReturnValue(fldDoor_GetWarpEff());
    return 1;
}

s32 fldCommand_FLD_MODEL_LIGHT(void) {
    fldNpcLightSet(evtGetUnitHandle(scrGetIntPara(0)));
    return 1;
}

INCLUDE_ASM(const s32, "field/fldCommand", fldCommand_FLD_SAGASI_AKUMA);

u32 fldCommand_FLD_ANAHORI_INIT(void) {
    fldAnahoriSelectInit();
    return 1;
}

u32 fldCommand_FLD_ANAHORI_GET(void) {
    u32 busy;

    busy = fldAnahoriSelectGet();
    scrSetIntReturnValue(busy);
    return 1;
}

u32 fldCommand_FLD_ANAHORI_NEXT(void) {
    fldAnahoriSelectNext();
    return 1;
}

u32 fldCommand_FLD_ANAHORI_DEL(void) {
    fldAnahoriSelectDel();
    return 1;
}

s32 fldCommand_FLD_NPC_SINEN(void) {
    gFldKyouseiTalk = scrGetIntPara(0);
    return 1;
}

s32 fldCommand_FLD_GET_KIRETA(void) {
    s32 changed = 0;
    switch (scrGetIntPara(0)) {
    case 0:
        if (fldGlobalBase.consumedFlags & 1) {
            fldGlobalBase.consumedFlags &= ~1;
            changed = 1;
        }
        break;
    case 1:
        if (fldGlobalBase.consumedFlags & 2) {
            fldGlobalBase.consumedFlags &= ~2;
            changed = 1;
        }
        break;
    case 2:
        if (fldGlobalBase.consumedFlags & 4) {
            fldGlobalBase.consumedFlags &= ~4;
            changed = 1;
        }
        break;
    case 3:
        if (fldGlobalBase.consumedFlags & 8) {
            fldGlobalBase.consumedFlags &= ~8;
            changed = 1;
        }
        break;
    }
    scrSetIntReturnValue(changed);
    return 1;
}

s32 fldCommand_FLD_GET_ITEMBOX_RND(void) {
    s32 threshold = D_00316EC8[evtGetAgeOfMoon()];
    s32 value = dds3GetRandIntA(0, 100);

    printf(D_00532AC8, value, threshold);
    if (threshold >= value) {
        scrSetIntReturnValue(1);
    } else {
        scrSetIntReturnValue(0);
    }
    return 1;
}

s32 fldCommand_FLD_MODEL_DANTE(void) {
    if (D_0054EAF0 == NULL) {
        scrSetIntReturnValue(-1);
        return 1;
    }
    scrSetIntReturnValue(D_0054EAF0->value);
    return 1;
}

u32 fldCommand_FLD_DANTE_AI_STOP(void) {
    evtBitOff(0xAF4);
    return 1;
}

u32 fldCommand_FLD_DANTE_AI_START(void) {
    evtBitOn(0xAF4);
    return 1;
}

s32 fldCommand_FLD_DANTE_STANCE(void) {
    fldRunDanteStanceSet(scrGetIntPara(0));
    return 1;
}

INCLUDE_RODATA(const s32, "field/fldCommand", D_00532AC8);

