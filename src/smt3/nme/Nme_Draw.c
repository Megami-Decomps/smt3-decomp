#include "common.h"

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeDrawInputUBar);

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeDrawFuncIcon);

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeDrawInputPanel);

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeDraw);

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeGetCursorPos);

INCLUDE_ASM(const s32, "nme/Nme_Draw", func_0029A108);

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeStepCursor);

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeDeleteChar);

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeConfirmChar);

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeBackspace);

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

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeAdvanceAnim);
