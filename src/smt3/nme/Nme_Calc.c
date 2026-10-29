#include "common.h"
#include "kernel.h"
#include "nme.h"
#include "font.h"

extern nmeWork_t *D_00552024;
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
extern s32 cmpDrawBoxGDefault(s32 x, s32 y, s32 z, s32 width, s32 height,
                              s32 *colors, s32 ot);
extern void nmeDrawCursorAt(s32 x, s32 y, s32 z, s8 mode, s32 ot);
extern void fclDrawSpriteRes(s32 x, s32 y, s32 z, s32 *clip, s32 page, s32 spriteId,
                             s32 *texList, nmeSpriteTable_t *table, s32 ot);

s32 nmeCalc(dds3ProcessID_t *id)
{
    D_00552024 = dds3GetProcessData(id);
    if (dds3FadeCheck() != 0) {
        return 0;
    }
    if (D_00552024->sequence.flag & 0x10) {
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

typedef struct {
    const char *question;
    const char *yes;
    const char *no;
} nmeColoredTextStrings_t;

extern const nmeColoredTextStrings_t D_005487F8;
extern void cmpSetSkillListRows(s32 rows);
extern frFontHandle_t *frwGetSmallFontFRQ2(s32 x, s32 y, s32 priority, u32 color,
                                             const char *text, frFontHandle_t *previous);
extern void frSetDrawPosition(frFontHandle_t *request, u32 x, u32 y);
extern void frSetPriority(frFontHandle_t *request, u32 priority);
extern void frSetFontColorNo(frFontHandle_t *request, u8 colorNo);
extern s32 func_001C7E38(frFontHandle_t *request, s8 mode, u32 ot);
extern s32 frDestroyFontRequestAfter(frFontHandle_t *request);
extern u8 frDisableMode(u8 flag);

void nmeDrawColoredText(s8 selected)
{
    nmeColoredTextStrings_t text = D_005487F8;
    frFontHandle_t *request;

    cmpSetSkillListRows(0x18);
    request = frwGetSmallFontFRQ2(0x97, 0x101, 0xFF, 0x80808080, text.question, NULL);
    frSetDrawPosition(request, 0x970, 0x808);
    frSetPriority(request, 0xFF0);
    func_001C7E38(request, 0, 0x47);
    frDestroyFontRequestAfter(request);

    request = frwGetSmallFontFRQ2(0x9D, 0x122, 0xFF, 0x80808080, text.yes, NULL);
    frSetDrawPosition(request, 0x9D0, 0x910);
    if (selected == 0) {
        frSetFontColorNo(request, 1);
        nmeDrawTextBox(0x9A0, 0x920, 0xFF);
    }
    frSetPriority(request, 0xFF0);
    func_001C7E38(request, 0, 0x47);
    frDestroyFontRequestAfter(request);

    request = frwGetSmallFontFRQ2(0x9D, 0x13D, 0xFF, 0x80808080, text.no, NULL);
    frSetDrawPosition(request, 0x9D0, 0x9E8);
    if (selected == 1) {
        frSetFontColorNo(request, 1);
        nmeDrawTextBox(0x9A0, 0x9F0, 0xFF);
    }
    frSetPriority(request, 0xFF0);
    func_001C7E38(request, 0, 0x47);
    frDestroyFontRequestAfter(request);

    frDisableMode(1);
    cmpSetSkillListRows(-1);
}

extern void nmeDrawNameText(s32 x, s32 y, s32 width, s32 height, u32 color, s32 mode);
extern void nmeDrawNameString(s32 x, s32 y, u32 color, s32 mode);
extern void nmeDrawEditIcons(void);
extern void nmeDrawColoredText(s8 color);

void nmeDrawButtonGuide(void)
{
    fclDrawSpriteRes(0, 0, 0, 0, 3, 0x17,
                     (s32 *)namelist_buttonAnim_end->textures, &GBWK_2, 0x47);
    fclDrawSpriteRes(0, 0, 0, 0, 3, 0x1D,
                     (s32 *)namelist_buttonAnim_end->textures, &GBWK_2, 0x47);
    nmeDrawNameText(0x2A, 0x64, 0xF4, 0x64, 0xFFD19280, 1);
    if (namelist_buttonAnim_end->entryMode == 0) {
        fclDrawSpriteRes(0, 0, 0, 0, 3, 0x18,
                         (s32 *)namelist_buttonAnim_end->textures, &GBWK_2, 0x47);
        nmeDrawNameString(0x83, 0x9A, 0xFFD19280, 1);
    }
    nmeDrawEditIcons();
    nmeDrawColoredText((s8)((u8)namelist_buttonAnim_end->wrapCursor.unk_22 +
                            (u8)namelist_buttonAnim_end->wrapCursor.unk_24));
}

void nmeDrawPanelSprites(void)
{
    s32 colors[4];

    fclDrawSpriteRes(0, 0, 0, 0, 3, 0x5,
                     (s32 *)namelist_buttonAnim_end->textures, &GBWK_2, 0x47);
    fclDrawSpriteRes(0, 0, 0, 0, 3, 0x6,
                     (s32 *)namelist_buttonAnim_end->textures, &GBWK_2, 0x47);
    fclDrawSpriteRes(0, 0, 0, 0, 3, 0x7,
                     (s32 *)namelist_buttonAnim_end->textures, &GBWK_2, 0x47);
    fclDrawSpriteRes(0, 0, 0, 0, 3, 0x8,
                     (s32 *)namelist_buttonAnim_end->textures, &GBWK_2, 0x47);
    fclDrawSpriteRes(0, 0, 0, 0, 3, 0x9,
                     (s32 *)namelist_buttonAnim_end->textures, &GBWK_2, 0x47);
    fclDrawSpriteRes(0, 0, 0, 0, 3, 0xA,
                     (s32 *)namelist_buttonAnim_end->textures, &GBWK_2, 0x47);
    fclDrawSpriteRes(0, 0, 0, 0, 3, 0xB,
                     (s32 *)namelist_buttonAnim_end->textures, &GBWK_2, 0x47);
    fclDrawSpriteRes(0, 0, 0, 0, 3, 0xC,
                     (s32 *)namelist_buttonAnim_end->textures, &GBWK_2, 0x47);

    colors[0] = 0x80808053;
    colors[1] = 0x80808053;
    colors[2] = 0x80808053;
    colors[3] = 0x80808053;

    fclDrawSpriteRes(0, 0, 0, colors, 3, 0xD,
                     (s32 *)namelist_buttonAnim_end->textures, &GBWK_2, 0x47);
    fclDrawSpriteRes(0, 0, 0, colors, 3, 0xE,
                     (s32 *)namelist_buttonAnim_end->textures, &GBWK_2, 0x47);
}

void nmeDrawNamePanel(void)
{
    nmeSpriteTable_t *table = &GBWK_2;
    s32 ot = 0x47;

    fclDrawSpriteRes(0, 0, 0, 0, 3, 0,
                     (s32 *)namelist_buttonAnim_end->textures, table, ot);
    fclDrawSpriteRes(0, 0, 0, 0, 3, 1,
                     (s32 *)namelist_buttonAnim_end->textures, table, ot);
    fclDrawSpriteRes(0, 0, 0, 0, 3, 2,
                     (s32 *)namelist_buttonAnim_end->textures, table, ot);
}

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

