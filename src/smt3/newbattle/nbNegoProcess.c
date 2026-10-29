#include "common.h"
#include "libsdf.h"
#include "basic.h"
#include "scr.h"
#include "ps2asm.h"
#include "newdata.h"

typedef struct evtUnitHandle_t
{
    u8 unk_00[0x8C];
    dds3Basic_t *basic;
    u8 unk_90[0x14];
    u32 flags;
} evtUnitHandle_t;

typedef struct effBTLSysHandle_t effBTLSysHandle_t;

typedef struct NB_FILEHANDLE_T NB_FILEHANDLE_T;

typedef struct
{
    sdfVector4_t orgpos;
    sdfVector4_t orgrot;
    sdfVector4_t pos;
    sdfVector4_t rot;
    sdfVector4_t center;
    f32 r;
    f32 h;
    f32 cr;
    f32 scl;
    f32 oneshotr;
    f32 allshotr;
    f32 movey;
    f32 movez;
    s32 type;
    u32 time;
    u32 stat;
    u32 flag;
    u32 anime;
    dds3Basic_t *basic;
    s32 serial;
    evtUnitHandle_t *hunit;
    s16 partyindex;
    s16 formindex;
    u32 autotime;
    effBTLSysHandle_t *badeff;
    u32 modelcol;
    f32 motspeed;
    s16 motendtype;
    s16 motenddata;
    s16 nexthokan;
    s16 nexthokanb;
    s16 waitmot;
    s16 motcate;
    s32 reserve4;
    NB_FILEHANDLE_T *packh;
    char *akey;
    s32 read_minor;
    u32 tartime;
    f32 range;
    f32 addmovet;
    f32 movet;
    sdfVector4_t tarpos;
    sdfVector4_t startpos;
} nbFormation_t;

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

/* A loaded pac file; only the chain down to the decoded buffer is named. */
typedef struct { u8 unk_00[0x8]; void *buf; } nbPacEntry_t;

typedef struct { u8 unk_00[0x60]; nbPacEntry_t *entry; } nbPacHandle_t;

/* 0xE0 bytes; only the field this unit reads is named. */
typedef struct
{
    s16 id;
    u16 unk_02;
    s32 refcnt;
    void *file[26];
    void *buf[26];
    void *motfile;
    void *motbuf;
} nbStatWork_t;

typedef struct effBTLCutHandle_t effBTLCutHandle_t;

/* Only the fields this unit touches are named; the rest of the layout is unconfirmed. */
typedef struct nbMainProcessData_t
{
    nbFormation_t *form;
    u8 unk_04[0xC];
    nbParty_t *party;
    u8 unk_14[0x4];
    s16 playerpcnt;
    s16 enemypcnt;
    u8 unk_1C[0x4];
    nbStatWork_t *statwork;
    void *statbuf;
    s32 unk_28;
    u8 unk_2C[0x8];
    s32 encno;
    u8 unk_38[0x8];
    s32 actionstackp;
    u32 endtype;
    u8 unk_48[0x18];
    u32 stat;
    u8 unk_64[0x4];
    u8 turn;
    u8 unk_69[0xF];
    s16 press4_p;
    s16 pressturn;
    u16 stagemajor;
    u8 unk_7E[0x6];
    s32 unk_84;
    s32 eventno;
    s32 eventrid;
    s16 negopformindex;
    s16 negoeformindex;
    s32 negoresult;
    u8 item[4];
    u8 itemcnt[4];
    u32 maka;
    u32 exp;
    u8 unk_A8[0xC];
    nbPacHandle_t *formpack;
    u8 unk_B8[0x4];
    dds3ProcessID_t *MainProcessID;
    u8 unk_C0[0x1C];
    void *packdata;
    u8 unk_E0[0x14];
    dds3ProcessID_t *koukaproc;
    dds3ProcessID_t *negoproc;
    u8 unk_FC[0x4];
    dds3ProcessID_t *debugproc;
    dds3ProcessID_t *aiproc;
    dds3ProcessID_t *pnlproc;
    dds3ProcessID_t *helpproc;
    dds3ProcessID_t *autocommproc;
    dds3ProcessID_t *encproc;
    dds3ProcessID_t *eventproc;
    u8 unk_11C[0x4];
    void *pnltex[4];
    s32 unk_130;
    s32 unk_134;
    nbPacHandle_t *modelpack;
    void *modelpackbuf;
} nbMainProcessData_t;

typedef struct
{
    u32 flag;
    u8 race;
    u8 level;
    u16 hp;
    u16 maxhp;
    u16 mp;
    u16 maxmp;
    s16 aisyoid;
    s8 param[6];
    u16 skill[8];
    s16 keisyotype;
    u16 keisyoform;
    u16 dropmakka;
    u16 dropexp;
    u8 dropitem[2];
    u8 droppoint[2];
    u16 specialbit;
    u8 specialitem;
    u8 specialpoint;
    u8 hougyokupoint;
    u8 masekipoint;
    s8 attackattr;
    u8 attackcnt;
    u8 attackinterval;
    u8 reserve1;
} datDevilFormat_t;

/* 0x1A4 bytes; only the fields this unit touches are named. */
typedef struct nbActionProcessData_t
{
    nbMainProcessData_t *data;
    u32 stat;
    u8 unk_08[0x1C];
    nbParty_t *party;
    nbFormation_t *form;
    datUnitWork_t *work;
    s32 partyindex;
    s32 type;
    s32 target;
    s32 select;
    u8 unk_40[0x28];
    s32 massuruid;
    u8 unk_6C[0x124];
    u32 aiselect;
    s32 aijokentbl;
    u8 aichktbl[0x8];
    u32 aisummonid;
} nbActionProcessData_t;

typedef struct nbNegoProcessData_t
{
    nbMainProcessData_t *data;
    nbActionProcessData_t *actdata;
    u32 stat;
    s32 negotype;
    s32 waitframe;
    u32 nownegoid;
    s32 nownegophase;
    u32 nextnegoid;
    s32 nextkouhocnt;
    u32 nextkouho[8];
    s32 nowmsgid;
    s32 nowmsgno;
    s32 nowmes;
    s32 msgwndid;
    u32 msgwndcnt;
    s32 nowselect;
    u32 workid[8];
    s32 requesttype;
    s32 requestdata;
    s32 requesthpmp;
    s32 prevreqtype;
    s32 cnt6_36;
    s32 cnt6_37;
    s32 cnt6_58;
    s32 cnt6_66;
    s32 mes6_2;
    s32 mes6_25;
    s32 mes6_67;
    s32 flag1_6;
    s32 flag1_39;
    s32 historyindex;
    u32 history[14];
    s32 historymes[14];
    s32 endid;
    s32 endtype;
    s32 negoreqtype;
    u32 negoskillid;
    s32 aisyo;
    s32 aisyomes;
    s32 hojopartyindex;
    s32 hojoskillid;
    s32 hojoseikou;
    s32 hojoaisyo;
    s32 hojoaisyomes;
    s32 qtype;
    s32 qindex;
    datUnitWork_t *pwork;
    datUnitWork_t *ework;
    s32 pformindex;
    s32 eformindex;
    nbFormation_t *pform;
    nbFormation_t *eform;
    s32 ppartyindex;
    s32 epartyindex;
    nbParty_t *pparty;
    nbParty_t *eparty;
    s32 pkucyo;
    s32 ekucyo;
    s32 innen_id;
    s32 temp_param[4];
    s32 boupartyindex;
    s32 getmakka;
    s32 getitem;
    s32 getinfotype;
    s32 getinfomsg;
    s32 flowmsg[31];
    s32 questionmsg;
    s32 aisyomsg[31];
    s32 infomsg[31];
    s32 helpmsg;
    s32 helpmsg2;
    s32 innenmsg;
    s32 bossmsg;
    s32 selectcur;
} nbNegoProcessData_t;

typedef union
{
    s8 c[4];
    s16 s[2];
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
    nbParty_t *party;
    s32 partyno;
    effBTLCutHandle_t *cuteff;
    nbKoukaData_u data1;
    nbKoukaData_u data2;
    sdfVector4_t v;
} nbKoukaPacket_t;

extern s32 printf(const char *format, ...);

extern u32 dds3GetRandIntA(void *aLimit, u32 name);

extern void itfMesMngSetVar(s32 id, s32 index, s32 num, s32 type);

extern void itfMesMngMsgWndCls(s32 msgid);

extern datDevilFormat_t datDevilTable[];

extern void *dds3GetProcessData(dds3ProcessID_t *id);

extern void itfMesMngRelease(s32 id);
extern void sdfChipFreeDirect(void *ptr);
extern s32 p2sdBgmVolUp(void);

extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);

extern u32 f;

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, void *drawExit,
                                         void *deleteExit, void *p);

extern u8 dds3PadButtonState[];

extern s32 nbGetInfoMsg(s32 infotype);

extern void nbSetMesItem(s32 msgid, s32 it);

typedef struct { u8 rate; u8 item; } nbMasterTextKouho_t;

typedef struct {
    s32 mesno;
    u8 unk_04[0x4];
    u8 phase;
    u8 honboutype;
    u8 unk_0A[0xA];
    u16 negorate;
    nbMasterTextKouho_t itema[4];
    nbMasterTextKouho_t itemb[4];
    nbMasterTextKouho_t stone[4];
    u8 unk_2E[0x2];
} datDevilNegoFormat_t;

extern datDevilNegoFormat_t master_text[];

extern s32 nbGetKureItemA(nbNegoProcessData_t *n, datUnitWork_t *a, datUnitWork_t *b);

/* The nb event script work; only the fields this unit touches are named. */
typedef struct nbEventProcessData_t {
    nbMainProcessData_t *data;
    s32 eventrid;
    s32 time;
    s32 seq;
    s32 seqtime;
    void *userdata;
    void (*unk_18)(dds3ProcessID_t *id);
    void *file;
    nbPacHandle_t *pfile;
    void *mem;
} nbEventProcessData_t;

extern nbKoukaPacket_t *nbAddMotionKoukaPacket(s32 startframe, s32 uniqueid, nbFormation_t *form,
                                               s32 motion);

typedef struct { u16 item; u16 cnt; } nbInnenPhase_t;

/* 60-byte entries; only the fields this unit reads are named. */
typedef struct {
    s16 id;
    u16 pid;
    u16 eid;
    u16 unk_06;
    nbInnenPhase_t phase[13];
} nbInnenRec_t;

extern nbInnenRec_t D_003E3158[];

extern s32 nbSearchHojoSkillWork(nbNegoProcessData_t *n, s32 type);

extern nbKoukaPacket_t *nbAddSePacket2(s32 startframe, s32 uniqueid, s32 se);

extern s32 evtBitCheck(s32 no);

extern u8 datNegoReqTable[];

extern u8 datNegoTuikaReqtype[];

extern nbNegoProcessData_t *InitNegoProcessData(nbActionProcessData_t *actdata, s32 pformindex,
                                                s32 eformindex, s32 negotype, s32 skillid);

extern void nbSetNegoWaitMotion(nbFormation_t *form);

extern void nbSetAllHojoSkillWork(nbNegoProcessData_t *n);

extern void p2sdBgmVolDown(void);

extern void *nbNegoProcess1(dds3ProcessID_t *id);

extern void nbNegoDebugPrint(nbNegoProcessData_t *n);

extern s32 nbCheckNegoMessage(nbNegoProcessData_t *n);

extern char D_00544F10[];

extern void nbNegoShutdown(dds3ProcessID_t *id);

extern char D_00551160[];

extern s32 nbGetNegoKureMakkaA(nbNegoProcessData_t *n, datUnitWork_t *a, datUnitWork_t *b);

extern s32 (*D_00527518[])(nbEventProcessData_t *e);

extern s32 nbGetHojoSkillWorkCnt(nbNegoProcessData_t *n);

extern s32 nbGetQuestionType(datUnitWork_t *w);

extern s32 nbGetNegoYaruMakkaB(nbNegoProcessData_t *n, datUnitWork_t *a, datUnitWork_t *b);

extern u8 datNegoReqItemListA[];

extern u8 datNegoReqItemListB[];

extern u8 D_003E2318[];

extern u8 D_003E23F8[];

extern s32 nbSelectKureItemA(s32 lv_idx);

extern s32 nbSelectKureItemB(s32 lv_idx);

extern s32 (*D_00527420[])(nbEventProcessData_t *e);

extern s32 itfMesMngGetSelPhase(s32 msgid);

extern void itfMesMngSetSelPhase(s32 msgid, s32 phase);

extern s32 itfMesMngGetSelectNo(s32 msgid);

extern void itfMesMngRequestSelectRelease(s32 id);

extern s32 itfMesMngGetPhase(s32 msgid);

extern void itfMesMngSetPhase(s32 msgid, s32 phase);

extern void itfMesMngMarkBitStat(s32 id, s32 flag);

extern void itfMesMngRequestSelect(s32 msg, s32 id, s32 flag);

extern void itfMesMngRequestMessage(s32 msg, s32 id, s32 flag);

extern s32 dds3SelectMenu(s32 pad, s32 listsx, s32 listsy, s32 winsx, s32 winsy,
                          s32 *basex, s32 *basey, s32 *x, s32 *y);

extern s32 (*D_004F0788[])(nbActionProcessData_t *a, s32 joken);

extern s32 (*D_00527688[])(nbEventProcessData_t *e);

extern s32 (*D_00527698[])(nbEventProcessData_t *e);

typedef struct { s16 jouken[3]; u8 pad[0xC6]; } nbInfoSelectEntry_t;

extern nbInfoSelectEntry_t InfoSelectTable[];
extern s32 nbCheckSelectMesJoukenAri(s32 index, s32 limit);

extern void nbAddNegoIdKouho(nbNegoProcessData_t *n, u32 id);

extern void nbDispNegoMessage(nbNegoProcessData_t *n, s32 msgid, s32 msgno, s32 sel);

extern s32 (*nbE902Table[])(nbEventProcessData_t *e);

extern s32 nbSetNegoYesMotion(nbNegoProcessData_t *n, nbFormation_t *form);

extern s32 nbSetNegoNoMotion(nbNegoProcessData_t *n, nbFormation_t *form);

extern s32 (*D_004EF4B0[])(nbNegoProcessData_t *n);

extern s32 (*D_004EF550[])(nbNegoProcessData_t *n);

extern s32 (*D_004EF698[])(nbNegoProcessData_t *n);

extern s32 (*D_004EF6F8[])(nbNegoProcessData_t *n);

extern s32 (*D_004EF780[])(nbNegoProcessData_t *n);

extern s32 (*D_004EF7C8[])(nbNegoProcessData_t *n);

extern s32 (*D_004EF7D8[])(nbNegoProcessData_t *n);

extern s32 (*D_004EF810[])(nbNegoProcessData_t *n);

extern s32 (*D_004EF868[])(nbNegoProcessData_t *n);

extern s32 (*D_004EF8D8[])(nbNegoProcessData_t *n);

extern s32 (*D_004EF928[])(nbNegoProcessData_t *n);

extern s32 (*D_004EF950[])(nbNegoProcessData_t *n);

extern s32 (*D_004EF960[])(nbNegoProcessData_t *n);

extern s32 (*D_004EFA28[])(nbNegoProcessData_t *n);

extern s32 (*D_004EFA60[])(nbNegoProcessData_t *n);

extern s32 (*D_004EFAC0[])(nbNegoProcessData_t *n);

void nbAddNegoIdKouho(nbNegoProcessData_t *n, u32 id)
{
    s32 cnt;

    cnt = n->nextkouhocnt;
    if (cnt < 8) {
        n->nextkouho[cnt] = id;
        n->nextkouhocnt++;
    }
}

u32 nbGetPrevNegoId(nbNegoProcessData_t *n)
{
    return n->history[n->historyindex];
}

u32 nbGetBackNoNegoId(nbNegoProcessData_t *n, s32 a)
{
    return n->history[(n->historyindex - a + 15) % 14];
}

void nbSetMesMakka(s32 msgid, s32 i)
{
    itfMesMngSetVar(msgid, 3, i, 0);
}

void nbSetMesItem(s32 msgid, s32 it)
{
    itfMesMngSetVar(msgid, 2, it, 3);
}

void nbClsNegoMessage(nbNegoProcessData_t *n)
{
    s32 msgwndid;

    msgwndid = n->msgwndid;
    if (msgwndid >= 0) {
        itfMesMngMsgWndCls(msgwndid);
        n->msgwndid = -1;
    }
}

void nbDispNegoMessage(nbNegoProcessData_t *n, s32 msgid, s32 msgno, s32 sel)
{
    n->nowmsgid = msgid;
    n->nowmsgno = msgno;
    n->nowselect = 0;
    if (sel != 0) {
        n->stat |= 4;
    } else {
        n->stat &= ~4;
        n->nowmes = msgno;
    }
}

s32 nbCheckNegoMessage(nbNegoProcessData_t *n)
{
    s32 ret;

    if (n->nowmsgid < 0) {
        n->nowmsgid = -1;
        return 1;
    }
    ret = 0;
    if (n->stat & 4) {
        if (itfMesMngGetSelPhase(n->nowmsgid) < 0) {
            itfMesMngSetSelPhase(n->nowmsgid, 0);
            ret = 1;
            n->nowselect = itfMesMngGetSelectNo(n->nowmsgid);
            itfMesMngRequestSelectRelease(n->nowmsgid);
            n->nowmsgid = -1;
        }
    } else {
        if (itfMesMngGetPhase(n->nowmsgid) < 0) {
            itfMesMngSetPhase(n->nowmsgid, 0);
            ret = 1;
            n->nowmsgid = -1;
        }
    }
    return ret;
}

void nbSetNegoMesVar(s32 msgid, datUnitWork_t *p, datUnitWork_t *e)
{
    itfMesMngSetVar(msgid, 0, e->id, 1);
    itfMesMngSetVar(msgid, 4, datDevilTable[e->id].race, 8);
    if (p->flag & 4) {
        itfMesMngSetVar(msgid, 1, 0, 0xA);
        itfMesMngSetVar(msgid, 5, datDevilTable[0].race, 8);
    } else {
        itfMesMngSetVar(msgid, 1, p->id, 1);
        itfMesMngSetVar(msgid, 5, datDevilTable[p->id].race, 8);
    }
}

s32 nbGetNegoYaruMakkaA(nbNegoProcessData_t *n, datUnitWork_t *a, datUnitWork_t *b)
{
    s32 maka;

    maka = (datDevilTable[b->id].dropmakka * ((s32)dds3GetRandIntA(0, 4) + 5) + 5) / 10 * 10;
    return maka != 0 ? maka : 1;
}

s32 nbGetNegoYaruMakkaB(nbNegoProcessData_t *n, datUnitWork_t *a, datUnitWork_t *b)
{
    s32 maka;

    maka = nbGetNegoKureMakkaA(n, a, b);
    maka = (maka * ((s32)dds3GetRandIntA(0, 30) + 40) / 100 + 5) / 10 * 10;
    return maka != 0 ? maka : 1;
}

s32 nbGetNegoKureMakkaA(nbNegoProcessData_t *n, datUnitWork_t *a, datUnitWork_t *b)
{
    s32 maka;

    maka = (s32)((f32)(b->level * ((s32)dds3GetRandIntA(0, 10) + 21)) * 0.8f);
    maka = (maka + 5) / 10 * 10;
    return maka != 0 ? maka : 1;
}

s32 nbGetNegoKureMakkaB(nbNegoProcessData_t *n, datUnitWork_t *a, datUnitWork_t *b)
{
    s32 maka;

    maka = nbGetNegoKureMakkaA(n, a, b);
    maka = (maka * ((s32)dds3GetRandIntA(0, 20) + 50) / 100 + 5) / 10 * 10;
    return maka != 0 ? maka : 1;
}

s32 nbGetNegoKureHpA(nbNegoProcessData_t *n, datUnitWork_t *a, datUnitWork_t *b)
{
    if (evtBitCheck(8) != 0) {
        return b->maxhp * ((s32)dds3GetRandIntA(0, 10) + 20) / 100;
    }
    return b->maxhp * ((s32)dds3GetRandIntA(0, 8) + 14) / 100;
}

s32 nbGetNegoKureHpB(nbNegoProcessData_t *n, datUnitWork_t *a, datUnitWork_t *b)
{
    return b->maxhp * ((s32)dds3GetRandIntA(0, 10) + 10) / 100;
}

s32 nbGetNegoKureMpA(nbNegoProcessData_t *n, datUnitWork_t *a, datUnitWork_t *b)
{
    return b->maxmp * ((s32)dds3GetRandIntA(0, 30) + 10) / 100;
}

s32 nbGetNegoKureMpB(nbNegoProcessData_t *n, datUnitWork_t *a, datUnitWork_t *b)
{
    return b->maxmp * ((s32)dds3GetRandIntA(0, 10) + 10) / 100;
}

s32 nbGetKureItemA(nbNegoProcessData_t *n, datUnitWork_t *a, datUnitWork_t *b)
{
    s32 group;

    if (b->level == 0) {
        return 0;
    }
    if (b->level >= 0x64) {
        return 0;
    }
    group = (b->level - 1) / 5;
    return D_003E2318[(group * 4 + nbSelectKureItemA(group)) * 2];
}

s32 nbSelectKureItemA(s32 lv_idx)
{
    u8 *p;
    s32 r;
    s32 sum;
    s32 i;

    r = dds3GetRandIntA(0, 0x64);
    p = &datNegoReqItemListA[lv_idx * 8];
    sum = 0;
    for (i = 0; i < 4; i++, p += 2) {
        if (r < sum + *p) {
            return i;
        }
        sum += *p;
    }
    return 0;
}

s32 nbGetKureItemB(nbNegoProcessData_t *n, datUnitWork_t *a, datUnitWork_t *b)
{
    s32 group;

    if (b->level == 0) {
        return 0;
    }
    if (b->level >= 0x64) {
        return 0;
    }
    group = (b->level - 1) / 5;
    return D_003E23F8[(group * 3 + nbSelectKureItemB(group)) * 2];
}

s32 nbSelectKureItemB(s32 lv_idx)
{
    u8 *p;
    s32 r;
    s32 sum;
    s32 i;

    r = dds3GetRandIntA(0, 0x64);
    p = &datNegoReqItemListB[lv_idx * 6];
    sum = 0;
    for (i = 0; i < 3; i++, p += 2) {
        if (r < sum + *p) {
            return i;
        }
        sum += *p;
    }
    return 0;
}

s32 nbGetNegoRequestType(s32 reqtype)
{
    u8 *p;
    s32 i;
    s32 sum;
    s32 r;

    r = dds3GetRandIntA(0, 100);
    sum = 0;
    p = &datNegoReqTable[reqtype * 5];
    for (i = 0; i < 5; i++) {
        if (r < sum + *p) {
            return i;
        }
        sum += *p;
        p++;
    }
    return 0;
}

s32 nbGetNegoTuikaRequestType(s32 tuikareqtype)
{
    u8 *p;
    s32 i;
    s32 sum;
    s32 r;

    r = dds3GetRandIntA(0, 100);
    sum = 0;
    p = &datNegoTuikaReqtype[tuikareqtype * 3];
    for (i = 0; i < 3; i++) {
        if (r < sum + *p) {
            return i;
        }
        sum += *p;
        p++;
    }
    return 0;
}

typedef struct { s32 requestmode; u8 remainder[0x64]; } nbNegoSkillEntry_t;
#ifdef NON_MATCHING
typedef struct { s8 skillattr; s8 type; s16 index; u16 keisyoform; s8 capacity; u8 flag; } nbSkillAttr_t;
extern nbSkillAttr_t attr[];
extern nbNegoSkillEntry_t D_003E1338[];
extern datUnitWork_t *nbGetUnitWorkPointer(nbMainProcessData_t *data, nbParty_t *party);
extern s32 datCheckTalk(datUnitWork_t *work);

void nbSetAllHojoSkillWork(nbNegoProcessData_t *n)
{
    nbParty_t *party;
    datUnitWork_t *work;
    s32 i;
    s32 j;
    s32 count;
    u16 skill;
    u16 *slot;

    for (i = 0; i < 14; i++) {
        party = &n->data->party[i];
        slot = party->hojoskill;
        for (j = 0; j < 8; j++) {
            *slot++ = 0;
        }
        party = &n->data->party[i];
        party->flag &= ~0x80;
        if (i < 4 && (party->flag & 1) != 0 && i != n->ppartyindex) {
            work = nbGetUnitWorkPointer(n->data, party);
            if (datCheckTalk(work) != 0) {
                count = 0;
                for (j = 0; j < work->skillcnt; j++) {
                    skill = work->skill[j];
                    if (attr[skill].type == 2 && D_003E1338[skill - 0x180].requestmode == 7) {
                        n->data->party[i].hojoskill[count++] = skill;
                    }
                }
            }
        }
    }
}
#else
INCLUDE_ASM(const s32, "newbattle/nbNegoProcess", nbSetAllHojoSkillWork);
#endif

s32 nbGetHojoSkillWorkCnt(nbNegoProcessData_t *n)
{
    nbMainProcessData_t *data;
    s32 cnt;
    s32 i;
    s32 j;

    data = n->data;
    cnt = 0;
    for (i = 0; i < 4; i++) {
        if (data->party[i].flag & 1) {
            if (!(data->party[i].flag & 0x80)) {
                for (j = 0; j < 8; j++) {
                    if (n->data->party[i].hojoskill[j] != 0) {
                        if (!(n->data->party[i].hojoskill[j] & 0x8000)) {
                            cnt++;
                        }
                    }
                }
            }
        }
    }
    return cnt;
}

INCLUDE_ASM(const s32, "newbattle/nbNegoProcess", nbSearchHojoSkillWork);

INCLUDE_ASM(const s32, "newbattle/nbNegoProcess", nbGetQuestionType);

s32 nbGetNegoYaruItemA(datUnitWork_t *w)
{
    u8 *p;
    s32 r;
    s32 sum;
    s32 i;

    sum = 0;
    r = dds3GetRandIntA(0, 0x64);
    /* Walks the item byte of each entry; its weight is the byte before it. */
    p = &master_text[w->id].itema[0].item;
    for (i = 0; i < 4; i++, p += 2) {
        if (r < sum + p[-1]) {
            return *p;
        }
        sum += p[-1];
    }
    return 0;
}

s32 nbGetNegoYaruItemB(datUnitWork_t *w)
{
    u8 *p;
    s32 r;
    s32 sum;
    s32 i;

    sum = 0;
    r = dds3GetRandIntA(0, 0x64);
    /* Walks the item byte of each entry; its weight is the byte before it. */
    p = &master_text[w->id].itemb[0].item;
    for (i = 0; i < 4; i++, p += 2) {
        if (r < sum + p[-1]) {
            return *p;
        }
        sum += p[-1];
    }
    return 0;
}

s32 nbGetNegoYaruStone(datUnitWork_t *w)
{
    u8 *p;
    s32 r;
    s32 sum;
    s32 i;

    sum = 0;
    r = dds3GetRandIntA(0, 0x64);
    /* Walks the item byte of each entry; its weight is the byte before it. */
    p = &master_text[w->id].stone[0].item;
    for (i = 0; i < 4; i++, p += 2) {
        if (r < sum + p[-1]) {
            return *p;
        }
        sum += p[-1];
    }
    return 0;
}

extern char D_00544E48[];
extern char D_00544E70[];

s32 nbGetInfoMsg(s32 infotype)
{
    /* Both lists reserve the full 60-message table; each type scans 30 rows. */
    s32 normalList[60];
    s32 conditionList[60];
    s32 base;
    s32 normalCount;
    s32 conditionCount;
    s32 i;
    s32 id;
    s32 useConditionList;
    s32 selected;

    base = 0;
    if (infotype == 2) {
        base = 30;
    }
    normalCount = 0;
    conditionCount = 0;
    for (i = 0; i < 30; i++) {
        if (InfoSelectTable[i + base].jouken[0] == 0) {
            normalList[normalCount++] = i + base;
        }
    }

    for (i = 0; i < 30; i++) {
        id = i + base;
        if (nbCheckSelectMesJoukenAri(id, 100) == 1) {
            conditionList[conditionCount++] = id;
        }
    }

    useConditionList = 0;
    if (conditionCount != 0) {
        if (dds3GetRandIntA(NULL, 10) < 2) {
            useConditionList = 0;
        } else {
            useConditionList = 1;
        }
    }
    if (useConditionList == 0) {
        selected = normalList[dds3GetRandIntA(NULL, normalCount)];
    } else {
        selected = conditionList[dds3GetRandIntA(NULL, conditionCount)];
    }

    printf(D_00544E48, useConditionList, selected);
    printf(D_00544E70, normalCount, conditionCount);
    return selected;
}


/* The caller passes 100; this provider checks its fixed three condition entries. */
s32 nbCheckSelectMesJoukenAri(s32 index, s32 limit)
{
    s32 i;
    s32 ret;
    s32 jouken;

    (void)limit;
    ret = 0;
    for (i = 0; i < 3; i++) {
        jouken = InfoSelectTable[index].jouken[i];
        if (jouken == 0) {
            break;
        }
        if (evtBitCheck(jouken) == 0) {
            return 0;
        }
        ret = 1;
    }
    return ret;
}

s32 nbSetNegoNoMotion(nbNegoProcessData_t *n, nbFormation_t *form)
{
    nbClsNegoMessage(n);
    nbAddMotionKoukaPacket(0, (s32)n->actdata, form, 0xF);
    nbAddSePacket2(0, (s32)n->actdata, nbPlayMotionSe(form) + 1);
    return 0xF;
}

s32 nbSetNegoYesMotion(nbNegoProcessData_t *n, nbFormation_t *form)
{
    nbClsNegoMessage(n);
    nbAddMotionKoukaPacket(0, (s32)n->actdata, form, 0xE);
    nbAddSePacket2(0, (s32)n->actdata, nbPlayMotionSe(form));
    return 0xF;
}

s32 nbFlowFindUnitEntry(nbNegoProcessData_t *n)
{
    nbInnenRec_t *p;
    s32 i;

    p = D_003E3158;
    for (i = 0; i < 64; i++) {
        if (p->id != 0 && n->pwork->id == p->pid && n->ework->id == p->eid) {
            return i;
        }
        p++;
    }
    return -1;
}

extern void *sdfGrAllocPacketBuff(u32 size);
extern void sdfDrawTagInit(sdfDrawTag_t *tag);
extern void sdfDrawTagLink(sdfDrawTag_t *tag, u32 packet);
extern void *sdfDevPrintf(u32 x, u32 y, u32 z, s32 attr, const char *format, ...);
extern sdfGrOt_t D_002F67F8;
extern char D_00544E98[];
extern char D_00544EA8[];
extern char D_00551148[];
extern char D_00551150[];

void nbNegoDebugPrint(nbNegoProcessData_t *n)
{
    sdfDrawTag_t *tag;
    u32 packet;
    s32 index;
    s32 i;
    s32 message;
    s32 y;
    u32 historyMessageId;

    y = 0x7CE0;
    tag = sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);

    packet = (u32)sdfDevPrintf(0x7100, 0x7B60, 0x00FFFF80, 0, D_00544E98,
                               (s32)(n->nownegoid >> 16), (s32)(n->nownegoid & 0xFFFF));
    sdfDrawTagLink(tag, packet);
    packet = (u32)sdfDevPrintf(0x7100, 0x7C80, 0x00FFFF80, 5, D_00551148);
    sdfDrawTagLink(tag, packet);

    index = (n->historyindex + 1) % 14;
    for (i = 0; i < 14; i++) {
        historyMessageId = n->history[index];
        message = n->historymes[index];
        if (message != 0) {
            packet = (u32)sdfDevPrintf(0x7100, y, 0x00FFFF80, 0, D_00544EA8,
                                       (s32)(historyMessageId >> 16), (s32)(historyMessageId & 0xFFFF), message);
            sdfDrawTagLink(tag, packet);
        } else {
            packet = (u32)sdfDevPrintf(0x7100, y, 0x00FFFF80, 0, D_00551150,
                                       (s32)(historyMessageId >> 16), (s32)(historyMessageId & 0xFFFF));
            sdfDrawTagLink(tag, packet);
        }
        index = (index + 1) % 14;
        y += 0x60;
    }
    D_002F67F8.regFunc(&D_002F67F8, tag);
}

extern char D_00544EB8[];
extern char D_00544ED8[];
extern char D_00544EF0[];
extern char D_00544F00[];
extern char D_00551158[];

void nbNegoDebugPrint2(nbNegoProcessData_t *n)
{
    sdfDrawTag_t *tag;
    u32 packet;
    s32 i;
    u32 id;

    tag = sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    if (n->endid == 0) {
        packet = (u32)sdfDevPrintf(0x7A00, 0x7C80, 0xFFFF80, 0xE, D_00544EB8,
                                  (s32)(n->nownegoid >> 16), (s32)(n->nownegoid & 0xFFFF));
        sdfDrawTagLink(tag, packet);
    } else {
        packet = (u32)sdfDevPrintf(0x7A00, 0x7C80, 0xFFFF80, 0xE, D_00544ED8);
        sdfDrawTagLink(tag, packet);
    }
    if (n->endid == 0) {
        for (i = 0; i < n->nextkouhocnt; i++) {
            id = n->nextkouho[i];
            packet = (u32)sdfDevPrintf(0x7A00 + i * 0x480, 0x7D40, 0xFFFF80,
                                      i == n->selectcur ? 4 : 0, D_00551150,
                                      (s32)(id >> 16), (s32)(id & 0xFFFF));
            sdfDrawTagLink(tag, packet);
        }
    } else {
        packet = (u32)sdfDevPrintf(0x7A00, 0x7D40, 0xFFFF80, 4, D_00551158);
        sdfDrawTagLink(tag, packet);
    }
    packet = (u32)sdfDevPrintf(0x7A00, 0x7E60, 0xFFFF80, 0, D_00544EF0);
    sdfDrawTagLink(tag, packet);
    packet = (u32)sdfDevPrintf(0x7A00, 0x7EC0, 0xFFFF80, 0, D_00544F00);
    sdfDrawTagLink(tag, packet);
    D_002F67F8.regFunc(&D_002F67F8, tag);
}

s32 NegoDebug(nbNegoProcessData_t *n)
{
    s32 sel;

    if (n->nextkouhocnt == 0) {
        if ((s8)dds3PadButtonState[0x21] >= 0) {
            return 0;
        }
        return 1;
    }
    if (dds3SelectMenu(0, n->nextkouhocnt, 0, n->nextkouhocnt, 0, NULL, NULL, &n->selectcur,
                       NULL) == 1) {
        n->nextnegoid = *(u32 *)((u8 *)(n->selectcur * 4) + (u32)n + 0x24);
        return 1;
    }
    if ((s8)dds3PadButtonState[0x20] < 0) {
        sel = dds3GetRandIntA(0, n->nextkouhocnt);
        n->nextnegoid = *(u32 *)((u8 *)(sel * 4) + (u32)n + 0x24);
        return 1;
    }
    return 0;
}

void *nbNegoProcessMessageChk(dds3ProcessID_t *id)
{
    nbNegoProcessData_t *n;

    n = dds3GetProcessData(id);
    if (n->data->stat & 0x400000) {
        nbNegoDebugPrint(n);
    }
    if (nbCheckNegoMessage(n) != 0) {
        printf(D_00544F10);
        return nbNegoProcess1;
    }
    return NULL;
}

void *nbNegoProcessMessage(dds3ProcessID_t *id)
{
    nbNegoProcessData_t *n;

    n = dds3GetProcessData(id);
    if (n->data->stat & 0x400000) {
        nbNegoDebugPrint(n);
    }
    if (n->msgwndid >= 0 && n->msgwndid != n->nowmsgid) {
        itfMesMngMsgWndCls(n->msgwndid);
        n->msgwndid = -1;
    }
    itfMesMngMarkBitStat(n->nowmsgid, 0x200000);
    if (n->stat & 4) {
        itfMesMngRequestSelect(n->nowmsgid, n->nowmsgno, 0);
    } else {
        itfMesMngRequestMessage(n->nowmsgid, n->nowmsgno, 0);
    }
    n->msgwndid = n->nowmsgid;
    return nbNegoProcessMessageChk;
}

s32 nbNegoSetProcessData(dds3ProcessID_t *id)
{
    nbNegoProcessData_t *n;

    n = dds3GetProcessData(id);
    n->data->negoresult = n->endtype;
    return -1;
}

INCLUDE_ASM(const s32, "newbattle/nbNegoProcess", nbNegoProcess1);

void nbNegoShutdown(dds3ProcessID_t *id)
{
    nbNegoProcessData_t *process = dds3GetProcessData(id);
    s32 i;

    for (i = 0; i < 31; i++) {
        if (process->flowmsg[i] >= 0) {
            itfMesMngRelease(process->flowmsg[i]);
            process->flowmsg[i] = -1;
        }
    }
    if (process->questionmsg >= 0) {
        itfMesMngRelease(process->questionmsg);
        process->questionmsg = -1;
    }
    for (i = 0; i < 31; i++) {
        if (process->infomsg[i] >= 0) {
            itfMesMngRelease(process->infomsg[i]);
            process->infomsg[i] = -1;
        }
    }
    for (i = 0; i < 31; i++) {
        if (process->aisyomsg[i] >= 0) {
            itfMesMngRelease(process->aisyomsg[i]);
            process->aisyomsg[i] = -1;
        }
    }
    if (process->bossmsg >= 0) {
        itfMesMngRelease(process->bossmsg);
        process->bossmsg = -1;
    }
    if (process->helpmsg >= 0) {
        itfMesMngRelease(process->helpmsg);
        process->helpmsg = -1;
    }
    if (process->helpmsg2 >= 0) {
        itfMesMngRelease(process->helpmsg2);
        process->helpmsg2 = -1;
    }
    if (process->innenmsg >= 0) {
        itfMesMngRelease(process->innenmsg);
        process->innenmsg = -1;
    }
    if (process->data->negoproc != NULL) {
        process->data->negoproc = NULL;
    }
    sdfChipFreeDirect(process);
    p2sdBgmVolUp();
}

/* Negotiation skills have a 0x68-byte record; the first word selects request mode. */
extern nbNegoSkillEntry_t D_003E1338[];
typedef struct { void *flow; void *aisyo; void *question; void *info; } nbNegoMsgEntry_t;
extern nbNegoMsgEntry_t negoMsgTable[];
extern u8 itfMesEmbedded_4EB810[], itfMesEmbedded_4EE8B0[];
extern u8 itfMesEmbedded_4EF0C8[], itfMesEmbedded_4E3DD8[];
extern void *sdfChipAllocDirect(s32 size);
extern void *memset(void *dest, s32 value, u32 size);
extern datUnitWork_t *nbGetUnitWorkFromFormindex(s32 index);
extern nbParty_t *nbGetPartyFromFormindex(s32 index);
typedef struct itfMesResource_t itfMesResource_t;
extern s32 itfMesMngInitialize(itfMesResource_t *message);
extern s32 nbMemoryLoadNegoSe(nbFormation_t *form);

INCLUDE_RODATA(const s32, "newbattle/nbNegoProcess", D_00544E48);

INCLUDE_RODATA(const s32, "newbattle/nbNegoProcess", D_00544E70);

INCLUDE_RODATA(const s32, "newbattle/nbNegoProcess", D_00544E98);

INCLUDE_RODATA(const s32, "newbattle/nbNegoProcess", D_00544EA8);

INCLUDE_RODATA(const s32, "newbattle/nbNegoProcess", D_00544EB8);

INCLUDE_RODATA(const s32, "newbattle/nbNegoProcess", D_00544ED8);

INCLUDE_RODATA(const s32, "newbattle/nbNegoProcess", D_00544EF0);

INCLUDE_RODATA(const s32, "newbattle/nbNegoProcess", D_00544F00);

INCLUDE_RODATA(const s32, "newbattle/nbNegoProcess", D_00544F10);

nbNegoProcessData_t *InitNegoProcessData(nbActionProcessData_t *actdata, s32 pformindex,
                                      s32 eformindex, s32 negotype, s32 skillid)
{
    nbNegoProcessData_t *n;
    nbNegoMsgEntry_t *message;
    s32 skillindex;
    s32 requesttype = 0;
    s32 i;

    n = sdfChipAllocDirect(sizeof(*n));
    memset(n, 0, sizeof(*n));
    n->actdata = actdata;
    n->data = actdata->data;
    switch (negotype) {
    case 0:
    case 6: n->nownegoid = 0x10007; break;
    case 1: n->nownegoid = 0xC0001; skillid = 0x180; break;
    case 2: n->nownegoid = 0xD0001; skillid = 0x180; break;
    case 3: n->nownegoid = 0xE0001; break;
    case 4: n->nownegoid = 0xF0001; skillid = 0x180; break;
    case 5: n->nownegoid = 0x100001; skillid = 0x180; break;
    }
    n->negotype = negotype;
    n->nowmsgid = -1;
    n->msgwndid = -1;
    n->prevreqtype = -1;
    n->flag1_6 = 0;
    n->flag1_39 = 0;
    skillindex = skillid - 0x180;
    switch (D_003E1338[skillindex].requestmode) {
    case 0: requesttype = 0; break;
    case 1: requesttype = 2; break;
    case 2: requesttype = 3; break;
    case 3: requesttype = 4; break;
    case 4: requesttype = 1; break;
    case 5: requesttype = 5; break;
    case 6: requesttype = 6; break;
    case 7: break;
    }
    n->negoreqtype = requesttype;
    n->negoskillid = skillid;
    n->pwork = nbGetUnitWorkFromFormindex(pformindex);
    n->pparty = nbGetPartyFromFormindex(pformindex);
    n->pform = &n->data->form[pformindex];
    n->pformindex = pformindex;
    n->ppartyindex = n->pform->partyindex;
    n->ework = nbGetUnitWorkFromFormindex(eformindex);
    n->eparty = nbGetPartyFromFormindex(eformindex);
    n->eform = &n->data->form[eformindex];
    n->eformindex = eformindex;
    n->epartyindex = n->eform->partyindex;
    n->pkucyo = master_text[n->pwork->id].mesno;
    n->ekucyo = master_text[n->ework->id].mesno;
    for (i = 0; i < 31; i++) {
        n->flowmsg[i] = -1;
        if (negoMsgTable[i].flow) {
            n->flowmsg[i] = itfMesMngInitialize(negoMsgTable[i].flow);
            nbSetNegoMesVar(n->flowmsg[i], n->pwork, n->ework);
        }
    }
    message = &negoMsgTable[n->ekucyo];
    n->questionmsg = itfMesMngInitialize(message->question);
    nbSetNegoMesVar(n->questionmsg, n->pwork, n->ework);
    for (i = 0; i < 31; i++) {
        n->infomsg[i] = -1;
        if (negoMsgTable[i].info) {
            n->infomsg[i] = itfMesMngInitialize(negoMsgTable[i].info);
            nbSetNegoMesVar(n->infomsg[i], n->pwork, n->ework);
        }
    }
    for (i = 0; i < 31; i++) {
        n->aisyomsg[i] = -1;
        if (negoMsgTable[i].aisyo) {
            n->aisyomsg[i] = itfMesMngInitialize(negoMsgTable[i].aisyo);
            nbSetNegoMesVar(n->aisyomsg[i], n->pwork, n->ework);
        }
    }
    n->bossmsg = itfMesMngInitialize((itfMesResource_t *)itfMesEmbedded_4EB810);
    nbSetNegoMesVar(n->bossmsg, n->pwork, n->ework);
    n->helpmsg = itfMesMngInitialize((itfMesResource_t *)itfMesEmbedded_4EE8B0);
    nbSetNegoMesVar(n->helpmsg, n->pwork, n->ework);
    n->helpmsg2 = itfMesMngInitialize((itfMesResource_t *)itfMesEmbedded_4EF0C8);
    nbSetNegoMesVar(n->helpmsg2, n->pwork, n->ework);
    n->innenmsg = itfMesMngInitialize((itfMesResource_t *)itfMesEmbedded_4E3DD8);
    nbSetNegoMesVar(n->innenmsg, n->pwork, n->ework);
    nbMemoryLoadNegoSe(n->eform);
    return n;
}

void nbInitNegoProcess(nbActionProcessData_t *actdata, s32 pformindex, s32 eformindex, s32 negotype,
                      s32 skillid)
{
    nbNegoProcessData_t *n;
    dds3ProcessID_t *id;

    n = InitNegoProcessData(actdata, pformindex, eformindex, negotype, skillid);
    nbSetNegoWaitMotion(n->eform);
    nbSetAllHojoSkillWork(n);
    p2sdBgmVolDown();
    id = dds3StartProcess(D_00551160, 0x3F9, 1, 1, nbNegoProcess1, nbNegoShutdown, n);
    dds3LinkProcess(actdata->data->MainProcessID, id);
    actdata->data->negoproc = id;
}
INCLUDE_SDATA(const s32, "newbattle/nbNegoProcess", D_00551148);

INCLUDE_SDATA(const s32, "newbattle/nbNegoProcess", D_00551150);

INCLUDE_SDATA(const s32, "newbattle/nbNegoProcess", D_00551158);

INCLUDE_SDATA(const s32, "newbattle/nbNegoProcess", D_00551160);

