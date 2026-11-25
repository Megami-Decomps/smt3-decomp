#include "common.h"
#include "dds3PadButtonState.h"
#include "facility.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"
#include "sdfFrame.h"

extern void sdfMemFreeDirect(void *aHandle);
extern u32 dds3GetKernelCount(void);






extern void fclDrawSpriteCore(s32 X, s32 Y, s32 W, s32 H, s32 *pCol, fclSprite_t *pSprite,
                              u8 FlipFlag, u8 AlphaFlag, u8 FilterFlag, s32 OtNo);

extern void fclDrawSpriteAngleCore(s32 X, s32 Y, s32 W, s32 H, f32 Angle, s32 *pCol,
                                   fclSprite_t *pSprite, u8 FlipFlag, u8 AlphaFlag, u8 FilterFlag, s32 OtNo);

extern void fclSetSprite(fclSprite_t *pSprite, s32 Xsize, s32 Ysize, s32 Z, s32 U1, s32 V1, s32 U2,
                         s32 V2, s32 Col, s32 TexHandle);

extern void fclDrawSpriteEx(s32 X, s32 Y, s32 W, s32 H, fclSprite_t *pSprite, s32 OtNo);

extern s32 fclRunPanelAnim(void *work);

extern sdfGrOt_t sdfGrOtTable[85];


extern void fclRagTexInit(fclRagTex_t *tex, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                          void (*calc)(void *));

extern void fclRagTexCalcSizes();

/* Called above its definition, where ee-gcc assumed int. */

extern void fclDrawPhaseHeader(u8 *work, s32 color, s32 mode, s32 id);

extern s32 fclGetPanelAnimTarget(fclPanelAnim_t *anim);

extern s32 fclGetPanelAnimPhase(fclPanelAnim_t *anim);


extern s32 sdfTexSet(void *aTex);

s32 fclLoadTexture(char *pFileName)
{
    void *data;
    s32 size;
    void *file;
    s32 handle;

    file = sdfDevLoadFileEx(pFileName, (u32 *)&data, &size);
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

/* NON_MATCHING: 104 of 180 words differ (first at +0x20). */
/* Behaviour also checked against retail: 3 PCSX2 call vectors give the same return and memory,
 * menu main savestate, covering only the paths those calls took.
 * (sdfport bc29478, tools/funcvec, tests/funcvec.sh). */
extern void fclDrawPanelSprite2(s32 id, s32 arg);
extern s32 sdf2DGetPrimPacketSize(s32 aNumOfRegs, s32 aNumOfPrim);
extern s32 sdf2DLinkTexPacket(sdfDrawTag_t *aTag, sdfTexHandle_t *aTexHndl, s32 aCtx);
extern s32 sdfGrAllocPacketBuff(s32 aSize);
extern sdfGrPacketHeader_t *sdf2DMakePrimPacketHeader(sdfGrPacketHeader_t *aPacketPtr, s32 aPrimReg, s32 aNumOfRegs, s64 aGifRegs, s32 aNumOfPrim);
extern void *sdf2DGetPrimPtr(sdfGrPacketHeader_t *aPacketPtr);
extern void sdfDrawTagInit(sdfDrawTag_t *a);
extern void sdfDrawTagLink(sdfDrawTag_t *aTag, addr_t aPacket);
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
    u32 wide;
    u32 tall;
    FAKEMATCH_ASM_REG(u32, uMin, "$5");
    FAKEMATCH_ASM_REG(u32, vMin, "$3");
    FAKEMATCH_ASM_REG(u32, uMax, "$2");
    FAKEMATCH_ASM_REG(u32, vMax, "$4");
    u8 flip = FlipFlag;

    if (FilterFlag == 0) {
        sdfTexSetFilter(EE_ADDR_AS(sdfTexHandle_t *, pSprite->tex), 0, 1);
    } else {
        sdfTexSetFilter(EE_ADDR_AS(sdfTexHandle_t *, pSprite->tex), 1, 1);
    }
    packet = EE_ADDR_AS(void *, sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(5, 1)));
    sdf2DMakePrimPacketHeader(packet, 0x156, 5, 0x43431, 1);
    prim = (u64 *)sdf2DGetPrimPtr(packet);
    {
        /* Snapshot the four actual UV fields via their halfword addresses. */
        const u16 *leftU = &pSprite->u0;
        const u16 *leftV = &pSprite->v0;
        const u16 *rightU = &pSprite->u1;
        const u16 *rightV = &pSprite->v1;
        x0 = (u32)X + 0x6FF8;
        uMin = *leftU;
        vMin = *leftV;
        uMax = *rightU;
        vMax = *rightV;
        y0 = (u32)Y + 0x78FC;
        wide = (u32)W << 4;
        tall = (u32)H << 3;
        uv[0] = uMin + 8;
        uv[2] = uMax + 8;
        uv[1] = vMin + 8;
        uv[3] = vMax + 8;
        x1 = x0 + wide;
        y1 = y0 + tall;
    }
    if ((flip & 1) != 0) {
        t = x0;
        x0 = x1;
        x1 = t;
    }
    {
        FAKEMATCH_ASM_REG(s32, yFlip, "$2") = flip & 2;
        if (yFlip != 0) {
            t = y0;
            y0 = y1;
            y1 = t;
        }
    }
    if (pCol == NULL) {
        prim[0] = pSprite->color[0] | ((u64)pSprite->color[1] << 32);
        prim[1] = pSprite->color[2] | ((u64)pSprite->color[3] << 32);
    } else {
        u32 alpha;
        u32 red;
        u32 green;
        u32 blue;
        c = (u32)*pCol;
        /* Native loads the actual low byte separately from the packed word. */
        alpha = *(const u8 *)pCol;
        red = (c >> 24) & 0xFF;
        green = (c >> 16) & 0xFF;
        blue = (c >> 8) & 0xFF;
        prim[0] = (u64)red | ((u64)green << 32);
        prim[1] = (u64)blue | ((u64)alpha << 32);
    }
    prim[2] = uv[0] | ((u64)uv[1] << 32);
    prim[4] = x0 | ((u64)y0 << 32);
    prim[5] = (u32)pSprite->Z;
    prim[6] = uv[2] | ((u64)uv[3] << 32);
    prim[8] = x1 | ((u64)y1 << 32);
    prim[9] = (u32)pSprite->Z;
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
    tag = EE_ADDR_AS(void *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(tag);
    sdf2DLinkTexPacket(tag, EE_ADDR_AS(sdfTexHandle_t *, pSprite->tex), 0);
    sdfDrawTagLink(tag, EE_ADDR(packet));
    otp = &sdfGrOtTable[OtNo];
    otp->regFunc(otp, tag);
    fclDrawPanelSprite2(0x44, OtNo);
}

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
    FAKEMATCH_ASM_REG(s32, blendMode, "$8");

    if (FilterFlag == 0) {
        sdfTexSetFilter(EE_ADDR_AS(sdfTexHandle_t *, pSprite->tex), 0, 1);
    } else {
        sdfTexSetFilter(EE_ADDR_AS(sdfTexHandle_t *, pSprite->tex), 1, 1);
    }
    packet = EE_ADDR_AS(void *, sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(3, 4)));
    sdf2DMakePrimPacketHeader(packet, 0x15C, 3, 0x531, 4);
    prim = (u64 *)sdf2DGetPrimPtr(packet);
    x0 = (u32)X + 0x7000;
    y0 = (u32)Y + 0x7900;
    x1 = (u32)x0 + ((u32)W << 4);
    y1 = (u32)y0 + ((u32)H << 3);
    hw = ((s32)((u32)x1 - (u32)x0)) / 2;
    hh = ((s32)((u32)y1 - (u32)y0)) / 2;
    cx = (u32)x0 + (u32)hw;
    cy = (u32)y0 + (u32)hh;
    pt[0].x = x0;
    pt[0].y = y0;
    pt[3].x = x1;
    pt[3].y = y1;
    /* Keep native outgoing rotation arguments within each call's phase. */
    {
        FAKEMATCH_ASM_REG(f32, outgoingAngle, "$f12") = Angle;
        FAKEMATCH_ASM_LIVE_F32(outgoingAngle);
        fclCalcRotPos(&pt[0].x, &pt[0].y, outgoingAngle, -hw, -hh, cx, cy);
    }
    {
        FAKEMATCH_ASM_REG(s32, dy, "$7") = -hh;
        FAKEMATCH_ASM_REG(s32, dx, "$6") = hw;
        FAKEMATCH_ASM_REG(s32, xMid, "$8") = cx;
        FAKEMATCH_ASM_REG(s32, yMid, "$9") = cy;
        fclCalcRotPos(&pt[1].x, &pt[1].y, Angle, dx, dy, xMid, yMid);
    }
    {
        FAKEMATCH_ASM_REG(s32, dx, "$6") = -hw;
        FAKEMATCH_ASM_REG(s32, dy, "$7") = hh;
        FAKEMATCH_ASM_REG(s32, xMid, "$8") = cx;
        FAKEMATCH_ASM_REG(s32, yMid, "$9") = cy;
        fclCalcRotPos(&pt[2].x, &pt[2].y, Angle, dx, dy, xMid, yMid);
    }
    {
        FAKEMATCH_ASM_REG(s32, dx, "$6") = hw;
        FAKEMATCH_ASM_REG(s32, dy, "$7") = hh;
        FAKEMATCH_ASM_REG(s32, xMid, "$8") = cx;
        FAKEMATCH_ASM_REG(s32, yMid, "$9") = cy;
        fclCalcRotPos(&pt[3].x, &pt[3].y, Angle, dx, dy, xMid, yMid);
    }
    {
        FAKEMATCH_ASM_REG(u32, tex0, "$5") = pSprite->unk_08;
        FAKEMATCH_ASM_REG(u32, tex2, "$6") = pSprite->unk_0C;
        FAKEMATCH_ASM_REG(u32, rawFlip, "$2") = FlipFlag & 1;
        FAKEMATCH_ASM_REG(u32, tex3, "$3") = pSprite->unk_0E;
        FAKEMATCH_ASM_REG(u32, tex1, "$4") = pSprite->unk_0A;
        u32 flipX;
        FAKEMATCH_ASM_READ(tex2);
        FAKEMATCH_ASM_READ_PAIR(rawFlip, tex1);
        flipX = (u8)rawFlip;
        uv[0] = tex0;
        uv[2] = tex2;
        uv[1] = tex1;
        uv[3] = tex3;
        if (flipX != 0) {
            t = pt[0].x;
            pt[0].x = pt[1].x;
            pt[1].x = t;
            t = pt[2].x;
            pt[2].x = pt[3].x;
            pt[3].x = t;
        }
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
        u64 p0;
        u64 p1;
        u32 alpha;
        FAKEMATCH_ASM_REG(u32, c, "$2");
        c = (u32)*pCol;
        alpha = *(const u8 *)pCol;
        p0 = ((c >> 24) & 0xff) | ((u64)((c >> 16) & 0xff) << 32);
        p1 = ((c >> 8) & 0xff) | ((u64)alpha << 32);
        prim[0] = p0;
        prim[1] = p1;
        prim[6] = p0;
        prim[7] = p1;
        prim[12] = p0;
        prim[13] = p1;
        prim[18] = p0;
        prim[19] = p1;
    }
    /* Keep real coordinate and UV lanes in their native packing phases.
     * Each hard register is reused only after its previous lane is consumed. */
    {
        FAKEMATCH_ASM_REG(u64, top, "$4");
        FAKEMATCH_ASM_REG(u64, left, "$7");
        u64 uv0;
        u64 z;
        s32 rawZ;
        s32 logicalZ;
        u64 y0hi;
        FAKEMATCH_ASM_REG(u64, right, "$6");
        FAKEMATCH_ASM_REG(u64, uv1, "$4");
        u64 xy0;
        FAKEMATCH_ASM_REG(u64, y1hi, "$2");
        FAKEMATCH_ASM_REG(u64, bottom, "$4");
        FAKEMATCH_ASM_REG(u64, xy1, "$3");
        u64 uv2;
        FAKEMATCH_ASM_REG(u64, y2hi, "$2");
        FAKEMATCH_ASM_REG(u64, uv3, "$6");
        FAKEMATCH_ASM_REG(u64, xy2, "$3");
        FAKEMATCH_ASM_REG(u64, y3hi, "$2");
        u64 xy3;
        top = uv[1];
        blendMode = 1;
        FAKEMATCH_ASM_READ_PAIR(top, blendMode);
        left = uv[0];
        top <<= 32;
        FAKEMATCH_ASM_READ(top);
        rawZ = pSprite->Z;
        uv0 = left | top;
        y0hi = (u32)pt[0].y;
        prim[2] = uv0;
        logicalZ = rawZ >> 4;
        /* Load right U between the low Y load and the Y lane shift. */
        right = uv[2];
        y0hi <<= 32;
        FAKEMATCH_ASM_READ(right);
        z = (u32)logicalZ;
        uv1 = right | top;
        xy0 = (u32)pt[0].x | y0hi;
        FAKEMATCH_ASM_READ(uv1);
        prim[8] = uv1;
        prim[4] = xy0;
        prim[5] = z;
        y1hi = (u32)pt[1].y;
        bottom = uv[3];
        FAKEMATCH_ASM_READ_PAIR(y1hi, bottom);
        xy1 = (u32)pt[1].x;
        y1hi <<= 32;
        bottom <<= 32;
        prim[11] = z;
        xy1 |= y1hi;
        y2hi = (u32)pt[2].y;
        uv2 = left | bottom;
        prim[10] = xy1;
        prim[14] = uv2;
        y2hi <<= 32;
        FAKEMATCH_ASM_READ(y2hi);
        uv3 = right | bottom;
        FAKEMATCH_ASM_READ_PAIR(y2hi, uv3);
        xy2 = (u32)pt[2].x | y2hi;
        prim[20] = uv3;
        y3hi = (u32)pt[3].y;
        prim[17] = z;
        /* Expose the completed packet word to retain native Z/XY store order. */
        FAKEMATCH_ASM_READ_MEM(prim[17]);
        prim[16] = xy2;
        y3hi <<= 32;
        FAKEMATCH_ASM_READ(y3hi);
        xy3 = (u32)pt[3].x | y3hi;
        prim[23] = z;
        prim[22] = xy3;
    }
    if (AlphaFlag != blendMode) goto notOne;
    goto one;
notOne:
    if (AlphaFlag != 2) goto other;
    goto two;
one:
    fclDrawPanelSprite2(0x48, OtNo);
    goto blendDone;
two:
    fclDrawPanelSprite2(0x42, OtNo);
    goto blendDone;
other:
    fclDrawPanelSprite2(0x44, OtNo);
blendDone:
    tag = EE_ADDR_AS(void *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(tag);
    sdf2DLinkTexPacket(tag, EE_ADDR_AS(sdfTexHandle_t *, pSprite->tex), 0);
    sdfDrawTagLink(tag, EE_ADDR(packet));
    otp = &sdfGrOtTable[OtNo];
    otp->regFunc(otp, tag);
    fclDrawPanelSprite2(0x44, OtNo);
}

/* Both native mode paths test the same signed pad-state byte. */
s32 func_00214FD8(s32 mode)
{
    if (mode == 0) {
        if ((s8)((u8)dds3PadButtonState[0x22]) < 0) return 1;
    } else {
        if ((s8)((u8)dds3PadButtonState[0x22]) < 0) return 1;
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

void fclRagTexInit(fclRagTex_t *tex, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, void (*calc)(void *))
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
        anim->state.phase += SDF_FRAME_STEP_INT(anim->state.phaseStep);
        if (anim->state.phase > 0x200) {
            anim->state.phase = 0x200;
        }
        break;
    case 2:
        anim->state.phase -= SDF_FRAME_STEP_INT(anim->state.phaseStep);
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

extern void fclDrawPanelSprite2(s32 id, s32 ot);
extern void cmpDrawTriG(const u32 *xs, const u32 *ys, u32 z, const u32 *colors, u32 ot);

void fclDrawPhaseValue(void *work, s32 color, s32 ot)
{
    fclPhasePanel_t *panel = work;
    u32 points[2][3];
    u32 colors[3];
    s32 height;
    fclDrawPanelSprite2(0x48, ot);
    colors[0] = color;
    colors[1] = color & 0xFF;
    colors[2] = color & 0xFF;
    points[0][0] = (u32)panel->x + panel->width * 16;
    points[1][0] = panel->y;
    points[0][1] = (u32)panel->x + panel->width * 16;
    points[1][1] = (u32)panel->y + (panel->height * 80 / 100) * 8;
    points[0][2] = (u32)panel->x + panel->width * 16 - (panel->height * 80 / 100) * 16;
    points[1][2] = panel->y;
    cmpDrawTriG(points[0], points[1], panel->z, colors, ot);
    height = panel->height;
    points[0][0] = panel->x;
    points[1][0] = (u32)panel->y + height * 8;
    points[0][1] = (u32)panel->x + (height * 80 / 100) * 16;
    points[1][1] = (u32)panel->y + height * 8;
    points[0][2] = panel->x;
    points[1][2] = (u32)panel->y + (height / 5) * 8;
    cmpDrawTriG(points[0], points[1], panel->z, colors, ot);
    fclDrawPanelSprite2(0x44, ot);
}


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

extern void fclDrawPanelSprite(s32 id, s32 ot);
extern void fclDrawPhaseValue(void *work, s32 color, s32 ot);

void fclDrawPhaseHeader(u8 *work, s32 color, s32 mode, s32 ot)
{
    FAKEMATCH_ASM_REG(s32, orderTable, "$17") = ot;
    FAKEMATCH_ASM_REG(s32, phaseColor, "$19") = color;

    FAKEMATCH_ASM_LIVE(orderTable);
    FAKEMATCH_ASM_LIVE_PAIR(orderTable, phaseColor);
    fclDrawPanelSprite(0x33001, orderTable);
    if (mode == 1) {
        fclDrawPhaseValue(work, phaseColor, orderTable);
    } else {
        fclDrawPhaseValue(work, 0x141D2D4C, orderTable);
    }
    {
        /* Keep the final ID in a wide EE register until the s32 call. */
        FAKEMATCH_ASM_REG(long, sid, "$4");
        sid = 0x5101BL;
        fclDrawPanelSprite(sid, orderTable);
    }
}
