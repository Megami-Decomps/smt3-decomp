#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"
#include "kernel.h"

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
    /* 0x0C */ s32 stat;
    /* 0x10 */ s16 act;
    /* 0x12 */ u16 deact;
    /* 0x14 */ s32 ctr_act;
    /* 0x18 */ s32 ctr_deact;
    /* 0x1C */ s32 ctr;
    /* 0x20 */ s32 num;
} fclRibbon_t;

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
    /* 0x27 */ u8 unk_27[5];
    /* 0x2C */ s32 unk_2C;
    /* 0x30 */ s32 unk_30;
    /* 0x34 */ s32 unk_34;
} cmpCursorInfo_t;

typedef struct
{
    /* 0x00 */ cmpCursorInfo_t cursor;
    /* 0x38 */ s8 unk_38;
} cmpPadStopWork_t;

typedef struct
{
    u8 reserved00000[0x15C];
    cmpCursorInfo_t listCursor;
} ragListWindow_t;

typedef struct
{
    /* 0x0000 */ u8 unk_0000;
    /* 0x0001 */ s8 unk_0001;
    /* 0x0002 */ s8 unk_0002;
    /* 0x0003 */ s8 unk_0003;
    /* 0x0004 */ s8 unk_0004;
    /* 0x0005 */ s8 unk_0005;
    /* 0x0006 */ u8 unk_0006[2];
    /* 0x0008 */ s32 unk_0008;
    /* 0x000C */ u8 unk_000C[0x0D];
    /* 0x0019 */ s8 ragType;
    /* 0x001A */ s8 heartItems[0x20];
    /* 0x003A */ s8 heartItemCount;
    /* 0x003B */ s8 itemList2[8];
    /* 0x0043 */ s8 itemList2Count;
    /* 0x0044 */ s8 itemList3[8];
    /* 0x004C */ s8 itemList3Count;
    /* 0x004D */ u8 reserved004D[3];
    /* 0x0050 */ datUnitWork_t previewUnit;
    /* 0x013C */ u8 reserved013C[4];
    /* 0x0140 */ cmpCursorInfo_t listCursor;
    /* 0x0178 */ u8 unk_0178[0x1004];
    /* 0x117C */ ragListWindow_t heartListWindow;
    /* 0x1310 */ u8 reserved1310[0x1004];
    /* 0x2314 */ ragListWindow_t itemList2Window;
    /* 0x24A8 */ u8 reserved24A8[0x1004];
    /* 0x34AC */ ragListWindow_t itemList3Window;
    /* 0x3640 */ u8 reserved3640[0x1008];
    /* 0x4648 */ s32 script;
    /* 0x464C */ s32 scriptArg;
    /* 0x4650 */ s32 unk_4650;
    /* 0x4654 */ s32 texHandle[2];
    /* 0x465C */ u8 unk_465C[0xE4];
    /* 0x4740 */ u8 seqMitama[4];
} ragUpdateWork_t;

extern ragUpdateWork_t *ragUpdateWork;

typedef struct
{
    u8 unk_00000[0xA50];
    u8 maxstock;
    u8 unk_00A51[3];
    datUnitWork_t unitwork[16];
    u8 unk_01914[0x14];
    u8 item[0xA0];
} ragGlobalWorkData_t;

extern ragGlobalWorkData_t *ragGlobalWork asm("DDS3_GBWK");

extern s32 fclRunPanelAnim(void *work);

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
    /* 0x0001C */ cmpCursorInfo_t combineCursor;
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
    /* 0x17640 */ cmpCursorInfo_t menuCursor;
    /* 0x17678 */ u8 unk_17678[0x3060];
    /* 0x1A6D8 */ s32 texHandle[7];
    /* 0x1A6F4 */ u8 unk_1A6F4[0x24];
    /* 0x1A718 */ u8 *table;
    /* 0x1A71C */ s32 unk_1A71C;
    /* 0x1A720 */ fclRibbon_t *ribbon;
} cmbGbWork_t;

extern void cmpResetCursorInfo(cmpCursorInfo_t *cursor);

extern void fclScriptForcedEnd(u8 *PID);

typedef struct
{
    /* 0x0000 */ u8 unk_0000[9];
    /* 0x0009 */ u8 unk_0009[0x17];
    /* 0x0020 */ s32 flags;
    /* 0x0024 */ u8 unk_0024[0x138];
    /* 0x015C */ cmpCursorInfo_t listCursor;
} lstWork_t;

extern u8 ragHeartsTable[];

extern void cmpMoveCursorListEnd(cmpCursorInfo_t *cursor);

extern s32 cmpGetPressedButton(s32 mask);

extern s32 cmpPlayCursorSE(s32 button);

extern s32 cmpChkSeqInfo(cmbGbWork_t *work);

typedef struct
{
    /* 0x00 */ u16 item;
    /* 0x02 */ s8 count;
    /* 0x03 */ u8 unk_03;
} ragItemReq_t;

typedef struct
{
    datUnitWork_t unit;
} cmbUnit_t;

s8 cmbFindStockSlot(cmbUnit_t *list, u8 count);

s8 ragUseItemOnUnit(u16 id, datUnitWork_t *unit, u8 count);

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

s8 ragChkItemErr(u16 itemId)
{
    if (ragGlobalWork->item[itemId] >= 0x63) {
        ragUpdateWork->unk_0005 = 1;
        ragUpdateWork->unk_0008 = fclScriptStart(ragUpdateWork->script, 4,
                                                 &ragUpdateWork->scriptArg);
        ragUpdateWork->unk_0002 = ragUpdateWork->unk_0001;
        ragUpdateWork->unk_0003 = ragUpdateWork->unk_0001;
        ragUpdateWork->unk_0001 = 6;
        ragUpdateWork->unk_0004 = -1;
        return 1;
    }
    return 0;
}

s32 ragCheckItemUse(u16 itemId)
{
    if (cmbFindStockSlot((cmbUnit_t *)&ragGlobalWork->unitwork[4],
                         ragGlobalWork->maxstock) < 0) {
        ragUpdateWork->unk_0005 = 1;
        ragUpdateWork->unk_0008 = fclScriptStart(ragUpdateWork->script, 6,
                                                 &ragUpdateWork->scriptArg);
        ragUpdateWork->unk_0002 = ragUpdateWork->unk_0001;
        ragUpdateWork->unk_0003 = ragUpdateWork->unk_0001;
        ragUpdateWork->unk_0001 = 6;
        ragUpdateWork->unk_0004 = -1;
        return 1;
    }

    if (ragUseItemOnUnit(itemId, ragGlobalWork->unitwork, 16) >= 0) {
        ragUpdateWork->unk_0005 = 1;
        ragUpdateWork->unk_0008 = fclScriptStart(ragUpdateWork->script, 5,
                                                 &ragUpdateWork->scriptArg);
        ragUpdateWork->unk_0002 = ragUpdateWork->unk_0001;
        ragUpdateWork->unk_0003 = ragUpdateWork->unk_0001;
        ragUpdateWork->unk_0001 = 6;
        ragUpdateWork->unk_0004 = -1;
        return 2;
    }
    return 0;
}

s8 ragChkDevilLevelErr(datUnitWork_t *stock)
{
    if (ragGlobalWork->unitwork[0].level < stock->level) {
        ragUpdateWork->unk_0005 = 1;
        ragUpdateWork->unk_0008 = fclScriptStart(ragUpdateWork->script, 7,
                                                 &ragUpdateWork->scriptArg);
        ragUpdateWork->unk_0002 = ragUpdateWork->unk_0003;
        ragUpdateWork->unk_0003 = ragUpdateWork->unk_0001;
        ragUpdateWork->unk_0001 = 6;
        ragUpdateWork->unk_0004 = -1;
        return 1;
    } else {
        return 0;
    }
}

typedef struct {
    u16 devilId;
    s8 custom;
    u8 reserved03;
    u8 level;
    u8 reserved05[5];
    u8 params[6];
    u16 skills[8];
    u8 reserved20[12];
} ragOffer_t;
extern void ragSetSeqPhase2(ragOffer_t *offer, datUnitWork_t *unit);

void ragSetSeqPhase(s8 phase)
{
    s8 index;
    switch (phase) {
    case 2:
        index = (u8)ragUpdateWork->itemList2Window.listCursor.unk_22 +
                (u8)ragUpdateWork->itemList2Window.listCursor.unk_24;
        ragSetSeqPhase2(&((ragOffer_t *)(ragHeartsTable + 0x160))[
            (u8)ragUpdateWork->itemList2[index]], &ragUpdateWork->previewUnit);
        return;
    case 3:
        index = (u8)ragUpdateWork->itemList3Window.listCursor.unk_22 +
                (u8)ragUpdateWork->itemList3Window.listCursor.unk_24;
        ragSetSeqPhase2(&((ragOffer_t *)(ragHeartsTable + 0x500))[
            (u8)ragUpdateWork->itemList3[index]], &ragUpdateWork->previewUnit);
        break;
    }
}

s32 ragGetHeartsFlag(s8 index)
{
    s32 flag;

    flag = 0;
    switch (index) {
    case 1: {
        s32 entry = ragUpdateWork->heartListWindow.listCursor.unk_22 +
                    (u8)ragUpdateWork->heartListWindow.listCursor.unk_24;
        flag = ragUpdateWork->heartItems[entry];
        break;
    }
    case 2: {
        s32 entry = ragUpdateWork->itemList2Window.listCursor.unk_22 +
                    (u8)ragUpdateWork->itemList2Window.listCursor.unk_24;
        flag = ragUpdateWork->itemList2[entry];
        break;
    }
    case 3: {
        s32 entry = ragUpdateWork->itemList3Window.listCursor.unk_22 +
                    (u8)ragUpdateWork->itemList3Window.listCursor.unk_24;
        flag = ragUpdateWork->itemList3[entry];
        break;
    }
    }
    return flag;
}
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
                return ragUpdateWork->previewUnit.id;
            }
        }
        return 0;
    }
    return *(u16 *)(ragHeartsTable + flag * 14);
}
#else
INCLUDE_ASM(const s32, "facility/fclRagUpdate", ragCheckHeartsChoice);
#endif

extern void ragCreateAllList(s32 type, s32 count, void *work);

s32 ragUpdateCallSeq(s32 SeqNo)
{
    u32 phase;

    ragCreateAllList(1, ragUpdateWork->ragType, ragUpdateWork);
    phase = (s32)ragUpdateWork->listCursor.unk_22 +
            (u8)ragUpdateWork->listCursor.unk_24;
    if (phase < 5) {
        switch (phase) {
        case 0:
            fclScriptStart(ragUpdateWork->script, 1, &ragUpdateWork->scriptArg);
            ragUpdateWork->unk_0001 = 1;
            ragUpdateWork->unk_0004 = 1;
            break;
        case 1:
            fclScriptStart(ragUpdateWork->script, 2, &ragUpdateWork->scriptArg);
            ragUpdateWork->unk_0001 = 2;
            ragUpdateWork->unk_0004 = 1;
            break;
        case 2:
            fclScriptStart(ragUpdateWork->script, 3, &ragUpdateWork->scriptArg);
            ragUpdateWork->unk_0001 = 3;
            ragUpdateWork->unk_0004 = 1;
            break;
        case 3:
            fclSetWorkValue(2, 1);
            ragUpdateWork->unk_0005 = 1;
            ragUpdateWork->unk_0008 = fclScriptStart(ragUpdateWork->script, 0xB,
                                                       &ragUpdateWork->scriptArg);
            ragUpdateWork->unk_0001 = 4;
            ragUpdateWork->unk_0003 = 4;
            ragUpdateWork->unk_0004 = -1;
            ragUpdateWork->unk_0002 = 0;
            break;
        case 4:
            ragExit();
            break;
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "facility/fclRagUpdate", ragUpdateSeqItem);

extern ragItemReq_t *ragGetConfirmEntry(s32 kind, u8 flag);

extern void ragSetSeqPhase(s8 phase);

extern s32 ragCheckItemUse(u16 item);

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
        if (ragCheckItemUse(ragUpdateWork->previewUnit.id) != 0) {
            break;
        }
        fclScriptForcedEnd((u8 *)ragUpdateWork->scriptArg);
        cmpModelSetup(ragUpdateWork->previewUnit.id, 0);
        ragUpdateWork->unk_0001 = 7;
        ragUpdateWork->unk_0004 = button;
        break;
    case 2:
        fclScriptStart(ragUpdateWork->script, 0xA, &ragUpdateWork->scriptArg);
        cmpResetCursorInfo((cmpCursorInfo_t *)((u8 *)ragUpdateWork + 0x2470));
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
        if (ragCheckItemUse(ragUpdateWork->previewUnit.id) != 0) {
            break;
        }
        fclScriptForcedEnd((u8 *)ragUpdateWork->scriptArg);
        cmpModelSetup(ragUpdateWork->previewUnit.id, 0);
        ragUpdateWork->unk_0001 = 7;
        ragUpdateWork->unk_0004 = button;
        break;
    case 2:
        fclScriptStart(ragUpdateWork->script, 0xA, &ragUpdateWork->scriptArg);
        cmpResetCursorInfo((cmpCursorInfo_t *)((u8 *)ragUpdateWork + 0x3608));
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

void ragUpdateReturn(void)
{
    s32 last = ragUpdateWork->unk_0003;

    switch (last) {
    case 1:
        fclScriptStart(ragUpdateWork->script, 1, &ragUpdateWork->scriptArg);
        ragUpdateWork->unk_0001 = last;
        break;
    case 2:
        fclScriptStart(ragUpdateWork->script, 2, &ragUpdateWork->scriptArg);
        ragUpdateWork->unk_0001 = last;
        break;
    case 3:
        fclScriptStart(ragUpdateWork->script, 3, &ragUpdateWork->scriptArg);
        ragUpdateWork->unk_0001 = last;
        break;
    }
    ragUpdateWork->unk_0004 = 1;
}

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

extern void *dds3GetProcessData(dds3ProcessID_t *id);
extern s32 fclChkDbgProcess(void);
extern void ragUpdateSeqRoot(void);
extern void ragUpdateSeqItem(void);
extern void ragUpdateSeqStatus(void);
extern s32 ragUpdateSeqConfirm(void);
extern s32 cmpUpdateSeqInfo(void *work, s8 previous);

s32 ragUpdate(dds3ProcessID_t *id)
{
    s8 previous;
    u8 flags;
    ragUpdateWork = dds3GetProcessData(id);
    previous = ragUpdateWork->unk_0001;
    if (fclChkDbgProcess() != 0) return 0;
    ragUpdateSeqMitama();
    if (fclGetFlag(1) == 0) return 0;
    flags = ragUpdateWork->unk_0000;
    if (flags & 0x40) return -1;
    if ((flags & 2) && (flags & 1)) {
        switch (ragUpdateWork->unk_0001) {
        case 0: ragUpdateSeqRoot(); break;
        case 1: ragUpdateSeqItem(); break;
        case 2: ragUpdateSeq2(); break;
        case 3: ragUpdateSeq3(); break;
        case 4: ragUpdateSeqTalk(); break;
        case 5:
            if (cmpChkSeqInfo((cmbGbWork_t *)ragUpdateWork) == 2) {
                fclClearFlag(1);
                fclSetWorkValue(1, 0);
                ragUpdateWork->unk_0000 |= 0x10;
            }
            break;
        case 6: ragUpdateSeqErr(); break;
        case 7: ragUpdateSeqStatus(); break;
        case 8:
            if (ragUpdateSeqConfirm() == 0) return 0;
            break;
        }
    }
    cmpUpdateSeqInfo(ragUpdateWork, previous);
    return 0;
}

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

