#include "common.h"
#include "basic.h"
#include "kernel.h"
#include "libsdf.h"
#include "ps2asm.h"
#include "newdata.h"
#include "field.h"

extern char D_0054EA38[], D_0054EA40[], D_0054EA48[], D_0054EA50[], D_0054EA58[];

extern dds3Basic_t *dds3GetControlScene(void);

typedef struct {
    u8 x;
    u8 y;
    u8 z;
    u8 w;
    u8 unk04[0xC];
} fldMapCell_t;

typedef struct {
    u32 firstOffset;
    u32 cellCount;
    u32 secondOffset;
} fldRunMapHeader_t;

extern void *mhfldWaveFile;

extern void *mhfldPlayerFile;

extern void *mhfldAnaFile;

extern s32 gFldRndEncBtl;

extern s32 fldLoadNextGetStatus2(void);

extern s32 fldLoadNextGetStatus(void);

extern void fldLoadNextTask(void);

extern void fldFreeNextFile(void);

extern s32 fldBgmIsLoading(void);

extern void fldWaitBgmLoad(void);

extern s32 sdfDelayedResrcCheck(void);

extern s32 fldNext_fld;

extern s32 D_0054EB7C;

extern s32 gFldAnahoriAkumaIdx;

extern s32 gFldAnahoriAkumaIdxMax;

extern s16 fldRun_MAP_OX;

extern s16 fldRun_MAP_OZ;

extern s16 fldRun_MAP_BX;

extern s16 fldRun_MAP_BZ;

extern s32 dds3AdminiGetNextSequence(void);

extern s32 dds3AdminiGetPrevHistoryFlag(void);

extern void fldDoor_EveBtlExit(s32 fld, s32 arg1);

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
    u32 flags;
    s32 stage;
} fldSequenceController_t;

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

extern s32 fldProcShutDownFlg;

extern s32 ProcSequence(dds3ProcessID_t *process);

extern s32 ProcDraw(dds3ProcessID_t *process);

extern void fldFreeSnowSeed(void);

extern void fldAutoMapSeqSetFlag(u32 flag);

extern void fldFreeInputPanel(void);

extern void evtDisableMoonDraw(void);

extern void fldFreeAddressProcess(void);

extern void dds3EffectDistortionBlurDelete(void *blur);

extern void campKillProcesses(void);

extern void scrAllReleaseScript(void);

extern dds3Basic_t *fldAnaBasic;

extern void *hDistortionBlur;

extern char D_00531950[];

extern s32 gfldPlayerSize;

extern void *gfldAnaPtr;

extern s32 gfldAnaSize;

extern void *gfldWavePtr;

extern s32 gfldWaveSize;

extern void *gfldPlayerPtr;

extern s32 gfldPlayerSizeBk;

extern void dds3AdminiCallSequence(s32 sequence, void *data, s32 size, s32 mode);

extern void fldSetNextField(s32 field, s32 parameter);

extern fldSceneParam_t fldSceneParam;

extern s32 gFldNowDZone;

extern s32 fldProcCampWait;

extern s32 fldJDataHandle __attribute__((section(".data")));

extern s32 campCheckExitState(void);

extern void fldEncountClearFlag(u32 flag);

extern dds3Basic_t *D_0054EAB0;

extern dds3Basic_t *fldRunTama2Basic;

extern dds3Basic_t *fldRunTama3Basic;

extern s32 D_0054EABC;

extern void evtEnableMoonDraw(void);

extern void fldSkyRefreshTbl(void);

extern char D_0054EB40[];

extern void fldStatusSet(void);

extern dds3Basic_t *dds3GetActiveScene(void);

extern void dds3VisibleSceneBasic(dds3Basic_t *scene, s32 flag);

extern u8 cmpGetLoadState(void);

extern void dds3FadeOut2(s32 frame);

extern void evtKillFieldScript(void);

extern void p2sdStopAllSe(void);

extern void dds3FadeIn2(s32 frame);

extern s32 fldPlayerChkAna(void);

extern void p2sdPlaySE(s32 id, s32 vol, s32 pan);

extern void fldPlayerPosRot(void);

extern void dds3FadeOut(s32 red, s32 green, s32 blue, s32 frame);

extern void fldPlayerMotReset(void);

extern void cmpProcessStart(void);

extern s32 dds3SceneBasicStartScript(dds3Basic_t *scene, const char *name);

extern s32 fldLightMaState __attribute__((section(".data")));

extern s32 fldChkVortex(void);

extern s32 fldProcCampChk(void);

extern s32 fldProcAmChk(void);

extern s32 fldEncountTestFlag(u32 flag);

extern s32 fldAutoMapSeqTestFlag(u32 flag);

extern void fldAutoMapSeqClearFlag(u32 flag);

extern void fldEncountSetFlag(u32 flag);

extern void fldReleaseUnitPad(void);

extern void fldAutoMapSeqStart(void);

typedef struct scrProcessWork_s scrProcessWork_s;

typedef struct dds3FontReq_t dds3FontReq_t;

/* destroy runs when the request leaves the list, draw once per frame while it
 * is still on it. */
typedef struct
{
    void (*destroy)(dds3FontReq_t *req);
    void (*draw)(dds3FontReq_t *req);
} dds3FontReqFunc_t;

struct dds3FontReq_t
{
    dds3FontReq_t *prev;
    dds3FontReq_t *next;
    s32 group;
    dds3FontReqFunc_t *func;
};

extern dds3FontReqFunc_t dds3FontReqTable;

extern dds3FontReqFunc_t D_0054EA30;

extern u8 dds3PadButtonState[];

extern char D_00531860[];

extern fldSequenceController_t fldAutoMapSeqFlags;

extern u32 fldEncountFlags;

extern s32 fldRunDanteSe_asise;

extern s8 slNewGameBit2208, slNewGameBit2209, slNewGameBit2210, slNewGameBit2211;

extern s8 slNewGameBit2212, slNewGameBit2213, slNewGameBit2323, slNewGameBit162;

extern u16 D_0054EA16;

extern s32 D_0054EA18;

extern sdfMemHandle_t *D_0054EA1C;

extern s32 gFldAnahoriAkumaIdx;

extern s32 gFldAnahoriAkumaIdxMax;

extern s32 fldProcAmWait;

extern s32 fldNext_fld;

extern s32 D_0054EB7C;

extern s32 fldNextFieldId;

extern s32 fldElvState;

extern s32 fldElvFloorTarget;

extern s32 gFldElvFloorNow;

extern s32 fldElvFloorFrom;

extern s32 fldElvFloorNum;

extern s32 fldElvFloorTo;

extern s32 fldElvFloorDisp;

extern sdfMemHandle_t *fldPlayerAnimData;

extern sdfMemHandle_t *fldRunMapData;

extern fldRunMapHeader_t *fldRunMapHeader;

extern fldMapCell_t *fldRunMapCellData;

extern void *pFroMapHead;

extern void *fldRunMapScale;

extern dds3ProcessID_t *dds3StartProcess(char *namestr, u32 priority, s32 starttimer,
                                         s32 shutdowntimer, dds3ProcessFunc_t processfunc,
                                         dds3ProcessFunc_t shutdownfunc, void *data);

extern s32 dds3KillProcessName(char *name, s32 shutdowntimer);

extern scrProcessWork_s *scrSearchScriptName(char *scriptname);

extern s32 fldAutoMapChkMode(void);

extern void fldSetTerminalState(s32 type, s32 idx);

extern void fldPauseBgm(s32 fld, s32 area, void *name);

extern void fldProcNextJump(void);

extern s32 fldDoor_SiseExit(void);

extern void fldEveHit_End(void);

extern void fldProcShutDown(void);

void InitOoyTest(void);

void fldRegistUnitPad(void);

INCLUDE_RODATA(const s32, "field/fldProcess", D_00531860);

INCLUDE_ASM(const s32, "field/fldProcess", ProcRandomEncountCheck);

s32 fldProcKiretaChk(void)
{
    return scrSearchScriptName(D_00531860) != NULL;
}

s32 fldProcHirakenaiChk(void)
{
    return scrSearchScriptName("hirakenai_label") != NULL;
}

s32 fldProcCampChk(void)
{
    if (fldProcCampWait > 0) {
        return 2;
    }
    if (campCheckExitState() != 0) {
        return 1;
    }
    return fldEncountTestFlag(0x20) != 0 ? 0 : 3;
}

s32 ProcCampCheck(void)
{
    if (fldChkVortex() == 0) {
        return 0;
    }
    if (fldProcCampWait > 0) {
        fldProcCampWait = fldProcCampWait - 1;
        if (fldProcCampWait == 0) {
            dds3VisibleSceneBasic(dds3GetActiveScene(), 0);
            dds3FadeIn2(4);
            cmpProcessStart();
        }
        return 0;
    }
    if (fldEncountTestFlag(0x20) == 0) {
        if (fldProcCampChk() == 3) {
            fldAutoMapSeqClearFlag(1);
            fldRegistUnitPad();
            evtEnableMoonDraw();
            if (fldLightMaState != 0) {
                fldSkyRefreshTbl();
            } else {
                fldSkyRefreshTbl();
            }
            fldStatusSet();
            dds3VisibleSceneBasic(dds3GetActiveScene(), 1);
            fldEncountSetFlag(0x20);
            fldEncountClearFlag(0x80);
            if (cmpGetLoadState() == 1) {
                dds3FadeOut2(0);
                fldEveHit_End();
                fldAutoMapSeqSetFlag(1);
                fldAutoMapSeqSetFlag(2);
                fldReleaseUnitPad();
                evtKillFieldScript();
                p2sdStopAllSe();
                dds3AdminiCallSequence(4, NULL, 0, 0);
                return 0;
            }
            dds3FadeOut2(0);
            dds3FadeIn2(8);
            return 0;
        }
    }
    if (fldEncountTestFlag(0x20) == 0) {
        return 0;
    }
    if (fldEncountTestFlag(0x40) == 0) {
        return 0;
    }
    if (fldAutoMapSeqTestFlag(1) == 1) {
        return 0;
    }
    if (fldPlayerChkAna() != 0) {
        return 0;
    }
    if (fldAutoMapChkMode() != 0) {
        return 0;
    }
    if (fldGlobalBase.menuOpenFlag != 0) {
        goto ret0;
    }
    if (((s8 *)dds3PadButtonState)[0x20] >= 0) {
        goto ret0;
    }
    p2sdPlaySE(0xE, 0x7F, 0x3F);
    fldAutoMapSeqSetFlag(1);
    fldReleaseUnitPad();
    fldPlayerPosRot();
    evtDisableMoonDraw();
    fldGlobalBase.menuOpenFlag = 4;
    fldGlobalBase.encountState = 5;
    fldEncountClearFlag(0x20);
    fldEncountSetFlag(0x80);
    dds3FadeOut(0, 0, 0, 4);
    fldProcCampWait = 5;
    fldPlayerMotReset();
ret0:
    return 0;
}

s32 fldProcAmChk(void)
{
    if (fldProcAmWait > 0) {
        return 2;
    }
    return fldAutoMapChkMode() != 0;
}

s32 ProcAmCheck(void)
{
    s32 trigger = 0;

    if (fldGlobalBase.area < 0xA) {
        return 0;
    }
    if (fldProcCampChk() != 0) {
        return 0;
    }
    if (fldProcAmChk() == 0) {
        if (fldEncountTestFlag(0x40) == 0) {
            return 0;
        }
    }
    if ((fldRunDanteGunFlag.unk58 & fldEncountTestFlag(0x40)) != 0) {
        if (((s8 *)dds3PadButtonState)[0x22] != 0) {
            trigger = 1;
        }
    } else {
        if (((s8 *)dds3PadButtonState)[0x22] < 0) {
            trigger = 1;
        } else {
            trigger = 0;
        }
    }
    if (fldGlobalBase.area == 0x26 ||
        (fldGlobalBase.area == 0x2C && fldGlobalBase.floor + 1 >= 0xC &&
         fldGlobalBase.floor + 1 < 0x1C) ||
        (fldGlobalBase.area == 0x2C && fldGlobalBase.floor == 0x1E) ||
        (fldGlobalBase.area == 0x2D && fldGlobalBase.floor == 0x1D) ||
        (fldGlobalBase.area == 0x2D && fldGlobalBase.floor == 0x1E)) {
        if (fldGlobalBase.menuOpenFlag != 0) {
            goto ret0;
        }
        if (fldEncountTestFlag(0x40) == 0) {
            goto ret0;
        }
        if (trigger != 0) {
            fldGlobalBase.menuOpenFlag = 4;
            fldGlobalBase.encountState = 5;
            fldReleaseUnitPad();
            dds3SceneBasicStartScript(dds3GetActiveScene(), "hirakenai_label");
        }
        goto ret0;
    }
    if (fldProcAmWait > 0) {
        fldProcAmWait = fldProcAmWait - 1;
        if (fldProcAmWait == 0) {
            dds3VisibleSceneBasic(dds3GetActiveScene(), 0);
            dds3FadeIn2(4);
            fldAutoMapSeqStart();
        }
        goto ret0;
    }
    if (fldProcAmChk() != 0) {
        goto ret0;
    }
    if (fldGlobalBase.menuOpenFlag != 0) {
        goto ret0;
    }
    if (fldPlayerChkAna() != 0) {
        goto ret0;
    }
    if (fldEncountTestFlag(0x40) == 0) {
        goto ret0;
    }
    if (trigger != 0) {
        if (fldRunDanteGunFlag.unk48 != 0) {
            goto ret0;
        }
        fldGlobalBase.menuOpenFlag = 4;
        fldGlobalBase.encountState = 5;
        fldReleaseUnitPad();
        fldPlayerPosRot();
        dds3FadeOut(0, 0, 0, 4);
        fldProcAmWait = 5;
        fldPlayerMotReset();
    }
    ret0:
    return 0;
}

s32 RetryMapLoad(void)
{
    fldSceneParam_t *param = &fldSceneParam;
    s32 flag;

    if ((fldDoorExitFlag & 2) && (s8)fldDoorExitMode[0] != 0) {
        dds3SceneBasicStartScript(dds3GetControlScene(), (const char *)fldDoorExitMode);
        flag = fldDoorExitFlag;
        if ((flag & 1) == 0) {
            fldDoorExitMode[0] = 0;
            fldDoorExitFlag = flag & 0xFD;
        }
        return 1;
    }
    if (param->mode == 0 && param->note[0] != 0) {
        dds3SceneBasicStartScript(dds3GetControlScene(), param->note);
        return 1;
    }
    if (fldGlobalBase.retryScript == NULL) {
        goto ret0;
    }
    dds3SceneBasicStartScript(dds3GetControlScene(), fldGlobalBase.retryScript);
    return 1;
ret0:
    return 0;
}

INCLUDE_ASM(const s32, "field/fldProcess", ProcSequence);

INCLUDE_ASM(const s32, "field/fldProcess", ProcDraw);

void fldAutoMapSeqSetFlag(u32 flag)
{
    fldAutoMapSeqFlags.flags |= flag;
}

void fldAutoMapSeqClearFlag(u32 flag)
{
    fldAutoMapSeqFlags.flags &= ~flag;
}

s32 fldAutoMapSeqTestFlag(u32 flag)
{
    return (fldAutoMapSeqFlags.flags & flag) != 0;
}

void fldEncountSetFlag(u32 flag)
{
    fldEncountFlags |= flag;
}

void fldEncountClearFlag(u32 flag)
{
    fldEncountFlags &= ~flag;
}

s32 fldEncountTestFlag(u32 flag)
{
    return (fldEncountFlags & flag) != 0;
}

void fldProcTrigSequence(fldSceneParam_t *record, u32 mode) {
    fldSceneParam_t *scene = &fldSceneParam;
    fldSequenceController_t *controller = &fldAutoMapSeqFlags;

    controller->stage = 0;
    controller->flags = 0;
    scene->mode = mode;
    if (mode == 0) {
        scene->sequenceWord3C = record->sequenceWord3C;
        scene->sequenceWord38 = record->sequenceWord38;
        strcpy(scene->name, record->name);
        scene->stage = record->stage;
        scene->code = record->code;
        scene->kind = record->kind;
        scene->sequenceMode = record->sequenceMode;
        scene->enabled = record->enabled;
        scene->restore = record->restore;
        scene->link = record->link;
        strcpy(scene->detail, record->detail);
        strcpy(scene->note, record->note);
        scene->options = record->options;
    }
    dds3StartProcess("fldProcSequence", 0x3F7, 0, 0, ProcSequence, 0, 0);
    dds3StartProcess(D_00531950, 0x2B0A, 0, 0, ProcDraw, 0, 0);
    fldProcShutDownFlg = 1;
    InitOoyTest();
}

void fldProcShutDown(void)
{
    if (fldProcShutDownFlg == 0) {
        return;
    }
    fldProcShutDownFlg = 0;
    fldFreeSnowSeed();
    fldEncountClearFlag(0x10);
    fldEncountClearFlag(0x20);
    fldEncountClearFlag(0x80);
    fldAutoMapSeqSetFlag(1);
    fldAutoMapSeqSetFlag(2);
    dds3KillProcessName("fldProcSequence", 1);
    dds3KillProcessName(D_00531950, 1);
    fldFreeInputPanel();
    evtDisableMoonDraw();
    fldFreeAddressProcess();
    if (hDistortionBlur != NULL) {
        dds3EffectDistortionBlurDelete(hDistortionBlur);
        hDistortionBlur = NULL;
    }
    campKillProcesses();
    scrAllReleaseScript();
}

s32 fldProcShutDown2(void)
{
    if (fldJDataHandle == 0) {
        if (fldLoadNextGetStatus2() != 0) {
            fldLoadNextTask();
            if (fldLoadNextGetStatus() == 1) {
                return 0;
            }
            fldFreeNextFile();
            return 0;
        }
    }
    if (fldBgmIsLoading() != 0) {
        fldWaitBgmLoad();
        return 0;
    }
    return sdfDelayedResrcCheck() == 0;
}

void fldProcSetTerminal(s32 type, s32 idx)
{
    fldSetTerminalState(type, idx);
}

void fldProc_SisetuReturnPoint(s32 fld, s32 area, void *pName)
{
    fldPauseBgm(fld, area, pName);
}

void fldProc_CallSisetuReturn(void)
{
    if (fldNext_fld != 0) {
        fldProcNextJump();
    } else {
        fldDoor_SiseExit();
    }
}

void fldSetNextField(s32 fld, s32 area)
{
    fldNext_fld = fld;
    D_0054EB7C = area;
    if (fld == 14 && area == 0x25B) {
        fldNextFieldId = 1;
    }
}

void fldProcNextJump(void)
{
    if (fldNext_fld != 0) {
        if (dds3AdminiGetNextSequence() > 0) {
            return;
        }
        if (dds3AdminiGetNextSequence() < 0) {
            if (dds3AdminiGetPrevHistoryFlag() & 1) {
                return;
            }
        }
        fldDoor_EveBtlExit(fldNext_fld, D_0054EB7C);
        fldNext_fld = 0;
    }
}

INCLUDE_RODATA(const s32, "field/fldProcess", D_00531950);

INCLUDE_SDATA(const s32, "field/fldProcess", fldNextFieldId);

INCLUDE_SDATA(const s32, "field/fldProcess", D_0054EB54);

INCLUDE_SDATA(const s32, "field/fldProcess", fldEncountSagasiFlag);

INCLUDE_SDATA(const s32, "field/fldProcess", hDistortionBlur);

INCLUDE_SDATA(const s32, "field/fldProcess", gFldRndEncBtl);

INCLUDE_SDATA(const s32, "field/fldProcess", fldEncountCheckWork);

INCLUDE_SDATA(const s32, "field/fldProcess", fldProcCampWait);

INCLUDE_SDATA(const s32, "field/fldProcess", fldProcAmWait);

INCLUDE_SDATA(const s32, "field/fldProcess", gFldStart);

INCLUDE_SDATA(const s32, "field/fldProcess", fldProcShutDownFlg);

INCLUDE_SDATA(const s32, "field/fldProcess", fldNext_fld);

INCLUDE_SDATA(const s32, "field/fldProcess", D_0054EB7C);

INCLUDE_SDATA(const s32, "field/fldProcess", fldAutoMapSeqFlags);

INCLUDE_SDATA(const s32, "field/fldProcess", fldSequenceState);

INCLUDE_SDATA(const s32, "field/fldProcess", fldEncountFlags);

INCLUDE_SDATA(const s32, "field/fldProcess", D_0054EB8C);

INCLUDE_SDATA(const s32, "field/fldProcess", fldEncountTimer);

INCLUDE_SDATA(const s32, "field/fldProcess", D_0054EB94);

