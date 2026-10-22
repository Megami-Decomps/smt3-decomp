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
    u32 unk_04;
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

extern s32 printf(const char *format, ...);

extern datUnitWork_t *nbGetUnitWorkFromFormindex(s32 formindex);

extern datUnitWork_t *nbGetUnitWorkPointer(nbMainProcessData_t *data, nbParty_t *party);

extern s32 datGet3Zokusei(datUnitWork_t *work);

extern s32 dds3GetRandIntA(s32 aLimit, s32 name);

extern s32 nbCheckSkillCost(s32 formindex, s32 nskill);

/* 164-byte entries indexed by devil id; only the field this unit reads is named. */
typedef struct {
    u8 unk_00[0x2];
    u16 ailevel;
    struct { u32 joken[2]; s32 no; } entry[13];
    u8 unk_A4[0x4];
} datDevilAI_t;

extern datDevilAI_t D_003989E0[];

extern s32 nbSetAiTarget(nbActionProcessData_t *a, s32 code, s32 n);

extern s32 nbAiChkJoken(nbActionProcessData_t *a, u32 jo);

extern s32 nbGetHojoCounter(s32 formindex, s32 type);

extern void nbRunAi(nbActionProcessData_t *a);

extern void nbRunAiSub(nbActionProcessData_t *a, s32 tbl);

extern char D_00545728[];

/* One devil AI action: its weight, the skill it picks and the target rule. */
typedef struct { u16 ritu; u16 skill; u32 target; } datDevilAITbl_t;

extern u8 D_00398A08[];

extern s32 datGetPlayerLevel(void);

extern s32 nbGetLvAvg(nbMainProcessData_t *data, s32 side);

/* The nb event script work; only the fields this unit touches are named. */
typedef struct nbEventProcessData_t {
    nbMainProcessData_t *data;
    s32 unk_04;
    s32 time;
    s32 seq;
    s32 seqtime;
    void *unk_14;
    void (*unk_18)(dds3ProcessID_t *id);
    void *file;
    nbPacHandle_t *unk_20;
    void *mem;
} nbEventProcessData_t;

extern s32 ChkAiHojo(s32 formindex, s32 n);

extern s32 (*D_00527518[])(nbEventProcessData_t *e);

extern void nbSetHojoCounter(s32 formindex, s32 type, s32 point);

extern s32 ChkAiHojoMax(s32 formindex, s32 n);

extern u32 datGetAisyo(datUnitWork_t *work, s32 attr);

extern s32 (*D_00527420[])(nbEventProcessData_t *e);

extern s32 nbGetAiEntry(nbActionProcessData_t *a, s32 index);

extern s32 D_003989EC[][41];

extern char D_00545840[];

extern s32 (*D_004F0788[])(nbActionProcessData_t *a, s32 joken);

extern s32 (*D_00527688[])(nbEventProcessData_t *e);

extern s32 (*D_00527698[])(nbEventProcessData_t *e);

extern s32 (*nbE902Table[])(nbEventProcessData_t *e);

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

s32 nbCheckSkillUse(s32 formindex, s32 nskill);

s32 Chk_MYHP(nbActionProcessData_t *a, s32 n)
{
    if (a->work->hp * 100 / a->work->maxhp <= n) {
        a->aiselect = 1 << a->form->formindex;
        return 1;
    }
    return 0;
}

s32 Chk_MYMP(nbActionProcessData_t *a, s32 n)
{
    if (a->work->mp <= n) {
        a->aiselect = 1 << a->form->formindex;
        return 1;
    }
    return 0;
}

s32 Chk_FRHP(nbActionProcessData_t *a, s32 n)
{
    datUnitWork_t *work;
    s32 i;
    s32 one;

    one = 1;
    for (i = 4; i < 14; i++) {
        if (!(a->data->party[i].flag & 1)) {
            continue;
        }
        if (a->data->party[i].flag & 8) {
            continue;
        }
        if (a->data->party[i].flag & 0x10) {
            continue;
        }
        if (a->data->party[i].flag & 0x20) {
            continue;
        }
        work = nbGetUnitWorkPointer(a->data, &a->data->party[i]);
        if (n < work->hp * 100 / work->maxhp) {
            continue;
        }
        a->aiselect |= one << a->data->party[i].formindex;
    }
    return a->aiselect != 0;
}

s32 Chk_DMGCNT_285578(nbActionProcessData_t *a, s32 n)
{
    if (nbGetHojoCounter(a->form->formindex, 0) >= n) {
        a->aiselect = 1 << a->form->formindex;
        return 1;
    }
    return 0;
}

s32 Chk_CHKCNT_2855D8(nbActionProcessData_t *a, s32 n)
{
    s32 cnt;

    cnt = nbGetHojoCounter(a->form->formindex, a->aijokentbl + 1);
    if (cnt < 127) {
        cnt++;
    }
    nbSetHojoCounter(a->form->formindex, a->aijokentbl + 1, cnt);
    if (cnt < n) {
        return 0;
    }
    a->aiselect = 1 << a->form->formindex;
    return 1;
}

s32 Chk_FRCNT(nbActionProcessData_t *a, s32 n)
{
    nbParty_t *party;
    s32 i;
    s32 one;

    one = 1;
    if (n >= a->data->enemypcnt) {
        party = &a->data->party[4];
        for (i = 9; i >= 0; i--, party++) {
            if (!(party->flag & 1)) {
                continue;
            }
            if (party->flag & 8) {
                continue;
            }
            if (party->flag & 0x10) {
                continue;
            }
            if (party->flag & 0x20) {
                continue;
            }
            a->aiselect |= one << party->formindex;
        }
        return 1;
    }
    return 0;
}

s32 Chk_ENCNT(nbActionProcessData_t *a, s32 n)
{
    nbParty_t *party;
    s32 i;
    s32 one;

    one = 1;
    if (n >= a->data->playerpcnt) {
        party = a->data->party;
        for (i = 3; i >= 0; i--, party++) {
            if (!(party->flag & 1)) {
                continue;
            }
            if (party->flag & 8) {
                continue;
            }
            if (party->flag & 0x10) {
                continue;
            }
            if (party->flag & 0x20) {
                continue;
            }
            a->aiselect |= one << party->formindex;
        }
        return 1;
    }
    return 0;
}

s32 Chk_MYBAD(nbActionProcessData_t *a, s32 n)
{
    if ((a->work->badstatus & 0xFFF) == n) {
        a->aiselect = 1 << a->form->formindex;
        return 1;
    }
    return 0;
}

s32 Chk_FRBAD(nbActionProcessData_t *a, s32 n)
{
    s32 i;
    s32 one;

    one = 1;
    for (i = 4; i < 14; i++) {
        if (!(a->data->party[i].flag & 1)) {
            continue;
        }
        if (a->data->party[i].flag & 8) {
            continue;
        }
        if (a->data->party[i].flag & 0x10) {
            continue;
        }
        if (a->data->party[i].flag & 0x20) {
            continue;
        }
        if ((nbGetUnitWorkPointer(a->data, &a->data->party[i])->badstatus & 0xFFF) != n) {
            continue;
        }
        a->aiselect |= one << a->data->party[i].formindex;
    }
    return a->aiselect != 0;
}

s32 Chk_ENBAD(nbActionProcessData_t *a, s32 n)
{
    s32 i;
    s32 one;

    one = 1;
    for (i = 0; i < 4; i++) {
        if (!(a->data->party[i].flag & 1)) {
            continue;
        }
        if (a->data->party[i].flag & 8) {
            continue;
        }
        if (a->data->party[i].flag & 0x10) {
            continue;
        }
        if (a->data->party[i].flag & 0x20) {
            continue;
        }
        if ((nbGetUnitWorkPointer(a->data, &a->data->party[i])->badstatus & 0xFFF) != n) {
            continue;
        }
        a->aiselect |= one << a->data->party[i].formindex;
    }
    return a->aiselect != 0;
}

s32 Chk_ENALLBAD(nbActionProcessData_t *a, s32 n)
{
    s32 i;
    s32 cnt;
    u32 bit;
    s32 one;

    one = 1;
    cnt = 0;
    bit = 0;
    for (i = 0; i < 4; i++) {
        if (!(a->data->party[i].flag & 1)) {
            continue;
        }
        if (a->data->party[i].flag & 8) {
            continue;
        }
        if (a->data->party[i].flag & 0x10) {
            continue;
        }
        if (a->data->party[i].flag & 0x20) {
            continue;
        }
        if ((nbGetUnitWorkPointer(a->data, &a->data->party[i])->badstatus & 0xFFF) != n) {
            continue;
        }
        cnt++;
        bit |= one << a->data->party[i].formindex;
    }
    if (cnt == a->data->playerpcnt) {
        a->aiselect = bit;
        return 1;
    }
    return 0;
}

s32 Chk_ENID(nbActionProcessData_t *a, s32 n)
{
    datUnitWork_t *work;
    s32 i;
    s32 one;

    one = 1;
    for (i = 0; i < 4; i++) {
        if (!(a->data->party[i].flag & 1)) {
            continue;
        }
        if (a->data->party[i].flag & 8) {
            continue;
        }
        if (a->data->party[i].flag & 0x10) {
            continue;
        }
        if (a->data->party[i].flag & 0x20) {
            continue;
        }
        work = nbGetUnitWorkPointer(a->data, &a->data->party[i]);
        if (work->flag & 4) {
            continue;
        }
        if (work->id != n) {
            continue;
        }
        a->aiselect |= one << a->data->party[i].formindex;
    }
    return a->aiselect != 0;
}

s32 Chk_FRID(nbActionProcessData_t *a, s32 n)
{
    s32 i;
    s32 one;

    one = 1;
    for (i = 4; i < 14; i++) {
        if (i == a->partyindex) {
            continue;
        }
        if (!(a->data->party[i].flag & 1)) {
            continue;
        }
        if (a->data->party[i].flag & 8) {
            continue;
        }
        if (a->data->party[i].flag & 0x10) {
            continue;
        }
        if (a->data->party[i].flag & 0x20) {
            continue;
        }
        if (nbGetUnitWorkPointer(a->data, &a->data->party[i])->id != n) {
            continue;
        }
        a->aiselect |= one << a->data->party[i].formindex;
    }
    return a->aiselect != 0;
}

s32 ChkAiHojo(s32 formindex, s32 n)
{
    switch (n) {
    case 0:
        if (nbGetHojoCounter(formindex, 4) > 0) {
            return 1;
        }
        break;
    case 1:
        if (nbGetHojoCounter(formindex, 4) < 0) {
            return 1;
        }
        break;
    case 2:
        if (nbGetHojoCounter(formindex, 5) > 0) {
            return 1;
        }
        break;
    case 3:
        if (nbGetHojoCounter(formindex, 5) < 0) {
            return 1;
        }
        break;
    case 4:
        if (nbGetHojoCounter(formindex, 6) > 0) {
            return 1;
        }
        break;
    case 5:
        if (nbGetHojoCounter(formindex, 6) < 0) {
            return 1;
        }
        break;
    case 6:
        if (nbGetHojoCounter(formindex, 7) > 0) {
            return 1;
        }
        break;
    case 7:
        if (nbGetHojoCounter(formindex, 7) < 0) {
            return 1;
        }
        break;
    case 8:
        if (nbGetHojoCounter(formindex, 8) > 0) {
            return 1;
        }
        break;
    case 9:
        if (nbGetHojoCounter(formindex, 8) < 0) {
            return 1;
        }
        break;
    case 11:
        if (nbGetHojoCounter(formindex, 9) > 0) {
            return 1;
        }
        break;
    case 12:
        if (nbGetHojoCounter(formindex, 10) > 0) {
            return 1;
        }
        break;
    case 13:
        if (nbGetHojoCounter(formindex, 11) > 0) {
            return 1;
        }
        break;
    case 14:
        if (nbGetHojoCounter(formindex, 12) > 0) {
            return 1;
        }
        break;
    case 15:
        if (nbGetHojoCounter(formindex, 13) > 0) {
            return 1;
        }
        break;
    case 16:
        if (nbGetHojoCounter(formindex, 14) > 0) {
            return 1;
        }
        break;
    case 17:
        if (nbGetHojoCounter(formindex, 15) > 0) {
            return 1;
        }
        break;
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbAi", ChkAiHojoMax);

s32 Chk_ENHOJO(nbActionProcessData_t *a, s32 n)
{
    s32 i;
    s32 one;

    one = 1;
    for (i = 0; i < 4; i++) {
        if (!(a->data->party[i].flag & 1)) {
            continue;
        }
        if (a->data->party[i].flag & 8) {
            continue;
        }
        if (a->data->party[i].flag & 0x10) {
            continue;
        }
        if (a->data->party[i].flag & 0x20) {
            continue;
        }
        if (ChkAiHojo(a->data->party[i].formindex, n)) {
            a->aiselect |= one << a->data->party[i].formindex;
        }
    }
    return a->aiselect != 0;
}

s32 Chk_FRHOJO(nbActionProcessData_t *a, s32 n)
{
    s32 i;
    s32 one;

    one = 1;
    for (i = 4; i < 14; i++) {
        if (!(a->data->party[i].flag & 1)) {
            continue;
        }
        if (a->data->party[i].flag & 8) {
            continue;
        }
        if (a->data->party[i].flag & 0x10) {
            continue;
        }
        if (a->data->party[i].flag & 0x20) {
            continue;
        }
        if (ChkAiHojo(a->data->party[i].formindex, n)) {
            a->aiselect |= one << a->data->party[i].formindex;
        }
    }
    return a->aiselect != 0;
}

/* NON_MATCHING: 1 of 55 words differ (first at +0x6C). */
#ifdef NON_MATCHING
s32 Chk_ENHOJOMAX(nbActionProcessData_t *a, s32 n)
{
    s32 i;
    s32 one;

    one = 1;
    for (i = 0; i < 4; i++) {
        if (!(a->data->party[i].flag & 1)) {
            continue;
        }
        if (a->data->party[i].flag & 8) {
            continue;
        }
        if (a->data->party[i].flag & 0x10) {
            continue;
        }
        if (a->data->party[i].flag & 0x20) {
            continue;
        }
        if (ChkAiHojoMax(a->data->party[i].formindex, n) == 0) {
            continue;
        }
        a->aiselect |= one << a->data->party[i].formindex;
    }
    return a->aiselect != 0;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbAi", Chk_ENHOJOMAX);
#endif

/* NON_MATCHING: 1 of 55 words differ (first at +0x6C). */
#ifdef NON_MATCHING
s32 Chk_FRHOJOMAX(nbActionProcessData_t *a, s32 n)
{
    s32 i;
    s32 one;

    one = 1;
    for (i = 4; i < 14; i++) {
        if (!(a->data->party[i].flag & 1)) {
            continue;
        }
        if (a->data->party[i].flag & 8) {
            continue;
        }
        if (a->data->party[i].flag & 0x10) {
            continue;
        }
        if (a->data->party[i].flag & 0x20) {
            continue;
        }
        if (ChkAiHojoMax(a->data->party[i].formindex, n) == 0) {
            continue;
        }
        a->aiselect |= one << a->data->party[i].formindex;
    }
    return a->aiselect != 0;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbAi", Chk_FRHOJOMAX);
#endif

s32 Chk_BA_286248(nbActionProcessData_t *a, s32 n)
{
    nbParty_t *party;
    s32 i;
    s32 one;

    one = 1;
    if (a->data->stat & 0x10) {
        party = a->data->party;
        for (i = 3; i >= 0; i--, party++) {
            if (!(party->flag & 1)) {
                continue;
            }
            if (party->flag & 8) {
                continue;
            }
            if (party->flag & 0x10) {
                continue;
            }
            if (party->flag & 0x20) {
                continue;
            }
            a->aiselect |= one << party->formindex;
        }
        return 1;
    }
    return 0;
}

s32 Chk_POW_2862D0(nbActionProcessData_t *a, s32 n)
{
    nbParty_t *party;
    s32 i;
    s32 one;

    one = 1;
    if (a->data->stat & 0x80) {
        party = a->data->party;
        for (i = 3; i >= 0; i--, party++) {
            if (!(party->flag & 1)) {
                continue;
            }
            if (party->flag & 8) {
                continue;
            }
            if (party->flag & 0x10) {
                continue;
            }
            if (party->flag & 0x20) {
                continue;
            }
            a->aiselect |= one << party->formindex;
        }
        return 1;
    }
    return 0;
}

s32 Chk_ENLIGHT(nbActionProcessData_t *a, s32 n)
{
    s32 i;
    datUnitWork_t *work;    for (i = 0; i < 4; i++) {
        if (!(a->data->party[i].flag & 1)) {
            continue;
        }
        if (a->data->party[i].flag & 8) {
            continue;
        }
        if (a->data->party[i].flag & 0x10) {
            continue;
        }
        if (a->data->party[i].flag & 0x20) {
            continue;
        }
        work = nbGetUnitWorkPointer(a->data, &a->data->party[i]);
        if (work->flag & 4) {
            continue;
        }
        if (datGet3Zokusei(work) != 1) {
            continue;
        }
        a->aiselect |= 1 << a->data->party[i].formindex;
    }
    return a->aiselect != 0;
}

s32 Chk_ENDARK(nbActionProcessData_t *a, s32 n)
{
    s32 i;
    datUnitWork_t *work;    for (i = 0; i < 4; i++) {
        if (!(a->data->party[i].flag & 1)) {
            continue;
        }
        if (a->data->party[i].flag & 8) {
            continue;
        }
        if (a->data->party[i].flag & 0x10) {
            continue;
        }
        if (a->data->party[i].flag & 0x20) {
            continue;
        }
        work = nbGetUnitWorkPointer(a->data, &a->data->party[i]);
        if (work->flag & 4) {
            continue;
        }
        if (datGet3Zokusei(work) != -1) {
            continue;
        }
        a->aiselect |= 1 << a->data->party[i].formindex;
    }
    return a->aiselect != 0;
}

s32 Chk_SENSEI_286530(nbActionProcessData_t *a)
{
    if (ChkAiHojo(a->form->formindex, 0x11) != 0) {
        a->aiselect = 1 << a->form->formindex;
        return 1;
    }
    return 0;
}

s32 Chk_PLV(nbActionProcessData_t *a, s32 n)
{
    if (datGetPlayerLevel() < n) {
        return 0;
    }
    return 1;
}

s32 Chk_PPARTYLV(nbActionProcessData_t *a, s32 n)
{
    if (nbGetLvAvg(a->data, 0) < n) {
        return 0;
    }
    return 1;
}

s32 Chk_TURN(nbActionProcessData_t *a, s32 n)
{
    if (a->data->turn < n) {
        return 0;
    }
    return 1;
}

s32 Chk_ENHAMAMHK(nbActionProcessData_t *a, s32 n)
{
    s32 i;
    for (i = 0; i < 4; i++) {
        if (!(a->data->party[i].flag & 1)) {
            continue;
        }
        if (a->data->party[i].flag & 8) {
            continue;
        }
        if (a->data->party[i].flag & 0x10) {
            continue;
        }
        if (a->data->party[i].flag & 0x20) {
            continue;
        }
        if (datGetAisyo(nbGetUnitWorkPointer(a->data, &a->data->party[i]), 6) & 0x70000) {
            continue;
        }
        a->aiselect |= 1 << a->data->party[i].formindex;
    }
    return a->aiselect != 0;
}

s32 Chk_SKILLNOMP(nbActionProcessData_t *a, s32 n)
{
    if (nbCheckSkillCost(a->form->formindex, n) != 0) {
        a->aiselect = 1 << a->form->formindex;
        return 1;
    }
    return 0;
}

s32 nbAiChkJoken(nbActionProcessData_t *a, u32 jo)
{
    u32 prev;
    s32 type;
    s32 para;

    type = jo >> 16;
    para = jo & 0xFFFF;
    if (type == 0) {
        return 1;
    }
    prev = a->aiselect;
    a->aiselect = 0;
    if (D_004F0788[type](a, para) == 0) {
        return 0;
    }
    if (prev != 0) {
        a->aiselect = prev & a->aiselect;
    }
    return 1;
}

/* NON_MATCHING: 11 of 54 words differ (first at +0x78). */
#ifdef NON_MATCHING
s32 nbGetAiEntry(nbActionProcessData_t *a, s32 index)
{
    u32 j1;
    u32 j2;

    a->aiselect = 0;
    j1 = D_003989E0[a->work->id].entry[index].joken[0];
    j2 = D_003989E0[a->work->id].entry[index].joken[1];
    a->aijokentbl = index;
    if (nbAiChkJoken(a, j1) == 0) {
        return 0;
    }
    if (nbAiChkJoken(a, j2) == 0) {
        return 0;
    }
    if ((j1 >> 16) == 4 || (j2 >> 16) == 4) {
        nbSetHojoCounter(a->form->formindex, index + 1, 0);
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbAi", nbGetAiEntry);
#endif

/* NON_MATCHING: 2 of 70 words differ (first at +0xB8). */
#ifdef NON_MATCHING
s32 nbPickAiAction(nbActionProcessData_t *a, s32 tbl)
{
    s32 list[8];
    datDevilAITbl_t *row;
    s32 cnt;
    s32 total;
    s32 acc;
    s32 r;
    s32 i;

    cnt = 0;
    total = 0;
    row = (datDevilAITbl_t *)(D_00398A08 + a->work->id * 164 + tbl * 40);
    for (i = 0; i < 5; i++) {
        if ((s8)a->aichktbl[i] <= 0) {
            list[cnt++] = i;
            total += row[i].ritu;
        }
    }
    if (cnt == 0) {
        return -1;
    }
    if (total == 0) {
        return -1;
    }
    r = dds3GetRandIntA(0, total);
    acc = 0;
    for (i = 0; i < cnt; i++) {
        if (r < acc + row[list[i]].ritu) {
            return list[i];
        }
        acc += row[list[i]].ritu;
    }
    return -1;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbAi", nbPickAiAction);
#endif

INCLUDE_ASM(const s32, "newbattle/nbAi", SetKoudou);

INCLUDE_ASM(const s32, "newbattle/nbAi", Sel_AI);

void Sel_LIFEMIN(nbActionProcessData_t *a, s32 n)
{
    datUnitWork_t *work;
    s32 i;
    s32 index;
    s32 min;

    index = -1;
    min = 10000;
    for (i = 0; i < 16; i++) {
        if (!(a->data->form[i].stat & 4)) {
            continue;
        }
        work = nbGetUnitWorkFromFormindex(i);
        if (work->hp < min) {
            min = work->hp;
            index = i;
        }
    }
    if (index >= 0) {
        a->data->form[index].stat |= 8;
    }
}

void Sel_BAARLV(nbActionProcessData_t *a, s32 n)
{
    datUnitWork_t *work;
    s32 i;
    s32 index;
    s32 min;

    index = -1;
    min = 10000;
    for (i = 0; i < 16; i++) {
        if (!(a->data->form[i].stat & 4)) {
            continue;
        }
        work = nbGetUnitWorkFromFormindex(i);
        if ((work->badstatus & 0xFFF) == 0x200) {
            continue;
        }
        if (work->level < min) {
            min = work->level;
            index = i;
        }
    }
    if (index >= 0) {
        a->data->form[index].stat |= 8;
    }
}

void Sel_LVMIN(nbActionProcessData_t *a, s32 n)
{
    datUnitWork_t *work;
    s32 i;
    s32 index;
    s32 min;

    index = -1;
    min = 10000;
    for (i = 0; i < 16; i++) {
        if (!(a->data->form[i].stat & 4)) {
            continue;
        }
        work = nbGetUnitWorkFromFormindex(i);
        if (work->level < min) {
            min = work->level;
            index = i;
        }
    }
    if (index >= 0) {
        a->data->form[index].stat |= 8;
    }
}

void Sel_BAD(nbActionProcessData_t *a, s32 n)
{
    s32 i;

    for (i = 0; i < 16; i++) {
        if (!(a->data->form[i].stat & 4)) {
            continue;
        }
        if ((nbGetUnitWorkFromFormindex(i)->badstatus & 0xFFF) != n) {
            continue;
        }
        a->data->form[i].stat |= 8;
    }
}

void Sel_ID(nbActionProcessData_t *a, s32 n)
{
    datUnitWork_t *work;
    s32 i;

    for (i = 0; i < 16; i++) {
        if (!(a->data->form[i].stat & 4)) {
            continue;
        }
        work = nbGetUnitWorkFromFormindex(i);
        if (work->flag & 4) {
            continue;
        }
        if (work->id != n) {
            continue;
        }
        a->data->form[i].stat |= 8;
    }
}

void Sel_LIGHT(nbActionProcessData_t *a, s32 n)
{
    datUnitWork_t *work;
    s32 i;

    for (i = 0; i < 16; i++) {
        if (!(a->data->form[i].stat & 4)) {
            continue;
        }
        work = nbGetUnitWorkFromFormindex(i);
        if (work->flag & 4) {
            continue;
        }
        if (datGet3Zokusei(work) != 1) {
            continue;
        }
        a->data->form[i].stat |= 8;
    }
}

void Sel_DARK(nbActionProcessData_t *a, s32 n)
{
    datUnitWork_t *work;
    s32 i;

    for (i = 0; i < 16; i++) {
        if (!(a->data->form[i].stat & 4)) {
            continue;
        }
        work = nbGetUnitWorkFromFormindex(i);
        if (work->flag & 4) {
            continue;
        }
        if (datGet3Zokusei(work) != -1) {
            continue;
        }
        a->data->form[i].stat |= 8;
    }
}

void Sel_PLAYER(nbActionProcessData_t *a, s32 n)
{
    s32 i;

    for (i = 0; i < 16; i++) {
        if (!(a->data->form[i].stat & 4)) {
            continue;
        }
        if (!(nbGetUnitWorkFromFormindex(i)->flag & 4)) {
            continue;
        }
        a->data->form[i].stat |= 8;
    }
}

void Sel_JOKEN(nbActionProcessData_t *a)
{
    nbFormation_t *form;
    s32 i;
    s32 one;

    form = a->data->form;
    for (i = 0; i < 16; i++) {
        one = 1;
        if ((form->stat & 4) && (a->aiselect & (one << i))) {
            form->stat |= 8;
        }
        form++;
    }
}

void Sel_RND(nbActionProcessData_t *a)
{
    nbFormation_t *form;
    s32 i;

    form = a->data->form;
    for (i = 0; i < 16; i++) {
        if (form->stat & 4) {
            form->stat |= 8;
        }
        form++;
    }
}

INCLUDE_ASM(const s32, "newbattle/nbAi", SetSingleTarget2);

s32 SetGroupTarget2(nbActionProcessData_t *a, s32 code, s32 n)
{
    nbFormation_t *form;
    s32 i;
    s32 ret;

    ret = 0;
    switch (code) {
    case TAR_CODE_AI:
        break;
    case TAR_CODE_LIFEMIN:
    case TAR_CODE_BAD:
    case TAR_CODE_ID:
    case TAR_CODE_LIGHT:
    case TAR_CODE_DARK:
    case TAR_CODE_PLAYER:
    case TAR_CODE_JOKEN:
    case TAR_CODE_RND:
    case TAR_CODE_BAARLV:
    case TAR_CODE_LVMIN:
        form = a->data->form;
        for (i = 0; i < 16; i++) {
            if (form->stat & 4) {
                form->stat |= 8;
            }
            form++;
        }
        break;
    default:
        code = 0;
        break;
    }
    for (i = 0; i < 16; i++) {
        if (a->data->form[i].stat & 8) {
            break;
        }
    }
    if (i == 16) {
        code = 0;
        n = 0;
    }
    if (code == 0) {
        ret = Sel_AI(a, n, 1);
    }
    return ret;
}

/* NON_MATCHING: 15 of 49 words differ (first at +0x3C). */
#ifdef NON_MATCHING
s32 ChkCost(nbActionProcessData_t *a)
{
    s32 skill;
    s32 ret;
    s32 ailevel;

    ret = 0;
    switch (a->work->nowcommand) {
    case 0:
        skill = 0;
        break;
    case 1:
    case 5:
        skill = a->work->nowindex;
        break;
    default:
        return 0;
    }
    if (nbCheckSkillCost(a->form->formindex, skill) != 0) {
        ailevel = D_003989E0[a->work->id].ailevel;
        if (ailevel < 3) {
            ret = ailevel != 0;
        }
    }
    return ret;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbAi", ChkCost);
#endif

s32 ChkClose(nbActionProcessData_t *a)
{
    s32 nowindex;
    s32 ret;

    ret = 1;
    switch (a->work->nowcommand) {
    case 0:
        nowindex = 0;
        break;
    case 1:
    case 5:
        nowindex = a->work->nowindex;
        break;
    default:
        return ret;
    }
    if (nbCheckSkillUse(a->form->formindex, nowindex) >= 1) {
        switch (D_003989E0[a->work->id].ailevel) {
        case 0:
            break;
        case 1:
        case 2:
            ret = 0;
            break;
        }
    }
    return ret;
}

INCLUDE_ASM(const s32, "newbattle/nbAi", nbSetAiTarget);

INCLUDE_ASM(const s32, "newbattle/nbAi", nbRunAiSub);

/* NON_MATCHING: 1 of 47 words differ (first at +0x3C). */
#ifdef NON_MATCHING
void nbRunAi(nbActionProcessData_t *a)
{
    s32 i;
    s32 no;

    no = 0;
    for (i = 0; i < 3; i++) {
        if (nbGetAiEntry(a, i) != 0) {
            no = D_003989EC[a->work->id][i * 3];
            break;
        }
    }
    printf(D_00545840, no);
    nbRunAiSub(a, no);
}
#else
INCLUDE_ASM(const s32, "newbattle/nbAi", nbRunAi);
#endif

INCLUDE_RODATA(const s32, "newbattle/nbAi", D_00545850);

INCLUDE_RODATA(const s32, "newbattle/nbAi", D_00545868);

