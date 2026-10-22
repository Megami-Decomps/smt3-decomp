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
    s32 unk_68;
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

extern u8 D_003E2470[];

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

extern s32 nbGetNegoYaruMakkaB(nbNegoProcessData_t *n, datUnitWork_t *a, datUnitWork_t *b);

extern s32 (*D_00527420[])(nbEventProcessData_t *e);

extern s32 (*D_004F0788[])(nbActionProcessData_t *a, s32 joken);

extern s32 (*D_00527688[])(nbEventProcessData_t *e);

extern s32 (*D_00527698[])(nbEventProcessData_t *e);

extern void nbAddNegoIdKouho(nbNegoProcessData_t *n, u32 id);

extern void nbDispNegoMessage(nbNegoProcessData_t *n, s32 msgid, s32 msgno, s32 sel);

extern s32 (*nbE902Table[])(nbEventProcessData_t *e);

extern s32 datCheckItemFull(s32 id);

// @@DECLS@@
extern void datAddMaka(s32 addmaka);

extern void datAddItem(s32 id, s32 num);

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

s32 nbGetNegoYaruItemB(datUnitWork_t *w);

void nbSetMesMakka(s32 msgid, s32 i);

s32 Flow12_1(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x1001F);
    n->workid[3] = 0xC0002;
    return 0;
}

s32 Flow12_2(nbNegoProcessData_t *n)
{
    s32 row;
    s32 rand;
    s32 sum;
    s32 i;

    sum = 0;
    nbAddNegoIdKouho(n, 0xC0003);
    nbAddNegoIdKouho(n, 0xC0004);
    nbAddNegoIdKouho(n, 0xC0018);
    row = master_text[n->ework->id].negorate;
    rand = dds3GetRandIntA(0, 0x64);
    for (i = 0; i < 3; i++) {
        if (rand < sum + D_003E2470[i + row * 3]) {
            n->nextnegoid = n->nextkouho[i];
            break;
        }
        sum += D_003E2470[i + row * 3];
    }
    return 0;
}

/* NON_MATCHING: 73 of 97 words differ (first at +0x34). */
#ifdef NON_MATCHING
s32 Flow12_3(nbNegoProcessData_t *n)
{
    s32 plevel;
    s32 elevel;
    s32 pparam;
    s32 eparam;
    s32 rate;
    u32 next;

    switch (n->nownegophase) {
    case 0:
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xEB, 0);
        break;
    case 1:
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xD5, 1);
        break;
    case 2:
        nbAddNegoIdKouho(n, 0xC0010);
        nbAddNegoIdKouho(n, 0xC0011);
        nbAddNegoIdKouho(n, 0xC0012);
        nbAddNegoIdKouho(n, 0xC0013);
        plevel = n->pwork->level;
        pparam = datGetParam(n->pwork, 1);
        elevel = n->ework->level;
        eparam = datGetParam(n->ework, 1);
        if (plevel >= elevel) {
            if (pparam < eparam) {
                rate = 0x28;
            } else {
                rate = 0x50;
            }
        } else {
            if (pparam < eparam) {
                rate = 0x14;
            } else {
                rate = 0x3C;
            }
        }
        if (n->nowselect == 0) {
            if (dds3GetRandIntA(0, 0x64) < rate) {
                next = n->nextkouho[0];
            } else {
                next = n->nextkouho[1];
            }
        } else {
            if (dds3GetRandIntA(0, 0x64) < 0x64 - rate) {
                next = n->nextkouho[2];
            } else {
                next = n->nextkouho[3];
            }
        }
        n->nextnegoid = next;
        break;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "newbattle/NbNegoFlowMukan", Flow12_3);
#endif

/* NON_MATCHING: 73 of 97 words differ (first at +0x34). */
#ifdef NON_MATCHING
s32 Flow12_4(nbNegoProcessData_t *n)
{
    s32 plevel;
    s32 elevel;
    s32 pparam;
    s32 eparam;
    s32 rate;
    u32 next;

    switch (n->nownegophase) {
    case 0:
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xEC, 0);
        break;
    case 1:
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xD6, 1);
        break;
    case 2:
        nbAddNegoIdKouho(n, 0xC0014);
        nbAddNegoIdKouho(n, 0xC0015);
        nbAddNegoIdKouho(n, 0xC0016);
        nbAddNegoIdKouho(n, 0xC0017);
        plevel = n->pwork->level;
        pparam = datGetParam(n->pwork, 1);
        elevel = n->ework->level;
        eparam = datGetParam(n->ework, 1);
        if (plevel >= elevel) {
            if (pparam < eparam) {
                rate = 0x28;
            } else {
                rate = 0x50;
            }
        } else {
            if (pparam < eparam) {
                rate = 0x14;
            } else {
                rate = 0x3C;
            }
        }
        if (n->nowselect == 0) {
            if (dds3GetRandIntA(0, 0x64) < rate) {
                next = n->nextkouho[0];
            } else {
                next = n->nextkouho[1];
            }
        } else {
            if (dds3GetRandIntA(0, 0x64) < 0x64 - rate) {
                next = n->nextkouho[2];
            } else {
                next = n->nextkouho[3];
            }
        }
        n->nextnegoid = next;
        break;
    }
    return 0;
}
#else
/* NON_MATCHING: 73 of 97 words differ (first at +0x34); four words short. */
#ifdef NON_MATCHING
s32 Flow12_4(nbNegoProcessData_t *n)
{
    s32 plv;
    s32 elv;
    s32 ppar;
    s32 epar;
    s32 ritu;

    switch (n->nownegophase) {
    case 0:
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xEC, 0);
        break;
    case 1:
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xD6, 1);
        break;
    case 2:
        nbAddNegoIdKouho(n, 0xC0014);
        nbAddNegoIdKouho(n, 0xC0015);
        nbAddNegoIdKouho(n, 0xC0016);
        nbAddNegoIdKouho(n, 0xC0017);
        plv = n->pwork->level;
        ppar = datGetParam(n->pwork, 1);
        elv = n->ework->level;
        epar = datGetParam(n->ework, 1);
        if (plv < elv) {
            ritu = ppar < epar ? 0x14 : 0x3C;
        } else {
            ritu = ppar < epar ? 0x28 : 0x50;
        }
        if (n->nowselect == 0) {
            n->nextnegoid =
                dds3GetRandIntA(0, 0x64) < ritu ? n->nextkouho[0] : n->nextkouho[1];
        } else {
            n->nextnegoid = dds3GetRandIntA(0, 0x64) < 0x64 - ritu ? n->nextkouho[2]
                                                                   : n->nextkouho[3];
        }
        break;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "newbattle/NbNegoFlowMukan", Flow12_4);
#endif
#endif

s32 Flow12_24(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoNoMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0xC0019);
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xED, 0);
        break;
    }
    return 0;
}

s32 Flow12_25(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA001D);
    nbDispNegoMessage(n, n->helpmsg, 1, 0);
    return 0;
}

s32 Flow12_16(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoYesMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0xC001A);
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xEE, 0);
        break;
    }
    return 0;
}

s32 Flow12_17(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoNoMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0xC002E);
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xEF, 0);
        break;
    }
    return 0;
}

s32 Flow12_18(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoYesMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0xC001A);
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xF0, 0);
        break;
    }
    return 0;
}

s32 Flow12_19(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoNoMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0xC002E);
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xF1, 0);
        break;
    }
    return 0;
}

s32 Flow12_20(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoYesMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0xC001A);
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xF2, 0);
        break;
    }
    return 0;
}

s32 Flow12_21(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoNoMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0xC002E);
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xF3, 0);
        break;
    }
    return 0;
}

s32 Flow12_22(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoYesMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0xC001A);
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xF4, 0);
        break;
    }
    return 0;
}

s32 Flow12_23(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoNoMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0xC002E);
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xF5, 0);
        break;
    }
    return 0;
}

s32 Flow12_26(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xC001B);
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/NbNegoFlowMukan", Flow12_27);

s32 Flow12_28(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xC001D);
    if (n->negotype != 2) {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xF9, 0);
    } else {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x105, 0);
    }
    return 0;
}

s32 Flow12_29(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xD4, 1);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0xC001E);
        nbAddNegoIdKouho(n, 0xC0026);
        n->nextnegoid = n->nextkouho[n->nowselect];
        break;
    }
    return 0;
}

s32 Flow12_30(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoYesMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0xC001F);
        if (n->negotype != 2) {
            if (dds3GetRandIntA(0, 2) == 0) {
                nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xFA, 0);
            } else {
                nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xFB, 0);
            }
        } else {
            if (dds3GetRandIntA(0, 2) == 0) {
                nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x106, 0);
            } else {
                nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x107, 0);
            }
        }
        break;
    }
    return 0;
}

s32 Flow12_31(nbNegoProcessData_t *n)
{
    nbDispNegoMessage(n, n->helpmsg, 2, 0);
    return 9;
}

s32 Flow12_35(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xC0024);
    if (n->negotype != 2) {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xF6, 0);
    } else {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x102, 0);
    }
    return 0;
}

s32 Flow12_36(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xC0025);
    if (dds3GetRandIntA(0, 2) == 0) {
        n->getinfotype = 0;
    } else {
        n->getinfotype = 2;
    }
    n->getinfomsg = nbGetInfoMsg(n->getinfotype);
    return 0;
}

s32 Flow12_37(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA001C);
    nbDispNegoMessage(n, n->infomsg[n->ekucyo], n->getinfomsg, 0);
    return 0;
}

s32 Flow12_38(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xC0027);
    if (dds3GetRandIntA(0, 2) == 0) {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x108, 0);
    } else {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x109, 0);
    }
    return 0;
}

s32 Flow12_39(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA001D);
    nbDispNegoMessage(n, n->helpmsg, 1, 0);
    return 0;
}

s32 Flow12_40(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xC0029);
    if (n->negotype != 2) {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xF7, 0);
    } else {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x103, 0);
    }
    n->getmakka = nbGetNegoYaruMakkaB(n, n->pwork, n->ework);
    return 0;
}

s32 Flow12_41(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xC002A);
    nbSetMesMakka(n->helpmsg, n->getmakka);
    nbDispNegoMessage(n, n->helpmsg, 3, 0);
    return 0;
}

s32 Flow12_42(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA001D);
    datAddMaka(n->getmakka);
    return 0;
}

s32 Flow12_43(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xC002C);
    nbAddNegoIdKouho(n, 0xC0028);
    n->getitem = nbGetNegoYaruItemB(n->ework);
    if (n->getitem == 0) {
        n->getitem = 1;
    }
    if (datCheckItemFull(n->getitem) != 0) {
        n->nextnegoid = n->nextkouho[1];
        return 0;
    }
    if (n->negotype != 2) {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xF8, 0);
    } else {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x104, 0);
    }
    return 0;
}

s32 Flow12_44(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xC002D);
    nbSetMesItem(n->helpmsg, n->getitem);
    nbDispNegoMessage(n, n->helpmsg, 5, 0);
    return 0;
}

s32 Flow12_45(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA001D);
    datAddItem(n->getitem, 1);
    return 0;
}

s32 Flow12_46(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xC002F);
    return 0;
}

s32 Flow12_47(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xC0030);
    nbAddNegoIdKouho(n, 0xC0031);
    if (dds3GetRandIntA(0, 2) == 0) {
        n->nextnegoid = n->nextkouho[0];
    } else {
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

s32 Flow12_48(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA001D);
    return 0;
}

s32 Flow12_49(nbNegoProcessData_t *n)
{
    return 4;
}

s32 nbNegoFlowMukan(nbNegoProcessData_t *n)
{
    return D_004EF960[(u16)n->nownegoid](n);
}
