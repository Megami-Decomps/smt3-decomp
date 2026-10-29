#include "common.h"
#include "interface.h"
#include "camp.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"

extern void lstMoveCursor(lstWork_t *pListWindow, s8 Dir);

extern void lstInitListWindow(s32 X, s32 Y, s32 Z, s32 W, s32 H, s32 Col, s8 Style,
                              lstWork_t *pListWindow);

extern void lstMoveCursor2(lstWork_t *pListWindow, s8 Dir);

extern void fclInitPanel(lstWork_t *work, s32 X, s32 Y, s32 Z, s32 W, s32 H, s32 color, s8 type,
                         s8 speed);

extern void fclInitPanelPos(s32 X, s32 Y, s32 Z, u16 W, u16 H, s32 Work, void *pPanelPos);

extern void fclDrawPanel(lstWork_t *pPanel, s32 OtNo);

extern void fclDrawPanelSprite(s32 id, s32 ot);

extern void cmpUpdateCursorSlots(cmpCursorInfo_t *cursor);

extern s32 fclUpdatePanel(lstWork_t *pPanel);


#ifdef NON_MATCHING
extern void *memset(void *a, s32 b, u32 c);
#endif

void lstInitListWindow(s32 X, s32 Y, s32 Z, s32 W, s32 H, s32 Col, s8 Style, lstWork_t *work)
{
    if (Style == 0) {
        fclInitPanel(work, X, Y, Z, W, H, 0, 1, 0x10);
    } else {
        fclInitPanel(work, X, Y, Z, W, H, 0, 3, 0x10);
    }
    fclInitPanelPos(X, Y, Z, W, H, 0, work->panelPos);
    work->unk_58 = 0;
    work->unk_54 = Col;
    memset(work->slotFlags, 1, sizeof(work->slotFlags));
}

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

void lstDrawUpDownCursor(lstWork_t *work, s32 ot)
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

/* NON_MATCHING: 26 of 33 words match at the exact size; the scheduler sinks the
 * parameter sign-extension past the first call. */
#ifdef NON_MATCHING
void lstDrawListWindow(lstWork_t *work, s8 noCursor, s32 ot)
{
    s32 hideCursor = noCursor;

    fclDrawPanel(work, ot);
    fclDrawPanelSprite(0x53001, ot);
    if (hideCursor == 0) {
        lstDrawUpDownCursor(work, ot);
    }
    fclDrawPanelSprite(0x5101B, ot);
    cmpUpdateCursorSlots(&work->listCursor);
    fclUpdatePanel(work);
}
#else
INCLUDE_ASM(const s32, "interface/lstListWindow", lstDrawListWindow);
#endif

typedef struct lstWindowWork_t
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

extern s32 datGetItemSlotIndex(u16 id);

extern void datAddHearts(s32 id);

s32 datGetItemSlotIndex(u16 id)
{
    s8 slots[25] = {1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13,
                    14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25};
    s32 index = 0;

    if ((u16)(id - 0x40) < 0x19) {
        index = slots[id - 0x40];
    }
    return index;
}

INCLUDE_RODATA(const s32, "interface/lstListWindow", D_0053D708);

