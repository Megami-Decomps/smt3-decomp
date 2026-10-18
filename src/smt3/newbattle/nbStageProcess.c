#include "common.h"
#include "kernel.h"
#include "nb.h"
#include "ps2asm.h"


extern f32 aViewEye[];
extern f32 D_002FA240[];
extern f32 dds3ScreenParam[];
extern void ReverseMatrix(void);
extern void MultMatrixRev(f32 *matrix);
/* These inherited symbol names identify the soft-double conversion and
 * comparison helpers in this executable, not integer conversion/division. */
extern f64 sdfFloatToULong(f32 value);
extern s32 dpdiv(f64 left, f64 right);


/* 8-byte rows at +0x88 indexed by formation slot. */
typedef struct nbTimeList_t {
    u16 unlock;
    u16 end;
    u16 hp;
    u16 pad06;
} nbTimeList_t;

typedef struct nbActionProcessData_t {
    nbMainProcessData_t *data;
    u32 unk04;
    s8 seq;
    s8 nextseq;
    s16 unk0A;
    s32 unk0C;
    u8 unk10[8];
    s32 unk18;
    u8 unk1C[0xC];
    nbFormation_t *form;
    datUnitWork_t *work;
    u8 unk30[0xC];
    s32 unk3C;
    u8 unk40[8];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    u8 unk58[0xC];
    s32 unk64;
    u8 unk68[0xC];
    void *unk74;
    u8 unk78[0x10];
    nbTimeList_t timelist[32];
    u8 unk188[4];
    s32 hitstopframe;
} nbActionProcessData_t;

typedef struct pmdProcessWork_t {
    nbFormation_t *form;
} pmdProcessWork_t;

/* One object: StageData is its byte at +1 and nbStageParam its word at +8. */
typedef struct nbStageState_t {
    u8 unk00;
    u8 data;
    u8 pad02[2];
    s16 unk04;
    s16 unk06;
    u32 param;
} nbStageState_t;

typedef struct nbActionFunc_t {
    void (*set)(nbActionProcessData_t *a);
    s32 (*check)(nbActionProcessData_t *a);
} nbActionFunc_t;

/* 164-byte rows indexed by datUnitWork_t::id. */
typedef struct nbDevilData_t {
    u16 aiscript;
    u8 unk02[0xA2];
} nbDevilData_t;

/* Rows of the same 164-byte stride, a separate object from D_003989E0. */
typedef struct nbDeadEvent_t {
    u16 script;
    u16 event;
    u8 unk04[0xA0];
} nbDeadEvent_t;

typedef struct nbCommSelProcessData_t {
    void *handle;
    nbMainProcessData_t *data;
    u8 unk08[4];
    s8 stat;
    u8 unk0D[0x14A7];
    u32 unk14B4;
} nbCommSelProcessData_t;

extern char D_005441A8[];

extern void *dds3GetProcessData(dds3ProcessID_t *id);
extern dds3ProcessID_t *dds3SearchProcessName(const char *name);
extern void SetTarget_nbActionProcess(nbActionProcessData_t *a);
extern s32 AddTimeListHp(nbActionProcessData_t *a, s32 formindex, s32 add);
extern void fldSkySetAlpha(s32 alp);
extern void nbPanelAllShow(void);
extern void nbPanelPressGaugeShow(void);
extern s32 nbRunAi(nbActionProcessData_t *a);
extern void ClearTimeList(nbActionProcessData_t *a);
extern void SetHpForTimeList(nbActionProcessData_t *a);
extern void nbInitAiScript(nbActionProcessData_t *a, s32 no);
extern s32 nbGetDevilFormatFlag(s32 formindex);
extern s32 dds3KillProcess(dds3ProcessID_t *id, s32 shutdowntimer);
extern void nbPanelStockSelectInit(void);
extern u32 nbPanelStockSelect(nbMainProcessData_t *data, u32 pad);
extern void sdfMemFreeDirect(void *handle);
extern void evtFreeAllUnit(void);
extern void evtFreeStage(void);
extern s32 sdfDelayedResrcCheck(void);
extern void SetCommCursor2(nbCommSelProcessData_t *s);
extern void nbSetOrderSide(s32 side);

extern s32 StkSelId;
extern nbStageState_t nbStageState;
extern nbActionFunc_t ActionFuncTable[];

extern s32 nbGetUniqueIdKoukaPacketCnt(nbActionProcessData_t *a);
extern void effBTLDelete(void *hBTLEffect);
extern void nbCameraSetState(u16 state);
extern void nbReturnUnit(s32 partyno, s32 lost, s32 dead);
extern s32 datGetItemSkillNo(s32 itemid);
extern datUnitWork_t *nbGetUnitWorkFromFormindex(s32 formindex);
extern s32 nbGetJyokyoMes(nbMainProcessData_t *data, s32 jyokyomes);
extern s32 nbMakeUnitFadeInPacket(s32 startframe, void *id, nbFormation_t *form, u32 col,
                                  s32 delay, s32 aframe, s32 colframe);
extern void fldSkyCalcTable(void);
extern void fldSkyCalcFilter(void);
extern void fldSkyDrawFilter(void);
extern void nbStageSp(void);
extern datUnitVisual_t datModelParamTable[];
extern datUnitVisual_t D_003494B0;
extern nbDevilData_t D_003989E0[];
extern nbDeadEvent_t D_00398A80[];
extern void itfMesMngSetVar(s32 id, s32 index, s32 num, s32 type);
extern void nbAddFlareChargePacket(s32 start, nbActionProcessData_t *a, s32 type);

void SetNextActionSeq(nbActionProcessData_t *a, s32 nextseq);
nbCommSelProcessData_t *GetCommSelProcessData(void);
datUnitVisual_t *nbGetUnitVisual(datUnitWork_t *work);
s32 nbGetUnitID(datUnitWork_t *work);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetBattleLight);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbAddStageCamera);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbAddStageCamera2);

void nbSetStageParam(s16 a, s16 b)
{
    nbStageState.unk04 = a;
    nbStageState.unk06 = b;
    nbStageState.data = 2;
    nbStageState.unk00 = 0;
    nbStageState.param = 0x80;
}

void func_0024C6F0(void)
{
    nbStageState.param = 0;
    nbStageState.unk04 = 0;
    nbStageState.unk06 = 0;
    nbStageState.unk00 = 0;
    nbStageState.data = 3;
}

s32 nbGetStageData(void)
{
    return nbStageState.data;
}

void nbFreeStage(void)
{
    evtFreeAllUnit();
    while (sdfDelayedResrcCheck() != 0) {
    }
    evtFreeStage();
    while (sdfDelayedResrcCheck() != 0) {
    }
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", StageLoad);

void StageFree(void)
{
    nbMainProcessData_t *data = nbGetMainProcessData();

    evtFreeStage();
    nbStageState.unk04 = 0;
    nbStageState.unk06 = 0;
    data->unk7C = 0;
    data->unk7E = 0;
    nbStageState.data = 0;
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbStageProcess1);

void nbStageShutdown(dds3ProcessID_t *id)
{
    nbGetMainProcessData()->stageProcess = 0;
    StageFree();
    nbStageState.data = 0;
}

void *nbStageDrawProcess1(dds3ProcessID_t *id)
{
    if (nbStageState.data == 1) {
        fldSkyCalcTable();
        fldSkyCalcFilter();
        fldSkyDrawFilter();
        nbStageSp();
    }
    return NULL;
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbInitStageProcess);

void nbStageFilterAlp(s32 alp)
{
    fldSkySetAlpha(alp);
}

void func_0024CBD0(u32 param)
{
    nbStageState.param = param;
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbStageSp);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", GetDyingBadWaitMotion);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbSetNegoWaitMotion);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbSetUnitMotion);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbSetWaitMotion);

s32 nbCalcUnitVisual(nbFormation_t *form, s32 part)
{
    datUnitVisual_t *visual = nbGetUnitVisual(nbGetUnitWorkFromFormindex(form->formindex));

    return visual->part[part].unk00;
}

s32 nbCalcUnitVisual2(nbFormation_t *form, s32 part)
{
    datUnitVisual_t *visual = nbGetUnitVisual(nbGetUnitWorkFromFormindex(form->formindex));

    return visual->part[part].unk01;
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbGetMotionActframe);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbGetMotionSpeed);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbGetMotionNo);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbAddUnitStock);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbSummonUnit);

void func_0024D938(void)
{
    s32 i;

    for (i = 4; i < 14; i++) {
        nbReturnUnit(i, 0, 0);
    }
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbReturnUnit);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nb_24DBF0);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nb_24DC80);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbRunKoukaType1);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbRunKoukaType2);

void nbSetSakeKoukaParam(nbFormation_t *form, f32 range, f32 power)
{
    form->type = 2;
    form->damageRange = range;
    form->shakePhase = 0;
    form->shakePower = power;
    form->addmovet = power;
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbSetDamage);

void nbSetDamage2(nbFormation_t *form, f32 range)
{
    form->type = 1;
    form->time = 0;
    form->damageFrames = 0x14;
    form->damageRange = range;
    form->addmovet = 0.0f;
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbSetBadstatMotion);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbSetBadstatMotion2);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbInitUnitEffect);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbLoadUnitGroup);

s32 nbCheckLoad(nbFormation_t *form)
{
    s32 flag = form->stat;

    if ((flag & 1) == 0) {
        return 0;
    }
    if ((flag & 0x40) == 0) {
        return 0;
    }
    if ((flag & 2) != 0) {
        return 0;
    }
    if ((form->flag & 4) != 0) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbCheckLoadGroup);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", UpdateFormAnime);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", UpdateFormPosRot);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbUnitDrawProcess1);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", RunSake);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", RunBadAction);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", RunDamage);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbCheckModelLoad);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", RegistUnit);

s32 CheckLoad_nbUnitProcess(pmdProcessWork_t *wdata)
{
    nbFormation_t *form = wdata->form;
    s32 i;

    for (i = 0; i < 16; i++) {
        if ((form->stat & 1) != 0 && (form->flag & 2) != 0) {
            return 1;
        }
        form++;
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", RunLoad);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_00543720);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_00543730);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbUnitProcess2);

void *nbUnitShutdown(dds3ProcessID_t *id)
{
    nbGetMainProcessData()->unitProcess = 0;
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbInitUnitProcess);

void nbCheckPacketState(nbActionProcessData_t *a, s32 startframe)
{
    if (a->form->motendtype == 2) {
        nbMakeUnitFadeInPacket(startframe, a, a->form, 0, 2, 4, 4);
        a->form->motendtype = 0;
    }
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetDeadPacket);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetPress);

void SetNewPress(nbActionProcessData_t *a, s32 presstype)
{
    if (a->unk18 < presstype) {
        a->unk18 = presstype;
    }
}

void SetHitStopFrame(nbActionProcessData_t *a, s32 frame)
{
    frame += 7;
    if (a->hitstopframe == 0 || frame < a->hitstopframe) {
        a->hitstopframe = frame;
    }
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetEnemyEscapePacket);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetAuroraPacket);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetAddPressPacket);

void SetAddFlareChargePacket(nbActionProcessData_t *a, s32 start)
{
    nbAddFlareChargePacket(start, a, 2);
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetMakaPacket);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetTorafuPacket);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetAnalyzePacket);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetPlayerSummonPacket);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetSakePacket);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetMissPacket);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetOngShadowPacket);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetOngShadowPacket2);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetAllPacket);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetTarget_nbActionProcess);

void ClearTimeList(nbActionProcessData_t *a)
{
    s32 i;

    for (i = 0; i < 32; i++) {
        a->timelist[i].unlock = 0;
        a->timelist[i].end = 0;
        a->timelist[i].hp = 0;
    }
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetHpForTimeList);

s32 GetTimeListHp(nbActionProcessData_t *a, s32 formindex)
{
    return a->timelist[formindex].hp;
}

s32 CheckTimeListDead(nbActionProcessData_t *a, s32 formindex)
{
    if (a->timelist[formindex].hp != 0) {
        return 0;
    }
    if ((nbGetDevilFormatFlag(formindex) & 0x100) != 0) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", CheckTimeListDeadEnemyParty);

s32 AddTimeListHp(nbActionProcessData_t *a, s32 formindex, s32 add)
{
    s32 hp;

    if ((a->data->stat & 0x20000) != 0) {
        return 0;
    }
    hp = a->timelist[formindex].hp + add;
    if (hp < 0) {
        a->timelist[formindex].hp = 0;
        return 1;
    }
    a->timelist[formindex].hp = hp;
    return 0;
}

s32 nbAddTimeListHp(nbActionProcessData_t *a, s32 formindex, s32 add)
{
    return AddTimeListHp(a, formindex, add);
}

s32 GetUnlockTime(nbActionProcessData_t *a, s32 formindex)
{
    return a->timelist[formindex].unlock;
}

s32 GetEndTime(nbActionProcessData_t *a, s32 formindex)
{
    return a->timelist[formindex].end;
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", OverwriteTimeList);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetTimeList);

s32 GetAllEndTime(nbActionProcessData_t *a)
{
    s32 max = 0;
    s32 i;

    for (i = 0; i < 32; i++) {
        if (max < a->timelist[i].end) {
            max = a->timelist[i].end;
        }
    }
    return max;
}

s32 GetAllUnlockTime(nbActionProcessData_t *a)
{
    s32 max = 0;
    s32 i;

    for (i = 0; i < 32; i++) {
        if (max < a->timelist[i].unlock) {
            max = a->timelist[i].unlock;
        }
    }
    return max;
}

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_005437F8);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", GetKoukaList);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetTargetPlayer);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetTargetParty);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", CheckArrCounter);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", CheckCounter);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", CheckMukanNego);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", CheckSenseiNego);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_00543880);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_005438A0);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_005438B0);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_005438C0);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", CheckInotiNego);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", CheckBinjoNego);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", CheckDeadScript);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", CheckKoketaNego);

void KyouseiPress(nbActionProcessData_t *a)
{
    nbMainProcessData_t *data = a->data;

    data->unk78 = 0;
    data->unk7A = 0;
    if (data->unkE8 != NULL) {
        dds3KillProcess(data->unkE8, 0);
    }
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", DokodemoPoa);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetNowActionStat);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetDamagePacket);

void SetAction_NON(void) {
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", CheckAction_NON);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetAction_COMM);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", CheckAction_COMM);

void SetAction_AI(nbActionProcessData_t *a)
{
    a->work->nowtform = -1;
    a->work->prevcommand = 0;
    nbRunAi(a);
}

s32 CheckAction_AI(nbActionProcessData_t *a)
{
    SetNextActionSeq(a, 7);
    return 1;
}

void SetAction_AISCR(nbActionProcessData_t *a)
{
    datUnitWork_t *work = a->work;
    u16 id = work->id;

    work->nowtform = -1;
    work->prevcommand = 0;
    nbInitAiScript(a, D_003989E0[id].aiscript);
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", CheckAction_AISCR);

void SetAction_DEADSCRIPT(nbActionProcessData_t *a)
{
    ClearTimeList(a);
    SetHpForTimeList(a);
    nbInitAiScript(a, a->unk54);
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", CheckAction_DEADSCRIPT);

void SetAction_BAD(nbActionProcessData_t *a)
{
    nbPanelAllShow();
    nbPanelPressGaugeShow();
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", CheckAction_BAD);

void SetAction_BADWAIT(nbActionProcessData_t *a)
{
    a->unk04 |= 0x20;
    if ((a->data->stat & 8) != 0) {
        nbCameraSetState(0x19);
        return;
    }
    nbCameraSetState(3);
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", CheckAction_BADWAIT);

void SetAction_TARSEL(nbActionProcessData_t *a)
{
    SetTarget_nbActionProcess(a);
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", CheckAction_TARSEL);

void SetAction_STKSEL(nbActionProcessData_t *a)
{
    a->unk3C = 0;
    nbPanelStockSelectInit();
    nbPanelStockSelect(a->data, 0);
    StkSelId = -1;
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", CheckAction_STKSEL);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetAction_READY);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", CheckAction_READY);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetAction_SKILL);

s32 CheckAction_SKILL(nbActionProcessData_t *a)
{
    SetNextActionSeq(a, 0x12);
    return 1;
}

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_00543E20);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_00543E38);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_00543E58);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_00543E78);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_00543E98);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_00543EB8);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_00543EE8);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", jtbl_00543F00);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", jtbl_00543F40);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", jtbl_00543F80);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetAction_NEGO);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", CheckAction_NEGO);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetAction_NEGORESULT);

s32 CheckAction_NEGORESULT(nbActionProcessData_t *a)
{
    SetNextActionSeq(a, 0x12);
    return 1;
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetAction_WAIT);

s32 CheckAction_WAIT(nbActionProcessData_t *a)
{
    SetNextActionSeq(a, 0x12);
    return 1;
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetAction_ESCAPE);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", CheckAction_ESCAPE);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetAction_RETURN);

s32 CheckAction_RETURN(nbActionProcessData_t *a)
{
    SetNextActionSeq(a, 0x12);
    return 1;
}

void SetAction_INVISIBLE(void) {
}

s32 CheckAction_INVISIBLE(nbActionProcessData_t *a)
{
    SetNextActionSeq(a, 0x12);
    return 1;
}

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_00544020);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetAction_CHECK1);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", CheckAction_CHECK1);

void SetAction_CHECK3(void) {
}

s32 CheckAction_CHECK3(nbActionProcessData_t *a)
{
    if (nbGetUniqueIdKoukaPacketCnt(a) > 0) {
        return 0;
    }
    SetNextActionSeq(a, 0x15);
    return 1;
}

void SetAction_ATO(void) {
}

s32 CheckAction_ATO(nbActionProcessData_t *a)
{
    if (a->unk74 != NULL) {
        effBTLDelete(a->unk74);
        a->unk74 = NULL;
    }
    SetNextActionSeq(a, 0x16);
    return 1;
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetAction_END);

s32 CheckAction_END(nbActionProcessData_t *a)
{
    return 1;
}

s32 nbGetActionSeq(dds3ProcessID_t *id)
{
    nbActionProcessData_t *a;

    if (id == NULL) {
        return 0;
    }
    a = dds3GetProcessData(id);
    return a->seq;
}

s32 nbGetPrevActionSeq(void)
{
    nbMainProcessData_t *data = nbGetMainProcessData();
    s32 i = data->unkE4 - 1;
    dds3ProcessID_t *id;

    if (i < 0) {
        i = 3;
    }
    id = data->unkD4[i];
    if (id == NULL) {
        return -1;
    }
    return nbGetActionSeq(id);
}

void SetActionSeq(nbActionProcessData_t *a, s32 seq)
{
    a->seq = seq;
    a->nextseq = 0;
    a->unk0A = 0;
    a->unk0C = 0;
    ActionFuncTable[(s8)seq].set(a);
}

void SetNextActionSeq(nbActionProcessData_t *a, s32 nextseq)
{
    a->nextseq = nextseq;
}

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_00544070);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_00544088);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_005440A0);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_005440B0);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_005440C0);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_005440D8);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_005440E8);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbActionProcess1);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbActionShutdown);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", InitActionProcessData);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbInitActionProcess);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbAddOrder);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbSortOrder);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbSetOrderSide);

void nbSetOrder(void)
{
    if ((nbGetMainProcessData()->stat & 8) == 0) {
        nbSetOrderSide(0);
        return;
    }
    nbSetOrderSide(1);
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbSetNextOrder);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbUpdateOrder);

/* vu0 routine: project vf10 only when it lies in the forward view cone. */
s32 nbClipEffectNum(s32 screenPosition[4])
{
    f32 viewDistance, forwardDot;
    f32 *projectionData;

    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR_B(aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(viewDistance);
    if (dpdiv(sdfFloatToULong(viewDistance), 16.0) <= 0)
        return 0;

    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADMATRIX(D_002FA240);
    SDF_VU0_COPYMATRIX_A_TO_B();
    ReverseMatrix();
    VU0_COPY_VF30_TO_VF10();
    SDF_VU0_NORMALIZE();
    SDF_VU0_INNERPRODUCT(forwardDot);
    if (forwardDot <= 0.5f)
        return 0;

    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_COPYMATRIX_B_TO_A();
    projectionData = dds3ScreenParam + 12;
    MultMatrixRev(projectionData);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    projectionData += 16;
    SDF_VU0_LOADVECTOR_B(projectionData);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(dds3ScreenParam + 32);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_FTOI4();
    SDF_VU0_STOREVECTOR(screenPosition);
    screenPosition[0] -= 0x7000;
    screenPosition[1] -= 0x7900;
    return 1;
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbLoadKoukaPos);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbGetKoukaMarkPos);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbMakeUnitMatrix);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbMakeUnitDrawMatrix);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbDrawRotatedModelX);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbDrawRotatedModel);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbGetUnitMarkPos);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbDrawPanelSprite);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbDrawKoukaSprite);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_00544130);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_00544140);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbSetTargetSkill);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbSetNormalSkill0);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbGetNakama);

void nbSetDevilNameMes(s32 hmsg, s32 no, datUnitWork_t *work)
{
    if (work->id == 0) {
        itfMesMngSetVar(hmsg, 0, 0, 10);
        return;
    }
    itfMesMngSetVar(hmsg, no, work->id, 1);
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbSetJyokyoMes);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbGetJyokyoMes);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbGetModelColor);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbAppendEnemyDevilWork);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbGetRandamStock);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbSetMainPhase);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbCountActiveUnits);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbSetupBattleUnits);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbSetPlayerStartData);

s32 nbGetUnitID(datUnitWork_t *work)
{
    s32 id = 0;

    if ((work->flag & 4) == 0) {
        id = work->id;
    }
    return (work->badstatus & 0xFFF) != 0x200 ? id : 0xFF;
}

datUnitVisual_t *nbGetUnitVisual(datUnitWork_t *work)
{
    s32 id = nbGetUnitID(work);

    if (id == 0) {
        return &D_003494B0;
    }
    return &datModelParamTable[id];
}

s32 nbCheckEventDead(nbFormation_t *form)
{
    s32 id = nbGetUnitID(nbGetUnitWorkFromFormindex(form->formindex));
    nbDeadEvent_t *p;

    if (id == 0) {
        return 0;
    }
    p = &D_00398A80[id];
    if (p->script != 0) {
        return 1;
    }
    return p->event != 0;
}

s32 nbCheckDeadScript(nbFormation_t *form)
{
    s32 id = nbGetUnitID(nbGetUnitWorkFromFormindex(form->formindex));
    nbDeadEvent_t *p;

    if (id == 0) {
        return 0;
    }
    p = &D_00398A80[id];
    if (p->script != 0) {
        return p->script;
    }
    return 0;
}

s32 nbCheckDeadEvent(nbFormation_t *form)
{
    s32 id = nbGetUnitID(nbGetUnitWorkFromFormindex(form->formindex));
    nbDeadEvent_t *p;

    if (id == 0) {
        return 0;
    }
    p = &D_00398A80[id];
    if (p->event != 0) {
        return p->event;
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbAddKagututiMoon);

s32 nbGetItemSkillNo(nbActionProcessData_t *a, s32 itemid)
{
    s32 no = datGetItemSkillNo(itemid);

    if (itemid == 1) {
        no = a->unk64;
    }
    return no;
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", GetCommListCnt);

nbCommSelProcessData_t *GetCommSelProcessData(void)
{
    return dds3GetProcessData(dds3SearchProcessName(D_005441A8));
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", DispPanelCursor);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_00544170);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_00544190);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_005441A8);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", DispPanelLabel);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", DispPanelSitaji);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", DispComm1PanelComm);

void DispComm1PanelTalk(void) {
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", DispComm1PanelItem);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", DispHpPanel);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", DispMpPanel);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", DispComm1PanelSummon);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", DispCommCursor);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", DispSummonCursor);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", DispComm1PanelCursor);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", DispHelpPanel);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", DispCommandPanel);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", AnimeCommandList);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", DispComm1Summon);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", DispComm1Item);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", DispComm1Talk);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", DispComm1Comm);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", DispHelp1);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", DispCommandList2);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", CheckSelectError);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", MemoryCommand);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", GetNextCommType);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SelectCommandList);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SelectCommandListAuto);

s32 nbGetCommSelStat(void)
{
    return GetCommSelProcessData()->stat;
}

void nbSetCommSelStat(s32 stat)
{
    GetCommSelProcessData()->stat = stat;
}

void nbCloseCommSel(void)
{
    nbCommSelProcessData_t *s = GetCommSelProcessData();

    s->stat = 4;
    s->unk14B4 = (s->unk14B4 | 2) & ~1;
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbCommSelProcess1);

void nbCommSelShutdown(dds3ProcessID_t *id)
{
    nbCommSelProcessData_t *s = dds3GetProcessData(id);
    nbMainProcessData_t *data = s->data;

    if (data->unkE8 == id) {
        data->unkE8 = NULL;
    }
    sdfMemFreeDirect(s->handle);
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetCommList);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetPrevCursor);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetCommCursor2);

void nbSetAutoCommSel(void)
{
    nbCommSelProcessData_t *s = GetCommSelProcessData();

    SetCommCursor2(s);
    s->unk14B4 |= 0x10;
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", InitCommSelProcessData);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbInitCommSelProcess);
INCLUDE_SDATA(const s32, "newbattle/nbStageProcess", D_00551110);

INCLUDE_SDATA(const s32, "newbattle/nbStageProcess", D_00551114);

INCLUDE_SDATA(const s32, "newbattle/nbStageProcess", D_00551118);

INCLUDE_SDATA(const s32, "newbattle/nbStageProcess", StkSelId);

INCLUDE_SDATA(const s32, "newbattle/nbStageProcess", D_00551124);

INCLUDE_SDATA(const s32, "newbattle/nbStageProcess", D_00551128);

INCLUDE_SDATA(const s32, "newbattle/nbStageProcess", D_00551130);

INCLUDE_SDATA(const s32, "newbattle/nbStageProcess", D_00551134);

