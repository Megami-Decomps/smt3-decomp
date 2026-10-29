#include "common.h"
#include "basic.h"
#include "kernel.h"
#include "libsdf.h"
#include "ps2asm.h"
#include "newdata.h"
#include "field.h"
#include "effect.h"

extern s32 D_0054EB54;
extern u32 fldEncountTimer;

extern char D_0054EA38[], D_0054EA40[], D_0054EA48[], D_0054EA50[], D_0054EA58[];

extern dds3Basic_t *dds3GetControlScene(void);
extern void dds3SetActiveScene(dds3Basic_t *scene);
extern s32 evtBitCheck(s32 no);
extern void evtBitOff(s32 no);
extern void evtBitOn(s32 no);
extern s32 ProcRandomEncountCheck(void);
extern void fldLoadNextFile_SetId_BtlRet(u32 field, u32 area);
extern char *strcpy(char *destination, const char *source);

extern void *mhfldWaveFile;

extern void *mhfldPlayerFile;

extern void *mhfldAnaFile;

extern s32 gFldRndEncBtl;

extern u8 fldLoadNextGetStatus2(void);

extern u32 fldLoadNextGetStatus(void);

extern void dds3SetActiveScene(dds3Basic_t *scene);
extern s32 evtBitCheck(s32 no);
extern void evtBitOff(s32 no);
extern void evtBitOn(s32 no);
extern s32 ProcRandomEncountCheck(void);
extern void fldLoadNextFile_SetId_BtlRet(u32 field, u32 area);
extern char *strcpy(char *destination, const char *source);

extern s32 fldLoadNextTask(void);

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

extern void fldDoor_EveBtlExit(s32 field, s32 eveno);



typedef struct {
    u32 flags;
    s32 stage;
} fldSequenceController_t;

extern fldDanteGunState_t fldRunDanteGunFlag;

extern u8 fldDoorExitFlag;

extern s8 fldDoorExitMode[];

extern char pointResName[];

extern char D_00531690[], D_00531950[];

extern s32 fldProcShutDownFlg;

extern s32 ProcSequence(dds3ProcessID_t *id);

extern s32 ProcDraw(dds3ProcessID_t *id);

extern void fldFreeSnowSeed(void);

extern void fldAutoMapSeqSetFlag(u32 flag);

extern void fldFreeInputPanel(void);

extern void evtDisableMoonDraw(void);

extern void fldFreeAddressProcess(void);

extern void dds3EffectDistortionBlurDelete(effBlurScatterWork_t *hDistortionBlur);

extern void campKillProcesses(void);

extern void scrAllReleaseScript(void);

extern dds3Basic_t *fldAnaBasic;

extern effBlurScatterWork_t *hDistortionBlur;

extern char D_00531950[];

extern s32 gfldPlayerSize;

extern void *gfldAnaPtr;

extern s32 gfldAnaSize;

extern void *gfldWavePtr;

extern s32 gfldWaveSize;

extern void *gfldPlayerPtr;

extern s32 gfldPlayerSizeBk;

extern void dds3AdminiCallSequence(s32 sequence, void *data, s32 datasize, s32 restore);

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

extern void dds3VisibleSceneBasic(dds3Basic_t *basic, s32 visible);

extern u8 cmpGetLoadState(void);

extern void dds3FadeOut2(s32 frame);

extern void evtKillFieldScript(void);

extern void p2sdStopAllSe(void);

extern void dds3FadeIn2(s32 frame);

extern s32 fldPlayerChkAna(void);

extern void p2sdPlaySE(s32 id, s32 vol, s32 pan);

extern void fldPlayerPosRot(void);

extern void dds3FadeOut(s32 r, s32 g, s32 b, s32 frame);

extern void fldPlayerMotReset(void);

extern void cmpProcessStart(void);

extern s32 dds3SceneBasicStartScript(dds3Basic_t *scene, const char *procname);

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

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, dds3ProcessFunc_t drawExit,
                                         dds3ProcessFunc_t deleteExit, void *p);

extern s32 dds3KillProcessName(char *name, s32 shutdowntimer);

extern scrProcessWork_s *scrSearchScriptName(char *scriptname);

extern u32 fldAutoMapChkMode(void);

extern void fldSetTerminalState(s32 type, s32 idx);

extern void fldPauseBgm(void);

extern void fldProcNextJump(void);

extern s32 fldDoor_SiseExit(void);

extern void fldEveHit_End(void);

extern void fldProcShutDown(void);

void InitOoyTest(void);

void fldRegistUnitPad(void);

INCLUDE_RODATA(const s32, "field/fldProcess", D_00531860);

#ifdef NON_MATCHING
extern sdfVector4_t D_002FC490;
extern s32 fldPlayerStatus[];
extern s32 fldPlayerStepDist;
extern s32 fldSequenceMode;
extern s32 fldEncountCheckWork;
extern s32 fldEncountSagasiFlag;
extern u32 D_0054EB94;
extern dds3Basic_t *dds3GetActiveUnitSceneBasic(dds3Basic_t *scene);
extern void dds3GetPosPoseSet(dds3Basic_t *basic);
extern s32 evtGetAgeOfMoon16(void);
extern void evtMoonAddMove(f32 distance);
extern void datPartyDamageFld(s32 mode);
extern void datFieldSkillKaifuku(void);
extern s32 datCheckSkillTakaraSagasi(void);
extern void datDebugPrintf(const char *format, ...);
extern s32 fldCheckEveRngEncount(void);
extern s32 fldRunEncountChk(void);
extern u32 nbEncountCalc(s32 encounter, f32 distance);
extern void CallBattleSub(s32 encno, s32 eventno);
extern void fclClearTerminalState(void);
extern void p2sdPlaySE(s32 index, s32 volume, s32 pan);
extern void dds3FadeIn(s32 r, s32 g, s32 b, s32 frames);

s32 ProcRandomEncountCheck(void)
{
    dds3Basic_t *scene;
    dds3Basic_t *unit;
    f32 distance;
    f32 damageDistance;
    s32 oldMoon;
    s32 newMoon;
    s32 encounter;
    s32 damageMode;
    s32 specialFloor;

    if (fldGlobalBase.menuOpenFlag > 0) fldGlobalBase.menuOpenFlag--;
    if (fldGlobalBase.encountState > 0) fldGlobalBase.encountState--;
    if (fldGlobalBase.word5C > 0) fldGlobalBase.word5C--;
    if (fldEncountTestFlag(0x40) == 0) return 0;
    if (fldAutoMapSeqTestFlag(1) == 1) return 0;
    scene = dds3GetActiveScene();
    if (scene == NULL) return 0;
    unit = dds3GetActiveUnitSceneBasic(scene);
    if (unit == NULL) return 0;

    dds3GetPosPoseSet(unit);
    SDF_VU0_LOADVECTOR_B(&D_002FC490);
    SDF_VU0_STOREVECTOR(&D_002FC490);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(distance);
    if (distance < 0.1999999881f) distance = 0.0f;
    fldGlobalBase.playerMoveDistance = distance;
    oldMoon = evtGetAgeOfMoon16();
    if (fldGlobalBase.word5C > 0) {
        dds3GetPosPoseSet(unit);
        SDF_VU0_STOREVECTOR(&D_002FC490);
        fldGlobalBase.wordC8 = 0;
        return 0;
    }
    if (fldGlobalBase.area < 10) evtMoonAddMove(distance * 4.0f);
    else evtMoonAddMove(distance);
    if (fldGlobalBase.consumedFlags & 0xF) {
        fldReleaseUnitPad();
        dds3SceneBasicStartScript(dds3GetActiveScene(), D_00531860);
        fldPlayerPosRot();
        return 0;
    }
    newMoon = evtGetAgeOfMoon16();
    specialFloor = (fldGlobalBase.area == 42 && (fldGlobalBase.floor == 13 || fldGlobalBase.floor == 25)) ||
                   (fldGlobalBase.area == 44 && (fldGlobalBase.floor == 2 || fldGlobalBase.floor == 4 || fldGlobalBase.floor == 24));
    if (specialFloor) {
        if (fldSequenceMode != 0) {
            fldGlobalBase.floatB4 += distance;
            if (fldGlobalBase.floatB4 > 450.0f) {
                fldEncountClearFlag(0x40);
                fldGlobalBase.word80 = 60;
                fldGlobalBase.floatB4 = 0.0f;
                datPartyDamageFld(4);
                return 0;
            }
        }
    } else if (fldGlobalBase.wordB0 != 0 || (fldPlayerStatus[0] & 1)) {
        damageDistance = 450.0f;
        fldGlobalBase.floatB4 += distance;
        if (fldGlobalBase.wordB0 == 0 && (fldPlayerStatus[0] & 1)) damageDistance = 1000.0f;
        if (damageDistance < fldGlobalBase.floatB4) {
            if (fldGlobalBase.wordB0 != 0) {
                if (DDS3_GBWK->maFlags & 4) {
                    fldGlobalBase.floatB4 = 0.0f;
                    fldPlayerStepDist = 0;
                    damageMode = 0;
                } else damageMode = 2;
            } else damageMode = 1;
            if (damageMode != 0) {
                fldPlayerStepDist = 60;
                fldGlobalBase.floatB4 = 0.0f;
                datPartyDamageFld(damageMode);
            }
        }
    } else {
        fldGlobalBase.floatB4 = 450.0f;
    }
    if (oldMoon != newMoon) datFieldSkillKaifuku();
    if (fldLoadNextGetStatus() == 1) return 0;
    if (fldGlobalBase.area < 10 && fldCheckEveRngEncount() == 0 &&
        fldEncountCheckWork != evtGetAgeOfMoon16() && DDS3_GBWK->unkA42[0] == 0) {
        datDebugPrintf("Takara Sagasi Try!!\n");
        fldEncountSagasiFlag = datCheckSkillTakaraSagasi();
        if (fldEncountSagasiFlag != 0) {
            fldReleaseUnitPad();
            dds3SceneBasicStartScript(dds3GetActiveScene(), "takarasagasi_label");
            fldEncountCheckWork = evtGetAgeOfMoon16();
            datDebugPrintf("Takara Sagasi Dvl[%x]\n", fldEncountSagasiFlag);
            return 0;
        }
    }
    fldEncountCheckWork = evtGetAgeOfMoon16();
    if (fldRunDanteGunFlag.enabled != 0 && fldRunEncountChk() != 0 && fldRunDanteGunFlag.word5C != 0) {
        fldEveHit_End();
        fldAutoMapSeqSetFlag(1);
        fldAutoMapSeqSetFlag(2);
        fldReleaseUnitPad();
        evtKillFieldScript();
        D_0054EB94 = 0x40A;
        fldEncountTimer = 0;
        switch (fldGlobalBase.floor) {
        case 3: fldSetNextField(43, 200); break;
        case 4: case 5: fldSetNextField(43, 201); break;
        case 7: case 8: case 9: case 10: fldSetNextField(43, 202); break;
        }
        CallBattleSub(0x40A, 0);
        return -1;
    }
    if (distance < 9998.0f && fldRunDanteGunFlag.enabled == 0) return 0;
    if (fldEncountTestFlag(0x10) == 0) return 0;
    encounter = fldGlobalBase.wordC0;
    if (encounter == 0) encounter = fldGlobalBase.wordBC;
    fldEncountTimer = encounter;
    D_0054EB94 = nbEncountCalc(encounter, distance);
    if (D_0054EB94 == 0) return 0;
    if (fldRunDanteGunFlag.enabled == 1 || distance > 9998.0f) {
        fldEveHit_End();
        fldAutoMapSeqSetFlag(1);
        fldAutoMapSeqSetFlag(2);
        fldReleaseUnitPad();
        evtKillFieldScript();
        return -1;
    }
    fclClearTerminalState();
    p2sdPlaySE(15, 127, 63);
    dds3FadeIn(128, 128, 128, 6);
    D_0054EB54++;
    return 0;
}
#else
INCLUDE_ASM(const s32, "field/fldProcess", ProcRandomEncountCheck);
#endif

s32 fldProcKiretaChk(void)
{
    return scrSearchScriptName(D_00531860) != NULL;
}

s32 fldProcHirakenaiChk(void)
{
    return scrSearchScriptName("hirakenai_label") != NULL;
}

extern fldUnitControl_t *fldGetUnitControl(void);
extern effBlurScatterWork_t *dds3EffectDistortionBlurCreate(dds3Effect_DistortionBlurParam_t *param);
extern dds3ProcessID_t *dds3SearchProcessPriority(s32 priority);
extern dds3Basic_t *D_0054EAE8;
extern u32 dds3BackColor;
extern s32 fldSequenceState, gFldStart, fldSkyEnable, gfldPlayerHasiCnt;
extern s32 fldWmWarpMode;

extern s32 fmProcessChk(void), fldGetRestoreMode(void), fldWaitBgmReload(void);
extern s32 dds3FadeCheck(void), fldEveHit_ChkEveHitAfter(void), fldPlayerCalc(void);
extern void fldEveHit_ChkEveHit(dds3Basic_t *basic);
extern void fldSkySetTbl(s32 index, s32 blend), fldSkySetTableAnime(s32 type);
extern void dds3ObjectBaseOffFlag(dds3Basic_t *basic, s32 flag);
extern void fldPanelSet(s32 mode);
extern void *fldCamMain(void);
extern void fldResetSequenceParams(void);
extern void evtEnableMoonCalc(void);
extern void fldReadyScene(void);
extern void fldInitInputPanel(void);
extern void fldInitAddressProcess(void);
extern void fldCamResetNpc(void);
extern void fldSnowSeedInit(void);
extern void fldSnowSeedReset(void);
extern void fldBgmLoad(void);
extern void fldBgmPlay(void);
extern void fldPlayFieldBgm(void);
extern void fldBgmContinue(void);
extern void fldSnowInit(void);
extern void fldRegistUnitPad(void);
extern void fldSetSnowActive(void);
extern void fldRunView(void);
extern void encStart(void);
extern void fldLightMaTask(void);
extern void fldSkyCalcTable(void);
extern void fldSkyCalcFilter(void);
extern void fldHitWM_Mask(void);
extern void fldPlaySe3D(void);
extern void fldBgmBitChk(void);
extern void fldAutoMapProc(void);
extern void fldNpcCalc(void);
extern void fldItemBoxCalcTask(void);

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
    if (param->mode == 0 && param->FoceScript[0] != 0) {
        dds3SceneBasicStartScript(dds3GetControlScene(), param->FoceScript);
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

s32 ProcSequence(dds3ProcessID_t *id)
{
    dds3Effect_DistortionBlurParam_t blur25;
    dds3Effect_DistortionBlurParam_t blur37;
    dds3Effect_DistortionBlurParam_t blur42;
    s32 camp;
    fldSceneParam_t *param = &fldSceneParam;
    fldSequenceController_t *controller = &fldAutoMapSeqFlags;

    if (dds3AdminiGetNextSequence() != -1 || fmProcessChk() != 0) return 0;
    fldLoadNextTask();
    if (fldSequenceState == 0 && fldLoadNextGetStatus() == 1) return 0;
    if (fldBgmIsLoading() != 0) fldWaitBgmLoad();
    camp = fldProcCampChk();
    if (camp < 3) {
        if (camp > 0) {
            ProcCampCheck();
            return 0;
        }
    }
    if (fldEncountTestFlag(0x40) == 0) fldPlayerPosRot();
    switch (controller->stage) {
    case 0:
        gFldRndEncBtl = 0;
        gFldStart = 1;
        fldResetSequenceParams();
        dds3BackColor = 0x80000000;
        evtEnableMoonCalc();
        fldSkyEnable = 0;
        if ((u32)(fldGlobalBase.cameraParamIndex - 6) < 2 || fldGlobalBase.cameraParamIndex == 1)
            fldGlobalBase.cameraParamIndex = 0;
        fldReadyScene();
        dds3SetActiveScene(dds3GetControlScene());
        fldInitInputPanel();
        evtEnableMoonDraw();
        fldInitAddressProcess();
        if (fldGlobalBase.area >= 200) {
            fldRunDanteGunFlag.skyTable = 0;
            fldRunDanteGunFlag.word30 = 0;
            fldSkySetTbl(0, 0);
            fldSkySetTableAnime(fldRunDanteGunFlag.word30);
        }
        if (fldGlobalBase.area == 25) {
            blur25.number = 35;
            blur25.interval = 50;
            blur25.speed = 0.08f;
            blur25.abgr = 0x23919191;
            blur25.blend = 0x44;
            blur25.rotate = 0.02499999851f;
            blur25.distortion = 0.03999999911f;
            blur25.area = 0x100;
            blur25.width = 0x109;
            blur25.center[0] = 0;
            blur25.center[1] = 0;
            hDistortionBlur = dds3EffectDistortionBlurCreate(&blur25);
        }
        if (fldGlobalBase.area == 37) {
            blur37.number = 15;
            blur37.interval = 30;
            blur37.speed = 0.152f;
            blur37.abgr = 0x14878787;
            blur37.blend = 0x48;
            blur37.distortion = 0.015f;
            blur37.rotate = 0;
            blur37.center[0] = 0;
            blur37.center[1] = 0;
            blur37.area = 0x136;
            blur37.width = 0x136;
            hDistortionBlur = dds3EffectDistortionBlurCreate(&blur37);
        }
        if (fldGlobalBase.area == 42 || fldGlobalBase.area == 44) {
            blur42.number = 20;
            blur42.interval = 20;
            blur42.speed = 0.1999999881f;
            blur42.abgr = 0x1E000082;
            blur42.blend = 0x44;
            blur42.rotate = 1;
            blur42.distortion = 0.1999999881f;
            blur42.center[0] = 0;
            blur42.center[1] = 0;
            blur42.area = 200;
            blur42.width = 200;
            hDistortionBlur = dds3EffectDistortionBlurCreate(&blur42);
        }
        dds3FadeOut2(0);
        fldCamResetNpc();
        fldSnowSeedInit();
        fldSnowSeedReset();
        fldStatusSet();
        fldBgmLoad();
        fldEncountClearFlag(0x40);
        RetryMapLoad();
        fldCamMain();
        controller->stage++;
        break;
    case 1:
        if (fldWaitBgmReload() != 0) {
            gFldStart = 0;
            if (fldGetRestoreMode() == 1 || fldGetRestoreMode() == 4 || fldGetRestoreMode() == 6 || fldNextFieldId != 0) {
                fldBgmContinue();
            } else if (param->bgmMode == 0) {
                fldBgmPlay();
            } else {
                fldPlayFieldBgm();
            }
            fldNextFieldId = 0;
            controller->stage++;
        }
        break;
    case 2:
        controller->stage++;
        break;
    case 3:
        fldSnowInit();
        if (fldGlobalBase.wordCC == 0 && (fldGlobalBase.area != 11 || fldGlobalBase.floor != 1) &&
            dds3FadeCheck() == 0 && gfldPlayerHasiCnt == 0) dds3FadeIn2(8);
        controller->stage++;
        if (dds3SearchProcessPriority(0x3EA) == NULL) fldRegistUnitPad();
        dds3ObjectBaseOffFlag(fldGetUnitControl()->handle, 0x40);
        fldEncountSetFlag(0x10);
        fldEncountSetFlag(0x20);
        fldSetSnowActive();
        break;
    case 4:
        if (fldRunDanteGunFlag.word5C != 0) fldRunView();
        fldSkyEnable = 1;
        if (fldEncountTestFlag(0x40) != 0 && fldEveHit_ChkEveHitAfter() != 0) return 0;
        if (fldEncountTestFlag(0x40) != 0 && fldGlobalBase.wordCC != 0) goto check_hit;
        if ((evtBitCheck(0x8A0) != 0 || evtBitCheck(0x8B1) != 0) &&
            fldEncountTestFlag(0x40) != 0 && (s8)dds3PadButtonState[0x2D] < 0) {
            if (evtBitCheck(0xAF0) != 0) evtBitOff(0xAF0);
            else evtBitOn(0xAF0);
        }
        if (fldRunDanteGunFlag.gunFlag == 0) {
            ProcCampCheck();
            if (fldAutoMapSeqTestFlag(2) != 0) return 0;
            if (fldEncountTestFlag(0x20) == 0) break;
            if (fldRunDanteGunFlag.gunFlag == 0) ProcAmCheck();
        }
        if (fldEncountTestFlag(0x40) != 0) {
            fldPanelSet(1);
            ProcRandomEncountCheck();
        }
        if (fldAutoMapSeqTestFlag(2) == 1) {
            controller->stage++;
            break;
        }
        fldPlayerCalc();
        if (fldEncountTestFlag(0x40) == 0) break;
        if (fldGlobalBase.area < 10 && fldWmWarpMode == 3) break;
    check_hit:
        fldEveHit_ChkEveHit(D_0054EAE8);
        break;
    case 5:
        dds3VisibleSceneBasic(dds3GetActiveScene(), 0);
        encStart();
        fldPanelSet(0);
        fldPauseBgm();
        p2sdPlaySE(15, 127, 63);
        /* fall through */
    case 7:
        fldFreeNextFile();
        fldLoadNextFile_SetId_BtlRet(fldGlobalBase.area, fldGlobalBase.floor + 1);
        controller->stage = 0;
        D_0054EB54++;
        gFldRndEncBtl = 1;
        dds3AdminiCallSequence(0x13, &fldEncountTimer, 8, 1);
        return -1;
    }
    switch (fldGlobalBase.word90) {
    case 0:
    case 1:
    case 4:
    case 5:
        break;
    case 2:
    case 6:
        if (fldGlobalBase.word94 > 0) fldGlobalBase.word94--;
        break;
    case 3:
    case 7:
        if (fldGlobalBase.word94 < 15) fldGlobalBase.word94++;
        break;
    }
    if (fldProcCampChk() != 1) {
        fldLightMaTask();
        fldSkyCalcTable();
        fldSkyCalcFilter();
        if (fldGlobalBase.area == 1) fldHitWM_Mask();
    }
    if (controller->stage >= 4) {
        fldPlaySe3D();
        fldBgmBitChk();
        if (fldRunDanteGunFlag.gunFlag == 0) {
            fldAutoMapProc();
            fldAutoMapChkMode();
        }
        if (fldAutoMapChkMode() == 0 && fldProcCampChk() == 0 && fldProcKiretaChk() == 0 &&
            fldProcHirakenaiChk() == 0 && fldGlobalBase.word80 == 0) {
            fldNpcCalc();
            fldItemBoxCalcTask();
        }
    }
    if (fldProcCampChk() != 1) fldCamMain();
    return 0;
}


#ifdef NON_MATCHING
extern s32 fldDrawFlag[];
extern s32 fldDbg_ExBlurOn;
extern s32 nbGetEncountOn(void);
extern void fldDbgPrint2D(s32 x, s32 y, const char *text);
extern void fldNpcDraw(void);
extern void fldWaveTask(void);
extern void fldAutoMapDraw(void);
extern void fldSnowTask(void);
extern void fldElvDrawFloorNum(void);
extern void dds3EffectBlur(const dds3Effect_Blur_t *source);
extern void dds3EffectDistortionBlurDraw(effBlurScatterWork_t *work);

#define DRAW_FIELD_BLUR(color, mode, angle, zoom) do { \
    blur.abgr = (color); \
    blur.blend = (mode); \
    blur.rotate = (angle); \
    blur.scale = (zoom); \
    blur.center[0] = 0; \
    blur.center[1] = 0; \
    blur.xy[0][0] = 0; \
    blur.xy[0][1] = 0; \
    blur.xy[1][0] = 0x200; \
    blur.xy[1][1] = 0x1C0; \
    dds3EffectBlur(&blur); \
} while (0)

s32 ProcDraw(dds3ProcessID_t *id)
{
    dds3Effect_Blur_t blur;
    s32 status;
    s32 area;
    s32 areaAfterDistortion;

    if (dds3AdminiGetNextSequence() != -1) return 0;
    status = fldLoadNextGetStatus();
    if (status == 1) {
        if (fldDrawFlag[0] == status) fldDbgPrint2D(0x40, 0xC0, "NowLoading...");
        if (fldSequenceState == 0) return 0;
    }
    if (nbGetEncountOn() != 0 || fmProcessChk() != 0 ||
        fldAutoMapSeqTestFlag(2) != 0 || fldProcCampChk() == 1) return 0;
    if (fldAutoMapChkMode() == 0 && fldProcCampChk() == 0) fldNpcDraw();
    fldWaveTask();
    fldAutoMapDraw();
    fldSnowTask();
    fldElvDrawFloorNum();
    if (fldAutoMapSeqFlags.stage < 2 || fldDbg_ExBlurOn == 0 || nbGetEncountOn() != 0) return 0;

    if (fldGlobalBase.area == 0x12 || fldGlobalBase.area == 0x1C || fldGlobalBase.area == 0x1E)
        DRAW_FIELD_BLUR(0x3C8080AA, 0x44, 0.02f, 0.9859999418f);
    if (((u32)fldGlobalBase.area - 2) < 3 || fldGlobalBase.area == 6 ||
        fldGlobalBase.area == 7 || fldGlobalBase.area == 8 ||
        (fldGlobalBase.area == 12 && fldGlobalBase.floor == 0))
        DRAW_FIELD_BLUR(0x2D7882B4, 0x44, 0.0f, 1.0f);
    if (fldGlobalBase.area == 5)
        DRAW_FIELD_BLUR(0x2D8282B4, 0x44, 0.0f, 1.0f);
    if (fldGlobalBase.area == 15) {
        if (((u32)fldGlobalBase.floor - 19) < 2)
            DRAW_FIELD_BLUR(0x2D7882B4, 0x44, 0.0f, 1.0f);
        if (((u32)fldGlobalBase.floor - 2) < 3)
            DRAW_FIELD_BLUR(0x1E8080C6, 0x44, 0.0f, 0.9859999418f);
    }
    if (fldGlobalBase.area == 16)
        DRAW_FIELD_BLUR(0x1EC8B4C8, 0x44, 0.0f, 1.0f);
    if (fldGlobalBase.area == 17 && fldGlobalBase.floor == 0)
        DRAW_FIELD_BLUR(0x1E8080C6, 0x44, 0.0f, 0.9859999418f);
    if (fldGlobalBase.area == 23 && ((u32)fldGlobalBase.floor < 2 || fldGlobalBase.floor == 6))
        DRAW_FIELD_BLUR(0x2380A0DC, 0x44, 0.0f, 0.9859999418f);
    if (fldGlobalBase.area == 24 && fldGlobalBase.floor == 17)
        DRAW_FIELD_BLUR(0x288296B4, 0x44, 0.0f, 1.0f);
    if (fldGlobalBase.area == 25 && (((u32)fldGlobalBase.floor - 16) < 5 || fldGlobalBase.floor == 24))
        dds3EffectDistortionBlurDraw(hDistortionBlur);
    if (fldGlobalBase.area == 27 && (u32)fldGlobalBase.floor < 5)
        DRAW_FIELD_BLUR(0x1E8296B4, 0x44, 0.0f, 1.0f);
    if ((fldGlobalBase.area == 32 && fldGlobalBase.floor == 19) ||
        (fldGlobalBase.area == 36 && fldGlobalBase.floor == 20) ||
        (fldGlobalBase.area == 37 && fldGlobalBase.floor == 25))
        DRAW_FIELD_BLUR(0x38948080, 0x48, 0.0f, 1.0f);
    if (fldGlobalBase.area == 34 && ((u32)fldGlobalBase.floor - 1) < 2)
        DRAW_FIELD_BLUR(0x1E82DCFA, 0x44, 0.0f, 1.0f);
    if (fldGlobalBase.area == 36 && fldGlobalBase.floor == 16)
        DRAW_FIELD_BLUR(0x237882AA, 0x44, 0.0f, 1.0f);
    area = fldGlobalBase.area;
    if (area == 37) {
        if (((u32)fldGlobalBase.floor - 6) < 17) dds3EffectDistortionBlurDraw(hDistortionBlur);
        areaAfterDistortion = fldGlobalBase.area;
        if (areaAfterDistortion == area) {
            if (fldGlobalBase.floor == 23) DRAW_FIELD_BLUR(0x237882AA, 0x48, 0.0f, 1.0f);
            if (fldGlobalBase.area == areaAfterDistortion && fldGlobalBase.floor == 26)
                DRAW_FIELD_BLUR(0x19828CA0, 0x48, 0.0f, 1.0f);
        }
    }
    if (fldGlobalBase.area == 42 && (fldGlobalBase.floor == 13 || fldGlobalBase.floor == 25))
        dds3EffectDistortionBlurDraw(hDistortionBlur);
    if (fldGlobalBase.area == 44 && (fldGlobalBase.floor == 2 || fldGlobalBase.floor == 4 || fldGlobalBase.floor == 24))
        dds3EffectDistortionBlurDraw(hDistortionBlur);
    return 0;
}
#undef DRAW_FIELD_BLUR
#else
INCLUDE_ASM(const s32, "field/fldProcess", ProcDraw);
#endif

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

void fldProcTrigSequence(fldSceneParam_t *srcParam, u32 mode) {
    fldSceneParam_t *scene = &fldSceneParam;
    fldSequenceController_t *controller = &fldAutoMapSeqFlags;

    controller->stage = 0;
    controller->flags = 0;
    scene->mode = mode;
    if (mode == 0) {
        scene->pointResrcID = srcParam->pointResrcID;
        scene->sequenceWord38 = srcParam->sequenceWord38;
        strcpy(scene->pointResName, srcParam->pointResName);
        scene->loadField = srcParam->loadField;
        scene->bgm = srcParam->bgm;
        scene->loadArea = srcParam->loadArea;
        scene->bgmMode = srcParam->bgmMode;
        scene->loadScript = srcParam->loadScript;
        scene->callmode = srcParam->callmode;
        scene->camMode = srcParam->camMode;
        strcpy(scene->camName, srcParam->camName);
        strcpy(scene->FoceScript, srcParam->FoceScript);
        scene->loadMode = srcParam->loadMode;
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
    fldPauseBgm();
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

