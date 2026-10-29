#include "common.h"
#include "newbattle.h"
#include "kernel.h"
#include "nb.h"
#include "ps2asm.h"
#include "model.h"
#include "event.h"
#include "motion.h"

extern char D_00543FB8[]; /* "nego result = %d\n" */

extern char D_00544020[];

extern s32 printf(const char *format, ...);


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
    sdfMemHandle_t *midibuf;
    u32 midino;
    nbTimeList_t timelist[32];
    u32 packettime;
    s32 hitstopframe;
    u8 unk190[0x10];
    u32 aisummonid;
} nbActionProcessData_t;

/* 128-byte party records. */


typedef struct nbActionFunc_t {
    void (*set)(nbActionProcessData_t *a);
    s32 (*check)(nbActionProcessData_t *a);
} nbActionFunc_t;

/* 164-byte rows indexed by datUnitWork_t::id. */
typedef struct nbDevilData_t {
    u16 aiscript;
    u8 unk02[0xA2];
} nbDevilData_t;

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

extern struct nbKoukaPacket_t *nbAddFormOnOffPacket(s32 startframe, s32 uniqueid,
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

extern s32 nbMakeDeadVisualPacket(s32 startframe, s32 uniqueid, nbFormation_t *form,
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

void SetAuroraPacket(nbActionProcessData_t *a, s32 formindex, s32 sframe)
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
    nbAddFormOnOffPacket(sframe + 12, (s32)a, form, 0);
    a->data->endtype = 3;
}

void SetAnalyzePacket(nbActionProcessData_t *a, s32 dformindex, s32 sframe)
{
    nbAddAnalyzePacket(sframe + 2, a, &a->data->form[dformindex]);
    nbPanelAnalyzeStart();
    a->stat |= 0x10;
}

#ifdef NON_MATCHING
extern s32 nbAppendEnemyDevilWork(nbMainProcessData_t *data, s32 id, s32 formindex);
extern s32 nbGetRandamStock(void);
#endif

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

#ifdef NON_MATCHING
extern f32 nbGetAisyoRitu(s32 skill, s32 source, s32 target);
extern u32 nbGetKoukaBadDamage(s32 skill, s32 source, s32 target, f32 rate, s32 virtual);
extern u32 nbGetKoukaBadKaifuku(s32 skill, s32 source, s32 target, f32 rate, s32 virtual);
extern s32 nbGetHitType(nbActionProcessData_t *action, s32 skill, s32 source, s32 target);
extern f32 nbGetCriticalPow(s32 source, f32 rate);
extern u32 nbGetAisyo(s32 skill, s32 formindex, s32 attr);
extern s32 nbGetNormalSkillAttr(s32 skill);
extern s32 datGetBadStatusAttr(s32 bad);
extern s32 nbCheckFormationFriend(s32 source, s32 target);
extern void nbCheckPacketState(nbActionProcessData_t *action, s32 frame);
extern s32 nbGetDeadType(s32 skill, s32 source, s32 target);
extern s32 nbGetKoukaHp(s32 damage, s32 skill, s32 source, s32 target, f32 rate, s32 count);
extern s32 nbGetKoukaMp(s32 damage, s32 skill, s32 source, s32 target, f32 rate, s32 count);
extern u32 nbCheckHitEffect(s32 skill, s32 source, s32 target);
extern f32 nbGetHpDrainRitu(s32 skill, s32 source, s32 target);
extern f32 nbGetMpDrainRitu(s32 skill, s32 source, s32 target);
extern s32 nbCheckStoneDead(s32 skill, s32 target);
extern u32 nbGetBadStat(s32 formindex);
extern f32 nbGetGensyoRitu(s32 skill, s32 source, s32 target);
extern s32 nbCheckBadKaifuku(datUnitWork_t *work, s32 type);
extern void nbSetBadStat(s32 formindex, s32 sw, s32 bad);
extern s32 nbMakeBadKaifukuPacket(s32 frame, s32 uniqueid, nbFormation_t *form);
extern s32 nbMakeKaifukuPacket(s32 frame, s32 uniqueid, nbFormation_t *form,
                                s32 type, s32 hp, s32 mp, u32 bad);
extern struct nbKoukaPacket_t *nbAddHitEffKoukaPacket(s32 frame, s32 uniqueid,
                                                     nbFormation_t *form, s32 type);
extern struct nbKoukaPacket_t *nbAddHojoKoukaPacket(s32 frame, s32 uniqueid,
                                                    nbFormation_t *form, u32 type, s32 point);
extern datDevilFormat_t datDevilTable[];
s32 GetTimeListHp(nbActionProcessData_t *a, s32 formindex);
s32 GetUnlockTime(nbActionProcessData_t *a, s32 formindex);
s32 CheckCounter(nbActionProcessData_t *a, s32 from, s32 to);

void SetAllPacket(nbActionProcessData_t *a, s32 koukatype, s32 nskill, s32 sformindex,
                  s32 dformindex, s32 count, s32 frame, f32 rate)
{
    extern u32 nbSetAllGetDevilFormatFlag(s32 formindex) asm("nbGetDevilFormatFlag");
    datNormalSkill_t *skill = &datSkillTable.normal[nskill];
    datUnitWork_t *work;
    s32 target = dformindex, originalTarget = dformindex;
    s32 autoskill, hit, type, reflect = 0, canDrain = 1, hidden;
    s32 hpDamage, hpHeal, mpDamage, mpHeal, totalHpDamage = 0, totalMpDamage = 0;
    s32 totalHpHeal = 0, totalMpHeal = 0, hpDrain = 0, mpDrain = 0;
    s32 dead = 0, deadType = 0, damage, hitEffect, blocked, counter = 0;
    s32 i, amount, before, interval, savedFrame;
    u32 badDamage, badHeal, appliedBad = 0;
    f32 speed, drain;

    rate = nbGetAisyoRitu(nskill, sformindex, target) * rate;
    badDamage = nbGetKoukaBadDamage(nskill, sformindex, target, rate, 0);
    badHeal = nbGetKoukaBadKaifuku(nskill, sformindex, target, rate, 0);
    autoskill = a->autoskill;
    hit = nbGetHitType(a, nskill, sformindex, target);
    if (koukatype == 1) {
        a->autoskill = autoskill;
    }
    if (hit == 1) {
        rate = nbGetCriticalPow(sformindex, rate);
    }
    speed = (a->stat & 0x100) ? 3.0f : 1.0f;
    type = 0;
    switch (koukatype) {
    case 1:
        if (nskill == 0x107) {
            frame += 8;
            nbGetMainProcessData()->d31_rebehansya = 1;
        }
        if (nskill == 0x108) {
            frame += 90;
        }
        if (nskill == 0x113) {
            nbGetMainProcessData()->d31_andahansya = 1;
        }
        if (target < 16) {
            nbAddHitEffKoukaPacket(frame, (s32)a, &a->data->form[target], 1);
        }
        SetHitStopFrame(a, frame);
        SetTimeList(a, target, frame, 15, 30);
        frame += 13;
        target = sformindex;
        count = 1;
        canDrain = 0;
        if (nbGetAisyo(-1, sformindex, nbGetNormalSkillAttr(nskill)) & 0x70000) {
            badDamage = 0;
            type = 0x10000;
        }
        switch (badDamage) {
        case 1:
        case 2:
        case 4:
        case 8:
        case 0x10:
        case 0x20:
        case 0x40:
        case 0x80:
        case 0x100:
        case 0x200:
        case 0x400:
        case 0x800:
        case 0x1000:
        case 0x2000:
        case 0x4000:
            if (nbGetAisyo(nskill, sformindex, datGetBadStatusAttr(badDamage)) & 0x170000) {
                badDamage = 0;
            }
            break;
        }
        if ((a->data->stat & 0x400) && !nbCheckFormationPlayerSide(sformindex)) {
            badDamage = 0;
            type = 0x10000;
        }
        if (nskill == 0xD4) {
            badDamage = 0;
            type = 0x10000;
        }
        hit = 0;
        break;
    case 2:
        if (nskill == 0x106) {
            frame += 8;
        }
        if (nskill == 0x107) {
            frame += 60;
        }
        if (nskill == 0x108) {
            frame += 90;
        }
        if (target < 16) {
            nbAddHitEffKoukaPacket(frame, (s32)a, &a->data->form[target], 2);
        }
        SetHitStopFrame(a, frame);
        SetTimeList(a, target, frame, 15, 30);
        frame += 13;
        count = 1;
        canDrain = 0;
        reflect = 1;
        badDamage = 0;
        break;
    case 3:
        SetHitStopFrame(a, frame);
        count = 1;
        canDrain = 0;
        badDamage = 0;
        type = 0x10000;
        break;
    }
    hidden = target >= 16;
    if (!hidden && (a->data->party[a->data->form[target].partyindex].flag & 0x20)) {
        count = 0;
    }
    if (frame < GetUnlockTime(a, target)) {
        frame = GetUnlockTime(a, target);
    }
    if (koukatype == 1 && !(type & 0x70000)) {
        nbCheckPacketState(a, frame);
    }
    for (i = 0; i < count; i++) {
        dead = 0;
        if (badDamage == 0x800) {
            dead = 1;
            deadType = nbGetDeadType(nskill, sformindex, target);
        }
        hpDamage = nbGetKoukaHp(1, nskill, sformindex, target, rate, count);
        hpHeal = nbGetKoukaHp(0, nskill, sformindex, target, rate, count);
        switch (badDamage) {
        case 0x4000:
            badDamage = 0;
            amount = nbGetUnitWorkFromFormindex(target)->hp;
            before = amount - (u32)amount / 10;
            if (before >= amount) {
                before = amount - 1;
            }
            hpDamage = before < 0 ? 0 : before;
            break;
        case 0x2000:
            badDamage = 0;
            amount = nbGetUnitWorkFromFormindex(target)->hp;
            before = amount - ((u32)amount >> 2);
            if (before >= amount) {
                before = amount - 1;
            }
            hpDamage = before < 0 ? 0 : before;
            break;
        case 0x1000:
            badDamage = 0;
            hpDamage = nbGetUnitWorkFromFormindex(target)->hp >> 1;
            break;
        }
        printf("HP damage = %d, kaifuku = %d\n", hpDamage, hpHeal);
        mpDamage = nbGetKoukaMp(1, nskill, sformindex, target, rate, count);
        mpHeal = nbGetKoukaMp(0, nskill, sformindex, target, rate, count);
        printf("MP damage = %d, kaifuku = %d\n", mpDamage, mpHeal);
        if (reflect) {
            if (hpDamage >= 0) {
                hpHeal = hpHeal >= 0 ? hpHeal + hpDamage : hpDamage;
            }
            hpDamage = -1;
            if (mpDamage >= 0) {
                mpHeal = mpHeal >= 0 ? mpHeal + mpDamage : mpDamage;
            }
            mpDamage = -1;
            if (dead) {
                dead = 0;
            }
        }
        blocked = 0;
        if (type == 0x10000) {
            if (!hidden) {
                nbAddMojiEffKoukaPacket(frame, 45, (s32)a, &a->data->form[target], 4);
                blocked = 1;
            }
            hpDamage = mpDamage = -1;
            if (dead) {
                dead = 0;
            }
        }
        damage = 0;
        hitEffect = nbCheckHitEffect(nskill, sformindex, target);
        if (i == count - 1 && badDamage != 0) {
            appliedBad = badDamage;
            damage = 1;
        } else if (hpDamage >= 0 || mpDamage >= 0) {
            appliedBad = 0;
            damage = 1;
        }
        if (dead) {
            appliedBad = 0;
            damage = 1;
        }
        if (damage) {
            if (koukatype == 0 && skill->koukatype == 0) {
                counter = 1;
            }
            if (dead) {
                if (nbCheckKuisibari2Skill(target)) {
                    dead = 0;
                    if (appliedBad == 0x800) {
                        appliedBad = 0;
                    }
                    hpDamage = GetTimeListHp(a, target) - 1;
                    nbAddSkillNamePacket(frame, (s32)a, 0x168);
                } else if (nbCheckKuisibariSkill(target)) {
                    dead = 0;
                    if (appliedBad == 0x800) {
                        appliedBad = 0;
                    }
                    hpDamage = GetTimeListHp(a, target) - 1;
                    nbAddSkillNamePacket(frame, (s32)a, 0x159);
                }
            } else if (hpDamage > 0 && hpDamage >= GetTimeListHp(a, target)) {
                if (nbCheckKuisibariSkill(target)) {
                    hpDamage = GetTimeListHp(a, target) - 1;
                    nbAddSkillNamePacket(frame, (s32)a, 0x159);
                } else if (nbCheckKuisibari2Skill(target)) {
                    hpDamage = GetTimeListHp(a, target) - 1;
                    nbAddSkillNamePacket(frame, (s32)a, 0x168);
                }
            }
            if (hpDamage > 0) {
                if (canDrain) {
                    amount = hpDamage;
                    if (GetTimeListHp(a, target) < hpDamage) {
                        amount = GetTimeListHp(a, target);
                    }
                    totalHpDamage += amount;
                }
                drain = nbGetHpDrainRitu(nskill, sformindex, target);
                if (drain > 0.0f) {
                    hpDrain += (s32)((f32)hpDamage * drain);
                }
                AddTimeListHp(a, target, -hpDamage);
                if (CheckTimeListDead(a, target)) {
                    dead = 1;
                    deadType = nbGetDeadType(nskill, sformindex, target);
                    if (nbGetBadStat(target) == 0x400) {
                        deadType = 1;
                    }
                } else if (nbCheckStoneDead(nskill, target)) {
                    dead = 1;
                    deadType = 1;
                }
            }
            if (mpDamage > 0) {
                if (canDrain) {
                    totalMpDamage += mpDamage;
                }
                drain = nbGetMpDrainRitu(nskill, sformindex, target);
                if (drain > 0.0f) {
                    mpDrain += (s32)((f32)mpDamage * drain);
                }
            }
            if (a->data->stat & 0x20000) {
                dead = 0;
            }
            if (nbSetAllGetDevilFormatFlag(target) & 0x100) {
                dead = 0;
            }
            nbMakeDamagePacket(frame, (s32)a, &a->data->form[target], hit, hpDamage, mpDamage,
                               appliedBad, hitEffect, dead, deadType, 1, koukatype == 1 ? 2 : 0);
            if (koukatype == 1) {
                nbAddSePacket(frame, (s32)a, 0x10002);
            } else if ((koukatype >= 3 || koukatype <= 0) &&
                       (nskill == 0 || (nskill >= 0xA4 && nskill < 0xA7))) {
                nbAddSePacket(frame, (s32)a, hit == 1 ? 0x10003 : 0x10002);
            }
            if (dead) {
                before = frame;
                frame += 30;
                SetTimeList(a, target, before, 30, 30);
                AddTimeListHp(a, target, -GetTimeListHp(a, target));
            } else {
                work = a->work;
                interval = 10;
                if (!(work->flag & 4)) {
                    if (work->nowcommand == 0 ||
                        (work->nowcommand == 1 && work->nowindex >= 0xA4 &&
                         work->nowindex < 0xA8)) {
                        interval = datDevilTable[work->id].attackinterval;
                    }
                }
                interval = (s32)((f32)interval / speed);
                if (interval < 5) {
                    interval = 5;
                }
                SetTimeList(a, target, frame, interval, interval);
                frame += interval;
            }
            if (!dead && appliedBad == 0 && !hidden &&
                nbCheckBadKaifuku(nbGetUnitWorkFromFormindex(target), 2)) {
                nbMakeBadKaifukuPacket(frame, (s32)a, &a->data->form[target]);
            }
        }
        if (hpHeal > 0) {
            totalHpHeal += hpHeal;
        }
        if (mpHeal > 0) {
            totalMpHeal += mpHeal;
        }
        rate *= nbGetGensyoRitu(nskill, sformindex, target);
        if (dead) {
            break;
        }
        if (!damage && hpHeal <= 0 && mpHeal <= 0 && skill->hojotype == 0 && !blocked && badHeal == 0) {
            hit = 0;
            printf("MISS!!!\n");
            nbAddMojiEffKoukaPacket(frame, 45, (s32)a, &a->data->form[target], 3);
            SetHitStopFrame(a, frame);
            break;
        }
    }
    a->allhpdamage += totalHpDamage;
    a->allmpdamage += totalMpDamage;
    savedFrame = frame;
    if (!dead) {
        if (totalHpHeal > 0 || totalMpHeal > 0 || badHeal != 0) {
            if (totalHpHeal <= 0) {
                totalHpHeal = -1;
            }
            nbMakeKaifukuPacket(frame, (s32)a, &a->data->form[target], 0, totalHpHeal,
                                totalMpHeal > 0 ? totalMpHeal : -1, badHeal);
            if (badHeal) {
                nbSetBadStat(target, 0, badHeal);
            }
            before = frame;
            if (totalHpHeal > 0) {
                AddTimeListHp(a, target, totalHpHeal);
            }
            frame += 15;
            SetTimeList(a, target, before, 15, 20);
        }
        if (!dead && !hidden && skill->hojotype) {
            nbAddHojoKoukaPacket(frame, (s32)a, &a->data->form[target],
                                  skill->hojotype, skill->hojopoint);
            SetTimeList(a, target, frame, 10, 15);
        }
    }
    if ((hpDrain > 0 || mpDrain > 0) && canDrain) {
        before = savedFrame + 5;
        if (hpDrain <= 0) {
            hpDrain = -1;
        }
        nbMakeKaifukuPacket(before + 10, (s32)a, &a->data->form[sformindex], 2, hpDrain,
                            mpDrain > 0 ? mpDrain : -1, 0);
        if (hpDrain > 0) {
            AddTimeListHp(a, sformindex, hpDrain);
        }
        SetTimeList(a, sformindex, before, 15, 20);
    }
    if (!dead && counter && a->type == 0) {
        CheckCounter(a, target, sformindex);
    }
    nbSetKuisibariStop(target);
    if (dead) {
        SetDeadPacket(a, target);
    }
    work = nbGetUnitWorkFromFormindex(target);
    if (nskill >= 0x31 && nskill < 0x33) {
        nbSetJyokyoMes(a, 0x4E, 1, work->id);
    } else if (nskill == 0xE0) {
        nbSetJyokyoMes(a, 0x58, 1, work->id);
    }
    switch (koukatype) {
    case 0:
        if (hit == 0) {
            SetPress(a, -1.0f);
            SetNewPress(a, 0);
        } else if (hit == 1) {
            SetPress(a, -0.5f);
            SetNewPress(a, 3);
        } else if (hit == 2) {
            SetPress(a, -0.5f);
            SetNewPress(a, 2);
        }
        break;
    case 1:
    case 2:
    case 3:
        if (!nbCheckFormationFriend(sformindex, originalTarget)) {
            SetPress(a, koukatype == 3 ? -2.0f : -4.0f);
            SetNewPress(a, koukatype == 1 ? 6 : koukatype == 2 ? 7 : 5);
        } else {
            SetPress(a, -1.0f);
            SetNewPress(a, 0);
        }
        break;
    }
}
#else
INCLUDE_ASM(const s32, "newbattle/nbActionProcess", SetAllPacket);
#endif

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

#ifdef NON_MATCHING
void GetKoukaList(nbActionProcessData_t *a, s8 koukalist[32][2], u32 select, s32 nskill)
{
    s8 candidates[32][2];
    nbFormation_t *form;
    datUnitWork_t *work;
    s32 i, j, count, used, last, index, limit, hits, hp, minHp;
    s32 first, second;
    u32 animetype;
    s8 tmp;

    for (i = 0; i < 32; i++) {
        koukalist[i][0] = -1;
        koukalist[i][1] = 0;
    }
    if (a->runerr != 0) {
        return;
    }
    hits = datSkillTable.normal[nskill].targetcntmin;
    if (hits < datSkillTable.normal[nskill].targetcntmax) {
        hits += dds3GetRandIntA(NULL, datSkillTable.normal[nskill].targetcntmax - hits + 1);
    }
    printf("\310\277\261\307\262\363\277\364=%d, select = 0x%08X\n", hits, select);
    for (i = 0; i < 16; i++) {
        if (select & (1U << i)) {
            form = &a->data->form[i];
            if ((form->stat & 1) && ((form->stat & 0x40) >= 0x40) &&
                ((a->data->party[form->partyindex].flag & 0x20) || CheckTimeListDead(a, i))) {
                select &= ~(1U << i);
            }
        }
    }
    if (datSkillTable.normal[nskill].targetrandom == 1) {
        count = 0;
        for (i = 0; i < 32; i++) {
            if (select & (1U << i)) {
                candidates[count][0] = i;
                candidates[count][1] = 0;
                count++;
            }
        }
        if (count == 0) {
            return;
        }
        limit = count + dds3GetRandIntA(NULL, 2);
        last = -1;
        used = 0;
        for (i = 0; i < hits; i++) {
            do {
                index = dds3GetRandIntA(NULL, count);
                j = candidates[index][0];
            } while (count >= 2 && last == j);
            last = j;
            if (candidates[index][1] < limit) {
                candidates[index][1]++;
                for (index = 0; index < used; index++) {
                    if (koukalist[index][0] == j) {
                        break;
                    }
                }
                if (index != used) {
                    koukalist[index][1]++;
                } else {
                    koukalist[used][0] = j;
                    koukalist[used][1] = 1;
                    used++;
                }
            }
        }
        return;
    }
    if ((u32)(nskill - 0x9E) < 2) {
        candidates[0][0] = 0;
        count = 0;
        minHp = 101;
        for (i = 0; i < 16; i++) {
            if (select & (1U << i)) {
                form = &a->data->form[i];
                if ((form->stat & 1) && ((form->stat & 0x40) >= 0x40)) {
                    work = nbGetUnitWorkFromFormindex(form->formindex);
                    if (work->hp != 0) {
                        hp = (work->hp * 100) / work->maxhp;
                        if (hp < minHp) {
                            minHp = hp;
                            candidates[0][0] = i;
                            count = 1;
                        } else if (hp == minHp) {
                            candidates[count][0] = i;
                            count++;
                        }
                    }
                }
            }
        }
        index = 0;
        if (count > 0) {
            index = (s32)dds3GetRandIntA(NULL, count * 10) % count;
        }
        koukalist[0][1] = 1;
        koukalist[0][0] = candidates[index][0];
        return;
    }
    animetype = datSkillTable.visual[nskill].animetype;
    if ((a->data->stat & 8) && nskill == 0xCC) {
        switch (animetype) {
        case 4: animetype = 8; break;
        case 5: animetype = 9; break;
        case 6: animetype = 10; break;
        case 7: animetype = 11; break;
        case 8: animetype = 4; break;
        case 9: animetype = 5; break;
        case 10: animetype = 6; break;
        case 11: animetype = 7; break;
        }
    }
    switch (animetype) {
    case 0: case 1: case 2: case 3:
    case 4: case 5: case 6: case 7: {
        count = 0;
        for (i = 0; i < 32; i++) {
            if (select & (1U << i)) {
                koukalist[count][0] = i;
                koukalist[count][1] = hits;
                count++;
            }
        }
        break;
    }
    case 8: case 9: case 10: case 11: {
        count = 0;
        for (i = 31; i >= 0; i--) {
            if (select & (1U << i)) {
                koukalist[count][0] = i;
                koukalist[count][1] = hits;
                count++;
            }
        }
        break;
    }
    case 12: case 13: case 14: case 15: {
        count = 0;
        for (i = 0; i < 32; i++) {
            if (select & (1U << i)) {
                koukalist[count][0] = i;
                koukalist[count][1] = hits;
                count++;
            }
        }
        if (count >= 2) {
            for (i = 0; i < 32; i++) {
                first = dds3GetRandIntA(NULL, count);
                second = dds3GetRandIntA(NULL, count);
                for (j = 0; j < 2; j++) {
                    tmp = koukalist[first][j];
                    koukalist[first][j] = koukalist[second][j];
                    koukalist[second][j] = tmp;
                }
            }
        }
        break;
    }
    }
}
#else
INCLUDE_ASM(const s32, "newbattle/nbActionProcess", GetKoukaList);
#endif


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
    if (master_text[work->id].senseitype == 0) {
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

#ifdef NON_MATCHING
s32 CheckInotiNego(nbActionProcessData_t *a)
{
    nbFormation_t *form;
    datUnitWork_t *work;
    struct nbParty_t *party;
    s32 i;
    s32 id;
    s32 rate;

    if ((u32)((u16)a->data->StageMajor - 0xF0) < 7 || (a->data->stat & 1) != 0 ||
        (a->party->flag & 6) != 0 || a->partyindex >= 4) {
        return 0;
    }
    if (evtGetAgeOfMoon() == 8 || datCheckTalk(a->work) == 0) {
        return 0;
    }
    if (a->work->nowcommand != 0 && a->work->nowcommand != 1 && a->work->nowcommand != 5) {
        return 0;
    }
    if (a->data->enemypcnt != 1) {
        return 0;
    }
    for (i = 4; i < 14; i++) {
        form = &a->data->form[i];
        if ((form->stat & 0x49) != 0x49) {
            continue;
        }
        work = nbGetUnitWorkFromFormindex(i);
        party = &a->data->party[form->partyindex];
        if ((party->flag & 1) == 0 || (party->flag & 0x20) != 0 || CheckTimeListDead(a, i) != 0) {
            continue;
        }
        id = work->id;
        if (master_text[id].inotitype == 0) {
            continue;
        }
        if ((a->data->stat & 0x2000000) == 0 && a->timelist[i].hp * 100 / work->maxhp >= 25) {
            continue;
        }
        if ((a->work->flag & 4) == 0 && a->work->id == id) {
            continue;
        }
        if (datCheckTalk(work) == 0) {
            continue;
        }
        if ((a->data->stat & 0x2000000) != 0) {
            rate = 100;
        } else {
            if (((dds3LiveBattleUnitsView_t *)DDS3_GBWK)->units[0].level < work->level) {
                continue;
            }
            rate = 6;
        }
        if ((s32)dds3GetRandIntA(NULL, 100) < rate) {
            nbPushAction(1, form->partyindex, a->partyindex, 0);
            printf("inoti party = %d (%X)\n", form->partyindex, work->id);
            return 1;
        }
    }
    return 0;
}
#else
INCLUDE_RODATA(const s32, "newbattle/nbActionProcess", D_00543880);

INCLUDE_RODATA(const s32, "newbattle/nbActionProcess", D_005438A0);

INCLUDE_RODATA(const s32, "newbattle/nbActionProcess", D_005438B0);

INCLUDE_RODATA(const s32, "newbattle/nbActionProcess", D_005438C0);

INCLUDE_ASM(const s32, "newbattle/nbActionProcess", CheckInotiNego);
#endif

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
                nbMakeDeadVisualPacket(0, (s32)a, form, 0, 1);
                SetTimeList(a, i, 0, 30, 30);
                SetDeadPacket(a, i);
            }
        }
    }
    if (a->data->CommSelProcessID != NULL) {
        dds3KillProcess(a->data->CommSelProcessID, 0);
    }
}

/* Classify both HP and MP effects using the native skill-type groups. */
void SetNowActionStat(nbActionProcessData_t *a, s32 comm, s32 nskill) {
    s32 i;
    s32 type;

    if (a->partyindex >= 4) {
        return;
    }
    switch (comm) {
    case 0:
        a->stat |= 0x10000;
        break;
    case 1:
        if (datSkillTable.normal[nskill].koukatype == 0) {
            for (i = 0; i < 2; i++) {
                if (i == 0) {
                    type = datSkillTable.normal[nskill].hptype;
                } else {
                    type = datSkillTable.normal[nskill].mptype;
                }
                switch (type) {
                case 1: case 3: case 4: case 6: case 8:
                case 10: case 12: case 13: case 14:
                    a->stat |= 0x10000;
                    break;
                }
            }
        }
        break;
    }
    if (comm == 1 && datSkillTable.normal[nskill].koukatype == 1) {
        for (i = 0; i < 2; i++) {
            if (i == 0) {
                type = datSkillTable.normal[nskill].hptype;
            } else {
                type = datSkillTable.normal[nskill].mptype;
            }
            switch (type) {
            case 1: case 3: case 4: case 6: case 8:
            case 10: case 12: case 13: case 14:
                a->stat |= 0x20000;
                break;
            }
        }
        if (datSkillTable.normal[nskill].badtype == 1 ||
            datSkillTable.normal[nskill].badtype == 3) {
            a->stat |= 0x20000;
        }
    }
    for (i = 0; i < 2; i++) {
        if (i == 0) {
            type = datSkillTable.normal[nskill].hptype;
        } else {
            type = datSkillTable.normal[nskill].mptype;
        }
        switch (type) {
        case 2: case 5: case 7: case 9: case 11: case 15:
            a->stat |= 0x40000;
            break;
        }
    }
    if (comm == 1 && datSkillTable.normal[nskill].koukatype == 1) {
        a->stat |= 0x80000;
    }
    if (comm == 5) {
        a->stat |= 0x100000;
    }
    if (comm != 1) {
        if (comm == 6) {
            a->stat |= 0x200000;
        }
    } else {
        switch (datSkillTable.normal[nskill].program) {
        case 2:
        case 3:
            a->stat |= 0x200000;
            break;
        }
    }
}

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

#ifdef NON_MATCHING
extern s8 dds3PadButtonState[];
extern void nbSetAutoCommSel(void);
extern s32 nbGetCommSelStat(void);
extern void nbSetCommSelStat(s32 stat);
extern void nbCloseCommSel(void);
extern void nbClearActionFlags(nbMainProcessData_t *data);
extern s32 nbSetActionCommand(nbMainProcessData_t *data, s32 command);
extern s32 nbGetItemSkillNo(nbActionProcessData_t *a, s32 itemid);
extern s32 nbCheckBattleSkillError1(s32 skill);

s32 CheckAction_COMM(nbActionProcessData_t *a)
{
    s32 result = 0;
    s32 close = 0;
    s32 sound = 0;
    datUnitWork_t *work;
    s32 command;
    u16 currentCommand;
    s32 skill;
    s32 err;

    if (dds3DebugSign && dds3PadButtonState[0x29] && dds3PadButtonState[0x2B]) {
        DokodemoPoa(a);
        a->packettime = 60;
        SetNextActionSeq(a, 0x14);
        return 1;
    }
    if (dds3DebugSign && dds3PadButtonState[0x29] && dds3PadButtonState[0x23]) {
        KyouseiPress(a);
        a->packettime = 60;
        SetNextActionSeq(a, 0x14);
        return 1;
    }
    if ((a->data->stat & 0x40) && !(a->stat & 0x80)) {
        a->stat |= 0x80;
        nbSetAutoCommSel();
    }
    if (nbGetCommSelStat() == 3) {
        sound = 8;
        work = nbGetUnitWorkPointer(a->data, a->party);
        command = work->nowcommand;
        currentCommand = *(volatile u16 *)&work->nowcommand;
        switch (command) {
        case 0:
            SetNextActionSeq(a, 5);
            result = 1;
            break;
        case 1:
        case 5:
            skill = work->nowindex;
            if (command == 1) {
                switch (skill) {
                case 0xDE: case 0xDF: case 0xE2: case 0xE3: case 0xE4:
                case 0xE5: case 0xE6: case 0xFC:
                    nbClearActionFlags(a->data);
                    nbSetActionCommand(a->data, 0);
                    currentCommand = work->nowcommand;
                    skill = work->nowindex;
                    break;
                }
            }
            if ((s16)currentCommand != 1) {
                skill = nbGetItemSkillNo(a, work->nowindex);
                printf("item no = %d\n", work->nowindex);
                printf("skill no = %d\n", skill);
            }
            err = nbCheckBattleSkillError1(skill);
            if (err != 0 && !(a->stat & 0x80)) {
                sound = -1;
                nbDispSkillErrorMessage(err);
                nbSetCommSelStat(2);
            } else {
                if ((a->party->flag & 6) || !(datSkillTable.normal[skill].targetarea & 1) ||
                    (datSkillTable.normal[skill].targetarea & 0xC)) {
                    SetNextActionSeq(a, 5);
                } else {
                    SetNextActionSeq(a, 6);
                }
                result = 1;
            }
            break;
        case 4:
            a->negotype = 0;
            result = 1;
            SetNextActionSeq(a, 5);
            break;
        case 6:
            result = 1;
            nbClearActionFlags(a->data);
            nbSetActionCommand(a->data, 0);
            SetNextActionSeq(a, 5);
            break;
        default:
            SetNextActionSeq(a, 7);
            close = 1;
            result = 1;
            break;
        }
        if (sound >= 0 && !(a->stat & 0x80)) {
            nbPlaySe(sound);
        }
    }
    if (close) nbCloseCommSel();
    return result;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbActionProcess", CheckAction_COMM);
#endif

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
    nbFormation_t *form, s32 type, s32 hp, s32 mp, u32 badstat);
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

extern s32 nbGetItemSkillNo(nbActionProcessData_t *a, s32 itemid);
extern void nbCloseCommSel(void);
extern void nbSetAutoCommSel(void);
extern void nbSetCommSelStat(s32 stat);

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
extern void SetAllPacket(nbActionProcessData_t *a, s32 koukatype, s32 nskill, s32 sformindex,
                         s32 dformindex, s32 count, s32 frame, f32 rate);
extern void GetKoukaList(nbActionProcessData_t *a, s8 koukalist[32][2], u32 select, s32 nskill);
extern s32 nbGetNormalSkillAttr(s32 nskill);
extern f32 nbGetGensyoRitu(s32 nskill, s32 sformindex, s32 dformindex);
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

#ifdef NON_MATCHING
extern char D_00551128[];
extern void datDebugPrintf(const char *format, ...);
extern s32 nbGetUnitFromAction(nbFormation_t *form, s32 motion);
extern s32 p2sdLoadCheckMidi(s32 midi);
extern struct nbKoukaPacket_t *nbAddSePacket2(s32 frame, s32 uniqueid, s32 midi);
extern s32 pacFileCheckEnd(const struct fileRequest *file);
extern sdfMemHandle_t *fileGetMemHandle(const struct fileRequest *file);
extern u32 fileGetLoadSize(const struct fileRequest *file);
extern void *sdfMemLock(sdfMemHandle_t *handle);
extern void sdfMemFreeDirect(sdfMemHandle_t *handle);
extern s32 fileHandleFree(struct fileRequest *file);
extern void p2sdSetPcmBuffer(s32 data, s32 size);
extern struct effBTL_t *effBTLCreate(void *data);

s32 CheckAction_READY(nbActionProcessData_t *a)
{
    s32 size;
    void *buffer;
    sdfMemHandle_t *handle;

    switch (a->seqcnt) {
    case 0:
        a->skilltime++;
        if (a->motse) {
            if (p2sdLoadCheckMidi(nbGetUnitFromAction(a->form, a->motion)) == 0) {
                printf("mot se wait(1)..\n");
                return 0;
            }
            a->data->stat &= ~0x200;
            nbAddSePacket2(0, (s32)a, nbGetUnitFromAction(a->form, a->motion));
        }
        datDebugPrintf(D_00551128);
        a->hEffBTL = NULL;
        if (a->bedfile == NULL && a->midifile == NULL) {
            a->seqcnt = 5;
        } else if (a->midifile != NULL) {
            a->seqcnt = 1;
        } else {
            a->seqcnt = 3;
        }
        break;
    case 1:
        a->skilltime++;
        if (pacFileCheckEnd(a->midifile)) {
            a->midibuf = fileGetMemHandle(a->midifile);
            size = fileGetLoadSize(a->midifile);
            buffer = sdfMemLock(a->midibuf);
            printf("se size = %d\n", size);
            if (p2sdLoadCheckMidi(a->midino) == 0) {
                p2sdSetPcmBuffer((s32)buffer, size);
                a->data->stat |= 0x200;
            }
            datDebugPrintf("load end\n");
            a->seqcnt = 2;
        } else {
            printf("load se or bed\n");
            datDebugPrintf("load se or bed..\n");
        }
        break;
    case 2:
        a->skilltime++;
        if (p2sdLoadCheckMidi(a->midino)) {
            printf("se ok!! [%08X]\n", a->midino);
            datDebugPrintf("se ok!! [%08X]\n", a->midino);
            sdfMemFreeDirect(a->midibuf);
            fileHandleFree(a->midifile);
            a->seqcnt = a->bedfile != NULL ? 3 : 5;
            a->data->stat &= ~0x200;
        } else {
            printf("se wait(2)..\n");
            datDebugPrintf("se wait..\n");
        }
        break;
    case 3:
        a->skilltime++;
        if (pacFileCheckEnd(a->bedfile)) {
            printf("bed ok!!\n");
            datDebugPrintf("bed ok!!\n");
            handle = fileGetMemHandle(a->bedfile);
            a->hEffBTL = effBTLCreate(sdfMemLock(handle));
            sdfMemFreeDirect(handle);
            fileHandleFree(a->bedfile);
            a->seqcnt = 5;
        } else {
            printf("load bed\n");
            datDebugPrintf("load bed..\n");
        }
        break;
    case 5:
        if (a->timer && (u16)a->seqtime < a->timer) {
            return 0;
        }
        switch (a->work->nowcommand) {
        case 0: case 1: case 5: case 6: SetNextActionSeq(a, 8); break;
        case 2: SetNextActionSeq(a, 13); break;
        case 3: case 8: case 9: case 10: SetNextActionSeq(a, 12); break;
        case 4: SetNextActionSeq(a, 9); break;
        case 7: SetNextActionSeq(a, 14); break;
        }
        datDebugPrintf("TOTAL SETUP TIME = %d/%d\n", a->skilltime, a->timer);
        if (a->jyokyomes > 0) {
            nbDispJyokyoHelp(a->data->mesmngid, a->jyokyomes, a->jyo_varid, a->jyo_varind);
            a->jyokyomes = 0;
        }
        return 1;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbActionProcess", CheckAction_READY);
#endif

/* NON_MATCHING: 1118 of 1280 words differ (first at +0xC8); 10 words short of retail's 1290. */
#ifdef NON_MATCHING
struct effBTLCut_t;
extern struct effBTLCut_t *effBTLCutCreate(struct effBTL_t *effect, u16 cut, void *form, s32 stat);
extern s32 nbGetKoukaType(s32 skill, s32 source, s32 target);
extern s32 nbGetItemSkillNo(nbActionProcessData_t *a, s32 itemid);
extern f32 nbGetMotionSpeed(nbFormation_t *form, s32 motion, f32 rate);
extern s32 nbGetMotionActframe(nbFormation_t *form, s32 motion, f32 rate);
extern s32 nbHatudoCheckSkill(datUnitWork_t *work, s32 skill);
extern s32 nbCheckBattleSkillError3(s32 skill, u32 select);
extern void effBTLSkillSetState(s32 state);
extern s32 effBTLGetMotionChangeFrame(struct effBTL_t *effect, u16 cut);
extern s32 effBTLGetCutEndFrame(struct effBTL_t *effect, u16 cut);
extern s32 effBTLCutGetMotionChangeFrame(struct effBTLCut_t *cut);
extern u32 effBTLCutGetCutEndFrame(struct effBTLCut_t *cut);
extern void effBTLCutSetDrawEndFrame(struct effBTLCut_t *cut, u32 frame);
extern void effBTLCutSetCameraState(struct effBTLCut_t *cut);
extern f64 fptodp(f32 value);
extern struct nbKoukaPacket_t *nbAddSkillEffectKoukaPacket(s32 start, s32 length, s32 uniqueid, struct effBTLCut_t *cut);
extern struct nbKoukaPacket_t *nbAddSkillEffectKoukaPacket2(s32 start, s32 length, s32 uniqueid, struct effBTL_t *effect, s32 cut, nbFormation_t *form, s32 type);
extern struct nbKoukaPacket_t *nbAddFormFlagOnOffPacket(s32 start, s32 uniqueid, nbFormation_t *form, s32 sw, u32 flag);
extern struct nbKoukaPacket_t *nbAddJyokyoHelpPacket(s32 start, s32 uniqueid, s32 no, s32 var_id, s32 var_ind);
extern struct nbKoukaPacket_t *nbAddSkillColorOffPacket(s32 start, s32 uniqueid);
extern struct nbKoukaPacket_t *nbAddMotionStopClearPacket(s32 start, s32 uniqueid, nbFormation_t *form);
extern void nbAddHatudoEffectEndPacket(s32 start, s32 uniqueid);
extern void nbAddHitStopPacket(s32 start, s32 uniqueid);
extern void nbDispSkillName(s32 skill);

void SetAction_SKILL(nbActionProcessData_t *a)
{
    s8 list[32][2];
    struct effBTLCut_t *cut;
    s32 i;
    s32 cutEnd;
    s32 count;
    s32 frame;
    s32 state;
    s32 showCut;
    s32 successful;
    s32 anyHit;
    s32 reflectOnly;
    s32 absorbOnly;
    s32 anyNormal;
    s32 skill, endFrame, cutFrame, motionFrame, effectType, type, singleCut, startframe;
    s32 amount, hp, mp, autoFrame, help, dead;
    f32 speed, power, motionSpeed;

    cut = NULL;
    a->newpresstype = 0;
    a->data->d31_kantuu = 0;
    cutEnd = 0;
    count = 0;
    state = 0;
    showCut = 0;
    successful = 1;
    anyHit = 0;
    reflectOnly = 1;
    absorbOnly = 1;
    anyNormal = 0;
    speed = (a->stat & 0x100) ? 3.0f : 1.0f;
    if (a->hEffBTL != NULL && a->runerr == 0) {
        cut = effBTLCutCreate(a->hEffBTL, 1, a->form, 1);
        motionFrame = effBTLCutGetMotionChangeFrame(cut);
        endFrame = effBTLCutGetCutEndFrame(cut);
        printf("mot %d, cut %d\n", motionFrame, endFrame);
        SetTimeList(a, a->form->formindex, 0, endFrame, endFrame);
    } else {
        endFrame = 0;
        amount = (s32)(10.0f / speed);
        SetTimeList(a, a->form->formindex, 0, amount, amount);
    }
    if (a->form->motendtype == 2 && a->form->motcate >= 0 && a->form->motenddata >= 0) {
        motionSpeed = nbGetMotionSpeed(a->form, a->form->motcate, speed);
        amount = (s32)((f32)a->form->motenddata / motionSpeed) -
                 nbGetMotionActframe(a->form, a->form->motcate, speed) + (s32)(10.0f / speed);
        OverwriteTimeList(a, a->form->formindex, 0, amount, amount);
    }
    skill = 0;
    switch (a->work->nowcommand) {
    case 0:
        break;
    case 1:
        skill = a->work->nowindex;
        break;
    case 5:
        skill = nbGetItemSkillNo(a, a->work->nowindex);
        break;
    case 6:
        skill = 0xFF;
        break;
    }
    power = 1.0f;
    SetNowActionStat(a, a->work->nowcommand, skill);
    frame = endFrame;
    GetKoukaList(a, list, a->select, skill);
    for (i = 0; i < 32 && list[i][0] >= 0; i++) {
        frame = a->timelist[list[i][0]].lockframe;
        if (endFrame >= frame) {
            frame = endFrame;
        }
        startframe = frame;
        type = nbGetKoukaType(skill, a->form->formindex, list[i][0]);
        printf("\245\271\245\255\245\353\270\372\262\314\245\277\245\244\245\327 = %d\n", type);
        singleCut = type < 3;
        if (a->type == 4 && singleCut) {
            switch (type) {
            case 1:
            case 2:
                type = 3;
                break;
            }
            singleCut = type < 3;
        }
        cutFrame = 0;
        if (a->hEffBTL != NULL && i < 16) {
            switch (type) {
            case 0:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
            case 14:
                cutFrame = effBTLGetMotionChangeFrame(a->hEffBTL, 2);
                cutEnd = effBTLGetCutEndFrame(a->hEffBTL, 2);
                printf("cut3 mottime:%d  endtime:%d\n", cutFrame, cutEnd);
                SetTimeList(a, list[i][0], startframe, 0, cutEnd + 1);
                break;
            }
        }
        startframe += cutFrame;
        frame += cutFrame;
        nbAddFormFlagOnOffPacket(startframe, (s32)a, &a->data->form[list[i][0]], 0, 0x2000);
        if ((u32)(type - 1) >= 2) {
            reflectOnly = 0;
        }
        if (type != 2) {
            absorbOnly = 0;
        }
        switch (type) {
        case 0:
        case 2:
        case 3:
            anyHit = 1;
            break;
        }
        if (type == 0) {
            anyNormal = 1;
        }
        switch (type) {
        case 0:
        case 1:
        case 2:
        case 3:
            SetAllPacket(a, type, skill, a->form->formindex, list[i][0], list[i][1], startframe, power);
            break;
        case 4:
            SetSakePacket(a, skill, a->form->formindex, list[i][0], list[i][1], startframe);
            break;
        case 5:
            SetMissPacket(a, skill, a->form->formindex, list[i][0], list[i][1], startframe);
            break;
        case 6:
            if (!SetPlayerSummonPacket(a, skill, a->form->formindex, list[i][0], startframe)) {
                successful = 0;
            }
            break;
        case 7:
            SetAnalyzePacket(a, list[i][0], startframe);
            break;
        case 8:
            SetTorafuPacket(a, list[i][0], startframe);
            break;
        case 9:
            SetMakaPacket(a, list[i][0], startframe);
            break;
        case 10:
            SetEnemyEscapePacket(a, list[i][0], startframe);
            break;
        case 11:
            SetOngShadowPacket(a, skill, a->form->formindex, list[i][0], list[i][1], startframe);
            break;
        case 12:
            SetAuroraPacket(a, list[i][0], startframe);
            break;
        case 13:
            SetAddPressPacket(a, skill);
            break;
        case 14:
            SetAddFlareChargePacket(a, startframe);
            break;
        }
        if (type == 0 && (a->data->stat & 0x400) && (u32)(list[i][0] - 4) < 10 &&
            nbGetNormalSkillAttr(skill) != 0xE) {
            SetOngShadowPacket2(a, list[i][0], startframe + 5);
        }
        if (a->hEffBTL != NULL && i < 16 && successful) {
            switch (type) {
            case 0:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
            case 14:
                if (type != 3 || skill != 0x107) {
                    dead = CheckTimeListDead(a, list[i][0]);
                    effectType = 1;
                    if ((u32)(type - 4) < 2) {
                        effectType = 0x20;
                    }
                    if (dead) {
                        effectType = 3;
                    }
                    nbAddSkillEffectKoukaPacket2(startframe - cutFrame, cutEnd, (s32)a, a->hEffBTL, 2,
                                                 &a->data->form[list[i][0]], effectType);
                    nbAddSePacket(startframe - cutFrame, (s32)a, ((skill + 0x200) << 16) | 1);
                }
                break;
            }
        }
        power *= nbGetGensyoRitu(skill, a->form->formindex, list[i][0]);
        if (datSkillTable.normal[skill].koukatype == 0) {
            nbSetHojoCounter(a->form->formindex, 0xF, 0);
        }
        if (type == 3) {
            switch (nbGetNormalSkillAttr(skill)) {
            case 1:
                nbSetHojoCounter(list[i][0], 0xA, 0);
                break;
            case 2:
                nbSetHojoCounter(list[i][0], 9, 0);
                break;
            case 6:
                nbSetHojoCounter(list[i][0], 0xC, 0);
                break;
            case 7:
                nbSetHojoCounter(list[i][0], 0xB, 0);
                break;
            }
        }
        if ((u32)(skill - 0x103) < 2) {
            nbSetHojoCounter(list[i][0], 0xB, 0);
        }
        if (type == 1) {
            nbAddHatudoEffectEndPacket(startframe, (s32)a);
        }
        switch (datSkillTable.visual[skill].animetype) {
        case 0:
            break;
        case 1:
        case 5:
        case 9:
        case 13:
            endFrame += 5;
            break;
        case 2:
        case 6:
        case 10:
        case 14:
            endFrame += 8;
            break;
        case 3:
        case 7:
        case 11:
        case 15:
            endFrame += 10;
            break;
        case 4:
        case 8:
        case 12:
            endFrame++;
            break;
        }
        switch (type) {
        case 0:
            break;
        case 1:
            state |= 4;
            break;
        case 2:
            state |= 8;
            break;
        case 4:
            state |= 0x20;
            break;
        case 3:
            state |= 0x10;
            break;
        case 5:
            state |= 0x40;
            break;
        case 6:
        case 7:
        case 8:
        case 9:
            break;
        }
        if (!singleCut || type <= 0) {
            showCut = 1;
        }
        count++;
    }
    if (reflectOnly && skill == 0x108) {
        showCut = 1;
    }
    if (absorbOnly && (u32)(skill - 0x106) < 2) {
        showCut = 1;
    }
    if (skill == 0 && a->autoskill == 0 && nbHatudoCheckSkill(a->work, 0x12E) &&
        !CheckTimeListDead(a, a->form->formindex)) {
        a->autoskill = 0x12E;
        autoFrame = GetAllEndTime(a);
        hp = a->allhpdamage / datSkillTable.special[0xE].n;
        mp = a->allmpdamage / datSkillTable.special[0xE].m;
        if (hp == 0) {
            hp = -1;
        }
        if (mp == 0) {
            mp = -1;
        }
        if (hp > 0 || mp > 0) {
            nbMakeKaifukuPacket(autoFrame, (s32)a, a->form, 3, hp, mp, 0);
            if (hp > 0) {
                AddTimeListHp(a, a->form->formindex, hp);
            }
            amount = (s32)(20.0f / speed);
            SetTimeList(a, a->form->formindex, autoFrame, amount, amount);
        }
    }
    if (skill == 0x11E && a->autoskill == 0 && !CheckTimeListDead(a, a->form->formindex) &&
        a->runerr == 0) {
        a->autoskill = 0x12E;
        autoFrame = GetAllEndTime(a);
        a->autoskill = 0;
        hp = (a->work->maxhp * datSkillTable.normal[0x11E].hojopoint) / 100;
        if (hp == 0) {
            hp = -1;
        }
        if (hp > 0) {
            nbMakeKaifukuPacket(autoFrame, (s32)a, a->form, 0, hp, -1, 0);
            AddTimeListHp(a, a->form->formindex, hp);
            amount = (s32)(20.0f / speed);
            SetTimeList(a, a->form->formindex, autoFrame, amount, amount);
        }
    }
    if (skill == 0x109 && a->autoskill == 0 && !CheckTimeListDead(a, a->form->formindex) &&
        a->runerr == 0) {
        a->autoskill = 0x12E;
        autoFrame = GetAllEndTime(a);
        a->autoskill = 0;
        mp = (a->work->maxmp * datSkillTable.normal[0x109].mpn) / 100;
        if (mp == 0) {
            mp = -1;
        }
        if (mp > 0) {
            nbMakeKaifukuPacket(autoFrame, (s32)a, a->form, 0, -1, mp, 0);
            amount = (s32)(20.0f / speed);
            SetTimeList(a, a->form->formindex, autoFrame, amount, amount);
        }
    }
    effBTLSkillSetState(state);
    autoFrame = GetAllEndTime(a);
    OverwriteTimeList(a, a->form->formindex, 0, (s32)((f32)autoFrame * 0.79999996f), -1);
    if (CheckTimeListDead(a, a->form->formindex) && !(a->party->flag & 0x20)) {
        nbMakeDeadVisualPacket(frame, (s32)a, a->form, 0, 1);
        SetTimeList(a, a->form->formindex, frame, 30, 30);
        SetDeadPacket(a, a->form->formindex);
    }
    if (!CheckTimeListDead(a, a->form->formindex) && a->type == 0) {
        CheckArrCounter(a);
    }
    printf("\245\310\241\274\245\277\245\353\245\300\245\341\241\274\245\270 HP=%d, MP=%d\n", a->allhpdamage, a->allmpdamage);
    printf("skill kouka end time = %d\n", autoFrame);
    printf("skill kouka unlock time = %d\n", GetAllUnlockTime(a));
    if (a->runerr == 0) {
        if (a->autoskill != 0) {
            switch (a->autoskill) {
            case 0x12B: nbDispSkillName(0x12B); break;
            case 0x12E: nbDispSkillName(0x12E); break;
            case 0x12C: nbDispSkillName(0x12C); break;
            case 0x12D: nbDispSkillName(0x12D); break;
            }
        }
        if (a->runerr == 0) {
            switch (skill) {
            case 0x34: nbSetJyokyoMes(a, 0x10, -1, 0); break;
            case 0x36: nbSetJyokyoMes(a, 0x12, -1, 0); break;
            case 0x35: nbSetJyokyoMes(a, 0x14, -1, 0); break;
            case 0x39: nbSetJyokyoMes(a, 0x16, -1, 0); break;
            case 0x11A:
                if (anyHit) {
                    nbSetJyokyoMes(a, 0x16, -1, 0);
                }
                break;
            case 0x40: nbSetJyokyoMes(a, 0x18, -1, 0); break;
            case 0x41: nbSetJyokyoMes(a, 0x1A, -1, 0); break;
            case 0x42: nbSetJyokyoMes(a, 0x1C, -1, 0); break;
            case 0x43: nbSetJyokyoMes(a, 0x1E, -1, 0); break;
            case 0x44: nbSetJyokyoMes(a, 0x20, -1, 0); break;
            case 0x46: nbSetJyokyoMes(a, 0x22, -1, 0); break;
            case 0x45: nbSetJyokyoMes(a, 0x24, -1, 0); break;
            case 0xCB: nbSetJyokyoMes(a, 0x50, -1, 0); break;
            case 0xCC: nbSetJyokyoMes(a, 0x52, -1, 0); break;
            case 0xCD: nbSetJyokyoMes(a, 0x54, -1, 0); break;
            case 0x109: nbSetJyokyoMes(a, 0x54, -1, 0); break;
            case 0x112: nbSetJyokyoMes(a, 0x5C, -1, 0); break;
            case 0xCE: nbSetJyokyoMes(a, 0x56, -1, 0); break;
            case 0x4D: nbSetJyokyoMes(a, 0x5C, -1, 0); break;
            case 0x114: nbSetJyokyoMes(a, 0x62, -1, 0); break;
            case 0x110: nbSetJyokyoMes(a, 0x60, -1, 0); break;
            default:
                if (anyNormal && a->data->d31_kantuu) {
                    nbSetJyokyoMes(a, 0x64, -1, 0);
                }
                break;
            }
            if (nbCheckBattleSkillError3(skill, a->select)) {
                printf("Kereijyou Koukaga nai ---------------------\n");
                nbSetJyokyoMes(a, 0x44, -1, 0);
            }
            help = a->jyokyomes;
            if (help > 0 && (a->stat & 0x20)) {
                if (help & 1) {
                    help--;
                    a->jyokyomes = help;
                } else {
                    help++;
                    a->jyokyomes = help;
                }
            }
        } else {
            help = a->jyokyomes;
        }
    } else {
        help = a->jyokyomes;
    }
    if (help > 0) {
        switch (skill) {
        case 0x31:
        case 0x32:
            frame += 20;
            SetTimeList(a, a->form->formindex, frame, 40, 40);
            help = a->jyokyomes;
            break;
        }
        nbAddJyokyoHelpPacket(frame, (s32)a, help, a->jyo_varid, a->jyo_varind);
        a->jyokyomes = 0;
    }
    if (cut != NULL) {
        effBTLCutSetDrawEndFrame(cut, autoFrame);
        nbAddSkillColorOffPacket(autoFrame, (s32)a);
        if (showCut) {
            nbAddSkillEffectKoukaPacket(0, autoFrame, (s32)a, cut);
            if (absorbOnly && skill == 0x106) {
                nbAddSePacket(0, (s32)a, 0x03060001);
            } else {
                nbAddSePacket(0, (s32)a, (skill + 0x200) << 16);
            }
        } else {
            effBTLCutSetCameraState(cut);
        }
    } else {
        nbAddSkillColorOffPacket(autoFrame, (s32)a);
    }
    if (a->form->motendtype == 1) {
        nbAddMotionStopClearPacket(autoFrame * 60 / 100, (s32)a, a->form);
    }
    if (count > 0) {
        if (a->type == 0) {
            nbMakePressPacket(frame, (s32)a, a->press);
            nbMakeNewPressPacket(frame, (s32)a, a->newpresstype, a->form);
            printf("press %f\n", fptodp(a->press));
        }
    } else {
        nbMakePressPacket(frame, (s32)a, -1.0f);
        nbMakeNewPressPacket(frame, (s32)a, 0, a->form);
    }
    if (a->newaddpresstype > 0) {
        nbMakeNewPressPacket(frame + 20, (s32)a, a->newaddpresstype, a->form);
    }
    if (a->hitstopframe > 0 && evtBitCheck(0xC09)) {
        amount = (a->data->stat & 8) ? a->data->enemypcnt : a->data->playerpcnt;
        if ((s32)((a->data->press3 + a->press / (f32)amount) * 100.0f) <= 0) {
            nbAddHitStopPacket(a->hitstopframe, (s32)a);
        }
    }
}
#else
INCLUDE_ASM(const s32, "newbattle/nbActionProcess", SetAction_SKILL);
#endif

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

#ifdef NON_MATCHING
extern s32 nbMakeUnitFadeInPacket(s32 startframe, s32 uniqueid, nbFormation_t *form,
                                  u32 col, s32 delay, s32 aframe, s32 colframe);
extern struct nbKoukaPacket_t *nbAddMotionStopClearPacket(s32 startframe, s32 uniqueid,
                                                          nbFormation_t *form);
extern void nbAddHatudoEffectEndPacket(s32 startframe, s32 uniqueid);
extern void nbAddPanelStockFadeOutPacket(s32 startframe, s32 uniqueid);
extern void nbAddPanelStockEndPacket(s32 startframe, s32 uniqueid);

void SetAction_CHECK1(nbActionProcessData_t *a)
{
    s32 i;
    s32 unlock;
    s32 end;
    s32 delay;
    s32 fade;
    nbFormation_t *form;

    if (CheckTimeListDead(a, a->form->formindex) == 0 &&
        nbGetBadStat(a->form->formindex) == 0x40 &&
        CheckTimeListDeadEnemyParty(a, a->form->formindex) == 0 &&
        a->data->endtype == 0 &&
        (a->work->nowcommand != 5 || a->work->nowindex != 1) &&
        !(a->party->flag & 0x20)) {
        SetDamagePacket(a, a->work->maxhp >> 3);
    }
    unlock = GetAllUnlockTime(a);
    for (i = 0; i < 16; i++) {
        form = &a->data->form[i];
        if ((form->stat & 1) && (form->stat & 0x40) &&
            CheckTimeListDead(a, i) &&
            !(a->data->party[a->data->form[i].partyindex].flag & 0x20)) {
            printf("dead formindex = %d!!\n", i);
            nbMakeDeadVisualPacket(unlock, (s32)a, &a->data->form[i], 0, 1);
            SetTimeList(a, i, unlock, 30, 30);
            SetDeadPacket(a, i);
            SetTimeList(a, i, unlock, 20, 20);
        }
    }
    if (CheckTimeListDead(a, a->form->formindex) == 0) {
        end = GetAllEndTime(a);
        unlock = GetAllUnlockTime(a);
        delay = 0;
        if (a->form->motendtype != 2 && !(a->stat & 0x100)) {
            delay = 3;
        }
        if (a->form->stat & 0x10) {
            if (a->form->motendtype == 2) {
                SetTimeList(a, a->form->formindex, end, 6, 6);
            } else {
                SetTimeList(a, a->form->formindex, end, delay + 1, delay + 1);
            }
            if (a->form->motendtype == 2) {
                unlock += delay;
                nbMakeUnitFadeInPacket(unlock, (s32)a, a->form, 0, 2, 4, 4);
                nbAddFormOnOffPacket(unlock, (s32)a, a->form, 0);
                if (a->form->motendtype == 2) {
                    nbAddMotionStopClearPacket(unlock, (s32)a, a->form);
                }
            }
        }
    }
    a->packettime = GetAllEndTime(a);
    nbAddHatudoEffectEndPacket(a->packettime * 70 / 100, (s32)a);
    if (a->stat & 0x400) {
        fade = a->packettime - 10;
        if (fade < 0) fade = 0;
        nbAddPanelStockFadeOutPacket(fade, (s32)a);
        nbAddPanelStockEndPacket(a->packettime - 1, (s32)a);
    }
    printf("allframe %d\n", a->packettime);
}
#else
INCLUDE_RODATA(const s32, "newbattle/nbActionProcess", D_00544020);

INCLUDE_ASM(const s32, "newbattle/nbActionProcess", SetAction_CHECK1);
#endif

#ifdef NON_MATCHING
extern s8 dds3PadButtonState[];
extern void nbPanelAnalyzeEnd(void);
extern void nbOnOffKoukaPacketFlag(s32 type, u32 flag, s32 on);
extern void nbUpdateOrder(void);
extern s32 CheckInotiNego(nbActionProcessData_t *a);
extern void datClearCommMemory(datUnitWork_t *work);
extern char D_00544058[];

s32 CheckAction_CHECK1(nbActionProcessData_t *a)
{
    datUnitWork_t *work;

    if (a->stat & 0x10) {
        if (dds3PadButtonState[0x21] < 0 || dds3PadButtonState[0x23] < 0) {
            a->stat &= ~0x10;
            nbPanelAnalyzeEnd();
            a->packettime = 21;
            nbOnOffKoukaPacketFlag(14, 2, 0);
        } else {
            nbOnOffKoukaPacketFlag(14, 2, 1);
        }
        return 0;
    }
    if (a->packettime >= 16) {
        a->packettime--;
        return 0;
    }
    nbUpdateOrder();
    if (CheckTimeListDead(a, a->form->formindex) == 0) {
        CheckInotiNego(a);
    }
    if (CheckTimeListDead(a, a->form->formindex) == 0) {
        CheckBinjoNego(a);
    }
    if (CheckTimeListDead(a, a->form->formindex) == 0 && a->form->motendtype == 3) {
        a->form->stat |= 0x4000;
    }
    CheckDeadScript(a);
    work = nbGetUnitWorkPointer(a->data, a->party);
    if (work == NULL) {
        a->data->prevflag = 0;
        a->data->previndex = 0;
        a->data->prevcommand = 0;
        a->data->prevcommind = 0;
    } else {
        a->data->previndex = a->partyindex;
        a->data->prevcommand = work->nowcommand;
        a->data->prevcommind = work->nowindex;
        printf(D_00544058, work->nowindex);
        a->data->prevflag = 0;
        if (a->stat & 8) {
            a->data->prevflag = 1;
        }
        if (work->nowcommand == 0 || work->nowcommand == 3 ||
            (work->nowcommand == 1 && work->nowindex != 0x31 && work->nowindex != 0x32 &&
             (work->nowindex < 0xA4 || work->nowindex > 0xA6) && work->nowindex != 0xF7)) {
            work->prevcommand = work->nowcommand;
            work->prevtform = work->nowtform;
            work->previndex = work->nowindex;
            work->prevtarea = work->nowtarea;
        } else if (work->nowcommand != 1 || work->nowindex == 0x31 || work->nowindex == 0x32) {
            datClearCommMemory(work);
        }
    }
    SetNextActionSeq(a, 20);
    return 1;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbActionProcess", CheckAction_CHECK1);
#endif

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

