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
    dds3ProcessID_t *mainproc;
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

/* 0x1A4 bytes; only the fields this unit touches are named. */
typedef struct nbActionProcessData_t
{
    nbMainProcessData_t *data;
    u32 unk_04;
    u8 unk_08[0x1C];
    nbParty_t *party;
    nbFormation_t *form;
    datUnitWork_t *work;
    s32 partyindex;
    s32 type;
    s32 target;
    s32 selectbit;
    u8 unk_40[0x28];
    s32 unk_68;
    u8 unk_6C[0x124];
    u32 aiselect;
    s32 aijokentbl;
    u8 unk_198[0x8];
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

extern s32 dds3GetRandIntA(s32 min, s32 max);

extern u32 f;

extern void nbSetMesItem(s32 msgid, s32 it);

extern void nbCameraSetTarget(nbFormation_t *form);

/* The nb event script work; only the fields this unit touches are named. */
typedef struct nbEventProcessData_t {
    nbMainProcessData_t *data;
    s32 unk_04;
    s32 unk_08;
    s32 step;
    s32 frame;
    void *unk_14;
    void (*unk_18)(dds3ProcessID_t *id);
    void *unk_1C;
    nbPacHandle_t *unk_20;
    void *unk_24;
} nbEventProcessData_t;

extern s32 datGetRace(datUnitWork_t *unit);

extern s32 nbSearchHojoSkillWork(nbNegoProcessData_t *n, s32 type);

extern s32 evtBitCheck(s32 bit);

extern s32 (*D_00527518[])(nbEventProcessData_t *e);

extern s32 nbGetHojoSkillWorkCnt(nbNegoProcessData_t *n);

extern void nbPanelDraw(s32 show);

extern nbKoukaPacket_t *nbMakeKaifukuPacket(s32 startframe, s32 uniqueid, nbFormation_t *form,
                                            s32 type, s32 hp, s32 mp, s32 bad);

extern s32 nbCheckFlowUnit(nbNegoProcessData_t *n);

extern s32 nbGetNegoYaruMakkaB(nbNegoProcessData_t *n, datUnitWork_t *pwork, datUnitWork_t *ework);

extern s32 (*D_00527420[])(nbEventProcessData_t *e);

extern s32 (*D_004F0788[])(nbActionProcessData_t *a, s32 joken);

extern s32 (*D_00527688[])(nbEventProcessData_t *e);

extern s32 (*D_00527698[])(nbEventProcessData_t *e);

extern void nbAddNegoIdKouho(nbNegoProcessData_t *n, u32 id);

extern void nbDispNegoMessage(nbNegoProcessData_t *n, s32 mes, s32 type, s32 wait);

extern s32 (*nbE902Table[])(nbEventProcessData_t *e);

extern s32 datCheckItemFull(s32 item);

// @@DECLS@@
extern void datAddMaka(s32 maka);

extern void datAddItem(s32 item, s32 cnt);

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

nbKoukaPacket_t *nbAddHitEffKoukaPacket(s32 startframe, s32 uniqueid, nbFormation_t *form, s32 type);

nbKoukaPacket_t *nbAddMojiEffKoukaPacket(s32 startframe, s32 frame, s32 uniqueid, nbFormation_t *form, s32 type);

s32 nbMakeBadDamagePacket(s32 startframe, s32 uniqueid, nbFormation_t *form, u32 bad);

void nbClsNegoMessage(nbNegoProcessData_t *n);

s32 nbGetNegoYaruItemB(datUnitWork_t *w);

void nbSetMesMakka(s32 msgid, s32 i);

/* NON_MATCHING: 15 of 63 words differ (first at +0x7C); one word short. */
#ifdef NON_MATCHING
s32 Flow10_1(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA0002);
    nbAddNegoIdKouho(n, 0xA000B);
    nbAddNegoIdKouho(n, 0x6002B);
    if ((n->ework->id == 0x5C || n->ework->id == 0x3D || n->ework->id == 0x61) &&
        evtBitCheck(8) == 0 && n->negoreqtype == 0) {
        n->nextnegoid = n->nextkouho[1];
        return 0;
    }
    if (n->cnt6_37 != 0) {
        if ((u32)dds3GetRandIntA(0, 0xA) < 6) {
            n->nextnegoid = n->nextkouho[2];
        } else {
            n->nextnegoid = n->nextkouho[1];
        }
        return 0;
    }
    n->nextnegoid = n->nextkouho[dds3GetRandIntA(0, 2)];
    if (datGetRace(n->ework) != 0x21) {
        return 0;
    }
    n->nextnegoid = n->nextkouho[1];
    return 0;
}
#else
INCLUDE_ASM(const s32, "newbattle/NbNegoFlowNg", Flow10_1);
#endif

s32 Flow10_2(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoNoMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0xA0003);
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], dds3GetRandIntA(0, 2) + 0xD7, 0);
        break;
    }
    return 0;
}

s32 Flow10_3(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA0006);
    nbAddNegoIdKouho(n, 0xA0004);
    if (nbGetHojoSkillWorkCnt(n) > 0) {
        n->nextnegoid = n->nextkouho[1];
    } else {
        n->nextnegoid = n->nextkouho[0];
    }
    return 0;
}

s32 Flow10_4(nbNegoProcessData_t *n)
{
    s32 r;

    nbAddNegoIdKouho(n, 0x60034);
    nbAddNegoIdKouho(n, 0xA0006);
    n->workid[0] = 0xA000A;
    n->workid[1] = 0xA0005;
    r = nbSearchHojoSkillWork(n, 1);
    if (r != 0) {
        n->hojopartyindex = (u32)r >> 16;
        n->hojoskillid = r & 0xFFFF;
        n->nextnegoid = n->nextkouho[0];
    } else {
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

s32 Flow10_5(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoNoMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0xA0006);
        nbCameraSetTarget(n->eform);
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xDA, 0);
        break;
    }
    return 0;
}

s32 Flow10_6(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA0007);
    nbAddNegoIdKouho(n, 0xA0008);
    nbAddNegoIdKouho(n, 0xA0009);
    nbCameraSetTarget(n->eform);
    switch (n->negoreqtype) {
    case 0:
        n->nextnegoid = n->nextkouho[1];
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        n->nextnegoid = n->nextkouho[0];
        break;
    case 6:
        n->nextnegoid = n->nextkouho[2];
        break;
    }
    return 0;
}

s32 Flow10_7(nbNegoProcessData_t *n)
{
    s32 mes;

    mes = 0;
    switch (n->negoreqtype) {
    case 1:
    case 5:
        mes = 0xDC;
        break;
    case 2:
        mes = dds3GetRandIntA(0, 2) == 0 ? 0xDC : 0xDE;
        break;
    case 3:
        mes = 0xDD;
        break;
    case 4:
        mes = 0xDE;
        break;
    }
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], mes, 0);
    if (nbCheckFlowUnit(n) == 1) {
        nbAddNegoIdKouho(n, 0xA0021);
        n->temp_param[0] = 4;
        return 0;
    }
    return 4;
}

s32 Flow10_8(nbNegoProcessData_t *n)
{
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xDB, 0);
    if (nbCheckFlowUnit(n) == 1) {
        nbAddNegoIdKouho(n, 0xA0021);
        n->temp_param[0] = 4;
        return 0;
    }
    return 4;
}

s32 Flow10_9(nbNegoProcessData_t *n)
{
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xDF, 0);
    if (nbCheckFlowUnit(n) == 1) {
        nbAddNegoIdKouho(n, 0xA0021);
        n->temp_param[0] = 4;
        return 0;
    }
    return 4;
}

s32 Flow10_33(nbNegoProcessData_t *n)
{
    u32 flag;

    switch (n->nownegophase) {
    case 0:
        nbCameraSetTaizi(n->pform, n->eform);
        break;
    case 1:
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x122, 0);
        break;
    case 2:
        flag = datGetAisyoFlag(n->ework, 7);
        if ((flag & 0x20000) != 0) {
            nbAddHitEffKoukaPacket(0, 0, n->eform, 1);
        } else if ((flag & 0x40000) != 0) {
            nbAddHitEffKoukaPacket(0, 0, n->eform, 2);
        } else if ((flag & 0x110000) != 0) {
            nbAddHitEffKoukaPacket(0, 0, n->eform, 3);
            nbAddMojiEffKoukaPacket(0, 0x2D, 0, n->eform, 4);
        } else {
            nbMakeBadDamagePacket(0, 0, n->eform, 0x400);
            nbAddHitEffKoukaPacket(0, 0, n->eform, 0xA);
        }
        break;
    case 3:
        n->waitframe = 0x2D;
        break;
    case 4:
        return n->temp_param[0];
    }
    return 0;
}

/* NON_MATCHING: 15 of 35 words differ (first at +0x34). */
#ifdef NON_MATCHING
s32 Flow10_10(nbNegoProcessData_t *n)
{
    s32 mes;
    s32 type;

    nbAddNegoIdKouho(n, 0x6004A);
    n->workid[6] = 0x60009;
    n->cnt6_36 = 0;
    n->cnt6_37 = 0;
    if (n->hojoaisyo == 1) {
        mes = n->aisyomsg[n->ekucyo];
        type = n->hojoaisyomes;
    } else {
        mes = n->flowmsg[n->ekucyo];
        type = 0xD9;
    }
    nbDispNegoMessage(n, mes, type, 0);
    return 0;
}
#else
INCLUDE_ASM(const s32, "newbattle/NbNegoFlowNg", Flow10_10);
#endif

s32 Flow10_11(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA000C);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xE0, 0);
    return 0;
}

s32 Flow10_12(nbNegoProcessData_t *n)
{
    s32 rate;

    rate = 0x28;
    nbAddNegoIdKouho(n, 0xA000D);
    nbAddNegoIdKouho(n, 0xA0015);
    switch (n->aisyo) {
    case 1:
        rate = 0x46;
        break;
    case 2:
        rate = 0;
        break;
    }
    if (n->cnt6_36 == 0) {
        n->nextnegoid = n->nextkouho[1];
    } else if (dds3GetRandIntA(0, 100) < rate) {
        n->nextnegoid = n->nextkouho[0];
    } else {
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

s32 Flow10_13(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA0010);
    nbAddNegoIdKouho(n, 0xA0010);
    nbAddNegoIdKouho(n, 0xA0011);
    n->nextnegoid = n->nextkouho[dds3GetRandIntA(0, 3)];
    return 0;
}

s32 Flow10_15(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA0012);
    n->getmakka = nbGetNegoYaruMakkaB(n, n->pwork, n->ework);
    nbCameraSetTarget(n->eform);
    nbSetMesMakka(n->flowmsg[n->ekucyo], n->getmakka);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xE2, 0);
    return 0;
}

s32 Flow10_16(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA0013);
    nbAddNegoIdKouho(n, 0xA0011);
    n->getitem = nbGetNegoYaruItemB(n->ework);
    if (n->getitem == 0) {
        n->getitem = 0x24;
    }
    if (datCheckItemFull(n->getitem) != 0) {
        n->nextnegoid = n->nextkouho[1];
        return 0;
    }
    n->nextnegoid = n->nextkouho[0];
    nbCameraSetTarget(n->eform);
    nbSetMesItem(n->flowmsg[n->ekucyo], n->getitem);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xE1, 0);
    return 0;
}

s32 Flow10_17(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA0014);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xE3, 0);
    return 0;
}

s32 Flow10_18(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA001E);
    datAddMaka(n->getmakka);
    return 0;
}

s32 Flow10_19(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA001F);
    datAddItem(n->getitem, 1);
    return 0;
}

s32 Flow10_20(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA0020);
    nbClsNegoMessage(n);
    nbPanelDraw(1);
    nbMakeKaifukuPacket(0, 0, n->pform, 0, n->pwork->maxhp, -1, 0);
    nbAddHitEffKoukaPacket(0, 0, n->pform, 8);
    n->waitframe = 0;
    return 0;
}

/* NON_MATCHING: 11 of 37 words differ (first at +0x5C). */
#ifdef NON_MATCHING
s32 Flow10_21(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA0018);
    nbAddNegoIdKouho(n, 0xA001C);
    if (((n->ework->id == 0x5C || n->ework->id == 0x3D || n->ework->id == 0x61) &&
         evtBitCheck(8) == 0) ||
        (u32)dds3GetRandIntA(0, 10) < 5) {
        n->nextnegoid = n->nextkouho[1];
    } else {
        n->nextnegoid = n->nextkouho[0];
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "newbattle/NbNegoFlowNg", Flow10_21);
#endif

s32 Flow10_22(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA0017);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xEA, 0);
    return 0;
}

s32 Flow10_23(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA001D);
    nbDispNegoMessage(n, n->helpmsg, 1, 0);
    return 0;
}

s32 Flow10_24(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA0019);
    nbAddNegoIdKouho(n, 0xA001A);
    nbAddNegoIdKouho(n, 0xA001B);
    switch (n->negoreqtype) {
    case 0:
        n->nextnegoid = n->nextkouho[1];
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        n->nextnegoid = n->nextkouho[0];
        break;
    case 6:
        n->nextnegoid = n->nextkouho[2];
        break;
    }
    return 0;
}

s32 Flow10_25(nbNegoProcessData_t *n)
{
    s32 mes;

    mes = 0;
    switch (n->negoreqtype) {
    case 1:
    case 5:
        mes = 0xE6;
        break;
    case 2:
        mes = dds3GetRandIntA(0, 2) == 0 ? 0xE6 : 0xE8;
        break;
    case 3:
        mes = 0xE7;
        break;
    case 4:
        mes = 0xE8;
        break;
    }
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], mes, 0);
    if (nbCheckFlowUnit(n) == 1) {
        nbAddNegoIdKouho(n, 0xA0021);
        n->temp_param[0] = 1;
        return 0;
    }
    return 1;
}

s32 Flow10_26(nbNegoProcessData_t *n)
{
    s32 r;

    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xE5, 0);
    r = nbCheckFlowUnit(n);
    if (r == 1) {
        nbAddNegoIdKouho(n, 0xA0021);
        n->temp_param[0] = r;
        return 0;
    }
    return 1;
}

s32 Flow10_27(nbNegoProcessData_t *n)
{
    s32 r;

    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xE9, 0);
    r = nbCheckFlowUnit(n);
    if (r == 1) {
        nbAddNegoIdKouho(n, 0xA0021);
        n->temp_param[0] = r;
        return 0;
    }
    return 1;
}

s32 Flow10_28(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA0017);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xE4, 0);
    return 0;
}

s32 Flow10_29(nbNegoProcessData_t *n)
{
    return 2;
}

s32 Flow10_30(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA0016);
    nbSetMesMakka(n->helpmsg, n->getmakka);
    nbDispNegoMessage(n, n->helpmsg, 3, 0);
    return 0;
}

s32 Flow10_31(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA0016);
    nbSetMesItem(n->helpmsg, n->getitem);
    nbDispNegoMessage(n, n->helpmsg, 5, 0);
    return 0;
}

s32 Flow10_32(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA0016);
    nbDispNegoMessage(n, n->helpmsg2, 7, 0);
    nbPanelDraw(0);
    n->waitframe = 60;
    return 0;
}

s32 nbNegoFlowNg(nbNegoProcessData_t *n)
{
    return D_004EF6F8[(u16)n->nownegoid](n);
}
