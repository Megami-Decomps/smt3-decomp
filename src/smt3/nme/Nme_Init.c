#include "common.h"
#include "kernel.h"
#include "libsdf.h"
#include "nme.h"


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

extern char D_00548780[];
extern char D_00548798[];
extern char D_005487B0[];
extern char *nmeCharTexFile[4];
extern char *D_005276B0 __attribute__((section(".data")));
extern s32 fclLoadTexture(char *filename);
extern s32 evtBitCheck(s32 bit);

void nmeTexLoad(s8 mode, nmeWork_t *work)
{
    work->textures[0] = (sdfTexHandle_t *)fclLoadTexture(D_00548780);
    work->textures[1] = (sdfTexHandle_t *)fclLoadTexture(D_00548798);
    work->textures[2] = (sdfTexHandle_t *)fclLoadTexture(D_005487B0);
    if (evtBitCheck(0x8B0) != 0 && mode == 0) {
        work->textures[3] = (sdfTexHandle_t *)fclLoadTexture(D_005276B0);
    } else {
        work->textures[3] = (sdfTexHandle_t *)fclLoadTexture(nmeCharTexFile[mode]);
    }
}

void nmeFreeNameTextures(nmeWork_t *work) {
    s32 textureIndex;
    for (textureIndex = 0; textureIndex < 4; textureIndex++) {
        sdfTexFree(work->textures[textureIndex]);
    }
}

extern sdfMemHandle_t *sdfMemAllocDirect(u32 size);
extern void *sdfMemLock(sdfMemHandle_t *handle);
extern void cmpInitSeqInfo(void *work, s32 parent);
extern void nmeInitNameBuffer(s8 mode, nmeWork_t *work);
extern void nmeInitCursor(s8 mode, nmeWork_t *work);
extern void nmeTexLoad(s8 mode, nmeWork_t *work);
extern void frLoadFontData(s32 style, const char *filename);
extern char D_00548768[];

nmeWork_t *nmeLoadFont(s8 target, s8 mode, s32 parent)
{
    sdfMemHandle_t *handle = sdfMemAllocDirect(0x51C8);
    nmeWork_t *work = sdfMemLock(handle);

    memset(work, 0, 0x51C8);
    work->memHandle = handle;
    cmpInitSeqInfo(work, parent);
    work->sequence.current = 0;
    work->entryMode = mode;
    work->sequence.change = 1;
    work->targetNo = target;
    work->sequence.last = 0;
    work->inputRowEnabled[0] = 0;
    work->inputRowEnabled[1] = 0;
    work->inputRowEnabled[2] = 0;
    nmeInitNameBuffer(0, work);
    nmeInitCursor(4, work);
    nmeInitNameRows(target, work);
    frLoadFontData(4, D_00548768);
    nmeTexLoad(target, work);
    return work;
}

INCLUDE_ASM(const s32, "nme/Nme_Init", nmeBuildNameFont);

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

INCLUDE_RODATA(const s32, "nme/Nme_Init", D_00548768);

INCLUDE_RODATA(const s32, "nme/Nme_Init", D_00548780);

INCLUDE_RODATA(const s32, "nme/Nme_Init", D_00548798);

INCLUDE_RODATA(const s32, "nme/Nme_Init", D_005487B0);

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

