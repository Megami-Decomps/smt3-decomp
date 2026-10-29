#include "common.h"
#include "facility.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"

extern void sdfChipFreeDirect(void *aPtr);

#ifdef NON_MATCHING
extern void cmpDrawBoxF(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
extern void fclDrawPanelSprite2(s32 id, s32 ot);
#endif

void fclDrawPanelTypeDef(fclPanel_t *pPanel, s32 OtNo)
{
    s32 color = pPanel->Col;

    fclDrawPanelSprite2(0x44, OtNo);
    cmpDrawBoxF(pPanel->X, pPanel->Y, pPanel->Z, (u16)pPanel->W,
                (u16)pPanel->H, color, OtNo);
}

void fclUpdatePanelTypeDef(fclPanel_t *pPanel)
{
    pPanel->Timer += pPanel->Spd;
    if (pPanel->Timer > 0x100) {
        pPanel->Timer = 0x100;
    }
}

extern void fclDrawPanelSprite2(s32 id, s32 ot);
extern void cmpDrawTriG(const u32 *xs, const u32 *ys, u32 z, const u32 *colors, u32 ot);

/* NON_MATCHING: 6 of 64 words differ in the second color-constant schedule. */
#ifdef NON_MATCHING
void fclDrawPanelType0Grad(fclPanel_t *panel, u32 ot)
{
    u32 coords[2][3];
    u32 colors[3];

    fclDrawPanelSprite2(0x48, ot);

    colors[0] = 0x546D8B33;
    colors[1] = 0x33;
    colors[2] = colors[0] & 0xFF;
    coords[0][0] = 0;
    coords[1][0] = 0;
    coords[0][1] = 0x30C0;
    coords[1][1] = 0;
    coords[0][2] = 0;
    coords[1][2] = 0xC80;

    cmpDrawTriG(coords[0], coords[1], panel->Z, colors, ot);

    colors[0] = 0x31486366;
    colors[1] = 0x66;
    colors[2] = colors[0] & 0xFF;
    coords[0][0] = 0x2000;
    coords[1][0] = 0xF00;
    coords[0][1] = (u32)-0x10C0;
    coords[1][1] = 0xF00;
    coords[0][2] = 0x2000;
    coords[1][2] = 0x280;

    cmpDrawTriG(coords[0], coords[1], panel->Z, colors, ot);

    fclDrawPanelSprite2(0x44, ot);
}
#else
INCLUDE_ASM(const s32, "facility/fclPanel", fclDrawPanelType0Grad);
#endif

extern u32 fclBlendColor(u32 firstColor, u32 secondColor, u32 weight);
extern void cmpDrawBoxGDefault(s32 x, s32 y, s32 z, s32 width, s32 height,
                              s32 *colors, s32 ot);

extern void cmpDrawBoxF(s32 x, s32 y, s32 z, s32 width, s32 height, s32 color, s32 ot);
extern void fclDrawPanelType0Grad(fclPanel_t *panel, u32 ot);

/* NON_MATCHING: 57 of 229 words differ at the native 916-byte size. */
#ifdef NON_MATCHING
void fclDrawPanelType0(fclPanel_t *panel, s32 ot)
{
    s32 value = 100;
    u32 topLight = 0x020D1240;
    u32 topDark = 0x020D1200;
    u32 blendBase = 0x0602253C;
    s32 alphaMax = 0x100;
    s32 timer = panel->Timer;
    s32 h = (u16)panel->H;
    s32 width = ((h * 5) << 4) / value;
    s32 t = (alphaMax - timer) * width;
    s32 curve = t / 0x100;
    f32 root = fsqrtf((f32)curve);
    s32 height = h - 8;
    s32 color;
    s32 colors[4];
    s32 y;
    s32 originalY;

    curve = (s32)(root * root);
    curve = width - curve;

    alphaMax = 0x66;
    value = 0x11171800;
    color = value | ((u32)(timer * alphaMax) >> 8);
    fclDrawPanelSprite2(0x44, ot);
    cmpDrawBoxF(0, panel->Y + 0x20, panel->Z, 0x200, height, color, ot);

    originalY = panel->Y;
    y = originalY - 0xB0;
    height = curve + 0x22;
    colors[0] = topDark;
    colors[1] = topDark;
    colors[2] = topLight;
    colors[3] = topLight;
    fclDrawPanelSprite2(0x44, ot);
    cmpDrawBoxGDefault(0, y, panel->Z, 0x200, 0x16, colors, ot);
    y = originalY + 0x20;
    fclDrawPanelSprite2(0x44, ot);
    cmpDrawBoxF(0, originalY, panel->Z, 0x200, 4, 0x80, ot);
    fclDrawPanelSprite2(0x48, ot);
    cmpDrawBoxF(0, y, panel->Z, 0x200, 4, 0x1F252B80, ot);
    y = originalY + 0x40;

    color = fclBlendColor(0x3C, blendBase, 0x100 - panel->Timer);
    colors[0] = color;
    colors[1] = color;
    colors[2] = color & 0xFF;
    colors[3] = color & 0xFF;
    fclDrawPanelSprite2(0x48, ot);
    cmpDrawBoxGDefault(0, y, panel->Z, 0x200, 0x22, colors, ot);

    color = fclBlendColor(0x3C, blendBase, panel->Timer);
    colors[0] = color;
    colors[1] = color;
    colors[2] = color & 0xFF;
    colors[3] = color & 0xFF;
    fclDrawPanelSprite2(0x48, ot);
    cmpDrawBoxGDefault(0, y, panel->Z, 0x200, height, colors, ot);

    h = (u16)panel->H;
    y = panel->Y;
    y += (h - 8) << 3;
    y -= 0x110;
    colors[0] = 0x3C;
    colors[1] = 0x3C;
    colors[2] = blendBase;
    colors[3] = blendBase;
    fclDrawPanelSprite2(0x48, ot);
    cmpDrawBoxGDefault(0, y, panel->Z, 0x200, 0x22, colors, ot);
    y += 0x110;
    fclDrawPanelSprite2(0x48, ot);
    cmpDrawBoxF(0, y, panel->Z, 0x200, 4, 0x1F252B80, ot);
    y += 0x20;
    fclDrawPanelSprite2(0x44, ot);
    cmpDrawBoxF(0, y, panel->Z, 0x200, 4, 0x80, ot);
    y += 0x20;
    colors[0] = topLight;
    colors[1] = topLight;
    colors[2] = topDark;
    colors[3] = topDark;
    fclDrawPanelSprite2(0x44, ot);
    cmpDrawBoxGDefault(0, y, panel->Z, 0x200, 0x16, colors, ot);
    fclDrawPanelSprite2(0x44, ot);
    fclDrawPanelType0Grad(panel, ot);
}
#else
INCLUDE_ASM(const s32, "facility/fclPanel", fclDrawPanelType0);
#endif

void fclUpdatePanelType0(fclPanel_t *pPanel)
{
    pPanel->Timer += pPanel->Spd;
    if (pPanel->Timer > 0x100) {
        pPanel->Timer = 0x100;
    }
}


extern s32 D_00345D88[];
extern fclSpriteTbl_t fclSpriteTable[];
extern void cmpDrawBoxF(s32 x, s32 y, s32 z, s32 width, s32 height, s32 color, s32 ot);
extern void fclDrawSpriteFilter(s32 x, s32 y, s32 z, u8 filter, s32 *color,
                                s32 id, s32 spriteId, s32 *textures,
                                fclSpriteTbl_t *table, s32 ot);
extern void fclDrawSpriteSizeFilter(s32 x, s32 y, s32 z, s32 width, s32 height,
                                    u8 filter, s32 *color, s32 id, u16 spriteId,
                                    s32 *textures, fclSpriteTbl_t *table, s32 ot);
extern void fclDrawPanelBorder(fclPanel_t *panel, s32 z, s32 type, s32 ot);

#ifdef NON_MATCHING
void fclDrawPanelType1(fclPanel_t *panel, s32 ot)
{
    s32 border = (panel->Flag & 0x10000) != 0 ? 0x18 : 6;
    s32 width = (u16)panel->W;
    s32 z = panel->Z << 4;

    fclDrawPanelSprite2(0x44, ot);
    cmpDrawBoxF(panel->X, panel->Y, panel->Z, border, (u16)panel->H, 0x13141559, ot);
    cmpDrawBoxF(panel->X + ((border + 0x13) << 4), panel->Y, panel->Z,
                width - border - 0x13, (u16)panel->H, 0x13141559, ot);
    fclDrawSpriteFilter(panel->X + (border << 4), panel->Y, z, 0, NULL,
                        10, 3, D_00345D88, fclSpriteTable, ot);
    fclDrawSpriteSizeFilter(panel->X + (border << 4), panel->Y + 0xC8, z, 0x13,
                            (u16)panel->H - 0x32, 0, NULL, 10, 5,
                            D_00345D88, fclSpriteTable, ot);
    fclDrawSpriteFilter(panel->X + (border << 4), panel->Y + (((u16)panel->H - 0x19) << 3),
                        z, 0, NULL, 10, 4, D_00345D88, fclSpriteTable, ot);
    fclDrawPanelBorder(panel, z, border, ot);
}
#else
INCLUDE_ASM(const s32, "facility/fclPanel", fclDrawPanelType1);
#endif

void fclUpdatePanelType1(fclPanel_t *pPanel)
{
    pPanel->Timer += pPanel->Spd;
    if (pPanel->Timer > 0x200) {
        pPanel->Timer = 0;
    }
}

void fclDrawPanelType2(fclPanel_t *panel, s32 ot)
{
    s32 color = panel->Col;

    fclDrawPanelSprite2(0x44, ot);
    cmpDrawBoxF(panel->X, panel->Y, panel->Z, (u16)panel->W,
                (u16)panel->H - 0x1A, color, ot);
    cmpDrawBoxF(panel->X,
                       panel->Y + (s32)((u32)((u16)panel->H - 0x1A) << 3), panel->Z,
                       (u16)panel->W, 0x1A, 0x3151A40, ot);
}

void fclUpdatePanelType2(fclPanel_t *pPanel)
{
    pPanel->Timer += pPanel->Spd;
    if (pPanel->Timer > 0x100) {
        pPanel->Timer = 0x100;
    }
}


extern void fclSetSprite(fclSprite_t *sprite, s32 width, s32 height, s32 z,
                         s32 u0, s32 v0, s32 u1, s32 v1, s32 color,
                         s32 texture);
extern void fclDrawSpriteCore(s32 x, s32 y, s32 width, s32 height,
                              s32 *color, fclSprite_t *sprite, u8 flip,
                              u8 alpha, u8 filter, s32 ot);

void fclDrawPanelCorners(fclPanel_t *panel, s32 ot)
{
    fclSprite_t sprite;
    s32 width = (u16)panel->W - 0x6B;
    s32 bottom = (u16)panel->H - 3;
    s32 rightX;

    fclSetSprite(&sprite, 0x49, 4, panel->Z << 4, 0x94, 0x44,
                 0xDD, 0x48, 0x80808080, panel->TexHandle);
    rightX = panel->X + (width << 4);
    fclDrawSpriteCore(rightX, panel->Y, 0x49, 4, NULL,
                      &sprite, 0, 0, 1, ot);
    fclDrawSpriteCore(rightX, panel->Y + (bottom << 3), 0x49, 4, NULL,
                      &sprite, 2, 0, 1, ot);

    fclSetSprite(&sprite, width, 3, panel->Z << 4, 0x94, 0x44,
                 0x95, 0x48, 0x80808080, panel->TexHandle);
    fclDrawSpriteCore(panel->X, panel->Y, width, 4, NULL, &sprite,
                      0, 0, 1, ot);
    fclDrawSpriteCore(panel->X, panel->Y + (bottom << 3), width, 4, NULL,
                      &sprite, 2, 0, 1, ot);
}

extern s32 fclRagCalcTexSize(s32 size, s32 mode);
extern void cmpDrawBoxGDefault(s32 x, s32 y, s32 z, s32 w, s32 h,
                              s32 *colors, s32 ot);

void fclDrawPanelType3BackGrad(fclPanel_t *panel, s32 ot)
{
    s32 colors[4];
    s32 color = 40;
    s32 width = (u16)panel->W - 70;
    s32 texSize = fclRagCalcTexSize(100, 0x200 - panel->Timer2);
    s32 heightOffset = 30;

    colors[0] = 40;
    colors[1] = 0;
    colors[2] = 40;
    colors[3] = 0;
    cmpDrawBoxF(-texSize, panel->Y - 120, panel->Z, width,
                (u16)panel->H + heightOffset, color, ot);
    cmpDrawBoxGDefault((width << 4) - texSize, panel->Y - 120,
                       panel->Z, 60, (u16)panel->H + heightOffset,
                       colors, ot);
}

void fclDrawPanelEdges(fclPanel_t *panel, s32 ot)
{
    struct
    {
        fclSprite_t leftEdge;
        fclSprite_t rightEdge;
    } edges;
    s32 y;

    fclSetSprite(&edges.leftEdge, 53, 29, panel->Z << 4,
                 3, 32, 56, 61, 0x80808080, panel->TexHandle);
    fclSetSprite(&edges.rightEdge, 244, 29, panel->Z << 4,
                 3, 1, 247, 30, 0x80808080, panel->TexHandle);

    y = panel->Y + 48;
    while (y < panel->Y + (((u16)panel->H - 9) << 3)) {
        fclDrawSpriteCore(panel->X, y, 53, 29, NULL, &edges.leftEdge,
                          0, 0, 1, ot);
        fclDrawSpriteCore(panel->X + 0x350, y, 244, 29, NULL, &edges.rightEdge,
                          0, 0, 1, ot);
        y += 0xE8;
    }
}

#ifdef NON_MATCHING
void fclDrawPanelType3(fclPanel_t *panel, s32 ot)
{
    s32 z = panel->Z << 4;

    if (panel->TexHandle != 0) {
        fclDrawPanelType3BackGrad(panel, ot);
        fclDrawPanelCorners(panel, ot);
        fclDrawPanelEdges(panel, ot);
    }
    fclDrawSpriteFilter(panel->X + 0x170, panel->Y + 0x30, z, 0, NULL,
                        10, 13, D_00345D88, fclSpriteTable, ot);
    fclDrawSpriteSizeFilter(panel->X + 0x170, panel->Y + 0xF8, z, 0x13,
                            (u16)panel->H - 0x3E, 0, NULL, 10, 15,
                            D_00345D88, fclSpriteTable, ot);
    fclDrawSpriteFilter(panel->X + 0x170, panel->Y + (((u16)panel->H - 0x1F) << 3),
                        z, 0, NULL, 10, 14, D_00345D88, fclSpriteTable, ot);
    fclDrawPanelBorder(panel, z, 0x17, ot);
}
#else
INCLUDE_ASM(const s32, "facility/fclPanel", fclDrawPanelType3);
#endif

void fclUpdatePanelType3(fclPanel_t *pPanel)
{
    pPanel->Timer += pPanel->Spd;
    if (pPanel->Timer >= 0x201) {
        pPanel->Timer = 0;
    }
    pPanel->Timer2 += 0x20;
    if (pPanel->Timer2 >= 0x201) {
        pPanel->Timer2 = 0x200;
    }
}

void fclSetPanelTex(fclPanel_t *pPanel, s32 UV1, s32 UV2, s32 UV3, s32 UV4, s32 TexHandle)
{
    pPanel->unk_24 = UV1;
    pPanel->unk_28 = UV2;
    pPanel->unk_2C = UV3;
    pPanel->unk_30 = UV4;
    pPanel->TexHandle = TexHandle;
}

extern void fclDrawPanelType0(fclPanel_t *panel, s32 ot);
extern void fclDrawPanelType1(fclPanel_t *panel, s32 ot);
extern void fclDrawPanelType3(fclPanel_t *panel, s32 ot);

#ifdef NON_MATCHING
extern void *memset(void *a, s32 b, u32 c);
#endif

void fclInitPanel(fclPanel_t *panel, s32 x, s32 y, s32 z, s32 w,
                  s32 h, s32 color, s8 type, s8 speed)
{
    memset(panel, 0, sizeof(*panel));
    panel->X = x;
    panel->Y = y;
    panel->Z = z;
    panel->W = w;
    panel->H = h;
    panel->Col = color;
    panel->Spd = speed;
    panel->Timer = 0;
    panel->Timer2 = 0;
    panel->TexHandle = 0;

    switch (type) {
    case 0:
        panel->draw = fclDrawPanelType0;
        panel->update = fclUpdatePanelType0;
        break;
    case 1:
        panel->draw = fclDrawPanelType1;
        panel->update = fclUpdatePanelType1;
        break;
    case 2:
        panel->draw = fclDrawPanelType2;
        panel->update = fclUpdatePanelType2;
        break;
    case 3:
        panel->draw = fclDrawPanelType3;
        panel->update = fclUpdatePanelType3;
        break;
    default:
        panel->draw = fclDrawPanelTypeDef;
        panel->update = fclUpdatePanelTypeDef;
        break;
    }
}

extern void *sdfChipAllocDirect(s32 size);

fclPanel_t *fclCreatePanel(s32 x, s32 y, s32 z, s32 w, s32 h, s32 color,
                           s8 type, s8 speed)
{
    fclPanel_t *panel = sdfChipAllocDirect(sizeof(*panel));

    fclInitPanel(panel, x, y, z, w, h, color, type, speed);
    return panel;
}

s32 fclDestroyPanel(void *pPanel)
{
    sdfChipFreeDirect(pPanel);
    return 1;
}

/* NON_MATCHING: 8 of 21 words differ (first at +0x10). */
#ifdef NON_MATCHING
extern void fclDrawPanelSprite(s32 id, s32 arg);

void fclDrawPanel(fclPanel_t *pPanel, s32 OtNo)
{
    fclDrawPanelSprite(0x53001, OtNo);
    pPanel->draw(pPanel, OtNo);
    fclDrawPanelSprite(0x5101B, OtNo);
}
#else
INCLUDE_ASM(const s32, "facility/fclPanel", fclDrawPanel);
#endif

void fclUpdatePanel(fclPanel_t *pPanel)
{
    pPanel->update(pPanel);
}
