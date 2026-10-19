#include "common.h"
#include "datCalc.h"

extern s32 evtBitCheck(s32 no);
extern s32 GetButuriAttack(s32 skill, datUnitWork_t *attacker, datUnitWork_t *target, s32 power);
extern s32 datGetAisyo(datUnitWork_t *work, s32 element);
extern void func_0022E168(u16 devilId);

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

INCLUDE_ASM(const s32, "newdata/datCalc", datGetMagicAtkPow);

INCLUDE_ASM(const s32, "newdata/datCalc", datGetNormalHitPow);

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

INCLUDE_ASM(const s32, "newdata/datCalc", datGetSakePow);

INCLUDE_ASM(const s32, "newdata/datCalc", GetButuriAttack);

s32 GetButuriKaifuku(s32 skill, datUnitWork_t *attacker, datUnitWork_t *target, s32 power)
{
    return GetButuriAttack(skill, attacker, target, power);
}

INCLUDE_ASM(const s32, "newdata/datCalc", GetMagicAttack);

INCLUDE_ASM(const s32, "newdata/datCalc", GetMagicKaifuku);

INCLUDE_ASM(const s32, "newdata/datCalc", GetMaxHpWazaPoint);

INCLUDE_ASM(const s32, "newdata/datCalc", GetAisyoRitu);

INCLUDE_RODATA(const s32, "newdata/datCalc", D_005430C0);

INCLUDE_ASM(const s32, "newdata/datCalc", datGetSkillKouka);

INCLUDE_ASM(const s32, "newdata/datCalc", datGetSkillBadKouka);

INCLUDE_ASM(const s32, "newdata/datCalc", datExecSkill);

INCLUDE_ASM(const s32, "newdata/datCalc", datOverwriteBadStat);

INCLUDE_ASM(const s32, "newdata/datCalc", datSetBadStat);

void datResetBadStat(datUnitWork_t *work, s32 mask)
{
    work->badstatus &= ~mask;
}

INCLUDE_ASM(const s32, "newdata/datCalc", datGetMaxHp);

INCLUDE_ASM(const s32, "newdata/datCalc", datGetMaxMp);

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

INCLUDE_ASM(const s32, "newdata/datCalc", datCheckSyojiSkill);

INCLUDE_ASM(const s32, "newdata/datCalc", datGetAisyo);

u32 datGetAisyoValue(datUnitWork_t *work, s32 element)
{
    return (u16)datGetAisyo(work, element);
}

u32 datGetAisyoFlag(datUnitWork_t *work, s32 element)
{
    return datGetAisyo(work, element) & 0xFFFF0000;
}

INCLUDE_ASM(const s32, "newdata/datCalc", datGetBadStatusAttr);

s32 datCheckDying(datUnitWork_t *work)
{
    return work->hp * 100 / work->maxhp < 25;
}

void datAddMaka(s32 amount)
{
    s32 balance = DDS3_GBWK->maka + amount;

    if (balance < 0) {
        balance = 0;
    }
    if (balance > 9999999) {
        balance = 9999999;
    }
    DDS3_GBWK->maka = balance;
}

INCLUDE_ASM(const s32, "newdata/datCalc", datAddMakaPar);

s32 datCheckMaka(s32 amount)
{
    if (DDS3_GBWK->maka < amount) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "newdata/datCalc", datAddItem);

INCLUDE_ASM(const s32, "newdata/datCalc", datCheckItem);

INCLUDE_ASM(const s32, "newdata/datCalc", datCheckItemFull);

INCLUDE_ASM(const s32, "newdata/datCalc", datGetRace);

INCLUDE_ASM(const s32, "newdata/datCalc", datCheckTalk);

INCLUDE_ASM(const s32, "newdata/datCalc", datSearchDevilStock);

INCLUDE_ASM(const s32, "newdata/datCalc", datGetPlayerLevel);

INCLUDE_ASM(const s32, "newdata/datCalc", datCheckPlayerDead);

INCLUDE_ASM(const s32, "newdata/datCalc", datGetPlayerParam);

INCLUDE_ASM(const s32, "newdata/datCalc", datAddPlayerParam);

INCLUDE_ASM(const s32, "newdata/datCalc", datGetNakamaParam);

INCLUDE_ASM(const s32, "newdata/datCalc", datGetPlayerLND);

INCLUDE_ASM(const s32, "newdata/datCalc", datDeadNakama);

INCLUDE_ASM(const s32, "newdata/datCalc", datPartyDamage);

INCLUDE_ASM(const s32, "newdata/datCalc", datPartyDamageFld);

INCLUDE_ASM(const s32, "newdata/datCalc", datPartyDamagePipe);

INCLUDE_ASM(const s32, "newdata/datCalc", datPartyKaifuku);

INCLUDE_ASM(const s32, "newdata/datCalc", datGet3Zokusei);

INCLUDE_ASM(const s32, "newdata/datCalc", datCheckStockFull);

INCLUDE_ASM(const s32, "newdata/datCalc", datSetDevilWork);

INCLUDE_ASM(const s32, "newdata/datCalc", datAddDevil);

INCLUDE_ASM(const s32, "newdata/datCalc", datAddUniqueDevil);

INCLUDE_ASM(const s32, "newdata/datCalc", datSearchUniqueDevilStock);

INCLUDE_ASM(const s32, "newdata/datCalc", datGetUniqueDevilId);

void datAddExp(datUnitWork_t *work, s32 amount)
{
    work->exp += amount;
}

INCLUDE_ASM(const s32, "newdata/datCalc", datAddHearts);

INCLUDE_ASM(const s32, "newdata/datCalc", datRemoveHearts);

INCLUDE_ASM(const s32, "newdata/datCalc", datEquipHearts);

INCLUDE_ASM(const s32, "newdata/datCalc", datSetAutoKaifukuSkill);

INCLUDE_ASM(const s32, "newdata/datCalc", datFieldSkillKaifuku);

INCLUDE_ASM(const s32, "newdata/datCalc", datCheckSkillInParty);

INCLUDE_ASM(const s32, "newdata/datCalc", datCheckSkillTakaraSagasi);

INCLUDE_ASM(const s32, "newdata/datCalc", datClearCommMemory);

INCLUDE_ASM(const s32, "newdata/datCalc", datAllClearCommMemory);

INCLUDE_ASM(const s32, "newdata/datCalc", datGetNakamaNum);

INCLUDE_ASM(const s32, "newdata/datCalc", datAddKotowariPoint);

INCLUDE_ASM(const s32, "newdata/datCalc", datGetKotowariLank);

INCLUDE_ASM(const s32, "newdata/datCalc", datGetSkillCost);

INCLUDE_ASM(const s32, "newdata/datCalc", datInitPlayer);

INCLUDE_ASM(const s32, "newdata/datCalc", datInitBossPress);

void datDelDevil(u16 devilId)
{
    func_0022E168(devilId);
}

INCLUDE_ASM(const s32, "newdata/datCalc", datGetItemSkillNo);

u16 datGetLevelForDraw(datUnitWork_t *work)
{
    s32 level = work->level;

    return level < 100 ? level : 99;
}

INCLUDE_RODATA(const s32, "newdata/datCalc", D_00543170);

INCLUDE_ASM(const s32, "newdata/datCalc", datCopyDevilWork);

INCLUDE_ASM(const s32, "newdata/datCalc", datBossRashGetRec);

INCLUDE_ASM(const s32, "newdata/datCalc", datBossRashUpdRec);

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

