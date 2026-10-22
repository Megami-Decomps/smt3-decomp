#include "fpu.h"
#include "common.h"
#include "libsdf.h"
#include "kernel.h"
#include "ps2asm.h"
#include "basic.h"
#include "billboard.h"
#include "draw.h"
#include "scr.h"
#include "draw3d.h"
#include "field.h"
#include "model.h"
#include "motion.h"
#include "newdata.h"
#include "event.h"
#include "effect.h"

typedef struct fldActorEntry_t fldActorEntry_t;

typedef union fldWaypointHeaderValues_t fldWaypointHeaderValues_t;

typedef struct fldWaypointHeader_t fldWaypointHeader_t;

typedef struct fldWaypointBlock_t fldWaypointBlock_t;

typedef struct devState_t devState_t;

struct fldActorEntry_t {
    s8 kind;
    u8 pad01;
    s16 requiredFlag;
    s16 floor;
    char name[0xC];
    s16 motion;
    s16 secondaryMotion;
    s16 sound;
    char motionName[0xC];
    char otherName[0xC];
    s8 variantMode;
    s8 flags31;
    s16 variant;
    s16 sequenceKind;
    s16 sequenceValue;
    char sequenceName[0xC];
    s8 linkKind;
    s8 rowIndex;
    char linkName[0xC];
    s8 sequenceCode;
    s8 selectedRoom;
    s8 flags54;
    char taskName[0xF];
};

enum { FLD_WAYPOINT_HEADER_COUNT = 5, FLD_WAYPOINT_BLOCK_BYTES = 0x64A0 };

union fldWaypointHeaderValues_t {
    s16 data[12];
    struct {
        s16 floors[6];
        s16 blocks[6];
    } elevator;
};

struct fldWaypointHeader_t {
    s32 area;
    s16 sound;
    s16 floorCount;
    fldWaypointHeaderValues_t body;
};

struct fldWaypointBlock_t {
    fldWaypointHeader_t headers[FLD_WAYPOINT_HEADER_COUNT];
    fldActorEntry_t actors[256];
};

extern u32 D_0054EC44;

extern u32 fldDrawEnv;

extern sdfTexHandle_t *D_0054EC60;

extern void *memset(void *, int, unsigned int);

extern u8 D_0054EC50[];

extern u8 D_0054EC58[];

extern s32 fldCamPhokanState;

extern s32 fldCamPHokanMode;

dds3Basic_t *dds3GetActiveScene(void);

extern f32 fldCamElvState;

extern s32 gencStageInnAlp;

extern s32 encDrawState;

extern u32 fldSkyTable;

extern u32 fldSkyTableIdx;

extern u32 fldSkyLightMaTbl;

extern void *fldSkyDefaultData;

/* fldSkyDataTable stride, 292 bytes; only the filter mode at offset 0 is used here. */
typedef struct {
    s32 mode;
    s32 colors[8][9];
} fldSkyData_t;

extern fldSkyData_t *fldSkyDataTable;

extern void *fldSkyDefault;

extern s32 fldSkyTblUpperIdx;

extern s32 D_0054ED5C;

extern f32 D_0054ED60;

extern s32 D_0054ED64;

extern s32 D_0054ED68;

extern s32 D_0054ED6C;

extern f32 D_0054ED70;

extern f32 D_0054ED74;

extern s32 D_0054ED7C;

extern f32 D_0054ED80;

extern s32 D_0054ED84;

extern s32 D_0054ED8C;

extern s32 fldSkyEnable;

extern s32 fldSkyFilter;

extern s32 fldSkyFilterPrev;

extern f32 fldSkyFilterRate;

extern f32 fldSkyFilterTime;

extern s32 fldGb __attribute__((section(".data")));

extern u32 fldItemBoxObjBuf;

extern u32 fldMatterTable;

extern u32 D_0054EDA8;

extern u32 D_0054EDAC;

extern s32 D_0054EDA0;

extern s32 fldPlaneCount;

extern s32 fldItemBoxObjCount;

extern s32 gFldEveRngIdx_N;

extern s32 fldEveRngCount;

extern s32 gFldEveRngIdx;

extern s32 gFldEveRngIdx_T;

extern s32 fldEveHitState;

extern u32 gEveNum;

extern s8 fldDoorExitMode[];

extern u8 *fldDoor_Find3(void);

extern u8 fldDoorExitFlag;

extern s32 strcmp(const char *left, const char *right);

extern s32 gFldInfIdx;

void fldMakeFilePath(char *pathstr, s32 field, s32 area);

devState_t *sdfFileOpen(const char *aFileName);

void sdfFileRead(devState_t *aHandle, void *aPtr, s32 aLeng);

void sdfFileClose(devState_t *aHandle);

extern s32 sprintf(char *, const char *, ...);

extern void *memcpy(void *, const void *, unsigned int);

extern s32 gFldInf_HitIdx;

extern s32 gFldInf_WpExit;

extern u8 *fldDoor_Find2(const char *);

dds3Basic_t *dds3SceneBasicSearchNameAndType(dds3Basic_t *scene, s32 type, const u8 *name);

dds3Basic_t *dds3SetActiveCameraSceneBasic(dds3Basic_t *pSceneBasic, dds3Basic_t *pCameraBasic);

s32 evtBitCheck(s32 no);

extern fldWaypointBlock_t fldDoorParamTable;

void fldPlayerOff(void);

s32 fldLoadNextFile(s32 field, s32 area);

extern u32 gFld_ElvTakasa;

extern s32 fldDoorWarpTarget;

/* fldDoorTable is fldDoorParamTable.actors: the same 100-byte waypoint record,
   reached through its own symbol at fldDoorParamTable + 0xA0. */
extern fldActorEntry_t fldDoorTable[256];

/* The 0xA0 scene request block dds3AdminiCallSequence(6, ...) takes. */
typedef struct {
    u8 safeUnitPoint[0x30];
    u32 flag;
    s32 mode;
    u32 sequenceWord38;
    u32 pointResrcID;
    char pointResName[16];
    s32 loadField;
    s32 loadArea;
    s32 loadScript;
    s32 callmode;
    u16 bgm;
    u16 bgmMode;
    s32 camMode;
    u8 unk68[8];
    char camName[16];
    char FoceScript[16];
    u32 loadMode;
    u8 unk94[12];
} fldSceneParam_t;

extern void fldSetSceneParam4(fldSceneParam_t *pParam, s32 loadField, s32 loadArea, const char *pointResName,
                              s32 bgm, s32 camMode, const char *camName);

extern void dds3AdminiCallSequence(s32 sequence, void *data, s32 datasize, s32 restore);

extern s32 D_0054EDF4;

extern s32 fldDoorSiseState;

extern s32 D_0054EDFC;

extern s32 D_0054EE00;

extern s32 gFldNowDZone;

void fldDoorLoad(s32 field);

extern s32 scriptlabel;

extern void *fldDrwZBuf;

extern f32 fldWmFastMove;

extern void *hPartialMultiBlur;

extern s32 gfldPlayerHasiCnt;

extern s32 gfldPlayerDamegeCnt;

extern s32 fldPlayerStepDist;

extern s32 gSkyTblAnime;

extern f32 gSkyTblAnimeCntI;

extern s32 gSkyTblBaseIdx;

extern char D_0054EC80[];

extern s32 gencOn;

extern s32 gencMode;

extern s32 gFilterAlpha;

extern s32 gFilterAnime;

extern f32 gFilterAnimeCntF;

extern s32 gFilterAnimeOfsA;

extern s32 fldCamResetOn;

extern s32 fldCamMiwatasi;

extern s32 fldElvCameraScene;

extern s32 fldWmWarpMode;

extern s32 gFldWapPanel;

extern s32 gFldKyouseiTalk;

extern s32 fldArea[];

extern u8 D_00303020[];

extern s32 gFldWap_after_nextidx;

extern s32 fldPlayerFastMode;

extern f32 fldWmapPlayerState;

/* fldDoorEventTable stride, 84 bytes; the clear leaves +0x20 alone. */
typedef struct {
    s32 unk00;
    s32 unk04;
    s32 unk08;
    s32 unk0C;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    f32 unk50;
} fldDoorEvent_t;

extern fldDoorEvent_t fldDoorEventTable[256];

extern dds3Basic_t *dds3SceneBasicSearchName(dds3Basic_t *scene, const char *name);

extern f32 D_0054ECB4;

extern f32 D_0054ECB8;

void fldPlayerCalc_WarpStart(void);

extern s32 D_0054ED90;

extern s32 fldHitChkMatter;

extern s32 fldEveRngPrevIdx;

extern s32 fldEveRngHitIdx;

extern s32 fldEveHitCount;

void fldClearDoorEvents(void);

const char *fldEveHitResName_Find(u32 pid);

extern u8 D_00302FA0[];

extern u8 D_00302FB0[];

extern u8 D_00302FC0[];

extern u8 D_00302FD0[];

extern u8 D_00302FF0[];

extern u8 D_00303000[];

extern u8 D_00303010[];

extern u8 D_00303030[];

u8 *fldDoor_Find2(const char *evename) {
    s32 i;
    fldActorEntry_t *entry;

    if (evename == NULL) return NULL;
    for (i = 0; i < 256; i++) {
        entry = &fldDoorTable[i];
        if (entry->requiredFlag != 0 && evtBitCheck(entry->requiredFlag) == 0) continue;
        if (entry->floor != fldGlobalBase.floor + 1) continue;
        if (strcmp(evename, entry->name) != 0) continue;
        scriptlabel = i;
        switch (entry->kind) {
        case 1:
            if (fldGlobalBase.area < 10) {
                if (entry->variantMode == 0 && entry->variant != 0 && entry->variant < 11) {
                    gFldWapPanel = 7;
                    return D_00303010;
                }
                gFldWapPanel = 7;
                return D_00303010;
            }
            if (entry->variantMode == 0 && entry->variant != 0 && entry->variant < 10) {
                gFldWapPanel = 6;
                return D_00303000;
            }
            gFldWapPanel = 0;
            return D_00302FA0;
        case 2:
            gFldWapPanel = 1;
            return D_00302FB0;
        case 3:
            gFldWapPanel = 2;
            return D_00302FC0;
        case 4:
            if (fldGlobalBase.area >= 40) {
                gFldWapPanel = 9;
                return D_00303030;
            }
            gFldWapPanel = 3;
            return D_00302FD0;
        case 5:
            gFldWapPanel = 5;
            return D_00302FF0;
        case 6:
        case 7:
        case 8:
        case 9:
            break;
        case 10:
            gFldWapPanel = 0;
            return D_00303020;
        }
    }
    return NULL;
}

u8 *fldDoor_Find3(void) {
    s32 idx = gFldWap_after_nextidx;
    fldActorEntry_t *entry = &fldDoorTable[idx];

    if (entry->floor == fldArea[0] + 1) {
        if (entry->kind == 10) {
            scriptlabel = idx;
            gFldWapPanel = 8;
            return D_00303020;
        }
    }
    return NULL;
}

s32 fldDoor_GetWarpEff(void)
{
    return fldDoorTable[scriptlabel].motion;
}

s32 fldWapGetDoor(void) {
    fldActorEntry_t *entry = &fldDoorTable[scriptlabel];
    s32 motion;

    if (entry->kind == 1) {
        motion = entry->motion;
        if (motion == 5 || entry->secondaryMotion == 5 || motion == 6 ||
        entry->secondaryMotion == 6 || motion == 7 ||
        entry->secondaryMotion == 7 || motion == 8 ||
        entry->secondaryMotion == 8) {
            return 0x28;
        }
        return 0x14;
    }
    return 0;
}

extern scrProcessWork_s *scrGetScriptHandle(void);

extern void fldSePlay(s32 seid);

extern void fldFindDoorEvent(s32 handle);

extern u32 fldLoadNextGetStatus(void);

extern void fldStartBgmLoad(s32 kind);

extern void dds3FadeOut(s32 r, s32 g, s32 b, s32 frame);

extern void dds3FadeColorSet(s32 r, s32 g, s32 b);

extern void fldAnaStart(f32 x, f32 y, f32 z);

extern f32 fldPlayerWarpState;

void fldDoorRun_FindStart(s32 resid) {
    s32 idx;
    fldActorEntry_t *entry;
    const char *name;

    if (fldGlobalBase.area < 10) {
        fldPlayerCalc_WarpStart();
        return;
    }
    if (resid != 0) {
        name = fldEveHitResName_Find((u32)scrGetScriptHandle()->ProcessID);
        if (name == 0) return;
        if (fldDoor_Find2(name) == 0) return;
    }
    idx = scriptlabel;
    entry = &fldDoorTable[idx];
    if (entry->kind == 1) {
        if (entry->floor != fldGlobalBase.floor + 1) return;
        fldSePlay(entry->sound);
        fldFindDoorEvent(fldDoorEventTable[idx].unk04);
        if (entry->variant >= 6) return;
        if (entry->variant < 0) return;
        if (fldLoadNextGetStatus() != 0) return;
        fldStartBgmLoad(entry->variant);
        return;
    }
    if (entry->kind == 2) {
        if (entry->floor != fldGlobalBase.floor + 1) return;
        fldPlayerFastMode = 1;
        fldPlayerWarpState = *(f32 *)&fldDoorEventTable[idx].unk24;
        if (entry->motion == 1) {
            fldSePlay(0x38);
            dds3FadeOut(0xC0, 0xC0, 0xC0, 0xF);
            return;
        }
        dds3FadeOut(0, 0, 0, 0xF);
        return;
    }
    if (entry->kind == 3) {
        dds3FadeColorSet(0, 0, 0);
        fldAnaStart(fldGlobalBase.x, fldGlobalBase.y, fldGlobalBase.z);
        return;
    }
    if (entry->kind == 5) {
        if (entry->motion == 0) {
            gfldPlayerDamegeCnt = 100;
        } else {
            gfldPlayerDamegeCnt = -100;
        }
    } else if (entry->kind == 10) {
        fldSePlay(0x38);
    }
}

void fldDoor_ExitCamera(const char *evename) {
    fldActorEntry_t *entry;
    char *entryName;
    dds3Basic_t *camera;
    s32 i;

    if (evename == NULL) {
        return;
    }
    for (i = 0; i < 0x100; i++) {
        entry = &fldDoorParamTable.actors[i];
        if (entry->requiredFlag != 0 && evtBitCheck(entry->requiredFlag) == 0) {
            continue;
        }
        if (entry->kind == 0) {
            continue;
        }
        entryName = entry->name;
        if (entry->floor == fldGlobalBase.floor + 1) {
            if (strcmp(evename, entryName) == 0) {
                if (entry->variantMode == 2) {
                    return;
                }
                if (entry->variantMode == 0 && entry->linkKind == 3) {
                    camera = (dds3Basic_t *)dds3SceneBasicSearchNameAndType(dds3GetActiveScene(), 4,
                                                                                  entry->linkName);
                    dds3SetActiveCameraSceneBasic(dds3GetActiveScene(), camera);
                    fldPlayerOff();
                    return;
                }
            }
        }
    }
}

s32 fldDoor_ChkWarp(void)
{
    return gFldWapPanel == 8;
}

extern s32 gFldSndTransSrc;

extern s32 fldBgmTransChk(s32 id1, s32 id2);

extern void fldBgmFadeOut(void);

extern void fldBgmTrans(void);

s32 fldWap_SetBgm(fldActorEntry_t *pWap) {
    s32 result = 0;

    if (fldGlobalBase.area != 0x22 && fldGlobalBase.area != 0x20 && fldGlobalBase.area != 0x25 &&
        fldGlobalBase.area != 0x24 && fldGlobalBase.area != 0x29 && fldGlobalBase.area != 0x2A &&
        fldGlobalBase.area != 0x2B && fldGlobalBase.area != 0x2C && fldGlobalBase.area != 0x2D) {
        if (fldGlobalBase.sequenceCode != pWap->sequenceCode) {
            result = fldBgmTransChk(fldGlobalBase.sequenceCode, pWap->sequenceCode);
            if (pWap->sequenceCode != 0) {
                if (result == 0) {
                    fldBgmFadeOut();
                }
                if (result == 1) {
                    gFldSndTransSrc = fldGlobalBase.sequenceCode;
                }
                fldGlobalBase.sequenceCode = pWap->sequenceCode;
                if (result == 1) {
                    fldBgmTrans();
                }
            } else if (result == 1) {
                gFldSndTransSrc = fldGlobalBase.sequenceCode;
                fldGlobalBase.sequenceCode = result;
                fldBgmTrans();
            }
        }
    }
    if (pWap->selectedRoom != 0) {
        fldGlobalBase.playerSeTable = pWap->selectedRoom - 1;
    }
    fldGlobalBase.cameraParamIndex = pWap->rowIndex;
    return result;
}

INCLUDE_ASM(const s32, "field/fldWap", fldDoor_Warp);

INCLUDE_RODATA(const s32, "field/fldWap", D_00531F18);

INCLUDE_RODATA(const s32, "field/fldWap", D_00531F28);

INCLUDE_RODATA(const s32, "field/fldWap", jtbl_00531F40);

INCLUDE_ASM(const s32, "field/fldWap", fldDoor_WarpEx);

INCLUDE_ASM(const s32, "field/fldWap", fldSetTerminalState);

INCLUDE_ASM(const s32, "field/fldWap", fldSetTerminalIndex);

void fldDoor_SiseExit(void) {
    fldSceneParam_t record;
    fldActorEntry_t *entry;
    s32 index;
    s32 state;
    s32 zone;
    s32 sub;
    s32 nextArea;
    s32 nextKind;
    s32 shortcut;

    if (D_0054EDF4 == 0) {
        return;
    }
    state = fldDoorSiseState;
    zone = D_0054EDFC;
    sub = D_0054EE00;
    fldDoorLoad(D_0054EDF4);
    for (index = 0; index < 0x100; index++) {
        entry = &fldDoorTable[index];
        if (entry->requiredFlag != 0 && evtBitCheck(entry->requiredFlag) == 0) {
            continue;
        }
        if (entry->kind != 7) {
            continue;
        }
        shortcut = (zone == 3 || zone == 4);
        if (shortcut == 0) {
            if (state != entry->floor) {
                continue;
            }
        }
        if (zone != entry->motion) {
            continue;
        }
        if (sub != entry->secondaryMotion) {
            continue;
        }
        if (entry->variantMode != 0) {
            continue;
        }
        nextArea = entry->variant;
        if (nextArea == 0) {
            nextArea = D_0054EDF4;
        }
        nextKind = entry->sequenceKind;
        if (nextKind == 0) {
            nextKind = fldDoorSiseState;
        }
        if (fldGlobalBase.area != nextArea) {
            fldBgmFadeOut();
        }
        fldWap_SetBgm(entry);
        if (entry->flags54 != 0) {
            fldDoorExitFlag = entry->flags54;
            strcpy((char *)fldDoorExitMode, entry->taskName);
            gFldWap_after_nextidx = index + 1;
        }
        gFldNowDZone = entry->flags31;
        fldSetSceneParam4(&record, nextArea, nextKind, entry->sequenceName, entry->sequenceCode,
                          entry->linkKind, entry->linkName);
        if (shortcut != 0) {
            record.callmode = 5;
        } else {
            record.callmode = 4;
        }
        D_0054EDF4 = 0;
        fldDoorSiseState = 0;
        D_0054EDFC = 0;
        D_0054EE00 = 0;
        dds3AdminiCallSequence(6, &record, 0xA0, 0);
        return;
    }
}

extern s16 D_002FC1A0[][2];

extern s16 fldAllCnt[][2];

extern void fldSetSceneParamLoad(fldSceneParam_t *pParam, s32 loadField, s32 loadArea, const char *pointResName,
                                 s32 bgm, s32 camMode, const char *camName);

void fldWap_Load(s32 tmnltype, s32 tmnlid, fldSceneParam_t *param) {
    fldActorEntry_t *entry;
    s32 i;
    s32 field;
    s32 zone;
    s32 nextKind;

    if (tmnltype == 0) {
        field = D_002FC1A0[tmnlid][0];
        zone = 3;
    } else {
        zone = 4;
        field = fldAllCnt[tmnlid][0];
    }
    if (field == 0) return;
    fldDoorLoad(field);
    for (i = 0; i < 0x100; i++) {
        entry = &fldDoorTable[i];
        if (entry->requiredFlag != 0 && evtBitCheck(entry->requiredFlag) == 0) continue;
        if (entry->kind != 7) continue;
        if (zone != entry->motion) continue;
        if (tmnlid != entry->secondaryMotion) continue;
        if (entry->variantMode != 0) continue;
        fldGlobalBase.cameraParamIndex = entry->rowIndex;
        if (entry->flags54 != 0) {
            fldDoorExitFlag = entry->flags54;
            strcpy((char *)fldDoorExitMode, entry->taskName);
            gFldWap_after_nextidx = i + 1;
        }
        if (entry->variant != 0) {
            field = entry->variant;
        }
        if (entry->sequenceKind != 0) {
            nextKind = entry->sequenceKind;
        } else {
            nextKind = 1;
        }
        gFldNowDZone = entry->flags31;
        if (field == 0) return;
        if (nextKind == 0) return;
        fldSetSceneParamLoad(param, field, nextKind, entry->sequenceName, entry->sequenceCode,
                             entry->linkKind, entry->linkName);
        param->callmode = 5;
        if (fldGlobalBase.sequenceCode != entry->sequenceCode) {
            if (entry->sequenceCode != 0) {
                fldGlobalBase.sequenceCode = entry->sequenceCode;
            }
        }
        if (entry->selectedRoom != 0) {
            fldGlobalBase.playerSeTable = entry->selectedRoom - 1;
        }
        return;
    }
}

extern s16 D_002FC1A2[][2];

extern void fldSetNextField(s32 field, s32 parameter);

extern void fldAutoMapOpenBlock2(s32 fld, s32 area, s32 SBlock);

extern void evtBitOn(s32 no);

void fldDoor_EveBtlExit(s32 field, s32 eveno) {
    fldSceneParam_t record;
    s32 block[4];
    s32 param[4];
    fldActorEntry_t *entry;
    s32 i;
    s32 nextArea;
    s32 nextKind;

    fldDoorLoad(field);
    for (i = 0; i < 0x100; i++) {
        entry = &fldDoorTable[i];
        if (entry->requiredFlag != 0 && evtBitCheck(entry->requiredFlag) == 0) continue;
        if (entry->kind != 8) continue;
        if (entry->motion != eveno) continue;
        if (entry->variantMode == 2) {
            fldSetNextField(0, 0);
            fldBgmFadeOut();
            fldAutoMapOpenBlock2(fldGlobalBase.area, fldGlobalBase.floor + 1, entry->sequenceValue);
            D_0054EDF4 = fldGlobalBase.area;
            fldDoorSiseState = fldGlobalBase.floor + 1;
            D_0054EDFC = entry->variant;
            D_0054EE00 = entry->sequenceKind;
            switch ((u32)entry->variant) {
            case 0:
                param[0] = entry->sequenceKind;
                dds3AdminiCallSequence(0x1C, param, 4, 0);
                return;
            case 1:
                param[0] = entry->sequenceKind;
                dds3AdminiCallSequence(0x1D, param, 4, 0);
                return;
            case 2:
                param[0] = entry->sequenceKind;
                dds3AdminiCallSequence(0x1B, param, 4, 0);
                return;
            case 3:
                block[0] = 0;
                block[1] = entry->sequenceKind;
                if (D_002FC1A2[block[1]][0] != 0) {
                    evtBitOn(D_002FC1A2[block[1]][0]);
                }
                DDS3_GBWK->previewX = block[0];
                DDS3_GBWK->previewY = block[1];
                dds3AdminiCallSequence(0x1F, block, 8, 0);
                return;
            case 4:
                block[0] = 1;
                block[1] = entry->sequenceKind;
                DDS3_GBWK->previewX = block[0];
                DDS3_GBWK->previewY = block[1];
                dds3AdminiCallSequence(0x1F, block, 8, 0);
                return;
            case 5:
                dds3AdminiCallSequence(0x1E, 0, 0, 0);
                return;
            }
            return;
        }
        if (entry->variantMode != 0) continue;
        nextArea = field;
        if (entry->variant != 0) {
            nextArea = entry->variant;
        }
        nextKind = entry->sequenceKind;
        if (fldGb != nextArea) {
            fldBgmFadeOut();
        }
        fldWap_SetBgm(entry);
        if (entry->flags54 != 0) {
            fldDoorExitFlag = entry->flags54;
            strcpy((char *)fldDoorExitMode, entry->taskName);
            gFldWap_after_nextidx = i + 1;
        }
        gFldNowDZone = entry->flags31;
        fldSetSceneParam4(&record, nextArea, nextKind, entry->sequenceName, entry->sequenceCode,
                          entry->linkKind, entry->linkName);
        record.callmode = 6;
        dds3AdminiCallSequence(6, &record, 0xA0, 0);
        return;
    }
}

void fldDoor_WarpCash(const char *evename) {
    fldActorEntry_t *entry;
    s32 area;
    s32 room;
    s32 i;

    if (evename == NULL) {
        return;
    }
    for (i = 0; i < 0x100; i++) {
        entry = &fldDoorParamTable.actors[i];
        if (entry->requiredFlag != 0 && evtBitCheck(entry->requiredFlag) == 0) {
            continue;
        }
        if (entry->kind == 1 && entry->variantMode == 0) {
            if (entry->floor == fldGlobalBase.floor + 1 && strcmp(evename, entry->name) == 0) {
                area = entry->variant ? entry->variant : fldGlobalBase.area;
                room = entry->sequenceKind;
                if (room == 0) {
                    room = fldGlobalBase.floor + 1;
                }
                fldLoadNextFile(area, room);
                return;
            }
        } else if (entry->kind == 2 && entry->variantMode == 0) {
            if (entry->floor == fldGlobalBase.floor + 1 && strcmp(evename, entry->name) == 0) {
                area = entry->variant ? entry->variant : fldGlobalBase.area;
                room = entry->sequenceKind;
                if (room == 0) {
                    room = fldGlobalBase.floor + 1;
                }
                fldLoadNextFile(area, room);
                return;
            }
        } else if (entry->kind == 3 && entry->variantMode == 0) {
            if (entry->floor == fldGlobalBase.floor + 1 && strcmp(evename, entry->name) == 0) {
                area = entry->variant ? entry->variant : fldGlobalBase.area;
                room = entry->sequenceKind;
                if (room == 0) {
                    room = fldGlobalBase.floor + 1;
                }
                fldLoadNextFile(area, room);
                return;
            }
        } else if (entry->kind == 4 && entry->variantMode == 0) {
            if (entry->floor == fldGlobalBase.floor + 1 && strcmp(evename, entry->name) == 0) {
                area = entry->variant ? entry->variant : fldGlobalBase.area;
                room = entry->sequenceKind;
                if (room == 0) {
                    room = fldGlobalBase.floor + 1;
                }
                fldLoadNextFile(area, room);
                return;
            }
        } else if (entry->kind == 5 && entry->variantMode == 0) {
            if (entry->floor == fldGlobalBase.floor + 1 && strcmp(evename, entry->name) == 0) {
                area = entry->variant ? entry->variant : fldGlobalBase.area;
                room = entry->sequenceKind;
                if (room == 0) {
                    room = fldGlobalBase.floor + 1;
                }
                fldLoadNextFile(area, room);
                return;
            }
        }
    }
}

s32 fldDoor_GetHasigoTbl(s32 getmode) {
    fldActorEntry_t *entry = &fldDoorTable[scriptlabel];
    dds3Basic_t *node;

    switch (getmode) {
    case 0:
        return entry->motion;
    case 1:
        node = dds3SceneBasicSearchName(dds3GetActiveScene(), entry->motionName);
        if (node != NULL) {
            return node->serialID;
        }
        /* fallthrough */
    case 2:
        node = dds3SceneBasicSearchName(dds3GetActiveScene(), entry->otherName);
        if (node != NULL) {
            return node->serialID;
        }
        break;
    }
    return 0;
}

s32 fldDoor_GetElvTbl(u32 getmode) {
    s32 slot = gFld_ElvTakasa;

    switch (getmode) {
    case 0:
        return fldDoorParamTable.headers[slot].floorCount;
    case 1:
        return fldDoorParamTable.headers[slot].sound;
    case 2:
        return fldDoorParamTable.headers[slot].body.data[0];
    case 3:
        return fldDoorParamTable.headers[slot].body.data[1];
    case 4:
        return fldDoorParamTable.headers[slot].body.data[2];
    case 5:
        return fldDoorParamTable.headers[slot].body.data[3];
    case 6:
        return fldDoorParamTable.headers[slot].body.data[4];
    case 7:
        return fldDoorParamTable.headers[slot].body.data[5];
    case 8:
        return fldDoorParamTable.headers[slot].body.data[6];
    case 9:
        return fldDoorParamTable.headers[slot].body.data[7];
    case 10:
        return fldDoorParamTable.headers[slot].body.data[8];
    case 11:
        return fldDoorParamTable.headers[slot].body.data[9];
    case 12:
        return fldDoorParamTable.headers[slot].body.data[10];
    case 13:
        return fldDoorParamTable.headers[slot].body.data[11];
    case 14:
        return fldDoorParamTable.headers[slot].floorCount - fldDoorWarpTarget - 1;
    }
    return 0;
}

s32 fldDoor_ElvTakasaToFloor(s32 takasa) {
    s32 slot = gFld_ElvTakasa;
    s32 count = fldDoorParamTable.headers[slot].floorCount;

    if (count - 1 < takasa) {
        return count - fldDoorWarpTarget - 1;
    }
    takasa = count - takasa - 1;
    return fldDoorParamTable.headers[slot].body.data[takasa];
}

extern s16 gFld_ElvId[];

void fldDoor_ElvExit(s32 select) {
    fldSceneParam_t record;
    s32 param[4];
    fldActorEntry_t *entry;
    s32 i;
    s32 count;
    s32 target;
    s32 nextArea;
    s32 nextKind;

    count = gFld_ElvId[gFld_ElvTakasa * 16 + 1];
    if (select >= count) {
        target = fldDoorWarpTarget;
    } else {
        target = count - select - 1;
    }
    for (i = 0; i < 0x100; i++) {
        entry = &fldDoorTable[i];
        if (entry->requiredFlag != 0 && evtBitCheck(entry->requiredFlag) == 0) {
            continue;
        }
        if (entry->kind != 6) {
            continue;
        }
        if (entry->motion - 1 == gFld_ElvTakasa && entry->secondaryMotion == target &&
            entry->variantMode == 0) {
            if (entry->variant == 0) {
                nextArea = fldGlobalBase.area;
            } else {
                nextArea = entry->variant;
            }
            if (entry->sequenceKind == 0) {
                nextKind = fldGlobalBase.floor + 1;
            } else {
                nextKind = entry->sequenceKind;
            }
            if (fldGlobalBase.area != nextArea) {
                fldBgmFadeOut();
            }
            fldWap_SetBgm(entry);
            if (entry->flags54 != 0) {
                fldDoorExitFlag = entry->flags54;
                strcpy((char *)fldDoorExitMode, entry->taskName);
                gFldWap_after_nextidx = i + 1;
            }
            gFldNowDZone = entry->flags31;
            fldAutoMapOpenBlock2(nextArea, nextKind,
                                 gFld_ElvId[gFld_ElvTakasa * 16 + target + 8]);
            fldSetSceneParam4(&record, nextArea, nextKind, entry->sequenceName,
                              entry->sequenceCode, entry->linkKind, entry->linkName);
            dds3AdminiCallSequence(6, &record, 0xA0, 0);
            return;
        }
        if (entry->kind == 6 && entry->motion - 1 == gFld_ElvTakasa &&
            entry->secondaryMotion == target && entry->variantMode == 3) {
            param[0] = entry->variant;
            fldSetNextField(fldGlobalBase.area, entry->variant);
            fldBgmFadeOut();
            if (entry->sequenceKind == 0 && entry->sequenceValue != 0) {
                fldSetNextField(entry->sequenceValue, param[0]);
            }
            dds3AdminiCallSequence(7, param, 4, 0);
            return;
        }
    }
}

void fldClearDoorEvents(void) {
    fldDoorEvent_t *entry = fldDoorEventTable;
    s32 i;

    for (i = 0; i < 256; i++, entry++) {
        entry->unk00 = 0;
        entry->unk04 = 0;
        entry->unk08 = 0;
        entry->unk0C = -1;
        entry->unk10 = 0;
        entry->unk14 = -1;
        entry->unk18 = 0;
        entry->unk1C = 0;
        entry->unk24 = 0;
        entry->unk28 = 0;
        entry->unk2C = 0;
        entry->unk30 = 0;
        entry->unk34 = 0;
        entry->unk38 = 0;
        entry->unk3C = 0;
        entry->unk40 = 0;
        entry->unk44 = 0;
        entry->unk48 = 0;
        entry->unk4C = 0;
        entry->unk50 = 0;
    }
    scriptlabel = -1;
}

void fldDoorLoad(s32 field) {
    char path[64];
    char directory[32];
    devState_t *command;
    if (field >= 100) {
        memset(&fldDoorParamTable, 0, sizeof(fldDoorParamTable));
    } else {
        fldMakeFilePath(directory, field, 1);
        sprintf(path, "%sF%03d.WAP", directory, field);
        command = sdfFileOpen(path);
        sdfFileRead(command, &fldDoorParamTable, sizeof(fldDoorParamTable));
        sdfFileClose(command);
    }
}

void fldGetDoorEntry(const void *source) {
    memcpy(&fldDoorParamTable, source, sizeof(fldDoorParamTable));
}

INCLUDE_ASM(const s32, "field/fldWap", fldSetupDoorScenes);

void fldFindDoorEvent(s32 id) {
    fldDoorEvent_t *e;
    fldActorEntry_t *entry;
    s32 i;

    if (id == 0) {
        return;
    }
    if (id == -1) {
        return;
    }
    e = fldDoorEventTable;
    entry = fldDoorTable;
    for (i = 0; i < 256; i++) {
        if (e->unk00 == 1 && e->unk04 == id) {
            s32 motion = entry->motion;

            e->unk00 = 2;
            e->unk18 = 0;
            e->unk1C = 0;
            if (motion == 5 || entry->secondaryMotion == 5 || motion == 6 ||
                entry->secondaryMotion == 6 || motion == 7 ||
                entry->secondaryMotion == 7 || motion == 8 ||
                entry->secondaryMotion == 8) {
                e->unk20 = 0x28;
            } else {
                e->unk20 = 0x14;
            }
            e->unk24 = 0.0f;
            e->unk28 = 0.0f;
            e->unk2C = 0.0f;
            e->unk3C = 0.0f;
            e->unk40 = 0.0f;
            e->unk44 = 0.0f;
        }
        e++;
        entry++;
    }
}

s32 fldDoor_Step(s32 matter_resid, f32 *x, f32 *y, f32 *z) {
    f32 steps[50] = {
        0.5f, 1.4f, 1.0f, 0.5f, 1.0f, 0.6f, 1.2f, 0.8f, 1.2f, 0.8f,
        0.5f, 1.4f, 1.0f, 0.5f, 1.0f, 0.6f, 1.2f, 0.8f, 1.2f, 0.8f,
        0.5f, 1.4f, 1.0f, 0.5f, 1.0f, 0.6f, 1.2f, 0.8f, 1.2f, 0.8f,
        0.5f, 1.4f, 1.0f, 0.5f, 1.0f, 0.6f, 1.2f, 0.8f, 1.2f, 0.8f,
        0.5f, 1.4f, 1.0f, 0.5f, 1.0f, 0.6f, 1.2f, 0.8f, 1.2f, 0.8f,
    };
    fldDoorEvent_t *e;
    s32 i;

    if (fldGb >= 200) {
        return 0;
    }
    for (i = 0; i < 256; i++) {
        e = (fldDoorEvent_t *)((u8 *)(i * 0x54) + (u32)fldDoorEventTable);
        if (e->unk0C == matter_resid) {
            if (e->unk00 == 2) {
                s32 count = e->unk18;
                s32 limit = e->unk20;
                if (count < limit) {
                    if (limit == 0x28) {
                        f32 step = steps[count];
                        e->unk24 += e->unk30 * step;
                        e->unk28 += e->unk34 * step;
                        e->unk2C += e->unk38 * step;
                    } else {
                        e->unk24 += e->unk30;
                        e->unk28 += e->unk34;
                        e->unk2C += e->unk38;
                        if (evtBitCheck(0xAF3) != 0) {
                            e->unk24 += e->unk30;
                            e->unk28 += e->unk34;
                            e->unk2C += e->unk38;
                        }
                    }
                    e->unk18 = e->unk18 + 1;
                } else if (evtBitCheck(0xAF3) != 0) {
                    e->unk00 = 3;
                    e->unk18 = 0;
                }
            } else if (e->unk00 == 3) {
                if (e->unk18 < 0x14) {
                    e->unk24 -= e->unk30;
                    e->unk28 -= e->unk34;
                    e->unk2C -= e->unk38;
                    if (evtBitCheck(0xAF3) != 0) {
                        e->unk24 -= e->unk30;
                        e->unk28 -= e->unk34;
                        e->unk2C -= e->unk38;
                    }
                    e->unk18 = e->unk18 + 1;
                } else {
                    e->unk18 = 0;
                    e->unk00 = 1;
                }
            } else {
                continue;
            }
            *x = e->unk24;
            *y = e->unk28;
            *z = e->unk2C;
            return 1;
        }
        if (e->unk14 == matter_resid) {
            if (e->unk00 == 2) {
                s32 count = e->unk1C;
                s32 limit = e->unk20;
                if (count < limit) {
                    if (limit == 0x28) {
                        f32 step = steps[count];
                        e->unk3C += e->unk48 * step;
                        e->unk40 += e->unk4C * step;
                        e->unk44 += e->unk50 * step;
                    } else {
                        e->unk3C += e->unk48;
                        e->unk40 += e->unk4C;
                        e->unk44 += e->unk50;
                    }
                    e->unk1C = count + 1;
                } else if (evtBitCheck(0xAF3) != 0) {
                    e->unk1C = 0;
                }
            } else if (e->unk00 == 3) {
                if (e->unk1C < 0x14) {
                    e->unk3C -= e->unk48;
                    e->unk40 -= e->unk4C;
                    e->unk44 -= e->unk50;
                    e->unk1C = e->unk1C + 1;
                } else {
                    e->unk1C = 0;
                }
            } else {
                continue;
            }
            *x = e->unk3C;
            *y = e->unk40;
            *z = e->unk44;
            return 1;
        }
    }
    return 0;
}

void func_00144288(void) {
}

INCLUDE_SDATA(const s32, "field/fldWap", gFldWap_after_nextidx);

INCLUDE_SDATA(const s32, "field/fldWap", gFld_ElvTakasa);

INCLUDE_SDATA(const s32, "field/fldWap", fldDoorWarpTarget);

INCLUDE_SDATA(const s32, "field/fldWap", D_0054EDF4);

INCLUDE_SDATA(const s32, "field/fldWap", fldDoorSiseState);

INCLUDE_SDATA(const s32, "field/fldWap", D_0054EDFC);

INCLUDE_SDATA(const s32, "field/fldWap", D_0054EE00);

INCLUDE_SDATA(const s32, "field/fldWap", scriptlabel);

INCLUDE_SDATA(const s32, "field/fldWap", gFldWapPanel);

INCLUDE_SDATA(const s32, "field/fldWap", D_0054EE0C);

