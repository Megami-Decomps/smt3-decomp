#include "common.h"

typedef struct {
    u8 unk00[0x51B4];
    void *textures[4];
} nmeTextureWork_t;
extern void sdfTexFree(void *texture);
extern void dds3KillProcessName(const char *name, s32 hierarchy);
extern char D_00551690[];
extern char D_005487C8[];
extern char D_005487D8[];
extern s8 gProcessStat;

INCLUDE_ASM(const s32, "code_00297928", nmeCharToIndex);

INCLUDE_ASM(const s32, "code_00297928", nmeDecodeChar);

INCLUDE_ASM(const s32, "code_00297928", frAllocNameData);

INCLUDE_ASM(const s32, "code_00297928", nmeStartFadeOut);

INCLUDE_ASM(const s32, "code_00297928", nmeGetInputMode);

u32 nmeGetMaxNameLen(void) {
    return 8;
}

INCLUDE_ASM(const s32, "code_00297928", nmeGetModeFlag);

INCLUDE_ASM(const s32, "code_00297928", nmeInitNameRow);

INCLUDE_ASM(const s32, "code_00297928", nmeInitNameRows);

INCLUDE_ASM(const s32, "code_00297928", nmeGetRowCount);

INCLUDE_ASM(const s32, "code_00297928", nmeInitNameBuffer);

INCLUDE_ASM(const s32, "code_00297928", nmeInitNameCursor);

INCLUDE_ASM(const s32, "code_00297928", nmeInitCursor);

INCLUDE_RODATA(const s32, "code_00297928", D_00548768);

INCLUDE_ASM(const s32, "code_00297928", nmeTexLoad);

void nmeFreeNameTextures(nmeTextureWork_t *work) {
    s32 textureIndex;
    for (textureIndex = 0; textureIndex < 4; textureIndex++) {
        sdfTexFree(work->textures[textureIndex]);
    }
}

INCLUDE_ASM(const s32, "code_00297928", nmeLoadFont);

INCLUDE_ASM(const s32, "code_00297928", nmeBuildNameFont);

INCLUDE_ASM(const s32, "code_00297928", nmeDestroyName);

INCLUDE_ASM(const s32, "code_00297928", nmeStartProcesses);

void nmeNameProcessEnd(void) {
    dds3KillProcessName(D_00551690, 1);
    dds3KillProcessName(D_005487C8, 1);
    dds3KillProcessName(D_005487D8, 1);
}

s32 nmeCheckProcessStat(void) {
    s32 state = gProcessStat;
    if (state == 1) {
        return 1;
    }
    if (state < 2) {
        return 0;
    }
    if (state == 2) {
        gProcessStat = 0;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_00297928", func_00298678);

INCLUDE_ASM(const s32, "code_00297928", nme_298698);

INCLUDE_ASM(const s32, "code_00297928", nmeCalc);

INCLUDE_ASM(const s32, "code_00297928", nmeDrawCharRow);

INCLUDE_ASM(const s32, "code_00297928", nmeDrawSymbolRow);

INCLUDE_ASM(const s32, "code_00297928", nmeDrawNameText);

INCLUDE_ASM(const s32, "code_00297928", nmeDrawNameString);

INCLUDE_ASM(const s32, "code_00297928", nmeDrawEditIcons);

INCLUDE_ASM(const s32, "code_00297928", nmeDrawTextBox);

INCLUDE_ASM(const s32, "code_00297928", nmeDrawColoredText);

INCLUDE_ASM(const s32, "code_00297928", nmeDrawButtonGuide);

INCLUDE_ASM(const s32, "code_00297928", nmeDrawPanelSprites);

INCLUDE_ASM(const s32, "code_00297928", nmeDrawNamePanel);

INCLUDE_ASM(const s32, "code_00297928", nmeDrawCharCell);

INCLUDE_ASM(const s32, "code_00297928", nmeDrawCursorAt);

INCLUDE_ASM(const s32, "code_00297928", nmeDrawCursor);

INCLUDE_ASM(const s32, "code_00297928", nmeDrawTabSprite);

INCLUDE_ASM(const s32, "code_00297928", nmeDrawModeTabs);

INCLUDE_ASM(const s32, "code_00297928", nmeDrawInputUBar);

INCLUDE_ASM(const s32, "code_00297928", nmeDrawFuncIcon);

INCLUDE_ASM(const s32, "code_00297928", nmeDrawInputPanel);

INCLUDE_ASM(const s32, "code_00297928", nmeDraw);

INCLUDE_ASM(const s32, "code_00297928", nmeGetCursorPos);

INCLUDE_ASM(const s32, "code_00297928", func_0029A108);

INCLUDE_ASM(const s32, "code_00297928", nmeStepCursor);

INCLUDE_ASM(const s32, "code_00297928", nmeDeleteChar);

INCLUDE_ASM(const s32, "code_00297928", nmeConfirmChar);

INCLUDE_ASM(const s32, "code_00297928", nmeBackspace);

INCLUDE_ASM(const s32, "code_00297928", nmeMoveCursorBy);

INCLUDE_ASM(const s32, "code_00297928", nmeSyncCursor);

INCLUDE_ASM(const s32, "code_00297928", nmeMoveCursor);

INCLUDE_ASM(const s32, "code_00297928", nmeMoveCursor2);

INCLUDE_ASM(const s32, "code_00297928", nmeResetCursor);

INCLUDE_ASM(const s32, "code_00297928", nmeFindNextChar);

INCLUDE_ASM(const s32, "code_00297928", nmeMoveCursorWrap);

u32 nmeInputIsLocked(void) {
    return 0;
}

INCLUDE_ASM(const s32, "code_00297928", nmeCheckInputMode);

INCLUDE_ASM(const s32, "code_00297928", nmeUpdateInput);

INCLUDE_ASM(const s32, "code_00297928", nmeAdvanceAnim);

INCLUDE_ASM(const s32, "code_00297928", nmeUpdate);

INCLUDE_RODATA(const s32, "code_00297928", D_005487C8);

INCLUDE_RODATA(const s32, "code_00297928", D_005487D8);

INCLUDE_RODATA(const s32, "code_00297928", D_005487E8);

INCLUDE_RODATA(const s32, "code_00297928", D_005487F8);

INCLUDE_RODATA(const s32, "code_00297928", D_00548808);

INCLUDE_RODATA(const s32, "code_00297928", D_00548818);

INCLUDE_RODATA(const s32, "code_00297928", D_00548828);

INCLUDE_RODATA(const s32, "code_00297928", D_00548838);

INCLUDE_RODATA(const s32, "code_00297928", D_00548848);

INCLUDE_RODATA(const s32, "code_00297928", D_00548858);

INCLUDE_SDATA(const s32, "code_00297928", nmeProcessId);

INCLUDE_SDATA(const s32, "code_00297928", gProcessStat);

INCLUDE_SDATA(const s32, "code_00297928", D_0055165D);

INCLUDE_SDATA(const s32, "code_00297928", D_00551660);

INCLUDE_SDATA(const s32, "code_00297928", D_00551668);

INCLUDE_SDATA(const s32, "code_00297928", D_00551670);

INCLUDE_SDATA(const s32, "code_00297928", D_00551678);

INCLUDE_SDATA(const s32, "code_00297928", D_00551680);

INCLUDE_SDATA(const s32, "code_00297928", D_00551688);

INCLUDE_SDATA(const s32, "code_00297928", D_00551690);

INCLUDE_SDATA(const s32, "code_00297928", D_00551698);

INCLUDE_SDATA(const s32, "code_00297928", D_005516A0);

INCLUDE_SDATA(const s32, "code_00297928", D_005516A8);

INCLUDE_SDATA(const s32, "code_00297928", D_005516B0);

INCLUDE_SDATA(const s32, "code_00297928", D_005516B8);

INCLUDE_SDATA(const s32, "code_00297928", D_005516BC);

