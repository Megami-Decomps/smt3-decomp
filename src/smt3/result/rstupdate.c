#include "common.h"
#include "facility.h"
#include "camp.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"
#include "kernel.h"



extern s8 fclEndMessage(void);
extern datDevilTableEntry_t datDevilTable[];

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
    /* 0x002D */ u8 reserved2D[0x46];
    /* 0x0073 */ s8 powerState;
    /* 0x0074 */ s8 powerSlot;
    /* 0x0075 */ u8 reserved75;
    /* 0x0076 */ u16 powerSkill;
    /* 0x0078 */ s32 unk_78;
    /* 0x007C */ s8 evoPhase;
    /* 0x007D */ u8 unk_7D;
    /* 0x007E */ s8 evoResult;
    /* 0x007F */ u8 reserved7F;
    /* 0x0080 */ datUnitWork_t evoWork;
    /* 0x016C */ datUnitWork_t *pCurrentStock;
    /* 0x0170 */ datUnitWork_t workStock;
    /* 0x025C */ cmpPadStopWork_t paramCursor;
    /* 0x0298 */ u8 reserved0298[0x1000];
    /* 0x1298 */ s8 giftKind;
    /* 0x1299 */ u8 reserved1299;
    /* 0x129A */ u16 giftId;
    /* 0x129C */ u8 giftMessage;
    /* 0x129D */ s8 giftStep;
    /* 0x129E */ s8 Flag;
    /* 0x129F */ u8 unk_129F[6];
    /* 0x12A5 */ u8 unk_12A5[3];
    /* 0x12A8 */ cmpCursorInfo_t cursorInfo;
    /* 0x12E0 */ u8 unk_12E0[0x1004];
    /* 0x22E4 */ u8 unk_22E4;
    /* 0x22E5 */ u8 unk_22E5[0x0B];
    /* 0x22F0 */ u16 selectedSkill;
    /* 0x22F2 */ u8 reserved22F2[2];
    /* 0x22F4 */ s32 panelEffectIndex;
    /* 0x22F8 */ u8 reserved22F8[0x10];
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
    /* 0x00000 */ u8 unk_00000[0xA54];
    /* 0x00A54 */ cmbUnit_t party[16];
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



u16 rstSkillSlotIsFull(datUnitWork_t *skills, cmpCursorInfo_t *slot)
{
    s8 index = *(u8 *)&slot->unk_22 + (u8)slot->unk_24;

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

#ifdef NON_MATCHING
extern s32 cmpGetPressedButton(s32 mask);
extern s8 cmpGetParamGaugeColor(s8 index);
extern s32 cmbGetParamBase(datUnitWork_t *unit, s32 index);
extern s32 fclStartSelMessage(s32 message);
extern s32 cmpMoveCursor(cmpCursorInfo_t *cursor, s8 direction);
extern s32 cmpChkPadPressUpDown(void);
extern s32 cmpDisablePadBitStop(cmpPadStopWork_t *work, s32 bits, s32 stop);
extern void cmpPlayCursorSE(u32 button);

void rstUpdateSeqAsignPlayerParam(datUnitWork_t *unit)
{
    s32 values[6];
    s32 i;
    s32 allMax = 1;
    u16 button = (u16)cmpGetPressedButton(0xF3);
    s8 cursor = (s8)(*(u8 *)&startPos->paramCursor.cursor.unk_22 +
                     (u8)startPos->paramCursor.cursor.unk_24);
    s32 parameter = cmpGetParamGaugeColor(cursor);
    rstWork_t *work;

    values[1] = 0; /* Retail leaves this unset; the cursor table never selects it. */
    for (i = 0; i < 6; i++) {
        if (i != 1) {
            values[i] = cmbGetParamBase(unit, i) + (s8)startPos->unk_129F[i];
            if (values[i] < 40) {
                allMax = 0;
            }
        }
    }
    if ((s8)startPos->AsignParam == 0 || allMax != 0) {
        startPos->AsignParam = 0;
        fclStartMessage(2);
        fclStartSelMessage(0x2B);
        startPos->unk_0001 = 0x18;
        startPos->unk_0004 = 1;
        return;
    }
    if (fclChkMessage() != 0) {
        return;
    }
    switch (button) {
    case 1:
    case 0x80:
        if (values[parameter] >= 40) {
            button = 0x8000;
        } else if ((s8)startPos->AsignParam > 0) {
            startPos->unk_129F[parameter]++;
            startPos->AsignParam--;
        }
        break;
    case 0x40:
        if ((s8)startPos->unk_129F[parameter] > 0) {
            startPos->unk_129F[parameter]--;
            startPos->AsignParam++;
        }
        break;
    case 2:
        rstResetAsignParam();
        break;
    case 0x10:
        cmpMoveCursor(&startPos->paramCursor.cursor, -1);
        break;
    case 0x20:
        cmpMoveCursor(&startPos->paramCursor.cursor, 1);
        break;
    }
    rstSetMaxHpMp(0, unit);
    i = cmpChkPadPressUpDown();
    work = startPos;
    if (i == 0) {
        cmpMoveCursor(&work->paramCursor.cursor, 0);
        work = startPos;
    }
    cmpPlayCursorSE((u16)cmpDisablePadBitStop(&work->paramCursor, button, 0x30));
}
#else
INCLUDE_ASM(const s32, "result/rstupdate", rstUpdateSeqAsignPlayerParam);
#endif

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

extern char *cmpGetParamName(s8 index);
extern s32 sprintf(char *buffer, const char *format, ...);
extern char D_0054FD00[]; /* "%d", padded eight-byte small-data span. */

void rstUpdateSeqDevilParam(void)
{
    s32 index;
    char buffer[16];

    if (fclChkMessage() != 0) {
        return;
    }
    fclSetMessageVar_20743088(1, (s32)text2[startPos->pCurrentStock->id]);
    index = startPos->Flag;
    if (index < 6) {
        do {
            if ((s8)startPos->unk_129F[index] > 0) {
                fclSetMessageVar_20743088(8, (s32)cmpGetParamName((s8)index));
                sprintf(buffer, D_0054FD00, (s8)startPos->unk_129F[index]);
                fclSetMessageVar_20743088(9, (s32)buffer);
                if (startPos->Flag == 0) {
                    fclStartMessage(3);
                } else {
                    fclStartMessage(5);
                }
                startPos->Flag = index + 1;
                return;
            }
            index++;
        } while (index < 6);
    }
    if (startPos->Flag == 0) {
        fclStartMessage(4);
        startPos->Flag = 1;
    } else {
        rstSetMaxHpMp(0, startPos->pCurrentStock);
        startPos->unk_0001 = (u8)startPos->unk_0001 + 1;
        startPos->unk_0004 = 1;
    }
}

extern char *pStr[];
extern char *pstr[];
extern void datAddItem(s32 id, s32 count);
extern s32 datAddHearts(s32 id);
extern void rstSetMotionReqFromTbl(s8 type);
extern s32 fclChkSelMessage(void);
extern s8 fclGetSelMessagePos(void);

void rstUpdateSeqGift(void)
{
    if (startPos->giftId == 0) {
        if (startPos->Flag > 0) {
            switch (startPos->Flag) {
            case 1:
                fclStartMessage(startPos->giftMessage);
                break;
            case 2:
                fclStartMessage(27);
                break;
            case 3:
                fclStartMessage(26);
                break;
            }
            startPos->Flag = 0;
        }
        switch (fclChkMessage()) {
        case 2:
            if (startPos->giftStep == 0) {
                p2sdPlaySE(25, 127, 63);
                return;
            }
            break;
        case 0:
            if (startPos->giftStep > 0) {
                startPos->unk_0001 = (u8)startPos->unk_0001 + 1;
                startPos->unk_0004 = 1;
            } else {
                startPos->Flag = 3;
                startPos->giftStep = (u8)startPos->giftStep + 1;
            }
            break;
        }
    } else {
        if (fclChkSelMessage() != 0)
            return;
        switch (fclGetSelMessagePos()) {
        case 0:
            fclEndMessage();
            fclSetMessageVar_20743088(1, (s32)text2[startPos->pCurrentStock->id]);
            if (startPos->giftKind == 0) {
                fclSetMessageVar_20743088(2, (s32)pStr[startPos->giftId]);
                datAddItem(startPos->giftId, 1);
            } else {
                fclSetMessageVar_20743088(2, (s32)pstr[startPos->giftId]);
                datAddHearts(startPos->giftId);
            }
            startPos->Flag = 1;
            {
                rstWork_t *work = startPos;
                work->giftId = 0;
                work->giftStep = 0;
            }
            rstSetMotionReqFromTbl(1);
            break;
        case 2:
            break;
        case 1:
            fclEndMessage();
            startPos->Flag = 2;
            {
                rstWork_t *work = startPos;
                work->giftStep = 1;
                work->giftId = 0;
            }
            rstSetMotionReqFromTbl(4);
            break;
        }
    }
}

extern s32 rstWaitConfirmButton(void);
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


extern void rstInitSkillAct(s8 Pos);

void rstUpdateSeqSkillPowerUp(void)
{
    s8 phase;
    if (startPos->powerState < 0) {
        s32 button = rstWaitConfirmButton();
        if (button == 1) {
            return;
        }
        if (button == 2) {
            if (startPos->Flag > 0) {
                switch (startPos->Flag) {
                case 1:
                    fclStartMessage(15);
                    break;
                case 2:
                    fclStartMessage(16);
                    break;
                }
            }
            rstClearFlag(1);
            startPos->Flag = 0;
        } else {
            if (startPos->Flag == 3) {
                fclStartMessage(17);
                startPos->Flag = 0;
            } else if (startPos->Flag == 4) {
                fclStartMessage(14);
                startPos->Flag = 0;
            } else if (fclChkMessage() == 0) {
                startPos->unk_0001 = (u8)startPos->unk_0001 + 1;
                startPos->unk_0004 = 1;
            }
        }
    } else if (fclChkSelMessage() == 0) {
        switch (fclGetSelMessagePos()) {
        case 0:
            fclEndMessage();
            phase = startPos->powerState;
            /* Separate cases retain the native shared completion after
             * the skill callbacks; grouped labels select a range test. */
            switch (phase) {
            case 1:
                rstOverWriteSkill(&startPos->pCurrentStock->skill[startPos->powerSlot], startPos->powerSkill);
                rstSetMotionReqFromTbl(2);
                rstInitSkillAct(startPos->powerSlot);
                startPos->Flag = phase;
                break;
            case 2:
                rstOverWriteSkill(&startPos->pCurrentStock->skill[startPos->powerSlot], startPos->powerSkill);
                rstSetMotionReqFromTbl(2);
                rstInitSkillAct(startPos->powerSlot);
                startPos->Flag = phase;
                break;
            default:
                startPos->Flag = 3;
                break;
            }
            startPos->powerState = -1;
            break;
        case 2:
            break;
        case 1:
            fclEndMessage();
            rstSetMotionReqFromTbl(3);
            startPos->Flag = 4;
            startPos->powerState = -1;
            break;
        }
    }
}

extern s32 datExplainTable[];

void rstStartIntroduce(u16 DevilID)
{
    fclSetMessageVar_20743088(0, (s32)text2[DevilID]);
    fclSetMessageVar_20743088(4, datExplainTable[datDevilTable[DevilID].race]);
    if (DevilID == 0xCF) {
        fclStartMessage(0xC6);
        return;
    }
    fclStartMessage(master_text[DevilID].kucyo + 0xA8);
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

extern s32 rstChkEvoAct(void);
extern s32 fclGetFlag(s8 index);
extern s32 fclSetWorkValue(s8 index, s32 value);
extern void rstPrepareStatusModel(s32 group, s32 id, s32 blocking);
extern s32 cmpModelSetup(u16 id, s32 blocking);
extern void cmpModelInitCore2(void);
extern void dds3FadeIn(s32 r, s32 g, s32 b, s32 frames);
extern s32 dds3FadeCheck(void);
extern s32 fclEncycRegist(datUnitWork_t *entry, s32 store);
extern void cmbCopyEvoDevilToStock(datUnitWork_t *destination, datUnitWork_t *source);
extern f32 dds3ScreenParam[];

void rstUpdateSeqEvoAct(void)
{
    switch (startPos->evoPhase) {
    case 0:
        startPos->unk_78 = 0;
        if (startPos->unk_22E4 != 0) {
            startPos->unk_22E4 -= 4;
        } else {
            startPos->evoPhase++;
        }
        break;
    case 1:
        rstPrepareStatusModel(6, startPos->evoWork.id, 0);
        startPos->evoPhase++;
        break;
    case 2:
        rstStartEvoActSE();
        if (fclGetFlag(0) != 0) {
            cmpModelSetup(startPos->evoWork.id, 1);
            startPos->evoPhase++;
        }
        break;
    case 3:
        if (fclGetFlag(1) != 0) {
            rstStartIntroduce(startPos->evoWork.id);
            startPos->evoPhase++;
        }
        break;
    case 4:
        if (fclChkMessage() != 0) {
            return;
        }
        startPos->unk_7D = 1;
        fclSetWorkValue(3, 0);
        startPos->evoPhase++;
        break;
    case 5:
        if (dds3FadeCheck() != 0) {
            break;
        }
        if (rstChkEvoAct() != 0) {
            return;
        }
        startPos->unk_22E4 = 0x80;
        cmpModelInitCore2();
        dds3FadeIn(0, 0, 0, 0xF);
        fclSetMessageVar_20743088(0x11,
                                  (s32)text2[startPos->pCurrentStock->id]);
        fclSetMessageVar_20743088(0x12, (s32)text2[startPos->evoWork.id]);
        rstChkResult2Process((u8 *)startPos);
        cmbCopyEvoDevilToStock(startPos->pCurrentStock, &startPos->evoWork);
        fclEncycRegist(startPos->pCurrentStock, 0);
        startPos->unk_0001 = 0x11;
        return;
    }
    /* Exact retail divisor (0x3F999999), below the modern 1.2f value. */
    dds3ScreenParam[4] =
        (dds3ScreenParam[4] - 2048.0f) / 0x1.333332p+0f + 2048.0f;
}

extern void fclFreeStage(void);
extern void rstSetEvoState(void);
extern s32 fclChkSelMessage(void);
extern s8 fclGetSelMessagePos(void);
extern s32 fclSetWorkValue(s8 index, s32 value);
extern void func_0023FCC0(void);

void rstUpdateSeqEvo(void)
{
    s32 state = startPos->Flag;

    if (state > 0) {
        switch (state) {
        case 1:
            fclStartMessage(0x16);
            startPos->unk_0005[0] = state;
            break;
        case 2:
            fclSetMessageVar_20743088(1, (s32)text2[startPos->pCurrentStock->id]);
            fclStartMessage(0x17);
            startPos->unk_0005[0] = 1;
            break;
        case 3:
            if (fclChkMessage() != 0 || rstChkEvoAct() != 0)
                return;
            fclFreeStage();
            startPos->evoResult = -1;
            startPos->unk_0004 = 1;
            return;
        }
        startPos->Flag = 3;
    }

    if (fclChkSelMessage() != 0) {
        return;
    }
    switch (fclGetSelMessagePos()) {
    case 0:
        fclEndMessage();
        rstSetEvoState();
        startPos->unk_0001 = 0x14;
        startPos->unk_0004 = 1;
        fclSetWorkValue(1, 0);
        fclSetWorkValue(2, 0);
        startPos->Flag = 1;
        break;
    case 2:
        break;
    case 1:
        fclEndMessage();
        fclSetWorkValue(1, 1);
        fclSetWorkValue(2, 0);
        func_0023FCC0();
        startPos->Flag = 2;
        break;
    }
}

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

extern s32 cmpGetPressedButton(s32 mask);
extern s32 cmpChkPadPressUpDown(void);
extern s32 cmpMoveCursor(cmpCursorInfo_t *cursor, s8 direction);
extern void cmpPlayCursorSE(u32 button);
extern s32 fclChkMessage(void);
extern s32 fclStartSelMessage(s32 message);
extern char *text1[];

s32 rstUpdateSeqDestroySkill(void)
{
    u16 button = (u16)cmpGetPressedButton(0xF3);
    u8 cursorIndex = (u8)((u8)startPos->cursorInfo.unk_22 +
                          (u8)startPos->cursorInfo.unk_24);

    if (fclChkMessage() != 0)
        return 0;

    switch (button) {
    case 1:
        startPos->selectedSkill =
            rstSkillSlotIsFull(startPos->pCurrentStock,
                               (void *)&startPos->cursorInfo);
        fclSetMessageVar_20743088(10, (s32)text1[startPos->selectedSkill]);
        fclStartMessage(8);
        fclStartSelMessage(0x2B);
        startPos->unk_0001 = 0x16;
        startPos->unk_0004 = (s8)button;
        break;
    case 2:
        return 0;
    case 0x40:
        if (cursorIndex < 8) {
            if (cursorIndex < 4)
                startPos->cursorInfo.unk_24 = 8;
            else
                startPos->cursorInfo.unk_24 = (u8)startPos->cursorInfo.unk_24 + 0xFC;
        } else {
            startPos->cursorInfo.unk_24 = (u8)startPos->cursorInfo.unk_24 + 0xFC;
        }
        break;
    case 0x80:
        if (cursorIndex < 8) {
            if (cursorIndex < 4)
                startPos->cursorInfo.unk_24 += 4;
            else
                startPos->cursorInfo.unk_24 = 8;
        } else {
            startPos->cursorInfo.unk_24 = 0;
        }
        break;
    case 0x10:
        if (startPos->cursorInfo.unk_22 + (u8)startPos->cursorInfo.unk_24 == 8)
            return 0;
        cmpMoveCursor(&startPos->cursorInfo, -1);
        break;
    case 0x20:
        if (startPos->cursorInfo.unk_22 + (u8)startPos->cursorInfo.unk_24 == 8)
            return 0;
        cmpMoveCursor(&startPos->cursorInfo, 1);
        break;
    default:
        break;
    }

    if (cmpChkPadPressUpDown() == 0)
        cmpMoveCursor(&startPos->cursorInfo, 0);
    cmpPlayCursorSE(button);
    return 1;
}


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

extern char *text1[];

s32 rstUpdateSeqDestroyConfirm(void)
{
    s32 slot;

    if (startPos->Flag > 0) {
        switch (startPos->Flag) {
        case 1:
        case 2:
            if (startPos->pCurrentStock->id == 0)
                fclStartMessage(9);
            else
                fclStartMessage(10);
            startPos->unk_0005[0] = 1;
            break;
        case 3:
            if (fclChkMessage() != 0)
                return 1;
            if (startPos->pCurrentStock->id == 0) {
                startPos->unk_0001 = 1;
                startPos->unk_0004 = 1;
            } else {
                startPos->unk_0001 = 8;
                startPos->unk_0004 = 1;
            }
            break;
        }
        startPos->Flag = 3;
    }
    if (fclChkSelMessage() != 0)
        return 0;
    switch (fclGetSelMessagePos()) {
    case 0:
        fclEndMessage();
        if (startPos->pCurrentStock->id == 0)
            rstGetHeartsSkill(1);
        /* This cursor position uses the low byte of the +0x22 field. */
        slot = (s8)(((u8 *)&startPos->cursorInfo)[0x22] +
                    (u8)startPos->cursorInfo.unk_24);
        if (slot < 8) {
            startPos->pCurrentStock->skill[slot] = 0;
            rstBuildDestroyList();
            rstAddSkill();
            startPos->Flag = 1;
        } else {
            rstSetMaxHpMp(1, startPos->pCurrentStock);
            {
                s32 count = startPos->LevelUpCnt;
                startPos->LevelUpCnt = count - 1;
            }
            startPos->Flag = 2;
        }
        fclSetMessageVar_20743088(10, (s32)text1[startPos->selectedSkill]);
        break;
    case 2:
        break;
    case 1:
        fclEndMessage();
        startPos->unk_0001 = 21;
        startPos->unk_0004 = 2;
        break;
    }
    return 1;
}

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

extern void rstInitSkillAct(s8 Pos);

s32 rstStandbyHeartsSkillPowerUp(void)
{
    s8 result;

    switch (rstWaitConfirmButton()) {
    case 1:
        return 0;
    case 2:
        switch (startPos->powerState) {
        case 1:
            fclStartMessage(31);
            break;
        case 2:
            fclStartMessage(32);
            break;
        }
        rstClearFlag(1);
        startPos->powerState = -1;
        return 1;
    default:
        break;
    }

    fclSetMessageVar_20743088(
        15, (s32)text1[startPos->pCurrentStock->skill[startPos->powerSlot]]);
    fclSetMessageVar_20743088(16, (s32)text1[startPos->powerSkill]);

    result = startPos->powerState;
    switch (result) {
    case 1:
        rstOverWriteSkill(
            &startPos->pCurrentStock->skill[startPos->powerSlot],
            startPos->powerSkill);
        cmbGlobalWork->hearts_sk[cmbGlobalWork->heartsequip]
                               [startPos->powerSlot] = startPos->powerSkill;
        rstInitSkillAct(startPos->powerSlot);
        return 0;
    case 2:
        rstOverWriteSkill(
            &startPos->pCurrentStock->skill[startPos->powerSlot],
            startPos->powerSkill);
        cmbGlobalWork->hearts_sk[cmbGlobalWork->heartsequip]
                               [startPos->powerSlot] = startPos->powerSkill;
        rstInitSkillAct(startPos->powerSlot);
        break;
    default:
        break;
    }
    return 0;
}


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
    cmbUnit_t *pu = &cmbGlobalWork->party[0];

    pu->unit.hp = pu->unit.maxhp;
    pu->unit.mp = pu->unit.maxmp;
    pu->unit.badstatus &= 0x8000;
}

extern s32 rstStandbyHeartsSkillPowerUp(void);
extern s32 cmpBadEffSetup(u16 id, u32 flag);
extern s32 cmpAddQuePanelEff(s32 a1, s32 a2, s32 a3, u16 a4, u16 a5,
                             u8 a6, u8 a7, s32 a8, s32 a9, u8 a10);
extern char D_0054FD08[];

s32 rstStandbyHeartsEvent(s8 event)
{
    startPos->panelEffectIndex = -1;

    switch (event) {
        case 0:
            if (rstStandbyHeartsAddPoint() == 0) {
                event = 7;
            } else {
                fclSetMessageVar_20743088(12, (s32)pstr[cmbGlobalWork->heartsequip]);
                fclSetMessageVar_20743088(8, (s32)cmpGetParamName((s8)startPos->bonusParamIndex));
                fclSetMessageVar_20743088(9, (s32)D_0054FD08);
                fclStartMessage(0x1E);
            }
            break;
        case 1:
            if (rstStandbyHeartsSkillPowerUp() == 0) {
                return 0;
            }
            break;
        case 2:
            fclStartMessage(0x21);
            cmpBadEffSetup(startPos->pCurrentStock->id, 0x40);
            break;
        case 3:
            fclStartMessage(0x22);
            cmpBadEffSetup(startPos->pCurrentStock->id, 0x100);
            break;
        case 4:
            fclStartMessage(0x23);
            cmpBadEffSetup(startPos->pCurrentStock->id, 0x8000);
            break;
        case 5:
            fclStartMessage(0x24);
            p2sdPlaySE(0x10, 0x7F, 0x3F);
            startPos->panelEffectIndex = cmpAddQuePanelEff(
                0, 0, 0, 0, 0, 8, 0, 0x41263080, 0x41263080, 4);
            break;
        case 6:
            fclStartMessage(0x25);
            p2sdPlaySE(0x10, 0x7F, 0x3F);
            startPos->panelEffectIndex = cmpAddQuePanelEff(
                0, 0, 0, 0, 0, 8, 0, 0x41263080, 0x41263080, 4);
            break;
        default:
            break;
    }
    if (event == 7) {
        fclStartMessage(0x26);
    }
    startPos->unk_0005[0] = 1;
    return 1;
}

extern void datOverwriteBadStat(datUnitWork_t *work, u32 badstatus);

/* The caller supplies the event; the current event remains in shared state. */
s32 rstSetHeartsEvent(s8 event)
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

extern s32 rstStandbyHeartsEvent(s8 event);
extern s8 rstGetPanelEffState(s32 index);

void rstUpdateSeqHeartsEvent(void)
{
    if (startPos->Flag > 0) {
        switch (startPos->Flag) {
        case 1:
            if (rstStandbyHeartsEvent(startPos->heartsEvent) == 0)
                return;
            break;
        case 2:
            fclStartMessage(29);
            startPos->unk_0005[0] = 1;
            break;
        case 3:
            if (fclChkMessage() != 0)
                return;
            if (rstGetPanelEffState(startPos->panelEffectIndex) != 0)
                return;
            rstSetHeartsEvent(startPos->heartsEvent);
            startPos->unk_0004 = 1;
            return;
        default:
            break;
        }
        startPos->Flag = 3;
    }
    if (fclChkSelMessage() != 0)
        return;
    switch (fclGetSelMessagePos()) {
    case 0:
        fclEndMessage();
        startPos->Flag = 1;
        break;
    case 2:
        break;
    case 1:
        fclEndMessage();
        startPos->Flag = 2;
        startPos->heartsEvent = -1;
        break;
    }
}

extern char *pstr[];
extern s32 datGetPlayerLNDLevel(datUnitWork_t *unit);
extern void rstSetDevilRegistered(s8 id);
extern void rstSetHeartsMasterPhase(void);
extern char D_0053DC40[]; /* Format occupies a padded 32-byte span. */

void rstUpdateSeqHeartsMaster(void)
{
    s32 before;
    s32 after;
    s32 i;

    if (fclChkMessage() != 0)
        return;
    switch (startPos->Flag) {
    case 0:
        fclSetMessageVar_20743088(12, (s32)pstr[cmbGlobalWork->heartsequip]);
        fclStartMessage(39);
        before = datGetPlayerLNDLevel(startPos->pCurrentStock);
        rstSetDevilRegistered((s8)cmbGlobalWork->heartsequip);
        after = datGetPlayerLNDLevel(startPos->pCurrentStock);
        if (before != after) {
            printf(D_0053DC40, after);
            if (after != 0)
                rstSetHeartsMasterPhase();
        }
        break;
    case 1:
        for (i = 0; i < 6; i++)
            startPos->pCurrentStock->param[i] += startPos->unk_129F[i];
        rstSetMaxHpMp(0, startPos->pCurrentStock);
        rstChkResult2Process((u8 *)startPos);
        startPos->heartsEvent = -1;
        startPos->unk_0004 = 1;
        return;
    }
    startPos->Flag = (u8)startPos->Flag + 1;
}


void rstUpdateBgTimer(void)
{
    startPos->unk_2310++;
    if (startPos->unk_2310 >= 0x900) {
        startPos->unk_2310 = 0;
    }
}

extern void *dds3GetProcessData(dds3ProcessID_t *id);
extern s32 dds3FadeCheck(void);
extern s32 cmpLoadStatusModel(void);
extern void rstUpdateSeqAsignPlayerParam(datUnitWork_t *unit);
extern void rstUpdateSeqSkillPowerUp(void);
extern void rstUpdateSeqEvo(void);
extern void rstUpdateSeqEvoAct(void);
extern s32 rstUpdateSeqDestroySkill(void);
extern void fclUpdateMessage(void);
extern s32 cmpUpdateSeqInfo(void *work, s8 previous);

INCLUDE_RODATA(const s32, "result/rstupdate", D_0053DC40);

s32 rstUpdate(dds3ProcessID_t *id)
{
    s8 previous;
    u8 flags;
    u8 enabled;

    startPos = dds3GetProcessData(id);
    previous = startPos->unk_0001;
    if (dds3FadeCheck() != 0)
        return 0;
    if (cmpLoadStatusModel() != 0)
        return 0;
    flags = startPos->unk_0000;
    if ((flags & 0x40) != 0)
        return -1;
    if ((flags & 2) != 0) {
        enabled = flags & 1;
        if (enabled != 0) {
            switch (startPos->unk_0001) {
            case 0:
                if ((s8)startPos->AsignParamMax != 0)
                    rstUpdateSeqAsignPlayerParam(&cmbGlobalWork->party[0].unit);
                else
                    rstUpdateSeqPlayerLevelUpOnly();
                break;
            case 1: rstUpdateSeqHeartsSkill(); break;
            case 3: rstUpdateSeqHeartsEvent(); break;
            case 5: rstUpdateSeqHeartsMaster(); break;
            case 6: rstUpdateSeqDevilParam(); break;
            case 8: rstUpdateSeqDefaultSkill(); break;
            case 10: rstUpdateSeqSkillPowerUp(); break;
            case 11:
                if (fclChkMessage() == 0) {
                    startPos->unk_0001 = (u8)startPos->unk_0001 + 1;
                    startPos->unk_0004 = 1;
                }
                break;
            case 13: rstUpdateSeqGift(); break;
            case 17: rstUpdateSeqEvo(); break;
            case 18: rstUpdateSeqEvoAt(); break;
            case 20: rstUpdateSeqEvoAct(); break;
            case 21: rstUpdateSeqDestroySkill(); break;
            case 22:
                if (rstUpdateSeqDestroyConfirm() == 0)
                    return 0;
                break;
            case 24:
                if (rstUpdateSeqConfirm() == 0)
                    return 0;
                break;
            }
        }
    }
    fclUpdateMessage();
    rstUpdateBgTimer();
    rstUpdateEvoBgFade();
    cmpUpdateSeqInfo(startPos, previous);
    return 0;
}


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

