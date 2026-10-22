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
} cmpCursorInfo_t;

typedef struct
{
    /* 0x0000 */ u8 unk_0000[9];
    /* 0x0009 */ u8 unk_0009[0x17];
    /* 0x0020 */ s32 flags;
    /* 0x0024 */ u8 unk_0024[0x138];
    /* 0x015C */ cmpCursorInfo_t listCursor;
} lstWork_t;

extern void lstMoveCursor(lstWork_t *pListWindow, s8 Dir);

extern void lstInitListWindow(s32 X, s32 Y, s32 Z, s32 W, s32 H, s32 Col, s32 Style,
                              void *pListWindow);

extern void lstMoveCursor2(lstWork_t *pListWindow, s8 Dir);

extern void cmpDrawScrollBar(s32 a, s32 b, s32 c, s32 d, s32 e, s32 color, s32 g, s32 h,
                             cmpCursorInfo_t *cursor, s32 j, s32 k);

INCLUDE_ASM(const s32, "interface/lstListWindow", lstInitListWindow);

void func_002134E0(u8 *base, s32 mask, s32 index)
{
    base[index + 0x59] |= mask;
}

void func_002134F8(u8 *base, u8 mask, s32 index)
{
    base[index + 0x59] &= ~mask;
}

/* Called above its definition, where ee-gcc assumed int. */
s32 cmpMoveCursor(cmpCursorInfo_t *cur, s8 dir);

void lstMoveCursor(lstWork_t *pListWindow, s8 Dir)
{
    s32 first;
    s32 pos;

    first = pListWindow->listCursor.unk_22 + (u8)pListWindow->listCursor.unk_24;
    if (Dir == 0) {
        cmpMoveCursor(&pListWindow->listCursor, 0);
        return;
    }
    while (cmpMoveCursor(&pListWindow->listCursor, Dir) != 0) {
        pos = pListWindow->listCursor.unk_22 + (u8)pListWindow->listCursor.unk_24;
        if (pos == first) {
            break;
        }
        if ((pListWindow->unk_0009[pos + 0x50] & 1) != 0) {
            break;
        }
    }
}

/* Called above its definition, where ee-gcc assumed int. */
s32 cmpMoveCursor2(cmpCursorInfo_t *cursor, s8 dir);

void lstMoveCursor2(lstWork_t *pListWindow, s8 Dir)
{
    s32 first;
    s32 pos;

    first = pListWindow->listCursor.unk_22 + (u8)pListWindow->listCursor.unk_24;
    if (Dir == 0) {
        cmpMoveCursor2(&pListWindow->listCursor, 0);
        return;
    }
    while (cmpMoveCursor2(&pListWindow->listCursor, Dir) != 0) {
        pos = pListWindow->listCursor.unk_22 + (u8)pListWindow->listCursor.unk_24;
        if (pos == first) {
            break;
        }
        if ((pListWindow->unk_0009[pos + 0x50] & 1) != 0) {
            break;
        }
    }
}

void lstDrawUpDownCursor(lstWork_t *work)
{
    s32 flags = work->flags & 0xFFFF0000;

    if (work->listCursor.unk_22 != 0) {
        flags |= 1;
    }
    if ((work->listCursor.unk_22 + work->listCursor.unk_26) != work->listCursor.unk_20) {
        flags |= 2;
    }
    work->flags = flags;
}

INCLUDE_ASM(const s32, "interface/lstListWindow", lstDrawListWindow);

typedef struct
{
    /* 0x0000 */ s32 x;
    /* 0x0004 */ s32 y;
    /* 0x0008 */ s32 unk_08;
    /* 0x000C */ u8 unk_0C[0x158];
    /* 0x0164 */ s32 unk_164;
    /* 0x0168 */ s32 unk_168;
    /* 0x016C */ s32 unk_16C;
    /* 0x0170 */ s32 unk_170;
} lstWindowWork_t;

void lstDrawListWindowCursor(lstWindowWork_t *pListWindow, s32 OtNo)
{
    cmpDrawScrollBar(pListWindow->x + pListWindow->unk_164 * 16, pListWindow->y + pListWindow->unk_168 * 8,
                     pListWindow->unk_08, pListWindow->unk_16C, pListWindow->unk_170, 0x80808080, 0x100, 0,
                     (cmpCursorInfo_t *)((u8 *)pListWindow + 0x15C), 0, OtNo);
}

extern void lstMoveCursor(lstWork_t *pListWindow, s8 Dir);

s32 lstPageUp(lstWork_t *plistwin)
{
    cmpCursorInfo_t *cursor = &plistwin->listCursor;
    s32 count = cursor->unk_26;
    s32 i;

    for (i = 0; i < count; i++) {
        if (cursor->StopFlag != 0) {
            break;
        }
        lstMoveCursor(plistwin, -1);
    }
    return i;
}

s32 lstPageDown(lstWork_t *plistwin)
{
    cmpCursorInfo_t *cursor = &plistwin->listCursor;
    s32 count = cursor->unk_26;
    s32 i;

    for (i = 0; i < count; i++) {
        if (cursor->StopFlag != 0) {
            break;
        }
        lstMoveCursor(plistwin, 1);
    }
    return i;
}

extern s32 datGetItemSlotIndex(s32 id);

extern void datAddHearts(s32 id);

INCLUDE_ASM(const s32, "interface/lstListWindow", datGetItemSlotIndex);

INCLUDE_RODATA(const s32, "interface/lstListWindow", D_0053D6E8);

INCLUDE_RODATA(const s32, "interface/lstListWindow", D_0053D708);

