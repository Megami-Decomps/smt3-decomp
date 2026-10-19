#include "font.h"
#include "kernel.h"

typedef struct frFontHeader_t frFontHeader_t;

typedef struct frFontResource_t frFontResource_t;

typedef struct frFontEntry_t frFontEntry_t;

typedef struct frFontNodeHeader_t frFontNodeHeader_t;

typedef struct frFontAtlas_t frFontAtlas_t;

typedef struct frFontSystem_t frFontSystem_t;

typedef union frFontDmaTag_t frFontDmaTag_t;

typedef struct frFontGsWrite_t frFontGsWrite_t;

typedef struct frFontGsPacket_t frFontGsPacket_t;

typedef struct frTextIndex_t frTextIndex_t;

typedef struct frTextBank_t frTextBank_t;

typedef struct frMessageContext_t frMessageContext_t;

typedef struct frFontImage_t frFontImage_t;

/* Serialized resource header; DDS2 moved the lookup offset by eight bytes. */
struct frFontHeader_t {
    u32 tableOffset;
    u8 pad04[6];
    u8 tableCount;
    u8 pad0B[3];
    u16 widthCount;
    u16 cellWidth;
    u16 cellHeight;
    u8 pad14[2];
    u8 hasExtra;
    u8 pad17;
    u32 lookupOffset;
};

/* Relative offsets select three packed-font resource regions. */
struct frFontResource_t {
    s32 firstOffset; /* 0x00 */
    s32 secondDelta; /* 0x04 */
    u8 opaque08[0x10];
    s32 thirdDelta; /* 0x18 */
};

struct frFontEntry_t {
    sdfMemHandle_t *allocation;
    frFontHeader_t *resourceHeader;
    s32 metricByteCount;
    s32 valueByteCount;
    s8 *flagBytes;
    void *unk14;
    frFontRecord_t **slots;
    struct frFontResource_t *resource;
    u32 unk20;
};

/* Free-list header stored immediately before each interface-pool payload. */
struct frFontNodeHeader_t {
    u32 slotIndex; /* Zero identifies the ring sentinel. */
    struct frFontNodeHeader_t *next;
};

/* frFontWork + 0x160; the small texture label aliases this owned subobject. */
struct frFontAtlas_t {
    s32 width;
    u32 widthExponent;
    s32 height;
    u32 heightExponent;
    u32 bufferBase;
    u32 bufferWidth;
};

struct frFontSystem_t {
    frFontEntry_t entries[9];
    s32 cachedItemCount;
    s32 itemCount;
    s32 glyphCount;
    struct frFontNodeHeader_t *itemPool;
    struct frFontNodeHeader_t *glyphPool;
    sdfGsMemSlice_t *textureHead0;
    sdfGsMemSlice_t *textureHead1;
    frFontAtlas_t atlas;
    u32 imageBuffers[6];
    u8 pad190[4];
    struct frFontHandle_t *glyphSlots[2];
};

/* The SDK DMA tag is accessed as a quadword and as its four command words. */
union frFontDmaTag_t {
    u128 value;
    u32 words[4];
};

struct frFontGsWrite_t {
    u64 value;
    u64 reg;
};

struct frFontGsPacket_t {
    frFontDmaTag_t dma;
    u64 gifTag;
    u64 gifRegisters;
    frFontGsWrite_t writes[6];
};

/* Serialized font-bank entries use byte offsets relative to their bank. */
struct frTextIndex_t {
    u32 unk00;
    u32 offset;
};

struct frTextBank_t {
    u8 pad00[0x18];
    s32 count;
    u8 pad1C[4];
    frTextIndex_t entries[1];
};

/* Encoded text cursor and pending glyph updates; distinct from a glyph (0x20). */
struct frMessageContext_t {
    s32 x;
    s32 y;
    s32 z;
    s8 fontIndex;
    s8 firstOption;
    s8 secondOption;
    u8 contextEncodedByte;
    u8 *encodedText;
    frFontHandle_t *glyphChain;
    s32 encodedTextOffset;
    s8 pendingCreate;
    s8 pendingPosition;
    u8 pad1E[2];
};

/* Output pointers address the tree, entry bit ranges, and packed bit words. */
struct frFontImage_t {
    void *treeNodes; /* 0x00 */
    void *entryBitOffsets; /* 0x04 */
    void *packedBitstream; /* 0x08 */
};

/* Pixel upload inputs shared by texture and font-atlas producers. */
typedef struct sdfImageRequest_t {
    void *pixels;
    sdfMemHandle_t *allocation;
    u8 allocationMode;
    u8 format;
    u16 bufferWidth;
    u32 destination;
    u16 x;
    u16 y;
    u16 width;
    u16 height;
} sdfImageRequest_t;

/* sdfImageSetRelease_t.freeMode: which heap owns the uploaded pixels. */
enum SdfImageSetMem
{
    SDF_IMAGESET_MEM_IGNORE = 0,
    SDF_IMAGESET_MEM_HANDLE = 1,
    SDF_IMAGESET_MEM_CHIP = 2
};

void frSetWhiteSpaceW(s32 slotId, s32 entryFlag);

extern frFontSystem_t gFT;

#define FR_FONT_BYTE_MASK 0xFF

#define FR_FONT_ENTRY_ENABLED 1

u32 frSwapColor(u32 word);

#define FR_FONT_BYTE_SHIFT 8

#define FR_FONT_MIDDLE_BYTE_MASK 0x0000FF00

#define FR_FONT_OUTER_BYTE_SHIFT 24

s32 frConvertVal2N(u32 bitValue);

void frCreateDrawEnvPacket(frFontGsPacket_t *packet);

void frCreateUserFontPacket(s32 x, s32 y, s32 depth, frTextHandle_t *glyph,
                  s32 drawFlags);

#define FR_FONT_GLYPH_HEIGHT_SHIFT 1

void frCopyFontClutToVram(s32 buffer, s32 image);

extern void sceGsSetDefLoadImage(void *, s16, s16, s32, s32, s32, s16, s16);

extern s32 WaitSema(s32);

extern volatile s32 dds3GsSema; /* semaphore handle shared with the IOP/interrupt side; declared volatile */

extern void FlushCache(s32);

extern void sceGsExecLoadImage(void *, s32);

extern s32 sceGsSyncPath(s32, s32);

extern s32 SignalSema(s32);

void frInitFontTable(s32 rowBudget, s32 poolCount);

extern sdfGsMemSlice_t * sdfGsMemAllocFromTail(s32 size, s32 allocationMode);

s32 fmInitFontMemControl(s32 unused, s32 columns, s32 rows, s32 cellWidth, s32 cellHeight,
                               s32 recordBytes);

extern frFontNodeHeader_t *frAllocFontTable(s32 payloadBytes, s32 count);

void frLoadStyleAll(void);

void frSetFontClut(void);

#define FR_FONT_INDEXED_TEXT_SLOT_COUNT 32

/* Address words substituted by the indexed encoded-text control codes. */
extern u32 gVarStr[32];

void frClearTexBuf(void);

extern s32 gRegistMax;

void frDestroyFontTable(dds3ProcessID_t *task);

void frDelStyleAll(void);

extern frFontHandle_t *frDestroyFontRequestNow(frFontHandle_t *glyph);

extern u32 frFreeFontTable(frFontNodeHeader_t *pool);

s32 fmDestroyFontMemControl(void);

extern void sdfGsMemFree(sdfGsMemSlice_t *textureBlock);

void frAddFontStyle(
    u8 slotIndex, u8 *resourceBytes, sdfMemHandle_t *allocation
);

extern u32 sdfMemLock(sdfMemHandle_t *allocation);

#define FR_FONT_TABLE_ENTRY_SHIFT 6

#define FR_FONT_WORD_BYTES 4

void frDelFontStyle(s32 slotId);

#define FR_FONT_FIRST_RELEASABLE_SLOT 2

extern u32 frFontLoaded[];

extern void sdfMemFreeDirect(sdfMemHandle_t *allocation);

extern void frLoadFontData(s32 id, const char *path);

#define FR_FONT_ENTRY_CAPACITY 9

#define FR_FONT_IMAGE_WORD_COUNT 16

extern u8 frFontClutTable[];

#define FR_FONT_IMAGE_WORD_BYTES 4

#define FR_FONT_IMAGE_LAST_WORD 15

#define FR_FONT_IMAGE_BUFFER_COUNT 6

void frFlushGsFontTable(frTextHandle_t *glyph);

s32 fmRetFontMem(fmFontNode_t *node);

frFontHandle_t *frDestroyFontRequest(frFontHandle_t *glyph);

s32 frEraseEffect(frFontHandle_t *glyph);

s32 frDestroyFontRequestAfter(frFontHandle_t *glyph);

extern u32 dds3GetFrameID(void);

extern frFontHandle_t *frCombineFontRequestEx(frFontHandle_t *, frFontHandle_t *, s32);

s32 frGetGsFontNum(void);

s32 frGetTextHandleNum(void);

u32 frGetFontHandleNum(void);

extern u32 frFontHandleNum[];

s32 frGetYShiftLev(s8 fontIndex, s32 shiftIndex);

frFontHandle_t *frSetUserImage(frFontRecord_t *source, frFontHandle_t *destination);

extern void *frAllocFontNode(frFontNodeHeader_t *pool);

extern void frInitFontHandle(frFontHandle_t *glyph);

void frInitTextHandle(frTextHandle_t *glyph, s32 glyphId, s32 fontIndex, s32 firstOption, s32 packedFlags, s32 secondOption);

extern s8 gFontBaseWidth[];

u32 frGetDefaultFontWidth(u8 fontIndex);

u32 frGetDefaultFontHeight(u8 fontIndex);

frFontHandle_t *frLinkUserImage(frFontRecord_t *source, frFontHandle_t *destination);

frFontHandle_t *frCombineFontRequest(frFontHandle_t *previous, frFontHandle_t *next);

frFontRecord_t *frTranslateUserData(s32 width, s32 height, void *pixels, s8 ownsPixels);


fmFontNode_t *fmRentFontMem(void);

#define FR_FONT_ATLAS_POSITION_SHIFT 4

#define FR_FONT_PIXEL_FORMAT 0x14

#define FR_FONT_ATLAS_DEST_SHIFT 6

extern void sdfImageSetReq(sdfImageRequest_t *request);

u8 *frDecodeFontImage(u8 fontIndex, s32 entryIndex);

extern void *sdfChipAllocDirect(s32 size);

#define FR_FONT_RESOURCE_CLONE_BYTES 0x120

extern void frGetFontImagePtrs(frFontResource_t *resource,
                              frFontImage_t *segments);

extern s32 frDecodeFontRow(u8 *decodedBytes, s32 entryIndex,
                                void *resourceBase,
                                frFontImage_t *segments);

frFontRecord_t *frTranslateFont2GS(frTextHandle_t *glyph, s32 itemIndex);

u16 frGetFontWidth(s32 slotIndex);

u16 frGetFontHeight(s32 slotIndex);

extern u8 gModeFlag;

void frSetDrawSpeed(frFontHandle_t *glyph, s32 inputValue);

#define FR_FONT_CONTEXT_BYTE_THRESHOLD 0x81

#define FR_FONT_CONTEXT_BYTE_CAP (-0x80)

void frSetFontSpace(frFontHandle_t *glyph, s32 spacing);

u32 frGetStrWidth(frFontHandle_t *parentGlyph);

void frSetFontSize(frFontHandle_t *glyph, s32 cellAdvance, s32 cellHeight);

void frSetDrawPosition(frFontHandle_t *glyph, u32 x, u32 y);

void frSetPriority(frFontHandle_t *glyph, u32 unshiftedValue);

#define FR_FONT_CONTEXT_VALUE_SHIFT 4

void frSetFontColorNo(frFontHandle_t *glyph, u8 firstOption);

void frSetFontColor(frFontHandle_t *parentNode, u32 colorWord);

void frEnableMode(s32 mask);

u8 frDisableMode(u8 mask);

void frSetFontDrawOT(u32 renderFlags);

extern u32 gFontDrawOT;

#define FR_FONT_FADE_OPTION 2

#define FR_FONT_FADE_VALUE_STEP 8

#define FR_FONT_FADE_Y_STEP 0x10

s32 frDrawFont(frFontHandle_t *glyph);

extern s32 frDrawFontEx(frFontHandle_t *glyph, s8 mode);

s32 frFlipPacketBuf(dds3ProcessID_t *task);

void frSetExtDic(u8 *fontData);

#define FR_FONT_TEMPORARY_SLOT 8

void frDestroyExtDic(void);

s32 frStrLen(s8 *text);

u32 frGetStrWidth2(frFontHandle_t *glyphChain);

#define FR_FONT_DEFAULT_CELL_SLOT_COUNT 2

extern s32 frDefaultFontHandle;

void frSetMessageDefStepY(void);

extern u32 gMessageStepY;

#define FR_FONT_DEFAULT_CURSOR_SPACING 0x14

void frSetMessageStepY(u32 lineStep);


u8 *MessageNumSearch(s32 textId, frTextBank_t *bank, s32 mode);

extern void *memcpy(void *destination, const void *source, u32 size);

void frMessageReturnCode(frMessageContext_t *ctx);

#define FR_FONT_CONTEXT_CURSOR_SCALE 8

frFontHandle_t *MessageRequest(s32 x, s32 y, u8 *encodedText, frFontHandle_t *sub);

frFontHandle_t *MessageRequestEx2(s32 x, s32 y, s32 depth, s32 channel0, s32 channel1, s32 channel2, s32 channel3, u8 *encodedText, frFontHandle_t *sub);

frFontHandle_t *MessageRequestEx(s32 x, s32 y, s32 channel0, s32 channel1, s32 channel2, s32 channel3, u8 *encodedText, frFontHandle_t *sub);

frFontHandle_t *frAnalyzeMessage(frMessageContext_t *stream);

u32 frGetMessageResult(u32 mask);

extern u32 gMessageRequestResult;

void frSetVariableRequest(s32 slotIndex, s32 encodedTextAddress);

extern s8 gMessageSpaceX;

extern u16 frMessageCode;

extern s32 frFindTagCode(u16 code);

extern s32 frFindTagCode2(u16 code);

extern frFontHandle_t *frGetNameFRQ(s8 nameIndex, frFontHandle_t *chain, s8 firstOption);

extern frFontHandle_t *frGetName1NFRQ(s8 nameIndex, frFontHandle_t *chain, s8 firstOption);

extern frFontHandle_t *frGetName2NFRQ(s8 nameIndex, frFontHandle_t *chain, s8 firstOption);

extern frFontHandle_t *frGetCNameFRQ(s8 nameIndex, s8 fontIndex, frFontHandle_t *chain, s8 firstOption);

extern s32 frAnalyzeTag(s32 code, frMessageContext_t *ctx);

extern u8 frSetFontString_style;

extern void frGetMessageCode(u32 code);

frTextHandle_t *frSetFont(u16 glyphCode, u8 fontIndex, u8 firstOption, u8 secondOption, s32 shiftFlag);

void frCreateVariableRequest(u8 slotIndex, frMessageContext_t *ctx);

extern sdfMemHandle_t *sdfMemAllocDirect(s32 size);

extern void *memset(void *destination, s32 value, u32 size);

extern u32 strlen(const char *text);

extern void frRestoreNameImage(s8 nameIndex);

extern s32 frFreeFontNode(void *payload, frFontNodeHeader_t *pool);

extern void frGetXShiftLev(frTextHandle_t *glyph, s32 shiftIndex, s32 isFirst);

u32 frPreCalcStrWidth(s8 *text, s32 fontIndex, s32 sharedFlags);

extern sdfMemHandle_t *sdfDevLoadFileEx(const char *path, u32 *destination, s32 size);

s32 frCalcStringHash(s8 *text);

extern u8 D_0054F430[];

extern frFontHandle_t *frSetFontString(u8 *text, s8 fontIndex, s8 firstOption, s8 secondOption, frFontHandle_t *chain, s32 flag);

frFontHandle_t *frLinkFontString(u8 *text, s8 fontIndex, s8 firstOption, s8 secondOption, frFontHandle_t *destination);

void frCreateNewHandle(frMessageContext_t *ctx);

void frPushTagChar(frMessageContext_t *ctx, u16 code);

s32 frGetTagArgCount(s32 tagCode);

frFontHandle_t *MessageRequestNS(s32 x, s32 y, s32 depth, s32 channel0, s32 channel1, s32 channel2, s32 channel3, u8 *encodedText);

/* Select by the low byte of slotId; enable the entry and store value + 1 as a byte. */
void frSetWhiteSpaceW(s32 slotId, s32 entryFlag) {
    frFontEntry_t *entry = &gFT.entries[slotId & FR_FONT_BYTE_MASK];

    entry->flagBytes[0] = FR_FONT_ENTRY_ENABLED;
    entry->flagBytes[1] = entryFlag + 1;
}

/* Reverse the four byte positions of a 32-bit word. */
u32 frSwapColor(u32 word) {
    u32 shiftedWord = word >> FR_FONT_BYTE_SHIFT;
    u32 lowerMiddleByte = word & FR_FONT_MIDDLE_BYTE_MASK;
    u32 outerBytes = word << FR_FONT_OUTER_BYTE_SHIFT;

    word >>= FR_FONT_OUTER_BYTE_SHIFT;
    shiftedWord &= FR_FONT_MIDDLE_BYTE_MASK;
    lowerMiddleByte <<= FR_FONT_BYTE_SHIFT;
    outerBytes |= word;
    shiftedWord |= lowerMiddleByte;
    return outerBytes | shiftedWord;
}

/* Return the zero-based highest set bit; both zero and one return zero. */
s32 frConvertVal2N(u32 bitValue) {
    s32 shiftCount = 0;

    if (bitValue == 0) {
        return 0;
    }
    do {
        bitValue = bitValue >> 1;
        shiftCount += 1;
    } while (bitValue != 0);
    return (shiftCount + FR_FONT_BYTE_MASK) & FR_FONT_BYTE_MASK;
}

void frCreateDrawEnvPacket(frFontGsPacket_t *packet) {
    QUAD_ZERO(packet->dma.value);
    packet->dma.words[0] = 0x70000007;
    packet->dma.words[2] = 0;
    packet->dma.words[3] = 0x50000007;
    packet->gifTag = 0x1000000000008006ULL;
    packet->gifRegisters = 0xE;
    packet->writes[0].value = 0;
    packet->writes[0].reg = 0x3F;
    packet->writes[1].value = 0x44;
    packet->writes[1].reg = 0x42;
    packet->writes[2].value = 0;
    packet->writes[2].reg = 0x49;
    packet->writes[3].value = 0x8080;
    packet->writes[3].reg = 0x3B;
    packet->writes[4].value = 0;
    packet->writes[4].reg = 0x4A;
    packet->writes[5].value = 0x517ED;
    packet->writes[5].reg = 0x47;
}

INCLUDE_ASM(const s32, "interface/frFont", frCreateDrawFontPacket_20550848);

/* Submit glyph-relative coordinates and the cache node's owned UV rectangle. */
void frCreateUserFontPacket(s32 x, s32 y, s32 depth, frTextHandle_t *glyph,
                  s32 drawFlags) {
    frCreateDrawFontPacket_20550848(x + glyph->x, y + glyph->y,
                 glyph->cellDimensions.cellWidth,
                 glyph->cellDimensions.cellHeight >> FR_FONT_GLYPH_HEIGHT_SHIFT,
                 glyph->renderValueOrSetupOrShade.setupBytes.firstOption, glyph->parentDimensionsOrRenderWord.renderWord, depth, 1,
                 &glyph->cachedItem->list->uv, &gFT.atlas, drawFlags);
}

/* Upload an image to GS memory at the given buffer, serialized by the GS semaphore. */
void frCopyFontClutToVram(s32 buffer, s32 image) {
    u8 loadImage[0x60];

    sceGsSetDefLoadImage(loadImage, (s16)buffer, 1, 0, 0, 0, 8, 2);
    WaitSema(dds3GsSema);
    FlushCache(0);
    sceGsExecLoadImage(loadImage, image);
    sceGsSyncPath(0, 0);
    SignalSema(dds3GsSema);
}

void frClearTexBuf(void) {
    u8 loadImage[0x60];
    sdfMemHandle_t *allocation;
    u8 *pixels;
    s32 byteCount = gFT.atlas.height * gFT.atlas.width;

    byteCount = (u32)byteCount >> 1;
    allocation = sdfMemAllocDirect(byteCount);
    pixels = (u8 *)sdfMemLock(allocation);
    memset(pixels, 0x11, byteCount);
    sceGsSetDefLoadImage(loadImage, gFT.atlas.bufferBase, gFT.atlas.bufferWidth,
                         FR_FONT_PIXEL_FORMAT, 0, 0, gFT.atlas.width, gFT.atlas.height);
    WaitSema(dds3GsSema);
    FlushCache(0);
    sceGsExecLoadImage(loadImage, (s32)pixels);
    sceGsSyncPath(0, 0);
    SignalSema(dds3GsSema);
    sdfMemFreeDirect(allocation);
}

void frLoadFontData(s32 id, const char *path) {
    u32 slotIndex = id & FR_FONT_BYTE_MASK;

    if (frFontLoaded[slotIndex] == 1) {
        return;
    }
    if (slotIndex == 1) {
        frAddFontStyle(1, NULL, gFT.entries[0].allocation);
    } else {
        frAddFontStyle(slotIndex, NULL, sdfDevLoadFileEx(path, 0, 0));
    }
    frFontLoaded[slotIndex] = 1;
}

void frInitFontTable(s32 rowBudget, s32 poolCount) {
    s32 i;
    s32 rowCount;
    s32 glyphsPerRow;
    s32 rowTotal;
    s32 glyphRingCount;
    sdfGsMemSlice_t *atlasHead;

    gFT.cachedItemCount = 0;
    gFT.itemCount = 0;
    gFT.glyphCount = 0;
    for (i = 0; i < 9; i++) {
        gFT.entries[i].resource = NULL;
    }
    glyphsPerRow = 21;
    rowCount = rowBudget / glyphsPerRow;
    rowTotal = rowCount;
    glyphRingCount = poolCount;
    poolCount = glyphRingCount * 4;
    gFT.atlas.width = 0x200;
    gFT.atlas.widthExponent = frConvertVal2N(0x200);
    gFT.atlas.height = 1 << (frConvertVal2N(rowTotal * 0x18) + 1);
    gFT.atlas.heightExponent = frConvertVal2N(gFT.atlas.height);
    gFT.textureHead1 = sdfGsMemAllocFromTail(0x180, 3);
    for (i = 0; i < 6; i++) {
        gFT.imageBuffers[i] = (gFT.textureHead1->addr >> 6) + i;
    }
    atlasHead = sdfGsMemAllocFromTail((u32)(gFT.atlas.width * gFT.atlas.height) >> 3, 2);
    gFT.textureHead0 = atlasHead;
    gFT.atlas.bufferBase = atlasHead->addr >> 6;
    gFT.atlas.bufferWidth = (u32)gFT.atlas.width >> 6;
    for (i = 0; i < 2; i++) {
        gFT.glyphSlots[i] = NULL;
    }
    fmInitFontMemControl(gFT.atlas.bufferBase, glyphsPerRow, rowCount, 0x18, 0x18, 0xC);
    gFT.itemPool = frAllocFontTable(0x2C, poolCount);
    gFT.glyphPool = frAllocFontTable(sizeof(frFontHandle_t), glyphRingCount);
    frLoadStyleAll();
    frSetFontClut();
    for (i = 0; i < FR_FONT_INDEXED_TEXT_SLOT_COUNT; i++) {
        gVarStr[i] = 0;
    }
    frClearTexBuf();
    gRegistMax = glyphsPerRow * rowTotal;
}

/* Release slot allocations, both glyph chains/rings, and the shared GS resource buffers. */
void frDestroyFontTable(dds3ProcessID_t *task) {
    frDelStyleAll();
    frDestroyFontRequestNow(gFT.glyphSlots[0]);
    frDestroyFontRequestNow(gFT.glyphSlots[1]);
    frFreeFontTable(gFT.itemPool);
    frFreeFontTable(gFT.glyphPool);
    fmDestroyFontMemControl();
    sdfGsMemFree(gFT.textureHead0);
    sdfGsMemFree(gFT.textureHead1);
}

/* Bind sections from resource bytes or a retained allocation.
 * Optional flag/value sections each start with a byte-length word; their data
 * precedes the word table and remaining resource data. Offsets are unchecked.
 */
void frAddFontStyle(
    u8 slotIndex, u8 *resourceBytes, sdfMemHandle_t *allocation
) {
    frFontEntry_t *entry;
    s32 sectionOffset;
    u32 lookupOffset;

    if (resourceBytes == NULL) {
        if (allocation != NULL) {
            resourceBytes = (u8 *)sdfMemLock(allocation);
        }
    }
    entry = &gFT.entries[slotIndex];
    entry->allocation = allocation;
    entry->resourceHeader = (frFontHeader_t *)resourceBytes;
    sectionOffset = entry->resourceHeader->tableOffset + (entry->resourceHeader->tableCount << FR_FONT_TABLE_ENTRY_SHIFT);
    if (entry->resourceHeader->hasExtra != 0) {
        s32 *flagSection = (s32 *)(resourceBytes + sectionOffset);
        s32 flagDataBytes = *flagSection;
        s32 *valueSection;
        s32 valueDataBytes;
        s32 flagSectionBytes;
        s32 valueSectionBytes;

        entry->flagBytes = (s8 *)(flagSection + 1);
        entry->metricByteCount = flagDataBytes;
        flagSectionBytes = flagDataBytes + FR_FONT_WORD_BYTES;
        sectionOffset += flagSectionBytes;
        valueSection = (s32 *)(resourceBytes + sectionOffset);
        valueDataBytes = *valueSection;
        entry->unk14 = valueSection + 1;
        entry->valueByteCount = valueDataBytes;
        valueSectionBytes = valueDataBytes + FR_FONT_WORD_BYTES;
        sectionOffset += valueSectionBytes;
    } else {
        entry->flagBytes = NULL;
        entry->unk14 = NULL;
        entry->metricByteCount = 0;
        entry->valueByteCount = 0;
    }
    entry->slots = (frFontRecord_t **)(resourceBytes + sectionOffset);
    sectionOffset += entry->resourceHeader->widthCount * FR_FONT_WORD_BYTES;
    entry->resource = (struct frFontResource_t *)(resourceBytes + sectionOffset);
    lookupOffset = entry->resourceHeader->lookupOffset;
    if (lookupOffset != 0) {
        entry->unk20 = (u32)(resourceBytes + lookupOffset);
    } else {
        entry->unk20 = 0;
    }
}

/* Slots zero/one are not freed here; clear the data pointer only when an allocation exists. */
void frDelFontStyle(s32 slotId) {
    u32 slotIndex = slotId & FR_FONT_BYTE_MASK;
    frFontEntry_t *entry;

    if (slotIndex < FR_FONT_FIRST_RELEASABLE_SLOT) {
        return;
    }
    frFontLoaded[slotIndex] = 0;
    entry = &gFT.entries[slotIndex];
    if (entry->allocation != NULL) {
        sdfMemFreeDirect(entry->allocation);
        entry->resource = NULL;
    }
    entry->slots = NULL;
}

/* Ensure the four named default font files are installed, in slot order. */
void frLoadStyleAll(void) {
    frLoadFontData(0, "/font/font0.fnt");
    frLoadFontData(1, "/font/font1.fnt");
    frLoadFontData(2, "/font/font2.fnt");
    frLoadFontData(3, "/font/font3.fnt");
}

/* Free each installed entry, including the temporary slot, without reloading. */
void frDelStyleAll(void) {
    s32 fontIndex;

    for (fontIndex = 0; fontIndex < FR_FONT_ENTRY_CAPACITY; fontIndex++) {
        if (gFT.entries[fontIndex].resource != NULL) {
            frDelFontStyle((u8)fontIndex);
        }
    }
}

/* Assemble six 16-word image blocks from little-endian source bytes and upload
 * each to its matching GS buffer. The inner countdown includes zero. */
void frSetFontClut(void) {
    u32 imageWords[FR_FONT_IMAGE_WORD_COUNT];
    s32 bufferIndex = 0;
    s32 sourceWordOffset = 0;
    frFontSystem_t *work = &gFT;
    u32 *imageBuffer = work->imageBuffers;
    const u8 *sourceTable = frFontClutTable;

    do {
        u32 *outputWord = imageWords;
        const u8 *sourceBytes = (const u8 *)(sourceWordOffset * FR_FONT_IMAGE_WORD_BYTES + (u32)sourceTable);
        s32 wordCountdown = FR_FONT_IMAGE_LAST_WORD;

        do {
            *outputWord = (((sourceBytes[3] << 8) | sourceBytes[2]) << 8 |
                       sourceBytes[1]) << 8 | sourceBytes[0];
            sourceBytes += FR_FONT_IMAGE_WORD_BYTES;
            outputWord++;
            wordCountdown--;
        } while (wordCountdown >= 0);

        frCopyFontClutToVram(*imageBuffer++, (s32)imageWords);
        bufferIndex++;
        sourceWordOffset += FR_FONT_IMAGE_WORD_COUNT;
    } while (bufferIndex < FR_FONT_IMAGE_BUFFER_COUNT);
}

void frFlushGsFontTable(frTextHandle_t *glyph) {
    frFontRecord_t *cachedItem = glyph->cachedItem;

    if (cachedItem != NULL) {
        if (cachedItem->refs == 0) {
            gFT.entries[glyph->renderValueOrSetupOrShade.setupBytes.fontIndex].slots[cachedItem->id] = NULL;
            fmRetFontMem(cachedItem->list);
            gFT.cachedItemCount--;
        }
    }
}

/* Retain the chain if any fade value changed; otherwise release it and return NULL. */
frFontHandle_t *frDestroyFontRequest(frFontHandle_t *glyph) {
    if (frEraseEffect(glyph) != 0) {
        return glyph;
    }
    return frDestroyFontRequestNow(glyph);
}

frFontHandle_t *frDestroyFontRequestNow(frFontHandle_t *glyph) {
    if (glyph == NULL) {
        return NULL;
    }
    do {
        frTextHandle_t *childGlyph = glyph->firstChild;
        frFontHandle_t *previousGlyph;

        while (childGlyph != NULL) {
            frTextHandle_t *nextGlyph = childGlyph->next;

            if (childGlyph->sourceItem == NULL) {
                childGlyph->cachedItem->refs--;
                frFlushGsFontTable(childGlyph);
            } else {
                frRestoreNameImage(*(s8 *)childGlyph->sourceItem);
            }
            frFreeFontNode(childGlyph, gFT.itemPool);
            gFT.itemCount--;
            childGlyph = nextGlyph;
        }
        previousGlyph = glyph->previous;
        frFreeFontNode(glyph, gFT.glyphPool);
        gFT.glyphCount--;
        glyph = previousGlyph;
    } while (glyph != NULL);
    return NULL;
}

/* Link without repositioning into the draw-buffer-indexed queue; return 0.
 * The low-byte buffer index is used directly, without a two-slot bounds check. */
s32 frDestroyFontRequestAfter(frFontHandle_t *glyph) {
    frFontHandle_t **queueSlot = &gFT.glyphSlots[dds3GetFrameID() & 0xFF];

    *queueSlot = frCombineFontRequestEx(*queueSlot, glyph, 0);
    return 0;
}

/* Return the live count of retained font-cache records. */
s32 frGetGsFontNum(void) {
    return gFT.cachedItemCount;
}

/* Return the live pooled child-glyph count. */
s32 frGetTextHandleNum(void) {
    return gFT.itemCount;
}

u32 frGetFontHandleNum(void) {
    return frFontHandleNum[0];
}

s32 frGetYShiftLev(s8 fontIndex, s32 shiftIndex) {
    return 0x18;
}

INCLUDE_ASM(const s32, "interface/frFont", frGetXShiftLev);

/* Create a glyph wrapper for source and append it to an optional destination chain. */
frFontHandle_t *frSetUserImage(frFontRecord_t *source, frFontHandle_t *destination) {
    frTextHandle_t *glyph;
    frTextHandle_t *previous;

    if (destination == NULL) {
        destination = frAllocFontNode(gFT.glyphPool);
        gFT.glyphCount++;
        frInitFontHandle(destination);
    }
    glyph = frAllocFontNode(gFT.itemPool);
    previous = destination->lastChild;
    gFT.itemCount++;
    frInitTextHandle(glyph, 0, 0, 0, 0xA09DC300, 0);
    if (previous == NULL) {
        destination->firstChild = glyph;
    } else {
        previous->next = glyph;
    }
    glyph->sourceItem = source;
    glyph->advance = gFontBaseWidth[0];
    glyph->cellDimensions.cellWidth = frGetDefaultFontWidth(0);
    glyph->cellDimensions.cellHeight = frGetDefaultFontHeight(0);
    glyph->previous = previous;
    destination->lastChild = glyph;
    destination->childCount++;
    destination->advance += glyph->advance;
    destination->parentDimensionsOrRenderWord.parentDimensions.cellAdvance = glyph->cellDimensions.cellWidth;
    destination->parentDimensionsOrRenderWord.parentDimensions.cellHeight = glyph->cellDimensions.cellHeight;
    return destination;
}

/* Append a source-glyph reference wrapper, not a deep copy of source storage.
 * If wrapper creation returns NULL, preserve the original destination. */
frFontHandle_t *frLinkUserImage(frFontRecord_t *source, frFontHandle_t *destination) {
    frFontHandle_t *referenceChain = frSetUserImage(source, 0);

    if (referenceChain == NULL) {
        return destination;
    }
    return frCombineFontRequest(destination, referenceChain);
}

/* Retain one atlas item and submit its pixel upload with the requested ownership
 * mode. Coordinates are converted from sixteenths; pool exhaustion is unchecked. */
frFontRecord_t *frTranslateUserData(s32 width, s32 height, void *pixels, s8 ownsPixels) {
    s32 imagePosition[2];
    sdfImageRequest_t uploadRequest;
    fmFontNode_t *atlasNode;
    frFontRecord_t *cachedItem;

    atlasNode = fmRentFontMem();
    cachedItem = atlasNode->item;
    /* Convert fixed-point atlas placement to integer image coordinates. */
    imagePosition[0] = atlasNode->uv.u0 >> FR_FONT_ATLAS_POSITION_SHIFT;
    imagePosition[1] = atlasNode->uv.v0 >> FR_FONT_ATLAS_POSITION_SHIFT;
    cachedItem->refs = 1;
    cachedItem->list = atlasNode;
    if (ownsPixels == 0) {
        uploadRequest.allocationMode = SDF_IMAGESET_MEM_IGNORE;
    } else {
        uploadRequest.allocationMode = SDF_IMAGESET_MEM_CHIP;
    }
    uploadRequest.pixels = pixels;
    uploadRequest.format = FR_FONT_PIXEL_FORMAT;
    uploadRequest.width = width;
    uploadRequest.height = height;
    uploadRequest.bufferWidth = gFT.atlas.width;
    uploadRequest.destination = gFT.atlas.bufferBase << FR_FONT_ATLAS_DEST_SHIFT;
    uploadRequest.x = imagePosition[0];
    uploadRequest.y = imagePosition[1];
    sdfImageSetReq(&uploadRequest);
    return cachedItem;
}

/* Decode one packed font-resource entry into a fixed-size clone.
 * Neither allocation nor entry index is checked. */
u8 *frDecodeFontImage(u8 fontIndex, s32 entryIndex) {
    frFontEntry_t *entry = &gFT.entries[fontIndex];
    u8 *clonedResource = sdfChipAllocDirect(FR_FONT_RESOURCE_CLONE_BYTES);
    frFontImage_t segments;

    frGetFontImagePtrs(entry->resource, &segments);
    frDecodeFontRow(clonedResource, entryIndex, entry->resource, &segments);
    return clonedResource;
}

/* Retain a cached table item, or create/upload a new one and cache it.
 * New records start with one reference; itemIndex is not bounds-checked. */
frFontRecord_t *frTranslateFont2GS(frTextHandle_t *glyph, s32 itemIndex) {
    frFontEntry_t *entry = &gFT.entries[glyph->renderValueOrSetupOrShade.setupBytes.fontIndex];
    frFontRecord_t *cachedItem = entry->slots[itemIndex];
    u8 *clonedResource;

    if (cachedItem != NULL) {
        cachedItem->refs++;
        return cachedItem;
    }
    clonedResource = frDecodeFontImage(glyph->renderValueOrSetupOrShade.setupBytes.fontIndex, itemIndex);
    cachedItem = frTranslateUserData(frGetFontWidth(glyph->renderValueOrSetupOrShade.setupBytes.fontIndex), frGetFontHeight(glyph->renderValueOrSetupOrShade.setupBytes.fontIndex), clonedResource, 1);
    cachedItem->id = itemIndex;
    entry->slots[itemIndex] = cachedItem;
    gFT.cachedItemCount++;
    return cachedItem;
}

/* Store the font/options, narrowed glyph code and high flag bits, then clear
 * positioning/links. Other glyph fields are deliberately not fully reset here. */
void frInitTextHandle(frTextHandle_t *glyph, s32 glyphId, s32 fontIndex, s32 firstOption, s32 packedFlags, s32 secondOption) {
    glyph->renderValueOrSetupOrShade.setupBytes.fontIndex = fontIndex;
    glyph->renderValueOrSetupOrShade.setupBytes.firstOption = firstOption;
    glyph->renderValueOrSetupOrShade.setupBytes.secondOption = secondOption;
    glyph->glyphCodeOrContext.glyphCode = glyphId;
    glyph->parentDimensionsOrRenderWord.renderWord = packedFlags & ~FR_FONT_BYTE_MASK;
    glyph->renderValueOrSetupOrShade.setupBytes.sharedFlags = gModeFlag;
    glyph->x = 0;
    glyph->y = 0;
    glyph->advance = 0;
    glyph->drawCount = 0;
    glyph->cachedItem = NULL;
    glyph->sourceItem = NULL;
    glyph->previous = NULL;
    glyph->next = NULL;
}

void frInitFontHandle(frFontHandle_t *glyph) {
    glyph->glyphCodeOrContext.byteRoles.encodedContextByte = FR_FONT_CONTEXT_BYTE_CAP;
    glyph->x = 0;
    glyph->y = 0;
    glyph->glyphCodeOrContext.byteRoles.spacing = 0;
    glyph->advance = 0;
    glyph->renderValueOrSetupOrShade.renderValue = 0;
    glyph->previous = NULL;
    glyph->next = NULL;
    glyph->chainHead = glyph;
    glyph->firstChild = NULL;
    glyph->lastChild = NULL;
    glyph->childCount = 0;
}

frTextHandle_t *frSetFont(u16 glyphCode, u8 fontIndex, u8 firstOption, u8 secondOption, s32 shiftFlag) {
    frTextHandle_t *glyph = frAllocFontNode(gFT.itemPool);
    s32 shiftIndex = 0;

    gFT.itemCount++;
    frInitTextHandle(glyph, glyphCode, fontIndex, firstOption, 0xA09DC300, secondOption);
    if (glyph->glyphCodeOrContext.glyphCode >= 0x20) {
        if (glyph->glyphCodeOrContext.glyphCode < 0x80) {
            if (glyph->glyphCodeOrContext.glyphCode == 0x5E) {
                glyph->glyphCodeOrContext.glyphCode = 0x26;
            }
            shiftIndex = glyph->glyphCodeOrContext.glyphCode - 0x20;
        } else {
            s32 relative = glyph->glyphCodeOrContext.glyphCode - 0x8080;

            shiftIndex = ((relative & FR_FONT_MIDDLE_BYTE_MASK) >> 1) + (relative & 0x7F);
        }
    }
    glyph->cachedItem = frTranslateFont2GS(glyph, shiftIndex);
    frGetXShiftLev(glyph, shiftIndex, shiftFlag);
    if ((glyph->renderValueOrSetupOrShade.setupBytes.sharedFlags & 0x10) != 0) {
        glyph->y = frGetYShiftLev((s8)fontIndex, shiftIndex);
    } else {
        glyph->y = 0;
    }
    return glyph;
}

frFontHandle_t *frLinkFontString(u8 *text, s8 fontIndex, s8 firstOption, s8 secondOption, frFontHandle_t *destination) {
    frFontHandle_t *chain = frSetFontString(text, fontIndex, firstOption, secondOption, NULL, 0);

    if (chain == NULL) {
        return destination;
    }
    return frCombineFontRequest(destination, chain);
}

frFontHandle_t *frSetFontString(u8 *text, s8 style, s8 firstOption, s8 secondOption, frFontHandle_t *chain, s32 relative) {
    s32 i;
    u32 length = 0;
    frTextHandle_t *glyph;
    frTextHandle_t *previous;

    if (style >= 0) {
        frSetFontString_style = style;
    }
    if (gFT.entries[frSetFontString_style].resource == NULL) {
        return NULL;
    }
    if (text != NULL) {
        length = strlen((const char *)text);
    }
    if (chain == NULL) {
        chain = frAllocFontNode(gFT.glyphPool);
        gFT.glyphCount++;
        frInitFontHandle(chain);
    }
    previous = chain->lastChild;
    i = 0;
    glyph = NULL;
    if (length != 0) {
        do {
            u32 code = (u8)text[i];

            if (code >= 0x80) {
                i++;
                code = (u8)text[i] | (code << FR_FONT_BYTE_SHIFT);
            }
            glyph = frSetFont(code, frSetFontString_style, firstOption, secondOption, (i != 0) ? 0 : (relative != 0));
            i++;
            if (previous == NULL) {
                chain->firstChild = glyph;
            } else {
                previous->next = glyph;
            }
            glyph->previous = previous;
            chain->advance += glyph->advance;
            chain->childCount++;
            frGetMessageCode(code);
            previous = glyph;
        } while (i < length);
    }
    if (glyph != NULL) {
        chain->lastChild = glyph;
        chain->parentDimensionsOrRenderWord.parentDimensions.cellAdvance = glyph->cellDimensions.cellWidth;
        chain->parentDimensionsOrRenderWord.parentDimensions.cellHeight = glyph->cellDimensions.cellHeight;
    }
    return chain;
}

/* Double the masked input byte and saturate the stored byte at 0x80.
 * Keep the original signed constant used for the saturated byte assignment. */
void frSetDrawSpeed(frFontHandle_t *glyph, s32 inputValue) {
    s32 encodedValue = (inputValue & FR_FONT_BYTE_MASK) * 2;

    if (encodedValue >= FR_FONT_CONTEXT_BYTE_THRESHOLD) {
        glyph->glyphCodeOrContext.byteRoles.encodedContextByte = FR_FONT_CONTEXT_BYTE_CAP;
    } else {
        glyph->glyphCodeOrContext.byteRoles.encodedContextByte = encodedValue;
    }
}

/* Store the signed spacing byte and refresh the cached child-chain advance. */
void frSetFontSpace(frFontHandle_t *glyph, s32 spacing) {
    glyph->glyphCodeOrContext.byteRoles.spacing = spacing;
    glyph->advance = frGetStrWidth(glyph);
}

/* Set the initial glyph's halfword dimensions and every visited child's advance/
 * byte dimensions, then remeasure only the initial glyph. Input must be non-NULL. */
void frSetFontSize(frFontHandle_t *glyph, s32 cellAdvance, s32 cellHeight) {
    frFontHandle_t *targetGlyph = glyph;
    frTextHandle_t *childGlyph;

    targetGlyph->parentDimensionsOrRenderWord.parentDimensions.cellAdvance = cellAdvance;
    targetGlyph->parentDimensionsOrRenderWord.parentDimensions.cellHeight = cellHeight;
    for (; glyph != NULL; glyph = glyph->previous) {
        for (childGlyph = glyph->firstChild; childGlyph != NULL; childGlyph = childGlyph->next) {
            childGlyph->advance = cellAdvance;
            childGlyph->cellDimensions.cellWidth = cellAdvance;
            childGlyph->cellDimensions.cellHeight = cellHeight;
        }
    }
    targetGlyph->advance = frGetStrWidth(targetGlyph);
}

/* Store the glyph position without scaling. */
void frSetDrawPosition(frFontHandle_t *glyph, u32 x, u32 y) {
    glyph->x = x;
    glyph->y = y;
}

/* Store the glyph render value in sixteenths. */
void frSetPriority(frFontHandle_t *glyph, u32 unshiftedValue) {
    glyph->renderValueOrSetupOrShade.renderValue = unshiftedValue >> FR_FONT_CONTEXT_VALUE_SHIFT;
}

/* Assign the first option byte across all visited children; NULL is a no-op. */
void frSetFontColorNo(frFontHandle_t *glyph, u8 firstOption) {
    frTextHandle_t *childGlyph;

    for (; glyph != NULL; glyph = glyph->previous) {
        for (childGlyph = glyph->firstChild; childGlyph != NULL; childGlyph = childGlyph->next) {
            childGlyph->renderValueOrSetupOrShade.setupBytes.firstOption = firstOption;
        }
    }
}

/* Replace each child's full color word across the parent glyph chain. */
void frSetFontColor(frFontHandle_t *parentNode, u32 colorWord) {
    for (; parentNode != NULL; parentNode = parentNode->previous) {
        frTextHandle_t *childNode;
        for (childNode = parentNode->firstChild; childNode != NULL; childNode = childNode->next) {
            childNode->parentDimensionsOrRenderWord.renderWord = colorWord;
        }
    }
}

/* OR the requested mask into the shared byte, preserving implicit narrowing. */
void frEnableMode(s32 mask) {
    mask |= gModeFlag;
    gModeFlag = mask;
}

/* Clear flag bits from the shared font flag byte; returns the previous value. */
u8 frDisableMode(u8 mask) {
    u8 previousFlags = gModeFlag;

    gModeFlag = previousFlags & ~mask;
    return previousFlags;
}

/* Replace the complete shared render-flag word used by subsequent drawing. */
void frSetFontDrawOT(u32 renderFlags) {
    gFontDrawOT = renderFlags;
}

/* Step only second-option-2 children: decrease the low byte by eight, clamp at
 * zero and move y by sixteen while changing it. Return whether any step occurred,
 * not whether the resulting chain still has a nonzero fade value. */
s32 frEraseEffect(frFontHandle_t *glyph) {
    frTextHandle_t *childGlyph;
    s32 didChange = 0;

    for (; glyph != NULL; glyph = glyph->previous) {
        for (childGlyph = glyph->firstChild; childGlyph != NULL; childGlyph = childGlyph->next) {
            if (childGlyph->renderValueOrSetupOrShade.setupBytes.secondOption == FR_FONT_FADE_OPTION) {
                u32 glyphWord = childGlyph->parentDimensionsOrRenderWord.renderWord;
                s32 fadeValue = glyphWord & FR_FONT_BYTE_MASK;

                if (fadeValue != 0) {
                    fadeValue -= FR_FONT_FADE_VALUE_STEP;
                    if (fadeValue < 0) {
                        fadeValue = 0;
                    }
                    childGlyph->y += FR_FONT_FADE_Y_STEP;
                    childGlyph->parentDimensionsOrRenderWord.renderWord = (glyphWord & ~FR_FONT_BYTE_MASK) | fadeValue;
                    didChange = 1;
                }
            }
        }
    }
    return didChange;
}

INCLUDE_ASM(const s32, "interface/frFont", frDrawEffect);

/* Draw through the shared render flags with mode zero. */
s32 frDrawFont(frFontHandle_t *glyph) {
    return frDrawFontEx(glyph, 0);
}

INCLUDE_ASM(const s32, "interface/frFont", frDrawFontEx);

/* Release queue slot 1 when the draw-buffer index's low byte is zero, otherwise
 * slot 0; return 0. This is buffer selection, not a current-font selection. */
s32 frFlipPacketBuf(dds3ProcessID_t *task) {
    s32 queueIndex = (dds3GetFrameID() & FR_FONT_BYTE_MASK) == 0;

    gFT.glyphSlots[queueIndex] = frDestroyFontRequestNow(gFT.glyphSlots[queueIndex]);
    return 0;
}

/* Link chains with the renderer's relative-positioning option enabled. */
frFontHandle_t *frCombineFontRequest(frFontHandle_t *previous, frFontHandle_t *next) {
    return frCombineFontRequestEx(previous, next, 1);
}

frFontHandle_t *frCombineFontRequestEx(frFontHandle_t *previous, frFontHandle_t *next, s32 relative) {
    if (previous == NULL) {
        return next;
    }
    if (next == NULL) {
        return previous;
    }
    previous->next = next->chainHead;
    next->chainHead->previous = previous;
    next->chainHead = previous->chainHead;
    if (relative == 1) {
        if ((next->firstChild != NULL) && (next->firstChild->glyphCodeOrContext.glyphCode == 0xD)) {
            s32 cellHeight = (s16)previous->parentDimensionsOrRenderWord.parentDimensions.cellHeight;

            next->x = previous->x;
            next->y = previous->y + cellHeight;
        } else {
            next->x = previous->x + previous->advance * 16;
            next->y = previous->y;
        }
    }
    return next;
}

/* Bind the caller's font data into the reserved temporary entry. */
void frSetExtDic(u8 *fontData) {
    frAddFontStyle(FR_FONT_TEMPORARY_SLOT, fontData, 0);
}

/* Free the reserved temporary font entry. */
void frDestroyExtDic(void) {
    frDelFontStyle(FR_FONT_TEMPORARY_SLOT);
}

/* Count characters until a zero lead byte. Any negative signed lead consumes
 * two bytes without checking its trailer; input must be a valid packed stream. */
s32 frStrLen(s8 *text) {
    s32 characterCount = 0;

    while (*text != 0) {
        if (*text >= 0) {
            text++;
        } else {
            text += 2;
        }
        characterCount++;
    }
    return characterCount;
}

/* Sum one parent's child advances and signed spacing after every child, even
 * the last. Require a non-NULL parent; preserve the existing unsigned result. */
u32 frGetStrWidth(frFontHandle_t *parentGlyph) {
    frTextHandle_t *childGlyph = parentGlyph->firstChild;
    s32 totalAdvance = 0;

    if (childGlyph != NULL) {
        s8 childSpacing = parentGlyph->glyphCodeOrContext.byteRoles.spacing;

        do {
            totalAdvance += childGlyph->advance;
            childGlyph = childGlyph->next;
            totalAdvance += childSpacing;
        } while (childGlyph != NULL);
    }
    return totalAdvance;
}

/* Sum all child advances/spacing from the chain origin. This is neither a line
 * count nor a maximum line width; require a non-NULL glyphChain. */
u32 frGetStrWidth2(frFontHandle_t *glyphChain) {
    frFontHandle_t *parentGlyph;
    frTextHandle_t *childGlyph;
    s32 totalAdvance = 0;

    for (parentGlyph = glyphChain->chainHead; parentGlyph != NULL; parentGlyph = parentGlyph->next) {
        childGlyph = parentGlyph->firstChild;
        if (childGlyph != NULL) {
            s8 childSpacing = parentGlyph->glyphCodeOrContext.byteRoles.spacing;

            do {
                totalAdvance += childGlyph->advance;
                childGlyph = childGlyph->next;
                totalAdvance += childSpacing;
            } while (childGlyph != NULL);
        }
    }
    return totalAdvance;
}

/* NON_MATCHING: 8 of 62 words differ (first at +0x2C). */
#ifdef NON_MATCHING
u32 frPreCalcStrWidth(s8 *text, s32 fontIndex, s32 sharedFlags) {
    frTextHandle_t glyph;
    s32 i = 0;
    s32 totalWidth = 0;
    u32 length;

    glyph.renderValueOrSetupOrShade.setupBytes.sharedFlags = sharedFlags;
    glyph.renderValueOrSetupOrShade.setupBytes.fontIndex = fontIndex;
    glyph.advance = 0;
    glyph.cellDimensions.cellWidth = frGetDefaultFontWidth(fontIndex);
    glyph.cellDimensions.cellHeight = frGetDefaultFontHeight(glyph.renderValueOrSetupOrShade.setupBytes.fontIndex);
    length = strlen((const char *)text);
    if (length == 0) {
        return totalWidth;
    }
    do {
        u32 code = (u8)text[i];
        s32 shiftIndex;

        if (code >= 0x80) {
            shiftIndex = code << FR_FONT_BYTE_SHIFT;
            i++;
            code = shiftIndex | (u8)text[i];
        }
        glyph.glyphCodeOrContext.glyphCode = code;
        if (code < 0x80) {
            shiftIndex = code - 0x20;
        } else {
            u32 relative = code - 0x8080;

            shiftIndex = ((relative & FR_FONT_MIDDLE_BYTE_MASK) >> 1) + (relative & 0x7F);
        }
        frGetXShiftLev(&glyph, shiftIndex, i == 0);
        i++;
        totalWidth += glyph.advance + 3;
    } while (i < length);
    return totalWidth;
}
#else
INCLUDE_ASM(const s32, "interface/frFont", frPreCalcStrWidth);
#endif

/* Slots 0/1 use the shared default size; later slots read their resource width.
 * Retain the nested signed check and the absence of upper-bound validation. */
u32 frGetDefaultFontWidth(u8 fontIndex) {
    s32 entryIndex = fontIndex;

    if (entryIndex < FR_FONT_DEFAULT_CELL_SLOT_COUNT) {
        if (entryIndex >= 0) {
            return frDefaultFontHandle;
        }
    }
    return gFT.entries[entryIndex].resourceHeader->cellWidth;
}

u16 frGetFontWidth(s32 slotIndex) {
    return gFT.entries[slotIndex & FR_FONT_BYTE_MASK].resourceHeader->cellWidth;
}

/* Slots 0/1 use the shared default size; later slots read their resource height.
 * Retain the nested signed check and the absence of upper-bound validation. */
u32 frGetDefaultFontHeight(u8 fontIndex) {
    s32 entryIndex = fontIndex;

    if (entryIndex < FR_FONT_DEFAULT_CELL_SLOT_COUNT) {
        if (entryIndex >= 0) {
            return frDefaultFontHandle;
        }
    }
    return gFT.entries[entryIndex].resourceHeader->cellHeight;
}

u16 frGetFontHeight(s32 slotIndex) {
    return gFT.entries[slotIndex & FR_FONT_BYTE_MASK].resourceHeader->cellHeight;
}

/* Restore Nocturne's default vertical message spacing of 20. */
void frSetMessageDefStepY(void) {
    gMessageStepY = FR_FONT_DEFAULT_CURSOR_SPACING;
}

/* Set the vertical advance used by encoded-message line breaks. */
void frSetMessageStepY(u32 lineStep) {
    gMessageStepY = lineStep;
}

/* Read the packed table count, then skip the table and its trailing metadata
 * word to reach the encoded text. The native parser copies that metadata even
 * though this lookup does not use its value. */
u8 *MessageNumSearch(s32 textId, frTextBank_t *bank, s32 mode) {
    u32 offset;
    u32 tableCount;
    u32 textMetadata;

    if (textId >= bank->count) {
        return NULL;
    }
    offset = bank->entries[textId].offset + 0x38;
    tableCount = 0;
    memcpy(&tableCount, (u8 *)bank + offset, 2);
    offset += 4;
    offset += tableCount * 4;
    memcpy(&textMetadata, (u8 *)bank + offset, sizeof(textMetadata));
    offset += sizeof(textMetadata);
    return (u8 *)bank + offset;
}

void frCreateVariableRequest(u8 slotIndex, frMessageContext_t *ctx) {
    u8 encoded[3];
    u8 *text = (u8 *)gVarStr[slotIndex];
    s32 length;
    u32 code;
    s32 i;
    s8 lastFontIndex;

    memset(encoded, 0, 3);
    if (text == NULL) {
        return;
    }
    length = strlen((const char *)text);
    lastFontIndex = ctx->fontIndex;
    for (i = 0; i < length; i++) {
        s8 fontIndex;

        encoded[0] = text[i];
        if ((encoded[0] & 0x80) != 0) {
            i++;
            code = encoded[0];
            encoded[1] = text[i];
            code = (code << FR_FONT_BYTE_SHIFT) | encoded[1];
        } else {
            encoded[1] = 0;
            code = encoded[0];
        }
        fontIndex = (u8)ctx->fontIndex;
        if (fontIndex != lastFontIndex) {
            frFontHandle_t *chain = ctx->glyphChain;

            if (chain != NULL) {
                if (chain->firstChild != NULL) {
                    ctx->glyphChain = frLinkFontString(D_0054F430, 0, ctx->firstOption, ctx->secondOption, chain);
                    chain = ctx->glyphChain;
                }
            } else {
                ctx->glyphChain = frLinkFontString(D_0054F430, 0, ctx->firstOption, ctx->secondOption, NULL);
                chain = ctx->glyphChain;
            }
            frSetDrawSpeed(chain, ctx->contextEncodedByte);
            lastFontIndex = fontIndex;
        }
        ctx->glyphChain = frSetFontString(encoded, fontIndex, ctx->firstOption, ctx->secondOption, ctx->glyphChain, i == 0);
        frSetFontSpace(ctx->glyphChain, gMessageSpaceX);
        frGetMessageCode(code);
    }
}

void frCreateNewHandle(frMessageContext_t *ctx) {
    frFontHandle_t *chain = ctx->glyphChain;

    if (chain != NULL) {
        if (chain->firstChild == NULL) {
            ctx->pendingCreate = 0;
        }
    }
    if (ctx->pendingCreate != 0) {
        ctx->glyphChain = frLinkFontString(D_0054F430, 0, ctx->firstOption, ctx->secondOption, chain);
        frSetDrawSpeed(ctx->glyphChain, ctx->contextEncodedByte);
        ctx->pendingCreate = 0;
    }
    if (ctx->pendingPosition != 0) {
        frSetDrawPosition(ctx->glyphChain, ctx->x, ctx->y);
        ctx->pendingPosition = 0;
    }
}

/* Advance by eight times cursor spacing and request both context updates. */
void frMessageReturnCode(frMessageContext_t *ctx) {
    ctx->y += gMessageStepY * FR_FONT_CONTEXT_CURSOR_SCALE;
    ctx->pendingCreate = 1;
    ctx->pendingPosition = 1;
}

void frPushTagChar(frMessageContext_t *ctx, u16 code) {
    u8 encoded[3];

    encoded[1] = code;
    encoded[0] = (u32)code >> FR_FONT_BYTE_SHIFT;
    encoded[2] = 0;
    frCreateNewHandle(ctx);
    ctx->glyphChain = frSetFontString(encoded, ctx->fontIndex, ctx->firstOption, ctx->secondOption, ctx->glyphChain, 0);
    frSetFontSpace(ctx->glyphChain, gMessageSpaceX);
    frSetPriority(ctx->glyphChain, ctx->z);
}

/* NON_MATCHING: 280 of 322 words differ (first at +0x44). */
#ifdef NON_MATCHING
s32 frAnalyzeTag(s32 code, frMessageContext_t *ctx) {
    s32 *offset = &ctx->encodedTextOffset;
    s32 argCount = code & 0xF;
    s32 pos = *offset;
    u8 *text = ctx->encodedText;
    s8 firstOption = ctx->firstOption;

    code = (code << FR_FONT_BYTE_SHIFT) | text[pos];
    pos++;
    *offset = pos;
    switch (code) {
    case 0xF206:
        ctx->fontIndex = text[pos] - 1;
        *offset = pos + 2;
        break;
    case 0xF202:
        ctx->firstOption = text[pos] - 1;
        *offset = pos + 2;
        break;
    case 0xF209:
        ctx->secondOption = text[pos] - 1;
        *offset = pos + 2;
        break;
    case 0xF207:
        ctx->contextEncodedByte = text[pos] - 1;
        *offset = pos + 2;
        break;
    case 0xF203:
        if (gVarStr[text[pos] - 1] != 0) {
            frCreateNewHandle(ctx);
            frCreateVariableRequest(ctx->encodedText[*offset] - 1, ctx);
        }
        *offset += 2;
        break;
    case 0xF20A: {
        s32 nameIndex;

        *offset = pos + 2;
        nameIndex = text[pos] - 1;
        frCreateNewHandle(ctx);
        ctx->glyphChain = frGetNameFRQ(nameIndex, ctx->glyphChain, firstOption);
        frSetFontSpace(ctx->glyphChain, gMessageSpaceX);
        break;
    }
    case 0xF20B: {
        s32 nameIndex;

        *offset = pos + 2;
        nameIndex = text[pos] - 1;
        frCreateNewHandle(ctx);
        ctx->glyphChain = frGetName1NFRQ(nameIndex, ctx->glyphChain, firstOption);
        frSetFontSpace(ctx->glyphChain, gMessageSpaceX);
        break;
    }
    case 0xF20C: {
        s32 nameIndex;

        *offset = pos + 2;
        nameIndex = text[pos] - 1;
        frCreateNewHandle(ctx);
        ctx->glyphChain = frGetName2NFRQ(nameIndex, ctx->glyphChain, firstOption);
        frSetFontSpace(ctx->glyphChain, gMessageSpaceX);
        break;
    }
    case 0xF20D: {
        s32 nameIndex;

        *offset = pos + 2;
        nameIndex = text[pos] - 1;
        frCreateNewHandle(ctx);
        ctx->glyphChain = frGetCNameFRQ(nameIndex, ctx->fontIndex, ctx->glyphChain, firstOption);
        frEnableMode(1);
        frSetFontSpace(ctx->glyphChain, gMessageSpaceX);
        frSetDrawSpeed(ctx->glyphChain, ctx->contextEncodedByte);
        break;
    }
    case 0xF20E:
        *offset = pos + 1;
        break;
    case 0xF10F:
        if ((gModeFlag & 4) == 0) {
            frMessageReturnCode(ctx);
        }
        break;
    case 0xF110:
        gMessageRequestResult |= 2;
        return 1;
    case 0xF111:
        if ((gModeFlag & 8) == 0) {
            break;
        }
        /* fallthrough */
    case 0xF104:
        gMessageRequestResult |= 1;
        return 1;
    case 0xF112:
        gMessageRequestResult |= 4;
        break;
    case 0xF113: {
        s32 tagChar = 0x80E3;

        if (frFindTagCode(frMessageCode) < 0) {
            tagChar = 0x80E2;
        }
        frPushTagChar(ctx, tagChar);
        break;
    }
    case 0xF114: {
        s32 tagChar = 0x80E5;

        if (frFindTagCode(frMessageCode) < 0) {
            tagChar = 0x80E4;
        }
        frPushTagChar(ctx, tagChar);
        break;
    }
    case 0xF115: {
        s32 tagChar = 0x80E1;

        if (frFindTagCode(frMessageCode) < 0) {
            tagChar = 0x80E0;
        }
        frPushTagChar(ctx, tagChar);
        break;
    }
    case 0xF116:
        if (frFindTagCode(frMessageCode) < 0) {
            frPushTagChar(ctx, 0x80E2);
        }
        break;
    case 0xF117:
        if (frFindTagCode(frMessageCode) < 0) {
            frPushTagChar(ctx, 0x80E7);
        } else {
            frPushTagChar(ctx, 0x80E6);
        }
        break;
    case 0xF118:
        if (frFindTagCode(frMessageCode) < 0) {
            if (frFindTagCode2(frMessageCode) < 0) {
                frPushTagChar(ctx, 0x80E9);
            }
        }
        frPushTagChar(ctx, 0x80E8);
        break;
    default:
        ctx->encodedTextOffset = (argCount - 1) * 2 + ctx->encodedTextOffset;
        break;
    }
    if (ctx->pendingCreate == 0) {
        ctx->pendingCreate = 1;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "interface/frFont", frAnalyzeTag);
#endif

s32 frGetTagArgCount(s32 tagCode) {
    s32 argCount = 2;

    switch (tagCode) {
    case 0xF202:
    case 0xF203:
    case 0xF206:
    case 0xF207:
    case 0xF209:
    case 0xF20A:
    case 0xF20B:
    case 0xF20C:
    case 0xF20D:
        argCount = 4;
        break;
    case 0xF20E:
        argCount = 3;
        break;
    }
    return argCount;
}

void frCopyTagArgs(s8 *text, s8 *dest, s32 maxArgs) {
    s32 argIndex = 0;

    while (*text != 0) {
        s8 code = (u8)text[0];
        s32 step;

        if ((code & 0xF0) == 0xF0) {
            step = frGetTagArgCount(((code << FR_FONT_BYTE_SHIFT) & FR_FONT_MIDDLE_BYTE_MASK) | (u8)text[1]);
        } else {
            step = 1;
            if (code == 0xA) {
                argIndex++;
            } else if (code & 0x80) {
                step = 2;
            }
        }
        if (argIndex == maxArgs) {
            if (*text != 0xA) {
                s32 i;

                for (i = 0; i < step; i++) {
                    *dest = (u8)*text;
                    dest++;
                    text++;
                }
            } else {
                text++;
            }
        } else {
            text += step;
        }
    }
    *dest = 0;
}

s32 frCalcStringHash(s8 *text) {
    s32 lineCount = 0;

    while (*text != 0) {
        s8 code = (u8)text[0];
        s32 step;

        if ((code & 0xF0) == 0xF0) {
            step = frGetTagArgCount(((code << FR_FONT_BYTE_SHIFT) & FR_FONT_MIDDLE_BYTE_MASK) | (u8)text[1]);
        } else {
            step = 1;
            if (code == 0xA) {
                lineCount++;
            } else if (code & 0x80) {
                step = 2;
            }
        }
        text += step;
    }
    return lineCount;
}

/* NON_MATCHING: 7 of 128 words differ (first at +0x6C). */
#ifdef NON_MATCHING
frFontHandle_t *frAnalyzeMessage(frMessageContext_t *ctx) {
    u8 encoded[3];
    s32 isFirst = 1;
    s32 *offset;

    if (((ctx->encodedText[0] & 0xF0) != 0xF0) && (ctx->encodedText[1] == 8)) {
        ctx->encodedTextOffset += 4;
    }
    gMessageRequestResult &= ~1;
    gMessageRequestResult &= ~2;
    gMessageRequestResult &= ~4;
    frEnableMode(1);
    offset = &ctx->encodedTextOffset;
    while (1) {
        s32 code = ctx->encodedText[*offset];

        (*offset)++;
        if (code == 0) {
            break;
        }
        if ((code & 0xF0) == 0xF0) {
            if (frAnalyzeTag(code, ctx) != 0) {
                break;
            }
            continue;
        }
        if (code == 0xA) {
            if ((gModeFlag & 4) != 0) {
                frMessageReturnCode(ctx);
            }
            isFirst = 1;
            continue;
        }
        frCreateNewHandle(ctx);
        if (code < 0x80) {
            encoded[0] = code;
            encoded[1] = 0;
            ctx->glyphChain = frSetFontString(encoded, ctx->fontIndex, ctx->firstOption, ctx->secondOption, ctx->glyphChain, isFirst);
        } else {
            code = (code << FR_FONT_BYTE_SHIFT) | ctx->encodedText[(*offset)++];
            if (code <= 0xC07F) {
                encoded[0] = (u32)code >> FR_FONT_BYTE_SHIFT;
                encoded[1] = code;
                encoded[2] = 0;
                ctx->glyphChain = frSetFontString(encoded, ctx->fontIndex, ctx->firstOption, ctx->secondOption, ctx->glyphChain, isFirst);
            } else {
                code -= 0x4000;
                encoded[0] = (u32)code >> FR_FONT_BYTE_SHIFT;
                encoded[1] = code;
                encoded[2] = 0;
                ctx->glyphChain = frSetFontString(encoded, FR_FONT_TEMPORARY_SLOT, ctx->firstOption, ctx->secondOption, ctx->glyphChain, isFirst);
            }
            frGetMessageCode(code & 0xFFFF);
        }
        isFirst = 0;
        frSetFontSpace(ctx->glyphChain, gMessageSpaceX);
        frSetPriority(ctx->glyphChain, ctx->z);
    }
    frDisableMode(1);
    frEnableMode(2);
    return ctx->glyphChain;
}
#else
INCLUDE_ASM(const s32, "interface/frFont", frAnalyzeMessage);
#endif

frFontHandle_t *MessageRequest(s32 x, s32 y, u8 *encodedText, frFontHandle_t *sub) {
    return MessageRequestEx2(x, y, 0, 0, 0, 0, 0x80, encodedText, sub);
}

frFontHandle_t *MessageRequestEx(s32 x, s32 y, s32 channel0, s32 channel1, s32 channel2, s32 channel3, u8 *encodedText, frFontHandle_t *sub) {
    return MessageRequestEx2(x, y, 0, channel0 & 0xFF, channel1 & 0xFF, channel2 & 0xFF, channel3 & 0xFF, encodedText, sub);
}

frFontHandle_t *MessageRequestEx2(s32 x, s32 y, s32 depth, s32 channel0, s32 channel1, s32 channel2, s32 channel3, u8 *encodedText, frFontHandle_t *sub) {
    frMessageContext_t args;

    args.x = x;
    args.y = y;
    args.z = depth << 4;
    args.fontIndex = channel0;
    args.firstOption = channel1;
    args.secondOption = channel2;
    args.contextEncodedByte = channel3;
    args.encodedText = encodedText;
    args.glyphChain = sub;
    args.encodedTextOffset = 0;
    args.pendingCreate = 1;
    args.pendingPosition = 1;
    return frAnalyzeMessage(&args);
}

frFontHandle_t *MessageRequestNS(s32 x, s32 y, s32 depth, s32 channel0, s32 channel1, s32 channel2, s32 channel3, u8 *encodedText) {
    s8 savedSpacing = gMessageSpaceX;
    frFontHandle_t *chain = MessageRequestEx2(x, y, depth, channel0 & 0xFF, channel1 & 0xFF, channel2 & 0xFF, channel3 & 0xFF, encodedText, NULL);

    gMessageSpaceX = savedSpacing;
    return chain;
}

u32 frGetMessageResult(u32 mask) {
    return gMessageRequestResult & mask;
}

/* Store an unchecked replacement-text address word in the font's indexed table. */
void frSetVariableRequest(s32 slotIndex, s32 encodedTextAddress) {
    gVarStr[slotIndex] = encodedTextAddress;
}

INCLUDE_ASM(const s32, "interface/frFont", frDumpFontTexture);

INCLUDE_RODATA(const s32, "interface/frFont", D_00535968);

INCLUDE_RODATA(const s32, "interface/frFont", D_00535978);

INCLUDE_RODATA(const s32, "interface/frFont", D_00535988);

INCLUDE_SDATA(const s32, "interface/frFont", gMessageRequestResult);

INCLUDE_SDATA(const s32, "interface/frFont", gMessageSpaceX);

INCLUDE_SDATA(const s32, "interface/frFont", D_0054F409);

INCLUDE_SDATA(const s32, "interface/frFont", gMessageStepY);

INCLUDE_SDATA(const s32, "interface/frFont", frDefaultFontHandle);

INCLUDE_SDATA(const s32, "interface/frFont", D_0054F414);

INCLUDE_SDATA(const s32, "interface/frFont", gModeFlag);

INCLUDE_SDATA(const s32, "interface/frFont", D_0054F41D);

INCLUDE_SDATA(const s32, "interface/frFont", gFontDrawOT);

INCLUDE_SDATA(const s32, "interface/frFont", frSetFontString_style);

INCLUDE_SDATA(const s32, "interface/frFont", D_0054F425);

INCLUDE_SDATA(const s32, "interface/frFont", frDrawFontWork);

INCLUDE_SDATA(const s32, "interface/frFont", D_0054F42C);

INCLUDE_SDATA(const s32, "interface/frFont", D_0054F430);

INCLUDE_SDATA(const s32, "interface/frFont", D_0054F438);

