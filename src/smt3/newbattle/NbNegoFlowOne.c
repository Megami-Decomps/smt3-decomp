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

extern datUnitWork_t *nbGetUnitWorkPointer(nbMainProcessData_t *data, nbParty_t *party);

extern s32 datGet3Zokusei(datUnitWork_t *work);

extern s32 evtGetAgeOfMoon(void);

extern s32 dds3GetRandIntA(s32 aLimit, s32 name);

extern datDevilFormat_t datDevilTable[];

typedef struct { s32 unk_00; u8 pad[0x64]; } nbNegoSkillEntry_t;

extern nbNegoSkillEntry_t D_003E1338[];

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

extern s32 datGetPlayerLevel(void);

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

extern s32 datGetRace(datUnitWork_t *work);

extern s32 datSearchDevilStock(s32 id);

extern s32 (*D_00527518[])(nbEventProcessData_t *e);

extern s32 datCheckTalk(datUnitWork_t *work);

extern s32 datCheckDying(datUnitWork_t *work);

extern s32 (*D_00527420[])(nbEventProcessData_t *e);

extern s32 (*D_004F0788[])(nbActionProcessData_t *a, s32 joken);

extern s32 (*D_00527688[])(nbEventProcessData_t *e);

extern s32 (*D_00527698[])(nbEventProcessData_t *e);

extern s32 nbFlowGetUnitEntry(datUnitWork_t *a, datUnitWork_t *b, s32 c, s32 d);

extern void nbAddNegoIdKouho(nbNegoProcessData_t *n, u32 id);

extern void nbDispNegoMessage(nbNegoProcessData_t *n, s32 msgid, s32 msgno, s32 sel);

extern s32 (*nbE902Table[])(nbEventProcessData_t *e);

// @@DECLS@@
extern void datAddMaka(s32 addmaka);

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

s32 nbFlowFindUnitEntry(nbNegoProcessData_t *n);

void nbSetMesMakka(s32 msgid, s32 i);

void nbSetNegoMesVar(s32 msgid, datUnitWork_t *p, datUnitWork_t *e);

s32 Flow1_3(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x10005);
    nbAddNegoIdKouho(n, 0x1001F);
    n->workid[3] = 0x10004;
    if (datCheckTalk(n->pwork) != 0 || (n->actdata->stat & 0x20) == 0) {
        n->nextnegoid = n->nextkouho[0];
    } else {
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

s32 Flow1_4(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = 0x1E;
        break;
    case 1: {
        u16 enemy = n->ework->id;
        if (enemy != 0x96 && enemy != 0x1D && enemy != 3 &&
            enemy != 8 && enemy != 0xD && enemy != 0xA &&
            enemy != 5 && enemy != 4 && enemy != 0x10 &&
            enemy != 0x18 && enemy != 0x1E && enemy != 0x99 &&
            evtGetAgeOfMoon() == 8) {
            nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x98, 0);
        } else {
            nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x111, 0);
        }
        return 1;
    }
    }
    return 0;
}

s32 Flow1_5(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x10006);
    nbAddNegoIdKouho(n, 0x1000A);
    n->nextnegoid = n->nextkouho[1];
    switch (n->negoskillid) {
    case 0x189:
        if (n->pwork->id == 0x8F) {
            n->temp_param[0] = 3;
            n->nextnegoid = n->nextkouho[0];
        }
        break;
    case 0x18A:
        if (n->pwork->id == 0x71) {
            n->temp_param[0] = 4;
            n->nextnegoid = n->nextkouho[0];
        }
        break;
    case 0x18B:
        if (n->pwork->id == 2) {
            n->temp_param[0] = 5;
            n->nextnegoid = n->nextkouho[0];
        }
        break;
    case 0x18C:
    case 0x18D:
    case 0x18E:
    case 0x18F:
    case 0x190:
    case 0x191:
    case 0x192:
    case 0x193:
    case 0x194:
    case 0x195:
    case 0x196:
    case 0x197:
    case 0x198:
    case 0x199:
    case 0x19A:
    case 0x19B:
    case 0x19C:
    case 0x19D:
    case 0x19E:
    case 0x19F:
    case 0x1A0:
    case 0x1A1:
    case 0x1A2:
    case 0x1A3:
        break;
    }
    return 0;
}

s32 Flow1_6(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x1000A);
    n->flag1_6 = 1;
    return 0;
}

s32 Flow1_7(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x10008);
    nbAddNegoIdKouho(n, 0x10003);
    if (datCheckTalk(n->ework) != 0) {
        n->nextnegoid = n->nextkouho[1];
    } else {
        n->nextnegoid = n->nextkouho[0];
    }
    return 0;
}

s32 Flow1_8(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x1001F);
    n->workid[3] = 0x10009;
    return 0;
}

s32 Flow1_9(nbNegoProcessData_t *n)
{
    s32 mes;

    mes = 8;
    switch (n->ework->badstatus & 0xFFF) {
    case 1:
        mes = 0xA;
        break;
    case 0x200:
        mes = 9;
        break;
    case 4:
        mes = 0xB;
        break;
    case 8:
        mes = 0xD;
        break;
    case 0x10:
        mes = 0xC;
        break;
    case 2:
        mes = 0xE;
        break;
    case 0x400:
        break;
    }
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], mes, 0);
    return 1;
}

s32 Flow1_10(nbNegoProcessData_t *n)
{
    nbMainProcessData_t *data;
    s32 major;

    nbAddNegoIdKouho(n, 0x1001F);
    nbAddNegoIdKouho(n, 0x1001A);
    n->workid[3] = 0x1000B;
    if (n->ework->id >= 0x100) {
        n->nextnegoid = n->nextkouho[0];
    } else {
        n->nextnegoid = n->nextkouho[1];
    }
    data = n->data;
    if ((data->stat & 1) != 0) {
        n->nextnegoid = n->nextkouho[0];
    }
    major = data->stagemajor;
    if (major >= 0xF0 && major < 0xF7) {
        n->nextnegoid = n->nextkouho[0];
    }
    return 0;
}

#ifdef NON_MATCHING
s32 Flow1_11(nbNegoProcessData_t *n)
{
    s32 major;

    switch (n->nownegophase) {
    case 0:
        n->waitframe = 30;
        return 0;
    case 1:
        major = n->data->stagemajor;
        if (major >= 0xF0 && major < 0xF7) {
            nbDispNegoMessage(n, n->bossmsg, 0x80, 0);
        } else if (n->ework->id >= 0x100) {
            nbDispNegoMessage(n, n->bossmsg, n->ework->id - 0x100, 0);
        } else {
            nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x12A, 0);
        }
        return 1;
    default:
        return 0;
    }
}
#else
INCLUDE_ASM(const s32, "newbattle/NbNegoFlowOne", Flow1_11);
#endif

s32 Flow1_33(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x1000C);
    nbAddNegoIdKouho(n, 0x1001F);
    n->workid[3] = 0x10020;
    if (n->pwork->id == n->ework->id) {
        n->nextnegoid = n->nextkouho[0];
        return 0;
    }
    if ((datGetRace(n->ework) == 0x13 || datGetRace(n->ework) == 0x12 ||
         n->ework->id == 0x9A) && n->negoreqtype == 0) {
        n->nextnegoid = n->nextkouho[1];
        return 0;
    }
    n->nextnegoid = (datGet3Zokusei(n->ework) < 0 && n->negoskillid != 0x1A5)
                        ? n->nextkouho[1]
                        : n->nextkouho[0];
    return 0;
}

s32 Flow1_32(nbNegoProcessData_t *n)
{
    if (n->ework->id == 0x3 && n->pwork->id == n->ework->id) {
        n->waitframe = 0xF;
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x74, 0);
        return 2;
    } else if (n->ework->id == 0x4 && n->pwork->id == n->ework->id) {
        n->waitframe = 0xF;
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x75, 0);
        return 2;
    } else if (n->ework->id == 0x5 && n->pwork->id == n->ework->id) {
        n->waitframe = 0xF;
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x76, 0);
        return 2;
    } else if (n->ework->id == 0x8 && n->pwork->id == n->ework->id) {
        n->waitframe = 0xF;
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x77, 0);
        return 2;
    } else if (n->ework->id == 0xA && n->pwork->id == n->ework->id) {
        n->waitframe = 0xF;
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x78, 0);
        return 2;
    } else if (n->ework->id == 0xD && n->pwork->id == n->ework->id) {
        n->waitframe = 0xF;
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x79, 0);
        return 2;
    } else if (n->ework->id == 0x10 && n->pwork->id == n->ework->id) {
        n->waitframe = 0xF;
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x7A, 0);
        return 2;
    } else if (n->ework->id == 0x18 && n->pwork->id == n->ework->id) {
        n->waitframe = 0xF;
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x7B, 0);
        return 2;
    } else if (n->ework->id == 0x1D && n->pwork->id == n->ework->id) {
        n->waitframe = 0xF;
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x7C, 0);
        return 2;
    } else if (n->ework->id == 0x1E && n->pwork->id == n->ework->id) {
        n->waitframe = 0xF;
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x7D, 0);
        return 2;
    } else if (n->ework->id == 0x96 && n->pwork->id == n->ework->id) {
        n->waitframe = 0xF;
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x7E, 0);
        return 2;
    } else if (n->ework->id == 0x99 && n->pwork->id == n->ework->id) {
        n->waitframe = 0xF;
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x7F, 0);
        return 2;
    }
    /* Qualified cc1probe --jump: .27.jump2 merges message 0x80 into
     * 0x127's call tail and keeps 0x20 separate. RTL/assembly parity
     * verified; this completion block follows CONTRIBUTING's forward-branch rule. */
    if (n->ework->id == 0x96 ||
        n->ework->id == 0x1D ||
        n->ework->id == 0x3 ||
        n->ework->id == 0x8 ||
        n->ework->id == 0xD ||
        n->ework->id == 0xA ||
        n->ework->id == 0x5 ||
        n->ework->id == 0x4 ||
        n->ework->id == 0x10 ||
        n->ework->id == 0x18 ||
        n->ework->id == 0x1E ||
        n->ework->id == 0x99) {
        n->waitframe = 0xF;
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x80, 0);
        goto complete;
    }
    if ((datGetRace(n->ework) == 0x13 || datGetRace(n->ework) == 0x12 ||
         n->ework->id == 0x9A) && n->negoreqtype == 0) {
        n->waitframe = 0xF;
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x127, 0);
        goto complete;
    }
    n->waitframe = 0xF;
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x20, 0);
complete:
    return 1;
}

s32 Flow1_12(nbNegoProcessData_t *n)
{
    s32 cnt;
    s32 rate;

    nbAddNegoIdKouho(n, 0x1001F);
    nbAddNegoIdKouho(n, 0x10027);
    n->workid[3] = 0x1000F;
    n->nextnegoid = n->nextkouho[1];
    cnt = *(u8 *)(datDevilTable[n->ework->id].race + (s32)DDS3_GBWK + 0x1CA8);
    if (cnt >= 3 && DDS3_GBWK->maka >= 0x1F4) {
        if ((u32)(cnt - 3) < 3) {
            rate = 0x1E;
        } else {
            rate = ((u32)(cnt - 6) < 5) ? 0x32 : 0x46;
        }
        if (dds3GetRandIntA(0, 0x64) < rate) {
            n->nextnegoid = n->nextkouho[0];
        }
    }
    return 0;
}

s32 Flow1_13(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x1000E);
    nbAddNegoIdKouho(n, 0x1000F);
    return 0;
}

s32 Flow1_14(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x1000F);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 6, 0);
    return 0;
}

s32 Flow1_15(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoNoMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0x10010);
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 7, 0);
        break;
    }
    return 0;
}

s32 Flow1_16(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x10011);
    n->getmakka = DDS3_GBWK->maka / 3;
    *(u8 *)(datDevilTable[n->ework->id].race + (s32)DDS3_GBWK + 0x1CA8) = 0;
    return 0;
}

s32 Flow1_17(nbNegoProcessData_t *n)
{
    nbSetMesMakka(n->helpmsg, n->getmakka);
    nbDispNegoMessage(n, n->helpmsg, 4, 0);
    datAddMaka(-n->getmakka);
    return 1;
}

s32 Flow1_18(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x10021);
    nbAddNegoIdKouho(n, 0x1001F);
    n->workid[3] = 0x20001;
    if (n->ework->id == 0x96 || n->ework->id == 0x1D || n->ework->id == 3 ||
        n->ework->id == 8 || n->ework->id == 0xD || n->ework->id == 0xA ||
        n->ework->id == 5 || n->ework->id == 4 || n->ework->id == 0x10 ||
        n->ework->id == 0x18 || n->ework->id == 0x1E || n->ework->id == 0x99) {
        n->nextnegoid = n->nextkouho[1];
        return 0;
    }
    if (datGetRace(n->ework) == 0x21) {
        n->nextnegoid = n->nextkouho[0];
        return 0;
    }
    if (evtGetAgeOfMoon() == 8) {
        n->nextnegoid = n->nextkouho[1];
    } else {
        n->nextnegoid = n->nextkouho[0];
    }
    return 0;
}

s32 Flow1_19(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x10014);
    nbAddNegoIdKouho(n, 0x1001F);
    n->workid[3] = 0x40001;
    n->nextnegoid = n->nextkouho[0];
    if ((n->pwork->flag & 4) == 0 && (n->ework->flag & 4) == 0 &&
        n->pwork->id == n->ework->id) {
        n->flag1_6 = 0;
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

s32 Flow1_20(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x10015);
    nbAddNegoIdKouho(n, 0x1001F);
    n->workid[3] = 0x30001;
    n->nextnegoid = n->nextkouho[0];
    if ((n->ework->flag & 4) == 0 && datSearchDevilStock(n->ework->id) >= 0 &&
        n->negoreqtype == 0) {
        n->flag1_6 = 0;
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

s32 Flow1_21(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x10016);
    nbAddNegoIdKouho(n, 0x1001B);
    if (n->data->enemypcnt == 1) {
        n->nextnegoid = n->nextkouho[1];
    } else {
        n->nextnegoid = n->nextkouho[0];
    }
    if ((n->data->stat & 0x800000) == 0) {
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

#ifdef NON_MATCHING
s32 Flow1_22(nbNegoProcessData_t *n)
{
    u8 candidates[10];
    s32 count = 0;
    s32 i;
    nbMainProcessData_t *data;
    nbParty_t *party;
    u16 flag;
    datUnitWork_t *work;

    nbAddNegoIdKouho(n, 0x10018);
    nbAddNegoIdKouho(n, 0x10017);
    n->nextnegoid = n->nextkouho[0];
    for (i = 4; i < 14; i++) {
        data = n->data;
        party = &data->party[i];
        flag = party->flag;
        if ((flag & 1) && (flag & 2) &&
            !(flag & 0x38) && i != n->epartyindex) {
            work = nbGetUnitWorkPointer(data, party);
            if (datCheckTalk(work) && master_text[work->id].phase == 1) {
                candidates[count++] = i;
            }
        }
    }
    if (count > 0) {
        i = candidates[dds3GetRandIntA(0, count)];
        n->nextnegoid = n->nextkouho[1];
        n->boupartyindex = i;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "newbattle/NbNegoFlowOne", Flow1_22);
#endif

s32 Flow1_23(nbNegoProcessData_t *n)
{
    datUnitWork_t *work;
    datDevilNegoFormat_t *mt;
    s32 *msg;
    u32 messageOffset;

    work = nbGetUnitWorkPointer(n->data, &n->data->party[n->boupartyindex]);
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoNoMotion(n, n->eform);
        return 0;
    case 1:
        mt = &master_text[work->id];
        if (mt->phase == n->nownegophase) {
            /* Form the byte offset within the actual negotiation work first. */
            messageOffset = mt->mesno * sizeof(n->flowmsg[0]) +
                            (u32)&((nbNegoProcessData_t *)0)->flowmsg;
            msg = (s32 *)((u32)n + messageOffset);
            nbSetNegoMesVar(*msg, n->pwork, work);
            nbDispNegoMessage(n, *msg, 0x10, 0);
        }
        return 4;
    }
    return 0;
}

s32 Flow1_24(nbNegoProcessData_t *n)
{
    datUnitWork_t *work;
    s32 chance;
    s32 i;

    nbAddNegoIdKouho(n, 0x1001B);
    nbAddNegoIdKouho(n, 0x10019);
    n->nextnegoid = n->nextkouho[0];
    chance = 0x32 / (n->data->enemypcnt - 1);

    for (i = 4; i < 14; i++) {
        nbParty_t *party = &n->data->party[i];
        u16 flags = party->flag;

        if ((flags & 1) == 0) {
            continue;
        }
        if ((flags & 2) == 0) {
            continue;
        }
        if (flags & 0x38) {
            continue;
        }
        if (i == n->epartyindex) {
            continue;
        }

        work = nbGetUnitWorkPointer(n->data, party);
        if (datCheckTalk(work) == 0) {
            continue;
        }
        if (master_text[work->id].phase == 2) {
            continue;
        }
        if (dds3GetRandIntA(0, 100) >= chance) {
            continue;
        }
        n->nextnegoid = n->nextkouho[1];
        n->boupartyindex = i;
        break;
    }
    return 0;
}

extern struct nbKoukaPacket_t *nbAddMojiEffKoukaPacket(s32 startframe, s32 frame,
                                                  s32 uniqueid, nbFormation_t *form,
                                                  s32 type);
extern void nbCameraSetBougai(nbFormation_t *form);
extern s32 nbMemoryLoadNegoSe(nbFormation_t *form);

s32 Flow1_25(nbNegoProcessData_t *n)
{
    datUnitWork_t *work;
    nbFormation_t *form;
    datDevilNegoFormat_t *mt;
    s32 *msg;
    u32 messageOffset;
    s32 phase;

    work = nbGetUnitWorkPointer(n->data, &n->data->party[n->boupartyindex]);
    phase = n->nownegophase;
    form = &n->data->form[n->data->party[n->boupartyindex].formindex];

    switch (phase) {
    case 0:
        nbAddMojiEffKoukaPacket(0, 0x1E, (s32)n->actdata, form, 0);
        nbCameraSetBougai(form);
        nbMemoryLoadNegoSe(form);
        return 0;
    case 1:
        nbSetNegoNoMotion(n, form);
        mt = &master_text[work->id];
        /* Keep the message-array offset grouped before adding the work base. */
        messageOffset = mt->mesno * sizeof(n->flowmsg[0]) +
                        (u32)&((nbNegoProcessData_t *)0)->flowmsg;
        msg = (s32 *)((u32)n + messageOffset);
        nbSetNegoMesVar(*msg, n->pwork, work);
        nbDispNegoMessage(n, *msg, 0x11, 0);
        return 4;
    }
    return 0;
}

s32 Flow1_26(nbNegoProcessData_t *n)
{
    s32 entry;

    nbAddNegoIdKouho(n, 0x10012);
    nbAddNegoIdKouho(n, 0x1001F);
    n->workid[3] = 0x50001;
    if (evtGetAgeOfMoon() == 8) {
        n->nextnegoid = n->nextkouho[0];
        return 0;
    }
    entry = nbFlowFindUnitEntry(n);
    if (entry != -1) {
        n->innen_id = entry;
        n->flag1_6 = 0;
        n->nextnegoid = n->nextkouho[1];
    } else {
        n->nextnegoid = n->nextkouho[0];
    }
    return 0;
}

s32 Flow1_27(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x1001F);
    nbAddNegoIdKouho(n, 0x1001C);
    n->workid[3] = 0x1001E;
    if (master_text[n->ework->id].honboutype == 1) {
        n->nextnegoid = n->nextkouho[1];
    } else {
        n->nextnegoid = n->nextkouho[0];
    }
    return 0;
}

s32 Flow1_28(nbNegoProcessData_t *n)
{
    if (n->negotype != 3) {
        nbAddNegoIdKouho(n, 0x1001D);
        nbAddNegoIdKouho(n, 0x1001F);
    } else {
        nbAddNegoIdKouho(n, 0x1001D);
        nbAddNegoIdKouho(n, 0x1001E);
    }
    n->nextnegoid = n->nextkouho[1];
    n->workid[3] = 0x1001E;
    if (datCheckDying(n->ework) == 0 && dds3GetRandIntA(0, 100) < 0x32) {
        n->nextnegoid = n->nextkouho[0];
    }
    return 0;
}

s32 Flow1_29(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoNoMotion(n, n->eform);
        break;
    case 1:
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x12, 0);
        return 4;
    }
    return 0;
}

s32 Flow1_30(nbNegoProcessData_t *n)
{
    s32 type;
    s32 mes;

    switch (n->nownegophase) {
    case 0:
        n->waitframe = 0xF;
        break;
    case 1:
        nbAddNegoIdKouho(n, 0x10022);
        type = n->negoreqtype;
        mes = type + 0x13;
        if (type == 0) {
            if (n->data->enemypcnt == 1) {
                mes = 0xF;
            }
        }
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], mes, 0);
        break;
    }
    return 0;
}

extern s32 nbGetMotionNo(nbFormation_t *form, s32 motion);
extern s32 nbMakeUnitMovePacket(s32 startframe, s32 uniqueid,
                                nbFormation_t *form, u16 targetMask, s32 type,
                                f32 speed);
extern void nbCameraSetTaizi(nbFormation_t *invoke, nbFormation_t *target);
extern void nbResetFormPosRot(nbFormation_t *form);
extern void nbFormDispOn2(nbFormation_t *form);

s32 Flow1_31(nbNegoProcessData_t *n)
{
    s32 i;

    switch (n->nownegophase) {
    case 0:
        switch (n->negotype) {
        case 0:
        case 6:
            nbGetMotionNo(n->pform, 0);
            n->pform->cr = 100.0f / n->pform->scl;
            n->eform->cr = 100.0f / n->eform->scl;
            nbMakeUnitMovePacket(0, (s32)n->actdata, n->pform,
                                 (u16)n->actdata->select, 3, 1.0f);
            nbCameraSetTaizi(n->pform, n->eform);
            break;
        case 1:
        case 2:
        case 3:
            nbGetMotionNo(n->eform, 0);
            n->pform->cr = 100.0f / n->pform->scl;
            n->eform->cr = 100.0f / n->eform->scl;
            nbMakeUnitMovePacket(0, (s32)n->actdata, n->eform,
                                 (u16)n->actdata->select, 3, 1.0f);
            nbCameraSetTaizi(n->pform, n->eform);
            break;
        case 4:
            nbGetMotionNo(n->eform, 0);
            n->pform->cr = 100.0f / n->pform->scl;
            n->eform->cr = 100.0f / n->eform->scl;
            nbMakeUnitMovePacket(0, (s32)n->actdata, n->eform,
                                 (u16)n->actdata->select, 3, 1.0f);
            nbCameraSetTaizi(n->pform, n->eform);
            break;
        }
        for (i = 0; i < 4; i++) {
            nbResetFormPosRot(&n->data->form[i]);
        }
        nbFormDispOn2(n->pform);
        nbFormDispOn2(n->eform);
        break;
    case 1:
        break;
    case 2:
        if (n->flag1_6 == 1) {
            nbDispNegoMessage(n, n->flowmsg[n->ekucyo], n->temp_param[0], 0);
            n->flag1_6 = 0;
        }
        if (n->flag1_39 == 1) {
            nbDispNegoMessage(n, n->flowmsg[n->ekucyo], n->temp_param[0], 0);
            return 1;
        }
        break;
    case 3:
        nbAddNegoIdKouho(n, n->workid[3]);
        break;
    }
    return 0;
}

s32 Flow1_34(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x10023);
    nbAddNegoIdKouho(n, 0x10026);
    if (D_003E1338[n->negoskillid - 0x180].unk_00 == 0) {
        n->nextnegoid = n->nextkouho[0];
    } else {
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

s32 Flow1_35(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x10024);
    nbAddNegoIdKouho(n, 0x10025);
    if (datGetPlayerLevel() < datDevilTable[n->ework->id].level) {
        n->nextnegoid = n->nextkouho[0];
    } else {
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

s32 Flow1_36(nbNegoProcessData_t *n)
{
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x36, 0);
    return 1;
}

s32 Flow1_37(nbNegoProcessData_t *n)
{
    s32 mes;
    s32 found;

    found = 0;
    mes = 0;
    switch (datDevilTable[n->ework->id].race) {
    case 0x21:
        found = 1;
        mes = 0x37;
        break;
    case 7:
        found = 1;
        mes = 0x65;
        break;
    case 8:
        found = 1;
        mes = 0x66;
        break;
    }
    if (found == 1) {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], mes, 0);
        return 1;
    }
    nbAddNegoIdKouho(n, 0x60001);
    return 0;
}

s32 Flow1_38(nbNegoProcessData_t *n)
{
    if (datDevilTable[n->ework->id].race == 0x21 && n->negoskillid == 0x193) {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x64, 0);
        return 1;
    }
    nbAddNegoIdKouho(n, 0x60001);
    return 0;
}

s32 Flow1_39(nbNegoProcessData_t *n)
{
    if (datDevilTable[n->ework->id].race == 0x21 && n->negoreqtype == 0) {
        n->flag1_39 = 1;
        nbAddNegoIdKouho(n, 0x1001F);
        if (dds3GetRandIntA(0, 2) != 0) {
            n->temp_param[0] = 0x2B;
        } else {
            n->temp_param[0] = 0x2C;
        }
    } else {
        nbAddNegoIdKouho(n, 0x10013);
    }
    return 0;
}

s32 nbNegoFlowOne(nbNegoProcessData_t *n)
{
    return D_004EF4B0[(u16)n->nownegoid](n);
}

INCLUDE_ASM(const s32, "newbattle/NbNegoFlowOne", nbFlowGetUnitEntry);

s32 nbFlowCheckUnit(nbNegoProcessData_t *n, s32 ptype, s32 etype, s32 data)
{
    s32 p;
    s32 e;

    p = nbFlowGetUnitEntry(n->pwork, n->ework, ptype, data);
    e = nbFlowGetUnitEntry(n->ework, n->pwork, etype, data);
    if (p != 0 && e != 0) {
        return 1;
    }
    return 0;
}

s32 nbFlowCheckUnit2(datUnitWork_t *pwork, datUnitWork_t *ework, s32 ptype, s32 etype,
                     s32 data)
{
    s32 p;
    s32 e;

    p = nbFlowGetUnitEntry(pwork, ework, ptype, data);
    e = nbFlowGetUnitEntry(ework, pwork, etype, data);
    if (p != 0 && e != 0) {
        return 1;
    }
    return 0;
}
