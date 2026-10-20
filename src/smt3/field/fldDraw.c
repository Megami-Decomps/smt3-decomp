#include "fpu.h"
#include "common.h"
#include "libsdf.h"
#include "kernel.h"
#include "ps2asm.h"
#include "basic.h"
#include "billboard.h"
#include "draw.h"
#include "scr.h"
#include "draw3d.h"
#include "field.h"
#include "model.h"
#include "motion.h"
#include "newdata.h"
#include "event.h"
#include "effect.h"

typedef struct fldSpriteCorner_t fldSpriteCorner_t;

typedef struct fldSpriteVertex_t fldSpriteVertex_t;

typedef struct TMX_HEADER_t TMX_HEADER_t;

struct fldSpriteCorner_t {
    s32 u;
    s32 v;
    u8 pad08[8];
    s32 x;
    s32 y;
    s32 mask;
    s16 flag;
    u8 pad1E[2];
};

struct fldSpriteVertex_t {
    s32 r;
    s32 g;
    s32 b;
    s32 a;
    fldSpriteCorner_t corner[2];
};

typedef struct sdfTexBuf_t sdfTexBuf_t;

typedef struct dmaPacketHeader_t dmaPacketHeader_t;

typedef struct frFontGlyph_t frFontGlyph_t;

typedef struct {
    s32 px;
    s32 py;
    s32 sx;
    s32 sy;
    u32 pz;
    u32 panelCol;
    u32 frameCol;
    s32 cx;
    s32 cy;
    s32 cz;
    sdfDrawTag_t *tag;
} dbgConsCtl_t;

sdfTexHandle_t *sdfTexSet(const TMX_HEADER_t *header);

extern u32 D_0054EC44;

extern u32 fldDrawEnv;

extern sdfTexHandle_t *D_0054EC60;

extern u32 D_00551C9C;

extern u8 sdfTexEmbedded_2FC4A0[];

extern sdfMat_t *sdfMatNew(void);

s32 sdf2DLinkTexPacket(sdfDrawTag_t *packetList, sdfTexHandle_t *texture, s32 contextOffset);

sdfGrPacketHeader_t *sdf2DAllocSprite(s32 loopCount);

void sdfDrawTagInit(sdfDrawTag_t *list);

void sdfDrawTagLink(sdfDrawTag_t *list, u32 packetAddress);

s32 sdfGrAllocPacketBuff(s32 size);

extern s32 sdf2DGetPrimPtr(s32);

void sdfPrimLinkGt4(
    sdfDrawTag_t *list, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0,
    s32 color0, s32 x1, s32 y1, s32 u1, s32 v1, s32 color1, s32 x2, s32 y2,
    s32 u2, s32 v2, s32 color2, s32 x3, s32 y3, s32 u3, s32 v3, s32 color3,
    s32 depth, s32 (*allocatePacket)(s32));

sdfDrawTag_t *dds3FontReqAllocDrawTag(void);

void MultMatrixRev(const void *matrix);

extern u8 D_002FA240[];

extern sdf3DScreenParam_t dds3ScreenParam;

extern sdf3DScreenParam_t dds3ScreenParam2;

extern u8 aProjMatrix[];

extern u32 sdf2DMakeStdPacket(u32, s32);

sdfGrPacketHeader_t *sdf2DMakePrimPacketHeader(sdfGrPacketHeader_t *packet, s32 primitiveFlags,
                                       s32 registerCount, s64 registerList,
                                       s32 loopCount);

extern s32 sdf2DGetPrimPacketSize(s32 registerCount, s32 loopCount);

struct sdfTexBuf_t *sdfTexGetPacket(sdfTexHandle_t *texture);

s32 sdfTexGetPacketLength(const sdfTexHandle_t *texture);

void SetRotateMatrixZ(f32 angle);

void sdfDrawTagLinkRef(sdfDrawTag_t *list, u32 packetAddress);

extern void sdf3DMakeDirectRefPacket(dmaPacketHeader_t *, u32, s32);

extern void sdfBillBoardMakeTMPacket(u8 *, const f32 *);

extern void sdfBillBoardMake1(u8 *, const sdfVector4_t *, u32, const UV_t *, const VTX_t *);

sdfGrPacketHeader_t *effDebugDrawWireModel(const f32 vertices[][4], const u32 *colors,
                                                 s32 vertexCount, u32 primitiveFlags);

extern f32 D_002FC720[][4];

extern u32 D_002FC740[];

void sdf3DPrimLinkResetTm(sdfDrawTag_t *packetList,
                              s32 (*allocatePacket)(s32));

void sdf3DPrimLinkMat(sdfDrawTag_t *packetList, sdfMat_t *asset,
                              s32 (*allocatePacket)(s32));

u32 *sdf3DPrimMakePoly(sdf3DPrimPolyParm_t *request);

extern void *memset(void *, int, unsigned int);

void sdfFreeAutoDelayed(void *data);

void sdfLinkStoreImage(
    sdfDrawTag_t *list, sdfGsMemSlice_t *source, s32 sourceX, s32 sourceY,
    s32 width, s32 height, s32 resourceAddress, s32 arg7, s32 arg8,
    s32 (*allocatePacket)(s32));

sdfDrawTag_t *sdfPrimAllocDrawTag(s32 (*allocator)(s32));

extern sdfGrFrameSet_t D_00529388;

void sdfLinkLoadImage(
    sdfDrawTag_t *list, sdfGsMemSlice_t *source, s32 destinationX,
    s32 destinationY, s32 width, s32 height, s32 sourceAddress,
    s32 (*allocatePacket)(s32));

s32 frDestroyFontRequestAfter(struct frFontGlyph_t *glyph);

s32 frDrawFont(frFontGlyph_t *glyph);

void sdfPrimSetOpaquePacket(sdfPrimTransPacket_t *packet);

void sdfDevSetPrintCB(sifCommand_t *packet, s32 source, s32 end, s32 argument, s32 index);

void *sdfDevPrintfEx(void *packet, const char *format, ...);

extern u8 D_0054EC50[];

extern void InversMatrix(void);

extern u8 D_0054EC58[];

f32 sdfSin(f32 angle);

f32 sdfCos(f32 angle);

extern void *memcpy(void *, const void *, unsigned int);

extern void *fldDrwZBuf;

extern void *sdfAllocAuto(u32 size);

extern void sdfFreeAutoDelayed(void *p);

extern sdfGrOt_t sdfGrOtTable[85];

extern s32 gencOn;

extern s32 gencMode;

extern s32 rsetFrameMaskOtOffset;

/* dds3DrawBank_t stride; only the held frame texture block at 0x1300 is used here. */
typedef struct {
    u8 unk0000[0x1300];
    u8 frameTex[0x8E0];
} fldDrawBank_t;

extern fldDrawBank_t dds3DrawEnv[];

extern u32 dds3GetFrameID(void);

extern void sdfGrSetFrameMaskRGB(const u8 *source, void *packet, s32 gsContext);

extern void sdfGrSetFrameMaskReset(const u8 *source, void *packet, s32 gsContext);

extern void sdfDrawTagLinkRefGif(sdfDrawTag_t *list, u32 packetAddress);

extern void sdfGrLinkDispFrameTex(sdfDrawTag_t *list, u32 source, void *node);

void fldDrawInit(void) {
    if (D_0054EC44 == 0) {
        sdfMat_t *object;
        D_0054EC44 = 1;
        object = sdfMatNew();
        fldDrawEnv = (u32)object;
        object->constant = 1.0f;
        D_00551C9C = (u32)sdfMatNew();
        D_0054EC60 = sdfTexSet((TMX_HEADER_t *)(sdfTexEmbedded_2FC4A0));
    }
}

sdfGrOt_t *fldDrawGetOt(void)
{
    return &sdfGrOtTable[rsetFrameMaskOtOffset];
}

void DrwSprite2D(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh, s32 color, sdfTexHandle_t *texture) {
    void *handle = sdf2DAllocSprite(1);
    fldSpriteVertex_t *vtx = (fldSpriteVertex_t *)sdf2DGetPrimPtr((s32)handle);
    s32 ubase = u * 16;
    s32 xl = x * 16 + 0x7000;
    s32 vbase = v * 16;
    s32 yt = y * 8 + 0x7900;
    sdfDrawTag_t *command;
    sdfGrOt_t *descriptor;

    vtx->r = color & 0xFF;
    vtx->g = (color >> 8) & 0xFF;
    vtx->b = (color >> 16) & 0xFF;
    vtx->a = (color >> 24) & 0xFF;
    vtx->corner[0].u = ubase;
    vtx->corner[0].v = vbase;
    vtx->corner[0].x = xl;
    vtx->corner[0].y = yt;
    vtx->corner[0].mask = -1;
    vtx->corner[0].flag = 0;
    vtx->corner[1].u = ubase + uw * 16;
    vtx->corner[1].v = vbase + vh * 16;
    vtx->corner[1].x = xl + w * 16;
    vtx->corner[1].y = yt + h * 8;
    vtx->corner[1].mask = -1;
    vtx->corner[1].flag = 0;
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    sdf2DLinkTexPacket(command, texture, 0);
    sdfDrawTagLink(command, (u32)handle);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void DrwSprite2DC(s32 x, s32 y, s32 width, s32 height,
                   s32 u, s32 v, s32 textureWidth, s32 textureHeight,
                   u32 color0, u32 color1, u32 color2, u32 color3, sdfTexHandle_t *texture) {
    sdfDrawTag_t *list;
    sdfGrOt_t *surface;
    s32 left, top, right, bottom;
    s32 uLeft, vTop, uRight, vBottom;

    list = dds3FontReqAllocDrawTag();
    sdf2DLinkTexPacket(list, texture, 0);
    left = (x << 4) + 0x7000;
    top = (y << 3);
    right = (x << 4) + (width << 4) + 0x7000;
    bottom = top + (height << 3) + 0x7900;
    top += 0x7900;
    uLeft = (u << 4);
    vTop = (v << 4);
    uRight = uLeft + (textureWidth << 4);
    vBottom = vTop + (textureHeight << 4);
    sdfPrimLinkGt4(list, 0x40,
        left, top, uLeft, vTop, color0,
        right, top, uRight, vTop, color1,
        left, bottom, uLeft, vBottom, color3,
        right, bottom, uRight, vBottom, color2,
        -1, NULL);
    surface = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))surface->regFunc)(surface, list);
}

void DrwSprite2DF(f32 x, f32 y, f32 w, f32 h, s32 u, s32 v, s32 uw, s32 vh, s32 color, sdfTexHandle_t *texture) {
    void *handle = sdf2DAllocSprite(1);
    fldSpriteVertex_t *vtx = (fldSpriteVertex_t *)sdf2DGetPrimPtr((s32)handle);
    s32 r = color & 0xFF;
    s32 g = (color >> 8) & 0xFF;
    s32 b = (color >> 16) & 0xFF;
    s32 a = (color >> 24) & 0xFF;
    s32 ubase = u * 16;
    s32 vbase = v * 16;
    s32 xl = (s32)(x * 16.0f) + 0x7000;
    s32 yt = (s32)(y * 8.0f) + 0x7900;
    sdfDrawTag_t *command;
    sdfGrOt_t *descriptor;

    vtx->r = r;
    vtx->g = g;
    vtx->b = b;
    vtx->a = a;
    vtx->corner[0].u = ubase;
    vtx->corner[0].v = vbase;
    vtx->corner[0].x = xl;
    vtx->corner[0].y = yt;
    vtx->corner[0].mask = -1;
    vtx->corner[0].flag = 0;
    vtx->corner[1].u = ubase + uw * 16;
    vtx->corner[1].v = vbase + vh * 16;
    vtx->corner[1].x = xl + (s32)(w * 16.0f);
    vtx->corner[1].y = yt + (s32)(h * 8.0f);
    vtx->corner[1].mask = -1;
    vtx->corner[1].flag = 0;
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    sdf2DLinkTexPacket(command, texture, 0);
    sdfDrawTagLink(command, (u32)handle);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void fldPoint3To2(f32 *dstX, f32 *dstY, f32 x, f32 y, f32 z) {
    f32 vec[4] = { x, y, z, 1.0f };
    f32 result[4];

    SDF_VU0_LOADMATRIX(D_002FA240);
    MultMatrixRev(&dds3ScreenParam.TM);
    SDF_VU0_COPYMATRIX_A_TO_B();
    SDF_VU0_LOADVECTOR_MEMOPS(*(const Quad *)vec);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    SDF_VU0_LOADVECTOR_B(&dds3ScreenParam.scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(&dds3ScreenParam.trans);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(result);
    *dstX = result[0];
    *dstY = result[1];
}

void fldPoint3To2H(f32 *dstX, f32 *dstY, f32 x, f32 y, f32 z) {
    f32 vec[4] = { x, y, z, 1.0f };
    f32 result[4];

    SDF_VU0_LOADMATRIX(aProjMatrix);
    MultMatrixRev(&dds3ScreenParam2.TM);
    SDF_VU0_COPYMATRIX_A_TO_B();
    SDF_VU0_LOADVECTOR_MEMOPS(*(const Quad *)vec);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    SDF_VU0_LOADVECTOR_B(&dds3ScreenParam2.scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(&dds3ScreenParam2.trans);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(result);
    *dstX = result[0];
    *dstY = result[1];
}

void fldPoint3To2_A(void) {
    SDF_VU0_LOADMATRIX(D_002FA240);
    MultMatrixRev(&dds3ScreenParam.TM);
    SDF_VU0_COPYMATRIX_A_TO_B();
}

void fldPoint3To2_B(f32 *dstX, f32 *dstY, f32 x, f32 y, f32 z) {
    f32 vec[4] = { x, y, z, 1.0f };
    f32 result[4];

    SDF_VU0_LOADVECTOR_MEMOPS(*(const Quad *)vec);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    SDF_VU0_LOADVECTOR_B(&dds3ScreenParam.scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(&dds3ScreenParam.trans);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(result);
    *dstX = result[0];
    *dstY = result[1];
}

extern void rsetOtOfs(s32 ofs);

/* NON_MATCHING: exact on its own; its presence turns two bnel of encDrawProc
   into bne. */
#ifdef NON_MATCHING
void rsetOtOfs(s32 ofs)
{
    rsetFrameMaskOtOffset = ofs;
}
#else
INCLUDE_ASM(const s32, "field/fldDraw", rsetOtOfs);
#endif

void rsetTexa(s32 lower, s32 bits, u64 upper) {
    sdfDrawTag_t *command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    s32 packet;
    u64 *entry;
    sdfGrOt_t *descriptor;

    sdfDrawTagInit(command);
    packet = sdfGrAllocPacketBuff(0x30);
    entry = (u64 *)sdf2DMakeStdPacket(packet, 0x30);
    entry[5] = 0x3B;
    entry[4] = (u64)(bits << 15) | (upper << 32) | lower;
    sdfDrawTagLink(command, packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void rsetTest(s32 bit0, s32 bit1, s32 bit4, s32 bit12, s32 bit14, s32 bit15, s32 unused, s32 bit17) {
    sdfDrawTag_t *command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    s32 packet;
    u64 *data;
    sdfGrOt_t *descriptor;

    sdfDrawTagInit(command);
    packet = sdfGrAllocPacketBuff(0x30);
    data = (u64 *)sdf2DMakeStdPacket(packet, 0x30);
    data[4] = (bit17 << 17) | 0x10000 | (bit15 << 15) | (bit14 << 14) | (bit12 << 12) | (bit4 << 4) | (bit1 << 1) | bit0;
    data[5] = 0x47;
    sdfDrawTagLink(command, packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void rsetAlpha(s32 mode) {
    sdfDrawTag_t *command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    s32 packet;
    u64 *data;
    sdfGrOt_t *descriptor;

    sdfDrawTagInit(command);
    packet = sdfGrAllocPacketBuff(0x30);
    data = (u64 *)sdf2DMakeStdPacket(packet, 0x30);
    switch (mode) {
    case 1:
        data[4] = 0x48;
        break;
    case 2:
        data[4] = 0x42;
        break;
    case 3:
        data[4] = 0x84;
        break;
    case 4:
        data[4] = 0x06;
        break;
    case 5:
        data[4] = 0x89;
        break;
    case 6:
        data[4] = 0x42;
        break;
    case 10:
        data[4] = 0x2A;
        break;
    case 20:
        data[4] = 0x4A;
        break;
    default:
        data[4] = 0x44;
        break;
    }
    data[5] = 0x42;
    sdfDrawTagLink(command, packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void aDrawLineStripPrim(s32 x0, s32 y0, s32 x1, s32 y1, u32 gsWord0, u32 gsWord1, u32 gsWord2) {
    s32 coords[4];
    sdfDrawTag_t *command;
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    u64 lo;
    u64 hi;
    s32 *pos;
    sdfGrOt_t *descriptor;
    s32 i;

    coords[0] = x0 * 16;
    coords[1] = y0 * 16;
    coords[2] = x1 * 16;
    coords[3] = y1 * 16;
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 2));
    sdf2DMakePrimPacketHeader(packet, 0x49, 2, 0x41, 2);
    dst = (u64 *)sdf2DGetPrimPtr((s32)packet);
    lo = (u64)gsWord0 | ((u64)gsWord1 << 32);
    hi = (u64)gsWord2 | (0x8000LL << 24);
    pos = coords;
    for (i = 0; i < 2; i++) {
        dst[0] = lo;
        dst[1] = hi;
        dst += 2;
        dst[1] = 0xFFFFFF;
        dst[0] = (u64)(u32)(pos[0] + 0x7000) | ((u64)(pos[1] + 0x7900) << 32);
        pos += 2;
        dst += 2;
    }
    sdfDrawTagLink(command, (u32)packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void aDrawLineDirect(u32 x0, u32 y0, u32 x1, u32 y1, u32 gsWord0, u32 gsWord1, u32 gsWord2) {
    u32 coords[4];
    sdfDrawTag_t *command;
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    sdfGrOt_t *descriptor;
    u32 *pos;
    u64 lo;
    u64 hi;
    s32 i;

    coords[0] = x0;
    coords[1] = y0;
    coords[2] = x1;
    coords[3] = y1;
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 2));
    sdf2DMakePrimPacketHeader(packet, 0x49, 2, 0x41, 2);
    dst = (u64 *)sdf2DGetPrimPtr((s32)packet);
    lo = (u64)gsWord0 | ((u64)gsWord1 << 32);
    hi = (u64)gsWord2 | (0x8000LL << 24);
    pos = coords;
    for (i = 0; i < 2; i++) {
        dst[0] = lo;
        dst[1] = hi;
        dst += 2;
        dst[1] = 0xFFFFFF;
        dst[0] = (u64)pos[0] | ((u64)pos[1] << 32);
        pos += 2;
        dst += 2;
    }
    sdfDrawTagLink(command, (u32)packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void aDrawBoxF(s32 x, s32 y, s32 w, s32 h, u32 gsWord0, u32 gsWord1, u32 gsWord2, u32 gsWord3) {
    s32 coords[8];
    sdfDrawTag_t *command;
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    u64 lo;
    u64 hi;
    s32 *pos;
    sdfGrOt_t *descriptor;
    s32 i;

    coords[0] = x * 16;
    coords[1] = y * 16;
    coords[2] = (x + w) * 16;
    coords[3] = y * 16;
    coords[4] = (x + w) * 16;
    coords[5] = (y + h) * 16;
    coords[6] = x * 16;
    coords[7] = (y + h) * 16;
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 4));
    sdf2DMakePrimPacketHeader(packet, 0x4D, 2, 0x41, 4);
    dst = (u64 *)sdf2DGetPrimPtr((s32)packet);
    lo = (u64)gsWord0 | ((u64)gsWord1 << 32);
    hi = (u64)gsWord2 | ((u64)gsWord3 << 32);
    pos = coords;
    for (i = 0; i < 4; i++) {
        dst[0] = lo;
        dst[1] = hi;
        dst += 2;
        dst[1] = 0xFFFFFF;
        dst[0] = (u64)(u32)(pos[0] + 0x7000) | ((u64)(pos[1] + 0x7900) << 32);
        pos += 2;
        dst += 2;
    }
    sdfDrawTagLink(command, (u32)packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void aDrawBoxFZ(s32 x, s32 y, s32 w, s32 h, u32 vertexTag, u32 gsWord0, u32 gsWord1, u32 gsWord2, u32 gsWord3) {
    s32 coords[8];
    sdfDrawTag_t *command;
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    u64 lo;
    u64 hi;
    u64 tag;
    s32 *pos;
    sdfGrOt_t *descriptor;
    s32 i;

    coords[0] = x * 16;
    coords[1] = y * 16;
    coords[2] = (x + w) * 16;
    coords[3] = y * 16;
    coords[4] = (x + w) * 16;
    coords[5] = (y + h) * 16;
    coords[6] = x * 16;
    coords[7] = (y + h) * 16;
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 4));
    sdf2DMakePrimPacketHeader(packet, 0x4D, 2, 0x41, 4);
    dst = (u64 *)sdf2DGetPrimPtr((s32)packet);
    tag = vertexTag;
    lo = (u64)gsWord0 | ((u64)gsWord1 << 32);
    hi = (u64)gsWord2 | ((u64)gsWord3 << 32);
    pos = coords;
    for (i = 0; i < 4; i++) {
        dst[0] = lo;
        dst[1] = hi;
        dst += 2;
        dst[1] = tag;
        dst[0] = (u64)(u32)(pos[0] + 0x7000) | ((u64)(pos[1] + 0x7900) << 32);
        pos += 2;
        dst += 2;
    }
    sdfDrawTagLink(command, (u32)packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void aDrawBoxF_Direct(s32 x0, s32 y0, s32 x1, s32 y1, u32 gsWord0, u32 gsWord1, u32 gsWord2, u32 gsWord3) {
    u32 coords[8];
    sdfDrawTag_t *command;
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    u64 lo;
    u64 hi;
    u32 *pos;
    sdfGrOt_t *descriptor;
    s32 i;

    coords[0] = x0;
    coords[1] = y0;
    coords[2] = x1;
    coords[3] = y0;
    coords[4] = x1;
    coords[5] = y1;
    coords[6] = x0;
    coords[7] = y1;
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 4));
    sdf2DMakePrimPacketHeader(packet, 0x4D, 2, 0x41, 4);
    dst = (u64 *)sdf2DGetPrimPtr((s32)packet);
    lo = (u64)gsWord0 | ((u64)gsWord1 << 32);
    hi = (u64)gsWord2 | ((u64)gsWord3 << 32);
    pos = coords;
    for (i = 0; i < 4; i++) {
        dst[0] = lo;
        dst[1] = hi;
        dst += 2;
        dst[1] = 0xFFFFFFFFULL;
        dst[0] = (u64)pos[0] | ((u64)pos[1] << 32);
        pos += 2;
        dst += 2;
    }
    sdfDrawTagLink(command, (u32)packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void aDrawTriG(s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, u32 r0, u32 g0, u32 b0, u32 a0, u32 r1, u32 g1, u32 b1, u32 a1, u32 r2, u32 g2, u32 b2, u32 a2) {
    s32 coords[6];
    sdfDrawTag_t *command;
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    s32 i;
    sdfGrOt_t *descriptor;

    coords[0] = x0 * 16;
    coords[1] = y0 * 16;
    coords[2] = x1 * 16;
    coords[3] = y1 * 16;
    coords[4] = x2 * 16;
    coords[5] = y2 * 16;
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 3));
    sdf2DMakePrimPacketHeader(packet, 0x4D, 2, 0x41, 3);
    dst = (u64 *)sdf2DGetPrimPtr((s32)packet);
    for (i = 0; i < 3; i++) {
        if (i == 0) {
            dst[0] = (u64)r0 | ((u64)g0 << 32);
            dst[1] = (u64)b0 | ((u64)a0 << 32);
        } else if (i == 1) {
            dst[0] = (u64)r1 | ((u64)g1 << 32);
            dst[1] = (u64)b1 | ((u64)a1 << 32);
        } else if (i == 2) {
            dst[0] = (u64)r2 | ((u64)g2 << 32);
            dst[1] = (u64)b2 | ((u64)a2 << 32);
        }
        dst += 2;
        dst[1] = 0xFFFFFF;
        dst[0] = (u64)(u32)(coords[i * 2] + 0x7000) |
            ((u64)(coords[i * 2 + 1] + 0x7900) << 32);
        dst += 2;
    }
    sdfDrawTagLink(command, (u32)packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void aDrawBoxG(s32 x, s32 y, s32 w, s32 h, u32 r0, u32 g0, u32 b0, u32 a0, u32 r1, u32 g1, u32 b1, u32 a1, u32 r2, u32 g2, u32 b2, u32 a2, u32 r3, u32 g3, u32 b3, u32 a3) {
    s32 coords[8];
    sdfDrawTag_t *command;
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    s32 i;
    s32 *pos;
    sdfGrOt_t *descriptor;

    coords[0] = x * 16;
    coords[1] = y * 16;
    coords[2] = (x + w) * 16;
    coords[3] = y * 16;
    coords[4] = (x + w) * 16;
    coords[5] = (y + h) * 16;
    coords[6] = x * 16;
    coords[7] = (y + h) * 16;
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 4));
    sdf2DMakePrimPacketHeader(packet, 0x4D, 2, 0x41, 4);
    dst = (u64 *)sdf2DGetPrimPtr((s32)packet);
    for (i = 0, pos = coords; i < 4; i++) {
        if (i == 0) {
            dst[0] = (u64)r0 | ((u64)g0 << 32);
            dst[1] = (u64)b0 | ((u64)a0 << 32);
        } else if (i == 1) {
            dst[0] = (u64)r1 | ((u64)g1 << 32);
            dst[1] = (u64)b1 | ((u64)a1 << 32);
        } else if (i == 2) {
            dst[0] = (u64)r2 | ((u64)g2 << 32);
            dst[1] = (u64)b2 | ((u64)a2 << 32);
        } else {
            dst[0] = (u64)r3 | ((u64)g3 << 32);
            dst[1] = (u64)b3 | ((u64)a3 << 32);
        }
        dst += 2;
        dst[1] = 0xFFFFFF;
        dst[0] = (u64)(u32)(pos[0] + 0x7000) |
            ((u64)(pos[1] + 0x7900) << 32);
        dst += 2;
        pos += 2;
    }
    sdfDrawTagLink(command, (u32)packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void aDrawBoxGF(u32 r0, u32 g0, u32 b0, u32 a0, u32 r1, u32 g1, u32 b1, u32 a1, u32 r2, u32 g2, u32 b2, u32 a2, u32 r3, u32 g3, u32 b3, u32 a3, f32 x, f32 y, f32 w, f32 h) {
    s32 coords[8];
    sdfDrawTag_t *command;
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    s32 i;
    s32 *pos;
    sdfGrOt_t *descriptor;

    coords[0] = (s32)(x * 16.0f);
    coords[1] = (s32)(y * 8.0f);
    coords[2] = (s32)((x + w) * 16.0f);
    coords[3] = (s32)(y * 8.0f);
    coords[4] = (s32)((x + w) * 16.0f);
    coords[5] = (s32)((y + h) * 8.0f);
    coords[6] = (s32)(x * 16.0f);
    coords[7] = (s32)((y + h) * 8.0f);
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 4));
    sdf2DMakePrimPacketHeader(packet, 0x4D, 2, 0x41, 4);
    dst = (u64 *)sdf2DGetPrimPtr((s32)packet);
    for (i = 0, pos = coords; i < 4; i++) {
        if (i == 0) {
            dst[0] = (u64)r0 | ((u64)g0 << 32);
            dst[1] = (u64)b0 | ((u64)a0 << 32);
        } else if (i == 1) {
            dst[0] = (u64)r1 | ((u64)g1 << 32);
            dst[1] = (u64)b1 | ((u64)a1 << 32);
        } else if (i == 2) {
            dst[0] = (u64)r2 | ((u64)g2 << 32);
            dst[1] = (u64)b2 | ((u64)a2 << 32);
        } else {
            dst[0] = (u64)r3 | ((u64)g3 << 32);
            dst[1] = (u64)b3 | ((u64)a3 << 32);
        }
        dst += 2;
        dst[1] = 0xFFFFFF;
        dst[0] = (u64)(u32)(pos[0] + 0x7000) |
            ((u64)(pos[1] + 0x7900) << 32);
        dst += 2;
        pos += 2;
    }
    sdfDrawTagLink(command, (u32)packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void func_0012A140(s32 x, s32 y, s32 w, s32 h, u32 gsWord0, u32 gsWord1, u32 gsWord2, u32 gsWord3, u32 vertexTag) {
    s32 coords[8];
    sdfDrawTag_t *command;
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    u64 lo;
    u64 hi;
    u64 tag;
    s32 *pos;
    sdfGrOt_t *descriptor;
    s32 i;

    coords[0] = x * 16;
    coords[1] = y * 16;
    coords[2] = (x + w) * 16;
    coords[3] = y * 16;
    coords[4] = (x + w) * 16;
    coords[5] = (y + h) * 16;
    coords[6] = x * 16;
    coords[7] = (y + h) * 16;
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 4));
    sdf2DMakePrimPacketHeader(packet, 0x4D, 2, 0x41, 4);
    dst = (u64 *)sdf2DGetPrimPtr((s32)packet);
    lo = (u64)gsWord0 | ((u64)gsWord1 << 32);
    hi = (u64)gsWord2 | ((u64)gsWord3 << 32);
    tag = vertexTag;
    pos = coords;
    for (i = 0; i < 4; i++) {
        dst[0] = lo;
        dst[1] = hi;
        dst += 2;
        dst[1] = tag;
        dst[0] = (u64)(u32)(pos[0] + 0x7000) | ((u64)(pos[1] + 0x7900) << 32);
        pos += 2;
        dst += 2;
    }
    sdfDrawTagLink(command, (u32)packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void drawSprite3D(sdfTexHandle_t *texture, Sprite3D_t *sprite) {
    sdfDrawTag_t *command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    dmaPacketHeader_t *header;
    u8 *packet;
    f32 mat[16];
    sdfGrOt_t *descriptor;

    sdfDrawTagInit(command);
    header = (dmaPacketHeader_t *)sdfGrAllocPacketBuff(0x20);
    sdf3DMakeDirectRefPacket(header, (u32)sdfTexGetPacket(texture), sdfTexGetPacketLength(texture));
    sdfDrawTagLinkRef(command, (u32)header);
    SetRotateMatrixZ(sprite->rot);
    SDF_VU0_STOREMATRIX(mat);
    packet = (u8 *)sdfGrAllocPacketBuff(0x38);
    sdfBillBoardMakeTMPacket(packet, mat);
    sdfDrawTagLink(command, (u32)packet);
    packet = (u8 *)sdfGrAllocPacketBuff(0x80);
    sdfBillBoardMake1(packet, &sprite->pos, sprite->col, sprite->uv, sprite->vt);
    sdfDrawTagLink(command, (u32)packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void rsetFrameMaskRgbOn(void) {
    sdfDrawTag_t *command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    void *texture;
    sdfGrOt_t *descriptor;

    sdfDrawTagInit(command);
    texture = (void *)sdfGrAllocPacketBuff(0x40);
    sdfGrSetFrameMaskRGB(dds3DrawEnv[dds3GetFrameID()].frameTex, texture, 0);
    sdfDrawTagLinkRefGif(command, (u32)texture);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void rsetFrameMaskRgbOff(void) {
    sdfDrawTag_t *command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    void *texture;
    sdfGrOt_t *descriptor;

    sdfDrawTagInit(command);
    texture = (void *)sdfGrAllocPacketBuff(0x40);
    sdfGrSetFrameMaskReset(dds3DrawEnv[dds3GetFrameID()].frameTex, texture, 0);
    sdfDrawTagLinkRefGif(command, (u32)texture);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void ooyTestLine3D(u32 first, u32 second, f32 x, f32 y, f32 z, f32 u, f32 v, f32 w) {
    sdfDrawTag_t *list;
    s32 record;
    sdfGrOt_t *descriptor;
    D_002FC720[0][0] = x;
    D_002FC720[0][1] = y;
    D_002FC720[0][2] = z;
    D_002FC720[1][0] = u;
    D_002FC720[1][1] = v;
    D_002FC720[1][2] = w;
    D_002FC740[1] = second;
    D_002FC740[0] = first;
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    record = (s32)effDebugDrawWireModel((const f32 (*)[4])D_002FC720, D_002FC740, 2, 0x80);
    sdfDrawTagLink(list, record);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, list);
}

void fldDrawPoly3(s32 color0, s32 color1, s32 color2,
                         f32 x0, f32 y0, f32 z0,
                         f32 x1, f32 y1, f32 z1,
                         f32 x2, f32 y2, f32 z2) {
    sdf3DPrimPolyParm_t desc;
    sdfVector4_t vertices[3];
    u32 colors[3];
    sdfDrawTag_t *command;
    sdfGrOt_t *descriptor;

    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    sdf3DPrimLinkResetTm(command, 0);
    sdf3DPrimLinkMat(command, (sdfMat_t *)fldDrawEnv, 0);
    memset(&desc, 0, 0x2C);
    desc.color = 0x80808080;
    desc.faceCount = 1;
    desc.pointCount = 3;
    desc.vertex = vertices;
    desc.vertexColor = colors;
    vertices[0].x = x0;
    vertices[0].y = y0;
    vertices[0].z = z0;
    vertices[1].x = x1;
    vertices[1].y = y1;
    vertices[1].z = z1;
    vertices[2].x = x2;
    vertices[2].y = y2;
    vertices[2].z = z2;
    colors[0] = color0;
    colors[1] = color1;
    colors[2] = color2;
    sdfDrawTagLink(command, (u32)sdf3DPrimMakePoly(&desc));
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

extern u64 D_002FC750[];

void fldDrawPoly4(s32 color0, f32 x0, f32 y0, f32 z0,
                  s32 color1, f32 x1, f32 y1, f32 z1,
                  s32 color2, f32 x2, f32 y2, f32 z2,
                  s32 color3, f32 x3, f32 y3, f32 z3) {
    sdf3DPrimPolyParm_t desc;
    sdfVector4_t vertices[4];
    u32 colors[4];
    u64 strip[8];
    sdfDrawTag_t *command;
    sdfGrOt_t *descriptor;

    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    sdf3DPrimLinkResetTm(command, 0);
    sdf3DPrimLinkMat(command, (sdfMat_t *)fldDrawEnv, 0);
    memset(&desc, 0, 0x2C);
    desc.color = 0x80808080;
    desc.faceCount = 2;
    desc.pointCount = 4;
    desc.vertex = vertices;
    desc.vertexColor = colors;
    desc.faceIndex = (u32 *)strip;
    memcpy(strip, D_002FC750, sizeof(strip));
    vertices[0].x = x0;
    vertices[0].y = y0;
    vertices[0].z = z0;
    vertices[1].x = x1;
    vertices[1].y = y1;
    vertices[1].z = z1;
    vertices[2].x = x2;
    vertices[2].y = y2;
    vertices[2].z = z2;
    vertices[3].x = x3;
    vertices[3].y = y3;
    vertices[3].z = z3;
    colors[0] = color0;
    colors[1] = color1;
    colors[2] = color2;
    colors[3] = color3;
    sdfDrawTagLink(command, (u32)sdf3DPrimMakePoly(&desc));
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void fldDrawTexPoly4(s32 color0, f32 x0, f32 y0, f32 z0,
                     s32 color1, f32 x1, f32 y1, f32 z1,
                     s32 color2, f32 x2, f32 y2, f32 z2,
                     s32 color3, f32 x3, f32 y3, f32 z3,
                     sdfTexHandle_t *texture, f32 u0, f32 v0, f32 u1, f32 v1) {
    sdf3DPrimPolyParm_t desc;
    sdfVector4_t vertices[4];
    u32 colors[4];
    u64 strip[8];
    sdfVector2_t uvs[4];
    sdfMat_t *asset;
    sdfDrawTag_t *command;
    sdfGrOt_t *descriptor;

    asset = sdfMatNew();
    sdfMatSetTex(asset, texture);
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    sdf3DPrimLinkResetTm(command, 0);
    sdf3DPrimLinkMat(command, asset, 0);
    memset(&desc, 0, 0x2C);
    desc.color = 0x80808080;
    desc.faceCount = 2;
    desc.pointCount = 4;
    desc.vertex = vertices;
    desc.vertexColor = colors;
    desc.faceIndex = (u32 *)strip;
    desc.uv = uvs;
    memcpy(strip, D_002FC750, sizeof(strip));
    vertices[0].x = x0;
    vertices[0].y = y0;
    vertices[0].z = z0;
    vertices[1].x = x2;
    vertices[1].y = y2;
    vertices[1].z = z2;
    vertices[2].x = x3;
    vertices[2].y = y3;
    vertices[2].z = z3;
    vertices[3].x = x1;
    vertices[3].y = y1;
    vertices[3].z = z1;
    colors[0] = color0;
    colors[1] = color2;
    colors[2] = color3;
    colors[3] = color1;
    uvs[0].x = u0;
    uvs[0].y = v0;
    uvs[1].x = u0;
    uvs[1].y = v1;
    uvs[2].x = u1;
    uvs[2].y = v1;
    uvs[3].x = u1;
    uvs[3].y = v0;
    sdfDrawTagLink(command, (u32)sdf3DPrimMakePoly(&desc));
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
    sdfMatFreeDelayed(asset);
}

void fldDrawFrameSprite(void) {
    sdfDrawTag_t *list = sdfPrimAllocDrawTag(NULL);
    void *reference = (void *)sdfGrAllocPacketBuff(0x20);
    u64 *texturePacket;
    u64 *blendPacket;
    void *sprite;
    fldSpriteVertex_t *vertex;
    sdfGrOt_t *surface;

    sdfGrLinkDispFrameTex(list, (u32)dds3DrawEnv[dds3GetFrameID()].frameTex, reference);
    texturePacket = (u64 *)sdfGrAllocPacketBuff(0x40);
    texturePacket[0] = 3;
    texturePacket[1] = 0x5000000310000000ULL;
    texturePacket[2] = 0x1000000000008002ULL;
    texturePacket[3] = 0xE;
    texturePacket[4] = 0x8000000080ULL;
    texturePacket[5] = 0x3B;
    texturePacket[6] = 0;
    texturePacket[7] = 0x3F;
    sdfDrawTagLink(list, (u32)texturePacket);
    blendPacket = (u64 *)sdfGrAllocPacketBuff(0x40);
    blendPacket[0] = 3;
    blendPacket[1] = 0x5000000310000000ULL;
    blendPacket[2] = 0x1000000000008002ULL;
    blendPacket[3] = 0xE;
    blendPacket[4] = 0x31001;
    blendPacket[5] = 0x47;
    blendPacket[6] = 0x44;
    blendPacket[7] = 0x42;
    sdfDrawTagLink(list, (u32)blendPacket);
    sprite = sdf2DAllocSprite(1);
    vertex = (fldSpriteVertex_t *)sdf2DGetPrimPtr((s32)sprite);
    vertex->r = 0x80;
    vertex->g = 0x80;
    vertex->b = 0x80;
    vertex->a = 0x30;
    vertex->corner[0].u = 0;
    vertex->corner[0].v = 0;
    vertex->corner[0].x = 0x6FF7;
    vertex->corner[0].y = 0x78FB;
    vertex->corner[0].mask = 0x3FFF;
    vertex->corner[0].flag = 0;
    vertex->corner[1].u = 0x2000;
    vertex->corner[1].v = 0xE00;
    vertex->corner[1].x = 0x9009;
    vertex->corner[1].y = 0x8705;
    vertex->corner[1].mask = 0x3FFF;
    vertex->corner[1].flag = 0;
    sdfDrawTagLink(list, (u32)sprite);
    surface = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))surface->regFunc)(surface, list);
}

void fldDrawBokasi(void) {
    sdfDrawTag_t *list = sdfPrimAllocDrawTag(NULL);
    void *reference = (void *)sdfGrAllocPacketBuff(0x20);
    u64 *texturePacket;
    u64 *blendPacket;
    void *sprite;
    fldSpriteVertex_t *vertex;
    sdfGrOt_t *surface;

    sdfGrLinkDispFrameTex(list, (u32)dds3DrawEnv[dds3GetFrameID()].frameTex, reference);
    texturePacket = (u64 *)sdfGrAllocPacketBuff(0x40);
    texturePacket[0] = 3;
    texturePacket[1] = 0x5000000310000000ULL;
    texturePacket[2] = 0x1000000000008002ULL;
    texturePacket[3] = 0xE;
    texturePacket[4] = 0x8000000080ULL;
    texturePacket[5] = 0x3B;
    texturePacket[6] = 0;
    texturePacket[7] = 0x3F;
    sdfDrawTagLink(list, (u32)texturePacket);
    blendPacket = (u64 *)sdfGrAllocPacketBuff(0x40);
    blendPacket[0] = 3;
    blendPacket[1] = 0x5000000310000000ULL;
    blendPacket[2] = 0x1000000000008002ULL;
    blendPacket[3] = 0xE;
    blendPacket[4] = 0x33001;
    blendPacket[5] = 0x47;
    blendPacket[6] = 0x8000000064ULL;
    blendPacket[7] = 0x42;
    sdfDrawTagLink(list, (u32)blendPacket);
    sprite = sdf2DAllocSprite(1);
    vertex = (fldSpriteVertex_t *)sdf2DGetPrimPtr((s32)sprite);
    vertex->r = 0x81;
    vertex->g = 0x81;
    vertex->b = 0x81;
    vertex->a = 0x80;
    vertex->corner[0].u = 0;
    vertex->corner[0].v = 0;
    vertex->corner[0].x = 0x6FF8;
    vertex->corner[0].y = 0x78FB;
    vertex->corner[0].mask = 0;
    vertex->corner[0].flag = 0;
    vertex->corner[1].u = 0x2000;
    vertex->corner[1].v = 0xDFF;
    vertex->corner[1].x = 0x8FF8;
    vertex->corner[1].y = 0x86FB;
    vertex->corner[1].mask = 0;
    vertex->corner[1].flag = 0;
    sdfDrawTagLink(list, (u32)sprite);
    surface = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))surface->regFunc)(surface, list);
}

void fldDrawFrameSpriteAlpha(s32 alpha) {
    sdfDrawTag_t *list = sdfPrimAllocDrawTag(NULL);
    void *reference = (void *)sdfGrAllocPacketBuff(0x20);
    u64 *texturePacket;
    u64 *blendPacket;
    void *sprite;
    fldSpriteVertex_t *vertex;
    sdfGrOt_t *surface;

    sdfGrLinkDispFrameTex(list, (u32)dds3DrawEnv[dds3GetFrameID()].frameTex, reference);
    texturePacket = (u64 *)sdfGrAllocPacketBuff(0x40);
    texturePacket[0] = 3;
    texturePacket[1] = 0x5000000310000000ULL;
    texturePacket[2] = 0x1000000000008002ULL;
    texturePacket[3] = 0xE;
    texturePacket[4] = 0x8000000080ULL;
    texturePacket[5] = 0x3B;
    texturePacket[6] = 0;
    texturePacket[7] = 0x3F;
    sdfDrawTagLink(list, (u32)texturePacket);
    blendPacket = (u64 *)sdfGrAllocPacketBuff(0x40);
    blendPacket[0] = 3;
    blendPacket[1] = 0x5000000310000000ULL;
    blendPacket[2] = 0x1000000000008002ULL;
    blendPacket[3] = 0xE;
    blendPacket[4] = 0x33001;
    blendPacket[5] = 0x47;
    blendPacket[6] = ((u64)(u32)alpha << 32) | 0x64;
    blendPacket[7] = 0x42;
    sdfDrawTagLink(list, (u32)blendPacket);
    sprite = sdf2DAllocSprite(1);
    vertex = (fldSpriteVertex_t *)sdf2DGetPrimPtr((s32)sprite);
    vertex->r = 0x80;
    vertex->g = 0x80;
    vertex->b = 0x80;
    vertex->a = alpha;
    vertex->corner[0].u = 0;
    vertex->corner[0].v = 0;
    vertex->corner[0].x = 0x6FF8;
    vertex->corner[0].y = 0x78FB;
    vertex->corner[0].mask = 0;
    vertex->corner[0].flag = 0;
    vertex->corner[1].u = 0x2000;
    vertex->corner[1].v = 0xDFF;
    vertex->corner[1].x = 0x8FF8;
    vertex->corner[1].y = 0x86FB;
    vertex->corner[1].mask = 0;
    vertex->corner[1].flag = 0;
    sdfDrawTagLink(list, (u32)sprite);
    surface = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))surface->regFunc)(surface, list);
}

void fldDrawPos(const f32 *pos) {
    Sprite3D_t packet;
    f32 half = 36.0f;

    packet.uv[0].u = 0;
    packet.uv[0].v = 0;
    packet.uv[1].u = 0x200;
    packet.uv[1].v = 0;
    packet.uv[2].u = 0x200;
    packet.uv[2].v = 0x200;
    packet.uv[3].u = 0;
    packet.uv[3].v = 0x200;
    packet.vt[0].x = -half;
    packet.vt[0].y = -half;
    packet.vt[1].x = half;
    packet.vt[1].y = -half;
    packet.vt[2].x = half;
    packet.vt[2].y = half;
    packet.vt[3].x = -half;
    packet.vt[3].y = half;
    packet.col = 0x8080FF80;
    packet.pos.x = pos[0];
    packet.pos.y = pos[1];
    packet.pos.z = pos[2];
    packet.rot = 0.0f;
    rsetOtOfs(0x2E);
    rsetTest(1, 0, 0x80, 3, 0, 0, 1, 2);
    rsetAlpha(0);
    drawSprite3D(D_0054EC60, &packet);
}

void fldDrawPosCol(const f32 *pos, s32 color) {
    Sprite3D_t packet;
    f32 half = 36.0f;

    packet.uv[0].u = 0;
    packet.uv[0].v = 0;
    packet.uv[1].u = 0x200;
    packet.uv[1].v = 0;
    packet.uv[2].u = 0x200;
    packet.uv[2].v = 0x200;
    packet.uv[3].u = 0;
    packet.uv[3].v = 0x200;
    packet.vt[0].x = -half;
    packet.vt[0].y = -half;
    packet.vt[1].x = half;
    packet.vt[1].y = -half;
    packet.vt[2].x = half;
    packet.vt[2].y = half;
    packet.vt[3].x = -half;
    packet.vt[3].y = half;
    packet.col = color;
    packet.pos.x = pos[0];
    packet.pos.y = pos[1];
    packet.pos.z = pos[2];
    packet.rot = 0.0f;
    rsetOtOfs(0x2E);
    rsetTest(1, 0, 0x80, 3, 0, 0, 1, 2);
    rsetAlpha(0);
    drawSprite3D(D_0054EC60, &packet);
}

void fldDraw2DBar(s32 x, s32 y, s32 width, s32 height) {
    DrwSprite2D(x, y, 0x10, height, 0, 0, 0x10, 0x20, 0x60000040, D_0054EC60);
    DrwSprite2D(x + 0x10, y, width - 0x20, height, 0x10, 0, 1, 0x20, 0x60000040, D_0054EC60);
    DrwSprite2D(x + width - 0x10, y, 0x10, height, 0x10, 0, 0x10, 0x20, 0x60000040, D_0054EC60);
    rsetTest(1, 5, 0x80, 1, 0, 0, 1, 2);
    rsetAlpha(0);
}

void fldDrwZAlloc(void)
{
    if (fldDrwZBuf == NULL) {
        fldDrwZBuf = sdfAllocAuto(0x70000);
    }
}

void fldDrwZFree(void)
{
    if (fldDrwZBuf != NULL) {
        sdfFreeAutoDelayed(fldDrwZBuf);
        fldDrwZBuf = NULL;
    }
}

void fldDrwZSave(void) {
    if (fldDrwZBuf != 0) {
        sdfDrawTag_t *packet = sdfPrimAllocDrawTag(0);
        sdfGrOt_t *descriptor;
        sdfLinkStoreImage(packet, D_00529388.zbuffHandle,
                                0, 0, 0x200, 0xE0, (s32)fldDrwZBuf, 0, 0, 0);
        descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
        ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, packet);
    }
}

void fldDrwZLoad(void) {
    if (fldDrwZBuf != 0) {
        sdfDrawTag_t *packet = sdfPrimAllocDrawTag(0);
        sdfGrOt_t *descriptor;
        sdfLinkLoadImage(packet, D_00529388.zbuffHandle,
                                  0, 0, 0x200, 0xE0, (s32)fldDrwZBuf, 0);
        descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
        ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, packet);
    }
}

void fldKanjiDraw(s32 x, s32 y, u32 colors, const u8 *text)
{
    frFontGlyph_t *object;

    object = frwGetSmallFontFRQ(x << 4, y << 4, 0, colors, text, 0);
    frDrawFont(object);
    frDestroyFontRequestAfter(object);
}

void LF(dbgConsCtl_t *cons)
{
    cons->cy += 0x60;
}

void initDrawTag(dbgConsCtl_t *quad) {
    s32 packet;
    sdfDrawTag_t *packetList;

    packetList = dds3FontReqAllocDrawTag();
    quad->tag = packetList;
    packet = sdfGrAllocPacketBuff(0x40);
    sdfPrimSetOpaquePacket((sdfPrimTransPacket_t *)packet);
    sdfDrawTagLink(quad->tag, packet);
}

void fldDbgPrint3D(f32 x, f32 y, f32 z, s32 drawValue) {
    dbgConsCtl_t quad;
    sifCommand_t packet;
    f32 screenX;
    f32 screenY;

    SDF_VU0_LOADMATRIX(D_002FA240);
    InversMatrix();
    fldPoint3To2(&screenX, &screenY, x, y, z);
    if (screenX < -4000.0f || screenX > 4000.0f ||
        screenY < -4000.0f || screenY > 4000.0f) {
        return;
    }

    quad.cx = 0x73C0;
    quad.cy = 0x8440;
    quad.cz = 0x0FFFFF80;
    quad.sy = 0;
    quad.panelCol = 0x10000000;
    quad.frameCol = 0x80806020;
    quad.px = 0x73C0;
    quad.py = 0x8440;
    quad.sx = 0x1A40;
    quad.pz = 0x0FFFFF7F;
    initDrawTag(&quad);
    sdfDevSetPrintCB(&packet, (s32)screenX * 16, (s32)screenY * 16, quad.cz, 1);
    sdfDrawTagLink(quad.tag,
                    (u32)sdfDevPrintfEx(&packet, D_0054EC50, drawValue));
    LF(&quad);
    {
        sdfGrOt_t *textOt = &sdfGrOtTable[81];
        ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))textOt->regFunc)(textOt, quad.tag);
    }
}

void fldDbgPrint2D(s32 x, s32 y, const char *format) {
    dbgConsCtl_t quad;
    sifCommand_t packet;

    quad.cx = 0x73C0;
    quad.cy = 0x7CC0;
    quad.cz = 0x0FFFFF7E;
    quad.sy = 0;
    quad.panelCol = 0x10000000;
    quad.frameCol = 0x80806020;
    quad.px = 0x73C0;
    quad.py = 0x7CC0;
    quad.sx = 0x1A40;
    quad.pz = 0x0FFFFF7D;
    initDrawTag(&quad);
    sdfDevSetPrintCB(&packet, quad.cx + x, quad.cy + y, quad.cz, 0);
    sdfDrawTagLink(quad.tag, (u32)sdfDevPrintfEx(&packet, format));
    LF(&quad);
    {
        sdfGrOt_t *textOt = &sdfGrOtTable[83];
        ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))textOt->regFunc)(textOt, quad.tag);
    }
}

void fldDbgPrint2DC(s32 x, s32 y, s32 packetField, s32 drawValue) {
    dbgConsCtl_t quad;
    sifCommand_t packet;

    quad.cx = 0x73C0;
    quad.cy = 0x7CC0;
    quad.cz = 0x0FFFFF80;
    quad.sy = 0;
    quad.panelCol = 0x10000000;
    quad.frameCol = 0x80806020;
    quad.px = 0x73C0;
    quad.py = 0x7CC0;
    quad.sx = 0x1A40;
    quad.pz = 0x0FFFFF7F;
    initDrawTag(&quad);
    sdfDevSetPrintCB(&packet, quad.cx + x, quad.cy + y, quad.cz, packetField);
    sdfDrawTagLink(quad.tag, (u32)sdfDevPrintfEx(&packet, (const char *)drawValue));
    LF(&quad);
    {
        sdfGrOt_t *textOt = &sdfGrOtTable[81];
        ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))textOt->regFunc)(textOt, quad.tag);
    }
}

void fldDbgPrintNum2D(s32 x, s32 y, s32 drawValue) {
    dbgConsCtl_t quad;
    sifCommand_t packet;

    quad.cx = 0x7000;
    quad.cy = 0x7900;
    quad.cz = 0x0FFFFF80;
    quad.sy = 0;
    quad.panelCol = 0x10000000;
    quad.frameCol = 0x80806020;
    quad.px = 0x7000;
    quad.py = 0x7900;
    quad.sx = 0x1A40;
    quad.pz = 0x0FFFFF7F;
    initDrawTag(&quad);
    sdfDevSetPrintCB(&packet, quad.cx + x * 16, quad.cy + y * 8, quad.cz, 0);
    sdfDrawTagLink(quad.tag, (u32)sdfDevPrintfEx(&packet, D_0054EC50, drawValue));
    LF(&quad);
    {
        sdfGrOt_t *textOt = &sdfGrOtTable[81];
        ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))textOt->regFunc)(textOt, quad.tag);
    }
}

void fldDbgPrintNum2DH(s32 x, s32 y, s32 drawValue) {
    dbgConsCtl_t quad;
    sifCommand_t packet;

    quad.cx = 0x7000;
    quad.cy = 0x7900;
    quad.cz = 0x0FFFFF80;
    quad.sy = 0;
    quad.panelCol = 0x10000000;
    quad.frameCol = 0x80806020;
    quad.px = 0x7000;
    quad.py = 0x7900;
    quad.sx = 0x1A40;
    quad.pz = 0x0FFFFF7F;
    initDrawTag(&quad);
    sdfDevSetPrintCB(&packet, quad.cx + x * 16, quad.cy + y * 8, quad.cz, 0);
    sdfDrawTagLink(quad.tag, (u32)sdfDevPrintfEx(&packet, D_0054EC58, drawValue));
    LF(&quad);
    {
        sdfGrOt_t *textOt = &sdfGrOtTable[81];
        ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))textOt->regFunc)(textOt, quad.tag);
    }
}

void fldDbgPrintStr2D(s32 x, s32 y, s32 drawValue) {
    dbgConsCtl_t quad;
    sifCommand_t packet;

    quad.cx = 0x7000;
    quad.cy = 0x7900;
    quad.cz = 0x0FFFFF80;
    quad.sy = 0;
    quad.panelCol = 0x10000000;
    quad.frameCol = 0x80806020;
    quad.px = 0x7000;
    quad.py = 0x7900;
    quad.sx = 0x1A40;
    quad.pz = 0x0FFFFF7F;
    initDrawTag(&quad);
    sdfDevSetPrintCB(&packet, quad.cx + x * 16, quad.cy + y * 8, quad.cz, 0);
    sdfDrawTagLink(quad.tag, (u32)sdfDevPrintfEx(&packet, (const char *)drawValue));
    LF(&quad);
    {
        sdfGrOt_t *textOt = &sdfGrOtTable[81];
        ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))textOt->regFunc)(textOt, quad.tag);
    }
}

void fldDbgPrintStr2DC(s32 x, s32 y, s32 packetField, s32 drawValue) {
    dbgConsCtl_t quad;
    sifCommand_t packet;

    quad.cx = 0x7000;
    quad.cy = 0x7900;
    quad.cz = 0x0FFFFF80;
    quad.sy = 0;
    quad.panelCol = 0x10000000;
    quad.frameCol = 0x80806020;
    quad.px = 0x7000;
    quad.py = 0x7900;
    quad.sx = 0x1A40;
    quad.pz = 0x0FFFFF7F;
    initDrawTag(&quad);
    sdfDevSetPrintCB(&packet, quad.cx + x * 16, quad.cy + y * 8, quad.cz, packetField);
    sdfDrawTagLink(quad.tag, (u32)sdfDevPrintfEx(&packet, (const char *)drawValue));
    LF(&quad);
    {
        sdfGrOt_t *textOt = &sdfGrOtTable[81];
        ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))textOt->regFunc)(textOt, quad.tag);
    }
}

void fldDbgPrintStr2DCF(s32 packetField, s32 drawValue, f32 x, f32 y) {
    dbgConsCtl_t quad;
    sifCommand_t packet;

    quad.cx = 0x7000;
    quad.cy = 0x7900;
    quad.cz = 0x0FFFFF80;
    quad.sy = 0;
    quad.panelCol = 0x10000000;
    quad.frameCol = 0x80806020;
    quad.px = 0x7000;
    quad.py = 0x7900;
    quad.sx = 0x1A40;
    quad.pz = 0x0FFFFF7F;
    initDrawTag(&quad);
    sdfDevSetPrintCB(&packet, quad.cx + (s32)(x * 16.0f), quad.cy + (s32)(y * 8.0f), quad.cz, packetField);
    sdfDrawTagLink(quad.tag, (u32)sdfDevPrintfEx(&packet, (const char *)drawValue));
    LF(&quad);
    {
        sdfGrOt_t *textOt = &sdfGrOtTable[81];
        ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))textOt->regFunc)(textOt, quad.tag);
    }
}

void fldDrawMaruKage(u32 fade, f32 x, f32 y, f32 z, f32 radius) {
    s32 color;
    s32 angle;
    s32 prev;
    s32 i;
    f32 a;
    f32 b;
    f32 sinA;
    f32 cosA;
    f32 sinB;
    f32 cosB;

    if (fade != 0) {
        color = fade & 0xFF;
        rsetOtOfs(0x2E);
        rsetAlpha(5);
        if (color > 0x80) {
            color = 0x80;
        }
        color = 0x80 - color;
        if (color >= 0x80) {
            color = 0x7F;
        }
        if (color < 0x10) {
            color = 0x10;
        }
        color = (color << 24) | 0x10101;
        for (i = 0; i < 360; i += 10) {
            angle = (i + 360) % 360;
            prev = (i + 350) % 360;
            a = (f32)angle * 3.14f / 180.0f;
            sinA = sdfSin(a) * radius;
            cosA = sdfCos(a) * radius;
            b = (f32)prev * 3.14f / 180.0f;
            sinB = sdfSin(b) * radius;
            cosB = sdfCos(b) * radius;
            fldDrawPoly3(color, 0x80101010, 0x80101010,
                                x + 0.0f, y - 2.0f, z + 0.0f,
                                x + sinA, y - 2.0f, z + cosA,
                                x + sinB, y - 2.0f, z + cosB);
        }
        rsetAlpha(0);
    }
}

s32 nbGetEncountOn(void)
{
    return gencOn;
}

void nbSetEncountMode(s32 mode)
{
    gencMode = mode;
}
INCLUDE_SDATA(const s32, "field/fldDraw", D_0054EC44);

INCLUDE_SDATA(const s32, "field/fldDraw", fldDrwZBuf);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054EC4C);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054EC50);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054EC58);

INCLUDE_SDATA(const s32, "field/fldDraw", fldDrawEnv);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054EC60);

INCLUDE_SDATA(const s32, "field/fldDraw", hPartialMultiBlur);

INCLUDE_SDATA(const s32, "field/fldDraw", gencMode);

