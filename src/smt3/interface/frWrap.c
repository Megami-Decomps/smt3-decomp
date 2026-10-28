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
    struct frFontRequest_t *prev;
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

extern u32 frFontRowBuf;
extern char D_0054F448[]; /* Shared empty font-chain seed. */
extern frFontRequest_t *frSetFontString(const char *, s8, s8, s8, frFontRequest_t *, s32);
extern void frSetFontSize(frFontRequest_t *, s32, s32);
extern frFontRequest_t *frCombineFontRequestEx(frFontRequest_t *, frFontRequest_t *, s32);

extern u32 strlen(const char *text);

extern void *memcpy(void *a, const void *b, u32 c);

extern sdfMemHandle_t *sdfMemAllocDirect(u32 aSize);

extern void *sdfMemLock(sdfMemHandle_t *aHandle);

extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern void frSetWhiteSpaceW(s32 StyleNo, s32 Width);

extern void frEnableMode(s32 Flag);

extern u8 frDisableMode(u8 Flag);

extern frFontRequest_t *frLinkFontString(const char *, s8, s8, s8, frFontRequest_t *);

extern void frSetFontSpace(frFontRequest_t *, s32);

extern void frSetDrawPosition(frFontRequest_t *, u32, u32);

extern void frSetPriority(frFontRequest_t *, s32);

extern void frSetFontColor(frFontRequest_t *, u32);

extern void frSetFontColorNo(frFontRequest_t *, u8);

extern frFontRequest_t *frCombineFontRequest(frFontRequest_t *, frFontRequest_t *);

extern void frLoadFontData(s32 StyleNo, const char *pFileName);

extern void frDelFontStyle(s32 StyleNo);

typedef struct { u8 unk00[0x10]; u32 headerWord; } frPackedHeader_t;

#define ITF_NODE_HEADER_BYTES 8

#define ITF_ALLOCATION_HANDLE_BYTES 4

/* Staff callers supply a trailing request; this wrapper creates a fresh chain. */
frFontRequest_t *frwGetStaffFontFRQ(u32 x, u32 y, s32 depth, s32 alt, s32 measureFlag, s32 entryFlag, u32 colors, void *source, void *unusedRequest) {
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

frFontRequest_t *frwGetSmallFont1FRQ(s32 x, s32 y, s32 priority, u32 color,
                                      const char *text, frFontRequest_t *previous)
{
    frFontRequest_t *request = frLinkFontString(text, 0, 0, 0, NULL);
    frSetFontSize(request, 16, 18);
    frSetDrawPosition(request, x, y);
    frSetPriority(request, priority << 4);
    frSetFontColor(request, color);
    return frCombineFontRequestEx(previous, request, 0);
}

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

frFontRequest_t *frwGetSmallFontFRQ(s32 X, s32 Y, s32 Pri, s32 Color, const char *pStr, frFontRequest_t *pFRQ)
{
    frFontRequest_t *handle = frwGetFontEx(X, Y, Pri, Color, pStr, 1, 0, pFRQ);

    frSetFontSpace(handle, 3);
    return handle;
}

frFontRequest_t *frwGetSmallFontFRQ2(s32 x, s32 y, s32 priority, u32 color,
                                     const u8 *text, frFontRequest_t *previous)
{
    u8 converted[0x400];
    s32 length = strlen((const char *)text);
    s32 position = 0;
    frFontRequest_t *request;

    converted[length] = 0;
    while (position < length) {
        /* The character's byte and packed forms share one u16. Qualified cc1
         * RTL uids 60/94 share pseudo 96; global allocation gives it $4
         * (16 references, live length 14). Splitting the forms gives separate
         * pseudos, removes the narrowing and emits 364 rather than 372 bytes. */
        u16 code = text[position];

        if (code < 0x80) {
            converted[position] = code;
        } else {
            code = (code << 8) | text[position + 1];
            if (code != 0xFFFF) {
                converted[position] = (u8)(code >> 8);
                converted[position + 1] = (u8)code;
            } else {
                converted[position] = 0x80;
                converted[position + 1] = 0x80;
            }
            position++;
        }
        position++;
    }

    frEnableMode(1);
    request = frLinkFontString((char *)converted, 1, 0, 0, NULL);
    frEnableMode(2);
    frDisableMode(1);
    frSetDrawPosition(request, x, y);
    frSetPriority(request, priority << 4);
    frSetFontColor(request, color);
    frSetFontSpace(request, 3);
    return frCombineFontRequestEx(previous, request, 0);
}

frFontRequest_t *frwGetSmallFontNoPropFRQ(s32 x, s32 y, s32 depth, u32 colors,
                                       const char *text, frFontRequest_t *previous)
{
    return frwGetFontEx(x, y, depth, colors, text, 3, 1, previous);
}

frFontRequest_t *frwGetSmallNumFontFRQ(s32 x, s32 y, s32 priority, u32 color,
                                        const char *text, frFontRequest_t *previous)
{
    frFontRequest_t *request = frLinkFontString(D_0054F448, 0, 0, 0, previous);
    frDisableMode(2);
    request = frSetFontString(text, 2, 0, 0, request, 0);
    frEnableMode(2);
    frSetDrawPosition(request, x, y);
    frSetPriority(request, priority << 4);
    frSetFontColor(request, color);
    return request;
}

frFontRequest_t *frwGetNumFontFRQ(s32 x, s32 y, s32 priority, u32 color,
                                        const char *text, frFontRequest_t *previous)
{
    frFontRequest_t *request = frLinkFontString(D_0054F448, 0, 0, 0, previous);
    frDisableMode(2);
    request = frSetFontString(text, 3, 0, 0, request, 0);
    frEnableMode(2);
    frSetDrawPosition(request, x, y);
    frSetPriority(request, priority << 4);
    frSetFontColor(request, color);
    return request;
}

frFontRequest_t *frwGetSmallCaptionFontFRQ(s32 x, s32 y, s32 priority, u32 color,
                                           const char *text, frFontRequest_t *previous)
{
    frFontRequest_t *request = frSetFontString(text, 0, 0, 0, NULL, 0);
    frSetFontSize(request, 12, 16);
    frSetFontSpace(request, 3);
    frSetDrawPosition(request, x, y);
    frSetPriority(request, priority << 4);
    frSetFontColor(request, color);
    return frCombineFontRequestEx(previous, request, 0);
}

typedef struct frTextBank_t frTextBank_t;
extern frTextBank_t itfMesEmbedded_3E8CA8, itfMesEmbedded_3F39E0, itfMesEmbedded_3F7350;
extern u8 *MessageNumSearch(s32 index, frTextBank_t *bank, s32 size);
extern frFontRequest_t *MessageRequestNS(s32 x, s32 y, s32 depth, s32 style,
    s32 color, s32 type, s32 speed, u8 *text, frFontRequest_t *unusedPrevious);

frFontRequest_t *frwGetHelpFRQ(s32 x, s32 y, s32 priority, s8 bank, u16 index, s8 mode)
{
    u8 *text = NULL;
    frFontRequest_t *request;

    switch (bank) {
    case 0:
        text = MessageNumSearch(index, &itfMesEmbedded_3E8CA8, 0);
        break;
    case 1:
        text = MessageNumSearch(index, &itfMesEmbedded_3F39E0, 0);
        break;
    case 2:
        text = MessageNumSearch(index, &itfMesEmbedded_3F7350, 0);
        break;
    }
    switch (mode) {
    case 1:
        frDisableMode(4);
    case 3:
        frEnableMode(8);
        break;
    case 2:
        frDisableMode(4);
        break;
    }
    request = MessageRequestNS(x, y, priority, 1, 0, 0, 0x80, text, NULL);
    switch (mode) {
    case 1:
        frEnableMode(4);
    case 3:
        frDisableMode(8);
        break;
    case 2:
        frEnableMode(4);
        break;
    }
    return request;
}

extern s32 D_0054F44C, D_0054F458, frHelpPosX, frHelpPosY;
extern s8 D_0056BD60[];
extern s32 frCalcStringHash(s8 *text);
extern void frCopyTagArgs(s8 *text, s8 *destination, s32 line);

frFontRequest_t *frGetHelpPos(s32 x, s32 y, s32 priority, s32 bank, s32 index)
{
    u8 *text;

    switch (bank) {
    case 0:
        text = MessageNumSearch(index, &itfMesEmbedded_3E8CA8, 0);
        break;
    case 1:
        text = MessageNumSearch(index, &itfMesEmbedded_3F39E0, 0);
        break;
    default:
        text = MessageNumSearch(index, &itfMesEmbedded_3F7350, 0);
        break;
    }
    if (index != D_0054F44C) {
        D_0054F44C = index;
        frHelpPosX = 0;
        frHelpPosY = 0;
        D_0054F458 = frCalcStringHash((s8 *)text);
    }
    frHelpPosX++;
    if (frHelpPosX >= 31) {
        frHelpPosX = 0;
        frHelpPosY++;
        if (frHelpPosY >= D_0054F458) {
            frHelpPosY = 0;
        }
    }
    frCopyTagArgs((s8 *)text, D_0056BD60, frHelpPosY);
    return MessageRequestNS(x, y, priority, 1, 0, 0, 0x80, (u8 *)D_0056BD60, NULL);
}

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

extern s32 D_0054F460, D_0054F464;
extern s8 D_0054F468;
extern s8 dds3PadBitTable[];
extern frFontRequest_t *frTestFontHandle;
extern char D_005359F8[];
extern void frSetDrawSpeed(frFontRequest_t *request, s32 speed);

void fr_1CAC50(void)
{
    frFontRequest_t *request;

    request = frwGetSmallFontFRQ((D_0054F460 + 64) << 4,
                               (D_0054F464 + 32) << 3,
                               0xFFFFFF, 0x80808000, D_005359F8,
                               frTestFontHandle);
    frTestFontHandle = request;
    frSetDrawSpeed(request, 6);
    if (dds3PadBitTable[6] != 0) {
        if (D_0054F468 >= 0) {
            D_0054F468++;
        }
    }
    if (dds3PadBitTable[7] != 0) {
        u8 *alpha = (u8 *)&D_0054F468;
        if (*alpha != 0) {
            (*alpha)--;
        }
    }
    frSetFontColor(frTestFontHandle, 0x80808000 | (u8)D_0054F468);
}

INCLUDE_RODATA(const s32, "interface/frWrap", D_005359F8);

