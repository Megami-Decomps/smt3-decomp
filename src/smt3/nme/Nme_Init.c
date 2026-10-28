#include "common.h"
#include "kernel.h"
#include "libsdf.h"

typedef struct {
    u8 flag;
    s8 current, next, last, change, messageFlag;
    u8 unk06[2];
    dds3ProcessID_t *messageProcess;
    s32 timer;
    void *parent;
} cmpSeqInfo_t;

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
    /* 0x27 */ u8 unk_27;
    /* 0x28 */ s32 unk_28;
    /* 0x2C */ s32 unk_2C;
    /* 0x30 */ s32 unk_30;
    /* 0x34 */ s32 unk_34;
} cmpCursorInfo_t;

typedef struct {
    cmpSeqInfo_t sequence;
    sdfMemHandle_t *memHandle;
    s8 targetNo;
    s8 entryMode;
    s8 inputMode;
    u8 unk1B;
    u8 unk1C[0x1060];
    s8 cursorMode;
    u8 unk107D[0x2057];
    cmpCursorInfo_t cursor;
    u8 unk310C[0x2044];
    s8 inputPos;
    u8 unk5151[0x63];
    sdfTexHandle_t *textures[4];
    s32 baseRate;
} nmeWork_t;

extern void sdfTexFree(sdfTexHandle_t *aHandle);
extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

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

extern void frDelFontStyle(u8 styleNo);
extern void nmeBuildNameFont(s8 mode, nmeWork_t *work);
INCLUDE_ASM(const s32, "nme/Nme_Init", frAllocNameData);

extern u8 D_00552020[4];
extern void dds3FadeOut(s32 r, s32 g, s32 b, s32 frame);
void nmeStartFadeOut(void)
{
    dds3FadeOut(D_00552020[0], D_00552020[1], D_00552020[2], 0x1E);
}

s32 nmeGetInputMode(nmeWork_t *work) {
    s32 inputPos;

    if (work->inputMode < 2) {
        if (work->inputMode >= 0) {
            inputPos = work->inputPos;
            return inputPos / 8 != 0;
        }
    }
    return 2;
}

s32 nmeGetMaxNameLen(s32 mode) {
    return 8;
}

s8 nmeGetModeFlag(nmeWork_t *work) {
    return nmeGetMaxNameLen(nmeGetInputMode(work));
}

INCLUDE_ASM(const s32, "nme/Nme_Init", nmeInitNameRow);

extern void nmeInitNameRow(s8 mode, s32 row, void *data);
void nmeInitNameRows(s8 mode, nmeWork_t *work) {
    nmeInitNameRow(mode, 0, work);
    nmeInitNameRow(mode, 1, work);
    nmeInitNameRow(mode, 2, work);
}

s32 nmeGetRowCount(s8 mode)
{
    switch (mode) {
    case 0:
        return 0xF;
    case 1:
        return 0xF;
    case 2:
        return 0x10;
    case 3:
        return 0xF;
    default:
        return 0;
    }
}

INCLUDE_ASM(const s32, "nme/Nme_Init", nmeInitNameBuffer);

INCLUDE_ASM(const s32, "nme/Nme_Init", nmeInitNameCursor);

INCLUDE_ASM(const s32, "nme/Nme_Init", nmeInitCursor);

INCLUDE_RODATA(const s32, "nme/Nme_Init", D_00548768);

INCLUDE_ASM(const s32, "nme/Nme_Init", nmeTexLoad);

void nmeFreeNameTextures(nmeWork_t *work) {
    s32 textureIndex;
    for (textureIndex = 0; textureIndex < 4; textureIndex++) {
        sdfTexFree(work->textures[textureIndex]);
    }
}

INCLUDE_ASM(const s32, "nme/Nme_Init", nmeLoadFont);

INCLUDE_ASM(const s32, "nme/Nme_Init", nmeBuildNameFont);

extern nmeWork_t *nmeProcessId;
extern u32 dds3KernelDebugSignOrg;
extern u32 dds3KernelDebugSign;
extern void dds3FadeClear(void);

void nmeDestroyName(dds3ProcessID_t *processId) {
    if (nmeProcessId != NULL) {
        nmeBuildNameFont(nmeProcessId->targetNo, nmeProcessId);
        nmeFreeNameTextures(nmeProcessId);
        frDelFontStyle(4);
        sdfMemFreeDirect(nmeProcessId->memHandle);
        nmeProcessId = NULL;
        gProcessStat = 2;
        dds3KernelDebugSign = dds3KernelDebugSignOrg;
        dds3FadeClear();
    }
}

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

void func_00298678(void)
{
    if (nmeProcessId != NULL) {
        nmeProcessId->sequence.flag |= 0x10;
    }
}

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

