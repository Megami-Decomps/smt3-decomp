#include "common.h"
#include "kernel.h"
#include "newdata.h"

/* 8-byte rows at +0x88 indexed by formation slot. */
typedef struct nbTimeList_t {
    u16 unlock;
    u16 end;
    u16 hp;
    u16 pad06;
} nbTimeList_t;

typedef struct nbMainProcessData_t nbMainProcessData_t;
typedef struct nbFormation_t nbFormation_t;

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

typedef struct nbFormation_t {
    u8 unk00[0x70];
    s32 unk70;
    s32 unk74;
    s32 unk78;
    s32 unk7C;
    u8 unk80[0x12];
    s16 unk92;
    u8 unk94[0x10];
    s16 unkA4;
    u8 unkA6[0x12];
    s32 unkB8;
    f32 unkBC;
    f32 unkC0;
    s32 unkC4;
    f32 unkC8;
    u8 unkCC[0x24];
} nbFormation_t;

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

/* 0x310-byte rows; the 0x1C-byte subrows at +0x32 are indexed by part. */
typedef struct datUnitVisual_t {
    u8 unk00[0x32];
    struct {
        u8 unk00;
        s8 unk01;
        u8 unk02[0x1A];
    } part[8];
    u8 unk112[0x1FE];
} datUnitVisual_t;

struct nbMainProcessData_t {
    u8 unk00[0x60];
    u32 unk60;
    u8 unk64[0x14];
    s16 unk78;
    s16 unk7A;
    s16 unk7C;
    s16 unk7E;
    u8 unk80[0x4C];
    s32 unkCC;
    s32 unkD0;
    dds3ProcessID_t *unkD4[4];
    s32 unkE4;
    dds3ProcessID_t *unkE8;
};

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
extern nbMainProcessData_t *nbGetMainProcessData(void);
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

INCLUDE_ASM(const s32, "code_0024C470", SetBattleLight);

INCLUDE_ASM(const s32, "code_0024C470", nbAddStageCamera);

INCLUDE_ASM(const s32, "code_0024C470", nbAddStageCamera2);

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

INCLUDE_ASM(const s32, "code_0024C470", StageLoad);

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

INCLUDE_ASM(const s32, "code_0024C470", nbStageProcess1);

void nbStageShutdown(dds3ProcessID_t *id)
{
    nbGetMainProcessData()->unkCC = 0;
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

INCLUDE_ASM(const s32, "code_0024C470", nbInitStageProcess);

void nbStageFilterAlp(s32 alp)
{
    fldSkySetAlpha(alp);
}

void func_0024CBD0(u32 param)
{
    nbStageState.param = param;
}

INCLUDE_ASM(const s32, "code_0024C470", nbStageSp);

INCLUDE_ASM(const s32, "code_0024C470", GetDyingBadWaitMotion);

INCLUDE_ASM(const s32, "code_0024C470", nbSetNegoWaitMotion);

INCLUDE_ASM(const s32, "code_0024C470", nbSetUnitMotion);

INCLUDE_ASM(const s32, "code_0024C470", nbSetWaitMotion);

s32 nbCalcUnitVisual(nbFormation_t *form, s32 part)
{
    datUnitVisual_t *visual = nbGetUnitVisual(nbGetUnitWorkFromFormindex(form->unk92));

    return visual->part[part].unk00;
}

s32 nbCalcUnitVisual2(nbFormation_t *form, s32 part)
{
    datUnitVisual_t *visual = nbGetUnitVisual(nbGetUnitWorkFromFormindex(form->unk92));

    return visual->part[part].unk01;
}

INCLUDE_ASM(const s32, "code_0024C470", nbGetMotionActframe);

INCLUDE_ASM(const s32, "code_0024C470", nbGetMotionSpeed);

INCLUDE_ASM(const s32, "code_0024C470", nbGetMotionNo);

INCLUDE_ASM(const s32, "code_0024C470", nbAddUnitStock);

INCLUDE_ASM(const s32, "code_0024C470", nbSummonUnit);

void func_0024D938(void)
{
    s32 i;

    for (i = 4; i < 14; i++) {
        nbReturnUnit(i, 0, 0);
    }
}

INCLUDE_ASM(const s32, "code_0024C470", nbReturnUnit);

INCLUDE_ASM(const s32, "code_0024C470", nb_24DBF0);

INCLUDE_ASM(const s32, "code_0024C470", nb_24DC80);

INCLUDE_ASM(const s32, "code_0024C470", nbRunKoukaType1);

INCLUDE_ASM(const s32, "code_0024C470", nbRunKoukaType2);

void nbSetSakeKoukaParam(nbFormation_t *form, f32 range, f32 power)
{
    form->unk70 = 2;
    form->unkBC = range;
    form->unkC4 = 0;
    form->unkC0 = power;
    form->unkC8 = power;
}

INCLUDE_ASM(const s32, "code_0024C470", nbSetDamage);

void nbSetDamage2(nbFormation_t *form, f32 range)
{
    form->unk70 = 1;
    form->unk74 = 0;
    form->unkB8 = 0x14;
    form->unkBC = range;
    form->unkC8 = 0.0f;
}

INCLUDE_ASM(const s32, "code_0024C470", nbSetBadstatMotion);

INCLUDE_ASM(const s32, "code_0024C470", nbSetBadstatMotion2);

INCLUDE_ASM(const s32, "code_0024C470", nbInitUnitEffect);

INCLUDE_ASM(const s32, "code_0024C470", nbLoadUnitGroup);

s32 nbCheckLoad(nbFormation_t *form)
{
    s32 flag = form->unk78;

    if ((flag & 1) == 0) {
        return 0;
    }
    if ((flag & 0x40) == 0) {
        return 0;
    }
    if ((flag & 2) != 0) {
        return 0;
    }
    if ((form->unk7C & 4) != 0) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_0024C470", nbCheckLoadGroup);

INCLUDE_ASM(const s32, "code_0024C470", UpdateFormAnime);

INCLUDE_ASM(const s32, "code_0024C470", UpdateFormPosRot);

INCLUDE_ASM(const s32, "code_0024C470", nbUnitDrawProcess1);

INCLUDE_ASM(const s32, "code_0024C470", RunSake);

INCLUDE_ASM(const s32, "code_0024C470", RunBadAction);

INCLUDE_ASM(const s32, "code_0024C470", RunDamage);

INCLUDE_ASM(const s32, "code_0024C470", nbCheckModelLoad);

INCLUDE_ASM(const s32, "code_0024C470", RegistUnit);

s32 CheckLoad_nbUnitProcess(pmdProcessWork_t *wdata)
{
    nbFormation_t *form = wdata->form;
    s32 i;

    for (i = 0; i < 16; i++) {
        if ((form->unk78 & 1) != 0 && (form->unk7C & 2) != 0) {
            return 1;
        }
        form++;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_0024C470", RunLoad);

INCLUDE_RODATA(const s32, "code_0024C470", D_00543720);

INCLUDE_RODATA(const s32, "code_0024C470", D_00543730);

INCLUDE_ASM(const s32, "code_0024C470", nbUnitProcess2);

void *nbUnitShutdown(dds3ProcessID_t *id)
{
    nbGetMainProcessData()->unkD0 = 0;
}

INCLUDE_ASM(const s32, "code_0024C470", nbInitUnitProcess);

void nbCheckPacketState(nbActionProcessData_t *a, s32 startframe)
{
    if (a->form->unkA4 == 2) {
        nbMakeUnitFadeInPacket(startframe, a, a->form, 0, 2, 4, 4);
        a->form->unkA4 = 0;
    }
}

INCLUDE_ASM(const s32, "code_0024C470", SetDeadPacket);

INCLUDE_ASM(const s32, "code_0024C470", SetPress);

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

INCLUDE_ASM(const s32, "code_0024C470", SetEnemyEscapePacket);

INCLUDE_ASM(const s32, "code_0024C470", SetAuroraPacket);

INCLUDE_ASM(const s32, "code_0024C470", SetAddPressPacket);

void SetAddFlareChargePacket(nbActionProcessData_t *a, s32 start)
{
    nbAddFlareChargePacket(start, a, 2);
}

INCLUDE_ASM(const s32, "code_0024C470", SetMakaPacket);

INCLUDE_ASM(const s32, "code_0024C470", SetTorafuPacket);

INCLUDE_ASM(const s32, "code_0024C470", SetAnalyzePacket);

INCLUDE_ASM(const s32, "code_0024C470", SetPlayerSummonPacket);

INCLUDE_ASM(const s32, "code_0024C470", SetSakePacket);

INCLUDE_ASM(const s32, "code_0024C470", SetMissPacket);

INCLUDE_ASM(const s32, "code_0024C470", SetOngShadowPacket);

INCLUDE_ASM(const s32, "code_0024C470", SetOngShadowPacket2);

INCLUDE_ASM(const s32, "code_0024C470", SetAllPacket);

INCLUDE_ASM(const s32, "code_0024C470", SetTarget_nbActionProcess);

void ClearTimeList(nbActionProcessData_t *a)
{
    s32 i;

    for (i = 0; i < 32; i++) {
        a->timelist[i].unlock = 0;
        a->timelist[i].end = 0;
        a->timelist[i].hp = 0;
    }
}

INCLUDE_ASM(const s32, "code_0024C470", SetHpForTimeList);

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

INCLUDE_ASM(const s32, "code_0024C470", CheckTimeListDeadEnemyParty);

s32 AddTimeListHp(nbActionProcessData_t *a, s32 formindex, s32 add)
{
    s32 hp;

    if ((a->data->unk60 & 0x20000) != 0) {
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

INCLUDE_ASM(const s32, "code_0024C470", OverwriteTimeList);

INCLUDE_ASM(const s32, "code_0024C470", SetTimeList);

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

INCLUDE_RODATA(const s32, "code_0024C470", D_005437F8);

INCLUDE_ASM(const s32, "code_0024C470", GetKoukaList);

INCLUDE_ASM(const s32, "code_0024C470", SetTargetPlayer);

INCLUDE_ASM(const s32, "code_0024C470", SetTargetParty);

INCLUDE_ASM(const s32, "code_0024C470", CheckArrCounter);

INCLUDE_ASM(const s32, "code_0024C470", CheckCounter);

INCLUDE_ASM(const s32, "code_0024C470", CheckMukanNego);

INCLUDE_ASM(const s32, "code_0024C470", CheckSenseiNego);

INCLUDE_RODATA(const s32, "code_0024C470", D_00543880);

INCLUDE_RODATA(const s32, "code_0024C470", D_005438A0);

INCLUDE_RODATA(const s32, "code_0024C470", D_005438B0);

INCLUDE_RODATA(const s32, "code_0024C470", D_005438C0);

INCLUDE_ASM(const s32, "code_0024C470", CheckInotiNego);

INCLUDE_ASM(const s32, "code_0024C470", CheckBinjoNego);

INCLUDE_ASM(const s32, "code_0024C470", CheckDeadScript);

INCLUDE_ASM(const s32, "code_0024C470", CheckKoketaNego);

void KyouseiPress(nbActionProcessData_t *a)
{
    nbMainProcessData_t *data = a->data;

    data->unk78 = 0;
    data->unk7A = 0;
    if (data->unkE8 != NULL) {
        dds3KillProcess(data->unkE8, 0);
    }
}

INCLUDE_ASM(const s32, "code_0024C470", DokodemoPoa);

INCLUDE_ASM(const s32, "code_0024C470", SetNowActionStat);

INCLUDE_ASM(const s32, "code_0024C470", SetDamagePacket);

void SetAction_NON(void) {
}

INCLUDE_ASM(const s32, "code_0024C470", CheckAction_NON);

INCLUDE_ASM(const s32, "code_0024C470", SetAction_COMM);

INCLUDE_ASM(const s32, "code_0024C470", CheckAction_COMM);

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

INCLUDE_ASM(const s32, "code_0024C470", CheckAction_AISCR);

void SetAction_DEADSCRIPT(nbActionProcessData_t *a)
{
    ClearTimeList(a);
    SetHpForTimeList(a);
    nbInitAiScript(a, a->unk54);
}

INCLUDE_ASM(const s32, "code_0024C470", CheckAction_DEADSCRIPT);

void SetAction_BAD(nbActionProcessData_t *a)
{
    nbPanelAllShow();
    nbPanelPressGaugeShow();
}

INCLUDE_ASM(const s32, "code_0024C470", CheckAction_BAD);

void SetAction_BADWAIT(nbActionProcessData_t *a)
{
    a->unk04 |= 0x20;
    if ((a->data->unk60 & 8) != 0) {
        nbCameraSetState(0x19);
        return;
    }
    nbCameraSetState(3);
}

INCLUDE_ASM(const s32, "code_0024C470", CheckAction_BADWAIT);

void SetAction_TARSEL(nbActionProcessData_t *a)
{
    SetTarget_nbActionProcess(a);
}

INCLUDE_ASM(const s32, "code_0024C470", CheckAction_TARSEL);

void SetAction_STKSEL(nbActionProcessData_t *a)
{
    a->unk3C = 0;
    nbPanelStockSelectInit();
    nbPanelStockSelect(a->data, 0);
    StkSelId = -1;
}

INCLUDE_ASM(const s32, "code_0024C470", CheckAction_STKSEL);

INCLUDE_ASM(const s32, "code_0024C470", SetAction_READY);

INCLUDE_ASM(const s32, "code_0024C470", CheckAction_READY);

INCLUDE_ASM(const s32, "code_0024C470", SetAction_SKILL);

s32 CheckAction_SKILL(nbActionProcessData_t *a)
{
    SetNextActionSeq(a, 0x12);
    return 1;
}

INCLUDE_RODATA(const s32, "code_0024C470", D_00543E20);

INCLUDE_RODATA(const s32, "code_0024C470", D_00543E38);

INCLUDE_RODATA(const s32, "code_0024C470", D_00543E58);

INCLUDE_RODATA(const s32, "code_0024C470", D_00543E78);

INCLUDE_RODATA(const s32, "code_0024C470", D_00543E98);

INCLUDE_RODATA(const s32, "code_0024C470", D_00543EB8);

INCLUDE_RODATA(const s32, "code_0024C470", D_00543EE8);

INCLUDE_RODATA(const s32, "code_0024C470", jtbl_00543F00);

INCLUDE_RODATA(const s32, "code_0024C470", jtbl_00543F40);

INCLUDE_RODATA(const s32, "code_0024C470", jtbl_00543F80);

INCLUDE_ASM(const s32, "code_0024C470", SetAction_NEGO);

INCLUDE_ASM(const s32, "code_0024C470", CheckAction_NEGO);

INCLUDE_ASM(const s32, "code_0024C470", SetAction_NEGORESULT);

s32 CheckAction_NEGORESULT(nbActionProcessData_t *a)
{
    SetNextActionSeq(a, 0x12);
    return 1;
}

INCLUDE_ASM(const s32, "code_0024C470", SetAction_WAIT);

s32 CheckAction_WAIT(nbActionProcessData_t *a)
{
    SetNextActionSeq(a, 0x12);
    return 1;
}

INCLUDE_ASM(const s32, "code_0024C470", SetAction_ESCAPE);

INCLUDE_ASM(const s32, "code_0024C470", CheckAction_ESCAPE);

INCLUDE_ASM(const s32, "code_0024C470", SetAction_RETURN);

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

INCLUDE_RODATA(const s32, "code_0024C470", D_00544020);

INCLUDE_ASM(const s32, "code_0024C470", SetAction_CHECK1);

INCLUDE_ASM(const s32, "code_0024C470", CheckAction_CHECK1);

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

INCLUDE_ASM(const s32, "code_0024C470", SetAction_END);

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

INCLUDE_RODATA(const s32, "code_0024C470", D_00544070);

INCLUDE_RODATA(const s32, "code_0024C470", D_00544088);

INCLUDE_RODATA(const s32, "code_0024C470", D_005440A0);

INCLUDE_RODATA(const s32, "code_0024C470", D_005440B0);

INCLUDE_RODATA(const s32, "code_0024C470", D_005440C0);

INCLUDE_RODATA(const s32, "code_0024C470", D_005440D8);

INCLUDE_RODATA(const s32, "code_0024C470", D_005440E8);

INCLUDE_ASM(const s32, "code_0024C470", nbActionProcess1);

INCLUDE_ASM(const s32, "code_0024C470", nbActionShutdown);

INCLUDE_ASM(const s32, "code_0024C470", InitActionProcessData);

INCLUDE_ASM(const s32, "code_0024C470", nbInitActionProcess);

INCLUDE_ASM(const s32, "code_0024C470", nbAddOrder);

INCLUDE_ASM(const s32, "code_0024C470", nbSortOrder);

INCLUDE_ASM(const s32, "code_0024C470", nbSetOrderSide);

void nbSetOrder(void)
{
    if ((nbGetMainProcessData()->unk60 & 8) == 0) {
        nbSetOrderSide(0);
        return;
    }
    nbSetOrderSide(1);
}

INCLUDE_ASM(const s32, "code_0024C470", nbSetNextOrder);

INCLUDE_ASM(const s32, "code_0024C470", nbUpdateOrder);

INCLUDE_ASM(const s32, "code_0024C470", nbClipEffectNum);

INCLUDE_ASM(const s32, "code_0024C470", nbLoadKoukaPos);

INCLUDE_ASM(const s32, "code_0024C470", nbGetKoukaMarkPos);

INCLUDE_ASM(const s32, "code_0024C470", nbMakeUnitMatrix);

INCLUDE_ASM(const s32, "code_0024C470", nbMakeUnitDrawMatrix);

INCLUDE_ASM(const s32, "code_0024C470", nbDrawRotatedModelX);

INCLUDE_ASM(const s32, "code_0024C470", nbDrawRotatedModel);

INCLUDE_ASM(const s32, "code_0024C470", nbGetUnitMarkPos);

INCLUDE_ASM(const s32, "code_0024C470", nbDrawPanelSprite);

INCLUDE_ASM(const s32, "code_0024C470", nbDrawKoukaSprite);

INCLUDE_RODATA(const s32, "code_0024C470", D_00544130);

INCLUDE_RODATA(const s32, "code_0024C470", D_00544140);

INCLUDE_ASM(const s32, "code_0024C470", nbSetTargetSkill);

INCLUDE_ASM(const s32, "code_0024C470", nbSetNormalSkill0);

INCLUDE_ASM(const s32, "code_0024C470", nbGetNakama);

void nbSetDevilNameMes(s32 hmsg, s32 no, datUnitWork_t *work)
{
    if (work->id == 0) {
        itfMesMngSetVar(hmsg, 0, 0, 10);
        return;
    }
    itfMesMngSetVar(hmsg, no, work->id, 1);
}

INCLUDE_ASM(const s32, "code_0024C470", nbSetJyokyoMes);

INCLUDE_ASM(const s32, "code_0024C470", nbGetJyokyoMes);

INCLUDE_ASM(const s32, "code_0024C470", nbGetModelColor);

INCLUDE_ASM(const s32, "code_0024C470", nbAppendEnemyDevilWork);

INCLUDE_ASM(const s32, "code_0024C470", nbGetRandamStock);

INCLUDE_ASM(const s32, "code_0024C470", nbSetMainPhase);

INCLUDE_ASM(const s32, "code_0024C470", nbCountActiveUnits);

INCLUDE_ASM(const s32, "code_0024C470", nbSetupBattleUnits);

INCLUDE_ASM(const s32, "code_0024C470", nbSetPlayerStartData);

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
    s32 id = nbGetUnitID(nbGetUnitWorkFromFormindex(form->unk92));
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
    s32 id = nbGetUnitID(nbGetUnitWorkFromFormindex(form->unk92));
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
    s32 id = nbGetUnitID(nbGetUnitWorkFromFormindex(form->unk92));
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

INCLUDE_ASM(const s32, "code_0024C470", nbAddKagututiMoon);

s32 nbGetItemSkillNo(nbActionProcessData_t *a, s32 itemid)
{
    s32 no = datGetItemSkillNo(itemid);

    if (itemid == 1) {
        no = a->unk64;
    }
    return no;
}

INCLUDE_ASM(const s32, "code_0024C470", GetCommListCnt);

nbCommSelProcessData_t *GetCommSelProcessData(void)
{
    return dds3GetProcessData(dds3SearchProcessName(D_005441A8));
}

INCLUDE_ASM(const s32, "code_0024C470", DispPanelCursor);

INCLUDE_RODATA(const s32, "code_0024C470", D_00544170);

INCLUDE_RODATA(const s32, "code_0024C470", D_00544190);

INCLUDE_RODATA(const s32, "code_0024C470", D_005441A8);

INCLUDE_ASM(const s32, "code_0024C470", DispPanelLabel);

INCLUDE_ASM(const s32, "code_0024C470", DispPanelSitaji);

INCLUDE_ASM(const s32, "code_0024C470", DispComm1PanelComm);

void DispComm1PanelTalk(void) {
}

INCLUDE_ASM(const s32, "code_0024C470", DispComm1PanelItem);

INCLUDE_ASM(const s32, "code_0024C470", DispHpPanel);

INCLUDE_ASM(const s32, "code_0024C470", DispMpPanel);

INCLUDE_ASM(const s32, "code_0024C470", DispComm1PanelSummon);

INCLUDE_ASM(const s32, "code_0024C470", DispCommCursor);

INCLUDE_ASM(const s32, "code_0024C470", DispSummonCursor);

INCLUDE_ASM(const s32, "code_0024C470", DispComm1PanelCursor);

INCLUDE_ASM(const s32, "code_0024C470", DispHelpPanel);

INCLUDE_ASM(const s32, "code_0024C470", DispCommandPanel);

INCLUDE_ASM(const s32, "code_0024C470", AnimeCommandList);

INCLUDE_ASM(const s32, "code_0024C470", DispComm1Summon);

INCLUDE_ASM(const s32, "code_0024C470", DispComm1Item);

INCLUDE_ASM(const s32, "code_0024C470", DispComm1Talk);

INCLUDE_ASM(const s32, "code_0024C470", DispComm1Comm);

INCLUDE_ASM(const s32, "code_0024C470", DispHelp1);

INCLUDE_ASM(const s32, "code_0024C470", DispCommandList2);

INCLUDE_ASM(const s32, "code_0024C470", CheckSelectError);

INCLUDE_ASM(const s32, "code_0024C470", MemoryCommand);

INCLUDE_ASM(const s32, "code_0024C470", GetNextCommType);

INCLUDE_ASM(const s32, "code_0024C470", SelectCommandList);

INCLUDE_ASM(const s32, "code_0024C470", SelectCommandListAuto);

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

INCLUDE_ASM(const s32, "code_0024C470", nbCommSelProcess1);

void nbCommSelShutdown(dds3ProcessID_t *id)
{
    nbCommSelProcessData_t *s = dds3GetProcessData(id);
    nbMainProcessData_t *data = s->data;

    if (data->unkE8 == id) {
        data->unkE8 = NULL;
    }
    sdfMemFreeDirect(s->handle);
}

INCLUDE_ASM(const s32, "code_0024C470", SetCommList);

INCLUDE_ASM(const s32, "code_0024C470", SetPrevCursor);

INCLUDE_ASM(const s32, "code_0024C470", SetCommCursor2);

void nbSetAutoCommSel(void)
{
    nbCommSelProcessData_t *s = GetCommSelProcessData();

    SetCommCursor2(s);
    s->unk14B4 |= 0x10;
}

INCLUDE_ASM(const s32, "code_0024C470", InitCommSelProcessData);

INCLUDE_ASM(const s32, "code_0024C470", nbInitCommSelProcess);
INCLUDE_SDATA(const s32, "code_0024C470", D_00551110);

INCLUDE_SDATA(const s32, "code_0024C470", D_00551114);

INCLUDE_SDATA(const s32, "code_0024C470", D_00551118);

INCLUDE_SDATA(const s32, "code_0024C470", StkSelId);

INCLUDE_SDATA(const s32, "code_0024C470", D_00551124);

INCLUDE_SDATA(const s32, "code_0024C470", D_00551128);

INCLUDE_SDATA(const s32, "code_0024C470", D_00551130);

INCLUDE_SDATA(const s32, "code_0024C470", D_00551134);

