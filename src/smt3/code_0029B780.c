#include "fpu.h"
#include "common.h"
#include "ps2asm.h"
#include "basic.h"
#include "motion.h"

extern void (*D_00551828)(void);

extern void *dds3AllocConfigWork(s32 mode);

extern void *memset(void *a, s32 b, u32 c);

extern s32 dds3GetGlobalConfigFlag(s32 index);

extern void dds3PadVibrationSet(s32 port, s32 strength, s32 frames);

typedef struct dds3ConfigWork_t {
    u8 prefix[0x14];
    void *memHandle;
    s8 mode;
    u8 reserved19[3];
    u8 cursorAndItems[0x1084];
    s32 animationPhase;
    void *textureHandle;
} dds3ConfigWork_t;

typedef char dds3ConfigWork_size_must_be_0x10A8[
    sizeof(dds3ConfigWork_t) == 0x10A8 ? 1 : -1];

typedef struct fclRagTex_t {
    u8 unk00[0x20];
    void (*calcSizes)();
} fclRagTex_t;

extern void dds3FreeConfigTexture(void *texture);

extern void fclRagTexInit(fclRagTex_t *tex, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                          void (*calc)());

extern void fclRagTexCalcSizes();

extern void fclSetSprite(s32 pSprite, s32 Xsize, s32 Ysize, s32 Z, s32 U1, s32 V1, s32 U2, s32 V2, s32 Col, s32 TexHandle);

extern void fclDrawBG(void *pSprite, s32 OtNo);

extern s32 dds3ConfigBackup;

extern char D_00548870[];



extern s32 dds3DrawConfigPanel(s32 id);

extern char D_00548858[];

extern char D_005488B8[];

extern void *D_005516C4;

/* [1] is dds3ConfigBackup; both are written through one base register. */
extern s32 dds3ConfigValue[2];

extern s8 slLoadKillFlag;

extern u8 *GBWK;

extern u8 *DDS3_GBWK;

extern s32 fclGetPanelAnimPhase(void *p);

extern void sdfTexFree(void *aHandle);

extern s32 dds3GetConfigFlag(u32 index, s32 *table);

extern void *fclLoadTexture(char *pFileName);

extern void fclSetPanelAnimState(u8 *panel, s8 state);

extern s32 fclRunPanelAnim(void *work);

extern s32 dds3KillProcess(void *id, s32 shutdowntimer);

extern u8 dds3GetConfigFadeAlpha(void);

extern s32 dds3GetConfigAnimPhase(void);

extern s32 printf(const char *fmt, ...);

extern void *sdfMemAllocDirect(s32 aSize);

extern void *sdfMemLock(void *aHandle);
extern void cmpInitSeqInfo(void *work, s32 seq);
extern void cmpInitCursorLayout(void *cursor, s32 a, s32 b, s32 c, s32 d, s32 mode);
extern void cmpInitCursorRange(void *cursor, u8 count, u8 visible, s32 layout);

void slKillLoadProcess(void)
{
    if (D_005516C4 != NULL) {
        dds3KillProcess(D_005516C4, 1);
        D_005516C4 = NULL;
        slLoadKillFlag = 0;
    }
}

s32 dds3_29B7B0(void)
{
    return slLoadKillFlag;
}

void func_0029B7B8(void) {
}

s32 dds3ConfigChanged(void)
{
    if (dds3ConfigValue[0] != dds3ConfigValue[1]) {
        printf("Cfg change!\n");
        return 1;
    }
    printf("Cfg no change!\n");
    return 0;
}

void dds3ConfigSaveBackup(void)
{
    printf(D_00548858);
    dds3ConfigValue[0] = dds3ConfigValue[1] = *(s32 *)(DDS3_GBWK + 0x11AB8);
}

void dds3ConfigRestoreBackup(void)
{
    printf(D_00548870);
    *(s32 *)(DDS3_GBWK + 0x11AB8) = dds3ConfigBackup;
}

void dds3InitConfigItems(u8 *gbwk)
{
    fclRagTex_t *tex = (fclRagTex_t *)(gbwk + 0x1058);
    u32 i;

    for (i = 0; i < 2; i++) {
        fclRagTexInit(tex, 0, 0, 0x96, 0, 0x200, 0x20, fclRagTexCalcSizes);
        tex++;
    }
}

s32 dds3UpdateConfigCursor(void)
{
    fclSetPanelAnimState(GBWK + 0x1058, 2);
    return fclRunPanelAnim(GBWK + 0x1058);
}

s32 dds3GetConfigAnimPhase(void)
{
    return fclGetPanelAnimPhase(GBWK + 0x1058);
}

u8 dds3GetConfigFadeAlpha(void)
{
    return (0x200 - dds3GetConfigAnimPhase()) / 4;
}

void dds3ScrollConfigList(void)
{
    u8 *gbwk;
    s32 next;

    if (dds3GetConfigAnimPhase() > 0) {
        return;
    }
    gbwk = GBWK;
    next = *(s32 *)(gbwk + 0x10A0) + 0xC;
    *(s32 *)(gbwk + 0x10A0) = next;
    if (next > 0x200) {
        *(s32 *)(gbwk + 0x10A0) = 0;
    }
}

s32 dds3ToggleConfigFlag(u32 index, s32 *table)
{
    switch (index) {
    case 0:
        *table ^= 2;
        if (dds3GetGlobalConfigFlag(index)) {
            dds3PadVibrationSet(0, 1, 0xF);
            dds3PadVibrationSet(1, 0x80, 0xF);
        } else {
            dds3PadVibrationSet(0, 0, 0xF);
            dds3PadVibrationSet(1, 0, 0xF);
        }
        return 1;
    case 1:
        *table ^= 4;
        return 1;
    case 2:
        *table ^= 8;
        return 1;
    }
    return 0;
}

s32 dds3GetConfigFlag(u32 index, s32 *table)
{
    switch (index) {
    case 0:
        return ((*table >> 1) ^ 1) & 1;
    case 1:
        return *table & 4;
    case 2:
        return *table & 8;
    case 5:
        return 1;
    }
    return -1;
}

s32 dds3GetGlobalConfigFlag(s32 index)
{
    return dds3GetConfigFlag(index, (s32 *)(DDS3_GBWK + 0x11AB8));
}

INCLUDE_ASM(const s32, "code_0029B780", dds3DrawConfigLabel);

INCLUDE_ASM(const s32, "code_0029B780", dds3DrawConfigCursor);

extern u32 fclBlendColor(u32 firstColor, u32 secondColor, u32 weight);
extern s32 fclDrawPanelSprite2(s32 id, s32 ot);
extern s32 cmpDrawBoxF(s32 x, s32 y, s32 z, s32 width, s32 height, s32 color, s32 ot);

s32 dds3DrawConfigItem(s32 x, s32 y, s32 z, s32 ot)
{
    s32 phase = dds3GetConfigAnimPhase();
    u32 color0 = fclBlendColor(0x9195EE20, 0x20, 0x100 - phase / 2);

    phase = dds3GetConfigAnimPhase();
    {
        u32 color1 = fclBlendColor(0x6D6D9580, 0x20, 0x100 - phase / 2);
        fclDrawPanelSprite2(0x44, ot);
        cmpDrawBoxF(x, y, z, 0x200, 3, color0, ot);
        cmpDrawBoxF(x, y + 0x18, z, 0x200, 0xC, color1, ot);
        cmpDrawBoxF(x, y + 0x998, z, 0x200, 0xC, color1, ot);
        return cmpDrawBoxF(x, y + 0x9F8, z, 0x200, 3, color0, ot);
    }
}

INCLUDE_ASM(const s32, "code_0029B780", dds3DrawConfigSlider);

INCLUDE_ASM(const s32, "code_0029B780", dds3DrawConfigBar);

INCLUDE_ASM(const s32, "code_0029B780", dds3DrawConfigValue);

INCLUDE_ASM(const s32, "code_0029B780", dds3DrawConfigText);

s32 dds3DrawConfigBackground(s32 flags)
{
    u8 sprite[0x30];
    s32 ret = 1;

    if (GBWK[0x19] == 0) {
        GBWK[0x19] = ret;
        return ret;
    }
    fclSetSprite((s32)sprite, 0x200, 0x1C0, 0, 0, 0, 0xFF, 0x7F, 0x80808080,
                 *(s32 *)(GBWK + 0x10A4));
    fclDrawBG(sprite, flags);
}

INCLUDE_ASM(const s32, "code_0029B780", dds3DrawConfigPanel);

void *cfgTexLoad(void)
{
    return fclLoadTexture(D_005488B8);
}

void dds3FreeConfigTexture(void *handle)
{
    if (handle != NULL) {
        sdfTexFree(handle);
    }
}

void *dds3AllocConfigWork(s32 mode)
{
    void *handle;
    dds3ConfigWork_t *work;

    handle = sdfMemAllocDirect(sizeof(*work));
    work = (dds3ConfigWork_t *)sdfMemLock(handle);
    memset(work, 0, sizeof(*work));
    work->memHandle = handle;
    cmpInitSeqInfo(work, 0);
    work->mode = mode;
    cmpInitCursorLayout(work->cursorAndItems, 0, 0, 0, 0, 0);
    cmpInitCursorRange(work->cursorAndItems, 4, 4, 0);
    dds3InitConfigItems((u8 *)work);
    work->animationPhase = 0;
    if (mode == 1) {
        work->textureHandle = cfgTexLoad();
    } else {
        work->textureHandle = NULL;
    }
    return work;
}

INCLUDE_RODATA(const s32, "code_0029B780", D_00548858);

INCLUDE_RODATA(const s32, "code_0029B780", D_00548870);

INCLUDE_RODATA(const s32, "code_0029B780", D_00548888);

INCLUDE_RODATA(const s32, "code_0029B780", D_005488B8);

INCLUDE_SDATA(const s32, "code_0029B780", slLoadKillFlag);

INCLUDE_SDATA(const s32, "code_0029B780", D_005516C1);

INCLUDE_SDATA(const s32, "code_0029B780", D_005516C4);

INCLUDE_SDATA(const s32, "code_0029B780", D_005516C8);

INCLUDE_SDATA(const s32, "code_0029B780", D_005516D0);

INCLUDE_SDATA(const s32, "code_0029B780", D_005516D8);

INCLUDE_SDATA(const s32, "code_0029B780", D_005516E0);

INCLUDE_SDATA(const s32, "code_0029B780", D_005516E8);

INCLUDE_SDATA(const s32, "code_0029B780", D_005516F0);

INCLUDE_SDATA(const s32, "code_0029B780", dds3ConfigValue);

INCLUDE_SDATA(const s32, "code_0029B780", dds3ConfigBackup);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551700);

INCLUDE_SDATA(const s32, "code_0029B780", mapIndex);

INCLUDE_SDATA(const s32, "code_0029B780", mapPrev);

INCLUDE_SDATA(const s32, "code_0029B780", debugMode);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551711);

INCLUDE_SDATA(const s32, "code_0029B780", SectionIndex);

INCLUDE_SDATA(const s32, "code_0029B780", pipeInitDone);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551719);

INCLUDE_SDATA(const s32, "code_0029B780", pipeLoadState);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551724);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551728);

INCLUDE_SDATA(const s32, "code_0029B780", pbHitCheckCur);

INCLUDE_SDATA(const s32, "code_0029B780", offsetCount);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551738);

INCLUDE_SDATA(const s32, "code_0029B780", D_0055173C);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551740);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551744);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551748);

INCLUDE_SDATA(const s32, "code_0029B780", pipeIndex);

INCLUDE_SDATA(const s32, "code_0029B780", attackCount);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551751);

INCLUDE_SDATA(const s32, "code_0029B780", myMaka);

INCLUDE_SDATA(const s32, "code_0029B780", ZmakaCount);

INCLUDE_SDATA(const s32, "code_0029B780", playerDAMAGE);

INCLUDE_SDATA(const s32, "code_0029B780", rBoxNum);

INCLUDE_SDATA(const s32, "code_0029B780", offsetStep);

INCLUDE_SDATA(const s32, "code_0029B780", offsetStepCount);

INCLUDE_SDATA(const s32, "code_0029B780", CameraAction);

INCLUDE_SDATA(const s32, "code_0029B780", camMode);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551765);

INCLUDE_SDATA(const s32, "code_0029B780", myStatus);

INCLUDE_SDATA(const s32, "code_0029B780", pipeObjList);

INCLUDE_SDATA(const s32, "code_0029B780", pipeObjTail);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551770);

INCLUDE_SDATA(const s32, "code_0029B780", pipeMyMakaCount);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551778);

INCLUDE_SDATA(const s32, "code_0029B780", hWorkArea);

INCLUDE_SDATA(const s32, "code_0029B780", WorkAreaPtr);

INCLUDE_SDATA(const s32, "code_0029B780", PartsWork);

INCLUDE_SDATA(const s32, "code_0029B780", PipeCur);

INCLUDE_SDATA(const s32, "code_0029B780", PipeCur2);

INCLUDE_SDATA(const s32, "code_0029B780", PipeLine);

INCLUDE_SDATA(const s32, "code_0029B780", ObjWork);

INCLUDE_SDATA(const s32, "code_0029B780", basicMy);

INCLUDE_SDATA(const s32, "code_0029B780", MyLine);

INCLUDE_SDATA(const s32, "code_0029B780", PipeEMS);

INCLUDE_SDATA(const s32, "code_0029B780", PipeEML);

INCLUDE_SDATA(const s32, "code_0029B780", PipeRBOX);

INCLUDE_SDATA(const s32, "code_0029B780", PipeMaka1);

INCLUDE_SDATA(const s32, "code_0029B780", PipeZMaka);

INCLUDE_SDATA(const s32, "code_0029B780", PipeDMaka);

INCLUDE_SDATA(const s32, "code_0029B780", PipeDRBOX);

INCLUDE_SDATA(const s32, "code_0029B780", MakaTex);

INCLUDE_SDATA(const s32, "code_0029B780", makaWork);

INCLUDE_SDATA(const s32, "code_0029B780", pipe_CELL01_D3P);

INCLUDE_SDATA(const s32, "code_0029B780", pipe_BREAK01_D3P);

INCLUDE_SDATA(const s32, "code_0029B780", pipe_BREAK02_D3P);

INCLUDE_SDATA(const s32, "code_0029B780", pipe_DAMAGE01_D3P);

INCLUDE_SDATA(const s32, "code_0029B780", pipe_DAMAGE03_D3P);

INCLUDE_SDATA(const s32, "code_0029B780", pipe_ATTACK01_D3P);

INCLUDE_SDATA(const s32, "code_0029B780", D_005517DC);

INCLUDE_SDATA(const s32, "code_0029B780", D_005517E0);

INCLUDE_SDATA(const s32, "code_0029B780", D_005517E8);

INCLUDE_SDATA(const s32, "code_0029B780", D_005517F0);

INCLUDE_SDATA(const s32, "code_0029B780", pipe_Camera);

INCLUDE_SDATA(const s32, "code_0029B780", D_005517FC);

INCLUDE_SDATA(const s32, "code_0029B780", ResultIndex);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551801);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551808);

INCLUDE_SDATA(const s32, "code_0029B780", pipeMessData);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551810);

INCLUDE_SDATA(const s32, "code_0029B780", pipeMessCount);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551815);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551818);

INCLUDE_SDATA(const s32, "code_0029B780", pipeBulrLimitValue);

INCLUDE_SDATA(const s32, "code_0029B780", sdfDmacHandlerTable);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551824);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551828);

INCLUDE_SDATA(const s32, "code_0029B780", sdfVblankFlag);

