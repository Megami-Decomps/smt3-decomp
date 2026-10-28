#include "common.h"
#include "kernel.h"
#include "nme.h"
#include "font.h"

/* Name-entry work is 0x51C8 bytes; its sequence flag is the first byte. */
typedef struct {
    u8 sequenceFlags;
    u8 reserved01[0x51C7];
} nmeCalcWork_t;

extern nmeCalcWork_t *D_00552024;
extern void *dds3GetProcessData(dds3ProcessID_t *id);
extern s32 dds3FadeCheck(void);
extern void cmpUpdateTerminate(void *work);

typedef struct {
    void *res;
    void *entries;
} nmeSpriteTable_t;

/* This descriptor is in .data, outside the GP-relative range. */
extern nmeSpriteTable_t GBWK_2 __attribute__((section(".data")));
extern nmeWork_t *namelist_buttonAnim_end;
extern void frSetFontColor(frFontHandle_t *font, u32 color);
extern void fclDrawSpriteRes(s32 x, s32 y, s32 z, s32 *clip, s32 page, s32 spriteId,
                             s32 *texList, nmeSpriteTable_t *table, s32 ot);

s32 nmeCalc(dds3ProcessID_t *id)
{
    D_00552024 = dds3GetProcessData(id);
    if (dds3FadeCheck() != 0) {
        return 0;
    }
    if (D_00552024->sequenceFlags & 0x10) {
        cmpUpdateTerminate(D_00552024);
        return -1;
    }
    return 0;
}

void nmeDrawCharRow(s32 x, s32 y)
{
    nmeSpriteTable_t *table = &GBWK_2;
    s32 ot = 0x47;

    fclDrawSpriteRes(x, y, 0, 0, 3, 0x12,
                     (s32 *)namelist_buttonAnim_end->textures, table, ot);
    fclDrawSpriteRes(x, y, 0, 0, 3, 0x13,
                     (s32 *)namelist_buttonAnim_end->textures, table, ot);
    fclDrawSpriteRes(x, y, 0, 0, 3, 0x14,
                     (s32 *)namelist_buttonAnim_end->textures, table, ot);
}

void nmeDrawSymbolRow(s32 x, s32 y)
{
    nmeSpriteTable_t *table = &GBWK_2;
    s32 ot = 0x47;

    fclDrawSpriteRes(x, y, 0, 0, 3, 0x12,
                     (s32 *)namelist_buttonAnim_end->textures, table, ot);
    fclDrawSpriteRes(x, y, 0, 0, 3, 0x1B,
                     (s32 *)namelist_buttonAnim_end->textures, table, ot);
    fclDrawSpriteRes(x, y, 0, 0, 3, 0x1C,
                     (s32 *)namelist_buttonAnim_end->textures, table, ot);
}

INCLUDE_ASM(const s32, "nme/Nme_Calc", nmeDrawNameText);

INCLUDE_ASM(const s32, "nme/Nme_Calc", nmeDrawNameString);

void nmeDrawEditIcons(void)
{
    s32 colors[4];
    nmeSpriteTable_t *table = &GBWK_2;

    colors[0] = 0x80808079;
    colors[1] = 0x80808079;
    colors[2] = 0x80808079;
    colors[3] = 0x80808079;

    fclDrawSpriteRes(0, 0, 0x10, colors, 3, 0x19,
                     (s32 *)namelist_buttonAnim_end->textures, table, 0x47);
    fclDrawSpriteRes(0, 0, 0x10, colors, 3, 0x1A,
                     (s32 *)namelist_buttonAnim_end->textures, table, 0x47);
}

void nmeDrawTextBox(s32 x, s32 y, s32 z)
{
    s32 colors[4];

    colors[0] = 0x45202680;
    colors[1] = 0;
    colors[2] = 0x45202680;
    colors[3] = 0;
    cmpDrawBoxGDefault(x, y, z, 0x4F, 0x16, colors, 0x47);
}

INCLUDE_ASM(const s32, "nme/Nme_Calc", nmeDrawColoredText);

INCLUDE_ASM(const s32, "nme/Nme_Calc", nmeDrawButtonGuide);

INCLUDE_ASM(const s32, "nme/Nme_Calc", nmeDrawPanelSprites);

INCLUDE_ASM(const s32, "nme/Nme_Calc", nmeDrawNamePanel);

void nmeDrawCharCell(s32 x, s32 y, u16 index, s8 row, frFontHandle_t *font)
{
    nmeWork_t *work = namelist_buttonAnim_end;
    s32 drawX = x;
    s32 drawY = y;
    s32 modeSelection;
    s32 spriteId;

    if ((s8)work->unk1B != 0) {
        goto done;
    }
    if (work->primaryCursorActive != 0) {
        goto done;
    }
    if (work->alternateCursorActive != 0) {
        goto done;
    }
    modeSelection = work->modeCursor.unk_22 + (u8)work->modeCursor.unk_24;
    spriteId = (modeSelection == 3) ? 0x48 : 0x47;
    if (index != work->inputCursor.unk_22 + (u8)work->inputCursor.unk_24) {
        goto done;
    }
    if (row != work->nameCursor.unk_22 + (u8)work->nameCursor.unk_24) {
        goto done;
    }
    fclDrawSpriteRes(drawX << 4, drawY << 3, 0, 0, 0, spriteId,
                     (s32 *)work->textures, &GBWK_2, 0x47);
    frSetFontColor(font, 0xFFD19280);
done:
    return;
}

INCLUDE_ASM(const s32, "nme/Nme_Calc", nmeDrawCursorAt);

void nmeDrawCursor(void)
{
    nmeDrawCursorAt(0x29, 0xDA, 0, 0, 0x47);
}

void nmeDrawTabSprite(s32 x, s32 y, s32 unused)
{
    nmeSpriteTable_t *table = &GBWK_2;
    s32 ot = 0x47;

    fclDrawSpriteRes(x, y, 0, 0, 3, 0xF,
                     (s32 *)namelist_buttonAnim_end->textures, table, ot);
    fclDrawSpriteRes(x, y, 0, 0, 3, 0x10,
                     (s32 *)namelist_buttonAnim_end->textures, table, ot);
    fclDrawSpriteRes(x, y, 0, 0, 3, 0x11,
                     (s32 *)namelist_buttonAnim_end->textures, table, ot);
}

void nmeDrawModeTabs(s8 mode)
{
    nmeDrawTabSprite(0x540, 0x3D8, 0);
    switch (mode) {
    case 0:
    case 1:
        fclDrawSpriteRes(0, 0, 0, 0, 3, 0x15,
                         (s32 *)namelist_buttonAnim_end->textures, &GBWK_2, 0x47);
        break;
    case 2:
        fclDrawSpriteRes(0, 0, 0, 0, 3, 0x16,
                         (s32 *)namelist_buttonAnim_end->textures, &GBWK_2, 0x47);
        break;
    default:
        break;
    }
}

INCLUDE_RODATA(const s32, "nme/Nme_Calc", D_005487F8);

INCLUDE_RODATA(const s32, "nme/Nme_Calc", D_00548808);

INCLUDE_RODATA(const s32, "nme/Nme_Calc", D_00548818);

INCLUDE_RODATA(const s32, "nme/Nme_Calc", D_00548828);

