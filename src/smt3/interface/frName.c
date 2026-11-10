#include "common.h"
#include "font.h"
#include "libsdf.h"
#include "kernel.h"
#include "sdk_libc.h"

typedef struct
{
    u8 reserved0000[0x1D7C];
    u8 nameBitmap[4][16][0x120];
    u8 nameCounts[4][2];
    u8 nameGlyphCodes[64][2];
    u8 cNames[4][20];
    u8 cNameCounts[4];
} frNameGlobalWork_t;

extern frNameGlobalWork_t *frNameGlobalWork FAKEMATCH_ASM_NAME("DDS3_GBWK");
extern s32 frNameRefCount[4];
extern const char *frCNameTable[4];
extern void frAllocNameData(void);

extern void frTransNameImage(s8 target, s8 type);
extern void frSetNameData(s8 target, s8 type, s8 glyphCount, u8 *imageData);
extern frFontHandle_t *frCreateNameHandle(s8 target, s8 glyph, frFontHandle_t *chain);

extern u32 strlen(const char *text);
extern void *memcpy(void *a, const void *b, size_t c);
extern void *memset(void *a, s32 b, size_t c);

void frInitNameData(void)
{
    char name[3] = "\x80\x80";
    s32 i;
    s8 nameLength;
    s8 secondLength;
    s8 cNameLength;

    for (i = 3; i >= 0; i--) {
        frNameRefCount[i] = 0;
    }
    memset(frNameGlobalWork->nameBitmap, 0x11, sizeof(frNameGlobalWork->nameBitmap));
    frAllocNameData();

    i = 0;
    do {
        nameLength = (s8)strlen(name);
        memcpy(frNameGlobalWork->nameGlyphCodes[i * 16], name, nameLength);
        frNameGlobalWork->nameCounts[i][0] = nameLength / 2;
        secondLength = (s8)strlen(name);
        memcpy((u8 *)frNameGlobalWork->nameGlyphCodes[i * 16] + nameLength,
               name, secondLength);
        frNameGlobalWork->nameCounts[i][1] = secondLength / 2;
        cNameLength = (s8)strlen(frCNameTable[i]);
        memcpy(frNameGlobalWork->cNames[i], frCNameTable[i], cNameLength);
        frNameGlobalWork->cNameCounts[i] = cNameLength / 2;
        i++;
    } while (i <= 0);
}

extern void *sdfChipAllocDirect(s32 size);
extern GsFont_t *pFont[4][16];
extern GsFont_t *frTranslateUserData(s32 width, s32 height, void *data, s8 dataFreeMode);

void frSetNameData(s8 target, s8 type, s8 glyphCount, u8 *imageData)
{
    s32 firstSlot;
    s32 oldCount;
    s32 slot;

    if (type == 0) {
        firstSlot = 0;
        oldCount = (s8)frNameGlobalWork->nameCounts[(s32)target][0];
    } else {
        firstSlot = (s8)frNameGlobalWork->nameCounts[(s32)target][0];
        oldCount = (s8)frNameGlobalWork->nameCounts[(s32)target][1];
    }

    for (slot = firstSlot; slot < firstSlot + oldCount; slot++) {
        pFont[(s32)target][slot] = NULL;
    }

    for (slot = 0; slot < glyphCount; slot++) {
        u8 *copy = sdfChipAllocDirect(0x120);
        GsFont_t *font;

        memcpy(copy, imageData + slot * 0x120, 0x120);
        font = frTranslateUserData(24, 24, copy, 1);
        pFont[(s32)target][firstSlot + slot] = font;
        font->font_no = (u16)target;
    }

    printf("NAME[%d] : Translate. \n", (s32)target);
}

void frTransNameImage(s8 target, s8 type)
{
    if (frNameRefCount[target] == 0) {
        s8 firstCount = (s8)frNameGlobalWork->nameCounts[target][0];
        u8 *images = &frNameGlobalWork->nameBitmap[(s32)target][0][0];

        frSetNameData(target, 0, firstCount, images);
        {
            frNameGlobalWork_t *work = frNameGlobalWork;
            u8 *secondImages = &work->nameBitmap[(s32)target][(s32)firstCount][0];
            s8 secondCount = (s8)work->nameCounts[target][1];

            frSetNameData(target, 1, secondCount, secondImages);
        }
    }
    if (type == 0) {
        frNameRefCount[target] += frNameGlobalWork->nameCounts[target][0];
    } else {
        frNameRefCount[target] += frNameGlobalWork->nameCounts[target][1];
    }
}

extern GsFont_t *pFont[4][16];
extern void fmRetFontMem(FontMem_t *memory);

void frRestoreNameImage(s8 target)
{
    s32 index;
    /* This initialized first-byte temporary ends at the sum, before calls.
     * The localized binding is a reconstruction aid, not original spelling;
     * count remains ordinary C across the font-release loop. */
    FAKEMATCH_ASM_REG(s32, firstCount, "$6") = frNameGlobalWork->nameCounts[target][0];
    s32 count = firstCount + frNameGlobalWork->nameCounts[target][1];

    if (frNameRefCount[target] != 0) {
        if (--frNameRefCount[target] == 0) {
            for (index = 0; index < count; index++) {
                if (pFont[target][index] != NULL) {
                    fmRetFontMem(pFont[target][index]->font_mem);
                }
            }
            printf("NAME[%d] : Restore. \n", target);
        }
    }
}

extern void frGetMessageCode(u16 code);
extern frFontHandle_t *frSetUserImage(GsFont_t *font, frFontHandle_t *chain);
extern void frGetXShiftLev(frTextHandle_t *glyph, s32 shiftIndex, s32 isFirst);
extern s32 frGetYShiftLev(s8 style, s32 fontNo);

frFontHandle_t *frCreateNameHandle(s8 target, s8 glyph, frFontHandle_t *chain)
{
    s32 slot = target * 16 + glyph;
    s32 shiftIndex = frNameGlobalWork->nameGlyphCodes[slot][0];
    frTextHandle_t *child;

    shiftIndex = (shiftIndex << 8) | frNameGlobalWork->nameGlyphCodes[slot][1];
    frGetMessageCode(shiftIndex);
    shiftIndex -= 0x20;
    chain = frSetUserImage(pFont[target][glyph], chain);
    child = chain->lastChild;
    child->glyphCodeOrContext.glyphCode = shiftIndex + 0x20;
    frGetXShiftLev(child, shiftIndex, 0);
    chain->lastChild->y = frGetYShiftLev(0, shiftIndex);
    return chain;
}

frFontHandle_t *frGetName1NFRQ(s8 target, frFontHandle_t *chain, s32 firstOption)
{
    s8 glyph = 0;
    s32 i = 0;

    frTransNameImage(target, 0);
    for (; i < frNameGlobalWork->nameCounts[target][0]; i++, glyph++) {
        chain = frCreateNameHandle(target, glyph, chain);
        chain->lastChild->renderValueOrSetupOrShade.setupBytes.firstOption = firstOption;
    }
    return chain;
}

frFontHandle_t *frGetName2NFRQ(s8 target, frFontHandle_t *chain, s32 firstOption)
{
    s8 glyph = (s8)frNameGlobalWork->nameCounts[target][0];
    s32 i = 0;

    frTransNameImage(target, 1);
    for (; i < frNameGlobalWork->nameCounts[target][1]; i++, glyph++) {
        chain = frCreateNameHandle(target, glyph, chain);
        chain->lastChild->renderValueOrSetupOrShade.setupBytes.firstOption = firstOption;
    }
    return chain;
}

extern frFontHandle_t *frGetName1NFRQ(s8 target, frFontHandle_t *chain, s32 firstOption);
extern frFontHandle_t *frGetName2NFRQ(s8 target, frFontHandle_t *chain, s32 firstOption);

frFontHandle_t *frGetNameFRQ(s8 target, frFontHandle_t *chain, s32 firstOption)
{
    frFontHandle_t *name = frGetName2NFRQ(target, chain, firstOption);
    name->advance += 8;
    name->lastChild->advance += 8;
    return frGetName1NFRQ(target, name, firstOption);
}

extern u8 frEnableMode(s32 flag);
extern u8 frDisableMode(u8 flag);
extern frFontHandle_t *frLinkFontString(u8 *text, s8 style, s8 color, s8 type, frFontHandle_t *chain);
extern void frSetFontSpace(frFontHandle_t *font, s32 spacing);

frFontHandle_t *frGetCNameFRQ(s8 target, s8 style, frFontHandle_t *chain, s32 firstOption)
{
    u8 text[frNameGlobalWork->cNameCounts[target] * 2 + 1];
    s32 length = 0;
    s32 i;
    frFontHandle_t *font;

    for (i = 0; i < frNameGlobalWork->cNameCounts[target]; i++) {
        u8 first = frNameGlobalWork->cNames[target][i * 2];
        if (first == 0) {
            text[length] = frNameGlobalWork->cNames[target][i * 2 + 1];
        } else {
            text[length] = first;
            length++;
            text[length] = frNameGlobalWork->cNames[target][i * 2 + 1];
        }
        length++;
    }
    text[length] = 0;
    frEnableMode(1);
    font = frLinkFontString(text, style, firstOption, 0, chain);
    frDisableMode(1);
    frSetFontSpace(font, 4);
    return font;
}

char frWrapEmptyString[4] = "";

s32 frHelpLastIndex = -1;

s32 frHelpPosX = 0;

s32 frHelpPosY = 0;

s32 D_0054F458 = 0;

frFontHandle_t *frTestFontHandle = NULL;

s32 frTestFontX = 0;

s32 frTestFontY = 0;

s8 frTestFontAlpha = -128;


void *frTestFontTexture = NULL;

u8 itfGaugeQuadVertexIndex[8] = { 0, 1, 4, 3, 1, 2, 5, 4 };

u8 itfGaugeQuadColorIndex[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

