#include "common.h"
#include "datCalc.h"
#include "ps2asm.h"

extern s32 evtBitCheck(s32 no);
typedef struct datTakaraSagasiItem_t {
    u16 item;
    u16 par;
} datTakaraSagasiItem_t;

typedef struct datTakaraSagasiTable_t {
    u32 flag;
    datTakaraSagasiItem_t itemtbl[8];
} datTakaraSagasiTable_t;

/* FLD_SAGASI_AKUMA reads four inline 0x24-byte groups and their weights. */
typedef struct datTakaraSagasi_t {
    u32 flag;
    datTakaraSagasiTable_t tbl[4];
} datTakaraSagasi_t;
extern datTakaraSagasi_t datSagasiSkillTable;
extern void datDebugPrintf(const char *format, ...);
extern s32 GetButuriAttack(s32 nskill, datUnitWork_t *s, datUnitWork_t *d, s32 waza);
extern f64 sdfFloatToULong(f32 value);
extern f32 sdfDoubleToULong(f64 value);
EQUATE_DPTOFP();
EQUATE_FPTODP();
extern u32 datGetAisyo(datUnitWork_t *work, s32 attr);
extern s32 func_0022E168(u16 devilId);
typedef struct
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 skill;
    /* 0x04 */ u8 price[4];
} datItem_t;

extern datItem_t datItemTable[];

extern s32 datGetItemSlotIndex(s32 id);
extern void evtBitOff(s32 no);
typedef struct
{
    /* 0x00 */ u8 kind;
    /* 0x01 */ u8 type;
    /* 0x02 */ u16 skill;
} datLevelUpSkill_t;

typedef struct
{
    /* 0x00 */ u8 unk00[6];
    /* 0x06 */ datLevelUpSkill_t skill[24];
} datLevelUpRecord_t;

extern datLevelUpRecord_t datLevelUpTable[];
extern datUnitWork_t D_003493C0;

extern s32 memset(void *a, s32 b, s32 c);
extern s32 printf(const char *fmt, ...);
extern s32 datCopyDevilWorkEntry(datUnitWork_t *work, s32 level);
extern s32 fclEncycRegist(datUnitWork_t *work, s32 overwrite);
extern s32 datBossRashGetRec(s32 eid, s32 type);
typedef struct
{
    /* 0x00 */ s8 skillattr;
    /* 0x01 */ s8 type;
    /* 0x02 */ s16 index;
    /* 0x04 */ u16 keisyoform;
    /* 0x06 */ s8 capacity;
    /* 0x07 */ u8 flag;
} datSkill_t;

extern datSkill_t attr[];

extern s32 dds3GetRandIntA(s32 aLimit, s32 name);
extern s32 datCountRegisteredHearts(void);
extern void datAddHeartsEntry(s32 id, datUnitWork_t *work);
extern void evtBitOn(s32 no);
extern void cmbAddHeartsParam(s32 devilId, datUnitWork_t *work);
extern void rstLoadHeartsSkill(s8 HeartsID, datUnitWork_t *pStock);
extern u32 datAisyoTable[248][16];
extern void cmbSubHeartsParam(s32 devilId, datUnitWork_t *work);
extern void rstStoreHeartsSkill(s8 HeartsID, datUnitWork_t *pStock);
extern void datSetAutoKaifukuSkill(datUnitWork_t *work, s32 skill);
extern s32 datAddDevil(s32 id, s32 sw);
extern f32 D_0054E478;

extern s32 cmbGetHeartsParam(s8 devilId, s8 index);
extern void cmpSetMaxHPMP(datUnitWork_t *work);
extern s32 datGetPlayerLNDLevel(datUnitWork_t *work);

typedef struct
{
    /* 0x00 */ u32 flag;
    /* 0x04 */ u8 race;
    /* 0x05 */ u8 level;
    /* 0x06 */ u16 hp;
    /* 0x08 */ u16 maxhp;
    /* 0x0A */ u16 mp;
    /* 0x0C */ u16 maxmp;
    /* 0x0E */ s16 aisyoid;
    /* 0x10 */ s8 param[6];
    /* 0x16 */ u16 skill[8];
    /* 0x26 */ s16 keisyotype;
    /* 0x28 */ u16 keisyoform;
    /* 0x2A */ u16 dropmakka;
    /* 0x2C */ u16 dropexp;
    /* 0x2E */ u8 dropitem[2];
    /* 0x30 */ u8 droppoint[2];
    /* 0x32 */ u16 specialbit;
    /* 0x34 */ u8 specialitem;
    /* 0x35 */ u8 specialpoint;
    /* 0x36 */ u8 hougyokupoint;
    /* 0x37 */ u8 masekipoint;
    /* 0x38 */ s8 attackattr;
    /* 0x39 */ u8 attackcnt;
    /* 0x3A */ u8 attackinterval;
    /* 0x3B */ u8 reserve1;
} datDevilFormat_t;

extern datDevilFormat_t datDevilTable[];

s32 datGetBaseMaxHp(datUnitWork_t *work)
{
    s32 maxHp;

    if (work->flag & 4) {
        maxHp = (work->level + datGetParam(work, 3)) * 6;
    } else {
        maxHp = (work->level + datGetParam(work, 3)) * 6;
    }

    if ((work->flag & 0x20) == 0 && maxHp > 999) {
        maxHp = 999;
    }
    return maxHp;
}

s32 datGetBaseMaxMp(datUnitWork_t *work)
{
    s32 maxMp;

    if (work->flag & 4) {
        maxMp = ((work->level + datGetParam(work, 2)) * 2) * 1.5;
    } else {
        maxMp = ((work->level + datGetParam(work, 2)) * 2) * 1.5;
    }

    if ((work->flag & 0x20) == 0 && maxMp > 999) {
        maxMp = 999;
    }
    return maxMp;
}

s32 datGetNormalAtkPow(datUnitWork_t *work)
{
    s32 atkPow;

    if (work->flag & 4) {
        if (work->flag & 0x20) {
            atkPow = (datGetParam(work, 0) + work->level) * 2;
        } else {
            atkPow = (datGetParam(work, 0) + work->level) * 2;
        }
    } else {
        atkPow = (datGetParam(work, 0) + work->level) * 2;
    }

    if ((work->badstatus & 0xFFF) == 0x40) {
        atkPow = atkPow / 2;
    }
    return atkPow;
}

s32 datGetMagicAtkPow(datUnitWork_t *work)
{
    s32 atkPow;

    if (work->flag & 4) {
        if (work->flag & 0x20) {
        atkPow = ((datGetParam(work, 2) + work->level) * 2 + datGetParam(work, 2) / 2) * 0.85f;
        } else {
        atkPow = ((datGetParam(work, 2) + work->level) * 2 + datGetParam(work, 2) / 2) * 0.85f;
        }
    } else {
    atkPow = ((datGetParam(work, 2) + work->level) * 2 + datGetParam(work, 2) / 2) * 0.85f;
    }
    return atkPow;
}

s32 datGetNormalHitPow(datUnitWork_t *work)
{
    s32 sk;
    s32 lu;
    s32 hitPow;

    sk = datGetParam(work, 4);
    lu = datGetParam(work, 5);
    hitPow = sk * 2 + work->level;
    return hitPow + (lu + 5) / 2 + 0xF;
}

s32 datGetMagicHitPow(datUnitWork_t *work)
{
    s32 ma;
    s32 lu;
    s32 hitPow;

    ma = datGetParam(work, 2);
    lu = datGetParam(work, 5);
    hitPow = ma * 2 + work->level;
    return hitPow + (lu + 5) / 2 + 0xF;
}

s32 datGetDefPow(datUnitWork_t *work)
{
    return (datGetParam(work, 3) + work->level) * 2;
}

s32 datGetSakePow(datUnitWork_t *work)
{
    s32 sk = datGetParam(work, 4);
    s32 lu = datGetParam(work, 5);

    return (sk + 5) * 1.5 + (lu + 5) + (work->level + 5);
}

s32 GetButuriAttack(s32 nskill, datUnitWork_t *s, datUnitWork_t *d, s32 waza)
{
    s32 param;
    f32 value;

    datGetNormalAtkPow(s);
    datGetDefPow(d);

    if (s->flag & 0x20) {
        if (nskill == 0) {
            param = datGetParam(s, 0);
            value = ((f32)s->level * 2.0f + (f32)param * 2.0f) * 1.33f * 0.6f -
                    (48 / (s->level + 10));
        } else {
            param = datGetParam(s, 0);
            value = ((f32)s->level * 2.0f + (f32)param * 2.0f) * (f32)waza / 23.2f * 0.6f -
                    (50 / (s->level + 10));
        }
    } else if (nskill == 0) {
        param = datGetParam(s, 0);
        value = ((f32)s->level * 2.0f + (f32)param * 2.0f) * 1.33f * 0.8f;
    } else {
        param = datGetParam(s, 0);
        value = ((f32)s->level * 2.0f + (f32)param * 2.0f) * (f32)waza / 23.2f * 0.8f;
    }
    return value;
}

s32 GetButuriKaifuku(s32 nskill, datUnitWork_t *s, datUnitWork_t *d, s32 waza)
{
    return GetButuriAttack(nskill, s, d, waza);
}

s32 GetMagicAttack(s32 nskill, datUnitWork_t *s, datUnitWork_t *d, s32 waza)
{
    f32 value;
    f32 modifier;
    f32 result;
    s32 param;
    s32 level;
    s32 power;
    f32 magicBase;
    f32 magicLimit;

    magicLimit = (f32)datSkillTable.normal[nskill].magiclimit;
    magicBase = (f32)datSkillTable.normal[nskill].magicbase;
    power = s->level * waza;
    value = magicBase + (f32)(power * 2) / 21.0f;
    if (value > magicLimit) {
        value = magicLimit;
    }

    param = datGetParam(s, 2);
    level = s->level;
    modifier = ((f32)param - ((f32)level / 5.0f + 4.0f)) * 2.5f;
    level += 10;
    result = (f32)((f64)(value + (value / 100.0f) * modifier) * 0.8);

    if (s->flag & 0x20) {
        result = result * 0.75f - 50.0f / level;
    }
    return result;
}

s32 GetMagicKaifuku(s32 nskill, datUnitWork_t *s, datUnitWork_t *d, s32 waza)
{
    s32 ma = datGetParam(s, 2);
    s32 rnd = dds3GetRandIntA(0, 8);

    return (ma * 4 + s->level + rnd) / 10.0f * waza;
}

s32 GetMaxHpWazaPoint(s32 nskill, datUnitWork_t *s, datUnitWork_t *d, s32 waza)
{
    if (s->flag & 0x20) {
        return datGetBaseMaxHp(s) / 69.6f * waza * 0.6f - 50.0f / (s->level + 10);
    }
    return s->maxhp / 69.6f * waza;
}

f32 GetAisyoRitu(s32 nskill, datUnitWork_t *s, datUnitWork_t *d)
{
    s32 element = attr[nskill].skillattr;
    f32 ritu = 1.0f;

    switch (element) {
    case -1:
    case 12:
    case 13:
    case 14:
        break;
    default:
        ritu = (datGetAisyo(d, element) & 0xFFFF) * 0.01f;
    }
    return ritu;
}

INCLUDE_ASM(const s32, "newdata/datCalc", datGetSkillKouka);

INCLUDE_ASM(const s32, "newdata/datCalc", datGetSkillBadKouka);

extern s32 datGetSkillKouka(s32 nskill, s32 kind, datUnitWork_t *s, datUnitWork_t *d);
extern s32 datGetSkillBadKouka(s32 nskill, datUnitWork_t *s, datUnitWork_t *d);
extern void datResetBadStat(datUnitWork_t *work, s32 badstat);
extern void datSetBadStat(datUnitWork_t *work, u32 badstat);
extern s32 datAddMp(datUnitWork_t *work, s32 amount);

s32 datExecSkill(s32 nskill, datUnitWork_t *s, datUnitWork_t *d)
{
    s32 skillKouka;
    s32 mpKouka;
    s32 badKouka;
    u8 badType;

    if (s != NULL && d != NULL) {

        skillKouka = datGetSkillKouka(nskill, 0, s, d);
        mpKouka = datGetSkillKouka(nskill, 1, s, d);
        badKouka = datGetSkillBadKouka(nskill, s, d);
        if (skillKouka || mpKouka || badKouka) {
            if (badKouka != 0) {
                badType = datSkillTable.normal[nskill].badtype;
                if (badType == 1 || badType == 3) {
                    datSetBadStat(d, badKouka);
                } else {
                    datResetBadStat(d, badKouka);
                }
            }
            if (mpKouka != 0)
                datAddMp(d, mpKouka);
            if (skillKouka != 0) {
                datAddHp(d, skillKouka);
                if (d->hp == 0)
                    datSetBadStat(d, 0x800);
            }
        } else {
            return 0;
        }
    }
    switch (datSkillTable.normal[nskill].program) {
    case 8:
        DDS3_GBWK->maFlags |= 1;
        break;
    case 9:
        DDS3_GBWK->maFlags |= 2;
        break;
    case 10:
        DDS3_GBWK->maFlags |= 4;
        break;
    case 11:
        DDS3_GBWK->maFlags |= 8;
        break;
    }
    return 1;
}

void datOverwriteBadStat(datUnitWork_t *work, u32 badstat)
{
    if (badstat == 0x8000) {
        work->badstatus |= 0x8000;
    } else {
        work->badstatus = (work->badstatus & 0xF000) | (badstat & 0xFFF);
    }
}

void datSetBadStat(datUnitWork_t *work, u32 badstat)
{
    if (badstat == 0x8000) {
        work->badstatus |= 0x8000;
    } else if ((work->badstatus & 0xFFF) < badstat) {
        work->badstatus = (work->badstatus & 0xF000) | (badstat & 0xFFF);
    }
}

void datResetBadStat(datUnitWork_t *work, s32 badstat)
{
    work->badstatus &= ~badstat;
}

u32 datGetMaxHp(datUnitWork_t *work)
{
    u32 max;
    u32 add;

    add = 0;
    max = datGetBaseMaxHp(work);
    if (datCheckSyojiSkill(work, 0x122)) {
        add = max * 10 / 100;
    }
    if (datCheckSyojiSkill(work, 0x123)) {
        add += max * 20 / 100;
    }
    if (datCheckSyojiSkill(work, 0x124)) {
        add += max * 30 / 100;
    }
    max += add;
    if ((work->flag & 0x20) == 0 && max > 999) {
        max = 999;
    }
    return max;
}

u32 datGetMaxMp(datUnitWork_t *work)
{
    u32 max;
    u32 add;

    add = 0;
    max = datGetBaseMaxMp(work);
    if (datCheckSyojiSkill(work, 0x125)) {
        add = max * 10 / 100;
    }
    if (datCheckSyojiSkill(work, 0x126)) {
        add += max * 20 / 100;
    }
    if (datCheckSyojiSkill(work, 0x127)) {
        add += max * 30 / 100;
    }
    max += add;
    if ((work->flag & 0x20) == 0 && max > 999) {
        max = 999;
    }
    return max;
}

s32 datAddHp(datUnitWork_t *work, s32 hp)
{
    s32 newhp;

    newhp = work->hp + hp;
    if (newhp < 0) {
        newhp = 0;
    }
    if (newhp > work->maxhp) {
        newhp = work->maxhp;
    }
    work->hp = newhp;
    return newhp;
}

s32 datAddMp(datUnitWork_t *work, s32 amount)
{
    u32 mp = (u32)work->mp + amount;

    if ((s32)mp < 0) {
        mp = 0;
    }
    if ((s32)work->maxmp < (s32)mp) {
        mp = work->maxmp;
    }
    work->mp = mp;
    return mp;
}

s32 datGetBaseParam(datUnitWork_t *work, s32 paratype)
{
    s32 param;

    if (work->flag & 4) {
        param = work->param[paratype] + work->skillparam[paratype];
    } else {
        param = work->param[paratype] + work->skillparam[paratype] + work->mitamaparam[paratype];
    }

    if (param < 1) {
        param = 1;
    }
    if (param > 0x28) {
        param = 0x28;
    }
    return param;
}

s32 datGetParam(datUnitWork_t *work, s32 paratype)
{
    s32 param = datGetBaseParam(work, paratype);

    if ((work->badstatus & 0xFFF) == 0x200 && paratype != 4) {
        param = 1;
    }
    if (param < 1) {
        param = 1;
    }
    return param;
}

s32 datCheckSyojiSkill(datUnitWork_t *work, s32 skill)
{
    s32 i;

    if ((work->flag & 0x20) == 0) {
        for (i = 0; i < work->skillcnt; i++) {
            if (work->skill[i] == skill) {
                return 1;
            }
        }
    } else {
        for (i = 0; i < 8; i++) {
            if (datDevilTable[work->id].skill[i] == skill) {
                return 1;
            }
        }
    }
    return 0;
}

u32 datGetAisyo(datUnitWork_t *work, s32 attr)
{
    u32 aisyo;

    if (attr == -1) {
        return 0;
    }
    if (work->flag & 4) {
        aisyo = datAisyoTable[0][attr];
    } else {
        aisyo = datAisyoTable[datDevilTable[work->id].aisyoid][attr];
    }
    if ((work->badstatus & 0xFFF) == 0x400) {
        aisyo = datAisyoTable[246][attr];
    }
    return aisyo;
}

u32 datGetAisyoValue(datUnitWork_t *work, s32 attr)
{
    return (u16)datGetAisyo(work, attr);
}

u32 datGetAisyoFlag(datUnitWork_t *work, s32 attr)
{
    return datGetAisyo(work, attr) & 0xFFFF0000;
}

s32 datGetBadStatusAttr(s32 bad)
{
    s32 element = -1;

    switch (bad) {
    case 1:
        element = 3;
        break;
    case 2:
        element = 2;
        break;
    case 4:
        element = 0xA;
        break;
    case 8:
        element = 0xA;
        break;
    case 0x10:
        element = 9;
        break;
    case 0x20:
        element = 8;
        break;
    case 0x40:
        element = 8;
        break;
    case 0x80:
        element = 0xA;
        break;
    case 0x100:
        element = 9;
        break;
    case 0x200:
        element = 5;
        break;
    case 0x400:
        element = 7;
        break;
    case 0x800:
        break;
    case 0x1000:
        element = 5;
        break;
    case 0x2000:
        element = 5;
        break;
    case 0x4000:
        element = 5;
        break;
    }
    return element;
}

s32 datCheckDying(datUnitWork_t *work)
{
    return work->hp * 100 / work->maxhp < 25;
}

void datAddMaka(s32 addmaka)
{
    s32 balance = DDS3_GBWK->maka + addmaka;

    if (balance < 0) {
        balance = 0;
    }
    if (balance > 9999999) {
        balance = 9999999;
    }
    DDS3_GBWK->maka = balance;
}

void datAddMakaPar(s32 addmaka)
{
    s32 maka = DDS3_GBWK->maka;
    s32 delta;
    s32 mag;

    if (addmaka < 0) {
        mag = -addmaka;
    } else {
        mag = addmaka;
    }
    delta = maka * mag / 100;
    if (addmaka < 0) {
        maka = maka - delta;
    } else {
        maka = maka + delta;
    }

    if (maka < 0) {
        maka = 0;
    }
    if (maka > 9999999) {
        maka = 9999999;
    }
    DDS3_GBWK->maka = maka;
}

s32 datCheckMaka(s32 maka)
{
    if (DDS3_GBWK->maka < maka) {
        return 0;
    }
    return 1;
}

void datAddItem(s32 id, s32 num)
{
    s32 slot = datGetItemSlotIndex((u16)id);
    s32 count;

    if (slot != 0) {
        datAddHearts(slot);
        return;
    }
    if (id >= 0x70) {
        evtBitOn(id + 0x360);
        if (id != 0x8D) {
            return;
        }
        count = ((u8 *)id + (u32)DDS3_GBWK)[0x1928];
        if (num > 0) {
            count += num;
            if (count >= 0x64) {
                count = 0x63;
            }
        } else if (num < 0) {
            count += num;
            if (count <= 0) {
                count = 0;
                evtBitOff(0x3ED);
            }
        }
        ((u8 *)id + (u32)DDS3_GBWK)[0x1928] = count;
    } else {
        u32 ofs = id + 0x1920;

        count = ((u8 *)DDS3_GBWK + ofs)[8];
        count += num;
        if (count < 0) {
            count = 0;
        }
        if (id >= 0x60) {
            if (count >= 0x64) {
                count = 0x63;
            }
        } else {
            if (count >= 0x64) {
                count = 0x63;
            }
        }
        ((u8 *)DDS3_GBWK + ofs)[8] = count;
    }
}

s32 datCheckItem(s32 id, s32 num)
{
    if (((u8 *)id + (u32)DDS3_GBWK)[0x1928] < num) {
        return 0;
    }
    return 1;
}

s32 datCheckItemFull(s32 id)
{
    s32 count = ((u8 *)id + (u32)DDS3_GBWK)[0x1928];

    if (id >= 0x70) {
        if (count > 0) {
            return 1;
        }
    } else if (id >= 0x60) {
        if (count >= 0x63) {
            return 1;
        }
    } else {
        if (count >= 0x63) {
            return 1;
        }
    }
    return 0;
}

s32 datGetRace(datUnitWork_t *work)
{
    if (work->flag & 4) {
        return 0;
    }
    return datDevilTable[work->id].race;
}

s32 datCheckTalk(datUnitWork_t *work)
{
    switch (work->badstatus & 0xFFF) {
    case 1:
    case 2:
    case 4:
    case 8:
    case 0x10:
    case 0x200:
    case 0x400:
        return 0;
    }
    return 1;
}

s32 datSearchDevilStock(s32 id)
{
    s32 i;

    for (i = 0; i < 16; i++) {
        u32 flag = DDS3_GBWK->akuma[i].flag;

        if ((flag & 1) == 0) {
            continue;
        }
        if (flag & 4) {
            continue;
        }
        if (DDS3_GBWK->akuma[i].devilId == id) {
            return i;
        }
    }
    return -1;
}

s32 datGetPlayerLevel(void)
{
    return ((datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54))->level;
}

s32 datCheckPlayerDead(void)
{
    return (((datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54))->badstatus & 0xFFF) == 0x800;
}

/* NON_MATCHING: 7 of 20 words differ (first at +0x4). */
#ifdef NON_MATCHING
s32 datGetPlayerParam(s32 id)
{
    u8 hearts = DDS3_GBWK->heartsequip;
    s32 param = ((s8 *)((u8 *)id + (u32)DDS3_GBWK))[0xA6A];

    if (hearts == 0) {
        return param;
    }
    return param - cmbGetHeartsParam(hearts, id);
}
#else
INCLUDE_ASM(const s32, "newdata/datCalc", datGetPlayerParam);
#endif

void datAddPlayerParam(s32 id, s32 add)
{
    s8 *param = (s8 *)((u8 *)id + (u32)DDS3_GBWK) + 0xA6A;
    s32 value = *param + add;

    if (value >= 0x29) {
        value = 0x28;
    }
    *param = value;
    cmpSetMaxHPMP((datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54));
    ((datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54))->hp =
        ((datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54))->maxhp;
    ((datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54))->mp =
        ((datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54))->maxmp;
}

s32 datGetNakamaParam(s32 did, s32 id)
{
    u16 *devilid = &((datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54) + 1)->id;
    s8 *param = (s8 *)((u8 *)id + (u32)DDS3_GBWK) + 0xB56;
    s32 value = 0;
    s32 i;

    for (i = 0; i < 15; i++) {
        if (*devilid == did) {
            value = *param;
        }
        devilid = (u16 *)((u8 *)devilid + 0xEC);
        param += 0xEC;
    }
    return value;
}

s32 datGetPlayerLND(void)
{
    s32 level = datGetPlayerLNDLevel((datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54));

    if (level == 0) {
        return 1;
    }
    return (level - 1) % 3;
}

/* NON_MATCHING: 28 of 48 words differ (first at +0x44). */
#ifdef NON_MATCHING
void datDeadNakama(s32 did)
{
    s32 ofs;
    s32 i;

    ofs = 0xEC;
    for (i = 0; i < 15; i++) {
        u32 aofs = ofs + 0xA50;

        if (((fldAnahoriAkuma_t *)((u8 *)DDS3_GBWK + aofs))->flag & 1) {
            if (((fldAnahoriAkuma_t *)((u8 *)DDS3_GBWK + aofs))->devilId == did) {
                if (((fldAnahoriAkuma_t *)((u8 *)DDS3_GBWK + aofs))->hp != 0) {
                    ((fldAnahoriAkuma_t *)((u8 *)DDS3_GBWK + aofs))->hp = 0;
                    datSetBadStat((datUnitWork_t *)((u8 *)DDS3_GBWK + ofs + 0xA54), 0x800);
                }
                if (((fldAnahoriAkuma_t *)((u8 *)DDS3_GBWK + aofs))->flag & 2) {
                    ((fldAnahoriAkuma_t *)((u8 *)DDS3_GBWK + aofs))->flag &= ~2;
                }
            }
        }
        ofs += 0xEC;
    }
}
#else
INCLUDE_ASM(const s32, "newdata/datCalc", datDeadNakama);
#endif

INCLUDE_ASM(const s32, "newdata/datCalc", datPartyDamage);

INCLUDE_ASM(const s32, "newdata/datCalc", datPartyDamageFld);

/* NON_MATCHING: 54 of 68 words differ (first at +0x4). */
#ifdef NON_MATCHING
void datPartyDamagePipe(s32 big, s32 sml)
{
    s32 aofs;
    s32 i;

    if (big == 0 && sml == 0) {
        return;
    }
    aofs = 0xA50;
    for (i = 0; i < 16; i++) {
        if (((fldAnahoriAkuma_t *)((u8 *)DDS3_GBWK + aofs))->flag & 2) {
            s32 hp = ((fldAnahoriAkuma_t *)((u8 *)DDS3_GBWK + aofs))->hp;

            if (hp != 0) {
                s32 damage = hp / 10 * big + hp * 3 / 100 * sml;

                if (evtBitCheck(0xAF2) == 0) {
                    damage *= 3;
                }
                if (damage == 0) {
                    damage = 1;
                }
                hp -= damage;
                if (hp <= 0) {
                    hp = 1;
                }
                ((fldAnahoriAkuma_t *)((u8 *)DDS3_GBWK + aofs))->hp = hp;
            }
        }
        aofs += 0xEC;
    }
}
#else
INCLUDE_ASM(const s32, "newdata/datCalc", datPartyDamagePipe);
#endif

void datPartyKaifuku(void)
{
    s32 ofs;
    s32 i;

    ofs = 0;
    for (i = 0; i < 16; i++) {
        u32 aofs = ofs + 0xA50;

        if (((fldAnahoriAkuma_t *)((u8 *)DDS3_GBWK + aofs))->flag & 1) {
            datAddHp((datUnitWork_t *)((u8 *)DDS3_GBWK + ofs + 0xA54), 0x270F);
            datAddMp((datUnitWork_t *)((u8 *)DDS3_GBWK + ofs + 0xA54), 0x270F);
            ((fldAnahoriAkuma_t *)((u8 *)DDS3_GBWK + aofs))->badstatus = 0;
        }
        ofs += 0xEC;
    }
}

s32 datGet3Zokusei(datUnitWork_t *work)
{
    u32 flag;

    if (work->flag & 4) {
        return 0;
    }
    flag = datDevilTable[work->id].flag;
    if (flag & 0x40) {
        return 1;
    }
    if (flag & 0x80) {
        return -1;
    }
    return 0;
}

s32 datCheckStockFull(void)
{
    datUnitWork_t *unit = (datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54) + 4;
    s32 num = 0;
    s32 i;

    for (i = 0; i < 12; i++) {
        num += unit->flag & 1;
        unit++;
    }
    if (num < *(s32 *)((u8 *)DDS3_GBWK + 0xA50)) {
        return 0;
    }
    return 1;
}

void datSetDevilWork(s32 id, datUnitWork_t *work)
{
    s32 i;
    s32 skillcnt;
    s32 maxHp;
    s32 maxMp;

    memset(work, 0, sizeof(*work));
    work->flag = 1;
    work->id = id;
    work->level = datDevilTable[id].level;
    work->exp = 0;
    for (i = 0; i < 6; i++) {
        work->param[i] = datDevilTable[id].param[i];
    }
    skillcnt = 0;
    for (i = 0; i < 24; i++) {
        if (datLevelUpTable[id].skill[i].kind == 0 && datLevelUpTable[id].skill[i].type == 1) {
            work->skill[skillcnt++] = datLevelUpTable[id].skill[i].skill;
        }
    }
    maxHp = datGetMaxHp(work);
    work->maxhp = maxHp;
    work->hp = maxHp;
    maxMp = datGetMaxMp(work);
    work->maxmp = maxMp;
    work->mp = maxMp;
    work->skillcnt = skillcnt;
}

/* Partial view of the persistent party and stock records. */
typedef struct {
    u8 unknown00[0xA50];
    /* 0xA50 */ s32 maxstock;
    /* 0xA54 */ datUnitWork_t unitwork[16];
    /* 0x1914 */ s8 stocklist[16];
    /* 0x1924 */ s32 stockcnt;
} datPartyWork_t;

/* The same persistent-work pointer, through its recovered party/stock view. */
extern datPartyWork_t *datPartyWork asm("DDS3_GBWK");

s32 datAddDevil(s32 id, s32 sw)
{
    s32 partyCount = 0;
    s32 registerEntry = 1;
    s32 i;
    datUnitWork_t *party;

    if (id < 0) {
        registerEntry = 0;
        id &= 0x7FFFFFFF;
    }
    party = datPartyWork->unitwork;
    for (i = 15; i >= 0; i--, party++) {
        if (party->flag & 1) {
            if (party->flag & 2) partyCount++;
        }
    }
    for (i = 0; i < datPartyWork->maxstock; i++) {
        if ((datPartyWork->unitwork[i + 4].flag & 1) == 0) break;
    }
    if (i == datPartyWork->maxstock) return -1;
    datSetDevilWork(id, &datPartyWork->unitwork[i + 4]);
    if (registerEntry) {
        fclEncycRegist(&datPartyWork->unitwork[i + 4], 0);
    }
    if (sw && partyCount < 4) {
        datPartyWork->unitwork[i + 4].flag |= 2;
    }
    datPartyWork->stocklist[datPartyWork->stockcnt] = i + 4;
    datPartyWork->stockcnt++;
    return i + 4;
}
s32 datAddUniqueDevil(s32 id, s32 sw, s32 uid)
{
    s32 index = datAddDevil(id, sw);
    u32 flag = datPartyWork->unitwork[index].flag;

    datPartyWork->unitwork[index].uniqueid = uid;
    datPartyWork->unitwork[index].flag = flag | 0x400;
    if (uid == 1) {
        datPartyWork->unitwork[index].flag = flag | 0x1400;
    }
    return index;
}

s32 datSearchUniqueDevilStock(s32 uid)
{
    u16 *uidp = &((datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54))->uniqueid;
    u32 *flagp = &((datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54))->flag;
    s32 i;

    for (i = 0; i < 16; i++, flagp = (u32 *)((u8 *)flagp + 0xEC), uidp = (u16 *)((u8 *)uidp + 0xEC)) {
        u32 flag = *flagp;

        if ((flag & 1) == 0) {
            continue;
        }
        if (flag & 4) {
            continue;
        }
        if ((flag & 0x400) == 0) {
            continue;
        }
        if (*uidp == uid) {
            return i;
        }
    }
    return -1;
}

s32 datGetUniqueDevilId(s32 uid)
{
    u16 *idp = &((datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54))->id;
    u32 *flagp = &((datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54))->flag;
    s32 i;

    for (i = 0; i < 16; i++, flagp = (u32 *)((u8 *)flagp + 0xEC), idp = (u16 *)((u8 *)idp + 0xEC)) {
        u32 flag = *flagp;

        if ((flag & 1) == 0) {
            continue;
        }
        if (flag & 4) {
            continue;
        }
        if ((flag & 0x400) == 0) {
            continue;
        }
        if (idp[0xF] == uid) {
            return *idp;
        }
    }
    return -1;
}

void datAddExp(datUnitWork_t *w, s32 exp)
{
    w->exp += exp;
}

s32 datAddHearts(s32 id)
{
    s32 i;

    for (i = 0; i < DDS3_GBWK->heartscnt; i++) {
        if (DDS3_GBWK->hearts[i] == id) {
            return 0;
        }
    }
    ((u8 *)(u32)DDS3_GBWK->heartscnt + (u32)DDS3_GBWK)[0x1CFB] = id;
    DDS3_GBWK->heartscnt++;
    ((u8 *)id + (u32)DDS3_GBWK)[0x1D3B] = 0;
    if (datCountRegisteredHearts() != 0) {
        evtBitOn(0x860);
    }
    datAddHeartsEntry((u8)id, (datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54));
    return 1;
}

void datRemoveHearts(void)
{
    datUnitWork_t *player = (datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54);

    if (DDS3_GBWK->heartscnt != 0) {
        cmbSubHeartsParam(DDS3_GBWK->heartsequip, player);
        rstStoreHeartsSkill(DDS3_GBWK->heartsequip, player);
        DDS3_GBWK->heartsequip = 0;
        cmpSetMaxHPMP(player);
    }
}

void datEquipHearts(s32 HeartsID)
{
    datUnitWork_t *player = (datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54);

    if (DDS3_GBWK->heartscnt != 0) {
        if (DDS3_GBWK->heartsequip != HeartsID) {
            datRemoveHearts();
            DDS3_GBWK->heartsequip = HeartsID;
            cmbAddHeartsParam(DDS3_GBWK->heartsequip, player);
            rstLoadHeartsSkill(DDS3_GBWK->heartsequip, player);
            cmpSetMaxHPMP(player);
        }
    }
}

INCLUDE_ASM(const s32, "newdata/datCalc", datSetAutoKaifukuSkill);

void datFieldSkillKaifuku(void)
{
    s32 ofs;
    s32 i;

    ofs = 0;
    for (i = 0; i < 16; i++) {
        datUnitWork_t *unit = (datUnitWork_t *)((u8 *)DDS3_GBWK + ofs + 0xA54);
        u32 flag = unit->flag;

        ofs += 0xEC;
        if (flag & 1) {
            if (flag & 2) {
                datSetAutoKaifukuSkill(unit, 0x15D);
                datSetAutoKaifukuSkill(unit, 0x15E);
            }
        }
    }
}

s32 datCheckSkillInParty(s32 skillid)
{
    s32 i;
    s32 ofs;

    for (i = 0, ofs = 0; i < 16; i++) {
        datUnitWork_t *unit = (datUnitWork_t *)((u8 *)DDS3_GBWK + ofs + 0xA54);
        u32 flag = unit->flag;

        ofs += 0xEC;
        if (flag & 1) {
            if (flag & 2) {
                if (datCheckSyojiSkill(unit, skillid)) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

s32 datCheckSkillTakaraSagasi(void)
{
    s32 i;

    if (datSagasiSkillTable.flag != 0 && evtBitCheck(datSagasiSkillTable.flag) == 0) {
        return 0;
    }
    for (i = 0; i < 16; i++) {
        datUnitWork_t *unit = (datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54 + i * 0xEC);
        s32 luck;
        s32 random;

        if ((unit->flag & 1) == 0) {
            continue;
        }
        if ((unit->flag & 2) == 0) {
            continue;
        }
        luck = (unit->param[5] << 7) / 100;
        if (datCheckSyojiSkill(unit, 0x161) == 0) {
            continue;
        }
        random = dds3GetRandIntA(0, 0x20);
        datDebugPrintf("dvl:%d  luc:%d  rnd:%d\n", unit->id, luck, random);
        if (luck >= random) {
            return unit->id;
        }
    }
    return 0;
}

void datClearCommMemory(datUnitWork_t *work)
{
    work->prevcommand = 0;
    work->previndex = 0;
    work->prevtarea = -1;
    work->prevtform = -1;
}

void datAllClearCommMemory(void)
{
    s32 ofs;
    s32 i;

    ofs = 0;
    for (i = 0; i < 16; i++) {
        datClearCommMemory((datUnitWork_t *)((u8 *)DDS3_GBWK + ofs + 0xA54));
        ofs += 0xEC;
    }
}

s32 datGetNakamaNum(void)
{
    datUnitWork_t *unit = (datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54) + 4;
    s32 num = 0;
    s32 i;

    for (i = 0; i < 12; i++) {
        num += unit->flag & 1;
        unit++;
    }
    return num;
}

void datAddKotowariPoint(s32 type, s32 point)
{
    u8 *slot;
    s32 value;

    if (type != 0) {
        slot = (u8 *)type + (u32)DDS3_GBWK + 0x1CEC;
        value = *slot + point;
        if (value < 0) {
            value = 0;
        }
        if (value > 0x80) {
            value = 0x80;
        }
        *slot = value;
    }
}

s32 datGetKotowariLank(s32 lank)
{
    s32 rank[4];
    s32 i;
    s32 j;

    for (i = 1; i < 4; i++) {
        rank[i] = i;
    }
    for (i = 1; i < 3; i++) {
        for (j = i + 1; j < 4; j++) {
            s32 left = DDS3_GBWK->kotowari[rank[i]];
            s32 right = DDS3_GBWK->kotowari[rank[j]];
            s32 temp;

            if (left > right) {
                continue;
            }
            if (left == right && dds3GetRandIntA(0, 100) < 50) {
                continue;
            }
            temp = rank[i];
            rank[i] = rank[j];
            rank[j] = temp;
        }
    }
    return rank[lank + 1];
}

s32 datGetSkillCost(datUnitWork_t *w, s32 nskill)
{
    s32 cost = 0;

    switch (datSkillTable.normal[nskill].costtype) {
    case 1:
        if ((w->flag & 4) == 0) {
            if (datDevilTable[w->id].flag & 0x200) {
                return 0;
            }
            if (w->flag & 0x20) {
                return 0;
            }
        }
        cost = w->maxhp * datSkillTable.normal[nskill].cost / 100 +
               datSkillTable.normal[nskill].costbase;
        if (cost <= 0) {
            cost = 1;
        }
        break;
    case 2:
        if ((w->flag & 4) == 0) {
            if (datDevilTable[w->id].flag & 0x400) {
                return 0;
            }
        }
        cost = datSkillTable.normal[nskill].cost;
        break;
    }
    return cost;
}

/* NON_MATCHING: 7 of 75 words differ (first at +0xE4). */
#ifdef NON_MATCHING
void datInitPlayer(void)
{
    u8 *gbwk;
    s32 i;

    memset((u8 *)DDS3_GBWK + 0xA54, 0, 0xEC0);
    *(datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54) = D_003493C0;
    gbwk = (u8 *)DDS3_GBWK;
    *(s32 *)(gbwk + 0x1924) = 1;
    gbwk[0x1914] = 0;
    DDS3_GBWK->maka = 0;
    for (i = 0; i < 0xA0; i++) {
        ((u8 *)i + (u32)DDS3_GBWK)[0x1928] = 0;
    }
    evtBitOn(0xC04);
}
#else
INCLUDE_ASM(const s32, "newdata/datCalc", datInitPlayer);
#endif

void datInitBossPress(void)
{
    s16 *rec = (s16 *)((u8 *)DDS3_GBWK + 0x19CA);
    s32 i;

    for (i = 0; i < 40; i++) {
        rec[-1] = -1;
        rec[0] = -1;
        rec += 2;
    }
}

void datDelDevil(u16 id)
{
    func_0022E168(id);
}

s32 datGetItemSkillNo(s32 itemid)
{
    s32 list[6] = { 0xED, 0xEE, 0xEF, 0xF0, 0xF6, 0xF8 };

    if (itemid == 1) {
        return list[dds3GetRandIntA(0, 6)];
    }
    return datItemTable[itemid].skill;
}

u16 datGetLevelForDraw(datUnitWork_t *work)
{
    s32 level = work->level;

    return level < 100 ? level : 99;
}


void datCopyDevilWork(s32 id, datUnitWork_t *w)
{
    u16 *skill;
    s32 cnt;
    s32 i;

    w->level = datDevilTable[id].level;
    for (i = 0; i < 6; i++) {
        w->param[i] = datDevilTable[id].param[i];
    }
    cnt = 0;
    skill = w->skill;
    for (i = 0; i < 24; i++) {
        if (datLevelUpTable[id].skill[i].kind == 0 &&
            datLevelUpTable[id].skill[i].type == 1) {
            *skill++ = datLevelUpTable[id].skill[i].skill;
            cnt++;
        }
    }
    w->maxhp = datGetMaxHp(w);
    w->hp = w->maxhp;
    w->maxmp = datGetMaxMp(w);
    w->mp = w->maxmp;
    w->skillcnt = cnt;
    printf("YAMI-B Lv:%d\n", w->level);
    w->exp = datCopyDevilWorkEntry(w, w->level);
    printf("YAMI-B Exp:%d\n", w->exp);
    fclEncycRegist(w, 0);
}

INCLUDE_ASM(const s32, "newdata/datCalc", datBossRashGetRec);

void datBossRashUpdRec(s32 eid, s32 press)
{
    datBossPress_t *rec;
    s32 value;
    s32 best;
    s32 i;

    if (press != 0) {
        value = press;
    } else {
        value = 1;
    }
    if (eid == 0x3E1) {
        eid = 0x1D8;
    }
    if (eid == 0x3E0) {
        eid = 0x14E;
    }
    rec = datBossPress;
    for (i = 0; i < 40; i++) {
        if (rec->encid == eid) {
            if (evtBitCheck(rec->flag) == 0) {
                return;
            }
            *(s16 *)((u8 *)(i * 4) + (u32)DDS3_GBWK + 0x19CA) = value;
            best = datBossRashGetRec(eid, 0);
            if (value < best) {
                *(s16 *)((u8 *)(i * 4) + (u32)DDS3_GBWK + 0x19C8) = value;
                evtBitOn(rec->flag2);
            }
            return;
        }
        rec++;
    }
}

s32 datBossRashChk(s32 eid)
{
    datBossPress_t *press;
    s32 i;

    press = datBossPress;
    for (i = 0; i < 40; ++i) {
        if (press->encid == eid) {
            if (evtBitCheck(press->flag) != 0) {
                return 1;
            }
        }
        press++;
    }
    return 0;
}

