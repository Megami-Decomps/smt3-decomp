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

extern s32 dds3GetRandIntA(s32 aLimit, s32 name);

typedef struct { u16 type; u8 lo; u8 hi; } nbNegoRateEntry_t;

extern nbNegoRateEntry_t D_003E2540[][7];

extern s32 datGetParam(datUnitWork_t *work, s32 paratype);

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

extern s32 (*D_00527518[])(nbEventProcessData_t *e);

extern s32 nbGetQuestionType(datUnitWork_t *w);

extern s32 (*D_00527420[])(nbEventProcessData_t *e);

extern s32 (*D_004F0788[])(nbActionProcessData_t *a, s32 joken);

extern s32 (*D_00527688[])(nbEventProcessData_t *e);

extern s32 (*D_00527698[])(nbEventProcessData_t *e);

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

s32 Flow7_1(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x70003);
    return 0;
}

s32 Flow7_3(nbNegoProcessData_t *n)
{
    u8 *p;

    nbAddNegoIdKouho(n, 0x7000F);
    nbAddNegoIdKouho(n, 0x7000B);
    n->qtype = nbGetQuestionType(n->ework);
    p = (u8 *)(n->qtype + (s32)DDS3_GBWK + 0x1CE8); /* &negoques[n->qtype] */
    if (*p < 0xFF) {
        *p = *p + 1;
    }
    if (n->qtype == 0) {
        n->qindex = dds3GetRandIntA(0, 5);
        n->nextnegoid = n->nextkouho[0];
    } else {
        n->qindex = dds3GetRandIntA(0, 2);
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

/* NON_MATCHING: 1 of 84 words differs (first at +0xC8); the annul bit of one branch. */
#ifdef NON_MATCHING
s32 Flow7_11(nbNegoProcessData_t *n)
{
    s32 mes;

    mes = 0;
    switch (n->nownegophase) {
    case 0:
        switch (n->qtype) {
        case 1:
            mes = n->qindex == 0 ? 0x1F : 0x21;
            break;
        case 2:
            mes = n->qindex == 0 ? 0x23 : 0x25;
            break;
        case 3:
            mes = n->qindex == 0 ? 0x1B : 0x1D;
            break;
        }
        nbDispNegoMessage(n, n->questionmsg, mes, 0);
        break;
    case 1:
        switch (n->qtype) {
        case 1:
            mes = n->qindex == 0 ? 0x20 : 0x22;
            break;
        case 2:
            mes = n->qindex == 0 ? 0x24 : 0x26;
            break;
        case 3:
            mes = n->qindex == 0 ? 0x1C : 0x1E;
            break;
        }
        nbDispNegoMessage(n, n->questionmsg, mes, 1);
        nbAddNegoIdKouho(n, 0x70016);
        break;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "newbattle/NbNegoFlowQuestion", Flow7_11);
#endif

s32 Flow7_15(nbNegoProcessData_t *n)
{
    s32 mes;

    mes = 0;
    switch (n->nownegophase) {
    case 0:
        switch (n->qindex) {
        case 0:
            mes = 0x29;
            break;
        case 1:
            mes = 0x2F;
            break;
        case 2:
            mes = 0x35;
            break;
        case 3:
            mes = 0x3B;
            break;
        case 4:
            mes = 0x41;
            break;
        }
        nbDispNegoMessage(n, n->questionmsg, mes, 0);
        break;
    case 1:
        switch (n->qindex) {
        case 0:
            mes = 0x2A;
            break;
        case 1:
            mes = 0x30;
            break;
        case 2:
            mes = 0x36;
            break;
        case 3:
            mes = 0x3C;
            break;
        case 4:
            mes = 0x42;
            break;
        }
        nbDispNegoMessage(n, n->questionmsg, mes, 1);
        nbAddNegoIdKouho(n, 0x70014);
        break;
    }
    return 0;
}

/* NON_MATCHING: 28 of 76 words differ (first at +0x78). */
#ifdef NON_MATCHING
s32 Flow7_20(nbNegoProcessData_t *n)
{
    s32 rate;
    s32 lo;
    s32 hi;

    nbAddNegoIdKouho(n, 0x70017);
    nbAddNegoIdKouho(n, 0x70018);
    hi = D_003E2540[n->ekucyo][n->qindex].hi;
    lo = D_003E2540[n->ekucyo][n->qindex].lo;
    switch (D_003E2540[n->ekucyo][n->qindex].type) {
    case 0:
        if (!(datGetParam(n->pwork, 1) < datGetParam(n->ework, 1))) {
            if (n->nowselect != 0) {
                rate = 0x64 - lo;
            } else {
                rate = lo;
            }
        } else {
            if (n->nowselect != 0) {
                rate = 0x64 - hi;
            } else {
                rate = hi;
            }
        }
        break;
    case 1:
        if (!(n->pwork->level < n->ework->level)) {
            if (n->nowselect != 0) {
                rate = 0x64 - lo;
            } else {
                rate = lo;
            }
        } else {
            if (n->nowselect != 0) {
                rate = 0x64 - hi;
            } else {
                rate = hi;
            }
        }
        break;
    default:
        rate = 0x32;
        break;
    }
    if (dds3GetRandIntA(0, 0x64) < rate) {
        n->nextnegoid = n->nextkouho[0];
    } else {
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "newbattle/NbNegoFlowQuestion", Flow7_20);
#endif

INCLUDE_ASM(const s32, "newbattle/NbNegoFlowQuestion", Flow7_22);

s32 Flow7_23(nbNegoProcessData_t *n)
{
    s32 mes;

    mes = 0;
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoYesMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, n->workid[4]);
        switch (n->qindex) {
        case 0:
            mes = n->nowselect == 0 ? 0x2B : 0x2D;
            break;
        case 1:
            mes = n->nowselect == 0 ? 0x31 : 0x33;
            break;
        case 2:
            mes = n->nowselect == 0 ? 0x37 : 0x39;
            break;
        case 3:
            mes = n->nowselect == 0 ? 0x3D : 0x3F;
            break;
        case 4:
            mes = n->nowselect == 0 ? 0x43 : 0x45;
            break;
        }
        nbDispNegoMessage(n, n->questionmsg, mes, 0);
        break;
    }
    return 0;
}

s32 Flow7_24(nbNegoProcessData_t *n)
{
    s32 mes;

    mes = 0;
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoNoMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, n->workid[5]);
        n->workid[2] = 0x60045;
        switch (n->qindex) {
        case 0:
            mes = n->nowselect == 0 ? 0x2C : 0x2E;
            break;
        case 1:
            mes = n->nowselect == 0 ? 0x32 : 0x34;
            break;
        case 2:
            mes = n->nowselect == 0 ? 0x38 : 0x3A;
            break;
        case 3:
            mes = n->nowselect == 0 ? 0x3E : 0x40;
            break;
        case 4:
            mes = n->nowselect == 0 ? 0x44 : 0x46;
            break;
        }
        nbDispNegoMessage(n, n->questionmsg, mes, 0);
        break;
    }
    return 0;
}

s32 Flow7_25(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoYesMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, n->workid[4]);
        nbDispNegoMessage(n, n->questionmsg, 0x27, 0);
        break;
    }
    return 0;
}

s32 Flow7_26(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoNoMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, n->workid[5]);
        n->workid[2] = 0x60045;
        nbDispNegoMessage(n, n->questionmsg, 0x28, 0);
        break;
    }
    return 0;
}

s32 nbNegoFlowQuestion(nbNegoProcessData_t *n)
{
    return D_004EF868[(u16)n->nownegoid](n);
}
