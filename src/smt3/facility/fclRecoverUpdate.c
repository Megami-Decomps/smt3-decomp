#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"
#include "kernel.h"

extern s32 fclSetWorkValue(s8 index, s32 value);

s32 fclGetFlag(s8 index);

typedef struct
{
    u8 animState;
    u8 unk_01[3];
    s32 target;
    u8 unk_08[0xC];
    s32 phase;
    s32 phaseStep;
    s32 unk_1C;
    void (*calcSizes)();
} fclPanelAnim_t;

extern void fclSetPanelAnimState(fclPanelAnim_t *anim, u8 state);
extern s32 fclRunPanelAnim(fclPanelAnim_t *anim);

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
    /* 0x0000 */ s8 unk_00;
    /* 0x0001 */ s8 unk_01;
    /* 0x0002 */ s8 unk_02;
    /* 0x0003 */ s8 unk_03;
    /* 0x0004 */ u8 unk_0004;
    /* 0x0005 */ s8 unk_0005;
    /* 0x0006 */ u8 unk_0006[2];
    /* 0x0008 */ s32 unk_0008;
    /* 0x000C */ u8 unk_000C[0x14];
    /* 0x0020 */ cmpCursorInfo_t listCursor;
    /* 0x0058 */ u8 unk_0058[0x1004];
    /* 0x105C */ s32 recoverPrices[16];
    /* 0x109C */ u8 reserved109C[0x24];
    /* 0x10C0 */ s8 partyCount;
    /* 0x10C1 */ s8 reserveCount;
    /* 0x10C2 */ u8 reserved10C2;
    /* 0x10C3 */ s8 targetStock[16];
    /* 0x10D3 */ u8 reserved10D3[0x33F9];
    /* 0x44CC */ s32 script;
    /* 0x44D0 */ s32 scriptArg;
    /* 0x44D4 */ u8 unk_44D4[0x4C];
    /* 0x4520 */ fclPanelAnim_t panelAnim;
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

typedef struct
{
    /* 0x0000 */ u8 unk_0000[9];
    /* 0x0009 */ u8 unk_0009[0x17];
    /* 0x0020 */ s32 flags;
    /* 0x0024 */ u8 unk_0024[0x138];
    /* 0x015C */ cmpCursorInfo_t listCursor;
} lstWork_t;

extern void cmpMoveCursorListEnd(cmpCursorInfo_t *cursor);

extern s32 cmpGetPressedButton(s32 mask);

extern void cmpPlayCursorSE(u32 button);

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

extern s32 rcvAppAllRecover(datUnitWork_t *work);
extern void rcvClearStatusBits(datUnitWork_t *work);
extern void rcvReviveUnit(datUnitWork_t *work);
extern void rcvClearDeadFlag(datUnitWork_t *work);
extern void sfply_AdjustPrepEnd(datUnitWork_t *work);
extern void p2sdPlaySE(s32 trackId, s32 volume, s32 pan);

s32 rcvUpdateData(void)
{
    rcvUpdateWork_t *work = rcvUpdateWork;
    s32 slot;
    datUnitWork_t *unit;
    s32 price;
    s32 maka;

    if (*((u8 *)work + 0x10BC) != 0) {
        s8 unitIndex;

        slot = (s8)(*((u8 *)work + 0x12AE) + *((u8 *)work + 0x12B0));
        unitIndex = work->targetStock[slot];
        unit = (datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54) + unitIndex;
    } else {
        slot = 0;
        unit = (datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54);
    }

    price = *(s32 *)((u8 *)work + 0x105C + slot * 4);
    maka = DDS3_GBWK->maka;
    if (maka < price) {
        return 0;
    }
    DDS3_GBWK->maka = maka - price;

    switch (*((u8 *)work + 0x10BC)) {
    case 1:
        rcvAppAllRecover(unit);
        break;
    case 2:
        rcvClearStatusBits(unit);
        break;
    case 4:
        rcvReviveUnit(unit);
        p2sdPlaySE(0x300002, 0x7F, 0x3F);
        break;
    case 7:
        sfply_AdjustPrepEnd(unit);
        p2sdPlaySE(0x300001, 0x7F, 0x3F);
        break;
    default:
        rcvClearDeadFlag(unit);
        p2sdPlaySE(0x300003, 0x7F, 0x3F);
        break;
    }
    return 1;
}

extern void rcvCreateLocalStock(void *stock, s8 mask);

s32 rcvUpdateCallSeq(s8 sequence)
{
    s32 check = 0;
    s32 notice = 0;
    s32 script = 0;

    switch (sequence) {
    case 0:
        script = 5;
        check = 2;
        break;
    case 1:
        script = 1;
        notice = 8;
        rcvCreateLocalStock((u8 *)rcvUpdateWork + 0x10BE, 7);
        break;
    case 2:
        script = 1;
        rcvCreateLocalStock((u8 *)rcvUpdateWork + 0x10BE, 1);
        break;
    case 3:
        script = 1;
        rcvCreateLocalStock((u8 *)rcvUpdateWork + 0x10BE, 2);
        break;
    case 4:
        script = 3;
        notice = 10;
        rcvCreateLocalStock((u8 *)rcvUpdateWork + 0x10BE, 4);
        break;
    case 5:
        script = 2;
        notice = 9;
        check = 1;
        *((u8 *)rcvUpdateWork + 0x10BC) = 0;
        break;
    case 6:
        fclSetWorkValue(2, 1);
        rcvUpdateWork->unk_0005 = 1;
        rcvUpdateWork->unk_0008 = fclScriptStart(rcvUpdateWork->script, 6,
                                                  &rcvUpdateWork->scriptArg);
        rcvUpdateWork->unk_01 = 6;
        rcvUpdateWork->unk_03 = 6;
        rcvUpdateWork->unk_0004 = -1;
        rcvUpdateWork->unk_02 = 0;
        return 0;
    case 7:
        rcvExit();
        check = 2;
        break;
    }
    switch (check) {
    case 0:
        if (rcvUpdateWork->partyCount + rcvUpdateWork->reserveCount == 0) {
            if (rcvUpdateWork->unk_01 != 0) {
                rcvUpdateCallSeq(0);
                return 0;
            }
            rcvUpdateWork->unk_0005 = 1;
            rcvUpdateWork->unk_0008 = fclScriptStart(rcvUpdateWork->script, notice,
                                                      &rcvUpdateWork->scriptArg);
            rcvUpdateWork->unk_01 = 6;
            rcvUpdateWork->unk_03 = 6;
            rcvUpdateWork->unk_0004 = -1;
            rcvUpdateWork->unk_02 = 0;
            return 0;
        }
        break;
    case 1:
        if (!(DDS3_GBWK->akuma[0].badstatus & 0x8000)) {
            if (rcvUpdateWork->unk_01 != 0) {
                rcvUpdateCallSeq(0);
                return 0;
            }
            rcvUpdateWork->unk_0005 = 1;
            rcvUpdateWork->unk_0008 = fclScriptStart(rcvUpdateWork->script, notice,
                                                      &rcvUpdateWork->scriptArg);
            rcvUpdateWork->unk_01 = 6;
            rcvUpdateWork->unk_03 = 6;
            rcvUpdateWork->unk_0004 = -1;
            rcvUpdateWork->unk_02 = 0;
            return 0;
        }
        break;
    }
    if (rcvUpdateWork->unk_01 != sequence) {
        fclScriptStart(rcvUpdateWork->script, script, &rcvUpdateWork->scriptArg);
    }
    rcvUpdateWork->unk_01 = sequence;
    return 1;
}

extern void lstMoveCursor2(lstWork_t *pListWindow, s8 Dir);
extern s32 rcvUpdateData(void);
extern s32 rcvUpdateCallSeq(s8 SeqNo);

void rcvUpdateSeqAllRecover(void)
{
    u32 button = cmpGetPressedButton(0x33);

    if (fclGetFlag(0) == 0) {
        return;
    }
    switch (cmpChkSeqInfo((cmbGbWork_t *)rcvUpdateWork)) {
    case 1:
        return;
    case 2:
        rcvUpdateCallSeq(0);
        rcvUpdateWork->unk_0004 = 1;
        return;
    }
    switch (button) {
    case 1:
        if (rcvUpdateData() == 0) {
            button = 0x8000;
        } else {
            rcvUpdateCallSeq(1);
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

extern s32 cmpDisablePadBitStop(cmpPadStopWork_t *work, s32 bits, s32 stop);




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

extern s32 dds3GetProcessStat(void *id);

s32 rcvUpdateSeqDispel(s32 arg)
{
    if (dds3GetProcessStat((void *)rcvUpdateWork->scriptArg) == 2) {
        return 0;
    }
    switch (fclGetFlag(2)) {
    case 0:
        if (rcvUpdateData() == 0) {
            rcvUpdateWork->unk_0005 = 1;
            rcvUpdateWork->unk_0008 = fclScriptStart(rcvUpdateWork->script, 11,
                                                    &rcvUpdateWork->scriptArg);
            rcvUpdateWork->unk_01 = 6;
            rcvUpdateWork->unk_03 = 5;
            rcvUpdateWork->unk_0004 = -1;
            rcvUpdateWork->unk_02 = 0;
        } else {
            rcvUpdateWork->unk_0005 = 1;
            rcvUpdateWork->unk_0008 = fclScriptStart(rcvUpdateWork->script, 4,
                                                    &rcvUpdateWork->scriptArg);
            rcvUpdateWork->unk_01 = 6;
            rcvUpdateWork->unk_03 = 6;
            rcvUpdateWork->unk_0004 = -1;
            rcvUpdateWork->unk_02 = 0;
        }
        break;
    case 1:
        rcvUpdateCallSeq(rcvUpdateWork->unk_03);
        rcvUpdateWork->unk_0004 = 2;
        break;
    case 2:
        break;
    }
    return 1;
}

s32 rcvIsHealPhase(void)
{
    if (rcvUpdateWork->unk_01 == 6) {
        if (rcvUpdateWork->unk_03 == 5) {
            return 1;
        }
    }
    return 0;
}

void rcvUpdatePanelAnim(void)
{
    s32 active;

    switch (rcvUpdateWork->unk_01) {
    case 1:
    case 2:
    case 3:
    case 4:
        if (fclGetFlag(0) == 0) {
            return;
        }
        /* Fall through to the inactive panel state. */
    case 5:
        active = 0;
        break;
    default:
        active = 1;
        break;
    }
    if (rcvIsHealPhase()) {
        active = 0;
    }
    if (active == 0) {
        fclSetPanelAnimState(&rcvUpdateWork->panelAnim, 2);
    } else {
        fclSetPanelAnimState(&rcvUpdateWork->panelAnim, 1);
    }
    fclRunPanelAnim(&rcvUpdateWork->panelAnim);
}


extern s32 rcvRootHasItems(void *work);
extern s32 cmpMoveCursor(cmpCursorInfo_t *cursor, s8 direction);
extern s32 cmpChkPadPressUpDown(void);
extern s32 printf(const char *fmt, ...);

void rcvUpdateSeqRoot(void)
{
    u32 button = cmpGetPressedButton(0x33);
    s8 selection = (u8)rcvUpdateWork->listCursor.unk_22 +
                   (u8)rcvUpdateWork->listCursor.unk_24 + 1;
    s32 i;

    fclSetWorkValue(2, 0);
    if (fclGetFlag(0) == 0) {
        return;
    }
    switch (button) {
    case 1:
        if (rcvRootHasItems(&rcvUpdateWork->listCursor) == 0) {
            button = 0x8000;
        } else {
            rcvUpdateCallSeq(selection);
            rcvUpdateWork->unk_0004 = button;
            for (i = 0; i < rcvUpdateWork->partyCount; i++) {
                printf("P : %d %d\n", rcvUpdateWork->targetStock[i],
                       rcvUpdateWork->recoverPrices[i]);
            }
            for (i = 0; i < rcvUpdateWork->reserveCount; i++) {
                printf("S : %d %d\n",
                       rcvUpdateWork->targetStock[rcvUpdateWork->partyCount + i],
                       rcvUpdateWork->recoverPrices[i]);
            }
            printf("\n");
        }
        break;
    case 2:
        rcvExit();
        break;
    case 0x10:
        cmpMoveCursor(&rcvUpdateWork->listCursor, -1);
        break;
    case 0x20:
        cmpMoveCursor(&rcvUpdateWork->listCursor, 1);
        break;
    }
    if (cmpChkPadPressUpDown() == 0) {
        cmpMoveCursor(&rcvUpdateWork->listCursor, 0);
    }
    cmpPlayCursorSE(cmpDisablePadBitStop(
        (cmpPadStopWork_t *)&rcvUpdateWork->listCursor, button, 0x30));
}

extern s32 rcvUpdateCallSeq(s8 SeqNo);

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

extern void *dds3GetProcessData(dds3ProcessID_t *id);
extern void fclClearFlag(s8 index);
extern s32 cmpUpdateSeqInfo(void *work, s8 previous);

s32 rcvUpdate(dds3ProcessID_t *id)
{
    s8 previous;
    u8 flags;

    rcvUpdateWork = dds3GetProcessData(id);
    previous = rcvUpdateWork->unk_01;
    rcvUpdatePanelAnim();
    if (fclGetFlag(1) == 0) {
        return 0;
    }
    flags = *((u8 *)rcvUpdateWork);
    if (flags & 0x40) {
        return -1;
    }
    if ((flags & 2) && (flags & 1)) {
        switch (rcvUpdateWork->unk_01) {
        case 0:
            rcvUpdateSeqRoot();
            break;
        case 1:
            rcvUpdateSeqAllRecover();
            break;
        case 2:
            rcvUpdateSeqRecover();
            break;
        case 3:
            rcvUpdateSeqStatusRecover();
            break;
        case 4:
            rcvUpdateSeqRevive();
            break;
        case 5:
            if (rcvUpdateSeqDispel(0) == 0) {
                return 0;
            }
            break;
        case 6:
            rcvUpdateSeqTalk();
            break;
        case 7:
            if (cmpChkSeqInfo((cmbGbWork_t *)rcvUpdateWork) == 2) {
                fclClearFlag(1);
                fclSetWorkValue(1, 0);
                rcvUpdateWork->unk_00 |= 0x10;
            }
            break;
        }
    }
    cmpUpdateSeqInfo(rcvUpdateWork, previous);
    return 0;
}

INCLUDE_RODATA(const s32, "facility/fclRecoverUpdate", D_0053D4F0);

INCLUDE_RODATA(const s32, "facility/fclRecoverUpdate", D_0053D518);

INCLUDE_RODATA(const s32, "facility/fclRecoverUpdate", D_0053D528);

INCLUDE_RODATA(const s32, "facility/fclRecoverUpdate", D_0053D538);

