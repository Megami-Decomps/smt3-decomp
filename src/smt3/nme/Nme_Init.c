#include "common.h"

typedef struct {
    u8 unk00[0x51B4];
    void *textures[4];
} nmeTextureWork_t;

extern void sdfTexFree(void *aHandle);

extern void dds3KillProcessName(const char *name, s32 shutdowntimer);

extern char D_00551690[];

extern char D_005487C8[];

extern char D_005487D8[];

extern s8 gProcessStat;

u8 nmeCharToIndex(u8 lead, u8 trail) {
    s32 value = (((s32)lead << 8) | trail) - 0x8080;
    s32 index = ((u32)(value & 0xFF00) >> 1) + (value & 0x7F);
    return (u8)(index - 0x4A);
}

INCLUDE_ASM(const s32, "nme/Nme_Init", nmeDecodeChar);

INCLUDE_ASM(const s32, "nme/Nme_Init", frAllocNameData);

INCLUDE_ASM(const s32, "nme/Nme_Init", nmeStartFadeOut);

INCLUDE_ASM(const s32, "nme/Nme_Init", nmeGetInputMode);

u32 nmeGetMaxNameLen(void) {
    return 8;
}

INCLUDE_ASM(const s32, "nme/Nme_Init", nmeGetModeFlag);

INCLUDE_ASM(const s32, "nme/Nme_Init", nmeInitNameRow);

extern void nmeInitNameRow(s8 mode, s32 row, void *data);
void nmeInitNameRows(s8 mode, void *data) {
    nmeInitNameRow(mode, 0, data);
    nmeInitNameRow(mode, 1, data);
    nmeInitNameRow(mode, 2, data);
}

INCLUDE_ASM(const s32, "nme/Nme_Init", nmeGetRowCount);

INCLUDE_ASM(const s32, "nme/Nme_Init", nmeInitNameBuffer);

INCLUDE_ASM(const s32, "nme/Nme_Init", nmeInitNameCursor);

INCLUDE_ASM(const s32, "nme/Nme_Init", nmeInitCursor);

INCLUDE_RODATA(const s32, "nme/Nme_Init", D_00548768);

INCLUDE_ASM(const s32, "nme/Nme_Init", nmeTexLoad);

void nmeFreeNameTextures(nmeTextureWork_t *work) {
    s32 textureIndex;
    for (textureIndex = 0; textureIndex < 4; textureIndex++) {
        sdfTexFree(work->textures[textureIndex]);
    }
}

INCLUDE_ASM(const s32, "nme/Nme_Init", nmeLoadFont);

INCLUDE_ASM(const s32, "nme/Nme_Init", nmeBuildNameFont);

INCLUDE_ASM(const s32, "nme/Nme_Init", nmeDestroyName);

INCLUDE_ASM(const s32, "nme/Nme_Init", nmeStartProcesses);

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

INCLUDE_ASM(const s32, "nme/Nme_Init", func_00298678);

INCLUDE_ASM(const s32, "nme/Nme_Init", nme_298698);

INCLUDE_RODATA(const s32, "nme/Nme_Init", D_005487C8);

INCLUDE_RODATA(const s32, "nme/Nme_Init", D_005487D8);

INCLUDE_SDATA(const s32, "nme/Nme_Init", nmeProcessId);

INCLUDE_SDATA(const s32, "nme/Nme_Init", gProcessStat);

INCLUDE_SDATA(const s32, "nme/Nme_Init", D_0055165D);

INCLUDE_SDATA(const s32, "nme/Nme_Init", D_00551660);

INCLUDE_SDATA(const s32, "nme/Nme_Init", D_00551668);

INCLUDE_SDATA(const s32, "nme/Nme_Init", D_00551670);

INCLUDE_SDATA(const s32, "nme/Nme_Init", D_00551678);

INCLUDE_SDATA(const s32, "nme/Nme_Init", D_00551680);

INCLUDE_SDATA(const s32, "nme/Nme_Init", D_00551688);

INCLUDE_SDATA(const s32, "nme/Nme_Init", D_00551690);

INCLUDE_SDATA(const s32, "nme/Nme_Init", D_00551698);

INCLUDE_SDATA(const s32, "nme/Nme_Init", D_005516A0);

INCLUDE_SDATA(const s32, "nme/Nme_Init", D_005516A8);

INCLUDE_SDATA(const s32, "nme/Nme_Init", D_005516B0);

INCLUDE_SDATA(const s32, "nme/Nme_Init", D_005516B8);

INCLUDE_SDATA(const s32, "nme/Nme_Init", mccb);

