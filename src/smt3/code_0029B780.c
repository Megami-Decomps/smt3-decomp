#include "fpu.h"
#include "common.h"
#include "facility.h"
#include "ps2asm.h"
#include "basic.h"
#include "motion.h"
#include "font.h"

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
    u8 cursorAndItems[0x103C];
    fclPanelAnim_t panels[2];
    s32 animationPhase;
    void *textureHandle;
} dds3ConfigWork_t;

typedef char dds3ConfigWork_size_must_be_0x10A8[
    sizeof(dds3ConfigWork_t) == 0x10A8 ? 1 : -1];

extern void dds3FreeConfigTexture(void *texture);

extern void fclRagTexInit(fclRagTex_t *tex, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                          void (*calc)());

extern void fclRagTexCalcSizes();

extern void fclSetSprite(s32 pSprite, s32 Xsize, s32 Ysize, s32 Z, s32 U1, s32 V1, s32 U2, s32 V2, s32 Col, s32 TexHandle);

extern void fclDrawBG(void *pSprite, s32 OtNo);

extern s32 dds3ConfigBackup;

extern char D_00548870[];



extern void dds3DrawConfigPanel(s32 id);

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

typedef struct frTextBank_t frTextBank_t;
extern u8 *MessageNumSearch(s32 index, frTextBank_t *bank, s32 size);
extern frTextBank_t itfMesEmbedded_528D68;
extern void frSetMessageStepY(u32 step);
extern void frSetMessageDefStepY(void);
extern frFontHandle_t *MessageRequestNS(s32 x, s32 y, s32 depth, s32 style,
    s32 a, s32 b, s32 alpha, u8 *text, frFontHandle_t *previous);
extern void frSetFontColor(frFontHandle_t *font, u32 color);
extern s32 func_001C7E38(frFontHandle_t *font, s8 mode, u32 ot);
extern s32 frDestroyFontRequestAfter(frFontHandle_t *font);

s32 dds3DrawConfigLabel(s32 x, s32 y, s32 z, s32 index, s32 selected, s32 ot)
{
    s32 selectedColor = 0x58606080;
    s32 color;
    u8 alpha = dds3GetConfigFadeAlpha();
    u8 *text;
    frFontHandle_t *font;

    if (selected != 0) {
        selectedColor = 0xA09DC380;
    }
    color = (selectedColor & ~0xFF) | alpha;
    text = MessageNumSearch(index, &itfMesEmbedded_528D68, 0);
    frSetMessageStepY(0x16);
    font = MessageRequestNS(x, y, z, 1, 0, 0, 0x80, text, NULL);
    frSetMessageDefStepY();
    frSetFontColor(font, color);
    func_001C7E38(font, 1, ot);
    return frDestroyFontRequestAfter(font);
}



extern fclSpriteTbl_t dds3ConfigLayoutTable[];
extern s32 D_00345D88[];
extern void fclDrawSpriteFilter(s32 x, s32 y, s32 z, u8 filter, s32 *colors,
    s32 page, s32 sprite, s32 *textures, fclSpriteTbl_t *table, s32 ot);
extern void fclDrawSpriteRes(s32 x, s32 y, s32 z, s32 *colors,
    s32 page, s32 sprite, s32 *textures, fclSpriteTbl_t *table, s32 ot);
extern void dds3DrawConfigValue(s32 x, s32 y, s32 z, s32 index, s32 selected, s32 ot);
extern u32 fclBlendColor(u32 firstColor, u32 secondColor, u32 weight);
extern void cmpDrawBoxGDefault(s32 x, s32 y, s32 z, s32 width, s32 height, s32 *colors, s32 ot);
extern void fclDrawSpriteAlpha(s32 x, s32 y, s32 z, u8 mode, s32 *colors,
    s32 page, s32 sprite, s32 *textures, fclSpriteTbl_t *table, s32 ot);

/* NON_MATCHING: 7 of 179 words differ, at the native 716-byte size. */
#ifdef NON_MATCHING
extern void fclDrawPanelSprite2(s32 id, s32 ot);

void dds3DrawConfigCursor(s32 x, s32 y, s32 z, u32 width, s32 height, s32 selected, s32 ot)
{
    s32 colors[4];
    s32 phase = ((dds3ConfigWork_t *)GBWK)->animationPhase;
    u32 half;
    s32 blended;

    if (phase > 0x100) {
        phase = 0x200 - phase;
    }
    switch (selected) {
    case 1: {
        s32 fadedColor;
        s32 pulseColor;
        s32 depth;
        half = width >> 1;
        blended = fclBlendColor(0x75FFFA2C, 0x2200842C, phase);
        fclDrawPanelSprite2(0x48, ot);
        colors[0] = blended;
        colors[1] = 0x2200842C;
        colors[2] = blended;
        colors[3] = 0x2200842C;
        cmpDrawBoxGDefault(x, y, z, half, height, colors, ot);
        colors[0] = 0x2200842C;
        colors[1] = blended;
        colors[2] = 0x2200842C;
        colors[3] = blended;
        cmpDrawBoxGDefault(x + (half << 4), y, z, half, height, colors, ot);
        depth = z << 4;
        fclDrawPanelSprite2(0x44, ot);
        fadedColor = dds3GetConfigFadeAlpha() | 0x80808000;
        colors[0] = fadedColor;
        colors[1] = fadedColor;
        colors[2] = fadedColor;
        colors[3] = fadedColor;
        fclDrawSpriteRes(0x1A0, y, depth, colors, 0, 0xE, D_00345D88, dds3ConfigLayoutTable, ot);
        fclDrawSpriteRes(0x1D40, y, depth, colors, 0, 0xF, D_00345D88, dds3ConfigLayoutTable, ot);
        pulseColor = fclBlendColor(0x80808000, 0, phase);
        colors[0] = pulseColor;
        colors[1] = pulseColor;
        colors[2] = pulseColor;
        colors[3] = pulseColor;
        fclDrawSpriteAlpha(0x1A0, y, depth, 1, colors, 0, 0xE, D_00345D88, dds3ConfigLayoutTable, ot);
        fclDrawSpriteAlpha(0x1D40, y, depth, 1, colors, 0, 0xF, D_00345D88, dds3ConfigLayoutTable, ot);
        break;
    }
    case 0: {
        s32 color;
        s32 depth;
        color = dds3GetConfigFadeAlpha() | 0x80808000;
        colors[0] = color;
        colors[1] = color;
        colors[2] = color;
        colors[3] = color;
        depth = z << 4;
        fclDrawSpriteRes(0x1A0, y, depth, colors, 0, 0xD, D_00345D88, dds3ConfigLayoutTable, ot);
        fclDrawSpriteRes(0x1DC0, y, depth, colors, 0, 0xD, D_00345D88, dds3ConfigLayoutTable, ot);
        break;
    }
    }
}
#else
INCLUDE_ASM(const s32, "code_0029B780", dds3DrawConfigCursor);
#endif

extern u32 fclBlendColor(u32 firstColor, u32 secondColor, u32 weight);
extern void fclDrawPanelSprite2(s32 id, s32 ot);
extern void cmpDrawBoxF(s32 x, s32 y, s32 z, s32 width, s32 height, s32 color, s32 ot);

void dds3DrawConfigItem(s32 x, s32 y, s32 z, s32 ot)
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
        cmpDrawBoxF(x, y + 0x9F8, z, 0x200, 3, color0, ot);
    }
}

extern s32 fclGetPanelAnimTarget(fclPanelAnim_t *anim);

#ifdef NON_MATCHING
void dds3DrawConfigSlider(s32 x, s32 y, s32 z, s32 ot)
{
    s32 colors[4];
    s32 target = fclGetPanelAnimTarget(&((dds3ConfigWork_t *)GBWK)->panels[0]);

    fclDrawPanelSprite2(0x44, ot);
    colors[0] = 0x15043A40;
    colors[1] = 0;
    colors[2] = 0x15043A40;
    colors[3] = 0;
    cmpDrawBoxGDefault(x - target, y + 0x2B0, z, 0x9E, 0x121, colors, ot);
    colors[0] = 0;
    colors[1] = 0x80;
    colors[2] = 0;
    colors[3] = 0x80;
    cmpDrawBoxGDefault(x + target + 0x1120, y + 0x2B0, z, 0xEE, 0x121, colors, ot);
}
#else
INCLUDE_ASM(const s32, "code_0029B780", dds3DrawConfigSlider);
#endif

extern const s32 D_00548888[3][4];
extern void *memcpy(void *destination, const void *source, u32 size);

extern void dds3DrawConfigBar(s32 x, s32 y, s32 z, s32 index, s32 value, s32 ot);

/* NON_MATCHING: 4 of 113 words differ; exact 452-byte size. */
#ifdef NON_MATCHING
void dds3DrawConfigBar(s32 x, s32 y, s32 z, s32 index, s32 value, s32 ot)
{
    s32 colors[4];
    s32 selected[2];
    s32 spriteIds[3][4];
    s32 zScaled = z << 4;
    u32 alpha;

    memcpy(spriteIds, D_00548888, sizeof(spriteIds));
    alpha = dds3GetConfigFadeAlpha() | 0x80808000;

    colors[0] = alpha;
    colors[1] = alpha;
    colors[2] = alpha;
    colors[3] = alpha;
    if (value == 0) {
        selected[0] = spriteIds[index][0];
        selected[1] = spriteIds[index][3];
    } else {
        selected[0] = spriteIds[index][1];
        selected[1] = spriteIds[index][2];
    }
    fclDrawSpriteRes(x, y + 0x18, zScaled, colors, 0, (u16)selected[0],
                     D_00345D88, dds3ConfigLayoutTable, ot);
    fclDrawSpriteRes(x, y + 0x18, zScaled, colors, 0, (u16)selected[1],
                     D_00345D88, dds3ConfigLayoutTable, ot);
    fclDrawSpriteRes(0x1130, y + 0x18, zScaled, colors, 0, 0x10,
                     D_00345D88, dds3ConfigLayoutTable, ot);
}
#else
INCLUDE_ASM(const s32, "code_0029B780", dds3DrawConfigBar);
#endif

typedef struct frFontRequest_t frFontRequest_t;
typedef struct {
    const char *text;
    s32 optionCount;
} configValueLabel_t;
extern configValueLabel_t D_00528F28[];
extern void dds3DrawConfigCursor(s32 x, s32 y, s32 z, u32 width, s32 height,
                                s32 selected, s32 ot);
extern frFontRequest_t *frwGetSmallFontFRQ(s32 x, s32 y, s32 priority, s32 color,
                                         const char *text, frFontRequest_t *previous);
extern s32 func_001C7E38(frFontHandle_t *request, s8 mode, u32 ot);
extern s32 frDestroyFontRequestAfter(frFontHandle_t *request);

void dds3DrawConfigValue(s32 x, s32 y, s32 z, s32 index, s32 selected, s32 ot)
{
    s32 color[4];
    s32 textColor;
    s32 alphaColor;
    s32 zScaled = z << 4;
    s32 selectedTextColor = 0x8A8AB380;
    u8 fadeAlpha;
    frFontRequest_t *font;

    fadeAlpha = dds3GetConfigFadeAlpha();
    if (selected != 0) {
        selectedTextColor = 0xA9F3FF80;
    }
    textColor = (selectedTextColor & ~0xFF) | fadeAlpha;
    alphaColor = dds3GetConfigFadeAlpha() | 0x80808000;
    color[0] = alphaColor;
    color[1] = alphaColor;
    color[2] = alphaColor;
    color[3] = alphaColor;
    fclDrawSpriteRes(x, y, zScaled, color, 0, 8, D_00345D88, dds3ConfigLayoutTable, ot);
    fclDrawSpriteRes(x, y, zScaled, color, 0, 9, D_00345D88, dds3ConfigLayoutTable, ot);
    dds3DrawConfigCursor(0x1B0, y + 0x30, z, 0x1CA, 9, selected, ot);
    font = frwGetSmallFontFRQ(x + 0x340, y + 8, z, textColor,
                              D_00528F28[index].text, NULL);
    func_001C7E38((frFontHandle_t *)font, 1, ot);
    frDestroyFontRequestAfter((frFontHandle_t *)font);
}

/* NON_MATCHING: 41 of 145 words differ, first at +0x50; exact 580-byte size. */
#ifdef NON_MATCHING
void dds3DrawConfigText(s32 x, s32 y, s32 z, s32 count, s32 index, s32 value, s32 ot)
{
    s32 drawOptions = index < 3;
    s32 selected = index == (*(s16 *)(GBWK + 0x3E) + GBWK[0x40]);
    s32 colors[4];
    s32 color;
    s32 startY;
    s32 i;

    dds3DrawConfigValue(x, y, z, index, selected, ot);
    if (drawOptions && value >= 0)
        dds3DrawConfigBar(x, y, z, index, value, ot);
    color = dds3GetConfigFadeAlpha() | 0x80808000;
    colors[0] = color;
    colors[1] = color;
    colors[2] = color;
    colors[3] = color;
    if (drawOptions) {
        y += 0xA8;
        startY = y;
        for (i = 0; i < count; i++) {
            fclDrawSpriteFilter(x, y, z << 4, 0, colors, 0, 0,
                               D_00345D88, dds3ConfigLayoutTable, ot);
            fclDrawSpriteFilter(x, y, z << 4, 0, colors, 0, 1,
                               D_00345D88, dds3ConfigLayoutTable, ot);
            y += 0xB0;
        }
        dds3DrawConfigLabel(x + 0x540, startY + 8, z, index, selected, ot);
        fclDrawSpriteRes(0, y, z << 4, colors, 0, 10, D_00345D88, dds3ConfigLayoutTable, ot);
        fclDrawSpriteRes(0, y, z << 4, colors, 0, 11, D_00345D88, dds3ConfigLayoutTable, ot);
        fclDrawSpriteRes(0, y, z << 4, colors, 0, 12, D_00345D88, dds3ConfigLayoutTable, ot);
    }
}
#else
INCLUDE_ASM(const s32, "code_0029B780", dds3DrawConfigText);
#endif

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

extern void fclDrawPanelSprite(s32 id, s32 ot);
extern void dds3DrawConfigSlider(s32 x, s32 y, s32 z, s32 ot);
extern void dds3DrawConfigText(s32 x, s32 y, s32 z, s32 count, s32 index, s32 value, s32 ot);

/* NON_MATCHING: 5 of 150 words differ; exact 600-byte size. */
#ifdef NON_MATCHING
void dds3DrawConfigPanel(s32 ot)
{
    s32 colors[4];
    s32 color;

    fclDrawPanelSprite(0x3000F, ot);
    if (((dds3ConfigWork_t *)GBWK)->textureHandle != NULL) {
        dds3DrawConfigBackground(ot);
    }
    dds3DrawConfigSlider(0, -0x48, 0, ot);
    dds3DrawConfigText(0, 0x268, 0, 2, 0,
                        dds3GetConfigFlag(0, (s32 *)(DDS3_GBWK + 0x11AB8)), ot);
    dds3DrawConfigText(0, 0x4B0, 0, 3, 1,
                        dds3GetConfigFlag(1, (s32 *)(DDS3_GBWK + 0x11AB8)), ot);
    dds3DrawConfigText(0, 0x7A8, 0, 3, 2,
                        dds3GetConfigFlag(2, (s32 *)(DDS3_GBWK + 0x11AB8)), ot);
    dds3DrawConfigText(0, 0xAB0, 0, 1, 3, -1, ot);
    color = dds3GetConfigFadeAlpha() | 0x80808000;
    colors[0] = color;
    colors[1] = color;
    colors[2] = color;
    colors[3] = color;
    fclDrawSpriteRes(0, -0x110, 0, colors, 0, 2, D_00345D88, dds3ConfigLayoutTable, ot);
    fclDrawSpriteRes(0, -0x110, 0, colors, 0, 3, D_00345D88, dds3ConfigLayoutTable, ot);
    fclDrawSpriteRes(0, -0x110, 0, colors, 0, 4, D_00345D88, dds3ConfigLayoutTable, ot);
    fclDrawSpriteRes(0, -0x80, 0, colors, 0, 5, D_00345D88, dds3ConfigLayoutTable, ot);
    fclDrawSpriteRes(0, -0x80, 0, colors, 0, 6, D_00345D88, dds3ConfigLayoutTable, ot);
    fclDrawSpriteRes(0, -0x80, 0, colors, 0, 7, D_00345D88, dds3ConfigLayoutTable, ot);
    dds3DrawConfigItem(0, 0x1D8, 0, ot);
}

#else
INCLUDE_ASM(const s32, "code_0029B780", dds3DrawConfigPanel);
#endif

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

