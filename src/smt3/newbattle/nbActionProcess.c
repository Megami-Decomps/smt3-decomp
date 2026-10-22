#include "common.h"
#include "kernel.h"
#include "nb.h"
#include "ps2asm.h"
#include "model.h"
#include "event.h"
#include "motion.h"
#include "field.h"

extern char D_00543FB8[]; /* "nego result = %d\n" */

extern char D_00544020[];

extern s32 printf(const char *format, ...);

extern nbFormation_t *nbGetEventForm();

extern void nbPanelDraw(s32 flag);

extern void *sdfChipAllocDirect(u32 aSize);

extern s32 datCheckPlayerDead(void);

extern s32 dds3ExistProcess(dds3ProcessID_t *id);

/* The quadword copy of a formation position: 16-byte aligned, so ee-gcc moves
 * it with one lq/sq pair. */
typedef struct nbQuadVector_t {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} __attribute__((aligned(16))) nbQuadVector_t;

/* 8-byte rows at +0x88 indexed by formation slot. */
typedef struct nbTimeList_t {
    u16 lockframe;
    u16 end;
    u16 hp;
    u16 pad06;
} nbTimeList_t;

typedef struct nbActionProcessData_t {
    nbMainProcessData_t *data;
    u32 stat;
    s8 seq;
    s8 nextseq;
    s16 seqtime;
    s32 seqcnt;
    s32 skilltime;
    f32 press;
    s32 newpresstype;
    s16 newpress_p;
    s16 newpress_ten;
    s32 newaddpresstype;
    struct nbParty_t *party;
    nbFormation_t *form;
    datUnitWork_t *work;
    s32 partyindex;
    s32 type;
    s32 target;
    u32 select;
    u32 timer;
    s32 runerr;
    s32 jyokyomes;
    s32 jyo_varid;
    s32 jyo_varind;
    s32 adata;
    s32 autoskill;
    s32 allhpdamage;
    s32 allmpdamage;
    s32 massuruid;
    u32 negotype;
    s32 motion;
    s32 motse;
    void *hEffBTL;
    u8 unk78[0x10];
    nbTimeList_t timelist[32];
    u32 packettime;
    s32 hitstopframe;
    u8 unk190[0x10];
    u32 aisummonid;
} nbActionProcessData_t;

/* 128-byte party records. */
struct nbParty_t {
    u16 flag;
    s16 formindex;
    u8 unk04[0x7C];
};

typedef struct nbActionFunc_t {
    void (*set)(nbActionProcessData_t *a);
    s32 (*check)(nbActionProcessData_t *a);
} nbActionFunc_t;

/* 164-byte rows indexed by datUnitWork_t::id. */
typedef struct nbDevilData_t {
    u16 aiscript;
    u8 unk02[0xA2];
} nbDevilData_t;

typedef struct datDevilNegoFormat_t {
    u8 unk00[0xF];
    u8 talkflag;
    u8 unk10[0x20];
} datDevilNegoFormat_t;

extern datDevilNegoFormat_t master_text[];

extern char D_00543880[];

extern s32 evtBitCheck(s32 no);

extern s32 datCheckTalk(datUnitWork_t *work);

extern s32 evtGetAgeOfMoon(void);

extern char D_00543FA8[]; /* "NEGO p:%d e:%d\n" */

extern void nbInitNegoProcess(nbActionProcessData_t *actdata, s32 pformindex, s32 eformindex,
                              u32 negotype, s32 skillid);

extern struct nbKoukaPacket_t *nbAddUnitAKoukaPacket(s32 startframe, nbActionProcessData_t *uniqueid,
                                                     nbFormation_t *form, s32 a, s32 frame);

extern s32 effPCPBossONGY2GetLength(void);

extern s32 effPCPBossONGYGetLength(void);

extern struct nbKoukaPacket_t *nbAddOngOutPacket(s32 startframe, nbActionProcessData_t *uniqueid,
                                                 nbFormation_t *form);

extern struct nbKoukaPacket_t *nbAddMotionKoukaPacket(s32 startframe, nbActionProcessData_t *uniqueid,
                                                      nbFormation_t *form, s32 motion);

extern struct nbKoukaPacket_t *nbAddOngOut2Packet(s32 startframe, nbActionProcessData_t *uniqueid,
                                                  nbFormation_t *form);

extern s32 nbCheckSingleTargetSkill(s32 skillid);

extern struct nbKoukaPacket_t *nbAddSakeKoukaPacket(s32 startframe, nbActionProcessData_t *uniqueid,
                                                    nbFormation_t *form);

extern char D_00544070[];

extern char D_00544088[];

extern char D_005440A0[];

extern char D_005440B0[];

extern char D_005440C0[];

extern char D_005440D8[];

extern void nbPlaySe(s32 no);

extern struct nbKoukaPacket_t *nbAddSePacket(s32 startframe, void *uniqueid, s32 se);

extern void nbPanelStockEnd(void);

extern void *memset(void *a, s32 b, u32 c);

extern void dds3FadeOut(u32 r, u32 g, u32 b, u32 frame);

extern void *dds3GetProcessData(dds3ProcessID_t *id);

extern void SetTarget_nbActionProcess(nbActionProcessData_t *a);

extern s32 AddTimeListHp(nbActionProcessData_t *a, s32 formindex, s32 add);

extern s32 nbMakeDamagePacket(s32 startframe, nbActionProcessData_t *uniqueid, nbFormation_t *form,
                              s32 type, s32 hp, s32 mp, s32 badstat, s32 hiteff, s32 dead,
                              s32 deadtype, s32 moji, s32 animeoff);

extern struct nbKoukaPacket_t *nbAddSkillNamePacket(s32 startframe, nbActionProcessData_t *uniqueid,
                                                    s32 id);

extern s32 nbCheckKuisibariSkill(s32 formindex);

extern s32 nbCheckKuisibari2Skill(s32 formindex);

extern void nbSetKuisibariStop(s32 formindex);

extern void nbPanelAllShow(void);

extern void nbPanelPressGaugeShow(void);

extern s32 nbRunAi(nbActionProcessData_t *a);

extern void ClearTimeList(nbActionProcessData_t *a);

extern void SetHpForTimeList(nbActionProcessData_t *a);

extern void nbInitAiScript(nbActionProcessData_t *a, s32 no);

extern s32 nbGetDevilFormatFlag(s32 formindex);

extern s32 dds3KillProcess(dds3ProcessID_t *id, s32 shutdowntimer);

extern void nbPanelStockSelectInit(void);

extern u32 nbPanelStockSelect(nbMainProcessData_t *pMainData, u32 pad);

extern void nbMakeUnitFadeOutPacket(s32 startframe, nbActionProcessData_t *uniqueid, nbFormation_t *form,
                                    u32 col, s32 delay, s32 colframe, s32 aframe);

extern s32 nbAddReturnKoukaPacket(s32 startframe, nbActionProcessData_t *uniqueid, struct nbParty_t *party,
                                  s32 dead);

extern s32 nbCheckEscape(nbMainProcessData_t *data);

extern s32 nbMakePressPacket(s32 startframe, nbActionProcessData_t *uniqueid, f32 press);

extern s32 nbMakeNewPressPacket(s32 startframe, nbActionProcessData_t *uniqueid, s32 ptype,
                                nbFormation_t *form);

extern struct nbKoukaPacket_t *nbAddMojiEffKoukaPacket(s32 startframe, s32 frame,
                                                       nbActionProcessData_t *uniqueid,
                                                       nbFormation_t *form, s32 type);

extern struct nbKoukaPacket_t *nbAddFormOnOffPacket(s32 startframe, nbActionProcessData_t *uniqueid,
                                                    nbFormation_t *form, s32 sw);

s32 CheckKoketaNego(nbActionProcessData_t *a);

extern void *memset(void *a, s32 b, u32 c);

extern char D_00543768[]; /* "nb_unitdraw" */

extern char D_00544120[]; /* "nb_action" */

void *nbActionProcess1(dds3ProcessID_t *id);

void nbActionShutdown(dds3ProcessID_t *id);

nbActionProcessData_t *InitActionProcessData(nbMainProcessData_t *data, s32 type,
                                            s32 from, s32 to, s32 d);

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, void *drawExit,
                                         void *deleteExit, void *p);

extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);

extern void nbMakeDeadVisualPacket(s32 startframe, nbActionProcessData_t *uniqueid, nbFormation_t *form,
                                   s32 deadtype, s32 deadpcm);

void SetDeadPacket(nbActionProcessData_t *a, s32 formindex);

extern void nbPushAction(s32 type, s32 from, s32 to, s32 d);

extern void nbSetTargetSkill(nbActionProcessData_t *a, datUnitWork_t *work, s32 *ctype,
                             s32 *crule, s32 *carea, s32 mode);

extern void nbSetTargetProcess(nbActionProcessData_t *act, s32 partyindex, s32 ctype, s32 crule,
                               s32 carea);

extern void nbDispJyokyoHelp(s32 msgid, s32 no, s32 var_id, s32 var_ind);

extern s32 p2sdGetPcmState(void);

extern void p2sdSyncSound(void);

extern char D_005440E8[];

extern s32 nbMakeSummonPacket(s32 startframe, nbActionProcessData_t *uniqueid, s32 side, s32 stockno,
                              s32 formindex);

extern s32 nbGetTarSelStat(void);

extern void nbSetTarSelStat(s32 stat);

extern s32 nbCheckBattleSkillError2(s32 nskill, u32 select);

extern u32 nbGetSelectBit(nbMainProcessData_t *data);

extern s32 nbDispSkillErrorMessage(s32 err);

extern void nbSetTarSelJyokyoTime(s32 time);

extern s32 GetEndTime(nbActionProcessData_t *a, s32 formindex);

extern s32 nbGetExpMakaItem(datUnitWork_t *w);

extern s32 nbMakeDeadPacket(s32 startframe, nbActionProcessData_t *uniqueid, nbFormation_t *form,
                            s32 deadchk);

extern void sdfChipFreeDirect(void *aPtr);

extern s32 dds3GetRandIntA(s32 aLimit, s32 name);

s32 CheckTimeListDead(nbActionProcessData_t *a, s32 formindex);

s32 nbCheckDeadScript(nbFormation_t *form);

extern s32 StkSelId;

extern nbActionFunc_t ActionFuncTable[];

extern s32 nbGetUniqueIdKoukaPacketCnt(nbActionProcessData_t *uniqueid);

extern void effBTLDelete(void *hBTLEffect);

extern void nbCameraSetState(u16 state);

extern datUnitWork_t *nbGetUnitWorkFromFormindex(s32 formindex);

extern nbDevilData_t D_003989E0[];

extern void nbAddFlareChargePacket(s32 startframe, nbActionProcessData_t *uniqueid, s32 point);

void SetNextActionSeq(nbActionProcessData_t *a, s32 nextseq);

void SetDamagePacket(nbActionProcessData_t *a, s32 hp);

void SetTimeList(nbActionProcessData_t *a, s32 formindex, s32 start, s32 locktime, s32 endtime);

extern void nbAddAnalyzePacket(s32 startframe, nbActionProcessData_t *uniqueid, nbFormation_t *form);

extern void nbPanelAnalyzeStart(void);

extern struct nbParty_t *nbGetPartyFromFormindex(s32 formindex);

void nbSetJyokyoMes(nbActionProcessData_t *a, s32 jyokyomes, s32 var_id, s32 var_ind);

extern s32 nbGetMakaBaramaki(datUnitWork_t *w);

extern s32 nbCheckFormationPlayerSide(s32 formindex);

extern void datAddMaka(s32 addmaka);

extern s32 nbGetHojoCounter(s32 formindex, s32 type);

extern void nbSetHojoCounter(s32 formindex, s32 type, s32 point);

extern s32 nbMakeReturnPacket(s32 startframe, nbActionProcessData_t *uniqueid, struct nbParty_t *party);

extern void nbDelAllKoukaPacketType(s32 type);

extern void nbInitCommSelProcess(nbMainProcessData_t *data, struct nbParty_t *my,
                                 nbActionProcessData_t *act);

extern datUnitWork_t *nbGetUnitWorkPointer(nbMainProcessData_t *data, struct nbParty_t *party);

#ifdef NON_MATCHING

#else

#endif

#ifdef NON_MATCHING

#else

#endif

#ifdef NON_MATCHING

#else

#endif

#ifdef NON_MATCHING

#else

#endif

void SetDeadPacket(nbActionProcessData_t *a, s32 formindex)
{
    struct nbParty_t *party = &a->data->party[a->data->form[formindex].partyindex];
    s32 start;
    datUnitWork_t *work;

    if ((party->flag & 0x20) != 0) {
        return;
    }
    start = GetEndTime(a, formindex);
    work = nbGetUnitWorkPointer(a->data, party);
    if ((work->flag & 0x20) != 0) {
        nbGetExpMakaItem(work);
    }
    nbMakeDeadPacket(start, a, &a->data->form[formindex], 0);
}

void SetPress(nbActionProcessData_t *a, f32 press)
{
    if (press < -1.0f) {
        if (press < a->press) {
            a->press = press;
        }
    } else if (press > -1.0f) {
        if (a->press >= -1.0f) {
            if (a->press < press || a->press == 0.0f) {
                a->press = press;
            }
        }
    } else if (a->press == 0.0f) {
        a->press = press;
    }
}

void SetNewPress(nbActionProcessData_t *a, s32 presstype)
{
    if (a->newpresstype < presstype) {
        a->newpresstype = presstype;
    }
}

void SetHitStopFrame(nbActionProcessData_t *a, s32 frame)
{
    frame += 7;
    if (a->hitstopframe == 0 || frame < a->hitstopframe) {
        a->hitstopframe = frame;
    }
}

void SetEnemyEscapePacket(nbActionProcessData_t *a, s32 dformindex, s32 sframe)
{
    nbMakeReturnPacket(sframe, a, nbGetPartyFromFormindex(dformindex));
    SetTimeList(a, dformindex, sframe, 0x1E, 0x1E);
}

void SetAuroraPacket(nbActionProcessData_t *a, s32 formindex)
{
    s32 counter;

    nbGetPartyFromFormindex(formindex);
    counter = nbGetHojoCounter(formindex, 0x13);
    switch (counter) {
    case 0:
        counter = 0xF8;
        break;
    case 0xF8:
        counter = 0xF9;
        break;
    case 0xF9:
        counter = 0xFA;
        break;
    case 0xFA:
        counter = 0xFB;
        break;
    case 0xFB:
        counter = 0xF8;
        break;
    }
    nbSetHojoCounter(formindex, 0x13, counter);
    nbSetJyokyoMes(a, 0x5A, 1, nbGetUnitWorkFromFormindex(formindex)->id);
}

void SetAddPressPacket(nbActionProcessData_t *a, s32 nskill)
{
    if (datSkillTable.normal[nskill].program != 13) {
        return;
    }
    switch (datSkillTable.normal[nskill].hojopoint) {
    case 2:
        a->newaddpresstype = 0xF;
        break;
    case 3:
        a->newaddpresstype = 0x11;
        break;
    case 4:
        a->newaddpresstype = 0x10;
        break;
    }
}

void SetAddFlareChargePacket(nbActionProcessData_t *a, s32 start)
{
    nbAddFlareChargePacket(start, a, 2);
}

void SetMakaPacket(nbActionProcessData_t *a, s32 dformindex, s32 sframe)
{
    s32 maka = nbGetMakaBaramaki(a->work);

    if (nbCheckFormationPlayerSide(a->form->formindex) != 0) {
        s32 limit = DDS3_GBWK->maka;

        if (limit < maka) {
            maka = limit;
        }
    }
    SetTimeList(a, a->form->formindex, sframe, 0x1E, 0x1E);
    nbSetJyokyoMes(a, 0x2C, 0, maka);
    if (nbCheckFormationPlayerSide(a->form->formindex) != 0) {
        datAddMaka(-maka);
    } else {
        datAddMaka(maka);
    }
}

void SetTorafuPacket(nbActionProcessData_t *a, s32 dformindex, s32 sframe)
{
    nbFormation_t *form = &a->data->form[dformindex];

    nbMakeUnitFadeOutPacket(sframe, a, form, 0, 6, 6, 6);
    nbAddFormOnOffPacket(sframe + 12, a, form, 0);
    a->data->endtype = 3;
}

void SetAnalyzePacket(nbActionProcessData_t *a, s32 dformindex, s32 sframe)
{
    nbAddAnalyzePacket(sframe + 2, a, &a->data->form[dformindex]);
    nbPanelAnalyzeStart();
    a->stat |= 0x10;
}

s32 SetPlayerSummonPacket(nbActionProcessData_t *a, s32 nskill, s32 sformindex,
                          s32 dformindex, s32 sframe)
{
    datUnitWork_t *unit;
    nbMainProcessData_t *data;
    s32 f;
    s32 flag;
    s32 nowindex;

    data = a->data;
    f = a->party->flag & 2;
    flag = f != 0;
    if (flag == 0) {
        datUnitWork_t *work = a->work;

        if (work->nowcommand == 1 && work->nowindex == 0xDF) {
            nowindex = nbGetRandamStock();
            if (nowindex < 0) {
                nbSetJyokyoMes(a, 0x3E, -1, 0);
                return 0;
            }
        } else {
            nowindex = work->nowindex;
        }
    } else {
        nowindex = nbAppendEnemyDevilWork(data, a->aisummonid, dformindex);
        if (a->data->nakamayobi > 0) {
            a->data->nakamayobi--;
        }
    }
    nbMakeSummonPacket(sframe, a, flag, nowindex, dformindex);
    SetTimeList(a, a->form->formindex, sframe, 0xF, 0x14);
    if (flag == 0) {
        unit = (datUnitWork_t *)&DDS3_GBWK->akuma[nowindex].flag;
    } else {
        unit = &a->data->enemyunit[nowindex];
    }
    a->timelist[dformindex].hp = unit->hp;
    if (flag == 0) {
        nbSetJyokyoMes(a, 8, 1, unit->id);
    } else {
        nbSetJyokyoMes(a, 0x40, 1, unit->id);
    }
    return 1;
}

void SetSakePacket(nbActionProcessData_t *a, s32 nskill, s32 sformindex, s32 dformindex,
                   s32 cnt, s32 frame)
{
    nbAddSakeKoukaPacket(frame, a, &a->data->form[dformindex]);
    SetHitStopFrame(a, frame);
    SetTimeList(a, dformindex, frame, 0xA, 0x14);
    if (nbCheckSingleTargetSkill(nskill) != 0) {
        if (nbCheckFormationPlayerSide(dformindex) != 0) {
            nbSetJyokyoMes(a, 2, -1, 0);
        } else {
            nbSetJyokyoMes(a, 4, -1, 0);
        }
    }
    SetPress(a, -2.0f);
    SetNewPress(a, 4);
}

void SetMissPacket(nbActionProcessData_t *a, s32 nskill, s32 sformindex, s32 dformindex,
                   s32 cnt, s32 frame)
{
    nbAddMojiEffKoukaPacket(frame, 0x2D, a, &a->data->form[dformindex], 3);
    SetTimeList(a, dformindex, frame, 10, 20);
    SetPress(a, -2.0f);
    SetNewPress(a, 4);
}

void SetOngShadowPacket(nbActionProcessData_t *a, s32 nskill, s32 sformindex, s32 dformindex,
                        s32 cnt, s32 frame)
{
    s32 i;

    nbAddMojiEffKoukaPacket(frame, 0x2D, a, &a->data->form[dformindex], 4);
    nbAddOngOutPacket(frame, a, &a->data->form[dformindex]);
    nbAddUnitAKoukaPacket(frame + 1, a, &a->data->form[dformindex], 0, 0);
    nbMakeDeadPacket(frame + effPCPBossONGYGetLength(), a, &a->data->form[dformindex], 1);
    SetTimeList(a, dformindex, frame, 0x1E, 0x1E);
    frame += effPCPBossONGYGetLength();
    for (i = 4; i < 0xE; i++) {
        nbFormation_t *form;
        u32 stat;

        if (i == dformindex) {
            continue;
        }
        form = &a->data->form[i];
        stat = form->stat;
        if ((stat & 1) != 0) {
            if ((stat & 2) != 0) {
                nbAddMotionKoukaPacket(frame, a, form, 0xE);
                SetTimeList(a, i, frame, 0x14, 0x14);
            }
        }
    }
    SetPress(a, -2.0f);
    SetNewPress(a, 0xE);
}

void SetOngShadowPacket2(nbActionProcessData_t *a, s32 dformindex, s32 frame)
{
    s32 cnt = 0;
    s32 i;

    for (i = 4; i < 0xE; i++) {
        nbFormation_t *form;
        u32 stat;

        if (i == dformindex) {
            continue;
        }
        form = &a->data->form[i];
        stat = form->stat;
        if ((stat & 1) != 0) {
            if ((stat & 2) != 0) {
                nbAddUnitAKoukaPacket(frame + 1, a, form, 0, 0);
                cnt++;
                nbMakeDeadPacket(frame + effPCPBossONGY2GetLength() + 1, a, form, 1);
                nbAddOngOut2Packet(frame, a, form);
                SetTimeList(a, i, frame, 0x1E, 0x1E);
            }
        }
    }
    if (cnt > 0) {
        a->data->stat |= 0x4000;
    }
}

INCLUDE_ASM(const s32, "newbattle/nbActionProcess", SetAllPacket);

void SetTarget_nbActionProcess(nbActionProcessData_t *a)
{
    datUnitWork_t *work = a->work;
    s32 ctype;
    s32 crule;
    s32 carea;

    ctype = 0;
    crule = 0;
    carea = 0;
    switch (work->nowcommand) {
    case 6:
        *(s32 *)a->data->unk80 = 1;
        break;
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 7:
    case 8:
    default:
        if (a->work->nowcommand == 1 && a->work->nowindex == 0xDF) {
            *(s32 *)a->data->unk80 = 1;
        } else {
            *(s32 *)a->data->unk80 = 0;
        }
        break;
    }
    a->select = 0;
    nbSetTargetSkill(a, work, &ctype, &crule, &carea, 0);
    nbSetTargetProcess(a, a->partyindex, ctype, crule, carea);
}

void ClearTimeList(nbActionProcessData_t *a)
{
    s32 i;

    for (i = 0; i < 32; i++) {
        a->timelist[i].lockframe = 0;
        a->timelist[i].end = 0;
        a->timelist[i].hp = 0;
    }
}

void SetHpForTimeList(nbActionProcessData_t *a)
{
    s32 i;

    for (i = 0; i < 16; i++) {
        a->timelist[i].hp = 0;
        if ((a->data->form[i].stat & 1) != 0) {
            if ((a->data->form[i].stat & 0x40) != 0) {
                a->timelist[i].hp =
                    nbGetUnitWorkPointer(a->data,
                                         &a->data->party[a->data->form[i].partyindex])
                        ->hp;
            }
        }
    }
}

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

s32 CheckTimeListDeadEnemyParty(nbActionProcessData_t *a, s32 formindex)
{
    s32 start;
    s32 cnt;
    s32 i;

    if (formindex < 4) {
        start = 4;
        cnt = 10;
    } else {
        start = 0;
        cnt = 4;
    }
    for (i = start; i < start + cnt; i++) {
        if ((a->data->form[i].stat & 1) != 0) {
            if (CheckTimeListDead(a, i) == 0) {
                return 0;
            }
        }
    }
    return 1;
}

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
    return a->timelist[formindex].lockframe;
}

s32 GetEndTime(nbActionProcessData_t *a, s32 formindex)
{
    return a->timelist[formindex].end;
}

void OverwriteTimeList(nbActionProcessData_t *a, s32 formindex, s32 start, s32 locktime, s32 endtime)
{
    if (locktime >= 0) {
        if (a->timelist[formindex].lockframe < start + locktime) {
            a->timelist[formindex].lockframe = start + locktime;
        }
    }
    if (endtime >= 0) {
        if (a->timelist[formindex].end < start + endtime) {
            a->timelist[formindex].end = start + endtime;
        }
        if (a->timelist[formindex].end < a->timelist[formindex].lockframe) {
            a->timelist[formindex].end = a->timelist[formindex].lockframe;
        }
    }
}

void SetTimeList(nbActionProcessData_t *a, s32 formindex, s32 start, s32 locktime, s32 endtime)
{
    if (start < a->timelist[formindex].lockframe) {
        start = a->timelist[formindex].lockframe;
    }
    if (locktime >= 0) {
        if (a->timelist[formindex].lockframe < start + locktime) {
            a->timelist[formindex].lockframe = start + locktime;
        }
    }
    if (endtime >= 0) {
        if (a->timelist[formindex].end < start + endtime) {
            a->timelist[formindex].end = start + endtime;
        }
        if (a->timelist[formindex].end < a->timelist[formindex].lockframe) {
            a->timelist[formindex].end = a->timelist[formindex].lockframe;
        }
    }
}

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
        if (max < a->timelist[i].lockframe) {
            max = a->timelist[i].lockframe;
        }
    }
    return max;
}

INCLUDE_RODATA(const s32, "newbattle/nbActionProcess", D_005437F8);

INCLUDE_ASM(const s32, "newbattle/nbActionProcess", GetKoukaList);

void SetTargetPlayer(nbActionProcessData_t *a)
{
    nbFormation_t *form = a->data->form;
    s32 i;

    for (i = 0; i < 16; i++) {
        form[i].stat &= ~4;
        form[i].stat &= ~8;
    }
    for (i = 0; i < 16; i++) {
        if ((a->data->form[i].stat & 1) != 0) {
            if ((a->data->form[i].stat & 0x40) != 0) {
                if ((a->data->form[i].stat & 0x100) != 0) {
                    if ((nbGetUnitWorkFromFormindex(i)->flag & 4) != 0) {
                        a->data->form[i].stat |= 8;
                        return;
                    }
                }
            }
        }
    }
}

void SetTargetParty(nbActionProcessData_t *a, s32 partyindex)
{
    nbFormation_t *form;
    nbFormation_t *p;
    s32 i;

    form = a->data->form;
    for (i = 0; i < 16; i++) {
        form->stat &= ~4;
        form->stat &= ~8;
        form++;
    }
    p = a->data->form;
    for (i = 0; i < 16; i++) {
        if ((p->stat & 1) != 0) {
            if ((p->stat & 0x40) != 0) {
                if (p->partyindex == partyindex) {
                    p->stat |= 8;
                    return;
                }
            }
        }
        p++;
    }
}

s32 CheckArrCounter(nbActionProcessData_t *a)
{
    nbFormation_t *form;
    s32 hit;

    if (a->partyindex >= 4) {
        return 0;
    }
    form = nbGetEventForm();
    if (CheckTimeListDead(a, form->formindex) != 0) {
        return 0;
    }
    if (nbGetUnitWorkFromFormindex(form->formindex)->id != 0x123) {
        return 0;
    }
    hit = 0;
    if (evtBitCheck(0x851) != 0) {
        if ((a->stat & 0x10000) != 0) {
            hit = 1;
        }
    }
    if (evtBitCheck(0x852) != 0) {
        if ((a->stat & 0x20000) != 0) {
            hit = 1;
        }
    }
    if (evtBitCheck(0x853) != 0) {
        if ((a->stat & 0x40000) != 0) {
            hit = 1;
        }
    }
    if (evtBitCheck(0x854) != 0) {
        if ((a->stat & 0x80000) != 0) {
            hit = 1;
        }
    }
    if (evtBitCheck(0x855) != 0) {
        if ((a->stat & 0x100000) != 0) {
            hit = 1;
        }
    }
    if (evtBitCheck(0x856) != 0) {
        if ((a->stat & 0x200000) != 0) {
            hit = 1;
        }
    }
    if (hit == 0) {
        return 0;
    }
    nbPushAction(4, form->partyindex, a->partyindex, 0xF3);
    printf(D_00543880);
    return 1;
}

INCLUDE_ASM(const s32, "newbattle/nbActionProcess", CheckCounter);

INCLUDE_ASM(const s32, "newbattle/nbActionProcess", CheckMukanNego);

/* NON_MATCHING: 57 of 74 words differ (first at +0x4). */

#ifdef NON_MATCHING

s32 CheckSenseiNego(nbActionProcessData_t *a)
{
    u32 stat;
    datUnitWork_t *work;
    s32 rate;

    if ((u16)a->data->StageMajor >= 0xF0 && (u16)a->data->StageMajor <= 0xF6) {
        return 0;
    }
    stat = a->data->stat;
    if ((stat & 1) != 0) {
        return 0;
    }
    if ((stat & 0x80) == 0) {
        return 0;
    }
    if ((stat & 0x10) != 0) {
        return 0;
    }
    if ((stat & 0x100) != 0) {
        return 0;
    }
    if (evtGetAgeOfMoon() == 8) {
        return 0;
    }
    work = a->work;
    if (master_text[work->id].talkflag == 0) {
        return 0;
    }
    if (datCheckTalk(work) == 0) {
        return 0;
    }
    if (datCheckTalk((datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54)) == 0) {
        return 0;
    }
    if ((a->data->stat & 0x2000000) != 0) {
        rate = 0x64;
    } else {
        rate = 21 / a->data->enemypcnt;
    }
    if (dds3GetRandIntA(0, 0x64) < rate) {
        a->data->stat |= 0x100;
        return 1;
    }
    return 0;
}

#else

INCLUDE_ASM(const s32, "newbattle/nbActionProcess", CheckSenseiNego);

#endif

INCLUDE_RODATA(const s32, "newbattle/nbActionProcess", D_00543880);

INCLUDE_RODATA(const s32, "newbattle/nbActionProcess", D_005438A0);

INCLUDE_RODATA(const s32, "newbattle/nbActionProcess", D_005438B0);

INCLUDE_RODATA(const s32, "newbattle/nbActionProcess", D_005438C0);

INCLUDE_ASM(const s32, "newbattle/nbActionProcess", CheckInotiNego);

INCLUDE_ASM(const s32, "newbattle/nbActionProcess", CheckBinjoNego);

void CheckDeadScript(nbActionProcessData_t *a)
{
    s32 i;

    if (a->type == 5) {
        return;
    }
    for (i = 4; i < 14; i++) {
        nbFormation_t *form = &a->data->form[i];

        if ((form->stat & 1) != 0) {
            if (CheckTimeListDead(a, i) != 0) {
                s32 script = nbCheckDeadScript(form);

                if (script != 0) {
                    nbPushAction(5, form->partyindex, 0, script);
                }
            }
        }
    }
}

INCLUDE_ASM(const s32, "newbattle/nbActionProcess", CheckKoketaNego);

void KyouseiPress(nbActionProcessData_t *a)
{
    nbMainProcessData_t *data = a->data;

    data->press4_p = 0;
    data->press4_ten = 0;
    if (data->CommSelProcessID != NULL) {
        dds3KillProcess(data->CommSelProcessID, 0);
    }
}

void DokodemoPoa(nbActionProcessData_t *a)
{
    s32 i;

    for (i = 4; i < 14; i++) {
        nbFormation_t *form = &a->data->form[i];

        if ((form->stat & 1) != 0) {
            if ((form->stat & 0x40) != 0) {
                nbMakeDeadVisualPacket(0, a, form, 0, 1);
                SetTimeList(a, i, 0, 30, 30);
                SetDeadPacket(a, i);
            }
        }
    }
    if (a->data->CommSelProcessID != NULL) {
        dds3KillProcess(a->data->CommSelProcessID, 0);
    }
}

INCLUDE_ASM(const s32, "newbattle/nbActionProcess", SetNowActionStat);

/* NON_MATCHING: 4 of 120 words differ (first at +0x110). */

#ifdef NON_MATCHING

void SetDamagePacket(nbActionProcessData_t *a, s32 hp)
{
    s32 start = GetAllUnlockTime(a);
    s32 dead;

    if (nbCheckKuisibariSkill(a->form->formindex) != 0) {
        if (hp > 0) {
            if (hp >= GetTimeListHp(a, a->form->formindex)) {
                hp = GetTimeListHp(a, a->form->formindex) - 1;
                nbAddSkillNamePacket(start, a, 0x159);
                nbSetKuisibariStop(a->form->formindex);
            }
        }
    } else if (nbCheckKuisibari2Skill(a->form->formindex) != 0) {
        if (hp > 0) {
            if (hp >= GetTimeListHp(a, a->form->formindex)) {
                hp = GetTimeListHp(a, a->form->formindex) - 1;
                nbAddSkillNamePacket(start, a, 0x168);
                nbSetKuisibariStop(a->form->formindex);
            }
        }
    }
    AddTimeListHp(a, a->form->formindex, -hp);
    dead = CheckTimeListDead(a, a->form->formindex) != 0;
    nbMakeDamagePacket(start, a, a->form, 3, hp, -1, 0, 0, dead, 0, 1, 0);
    if (dead != 0) {
        SetTimeList(a, a->form->formindex, start, 0x1E, 0x1E);
        SetDeadPacket(a, a->form->formindex);
        SetTimeList(a, a->form->formindex, start, 0x14, 0x14);
    } else {
        SetTimeList(a, a->form->formindex, start, 0x1E, 0x1E);
    }
}

#else

INCLUDE_ASM(const s32, "newbattle/nbActionProcess", SetDamagePacket);

#endif

void SetAction_NON(void) {
}

INCLUDE_ASM(const s32, "newbattle/nbActionProcess", CheckAction_NON);

void SetAction_COMM(nbActionProcessData_t *a)
{
    if ((a->data->stat & 0x40) != 0) {
        a->stat |= 0x80;
    } else {
        nbDelAllKoukaPacketType(0x26);
    }
    nbPanelAllShow();
    nbPanelPressGaugeShow();
    nbInitCommSelProcess(a->data, a->party, a);
}

INCLUDE_ASM(const s32, "newbattle/nbActionProcess", CheckAction_COMM);

void SetAction_AI(nbActionProcessData_t *a)
{
    a->work->nowcommand = -1;
    a->work->nowindex = 0;
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

    work->nowcommand = -1;
    work->nowindex = 0;
    nbInitAiScript(a, D_003989E0[id].aiscript);
}

s32 CheckAction_AISCR(nbActionProcessData_t *a)
{
    if (dds3ExistProcess(a->data->AiScriptProcessID) != 0) {
        return 0;
    }
    a->data->AiScriptProcessID = NULL;
    if (a->data->negoresult == 0) {
        SetNextActionSeq(a, 7);
    } else {
        ClearTimeList(a);
        SetHpForTimeList(a);
        SetNextActionSeq(a, 10);
    }
    return 1;
}

void SetAction_DEADSCRIPT(nbActionProcessData_t *a)
{
    ClearTimeList(a);
    SetHpForTimeList(a);
    nbInitAiScript(a, a->adata);
}

s32 CheckAction_DEADSCRIPT(nbActionProcessData_t *a)
{
    if (dds3ExistProcess(a->data->AiScriptProcessID) == 0) {
        a->data->AiScriptProcessID = NULL;
        nbMakeUnitFadeOutPacket(0, a, a->form, 0, 10, 10, 10);
        nbAddReturnKoukaPacket(0x19, a, a->party, 1);
        SetTimeList(a, a->form->formindex, 0, 30, 30);
        SetNextActionSeq(a, 0x12);
        return 1;
    }
    return 0;
}

void SetAction_BAD(nbActionProcessData_t *a)
{
    nbPanelAllShow();
    nbPanelPressGaugeShow();
}

INCLUDE_ASM(const s32, "newbattle/nbActionProcess", CheckAction_BAD);

void SetAction_BADWAIT(nbActionProcessData_t *a)
{
    a->stat |= 0x20;
    if ((a->data->stat & 8) != 0) {
        nbCameraSetState(0x19);
        return;
    }
    nbCameraSetState(3);
}

s32 CheckAction_BADWAIT(nbActionProcessData_t *a)
{
    s32 ret = 0;

    switch (a->work->nowcommand) {
    case 0:
    case 1:
    case 4:
    case 7:
        if ((u16)a->seqtime == 5 && a->jyokyomes > 0) {
            nbDispJyokyoHelp(a->data->mesmngid, a->jyokyomes, a->jyo_varid, a->jyo_varind);
        }
        break;
    case 2:
    case 3:
    case 5:
    case 6:
    case 8:
    case 9:
    case 10:
    default:
        if ((u16)a->seqtime == 5 && a->jyokyomes > 0) {
            nbDispJyokyoHelp(a->data->mesmngid, a->jyokyomes, a->jyo_varid, a->jyo_varind);
        }
        break;
    }
    switch (a->work->nowcommand) {
    case 8:
    case 10:
        if ((u16)a->seqtime >= 0x1F) {
            ret = 1;
        }
        break;
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 9:
    default:
        if ((u16)a->seqtime >= 0x29 || a->jyokyomes == 0) {
            ret = 1;
        }
        break;
    }
    if (ret != 0) {
        SetNextActionSeq(a, 7);
        a->jyokyomes = 0;
        return 1;
    }
    return 0;
}

void SetAction_TARSEL(nbActionProcessData_t *a)
{
    SetTarget_nbActionProcess(a);
}

/* NON_MATCHING: 4 of 115 words differ (first at +0x120). */

#ifdef NON_MATCHING

s32 CheckAction_TARSEL(nbActionProcessData_t *a)
{
    s32 ret = 0;

    if ((a->data->stat & 0x40) != 0 && (a->stat & 0x80) == 0) {
        a->stat |= 0x80;
        nbSetCommSelStat(2);
        nbSetAutoCommSel();
        a->seq = 1;
        return 0;
    }
    switch (nbGetTarSelStat()) {
    case 0: {
        u32 sel = nbGetSelectBit(a->data);

        if ((a->stat & 0x80) == 0) {
            datUnitWork_t *work = a->work;
            s32 no;

            no = -1;
            if (work->nowcommand != 1) {
                if (work->nowcommand == 5) {
                    no = nbGetItemSkillNo(a, work->nowindex);
                }
            } else {
                no = work->nowindex;
            }
            if (no >= 0) {
                s32 err = nbCheckBattleSkillError2(no, sel);

                if (err != 0) {
                    nbDispSkillErrorMessage(err);
                    if (err == 4) {
                        nbSetTarSelJyokyoTime(0x1D);
                    }
                    nbSetTarSelStat(1);
                    break;
                }
            }
            nbPlaySe(8);
        }
        nbCloseCommSel();
        ret = 1;
        a->select = sel;
        SetNextActionSeq(a, 7);
        break;
    }
    case 1:
        break;
    case 2: {
        s32 i;

        for (i = 0; i < 16; i++) {
            a->data->form[i].stat &= ~8;
        }
        nbSetCommSelStat(2);
        a->seq = 1;
        break;
    }
    }
    return ret;
}

#else

INCLUDE_ASM(const s32, "newbattle/nbActionProcess", CheckAction_TARSEL);

#endif

void SetAction_STKSEL(nbActionProcessData_t *a)
{
    a->select = 0;
    nbPanelStockSelectInit();
    nbPanelStockSelect(a->data, 0);
    StkSelId = -1;
}

/* NON_MATCHING: 9 of 58 words differ (first at +0x38). */

#ifdef NON_MATCHING

s32 CheckAction_STKSEL(nbActionProcessData_t *a)
{
    nbFormation_t *form;
    s32 i;

    switch (StkSelId) {
    default:
        nbPanelStockSelect(a->data, 0);
        nbCloseCommSel();
        a->select = 1 << (StkSelId + 0x10);
        SetNextActionSeq(a, 7);
        a->stat |= 0x400;
        nbPlaySe(8);
        return 1;
    case -1:
        StkSelId = nbPanelStockSelect(a->data, 1);
        break;
    case -2:
        form = a->data->form;
        for (i = 0xF; i >= 0; i--) {
            form->stat &= ~8;
            form++;
        }
        nbSetCommSelStat(2);
        a->seq = 1;
        nbPlaySe(0xA);
        nbPanelStockEnd();
        break;
    }
    return 0;
}

#else

INCLUDE_ASM(const s32, "newbattle/nbActionProcess", CheckAction_STKSEL);

#endif

INCLUDE_ASM(const s32, "newbattle/nbActionProcess", SetAction_READY);

INCLUDE_ASM(const s32, "newbattle/nbActionProcess", CheckAction_READY);

INCLUDE_ASM(const s32, "newbattle/nbActionProcess", SetAction_SKILL);

s32 CheckAction_SKILL(nbActionProcessData_t *a)
{
    SetNextActionSeq(a, 0x12);
    return 1;
}

void SetAction_NEGO(nbActionProcessData_t *a)
{
    s32 p = -1;
    s32 e = -1;
    s32 i = 0;
    nbFormation_t *form = a->data->form;

    do {
        u32 stat = form[i].stat;

        if ((stat & 1) != 0) {
            if ((stat & 8) != 0) {
                if ((stat & 0x100) != 0) {
                    p = i;
                    break;
                }
                if ((stat & 0x200) != 0) {
                    e = i;
                    break;
                }
            }
        }
        i++;
    } while (i < 0x10);
    if ((a->form->stat & 0x100) != 0) {
        p = a->form->formindex;
    } else if ((a->form->stat & 0x200) != 0) {
        e = a->form->formindex;
    }
    printf(D_00543FA8, p, e);
    nbPanelDraw(0);
    a->data->negopformindex = p;
    a->data->negoeformindex = e;
    nbInitNegoProcess(a, p, e, a->negotype, a->work->nowindex);
}

s32 CheckAction_NEGO(nbActionProcessData_t *a)
{
    if (a->data->NegoProcessID == NULL) {
        printf(D_00543FB8, a->data->negoresult);
        SetNextActionSeq(a, 10);
        nbPanelDraw(1);
        if (datCheckPlayerDead() != 0) {
            a->data->negoresult = 8;
        }
        return 1;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "newbattle/nbActionProcess", D_00543E20);

INCLUDE_RODATA(const s32, "newbattle/nbActionProcess", D_00543E38);

INCLUDE_RODATA(const s32, "newbattle/nbActionProcess", D_00543E58);

INCLUDE_RODATA(const s32, "newbattle/nbActionProcess", D_00543E78);

INCLUDE_RODATA(const s32, "newbattle/nbActionProcess", D_00543E98);

INCLUDE_RODATA(const s32, "newbattle/nbActionProcess", D_00543EB8);

INCLUDE_RODATA(const s32, "newbattle/nbActionProcess", D_00543EE8);

INCLUDE_RODATA(const s32, "newbattle/nbActionProcess", jtbl_00543F00);

INCLUDE_RODATA(const s32, "newbattle/nbActionProcess", jtbl_00543F40);

INCLUDE_RODATA(const s32, "newbattle/nbActionProcess", jtbl_00543F80);

INCLUDE_RODATA(const s32, "newbattle/nbActionProcess", D_00543FA8);

INCLUDE_RODATA(const s32, "newbattle/nbActionProcess", D_00543FB8);

INCLUDE_ASM(const s32, "newbattle/nbActionProcess", SetAction_NEGORESULT);

s32 CheckAction_NEGORESULT(nbActionProcessData_t *a)
{
    SetNextActionSeq(a, 0x12);
    return 1;
}

void SetAction_WAIT(nbActionProcessData_t *a)
{
    switch (a->work->nowcommand) {
    case 10:
        SetDamagePacket(a, a->work->maxhp * 15 / 100);
        nbMakePressPacket(0, a, -1.0f);
        nbMakeNewPressPacket(0, a, 0, a->form);
        return;
    case 8:
        nbMakePressPacket(0, a, -1.0f);
        nbMakeNewPressPacket(0, a, 0, a->form);
        SetTimeList(a, a->form->formindex, 0, 5, 5);
        return;
    case 3:
        nbMakePressPacket(0, a, -0.5f);
        nbMakeNewPressPacket(0, a, 1, a->form);
        SetTimeList(a, a->form->formindex, 0, 5, 5);
        break;
    case 9:
        break;
    }
}

s32 CheckAction_WAIT(nbActionProcessData_t *a)
{
    SetNextActionSeq(a, 0x12);
    return 1;
}

void SetAction_ESCAPE(nbActionProcessData_t *a)
{
    if (a->data->esccnt < 255) {
        a->data->esccnt++;
    }
    nbMakePressPacket(0, a, -10.0f);
    nbMakeNewPressPacket(0, a, 8, a->form);
    if (nbCheckEscape(a->data) != 0) {
        a->data->endtype = 3;
        return;
    }
    CheckKoketaNego(a);
}

/* NON_MATCHING: 9 of 57 words differ (first at +0x1C). */

#ifdef NON_MATCHING

s32 CheckAction_ESCAPE(nbActionProcessData_t *a)
{
    switch ((u16)((u16)a->seqtime / 15)) {
    case 1:
        if (a->data->endtype == 3) {
            goto out;
        }
        a->packettime = 0;
        break;
    case 2:
        if (a->data->endtype != 3) {
            goto out;
        }
        a->packettime = 0x1E;
        dds3FadeOut(0xFF, 0xFF, 0xFF, 0x14);
        nbAddSePacket(0, a, 0x10000);
        SetTimeList(a, a->form->formindex, 0, 0x11, 0x11);
        break;
    default:
        goto out;
    }
    SetNextActionSeq(a, 0x12);
    return 1;
out:
    return 0;
}

#else

INCLUDE_ASM(const s32, "newbattle/nbActionProcess", CheckAction_ESCAPE);

#endif

void SetAction_RETURN(nbActionProcessData_t *a)
{
    printf(D_00544020);
    if ((a->data->stat & 8) != 0) {
        nbCameraSetState(0x15);
    }
    nbMakeReturnPacket(0, a, a->party);
    SetTimeList(a, a->form->formindex, 0, 40, 40);
    nbMakePressPacket(10, a, -0.5f);
    nbMakeNewPressPacket(10, a, 9, a->form);
}

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

INCLUDE_RODATA(const s32, "newbattle/nbActionProcess", D_00544020);

INCLUDE_ASM(const s32, "newbattle/nbActionProcess", SetAction_CHECK1);

INCLUDE_ASM(const s32, "newbattle/nbActionProcess", CheckAction_CHECK1);

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
    if (a->hEffBTL != NULL) {
        effBTLDelete(a->hEffBTL);
        a->hEffBTL = NULL;
    }
    SetNextActionSeq(a, 0x16);
    return 1;
}

void SetAction_END(nbActionProcessData_t *a)
{
    if ((a->stat & 0x10000) != 0) {
        printf(D_00544070);
    }
    if ((a->stat & 0x20000) != 0) {
        printf(D_00544088);
    }
    if ((a->stat & 0x40000) != 0) {
        printf(D_005440A0);
    }
    if ((a->stat & 0x80000) != 0) {
        printf(D_005440B0);
    }
    if ((a->stat & 0x100000) != 0) {
        printf(D_005440C0);
    }
    if ((a->stat & 0x200000) != 0) {
        printf(D_005440D8);
    }
}

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
    s32 i = data->NowActionProcess - 1;
    dds3ProcessID_t *id;

    if (i < 0) {
        i = 3;
    }
    id = data->ActionProcessID[i];
    if (id == NULL) {
        return -1;
    }
    return nbGetActionSeq(id);
}

void SetActionSeq(nbActionProcessData_t *a, s32 seq)
{
    a->seq = seq;
    a->nextseq = 0;
    a->seqtime = 0;
    a->seqcnt = 0;
    ActionFuncTable[(s8)seq].set(a);
}

void SetNextActionSeq(nbActionProcessData_t *a, s32 nextseq)
{
    a->nextseq = nextseq;
}

INCLUDE_RODATA(const s32, "newbattle/nbActionProcess", D_00544070);

INCLUDE_RODATA(const s32, "newbattle/nbActionProcess", D_00544088);

INCLUDE_RODATA(const s32, "newbattle/nbActionProcess", D_005440A0);

INCLUDE_RODATA(const s32, "newbattle/nbActionProcess", D_005440B0);

INCLUDE_RODATA(const s32, "newbattle/nbActionProcess", D_005440C0);

INCLUDE_RODATA(const s32, "newbattle/nbActionProcess", D_005440D8);

INCLUDE_RODATA(const s32, "newbattle/nbActionProcess", D_005440E8);

void *nbActionProcess1(dds3ProcessID_t *id)
{
    nbActionProcessData_t *a = dds3GetProcessData(id);
    s32 ok;
    u32 prev;
    s32 seq;
    s32 t;

    if ((a->stat & 0x400) != 0) {
        nbPanelStockSelect(a->data, 0);
    }
    ok = 1;
    prev = nbGetPrevActionSeq();
    if ((a->stat & 2) != 0) {
        seq = a->nextseq;
    } else {
        seq = a->seq;
    }
    switch (seq) {
    case 7:
        t = prev < 0x13;
        ok = t == 0;
        if (p2sdGetPcmState() != 0) {
            printf(D_005440E8);
            p2sdSyncSound();
        }
        break;
    case 8:
        seq = 0;
        break;
    case 9:
        seq = 0;
        break;
    case 10:
        seq = 0;
        break;
    case 11:
        seq = 0;
        break;
    case 12:
        seq = 0;
        break;
    case 13:
        seq = 0;
        break;
    case 14:
        seq = 0;
        break;
    }
    if ((a->stat & 0x40) != 0) {
        return 0;
    }
    if (ok != 0) {
        if ((a->stat & 2) != 0) {
            if (a->seq == 0x16) {
                return (void *)-1;
            }
            SetActionSeq(a, a->nextseq);
            a->stat &= ~2;
            return 0;
        }
        if (ActionFuncTable[a->seq].check(a) != 0) {
            a->stat |= 2;
        }
        if ((u16)a->seqtime <= 0xFFFE) {
            a->seqtime = a->seqtime + 1;
        }
    }
    return 0;
}

void nbActionShutdown(dds3ProcessID_t *id)
{
    nbActionProcessData_t *a = dds3GetProcessData(id);
    dds3ProcessID_t **p = a->data->ActionProcessID;
    s32 i;

    for (i = 0; i < 4; i++) {
        if (*p == id) {
            *p = NULL;
        }
        p++;
    }
    if (a->hEffBTL != NULL) {
        effBTLDelete(a->hEffBTL);
    }
    sdfChipFreeDirect(dds3GetProcessData(id));
}

nbActionProcessData_t *InitActionProcessData(nbMainProcessData_t *data, s32 type, s32 from,
                                            s32 to, s32 d)
{
    nbActionProcessData_t *a = sdfChipAllocDirect(0x1A4);
    struct nbParty_t *party;

    memset(a, 0, 0x1A4);
    a->seq = 0;
    a->data = data;
    party = &data->party[from];
    a->party = party;
    if ((party->flag & 8) != 0) {
        a->form = NULL;
    } else {
        a->form = &data->form[party->formindex];
    }
    a->work = nbGetUnitWorkPointer(data, a->party);
    a->partyindex = from;
    a->type = type;
    a->target = to;
    a->adata = d;
    a->press = -1.0f;
    a->newpresstype = 0;
    ClearTimeList(a);
    return a;
}

void nbInitActionProcess(nbMainProcessData_t *data, s32 type, s32 from, s32 to,
                         s32 d)
{
    nbActionProcessData_t *a;
    dds3ProcessID_t *id;

    data->NowActionProcess = (data->NowActionProcess + 1) % 4;
    a = InitActionProcessData(data, type, from, to, d);
    id = dds3StartProcess(D_00544120, 0x3F8, 1, 1, nbActionProcess1, nbActionShutdown, a);
    dds3LinkProcess(data->MainProcessID, id);
    data->ActionProcessID[data->NowActionProcess] = id;
}

INCLUDE_RODATA(const s32, "newbattle/nbActionProcess", D_00544120);

INCLUDE_SDATA(const s32, "newbattle/nbActionProcess", StkSelId);

INCLUDE_SDATA(const s32, "newbattle/nbActionProcess", D_00551124);

INCLUDE_SDATA(const s32, "newbattle/nbActionProcess", D_00551128);

