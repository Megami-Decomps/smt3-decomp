#include "common.h"
#include "nme.h"

extern s32 nmeGetInputMode(nmeWork_t *work);
extern void cmpSetCursorRow(s32 row, cmpCursorInfo_t *cursor);


extern nmeWork_t *nmeUpdateWork;
extern s32 nmeGetCursorPos(nmeWork_t *work);
extern void nmeStepCursor(s8 delta);
extern void nmeDeleteChar(s8 delta);

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeDrawInputUBar);

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeDrawFuncIcon);

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeDrawInputPanel);

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeDraw);

s32 nmeGetCursorPos(nmeWork_t *work) {
    s32 mode = nmeGetInputMode(work);

    switch (mode) {
    case 0:
        return work->inputPos;
    case 1: {
        s32 position = (u8)work->inputPos;
        position -= 8;
        return (s8)position;
    }
    default:
        return work->inputPos;
    }
}

INCLUDE_ASM(const s32, "nme/Nme_Draw", func_0029A108);

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeStepCursor);

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeDeleteChar);

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeConfirmChar);

void nmeBackspace(void)
{
    if (nmeGetCursorPos(nmeUpdateWork) > 0) {
        nmeStepCursor(-1);
    }
    nmeDeleteChar(1);
}

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeMoveCursorBy);

void nmeSyncCursor(void) {
    s8 row = (s8)((u8)nmeUpdateWork->cursor.unk_22 +
                  (u8)nmeUpdateWork->cursor.unk_24);

    cmpSetCursorRow((s32)row + 2, &nmeUpdateWork->inputCursor);
}

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeMoveCursor);

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeMoveCursor2);

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeResetCursor);

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeFindNextChar);

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeMoveCursorWrap);

u32 nmeInputIsLocked(void) {
    return 0;
}

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeCheckInputMode);

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeUpdateInput);

void nmeAdvanceAnim(void)
{
    nmeWork_t *work = nmeUpdateWork;
    work->animation += 2;
    if (work->animation >= 0x900) {
        work->animation = 0;
    }
}
