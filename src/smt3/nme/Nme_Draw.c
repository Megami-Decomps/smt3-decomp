#include "common.h"

typedef struct {
    u8 unknown[0x51C4];
    s32 animation;
} nmeWork_t;

extern nmeWork_t *nmeUpdateWork;
extern s32 nmeGetCursorPos(nmeWork_t *work);
extern void nmeStepCursor(s8 delta);
extern void nmeDeleteChar(s8 delta);

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeDrawInputUBar);

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeDrawFuncIcon);

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeDrawInputPanel);

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeDraw);

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeGetCursorPos);

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

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeSyncCursor);

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
