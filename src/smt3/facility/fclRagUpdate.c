#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"

extern s32 evtStartEvent(s32 no);

extern s32 fclSetWorkValue(s8 index, s32 value);

s32 fclGetFlag(s8 index);

void fclClearFlag(s8 index);

typedef struct
{
    /* 0x00 */ void *allocation;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ s16 unk_08;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ s32 flags;
    /* 0x10 */ s16 unk_10;
    /* 0x12 */ u16 unk_12;
    /* 0x14 */ s32 activeTime;
    /* 0x18 */ s32 deactiveTime;
    /* 0x1C */ s32 phase;
    /* 0x20 */ s32 count;
} fclRbnWork_t;

typedef struct
{
    /* 0x00 */ u8 unk_00[4];
    /* 0x04 */ s8 unk_04;
    /* 0x05 */ u8 unk_05[3];
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s32 unk_0C;
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
    /* 0x27 */ u8 unk_27[5];
    /* 0x2C */ s32 unk_2C;
    /* 0x30 */ s32 unk_30;
    /* 0x34 */ s32 unk_34;
} cmpCursorWork_t;

typedef struct
{
    /* 0x00 */ cmpCursorWork_t cursor;
    /* 0x38 */ s8 unk_38;
} cmpPadStopWork_t;

typedef struct
{
    /* 0x0000 */ u8 unk_0000;
    /* 0x0001 */ s8 unk_0001;
    /* 0x0002 */ s8 unk_0002;
    /* 0x0003 */ u8 unk_0003;
    /* 0x0004 */ s8 unk_0004;
    /* 0x0005 */ s8 unk_0005;
    /* 0x0006 */ u8 unk_0006[2];
    /* 0x0008 */ s32 unk_0008;
    /* 0x000C */ u8 unk_000C[0x48];
    /* 0x0054 */ u16 unk_0054;
    /* 0x0056 */ u8 unk_0056[0xEA];
    /* 0x0140 */ cmpCursorWork_t listCursor;
    /* 0x0178 */ u8 unk_0178[0x44D0];
    /* 0x4648 */ s32 script;
    /* 0x464C */ s32 scriptArg;
    /* 0x4650 */ s32 unk_4650;
    /* 0x4654 */ s32 texHandle[2];
    /* 0x465C */ u8 unk_465C[0xE4];
    /* 0x4740 */ u8 seqMitama[4];
} ragUpdateWork_t;

extern ragUpdateWork_t *ragUpdateWork;

extern void fclRunPanelAnim(void *work);

typedef struct
{
    /* 0x00 */ s8 index;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ u16 id;
} cmbStockSlot_t;

typedef struct
{
    /* 0x00000 */ u8 unk_00000;
    /* 0x00001 */ s8 unk_00001;
    /* 0x00002 */ u8 unk_00002;
    /* 0x00003 */ u8 unk_00003;
    /* 0x00004 */ s8 unk_00004;
    /* 0x00005 */ s8 unk_00005;
    /* 0x00006 */ u8 unk_00006[2];
    /* 0x00008 */ s32 unk_00008;
    /* 0x0000C */ u8 unk_0000C[0x10];
    /* 0x0001C */ cmpCursorWork_t combineCursor;
    /* 0x00054 */ u8 unk_00054[0x40BC];
    /* 0x04110 */ cmbStockSlot_t stockSlot[3];
    /* 0x0411C */ u8 birthPanel[0x123EE];
    /* 0x1650A */ s8 unk_1650A;
    /* 0x1650B */ u8 unk_1650B[5];
    /* 0x16510 */ s32 script;
    /* 0x16514 */ s32 scriptArg;
    /* 0x16518 */ u8 unk_16518[4];
    /* 0x1651C */ void *explainData;
    /* 0x16520 */ u8 unk_16520[0x2C];
    /* 0x1654C */ u32 *unk_1654C[5];
    /* 0x16560 */ u8 unk_16560[0x70];
    /* 0x165D0 */ s32 smoke[3];
    /* 0x165DC */ u8 unk_165DC[0x1064];
    /* 0x17640 */ cmpCursorWork_t menuCursor;
    /* 0x17678 */ u8 unk_17678[0x3060];
    /* 0x1A6D8 */ s32 texHandle[7];
    /* 0x1A6F4 */ u8 unk_1A6F4[0x24];
    /* 0x1A718 */ u8 *table;
    /* 0x1A71C */ s32 unk_1A71C;
    /* 0x1A720 */ fclRbnWork_t *ribbon;
} cmbGbWork_t;

extern void cmpResetCursorInfo(cmpCursorWork_t *cursor);

extern void fclScriptForcedEnd(u8 *PID);

typedef struct
{
    /* 0x0000 */ u8 unk_0000[9];
    /* 0x0009 */ u8 unk_0009[0x17];
    /* 0x0020 */ s32 flags;
    /* 0x0024 */ u8 unk_0024[0x138];
    /* 0x015C */ cmpCursorWork_t listCursor;
} lstWork_t;

extern u8 ragHeartsTable[];

extern void cmpMoveCursorListEnd(cmpCursorWork_t *cursor);

extern s32 cmpGetPressedButton(s32 mask);

extern s32 cmpPlayCursorSE(s32 button);

extern s32 cmpChkSeqInfo(cmbGbWork_t *work);

typedef struct
{
    /* 0x00 */ u16 item;
    /* 0x02 */ s8 count;
    /* 0x03 */ u8 unk_03;
} ragItemReq_t;

s32 ragFindItemSlot(ragItemReq_t *req);

extern s32 fclScriptStart(s32 pScriptData, s32 StartNo, void *pPID);

void ragExit(void)
{
    cmpMoveCursorListEnd(&ragUpdateWork->listCursor);
    ragUpdateWork->unk_0005 = 1;
    ragUpdateWork->unk_0008 = fclScriptStart(ragUpdateWork->script, 0xC,
                                             &ragUpdateWork->scriptArg);
    ragUpdateWork->unk_0001 = 5;
}

INCLUDE_ASM(const s32, "facility/fclRagUpdate", ragChkItemErr);

INCLUDE_ASM(const s32, "facility/fclRagUpdate", ragCheckItemUse);

INCLUDE_ASM(const s32, "facility/fclRagUpdate", ragChkDevilLevelErr);

INCLUDE_ASM(const s32, "facility/fclRagUpdate", ragSetSeqPhase);

INCLUDE_ASM(const s32, "facility/fclRagUpdate", ragGetHeartsFlag);

extern s32 ragGetHeartsFlag(s8 index);

/* NON_MATCHING: 22 of 29 words match, 116 bytes against retail's 128; retail
 * restores $16 twice, once in a `b`'s delay slot, and merges only the `jr`. */
#ifdef NON_MATCHING
u16 ragCheckHeartsChoice(s8 index)
{
    s32 flag = ragGetHeartsFlag(index);

    if (index != 1) {
        if (index > 0) {
            if (index < 4) {
                return ragUpdateWork->unk_0054;
            }
        }
        return 0;
    }
    return *(u16 *)(ragHeartsTable + flag * 14);
}
#else
INCLUDE_ASM(const s32, "facility/fclRagUpdate", ragCheckHeartsChoice);
#endif

INCLUDE_ASM(const s32, "facility/fclRagUpdate", ragUpdateCallSeq);

INCLUDE_ASM(const s32, "facility/fclRagUpdate", ragUpdateSeqItem);

extern ragItemReq_t *ragGetConfirmEntry(s32 kind, u8 flag);

extern void ragSetSeqPhase(s32 phase);

extern s32 ragCheckItemUse(s32 item);

extern s32 cmpModelSetup(s32 a, s32 b);

extern void lstMoveCursor(lstWork_t *pListWindow, s8 Dir);

extern s32 cmpChkPadPressUpDown(void);

extern s32 cmpDisablePadBitStop(cmpPadStopWork_t *work, s32 bits, s32 stop);

void ragUpdateSeq2(void)
{
    s32 button = cmpGetPressedButton(0x33) & 0xFFFF;
    ragItemReq_t *entry = ragGetConfirmEntry(1, ragGetHeartsFlag(2));

    if (fclGetFlag(0) == 0) {
        return;
    }
    switch (button) {
    case 1:
        if (ragFindItemSlot(entry) == 0) {
            button = 0x8000;
            break;
        }
        ragSetSeqPhase(2);
        if (ragCheckItemUse(ragUpdateWork->unk_0054) != 0) {
            break;
        }
        fclScriptForcedEnd((u8 *)ragUpdateWork->scriptArg);
        cmpModelSetup(ragUpdateWork->unk_0054, 0);
        ragUpdateWork->unk_0001 = 7;
        ragUpdateWork->unk_0004 = button;
        break;
    case 2:
        fclScriptStart(ragUpdateWork->script, 0xA, &ragUpdateWork->scriptArg);
        cmpResetCursorInfo((cmpCursorWork_t *)((u8 *)ragUpdateWork + 0x2470));
        ragUpdateWork->unk_0001 = 0;
        ragUpdateWork->unk_0004 = button;
        break;
    case 0x10:
        lstMoveCursor((lstWork_t *)((u8 *)ragUpdateWork + 0x2314), -1);
        break;
    case 0x20:
        lstMoveCursor((lstWork_t *)((u8 *)ragUpdateWork + 0x2314), 1);
        break;
    }
    if (cmpChkPadPressUpDown() == 0) {
        lstMoveCursor((lstWork_t *)((u8 *)ragUpdateWork + 0x2314), 0);
    }
    cmpPlayCursorSE(
        cmpDisablePadBitStop((cmpPadStopWork_t *)((u8 *)ragUpdateWork + 0x2470), button, 0x30) &
        0xFFFF);
}

void ragUpdateSeq3(void)
{
    s32 button = cmpGetPressedButton(0x33) & 0xFFFF;
    ragItemReq_t *entry = ragGetConfirmEntry(2, ragGetHeartsFlag(3));

    if (fclGetFlag(0) == 0) {
        return;
    }
    switch (button) {
    case 1:
        if (ragFindItemSlot(entry) == 0) {
            button = 0x8000;
            break;
        }
        ragSetSeqPhase(3);
        if (ragCheckItemUse(ragUpdateWork->unk_0054) != 0) {
            break;
        }
        fclScriptForcedEnd((u8 *)ragUpdateWork->scriptArg);
        cmpModelSetup(ragUpdateWork->unk_0054, 0);
        ragUpdateWork->unk_0001 = 7;
        ragUpdateWork->unk_0004 = button;
        break;
    case 2:
        fclScriptStart(ragUpdateWork->script, 0xA, &ragUpdateWork->scriptArg);
        cmpResetCursorInfo((cmpCursorWork_t *)((u8 *)ragUpdateWork + 0x3608));
        ragUpdateWork->unk_0001 = 0;
        ragUpdateWork->unk_0004 = button;
        break;
    case 0x10:
        lstMoveCursor((lstWork_t *)((u8 *)ragUpdateWork + 0x34AC), -1);
        break;
    case 0x20:
        lstMoveCursor((lstWork_t *)((u8 *)ragUpdateWork + 0x34AC), 1);
        break;
    }
    if (cmpChkPadPressUpDown() == 0) {
        lstMoveCursor((lstWork_t *)((u8 *)ragUpdateWork + 0x34AC), 0);
    }
    cmpPlayCursorSE(
        cmpDisablePadBitStop((cmpPadStopWork_t *)((u8 *)ragUpdateWork + 0x3608), button, 0x30) &
        0xFFFF);
}

INCLUDE_ASM(const s32, "facility/fclRagUpdate", ragUpdateReturn);

INCLUDE_ASM(const s32, "facility/fclRagUpdate", ragUpdateSeqStatus);

INCLUDE_ASM(const s32, "facility/fclRagUpdate", ragUpdateSeqConfirm);

void ragUpdateSeqMitama(void)
{
    fclRunPanelAnim(ragUpdateWork->seqMitama);
}

INCLUDE_ASM(const s32, "facility/fclRagUpdate", ragUpdateSeqRoot);

void ragUpdateSeqTalk(void)
{
    s32 info;

    fclSetWorkValue(2, 0);
    info = cmpChkSeqInfo((cmbGbWork_t *)ragUpdateWork);
    if (info == 1) {
        return;
    }
    if (info != 2) {
        return;
    }
    fclSetWorkValue(2, 2);
    fclScriptStart(ragUpdateWork->script, 0xA, &ragUpdateWork->scriptArg);
    ragUpdateWork->unk_0001 = (u8)ragUpdateWork->unk_0002;
    ragUpdateWork->unk_0004 = -1;
}

extern s32 ragUpdateCallSeq(s32 SeqNo);

void ragUpdateSeqErr(void)
{
    s32 info = cmpChkSeqInfo((cmbGbWork_t *)ragUpdateWork);

    if (info == 1) {
        return;
    }
    if (info == 2) {
        ragUpdateCallSeq(ragUpdateWork->unk_0002);
        ragUpdateWork->unk_0004 = 1;
    }
}

INCLUDE_ASM(const s32, "facility/fclRagUpdate", ragUpdate);

s32 fclStartRecoverEvent(s8 value)
{
    fclSetWorkValue(1, value);
    fclClearFlag(1);
    return evtStartEvent(0x321);
}

INCLUDE_RODATA(const s32, "facility/fclRagUpdate", D_0053D2E8);

INCLUDE_RODATA(const s32, "facility/fclRagUpdate", D_0053D310);

INCLUDE_RODATA(const s32, "facility/fclRagUpdate", D_0053D338);

INCLUDE_RODATA(const s32, "facility/fclRagUpdate", D_0053D360);

INCLUDE_RODATA(const s32, "facility/fclRagUpdate", D_0053D370);

INCLUDE_RODATA(const s32, "facility/fclRagUpdate", D_0053D380);

