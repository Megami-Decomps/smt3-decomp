#include "common.h"

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
    struct frFontRequest_t *previous;
    u8 unk28[0x1C];
} frFontRequest_t;
typedef struct frRelativeImage_t
{
    s32 firstOffset;
    s32 secondDelta;
    u8 unk08[0x10];
    s32 thirdDelta;
} frRelativeImage_t;
typedef struct frImagePtrs_t
{
    void *treeNodes;
    void *entryBitOffsets;
    void *packedBitstream;
} frImagePtrs_t;
typedef struct { u32 start, end; } frBitRange_t;
typedef struct frFontNode_t
{
    u32 slotIndex;
    struct frFontNode_t *next;
} frFontNode_t;
typedef struct
{
    sdfMemHandle_t *allocation;
    frFontNode_t first;
} frFontRing_t;
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
    itfLink_t *activeHead, *activeTail, *firstFree, *lastFree;
} itfLinkPool_t;
typedef struct { s32 x, y, z, w; } itfColorVector_t;
typedef struct { s32 x, y; } itfVertex_t;
typedef struct { u32 components[4]; } itfColor_t;
typedef struct { u8 unk00[0x20]; u64 value, registerCode; } itfGsState_t;

extern u32 frFontRowBuf;
extern frFontRequest_t *frTestFontHandle;
extern u64 D_003263C0[];
extern u8 D_0054F470[8], D_0054F478[8], D_003263D8[], D_003263E0[];
extern itfColorVector_t D_003263E8[];
extern u32 strlen(const char *text);
extern void *memcpy(void *dest, const void *source, u32 size);
extern sdfMemHandle_t *sdfMemAllocDirect(u32 size);
extern void *sdfMemLock(sdfMemHandle_t *handle);
extern void sdfMemFreeDirect(sdfMemHandle_t *handle);
extern void frSetWhiteSpaceW(s32 font, s32 width);
extern void frEnableMode(s32 mode);
extern u8 frDisableMode(u8 mode);
extern frFontRequest_t *frLinkFontString(const char *, s8, s8, s8, frFontRequest_t *);
extern void frSetFontSpace(frFontRequest_t *, s32);
extern void frSetDrawPosition(frFontRequest_t *, u32, u32);
extern void frSetPriority(frFontRequest_t *, s32);
extern void frSetFontColor(frFontRequest_t *, u32);
extern void frSetFontColorNo(frFontRequest_t *, u8);
extern frFontRequest_t *frCombineFontRequest(frFontRequest_t *, frFontRequest_t *);
extern void frLoadFontData(s32 font, const char *path);
extern void frDelFontStyle(s32 font);
extern void fclDestroyMessage(void);
extern void frDestroyFontRequest(frFontRequest_t *request);
extern sdfMemHandle_t *sdfDevLoadFileEx(const char *, u32 *, s32);
extern sdfTexHandle_t *sdfTexSet(void *data);
extern s32 sdf2DGetPrimPacketSize(s32 registers, s32 count);
extern s32 sdfGrAllocPacketBuff(u32 size);
extern void sdf2DMakePrimPacketHeader(void *packet, s32 prim, s32 registers, u64 descriptor, s32 count);
extern s32 sdf2DGetPrimPtr(s32 packet);
extern u32 sdf2DMakeStdPacket(u32 packet, s32 size);
extern s32 sdf2DLinkTexPacket(sdfDrawTag_t *, sdfTexHandle_t *, s32);
extern void sdfDrawTagLink(sdfDrawTag_t *, u32 packet);
extern s32 dds3AdminiGetActiveSequence(void);
extern void dds3AdminiExitSequence(void);
extern s32 printf(const char *format, ...);

typedef struct { u8 unk00[0x10]; u32 headerWord; } frPackedHeader_t;

#define ITF_BYTE_MASK 0xFF
#define ITF_NODE_HEADER_BYTES 8
#define ITF_ALLOCATION_HANDLE_BYTES 4
#define ITF_VECTOR_WORD_COUNT 4
#define ITF_RGBA_COMPONENT_COUNT 4
#define ITF_FIXED_FRACTION_BITS 16
#define ITF_FIXED_COMPONENT_MAX 0xff0000
#define ITF_NEUTRAL_COLOR 0x80808080
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

INCLUDE_ASM(const s32, "code_001C9530", frInitNameData);

INCLUDE_ASM(const s32, "code_001C9530", frSetNameData);

INCLUDE_ASM(const s32, "code_001C9530", frTransNameImage);

INCLUDE_ASM(const s32, "code_001C9530", frRestoreNameImage);

INCLUDE_ASM(const s32, "code_001C9530", frCreateNameHandle);

INCLUDE_ASM(const s32, "code_001C9530", frGetName1NFRQ);

INCLUDE_ASM(const s32, "code_001C9530", frGetName2NFRQ);

INCLUDE_ASM(const s32, "code_001C9530", frGetNameFRQ);

INCLUDE_ASM(const s32, "code_001C9530", frGetCNameFRQ);

frFontRequest_t *frwGetStaffFontFRQ(u32 x, u32 y, s32 depth, s32 alt, s32 measureFlag, s32 entryFlag, u32 colors, void *source) {
    frFontRequest_t *glyph;
    s32 kind = 4;

    if (alt) {
        kind = 5;
    }
    frSetWhiteSpaceW(kind, entryFlag);
    frEnableMode(1);
    frDisableMode(2);
    frDisableMode(0x10);
    glyph = frLinkFontString(source, kind, 0, 0, 0);
    frEnableMode(0x10);
    frEnableMode(2);
    frDisableMode(1);
    frSetFontSpace(glyph, measureFlag);
    frSetDrawPosition(glyph, x, y);
    frSetPriority(glyph, depth << 4);
    frSetFontColor(glyph, colors);
    frSetFontColorNo(glyph, 5);
    return glyph;
}

void frwLoadStaffFont(void) {
    frLoadFontData(4, "/font/staff1.fnt");
    frLoadFontData(5, "/font/staff2.fnt");
}

void frwFreeStaffFont(void) {
    frDelFontStyle(4);
    frDelFontStyle(5);
}

INCLUDE_ASM(const s32, "code_001C9530", frwGetSmallFont1FRQ);

frFontRequest_t *frwGetFontEx(s32 x, s32 y, s32 depth, u32 colors, const char *text,
                         s8 glyphMode, s8 sharedFlag, frFontRequest_t *previousGlyph) {
    char glyphData[3];
    s32 length;
    s32 position = 0;
    s32 glyphLength;
    frFontRequest_t *chain = previousGlyph;

    length = strlen(text);
    if (sharedFlag == 0) {
        frEnableMode(1);
    }
    frDisableMode(2);

    if (length > 0) {
        do {
            const char *character = text + position;
            frFontRequest_t *glyph;

            if (character[0] >= 0) {
                glyphData[0] = character[0];
                glyphLength = 1;
                glyphData[1] = 0;
            } else {
                glyphData[0] = character[0];
                glyphData[1] = character[1];
                glyphLength = 2;
                glyphData[2] = 0;
            }
            glyph = frLinkFontString(glyphData, glyphMode, 0, 0, 0);
            frSetPriority(glyph, depth << 4);
            frSetFontColor(glyph, colors);
            frSetFontSpace(glyph, 3);
            chain = frCombineFontRequest(chain, glyph);
            if (position == 0) {
                glyph->x = x;
            }
            position += glyphLength;
            glyph->y = y;
        } while (position < length);
    }

    frEnableMode(2);
    if (sharedFlag == 0) {
        frDisableMode(1);
    }
    return chain;
}

frFontRequest_t *frwGetSmallFontFRQ(s32 x, s32 y, s32 depth, s32 colors, const char *text, frFontRequest_t *parent)
{
    frFontRequest_t *handle = frwGetFontEx(x, y, depth, colors, text, 1, 0, parent);

    frSetFontSpace(handle, 3);
    return handle;
}

INCLUDE_ASM(const s32, "code_001C9530", frwGetSmallFontFRQ2);

INCLUDE_ASM(const s32, "code_001C9530", frwGetSmallFontNoPropFRQ);

INCLUDE_ASM(const s32, "code_001C9530", frwGetSmallNumFontFRQ);

INCLUDE_ASM(const s32, "code_001C9530", frwGetNumFontFRQ);

INCLUDE_ASM(const s32, "code_001C9530", frwGetSmallCaptionFontFRQ);

INCLUDE_ASM(const s32, "code_001C9530", frwGetHelpFRQ);

INCLUDE_ASM(const s32, "code_001C9530", frGetHelpPos);

s32 fr_1CA990(void) {
    return 0;
}

void func_001CA998(void) {
}

void func_001CA9A0(void) {
}

void func_001CA9A8(void) {
}

u32 fr_1CA9B0(u32 unused0, u32 unused1, u32 value) {
    return value;
}

/* Return the byte count saved by the most recent packed-resource decode. */
u32 fr_1CA9B8(void) {
    return frFontRowBuf;
}

u32 frGetFontHeader(u32 resourceBase) {
    return resourceBase;
}

/* Resolve three signed, cumulative relative offsets without validating bounds. */
void frGetFontImagePtrs(frRelativeImage_t *block, frImagePtrs_t *out) {
    s32 firstOffset = block->firstOffset;
    s32 secondOffset = firstOffset + block->secondDelta;
    s32 thirdOffset = secondOffset + block->thirdDelta;

    out->treeNodes = (u8 *)block + firstOffset;
    out->entryBitOffsets = (u8 *)block + secondOffset;
    out->packedBitstream = (u8 *)block + thirdOffset;
}

/* Read the opaque header word at 0x10; its role is not established. */
u32 fr_1CA9F8(u32 resourceBase) {
    return ((frPackedHeader_t *)frGetFontHeader(resourceBase))->headerWord;
}

/* Decode entryIndex's bit span through the resource's six-byte prefix-tree records.
 * Write terminal bytes to decodedBytes and return/cache the count. The font atlas
 * uses these bytes as pixels; neither entry nor output bounds are checked. */
s32 frDecodeFontRow(u8 *decodedBytes, s32 entryIndex, void *resourceBase, frImagePtrs_t *segments) {
    frBitRange_t range;
    s32 produced;
    u8 *table = segments->treeNodes;
    u16 *bitWords;
    u32 wordIndex;
    u32 start;
    s32 remaining;
    u32 state;
    u16 *root;
    u16 *node;

    memcpy(&range.start, (u8 *)segments->entryBitOffsets + entryIndex * 4, 4);
    memcpy(&range.end, (u8 *)segments->entryBitOffsets + entryIndex * 4 + 4, 4);
    produced = 0;
    frGetFontHeader((u32)resourceBase);
    start = range.start;
    bitWords = segments->packedBitstream;
    wordIndex = start >> 4;
    remaining = range.end - start;
    state = (bitWords[wordIndex] | 0x10000) >> (start & 0xF);
    root = (u16 *)(table + 2);
    node = root;
    do {
        u16 index = node[state & 1];

        node = (u16 *)(table + index * 6 + 2);
        if (*node == 0) {
            decodedBytes[produced++] = ((u8 *)node)[2];
            node = root;
        }
        state >>= 1;
        if (state == 1) {
            wordIndex++;
            state = bitWords[wordIndex] | 0x10000;
        }
        remaining--;
    } while (remaining != 0);
    frFontRowBuf = produced;
    return produced;
}

/* Build count usable nodes plus index-zero sentinel, retaining each payload gap.
 * The allocation handle is stored four bytes before the returned ring base. */
frFontNode_t *frAllocFontTable(s32 payloadBytes, s32 count) {
    sdfMemHandle_t *buffer;
    u8 *list;
    frFontNode_t *cursor;
    frFontNode_t *next;
    s32 i;

    buffer = sdfMemAllocDirect((payloadBytes + ITF_NODE_HEADER_BYTES) * (count + 1) + ITF_ALLOCATION_HANDLE_BYTES);
    list = (u8 *)sdfMemLock(buffer);
    i = 0;
    memcpy(list, &buffer, ITF_ALLOCATION_HANDLE_BYTES);
    list += ITF_ALLOCATION_HANDLE_BYTES;
    cursor = (frFontNode_t *)list;
    for (; i < count; i++) {
        next = (frFontNode_t *)((u8 *)cursor + payloadBytes + ITF_NODE_HEADER_BYTES);
        cursor->slotIndex = i;
        cursor->next = next;
        cursor = next;
    }
    cursor->slotIndex = count;
    cursor->next = (frFontNode_t *)list;
    return (frFontNode_t *)list;
}

/* Remove the next free node, or return NULL at the index-zero sentinel. */
void *frAllocFontNode(frFontNode_t *queue) {
    frFontNode_t *head = queue->next;

    if (head->slotIndex == 0) {
        return NULL;
    }
    queue->next = head->next;
    head->next = NULL;
    return head + 1;
}

/* Prepend a detached payload's header; NULL or an already linked node fails.
 * Header pointer formation before the NULL test is retained unchanged. */
s32 frFreeFontNode(void *payload, frFontNode_t *queue) {
    frFontNode_t *node = (frFontNode_t *)payload - 1;
    if (payload == NULL) {
        return 0;
    }
    if (node->next != NULL) {
        return 0;
    }
    node->next = queue->next;
    queue->next = node;
    return 1;
}

/* Release the handle preceding the original ring base, not an acquired payload. */
u32 frFreeFontTable(frFontNode_t *ringBase) {
    sdfMemFreeDirect(((frFontRing_t *)((u8 *)ringBase - ITF_ALLOCATION_HANDLE_BYTES))->allocation);
    return 1;
}

INCLUDE_ASM(const s32, "code_001C9530", fr_1CAC50);

INCLUDE_RODATA(const s32, "code_001C9530", D_005359F8);

INCLUDE_ASM(const s32, "code_001C9530", frTestFontInit);

void test_font_Shutdown(void) {
    fclDestroyMessage();
    frDestroyFontRequest(frTestFontHandle);
}

INCLUDE_ASM(const s32, "code_001C9530", test_font_1CAD88);

s32 test_font_Process(void) {
    return 0;
}

/* Initialize a FIFO free list and empty active list.
 * Native do/while writes at least two nodes; the s8 index can wrap. */
void itfMiscLinkInitialize(itfLinkPool_t *pool, itfLink_t *nodes, s32 count, s32 stride) {
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
    pool->lastFree = node;
    pool->firstFree = nodes;
    pool->activeTail = NULL;
    pool->activeHead = NULL;
}

/* Move the free-list head to the active-list tail; exhausted pools return NULL. */
itfLink_t *itfMiscLinkTake(itfLinkPool_t *pool) {
    itfLink_t *node = pool->firstFree;
    itfLink_t *next;

    if (node == NULL) {
        return NULL;
    }
    next = node->next;
    if (pool->activeHead != NULL) {
        node->previous = pool->activeTail;
        pool->activeTail->next = node;
    } else {
        node->previous = NULL;
        pool->activeHead = node;
    }
    node->next = NULL;
    pool->activeTail = node;
    if (next != NULL) {
        next->previous = NULL;
    } else {
        pool->lastFree = NULL;
    }
    pool->firstFree = next;
    return node;
}

/* Unlink an active node and append it to the free-list tail; membership is trusted. */
void itfMiscLinkGive(itfLink_t *node, itfLinkPool_t *pool) {
    itfLink_t *previous = node->previous;
    itfLink_t *next = node->next;

    if (previous != NULL) {
        previous->next = next;
    } else {
        pool->activeHead = next;
    }
    if (next != NULL) {
        next->previous = previous;
    } else {
        pool->activeTail = previous;
    }
    node->next = NULL;
    {
        itfLink_t *lastFree = pool->lastFree;
        node->previous = lastFree;
        if (lastFree != NULL) {
            lastFree->next = node;
        }
    }
    pool->lastFree = node;
    if (pool->firstFree == NULL) {
        pool->firstFree = node;
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
void itfMiscChangeFontVertexColor(frFontRequest_t *entry, u32 color) {
    for (; entry != NULL; entry = entry->previous) {
        frFontChild_t *child;
        for (child = entry->firstChild; child != NULL; child = child->next) {
            child->parentDimensionsOrRenderWord.renderWord = color;
        }
    }
}

/* Clear each child's low byte, then OR unmasked bits into the full color word. */
void itfMiscChangeFontAlpha(frFontRequest_t *entry, u32 colorBits) {
    for (; entry != NULL; entry = entry->previous) {
        frFontChild_t *child;
        for (child = entry->firstChild; child != NULL; child = child->next) {
            child->parentDimensionsOrRenderWord.renderWord = (child->parentDimensionsOrRenderWord.renderWord & ~ITF_BYTE_MASK) | colorBits;
        }
    }
}

/* Translate parent entries only; child coordinates are left unchanged. */
void itfMiscMoveFontPosition(frFontRequest_t *entry, u32 xOffset, u32 yOffset) {
    for (; entry != NULL; entry = entry->previous) {
        entry->x += xOffset;
        entry->y += yOffset;
    }
}

/* Keep the sprite texture handle while releasing the temporary file allocation. */
sdfTexHandle_t *itfMiscLoadTexture(const char *path) {
    sdfMemHandle_t *fileAllocation;
    sdfTexHandle_t *textureHandle;
    u32 assetInfo[ITF_VECTOR_WORD_COUNT];

    fileAllocation = sdfDevLoadFileEx(path, assetInfo, 0);
    textureHandle = sdfTexSet((void *)(assetInfo[0]));
    sdfMemFreeDirect(fileAllocation);
    return textureHandle;
}

/* Emit three per-vertex RGBA/XYZ2 records; PRIM's low bits select a triangle.
 * Coordinates receive the native GS screen biases; no clipping is performed. */
void itfMiscDrawTrianglePrim(itfVertex_t *vertices, itfColor_t *colors, s32 xOffset, s32 yOffset, u32 tail, sdfDrawTag_t *command) {
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    s32 i;

    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, ITF_TRIANGLE_VERTEX_COUNT));
    sdf2DMakePrimPacketHeader(packet, ITF_TRIANGLE_PRIMITIVE_BITS, 2, 0x51, ITF_TRIANGLE_VERTEX_COUNT);
    dst = (u64 *)sdf2DGetPrimPtr((s32)packet);
    for (i = 0; i < ITF_TRIANGLE_VERTEX_COUNT; i++) {
        dst[0] = (u64)colors->components[0] | ((u64)colors->components[1] << 32);
        dst[1] = (u64)colors->components[2] | ((u64)colors->components[3] << 32);
        colors++;
        dst += 2;
        dst[1] = (u64)tail;
        dst[0] = (u64)(u32)(vertices->x + xOffset + ITF_GS_X_BIAS) | ((u64)(vertices->y + yOffset + ITF_GS_Y_BIAS) << 32);
        vertices++;
        dst += 2;
    }
    sdfDrawTagLink(command, (u32)packet);
}

/* Emit an indexed four-vertex triangle fan with a color record per vertex. */
void itfMiscDrawRectanglePrim(itfVertex_t *vertices, itfColor_t *colors, u8 *vertexIndex, u8 *colorIndex, u32 tail, sdfDrawTag_t *command) {
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    s32 i;

    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, ITF_QUAD_VERTEX_COUNT));
    sdf2DMakePrimPacketHeader(packet, ITF_QUAD_FAN_PRIMITIVE_BITS, 2, 0x51, ITF_QUAD_VERTEX_COUNT);
    dst = (u64 *)sdf2DGetPrimPtr((s32)packet);
    for (i = 0; i < ITF_QUAD_VERTEX_COUNT; i++) {
        itfVertex_t *vertex = &vertices[*vertexIndex++];
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
void itfMiscDrawTexPoly(itfVertex_t *vertices, f32 *uvs, itfColor_t *colors, u32 tail, s32 flag, sdfDrawTag_t *command) {
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    s32 i;

    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(3, ITF_QUAD_VERTEX_COUNT));
    sdf2DMakePrimPacketHeader(packet, (flag << 9) | ITF_TEXTURED_FAN_PRIMITIVE_BITS, 3, 0x512, ITF_QUAD_VERTEX_COUNT);
    dst = (u64 *)sdf2DGetPrimPtr((s32)packet);
    for (i = 0; i < ITF_QUAD_VERTEX_COUNT; i++) {
        f32 *uvDst = (f32 *)dst;

        uvDst[0] = uvs[0];
        uvDst[1] = uvs[1];
        uvDst[2] = uvs[2];
        uvs += 4;
        dst += 2;
        dst[0] = (u64)colors->components[0] | ((u64)colors->components[1] << 32);
        dst[1] = (u64)colors->components[2] | ((u64)colors->components[3] << 32);
        colors++;
        dst += 2;
        dst[1] = (u64)tail;
        dst[0] = (u64)(u32)(vertices->x + ITF_GS_X_BIAS) | ((u64)(vertices->y + ITF_GS_Y_BIAS) << 32);
        vertices++;
        dst += 2;
    }
    sdfDrawTagLink(command, (u32)packet);
}

/* Bind the texture and emit one two-corner sprite using two packed UV pairs.
 * Unwritten padding words in the native packet remain untouched. */
void itfMiscDrawSpritePrim(void *vertexData, void *uvData, void *colorData, s32 tail, sdfTexHandle_t *texture, s32 flag, sdfDrawTag_t *command) {
    itfVertex_t *vertices = vertexData;
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
void itfMiscDrawSpritePrim2(itfVertex_t *vertices, itfColor_t *uv, itfColor_t *colors, u32 tail, s32 flag, sdfDrawTag_t *command) {
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    s32 x0, y0, x1, y1;

    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(5, 1));
    sdf2DMakePrimPacketHeader(packet, (flag << 9) | ITF_COLORED_SPRITE_PRIMITIVE_BITS, 5, 0x53531, 1);
    x0 = vertices[0].x + ITF_GS_X_BIAS;
    y0 = vertices[0].y + ITF_GS_Y_BIAS;
    x1 = vertices[1].x + ITF_GS_X_BIAS;
    y1 = vertices[1].y + ITF_GS_Y_BIAS;
    dst = (u64 *)sdf2DGetPrimPtr((s32)packet);
    dst[0] = (u64)colors->components[0] | ((u64)colors->components[1] << 32);
    dst[1] = (u64)colors->components[2] | ((u64)colors->components[3] << 32);
    dst[2] = (u64)uv->components[0] | ((u64)uv->components[1] << 32);
    dst[4] = (u64)(u32)x0 | ((u64)y0 << 32);
    dst[5] = (u64)tail;
    dst[6] = (u64)uv->components[2] | ((u64)uv->components[3] << 32);
    dst[8] = (u64)(u32)x1 | ((u64)y1 << 32);
    dst[9] = (u64)tail;
    sdfDrawTagLink(command, (u32)packet);
}

/* Emit an untextured two-corner GS sprite. */
void itfMiscDrawSpritePlanePrim(itfVertex_t *vertices, itfColor_t *colors, u32 tail, s32 flag, sdfDrawTag_t *command) {
    sdfGrPacketHeader_t *packet;
    u64 *dst;

    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(3, 1));
    sdf2DMakePrimPacketHeader(packet, (flag << 9) | ITF_SPRITE_PRIMITIVE_BITS, 3, 0x551, 1);
    dst = (u64 *)sdf2DGetPrimPtr((s32)packet);
    dst[0] = (u64)colors->components[0] | ((u64)colors->components[1] << 32);
    dst[1] = (u64)colors->components[2] | ((u64)colors->components[3] << 32);
    dst += 2;
    dst[1] = (u64)tail;
    dst[0] = (u64)(u32)(vertices[0].x + ITF_GS_X_BIAS) | ((u64)(vertices[0].y + ITF_GS_Y_BIAS) << 32);
    dst += 2;
    dst[1] = (u64)tail;
    dst[0] = (u64)(u32)(vertices[1].x + ITF_GS_X_BIAS) | ((u64)(vertices[1].y + ITF_GS_Y_BIAS) << 32);
    sdfDrawTagLink(command, (u32)packet);
}

/* Emit an indexed triangle strip, packing two vertices per GIF loop.
 * Header loop count truncates odd vertex counts; the native writer does not. */
void itfMiscDrawTriangleStripPrim(itfVertex_t *vertices, itfColor_t *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, sdfDrawTag_t *command) {
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    s32 pairCount = count >> 1;
    s32 i;

    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(4, pairCount));
    sdf2DMakePrimPacketHeader(packet, ITF_TRIANGLE_STRIP_PRIMITIVE_BITS, 4, 0x5151, pairCount);
    dst = (u64 *)sdf2DGetPrimPtr((s32)packet);
    for (i = 0; i < count; i++) {
        itfVertex_t *vertex = &vertices[*vertexIndex++];
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
void itfMiscDrawLineStripPrim(itfVertex_t *vertices, itfColor_t *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, sdfDrawTag_t *command) {
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    s32 i;

    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, count));
    sdf2DMakePrimPacketHeader(packet, ITF_LINE_STRIP_PRIMITIVE_BITS, 2, 0x51, count);
    dst = (u64 *)sdf2DGetPrimPtr((s32)packet);
    for (i = 0; i < count; i++) {
        itfVertex_t *vertex = &vertices[*vertexIndex++];
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
void itfMiscDrawLinePrim(itfVertex_t *vertices, itfColor_t *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, sdfDrawTag_t *command) {
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    s32 i;

    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, count));
    sdf2DMakePrimPacketHeader(packet, ITF_LINES_PRIMITIVE_BITS, 2, 0x51, count);
    dst = (u64 *)sdf2DGetPrimPtr((s32)packet);
    for (i = 0; i < count; i++) {
        itfVertex_t *vertex = &vertices[*vertexIndex++];
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
void itfMiscDrawTestPrim(sdfDrawTag_t *command, u64 value, s32 flag) {
    s32 packet = sdfGrAllocPacketBuff(ITF_STATE_PACKET_BYTES);
    itfGsState_t *dst = (itfGsState_t *)sdf2DMakeStdPacket(packet, ITF_STATE_PACKET_BYTES);

    dst->value = value;
    dst->registerCode = flag ? ITF_TEST_SECOND_CONTEXT : ITF_TEST_FIRST_CONTEXT;
    sdfDrawTagLink(command, packet);
}

/* Write an unchecked table entry to ALPHA for the selected GS context. */
void itfMiscDrawAlphaPrim(sdfDrawTag_t *command, s32 index, s32 flag) {
    s32 packet = sdfGrAllocPacketBuff(ITF_STATE_PACKET_BYTES);
    itfGsState_t *dst = (itfGsState_t *)sdf2DMakeStdPacket(packet, ITF_STATE_PACKET_BYTES);

    dst->value = D_003263C0[index];
    dst->registerCode = flag ? ITF_ALPHA_SECOND_CONTEXT : ITF_ALPHA_FIRST_CONTEXT;
    sdfDrawTagLink(command, packet);
}

/* Write CLAMP for the selected GS context. */
void itfMiscDrawClampPrim(sdfDrawTag_t *command, s32 flag, s32 wms, s32 wmt, s32 minu, s32 maxu, s32 minv, s32 maxv) {
    s32 packet = sdfGrAllocPacketBuff(ITF_STATE_PACKET_BYTES);
    itfGsState_t *dst = (itfGsState_t *)sdf2DMakeStdPacket(packet, ITF_STATE_PACKET_BYTES);

    dst->value = ((u64)maxv << 34) | ((u64)minv << 24) | (maxu << 14) | (minu << 4) | (wmt << 2) | wms;
    dst->registerCode = flag ? 9 : 8;
    sdfDrawTagLink(command, packet);
}

/* Draw filled and remaining quad regions. The optional highlight is produced
 * as signed fixed-point vectors and passed as their four-word RGBA image. */
void itfMiscDrawGauge(s32 value, s32 limit, s32 highlight,
                   itfVertex_t *bounds, u32 tail, itfColor_t *filledColor,
                   itfColor_t *remainingColor, sdfDrawTag_t *command) {
    itfVertex_t vertices[6];
    itfColorVector_t highlightColors[4];
    s32 left = bounds[0].x;
    s32 right = bounds[1].x;
    s32 width = (right - left) >> 4;
    s32 filledWidth;

    if (limit > 0) {
        filledWidth = (((value << 16) / limit) * width) >> 12;
    } else {
        filledWidth = 0;
    }
    vertices[0].x = left;
    vertices[0].y = bounds[0].y;
    vertices[1].x = filledWidth + left;
    vertices[1].y = bounds[0].y;
    vertices[2].x = right;
    vertices[2].y = bounds[0].y;
    vertices[3].x = left;
    vertices[3].y = bounds[1].y;
    vertices[4].x = filledWidth + left;
    vertices[4].y = bounds[1].y;
    vertices[5].x = right;
    vertices[5].y = bounds[1].y;
    itfMiscDrawRectanglePrim(vertices, filledColor, D_0054F470, D_0054F478, tail, command);
    itfMiscDrawRectanglePrim(vertices, remainingColor, D_0054F470 + 4, D_0054F478, tail, command);
    if (highlight != 0 && filledWidth >= 3) {
        itfMiscSetRatioRGB(highlightColors, filledColor->components[0],
                        filledColor->components[1], filledColor->components[2],
                        filledColor->components[3], D_003263E8, 4);
        itfMiscDrawLineStripPrim(vertices, (itfColor_t *)highlightColors,
                        D_003263D8, D_003263E0, 3, tail, command);
        itfMiscDrawLineStripPrim(vertices, (itfColor_t *)highlightColors,
                        D_003263D8 + 3, D_003263E0 + 3, 3, tail, command);
    }
}

/* Retail "DbgDevDump" task callback: perform no dump work and return zero. */
s32 dds3DebugDevDumpProcess(dds3ProcessID_t *task) {
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
INCLUDE_SDATA(const s32, "code_001C9530", D_0054F440);

INCLUDE_SDATA(const s32, "code_001C9530", D_0054F448);

INCLUDE_SDATA(const s32, "code_001C9530", D_0054F44C);

INCLUDE_SDATA(const s32, "code_001C9530", frHelpPosX);

INCLUDE_SDATA(const s32, "code_001C9530", frHelpPosY);

INCLUDE_SDATA(const s32, "code_001C9530", D_0054F458);

INCLUDE_SDATA(const s32, "code_001C9530", frTestFontHandle);

INCLUDE_SDATA(const s32, "code_001C9530", D_0054F460);

INCLUDE_SDATA(const s32, "code_001C9530", D_0054F464);

INCLUDE_SDATA(const s32, "code_001C9530", D_0054F468);

INCLUDE_SDATA(const s32, "code_001C9530", D_0054F46C);

INCLUDE_SDATA(const s32, "code_001C9530", D_0054F470);

INCLUDE_SDATA(const s32, "code_001C9530", D_0054F478);

