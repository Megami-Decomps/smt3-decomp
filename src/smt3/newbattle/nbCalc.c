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
    s32 encpackno;
    s32 packno;
    s32 packindex;
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
    u32 unk140;
    u32 d31_rebehansya;
    u32 d31_andahansya;
    u32 d31_kantuu; /* Native affinity override flag at 0x14C. */
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

typedef struct
{
    s8 skillattr;
    s8 type;
    s16 index;
    u16 keisyoform;
    s8 capacity;
    u8 flag;
} datSkill_t;

/* Only the fields this unit touches are named; the entry is 38 bytes. */
typedef struct
{
    s8 backAttack;
    s8 esc;
    u8 item;
    u8 itemcnt;
    u8 unk_04[0x2];
    u16 id[11];
    u8 unk_1C[0x4];
    /* 0x20 */ u16 senseiFlags;
    u8 unk_22;
    u8 unk_23;
    u16 mae;
} nbEncountForm_t;

extern s32 printf(const char *format, ...);

extern nbMainProcessData_t *nbGetMainProcessData(void);

extern datUnitWork_t *nbGetUnitWorkFromFormindex(s32 formindex);

extern u32 nbGetBadStat(s32 formindex);

extern datUnitWork_t *nbGetUnitWorkPointer(nbMainProcessData_t *data, nbParty_t *party);

extern void datAddHp(datUnitWork_t *work, s32 hp);

extern void datAddMp(datUnitWork_t *work, s32 mp);

extern void datSetBadStat(datUnitWork_t *work, s32 badstat);

extern void datResetBadStat(datUnitWork_t *work, s32 badstat);

extern s32 datGet3Zokusei(datUnitWork_t *work);

extern datSkillTable_t datSkillTable;

extern datSkill_t attr[];

extern nbEncountForm_t nbEncountFormTable[];

extern nbParty_t *nbGetPartyFromFormindex(s32 formindex);

extern s32 nbGetButuriAttack(s32 nskill, s32 sformindex, s32 dformindex, s32 waza);

extern s32 datCheckSyojiSkill(datUnitWork_t *work, s32 skill);

extern s32 nbHatudoCheckSkill(datUnitWork_t *work, s32 skill);

extern s32 evtGetAgeOfMoon(void);

extern u32 dds3GetRandIntA(void *state, u32 limit);

extern s32 nbCheckSkillCost(s32 formindex, s32 nskill);

extern datUnitVisual_t *nbGetUnitVisual(datUnitWork_t *work);

extern datDevilFormat_t datDevilTable[];

extern u32 nbGetAisyo(s32 nskill, s32 formindex, s32 attr);

extern s32 nbGetHojoCounter(s32 formindex, s32 type);

extern s32 nbGetAisyoTable(s32 formindex);

extern s32 nbGetEscapeSwitch(nbMainProcessData_t *data);

extern f32 nbGetHojoRitu(s32 formindex, s32 type);

extern char D_00544D30[];

extern long sdfFloatToULong(f32 value);

extern s32 nbGetLvAvg(nbMainProcessData_t *data, s32 side);

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

extern void nbSetHojoCounter(s32 formindex, s32 type, s32 point);

extern s32 nbGetHojoCounterMax(s32 hojotype);

extern s32 nbGetHojoCounterMin(s32 hojotype);

extern s32 (*D_00527420[])(nbEventProcessData_t *e);

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

INCLUDE_ASM(const s32, "newbattle/nbCalc", nbGetHitType);

s32 nbGet3Zokusei(s32 formindex)
{
    datUnitWork_t *unit;
    s32 z;

    unit = nbGetUnitWorkFromFormindex(formindex);
    z = datGet3Zokusei(unit);
    switch (z) {
    case 0:
        return 1;
    case 1:
        return 2;
    case -1:
        return 0;
    }
    return 1;
}

s32 nbCheckKoukaJyoken(s32 nskill, s32 sformindex, s32 dformindex)
{
    s32 hittype;
    s32 zoku;

    hittype = datSkillTable.normal[nskill].hittype;
    zoku = nbGet3Zokusei(dformindex);
    if ((hittype & 1) != 0) {
        return 1;
    }
    if ((hittype & 2) != 0 && zoku == 0) {
        return 1;
    }
    if ((hittype & 4) != 0 && zoku == 1) {
        return 1;
    }
    if ((hittype & 8) != 0 && zoku == 2) {
        return 1;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "newbattle/nbCalc", D_005449E0);

INCLUDE_RODATA(const s32, "newbattle/nbCalc", D_005449F8);

INCLUDE_ASM(const s32, "newbattle/nbCalc", nbGetKoukaType);

void nbAddHpMp(s32 formindex, s32 type, s32 n)
{
    datUnitWork_t *unit;

    unit = nbGetUnitWorkFromFormindex(formindex);
    if (unit == NULL) {
        return;
    }
    switch (type) {
    case 0:
        datAddHp(unit, n);
        break;
    case 1:
        datAddMp(unit, n);
        break;
    }
}

f32 nbGetAisyoRitu(s32 nskill, s32 sformindex, s32 dformindex)
{
    nbMainProcessData_t *mainData;
    datUnitWork_t *source;
    datUnitWork_t *target;
    s32 skillattr;
    s32 ratio;
    u32 aisyo;
    f32 ritu;

    mainData = nbGetMainProcessData();
    source = nbGetUnitWorkFromFormindex(sformindex);
    target = nbGetUnitWorkFromFormindex(dformindex);
    skillattr = nbGetNormalSkillAttr(nskill);
    ritu = 1.0f;

    switch (skillattr) {
    case -1:
    case 12:
    case 13:
    case 14:
        break;
    default:
        aisyo = nbGetAisyo(nskill, dformindex, skillattr);
        ratio = aisyo & 0xFFFF;
        if ((aisyo & 0x60000) != 0) {
            if (ratio == 0) {
                ratio = 100;
            }
        }
        if ((target->badstatus & 0xFFF) == 2 &&
            datSkillTable.normal[nskill].koukatype == 0 && ratio < 100) {
            ratio = 100;
        }
        if (datCheckSyojiSkill(source, 0x165) != 0 &&
            datSkillTable.normal[nskill].koukatype == 0 && ratio < 100) {
            ratio = 100;
            mainData->d31_kantuu = 1;
        }
        ritu = (f32)ratio * 0.01f;
        break;
    }

    if (datCheckSyojiSkill(source, 0x169) != 0) {
        ritu *= 1.5f;
    } else {
        switch (skillattr) {
        case 1:
            if (nbHatudoCheckSkill(source, 0x135) != 0) {
                ritu *= 1.5f;
            }
            break;
        case 2:
            if (nbHatudoCheckSkill(source, 0x136) != 0) {
                ritu *= 1.5f;
            }
            break;
        case 3:
            if (nbHatudoCheckSkill(source, 0x137) != 0) {
                ritu *= 1.5f;
            }
            break;
        case 4:
            if (nbHatudoCheckSkill(source, 0x138) != 0) {
                ritu *= 1.5f;
            }
            break;
        default:
            return ritu;
        }
    }
    return ritu;
}

INCLUDE_ASM(const s32, "newbattle/nbCalc", nbGetKoukaHp);

INCLUDE_ASM(const s32, "newbattle/nbCalc", nbGetKoukaMp);

f32 nbGetHpDrainRitu(s32 nskill, s32 sformindex, s32 dformindex)
{
    s32 hptype;

    hptype = datSkillTable.normal[nskill].hptype;
    if (hptype < 14) {
        if (hptype >= 12) {
            return 1.0f;
        }
    }
    return 0.0f;
}

f32 nbGetMpDrainRitu(s32 nskill, s32 sformindex, s32 dformindex)
{
    s32 mptype;

    mptype = datSkillTable.normal[nskill].mptype;
    if (mptype < 14) {
        if (mptype >= 12) {
            return 1.0f;
        }
    }
    return 0.0f;
}

f32 nbGetGensyoRitu(s32 nskill, s32 sformindex, s32 dformindex)
{
    return datSkillTable.normal[nskill].minus / 100.0f;
}

INCLUDE_ASM(const s32, "newbattle/nbCalc", nbGetKoukaBadDamage);

INCLUDE_RODATA(const s32, "newbattle/nbCalc", D_00544BA8);

INCLUDE_ASM(const s32, "newbattle/nbCalc", nbGetKoukaBadKaifuku);

u32 nbCheckHitEffect(s32 nskill, s32 sformindex, s32 dformindex)
{
    return datSkillTable.visual[nskill].bedno == 0;
}

s32 nbGetDeadType(s32 nskill, s32 sformindex, s32 dformindex)
{
    return 0;
}

extern char D_00544BF8[];

s32 nbCheckStoneDead(s32 nskill, s32 dformindex)
{
    datUnitWork_t *work;
    s32 skillattr;
    s32 param;
    s32 chance;
    f32 divisor;
    f32 rate;

    work = nbGetUnitWorkFromFormindex(dformindex);
    if (work == NULL) {
        return 0;
    }
    if (nbGetBadStat(dformindex) != 0x400) {
        return 0;
    }
    skillattr = nbGetNormalSkillAttr(nskill);
    if (skillattr != 0 && skillattr != 4) {
        return 0;
    }
    param = datGetParam(work, 5);
    divisor = ((f32)work->level + 20.0f) / 5.0f;
    rate = (f32)param / divisor;
    if ((work->flag & 4) != 0) {
        chance = 100.0f - rate * 40.0f;
    } else {
        chance = 100.0f - rate * 20.0f;
    }
    if (chance < 20) {
        chance = 20;
    }
    printf(D_00544BF8, chance);
    return (s32)dds3GetRandIntA(0, 100) < chance;
}

void nbSetBadStat(s32 formindex, s32 sw, s32 badstat)
{
    datUnitWork_t *unit;

    unit = nbGetUnitWorkFromFormindex(formindex);
    if (unit == NULL) {
        return;
    }
    if (sw != 0) {
        datSetBadStat(unit, badstat);
    } else {
        datResetBadStat(unit, badstat);
    }
}

INCLUDE_ASM(const s32, "newbattle/nbCalc", nbCheckBadKaifuku);

u32 nbGetBadStat(s32 formindex)
{
    datUnitWork_t *unit;

    unit = nbGetUnitWorkFromFormindex(formindex);
    if (unit == NULL) {
        return 0;
    }
    return unit->badstatus & 0xFFF;
}

s32 nbCheckBadCurse(s32 formindex)
{
    datUnitWork_t *unit;
    s32 bad;

    unit = nbGetUnitWorkFromFormindex(formindex);
    if (unit == NULL) {
        return 0;
    }
    bad = unit->badstatus & 0x8000;
    return bad != 0;
}

s32 nbGetAisyoTable(s32 formindex)
{
    datUnitWork_t *w;
    s32 ret;
    s32 cnt;

    w = nbGetUnitWorkFromFormindex(formindex);
    ret = 0;
    if ((w->flag & 4) == 0) {
        ret = datDevilTable[w->id].aisyoid;
    }
    cnt = nbGetHojoCounter(formindex, 0x13);
    if (cnt > 0) {
        ret = cnt;
    }
    switch (w->badstatus & 0xFFF) {
    case 0x400:
        ret = 0xF6;
        break;
    case 0x200:
        ret = 0xF7;
        break;
    }
    return ret;
}

INCLUDE_RODATA(const s32, "newbattle/nbCalc", D_00544BF8);

INCLUDE_RODATA(const s32, "newbattle/nbCalc", D_00544C10);

INCLUDE_ASM(const s32, "newbattle/nbCalc", nbGetAisyo);

s32 nbGetNormalSkillAttr(s32 nskill)
{
    return attr[nskill].skillattr;
}

void nbSetHojoCounter(s32 formindex, s32 type, s32 point)
{
    nbParty_t *party;

    party = nbGetPartyFromFormindex(formindex);
    if (party != NULL) {
        party->count[type] = point;
    }
}

s32 nbGetHojoCounter(s32 formindex, s32 type)
{
    nbParty_t *party;

    party = nbGetPartyFromFormindex(formindex);
    if (party == NULL) {
        return 0;
    }
    return party->count[type];
}

s32 nbGetHojoCounterMax(s32 hojotype)
{
    s32 max;

    max = 0;
    switch (hojotype) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 16:
    case 17:
    case 18:
        max = 127;
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        max = 4;
        break;
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
        max = 1;
        break;
    case 19:
        max = 32767;
        break;
    }
    return max;
}

s32 nbGetHojoCounterMin(s32 hojotype)
{
    s32 min;

    min = 0;
    switch (hojotype) {
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        min = -4;
        break;
    case 0:
    case 1:
    case 2:
    case 3:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
        min = 0;
        break;
    }
    return min;
}

s32 nbSetHojoAddCounter(s32 formindex, s32 type, s32 point, s32 nvirtual)
{
    nbParty_t *party;
    s32 old;
    s32 n;

    party = nbGetPartyFromFormindex(formindex);
    if (party == NULL) {
        return 0;
    }
    old = party->count[type];
    n = old + point;
    if (type < 20) {
        if (type >= 0) {
            if (nbGetHojoCounterMax(type) < n) {
                n = nbGetHojoCounterMax(type);
            }
            if (n < nbGetHojoCounterMin(type)) {
                n = nbGetHojoCounterMin(type);
            }
        }
    }
    if (nvirtual == 0) {
        party->count[type] = n;
    }
    return n - old;
}

INCLUDE_ASM(const s32, "newbattle/nbCalc", nbSetHojoKouka);

f32 nbGetHojoRitu(s32 formindex, s32 type)
{
    datUnitWork_t *work;
    s32 count;
    s32 moon;
    f32 ritu;

    work = nbGetUnitWorkFromFormindex(formindex);
    ritu = 1.0f;
    count = nbGetHojoCounter(formindex, type);
    if (work->id == 0x125) {
        moon = evtGetAgeOfMoon() - 4;
        count += moon / 2;
    }
    if (type < 8) {
        if (type < 6) {
            if (count < 0) {
                ritu = ((f32)count + 8.0f) * 0.125f;
            } else if (count > 0) {
                ritu = ((f32)(2 * count) + 8.0f) * 0.125f;
            }
        } else {
            if (count < 0) {
                ritu = ((f32)((-count) * 2) + 8.0f) * 0.125f;
            } else if (count > 0) {
                ritu = ((f32)(-count) + 8.0f) * 0.125f;
            }
        }
    } else {
        if (count < 0) {
            ritu = ((f32)count + 8.0f) * 0.125f;
        } else if (count > 0) {
            ritu = ((f32)(2 * count) + 8.0f) * 0.125f;
        }
    }
    if (type == 4 && nbGetHojoCounter(formindex, 15) > 0) {
        ritu *= 2.5f;
    }
    return ritu;
}

INCLUDE_ASM(const s32, "newbattle/nbCalc", nbGetButuriAttack);

s32 nbGetButuriKaifuku(s32 nskill, s32 sformindex, s32 dformindex, s32 waza)
{
    return nbGetButuriAttack(nskill, sformindex, dformindex, waza);
}

INCLUDE_ASM(const s32, "newbattle/nbCalc", nbGetMagicAttack);

s32 nbGetMagicKaifuku(s32 nskill, s32 sformindex, s32 dformindex, s32 waza)
{
    datUnitWork_t *swork;
    datUnitWork_t *dwork;
    char *fmt;
    f32 ritu;
    f32 v;
    s32 base;
    s32 ret;

    swork = nbGetUnitWorkFromFormindex(sformindex);
    dwork = nbGetUnitWorkFromFormindex(dformindex);
    ritu = nbGetHojoRitu(sformindex, 5);
    base = datGetParam(swork, 2) * 4 + swork->level + dds3GetRandIntA(0, 8);
    v = (f32)base / 10.0f * (f32)waza;
    fmt = D_00544D30;
    printf(fmt, waza, sdfFloatToULong(ritu), ret = v * ritu);
    return ret;
}

s32 nbGetMaxHpWazaPoint(s32 nskill, s32 sformindex, s32 dformindex, s32 waza)
{
    datUnitWork_t *swork;
    datUnitWork_t *dwork;
    f32 sritu;
    f32 dritu;
    f32 value;
    f32 base;
    f32 penalty;

    swork = nbGetUnitWorkFromFormindex(sformindex);
    dwork = nbGetUnitWorkFromFormindex(dformindex);
    sritu = nbGetHojoRitu(sformindex, 4);
    dritu = nbGetHojoRitu(dformindex, 7);
    if (swork->flag & 0x20) {
        base = (f32)datGetBaseMaxHp(swork) / 0x1.166666p+6f;
        penalty = 50.0f / (swork->level + 10);
        base = base * (f32)waza;
        base = base * 0x1.333332p-1f;
        value = base - penalty;
        if (!evtBitCheck(0xAF2)) {
            value = value * 0x1.570a3cp+0f;
        }
    } else {
        value = ((f32)swork->maxhp / 0x1.166666p+6f) * (f32)waza * 0x1.999998p-1f;
    }
    value = (value * sritu) * dritu;
    return value;
}

INCLUDE_ASM(const s32, "newbattle/nbCalc", nbGetVirtualAisyo);

INCLUDE_ASM(const s32, "newbattle/nbCalc", nbGetVirtualKoukaPoint);

s32 nbCheckSkillCost(s32 formindex, s32 nskill)
{
    datUnitWork_t *work;
    s32 costtype;
    s32 ret;

    ret = 0;
    work = nbGetUnitWorkFromFormindex(formindex);
    if (datSkillTable.normal[nskill].koukatype == 1 && (work->badstatus & 0xFFF) == 0x20) {
        return 3;
    }
    if (datSkillTable.normal[nskill].flag & 4) {
        return 0;
    }
    costtype = datSkillTable.normal[nskill].costtype;
    switch (costtype) {
    case 1:
        if (datGetSkillCost(work, nskill) >= work->hp) {
            ret = 1;
        }
        break;
    case 2:
        if (datGetSkillCost(work, nskill) > work->mp) {
            ret = costtype;
        }
        break;
    }
    return ret;
}

s32 nbCheckSkillUse(s32 formindex, s32 nskill)
{
    datUnitWork_t *unit;
    s32 cost;

    unit = nbGetUnitWorkFromFormindex(formindex);
    if ((unit->badstatus & 0xFFF) == 0x20) {
        if (datSkillTable.normal[nskill].koukatype == 1) {
            return 1;
        }
    }
    cost = nbCheckSkillCost(formindex, nskill);
    switch (cost) {
    case 1:
        return 2;
    case 2:
        return 3;
    }
    return 0;
}

void nbGetEncountTableItem(nbMainProcessData_t *data)
{
    u8 item;
    u8 itemcnt;
    s32 i;
    nbEncountForm_t *encount;

    if (data->encno <= 0 || datBossRashChk(data->encno) != 0) {
        return;
    }
    encount = &nbEncountFormTable[data->encno];
    item = encount->item;
    itemcnt = encount->itemcnt;
    if (item == 0 || itemcnt == 0) {
        return;
    }
    for (i = 0; i < 4; i++) {
        if (data->item[i] == item) {
            u8 count = data->itemcnt[i];

            count = count + itemcnt;
            data->itemcnt[i] = count;
            return;
        }
    }
    for (i = 0; i < 4; i++) {
        if (data->item[i] == 0) {
            data->item[i] = item;
            data->itemcnt[i] = itemcnt;
            return;
        }
    }
    for (i = 3; i > 0; i--) {
        data->item[i] = data->item[i - 1];
        data->itemcnt[i] = data->itemcnt[i - 1];
    }
    data->item[0] = item;
    data->itemcnt[0] = itemcnt;
}

INCLUDE_ASM(const s32, "newbattle/nbCalc", nbGetExpMakaItem);

s32 nbGetLvAvg(nbMainProcessData_t *data, s32 side)
{
    s32 i;
    s32 cnt;
    s32 sum;

    cnt = 0;
    sum = 0;
    for (i = 0; i < 14; i++) {
        if ((data->party[i].flag & 1) != 0 && (data->party[i].flag & 4) == 0) {
            if (side == 0) {
                if ((data->party[i].flag & 2) != 0) {
                    continue;
                }
            } else {
                if ((data->party[i].flag & 2) == 0) {
                    continue;
                }
            }
            cnt++;
            sum += nbGetUnitWorkPointer(data, &data->party[i])->level;
        }
    }
    if (cnt == 0) {
        return 0;
    }
    return sum / cnt;
}

s32 nbGetLvAvg2(nbMainProcessData_t *data, s32 side)
{
    datUnitWork_t *w;
    s32 i;
    s32 id;
    s32 cnt;
    s32 sum;

    sum = 0;
    cnt = 0;
    if (side == 0) {
        w = (datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54);
        for (i = 0; i < 16; i++) {
            if ((w->flag & 1) != 0) {
                if ((w->flag & 2) != 0) {
                    cnt++;
                    sum += w->level;
                }
            }
            w++;
        }
    } else {
        for (i = 0; i < 11; i++) {
            id = nbEncountFormTable[data->encno].id[i];
            if (id != 0) {
                cnt++;
                sum += datDevilTable[id].level;
            }
        }
    }
    if (cnt == 0) {
        return 0;
    }
    return sum / cnt;
}

s32 nbGetParamAvg(nbMainProcessData_t *data, s32 side, s32 paratype)
{
    s32 i;
    s32 cnt;
    s32 sum;

    cnt = 0;
    sum = 0;
    for (i = 0; i < 14; i++) {
        if ((data->party[i].flag & 1) != 0 && (data->party[i].flag & 4) == 0) {
            if (side == 0) {
                if ((data->party[i].flag & 2) != 0) {
                    continue;
                }
            } else {
                if ((data->party[i].flag & 2) == 0) {
                    continue;
                }
            }
            cnt++;
            sum += datGetParam(nbGetUnitWorkPointer(data, &data->party[i]), paratype);
        }
    }
    if (cnt == 0) {
        return 0;
    }
    return sum / cnt;
}

s32 nbGetParamAvg2(nbMainProcessData_t *data, s32 side, s32 paratype)
{
    datUnitWork_t *w;
    s32 i;
    s32 ofs;
    s32 id;
    s32 cnt;
    s32 sum;

    cnt = 0;
    sum = 0;
    if (side == 0) {
        for (i = 0, ofs = 0; i < 16; i++) {
            w = (datUnitWork_t *)((u8 *)DDS3_GBWK + ofs + 0xA54);
            ofs += 0xEC;
            if ((w->flag & 1) != 0) {
                if ((w->flag & 2) != 0) {
                    cnt++;
                    sum += datGetParam(w, paratype);
                }
            }
        }
    } else {
        for (i = 0; i < 11; i++) {
            id = nbEncountFormTable[data->encno].id[i];
            if (id != 0) {
                cnt++;
                sum += datDevilTable[id].param[paratype];
            }
        }
    }
    if (cnt == 0) {
        return 0;
    }
    return sum / cnt;
}

typedef struct {
    u16 encountid;
    u16 ritu;
    u8 sensei;
    u8 renzokuritu;
    u16 renzokuindex;
} datEncountRitu_t;

typedef struct {
    s32 normalritu;
    datEncountRitu_t ritu[12];
} datEncountTbl_t;

typedef struct {
    u16 stageid;
    u16 areaid;
    u16 btlsound;
    u16 moonchk;
    u16 table2;
    u16 eventbit1;
    u16 eventbit2;
    u8 table1[4];
    u8 reserved12[2];
    datEncountTbl_t enctable[4];
} nbEncountEntry_t;

typedef struct { f32 factor[6]; } nbCheckSenseiFactors_t;

extern nbEncountEntry_t nbEncountTable[];
extern const nbCheckSenseiFactors_t D_00544D58;
extern char D_00544D70[];

s32 nbCheckSensei(nbMainProcessData_t *data)
{
    nbCheckSenseiFactors_t factors = D_00544D58;
    nbEncountForm_t *form = &nbEncountFormTable[data->encno];
    datUnitWork_t *player;
    u16 senseiFlags = form->senseiFlags;
    s32 enemyLevel = 0;
    s32 enemyParam4 = 0;
    s32 enemyParam5 = 0;
    s32 playerLevel;
    s32 playerParam4;
    s32 playerParam5;
    s32 threshold;
    s32 roll;
    u8 weight;

    if (senseiFlags & 4) {
        return 0;
    }
    if (senseiFlags & 2) {
        return 1;
    }

    enemyLevel = nbGetLvAvg2(data, 1);
    enemyParam4 = nbGetParamAvg2(data, 1, 4);
    enemyParam5 = nbGetParamAvg2(data, 1, 5);
    player = (datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54);
    playerLevel = player->level;
    playerParam4 = datGetParam(player, 4);
    playerParam5 = datGetParam(player, 5);

    threshold = playerLevel + playerParam5 / 2 + playerParam4 -
                (enemyLevel + enemyParam5 / 2 + enemyParam4) + 0x48;
    if (threshold < 0x46) {
        threshold = 0x46;
    }
    if (threshold >= 0x56) {
        threshold = 0x55;
    }

    weight = nbEncountTable[data->encpackno].enctable[data->packno].ritu[data->packindex].sensei;
    if (playerLevel < enemyLevel + 0xF) {
        threshold = (s32)((f32)threshold * factors.factor[weight]);
    }

    roll = dds3GetRandIntA(0, 0x80);
    printf(D_00544D70, playerLevel, enemyLevel, weight, threshold, roll);
    if (roll < threshold) {
        return 0;
    }
    if (evtBitCheck(0xAF2) != 0 && (dds3GetRandIntA(0, 0x80) & 1) != 0) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "newbattle/nbCalc", nbCheckBackAttack);

INCLUDE_ASM(const s32, "newbattle/nbCalc", nbCheckEscape);

s32 nbGetEscapeSwitch(nbMainProcessData_t *data)
{
    if ((data->stat & 1) != 0) {
        return 0;
    }
    return nbEncountFormTable[data->encno].esc != 1;
}

INCLUDE_RODATA(const s32, "newbattle/nbCalc", D_00544D00);

INCLUDE_RODATA(const s32, "newbattle/nbCalc", D_00544D10);

INCLUDE_RODATA(const s32, "newbattle/nbCalc", D_00544D20);

INCLUDE_RODATA(const s32, "newbattle/nbCalc", D_00544D28);

INCLUDE_RODATA(const s32, "newbattle/nbCalc", D_00544D30);

INCLUDE_RODATA(const s32, "newbattle/nbCalc", D_00544D58);

INCLUDE_RODATA(const s32, "newbattle/nbCalc", D_00544D70);

INCLUDE_RODATA(const s32, "newbattle/nbCalc", D_00544DA8);

INCLUDE_RODATA(const s32, "newbattle/nbCalc", D_00544DE0);

extern const char D_00544DF0[];
INCLUDE_RODATA(const s32, "newbattle/nbCalc", D_00544DF0);
extern void nbSetMainPhase(s32 encno);

s32 nbCheckRenzokuEncount(nbMainProcessData_t *data)
{
    s32 rate;
    s32 nextIndex;
    u32 mode;

    if ((nbEncountFormTable[data->encno].senseiFlags & 0x10) != 0) {
        data->packindex = 1;
        data->encno = 0x149;
    } else {
        if (data->encpackno <= 0) {
            return 0;
        }
        mode = DDS3_GBWK->maFlags;
        rate = nbEncountTable[data->encpackno].enctable[data->packno].ritu[data->packindex].renzokuritu;
        nextIndex = nbEncountTable[data->encpackno].enctable[data->packno].ritu[data->packindex].renzokuindex;
        if ((mode & 2) != 0) {
            rate *= 5;
        }
        printf(D_00544DF0, rate, nextIndex);
        if ((s32)dds3GetRandIntA(0, 100) >= rate) {
            return 0;
        }
        data->packindex = nextIndex;
        data->encno = nbEncountTable[data->encpackno].enctable[data->packno].ritu[nextIndex].encountid;
    }
    nbSetMainPhase(data->encno);
    return 1;
}

s32 nbCheckKuisibariSkill(s32 formindex)
{
    datUnitWork_t *unit;
    nbParty_t *party;

    unit = nbGetUnitWorkFromFormindex(formindex);
    if (datCheckSyojiSkill(unit, 0x159) == 0) {
        return 0;
    }
    party = nbGetPartyFromFormindex(formindex);
    if ((party->flag & 0x400) != 0) {
        return 0;
    }
    party->flag |= 0x200;
    return 1;
}

s32 nbCheckKuisibari2Skill(s32 formindex)
{
    datUnitWork_t *unit;
    nbParty_t *party;

    unit = nbGetUnitWorkFromFormindex(formindex);
    if (datCheckSyojiSkill(unit, 0x168) == 0) {
        return 0;
    }
    party = nbGetPartyFromFormindex(formindex);
    if ((party->flag & 0x400) != 0) {
        return 0;
    }
    party->flag |= 0x200;
    return 1;
}

void nbSetKuisibariStop(s32 formindex)
{
    nbParty_t *party;

    party = nbGetPartyFromFormindex(formindex);
    if (party == NULL) {
        return;
    }
    if ((party->flag & 0x200) != 0) {
        party->flag |= 0x400;
    }
}

s32 nbHatudoCheckSkill(datUnitWork_t *work, s32 skill)
{
    if (datCheckSyojiSkill(work, skill) == 0) {
        return 0;
    }
    switch (skill) {
    case 300:
        return evtGetAgeOfMoon() == 8;
    case 301:
        return evtGetAgeOfMoon() == 0;
    }
    return 1;
}

s32 nbCheckSingleTargetSkill(s32 skillid)
{
    return datSkillTable.normal[skillid].targettype == 0;
}

s32 nbCheckFormationPlayerSide(s32 formindex)
{
    return formindex < 4;
}

s32 nbCheckFormationFriend(s32 formindex1, s32 formindex2)
{
    return nbCheckFormationPlayerSide(formindex1) == nbCheckFormationPlayerSide(formindex2);
}

INCLUDE_ASM(const s32, "newbattle/nbCalc", nbGetMakaBaramaki);

s32 nbCheckBadPanicAction(datUnitWork_t *w)
{
    u32 flag;
    s32 ritu;
    s32 hit;

    flag = w->flag;
    if ((flag & 4) != 0) {
        ritu = 30;
    } else if ((flag & 0x20) != 0) {
        ritu = 10;
    } else {
        ritu = 30;
    }
    hit = (s32)dds3GetRandIntA(0, 100) < ritu;
    if (hit) {
        return 0;
    }
    return 1;
}

s32 nbCheckBadStatusGameOver(nbMainProcessData_t *data)
{
    datUnitWork_t *unit;
    s32 i;

    for (i = 0; i < 4; i++) {
        unit = nbGetUnitWorkFromFormindex(i);
        if (unit != NULL && (unit->badstatus & 0xFFF) != 0x400) {
            break;
        }
    }
    return i == 4;
}

void nbSetHpScale(nbFormation_t *form)
{
    datUnitWork_t *unit;
    f32 ritu;

    unit = nbGetUnitWorkFromFormindex(form->formindex);
    if (unit == NULL) {
        return;
    }
    if (unit->hp == 0) {
        return;
    }
    ritu = (f32)unit->hp / (f32)unit->maxhp;
    form->scl = (nbGetUnitVisual(unit)->formscale - 0.5f) * ritu + 0.5f;
}

u32 nbGetDevilFormatFlag(s32 formindex)
{
    datUnitWork_t *unit;

    unit = nbGetUnitWorkFromFormindex(formindex);
    if (unit == NULL) {
        return 0;
    }
    if ((unit->flag & 4) != 0) {
        return 0;
    }
    return datDevilTable[unit->id].flag;
}

f32 nbGetCriticalPow(s32 sformindex, f32 pow)
{
    /* Player and enemy sides use the same multiplier. */
    if (sformindex < 4) {
        pow = pow * 1.5f;
    } else {
        pow = pow * 1.5f;
    }
    return pow;
}

s32 nbGetKakutokuExp(nbMainProcessData_t *data)
{
    s32 exp;
    s32 encno;

    encno = data->encno;
    exp = data->exp;
    if (encno == 267 || encno == 284) {
        if (exp > 20000) {
            exp = 20000;
        }
    }
    return exp;
}

s32 nbGetKakutokuMaka(nbMainProcessData_t *data)
{
    s32 maka;
    s32 encno;

    encno = data->encno;
    maka = data->maka;
    if (encno == 267 || encno == 284) {
        if (maka > 10000) {
            maka = 10000;
        }
    }
    return maka;
}

INCLUDE_SDATA(const s32, "newbattle/nbCalc", D_00551140);

