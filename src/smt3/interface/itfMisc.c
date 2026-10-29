#include "common.h"
#include "interface.h"
#include "libsdf.h"
#include "kernel.h"

/* Font chains and their children have different pool strides. Only the
 * coordinates, color word and links used by this unit are exposed. */
typedef struct frFontChild_t
{
    u8 unk00[0x10];
    union { u32 renderWord; } parentDimensionsOrRenderWord;
    u8 unk14[0x14];
    struct frFontChild_t *next;
} frFontChild_t;

typedef struct frFontRequest_t
{
    u32 unk00;
    s32 x;
    s32 y;
    u8 unk0C[0x10];
    frFontChild_t *firstChild;
    void *lastChild;
    struct frFontRequest_t *prev;
    u8 unk28[0x1C];
} frFontRequest_t;

typedef struct itfLink_t
{
    struct itfLink_t *previous;
    struct itfLink_t *next;
    s32 index;
    void *state;
    sdfMemHandle_t *resourceHandle;
} itfLink_t;

typedef struct
{
    itfLink_t *puse_top, *puse_tail, *pdisuse_top, *pdisuse_tail;
} itfmisclinkstack_t;

typedef struct { s32 x, y, z, w; } itfColorVector_t;

typedef struct { u8 unk00[0x20]; u64 value, registerCode; } itfGsState_t;

extern frFontRequest_t *frTestFontHandle;

extern u64 D_003263C0[];

extern u8 D_0054F470[8], D_0054F478[8], D_003263D8[], D_003263E0[];

extern itfColorVector_t D_003263E8[];

extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);


extern sdfTexHandle_t *sdfTexSet(void *aTex);

extern s32 sdf2DGetPrimPacketSize(s32 aNumOfRegs, s32 aNumOfPrim);

extern s32 sdfGrAllocPacketBuff(u32 aSize);

extern void sdf2DMakePrimPacketHeader(void *aPacketPtr, s32 aPrimReg, s32 aNumOfRegs, u64 aGifRegs, s32 aNumOfPrim);

extern s32 sdf2DGetPrimPtr(s32 aPacketPtr);

extern u32 sdf2DMakeStdPacket(u32 aPacket, s32 aLeng);

extern s32 sdf2DLinkTexPacket(sdfDrawTag_t *, sdfTexHandle_t *, s32);

extern void sdfDrawTagLink(sdfDrawTag_t *, u32 aPacket);

extern s32 dds3AdminiGetActiveSequence(void);

extern void dds3AdminiExitSequence(void);

extern s32 printf(const char *format, ...);

#define ITF_BYTE_MASK 0xFF

#define ITF_VECTOR_WORD_COUNT 4

#define ITF_FIXED_FRACTION_BITS 16

#define ITF_FIXED_COMPONENT_MAX 0xff0000

#define ITF_GS_X_BIAS 0x7000

#define ITF_GS_Y_BIAS 0x7900

#define ITF_TRIANGLE_VERTEX_COUNT 3

#define ITF_QUAD_VERTEX_COUNT 4

#define ITF_STATE_PACKET_BYTES 0x30

#define ITF_TEST_FIRST_CONTEXT 0x47

#define ITF_TEST_SECOND_CONTEXT 0x48

#define ITF_ALPHA_FIRST_CONTEXT 0x42

#define ITF_ALPHA_SECOND_CONTEXT 0x43

#define ITF_TRIANGLE_PRIMITIVE_BITS 0x4B

#define ITF_QUAD_FAN_PRIMITIVE_BITS 0x4D

#define ITF_TEXTURED_FAN_PRIMITIVE_BITS 0x5D

#define ITF_TEXTURED_SPRITE_PRIMITIVE_BITS 0x156

#define ITF_COLORED_SPRITE_PRIMITIVE_BITS 0x15E

#define ITF_SPRITE_PRIMITIVE_BITS 0x46

#define ITF_TRIANGLE_STRIP_PRIMITIVE_BITS 0x4C

#define ITF_LINE_STRIP_PRIMITIVE_BITS 0x14A

#define ITF_LINES_PRIMITIVE_BITS 0x49

/* Initialize a FIFO free list and empty active list.
 * Native do/while writes at least two nodes; the s8 index can wrap. */
void itfMiscLinkInitialize(itfmisclinkstack_t *pool, itfLink_t *nodes, s32 count, s32 stride) {
    s8 index = 0;
    itfLink_t *previous = NULL;
    itfLink_t *node = nodes;
    itfLink_t *next;

    do {
        count--;
        next = (itfLink_t *)((u8 *)node + stride);
        node->previous = previous;
        node->index = index;
        index++;
        node->next = next;
        previous = node;
        node = next;
    } while (count >= 2);
    node->previous = previous;
    node->index = index;
    node->next = NULL;
    pool->pdisuse_tail = node;
    pool->pdisuse_top = nodes;
    pool->puse_tail = NULL;
    pool->puse_top = NULL;
}

/* Move the free-list head to the active-list tail; exhausted pools return NULL. */
itfLink_t *itfMiscLinkTake(itfmisclinkstack_t *pstack) {
    itfLink_t *node = pstack->pdisuse_top;
    itfLink_t *next;

    if (node == NULL) {
        return NULL;
    }
    next = node->next;
    if (pstack->puse_top != NULL) {
        node->previous = pstack->puse_tail;
        pstack->puse_tail->next = node;
    } else {
        node->previous = NULL;
        pstack->puse_top = node;
    }
    node->next = NULL;
    pstack->puse_tail = node;
    if (next != NULL) {
        next->previous = NULL;
    } else {
        pstack->pdisuse_tail = NULL;
    }
    pstack->pdisuse_top = next;
    return node;
}

/* Unlink an active node and append it to the free-list tail; membership is trusted. */
void itfMiscLinkGive(itfLink_t *pelem, itfmisclinkstack_t *pstack) {
    itfLink_t *previous = pelem->previous;
    itfLink_t *next = pelem->next;

    if (previous != NULL) {
        previous->next = next;
    } else {
        pstack->puse_top = next;
    }
    if (next != NULL) {
        next->previous = previous;
    } else {
        pstack->puse_tail = previous;
    }
    pelem->next = NULL;
    {
        itfLink_t *lastFree = pstack->pdisuse_tail;
        pelem->previous = lastFree;
        if (lastFree != NULL) {
            lastFree->next = pelem;
        }
    }
    pstack->pdisuse_tail = pelem;
    if (pstack->pdisuse_top == NULL) {
        pstack->pdisuse_top = pelem;
    }
}

/* Multiply XYZ, cap only the positive product at 255 << 16, then shift by 16.
 * Negative products are not clamped; W comes from the argument. */
void itfMiscSetRatioRGB(itfColorVector_t *output, s32 scaleX, s32 scaleY, s32 scaleZ,
                   s32 w, const itfColorVector_t *input, s32 count) {
    while (count > 0) {
        s32 x = scaleX * input->x;
        s32 y = scaleY * input->y;
        s32 z = scaleZ * input->z;

        output->w = w;
        if (x > ITF_FIXED_COMPONENT_MAX) x = ITF_FIXED_COMPONENT_MAX;
        if (y > ITF_FIXED_COMPONENT_MAX) y = ITF_FIXED_COMPONENT_MAX;
        if (z > ITF_FIXED_COMPONENT_MAX) z = ITF_FIXED_COMPONENT_MAX;
        output->x = x >> ITF_FIXED_FRACTION_BITS;
        output->y = y >> ITF_FIXED_FRACTION_BITS;
        output->z = z >> ITF_FIXED_FRACTION_BITS;
        output++;
        input++;
        count--;
    }
}

/* Replace each child color; parent color words are not modified. */
void itfMiscChangeFontVertexColor(frFontRequest_t *pfont, u32 color) {
    for (; pfont != NULL; pfont = pfont->prev) {
        frFontChild_t *child;
        for (child = pfont->firstChild; child != NULL; child = child->next) {
            child->parentDimensionsOrRenderWord.renderWord = color;
        }
    }
}

/* Clear each child's low byte, then OR unmasked bits into the full color word. */
void itfMiscChangeFontAlpha(frFontRequest_t *pfont, u32 alpha) {
    for (; pfont != NULL; pfont = pfont->prev) {
        frFontChild_t *child;
        for (child = pfont->firstChild; child != NULL; child = child->next) {
            child->parentDimensionsOrRenderWord.renderWord = (child->parentDimensionsOrRenderWord.renderWord & ~ITF_BYTE_MASK) | alpha;
        }
    }
}

/* Translate parent entries only; child coordinates are left unchanged. */
void itfMiscMoveFontPosition(frFontRequest_t *pfont, u32 ofsx, u32 ofsy) {
    for (; pfont != NULL; pfont = pfont->prev) {
        pfont->x += ofsx;
        pfont->y += ofsy;
    }
}

/* Keep the sprite texture handle while releasing the temporary file allocation. */
sdfTexHandle_t *itfMiscLoadTexture(const char *filename) {
    sdfMemHandle_t *fileAllocation;
    sdfTexHandle_t *textureHandle;
    u32 assetInfo[ITF_VECTOR_WORD_COUNT];

    fileAllocation = sdfDevLoadFileEx(filename, assetInfo, 0);
    textureHandle = sdfTexSet((void *)(assetInfo[0]));
    sdfMemFreeDirect(fileAllocation);
    return textureHandle;
}

/* Emit three per-vertex RGBA/XYZ2 records; PRIM's low bits select a triangle.
 * Coordinates receive the native GS screen biases; no clipping is performed. */
void itfMiscDrawTrianglePrim(sdfIVector2_t *pvertex, itfColor_t *pcolor, s32 x, s32 y, u32 z, sdfDrawTag_t *pdrawtag) {
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    s32 i;

    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, ITF_TRIANGLE_VERTEX_COUNT));
    sdf2DMakePrimPacketHeader(packet, ITF_TRIANGLE_PRIMITIVE_BITS, 2, 0x51, ITF_TRIANGLE_VERTEX_COUNT);
    dst = (u64 *)sdf2DGetPrimPtr((s32)packet);
    for (i = 0; i < ITF_TRIANGLE_VERTEX_COUNT; i++) {
        dst[0] = (u64)pcolor->components[0] | ((u64)pcolor->components[1] << 32);
        dst[1] = (u64)pcolor->components[2] | ((u64)pcolor->components[3] << 32);
        pcolor++;
        dst += 2;
        dst[1] = (u64)z;
        dst[0] = (u64)(u32)(pvertex->x + x + ITF_GS_X_BIAS) | ((u64)(pvertex->y + y + ITF_GS_Y_BIAS) << 32);
        pvertex++;
        dst += 2;
    }
    sdfDrawTagLink(pdrawtag, (u32)packet);
}

/* Emit an indexed four-vertex triangle fan with a color record per vertex. */
void itfMiscDrawRectanglePrim(sdfIVector2_t *vertices, itfColor_t *colors, u8 *vertexIndex, u8 *colorIndex, u32 tail, sdfDrawTag_t *command) {
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    s32 i;

    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, ITF_QUAD_VERTEX_COUNT));
    sdf2DMakePrimPacketHeader(packet, ITF_QUAD_FAN_PRIMITIVE_BITS, 2, 0x51, ITF_QUAD_VERTEX_COUNT);
    dst = (u64 *)sdf2DGetPrimPtr((s32)packet);
    for (i = 0; i < ITF_QUAD_VERTEX_COUNT; i++) {
        sdfIVector2_t *vertex = &vertices[*vertexIndex++];
        itfColor_t *color = &colors[*colorIndex++];

        dst[0] = (u64)color->components[0] | ((u64)color->components[1] << 32);
        dst[1] = (u64)color->components[2] | ((u64)color->components[3] << 32);
        dst += 2;
        dst[1] = (u64)tail;
        dst[0] = (u64)(u32)(vertex->x + ITF_GS_X_BIAS) | ((u64)(vertex->y + ITF_GS_Y_BIAS) << 32);
        dst += 2;
    }
    sdfDrawTagLink(command, (u32)packet);
}

/* Emit a textured triangle fan: each four-float input supplies S/T/Q only.
 * The fourth float is skipped, and the context word is shifted without masking. */
void itfMiscDrawTexPoly(sdfIVector2_t *pvertex, f32 *pstq, itfColor_t *pcolor, u32 z, s32 context, sdfDrawTag_t *pdrawtag) {
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    s32 i;

    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(3, ITF_QUAD_VERTEX_COUNT));
    sdf2DMakePrimPacketHeader(packet, (context << 9) | ITF_TEXTURED_FAN_PRIMITIVE_BITS, 3, 0x512, ITF_QUAD_VERTEX_COUNT);
    dst = (u64 *)sdf2DGetPrimPtr((s32)packet);
    for (i = 0; i < ITF_QUAD_VERTEX_COUNT; i++) {
        f32 *uvDst = (f32 *)dst;

        uvDst[0] = pstq[0];
        uvDst[1] = pstq[1];
        uvDst[2] = pstq[2];
        pstq += 4;
        dst += 2;
        dst[0] = (u64)pcolor->components[0] | ((u64)pcolor->components[1] << 32);
        dst[1] = (u64)pcolor->components[2] | ((u64)pcolor->components[3] << 32);
        pcolor++;
        dst += 2;
        dst[1] = (u64)z;
        dst[0] = (u64)(u32)(pvertex->x + ITF_GS_X_BIAS) | ((u64)(pvertex->y + ITF_GS_Y_BIAS) << 32);
        pvertex++;
        dst += 2;
    }
    sdfDrawTagLink(pdrawtag, (u32)packet);
}

/* Bind the texture and emit one two-corner sprite using two packed UV pairs.
 * Unwritten padding words in the native packet remain untouched. */
void itfMiscDrawSpritePrim(void *vertexData, void *uvData, void *colorData, s32 tail, sdfTexHandle_t *texture, s32 flag, sdfDrawTag_t *command) {
    sdfIVector2_t *vertices = vertexData;
    itfColor_t *uv = uvData;
    itfColor_t *colors = colorData;
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    s32 x0, y0, x1, y1;

    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(5, 1));
    sdf2DMakePrimPacketHeader(packet, (flag << 9) | ITF_TEXTURED_SPRITE_PRIMITIVE_BITS, 5, 0x53531, 1);
    x0 = vertices[0].x + ITF_GS_X_BIAS;
    y0 = vertices[0].y + ITF_GS_Y_BIAS;
    x1 = vertices[1].x + ITF_GS_X_BIAS;
    y1 = vertices[1].y + ITF_GS_Y_BIAS;
    dst = (u64 *)sdf2DGetPrimPtr((s32)packet);
    dst[0] = (u64)colors->components[0] | ((u64)colors->components[1] << 32);
    dst[1] = (u64)colors->components[2] | ((u64)colors->components[3] << 32);
    dst[2] = (u64)uv->components[0] | ((u64)uv->components[1] << 32);
    dst[4] = (u64)(u32)x0 | ((u64)y0 << 32);
    dst[5] = (u64)(u32)tail;
    dst[6] = (u64)uv->components[2] | ((u64)uv->components[3] << 32);
    dst[8] = (u64)(u32)x1 | ((u64)y1 << 32);
    dst[9] = (u64)(u32)tail;
    sdf2DLinkTexPacket(command, texture, flag);
    sdfDrawTagLink(command, (u32)packet);
}

/* Emit a colored textured sprite without binding a texture in this function. */
void itfMiscDrawSpritePrim2(sdfIVector2_t *pos, itfColor_t *uv, itfColor_t *color, u32 z, s32 context, sdfDrawTag_t *pdrawtag) {
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    s32 x0, y0, x1, y1;

    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(5, 1));
    sdf2DMakePrimPacketHeader(packet, (context << 9) | ITF_COLORED_SPRITE_PRIMITIVE_BITS, 5, 0x53531, 1);
    x0 = pos[0].x + ITF_GS_X_BIAS;
    y0 = pos[0].y + ITF_GS_Y_BIAS;
    x1 = pos[1].x + ITF_GS_X_BIAS;
    y1 = pos[1].y + ITF_GS_Y_BIAS;
    dst = (u64 *)sdf2DGetPrimPtr((s32)packet);
    dst[0] = (u64)color->components[0] | ((u64)color->components[1] << 32);
    dst[1] = (u64)color->components[2] | ((u64)color->components[3] << 32);
    dst[2] = (u64)uv->components[0] | ((u64)uv->components[1] << 32);
    dst[4] = (u64)(u32)x0 | ((u64)y0 << 32);
    dst[5] = (u64)z;
    dst[6] = (u64)uv->components[2] | ((u64)uv->components[3] << 32);
    dst[8] = (u64)(u32)x1 | ((u64)y1 << 32);
    dst[9] = (u64)z;
    sdfDrawTagLink(pdrawtag, (u32)packet);
}

/* Emit an untextured two-corner GS sprite. */
void itfMiscDrawSpritePlanePrim(sdfIVector2_t *pos, itfColor_t *color, u32 z, s32 context, sdfDrawTag_t *pdrawtag) {
    sdfGrPacketHeader_t *packet;
    u64 *dst;

    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(3, 1));
    sdf2DMakePrimPacketHeader(packet, (context << 9) | ITF_SPRITE_PRIMITIVE_BITS, 3, 0x551, 1);
    dst = (u64 *)sdf2DGetPrimPtr((s32)packet);
    dst[0] = (u64)color->components[0] | ((u64)color->components[1] << 32);
    dst[1] = (u64)color->components[2] | ((u64)color->components[3] << 32);
    dst += 2;
    dst[1] = (u64)z;
    dst[0] = (u64)(u32)(pos[0].x + ITF_GS_X_BIAS) | ((u64)(pos[0].y + ITF_GS_Y_BIAS) << 32);
    dst += 2;
    dst[1] = (u64)z;
    dst[0] = (u64)(u32)(pos[1].x + ITF_GS_X_BIAS) | ((u64)(pos[1].y + ITF_GS_Y_BIAS) << 32);
    sdfDrawTagLink(pdrawtag, (u32)packet);
}

/* Emit an indexed triangle strip, packing two vertices per GIF loop.
 * Header loop count truncates odd vertex counts; the native writer does not. */
void itfMiscDrawTriangleStripPrim(sdfIVector2_t *vertices, itfColor_t *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, sdfDrawTag_t *command) {
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    s32 pairCount = count >> 1;
    s32 i;

    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(4, pairCount));
    sdf2DMakePrimPacketHeader(packet, ITF_TRIANGLE_STRIP_PRIMITIVE_BITS, 4, 0x5151, pairCount);
    dst = (u64 *)sdf2DGetPrimPtr((s32)packet);
    for (i = 0; i < count; i++) {
        sdfIVector2_t *vertex = &vertices[*vertexIndex++];
        itfColor_t *color = &colors[*colorIndex++];

        dst[0] = (u64)color->components[0] | ((u64)color->components[1] << 32);
        dst[1] = (u64)color->components[2] | ((u64)color->components[3] << 32);
        dst += 2;
        dst[1] = (u64)tail;
        dst[0] = (u64)(u32)(vertex->x + ITF_GS_X_BIAS) | ((u64)(vertex->y + ITF_GS_Y_BIAS) << 32);
        dst += 2;
    }
    sdfDrawTagLink(command, (u32)packet);
}

/* Emit indexed RGBA/XYZ2 records with the line-strip primitive. */
void itfMiscDrawLineStripPrim(sdfIVector2_t *vertices, itfColor_t *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, sdfDrawTag_t *command) {
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    s32 i;

    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, count));
    sdf2DMakePrimPacketHeader(packet, ITF_LINE_STRIP_PRIMITIVE_BITS, 2, 0x51, count);
    dst = (u64 *)sdf2DGetPrimPtr((s32)packet);
    for (i = 0; i < count; i++) {
        sdfIVector2_t *vertex = &vertices[*vertexIndex++];
        itfColor_t *color = &colors[*colorIndex++];

        dst[0] = (u64)color->components[0] | ((u64)color->components[1] << 32);
        dst[1] = (u64)color->components[2] | ((u64)color->components[3] << 32);
        dst += 2;
        dst[1] = (u64)tail;
        dst[0] = (u64)(u32)(vertex->x + ITF_GS_X_BIAS) | ((u64)(vertex->y + ITF_GS_Y_BIAS) << 32);
        dst += 2;
    }
    sdfDrawTagLink(command, (u32)packet);
}

/* Emit indexed RGBA/XYZ2 records with independent line primitives. */
void itfMiscDrawLinePrim(sdfIVector2_t *vertices, itfColor_t *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, sdfDrawTag_t *command) {
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    s32 i;

    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, count));
    sdf2DMakePrimPacketHeader(packet, ITF_LINES_PRIMITIVE_BITS, 2, 0x51, count);
    dst = (u64 *)sdf2DGetPrimPtr((s32)packet);
    for (i = 0; i < count; i++) {
        sdfIVector2_t *vertex = &vertices[*vertexIndex++];
        itfColor_t *color = &colors[*colorIndex++];

        dst[0] = (u64)color->components[0] | ((u64)color->components[1] << 32);
        dst[1] = (u64)color->components[2] | ((u64)color->components[3] << 32);
        dst += 2;
        dst[1] = (u64)tail;
        dst[0] = (u64)(u32)(vertex->x + ITF_GS_X_BIAS) | ((u64)(vertex->y + ITF_GS_Y_BIAS) << 32);
        dst += 2;
    }
    sdfDrawTagLink(command, (u32)packet);
}

/* Write TEST for the selected GS context, not the ALPHA blend register. */
void itfMiscDrawTestPrim(sdfDrawTag_t *pdrawtag, u64 para, s32 context) {
    s32 packet = sdfGrAllocPacketBuff(ITF_STATE_PACKET_BYTES);
    itfGsState_t *dst = (itfGsState_t *)sdf2DMakeStdPacket(packet, ITF_STATE_PACKET_BYTES);

    dst->value = para;
    dst->registerCode = context ? ITF_TEST_SECOND_CONTEXT : ITF_TEST_FIRST_CONTEXT;
    sdfDrawTagLink(pdrawtag, packet);
}

/* Write an unchecked table entry to ALPHA for the selected GS context. */
void itfMiscDrawAlphaPrim(sdfDrawTag_t *pdrawtag, s32 tp, s32 context) {
    s32 packet = sdfGrAllocPacketBuff(ITF_STATE_PACKET_BYTES);
    itfGsState_t *dst = (itfGsState_t *)sdf2DMakeStdPacket(packet, ITF_STATE_PACKET_BYTES);

    dst->value = D_003263C0[tp];
    dst->registerCode = context ? ITF_ALPHA_SECOND_CONTEXT : ITF_ALPHA_FIRST_CONTEXT;
    sdfDrawTagLink(pdrawtag, packet);
}

/* Write CLAMP for the selected GS context. */
void itfMiscDrawClampPrim(sdfDrawTag_t *pdrawtag, s32 context, s32 wms, s32 wmt, s32 min_u, s32 max_u, s32 min_v, s32 max_v) {
    s32 packet = sdfGrAllocPacketBuff(ITF_STATE_PACKET_BYTES);
    itfGsState_t *dst = (itfGsState_t *)sdf2DMakeStdPacket(packet, ITF_STATE_PACKET_BYTES);

    dst->value = ((u64)max_v << 34) | ((u64)min_v << 24) | (max_u << 14) | (min_u << 4) | (wmt << 2) | wms;
    dst->registerCode = context ? 9 : 8;
    sdfDrawTagLink(pdrawtag, packet);
}

/* Draw filled and remaining quad regions. The optional highlight is produced
 * as signed fixed-point vectors and passed as their four-word RGBA image. */
void itfMiscDrawGauge(s32 portion, s32 entire, s32 shadow,
                   sdfIVector2_t *pos, u32 z, itfColor_t *pcolor_portion,
                   itfColor_t *pcolor_entire, sdfDrawTag_t *pdrawtag) {
    sdfIVector2_t vertices[6];
    itfColorVector_t highlightColors[4];
    s32 left = pos[0].x;
    s32 right = pos[1].x;
    s32 width = (right - left) >> 4;
    s32 filledWidth;

    if (entire > 0) {
        filledWidth = (((portion << 16) / entire) * width) >> 12;
    } else {
        filledWidth = 0;
    }
    vertices[0].x = left;
    vertices[0].y = pos[0].y;
    vertices[1].x = filledWidth + left;
    vertices[1].y = pos[0].y;
    vertices[2].x = right;
    vertices[2].y = pos[0].y;
    vertices[3].x = left;
    vertices[3].y = pos[1].y;
    vertices[4].x = filledWidth + left;
    vertices[4].y = pos[1].y;
    vertices[5].x = right;
    vertices[5].y = pos[1].y;
    itfMiscDrawRectanglePrim(vertices, pcolor_portion, D_0054F470, D_0054F478, z, pdrawtag);
    itfMiscDrawRectanglePrim(vertices, pcolor_entire, D_0054F470 + 4, D_0054F478, z, pdrawtag);
    if (shadow != 0 && filledWidth >= 3) {
        itfMiscSetRatioRGB(highlightColors, pcolor_portion->components[0],
                        pcolor_portion->components[1], pcolor_portion->components[2],
                        pcolor_portion->components[3], D_003263E8, 4);
        itfMiscDrawLineStripPrim(vertices, (itfColor_t *)highlightColors,
                        D_003263D8, D_003263E0, 3, z, pdrawtag);
        itfMiscDrawLineStripPrim(vertices, (itfColor_t *)highlightColors,
                        D_003263D8 + 3, D_003263E0 + 3, 3, z, pdrawtag);
    }
}

/* Retail "DbgDevDump" task callback: perform no dump work and return zero. */
s32 dds3DebugDevDumpProcess(dds3ProcessID_t *proc_id) {
    return 0;
}

u32 itfCamp(void) {
    return 1;
}

/* Exit the active sequence unless already in camp, then report the halt. */
void itfCampStartProcess(void) {
    if (dds3AdminiGetActiveSequence() != 6) {
        dds3AdminiExitSequence();
    }
    printf("Camp process halted. \n");
}

