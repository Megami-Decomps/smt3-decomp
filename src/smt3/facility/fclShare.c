#include "common.h"
#include "facility.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"

extern void sdfMemFreeDirect(void *aHandle);






extern void fclDrawSpriteCore(s32 X, s32 Y, s32 W, s32 H, s32 *pCol, fclSprite_t *pSprite,
                              u8 FlipFlag, u8 AlphaFlag, u8 FilterFlag, s32 OtNo);

extern void fclDrawSpriteAngleCore(s32 X, s32 Y, s32 W, s32 H, f32 Angle, s32 *pCol,
                                   fclSprite_t *pSprite, u8 FlipFlag, u8 AlphaFlag, u8 FilterFlag, s32 OtNo);

extern void fclSetSprite(fclSprite_t *pSprite, s32 Xsize, s32 Ysize, s32 Z, s32 U1, s32 V1, s32 U2,
                         s32 V2, s32 Col, s32 TexHandle);

extern void fclDrawSpriteEx(s32 X, s32 Y, s32 W, s32 H, fclSprite_t *pSprite, s32 OtNo);

extern s32 fclRunPanelAnim(void *work);

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

extern s32 sdfTexSet(void *aTex);

s32 fclLoadTexture(char *pFileName)
{
    void *data;
    s32 size;
    void *file;
    s32 handle;

    file = sdfDevLoadFileEx(pFileName, &data, &size);
    handle = sdfTexSet(data);
    sdfMemFreeDirect(file);
    return handle;
}

void fclSetSpriteColor(u8 *pSprite, u32 Col)
{
    pSprite[0x1C] = Col >> 24;
    pSprite[0x1D] = Col >> 16;
    pSprite[0x1E] = Col >> 8;
    pSprite[0x1F] = Col;
}

void fclSetSpriteEx(fclSprite_t *pSprite, fclSpriteRect_t *pSpriteBase, s32 X, s32 Y, s32 Z, s32 Col,
                    s32 Flag, s32 TexHandle)
{
    fclSpriteRect_t *r;
    s32 u0;
    s32 v0;
    s32 u1;

    pSprite->X = X << 4;
    pSprite->tex = TexHandle;
    pSprite->Y = Y << 3;
    pSprite->Z = Z;
    pSprite->Flag = Flag;
    fclSetSpriteColor((u8 *)pSprite, Col);
    r = (fclSpriteRect_t *)&pSprite->unk_04;
    *r = *pSpriteBase;
    if (r->x == 0) {
        r->x = pSprite->unk_0C;
    }
    if (pSprite->unk_06 == 0) {
        pSprite->unk_06 = pSprite->unk_0E;
    }
    u0 = pSprite->unk_08;
    v0 = pSprite->unk_0A;
    u1 = pSprite->unk_0C;
    pSprite->unk_08 = u0 << 4;
    pSprite->unk_0A = v0 << 4;
    pSprite->unk_0C = (u1 + u0) << 4;
    pSprite->unk_0E = (pSprite->unk_0E + v0) << 4;
}

void fclSetSprite(fclSprite_t *pSprite, s32 Xsize, s32 Ysize, s32 Z, s32 U1, s32 V1, s32 U2, s32 V2,
                  s32 Col, s32 TexHandle)
{
    U1 <<= 4;
    V1 <<= 4;
    U2 <<= 4;
    V2 <<= 4;
    pSprite->unk_04 = Xsize;
    pSprite->unk_0E = V2;
    pSprite->tex = TexHandle;
    pSprite->Z = Z;
    pSprite->unk_06 = Ysize;
    pSprite->unk_08 = U1;
    pSprite->unk_0A = V1;
    pSprite->unk_0C = U2;
    pSprite->Y = 0;
    pSprite->X = 0;
    fclSetSpriteColor((u8 *)pSprite, Col);
}

void fclSetSpriteNormal(fclSprite_t *pSprite, s32 Xsize, s32 Ysize, s32 Z, s32 U1, s32 V1, s32 Col, s32 TexHandle)
{
    fclSetSprite(pSprite, Xsize, Ysize, Z, U1, V1, U1 + Xsize, V1 + Ysize, Col, TexHandle);
}

void fclDrawSprite(s32 X, s32 Y, fclSprite_t *pSprite)
{
    fclDrawSpriteEx(X, Y, pSprite->unk_04, pSprite->unk_06, pSprite, 0x47);
}

extern void func_002146F8(s32 X, s32 Y, s32 W, s32 H, s32 *pCol, fclSprite_t *pSprite,
                         u8 FlipFlag, s32 OtNo);

void fclDrawSpriteEx(s32 X, s32 Y, s32 W, s32 H, fclSprite_t *pSprite, s32 OtNo)
{
    func_002146F8(X, Y, W, H, NULL, pSprite, 0, OtNo);
}

void func_002146F8(s32 X, s32 Y, s32 W, s32 H, s32 *pCol, fclSprite_t *pSprite,
                   u8 FlipFlag, s32 OtNo)
{
    fclDrawSpriteCore(X << 4, Y << 3, W, H, pCol, pSprite, FlipFlag, 0, 1, OtNo);
}

/* NON_MATCHING: 111 of 180 words differ (first at +0x20). */
#ifdef NON_MATCHING
extern s32 fclDrawPanelSprite2(s32 id, s32 arg);
extern s32 sdf2DGetPrimPacketSize(s32 aNumOfRegs, s32 aNumOfPrim);
extern s32 sdf2DLinkTexPacket(sdfDrawTag_t *aTag, sdfTexHandle_t *aTexHndl, s32 aCtx);
extern s32 sdfGrAllocPacketBuff(s32 aSize);
extern sdfGrPacketHeader_t *sdf2DMakePrimPacketHeader(sdfGrPacketHeader_t *aPacketPtr, s32 aPrimReg, s32 aNumOfRegs, s64 aGifRegs, s32 aNumOfPrim);
extern void *sdf2DGetPrimPtr(sdfGrPacketHeader_t *aPacketPtr);
extern void sdfDrawTagInit(sdfDrawTag_t *a);
extern void sdfDrawTagLink(sdfDrawTag_t *aTag, u32 aPacket);
extern void sdfTexSetFilter(sdfTexHandle_t *aHandle, s32 aMagMode, s32 aMinMode);

void fclDrawSpriteCore(s32 X, s32 Y, s32 W, s32 H, s32 *pCol, fclSprite_t *pSprite, u8 FlipFlag,
                       u8 AlphaFlag, u8 FilterFlag, s32 OtNo)
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

    if (FilterFlag == 0) {
        sdfTexSetFilter(pSprite->tex, 0, 1);
    } else {
        sdfTexSetFilter(pSprite->tex, 1, 1);
    }
    packet = (void *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(5, 1));
    sdf2DMakePrimPacketHeader(packet, 0x156, 5, 0x43431, 1);
    prim = (u64 *)sdf2DGetPrimPtr(packet);
    uv[0] = pSprite->unk_08 + 8;
    uv[1] = pSprite->unk_0A + 8;
    uv[2] = pSprite->unk_0C + 8;
    uv[3] = pSprite->unk_0E + 8;
    x0 = X + 0x6FF8;
    y0 = Y + 0x78FC;
    x1 = x0 + (W << 4);
    y1 = y0 + (H << 3);
    if ((FlipFlag & 1) != 0) {
        t = x0;
        x0 = x1;
        x1 = t;
    }
    if ((FlipFlag & 2) != 0) {
        t = y0;
        y0 = y1;
        y1 = t;
    }
    if (pCol == NULL) {
        prim[0] = pSprite->color[0] | ((u64)pSprite->color[1] << 32);
        prim[1] = pSprite->color[2] | ((u64)pSprite->color[3] << 32);
    } else {
        c = *(u32 *)pCol;
        prim[1] = ((c >> 8) & 0xFF) | ((u64)(c & 0xFF) << 32);
        prim[0] = ((c >> 24) & 0xFF) | ((u64)((c >> 16) & 0xFF) << 32);
    }
    prim[2] = uv[0] | ((u64)uv[1] << 32);
    prim[4] = x1 | ((u64)y1 << 32);
    prim[5] = pSprite->Z;
    prim[6] = uv[2] | ((u64)uv[3] << 32);
    prim[8] = x0 | ((u64)y0 << 32);
    prim[9] = pSprite->Z;
    switch (AlphaFlag) {
    case 1:
        fclDrawPanelSprite2(0x48, OtNo);
        break;
    case 2:
        fclDrawPanelSprite2(0x42, OtNo);
        break;
    default:
        fclDrawPanelSprite2(0x44, OtNo);
        break;
    }
    tag = (void *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    sdf2DLinkTexPacket(tag, pSprite->tex, 0);
    sdfDrawTagLink(tag, packet);
    otp = &sdfGrOtTable[OtNo];
    otp->regFunc(otp, tag);
    fclDrawPanelSprite2(0x44, OtNo);
}
#else
INCLUDE_ASM(const s32, "facility/fclShare", fclDrawSpriteCore);
#endif

extern f32 sdfSin(f32 aRadius);

extern f32 sdfCos(f32 aRadius);

void fclCalcRotPos(s32 *pBufX, s32 *pBufY, f32 DistX, s32 DistY, s32 CenterX, s32 CenterY, s32 Angle)
{
    f32 rad = DistX * 0.0174532925f;
    f32 hh = CenterX * 0.875f;
    f32 hw = DistY / 2;

    *pBufX = sdfCos(rad) * hw - sdfSin(rad) * hh;
    *pBufY = sdfSin(rad) * hw + sdfCos(rad) * hh;
    *pBufX = *pBufX * 2 + CenterY;
    *pBufY = *pBufY / 0.875f + Angle;
}

typedef struct
{
    /* 0x0 */ s32 x;
    /* 0x4 */ s32 y;
} fclRotPoint_t;

extern void fclCalcRotPos(s32 *pBufX, s32 *pBufY, f32 DistX, s32 DistY, s32 CenterX, s32 CenterY, s32 Angle);

/* NON_MATCHING: 86 of 298 words differ at the exact size: the angle `mov.s`
 * scheduled out of three call delay slots, one byte of the colour read as a
 * shift rather than a load, and the quadword block hoisted. */
#ifdef NON_MATCHING
void fclDrawSpriteAngleCore(s32 X, s32 Y, s32 W, s32 H, f32 Angle, s32 *pCol,
                            fclSprite_t *pSprite, u8 FlipFlag, u8 AlphaFlag, u8 FilterFlag, s32 OtNo)
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

    if (FilterFlag == 0) {
        sdfTexSetFilter(pSprite->tex, 0, 1);
    } else {
        sdfTexSetFilter(pSprite->tex, 1, 1);
    }
    packet = (void *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(3, 4));
    sdf2DMakePrimPacketHeader(packet, 0x15C, 3, 0x531, 4);
    prim = (u64 *)sdf2DGetPrimPtr(packet);
    x0 = X + 0x7000;
    y0 = Y + 0x7900;
    x1 = x0 + (W << 4);
    y1 = y0 + (H << 3);
    hw = (x1 - x0) / 2;
    hh = (y1 - y0) / 2;
    cx = x0 + hw;
    cy = y0 + hh;
    pt[0].x = x0;
    pt[0].y = y0;
    pt[3].x = x1;
    pt[3].y = y1;
    fclCalcRotPos(&pt[0].x, &pt[0].y, Angle, -hw, -hh, cx, cy);
    fclCalcRotPos(&pt[1].x, &pt[1].y, Angle, hw, -hh, cx, cy);
    fclCalcRotPos(&pt[2].x, &pt[2].y, Angle, -hw, hh, cx, cy);
    fclCalcRotPos(&pt[3].x, &pt[3].y, Angle, hw, hh, cx, cy);
    uv[0] = pSprite->unk_08;
    uv[2] = pSprite->unk_0C;
    uv[1] = pSprite->unk_0A;
    uv[3] = pSprite->unk_0E;
    if ((FlipFlag & 1) != 0) {
        t = pt[0].x;
        pt[0].x = pt[1].x;
        pt[1].x = t;
        t = pt[2].x;
        pt[2].x = pt[3].x;
        pt[3].x = t;
    }
    if ((FlipFlag & 2) != 0) {
        t = pt[0].y;
        pt[0].y = pt[1].y;
        pt[1].y = t;
        t = pt[2].y;
        pt[2].y = pt[3].y;
        pt[3].y = t;
    }
    if (pCol == NULL) {
        prim[0] = pSprite->color[0] | ((u64)pSprite->color[1] << 32);
        prim[1] = pSprite->color[2] | ((u64)pSprite->color[3] << 32);
        prim[6] = pSprite->color[0] | ((u64)pSprite->color[1] << 32);
        prim[7] = pSprite->color[2] | ((u64)pSprite->color[3] << 32);
        prim[12] = pSprite->color[0] | ((u64)pSprite->color[1] << 32);
        prim[13] = pSprite->color[2] | ((u64)pSprite->color[3] << 32);
        prim[18] = pSprite->color[0] | ((u64)pSprite->color[1] << 32);
        prim[19] = pSprite->color[2] | ((u64)pSprite->color[3] << 32);
    } else {
        c = *(u32 *)pCol;
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
    prim[5] = (u32)(pSprite->Z >> 4);
    prim[8] = uv[2] | ((u64)uv[1] << 32);
    prim[10] = pt[1].x | ((u64)pt[1].y << 32);
    prim[11] = (u32)(pSprite->Z >> 4);
    prim[14] = uv[0] | ((u64)uv[3] << 32);
    prim[16] = pt[2].x | ((u64)pt[2].y << 32);
    prim[17] = (u32)(pSprite->Z >> 4);
    prim[20] = uv[2] | ((u64)uv[3] << 32);
    prim[22] = pt[3].x | ((u64)pt[3].y << 32);
    prim[23] = (u32)(pSprite->Z >> 4);
    switch (AlphaFlag) {
    case 1:
        fclDrawPanelSprite2(0x48, OtNo);
        break;
    case 2:
        fclDrawPanelSprite2(0x42, OtNo);
        break;
    default:
        fclDrawPanelSprite2(0x44, OtNo);
        break;
    }
    tag = (void *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    sdf2DLinkTexPacket(tag, pSprite->tex, 0);
    sdfDrawTagLink(tag, packet);
    otp = &sdfGrOtTable[OtNo];
    otp->regFunc(otp, tag);
    fclDrawPanelSprite2(0x44, OtNo);
}
#else
INCLUDE_ASM(const s32, "facility/fclShare", fclDrawSpriteAngleCore);
#endif

extern u8 dds3PadButtonState[0x40];

/* Both native mode paths test the same signed pad-state byte. */
s32 func_00214FD8(s32 mode)
{
    if (mode == 0) {
        if ((s8)dds3PadButtonState[0x22] < 0) return 1;
    } else {
        if ((s8)dds3PadButtonState[0x22] < 0) return 1;
    }
    return 0;
}

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

s32 fclRunPanelAnim(void *work)
{
    fclPanelAnim_t *anim = work;
    u8 state = anim->state.animState;

    switch (state) {
    case 0:
        break;
    case 1:
        anim->state.phase += anim->state.phaseStep;
        if (anim->state.phase > 0x200) {
            anim->state.phase = 0x200;
        }
        break;
    case 2:
        anim->state.phase -= anim->state.phaseStep;
        if (anim->state.phase < 0) {
            anim->state.phase = 0;
        }
        break;
    default:
        break;
    }
    anim->calcSizes(anim);
    return anim->state.phase;
}

void fclSetPanelAnimState(fclPanelAnim_t *anim, u8 state)
{
    anim->state.animState = state;
}

s32 fclGetPanelAnimTarget(fclPanelAnim_t *anim)
{
    return anim->state.target;
}

s32 fclGetPanelAnimPhase(fclPanelAnim_t *anim)
{
    return anim->state.phase;
}

void func_00215188(fclPanelAnim_t *anim, s32 value)
{
    anim->state.value = value;
}

s32 fcl_215190(fclPanelAnim_t *anim)
{
    return anim->state.value;
}

typedef struct
{
    s32 x;
    s32 y;
    s32 z;
    u16 width;
    u16 height;
    s32 color;
    u8 speed;
    u8 reserved15[3];
    s32 timer;
    s32 timer2;
    s32 flags;
} fclPhasePanel_t;

extern s32 fclDrawPanelSprite2(s32 id, s32 ot);
extern void cmpDrawTriG(const u32 *xs, const u32 *ys, u32 z, const u32 *colors, u32 ot);

#ifdef NON_MATCHING
void fclDrawPhaseValue(void *work, s32 color, s32 ot)
{
    fclPhasePanel_t *panel = work;
    u32 xs[3];
    u32 ys[3];
    u32 colors[3];
    s32 depth;
    s32 right;
    s32 y;
    s32 height;

    fclDrawPanelSprite2(0x48, ot);
    height = panel->height;
    depth = height * 80 / 100;
    right = panel->x + panel->width * 16;
    y = panel->y;
    colors[0] = color;
    colors[1] = color & 0xFF;
    colors[2] = color & 0xFF;
    xs[0] = right;
    xs[1] = right;
    xs[2] = right - depth * 16;
    ys[0] = y;
    ys[1] = y + depth * 8;
    ys[2] = y;
    cmpDrawTriG(xs, ys, panel->z, colors, ot);

    height = panel->height;
    y = panel->y;
    xs[0] = panel->x;
    xs[1] = panel->x + (height * 80 / 100) * 16;
    xs[2] = panel->x;
    ys[0] = y + height * 8;
    ys[1] = y + height * 8;
    ys[2] = y + (height / 5) * 8;
    cmpDrawTriG(xs, ys, panel->z, colors, ot);
    fclDrawPanelSprite2(0x44, ot);
}
#else
INCLUDE_ASM(const s32, "facility/fclShare", fclDrawPhaseValue);
#endif


extern s32 D_00345D88[];
extern fclSpriteTbl_t fclSpriteTable[];
extern void fclDrawSpriteRes(s32 x, s32 y, s32 z, s32 *clip, s32 page,
                             s32 spriteId, s32 *textures, fclSpriteTbl_t *table, s32 ot);

void fclDrawPanelBorder(void *work, s32 z, s32 inset, s32 ot)
{
    fclPhasePanel_t *panel = work;
    s32 timer;
    s32 flags;
    s32 sprite[2];
    s32 xOffset[2];
    s32 yOffset[2];
    s32 colors[4];
    s32 color[2];
    s32 active;
    s32 alpha;

    sprite[0] = 6;
    sprite[1] = 8;
    xOffset[1] = 5;
    xOffset[0] = 5;
    yOffset[0] = 0x1C;
    yOffset[1] = 0x29;
    active = 0;

    timer = panel->timer;
    if (timer > 0x100) {
        timer = 0x200 - timer;
    }
    flags = panel->flags;
    color[0] = 0x80808080;
    color[1] = 0x80808080;
    alpha = (timer / 4 + 0x40) & 0xFF;
    if (flags & 1) {
        sprite[0] = 7;
        xOffset[0] = -1;
        yOffset[0] = 0x15;
        color[0] = 0x80808000 | alpha;
        active = 1;
    }
    if (flags & 2) {
        sprite[1]++;
        xOffset[1] = -1;
        yOffset[1] = 0x30;
        color[1] = 0x80808000 | alpha;
        active = 1;
    }
    if (active == 0) {
        panel->timer = 0;
    }

    colors[0] = color[0];
    colors[1] = color[0];
    colors[2] = color[0];
    colors[3] = color[0];
    fclDrawSpriteRes(panel->x + ((inset + xOffset[0]) * 0x10),
                     panel->y + (yOffset[0] * 8), z, colors, 10,
                     (u16)sprite[0], D_00345D88, fclSpriteTable, ot);

    colors[0] = color[1];
    colors[1] = color[1];
    colors[2] = color[1];
    colors[3] = color[1];
    fclDrawSpriteRes(panel->x + ((inset + xOffset[1]) * 0x10),
                     panel->y + (((u16)panel->height - yOffset[1]) * 8),
                     z, colors, 10, (u16)sprite[1], D_00345D88,
                     fclSpriteTable, ot);
}

extern s32 fclDrawPanelSprite(s32 id, s32 ot);
extern void fclDrawPhaseValue(void *work, s32 color, s32 ot);

#ifdef NON_MATCHING
void fclDrawPhaseHeader(u8 *work, s32 color, s32 mode, s32 ot)
{
    fclDrawPanelSprite(0x33001, ot);
    if (mode == 1) {
        fclDrawPhaseValue(work, color, ot);
    } else {
        fclDrawPhaseValue(work, 0x141D2D4C, ot);
    }
    fclDrawPanelSprite(0x5101B, ot);
}
#else
INCLUDE_ASM(const s32, "facility/fclShare", fclDrawPhaseHeader);
#endif
