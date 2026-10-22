#include "common.h"
#include "kernel.h"
#include "nb.h"
#include "event.h"


/* nbKoukaPacket_t.type, as the kouka dispatchers switch on it. Names and
 * values from the IDB; the code corroborates them, with 39 and 40 calling
 * effPCPBossBUNSIN* and 41 and 42 calling effPCPBossONGY*. */
typedef enum NB_KOUKA_TYPE {
    SKILLEFF = 0,
    TARMOVE = 1,
    HOMEMOVE = 2,
    ROT = 3,
    HOMEROT = 4,
    UNITCOL = 5,
    UNITHOMECOL = 6,
    UNITA = 7,
    UNITHOMEA = 8,
    DMGMOT = 9,
    BADDMGMOT = 10,
    NB_KOUKA_TYPE_RETURN = 11,
    NB_KOUKA_TYPE_SUMMON = 12,
    SAKE = 13,
    SKILLCOLOFF = 14,
    HITEFF = 15,
    NUMEFF = 16,
    HPMP = 17,
    PRESS = 18,
    BADSTAT = 19,
    BADMOTKAIFUKU = 20,
    SKILLEFF2 = 21,
    MOJIEFF = 22,
    BADEFF = 23,
    BADEFFKAIFUKU = 24,
    SE = 25,
    SKILLEFF1 = 26,
    MOTION = 27,
    SE2 = 28,
    SELOCK = 29,
    HITSTOP = 30,
    NEWPRESS = 31,
    MOTIONSTOP = 32,
    MOTIONSTOPCLS = 33,
    ANALYZE = 34,
    JYOKYOHELP = 35,
    DEADPCM = 36,
    BADMOT = 37,
    FORMONOFF = 38,
    BOSSONGBUN = 39,
    BOSSONGBUN2 = 40,
    BOSSONGOUT = 41,
    BOSSONGOUT2 = 42,
    SKILLEFF3 = 43,
    FORMFLAGONOFF = 44,
    NB_KOUKA_TYPE_FLY = 45,
    DEADEVENT = 46,
    FADEOUT = 47,
    FADEIN = 48,
    SKILLNAME = 49,
    HOJO = 50,
    FLARECHARGE = 51,
    PADSHOCK = 52,
    EASYDMGMOT = 53,
    SKILLCOLON = 54,
    SKILLMOTION = 55,
    STKPNL_FOUT = 56,
    STKPNL_END = 57,
    HATUDOEFF_END = 58
} NB_KOUKA_TYPE;

typedef union
{
    s8 c[4];
    u8 uc[4];
    s16 s[2];
    u16 us[2];
    s32 i;
    f32 f;
    void *p;
} nbKoukaData_u;

typedef struct nbKoukaPacket_t
{
    s16 startframe;
    s16 framelength;
    s8 type;
    u8 flag;
    u16 time;
    s32 uniqueid;
    struct nbKoukaPacket_t *next;
    struct nbKoukaPacket_t *prev;
    nbFormation_t *form;
    datUnitWork_t *unit;
    struct nbParty_t *party;
    s32 partyno;
    struct effBTLCutHandle_t *cuteff;
    nbKoukaData_u data1;
    nbKoukaData_u data2;
    sdfVector4_t v;
} nbKoukaPacket_t;

typedef struct nbParty_t
{
    u16 flag;
    s16 formindex;
    s16 statindex;
    u16 partyindex;
    s16 count[20];
    u32 aisyo[16];
    u16 hojoskill[8];
} nbParty_t;

extern dds3ProcessID_t *dds3SearchProcessName(char *namestr);
extern void *dds3GetProcessData(dds3ProcessID_t *id);
extern void *sdfChipAllocDirect(u32 aSize);
extern void sdfChipFreeDirect(void *aPtr);
extern void nbPanelStockFadeOUT(void);
extern void nbPanelStockEnd(void);

extern void dds3PadVibrationSet(s32 port, s32 power, s32 frame);
extern void dds3SlowOn(u32 aSlowTime, u32 slowcnt);
extern void dds3Stop(u32 frame);
extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);
extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, dds3ProcessFunc_t drawExit,
                                         dds3ProcessFunc_t deleteExit, void *p);
extern s32 evtGetAgeOfMoon16(void);
extern void evtSetAgeOfMoon(s32 age);
extern void effBTLSkillEnd(void);
extern void effBTLSkillStart(s32 id, s32 stat);
extern void effBTLSystemDelete(struct effBTLSysHandle_t *hEffBTLSys);
extern void effBTLSystemDraw(struct effBTLSysHandle_t *hEffBTLSys);
extern struct effBTLSysHandle_t *effBTLSystemCreate(nbFormation_t *pFormation, s32 id);
extern void nbAddHpMp(s32 formindex, s32 type, s32 n);
extern void nbDispJyokyoHelp(s32 msgid, s32 no, s32 var_id, s32 var_ind);
extern void nbDispSkillName(s32 id);
extern void nbFormDispOff(s32 formindex);
extern void nbFormDispOn(s32 formindex);
extern nbMainProcessData_t *nbGetMainProcessData(void);
extern datUnitWork_t *nbGetUnitWorkFromFormindex(s32 formindex);
extern void nbPanelAnalyzeDraw(datUnitWork_t *pUnitWork);
extern void nbPanelDraw(s32 flag);
extern void nbReturnUnit(s32 partyno, s32 lost, s32 dead);
extern void nbRunKoukaType1(s32 partyindex, sdfVector4_t *pos, f32 time);
extern void nbRunKoukaType2(s32 partyindex, f32 time);
extern void nbSetDamage(nbFormation_t *form, f32 range, s32 week);
extern void nbSetDamage2(nbFormation_t *form, f32 range);
extern void nbSetBadstatMotion(nbFormation_t *form, s32 type, f32 range);
extern void nbSetBadstatMotion2(nbFormation_t *form);
extern void nbSetUnitMotion(nbFormation_t *form, s32 motion2, s32 mode, f32 ritu);
extern void nbSetWaitMotion(nbFormation_t *form, s32 motion, f32 ritu);
extern void nbSetBadStat(s32 formindex, s32 sw, s32 badstat);
extern s32 nbSetHojoKouka(s32 formindex, s32 hojotype, s32 hojopoint, s32 nvirtual);
extern void nbSetSakeKoukaParam(nbFormation_t *form, f32 range, f32 power);

extern void effBTLCutDelete(struct effBTLCutHandle_t *hBTLCutEffect);
extern void effBTLDelete(void *hBTLEffect);
extern void effBTLSystemCut1Delete(struct effBTLCutHandle_t *hBTLCutEffect);
extern void effBTLSystemDelete2(struct effBTLSysHandle_t *hEffBTLSys);
extern void effPCPBossBUNSINDelete(void *pPCPBossBUNSIN);
extern void effPCPBossBUNSIN2Delete(void *pPCPBossBUNSIN2);
extern void effPCPBossONGYDelete(void *pPCPBossONGY);
extern void effPCPBossONGY2Delete(void *pPCPBossONGY2);
extern void nbDelKoukaEntry(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);
extern void nbReturnKoukaUnit(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);
extern void effBTLCutDraw(struct effBTLCutHandle_t *hBTLCutEffect);
extern void effBTLSystemCut1Draw(struct effBTLCutHandle_t *hBTLCutEffect);
extern struct effBTLCutHandle_t *effBTLCutCreate(void *hBTLEffect, u16 CutNum, nbFormation_t *pFormation,
                                                 s32 stat);
extern s32 evtSetUnitColorA(evtUnit_t *h, s32 frame, u32 a);
extern s32 evtSetUnitColorRGB(evtUnit_t *h, s32 frame, u32 rgb);
extern void evtSetWaitHokanFrame(evtUnit_t *h, s32 before, s32 smooth);
extern s32 p2sdLoadCheckMidi(s32 MidiNo);
extern void p2sdPlaySE(s32 id, s32 vol, s32 pan);
extern void p2sdStopSe(s32 sound);
extern void datDebugPrintf(const char *fmt, ...);
extern s32 p2sdGetPcmState(void);
extern s32 p2sdLoadPcmFile(char *name, s32 channel);
extern s32 sprintf(char *buf, char *fmt, ...);
extern s32 printf(char *fmt, ...);
extern char D_00551138[];
extern void dds3FadeIn2(s32 frame);
extern void dds3FadeOut(s32 r, s32 g, s32 b, s32 frame);
extern u32 effBTLCutGetFrame(struct effBTLCutHandle_t *hBTLCutEffect);
extern void effBTLCutSetCutEndFrame(struct effBTLCutHandle_t *hBTLCutEffect, s32 frame);
extern void effPCPBossBUNSINDraw(void *pPCPBossBUNSIN);
extern void effPCPBossBUNSIN2Draw(void *pPCPBossBUNSIN2);
extern void effPCPBossONGYDraw(void *pPCPBossONGY);
extern void effPCPBossONGY2Draw(void *pPCPBossONGY2);
extern void nbClipKoukaNum(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);
extern void nbDrawKoukaNum(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);
extern void RunFlyKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);
extern void RunNumEffectKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);
extern void RunStockPanelEndKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);
extern void RunStockPanelFadeOutKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);
extern void RunUnitColKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);
extern void nbAddOrder(s32 findex);
extern void nbSetEnemyFormation(nbMainProcessData_t *data, s32 group);
extern void nbSetPlayerFormation(nbMainProcessData_t *data, s32 empty);
extern void nbSummonUnit(s32 side, s32 stockno, s32 formindex);
extern void nbUpdateOrder(void);
extern nbKoukaProcessData_t *InitKoukaProcessData(nbMainProcessData_t *data);
extern void nbLinkPacket(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);
extern void DelKoukaPacket(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);
extern void nbDelKoukaPacket(nbKoukaPacket_t *p);
extern s32 nbKoukaProcess1(dds3ProcessID_t *id);
extern s32 nbKoukaShutdown(dds3ProcessID_t *id);

nbKoukaProcessData_t *nbGetKoukaProcessData(void)
{
    return dds3GetProcessData(dds3SearchProcessName("nb_kouka"));
}

s32 nbGetKoukaPacketCnt(void)
{
    nbKoukaProcessData_t *work = nbGetKoukaProcessData();

    return work->packetcnt;
}

s32 nbGetUniqueIdKoukaPacketCnt(s32 uniqueid)
{
    nbKoukaPacket_t *p = nbGetKoukaProcessData()->first;
    s32 cnt = 0;

    while (p != NULL) {
        s32 id = p->uniqueid;

        p = p->next;
        if (id == uniqueid) {
            cnt++;
        }
    }
    return cnt;
}

s32 nbCountKoukaByType(s32 type)
{
    nbKoukaPacket_t *p = nbGetKoukaProcessData()->first;
    s32 cnt = 0;

    while (p != NULL) {
        s32 t = p->type;

        p = p->next;
        if (t == type) {
            cnt++;
        }
    }
    return cnt;
}

void nbLinkPacket(nbKoukaProcessData_t *k, nbKoukaPacket_t *p)
{
    if (k->first == NULL) {
        k->first = p;
    }
    if (k->last == NULL) {
        k->last = p;
    } else {
        k->last->next = p;
        p->prev = k->last;
        k->last = p;
    }
    k->packetcnt++;
}

void nbDelKoukaEntry(nbKoukaProcessData_t *k, nbKoukaPacket_t *p)
{
    if (p->prev == NULL) {
        k->first = p->next;
    } else {
        p->prev->next = p->next;
    }
    if (p->next == NULL) {
        k->last = p->prev;
    } else {
        p->next->prev = p->prev;
    }
    k->packetcnt--;
}

nbKoukaPacket_t *nbAddKoukaPacket(nbKoukaProcessData_t *k, s32 startframe, s32 framelength,
                                  s32 uniqueid)
{
    nbKoukaPacket_t *p = sdfChipAllocDirect(sizeof(nbKoukaPacket_t));

    p->startframe = startframe;
    p->framelength = framelength;
    p->uniqueid = uniqueid;
    p->partyno = -1;
    p->type = 0;
    p->flag = 0;
    p->time = 0;
    p->next = NULL;
    p->prev = NULL;
    p->form = NULL;
    p->unit = NULL;
    p->party = NULL;
    p->cuteff = NULL;
    p->data1.i = 0;
    p->data2.i = 0;
    nbLinkPacket(k, p);
    return p;
}

void DelKoukaPacket(nbKoukaProcessData_t *k, nbKoukaPacket_t *p)
{
    switch (p->type) {
    case BOSSONGBUN:
        effPCPBossBUNSINDelete(p->data1.p);
        break;
    case BOSSONGBUN2:
        effPCPBossBUNSIN2Delete(p->data1.p);
        break;
    case BOSSONGOUT:
        effPCPBossONGYDelete(p->data1.p);
        break;
    case BOSSONGOUT2:
        effPCPBossONGY2Delete(p->data1.p);
        break;
    case SKILLEFF:
    case SKILLEFF2:
    case SKILLEFF3:
        if (p->cuteff != NULL) {
            effBTLCutDelete(p->cuteff);
            p->cuteff = NULL;
        }
        if (p->type == SKILLEFF3) {
            effBTLDelete(p->data1.p);
        }
        break;
    case SKILLEFF1:
        if (p->cuteff != NULL) {
            effBTLSystemCut1Delete(p->cuteff);
            p->cuteff = NULL;
        }
        break;
    case HITEFF:
        switch (p->data1.i) {
        case SKILLEFF:
        case TARMOVE:
        case HOMEMOVE:
        case HOMEROT:
        case UNITCOL:
        case UNITHOMECOL:
        case UNITA:
        case UNITHOMEA:
        case DMGMOT:
        case BADDMGMOT:
            effBTLSystemDelete2(p->data2.p);
            break;
        case ROT:
            break;
        }
        break;
    case HATUDOEFF_END:
        break;
    }
    nbDelKoukaEntry(k, p);
    sdfChipFreeDirect(p);
}

void nbDelKoukaPacket(nbKoukaPacket_t *p)
{
    DelKoukaPacket(nbGetKoukaProcessData(), p);
}

void nbDelAllUniqueIDKoukaPacket(s32 uniqueid)
{
    dds3ProcessID_t *id = nbGetMainProcessData()->KoukaProcessID;
    nbKoukaProcessData_t *work;
    nbKoukaPacket_t *p;
    nbKoukaPacket_t *next;

    if (id == NULL) {
        return;
    }
    work = dds3GetProcessData(id);
    p = work->first;
    while (p != NULL) {
        next = p->next;
        if (p->uniqueid != uniqueid) {
            p = next;
            continue;
        }
        nbDelKoukaPacket(p);
        p = next;
    }
}

void nbDelAllKoukaPacketType(s32 type)
{
    dds3ProcessID_t *id = nbGetMainProcessData()->KoukaProcessID;
    nbKoukaProcessData_t *work;
    nbKoukaPacket_t *p;
    nbKoukaPacket_t *next;

    if (id == NULL) {
        return;
    }
    work = dds3GetProcessData(id);
    p = work->first;
    while (p != NULL) {
        next = p->next;
        if (p->type != type) {
            p = next;
            continue;
        }
        nbDelKoukaPacket(p);
        p = next;
    }
}

void nbDelAllMojiNumEffectKoukaPacket(nbFormation_t *form, s32 motno)
{
    dds3ProcessID_t *id = nbGetMainProcessData()->KoukaProcessID;
    nbKoukaProcessData_t *work;
    nbKoukaPacket_t *p;
    nbKoukaPacket_t *next;

    if (id == NULL) {
        return;
    }
    work = dds3GetProcessData(id);
    p = work->first;
    while (p != NULL) {
        next = p->next;
        if (p->type != NUMEFF && p->type != MOJIEFF) {
            p = next;
            continue;
        }
        if (p->form != form) {
            p = next;
            continue;
        }
        nbDelKoukaPacket(p);
        p = next;
    }
}

void nbDelUnEventKoukaPacket(void)
{
    dds3ProcessID_t *id = nbGetMainProcessData()->KoukaProcessID;
    nbKoukaProcessData_t *work;
    nbKoukaPacket_t *p;
    nbKoukaPacket_t *next;

    if (id == NULL) {
        return;
    }
    work = dds3GetProcessData(id);
    p = work->first;
    if (p == NULL) {
        return;
    }
    do {
        next = p->next;
        switch (p->type) {
        case SKILLCOLOFF:
            effBTLSkillEnd();
        case SE:
        case SE2:
        case SELOCK:
        case DEADPCM:
        case FADEOUT:
        case FADEIN:
            p = next;
            continue;
        case NB_KOUKA_TYPE_RETURN:
            nbReturnKoukaUnit(work, p);
        default:
            nbDelKoukaPacket(p);
            break;
        }
        p = next;
    } while (p != NULL);
}

void nbOnOffKoukaPacketFlag(s32 type, u32 flg, s32 sw)
{
    dds3ProcessID_t *id = nbGetMainProcessData()->KoukaProcessID;
    nbKoukaProcessData_t *work;
    nbKoukaPacket_t *p;
    nbKoukaPacket_t *next;

    if (id == NULL) {
        return;
    }
    work = dds3GetProcessData(id);
    p = work->first;
    while (p != NULL) {
        next = p->next;
        if (p->type != type) {
            p = next;
            continue;
        }
        if (sw != 0) {
            p->flag |= flg;
        } else {
            p->flag &= ~flg;
        }
        p = next;
    }
}

s32 nbCheckSePacket(s32 start, s32 se, s32 frame)
{
    nbKoukaProcessData_t *work = dds3GetProcessData(nbGetMainProcessData()->KoukaProcessID);
    nbKoukaPacket_t *p = work->first;

    while (p != NULL) {
        if (start >= p->startframe && start - frame < p->startframe) {
            if (p->type == SE) {
                goto check;
            }
            if (p->type >= 0x19) {
                if (p->type < 0x1E) {
                    if (p->type >= 0x1C) {
                    check:
                        if (p->data1.i == se) {
                            return 1;
                        }
                    }
                }
            }
        }
        p = p->next;
    }
    return 0;
}

void RunBossEffectKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p)
{
    s32 type = p->type;

    switch (type) {
    case BOSSONGBUN:
        effPCPBossBUNSINDraw(p->data1.p);
        break;
    case BOSSONGBUN2:
        effPCPBossBUNSIN2Draw(p->data1.p);
        break;
    case BOSSONGOUT:
        effPCPBossONGYDraw(p->data1.p);
        break;
    case BOSSONGOUT2:
        effPCPBossONGY2Draw(p->data1.p);
        type = 0;
        break;
    }
}

void RunHatudoEffectEndKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p)
{
    nbKoukaPacket_t *cur = k->first;

    while (cur != NULL) {
        if (cur->uniqueid == p->uniqueid && cur->type == SKILLEFF1 && cur->framelength >= 0xD) {
            u32 frame;

            cur->framelength = 0xC;
            frame = effBTLCutGetFrame(cur->cuteff);
            effBTLCutSetCutEndFrame(cur->cuteff, frame + 0xC);
        }
        cur = cur->next;
    }
}

void RunSkillEffectKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p)
{
    if (p->cuteff == NULL) {
        printf("cutcreate 3\n");
        p->cuteff = effBTLCutCreate(p->data1.p, p->data2.us[0], p->form, p->data2.s[1]);
        return;
    }
    switch (p->type) {
    case SKILLEFF:
    case SKILLEFF2:
    case SKILLEFF3:
        effBTLCutDraw(p->cuteff);
        break;
    case SKILLEFF1:
        effBTLSystemCut1Draw(p->cuteff);
        break;
    }
}

void nbRunKoukaCase1(nbKoukaProcessData_t *k, nbKoukaPacket_t *p)
{
    s32 partyno = p->partyno;
    nbParty_t *party = (nbParty_t *)((u8 *)(partyno * 0x80) + (u32)k->data->party);

    if ((party->flag & 1) == 0) {
        return;
    }
    switch (p->type) {
    case TARMOVE:
        nbRunKoukaType1(partyno, &p->v, p->data1.f);
        break;
    case HOMEMOVE:
        nbRunKoukaType2(partyno, p->data1.f);
        break;
    }
}

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", RunNumEffectKouka);

void RunUnitColKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p)
{
    nbFormation_t *form = p->form;
    u32 stat = form->stat;
    u32 color = 0;
    s32 frame = 0;
    s32 type;

    if ((stat & 1) == 0) {
        return;
    }
    if ((stat & 2) == 0) {
        return;
    }
    switch (p->type) {
    case UNITCOL:
    case UNITA:
        color = p->data2.i;
        frame = p->data1.i;
        break;
    case UNITHOMECOL:
    case UNITHOMEA:
        color = form->modelcol;
        frame = p->data1.i;
        break;
    }
    type = p->type;
    if (type < 5) {
        return;
    }
    if (!(type < 7)) {
        if (type < 9) {
            goto seta;
        }
        return;
    }
    evtSetUnitColorRGB(form->hunit, frame, color);
    goto done;
seta:
    evtSetUnitColorA(form->hunit, frame, color);
done:
    ;
}

void RunMotionStopKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p)
{
    nbFormation_t *form = p->form;
    u32 stat = form->stat;
    evtUnit_t *hunit;
    mdlObj_t *obj;

    if ((stat & 1) == 0) {
        return;
    }
    if ((stat & 2) == 0) {
        return;
    }
    hunit = form->hunit;
    obj = hunit->owner;
    switch (p->type) {
    case MOTIONSTOP:
        obj->first->speed = 0.0f;
        hunit->flags |= 0x400000;
        break;
    case MOTIONSTOPCLS:
        obj->first->speed = form->motspeed;
        hunit->flags &= ~0x400000;
        hunit->flags &= ~0x40;
        hunit->flags |= 0x20;
        break;
    }
}

void RunMotionKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p)
{
    s32 mot;

    switch (p->type) {
    case MOTION:
        if (p->data1.i == 0x17) {
            nbSetUnitMotion(p->form, 0x17, 2, 1.0f);
        } else {
            nbSetUnitMotion(p->form, p->data1.i, 0, 1.0f);
        }
        break;
    case SKILLMOTION:
        mot = p->data1.s[1];
        switch (mot) {
        case SKILLEFF:
            nbSetUnitMotion(p->form, p->data1.s[0], 0, p->data2.f);
            break;
        case TARMOVE:
            nbSetUnitMotion(p->form, p->data1.s[0], 3, p->data2.f);
            break;
        case HOMEMOVE:
        case ROT:
            nbSetUnitMotion(p->form, p->data1.s[0], 3, p->data2.f);
            evtSetWaitHokanFrame(p->form->hunit, 0, 0);
            mot = 0;
            break;
        }
        break;
    }
}

void RunDamageMotionKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p)
{
    switch (p->type) {
    case DMGMOT:
        nbSetDamage(p->form, p->data1.f, p->data2.i);
        break;
    case EASYDMGMOT:
        nbSetDamage2(p->form, p->data1.f);
        break;
    case BADDMGMOT:
    case BADMOT: {
        s32 bit = p->data2.i;

        switch (bit) {
        case TARMOVE:
        case HOMEMOVE:
        case NUMEFF:
        case 0x400:
            if (p->type == BADDMGMOT) {
                nbSetBadstatMotion(p->form, p->data2.i, p->data1.f);
            } else {
                nbSetBadstatMotion2(p->form);
            }
            break;
        case HOMEROT:
        case UNITHOMEA:
        case MOTIONSTOP:
        case 0x40:
        case 0x80:
        case 0x200: {
            nbFormation_t *form;

            if (p->type == BADDMGMOT) {
                nbSetDamage(p->form, p->data1.f, 1);
                form = p->form;
                if ((form->anime & 0x80) != 0) {
                    form->hunit->owner->first->speed = form->motspeed;
                }
            } else {
                form = p->form;
            }
            p->form->anime &= ~0x80;
            nbSetWaitMotion(form, 2, 1.0f);
            break;
        }
        case 0x100: {
            nbFormation_t *form;

            if (p->type == BADDMGMOT) {
                nbSetDamage(p->form, p->data1.f, 1);
                form = p->form;
                if ((form->anime & 0x80) != 0) {
                    form->hunit->owner->first->speed = form->motspeed;
                }
            } else {
                form = p->form;
            }
            p->form->anime &= ~0x80;
            nbSetWaitMotion(form, 2, 0.5f);
            break;
        }
        case -1: {
            nbFormation_t *form;

            nbSetDamage2(p->form, p->data1.f);
            nbSetUnitMotion(p->form, 2, 0, 1.0f);
            form = p->form;
            form->anime &= ~0x80;
            nbSetWaitMotion(form, 2, 1.0f);
            evtSetWaitHokanFrame(p->form->hunit, 0, 0);
            break;
        }
        default:
            nbSetBadstatMotion(p->form, p->data2.i, p->data1.f);
            bit = 0;
            break;
        }
        break;
    }
    }
}

void nbReturnKoukaUnit(nbKoukaProcessData_t *k, nbKoukaPacket_t *p)
{
    nbReturnUnit(p->party->partyindex, 0, p->data1.i);
    if (k->data->encno == 0x10F) {
        k->data->stat |= 0x2000;
    }
}

void RunSummonKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p)
{
    nbUpdateOrder();
    nbSummonUnit(p->data1.s[0], p->data1.s[1], p->data2.s[0]);
    switch (p->data1.s[0]) {
    case 1:
        nbSetEnemyFormation(k->data, 0);
        if ((k->data->stat & 8) != 0) {
            goto add;
        }
        break;
    case 0:
        nbSetPlayerFormation(k->data, 0);
        if ((k->data->stat & 8) == 0) {
        add:
            nbAddOrder(p->data2.s[0]);
        }
        break;
    }
}

void RunSakeKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p)
{
    nbSetSakeKoukaParam(p->form, p->data1.f, p->data2.f);
}

void RunBadEffKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p)
{
    if ((p->form->anime & 0x100) != 0) {
        if (p->form->badeff != NULL) {
            effBTLSystemDelete(p->form->badeff);
            p->form->badeff = NULL;
            p->form->anime &= ~0x100;
        }
    }
    if (p->data1.i != 0) {
        p->form->badeff = effBTLSystemCreate(p->form, p->data1.i);
        p->form->anime |= 0x100;
    }
}

void RunBadEffKaifukuKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p)
{
    nbFormation_t *form = p->form;

    if ((form->anime & 0x100) != 0) {
        if (form->badeff != NULL) {
            effBTLSystemDelete(form->badeff);
            p->form->badeff = NULL;
            p->form->anime &= ~0x100;
        }
    }
}

void RunHitEffectKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p)
{
    s32 hit = p->data1.i;

    switch (hit) {
    case 0:
    case 1:
    case 2:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
        effBTLSystemDraw(p->data2.p);
        hit = 0;
        break;
    case 3:
        break;
    }
}

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", NumEffectClip);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", nbClipKoukaNum);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", nbDrawKoukaNum);

void nbApplyHpMp(nbKoukaProcessData_t *k, nbKoukaPacket_t *p)
{
    if ((k->data->stat & 0x20000) == 0) {
        nbAddHpMp(p->form->formindex, p->data1.i, p->data2.i);
    }
}

void nbRunKoukaCase12(nbKoukaProcessData_t *k, nbKoukaPacket_t *p)
{
    nbMainProcessData_t *data = k->data;
    u32 stat = data->stat;
    s32 ten = 0;

    if ((stat & 0x10000) != 0) {
        return;
    }
    switch (p->type) {
    case PRESS: {
        f32 v = data->press3 + p->data1.f;

        if (v < 0.0f) {
            v = 0.0f;
        }
        if (1.0f < v) {
            v = 1.0f;
        }
        data->press3 = v;
        break;
    }
    case NEWPRESS: {
        s32 v;
        s16 cur;

        if ((stat & 8) == 0) {
            ten = data->press4_ten;
        }
        cur = (s16)(u16)data->press4_ten;
        v = data->press4_p + p->data1.s[0];
        if (v < 0) {
            v = 0;
        }
        data->press4_p = v;
        v = cur + p->data1.s[1];
        if (v < 0) {
            v = 0;
        }
        data->press4_ten = v;
        if ((stat & 8) == 0) {
            data->unk140 += ten - v;
        }
        break;
    }
    }
}

void nbApplyBadStat(nbKoukaProcessData_t *k, nbKoukaPacket_t *p)
{
    nbSetBadStat(p->form->formindex, p->data1.i, p->data2.i);
}

void RunBadMotKaifukuKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p)
{
    nbFormation_t *form = p->form;

    form->hunit->owner->first->speed = form->motspeed;
    nbSetWaitMotion(form, 0, 1.0f);
    if ((p->form->anime & 0x80) == 0) {
        nbSetUnitMotion(p->form, 0, 1, 1.0f);
    } else {
        nbSetUnitMotion(p->form, 0, 1, 1.0f);
    }
    p->form->anime &= ~0x80;
    p->form->type = 0;
    p->form->time = 0;
}

void RunSeKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p)
{
    s32 se = p->data1.i;
    s32 ok = p2sdLoadCheckMidi(se);

    if (k->data->d31_rebehansya != 0 && (se == 0x10008 || se == 0x10009)) {
        k->data->d31_rebehansya = 0;
        p2sdStopSe(0x1C070000);
    }
    if (*(s32 *)k->data->unk148 != 0 && se == 0x10008) {
        *(s32 *)k->data->unk148 = 0;
        p2sdStopSe(0x1C740000);
    }
    switch (p->type) {
    case SE:
        p2sdPlaySE(se, 0x7F, 0x3F);
        break;
    case SE2:
        if (ok != 0) {
            datDebugPrintf("se2 [%08X] play!\n", se);
            p2sdPlaySE(se, 0x7F, 0x3F);
        } else {
            datDebugPrintf("se2 [%08X] chk err\n", se);
        }
        break;
    case SELOCK:
        if (ok != 0) {
            p2sdPlaySE(se, 0x7F, 0x3F);
        } else {
            p->framelength = 1;
        }
        break;
    }
}

void RunPcmKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p)
{
    char name[32];

    if (p2sdGetPcmState() != 0) {
        return;
    }
    if ((k->data->flag & 1) != 0) {
        return;
    }
    if (p->type != DEADPCM) {
        return;
    }
    sprintf(name, "MDD_%03X.ADB", p->data1.i);
    printf(D_00551138, name);
    p2sdLoadPcmFile(name, 0x34);
    k->data->flag |= 1;
}

void RunHitStopKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p)
{
    u32 time = p->time;

    switch (time) {
    case 0:
        dds3Stop(0xC);
        break;
    case 1:
        dds3SlowOn(3, 0x18);
        time = 0;
        break;
    }
}

void RunAnalyzeKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p)
{
    datUnitWork_t *unit = nbGetUnitWorkFromFormindex(p->form->formindex);

    if (unit != NULL) {
        nbPanelAnalyzeDraw(unit);
        nbPanelDraw(0);
    }
}

void RunHelpKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p)
{
    s32 type = p->type;

    if (type == 0x23) {
        nbDispJyokyoHelp(k->data->mesmngid, p->data1.s[0], p->data1.s[1], p->data2.i);
        type = 0;
    }
}

void RunFormOnOffKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p)
{
    nbFormation_t *form = p->form;

    if (form == NULL) {
        return;
    }
    if ((form->stat & 1) == 0) {
        return;
    }
    if (p->data1.i != 0) {
        nbFormDispOn(form->formindex);
    } else {
        nbFormDispOff(form->formindex);
    }
}

void RunFormFlagOnOffKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p)
{
    nbFormation_t *form = p->form;
    u32 stat;

    if (form == NULL) {
        return;
    }
    stat = form->stat;
    if ((stat & 1) == 0) {
        return;
    }
    if (p->data1.i != 0) {
        form->stat = stat | p->data2.i;
    } else {
        form->stat = stat & ~p->data2.i;
    }
}

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", RunFlyKouka);

void RunDeadEventKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p)
{
    k->data->actionstackp = 0;
    k->data->eventno = p->data1.i;
}

void RunFadeKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p)
{
    s32 type = p->type;

    switch (type) {
    case FADEOUT:
        switch (p->data2.i) {
        case SKILLEFF:
            dds3FadeOut(0, 0, 0, p->data1.i);
            break;
        case TARMOVE:
            dds3FadeOut(0xFF, 0xFF, 0xFF, p->data1.i);
            break;
        }
        break;
    case FADEIN:
        dds3FadeIn2(p->data1.i);
        type = 0;
        break;
    }
}

void RunSkillNameKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p)
{
    nbDispSkillName(p->data1.i);
}

void RunHojoKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p)
{
    nbSetHojoKouka(p->form->formindex, p->data1.i, p->data2.i, 0);
}

void RunFlareChargeKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p)
{
    s32 age = evtGetAgeOfMoon16();

    evtSetAgeOfMoon((age + 2) % 16);
}

void RunPadShockKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p)
{
    dds3PadVibrationSet(p->data1.s[0], p->data1.uc[2], p->data2.s[0]);
}

void RunSkillColorKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p)
{
    s32 type = p->type;

    switch (type) {
    case SKILLCOLON:
        effBTLSkillStart(p->data1.i, p->data2.i);
        break;
    case SKILLCOLOFF:
        effBTLSkillEnd();
        type = 0;
        break;
    }
}

void RunStockPanelFadeOutKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p)
{
    nbPanelStockFadeOUT();
}

void RunStockPanelEndKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p)
{
    nbPanelStockEnd();
}

s32 nbKoukaProcess1(dds3ProcessID_t *id)
{
    nbKoukaProcessData_t *work = dds3GetProcessData(id);
    nbKoukaPacket_t *p = work->first;
    nbKoukaPacket_t *next;

    while (p != NULL) {
        next = p->next;
        switch (p->type) {
        case SKILLEFF:
        case UNITCOL:
        case UNITHOMECOL:
        case UNITA:
        case UNITHOMEA:
        case DMGMOT:
        case BADDMGMOT:
        case NB_KOUKA_TYPE_SUMMON:
        case SAKE:
        case BADMOTKAIFUKU:
        case SKILLEFF2:
        case SKILLEFF1:
        case MOTION:
        case MOTIONSTOP:
        case MOTIONSTOPCLS:
        case BADMOT:
        case FORMONOFF:
        case SKILLEFF3:
        case NB_KOUKA_TYPE_FLY:
        case HOJO:
        case EASYDMGMOT:
        case SKILLMOTION:
            if (p->form != NULL && (p->form->flag & 7) != 0) {
                p = next;
                continue;
            }
            break;
        case HATUDOEFF_END:
            break;
        }
        if (p->startframe > 0) {
            p->startframe--;
            p = next;
            continue;
        }
        if (p->framelength < 0) {
            goto del;
        }
        if ((p->flag & 2) != 0) {
            p = next;
            continue;
        }
        switch (p->type) {
        case BOSSONGBUN:
        case BOSSONGBUN2:
        case BOSSONGOUT:
        case BOSSONGOUT2:
            RunBossEffectKouka(work, p);
            break;
        case SKILLEFF:
        case SKILLEFF2:
        case SKILLEFF1:
        case SKILLEFF3:
            RunSkillEffectKouka(work, p);
            break;
        case HATUDOEFF_END:
            RunHatudoEffectEndKouka(work, p);
            break;
        case TARMOVE:
        case HOMEMOVE:
            nbRunKoukaCase1(work, p);
            break;
        case ROT:
        case HOMEROT:
            RunNumEffectKouka(work, p);
            break;
        case UNITCOL:
        case UNITHOMECOL:
        case UNITA:
        case UNITHOMEA:
            RunUnitColKouka(work, p);
            break;
        case MOTION:
        case SKILLMOTION:
            RunMotionKouka(work, p);
            break;
        case MOTIONSTOP:
        case MOTIONSTOPCLS:
            RunMotionStopKouka(work, p);
            break;
        case DMGMOT:
        case BADDMGMOT:
        case BADMOT:
        case EASYDMGMOT:
            RunDamageMotionKouka(work, p);
            break;
        case NB_KOUKA_TYPE_RETURN:
            nbReturnKoukaUnit(work, p);
            break;
        case NB_KOUKA_TYPE_SUMMON:
            RunSummonKouka(work, p);
            break;
        case SAKE:
            RunSakeKouka(work, p);
            break;
        case SKILLCOLOFF:
        case SKILLCOLON:
            RunSkillColorKouka(work, p);
            break;
        case HITEFF:
            RunHitEffectKouka(work, p);
            break;
        case NUMEFF:
            nbClipKoukaNum(work, p);
            break;
        case MOJIEFF:
            nbDrawKoukaNum(work, p);
            break;
        case HPMP:
            nbApplyHpMp(work, p);
            break;
        case PRESS:
        case NEWPRESS:
            nbRunKoukaCase12(work, p);
            break;
        case BADSTAT:
            nbApplyBadStat(work, p);
            break;
        case BADMOTKAIFUKU:
            RunBadMotKaifukuKouka(work, p);
            break;
        case BADEFF:
            RunBadEffKouka(work, p);
            break;
        case BADEFFKAIFUKU:
            RunBadEffKaifukuKouka(work, p);
            break;
        case SE:
        case SE2:
        case SELOCK:
            RunSeKouka(work, p);
            break;
        case DEADPCM:
            RunPcmKouka(work, p);
            break;
        case HITSTOP:
            RunHitStopKouka(work, p);
            break;
        case ANALYZE:
            RunAnalyzeKouka(work, p);
            break;
        case JYOKYOHELP:
            RunHelpKouka(work, p);
            break;
        case FORMONOFF:
            RunFormOnOffKouka(work, p);
            break;
        case FORMFLAGONOFF:
            RunFormFlagOnOffKouka(work, p);
            break;
        case NB_KOUKA_TYPE_FLY:
            RunFlyKouka(work, p);
            break;
        case DEADEVENT:
            RunDeadEventKouka(work, p);
            break;
        case FADEOUT:
        case FADEIN:
            RunFadeKouka(work, p);
            break;
        case SKILLNAME:
            RunSkillNameKouka(work, p);
            break;
        case HOJO:
            RunHojoKouka(work, p);
            break;
        case FLARECHARGE:
            RunFlareChargeKouka(work, p);
            break;
        case PADSHOCK:
            RunPadShockKouka(work, p);
            break;
        case STKPNL_FOUT:
            RunStockPanelFadeOutKouka(work, p);
            break;
        case STKPNL_END:
            RunStockPanelEndKouka(work, p);
            break;
        }
        if ((p->flag & 1) == 0) {
            p->framelength--;
        }
        p->time++;
        p = next;
        continue;
    del:
        nbDelKoukaPacket(p);
        p = next;
    }
    return 0;
}

s32 nbKoukaShutdown(dds3ProcessID_t *id)
{
    nbKoukaProcessData_t *work = dds3GetProcessData(id);

    while (work->first != NULL) {
        DelKoukaPacket(work, work->first);
    }
    work->data->KoukaProcessID = NULL;
    sdfChipFreeDirect(work);
}

nbKoukaProcessData_t *InitKoukaProcessData(nbMainProcessData_t *data)
{
    nbKoukaProcessData_t *work = sdfChipAllocDirect(sizeof(nbKoukaProcessData_t));

    work->data = data;
    work->packetcnt = 0;
    work->first = NULL;
    work->last = NULL;
    return work;
}

void nbInitKoukaProcess(nbMainProcessData_t *data)
{
    nbKoukaProcessData_t *work = InitKoukaProcessData(data);
    dds3ProcessID_t *id;

    id = dds3StartProcess("nb_kouka", 0x2B08, 1, 1, nbKoukaProcess1, nbKoukaShutdown, work);
    dds3LinkProcess(data->MainProcessID, id);
    data->KoukaProcessID = id;
}

INCLUDE_SDATA(const s32, "newbattle/nbKoukaProcess", D_00551138);

