#include "common.h"
#include "newbattle.h"
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





/* A loaded pac file; only the chain down to the decoded buffer is named. */


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
    s8 phase;
    s8 nextphase;
    u8 esccnt;
    u16 phasetime;
    u16 pad_6E;
    u32 battletime;
    f32 press3;
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
    u8 unk_40[0x18];
    s32 autoskill;
    u8 unk_5C[0xC];
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
extern s32 nbGetMagicAttack(s32 nskill, s32 sformindex, s32 dformindex, s32 waza);

typedef struct sdfRandCB_t sdfRandCB_t;

extern f32 dds3GetRandFloatA(sdfRandCB_t *state);
extern s32 evtBitCheck(s32 no);

extern u32 nbGetKoukaBadDamage(s32 nskill, s32 sformindex, s32 dformindex, f32 ritu, s32 nvirtual);

extern u32 nbGetKoukaBadKaifuku(s32 nskill, s32 sformindex, s32 dformindex, f32 ritu, s32 nvirtual);

extern s32 nbSetHojoKouka(s32 formindex, u32 hojotype, s32 hojopoint, s32 nvirtual);

extern s32 abs(s32 value);

extern s32 datCheckSyojiSkill(datUnitWork_t *work, s32 skill);

extern s32 nbHatudoCheckSkill(datUnitWork_t *work, s32 skill);

extern s32 evtGetAgeOfMoon(void);

extern u32 dds3GetRandIntA(void *state, u32 limit);

extern s32 nbCheckSkillCost(s32 formindex, s32 nskill);

extern datUnitVisual_t *nbGetUnitVisual(datUnitWork_t *work);

extern datDevilFormat_t datDevilTable[];

extern u32 nbGetAisyo(s32 nskill, s32 formindex, s32 attr);
extern s32 nbGetNormalSkillAttr(s32 nskill);

extern s32 nbGetHojoCounter(s32 formindex, s32 type);

extern s32 nbGetAisyoTable(s32 formindex);

extern s32 nbGetEscapeSwitch(nbMainProcessData_t *data);

extern f32 nbGetHojoRitu(s32 formindex, s32 type);

extern char D_00544D00[];
extern char D_00544D10[];
extern char D_00544D30[];
extern s32 datGetNormalAtkPow(datUnitWork_t *work);

extern long fptodp(f32 value);

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

#ifdef NON_MATCHING
s32 nbGetHitType(nbActionProcessData_t *a, s32 nskill, s32 sformindex, s32 dformindex)
{
    datUnitWork_t *source, *target;
    datNormalSkill_t *skill;
    s32 skillattr, aisyo, status, sourceParam, targetParam, point;
    s32 autoSkill = 0;
    f32 factor = 1.0f;
    f32 sourceRate, targetRate, baseRate, hitRate;

    source = nbGetUnitWorkFromFormindex(sformindex);
    target = nbGetUnitWorkFromFormindex(dformindex);
    skillattr = nbGetNormalSkillAttr(nskill);
    if (skillattr == -1 || (skillattr >= 12 && skillattr < 16)) {
        return 0;
    }
    aisyo = nbGetAisyo(nskill, dformindex, skillattr);
    if (nskill == 0) {
        if (nbHatudoCheckSkill(source, 0x12C)) {
            a->autoskill = 0x12C;
            return 1;
        }
        if (nbHatudoCheckSkill(source, 0x12D)) {
            a->autoskill = 0x12D;
            return 1;
        }
    }
    if (aisyo < 0) {
        return 2;
    }
    if (nbGetPartyFromFormindex(sformindex)->flag & 0x100) {
        return 1;
    }
    status = target->badstatus & 0xFFF;
    if (status == 1 || status == 2) {
        factor = 100.0f;
    } else if (status == 4 || status == 16) {
        factor = 8.0f;
    }
    if ((a->data->form[dformindex].flag & 0x2000) && evtBitCheck(0xAF2) == 0) {
        factor = 100.0f;
    }
    if (nskill == 0 && nbHatudoCheckSkill(source, 0x12B)) {
        autoSkill = 1;
        factor *= (f32)datSkillTable.special[11].n / 100.0f;
    }
    skill = &datSkillTable.normal[nskill];
    if (skill->koukatype != 0) {
        return 0;
    }
    (void)((volatile datUnitWork_t *)source)->flag;
    sourceParam = datGetParam(source, 4);
    sourceRate = (f32)((sourceParam / (source->level / 5 + 3)) * 100);
    targetParam = datGetParam(target, 4);
    targetRate = (f32)((targetParam / (target->level / 5 + 3)) * 100);
    baseRate = (f32)(100 - skill->failpoint);
    hitRate = baseRate + (sourceRate - targetRate) * 0.0625f;
    if (hitRate < 50.0f) {
        hitRate = 50.0f;
    }
    point = (s32)((hitRate / baseRate) * (f32)skill->criticalpoint * factor);
    printf("critical p:%d, z = %d, a = %d\n",
           ((volatile datNormalSkill_t *)skill)->criticalpoint, (s32)hitRate, point);
    if ((s32)dds3GetRandIntA(NULL, 100) < point) {
        if (autoSkill) {
            a->autoskill = 0x12B;
        }
        return 1;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbCalc", nbGetHitType);
#endif

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

extern char D_005449E0[], D_005449F8[], D_00551140[];
extern const volatile f32 D_0054E528, D_0054E52C;
extern u32 nbGetDevilFormatFlag(s32 formindex);
extern s32 datGetSakePow(datUnitWork_t *work);
extern s32 datGetMagicHitPow(datUnitWork_t *work);

#ifdef NON_MATCHING
s32 nbGetKoukaType(s32 nskill, s32 sformindex, s32 dformindex)
{
    nbMainProcessData_t *data = nbGetMainProcessData();
    volatile datNormalSkill_t *skill = &datSkillTable.normal[nskill];
    datUnitWork_t *source;
    datUnitWork_t *target;
    u32 formatFlag;
    u32 battleStat;
    u32 aisyo;
    u32 bad;
    s32 attr;
    s32 hit;
    u32 program;
    f32 levelRate;
    f32 sourceRate;
    f32 targetRate;
    f32 chance;
    char *format;

    switch (skill->program) {
    case 1: return 7;
    case 2:
    case 3: return 6;
    case 4: return 8;
    case 5: return 9;
    case 7: return 10;
    case 12: return 12;
    case 13: return 13;
    case 14: return 14;
    }

    source = nbGetUnitWorkFromFormindex(sformindex);
    target = nbGetUnitWorkFromFormindex(dformindex);
    formatFlag = nbGetDevilFormatFlag(dformindex);
    battleStat = data->stat;
    if (battleStat & 0x40000000) {
        return 1;
    }
    if (battleStat & 0x08000000) {
        return 0;
    }
    if (nskill == 0x9E && (battleStat & 1) && data->encno != 0x408 &&
        (target->flag & 0x20)) {
        return 5;
    }
    if (nskill == 0x9F && (battleStat & 1) && (target->flag & 0x20)) {
        return 5;
    }
    attr = nbGetNormalSkillAttr(nskill);
    if (attr != -1 && (attr < 12 || attr >= 15)) {
        aisyo = nbGetAisyo(nskill, dformindex, attr);
        if ((target->badstatus & 0xFFF) == 2 && skill->koukatype == 0) {
            aisyo &= ~0x70000;
        }
        if (aisyo & 0x20000) {
            return 1;
        }
        if (datCheckSyojiSkill(source, 0x165) != 0 && skill->koukatype == 0) {
            if (aisyo & 0x50000) {
                data->d31_kantuu = 1;
            }
        } else {
            if (aisyo & 0x40000) return 2;
            if (aisyo & 0x10000) return 3;
        }
    }

    if ((data->stat & 0x400) && (target->flag & 0x20)) {
        if (skill->targettype != 0) {
            if (attr != 14 && data->enemypcnt != 1) return 5;
        } else if (data->form[dformindex].stat & 0x1000) {
            return 11;
        }
    }
    program = skill->program;
    bad = target->badstatus & 0xFFF;
    if (program == 6 && bad != 4) return 5;
    if (program == 15 && bad != 0x40) return 5;
    if (bad == 1 || bad == 2 || bad == 4 || bad == 0x10 ||
        bad == 0x100 || bad == 0x400) {
        return 0;
    }
    if (nbCheckKoukaJyoken(nskill, sformindex, dformindex) == 0) {
        printf(D_00551140);
        return 5;
    }
    if (skill->hitlevel == 0xFF) return 0;
    levelRate = (f32)skill->hitlevel * D_0054E528;
    if (evtBitCheck(0xAF2) != 0 && skill->hptype == 3 && skill->hpn == 1 &&
        !(target->flag & 0x20)) {
        levelRate *= D_0054E52C;
    }
    sourceRate = nbGetHojoRitu(sformindex, 8);
    targetRate = nbGetHojoRitu(dformindex, 6);
    switch (skill->koukatype) {
    case 0: {
        s32 sourceParam;
        s32 targetParam;
        s32 failpoint;
        (void)((volatile datUnitWork_t *)source)->flag;
        sourceParam = datGetParam(source, 4);
        sourceParam = (sourceParam / (source->level / 5 + 3)) * 100;
        targetParam = datGetParam(target, 4);
        targetParam = (targetParam / (target->level / 5 + 3)) * 100;
        failpoint = skill->failpoint;
        chance = ((f32)(100 - failpoint) +
                  ((f32)sourceParam - (f32)targetParam) * 0.0625f) *
                 sourceRate * targetRate * levelRate;
        format = D_005449E0;
        break;
    }
    case 1: {
        f32 base = (source->flag & 0x20) ? 95.0f : 100.0f;
        s32 evasion = datGetSakePow(target);
        s32 accuracy = datGetMagicHitPow(source);
        chance = ((f32)(100 - (evasion * 100) / accuracy) * 0.5f + base) *
                 sourceRate * targetRate * levelRate;
        format = D_005449F8;
        break;
    }
    default:
        return 0;
    }
    if (chance < 50.0f) chance = 50.0f;
    if (chance > 100.0f) chance = 100.0f;
    if (nbGetBadStat(sformindex) == 0x100) chance *= 0.25f;
    hit = (s32)chance;
    printf(format, hit);
    if ((s32)dds3GetRandIntA(NULL, 100) >= hit) {
        return (formatFlag & 0x800) ? 5 : 4;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbCalc", nbGetKoukaType);
#endif

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

#ifdef NON_MATCHING
extern s32 nbGetButuriKaifuku(s32 nskill, s32 sformindex, s32 dformindex, s32 waza);
extern s32 nbGetMagicKaifuku(s32 nskill, s32 sformindex, s32 dformindex, s32 waza);
extern s32 nbGetMaxHpWazaPoint(s32 nskill, s32 sformindex, s32 dformindex, s32 waza);
extern s32 nbCheckFormationPlayerSide(s32 formindex);
#endif

s32 nbGetKoukaHp(s32 sw, s32 nskill, s32 sformindex, s32 dformindex, f32 ritu, s32 cnt)
{
    datUnitWork_t *swork;
    datUnitWork_t *dwork;
    s32 hpbase;
    s32 hpn;
    s32 result;
    s32 adjusted;
    s32 amount;
    s32 status;

    swork = nbGetUnitWorkFromFormindex(sformindex);
    dwork = nbGetUnitWorkFromFormindex(dformindex);
    hpn = datSkillTable.normal[nskill].hpn;
    hpbase = datSkillTable.normal[nskill].hpbase;
    result = -1;
    adjusted = -1;
    amount = 0;

    switch (datSkillTable.normal[nskill].hptype) {
    case 0:
        break;
    case 5:
        result = hpn + hpbase;
        break;
    case 15:
        result = dds3GetRandIntA(0, hpn) + hpbase;
        break;
    case 9:
        result = (dwork->hp * hpn) / 100 + hpbase;
        if (result <= 0) {
            result = 1;
        }
        break;
    case 11:
        result = (dwork->maxhp * hpn) / 100 + hpbase;
        if (result <= 0) {
            result = 1;
        }
        break;
    case 2:
        switch (datSkillTable.normal[nskill].koukatype) {
        case 0:
            amount = nbGetButuriKaifuku(nskill, sformindex, dformindex, hpn);
            break;
        case 1:
            amount = nbGetMagicKaifuku(nskill, sformindex, dformindex, hpn);
            break;
        }
        result = amount + hpbase;
        break;
    case 7:
        result = nbGetMaxHpWazaPoint(nskill, sformindex, dformindex, hpn) + hpbase;
        break;
    case 4:
    case 13:
        adjusted = hpn + hpbase;
        break;
    case 8:
        adjusted = (dwork->hp * hpn) / 100 + hpbase;
        break;
    case 10:
        adjusted = (dwork->maxhp * hpn) / 100 + hpbase;
        break;
    case 1:
    case 12:
    case 14:
        switch (datSkillTable.normal[nskill].koukatype) {
        case 0:
            amount = nbGetButuriAttack(nskill, sformindex, dformindex, hpn);
            status = swork->badstatus & 0xFFF;
            switch (status) {
            case 0x40:
                amount /= 2;
                break;
            case 0x200:
                amount /= 10;
                break;
            }
            break;
        case 1:
            amount = nbGetMagicAttack(nskill, sformindex, dformindex, hpn);
            break;
        }
        adjusted = amount + hpbase;
        if (datSkillTable.normal[nskill].hptype == 14 && cnt >= 2) {
            adjusted /= cnt;
        }
        break;
    case 6:
        adjusted = nbGetMaxHpWazaPoint(nskill, sformindex, dformindex, hpn);
        status = swork->badstatus & 0xFFF;
        switch (status) {
        case 0x40:
            adjusted /= 2;
            break;
        case 0x200:
            adjusted /= 10;
            break;
        }
        adjusted += hpbase;
        break;
    case 3:
        {
            s32 remaining;

            remaining = hpn + hpbase;
            if (remaining < dwork->hp) {
                adjusted = dwork->hp - remaining;
            }
        }
        break;
    }

    switch (datSkillTable.normal[nskill].hptype) {
    case 1:
    case 2:
    case 6:
    case 7:
    case 12:
    case 14:
        if (adjusted > 0) {
            switch (datSkillTable.normal[nskill].koukatype) {
            case 0:
                {
                    f32 spread;
                    f32 base;

                    spread = dds3GetRandFloatA(0);
                    base = (f32)adjusted;
                    spread -= 0.5f;
                    spread += spread;
                    spread *= 0.04999999702f;
                    adjusted = (s32)(base + base * spread);
                }
                break;
            case 1:
                {
                    f32 spread;
                    f32 base;

                    spread = dds3GetRandFloatA(0);
                    base = (f32)adjusted;
                    spread -= 0.5f;
                    spread += spread;
                    spread *= 0.09999999404f;
                    adjusted = (s32)(base + base * spread);
                }
                break;
            }
            if (adjusted < 0) {
                adjusted = 0;
            }
            adjusted = (s32)((f32)adjusted * ritu);
            if (evtBitCheck(0xAF2) != 0 && nbCheckFormationPlayerSide(sformindex) == 0) {
                adjusted = (s32)((f32)adjusted * 0.6999999881f);
            }
        }
        break;
    }

    if (adjusted == 0) {
        adjusted = 1;
    }
    if (sw != 0) {
        s32 hptype;

        hptype = datSkillTable.normal[nskill].hptype;
        if (hptype < 14) {
            if (hptype >= 12) {
                if (dwork->maxhp < adjusted) {
                    adjusted = dwork->maxhp;
                }
            }
        }
        return adjusted;
    }
    return result;
}

s32 nbGetKoukaMp(s32 sw, s32 nskill, s32 sformindex, s32 dformindex, f32 ritu, s32 cnt)
{
    datUnitWork_t *dwork;
    s32 mpbase;
    s32 mpn;
    s32 result;
    s32 adjusted;
    s32 amount;

    nbGetUnitWorkFromFormindex(sformindex);
    dwork = nbGetUnitWorkFromFormindex(dformindex);
    mpbase = datSkillTable.normal[nskill].mpbase;
    mpn = datSkillTable.normal[nskill].mpn;
    result = -1;
    adjusted = -1;
    amount = 0;

    switch (datSkillTable.normal[nskill].mptype) {
    case 0:
        break;
    case 5:
        result = mpn + mpbase;
        break;
    case 15:
        result = dds3GetRandIntA(0, mpn) + mpbase;
        break;
    case 9:
        result = (dwork->mp * mpn) / 100 + mpbase;
        break;
    case 11:
        result = (dwork->maxmp * mpn) / 100 + mpbase;
        break;
    case 2:
        switch (datSkillTable.normal[nskill].koukatype) {
        case 0:
            amount = nbGetButuriKaifuku(nskill, sformindex, dformindex, mpn);
            break;
        case 1:
            amount = nbGetMagicKaifuku(nskill, sformindex, dformindex, mpn);
            break;
        }
        result = amount + mpbase;
        break;
    case 7:
        result = nbGetMaxHpWazaPoint(nskill, sformindex, dformindex, mpn) + mpbase;
        break;
    case 4:
    case 13:
        adjusted = mpn + mpbase;
        break;
    case 8:
        adjusted = (dwork->mp * mpn) / 100 + mpbase;
        break;
    case 10:
        adjusted = (dwork->maxmp * mpn) / 100 + mpbase;
        break;
    case 1:
    case 12:
    case 14:
        switch (datSkillTable.normal[nskill].koukatype) {
        case 0:
            amount = nbGetButuriAttack(nskill, sformindex, dformindex, mpn);
            break;
        case 1:
            amount = nbGetMagicAttack(nskill, sformindex, dformindex, mpn);
            break;
        }
        if (datSkillTable.normal[nskill].mptype == 6) {
            amount = (s32)(f32)amount;
        }
        adjusted = amount + mpbase;
        if (datSkillTable.normal[nskill].mptype == 14 && cnt >= 2) {
            adjusted /= cnt;
        }
        break;
    case 6:
        adjusted = nbGetMaxHpWazaPoint(nskill, sformindex, dformindex, mpn) + mpbase;
        break;
    case 3:
        {
            s32 remaining;

            remaining = mpn + mpbase;
            if (remaining < dwork->mp) {
                adjusted = dwork->mp - remaining;
            }
        }
        break;
    }
    switch (datSkillTable.normal[nskill].mptype) {
    case 1:
    case 2:
    case 6:
    case 7:
    case 12:
    case 14:
        if (adjusted > 0) {
            switch (datSkillTable.normal[nskill].koukatype) {
            case 0:
                {
                    f32 spread;
                    f32 base;

                    spread = dds3GetRandFloatA(0);
                    base = (f32)adjusted;
                    spread -= 0.5f;
                    spread += spread;
                    spread *= 0.04999999702f;
                    adjusted = (s32)(base + base * spread);
                }
                break;
            case 1:
                {
                    f32 spread;
                    f32 base;

                    spread = dds3GetRandFloatA(0);
                    base = (f32)adjusted;
                    spread -= 0.5f;
                    spread += spread;
                    spread *= 0.04999999702f;
                    adjusted = (s32)(base + base * spread);
                }
                break;
            }
            if (adjusted < 0) {
                adjusted = 0;
            }
            adjusted = (s32)((f32)adjusted * ritu);
            if (evtBitCheck(0xAF2) != 0 && nbCheckFormationPlayerSide(sformindex) == 0) {
                adjusted = (s32)((f32)adjusted * 0.6999999881f);
            }
        }
        break;
    }

    if (sw != 0) {
        if (dwork->mp < adjusted) {
            adjusted = dwork->mp;
        }
        return adjusted;
    }
    return result;
}

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

extern const char D_00544BE8[];

#ifdef NON_MATCHING
u32 nbGetKoukaBadKaifuku(s32 nskill, s32 sformindex, s32 dformindex, f32 ritu, s32 nvirtual)
{
    volatile datNormalSkill_t *skill = &datSkillTable.normal[nskill];
    datUnitWork_t *source;
    datUnitWork_t *target;
    u32 status;
    s32 sourceParam;
    s32 targetParam;
    s32 success = 1;
    f32 chance;

    if (skill->badtype != 2) {
        return 0;
    }
    source = nbGetUnitWorkFromFormindex(sformindex);
    target = nbGetUnitWorkFromFormindex(dformindex);
    status = skill->basstatus & 0xFFF;
    if (skill->badlevel != 0xFF && nvirtual == 0) {
        chance = (f32)skill->badlevel;
        sourceParam = datGetParam(source, 2);
        sourceParam += datGetParam(source, 5) / 4;
        chance *= (f32)sourceParam;
        targetParam = datGetParam(target, 2);
        targetParam += datGetParam(target, 5) / 4;
        chance = (chance / 100.0f) * ((ritu * 100.0f) / (f32)targetParam);
        printf(D_00544BE8, (double)chance);
        success = (s32)dds3GetRandIntA(NULL, 100) < (s32)chance;
    }
    if (success && (status & (target->badstatus & 0xFFF))) {
        return status;
    }
    return 0;
}
#else
INCLUDE_RODATA(const s32, "newbattle/nbCalc", D_00544BA8);

INCLUDE_ASM(const s32, "newbattle/nbCalc", nbGetKoukaBadKaifuku);
#endif

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

extern const char D_00544C10[];

#ifdef NON_MATCHING
s32 nbCheckBadKaifuku(datUnitWork_t *work, s32 type)
{
    u32 status;
    f32 ratio;
    f32 base;
    s32 chance;

    if (work == NULL || !(work->flag & 1)) {
        return 0;
    }
    status = work->badstatus & 0xFFF;
    switch (type) {
    case 0:
        switch (status) {
        case 1:
        case 2:
            return 1;
        case 4:
        case 8:
        case 0x10:
        case 0x80:
            ratio = (f32)datGetParam(work, 5) / (f32)((work->level + 20) / 5);
            if (status == 4) {
                base = 20.0f;
            } else if (status == 0x80) {
                base = 40.0f;
            } else {
                base = 30.0f;
            }
            chance = (s32)(base * ratio);
            printf(D_00544C10, chance);
            return (s32)dds3GetRandIntA(NULL, 100) < chance;
        default:
            return 0;
        }
    case 1:
        return status == 0x10;
    case 2:
        return status == 4;
    case 3:
        switch (status) {
        case 1: case 2: case 4: case 8: case 0x10:
        case 0x80: case 0x200: case 0x400:
            return 1;
        default:
            return 0;
        }
    default:
        return 0;
    }
}
#else
INCLUDE_ASM(const s32, "newbattle/nbCalc", nbCheckBadKaifuku);
#endif

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

extern u32 datAisyoTable[248][16];
extern u32 datGetAisyo(datUnitWork_t *work, s32 attr);

INCLUDE_RODATA(const s32, "newbattle/nbCalc", D_00544BF8);

INCLUDE_RODATA(const s32, "newbattle/nbCalc", D_00544C10);

u32 nbGetAisyo(s32 nskill, s32 formindex, s32 attr)
{
    datUnitWork_t *work;
    nbParty_t *party;
    u32 aisyo;
    s32 count;
    s32 ratio;

    work = nbGetUnitWorkFromFormindex(formindex);
    if (formindex >= 16) {
        return datGetAisyo(work, attr);
    }
    party = nbGetPartyFromFormindex(formindex);
    if (attr == -1) {
        return 0;
    }
    if (attr >= -1) {
        if (attr < 16) {
            if (attr >= 12) {
                return 100;
            }
        }
    }

    aisyo = party->aisyo[attr];
    count = nbGetHojoCounter(formindex, 19);
    if (count > 0) {
        aisyo = datAisyoTable[count][attr];
    }
    switch (work->badstatus & 0xFFF) {
    case 0x400:
        aisyo = datAisyoTable[246][attr];
        break;
    case 0x200:
        aisyo = datAisyoTable[247][attr];
        break;
    }

    switch (attr) {
    case 1:
        if (nbGetHojoCounter(formindex, 10) > 0) {
            aisyo |= 0x10000;
        }
        break;
    case 2:
        if (nbGetHojoCounter(formindex, 9) > 0) {
            aisyo |= 0x10000;
        }
        break;
    case 6:
        if (nbGetHojoCounter(formindex, 12) > 0) {
            aisyo |= 0x10000;
        }
        break;
    case 7:
        if (nbGetHojoCounter(formindex, 11) > 0) {
            aisyo |= 0x10000;
        }
        break;
    }

    if (nskill >= 0 && attr != 5 && attr != 11) {
        switch (datSkillTable.normal[nskill].koukatype) {
        case 0:
            if (nbGetHojoCounter(formindex, 14) > 0) {
                aisyo |= 0x20000;
            }
            break;
        case 1:
            if (nbGetHojoCounter(formindex, 13) > 0) {
                aisyo |= 0x20000;
            }
            break;
        }
    }

    ratio = aisyo & 0xFFFF;
    switch (attr) {
    case 0:
        if (nbHatudoCheckSkill(work, 0x143) != 0) aisyo |= 0x10000;
        if (nbHatudoCheckSkill(work, 0x14D) != 0) aisyo |= 0x40000;
        if (nbHatudoCheckSkill(work, 0x152) != 0) aisyo |= 0x20000;
        if (nbHatudoCheckSkill(work, 0x139) != 0) ratio = ratio * datSkillTable.special[0x19].n / 100;
        break;
    case 1:
        if (nbHatudoCheckSkill(work, 0x144) != 0) aisyo |= 0x10000;
        if (nbHatudoCheckSkill(work, 0x14E) != 0) aisyo |= 0x40000;
        if (nbHatudoCheckSkill(work, 0x153) != 0) aisyo |= 0x20000;
        if (nbHatudoCheckSkill(work, 0x13A) != 0) ratio = ratio * datSkillTable.special[0x1A].n / 100;
        break;
    case 2:
        if (nbHatudoCheckSkill(work, 0x145) != 0) aisyo |= 0x10000;
        if (nbHatudoCheckSkill(work, 0x14F) != 0) aisyo |= 0x40000;
        if (nbHatudoCheckSkill(work, 0x154) != 0) aisyo |= 0x20000;
        if (nbHatudoCheckSkill(work, 0x13B) != 0) ratio = ratio * datSkillTable.special[0x1B].n / 100;
        break;
    case 3:
        if (nbHatudoCheckSkill(work, 0x146) != 0) aisyo |= 0x10000;
        if (nbHatudoCheckSkill(work, 0x150) != 0) aisyo |= 0x40000;
        if (nbHatudoCheckSkill(work, 0x155) != 0) aisyo |= 0x20000;
        if (nbHatudoCheckSkill(work, 0x13C) != 0) ratio = ratio * datSkillTable.special[0x1C].n / 100;
        break;
    case 4:
        if (nbHatudoCheckSkill(work, 0x147) != 0) aisyo |= 0x10000;
        if (nbHatudoCheckSkill(work, 0x151) != 0) aisyo |= 0x40000;
        if (nbHatudoCheckSkill(work, 0x156) != 0) aisyo |= 0x20000;
        if (nbHatudoCheckSkill(work, 0x13D) != 0) ratio = ratio * datSkillTable.special[0x1D].n / 100;
        break;
    case 6:
        if (nbHatudoCheckSkill(work, 0x148) != 0) aisyo |= 0x10000;
        if (nbHatudoCheckSkill(work, 0x13E) != 0) ratio = ratio * datSkillTable.special[0x1E].n / 100;
        break;
    case 7:
        if (nbHatudoCheckSkill(work, 0x149) != 0) aisyo |= 0x10000;
        if (nbHatudoCheckSkill(work, 0x13F) != 0) ratio = ratio * datSkillTable.special[0x1F].n / 100;
        break;
    case 8:
        if (nbHatudoCheckSkill(work, 0x14A) != 0) aisyo |= 0x10000;
        if (nbHatudoCheckSkill(work, 0x140) != 0) ratio = ratio * datSkillTable.special[0x20].n / 100;
        break;
    case 9:
        if (nbHatudoCheckSkill(work, 0x14B) != 0) aisyo |= 0x10000;
        if (nbHatudoCheckSkill(work, 0x141) != 0) ratio = ratio * datSkillTable.special[0x21].n / 100;
        break;
    case 10:
        if (nbHatudoCheckSkill(work, 0x14C) != 0) aisyo |= 0x10000;
        if (nbHatudoCheckSkill(work, 0x142) != 0) ratio = ratio * datSkillTable.special[0x22].n / 100;
        break;
    }
    return (aisyo & 0xFFFF0000) | ratio;
}

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

s32 nbSetHojoKouka(s32 formindex, u32 hojotype, s32 hojopoint, s32 nvirtual)
{
    s32 total;
    s32 change;
    s32 count;

    total = 0;
    if ((hojotype & 0x1) != 0) {
        change = nbSetHojoAddCounter(formindex, 4, hojopoint, nvirtual);
        change = abs(change);
        total += change;
    }
    if ((hojotype & 0x2) != 0) {
        change = nbSetHojoAddCounter(formindex, 4, -hojopoint, nvirtual);
        change = abs(change);
        total += change;
    }
    if ((hojotype & 0x4) != 0) {
        change = nbSetHojoAddCounter(formindex, 5, hojopoint, nvirtual);
        change = abs(change);
        total += change;
    }
    if ((hojotype & 0x8) != 0) {
        change = nbSetHojoAddCounter(formindex, 5, -hojopoint, nvirtual);
        change = abs(change);
        total += change;
    }
    if ((hojotype & 0x10) != 0) {
        change = nbSetHojoAddCounter(formindex, 6, hojopoint, nvirtual);
        change = abs(change);
        total += change;
    }
    if ((hojotype & 0x20) != 0) {
        change = nbSetHojoAddCounter(formindex, 6, -hojopoint, nvirtual);
        change = abs(change);
        total += change;
    }
    if ((hojotype & 0x40) != 0) {
        change = nbSetHojoAddCounter(formindex, 7, hojopoint, nvirtual);
        change = abs(change);
        total += change;
    }
    if ((hojotype & 0x80) != 0) {
        change = nbSetHojoAddCounter(formindex, 7, -hojopoint, nvirtual);
        change = abs(change);
        total += change;
    }
    if ((hojotype & 0x100) != 0) {
        change = nbSetHojoAddCounter(formindex, 8, hojopoint, nvirtual);
        change = abs(change);
        total += change;
    }
    if ((hojotype & 0x200) != 0) {
        change = nbSetHojoAddCounter(formindex, 8, -hojopoint, nvirtual);
        change = abs(change);
        total += change;
    }
    if ((hojotype & 0x1000) != 0) {
        change = nbSetHojoAddCounter(formindex, 9, hojopoint, nvirtual);
        change = abs(change);
        total += change;
    }
    if ((hojotype & 0x2000) != 0) {
        change = nbSetHojoAddCounter(formindex, 10, hojopoint, nvirtual);
        change = abs(change);
        total += change;
    }
    if ((hojotype & 0x4000) != 0) {
        change = nbSetHojoAddCounter(formindex, 11, hojopoint, nvirtual);
        change = abs(change);
        total += change;
    }
    if ((hojotype & 0x8000) != 0) {
        change = nbSetHojoAddCounter(formindex, 12, hojopoint, nvirtual);
        change = abs(change);
        total += change;
    }
    if ((hojotype & 0x10000) != 0) {
        change = nbSetHojoAddCounter(formindex, 13, hojopoint, nvirtual);
        change = abs(change);
        total += change;
    }
    if ((hojotype & 0x20000) != 0) {
        change = nbSetHojoAddCounter(formindex, 14, hojopoint, nvirtual);
        change = abs(change);
        total += change;
    }
    if ((hojotype & 0x1000000) != 0) {
        change = nbSetHojoAddCounter(formindex, 15, hojopoint, nvirtual);
        change = abs(change);
        total += change;
    }
    if ((hojotype & 0x400) != 0) {
        count = nbGetHojoCounter(formindex, 4);
        if (count > 0) {
            total += count;
            nbSetHojoCounter(formindex, 4, 0);
        }
        count = nbGetHojoCounter(formindex, 5);
        if (count > 0) {
            total += count;
            nbSetHojoCounter(formindex, 5, 0);
        }
        count = nbGetHojoCounter(formindex, 6);
        if (count > 0) {
            total += count;
            nbSetHojoCounter(formindex, 6, 0);
        }
        count = nbGetHojoCounter(formindex, 7);
        if (count > 0) {
            total += count;
            nbSetHojoCounter(formindex, 7, 0);
        }
        count = nbGetHojoCounter(formindex, 8);
        if (count > 0) {
            total += count;
            nbSetHojoCounter(formindex, 8, 0);
        }
    }
    if ((hojotype & 0x40000) != 0) {
        count = nbGetHojoCounter(formindex, 4);
        if (count < 0) {
            total -= count;
            nbSetHojoCounter(formindex, 4, 0);
        }
        count = nbGetHojoCounter(formindex, 5);
        if (count < 0) {
            total -= count;
            nbSetHojoCounter(formindex, 5, 0);
        }
        count = nbGetHojoCounter(formindex, 6);
        if (count < 0) {
            total -= count;
            nbSetHojoCounter(formindex, 6, 0);
        }
        count = nbGetHojoCounter(formindex, 7);
        if (count < 0) {
            total -= count;
            nbSetHojoCounter(formindex, 7, 0);
        }
        count = nbGetHojoCounter(formindex, 8);
        if (count < 0) {
            total -= count;
            nbSetHojoCounter(formindex, 8, 0);
        }
    }
    return total;
}

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

#ifdef NON_MATCHING
extern s32 datGetDefPow(datUnitWork_t *work);
#endif

s32 nbGetButuriAttack(s32 nskill, s32 sformindex, s32 dformindex, s32 waza)
{
    datUnitWork_t *swork;
    datUnitWork_t *dwork;
    f32 sritu;
    f32 dritu;
    f32 value;
    char *sfmt;
    char *dfmt;

    swork = nbGetUnitWorkFromFormindex(sformindex);
    dwork = nbGetUnitWorkFromFormindex(dformindex);
    sritu = nbGetHojoRitu(sformindex, 4);
    dritu = nbGetHojoRitu(dformindex, 7);
    datGetNormalAtkPow(swork);
    datGetDefPow(dwork);
    if (swork->flag & 0x20) {
        if (nskill == 0) {
            value = ((f32)swork->level * 2.0f + (f32)datGetParam(swork, 0) * 2.0f)
                * 0x1.547aep+0f * 0x1.333332p-1f - (f32)(48 / (swork->level + 10));
        } else {
            value = ((f32)swork->level * 2.0f + (f32)datGetParam(swork, 0) * 2.0f)
                * (f32)waza / 0x1.733332p+4f * 0x1.333332p-1f - (f32)(50 / (swork->level + 10));
        }
        if (!evtBitCheck(0xAF2)) {
            value *= 0x1.570a3cp+0f;
        }
    } else {
        if (nskill == 0) {
            value = ((f32)swork->level * 2.0f + (f32)datGetParam(swork, 0) * 2.0f)
                * 0x1.547aep+0f * 0x1.999998p-1f;
        } else {
            value = ((f32)swork->level * 2.0f + (f32)datGetParam(swork, 0) * 2.0f)
                * (f32)waza / 0x1.733332p+4f * 0x1.999998p-1f;
        }
    }
    sfmt = D_00544D00;
    printf(sfmt, sritu);
    dfmt = D_00544D10;
    printf(dfmt, dritu);
    value = (value * sritu) * dritu;
    return value;
}

s32 nbGetButuriKaifuku(s32 nskill, s32 sformindex, s32 dformindex, s32 waza)
{
    return nbGetButuriAttack(nskill, sformindex, dformindex, waza);
}

extern const volatile f64 D_00544D20;
extern const volatile f64 D_00544D28;

#ifdef NON_MATCHING
s32 nbGetMagicAttack(s32 nskill, s32 sformindex, s32 dformindex, s32 waza)
{
    datUnitWork_t *source;
    datNormalSkill_t *skill;
    f32 sourceRatio, targetRatio, power, limit, adjusted, damage, candidate;
    s32 level, cappedLevel, parameter, i;
    u32 flags;

    source = nbGetUnitWorkFromFormindex(sformindex);
    nbGetUnitWorkFromFormindex(dformindex);
    sourceRatio = nbGetHojoRitu(sformindex, 5);
    targetRatio = nbGetHojoRitu(dformindex, 7);
    level = source->level;
    skill = &datSkillTable.normal[nskill];
    power = (f32)skill->magicbase + (f32)(s32)((u32)level * (u32)waza * 2) / 21.0f;
    limit = (f32)skill->magiclimit;
    if (limit < power) {
        power = limit;
    }
    cappedLevel = level < 161 ? level : 160;
    parameter = datGetParam(source, 2);
    adjusted = power + (power / 100.0f) * ((parameter - ((f32)cappedLevel / 5.0f + 4.0f)) * 2.5f);
    damage = (f32)((f64)adjusted * D_00544D20);
    flags = source->flag;
    if (!(flags & 0x20)) {
        level = ((volatile datUnitWork_t *)source)->level;
        if (level >= 101) {
            level = ((volatile datUnitWork_t *)source)->level;
            i = 100;
            if (i < level) {
                do {
                    power = (f32)skill->magicbase + (f32)(s32)((u32)level * (u32)waza * 2) / 21.0f;
                    limit = (f32)skill->magiclimit;
                    if (limit < power) {
                        power = limit;
                    }
                    cappedLevel = level < 161 ? level : 160;
                    parameter = datGetParam(source, 2);
                    adjusted = power + (power / 100.0f) * ((parameter - ((f32)cappedLevel / 5.0f + 4.0f)) * 2.5f);
                    candidate = (f32)((f64)adjusted * D_00544D28);
                    if (damage < candidate) {
                        damage = candidate;
                    }
                    level = ((volatile datUnitWork_t *)source)->level;
                    i++;
                } while (i < level);
                flags = ((volatile datUnitWork_t *)source)->flag;
            }
        }
    }
    if (flags & 0x20) {
        damage = damage * 0.75f - 50.0f / (f32)(source->level + 10);
        if (evtBitCheck(0xAF2) == 0) {
            damage *= 0x1.570a3cp+0f;
        }
    }
    printf(D_00544D00, (f64)sourceRatio);
    printf(D_00544D10, (f64)targetRatio);
    return (s32)(damage * sourceRatio * targetRatio);
}
#else
INCLUDE_ASM(const s32, "newbattle/nbCalc", nbGetMagicAttack);
#endif

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
    printf(fmt, waza, fptodp(ritu), ret = v * ritu);
    return ret;
}

#ifdef NON_MATCHING
extern s32 datGetBaseMaxHp(datUnitWork_t *work);
#endif

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

#ifdef NON_MATCHING
u32 nbGetVirtualAisyo(s32 nskill, s32 sformindex, s32 dformindex)
{
    s32 skillattr = nbGetNormalSkillAttr(nskill);
    u32 aisyo;

    if (skillattr == -1 || (skillattr >= 12 && skillattr < 15)) {
        return 0;
    }
    aisyo = nbGetAisyo(nskill, dformindex, skillattr);
    if (aisyo & 0x20000) {
        return 0x20000;
    }
    if (aisyo & 0x40000) {
        return 0x40000;
    }
    if (aisyo & 0x10000) {
        return 0x10000;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbCalc", nbGetVirtualAisyo);
#endif

s32 nbGetVirtualKoukaPoint(s32 nskill, s32 sformindex, s32 dformindex)
{
    datUnitWork_t *target;
    f32 ritu;
    u32 badDamage;
    u32 badKaifuku;
    s32 hpDamage;
    s32 hpKaifuku;
    s32 mpDamage;
    s32 mpKaifuku;
    s32 hojo;
    s32 room;
    s32 result;

    nbGetUnitWorkFromFormindex(sformindex);
    target = nbGetUnitWorkFromFormindex(dformindex);
    ritu = nbGetAisyoRitu(nskill, sformindex, dformindex);
    badDamage = nbGetKoukaBadDamage(nskill, sformindex, dformindex, ritu, 1);
    badKaifuku = nbGetKoukaBadKaifuku(nskill, sformindex, dformindex, ritu, 1);
    hpDamage = nbGetKoukaHp(1, nskill, sformindex, dformindex, ritu, 1);
    hpKaifuku = nbGetKoukaHp(0, nskill, sformindex, dformindex, ritu, 1);
    if (hpKaifuku > 0) {
        room = target->maxhp - target->hp;
        if (room < hpKaifuku) {
            hpKaifuku = room;
        }
    }
    mpDamage = nbGetKoukaMp(1, nskill, sformindex, dformindex, ritu, 1);
    mpKaifuku = nbGetKoukaMp(0, nskill, sformindex, dformindex, ritu, 1);
    if (mpKaifuku > 0) {
        room = target->maxmp - target->mp;
        if (room < mpKaifuku) {
            mpKaifuku = room;
        }
    }

    hojo = 0;
    if (datSkillTable.normal[nskill].hojotype != 0) {
        hojo = nbSetHojoKouka(dformindex, datSkillTable.normal[nskill].hojotype,
                              datSkillTable.normal[nskill].hojopoint, 1);
    }

    result = 0;
    if (badDamage != 0) {
        result++;
    }
    if (badKaifuku != 0) {
        result++;
    }
    if (hpDamage >= 2) {
        result += hpDamage;
    }
    if (hpKaifuku >= 2) {
        result += hpKaifuku;
    }
    if (mpDamage >= 2) {
        result += mpDamage;
    }
    if (mpKaifuku >= 2) {
        result += mpKaifuku;
    }
    return result + hojo;
}

#ifdef NON_MATCHING
extern s32 datGetSkillCost(datUnitWork_t *w, s32 nskill);
#endif

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

#ifdef NON_MATCHING
extern s32 datBossRashChk(s32 eid);
#endif

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

extern s32 datGetPlayerLevel(void);

void nbGetExpMakaItem(datUnitWork_t *unit)
{
    nbMainProcessData_t *data;
    datDevilFormat_t *devil;
    s32 isBoss;
    s32 playerLevel;
    s32 levelGap;
    s32 exp;
    s32 maka;
    u8 item = 0;
    s32 i;

    if ((unit->flag & 4) != 0) {
        return;
    }

    data = nbGetMainProcessData();
    isBoss = datBossRashChk(data->encno);
    devil = &datDevilTable[unit->id];
    exp = devil->dropexp;
    playerLevel = datGetPlayerLevel();
    levelGap = playerLevel - unit->level;

    if (unit->level < 0x46) {
        if ((u32)(levelGap - 6) < 6) {
            exp = (s32)((f32)exp / 1.3f);
        } else if (levelGap >= 12) {
            exp = (s32)((f32)exp / 1.8f);
        }
    } else if (levelGap >= 6) {
        exp = (s32)((f32)exp / 1.3f);
    }
    maka = devil->dropmakka;

    if (isBoss != 0) {
        exp /= 10;
        maka /= 10;
    }
    data->exp += exp;
    data->maka += maka;

    if (isBoss != 0) {
        return;
    }

    if (devil->specialbit == 0 || evtBitCheck(devil->specialbit) != 0) {
        if (devil->specialitem != 0 &&
            (s32)dds3GetRandIntA(NULL, 100) < devil->specialpoint) {
            item = devil->specialitem;
        }
    }

    for (i = 0; i < 2 && item == 0; i++) {
        if (devil->dropitem[i] != 0 &&
            (s32)dds3GetRandIntA(NULL, 100) < devil->droppoint[i]) {
            item = devil->dropitem[i];
        }
    }

    if ((s32)dds3GetRandIntA(NULL, 100) < devil->hougyokupoint && item == 0) {
        item = 4;
    }
    if ((s32)dds3GetRandIntA(NULL, 100) < devil->masekipoint && item == 0) {
        item = 3;
    }

    if (item != 0) {
        for (i = 0; i < 4; i++) {
            if (data->item[i] == item) {
                data->itemcnt[i]++;
                return;
            }
        }
        for (i = 0; i < 4; i++) {
            if (data->item[i] == 0) {
                data->item[i] = item;
                data->itemcnt[i] = 1;
                return;
            }
        }
    }
}

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

extern s32 datCheckSkillInParty(s32 skill);
extern const char D_00544DA8[];

#ifdef NON_MATCHING
s32 nbCheckBackAttack(nbMainProcessData_t *data)
{
    datUnitWork_t *player;
    s32 enemyLevel;
    s32 enemyParam4;
    s32 enemyParam5;
    s32 playerLevel;
    s32 playerParam4;
    s32 playerParam5;
    s32 playerScore;
    s32 enemyScore;
    s32 rate = 12;
    s32 roll;
    s8 backAttack = nbEncountFormTable[data->encno].backAttack;

    if (backAttack == -1) return 0;
    if (backAttack == 1) return 1;
    enemyLevel = nbGetLvAvg2(data, 1);
    enemyParam4 = nbGetParamAvg2(data, 1, 4);
    enemyParam5 = nbGetParamAvg2(data, 1, 5);
    player = (datUnitWork_t *)&DDS3_GBWK->akuma[0].flag;
    playerLevel = player->level;
    playerParam4 = datGetParam(player, 4);
    playerParam5 = datGetParam(player, 5);
    playerScore = playerLevel + playerParam5 / 2 + playerParam4 + 10;
    enemyScore = enemyLevel + enemyParam5 / 2 + enemyParam4 + 10;
    roll = dds3GetRandIntA(NULL, 100);
    if (datCheckSkillInParty(0x12A) != 0) {
        rate = 12 / datSkillTable.special[0xA].n;
    }
    printf(D_00544DA8, playerScore, enemyScore, rate, roll);
    return roll < rate;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbCalc", nbCheckBackAttack);
#endif

extern s32 datCheckSkillInParty(s32 skill);
extern const char D_00544DE0[];

s32 nbCheckEscape(nbMainProcessData_t *data)
{
    s32 i;
    s32 allDisabled;
    datUnitWork_t *unit;
    s32 player4;
    s32 player5;
    s32 enemy4;
    s32 enemy5;
    s32 rate;
    s32 moonRoll;
    f32 ratio;

    if (nbEncountFormTable[data->encno].senseiFlags & 0x20) return 1;
    allDisabled = 1;
    for (i = 4; i < 14; i++) {
        unit = nbGetUnitWorkFromFormindex(i);
        if (unit != NULL) {
            switch (unit->badstatus & 0xFFF) {
            case 1:
            case 2:
            case 4:
            case 0x10:
            case 0x100:
            case 0x400:
                break;
            default:
                allDisabled = 0;
                break;
            }
        }
    }
    if (allDisabled != 0) return 1;
    player4 = nbGetParamAvg(data, 0, 4);
    player5 = nbGetParamAvg(data, 0, 5);
    enemy4 = nbGetParamAvg(data, 1, 4);
    enemy5 = nbGetParamAvg(data, 1, 5);
    ratio = (f32)(player4 + player5) / (enemy4 + enemy5);
    rate = (s32)(data->esccnt * 20 + ratio * 20.0f);
    if (datCheckSkillInParty(0x128) != 0) {
        rate = rate * datSkillTable.special[8].n / 100;
    }
    if (evtBitCheck(0xAF2) == 0) return 0;
    rate *= 2;
    printf(D_00544DE0, rate);
    if (rate >= (s32)dds3GetRandIntA(0, 100)) {
        if (evtGetAgeOfMoon() == 8) {
            moonRoll = dds3GetRandIntA(0, 4);
            if (moonRoll != 1) {
                if (evtBitCheck(0xAF2) != 0) {
                    if ((dds3GetRandIntA(0, 2) & 1) != 0) return 1;
                }
            } else {
                return moonRoll;
            }
        } else {
            return 1;
        }
    }
    return 0;
}

s32 nbGetEscapeSwitch(nbMainProcessData_t *data)
{
    if ((data->stat & 1) != 0) {
        return 0;
    }
    return nbEncountFormTable[data->encno].esc != 1;
}

extern const char D_00544DF0[];
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

extern f32 fabsf(f32 value);

#ifdef NON_MATCHING
s32 nbGetMakaBaramaki(datUnitWork_t *work)
{
    datUnitWork_t *player;
    f32 money = (f32)DDS3_GBWK->maka / 20.0f;
    f32 divisor;
    f32 ratio;
    f32 base;
    s32 amount;

    if (work->flag & 0x20) {
        player = (datUnitWork_t *)&DDS3_GBWK->akuma[0].flag;
        divisor = (f32)player->level / 5.0f + 4.0f;
        ratio = (f32)datGetParam(player, 5) / divisor;
        amount = (s32)((f32)datDevilTable[work->id].dropmakka * ratio);
    } else {
        divisor = (f32)work->level / 5.0f + 4.0f;
        ratio = (f32)datGetParam(work, 5) / divisor;
        if (fabsf(ratio) >= 0.0009999999311f) {
            amount = (s32)(money / ratio);
        } else {
            amount = 0;
        }
    }
    base = (f32)amount;
    amount = (s32)(base + base * (2.0f * (dds3GetRandFloatA(NULL) - 0.5f) * 0.09999999404f));
    if (evtBitCheck(0xAF2) != 0 && !(work->flag & 0x20)) {
        amount /= 10;
    }
    return amount > 0 ? amount : 1;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbCalc", nbGetMakaBaramaki);
#endif

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

INCLUDE_RODATA(const s32, "newbattle/nbCalc", D_00544D00);

INCLUDE_RODATA(const s32, "newbattle/nbCalc", D_00544D10);

INCLUDE_RODATA(const s32, "newbattle/nbCalc", D_00544D20);

INCLUDE_RODATA(const s32, "newbattle/nbCalc", D_00544D28);

INCLUDE_RODATA(const s32, "newbattle/nbCalc", D_00544D30);

INCLUDE_RODATA(const s32, "newbattle/nbCalc", D_00544D58);

INCLUDE_RODATA(const s32, "newbattle/nbCalc", D_00544D70);

INCLUDE_RODATA(const s32, "newbattle/nbCalc", D_00544DA8);

INCLUDE_RODATA(const s32, "newbattle/nbCalc", D_00544DE0);

INCLUDE_RODATA(const s32, "newbattle/nbCalc", D_00544DF0);

INCLUDE_SDATA(const s32, "newbattle/nbCalc", D_00551140);

