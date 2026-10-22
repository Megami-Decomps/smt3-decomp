#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"

extern s32 fclSetWorkValue(s8 index, s32 value);

s32 fclGetFlag(s8 index);

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
    /* 0x14 */ s32 ctr_act;
    /* 0x18 */ s32 ctr_deact;
    /* 0x1C */ s32 phase;
    /* 0x20 */ s32 count;
} fclRibbon_t;

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
    /* 0x0000 */ s8 unk_00;
    /* 0x0001 */ s8 unk_01;
    /* 0x0002 */ s8 unk_02;
    /* 0x0003 */ s8 unk_03;
    /* 0x0004 */ u8 unk_0004;
    /* 0x0005 */ s8 unk_0005;
    /* 0x0006 */ u8 unk_0006[2];
    /* 0x0008 */ s32 unk_0008;
    /* 0x000C */ u8 unk_000C[0x14];
    /* 0x0020 */ cmpCursorWork_t listCursor;
    /* 0x0058 */ u8 unk_0058[0x4474];
    /* 0x44CC */ s32 script;
    /* 0x44D0 */ s32 scriptArg;
} rcvUpdateWork_t;

extern rcvUpdateWork_t *rcvUpdateWork;

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
    /* 0x1A720 */ fclRibbon_t *ribbon;
} cmbGbWork_t;

typedef struct
{
    /* 0x0000 */ u8 unk_0000[9];
    /* 0x0009 */ u8 unk_0009[0x17];
    /* 0x0020 */ s32 flags;
    /* 0x0024 */ u8 unk_0024[0x138];
    /* 0x015C */ cmpCursorWork_t listCursor;
} lstWork_t;

extern void cmpMoveCursorListEnd(cmpCursorWork_t *cursor);

extern s32 cmpGetPressedButton(s32 mask);

extern s32 cmpPlayCursorSE(s32 button);

extern s32 cmpChkSeqInfo(cmbGbWork_t *work);

extern s32 fclScriptStart(s32 pScriptData, s32 StartNo, void *pPID);

extern s32 cmpDisablePadBitStop(cmpPadStopWork_t *work, s32 bits, s32 stop);

void rcvExit(void)
{
    cmpMoveCursorListEnd(&rcvUpdateWork->listCursor);
    rcvUpdateWork->unk_0005 = 1;
    rcvUpdateWork->unk_0008 = fclScriptStart(rcvUpdateWork->script, 7,
                                             &rcvUpdateWork->scriptArg);
    rcvUpdateWork->unk_01 = 7;
}

INCLUDE_ASM(const s32, "facility/fclRecoverUpdate", rcvUpdateData);

INCLUDE_ASM(const s32, "facility/fclRecoverUpdate", rcvUpdateCallSeq);

INCLUDE_ASM(const s32, "facility/fclRecoverUpdate", rcvUpdateSeqAllRecover);

extern s32 cmpDisablePadBitStop(cmpPadStopWork_t *work, s32 bits, s32 stop);

extern void lstMoveCursor2(lstWork_t *pListWindow, s8 Dir);

extern s32 rcvUpdateData(void);

extern s32 rcvUpdateCallSeq(s32 SeqNo);

void rcvUpdateSeqRecover(void)
{
    u32 button = cmpGetPressedButton(0x33);

    if (fclGetFlag(0) == 0) {
        return;
    }
    switch (button) {
    case 1:
        if (rcvUpdateData() == 0) {
            button = 0x8000;
        } else {
            rcvUpdateCallSeq(2);
            rcvUpdateWork->unk_0004 = button;
        }
        break;
    case 2:
        rcvUpdateCallSeq(0);
        rcvUpdateWork->unk_0004 = button;
        break;
    case 0x10:
        lstMoveCursor2((lstWork_t *)((u8 *)rcvUpdateWork + 0x1130), -1);
        break;
    case 0x20:
        lstMoveCursor2((lstWork_t *)((u8 *)rcvUpdateWork + 0x1130), 1);
        break;
    }
    lstMoveCursor2((lstWork_t *)((u8 *)rcvUpdateWork + 0x1130), 0);
    cmpPlayCursorSE(cmpDisablePadBitStop(
        (cmpPadStopWork_t *)((u8 *)rcvUpdateWork + 0x128C), button, 0x30));
}

void rcvUpdateSeqStatusRecover(void)
{
    u32 button = cmpGetPressedButton(0x33);

    if (fclGetFlag(0) == 0) {
        return;
    }
    switch (button) {
    case 1:
        if (rcvUpdateData() == 0) {
            button = 0x8000;
        } else {
            rcvUpdateCallSeq(3);
            rcvUpdateWork->unk_0004 = button;
        }
        break;
    case 2:
        rcvUpdateCallSeq(0);
        rcvUpdateWork->unk_0004 = button;
        break;
    case 0x10:
        lstMoveCursor2((lstWork_t *)((u8 *)rcvUpdateWork + 0x1130), -1);
        break;
    case 0x20:
        lstMoveCursor2((lstWork_t *)((u8 *)rcvUpdateWork + 0x1130), 1);
        break;
    }
    lstMoveCursor2((lstWork_t *)((u8 *)rcvUpdateWork + 0x1130), 0);
    cmpPlayCursorSE(cmpDisablePadBitStop(
        (cmpPadStopWork_t *)((u8 *)rcvUpdateWork + 0x128C), button, 0x30));
}

void rcvUpdateSeqRevive(void)
{
    u32 button = cmpGetPressedButton(0x33);

    if (fclGetFlag(0) == 0) {
        return;
    }
    switch (button) {
    case 1:
        if (rcvUpdateData() == 0) {
            button = 0x8000;
        } else {
            rcvUpdateCallSeq(4);
            rcvUpdateWork->unk_0004 = button;
        }
        break;
    case 2:
        rcvUpdateCallSeq(0);
        rcvUpdateWork->unk_0004 = button;
        break;
    case 0x10:
        lstMoveCursor2((lstWork_t *)((u8 *)rcvUpdateWork + 0x1130), -1);
        break;
    case 0x20:
        lstMoveCursor2((lstWork_t *)((u8 *)rcvUpdateWork + 0x1130), 1);
        break;
    }
    lstMoveCursor2((lstWork_t *)((u8 *)rcvUpdateWork + 0x1130), 0);
    cmpPlayCursorSE(cmpDisablePadBitStop(
        (cmpPadStopWork_t *)((u8 *)rcvUpdateWork + 0x128C), button, 0x30));
}

INCLUDE_ASM(const s32, "facility/fclRecoverUpdate", rcvUpdateSeqDispel);

s32 rcvIsHealPhase(void)
{
    if (rcvUpdateWork->unk_01 == 6) {
        if (rcvUpdateWork->unk_03 == 5) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "facility/fclRecoverUpdate", rcvUpdatePanelAnim);

INCLUDE_ASM(const s32, "facility/fclRecoverUpdate", rcvUpdateSeqRoot);

extern s32 rcvUpdateCallSeq(s32 SeqNo);

void rcvUpdateSeqTalk(void)
{
    s32 info;

    fclSetWorkValue(2, 0);
    info = cmpChkSeqInfo((cmbGbWork_t *)rcvUpdateWork);
    if (info == 1) {
        return;
    }
    if (info != 2) {
        return;
    }
    if (rcvUpdateWork->unk_01 == 6) {
        fclSetWorkValue(2, 2);
    }
    rcvUpdateCallSeq(rcvUpdateWork->unk_02);
    rcvUpdateWork->unk_0004 = -1;
}

INCLUDE_ASM(const s32, "facility/fclRecoverUpdate", rcvUpdate);

INCLUDE_RODATA(const s32, "facility/fclRecoverUpdate", D_0053D4F0);

INCLUDE_RODATA(const s32, "facility/fclRecoverUpdate", D_0053D518);

INCLUDE_RODATA(const s32, "facility/fclRecoverUpdate", D_0053D528);

INCLUDE_RODATA(const s32, "facility/fclRecoverUpdate", D_0053D538);

INCLUDE_SDATA(const s32, "facility/fclRecoverUpdate", D_0054FBF8);

