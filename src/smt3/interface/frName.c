#include "common.h"
#include "font.h"
#include "libsdf.h"
#include "kernel.h"

typedef struct
{
    u8 reserved0000[0x1D7C];
    u8 nameBitmap[0x4800];
    u8 nameCounts[4][2];
    u8 nameGlyphCodes[4][16][2];
    u8 cNames[4][20];
    u8 cNameCounts[4];
} frNameGlobalWork_t;

extern frNameGlobalWork_t *frNameGlobalWork asm("DDS3_GBWK");
extern s32 D_0056BD50[4];
extern const char *D_003263B0[4];
extern void frAllocNameData(void);

extern void frTransNameImage(s8 target, s8 type);
extern frFontHandle_t *frCreateNameHandle(s8 target, s8 glyph, frFontHandle_t *chain);

void frInitNameData(void)
{
    char name[3] = "\x80\x80";
    s32 i;
    s8 nameLength;
    s8 secondLength;
    s8 cNameLength;

    for (i = 3; i >= 0; i--) {
        D_0056BD50[i] = 0;
    }
    memset(frNameGlobalWork->nameBitmap, 0x11, sizeof(frNameGlobalWork->nameBitmap));
    frAllocNameData();

    i = 0;
    do {
        nameLength = (s8)strlen(name);
        memcpy(frNameGlobalWork->nameGlyphCodes[i][0], name, nameLength);
        frNameGlobalWork->nameCounts[i][0] = nameLength / 2;
        secondLength = (s8)strlen(name);
        memcpy((u8 *)frNameGlobalWork->nameGlyphCodes[i][0] + nameLength,
               name, secondLength);
        frNameGlobalWork->nameCounts[i][1] = secondLength / 2;
        cNameLength = (s8)strlen(D_003263B0[i]);
        memcpy(frNameGlobalWork->cNames[i], D_003263B0[i], cNameLength);
        frNameGlobalWork->cNameCounts[i] = cNameLength / 2;
        i++;
    } while (i <= 0);
}

INCLUDE_ASM(const s32, "interface/frName", frSetNameData);

INCLUDE_ASM(const s32, "interface/frName", frTransNameImage);

extern GsFont_t *pFont[4][16];
extern void fmRetFontMem(FontMem_t *memory);

/* NON_MATCHING: 12 of 54 words differ, exact size; count/counter setup ordering. */
#ifdef NON_MATCHING
void frRestoreNameImage(s8 target)
{
    s32 index;
    s32 count = frNameGlobalWork->nameCounts[target][0] +
                frNameGlobalWork->nameCounts[target][1];

    if (D_0056BD50[target] != 0) {
        if (--D_0056BD50[target] == 0) {
            for (index = 0; index < count; index++) {
                if (pFont[target][index] != NULL) {
                    fmRetFontMem(pFont[target][index]->font_mem);
                }
            }
            printf("NAME[%d] : Restore. \n", target);
        }
    }
}
#else
INCLUDE_ASM(const s32, "interface/frName", frRestoreNameImage);
#endif

INCLUDE_ASM(const s32, "interface/frName", frCreateNameHandle);

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

extern void frEnableMode(s32 flag);
extern u8 frDisableMode(u8 flag);
extern frFontHandle_t *frLinkFontString(u8 *text, s8 style, s8 color, s8 type, frFontHandle_t *chain);
extern void frSetFontSpace(frFontHandle_t *font, s32 spacing);

/* NON_MATCHING: 4 of 86 words differ, exact size; empty-name loop setup. */
#ifdef NON_MATCHING
frFontHandle_t *frGetCNameFRQ(s8 target, s8 style, frFontHandle_t *chain, s32 firstOption)
{
    u8 text[frNameGlobalWork->cNameCounts[target] * 2 + 1];
    u8 *out = text;
    s32 length = 0;
    s32 i;
    frFontHandle_t *font;

    for (i = 0; i < frNameGlobalWork->cNameCounts[target]; i++) {
        u8 first = frNameGlobalWork->cNames[target][i * 2];
        if (first == 0) {
            *out = frNameGlobalWork->cNames[target][i * 2 + 1];
        } else {
            *out++ = first;
            length++;
            *out = frNameGlobalWork->cNames[target][i * 2 + 1];
        }
        out++;
        length++;
    }
    text[length] = 0;
    frEnableMode(1);
    font = frLinkFontString(text, style, firstOption, 0, chain);
    frDisableMode(1);
    frSetFontSpace(font, 4);
    return font;
}
#else
INCLUDE_ASM(const s32, "interface/frName", frGetCNameFRQ);
#endif

INCLUDE_SDATA(const s32, "interface/frName", D_0054F448);

INCLUDE_SDATA(const s32, "interface/frName", D_0054F44C);

INCLUDE_SDATA(const s32, "interface/frName", frHelpPosX);

INCLUDE_SDATA(const s32, "interface/frName", frHelpPosY);

INCLUDE_SDATA(const s32, "interface/frName", D_0054F458);

INCLUDE_SDATA(const s32, "interface/frName", frTestFontHandle);

INCLUDE_SDATA(const s32, "interface/frName", D_0054F460);

INCLUDE_SDATA(const s32, "interface/frName", D_0054F464);

INCLUDE_SDATA(const s32, "interface/frName", D_0054F468);

INCLUDE_SDATA(const s32, "interface/frName", D_0054F46C);

INCLUDE_SDATA(const s32, "interface/frName", D_0054F470);

INCLUDE_SDATA(const s32, "interface/frName", D_0054F478);

