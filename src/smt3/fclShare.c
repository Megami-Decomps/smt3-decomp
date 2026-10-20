#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"

extern void sdfMemFreeDirect(void *ptr);

typedef struct
{
    /* 0x00 */ u8 animState;
    /* 0x01 */ u8 unk_01[3];
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ u8 unk_08[0xC];
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ s32 unk_1C;
} fclPanelAnim_t;

typedef struct
{
    /* 0x00 */ u16 x;
    /* 0x02 */ u16 y;
    /* 0x04 */ u16 u0;
    /* 0x06 */ u16 v0;
    /* 0x08 */ u16 u1;
    /* 0x0A */ u16 v1;
} fclSpriteRect_t;

typedef struct
{
    /* 0x00 */ s32 tex;
    /* 0x04 */ u16 unk_04;
    /* 0x06 */ u16 unk_06;
    /* 0x08 */ u16 unk_08;
    /* 0x0A */ u16 unk_0A;
    /* 0x0C */ u16 unk_0C;
    /* 0x0E */ u16 unk_0E;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ u8 color[4];
    /* 0x20 */ u8 unk_20;
} fclSprite_t;

extern void fclDrawSpriteCore(s32 x, s32 y, s32 w, s32 h, s32 *color, fclSprite_t *sprite,
                              u8 flags, u8 mode, u8 filter, s32 ot);

extern void fclDrawSpriteAngleCore(s32 x, s32 y, s32 w, s32 h, f32 angle, s32 *color,
                                   fclSprite_t *sprite, u8 flags, u8 mode, u8 filter, s32 ot);

extern void fclSetSprite(fclSprite_t *sprite, s32 x, s32 y, s32 d, s32 u0, s32 v0, s32 u1,
                         s32 v1, s32 color, s32 tex);

extern void fclDrawSpriteEx(s32 a, s32 b, s32 w, s32 h, fclSprite_t *sprite, s32 flags);

extern void fclRunPanelAnim(void *work);

extern sdfGrOt_t sdfGrOtTable[85];

typedef struct
{
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ u8 unk_01[3];
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s32 unk_0C;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ s32 unk_1C;
    /* 0x20 */ void (*calcSizes)();
} fclRagTex_t;

extern void fclRagTexInit(fclRagTex_t *tex, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                          void (*calc)());

extern void fclRagTexCalcSizes();

/* Called above its definition, where ee-gcc assumed int. */
void *sdfDevLoadFileEx(char *name, void **data, s32 *size);

extern void fclDrawPhaseHeader(u8 *work, s32 color, s32 mode, s32 id);

extern s32 fclGetPanelAnimTarget(fclPanelAnim_t *anim);

extern s32 fclGetPanelAnimPhase(fclPanelAnim_t *anim);

extern void *sdfDevLoadFileEx(char *name, void **data, s32 *size);

extern s32 sdfTexSet(void *data);

s32 fclLoadTexture(char *name)
{
    void *data;
    s32 size;
    void *file;
    s32 handle;

    file = sdfDevLoadFileEx(name, &data, &size);
    handle = sdfTexSet(data);
    sdfMemFreeDirect(file);
    return handle;
}

void fclSetSpriteColor(u8 *sprite, u32 color)
{
    sprite[0x1C] = color >> 24;
    sprite[0x1D] = color >> 16;
    sprite[0x1E] = color >> 8;
    sprite[0x1F] = color;
}

void fclSetSpriteEx(fclSprite_t *sprite, fclSpriteRect_t *rect, s32 a, s32 b, s32 d, s32 color,
                    s32 e, s32 tex)
{
    fclSpriteRect_t *r;
    s32 u0;
    s32 v0;
    s32 u1;

    sprite->unk_10 = a << 4;
    sprite->tex = tex;
    sprite->unk_14 = b << 3;
    sprite->unk_18 = d;
    sprite->unk_20 = e;
    fclSetSpriteColor((u8 *)sprite, color);
    r = (fclSpriteRect_t *)&sprite->unk_04;
    *r = *rect;
    if (r->x == 0) {
        r->x = sprite->unk_0C;
    }
    if (sprite->unk_06 == 0) {
        sprite->unk_06 = sprite->unk_0E;
    }
    u0 = sprite->unk_08;
    v0 = sprite->unk_0A;
    u1 = sprite->unk_0C;
    sprite->unk_08 = u0 << 4;
    sprite->unk_0A = v0 << 4;
    sprite->unk_0C = (u1 + u0) << 4;
    sprite->unk_0E = (sprite->unk_0E + v0) << 4;
}

void fclSetSprite(fclSprite_t *sprite, s32 x, s32 y, s32 d, s32 u0, s32 v0, s32 u1, s32 v1,
                  s32 color, s32 tex)
{
    u0 <<= 4;
    v0 <<= 4;
    u1 <<= 4;
    v1 <<= 4;
    sprite->unk_04 = x;
    sprite->unk_0E = v1;
    sprite->tex = tex;
    sprite->unk_18 = d;
    sprite->unk_06 = y;
    sprite->unk_08 = u0;
    sprite->unk_0A = v0;
    sprite->unk_0C = u1;
    sprite->unk_14 = 0;
    sprite->unk_10 = 0;
    fclSetSpriteColor((u8 *)sprite, color);
}

void fclSetSpriteNormal(fclSprite_t *a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h)
{
    fclSetSprite(a, b, c, d, e, f, e + b, f + c, g, h);
}

void fclDrawSprite(s32 a, s32 b, fclSprite_t *sprite)
{
    fclDrawSpriteEx(a, b, sprite->unk_04, sprite->unk_06, sprite, 0x47);
}

INCLUDE_ASM(const s32, "fclShare", fclDrawSpriteEx);

/* NON_MATCHING: 111 of 180 words differ (first at +0x20). */
#ifdef NON_MATCHING
void fclDrawSpriteCore(s32 x, s32 y, s32 w, s32 h, s32 *color, fclSprite_t *sprite, u8 flags,
                       u8 mode, u8 filter, s32 ot)
{
    sdfGrOt_t *otp;
    void *packet;
    u64 *prim;
    void *tag;
    u32 uv[4];
    u32 x0;
    u32 y0;
    u32 x1;
    u32 y1;
    u32 t;
    u32 c;

    if (filter == 0) {
        sdfTexSetFilter(sprite->tex, 0, 1);
    } else {
        sdfTexSetFilter(sprite->tex, 1, 1);
    }
    packet = (void *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(5, 1));
    sdf2DMakePrimPacketHeader(packet, 0x156, 5, 0x43431, 1);
    prim = (u64 *)sdf2DGetPrimPtr(packet);
    uv[0] = sprite->unk_08 + 8;
    uv[1] = sprite->unk_0A + 8;
    uv[2] = sprite->unk_0C + 8;
    uv[3] = sprite->unk_0E + 8;
    x0 = x + 0x6FF8;
    y0 = y + 0x78FC;
    x1 = x0 + (w << 4);
    y1 = y0 + (h << 3);
    if ((flags & 1) != 0) {
        t = x0;
        x0 = x1;
        x1 = t;
    }
    if ((flags & 2) != 0) {
        t = y0;
        y0 = y1;
        y1 = t;
    }
    if (color == NULL) {
        prim[0] = sprite->color[0] | ((u64)sprite->color[1] << 32);
        prim[1] = sprite->color[2] | ((u64)sprite->color[3] << 32);
    } else {
        c = *(u32 *)color;
        prim[1] = ((c >> 8) & 0xFF) | ((u64)(c & 0xFF) << 32);
        prim[0] = ((c >> 24) & 0xFF) | ((u64)((c >> 16) & 0xFF) << 32);
    }
    prim[2] = uv[0] | ((u64)uv[1] << 32);
    prim[4] = x1 | ((u64)y1 << 32);
    prim[5] = sprite->unk_18;
    prim[6] = uv[2] | ((u64)uv[3] << 32);
    prim[8] = x0 | ((u64)y0 << 32);
    prim[9] = sprite->unk_18;
    switch (mode) {
    case 1:
        fclDrawPanelSprite2(0x48, ot);
        break;
    case 2:
        fclDrawPanelSprite2(0x42, ot);
        break;
    default:
        fclDrawPanelSprite2(0x44, ot);
        break;
    }
    tag = (void *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    sdf2DLinkTexPacket(tag, sprite->tex, 0);
    sdfDrawTagLink(tag, packet);
    otp = &sdfGrOtTable[ot];
    otp->regFunc(otp, tag);
    fclDrawPanelSprite2(0x44, ot);
}
#else
INCLUDE_ASM(const s32, "fclShare", fclDrawSpriteCore);
#endif

extern f32 sdfSin(f32 angle);

extern f32 sdfCos(f32 angle);

void fclCalcRotPos(s32 *x, s32 *y, f32 angle, s32 w, s32 h, s32 ox, s32 oy)
{
    f32 rad = angle * 0.0174532925f;
    f32 hh = h * 0.875f;
    f32 hw = w / 2;

    *x = sdfCos(rad) * hw - sdfSin(rad) * hh;
    *y = sdfSin(rad) * hw + sdfCos(rad) * hh;
    *x = *x * 2 + ox;
    *y = *y / 0.875f + oy;
}

typedef struct
{
    /* 0x0 */ s32 x;
    /* 0x4 */ s32 y;
} fclRotPoint_t;

extern void fclCalcRotPos(s32 *x, s32 *y, f32 angle, s32 w, s32 h, s32 ox, s32 oy);

/* NON_MATCHING: 86 of 298 words differ at the exact size: the angle `mov.s`
 * scheduled out of three call delay slots, one byte of the colour read as a
 * shift rather than a load, and the quadword block hoisted. */
#ifdef NON_MATCHING
void fclDrawSpriteAngleCore(s32 x, s32 y, s32 w, s32 h, f32 angle, s32 *color,
                            fclSprite_t *sprite, u8 flags, u8 mode, u8 filter, s32 ot)
{
    sdfGrOt_t *otp;
    void *packet;
    u64 *prim;
    void *tag;
    u32 uv[4];
    fclRotPoint_t pt[4];
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    s32 hw;
    s32 hh;
    s32 cx;
    s32 cy;
    s32 t;
    u32 c;

    if (filter == 0) {
        sdfTexSetFilter(sprite->tex, 0, 1);
    } else {
        sdfTexSetFilter(sprite->tex, 1, 1);
    }
    packet = (void *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(3, 4));
    sdf2DMakePrimPacketHeader(packet, 0x15C, 3, 0x531, 4);
    prim = (u64 *)sdf2DGetPrimPtr(packet);
    x0 = x + 0x7000;
    y0 = y + 0x7900;
    x1 = x0 + (w << 4);
    y1 = y0 + (h << 3);
    hw = (x1 - x0) / 2;
    hh = (y1 - y0) / 2;
    cx = x0 + hw;
    cy = y0 + hh;
    pt[0].x = x0;
    pt[0].y = y0;
    pt[3].x = x1;
    pt[3].y = y1;
    fclCalcRotPos(&pt[0].x, &pt[0].y, angle, -hw, -hh, cx, cy);
    fclCalcRotPos(&pt[1].x, &pt[1].y, angle, hw, -hh, cx, cy);
    fclCalcRotPos(&pt[2].x, &pt[2].y, angle, -hw, hh, cx, cy);
    fclCalcRotPos(&pt[3].x, &pt[3].y, angle, hw, hh, cx, cy);
    uv[0] = sprite->unk_08;
    uv[2] = sprite->unk_0C;
    uv[1] = sprite->unk_0A;
    uv[3] = sprite->unk_0E;
    if ((flags & 1) != 0) {
        t = pt[0].x;
        pt[0].x = pt[1].x;
        pt[1].x = t;
        t = pt[2].x;
        pt[2].x = pt[3].x;
        pt[3].x = t;
    }
    if ((flags & 2) != 0) {
        t = pt[0].y;
        pt[0].y = pt[1].y;
        pt[1].y = t;
        t = pt[2].y;
        pt[2].y = pt[3].y;
        pt[3].y = t;
    }
    if (color == NULL) {
        prim[0] = sprite->color[0] | ((u64)sprite->color[1] << 32);
        prim[1] = sprite->color[2] | ((u64)sprite->color[3] << 32);
        prim[6] = sprite->color[0] | ((u64)sprite->color[1] << 32);
        prim[7] = sprite->color[2] | ((u64)sprite->color[3] << 32);
        prim[12] = sprite->color[0] | ((u64)sprite->color[1] << 32);
        prim[13] = sprite->color[2] | ((u64)sprite->color[3] << 32);
        prim[18] = sprite->color[0] | ((u64)sprite->color[1] << 32);
        prim[19] = sprite->color[2] | ((u64)sprite->color[3] << 32);
    } else {
        c = *(u32 *)color;
        prim[1] = ((c >> 8) & 0xFF) | ((u64)(c & 0xFF) << 32);
        prim[0] = ((c >> 24) & 0xFF) | ((u64)((c >> 16) & 0xFF) << 32);
        prim[7] = ((c >> 8) & 0xFF) | ((u64)(c & 0xFF) << 32);
        prim[6] = ((c >> 24) & 0xFF) | ((u64)((c >> 16) & 0xFF) << 32);
        prim[13] = ((c >> 8) & 0xFF) | ((u64)(c & 0xFF) << 32);
        prim[12] = ((c >> 24) & 0xFF) | ((u64)((c >> 16) & 0xFF) << 32);
        prim[19] = ((c >> 8) & 0xFF) | ((u64)(c & 0xFF) << 32);
        prim[18] = ((c >> 24) & 0xFF) | ((u64)((c >> 16) & 0xFF) << 32);
    }
    prim[2] = uv[0] | ((u64)uv[1] << 32);
    prim[4] = pt[0].x | ((u64)pt[0].y << 32);
    prim[5] = (u32)(sprite->unk_18 >> 4);
    prim[8] = uv[2] | ((u64)uv[1] << 32);
    prim[10] = pt[1].x | ((u64)pt[1].y << 32);
    prim[11] = (u32)(sprite->unk_18 >> 4);
    prim[14] = uv[0] | ((u64)uv[3] << 32);
    prim[16] = pt[2].x | ((u64)pt[2].y << 32);
    prim[17] = (u32)(sprite->unk_18 >> 4);
    prim[20] = uv[2] | ((u64)uv[3] << 32);
    prim[22] = pt[3].x | ((u64)pt[3].y << 32);
    prim[23] = (u32)(sprite->unk_18 >> 4);
    switch (mode) {
    case 1:
        fclDrawPanelSprite2(0x48, ot);
        break;
    case 2:
        fclDrawPanelSprite2(0x42, ot);
        break;
    default:
        fclDrawPanelSprite2(0x44, ot);
        break;
    }
    tag = (void *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    sdf2DLinkTexPacket(tag, sprite->tex, 0);
    sdfDrawTagLink(tag, packet);
    otp = &sdfGrOtTable[ot];
    otp->regFunc(otp, tag);
    fclDrawPanelSprite2(0x44, ot);
}
#else
INCLUDE_ASM(const s32, "fclShare", fclDrawSpriteAngleCore);
#endif

INCLUDE_ASM(const s32, "fclShare", func_00214FD8);

s32 fclRagCalcTexSize(s32 size, s32 mode)
{
    s32 n = (s32)(fsqrtf((f32)(size * 16)) * (f32)mode * 0.001953125f);

    return n * n;
}

extern s32 fclRagCalcTexSize(s32 size, s32 mode);

void fclRagTexCalcSizes(fclRagTex_t *tex)
{
    s32 mode = tex->unk_14;

    tex->unk_04 = fclRagCalcTexSize(tex->unk_0C, mode);
    tex->unk_08 = fclRagCalcTexSize(tex->unk_10, mode);
}

void fclRagTexInit(fclRagTex_t *tex, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, void (*calc)())
{
    tex->unk_00 = 0;
    tex->unk_04 = a;
    tex->unk_08 = b;
    tex->unk_0C = c;
    tex->unk_10 = d;
    tex->unk_14 = e;
    tex->unk_18 = f;
    tex->calcSizes = calc;
    calc(tex);
}

INCLUDE_ASM(const s32, "fclShare", fclRunPanelAnim);

void fclSetPanelAnimState(fclPanelAnim_t *anim, u8 state)
{
    anim->animState = state;
}

s32 fclGetPanelAnimTarget(fclPanelAnim_t *anim)
{
    return anim->unk_04;
}

s32 fclGetPanelAnimPhase(fclPanelAnim_t *anim)
{
    return anim->unk_14;
}

void func_00215188(fclPanelAnim_t *anim, s32 value)
{
    anim->unk_1C = value;
}

s32 fcl_215190(fclPanelAnim_t *anim)
{
    return anim->unk_1C;
}

INCLUDE_ASM(const s32, "fclShare", fclDrawPhaseValue);

INCLUDE_ASM(const s32, "fclShare", fclDrawPanelBorder);

INCLUDE_ASM(const s32, "fclShare", fclDrawPhaseHeader);
