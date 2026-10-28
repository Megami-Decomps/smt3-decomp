#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"

typedef struct fclPanel
{
    /* 0x00 */ s32 X;
    /* 0x04 */ s32 Y;
    /* 0x08 */ s32 Z;
    /* 0x0C */ s16 W;
    /* 0x0E */ s16 H;
    /* 0x10 */ s32 Col;
    /* 0x14 */ u8 Spd;
    /* 0x15 */ u8 unk_15[3];
    /* 0x18 */ s32 Timer;
    /* 0x1C */ s32 Timer2;
    /* 0x20 */ s32 Flag;
    /* 0x24 */ s32 unk_24;
    /* 0x28 */ s32 unk_28;
    /* 0x2C */ s32 unk_2C;
    /* 0x30 */ s32 unk_30;
    /* 0x34 */ s32 TexHandle;
    /* 0x38 */ void (*draw)(struct fclPanel *panel, s32 id);
    /* 0x3C */ void (*update)(struct fclPanel *panel);
} fclPanel_t;

extern void sdfChipFreeDirect(void *aPtr);

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

extern s32 fclDrawPanelSprite2(s32 id, s32 ot);
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

INCLUDE_ASM(const s32, "facility/fclPanel", fclDrawPanelType0);

void fclUpdatePanelType0(fclPanel_t *pPanel)
{
    pPanel->Timer += pPanel->Spd;
    if (pPanel->Timer > 0x100) {
        pPanel->Timer = 0x100;
    }
}

INCLUDE_ASM(const s32, "facility/fclPanel", fclDrawPanelType1);

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

typedef struct
{
    s32 tex;
    u16 width;
    u16 height;
    u16 u0;
    u16 v0;
    u16 u1;
    u16 v1;
    s32 X;
    s32 Y;
    s32 Z;
    u8 color[4];
    u8 flags;
} fclSprite_t;

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
extern s32 cmpDrawBoxGDefault(s32 x, s32 y, s32 z, s32 w, s32 h,
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

INCLUDE_ASM(const s32, "facility/fclPanel", fclDrawPanelType3);

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
s32 fclDrawPanel(fclPanel_t *pPanel, s32 OtNo)
{
    fclDrawPanelSprite(0x53001, OtNo);
    pPanel->draw(pPanel, OtNo);
    return fclDrawPanelSprite(0x5101B, OtNo);
}
#else
INCLUDE_ASM(const s32, "facility/fclPanel", fclDrawPanel);
#endif

void fclUpdatePanel(fclPanel_t *pPanel)
{
    pPanel->update(pPanel);
}
