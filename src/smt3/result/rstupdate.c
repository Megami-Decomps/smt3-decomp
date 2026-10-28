#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"

typedef struct
{
    /* 0x00 */ u8 StepY[4];
    /* 0x04 */ s8 StopFlag;
    /* 0x05 */ u8 unk_05[3];
    /* 0x08 */ s32 CursorSize;
    /* 0x0C */ s32 CursorPos;
    /* 0x10 */ u16 unk_10;
    /* 0x12 */ u16 unk_12;
    /* 0x14 */ u16 unk_14;
    /* 0x16 */ u16 unk_16;
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ s32 unk_1C;
    /* 0x20 */ u8 unk_20;
    /* 0x21 */ u8 unk_21;
    /* 0x22 */ s16 unk_22;
    /* 0x24 */ s8 unk_24;
    /* 0x25 */ s8 unk_25;
    /* 0x26 */ u8 unk_26;
    /* 0x27 */ u8 unk_27;
    /* 0x28 */ s32 unk_28;
    /* 0x2C */ s32 unk_2C;
    /* 0x30 */ s32 unk_30;
    /* 0x34 */ s32 unk_34;
} cmpCursorInfo_t;

typedef struct
{
    u8 flag;
    u8 unk_01[3];
    u8 race;
    u8 unk_05;
    u8 unk_06[54];
} datDevilInfo_t;

typedef struct
{
    /* 0x00 */ datUnitWork_t unit;
} cmbUnit_t;

extern datDevilInfo_t datDevilTable[];

typedef struct
{
    /* 0x0000 */ u8 unk_00[0x20];
    /* 0x0020 */ f32 screenOfsX;
    /* 0x0024 */ f32 screenOfsY;
    /* 0x0028 */ u8 unk_28[0x10];
    /* 0x0038 */ s32 texHandle[7];
    /* 0x0054 */ s32 stat;
    /* 0x0058 */ u16 unk_58;
    /* 0x005A */ u16 unk_5A;
    /* 0x005C */ u8 unk_5C[8];
    /* 0x0064 */ s16 lw_cur[4];
    /* 0x006C */ u8 unk_6C;
    /* 0x006D */ u8 unk_6D;
    /* 0x006E */ u16 modelId;
    /* 0x0070 */ s32 unk_70;
    /* 0x0074 */ u8 unk_74[0x10];
    /* 0x0084 */ u8 unk_84[0x2DC];
    /* 0x0360 */ void *explainData;
    /* 0x0364 */ s32 unk_364;
    /* 0x0368 */ void *allocation;
    /* 0x036C */ s32 pwork;
} fclEncyc_instance_tag;

typedef struct
{
    /* 0x0000 */ u8 unk_0000;
    /* 0x0001 */ s8 unk_0001;
    /* 0x0002 */ u8 unk_0002[2];
    /* 0x0004 */ s8 unk_0004;
    /* 0x0005 */ u8 unk_0005[0x13];
    /* 0x0018 */ u8 AsignParamMax;
    /* 0x0019 */ u8 AsignParam;
    /* 0x001A */ u8 unk_1A;
    /* 0x001B */ u8 LevelUpCnt;
    /* 0x001C */ u8 unk_1C[6];
    /* 0x0022 */ u16 EventParam;
    /* 0x0024 */ u16 GetHeartsSkill;
    /* 0x0026 */ s8 RecoverFlag;
    /* 0x0027 */ s8 HeartsSkFlag;
    /* 0x0028 */ s8 HeartsSkillResult;
    /* 0x0029 */ s8 heartsEvent;
    /* 0x002A */ u16 bonusParamIndex;
    /* 0x002C */ s8 DefaultSkillResult;
    /* 0x002D */ u8 reserved2D[0x4B];
    /* 0x0078 */ s32 unk_78;
    /* 0x007C */ u8 unk_7C[0xF0];
    /* 0x016C */ datUnitWork_t *pCurrentStock;
    /* 0x0170 */ u8 unk_170[0x112E];
    /* 0x129E */ s8 Flag;
    /* 0x129F */ u8 unk_129F[6];
    /* 0x12A5 */ u8 unk_12A5[3];
    /* 0x12A8 */ cmpCursorInfo_t cursorInfo;
    /* 0x12E0 */ u8 unk_12E0[0x1004];
    /* 0x22E4 */ u8 unk_22E4;
    /* 0x22E5 */ u8 unk_22E5[0x23];
    /* 0x2308 */ s32 unk_2308;
    /* 0x230C */ s32 unk_230C;
    /* 0x2310 */ u32 unk_2310;
    /* 0x2314 */ u32 evoBgFade;
} rstWork_t;

extern void rstSetMaxHpMp(s8 Mode, datUnitWork_t *pStock);

extern void func_00224230(datUnitWork_t *unit);

extern s8 dds3PadButtonState[];

extern rstWork_t *startPos;

typedef struct
{
    /* 0x00 */ datUnitWork_t unit;
} dds3PartyUnit_t;

typedef struct
{
    /* 0x00000 */ u8 unk_00000[0xA54];
    /* 0x00A54 */ dds3PartyUnit_t party[16];
    /* 0x01914 */ s8 stock[0x10];
    /* 0x01924 */ s32 stockCount;
    /* 0x01928 */ u8 unk_01928[0x3D];
    /* 0x01965 */ u8 unk_1965;
    /* 0x01966 */ u8 unk_01966[0x394];
    /* 0x01CFA */ u8 heartsequip;
    /* 0x01CFB */ u8 unk_01CFB[0x40];
    /* 0x01D3B */ u8 heartsskcnt[64];
    /* 0x01D7B */ u8 unk_01D7B[0xC0];
    /* 0x01E3B */ u8 unk_01E3B[0xFC85];
    /* 0x11AC0 */ u16 hearts_sk[32][12];
    /* 0x11DC0 */ u8 hearts_up_param[32][6];
    /* 0x11E80 */ s32 encycNum;
} cmbGlobalWork_t;

/* newdata.h declares the same symbol dds3GlobalWork_t *DDS3_GBWK, and this
 * unit reads it through a different member set at the same offsets. Both are
 * partial views; the alias holds them apart until one covers the object. */
extern cmbGlobalWork_t *cmbGlobalWork asm("DDS3_GBWK");

extern void p2sdPlaySE(s32 index, s32 volume, s32 pan);

extern s32 fclChkMessage(void);

void rstClearSkillSlot(cmbUnit_t *unit, u8 slot);

s8 rstFindSkillSlot(u16 skill, cmbUnit_t *unit);

extern char *text2[];

void fclSetMessageVar_20743088(s8 Index, s32 pStr);

s32 fclStartMessage(s32 MsgNo);

extern void (*DrawPhaseTable[])(fclEncyc_instance_tag *work);

void rstChkResult2Process(u8 *work);
extern void rstClearFlag(s8 index);

void rstOverWriteSkill(u16 *pSkill, u16 NewSkillID)
{
    *pSkill = NewSkillID;
    rstSetMaxHpMp(1, startPos->pCurrentStock);
}

extern s32 rstAddSkillToSlot(u16 skill, cmbUnit_t *unit);

void rstAddSkill(void)
{
    u16 skill;
    s8 slot;
    s32 count;

    if (startPos->pCurrentStock->id == 0) {
        skill = startPos->GetHeartsSkill;
    } else {
        skill = startPos->EventParam;
    }
    count = startPos->LevelUpCnt;
    startPos->LevelUpCnt = count - 1;
    rstAddSkillToSlot(skill, (cmbUnit_t *)startPos->pCurrentStock);
    slot = rstFindSkillSlot(skill, (cmbUnit_t *)startPos->pCurrentStock);
    if (slot >= 0) {
        rstClearSkillSlot((cmbUnit_t *)startPos->pCurrentStock, (u8)slot);
    }
    rstSetMaxHpMp(1, startPos->pCurrentStock);
}

s32 rstBuildDestroyList(void)
{
    datUnitWork_t *unit = startPos->pCurrentStock;
    s32 i;
    s8 count = 0;
    for (i = 0; i < unit->skillcnt; i++) {
        if (unit->skill[i] != 0) {
            u16 skill = unit->skill[i];
            unit->skill[i] = 0;
            unit->skill[count++] = skill;
        }
    }
    unit->skillcnt = count;
    return count;
}

typedef struct
{
    /* 0x00 */ u8 unk_00[0x22];
    /* 0x22 */ u8 unk_22;
    /* 0x23 */ u8 unk_23;
    /* 0x24 */ u8 unk_24;
} rstSkillSlot_t;

u16 rstSkillSlotIsFull(datUnitWork_t *skills, rstSkillSlot_t *slot)
{
    s8 index = slot->unk_22 + slot->unk_24;

    if (index >= skills->skillcnt) {
        if (startPos->pCurrentStock->id == 0) {
            return startPos->GetHeartsSkill;
        }
        return startPos->EventParam;
    }
    return skills->skill[index];
}

s32 rstWaitConfirmButton(void)
{
    s32 timer;

    if (startPos->unk_2308 > 0) {
        if (dds3PadButtonState[0x21] < 0) {
            rstClearFlag(1);
            startPos->unk_2308 = 1;
        }
        timer = startPos->unk_2308;
        if (timer == 1) {
            startPos->unk_2308 = 0;
            return 2;
        }
        startPos->unk_2308 = timer - 1;
        return 1;
    }
    return 0;
}

void rstResetAsignParam(void)
{
    startPos->AsignParam = startPos->AsignParamMax;
    rstChkResult2Process((u8 *)startPos);
}

void func_00224230(datUnitWork_t *unit)
{
    s32 i;

    for (i = 0; i < 6; i++) {
        unit->param[i] += startPos->unk_129F[i];
    }
}

void rstAddAsignParam(void)
{
    func_00224230(startPos->pCurrentStock);
}

INCLUDE_ASM(const s32, "result/rstupdate", rstUpdateSeqAsignPlayerParam);

void rstUpdateSeqPlayerLevelUpOnly(void)
{
    if (startPos->Flag == 0)
    {
        fclStartMessage(1);
        startPos->Flag = 1;
    }
    if (fclChkMessage() == 0)
    {
        startPos->unk_0001 = 1;
        startPos->unk_0004 = 1;
    }
}

INCLUDE_ASM(const s32, "result/rstupdate", rstUpdateSeqDevilParam);

INCLUDE_ASM(const s32, "result/rstupdate", rstUpdateSeqGift);

extern s32 rstWaitConfirmButton(void);
extern s8 fclEndMessage(void);
extern char *text1[];
extern void cmpResetCursorInfo(cmpCursorInfo_t *cursor);

void rstUpdateSeqDefaultSkill(void)
{
    s32 state = rstWaitConfirmButton();

    if (state == 1) {
        return;
    }
    if (state == 2) {
        fclSetMessageVar_20743088(10, (s32)text1[startPos->EventParam]);
        fclStartMessage(6);
        rstClearFlag(1);
        return;
    }
    if (fclChkMessage() != 0) {
        return;
    }
    if (startPos->DefaultSkillResult == 2) {
        fclEndMessage();
        fclStartMessage(7);
        cmpResetCursorInfo(&startPos->cursorInfo);
        startPos->cursorInfo.unk_24 = 8;
        startPos->unk_0001 = 0x15;
        startPos->unk_0004 = 1;
    } else {
        rstAddSkill();
        startPos->unk_0004 = 1;
    }
}


INCLUDE_ASM(const s32, "result/rstupdate", rstUpdateSeqSkillPowerUp);

typedef struct
{
    /* 0x00 */ s32 mesno;
    /* 0x04 */ u8 unk_04[0x2C];
} datMasterText_t;

extern s32 datExplainTable[];

extern datMasterText_t master_text[];

void rstStartIntroduce(u16 DevilID)
{
    fclSetMessageVar_20743088(0, (s32)text2[DevilID]);
    fclSetMessageVar_20743088(4, datExplainTable[datDevilTable[DevilID].race]);
    if (DevilID == 0xCF) {
        fclStartMessage(0xC6);
        return;
    }
    fclStartMessage(master_text[DevilID].mesno + 0xA8);
}

void rstUpdateEvoBgFade(void)
{
    u32 max = 0x32;
    rstWork_t *work = startPos;

    if (work->unk_0001 == 0x14) {
        u32 v = work->evoBgFade;

        if (v < 0x32) {
            work->evoBgFade = v + 1;
        }
        if (max < startPos->evoBgFade) {
            startPos->evoBgFade = max;
        }
    } else {
        work->evoBgFade = 0;
    }
}

void rstStartEvoActSE(void)
{
    s32 timer = startPos->unk_78;
    s32 next;

    if (timer < 0) {
        return;
    }
    next = timer + 1;
    startPos->unk_78 = next;
    if (next != 0x28) {
        return;
    }
    p2sdPlaySE(0x2B0001, 0x7F, 0x3F);
    startPos->unk_78 = -1;
}

INCLUDE_ASM(const s32, "result/rstupdate", rstUpdateSeqEvoAct);

INCLUDE_ASM(const s32, "result/rstupdate", rstUpdateSeqEvo);

extern s32 rstChkEvoAct(void);
extern void evtFreeStage(void);
extern void fclFilterOFF(void);
extern void func_0023FCC0(void);
extern s32 fclChkMessage(void);
extern s32 fclSetWorkValue(s8 index, s32 value);

void rstUpdateSeqEvoAt(void)
{
    if (startPos->Flag != 0) {
        if (rstChkEvoAct() != 0) {
            return;
        }
        if (fclChkMessage() != 0) {
            return;
        }
        evtFreeStage();
        fclFilterOFF();
        func_0023FCC0();
        startPos->Flag = 2;
        startPos->unk_0001 = 0x11;
        startPos->unk_0004 = 1;
    }
    {
        s32 message = fclChkMessage();
        if (message == 0 || message == 3) {
            fclSetWorkValue(1, 1);
            fclSetWorkValue(2, 0);
            startPos->Flag = 1;
        }
    }
}

extern s32 fclChkSelMessage(void);
extern s8 fclGetSelMessagePos(void);

s32 rstUpdateSeqConfirm(void)
{
    if (fclChkSelMessage() != 0)
        return 0;
    switch (fclGetSelMessagePos()) {
    case 0:
        rstAddAsignParam();
        rstChkResult2Process((u8 *)startPos);
        fclEndMessage();
        startPos->unk_0001 = 1;
        startPos->unk_0004 = 1;
        break;
    /* The third case preserves the native three-node dispatch.
     * See docs/levers/control-flow.md. */
    case 2:
        break;
    case 1:
        rstResetAsignParam();
        fclEndMessage();
        startPos->unk_0001 = 0;
        startPos->unk_0004 = 2;
        break;
    }
    return 1;
}

INCLUDE_ASM(const s32, "result/rstupdate", rstUpdateSeqDestroySkill);

extern u16 rstChkHeartsSkill(datUnitWork_t *unit);
extern s32 datGetDevilSlotState(u8 devilId);
extern s32 printf(const char *format, ...);

static const char D_0053DBD8[] = "New hearts skill ID : %02x\n";

s8 rstGetHeartsSkill(s8 type)
{
    datUnitWork_t *player = &cmbGlobalWork->party[0].unit;
    u16 skill = rstChkHeartsSkill(player);

    if (skill != 0) {
        if (type == 0) {
            player->skill[player->skillcnt++] = skill;
        }
        cmbGlobalWork->hearts_sk[cmbGlobalWork->heartsequip]
                                  [datGetDevilSlotState(cmbGlobalWork->heartsequip)] = skill;
        cmbGlobalWork->heartsskcnt[cmbGlobalWork->heartsequip]++;
        if (type == 0) {
            startPos->HeartsSkFlag++;
        }
        printf(D_0053DBD8, skill);
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "result/rstupdate", rstUpdateSeqDestroyConfirm);

extern s32 rstWaitConfirmButton(void);
extern s32 fclChkMessage(void);
extern s8 fclEndMessage(void);
extern char *text1[];
extern void cmpResetCursorInfo(cmpCursorInfo_t *cursor);
extern s8 rstGetHeartsSkill(s8 mode);

void rstUpdateSeqHeartsSkill(void)
{
    s32 state = rstWaitConfirmButton();

    if (state == 1) {
        return;
    }
    if (state == 2) {
        fclSetMessageVar_20743088(10, (s32)text1[startPos->GetHeartsSkill]);
        fclStartMessage(6);
        rstClearFlag(1);
        return;
    }
    if (fclChkMessage() != 0) {
        return;
    }
    if (startPos->HeartsSkillResult == 2) {
        fclEndMessage();
        fclStartMessage(7);
        cmpResetCursorInfo(&startPos->cursorInfo);
        startPos->cursorInfo.unk_24 = 8;
        startPos->unk_0001 = 0x15;
        startPos->unk_0004 = 1;
    } else {
        rstGetHeartsSkill(0);
        rstSetMaxHpMp(1, startPos->pCurrentStock);
        startPos->unk_0004 = 1;
    }
}

extern void cmbCopyCustomDevilToStock(datUnitWork_t *dst, datUnitWork_t *src);
extern s8 cmbAddLevelUpParamEx(datUnitWork_t *work, s8 mode);
extern s32 cmbGetParamBase(datUnitWork_t *unit, s32 index);

s32 rstStandbyHeartsAddPoint(void)
{
    datUnitWork_t copy;
    s8 index;

    cmbCopyCustomDevilToStock(&copy, startPos->pCurrentStock);
    index = cmbAddLevelUpParamEx(&copy, 0);
    if (index < 0) {
        return 0;
    }
    if (cmbGetParamBase(startPos->pCurrentStock, index) >= 0x28) {
        return 0;
    }
    startPos->unk_129F[index] = 1;
    startPos->bonusParamIndex = index;
    rstSetMaxHpMp(1, startPos->pCurrentStock);
    return 1;
}

INCLUDE_ASM(const s32, "result/rstupdate", rstStandbyHeartsSkillPowerUp);

void rstSetHeartsRecover(void)
{
    s32 i;

    startPos->RecoverFlag = 1;
    for (i = 0; i < cmbGlobalWork->stockCount; i++) {
        s8 stockIndex = cmbGlobalWork->stock[i];
        datUnitWork_t *unit = &cmbGlobalWork->party[(s32)stockIndex].unit;

        if ((unit->flag & 2) != 0) {
            unit->hp = unit->maxhp;
            unit->mp = unit->maxmp;
            unit->badstatus &= 0x8000;
        }
    }
}

void rstSetHeartsRecoverPlayer(void)
{
    dds3PartyUnit_t *pu = &cmbGlobalWork->party[0];

    pu->unit.hp = pu->unit.maxhp;
    pu->unit.mp = pu->unit.maxmp;
    pu->unit.badstatus &= 0x8000;
}

INCLUDE_ASM(const s32, "result/rstupdate", rstStandbyHeartsEvent);

extern void datOverwriteBadStat(datUnitWork_t *work, u32 badstatus);

s32 rstSetHeartsEvent(void)
{
    s32 result = -1;

    switch (startPos->heartsEvent) {
    case 0:
        rstChkResult2Process((u8 *)startPos);
        cmbGlobalWork->party[0].unit.levelupparam[startPos->bonusParamIndex]++;
        cmbGlobalWork->party[0].unit.param[startPos->bonusParamIndex]++;
        break;
    case 2:
        datOverwriteBadStat(&cmbGlobalWork->party[0].unit, 0x40);
        break;
    case 3:
        datOverwriteBadStat(&cmbGlobalWork->party[0].unit, 0x100);
        break;
    case 4:
        datOverwriteBadStat(&cmbGlobalWork->party[0].unit, 0x8000);
        break;
    case 5:
        rstSetHeartsRecover();
        break;
    case 6:
        rstSetHeartsRecoverPlayer();
        break;
    case 1:
    case 7:
        break;
    }
    startPos->heartsEvent = result;
    return result;
}

INCLUDE_ASM(const s32, "result/rstupdate", rstUpdateSeqHeartsEvent);

INCLUDE_ASM(const s32, "result/rstupdate", rstUpdateSeqHeartsMaster);

void rstUpdateBgTimer(void)
{
    startPos->unk_2310++;
    if (startPos->unk_2310 >= 0x900) {
        startPos->unk_2310 = 0;
    }
}

INCLUDE_ASM(const s32, "result/rstupdate", rstUpdate);

void func_002262C8(void) {
}

void func_002262D0(void) {
}

void func_002262D8(void) {
}

void func_002262E0(void) {
}

INCLUDE_RODATA(const s32, "result/rstupdate", D_0053DCD0);

INCLUDE_RODATA(const s32, "result/rstupdate", D_0053DCE8);

INCLUDE_RODATA(const s32, "result/rstupdate", D_0053DCF8);

INCLUDE_RODATA(const s32, "result/rstupdate", D_0053DD08);

INCLUDE_RODATA(const s32, "result/rstupdate", D_0053DD18);

INCLUDE_RODATA(const s32, "result/rstupdate", D_0053DD30);

INCLUDE_RODATA(const s32, "result/rstupdate", D_0053DD48);

INCLUDE_RODATA(const s32, "result/rstupdate", D_0053DD58);

INCLUDE_RODATA(const s32, "result/rstupdate", D_0053DD68);

INCLUDE_SDATA(const s32, "result/rstupdate", D_0054FD00);

INCLUDE_SDATA(const s32, "result/rstupdate", D_0054FD08);

