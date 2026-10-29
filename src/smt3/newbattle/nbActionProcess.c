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
    /* Asynchronous requests returned by fileLoad; the sound id is bank << 16. */
    struct fileRequest *bedfile;
    struct fileRequest *midifile;
    u32 unk80;
    u32 midino;
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
    s16 statindex;
    u16 partyindex;
    s16 count[20];
    u32 aisyo[16];
    u16 hojoskill[8];
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
    u8 unk10;
    u8 binjotype;
    u16 mukantype;
    u8 unk14[0x1C];
} datDevilNegoFormat_t;

extern datDevilNegoFormat_t master_text[];

/* Persistent unit work begins four bytes into the save record array. */
typedef struct dds3LiveBattleUnitsView_t {
    u8 unk00[0xA54];
    datUnitWork_t units[16];
} dds3LiveBattleUnitsView_t;

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

extern struct nbKoukaPacket_t *nbAddSePacket(s32 startframe, s32 uniqueid, s32 se);

extern void nbPanelStockEnd(void);

extern void *memset(void *a, s32 b, u32 c);

extern void dds3FadeOut(u32 r, u32 g, u32 b, u32 frame);

extern void *dds3GetProcessData(dds3ProcessID_t *id);

extern void SetTarget_nbActionProcess(nbActionProcessData_t *a);

extern s32 AddTimeListHp(nbActionProcessData_t *a, s32 formindex, s32 add);

extern s32 nbMakeDamagePacket(s32 startframe, s32 uniqueid, nbFormation_t *form,
                              s32 type, s32 hp, s32 mp, s32 badstat, s32 hiteff, s32 dead,
                              s32 deadtype, s32 moji, s32 animeoff);

extern struct nbKoukaPacket_t *nbAddSkillNamePacket(s32 startframe, s32 uniqueid,
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

extern s32 nbMakePressPacket(s32 startframe, s32 uniqueid, f32 press);

extern s32 nbMakeNewPressPacket(s32 startframe, s32 uniqueid, s32 ptype,
                                nbFormation_t *form);

extern struct nbKoukaPacket_t *nbAddMojiEffKoukaPacket(s32 startframe, s32 frame,
                                                       s32 uniqueid,
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

extern void nbGetExpMakaItem(datUnitWork_t *w);

extern s32 nbMakeDeadPacket(s32 startframe, nbActionProcessData_t *uniqueid, nbFormation_t *form,
                            s32 deadchk);

extern void sdfChipFreeDirect(void *aPtr);

extern u32 dds3GetRandIntA(void *state, u32 limit);
extern s32 datSearchDevilStock(s32 id);
extern s32 nbSetHojoAddCounter(s32 formindex, s32 type, s32 point, s32 nvirtual);

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
    nbAddMojiEffKoukaPacket(frame, 0x2D, (s32)a, &a->data->form[dformindex], 3);
    SetTimeList(a, dformindex, frame, 10, 20);
    SetPress(a, -2.0f);
    SetNewPress(a, 4);
}

void SetOngShadowPacket(nbActionProcessData_t *a, s32 nskill, s32 sformindex, s32 dformindex,
                        s32 cnt, s32 frame)
{
    s32 i;

    nbAddMojiEffKoukaPacket(frame, 0x2D, (s32)a, &a->data->form[dformindex], 4);
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
        a->data->TarSelType = 1;
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
            a->data->TarSelType = 1;
        } else {
            a->data->TarSelType = 0;
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

#ifdef NON_MATCHING
extern s32 datCheckSyojiSkill(datUnitWork_t *work, s32 skill);
extern char D_005438C0[];

s32 CheckCounter(nbActionProcessData_t *a, s32 from, s32 to)
{
    s32 skills[] = { 0x133, 0x132, 0x131, -1 };
    s32 counters[] = { 0xA6, 0xA5, 0xA4 };
    datUnitWork_t *work;
    s32 badstatus;
    s32 counter = 0;
    s32 i;
    s32 rate;

    if (CheckTimeListDead(a, to) != 0) {
        return 0;
    }
    work = nbGetUnitWorkFromFormindex(from);
    badstatus = work->badstatus & 0xFFF;
    if (badstatus == 1 || badstatus == 2 || badstatus == 4 || badstatus == 0x10 ||
        badstatus == 0x80 || badstatus == 0x100 || badstatus == 0x400) {
        return 0;
    }
    for (i = 0; skills[i] >= 0; i++) {
        if (datCheckSyojiSkill(work, skills[i]) != 0) {
            rate = 100 / datSkillTable.special[skills[i] - 0x120].n;
            if ((s32)dds3GetRandIntA(NULL, 100) < rate) {
                counter = counters[i];
                break;
            }
        }
    }
    if (counter != 0) {
        nbPushAction(4, a->data->form[from].partyindex, a->data->form[to].partyindex, counter);
        printf(D_005438C0);
        return 1;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbActionProcess", CheckCounter);
#endif

s32 CheckMukanNego(nbActionProcessData_t *a)
{
    s32 assistCount = 0;
    s32 i;
    s32 rate;

    if (((u32)(u16)a->data->StageMajor - 0xF0u) < 7u) return 0;
    if ((a->data->stat & 1) != 0) return 0;
    if ((a->party->flag & 2) == 0) return 0;
    if (master_text[a->work->id].mukantype == 0) return 0;
    if (datSearchDevilStock(a->work->id) >= 0) return 0;
    if (evtGetAgeOfMoon() == 8) return 0;
    if (datCheckTalk(a->work) == 0) return 0;
    if (datCheckTalk(((dds3LiveBattleUnitsView_t *)DDS3_GBWK)->units) == 0) return 0;
    if (a->data->enemypcnt < 2) return 0;
    if (nbGetHojoCounter(a->form->formindex, 0x10) != 0) return 0;
    if ((a->data->stat & 0x2000000) == 0 && a->data->turn < 3) return 0;

    for (i = 4; i < 14; i++) {
        if ((a->data->form[i].stat & 1) != 0) {
            if ((a->data->form[i].stat & 0x40) != 0) {
                if (nbGetHojoCounter(i, 0x10) == 0) {
                    assistCount++;
                }
            }
        }
    }
    if (assistCount != 1) return 0;

    rate = (a->data->stat & 0x2000000) == 0 ? 5 : 100;
    if ((s32)dds3GetRandIntA(NULL, 100) >= rate) return 0;
    nbSetHojoAddCounter(a->form->formindex, 0x10, 1, 0);
    return 1;
}

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
    if ((s32)dds3GetRandIntA(NULL, 0x64) < rate) {
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

#ifdef NON_MATCHING
s32 CheckBinjoNego(nbActionProcessData_t *a)
{
    nbFormation_t *form;
    datUnitWork_t *work;
    struct nbParty_t *party;
    s32 i;
    s32 id;
    s32 rate;

    if ((u32)((u16)a->data->StageMajor - 0xF0) < 7 || (a->data->stat & 1) != 0 ||
        (a->party->flag & 6) != 0 || a->partyindex >= 4 || (a->stat & 8) == 0) {
        return 0;
    }
    if (evtGetAgeOfMoon() == 8 || datCheckTalk(a->work) == 0) {
        return 0;
    }
    for (i = 4; i < 14; i++) {
        form = &a->data->form[i];
        if ((form->stat & 0x49) != 0x41) {
            continue;
        }
        party = &a->data->party[form->partyindex];
        if ((party->flag & 1) == 0 || (party->flag & 0x60) != 0) {
            continue;
        }
        work = nbGetUnitWorkFromFormindex(i);
        if (CheckTimeListDead(a, i) != 0) {
            continue;
        }
        id = work->id;
        if (master_text[id].binjotype == 0) {
            continue;
        }
        if ((a->work->flag & 4) == 0 && a->work->id == id) {
            continue;
        }
        if (datSearchDevilStock(id) >= 0 || datCheckTalk(work) == 0) {
            continue;
        }
        rate = (a->data->stat & 0x2000000) != 0 ? 100 : 2;
        if ((s32)dds3GetRandIntA(NULL, 100) < rate) {
            a->data->party[form->partyindex].flag |= 0x40;
            nbPushAction(2, form->partyindex, a->partyindex, 0);
            printf("binjo party = %d\n", form->partyindex);
            return 1;
        }
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbActionProcess", CheckBinjoNego);
#endif

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

#ifdef NON_MATCHING
s32 CheckKoketaNego(nbActionProcessData_t *a)
{
    s8 partyList[32];
    nbFormation_t *form;
    datUnitWork_t *work;
    s32 count = 0;
    s32 i;
    s32 status;

    if ((a->party->flag & 6) != 0 || a->partyindex >= 4) {
        return 0;
    }
    for (i = 4; i < 14; i++) {
        form = &a->data->form[i];
        if ((form->stat & 0x41) != 0x41 ||
            (a->data->party[form->partyindex].flag & 1) == 0 ||
            (a->data->party[form->partyindex].flag & 0x20) != 0) {
            continue;
        }
        work = nbGetUnitWorkFromFormindex(i);
        if (CheckTimeListDead(a, i) != 0) {
            continue;
        }
        status = work->badstatus & 0xFFF;
        if (status != 1 && status != 2 && status != 4 && status != 0x10 &&
            status != 0x100 && status != 0x400) {
            partyList[count++] = form->partyindex;
        }
    }
    if (count == 0) {
        return 0;
    }
    nbPushAction(3, partyList[dds3GetRandIntA(NULL, count)], a->partyindex, 0);
    printf("escape err party\n");
    return 1;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbActionProcess", CheckKoketaNego);
#endif

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
                nbAddSkillNamePacket(start, (s32)a, 0x159);
                nbSetKuisibariStop(a->form->formindex);
            }
        }
    } else if (nbCheckKuisibari2Skill(a->form->formindex) != 0) {
        if (hp > 0) {
            if (hp >= GetTimeListHp(a, a->form->formindex)) {
                hp = GetTimeListHp(a, a->form->formindex) - 1;
                nbAddSkillNamePacket(start, (s32)a, 0x168);
                nbSetKuisibariStop(a->form->formindex);
            }
        }
    }
    AddTimeListHp(a, a->form->formindex, -hp);
    dead = CheckTimeListDead(a, a->form->formindex) != 0;
    nbMakeDamagePacket(start, (s32)a, a->form, 3, hp, -1, 0, 0, dead, 0, 1, 0);
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

extern s32 datGetItemSkillNo(s32 itemid);
extern u32 nbGetBadStat(s32 formindex);
extern u32 dds3DebugSign;
extern s32 CheckSenseiNego(nbActionProcessData_t *a);

s32 CheckAction_NON(nbActionProcessData_t *a)
{
    u32 badStat;
    u16 partyFlag;

    a->data->negoresult = 0;
    a->data->negopformindex = 0;
    a->data->negoeformindex = 0;
    a->data->stat &= ~0x4000u;
    a->massuruid = datGetItemSkillNo(1);
    a->form->motendtype = 0;
    a->party->flag &= ~0x100u;

    if ((a->party->flag & 8) != 0) {
        SetNextActionSeq(a, 0xF);
        return 1;
    }

    switch (a->type) {
    case 1:
        a->work->nowcommand = 4;
        a->work->nowindex = 0x182;
        a->negotype = 4;
        SetTargetParty(a, a->target);
        a->select = nbGetSelectBit(a->data);
        SetNextActionSeq(a, 7);
        return 1;
    case 2:
        a->work->nowcommand = 4;
        a->work->nowindex = a->data->prevcommind;
        a->negotype = 3;
        SetTargetParty(a, a->target);
        a->select = nbGetSelectBit(a->data);
        SetNextActionSeq(a, 7);
        return 1;
    case 3:
        a->work->nowcommand = 4;
        a->work->nowindex = 0x182;
        a->negotype = 5;
        SetTargetParty(a, a->target);
        a->select = nbGetSelectBit(a->data);
        SetNextActionSeq(a, 7);
        return 1;
    case 4:
        a->work->nowcommand = 1;
        a->work->nowindex = a->adata;
        SetTargetParty(a, a->target);
        a->select = nbGetSelectBit(a->data);
        a->stat |= 4;
        SetNextActionSeq(a, 7);
        return 1;
    case 5:
        SetNextActionSeq(a, 0x13);
        return 1;
    default:
        nbSetHojoAddCounter(a->form->formindex, 17, 1, 0);
        nbSetHojoAddCounter(a->form->formindex, 18, 1, 0);
        badStat = nbGetBadStat(a->form->formindex);
        switch (badStat) {
        case 1:
        case 2:
        case 4:
        case 0x10:
        case 0x80:
        case 0x400:
            SetNextActionSeq(a, 4);
            return 1;
        }

        partyFlag = a->party->flag;
        if ((partyFlag & 2) != 0) {
            if (CheckMukanNego(a) != 0) {
                a->work->nowcommand = 4;
                a->work->nowindex = 0x182;
                a->negotype = 1;
                SetTargetPlayer(a);
                a->select = nbGetSelectBit(a->data);
                SetNextActionSeq(a, 7);
                return 1;
            }
            if (CheckSenseiNego(a) != 0) {
                a->work->nowcommand = 4;
                a->work->nowindex = 0x182;
                a->negotype = 2;
                SetTargetPlayer(a);
                a->select = nbGetSelectBit(a->data);
                SetNextActionSeq(a, 7);
                return 1;
            }
            if (dds3DebugSign == 1 && (a->data->stat & 0x10000000) != 0) {
                SetNextActionSeq(a, 1);
            } else if (D_003989E0[a->work->id].aiscript != 0) {
                SetNextActionSeq(a, 3);
            } else {
                SetNextActionSeq(a, 2);
            }
            return 1;
        }
        if ((partyFlag & 4) != 0) {
            SetNextActionSeq(a, 0x15);
            return 1;
        }
        SetNextActionSeq(a, 1);
        return 1;
    }
}

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

extern s32 nbMakeBadKaifukuPacket(s32 frame, s32 id, nbFormation_t *form);
extern s32 nbMakeKaifukuPacket(s32 frame, s32 id,
    nbFormation_t *form, s32 type, s32 hp, s32 mp, s32 flag);
extern s32 nbApplyBadCurse(nbActionProcessData_t *a);
extern s32 nbCheckBadCurse(s32 formindex);

s32 CheckAction_BAD(nbActionProcessData_t *a)
{
    ClearTimeList(a);
    SetHpForTimeList(a);
    a->jyokyomes = 0;
    switch (nbGetBadStat(a->form->formindex)) {
    case 1:
    case 2:
        nbMakePressPacket(0, (s32)a, -1.0f);
        nbMakeNewPressPacket(0, (s32)a, 0, a->form);
        nbMakeBadKaifukuPacket(5, (s32)a, a->form);
        if ((a->data->stat & 8) != 0) {
            nbCameraSetState(0x19);
        }
        SetTimeList(a, a->form->formindex, 0, 30, 30);
        SetNextActionSeq(a, 0x12);
        break;
    case 8:
    case 0x80:
        nbApplyBadCurse(a);
        SetNextActionSeq(a, 0xB);
        break;
    case 4:
        nbMakePressPacket(0, (s32)a, -1.0f);
        nbMakeNewPressPacket(0, (s32)a, 0, a->form);
        nbMakeKaifukuPacket(10, (s32)a, a->form, 0, a->work->maxhp >> 3,
                            a->work->maxmp >> 3, 0);
        if ((a->data->stat & 8) != 0) {
            nbCameraSetState(0x19);
        }
        SetTimeList(a, a->form->formindex, 0, 30, 30);
        SetNextActionSeq(a, 0x12);
        break;
    case 0x10:
    case 0x400:
        nbMakePressPacket(0, (s32)a, -1.0f);
        nbMakeNewPressPacket(0, (s32)a, 0, a->form);
        if ((a->data->stat & 8) != 0) {
            nbCameraSetState(0x19);
        }
        SetTimeList(a, a->form->formindex, 0, 30, 30);
        SetNextActionSeq(a, 0x12);
        break;
    default:
        if (nbCheckBadCurse(a->form->formindex) != 0) {
            nbApplyBadCurse(a);
            SetNextActionSeq(a, 0xB);
        }
        break;
    }
    return 1;
}

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

/* Native item rows, as consumed by the command and inventory providers. */
typedef struct {
    u8 use;
    u8 flag;
    u16 skill;
    u32 price;
} nbActionItem_t;
extern nbActionItem_t datItemTable[];

/* Motion records begin at +0x2C in the 0x310-byte model-visual row. */
typedef struct {
    s16 movetype;
    u16 actframe;
    s32 se_no;
    f32 motionsp;
    f32 atarisize;
    f32 movey;
    f32 movez;
    s16 bframe;
    s16 hframe;
} nbActionMotion_t;

extern void nbRestoreBgm(void);
extern datUnitVisual_t *nbGetUnitVisual(datUnitWork_t *work);
extern s32 dds3GetGlobalConfigFlag(s32 flag);
extern s32 nbCheckBadPanicAction(datUnitWork_t *work);
extern s32 nbCheckSkillUse(s32 formindex, s32 nskill);
extern s32 nbGetItemSkillNo(nbActionProcessData_t *a, s32 itemid);
extern void datAddItem(s32 id, s32 num);
extern char *effMakeBedFileName(u32 id);
extern struct fileRequest *fileLoad(const char *filename);
extern s32 sprintf(char *str, const char *format, ...);
extern char D_00575360[];
extern void nbDelAllMojiNumEffectKoukaPacket(nbFormation_t *form);
extern struct nbKoukaPacket_t *nbAddFormFlagOnOffPacket(s32 startframe,
    s32 uniqueid, nbFormation_t *form, s32 sw, u32 flag);
extern s32 nbCalcUnitVisual(nbFormation_t *form, s32 part);
extern s32 nbCalcUnitVisual2(nbFormation_t *form, s32 part);
extern s32 nbGetMotionNo(nbFormation_t *form, s32 motion);
extern struct nbKoukaPacket_t *nbAddSkillMotionKoukaPacket(s32 startframe,
    s32 uniqueid, nbFormation_t *form, s32 motion, s32 endtype, f32 rate);
extern f32 nbGetMotionSpeed(nbFormation_t *form, s32 motion, f32 rate);
extern s32 nbGetMotionActframe(nbFormation_t *form, s32 motion, f32 rate);
extern s32 nbMemoryLoadMotionSe(nbFormation_t *form, s32 motion);
extern s32 datGetSkillCost(datUnitWork_t *work, s32 nskill);
extern struct nbKoukaPacket_t *nbAddHpMpKoukaPacket(s32 startframe,
    s32 uniqueid, nbFormation_t *form, s32 n, s32 type);
extern struct nbKoukaPacket_t *nbAddHatudoEffectKoukaPacket(s32 startframe, s32 framelength,
    s32 uniqueid, nbFormation_t *form, s32 type, s32 mottype, s32 skillid);
extern struct nbKoukaPacket_t *nbAddHitEffKoukaPacket(s32 startframe,
    s32 uniqueid, nbFormation_t *form, s32 type);
extern void nbSetNormalSkill0(datUnitWork_t *work);
extern void effBTLSkillEnd(void);
extern void effBTLSkillSetFormationInfo(s32 invokeId, u32 targetBit);
extern void effBTLSkillStart(u32 id, u32 stat);
extern s32 nbMakeUnitMovePacket(s32 startframe, s32 uniqueid,
    nbFormation_t *form, u32 targetMask, s32 type, f32 speed);
extern void cmpSetSkillListRows(s32 rows);
extern s32 nbHatudoCheckSkill(datUnitWork_t *work, s32 nskill);
extern void nbDispSkillName(s32 skill);
extern void nbDispActionText(u32 command);
extern void nbDispTextById(s32 id);
void SetActionSeq(nbActionProcessData_t *a, s32 seq);

void SetAction_READY(nbActionProcessData_t *a)
{
    s32 motionType = -1;
    s32 skill = 0;
    s32 moveType = 0;
    datUnitVisual_t *visual;
    f32 motionRate;
    f32 moveRate;
    s32 motion;
    /* Live HP-cost delay, then independently reset formation indices.
     * Qualified allocation evidence: docs/levers/handle-registers.md. */
    s32 i;
    nbFormation_t *form;
    datNormalSkill_t *skillData;
    s32 cost;
    s32 hp;
    s32 midi;
    u32 stat;

    nbRestoreBgm();
    a->press = 0.0f;
    a->form->motenddata = a->form->motendtype = 0;
    a->runerr = 0;
    a->jyokyomes = 0;
    a->skilltime = 0;
    a->autoskill = 0;
    a->allhpdamage = 0;
    a->allmpdamage = 0;
    ClearTimeList(a);
    SetHpForTimeList(a);
    visual = nbGetUnitVisual(a->work);
    if (dds3GetGlobalConfigFlag(2) == 0) {
        if ((a->stat & 0x80) != 0 && a->work->nowcommand == 0) {
            a->stat |= 0x100;
        }
    }
    if ((a->stat & 0x100) != 0) {
        motionRate = 3.0f;
        moveRate = 1.5f;
        if (a->work->id == 0xC0) {
            motionRate = 1.0f;
        }
    } else {
        motionRate = 1.0f;
        moveRate = 1.0f;
    }
    if ((a->stat & 4) == 0) {
        a->stat |= 4;
        if (nbGetBadStat(a->form->formindex) == 8 && a->type != 3 &&
            nbCheckBadPanicAction(a->work) != 0) {
            SetActionSeq(a, 4);
            return;
        }
        if (nbCheckBadCurse(a->form->formindex) != 0 && dds3GetRandIntA(NULL, 4) == 0) {
            SetActionSeq(a, 4);
            return;
        }
    }
    switch (a->work->nowcommand) {
    case -1:
        SetActionSeq(a, 0x12);
        return;
    case 6:
        skill = 0xFF;
        break;
    case 5:
        skill = nbGetItemSkillNo(a, a->work->nowindex);
        break;
    case 1:
        skill = a->work->nowindex;
        switch (nbCheckSkillUse(a->form->formindex, skill)) {
        case 1:
            a->runerr = 1;
            nbSetJyokyoMes(a, 0x3C, -1, 0);
            break;
        case 2:
            a->runerr = 1;
            nbSetJyokyoMes(a, 0xE, -1, 0);
            break;
        case 3:
            a->runerr = 1;
            nbSetJyokyoMes(a, 0xC, -1, 0);
            break;
        }
        break;
    case 0: break;
    case 2: break;
    case 3: break;
    case 4: break;
    case 7: break;
    case 8: break;
    case 9: break;
    case 10: break;
    }
    switch (a->work->nowcommand) {
    case 0: case 1: case 4: case 5: case 6:
        if (a->select == 0) {
            a->runerr = 3;
            nbSetJyokyoMes(a, 0x3E, -1, 0);
        }
        break;
    }
    if (a->work->nowcommand == 5) {
        if ((datItemTable[a->work->nowindex].flag & 4) != 0) {
            datAddItem(a->work->nowindex, -1);
        }
    }
    a->stat &= ~1;
    switch (a->work->nowcommand) {
    case 0: break;
    case 1: case 5: case 6:
        a->bedfile = NULL;
        if (datSkillTable.visual[skill].bedno != 0) {
            printf("name [%d]:%s\n", skill, effMakeBedFileName(skill));
            a->bedfile = fileLoad(effMakeBedFileName(skill));
        }
        a->midifile = NULL;
        midi = skill + 0x200;
        sprintf(D_00575360, "/sound/MIDI%04X.SMG", midi);
        printf("MIDI :%s\n", D_00575360);
        a->midifile = fileLoad(D_00575360);
        a->midino = midi << 16;
        break;
    case 2: break;
    case 3: break;
    case 4: break;
    case 7: break;
    case 8: break;
    case 9: break;
    case 10: break;
    }
    switch (a->work->nowcommand) {
    case 0: motionType = 3; break;
    case 5: motionType = 0x19; break;
    case 1: motionType = datSkillTable.visual[a->work->nowindex].motion; break;
    case 6: motionType = 0x18; break;
    case 2: motionType = -1; break;
    case 3: motionType = -1; break;
    case 4: motionType = -1; break;
    case 7: motionType = -1; break;
    case 8: motionType = -1; break;
    case 9: motionType = -1; break;
    case 10: motionType = -1; break;
    }
    motion = -1;
    if (motionType >= 0) {
        nbDelAllMojiNumEffectKoukaPacket(a->form);
        nbAddFormFlagOnOffPacket(0, (s32)a, a->form, 0, 0x2000);
        if ((a->form->stat & 0x4000) != 0) {
            a->form->stat &= ~0x4000;
        }
        a->form->motendtype = nbCalcUnitVisual(a->form, motionType);
        a->form->motenddata = nbCalcUnitVisual2(a->form, motionType);
        motion = nbGetMotionNo(a->form, motionType);
        nbAddSkillMotionKoukaPacket(0, (s32)a, a->form, motionType, a->form->motendtype, motionRate);
        if (a->form->motendtype == 2 && a->form->motenddata >= 0) {
            f32 speed = nbGetMotionSpeed(a->form, motionType, motionRate);
            nbMakeUnitFadeOutPacket(a->form->motenddata / speed, a, a->form, 0, 2, 4, 4);
        }
    }
    a->form->motcate = motionType;
    a->motion = motion;
    if ((a->stat & 0x100) == 0) {
        a->motse = nbMemoryLoadMotionSe(a->form, motion);
    }
    switch (a->work->nowcommand) {
    case 0: case 1: case 5: case 6:
        a->timer = nbGetMotionActframe(a->form, motionType, motionRate);
        moveType = ((nbActionMotion_t *)((u8 *)visual + 0x2C))[motionType].movetype;
        if (a->work->nowcommand == 1 && skill == 0x73) {
            moveType = 2;
        }
        break;
    case 4:
        switch (a->negotype) {
        case 0: a->timer = 0; break;
        case 2:
            a->timer = 0x23;
            nbCameraSetState(0x15);
            nbAddMojiEffKoukaPacket(0, 0x1E, (s32)a, a->form, 0);
            break;
        case 1: case 3: case 4:
            a->timer = 0x23;
            nbCameraSetState(3);
            nbAddMojiEffKoukaPacket(0, 0x1E, (s32)a, a->form, 0);
            break;
        case 5: a->timer = 0; break;
        case 6: a->timer = 0; break;
        }
        break;
    default:
        a->timer = 0;
        moveType = 0;
        break;
    }
    printf("acttime = %d\n", a->timer);
    if (a->runerr == 0 && a->work->nowcommand == 1) {
        skillData = &datSkillTable.normal[skill];
        switch (skillData->costtype) {
        case 0: break;
        case 1:
            cost = datGetSkillCost(a->work, skill);
            if ((skillData->flag & 8) != 0) {
                i = 60;
                if (skill >= 0x73) {
                    i = skill < 0x75 ? 83 : 60;
                }
                hp = -0x7FFF;
                nbAddHpMpKoukaPacket(a->timer + i, (s32)a, a->form, hp, 0);
            } else {
                hp = -cost;
                nbAddHpMpKoukaPacket(1, (s32)a, a->form, hp, 0);
            }
            AddTimeListHp(a, a->form->formindex, hp);
            break;
        case 2:
            nbAddHpMpKoukaPacket(1, (s32)a, a->form, -datGetSkillCost(a->work, skill), 1);
            break;
        }
    }
    switch (a->work->nowcommand) {
    case 0: break;
    case 1: case 5: case 6:
        nbAddHatudoEffectKoukaPacket(0, a->timer, (s32)a, a->form,
            datSkillTable.visual[skill].hatudo, a->form->motcate, skill);
        break;
    case 2: break;
    case 3: break;
    case 4: break;
    case 7:
        nbAddHitEffKoukaPacket(0, (s32)a, a->form, 5);
        break;
    }
    switch (a->work->nowcommand) {
    case 0:
        nbSetNormalSkill0(a->work);
        nbDelAllKoukaPacketType(0xE);
        effBTLSkillEnd();
        effBTLSkillSetFormationInfo(a->party->formindex, (u16)a->select);
        effBTLSkillStart(0, 0);
        break;
    case 1: case 5: case 6:
        nbSetNormalSkill0(a->work);
        nbDelAllKoukaPacketType(0xE);
        effBTLSkillEnd();
        effBTLSkillSetFormationInfo(a->party->formindex, (u16)a->select);
        effBTLSkillStart(skill, 0);
        break;
    case 2: break;
    case 3: nbCameraSetState(3); break;
    case 4: break;
    case 7: break;
    case 8: break;
    case 9: break;
    case 10: break;
    }
    printf("select %d -> %08X\n", a->party->formindex, a->select);
    if (a->work->nowcommand != 4) {
        if (moveType == 2 && (a->work->nowcommand == 1 || a->work->nowcommand == 5)) {
            switch (datSkillTable.normal[skill].targettype) {
            case 0: moveType = 4; break;
            case 1: case 2: moveType = 5; break;
            }
        }
        nbMakeUnitMovePacket(1, (s32)a, a->form, (u16)a->select, moveType, moveRate);
    }
    for (i = 0; i < 16; i++) {
        if ((a->select & (1 << i)) != 0) {
            form = &a->data->form[i];
            switch (a->work->nowcommand) {
            case 4:
                nbMakeUnitMovePacket(0, (s32)a, form, 1 << a->form->formindex, 1, 1.0f);
                break;
            case 6:
                if ((form->stat & 0x41) == 0x41) {
                    nbMakeReturnPacket(0, a, &a->data->party[form->partyindex]);
                    nbAddHitEffKoukaPacket(0, (s32)a, form, 5);
                }
                break;
            }
        }
    }
    if (a->runerr == 0) {
        if ((a->party->flag & 2) != 0) {
            for (i = 0; i < 4; i++) {
                stat = a->data->form[i].stat;
                if ((stat & 1) != 0) {
                    if ((stat & 0x40) != 0) {
                        if ((stat & 8) != 0) {
                            nbSetHojoAddCounter(i, 0x10, 1, 0);
                        }
                    }
                }
            }
        } else {
            for (i = 4; i < 14; i++) {
                stat = a->data->form[i].stat;
                if ((stat & 1) != 0) {
                    if ((stat & 0x40) != 0) {
                        if ((stat & 8) != 0) {
                            nbSetHojoAddCounter(i, 0x10, 1, 0);
                        }
                    }
                }
            }
        }
    }
    nbPanelAllShow();
    nbPanelPressGaugeShow();
    if (a->work->nowcommand == 3 && (a->data->stat & 8) != 0) {
        nbCameraSetState(0x19);
    }
    cmpSetSkillListRows(20);
    switch (a->work->nowcommand) {
    case 0:
        if (nbHatudoCheckSkill(a->work, 0x130) != 0) {
            nbDispSkillName(0x130);
        } else {
            nbDispActionText(a->work->nowcommand);
        }
        break;
    case 2: case 3: case 6:
        nbDispActionText(a->work->nowcommand);
        break;
    case 7:
        if (nbCheckFormationPlayerSide(a->form->formindex) != 0) {
            if ((a->stat & 0x20) != 0) {
                nbSetJyokyoMes(a, 0x32, 1, a->work->id);
            } else {
                nbDispActionText(a->work->nowcommand);
            }
        } else if ((a->stat & 0x20) != 0) {
            nbSetJyokyoMes(a, 0x34, 1, a->work->id);
        } else {
            nbSetJyokyoMes(a, 0xA, 1, a->work->id);
        }
        break;
    case 5:
        nbDispTextById(a->work->nowindex);
        break;
    case 4:
        if (a->negotype == 0) {
            nbDispSkillName(a->work->nowindex);
        }
        break;
    case 1:
        if (a->work->id == 0x159 && a->work->nowindex == 0xE2) {
            nbDispSkillName(0x1F3);
        } else if (a->work->id == 0x15A && a->work->nowindex == 0xE2) {
            nbDispSkillName(0x1F0);
        } else if (a->work->id == 0x15B && a->work->nowindex == 0xE2) {
            nbDispSkillName(0x1F1);
        } else if (a->work->id == 0x15C && a->work->nowindex == 0xE2) {
            nbDispSkillName(0x1F2);
        } else {
            /* This display value is distinct from the special-skill tests. */
            s32 skillName = a->work->nowindex;

            if (skillName != 0xF7) {
                nbDispSkillName(skillName);
            }
        }
        break;
    case 8: break;
    case 9: break;
    case 10: break;
    }
    cmpSetSkillListRows(-1);
}

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

extern s32 nbGetNakama(datUnitWork_t *work);
extern void nbSetWaitMotion(nbFormation_t *form, s32 motion, f32 rate);
extern void nbSetUnitMotion(nbFormation_t *form, s32 motion, s32 mode, f32 rate);
extern void nbCameraSetTaizi(nbFormation_t *invoke, nbFormation_t *target);

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

void SetAction_NEGORESULT(nbActionProcessData_t *a)
{
    s32 pressPacket = 0;
    s32 pressType = 0;
    s32 setMotion = 0;
    s32 setCamera = 1;
    s32 i;
    s32 workId;
    s32 frame;
    s32 activeCount;
    u16 partyFlags;
    datUnitWork_t *work;
    nbFormation_t *form;
    struct nbParty_t *party;
    struct nbParty_t *scanParty;
    /* Fresh work reads share this live view; the final party scan has its own
     * cursor. Allocation/parity evidence: docs/levers/handle-registers.md. */
    nbMainProcessData_t *data;

    switch (a->data->negoresult) {
    case 7:
        setMotion = 1;
        break;
    case 1:
        pressPacket = 1;
        pressType = 12;
        a->press = -1.0f;
        setMotion = 1;
        break;
    case 2:
        pressPacket = 1;
        pressType = 12;
        party = nbGetPartyFromFormindex(a->data->negoeformindex);
        nbMakeReturnPacket(0, a, party);
        SetTimeList(a, a->data->negoeformindex, 0, 40, 40);
        a->press = -1.0f;
        break;
    case 3:
        pressPacket = 1;
        pressType = 11;
        party = nbGetPartyFromFormindex(a->data->negoeformindex);
        work = nbGetUnitWorkFromFormindex(a->data->negoeformindex);
        nbGetNakama(work);
        nbMakeReturnPacket(0, a, party);
        SetTimeList(a, a->data->negoeformindex, 0, 40, 40);
        a->press = -1.0f;
        break;
    case 4:
        pressPacket = 1;
        pressType = 13;
        a->press = -10.0f;
        setMotion = 1;
        break;
    case 5:
        i = 4;
        work = nbGetUnitWorkFromFormindex(a->data->negoeformindex);
        workId = work->id;
        for (; i < 14; i++) {
            data = a->data;
            partyFlags = data->party[i].flag;
            if ((partyFlags & 1) != 0) {
                if ((partyFlags & 2) != 0) {
                    if ((partyFlags & 0x20) == 0) {
                        if (nbGetUnitWorkPointer(data, &data->party[i])->id == workId) {
                            frame = dds3GetRandIntA(NULL, 20);
                            nbMakeReturnPacket(frame, a, &a->data->party[i]);
                            data = a->data;
                            if ((data->party[i].flag & 8) == 0) {
                                SetTimeList(a, data->party[i].formindex, frame, 40, 40);
                            }
                        }
                    }
                }
            }
        }
        pressPacket = 1;
        pressType = 12;
        a->press = -1.0f;
        break;
    case 6:
    case 9:
        if (a->data->negoresult == 9) {
            work = nbGetUnitWorkFromFormindex(a->data->negoeformindex);
            nbGetNakama(work);
        }
        for (i = 4; i < 14; i++) {
            data = a->data;
            partyFlags = data->party[i].flag;
            if ((partyFlags & 1) != 0) {
                if ((partyFlags & 2) != 0) {
                    if ((partyFlags & 0x20) == 0) {
                        frame = dds3GetRandIntA(NULL, 20);
                        nbMakeReturnPacket(frame, a, &a->data->party[i]);
                        data = a->data;
                        if ((data->party[i].flag & 8) == 0) {
                            SetTimeList(a, data->party[i].formindex, frame, 40, 40);
                        }
                    }
                }
            }
        }
        break;
    case 8:
        setMotion = 0;
        setCamera = 0;
        pressPacket = 0;
        break;
    }

    switch (a->negotype) {
    case 0:
    case 5:
    case 6:
        break;
    case 1:
    case 2:
        data = a->data;
        if (data->negoresult < 5) {
            if (data->negoresult < 2) {
                pressPacket = 0;
            }
        } else {
            pressPacket = 0;
        }
        break;
    case 3:
    case 4:
        data = a->data;
        if (data->negoresult != 4) {
            pressPacket = 0;
        }
        break;
    }
    if (pressPacket != 0) {
        nbMakePressPacket(0, (s32)a, a->press);
        nbMakeNewPressPacket(0, (s32)a, pressType, a->form);
    }

    if (setMotion != 0) {
        nbGetUnitWorkFromFormindex(a->data->negoeformindex);
        form = &a->data->form[a->data->negoeformindex];
        nbSetWaitMotion(form, 0, 1.0f);
        nbSetUnitMotion(form, 0, 1, 1.0f);
    }
    if (setCamera != 0 && a->negotype == 0 && (a->form->stat & 0x10) != 0) {
        nbCameraSetTaizi(a->form, &a->data->form[a->data->negoeformindex]);
    }

    activeCount = 0;
    data = a->data;
    scanParty = &data->party[4];
    for (i = 9; i >= 0; i--) {
        if ((scanParty->flag & 1) != 0 && (scanParty->flag & 0x20) == 0) {
            activeCount++;
        }
        scanParty++;
    }
    if (activeCount == 0) {
        data->endtype = 9;
    }
}

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
        nbMakePressPacket(0, (s32)a, -1.0f);
        nbMakeNewPressPacket(0, (s32)a, 0, a->form);
        return;
    case 8:
        nbMakePressPacket(0, (s32)a, -1.0f);
        nbMakeNewPressPacket(0, (s32)a, 0, a->form);
        SetTimeList(a, a->form->formindex, 0, 5, 5);
        return;
    case 3:
        nbMakePressPacket(0, (s32)a, -0.5f);
        nbMakeNewPressPacket(0, (s32)a, 1, a->form);
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
    nbMakePressPacket(0, (s32)a, -10.0f);
    nbMakeNewPressPacket(0, (s32)a, 8, a->form);
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
        nbAddSePacket(0, (s32)a, 0x10000);
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
    nbMakePressPacket(10, (s32)a, -0.5f);
    nbMakeNewPressPacket(10, (s32)a, 9, a->form);
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

