#include "common.h"
#include "facility.h"
#include "camp.h"
#include "result2.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"
#include "kernel.h"
#include "font.h"

extern datDevilTableEntry_t datDevilTable[];

extern u32 dds3GetRandA(void *state);

extern s32 evtBitCheck(s32 no);

extern s8 cmbAddLevelUpParamEx(void *work, s8 mode);

extern s32 datCopyDevilWorkEntry(datUnitWork_t *work, s32 level);

extern s32 dds3PadVibrationSet(s32 port, s32 power, s32 frames);

typedef struct
{
    /* 0x00 */ u16 skill;
    /* 0x02 */ u16 unk_02;
} datHeartsSkill_t;

typedef struct
{
    /* 0x00 */ u8 race;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ s16 param;
    /* 0x04 */ u8 unk_04[0x15];
    /* 0x19 */ s8 unk_19;
    /* The same eight records expose condition bytes before each skill. */
    union {
        struct {
            u8 unk_1A[2];
            datHeartsSkill_t heartsSkill[8];
        };
        struct {
            struct {
                u8 condition;
                u8 reserved;
                u16 skill;
            } heartConditions[8];
            u8 unk_3A[2];
        };
        struct {
            u8 reserved1A[0x20];
            u8 probabilities[8];
        };
    };
} datRaceEntry_t;

extern void rstSetMaxHpMp(s8 Mode, datUnitWork_t *pStock);


typedef struct
{
    u8 head[6];
    fclSkillParam_t entry[24];
} datLevelUp_t;



extern rstData_t *EventStart;

extern datRaceEntry_t D_00339E00[];

extern s8 D_00339E06[];

extern s8 D_00339E0C[];





extern s16 datGetDevilParam(u8 devilId);

extern s32 cmpStatSetTimer(s32 kind, s8 index);

extern void rstSetDestroySkillMaxHpMp(void);

extern void p2sdPlaySE(s32 index, s32 volume, s32 pan);





extern rstData_t *rstDrawWork;

typedef struct
{
    /* 0x00 */ u16 id;
    /* 0x02 */ s8 comment;
    /* 0x03 */ u8 unk_03;
} datCommentEntry_t;

extern datCommentEntry_t D_00339DF8[];

extern char D_0053DA90[];

extern char D_0053DAA0[];

extern s8 cmbCheckEvoUnlocked(u16 devilId, datUnitWork_t *list, u8 count);

extern s32 cmbCopyEvoDevil(datUnitWork_t *unit, u16 skill);

extern void *dds3SearchSceneBasic(void *pBasic, u32 serialID, s32 type);

extern evtUnit_t *evtGetUnitHandle2(void *unit);

/* Called above its definition, where ee-gcc assumed int. */
void *dds3GetActiveScene(void);

extern void *dds3GetActiveScene(void);

extern s32 cmpModelSetup(s32 a, s32 b);

s32 fclStartMessage(s32 MsgNo);

extern void (*DrawPhaseTable[])(fclEncyc_instance_tag *work);

extern s32 cmbGetLevelUpCount(datUnitWork_t *unit, u32 exp);

void func_0021FE30(s8 a, s8 b, f32 x, f32 y);

void rstAdvanceLevelUpStep(void);

typedef struct
{
    /* 0x00 */ s8 unk_00;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ u8 unk_02;
} rstMotionReq_t;

extern rstMotionReq_t D_00332458[];

void rstSetMotionReqFromTbl(s8 Type)
{
    s32 i = EventStart->unit->id * 7 + Type;

    func_0021FE30(D_00332458[i].unk_00, 0, (f32)D_00332458[i].unk_01,
                  (f32)D_00332458[i].unk_02);
}

extern s8 cmbChkSkillOwner(u16 skillId, datUnitWork_t *unit);

s32 rstChkAddSkill(u16 SkillID)
{
    if (cmbChkSkillOwner(SkillID, EventStart->unit) >= 0) {
        return 0;
    }
    return (EventStart->unit->skillcnt >= 8) ? 2 : 1;
}

s16 rstGetEquipHeartsAisyo(void)
{
    return datGetDevilParam(DDS3_GBWK->equipHearts);
}

extern u32 datGetMaxHp(datUnitWork_t *work);
extern u32 datGetMaxMp(datUnitWork_t *work);
extern s32 printf(const char *format, ...);
extern char D_0053DAB0[];

void rstSetDestroySkillMaxHpMp(void)
{
    datUnitWork_t *unit = EventStart->unit;
    datUnitWork_t work;
    u16 skill = unit->id == 0 ? EventStart->heartsSkill :
                               EventStart->eventValue;
    s32 hpChange;
    s32 mpChange;

    work = *unit;
    work.skillcnt = 0;
    hpChange = datGetMaxHp(&work);
    mpChange = datGetMaxMp(&work);
    work.skillcnt = 1;
    work.skill[0] = skill;
    hpChange = datGetMaxHp(&work) - hpChange;
    mpChange = datGetMaxMp(&work) - mpChange;
    unit->maxhp += hpChange;
    unit->maxmp += mpChange;
    printf(D_0053DAB0, skill, hpChange, mpChange);
}

extern void cmbCopyCustomDevilToStock(datUnitWork_t *dst, datUnitWork_t *src);
extern u32 datGetMaxHp(datUnitWork_t *unit);
extern u32 datGetMaxMp(datUnitWork_t *unit);
void rstSetMaxHpMp(s8 mode, datUnitWork_t *unit)
{
    datUnitWork_t copy;
    s32 i;
    cmbCopyCustomDevilToStock(&copy, unit);
    for (i = 0; i < 6; i++) {
        copy.param[i] += EventStart->bonusParam[i];
    }
    if (mode == 0 && EventStart->RecoverFlag != 0) {
        unit->maxhp = unit->hp = datGetMaxHp(&copy);
        unit->maxmp = unit->mp = datGetMaxMp(&copy);
    } else {
        unit->maxhp = datGetMaxHp(&copy);
        unit->maxmp = datGetMaxMp(&copy);
    }
    if (unit->hp > unit->maxhp) unit->hp = unit->maxhp;
    if (unit->mp > unit->maxmp) unit->mp = unit->maxmp;
}

extern s32 cmbGetParamBase(datUnitWork_t *unit, s32 index);

s32 rstChkParamLimitAll(datUnitWork_t *unit)
{
    s32 i;

    for (i = 0; i < 6; i++)
    {
        if (i == 1)
        {
            continue;
        }
        if (cmbGetParamBase(unit, i) < 0x28)
        {
            return 0;
        }
    }
    rstSetMaxHpMp(0, unit);
    return 1;
}

extern void cmbCopyDefaultDevilToStock(u16 devilId, datUnitWork_t *dst);
extern void rstAddExp(s32 count, datUnitWork_t *unit);
s32 rstSetLevelUpCount(datUnitWork_t *unit)
{
    datUnitWork_t copy;
    s32 offset = 0;
    EventStart->StartLevel = unit->level;
    EventStart->signedLevelUpCnt = cmbGetLevelUpCount(unit, 0);
    EventStart->unk_18 = EventStart->unk_19 = EventStart->signedLevelUpCnt;
    if (unit->flag & 0x200) {
        cmbCopyDefaultDevilToStock(unit->id, &copy);
        offset = unit->level - copy.level;
    }
    rstAddExp(EventStart->signedLevelUpCnt, unit);
    if ((unit->badstatus & 0x800) == 0 && EventStart->signedLevelUpCnt != 0) {
        EventStart->signedLevelUpCnt += offset;
        EventStart->StartLevel -= offset;
        rstSetMaxHpMp(0, unit);
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "result/rstcalc", rstSetCurrentDevil);

extern void cmbCopyCustomDevilToStock(datUnitWork_t *dst, datUnitWork_t *src);
extern void cmbCopyDefaultDevilToStock(u16 devilId, datUnitWork_t *dst);

void rstInitWorkStock(s8 IkenieChk)
{
    datUnitWork_t defaultWork;

    EventStart->EventNums = 0;
    cmbCopyCustomDevilToStock(&EventStart->workStock, EventStart->unit);
    EventStart->workStock.level = EventStart->StartLevel + 1;
    if (IkenieChk != 0 && (EventStart->unit->flag & 0x200) != 0) {
        cmbCopyDefaultDevilToStock(EventStart->unit->id, &defaultWork);
        EventStart->workStock.level = defaultWork.level;
    }
}

extern s32 datGetDevilSlotState(u8 devilId);
extern u8 datLevelUpTable[];

#ifdef NON_MATCHING
void rstCreateBeforeSkillList(s8 heartsId, datUnitWork_t *stock, s8 levelOffset,
                              rstSkillInfo_t *info)
{
    s32 i;
    s8 level;
    u16 skill;

    if (stock == NULL) {
        return;
    }
    info->SkillCnt = 0;
    if (stock->id == 0) {
        if (heartsId == 0) {
            return;
        }
        for (i = datGetDevilSlotState((u8)heartsId); i < 8; i++) {
            datRaceEntry_t *race = &D_00339E00[heartsId];

            skill = race->heartConditions[i].skill;
            level = (s8)race->heartConditions[i].condition;
            if (skill != 0 && cmbChkSkillOwner(skill, stock) < 0) {
                info->TargetLevel[info->SkillCnt] = level;
                info->skill[info->SkillCnt++] = skill;
            }
        }
    } else {
        datLevelUp_t *table = (datLevelUp_t *)datLevelUpTable;
        fclSkillParam_t *entry = table[stock->id].entry;

        for (i = 0; i < 24; i++) {
            s32 kind = stock->hensinmae == 0 ? 1 : 6;

            if (entry[i].kind == kind) {
                level = (s8)entry[i].level;
                if (stock->level + levelOffset < level) {
                    skill = entry[i].value;
                    if (cmbChkSkillOwner(skill, stock) < 0) {
                        info->TargetLevel[info->SkillCnt] = level;
                        info->skill[info->SkillCnt++] = skill;
                    }
                }
            }
        }
    }
}
#else
INCLUDE_ASM(const s32, "result/rstcalc", rstCreateBeforeSkillList);
#endif

extern void rstCreateBeforeSkillList(s8 HeartsID, datUnitWork_t *pStock, s8 SkillLevOfs,
                                     rstSkillInfo_t *pInfo);

s32 rstChkBeforeSkillOwner(u16 SkillID, s8 LevelOfs, datUnitWork_t *pStock)
{
    rstSkillInfo_t list;
    s32 i;

    rstCreateBeforeSkillList(*(s8 *)&DDS3_GBWK->heartsequip, pStock, LevelOfs, &list);
    for (i = 0; i < list.SkillCnt; i++) {
        if (SkillID == list.skill[i]) {
            return 1;
        }
    }
    return 0;
}

extern void rstChkResult2Process(u8 *work);
extern s32 datGetParam(datUnitWork_t *work, s32 paratype);

void rstAutoAsignDevilParam(void)
{
    datUnitWork_t work;
    s32 i;
    s32 param;
    s32 value;
    u8 *bonus;

    rstChkResult2Process((u8 *)EventStart);
    i = 0;
    cmbCopyCustomDevilToStock(&work, EventStart->unit);
    if (EventStart->unk_18 > 0) {
        do {
            param = cmbAddLevelUpParamEx(&work, 0);
            if (param >= 0) {
                value = datGetParam(&work, param);
                /* Integer address form: docs/levers/addressing-and-types.md. */
                bonus = (u8 *)((u32)param + (u32)EventStart + 0x129F);
                if (value + (s8)*bonus < 0x28) {
                    (*bonus)++;
                }
            }
            i++;
        } while (i < EventStart->unk_18);
    }
}

extern fclSkillParam_t *datFindKugatatiEntry(datUnitWork_t *unit, s8 *count);

s32 rstCalcEventInfo(u8 *eventInfo)
{
    do {
        if (EventStart->unit->level < EventStart->workStock.level) {
            break;
        }
        if (EventStart->EventNums == 0) {
            EventStart->eventIndex = 0;
            EventStart->eventEntries = datFindKugatatiEntry(&EventStart->workStock,
                                                           &EventStart->EventNums);
        }
        if (EventStart->eventEntries == NULL ||
            EventStart->EventNums == EventStart->eventIndex) {
            EventStart->EventNums = 0;
            if (EventStart->workStock.level >= 99) {
                break;
            }
            rstAddExp(1, &EventStart->workStock);
        } else {
            if (eventInfo != NULL) {
                *(u16 *)eventInfo = EventStart->eventEntries[EventStart->eventIndex].value;
            }
            return (s8)EventStart->eventEntries[EventStart->eventIndex++].kind;
        }
    } while (EventStart->EventNums == 0);
    return -1;
}

extern s32 rstSetLevelUpCount(datUnitWork_t *pStock);

extern s32 cmpModelSetup(s32 a, s32 b);

s32 rstCalcSeqPlayerLevelUp(void)
{
    datUnitWork_t *unit = (datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54);

    EventStart->unit = unit;
    if (rstSetLevelUpCount(unit) == 0) {
        return 0;
    }
    rstAdvanceLevelUpStep();
    cmpModelSetup(0, 0);
    if (rstChkParamLimitAll(EventStart->unit) != 0) {
        rstData_t *start = EventStart;

        start->unk_19 = 0;
        start->unk_18 = 0;
        return 2;
    }
    fclStartMessage(0);
    return 1;
}

extern s32 rstSetCurrentDevil(void);
extern void rstAutoAsignDevilParam(void);
extern void rstSetLevelUpFlag(void);
extern s32 p2sdStopSe(s32 id);
extern void p2sdPlayBGM(s32 id);
extern void dds3FadeOut(s32, s32, s32, s32);
extern void fclSetMessageVar_20743088(s8 index, s32 str);
extern char *text2[];

s32 rstCalcSeqDevilLevelUp(void)
{
    if (rstSetCurrentDevil() != 0) {
        cmpStatSetTimer(0, 0);
        EventStart->unk_129D[1] = 0;
        rstAutoAsignDevilParam();
        rstAdvanceLevelUpStep();
        cmpModelSetup(EventStart->unit->id, 0);
        rstSetMotionReqFromTbl(0);
        p2sdStopSe(0x2B0000);
        p2sdPlayBGM(0x2B0000);
        fclSetMessageVar_20743088(1, (s32)text2[EventStart->unit->id]);
    } else {
        EventStart->phase = 0x13;
        rstSetLevelUpFlag();
        dds3FadeOut(0, 0, 0, 15);
    }
    return 0;
}

extern s32 datFindCommentEntry(u16 devilId);
extern s32 cmbChkGetHearts(s32 rawId);
extern s32 rstRollComment(void);

s32 rstPickRandomComment(void)
{
    s32 id;

    if (dds3GetRandA(NULL) % 3 == 0) {
        id = datFindCommentEntry(EventStart->unit->id);
        if (id == 0 || cmbChkGetHearts(id) != 0) {
            EventStart->commentFromUnit = 0;
            EventStart->commentId = rstRollComment();
        } else {
            EventStart->commentFromUnit = 1;
            EventStart->commentId = id;
        }
        EventStart->commentMessage = master_text[EventStart->unit->id].kucyo * 4;
        EventStart->commentMessage += dds3GetRandA(NULL) & 3;
        EventStart->commentMessage += 0x2C;
        return 1;
    }
    EventStart->commentId = 0;
    return 0;
}

extern s32 rstCalcEventInfo(u8 *eventInfo);
s32 rstGetDefaultSkill(void)
{
    s32 eventType;
    s32 canAdd;
    while ((eventType = rstCalcEventInfo((u8 *)&EventStart->eventValue)) >= 0) {
        if (eventType != 1 && eventType != 6) {
            continue;
        }
        if (eventType == 6 && EventStart->unit->hensinmae == 0) {
            continue;
        }
        if (eventType == 1 && EventStart->unit->hensinmae != 0) {
            continue;
        }
        canAdd = rstChkAddSkill(EventStart->eventValue);
        if (canAdd != 0) {
            return (s8)canAdd;
        }
        EventStart->signedLevelUpCnt--;
    }
    return 0;
}

s32 rstChkSkillPowerUp1(void)
{
    s32 ret;

    if (DDS3_GBWK->party[0].unit.level < 7)
    {
        return 0;
    }
    if (EventStart->unit->id == 0)
    {
        dds3GetRandA(0);
        ret = 1;
    }
    else
    {
        ret = (dds3GetRandA(0) & 3) == 0;
    }
    return ret;
}

#ifdef NON_MATCHING
s32 rstChkSkillPowerUp2(void)
{
    u32 roll;

    if (EventStart->unit->id == 0) {
        roll = dds3GetRandA(NULL) % 254;
        if (roll < 112) {
            return 0;
        }
        if (roll < 254) {
            return 1;
        }
        return 2;
    }
    roll = dds3GetRandA(NULL) % 254;
    if (roll < 128) {
        return 0;
    }
    if (roll < 254) {
        return 1;
    }
    return 2;
}
#else
INCLUDE_ASM(const s32, "result/rstcalc", rstChkSkillPowerUp2);
#endif

extern s32 rstFindPowerUpSkill(u16 skillId);

u16 rstRndGetPowerUpSkill(datUnitWork_t *unit, s8 *pIdx)
{
    u16 skillList[24];
    s8 indexList[24];
    s8 skillCount = 0;
    s32 i;

    for (i = 0; i < unit->skillcnt; i++) {
        u16 skill = unit->skill[i];
        s32 powerUpSkill;

        if (skill == 0) {
            continue;
        }
        powerUpSkill = rstFindPowerUpSkill(skill);
        if (powerUpSkill != 0) {
            skillList[skillCount] = powerUpSkill;
            indexList[skillCount] = i;
            skillCount++;
        }
    }
    if (skillCount == 0) {
        return 0;
    }
    {
        s8 index = dds3GetRandA(NULL) % skillCount;
        if (pIdx != NULL) {
            *pIdx = indexList[index];
        }
        return skillList[index];
    }
}

s32 rstChkDevilSkillPowerUp(datUnitWork_t *pStock)
{
    s32 flag = pStock->flag & 0x40;

    if (flag != 0) {
        return 1;
    }
    return 0;
}

s32 rstChkPartyDevilSkillPowerUp(void)
{
    s32 result = 0;
    s32 i;

    for (i = 0; i < 16; i++) {
        datUnitWork_t *unit = (datUnitWork_t *)&DDS3_GBWK->akuma[i].flag;

        if ((unit->flag & 1) == 0) {
            continue;
        }
        if ((unit->flag & 4) != 0) {
            continue;
        }
        if ((unit->flag & 2) != 0) {
            result = rstChkDevilSkillPowerUp(unit);
        }
    }
    return result;
}

INCLUDE_ASM(const s32, "result/rstcalc", rstCalcSkillPowerUpCore);

extern s32 rstChkSkillPowerUp1(void);

extern s32 rstCalcSkillPowerUpCore(void);

s32 rstCalcSkillPowerUp(void)
{
    s32 result = rstChkSkillPowerUp1();

    if (result != 0) {
        result = rstCalcSkillPowerUpCore();
    }
    return result;
}

extern s32 rstCalcEvoLevelDiff(datUnitWork_t *, datUnitWork_t *, datUnitWork_t *, s32);
extern s32 cmbChkEvoNeedFlag(u16 devilId);
extern u8 datLevelUpTable[];

s32 rstChkEvoEvent(datUnitWork_t *stock, datUnitWork_t *evo, s32 index)
{
    s32 diff;
    s32 result;
    s32 level;

    if (index >= 0) {
        /* Byte-table view, with the six-byte header and entry offset first;
         * see docs/levers/addressing-and-types.md. */
        level = datLevelUpTable[index * sizeof(fclSkillParam_t) +
                               stock->id * sizeof(datLevelUp_t) + 6];
    } else {
        level = -1;
    }
    diff = rstCalcEvoLevelDiff((datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54),
                               stock, evo, level);
    if (diff > 0)
        return diff < 3 ? 1 : 2;
    result = cmbChkEvoNeedFlag(evo->id) != 0 ? 3 : 0;
    return result;
}

INCLUDE_ASM(const s32, "result/rstcalc", rstCalcEvo);

extern s32 rstFindEvoEntryType7(datUnitWork_t *unit);

extern s32 cmbChkDevilEvo(datUnitWork_t *unit);

s32 rstCalcSeqEvoAt(void)
{
    s32 level;

    if (rstFindEvoEntryType7(EventStart->unit) >= 0)
    {
        level = DDS3_GBWK->party[0].unit.level + 6;
        if (level < cmbChkDevilEvo(EventStart->unit))
        {
            return -1;
        }
        return 0;
    }
    return -1;
}

s32 rstCalcSeqDestroySkill(void)
{
    cmpStatSetTimer(0, 2);
    rstSetDestroySkillMaxHpMp();
    return 1;
}

extern u16 rstCheckHeartsSlot(u8 id, datUnitWork_t *unit);

u16 rstChkHeartsSkill(datUnitWork_t *unit)
{
    datUnitWork_t copy;
    u16 skill;

    cmbCopyCustomDevilToStock(&copy, unit);
    skill = rstCheckHeartsSlot(DDS3_GBWK->heartsequip, &copy);
    if (skill == 0x165 && evtBitCheck(0x8C1) == 0) {
        return 0;
    }
    if (skill == 0) {
        return 0;
    }
    if (cmbChkSkillOwner(skill, unit) >= 0) {
        return 0;
    }
    return EventStart->HeartsSkFlag < EventStart->signedLevelUpCnt ? skill : 0;
}

extern s32 datGetDevilSlotState(u8 devilId);
extern u16 rstGetHeartsSkillAt(u8 race, s8 index);

#ifdef NON_MATCHING
void rstRecalcHeartsSkill(u16 skillId)
{
    s32 i;
    s32 j;
    s32 slot;
    s8 devilId;
    u16 skill;
    s32 missing;
    u8 count;
    dds3GlobalWork_t *work;

    for (i = 0; i < DDS3_GBWK->heartscnt; i++) {
        devilId = (s8)DDS3_GBWK->hearts[i];
        for (j = 0; j < 8; j++) {
            skill = rstGetHeartsSkillAt((u8)devilId, (s8)j);
            if (skill == skillId) {
                missing = 1;
                for (slot = 0; slot < datGetDevilSlotState((u8)devilId); slot++) {
                    if (DDS3_GBWK->heartsSkills[devilId][slot] == skill) {
                        missing = 0;
                    }
                }
                if (missing != 0) {
                    work = DDS3_GBWK;
                    count = work->devilSlot[devilId]++;
                    work->heartsSkills[devilId][count] = skill;
                }
            }
        }
    }
}
#else
INCLUDE_ASM(const s32, "result/rstcalc", rstRecalcHeartsSkill);
#endif

void rstLoadHeartsSkill(void) {
}

void rstStoreHeartsSkill(void) {
}

#ifdef NON_MATCHING
extern void rstSetSkillActFlag(u8 flag);
#endif

void rstInitSkillAct(s8 Pos)
{
    cmpStatSetTimer(0, 1);
    rstSetSkillActFlag(Pos);
    EventStart->unk_2308 = 0x1E;
    p2sdPlaySE(0x2B0003, 0x7F, 0x3F);
}

void func_002215B8(datUnitWork_t *unit) {
}

extern s32 datDevilIsRegistered(u8 devilId);
extern s8 datGetDevilSlotIndex(u8 devilId);
extern s32 datGetDevilSlotState(u8 devilId);

#ifdef NON_MATCHING
s32 rstFindRegisteredDevil(void)
{
    s8 id = DDS3_GBWK->equipHearts;
    s32 slotCount;
    s32 roll;
    s32 i;

    if (id == 0 || datDevilIsRegistered((u8)id) != 0) {
        return -1;
    }
    slotCount = datGetDevilSlotIndex((u8)id);
    if ((s16)datGetDevilSlotState((u8)id) >= slotCount) {
        return -1;
    }
    if (dds3GetRandA(NULL) % 3 >= 2) {
        return -1;
    }
    roll = dds3GetRandA(NULL) % 255;
    for (i = 0; i < 8; i++) {
        if (roll < D_00339E00[id].probabilities[i]) {
            return (s8)i;
        }
    }
    return -1;
}
#else
INCLUDE_ASM(const s32, "result/rstcalc", rstFindRegisteredDevil);
#endif

/* Called above its definition, where ee-gcc assumed int. */
s32 datGetDevilSlotState(u8 devilId);

/* Called above its definition, where ee-gcc assumed int. */
/* Called above its definition, where ee-gcc assumed int. */
s32 datDevilIsRegistered(u8 devilId);

s8 datGetDevilSlotIndex(u8 devilId);

s32 rstFindDevilSlot(void)
{
    s8 raw = *(s8 *)&DDS3_GBWK->heartsequip;
    u8 id;
    s32 index;
    s32 state;
    s32 ret;

    if (raw == 0) {
        return -1;
    }
    id = raw;
    if (datDevilIsRegistered(id) != 0) {
        return -1;
    }
    index = datGetDevilSlotIndex(id);
    state = (s16)datGetDevilSlotState(id);
    ret = -1;
    if (state >= index) {
        ret = 0;
    }
    return ret;
}

s32 rstShakePad(void)
{
    return dds3PadVibrationSet(1, 0xFF, 0x1E);
}

extern void *dds3GetProcessData(dds3ProcessID_t *id);
extern void rstDrawEvoModel(void);
extern s32 dds3FadeCheck(void);
extern s32 cmpLoadStatusModel(void);
extern void cmpUpdateTerminate(void *work);
extern void rstInitEvoAct(void);
extern s32 rstFindRegisteredDevil(void);
extern s32 rstCalcSkillPowerUpCore(void);
extern s32 rstFindDevilSlot(void);
extern s32 rstCalcEvo(void);
extern char *pstr[];
extern char *text1[];
extern s32 fclStartSelMessage(s32 id);
extern void rstAddAsignParam(void);

INCLUDE_RODATA(const s32, "result/rstcalc", D_0053DAB0);

s32 rstCalc(dds3ProcessID_t *id)
{
    s32 process = 1;
    EventStart = dds3GetProcessData(id);
    if (EventStart->stepFlag == 1) {
        switch (EventStart->phase) {
        case 0:
            cmpStatSetTimer(0, 0);
            EventStart->HeartsSkFlag = 0;
            EventStart->unk_129D[1] = 0;
            if (rstCalcSeqPlayerLevelUp() != 0)
                break;
            process = 0;
            EventStart->phase = (u8)EventStart->phase + 1;
        case 1:
            if (process) {
                EventStart->heartsSkill = rstChkHeartsSkill(&DDS3_GBWK->party[0].unit);
                if (EventStart->heartsSkill != 0) {
                    EventStart->heartsResult = rstChkAddSkill(EventStart->heartsSkill);
                    rstInitSkillAct(8);
                    break;
                }
            }
            EventStart->phase = (u8)EventStart->phase + 1;
        case 2:
            if (process) {
                EventStart->heartsEvent = rstFindRegisteredDevil();
                if (EventStart->heartsEvent >= 0) {
                    EventStart->powerState = rstCalcSkillPowerUpCore();
                    if (EventStart->heartsEvent == 1) {
                        if (EventStart->powerState >= 3) {
                            EventStart->heartsEvent = 0;
                            EventStart->powerState = -1;
                        } else if (EventStart->powerState <= 0) {
                            EventStart->heartsEvent = 0;
                            EventStart->powerState = -1;
                        }
                    }
                    EventStart->unk_129D[1] = 0;
                    rstShakePad();
                    fclSetMessageVar_20743088(12, (s32)pstr[DDS3_GBWK->equipHearts]);
                    fclStartMessage(28);
                    fclStartSelMessage(43);
                }
            }
            EventStart->phase = (u8)EventStart->phase + 1;
        case 3:
            if (process && EventStart->heartsEvent >= 0)
                break;
            EventStart->phase = (u8)EventStart->phase + 1;
        case 4:
            if (process)
                EventStart->heartsEvent = rstFindDevilSlot();
            EventStart->phase = (u8)EventStart->phase + 1;
        case 5:
            if (process && EventStart->heartsEvent >= 0) {
                EventStart->unk_129D[1] = 0;
                break;
            }
            EventStart->phase = (u8)EventStart->phase + 1;
            goto next_devil;
        case 7:
            rstInitWorkStock(1);
            EventStart->unit->flag &= ~0x200;
            EventStart->phase = (u8)EventStart->phase + 1;
        case 8:
            EventStart->defaultSkillResult = rstGetDefaultSkill();
            if (EventStart->defaultSkillResult != 0) {
                rstInitSkillAct(8);
                break;
            }
            EventStart->phase = (u8)EventStart->phase + 1;
        case 9:
            rstInitWorkStock(0);
            EventStart->powerState = rstCalcSkillPowerUp();
            if (EventStart->powerState != 0) {
                EventStart->unk_129D[1] = 0;
                fclSetMessageVar_20743088(15, (s32)text1[EventStart->unit->skill[EventStart->powerSlot]]);
                fclSetMessageVar_20743088(16, (s32)text1[EventStart->powerSkill]);
                fclStartMessage(13);
                fclStartSelMessage(43);
            }
            EventStart->phase = (u8)EventStart->phase + 1;
        case 10:
            if (EventStart->powerState != 0)
                break;
            EventStart->phase = (u8)EventStart->phase + 1;
        case 11:
            if (rstPickRandomComment() != 0) {
                fclSetMessageVar_20743088(1, (s32)text2[EventStart->unit->id]);
                fclStartMessage(24);
                break;
            }
            EventStart->phase = (u8)EventStart->phase + 1;
        case 12:
            if (EventStart->commentId != 0) {
                EventStart->unk_129D[1] = 0;
                fclStartMessage(25);
                fclStartSelMessage(43);
            }
            EventStart->phase = (u8)EventStart->phase + 1;
        case 13:
            if (EventStart->commentId != 0)
                break;
            EventStart->phase = (u8)EventStart->phase + 1;
        case 14:
            {
                rstData_t *work = EventStart;
                if (work->powerState == 0 && work->commentId == 0)
                    func_002215B8(work->unit);
                work->phase = (u8)work->phase + 1;
            }
        case 15:
            EventStart->phase = (u8)EventStart->phase + 1;
        case 16:
            rstInitWorkStock(0);
            if (rstCalcEvo() == 0) {
                fclSetMessageVar_20743088(1, (s32)text2[EventStart->unit->id]);
                if (rstCalcSeqEvoAt() >= 0) {
                    fclStartMessage(18);
                    EventStart->drawFlag = 1;
                    EventStart->phase = 18;
                    EventStart->unk_129D[1] = 0;
                    rstInitEvoAct();
                    break;
                }
            }
            EventStart->phase = (u8)EventStart->phase + 1;
        case 17:
            if (EventStart->evoResult == 0) {
                rstInitEvoAct();
                EventStart->unk_129D[1] = 0;
                break;
            }
            rstAddAsignParam();
            EventStart->phase = 6;
        case 6:
next_devil:
            if (rstCalcSeqDevilLevelUp() != 0)
                return 0;
            break;
        case 20:
            EventStart->evoPhase = 0;
            break;
        case 21:
            EventStart->unk_129D[1] = 0;
            rstCalcSeqDestroySkill();
            break;
        case 22:
        case 24:
            EventStart->unk_129D[1] = 0;
            break;
        }
    }
    rstDrawEvoModel();
    EventStart->stepFlag = 0;
    if (dds3FadeCheck() != 0)
        return 0;
    if ((EventStart->flags & 0x10) != 0) {
        if (cmpLoadStatusModel() != 0)
            return 0;
        cmpUpdateTerminate(EventStart);
        return -1;
    }
    return 0;
}


extern s32 cmbGetHeartsParam(s8 devilId, s8 index);

s32 cmbGetParamBase(datUnitWork_t *unit, s32 index)
{
    s32 value;

    if (unit->id == 0) {
        value = unit->param[index];
        if (DDS3_GBWK->equipHearts != 0) {
            value -= cmbGetHeartsParam(DDS3_GBWK->equipHearts, index);
        }
    } else {
        value = datGetParam(unit, index);
    }
    return value;
}

extern s32 datGetParam(datUnitWork_t *work, s32 paratype);

s32 cmbGetParam(datUnitWork_t *unit, s32 index)
{
    s32 value;

    if (unit->id == 0) {
        value = unit->param[index];
    } else {
        value = datGetParam(unit, index);
    }
    return value;
}

s8 func_00221DB0(s8 index)
{
    return D_00339E00[index].unk_19;
}

s32 cmbChkGetHearts(s32 rawId)
{
    s8 id = rawId;
    s32 i;

    for (i = 0; i < ((u8 *)DDS3_GBWK)[0x1D7B]; i++) {
        if (DDS3_GBWK->unk_01CFB[i] == id) {
            return 1;
        }
    }
    return 0;
}

s32 datFindCommentEntry(u16 devilId)
{
    datCommentEntry_t *entry = D_00339DF8;
    u32 i;

    for (i = 0; i < 1; i++, entry++) {
        if (entry->id == devilId) {
            return entry->comment;
        }
    }
    return 0;
}

/* Callers supply a unit; the classification currently uses global state. */
#ifdef NON_MATCHING
extern s32 datGetPlayerLNDList(u8 *list);
#endif

s32 datGetPlayerLNDLevel(datUnitWork_t *unit)
{
    u8 list;
    s32 type = datGetPlayerLNDList(&list);
    u8 result;

    if (list == 0) {
        return 0;
    }
    list--;
    if (type == 1) {
        result = (list * 3) + 1;
    } else if (type == 4) {
        result = (list * 3) + 3;
    } else {
        result = (list * 3) + 2;
    }
    return result;
}

s32 datGetDevilRace(s8 index)
{
    return D_00339E00[index].race & 7;
}

extern void *memset(void *dst, s32 value, u32 size);
extern s8 datCountPlayerLND(void);

#ifdef NON_MATCHING
s32 datGetPlayerLNDList(u8 *outCount)
{
    s8 counts[3];
    s32 result = 0;
    s32 count;
    s32 i;

    memset(counts, 0, sizeof(counts));
    for (i = 0; i < DDS3_GBWK->heartscnt; i++) {
        s8 id = DDS3_GBWK->hearts[i];

        if (datDevilIsRegistered((u8)id) != 0) {
            switch (datGetDevilRace(id)) {
            case 1:
                counts[0]++;
                break;
            case 0:
                counts[1]++;
                break;
            case 4:
                counts[2]++;
                break;
            }
        }
    }
    if (counts[0] != counts[2] &&
        (counts[0] >= counts[1] || counts[2] >= counts[1])) {
        result = counts[0] < counts[2] ? 4 : 1;
    }
    count = datCountPlayerLND();
    if (count < 6) {
        count /= 2;
    } else if (count == 25) {
        count = 5;
    } else if (count < 12) {
        count = 3;
    } else {
        count = 4;
    }
    if (outCount != NULL) {
        *outCount = count;
    }
    return result;
}
#else
INCLUDE_ASM(const s32, "result/rstcalc", datGetPlayerLNDList);
#endif

s8 datGetDevilSlotIndex(u8 devilId)
{
    s8 count = 0;
    s32 i;

    if (devilId == 0) {
        return 0;
    }
    for (i = 0; i < 8; i++) {
        if (D_00339E00[devilId].heartsSkill[i].skill != 0) {
            count++;
        }
    }
    return count;
}

void func_00222120(u8 devilId)
{
    DDS3_GBWK->devilSlot[devilId] &= 0xF0;
}

s32 datDevilIsRegistered(u8 devilId)
{
    u8 flag = DDS3_GBWK->devilSlot[devilId] & 0x80;

    return flag != 0;
}

s8 datCountPlayerLND(void)
{
    s32 count = 0;
    s32 i = 0;

    if (*(u8 *)((s32)DDS3_GBWK + 0x1D7B) != 0)
    {
        do
        {
            if (datDevilIsRegistered(*(u8 *)(i + (s32)DDS3_GBWK + 0x1CFB)) != 0)
            {
                count++;
            }
            i++;
        } while (i < *(u8 *)((s32)DDS3_GBWK + 0x1D7B));
    }
    return count;
}

s32 datGetDevilSlotState(u8 devilId)
{
    return DDS3_GBWK->devilSlot[devilId] & 0xF;
}

void rstSetDevilRegistered(s8 devilId)
{
    DDS3_GBWK->devilSlot[devilId] |= 0x80;
}

u16 rstGetHeartsSkillAt(u8 race, s8 index)
{
    if (index >= 8) {
        return 0;
    }
    return D_00339E00[race].heartsSkill[index].skill;
}

extern s8 D_00339E1A[];

/* The row stride is not a multiple of the entry stride, so the index is in bytes. */
s8 func_00222250(u8 index, s8 slot)
{
    if (slot >= 8) {
        return -1;
    }
    return D_00339E1A[slot * 4 + index * 0x42];
}

s16 datGetDevilParam(u8 devilId)
{
    if (devilId == 0) {
        return 0;
    }
    return D_00339E00[devilId].param;
}

/* NON_MATCHING: 8 of 22 words differ (first at +0x28). */
#ifdef NON_MATCHING
s8 cmbGetHeartsParamByIndex(s8 race, s8 index, s8 extra)
{
    if (extra != 0) {
        return D_00339E0C[race * 66 + index];
    }
    return D_00339E06[race * 66 + index];
}
#else
INCLUDE_ASM(const s32, "result/rstcalc", cmbGetHeartsParamByIndex);
#endif

extern s8 cmbGetHeartsParamByIndex(s8 race, s8 index, s8 extra);

#ifdef NON_MATCHING
s32 cmbGetHeartsParamEx(s8 devilId, s8 index, s32 registered)
{
    s32 value = cmbGetHeartsParamByIndex(devilId, index, (s8)registered != 0);

    return (s8)(value + DDS3_GBWK->hearts_up_param[devilId][index]);
}
#else
INCLUDE_ASM(const s32, "result/rstcalc", cmbGetHeartsParamEx);
#endif

extern s32 cmbGetHeartsParamEx(s8 devilId, s8 index, s32 registered);

s32 cmbGetHeartsParam(s8 devilId, s8 index)
{
    s32 param;

    if (datDevilIsRegistered(devilId) != 0)
    {
        param = cmbGetHeartsParamEx(devilId, index, 1);
    }
    else
    {
        param = cmbGetHeartsParamEx(devilId, index, 0);
    }
    return param;
}

void cmbAddHeartsParam(s32 devilId, datUnitWork_t *unit)
{
    s8 *param = unit->param;
    s32 i;

    for (i = 0; i < 6; i++) {
        *param += cmbGetHeartsParam(devilId, i);
        param++;
    }
}

void cmbSubHeartsParam(s32 devilId, datUnitWork_t *unit)
{
    s8 *param = unit->param;
    s32 i;

    for (i = 0; i < 6; i++) {
        *param -= cmbGetHeartsParam(devilId, i);
        param++;
    }
}

void datAddHeartsSlot(s8 id, u16 value)
{
    s32 state = datGetDevilSlotState(id);
    u8 *gbwk = (u8 *)DDS3_GBWK;

    *(u16 *)(gbwk + (id * 12 + state) * 2 + 0x11AC0) = value;
    (*(u8 *)(id + (s32)gbwk + 0x1D3B))++;
}

s32 datAddHeartsEntry(u8 id, datUnitWork_t *work)
{
    s32 result = 0;
    s32 i;

    if (id > 0) {
        for (i = 0; i < 8; i++) {
            datRaceEntry_t *race = &D_00339E00[id];
            u16 skill = race->heartConditions[i].skill;

            if (skill != 0 && race->heartConditions[i].condition == 0) {
                work->skill[work->skillcnt++] = skill;
                datAddHeartsSlot((s8)id, skill);
            }
        }
        result = (s8)work->skillcnt;
    }
    return result;
}

#ifdef NON_MATCHING
s32 func_00222610(u8 id, u8 limit, datUnitWork_t *unit)
{
    datRaceEntry_t *race = &D_00339E00[id];
    u8 count = 0;
    s32 i;

    for (i = 0; i < 8; i++) {
        u8 condition = race->heartConditions[i].condition;
        u16 skill = race->heartConditions[i].skill;

        if (condition != 0 && skill != 0) {
            if (count++ >= limit) {
                return 0;
            }
            if (unit->level >= race->heartConditions[i].condition) {
                unit->skill[unit->skillcnt++] = skill;
            }
        }
    }
    return (s8)count;
}
#else
INCLUDE_ASM(const s32, "result/rstcalc", func_00222610);
#endif

/* Interior row label: the 66-byte stride is not a multiple of the
 * four-byte skill-entry stride. */
extern u8 D_00339E10[];

u16 rstCheckHeartsSlot(u8 id, datUnitWork_t *unit)
{
    s32 index = datGetDevilSlotState(id);
    u8 *entry;
    u16 skill;

    if (index < 8) {
        entry = D_00339E10 + index * 4 + id * sizeof(datRaceEntry_t);
        do {
            skill = *(u16 *)(entry + 12);
            if (skill != 0 && *(s8 *)(entry + 10) != 0 &&
                unit->level >= *(s8 *)(entry + 10)) {
                if (cmbChkSkillOwner(skill, unit) < 0)
                    return skill;
            }
            index++;
            entry += 4;
        } while (index < 8);
    }
    return 0;
}

extern void cmbClearUnitSkills(datUnitWork_t *unit);
extern u8 datLevelUpTable[];

#ifdef NON_MATCHING
void cmbInitStockUnit(u16 id, datUnitWork_t *unit)
{
    datLevelUp_t *row;
    s32 i;

    cmbClearUnitSkills(unit);
    row = &((datLevelUp_t *)datLevelUpTable)[id];
    for (i = 0; i < 24; i++) {
        if (row->entry[i].kind == 1 && row->entry[i].level == 0) {
            unit->skill[unit->skillcnt++] = row->entry[i].value;
        }
    }
}
#else
INCLUDE_ASM(const s32, "result/rstcalc", cmbInitStockUnit);
#endif

extern void cmbClearUnitSkills(datUnitWork_t *unit);
extern u8 datLevelUpTable[];

#ifdef NON_MATCHING
void cmb_222828(u16 devilId, u8 level, datUnitWork_t *unit)
{
    datLevelUp_t *table = (datLevelUp_t *)datLevelUpTable;
    fclSkillParam_t *entry;
    s32 i;

    cmbClearUnitSkills(unit);
    entry = table[devilId].entry;
    for (i = 0; i < 24; i++) {
        if (entry[i].kind == 1 && entry[i].level <= level) {
            unit->skill[unit->skillcnt++] = entry[i].value;
        }
    }
}
#else
INCLUDE_ASM(const s32, "result/rstcalc", cmb_222828);
#endif

s8 cmbCheckEvoUnlocked(u16 devilId, datUnitWork_t *list, u8 count)
{
    s32 i = 0;

    if (count != 0) {
        do {
            if ((list->flag & 1) != 0) {
                if (list->id == devilId) {
                    return i;
                }
            }
            i++;
            list++;
        } while (i < count);
    }
    return -1;
}

s8 cmbCountEvoCandidates(datUnitWork_t *list, u8 count)
{
    u8 n = 0;
    s32 i = count;

    while (i != 0) {
        u32 flag = list->flag;

        if ((flag & 1) != 0) {
            if ((flag & 4) == 0) {
                n++;
            }
        }
        list++;
        i--;
    }
    return n;
}

typedef struct
{
    /* 0x0 */ u16 devilId;
    /* 0x2 */ u16 flag;
} cmbEvoFlagEntry_t;

extern cmbEvoFlagEntry_t D_00339640[];

s32 cmbChkEvoNeedFlag(u16 devilId)
{
    cmbEvoFlagEntry_t *entry = D_00339640;
    u32 i;

    for (i = 0; i < 6; i++, entry++) {
        if ((devilId == entry->devilId) && (evtBitCheck(entry->flag) == 0)) {
            return 1;
        }
    }
    return 0;
}

extern u16 D_00339638[];

s32 func_002229E8(u16 value)
{
    u16 *entry = D_00339638;
    u32 i = 0;
    u16 match;

    /* Retail retains the unsigned one-entry loop and four-byte row stride. */
    do {
        match = entry[0];
        entry += 2;
        if (match == value) {
            return 1;
        }
        i++;
    } while (i < 1);
    return 0;
}

typedef struct
{
    /* 0x00 */ u8 unk_00[2];
    /* 0x02 */ s8 value;
    /* 0x03 */ u8 kind;
} cmpKugatatiEntry_t;

extern u8 datLevelUpTable[];

/* NON_MATCHING: the native lookup retains a table offset and a separate
 * address for the first matching entry. */
#ifdef NON_MATCHING
fclSkillParam_t *datFindKugatatiEntry(datUnitWork_t *unit, s8 *outcount)
{
    datLevelUp_t *table = (datLevelUp_t *)datLevelUpTable;
    fclSkillParam_t *entry = table[unit->id].entry;
    u8 level = unit->level;
    fclSkillParam_t *found = NULL;
    u8 count = 0;
    s32 i;

    for (i = 23; i >= 0; i--) {
        if (entry->level == level) {
            count++;
            if (count == 1) {
                found = entry;
            }
        }
        entry++;
    }
    if (outcount != NULL) {
        *outcount = count;
    }
    return found;
}
#else
INCLUDE_ASM(const s32, "result/rstcalc", datFindKugatatiEntry);
#endif

/* Header row contains sixteen source keys; each following row starts with its target key. */
extern s16 cmbEvoStatTable[][17];

u8 cmbCopyEvoStats(s16 first, s16 second)
{
    u8 column;
    u8 row;
    for (column = 0; column < 16; column++) {
        if (cmbEvoStatTable[0][column + 1] == first) {
            break;
        }
    }
    for (row = 0; row < 13; row++) {
        if (cmbEvoStatTable[row + 1][0] == second) {
            break;
        }
    }
    return (u8)cmbEvoStatTable[row + 1][column + 1];
}

s32 cmbCheckEvoPair(u16 maskA, u16 maskB)
{
    s32 i;

    for (i = 0; i < 0x10; i++) {
        if ((maskA >> i) & 1) {
            return ((u32)maskB >> i) & 1;
        }
    }
    return 0;
}

typedef struct
{
    /* 0x00 */ u16 devilId;
    /* 0x02 */ u8 value;
    /* 0x03 */ u8 unk_03;
} cmbEvoEntry_t;

extern cmbEvoEntry_t D_00339658[];

s32 cmbFindEvoEntry(u16 devilId)
{
    cmbEvoEntry_t *entry = D_00339658;
    u32 i;

    for (i = 0; i < 0xE3; i++, entry++) {
        if (entry->devilId == devilId) {
            return entry->value;
        }
    }
    return 1;
}

extern datSkill_t attr[];

#ifdef NON_MATCHING
s32 cmbCopyEvoDevil(datUnitWork_t *unit, u16 skill)
{
    u16 skillForm = attr[skill].keisyoform;
    s8 skillAttr = attr[skill].skillattr;
    s16 devilType = datDevilTable[unit->id].keisyotype;
    u16 devilForm = datDevilTable[unit->id].keisyoform;
    u32 value = cmbCopyEvoStats(skillAttr, devilType);
    s32 result = cmbCheckEvoPair(skillForm, devilForm);

    if (result != 0) {
        u8 quotient = value / (u32)cmbFindEvoEntry(skill);
        s32 level = quotient + unit->level - datDevilTable[unit->id].level + 1;

        if (level >= 100) {
            level = 99;
        }
        result = (u8)level;
    }
    return result;
}
#else
INCLUDE_ASM(const s32, "result/rstcalc", cmbCopyEvoDevil);
#endif

extern u16 D_00339BA8[];

s32 cmbEvoEntryExists(u16 devilId)
{
    u16 *entry = D_00339BA8;
    u32 i;

    for (i = 0; i < 0x25; i++, entry++) {
        if (devilId == *entry) {
            return 1;
        }
    }
    return 0;
}

/* NON_MATCHING: 25 of 34 words differ, 136 against retail's 132 bytes; the
 * narrowed count parameter copied into its own register in retail. */
#ifdef NON_MATCHING
s8 cmbCountEvoReady(datUnitWork_t *unit, u8 count)
{
    u8 ready = 0;

    while (count != 0) {
        if ((unit->flag & 1) != 0) {
            if ((unit->flag & 4) == 0) {
                if (cmbEvoEntryExists(unit->id) != 0) {
                    ready++;
                }
            }
        }
        unit++;
        count--;
    }
    return ready;
}
#else
INCLUDE_ASM(const s32, "result/rstcalc", cmbCountEvoReady);
#endif

typedef struct
{
    /* 0x00 */ u16 devilId;
    /* 0x02 */ u16 unk_02;
    /* 0x04 */ u16 value;
} cmbPairResult_t;

extern cmbPairResult_t D_00339BF8[];

s32 func_00222DC0(u16 devilId)
{
    cmbPairResult_t *entry = D_00339BF8;
    u32 i;

    for (i = 0; i < 6; i++, entry++) {
        if (entry->devilId == devilId) {
            return entry->value;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "result/rstcalc", cmbAddLevelUpParamEx);

s8 rst_223080(datUnitWork_t *unit)
{
    return cmbAddLevelUpParamEx(unit, 0);
}

/* NON_MATCHING: 3 of 27 words differ (first at +0x10). */
#ifdef NON_MATCHING
s32 rstFindEvoEntryType7(datUnitWork_t *unit)
{
    cmpKugatatiEntry_t *entry = (cmpKugatatiEntry_t *)&datLevelUpTable[unit->id * 0x66];
    u8 level = unit->level;
    s32 i;

    for (i = 0; i < 24; i++) {
        if (entry[1].kind == 7 && (u8)entry[1].value <= level) {
            return (s8)i;
        }
        entry++;
    }
    return -1;
}
#else
INCLUDE_ASM(const s32, "result/rstcalc", rstFindEvoEntryType7);
#endif

/* NON_MATCHING: 3 of 27 words differ (first at +0x10). */
#ifdef NON_MATCHING
s32 rstFindEvoEntryType5(datUnitWork_t *unit)
{
    cmpKugatatiEntry_t *entry = (cmpKugatatiEntry_t *)&datLevelUpTable[unit->id * 0x66];
    u8 level = unit->level;
    s32 i;

    for (i = 0; i < 24; i++) {
        if (entry[1].kind == 5 && (u8)entry[1].value < level) {
            return (s8)i;
        }
        entry++;
    }
    return -1;
}
#else
INCLUDE_ASM(const s32, "result/rstcalc", rstFindEvoEntryType5);
#endif

/* NON_MATCHING: 4 of 29 words differ (first at +0x10). */
#ifdef NON_MATCHING
s32 cmbChkDevilEvo(datUnitWork_t *unit)
{
    u8 *p = &datLevelUpTable[unit->id * 0x66 + 8];
    s32 i;

    for (i = 0; i < 24; i++) {
        if (p[-1] == 5) {
            datDevilTableEntry_t *info = &datDevilTable[*(u16 *)p];

            return (s8)info->level;
        }
        p += 4;
    }
    return -1;
}
#else
INCLUDE_ASM(const s32, "result/rstcalc", cmbChkDevilEvo);
#endif

#ifdef NON_MATCHING
s32 cmpFindKugatatiMsgEntry(datUnitWork_t *unit)
{
    datLevelUp_t *table = (datLevelUp_t *)datLevelUpTable;
    fclSkillParam_t *entry = table[unit->id].entry;
    s32 i;

    for (i = 0; i < 24; i++) {
        if (entry[i].kind == 5) {
            return (s8)entry[i].level;
        }
    }
    return -1;
}
#else
INCLUDE_ASM(const s32, "result/rstcalc", cmpFindKugatatiMsgEntry);
#endif

extern s8 cmbCountEvoReady(datUnitWork_t *unit, u8 count);

#ifdef NON_MATCHING
void cmbCalcEvoEvent(datUnitWork_t *unit, fclSkillParam_t *entry,
                     s8 hasNext, datUnitWork_t *result)
{
    u16 devilId;

    if (hasNext != 0 && entry[1].kind == 5) {
        s8 half = cmbCountEvoCandidates(unit, 16) / 2;

        if (half < cmbCountEvoReady(unit, 16)) {
            devilId = entry[0].value;
        } else {
            devilId = entry[1].value;
        }
    } else {
        devilId = entry[0].value;
    }
    if (result != NULL) {
        cmbCopyDefaultDevilToStock(devilId, result);
        if (result->level < unit->level) {
            s32 difference = unit->level - result->level;
            s32 i;

            result->level = unit->level;
            result->exp = datCopyDevilWorkEntry(result, result->level);
            for (i = 0; i < difference; i++) {
                s32 param = cmbAddLevelUpParamEx(result, 0);

                if (param >= 0 && datGetParam(result, param) < 40) {
                    result->param[param]++;
                }
            }
            result->flag |= 0x200;
        }
    }
}
#else
INCLUDE_ASM(const s32, "result/rstcalc", cmbCalcEvoEvent);
#endif

s32 rstCalcEvoLevelDiff(datUnitWork_t *a, datUnitWork_t *b, datUnitWork_t *c, s32 level)
{
    s32 limit = 0xFF;
    s32 diff;

    if (level >= 0) {
        limit = 0x63 - (b->level - level);
    }
    diff = (c->level - a->level) - 4;
    return (limit < diff) ? limit : diff;
}

extern void cmbClearUnitSkills(datUnitWork_t *unit);

void cmbCopyEvoDevilToStock(datUnitWork_t *unit, datUnitWork_t *source)
{
    datUnitWork_t work;
    s32 i;

    cmbCopyCustomDevilToStock(&work, source);
    cmbClearUnitSkills(&work);
    if (work.flag & 0x200) {
        unit->flag |= 0x200;
    }
    if (unit->hensinmae != 0) {
        work.hensinmae = unit->hensinmae;
    } else {
        work.hensinmae = unit->id;
    }
    work.flag = unit->flag;
    work.uniqueid = unit->uniqueid;
    work.exp += unit->exp;
    work.hensinmaeexp = unit->exp;
    for (i = 0; i < unit->skillcnt; i++) {
        if (cmbCopyEvoDevil(&work, unit->skill[i]) != 0) {
            work.skill[work.skillcnt++] = unit->skill[i];
        }
    }
    cmbCopyCustomDevilToStock(unit, &work);
    unit->hp = unit->maxhp;
    unit->mp = unit->maxmp;
}

typedef struct
{
    /* 0x00 */ u16 skill;
    /* 0x02 */ u16 result;
} rstPowerUpEntry_t;

extern rstPowerUpEntry_t D_00339C20[];

s32 rstFindPowerUpSkill(u16 skillId)
{
    rstPowerUpEntry_t *entry = D_00339C20;
    u32 i;

    for (i = 0; i < 0x37; i++, entry++) {
        if (skillId == entry->skill) {
            return entry->result;
        }
    }
    return 0;
}

s32 rstRollPowerUpSkill(u16 skill, datUnitWork_t *unit)
{
    s32 i;

    for (i = 0; i < 0x40; i++) {
        u16 candidate = D_00339658[dds3GetRandA(0) % 0xE3].devilId;
        s32 candidatePower = cmbFindEvoEntry(candidate);

        if (cmbChkSkillOwner(candidate, unit) < 0 &&
            cmbCopyEvoDevil(unit, candidate) != 0 && skill != candidate) {
            if (candidatePower <= cmbFindEvoEntry(skill) + 1 &&
                candidatePower >= cmbFindEvoEntry(skill) + 1) {
                return candidate;
            }
        }
    }
    return 0;
}

s32 datCalcDevilExp(s32 level)
{
    s32 exp = (s32)((f32)(level * level * level) * 1.42f + -5.0f);

    if (exp < 0) {
        exp = 0;
    }
    return exp;
}

extern s32 datCalcDevilExp(s32 level);

s32 datCopyDevilWorkEntry(datUnitWork_t *work, s32 level)
{
    s32 exp = datCalcDevilExp(level);
    s32 baseLevel;
    s32 scaled;
    f32 rate;

    if ((work->flag & 4) != 0) {
        return exp;
    }
    baseLevel = datDevilTable[work->id].level;
    rate = 2.7f - (f32)baseLevel * 0.017f;
    scaled = (s32)((f32)exp * rate);
    scaled = (s32)((f32)scaled - (f32)datCalcDevilExp(baseLevel) * rate);
    return scaled + work->hensinmaeexp;
}

s32 datAdvanceDevilLevel(datUnitWork_t *work)
{
    return datCopyDevilWorkEntry(work, work->level + 1);
}

typedef struct
{
    /* 0x00 */ u16 devilId;
    /* 0x02 */ u16 unk_02;
    /* 0x04 */ s8 unk_04;
    /* 0x05 */ s8 unk_05;
} cmbMitamaEntry_t;

extern cmbMitamaEntry_t D_00339D10[];

s32 func_00223748(u16 devilId)
{
    cmbMitamaEntry_t *entry = D_00339D10;
    u32 i;

    for (i = 0; i < 0x26; i++, entry++) {
        if (entry->devilId == devilId) {
            return entry->unk_05;
        }
    }
    return 0;
}

s32 func_00223788(u16 devilId)
{
    cmbMitamaEntry_t *entry = D_00339D10;
    u32 i;

    for (i = 0; i < 0x26; i++, entry++) {
        if (entry->devilId == devilId) {
            return entry->unk_04;
        }
    }
    return 0;
}

s32 rstRollComment(void)
{
    s8 attempt;
    u32 i;
    s32 draw;
    cmbMitamaEntry_t *entry;

    for (attempt = 0; attempt < 16; attempt++) {
        draw = dds3GetRandA(NULL) % D_00339D10[37].unk_02;
        for (i = 0, entry = D_00339D10; i < 38; i++, entry++) {
            if (draw < entry->unk_02) {
                u16 id = entry->devilId;
                if (DDS3_GBWK->party[0].unit.level >= entry->unk_04 &&
                    DDS3_GBWK->unk_01928[id] <= entry->unk_05) {
                    return id;
                }
            }
        }
    }
    return D_00339D10[0].devilId;
}

s8 func_00223890(datUnitWork_t *unit, s32 rawLevel)
{
    u16 id = unit->id;
    s32 count = 0;
    s32 level;
    s32 i;
    datLevelUp_t *table;

    /* The override is interpreted as a signed low byte before selection. */
    if ((s8)rawLevel > 0) {
        level = (s8)rawLevel;
    } else {
        level = (s8)unit->level;
    }
    table = (datLevelUp_t *)datLevelUpTable;
    for (i = 0; i < 24; i++) {
        if (level < table[id].entry[i].level) {
            return count;
        }
        if (table[id].entry[i].kind == 3) {
            count++;
        }
    }
    return count;
}


/* NON_MATCHING: 10 of 32 words differ, 128 against retail's 132 bytes; the
 * level read copied into its own register in retail. */
#ifdef NON_MATCHING
s32 cmbGetLevelUpCount(datUnitWork_t *unit, u32 exp)
{
    datUnitWork_t work;
    s32 count = 0;
    u32 level;

    cmbCopyCustomDevilToStock(&work, unit);
    work.exp += exp;
    while (work.exp >= datAdvanceDevilLevel(&work)) {
        level = work.level;
        work.level = level + 1;
        if (level >= 0xFF) {
            break;
        }
        count++;
    }
    return count;
}
#else
INCLUDE_ASM(const s32, "result/rstcalc", cmbGetLevelUpCount);
#endif

extern void rstDrawNumber(s32 x, s32 y, s32 number);
extern s32 fclChkMessage(void);
extern void cmpDrawStatusComEx(s32 a, u8 alpha, s8 b, s8 c, s32 d,
                               s8 e, s32 color, s8 f, s8 g, s8 h);

void rstDrawGetCursorRow(void)
{
    rstData_t *work;
    s32 row;

    work = rstDrawWork;
    row = (s8)(*(u8 *)&work->paramCursor.cursor.unk_22 + (u8)work->paramCursor.cursor.unk_24);
    if ((s8)work->AsignParamMax != 0) {
        rstDrawNumber(0, 0, (s8)work->AsignParam);
    }
    if (fclChkMessage() != 0 || (s8)rstDrawWork->AsignParamMax == 0) {
        cmpDrawStatusComEx((s32)rstDrawWork->unk_16C, 0x80, row, 0,
                           (s32)rstDrawWork->unk_129F, 0, 0xA09DC380, 2, 0, 0);
    } else {
        cmpDrawStatusComEx((s32)rstDrawWork->unk_16C, 0x80, row, 2,
                           (s32)rstDrawWork->unk_129F, 0, 0xA09DC380, 2, 0, 0);
    }
}

void rstDrawStatusRow(s8 mode, s8 row, s8 flags)
{
    cmpDrawStatusComEx((s32)rstDrawWork->unk_16C, rstDrawWork->unk_22E4,
                       -1, 0, (s32)rstDrawWork->unk_129F, mode,
                       0xA09DC380, 2, row, flags);
}

void rstDrawStatusRow2(s8 mode, s8 row, s8 selected)
{
    if (rstDrawWork->unk_2308 == 0) {
        cmpDrawStatusComEx((s32)rstDrawWork->unk_16C, rstDrawWork->unk_22E4,
                           -1, 3, (s32)rstDrawWork->unk_129F, mode,
                           0xA09DC380, 2, row, 0);
    } else {
        cmpDrawStatusComEx((s32)rstDrawWork->unk_16C, rstDrawWork->unk_22E4,
                           selected, 3, (s32)rstDrawWork->unk_129F, mode,
                           0xA09DC380, 2, row, 0);
    }
}

extern s32 fclChkMessage(void);
extern void cmpDrawStatusBars(s32 x, s32 y, s32 z);
extern u16 rstSkillSlotIsFull(datUnitWork_t *unit, cmpCursorInfo_t *slot);
extern frFontHandle_t *frGetHelpPos(s32 x, s32 y, s32 priority, s32 bank, s32 index);
extern void frSetFontColor(frFontHandle_t *request, u32 color);
extern s32 func_001C7E38(frFontHandle_t *request, s8 mode, u32 ot);
extern s32 frDestroyFontRequestAfter(frFontHandle_t *request);

s32 rstDrawStatusPanel(s8 mode, s8 showHelp)
{
    s8 selected = *(u8 *)&rstDrawWork->skillSlot.unk_22 + (u8)rstDrawWork->skillSlot.unk_24;
    frFontHandle_t *request;

    if (showHelp == 0 || fclChkMessage() != 0) {
        selected = -1;
    }
    if (selected >= 0) {
        u16 skill;

        cmpDrawStatusBars(0x46, 0x22, 0);
        cmpDrawStatusBars(0x46, 0x22, 0);
        skill = rstSkillSlotIsFull(rstDrawWork->unk_16C, &rstDrawWork->skillSlot);
        request = frGetHelpPos(0x180, 0x9A0, 0xFFFFF, 0, skill);
        frSetFontColor(request, 0x80C08080);
        func_001C7E38(request, 1, 0x47);
        frDestroyFontRequestAfter(request);
    }
    cmpDrawStatusComEx((s32)rstDrawWork->unk_16C, 0x80, selected, showHelp,
                       (s32)rstDrawWork->unk_129F, mode, 0xA09DC380, 2, 2, 0);
    return 1;
}

extern void *dds3SearchSceneBasic(void *pBasic, u32 serialID, s32 type);

extern evtUnit_t *evtGetUnitHandle2(void *unit);

s32 rstFindUnitScene(s32 work)
{
    void *scene = dds3GetActiveScene();
    void *basic;
    evtUnit_t *unit;

    if (scene == NULL) {
        return 0;
    }
    basic = dds3SearchSceneBasic(scene, DDS3_GBWK->integers[12], 5);
    if (basic == NULL) {
        return 0;
    }
    unit = evtGetUnitHandle2(basic);
    if (unit == NULL) {
        return 0;
    }
    if (unit->value == 0) {
        return 0;
    }
    *(u32 *)(*(u8 **)((u8 *)work + 0x18) + 0x80) = unit->value;
    return 1;
}

#ifdef NON_MATCHING
extern s32 cmpGetEncycWork(void);
#endif

s32 rstGetEncycPanel(void)
{
    s32 work;

    if (rstDrawWork->unk_7D == 0) {
        work = cmpGetEncycWork();
        if (work != 0) {
            return rstFindUnitScene(work);
        }
    }
}

s32 rstResetEncycPanel(void)
{
    s8 phase = rstDrawWork->Flag;

    if (phase < 2) {
        if (phase >= 0) {
            return rstGetEncycPanel();
        }
    }
    *(s32 *)(*(u8 **)((u8 *)cmpGetEncycWork() + 0x18) + 0x80) = 0;
}

s32 rstResetEncycPanel2(void)
{
    if (rstDrawWork->unk_7C < 5) {
        return rstGetEncycPanel();
    }
    *(s32 *)(*(u8 **)((u8 *)cmpGetEncycWork() + 0x18) + 0x80) = 0;
}
