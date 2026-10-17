#include "common.h"
#include "datCalc.h"

extern s32 evtBitCheck(s32 no);

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

INCLUDE_ASM(const s32, "newdata/datCalc", GetButuriKaifuku);

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

INCLUDE_ASM(const s32, "newdata/datCalc", datResetBadStat);

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

INCLUDE_ASM(const s32, "newdata/datCalc", datAddMp);

s32 datGetBaseParam(datUnitWork_t *work, s32 paratype)
{
    s32 param;

    if (work->flag & 4) {
        param = work->param[paratype] + work->skillparam[paratype];
    } else {
        param = work->param[paratype] + work->skillparam[paratype] + work->levelupparam[paratype];
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

INCLUDE_ASM(const s32, "newdata/datCalc", datGetAisyoValue);

INCLUDE_ASM(const s32, "newdata/datCalc", datGetAisyoFlag);

INCLUDE_ASM(const s32, "newdata/datCalc", datGetBadStatusAttr);

INCLUDE_ASM(const s32, "newdata/datCalc", datCheckDying);

INCLUDE_ASM(const s32, "newdata/datCalc", datAddMaka);

INCLUDE_ASM(const s32, "newdata/datCalc", datAddMakaPar);

INCLUDE_ASM(const s32, "newdata/datCalc", datCheckMaka);

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

INCLUDE_ASM(const s32, "newdata/datCalc", datAddExp);

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

INCLUDE_ASM(const s32, "newdata/datCalc", datDelDevil);

INCLUDE_ASM(const s32, "newdata/datCalc", datGetItemSkillNo);

INCLUDE_ASM(const s32, "newdata/datCalc", datGetLevelForDraw);

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

INCLUDE_RODATA(const s32, "newdata/datCalc", D_005431A8);

INCLUDE_RODATA(const s32, "newdata/datCalc", D_005431B8);

INCLUDE_RODATA(const s32, "newdata/datCalc", D_005431C8);

