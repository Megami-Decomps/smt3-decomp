#include "font.h"
#include "kernel.h"

typedef struct frFontHeader_t frFontHeader_t;

typedef struct frFontResource_t frFontResource_t;

typedef struct frFontData_t frFontData_t;

typedef struct frFontNodeHeader_t frFontNodeHeader_t;

typedef struct frFontAtlas_t frFontAtlas_t;

typedef struct frFontSystem_t frFontSystem_t;

typedef union frFontDmaTag_t frFontDmaTag_t;

typedef struct frFontGsWrite_t frFontGsWrite_t;

typedef struct frFontGsPacket_t frFontGsPacket_t;

typedef struct frTextIndex_t frTextIndex_t;

typedef struct frTextBank_t frTextBank_t;

typedef struct frMsgInfo_t frMsgInfo_t;

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

struct frFontData_t {
    sdfMemHandle_t *allocation;
    frFontHeader_t *resourceHeader;
    s32 metricByteCount;
    s32 valueByteCount;
    s8 *prop;
    void *ysht;
    GsFont_t **trans;
    struct frFontResource_t *resource;
    u32 data;
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
    frFontData_t entries[9];
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
struct frMsgInfo_t {
    s32 X;
    s32 Y;
    s32 Z;
    s8 Style;
    s8 ColNo;
    s8 Tp;
    u8 Spd;
    u8 *pMsg;
    frFontHandle_t *pFRQ;
    s32 Ofs;
    s8 FrqFlag;
    s8 RetFlag;
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

/* sdfImageSetPacket_tag.freeMode: which heap owns the uploaded pixels. */
enum SdfImageSetMem
{
    SDF_IMAGESET_MEM_IGNORE = 0,
    SDF_IMAGESET_MEM_HANDLE = 1,
    SDF_IMAGESET_MEM_CHIP = 2
};

void frSetWhiteSpaceW(s32 StyleNo, s32 Width);

extern frFontSystem_t gFT;

#define FR_FONT_BYTE_MASK 0xFF

#define FR_FONT_ENTRY_ENABLED 1

u32 frSwapColor(u32 Col);

#define FR_FONT_BYTE_SHIFT 8

#define FR_FONT_MIDDLE_BYTE_MASK 0x0000FF00

#define FR_FONT_OUTER_BYTE_SHIFT 24

s32 frConvertVal2N(u32 Val);

void frCreateDrawEnvPacket(frFontGsPacket_t *pDrawEnvPkt);

void frCreateUserFontPacket(s32 X, s32 Y, s32 Z, frTextHandle_t *pText,
                  s32 Ot);

#define FR_FONT_GLYPH_HEIGHT_SHIFT 1

void frCopyFontClutToVram(s32 GS_LocalAddr, s32 pClut);

extern void sceGsSetDefLoadImage(void *, s16, s16, s32, s32, s32, s16, s16);

extern s32 WaitSema(s32);

extern volatile s32 dds3GsSema; /* semaphore handle shared with the IOP/interrupt side; declared volatile */

extern void FlushCache(s32);

extern void sceGsExecLoadImage(void *, s32);

extern s32 sceGsSyncPath(s32, s32);

extern s32 SignalSema(s32);

void frInitFontTable(s32 GsFontNums, s32 TextNums);

extern sdfGsMemSlice_t * sdfGsMemAllocFromTail(s32 aSize, s32 aType);

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

void frDestroyFontTable(dds3ProcessID_t *id);

void frDelStyleAll(void);

extern frFontHandle_t *frDestroyFontRequestNow(frFontHandle_t *pFRQ);

extern u32 frFreeFontTable(frFontNodeHeader_t *pool);

s32 fmDestroyFontMemControl(void);

extern void sdfGsMemFree(sdfGsMemSlice_t *aHandle);

void frAddFontStyle(
    u8 StyleNo, u8 *pFontData, sdfMemHandle_t *DataMh
);

extern u32 sdfMemLock(sdfMemHandle_t *aHandle);

#define FR_FONT_TABLE_ENTRY_SHIFT 6

#define FR_FONT_WORD_BYTES 4

void frDelFontStyle(s32 StyleNo);

#define FR_FONT_FIRST_RELEASABLE_SLOT 2

extern u32 frFontLoaded[];

extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern void frLoadFontData(s32 StyleNo, const char *pFileName);

#define FR_FONT_ENTRY_CAPACITY 9

#define FR_FONT_IMAGE_WORD_COUNT 16

extern u8 frFontClutTable[];

#define FR_FONT_IMAGE_WORD_BYTES 4

#define FR_FONT_IMAGE_LAST_WORD 15

#define FR_FONT_IMAGE_BUFFER_COUNT 6

void frFlushGsFontTable(frTextHandle_t *pText);

s32 fmRetFontMem(FontMem_t *pFontArea);

frFontHandle_t *frDestroyFontRequest(frFontHandle_t *pFRQ);

s32 frEraseEffect(frFontHandle_t *pFRQ);

s32 frDestroyFontRequestAfter(frFontHandle_t *pFRQ);

extern u32 dds3GetFrameID(void);

extern frFontHandle_t *frCombineFontRequestEx(frFontHandle_t *, frFontHandle_t *, s32);

s32 frGetGsFontNum(void);

s32 frGetTextHandleNum(void);

u32 frGetFontHandleNum(void);

extern u32 frFontHandleNum[];

s32 frGetYShiftLev(s8 Style, s32 FontNo);

frFontHandle_t *frSetUserImage(GsFont_t *pFont, frFontHandle_t *pFRQ);

extern void *frAllocFontNode(frFontNodeHeader_t *pool);

extern void frInitFontHandle(frFontHandle_t *pFRQ);

void frInitTextHandle(frTextHandle_t *pText, s32 Code, s32 Style, s32 ColNo, s32 Color, s32 Tp);

extern s8 gFontBaseWidth[];

u32 frGetDefaultFontWidth(u8 Style);

u32 frGetDefaultFontHeight(u8 Style);

frFontHandle_t *frLinkUserImage(GsFont_t *pFont, frFontHandle_t *pFRQ);

frFontHandle_t *frCombineFontRequest(frFontHandle_t *pFRQ1, frFontHandle_t *pFRQ2);

GsFont_t *frTranslateUserData(s32 Width, s32 Height, void *pData, s8 DataFreeMode);


FontMem_t *fmRentFontMem(void);

#define FR_FONT_ATLAS_POSITION_SHIFT 4

#define FR_FONT_PIXEL_FORMAT 0x14

#define FR_FONT_ATLAS_DEST_SHIFT 6

extern void sdfImageSetReq(sdfImageRequest_t *parm);

u8 *frDecodeFontImage(u8 Style, s32 FontNo);

extern void *sdfChipAllocDirect(s32 aSize);

#define FR_FONT_RESOURCE_CLONE_BYTES 0x120

extern void frGetFontImagePtrs(frFontResource_t *resource,
                              frFontImage_t *segments);

extern s32 frDecodeFontRow(u8 *decodedBytes, s32 entryIndex,
                                void *resourceBase,
                                frFontImage_t *segments);

GsFont_t *frTranslateFont2GS(frTextHandle_t *pText, s32 FontNo);

u16 frGetFontWidth(s32 slotIndex);

u16 frGetFontHeight(s32 slotIndex);

extern u8 gModeFlag;

void frSetDrawSpeed(frFontHandle_t *pFRQ, s32 Speed);

#define FR_FONT_CONTEXT_BYTE_THRESHOLD 0x81

#define FR_FONT_CONTEXT_BYTE_CAP (-0x80)

void frSetFontSpace(frFontHandle_t *pFRQ, s32 Space);

u32 frGetStrWidth(frFontHandle_t *pFRQ);

void frSetFontSize(frFontHandle_t *pFRQ, s32 Width, s32 Height);

void frSetDrawPosition(frFontHandle_t *pFRQ, u32 X, u32 Y);

void frSetPriority(frFontHandle_t *pFRQ, u32 Pri);

#define FR_FONT_CONTEXT_VALUE_SHIFT 4

void frSetFontColorNo(frFontHandle_t *pFRQ, u8 ColorNo);

void frSetFontColor(frFontHandle_t *pFRQ, u32 Color);

void frEnableMode(s32 Flag);

u8 frDisableMode(u8 Flag);

void frSetFontDrawOT(u32 OtType);

extern u32 gFontDrawOT;

#define FR_FONT_FADE_OPTION 2

#define FR_FONT_FADE_VALUE_STEP 8

#define FR_FONT_FADE_Y_STEP 0x10

s32 frDrawFont(frFontHandle_t *pFRQ);

extern s32 frDrawFontEx(frFontHandle_t *pFRQ, s8 Mode);

s32 frFlipPacketBuf(dds3ProcessID_t *id);

void frSetExtDic(u8 *pExtFontData);

#define FR_FONT_TEMPORARY_SLOT 8

void frDestroyExtDic(void);

s32 frStrLen(s8 *pStr);

u32 frGetStrWidth2(frFontHandle_t *pFRQ);

#define FR_FONT_DEFAULT_CELL_SLOT_COUNT 2

extern s32 frDefaultFontHandle;

void frSetMessageDefStepY(void);

extern u32 gMessageStepY;

#define FR_FONT_DEFAULT_CURSOR_SPACING 0x14

void frSetMessageStepY(u32 StepY);


u8 *MessageNumSearch(s32 Index, frTextBank_t *pData, s32 Size);

extern void *memcpy(void *a, const void *b, u32 c);

void frMessageReturnCode(frMsgInfo_t *pMsgInfo);

#define FR_FONT_CONTEXT_CURSOR_SCALE 8

frFontHandle_t *MessageRequest(s32 X, s32 Y, u8 *pMsg, frFontHandle_t *pFRQ);

frFontHandle_t *MessageRequestEx2(s32 X, s32 Y, s32 Z, s32 Style, s32 ColNo, s32 Tp, s32 Spd, u8 *pMsg, frFontHandle_t *pFRQ);

frFontHandle_t *MessageRequestEx(s32 X, s32 Y, s32 Style, s32 ColNo, s32 Tp, s32 Spd, u8 *pMsg, frFontHandle_t *pFRQ);

frFontHandle_t *frAnalyzeMessage(frMsgInfo_t *pMsgInfo);

u32 frGetMessageResult(u32 Flag);

extern u32 gMessageRequestResult;

void frSetVariableRequest(s32 VarNo, s32 pStr);

extern s8 gMessageSpaceX;

extern u16 frMessageCode;

extern s32 frFindTagCode(u16 code);

extern s32 frFindTagCode2(u16 code);

extern frFontHandle_t *frGetNameFRQ(s8 target, frFontHandle_t *chain, s32 firstOption);

extern frFontHandle_t *frGetName1NFRQ(s8 target, frFontHandle_t *chain, s32 firstOption);

extern frFontHandle_t *frGetName2NFRQ(s8 target, frFontHandle_t *chain, s32 firstOption);

extern frFontHandle_t *frGetCNameFRQ(s8 nameIndex, s8 fontIndex, frFontHandle_t *chain, s8 firstOption);

extern s32 frAnalyzeTag(s32 code, frMsgInfo_t *mi);

extern u8 frSetFontString_style;

extern void frGetMessageCode(u32 code);

frTextHandle_t *frSetFont(u16 glyphCode, u8 fontIndex, u8 firstOption, u8 secondOption, s32 shiftFlag);

void frCreateVariableRequest(u8 VarNo, frMsgInfo_t *pMsgInfo);

extern sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

extern void *memset(void *a, s32 b, u32 c);

extern u32 strlen(const char *text);

extern void frRestoreNameImage(s8 TargetNo);

extern s32 frFreeFontNode(void *payload, frFontNodeHeader_t *pool);

extern void frGetXShiftLev(frTextHandle_t *glyph, s32 shiftIndex, s32 isFirst);

u32 frPreCalcStrWidth(s8 *pStr, s32 Style, s32 Mode);

extern sdfMemHandle_t *sdfDevLoadFileEx(const char *path, u32 *destination, s32 size);

s32 frCalcStringHash(s8 *text);

extern u8 D_0054F430[];

extern frFontHandle_t *frSetFontString(u8 *text, s8 fontIndex, s8 firstOption, s8 secondOption, frFontHandle_t *chain, s32 flag);

frFontHandle_t *frLinkFontString(u8 *pStr, s8 Style, s8 ColNo, s8 Tp, frFontHandle_t *pFRQ);

void frCreateNewHandle(frMsgInfo_t *pMsgInfo);

void frPushTagChar(frMsgInfo_t *ctx, u16 code);

s32 frGetTagArgCount(s32 tagCode);

frFontHandle_t *MessageRequestNS(s32 x, s32 y, s32 depth, s32 channel0, s32 channel1, s32 channel2, s32 channel3, u8 *encodedText);

/* Select by the low byte of slotId; enable the entry and store value + 1 as a byte. */
void frSetWhiteSpaceW(s32 StyleNo, s32 Width) {
    frFontData_t *entry = &gFT.entries[StyleNo & FR_FONT_BYTE_MASK];

    entry->prop[0] = FR_FONT_ENTRY_ENABLED;
    entry->prop[1] = Width + 1;
}

/* Reverse the four byte positions of a 32-bit word. */
u32 frSwapColor(u32 Col) {
    u32 shiftedWord = Col >> FR_FONT_BYTE_SHIFT;
    u32 lowerMiddleByte = Col & FR_FONT_MIDDLE_BYTE_MASK;
    u32 outerBytes = Col << FR_FONT_OUTER_BYTE_SHIFT;

    Col >>= FR_FONT_OUTER_BYTE_SHIFT;
    shiftedWord &= FR_FONT_MIDDLE_BYTE_MASK;
    lowerMiddleByte <<= FR_FONT_BYTE_SHIFT;
    outerBytes |= Col;
    shiftedWord |= lowerMiddleByte;
    return outerBytes | shiftedWord;
}

/* Return the zero-based highest set bit; both zero and one return zero. */
s32 frConvertVal2N(u32 Val) {
    s32 shiftCount = 0;

    if (Val == 0) {
        return 0;
    }
    do {
        Val = Val >> 1;
        shiftCount += 1;
    } while (Val != 0);
    return (shiftCount + FR_FONT_BYTE_MASK) & FR_FONT_BYTE_MASK;
}

void frCreateDrawEnvPacket(frFontGsPacket_t *pDrawEnvPkt) {
    QUAD_ZERO(pDrawEnvPkt->dma.value);
    pDrawEnvPkt->dma.words[0] = 0x70000007;
    pDrawEnvPkt->dma.words[2] = 0;
    pDrawEnvPkt->dma.words[3] = 0x50000007;
    pDrawEnvPkt->gifTag = 0x1000000000008006ULL;
    pDrawEnvPkt->gifRegisters = 0xE;
    pDrawEnvPkt->writes[0].value = 0;
    pDrawEnvPkt->writes[0].reg = 0x3F;
    pDrawEnvPkt->writes[1].value = 0x44;
    pDrawEnvPkt->writes[1].reg = 0x42;
    pDrawEnvPkt->writes[2].value = 0;
    pDrawEnvPkt->writes[2].reg = 0x49;
    pDrawEnvPkt->writes[3].value = 0x8080;
    pDrawEnvPkt->writes[3].reg = 0x3B;
    pDrawEnvPkt->writes[4].value = 0;
    pDrawEnvPkt->writes[4].reg = 0x4A;
    pDrawEnvPkt->writes[5].value = 0x517ED;
    pDrawEnvPkt->writes[5].reg = 0x47;
}

typedef struct {
    u64 uv;
    u64 rgbaq;
    u64 xyz;
} frFontSpriteCorner_t;

typedef struct {
    frFontDmaTag_t dma;
    u64 setupTag;
    u64 setupRegisters;
    frFontGsWrite_t tex0;
    frFontGsWrite_t tex1;
    frFontGsWrite_t clamp;
    u64 spriteTag;
    u64 spriteRegisters;
    frFontSpriteCorner_t corners[2];
} frFontSpritePacket_t;

/* Two direct views of the allocated DMA/GIF packet: header words and GS data. */
typedef union {
    frFontSpritePacket_t sprite;
    u32 words[36];
} frFontSpritePacketView_t;

extern u32 frFontEntryTable[];
extern sdfGrOt_t sdfGrOtTable[];
extern void *sdfGrAllocPacketBuff(u32 size);
extern void sdfDrawTagInit(sdfDrawTag_t *tag);
extern void sdfDrawTagLink(sdfDrawTag_t *tag, u32 packet);

void frCreateDrawFontPacket_20550848(s32 x, s32 y, s32 width, s32 height, u8 style,
    u32 color, u32 depth, s32 filtered, const frFontUvRect_t *uv,
    const frFontAtlas_t *atlas, s32 surface) {
    frFontSpritePacketView_t *packet;
    sdfDrawTag_t *list;
    sdfGrOt_t *drawSurface;
    u32 reversedColor;
    s32 right, bottom;
    u64 rgbaq;
    u64 z;
    u64 texBase, texWidth, texBuffer, texHeight;

    reversedColor = frSwapColor(color);
    packet = (frFontSpritePacketView_t *)sdfGrAllocPacketBuff(sizeof(frFontSpritePacketView_t));
    packet->words[0] = 0x70000008;
    packet->words[3] = 0x50000008;
    packet->sprite.tex1.value = ((u64)filtered << 5) | ((u64)filtered << 6);
    packet->sprite.setupTag = 0x10AF400000000003ULL;
    packet->sprite.setupRegisters = 0xE;
    packet->sprite.tex1.reg = 0x14;
    packet->words[1] = 0;
    packet->words[2] = 0;
    x += 0x7000;
    width <<= 4;
    y += 0x7900;
    height <<= 4;
    right = x + width;
    bottom = y + height;
    texBase = atlas->bufferBase | 0x8000000000000000ULL;
    texWidth = ((u64)atlas->widthExponent << 26) | 0x400000000ULL;
    texBuffer = ((u64)atlas->bufferWidth << 14) | 0x1400000;
    texHeight = ((u64)atlas->heightExponent << 30) | ((u64)frFontEntryTable[style] << 37);
    texBase |= texWidth;
    texBuffer |= texHeight;
    packet->sprite.tex0.reg = 6;
    packet->sprite.clamp.reg = 8;
    packet->sprite.spriteTag = 0x3400000000008002ULL;
    packet->sprite.spriteRegisters = 0x413;
    packet->sprite.tex0.value = texBase | texBuffer;
    packet->sprite.clamp.value = 0;
    rgbaq = reversedColor | 0x3F80000000000000ULL;
    z = (u64)depth << 32;
    packet->sprite.corners[0].uv = (uv->v1 << 16) | uv->u1;
    packet->sprite.corners[0].rgbaq = rgbaq;
    packet->sprite.corners[0].xyz = (u32)((right & 0xFFFF) | (bottom << 16)) | z;
    packet->sprite.corners[1].uv = (uv->v0 << 16) | uv->u0;
    packet->sprite.corners[1].rgbaq = rgbaq;
    packet->sprite.corners[1].xyz = (u32)((x & 0xFFFF) | (y << 16)) | z;
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(sizeof(sdfDrawTag_t));
    sdfDrawTagInit(list);
    sdfDrawTagLink(list, (u32)packet);
    drawSurface = &sdfGrOtTable[surface];
    drawSurface->regFunc(drawSurface, list);
}


/* Submit glyph-relative coordinates and the cache node's owned UV rectangle. */
void frCreateUserFontPacket(s32 X, s32 Y, s32 Z, frTextHandle_t *pText,
                  s32 Ot) {
    frCreateDrawFontPacket_20550848(X + pText->x, Y + pText->y,
                 pText->cellDimensions.cellWidth,
                 pText->cellDimensions.cellHeight >> FR_FONT_GLYPH_HEIGHT_SHIFT,
                 pText->renderValueOrSetupOrShade.setupBytes.firstOption, pText->parentDimensionsOrRenderWord.renderWord, Z, 1,
                 &pText->cachedItem->font_mem->uv, &gFT.atlas, Ot);
}

/* Upload an image to GS memory at the given buffer, serialized by the GS semaphore. */
void frCopyFontClutToVram(s32 GS_LocalAddr, s32 pClut) {
    u8 loadImage[0x60];

    sceGsSetDefLoadImage(loadImage, (s16)GS_LocalAddr, 1, 0, 0, 0, 8, 2);
    WaitSema(dds3GsSema);
    FlushCache(0);
    sceGsExecLoadImage(loadImage, pClut);
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

void frLoadFontData(s32 StyleNo, const char *pFileName) {
    u32 slotIndex = StyleNo & FR_FONT_BYTE_MASK;

    if (frFontLoaded[slotIndex] == 1) {
        return;
    }
    if (slotIndex == 1) {
        frAddFontStyle(1, NULL, gFT.entries[0].allocation);
    } else {
        frAddFontStyle(slotIndex, NULL, sdfDevLoadFileEx(pFileName, 0, 0));
    }
    frFontLoaded[slotIndex] = 1;
}

void frInitFontTable(s32 GsFontNums, s32 TextNums) {
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
    rowCount = GsFontNums / glyphsPerRow;
    rowTotal = rowCount;
    glyphRingCount = TextNums;
    TextNums = glyphRingCount * 4;
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
    gFT.itemPool = frAllocFontTable(0x2C, TextNums);
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
void frDestroyFontTable(dds3ProcessID_t *id) {
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
    u8 StyleNo, u8 *pFontData, sdfMemHandle_t *DataMh
) {
    frFontData_t *entry;
    s32 sectionOffset;
    u32 lookupOffset;

    if (pFontData == NULL) {
        if (DataMh != NULL) {
            pFontData = (u8 *)sdfMemLock(DataMh);
        }
    }
    entry = &gFT.entries[StyleNo];
    entry->allocation = DataMh;
    entry->resourceHeader = (frFontHeader_t *)pFontData;
    sectionOffset = entry->resourceHeader->tableOffset + (entry->resourceHeader->tableCount << FR_FONT_TABLE_ENTRY_SHIFT);
    if (entry->resourceHeader->hasExtra != 0) {
        s32 *flagSection = (s32 *)(pFontData + sectionOffset);
        s32 flagDataBytes = *flagSection;
        s32 *valueSection;
        s32 valueDataBytes;
        s32 flagSectionBytes;
        s32 valueSectionBytes;

        entry->prop = (s8 *)(flagSection + 1);
        entry->metricByteCount = flagDataBytes;
        flagSectionBytes = flagDataBytes + FR_FONT_WORD_BYTES;
        sectionOffset += flagSectionBytes;
        valueSection = (s32 *)(pFontData + sectionOffset);
        valueDataBytes = *valueSection;
        entry->ysht = valueSection + 1;
        entry->valueByteCount = valueDataBytes;
        valueSectionBytes = valueDataBytes + FR_FONT_WORD_BYTES;
        sectionOffset += valueSectionBytes;
    } else {
        entry->prop = NULL;
        entry->ysht = NULL;
        entry->metricByteCount = 0;
        entry->valueByteCount = 0;
    }
    entry->trans = (GsFont_t **)(pFontData + sectionOffset);
    sectionOffset += entry->resourceHeader->widthCount * FR_FONT_WORD_BYTES;
    entry->resource = (struct frFontResource_t *)(pFontData + sectionOffset);
    lookupOffset = entry->resourceHeader->lookupOffset;
    if (lookupOffset != 0) {
        entry->data = (u32)(pFontData + lookupOffset);
    } else {
        entry->data = 0;
    }
}

/* Slots zero/one are not freed here; clear the data pointer only when an allocation exists. */
void frDelFontStyle(s32 StyleNo) {
    u32 slotIndex = StyleNo & FR_FONT_BYTE_MASK;
    frFontData_t *entry;

    if (slotIndex < FR_FONT_FIRST_RELEASABLE_SLOT) {
        return;
    }
    frFontLoaded[slotIndex] = 0;
    entry = &gFT.entries[slotIndex];
    if (entry->allocation != NULL) {
        sdfMemFreeDirect(entry->allocation);
        entry->resource = NULL;
    }
    entry->trans = NULL;
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

void frFlushGsFontTable(frTextHandle_t *pText) {
    GsFont_t *cachedItem = pText->cachedItem;

    if (cachedItem != NULL) {
        if (cachedItem->used_count == 0) {
            gFT.entries[pText->renderValueOrSetupOrShade.setupBytes.fontIndex].trans[cachedItem->font_no] = NULL;
            fmRetFontMem(cachedItem->font_mem);
            gFT.cachedItemCount--;
        }
    }
}

/* Retain the chain if any fade value changed; otherwise release it and return NULL. */
frFontHandle_t *frDestroyFontRequest(frFontHandle_t *pFRQ) {
    if (frEraseEffect(pFRQ) != 0) {
        return pFRQ;
    }
    return frDestroyFontRequestNow(pFRQ);
}

frFontHandle_t *frDestroyFontRequestNow(frFontHandle_t *pFRQ) {
    if (pFRQ == NULL) {
        return NULL;
    }
    do {
        frTextHandle_t *childGlyph = pFRQ->firstChild;
        frFontHandle_t *previousGlyph;

        while (childGlyph != NULL) {
            frTextHandle_t *nextGlyph = childGlyph->next;

            if (childGlyph->sourceItem == NULL) {
                childGlyph->cachedItem->used_count--;
                frFlushGsFontTable(childGlyph);
            } else {
                frRestoreNameImage(*(s8 *)childGlyph->sourceItem);
            }
            frFreeFontNode(childGlyph, gFT.itemPool);
            gFT.itemCount--;
            childGlyph = nextGlyph;
        }
        previousGlyph = pFRQ->previous;
        frFreeFontNode(pFRQ, gFT.glyphPool);
        gFT.glyphCount--;
        pFRQ = previousGlyph;
    } while (pFRQ != NULL);
    return NULL;
}

/* Link without repositioning into the draw-buffer-indexed queue; return 0.
 * The low-byte buffer index is used directly, without a two-slot bounds check. */
s32 frDestroyFontRequestAfter(frFontHandle_t *pFRQ) {
    frFontHandle_t **queueSlot = &gFT.glyphSlots[dds3GetFrameID() & 0xFF];

    *queueSlot = frCombineFontRequestEx(*queueSlot, pFRQ, 0);
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

s32 frGetYShiftLev(s8 Style, s32 FontNo) {
    return 0x18;
}

INCLUDE_ASM(const s32, "interface/frFont", frGetXShiftLev);

/* Create a glyph wrapper for source and append it to an optional destination chain. */
frFontHandle_t *frSetUserImage(GsFont_t *pFont, frFontHandle_t *pFRQ) {
    frTextHandle_t *glyph;
    frTextHandle_t *previous;

    if (pFRQ == NULL) {
        pFRQ = frAllocFontNode(gFT.glyphPool);
        gFT.glyphCount++;
        frInitFontHandle(pFRQ);
    }
    glyph = frAllocFontNode(gFT.itemPool);
    previous = pFRQ->lastChild;
    gFT.itemCount++;
    frInitTextHandle(glyph, 0, 0, 0, 0xA09DC300, 0);
    if (previous == NULL) {
        pFRQ->firstChild = glyph;
    } else {
        previous->next = glyph;
    }
    glyph->sourceItem = pFont;
    glyph->advance = gFontBaseWidth[0];
    glyph->cellDimensions.cellWidth = frGetDefaultFontWidth(0);
    glyph->cellDimensions.cellHeight = frGetDefaultFontHeight(0);
    glyph->previous = previous;
    pFRQ->lastChild = glyph;
    pFRQ->childCount++;
    pFRQ->advance += glyph->advance;
    pFRQ->parentDimensionsOrRenderWord.parentDimensions.cellAdvance = glyph->cellDimensions.cellWidth;
    pFRQ->parentDimensionsOrRenderWord.parentDimensions.cellHeight = glyph->cellDimensions.cellHeight;
    return pFRQ;
}

/* Append a source-glyph reference wrapper, not a deep copy of source storage.
 * If wrapper creation returns NULL, preserve the original destination. */
frFontHandle_t *frLinkUserImage(GsFont_t *pFont, frFontHandle_t *pFRQ) {
    frFontHandle_t *referenceChain = frSetUserImage(pFont, 0);

    if (referenceChain == NULL) {
        return pFRQ;
    }
    return frCombineFontRequest(pFRQ, referenceChain);
}

/* Retain one atlas item and submit its pixel upload with the requested ownership
 * mode. Coordinates are converted from sixteenths; pool exhaustion is unchecked. */
GsFont_t *frTranslateUserData(s32 Width, s32 Height, void *pData, s8 DataFreeMode) {
    s32 imagePosition[2];
    sdfImageRequest_t uploadRequest;
    FontMem_t *atlasNode;
    GsFont_t *cachedItem;

    atlasNode = fmRentFontMem();
    cachedItem = atlasNode->user_data;
    /* Convert fixed-point atlas placement to integer image coordinates. */
    imagePosition[0] = atlasNode->uv.u0 >> FR_FONT_ATLAS_POSITION_SHIFT;
    imagePosition[1] = atlasNode->uv.v0 >> FR_FONT_ATLAS_POSITION_SHIFT;
    cachedItem->used_count = 1;
    cachedItem->font_mem = atlasNode;
    if (DataFreeMode == 0) {
        uploadRequest.allocationMode = SDF_IMAGESET_MEM_IGNORE;
    } else {
        uploadRequest.allocationMode = SDF_IMAGESET_MEM_CHIP;
    }
    uploadRequest.pixels = pData;
    uploadRequest.format = FR_FONT_PIXEL_FORMAT;
    uploadRequest.width = Width;
    uploadRequest.height = Height;
    uploadRequest.bufferWidth = gFT.atlas.width;
    uploadRequest.destination = gFT.atlas.bufferBase << FR_FONT_ATLAS_DEST_SHIFT;
    uploadRequest.x = imagePosition[0];
    uploadRequest.y = imagePosition[1];
    sdfImageSetReq(&uploadRequest);
    return cachedItem;
}

/* Decode one packed font-resource entry into a fixed-size clone.
 * Neither allocation nor entry index is checked. */
u8 *frDecodeFontImage(u8 Style, s32 FontNo) {
    frFontData_t *entry = &gFT.entries[Style];
    u8 *clonedResource = sdfChipAllocDirect(FR_FONT_RESOURCE_CLONE_BYTES);
    frFontImage_t segments;

    frGetFontImagePtrs(entry->resource, &segments);
    frDecodeFontRow(clonedResource, FontNo, entry->resource, &segments);
    return clonedResource;
}

/* Retain a cached table item, or create/upload a new one and cache it.
 * New records start with one reference; itemIndex is not bounds-checked. */
GsFont_t *frTranslateFont2GS(frTextHandle_t *pText, s32 FontNo) {
    frFontData_t *entry = &gFT.entries[pText->renderValueOrSetupOrShade.setupBytes.fontIndex];
    GsFont_t *cachedItem = entry->trans[FontNo];
    u8 *clonedResource;

    if (cachedItem != NULL) {
        cachedItem->used_count++;
        return cachedItem;
    }
    clonedResource = frDecodeFontImage(pText->renderValueOrSetupOrShade.setupBytes.fontIndex, FontNo);
    cachedItem = frTranslateUserData(frGetFontWidth(pText->renderValueOrSetupOrShade.setupBytes.fontIndex), frGetFontHeight(pText->renderValueOrSetupOrShade.setupBytes.fontIndex), clonedResource, 1);
    cachedItem->font_no = FontNo;
    entry->trans[FontNo] = cachedItem;
    gFT.cachedItemCount++;
    return cachedItem;
}

/* Store the font/options, narrowed glyph code and high flag bits, then clear
 * positioning/links. Other glyph fields are deliberately not fully reset here. */
void frInitTextHandle(frTextHandle_t *pText, s32 Code, s32 Style, s32 ColNo, s32 Color, s32 Tp) {
    pText->renderValueOrSetupOrShade.setupBytes.fontIndex = Style;
    pText->renderValueOrSetupOrShade.setupBytes.firstOption = ColNo;
    pText->renderValueOrSetupOrShade.setupBytes.secondOption = Tp;
    pText->glyphCodeOrContext.glyphCode = Code;
    pText->parentDimensionsOrRenderWord.renderWord = Color & ~FR_FONT_BYTE_MASK;
    pText->renderValueOrSetupOrShade.setupBytes.sharedFlags = gModeFlag;
    pText->x = 0;
    pText->y = 0;
    pText->advance = 0;
    pText->drawCount = 0;
    pText->cachedItem = NULL;
    pText->sourceItem = NULL;
    pText->previous = NULL;
    pText->next = NULL;
}

void frInitFontHandle(frFontHandle_t *pFRQ) {
    pFRQ->glyphCodeOrContext.byteRoles.encodedContextByte = FR_FONT_CONTEXT_BYTE_CAP;
    pFRQ->x = 0;
    pFRQ->y = 0;
    pFRQ->glyphCodeOrContext.byteRoles.spacing = 0;
    pFRQ->advance = 0;
    pFRQ->renderValueOrSetupOrShade.renderValue = 0;
    pFRQ->previous = NULL;
    pFRQ->next = NULL;
    pFRQ->chainHead = pFRQ;
    pFRQ->firstChild = NULL;
    pFRQ->lastChild = NULL;
    pFRQ->childCount = 0;
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

frFontHandle_t *frLinkFontString(u8 *pStr, s8 Style, s8 ColNo, s8 Tp, frFontHandle_t *pFRQ) {
    frFontHandle_t *chain = frSetFontString(pStr, Style, ColNo, Tp, NULL, 0);

    if (chain == NULL) {
        return pFRQ;
    }
    return frCombineFontRequest(pFRQ, chain);
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
void frSetDrawSpeed(frFontHandle_t *pFRQ, s32 Speed) {
    s32 encodedValue = (Speed & FR_FONT_BYTE_MASK) * 2;

    if (encodedValue >= FR_FONT_CONTEXT_BYTE_THRESHOLD) {
        pFRQ->glyphCodeOrContext.byteRoles.encodedContextByte = FR_FONT_CONTEXT_BYTE_CAP;
    } else {
        pFRQ->glyphCodeOrContext.byteRoles.encodedContextByte = encodedValue;
    }
}

/* Store the signed spacing byte and refresh the cached child-chain advance. */
void frSetFontSpace(frFontHandle_t *pFRQ, s32 Space) {
    pFRQ->glyphCodeOrContext.byteRoles.spacing = Space;
    pFRQ->advance = frGetStrWidth(pFRQ);
}

/* Set the initial glyph's halfword dimensions and every visited child's advance/
 * byte dimensions, then remeasure only the initial glyph. Input must be non-NULL. */
void frSetFontSize(frFontHandle_t *pFRQ, s32 Width, s32 Height) {
    frFontHandle_t *targetGlyph = pFRQ;
    frTextHandle_t *childGlyph;

    targetGlyph->parentDimensionsOrRenderWord.parentDimensions.cellAdvance = Width;
    targetGlyph->parentDimensionsOrRenderWord.parentDimensions.cellHeight = Height;
    for (; pFRQ != NULL; pFRQ = pFRQ->previous) {
        for (childGlyph = pFRQ->firstChild; childGlyph != NULL; childGlyph = childGlyph->next) {
            childGlyph->advance = Width;
            childGlyph->cellDimensions.cellWidth = Width;
            childGlyph->cellDimensions.cellHeight = Height;
        }
    }
    targetGlyph->advance = frGetStrWidth(targetGlyph);
}

/* Store the glyph position without scaling. */
void frSetDrawPosition(frFontHandle_t *pFRQ, u32 X, u32 Y) {
    pFRQ->x = X;
    pFRQ->y = Y;
}

/* Store the glyph render value in sixteenths. */
void frSetPriority(frFontHandle_t *pFRQ, u32 Pri) {
    pFRQ->renderValueOrSetupOrShade.renderValue = Pri >> FR_FONT_CONTEXT_VALUE_SHIFT;
}

/* Assign the first option byte across all visited children; NULL is a no-op. */
void frSetFontColorNo(frFontHandle_t *pFRQ, u8 ColorNo) {
    frTextHandle_t *childGlyph;

    for (; pFRQ != NULL; pFRQ = pFRQ->previous) {
        for (childGlyph = pFRQ->firstChild; childGlyph != NULL; childGlyph = childGlyph->next) {
            childGlyph->renderValueOrSetupOrShade.setupBytes.firstOption = ColorNo;
        }
    }
}

/* Replace each child's full color word across the parent glyph chain. */
void frSetFontColor(frFontHandle_t *pFRQ, u32 Color) {
    for (; pFRQ != NULL; pFRQ = pFRQ->previous) {
        frTextHandle_t *childNode;
        for (childNode = pFRQ->firstChild; childNode != NULL; childNode = childNode->next) {
            childNode->parentDimensionsOrRenderWord.renderWord = Color;
        }
    }
}

/* OR the requested mask into the shared byte, preserving implicit narrowing. */
void frEnableMode(s32 Flag) {
    Flag |= gModeFlag;
    gModeFlag = Flag;
}

/* Clear flag bits from the shared font flag byte; returns the previous value. */
u8 frDisableMode(u8 Flag) {
    u8 previousFlags = gModeFlag;

    gModeFlag = previousFlags & ~Flag;
    return previousFlags;
}

/* Replace the complete shared render-flag word used by subsequent drawing. */
void frSetFontDrawOT(u32 OtType) {
    gFontDrawOT = OtType;
}

/* Step only second-option-2 children: decrease the low byte by eight, clamp at
 * zero and move y by sixteen while changing it. Return whether any step occurred,
 * not whether the resulting chain still has a nonzero fade value. */
s32 frEraseEffect(frFontHandle_t *pFRQ) {
    frTextHandle_t *childGlyph;
    s32 didChange = 0;

    for (; pFRQ != NULL; pFRQ = pFRQ->previous) {
        for (childGlyph = pFRQ->firstChild; childGlyph != NULL; childGlyph = childGlyph->next) {
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
s32 frDrawFont(frFontHandle_t *pFRQ) {
    return frDrawFontEx(pFRQ, 0);
}

INCLUDE_ASM(const s32, "interface/frFont", frDrawFontEx);

/* Release queue slot 1 when the draw-buffer index's low byte is zero, otherwise
 * slot 0; return 0. This is buffer selection, not a current-font selection. */
s32 frFlipPacketBuf(dds3ProcessID_t *id) {
    s32 queueIndex = (dds3GetFrameID() & FR_FONT_BYTE_MASK) == 0;

    gFT.glyphSlots[queueIndex] = frDestroyFontRequestNow(gFT.glyphSlots[queueIndex]);
    return 0;
}

/* Link chains with the renderer's relative-positioning option enabled. */
frFontHandle_t *frCombineFontRequest(frFontHandle_t *pFRQ1, frFontHandle_t *pFRQ2) {
    return frCombineFontRequestEx(pFRQ1, pFRQ2, 1);
}

frFontHandle_t *frCombineFontRequestEx(frFontHandle_t *pFRQ1, frFontHandle_t *pFRQ2, s32 Mode) {
    if (pFRQ1 == NULL) {
        return pFRQ2;
    }
    if (pFRQ2 == NULL) {
        return pFRQ1;
    }
    pFRQ1->next = pFRQ2->chainHead;
    pFRQ2->chainHead->previous = pFRQ1;
    pFRQ2->chainHead = pFRQ1->chainHead;
    if (Mode == 1) {
        if ((pFRQ2->firstChild != NULL) && (pFRQ2->firstChild->glyphCodeOrContext.glyphCode == 0xD)) {
            s32 cellHeight = (s16)pFRQ1->parentDimensionsOrRenderWord.parentDimensions.cellHeight;

            pFRQ2->x = pFRQ1->x;
            pFRQ2->y = pFRQ1->y + cellHeight;
        } else {
            pFRQ2->x = pFRQ1->x + pFRQ1->advance * 16;
            pFRQ2->y = pFRQ1->y;
        }
    }
    return pFRQ2;
}

/* Bind the caller's font data into the reserved temporary entry. */
void frSetExtDic(u8 *pExtFontData) {
    frAddFontStyle(FR_FONT_TEMPORARY_SLOT, pExtFontData, 0);
}

/* Free the reserved temporary font entry. */
void frDestroyExtDic(void) {
    frDelFontStyle(FR_FONT_TEMPORARY_SLOT);
}

/* Count characters until a zero lead byte. Any negative signed lead consumes
 * two bytes without checking its trailer; input must be a valid packed stream. */
s32 frStrLen(s8 *pStr) {
    s32 characterCount = 0;

    while (*pStr != 0) {
        if (*pStr >= 0) {
            pStr++;
        } else {
            pStr += 2;
        }
        characterCount++;
    }
    return characterCount;
}

/* Sum one parent's child advances and signed spacing after every child, even
 * the last. Require a non-NULL parent; preserve the existing unsigned result. */
u32 frGetStrWidth(frFontHandle_t *pFRQ) {
    frTextHandle_t *childGlyph = pFRQ->firstChild;
    s32 totalAdvance = 0;

    if (childGlyph != NULL) {
        s8 childSpacing = pFRQ->glyphCodeOrContext.byteRoles.spacing;

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
u32 frGetStrWidth2(frFontHandle_t *pFRQ) {
    frFontHandle_t *parentGlyph;
    frTextHandle_t *childGlyph;
    s32 totalAdvance = 0;

    for (parentGlyph = pFRQ->chainHead; parentGlyph != NULL; parentGlyph = parentGlyph->next) {
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
u32 frPreCalcStrWidth(s8 *pStr, s32 Style, s32 Mode) {
    frTextHandle_t glyph;
    s32 i = 0;
    s32 totalWidth = 0;
    u32 length;

    glyph.renderValueOrSetupOrShade.setupBytes.sharedFlags = Mode;
    glyph.renderValueOrSetupOrShade.setupBytes.fontIndex = Style;
    glyph.advance = 0;
    glyph.cellDimensions.cellWidth = frGetDefaultFontWidth(Style);
    glyph.cellDimensions.cellHeight = frGetDefaultFontHeight(glyph.renderValueOrSetupOrShade.setupBytes.fontIndex);
    length = strlen((const char *)pStr);
    if (length == 0) {
        return totalWidth;
    }
    do {
        u32 code = (u8)pStr[i];
        s32 shiftIndex;

        if (code >= 0x80) {
            shiftIndex = code << FR_FONT_BYTE_SHIFT;
            i++;
            code = shiftIndex | (u8)pStr[i];
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
u32 frGetDefaultFontWidth(u8 Style) {
    s32 entryIndex = Style;

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
u32 frGetDefaultFontHeight(u8 Style) {
    s32 entryIndex = Style;

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
void frSetMessageStepY(u32 StepY) {
    gMessageStepY = StepY;
}

/* Read the packed table count, then skip the table and its trailing metadata
 * word to reach the encoded text. The native parser copies that metadata even
 * though this lookup does not use its value. */
u8 *MessageNumSearch(s32 Index, frTextBank_t *pData, s32 Size) {
    u32 offset;
    u32 tableCount;
    u32 textMetadata;

    if (Index >= pData->count) {
        return NULL;
    }
    offset = pData->entries[Index].offset + 0x38;
    tableCount = 0;
    memcpy(&tableCount, (u8 *)pData + offset, 2);
    offset += 4;
    offset += tableCount * 4;
    memcpy(&textMetadata, (u8 *)pData + offset, sizeof(textMetadata));
    offset += sizeof(textMetadata);
    return (u8 *)pData + offset;
}

void frCreateVariableRequest(u8 VarNo, frMsgInfo_t *pMsgInfo) {
    u8 encoded[3];
    u8 *text = (u8 *)gVarStr[VarNo];
    s32 length;
    u32 code;
    s32 i;
    s8 lastFontIndex;

    memset(encoded, 0, 3);
    if (text == NULL) {
        return;
    }
    length = strlen((const char *)text);
    lastFontIndex = pMsgInfo->Style;
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
        fontIndex = (u8)pMsgInfo->Style;
        if (fontIndex != lastFontIndex) {
            frFontHandle_t *chain = pMsgInfo->pFRQ;

            if (chain != NULL) {
                if (chain->firstChild != NULL) {
                    pMsgInfo->pFRQ = frLinkFontString(D_0054F430, 0, pMsgInfo->ColNo, pMsgInfo->Tp, chain);
                    chain = pMsgInfo->pFRQ;
                }
            } else {
                pMsgInfo->pFRQ = frLinkFontString(D_0054F430, 0, pMsgInfo->ColNo, pMsgInfo->Tp, NULL);
                chain = pMsgInfo->pFRQ;
            }
            frSetDrawSpeed(chain, pMsgInfo->Spd);
            lastFontIndex = fontIndex;
        }
        pMsgInfo->pFRQ = frSetFontString(encoded, fontIndex, pMsgInfo->ColNo, pMsgInfo->Tp, pMsgInfo->pFRQ, i == 0);
        frSetFontSpace(pMsgInfo->pFRQ, gMessageSpaceX);
        frGetMessageCode(code);
    }
}

void frCreateNewHandle(frMsgInfo_t *pMsgInfo) {
    frFontHandle_t *chain = pMsgInfo->pFRQ;

    if (chain != NULL) {
        if (chain->firstChild == NULL) {
            pMsgInfo->FrqFlag = 0;
        }
    }
    if (pMsgInfo->FrqFlag != 0) {
        pMsgInfo->pFRQ = frLinkFontString(D_0054F430, 0, pMsgInfo->ColNo, pMsgInfo->Tp, chain);
        frSetDrawSpeed(pMsgInfo->pFRQ, pMsgInfo->Spd);
        pMsgInfo->FrqFlag = 0;
    }
    if (pMsgInfo->RetFlag != 0) {
        frSetDrawPosition(pMsgInfo->pFRQ, pMsgInfo->X, pMsgInfo->Y);
        pMsgInfo->RetFlag = 0;
    }
}

/* Advance by eight times cursor spacing and request both context updates. */
void frMessageReturnCode(frMsgInfo_t *pMsgInfo) {
    pMsgInfo->Y += gMessageStepY * FR_FONT_CONTEXT_CURSOR_SCALE;
    pMsgInfo->FrqFlag = 1;
    pMsgInfo->RetFlag = 1;
}

void frPushTagChar(frMsgInfo_t *ctx, u16 code) {
    u8 encoded[3];

    encoded[1] = code;
    encoded[0] = (u32)code >> FR_FONT_BYTE_SHIFT;
    encoded[2] = 0;
    frCreateNewHandle(ctx);
    ctx->pFRQ = frSetFontString(encoded, ctx->Style, ctx->ColNo, ctx->Tp, ctx->pFRQ, 0);
    frSetFontSpace(ctx->pFRQ, gMessageSpaceX);
    frSetPriority(ctx->pFRQ, ctx->Z);
}

/* NON_MATCHING: 280 of 322 words differ (first at +0x44). */
#ifdef NON_MATCHING
s32 frAnalyzeTag(s32 code, frMsgInfo_t *mi) {
    s32 *offset = &mi->Ofs;
    s32 argCount = code & 0xF;
    s32 pos = *offset;
    u8 *text = mi->pMsg;
    s8 firstOption = mi->ColNo;

    code = (code << FR_FONT_BYTE_SHIFT) | text[pos];
    pos++;
    *offset = pos;
    switch (code) {
    case 0xF206:
        mi->Style = text[pos] - 1;
        *offset = pos + 2;
        break;
    case 0xF202:
        mi->ColNo = text[pos] - 1;
        *offset = pos + 2;
        break;
    case 0xF209:
        mi->Tp = text[pos] - 1;
        *offset = pos + 2;
        break;
    case 0xF207:
        mi->Spd = text[pos] - 1;
        *offset = pos + 2;
        break;
    case 0xF203:
        if (gVarStr[text[pos] - 1] != 0) {
            frCreateNewHandle(mi);
            frCreateVariableRequest(mi->pMsg[*offset] - 1, mi);
        }
        *offset += 2;
        break;
    case 0xF20A: {
        s32 nameIndex;

        *offset = pos + 2;
        nameIndex = text[pos] - 1;
        frCreateNewHandle(mi);
        mi->pFRQ = frGetNameFRQ(nameIndex, mi->pFRQ, firstOption);
        frSetFontSpace(mi->pFRQ, gMessageSpaceX);
        break;
    }
    case 0xF20B: {
        s32 nameIndex;

        *offset = pos + 2;
        nameIndex = text[pos] - 1;
        frCreateNewHandle(mi);
        mi->pFRQ = frGetName1NFRQ(nameIndex, mi->pFRQ, firstOption);
        frSetFontSpace(mi->pFRQ, gMessageSpaceX);
        break;
    }
    case 0xF20C: {
        s32 nameIndex;

        *offset = pos + 2;
        nameIndex = text[pos] - 1;
        frCreateNewHandle(mi);
        mi->pFRQ = frGetName2NFRQ(nameIndex, mi->pFRQ, firstOption);
        frSetFontSpace(mi->pFRQ, gMessageSpaceX);
        break;
    }
    case 0xF20D: {
        s32 nameIndex;

        *offset = pos + 2;
        nameIndex = text[pos] - 1;
        frCreateNewHandle(mi);
        mi->pFRQ = frGetCNameFRQ(nameIndex, mi->Style, mi->pFRQ, firstOption);
        frEnableMode(1);
        frSetFontSpace(mi->pFRQ, gMessageSpaceX);
        frSetDrawSpeed(mi->pFRQ, mi->Spd);
        break;
    }
    case 0xF20E:
        *offset = pos + 1;
        break;
    case 0xF10F:
        if ((gModeFlag & 4) == 0) {
            frMessageReturnCode(mi);
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
        frPushTagChar(mi, tagChar);
        break;
    }
    case 0xF114: {
        s32 tagChar = 0x80E5;

        if (frFindTagCode(frMessageCode) < 0) {
            tagChar = 0x80E4;
        }
        frPushTagChar(mi, tagChar);
        break;
    }
    case 0xF115: {
        s32 tagChar = 0x80E1;

        if (frFindTagCode(frMessageCode) < 0) {
            tagChar = 0x80E0;
        }
        frPushTagChar(mi, tagChar);
        break;
    }
    case 0xF116:
        if (frFindTagCode(frMessageCode) < 0) {
            frPushTagChar(mi, 0x80E2);
        }
        break;
    case 0xF117:
        if (frFindTagCode(frMessageCode) < 0) {
            frPushTagChar(mi, 0x80E7);
        } else {
            frPushTagChar(mi, 0x80E6);
        }
        break;
    case 0xF118:
        if (frFindTagCode(frMessageCode) < 0) {
            if (frFindTagCode2(frMessageCode) < 0) {
                frPushTagChar(mi, 0x80E9);
            }
        }
        frPushTagChar(mi, 0x80E8);
        break;
    default:
        mi->Ofs = (argCount - 1) * 2 + mi->Ofs;
        break;
    }
    if (mi->FrqFlag == 0) {
        mi->FrqFlag = 1;
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
frFontHandle_t *frAnalyzeMessage(frMsgInfo_t *pMsgInfo) {
    u8 encoded[3];
    s32 isFirst = 1;
    s32 *offset;

    if (((pMsgInfo->pMsg[0] & 0xF0) != 0xF0) && (pMsgInfo->pMsg[1] == 8)) {
        pMsgInfo->Ofs += 4;
    }
    gMessageRequestResult &= ~1;
    gMessageRequestResult &= ~2;
    gMessageRequestResult &= ~4;
    frEnableMode(1);
    offset = &pMsgInfo->Ofs;
    while (1) {
        s32 code = pMsgInfo->pMsg[*offset];

        (*offset)++;
        if (code == 0) {
            break;
        }
        if ((code & 0xF0) == 0xF0) {
            if (frAnalyzeTag(code, pMsgInfo) != 0) {
                break;
            }
            continue;
        }
        if (code == 0xA) {
            if ((gModeFlag & 4) != 0) {
                frMessageReturnCode(pMsgInfo);
            }
            isFirst = 1;
            continue;
        }
        frCreateNewHandle(pMsgInfo);
        if (code < 0x80) {
            encoded[0] = code;
            encoded[1] = 0;
            pMsgInfo->pFRQ = frSetFontString(encoded, pMsgInfo->Style, pMsgInfo->ColNo, pMsgInfo->Tp, pMsgInfo->pFRQ, isFirst);
        } else {
            code = (code << FR_FONT_BYTE_SHIFT) | pMsgInfo->pMsg[(*offset)++];
            if (code <= 0xC07F) {
                encoded[0] = (u32)code >> FR_FONT_BYTE_SHIFT;
                encoded[1] = code;
                encoded[2] = 0;
                pMsgInfo->pFRQ = frSetFontString(encoded, pMsgInfo->Style, pMsgInfo->ColNo, pMsgInfo->Tp, pMsgInfo->pFRQ, isFirst);
            } else {
                code -= 0x4000;
                encoded[0] = (u32)code >> FR_FONT_BYTE_SHIFT;
                encoded[1] = code;
                encoded[2] = 0;
                pMsgInfo->pFRQ = frSetFontString(encoded, FR_FONT_TEMPORARY_SLOT, pMsgInfo->ColNo, pMsgInfo->Tp, pMsgInfo->pFRQ, isFirst);
            }
            frGetMessageCode(code & 0xFFFF);
        }
        isFirst = 0;
        frSetFontSpace(pMsgInfo->pFRQ, gMessageSpaceX);
        frSetPriority(pMsgInfo->pFRQ, pMsgInfo->Z);
    }
    frDisableMode(1);
    frEnableMode(2);
    return pMsgInfo->pFRQ;
}
#else
INCLUDE_ASM(const s32, "interface/frFont", frAnalyzeMessage);
#endif

frFontHandle_t *MessageRequest(s32 X, s32 Y, u8 *pMsg, frFontHandle_t *pFRQ) {
    return MessageRequestEx2(X, Y, 0, 0, 0, 0, 0x80, pMsg, pFRQ);
}

frFontHandle_t *MessageRequestEx(s32 X, s32 Y, s32 Style, s32 ColNo, s32 Tp, s32 Spd, u8 *pMsg, frFontHandle_t *pFRQ) {
    return MessageRequestEx2(X, Y, 0, Style & 0xFF, ColNo & 0xFF, Tp & 0xFF, Spd & 0xFF, pMsg, pFRQ);
}

frFontHandle_t *MessageRequestEx2(s32 X, s32 Y, s32 Z, s32 Style, s32 ColNo, s32 Tp, s32 Spd, u8 *pMsg, frFontHandle_t *pFRQ) {
    frMsgInfo_t args;

    args.X = X;
    args.Y = Y;
    args.Z = Z << 4;
    args.Style = Style;
    args.ColNo = ColNo;
    args.Tp = Tp;
    args.Spd = Spd;
    args.pMsg = pMsg;
    args.pFRQ = pFRQ;
    args.Ofs = 0;
    args.FrqFlag = 1;
    args.RetFlag = 1;
    return frAnalyzeMessage(&args);
}

frFontHandle_t *MessageRequestNS(s32 x, s32 y, s32 depth, s32 channel0, s32 channel1, s32 channel2, s32 channel3, u8 *encodedText) {
    s8 savedSpacing = gMessageSpaceX;
    frFontHandle_t *chain = MessageRequestEx2(x, y, depth, channel0 & 0xFF, channel1 & 0xFF, channel2 & 0xFF, channel3 & 0xFF, encodedText, NULL);

    gMessageSpaceX = savedSpacing;
    return chain;
}

u32 frGetMessageResult(u32 Flag) {
    return gMessageRequestResult & Flag;
}

/* Store an unchecked replacement-text address word in the font's indexed table. */
void frSetVariableRequest(s32 VarNo, s32 pStr) {
    gVarStr[VarNo] = pStr;
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

