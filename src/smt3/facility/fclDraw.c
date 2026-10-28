#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"
#include "font.h"

typedef struct
{
    u8 flag;
    u8 unk_01[3];
    u8 race;
    u8 unk_05;
    u8 unk_06[54];
} datDevilInfo_t;

typedef struct
{
    /* 0x00 */ datUnitWork_t unit;
} cmbUnit_t;

extern datDevilInfo_t datDevilTable[];

extern s32 sprintf(char *buf, const char *fmt, ...);
extern frFontHandle_t *frwGetSmallNumFontFRQ(s32 x, s32 y, s32 priority, u32 color,
                                              const char *text, frFontHandle_t *previous);
extern frFontHandle_t *frwGetSmallFontFRQ2(s32 x, s32 y, s32 priority, s32 color,
                                            char *text, frFontHandle_t *previous);
extern frFontHandle_t *frwGetSmallCaptionFontFRQ(s32 x, s32 y, s32 priority, u32 color,
                                                  const char *text, frFontHandle_t *previous);
extern void frSetDrawPosition(frFontHandle_t *request, u32 x, u32 y);
extern s32 func_001C7E38(frFontHandle_t *request, s8 mode, u32 ot);
extern s32 frDestroyFontRequestAfter(frFontHandle_t *request);

extern void sortSkillList(cmbUnit_t *unit);

typedef struct
{
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s16 unk_0C;
    /* 0x0E */ s16 unk_0E;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ s32 unk_1C;
} fclPanelPos_t;

typedef struct
{
    /* 0x00 */ u16 x;
    /* 0x02 */ u16 y;
    /* 0x04 */ u16 u0;
    /* 0x06 */ u16 v0;
    /* 0x08 */ u16 u1;
    /* 0x0A */ u16 v1;
} fclSpriteRect_t;

typedef struct
{
    /* 0x00 */ s32 tex;
    /* 0x04 */ u16 unk_04;
    /* 0x06 */ u16 unk_06;
    /* 0x08 */ u16 unk_08;
    /* 0x0A */ u16 unk_0A;
    /* 0x0C */ u16 unk_0C;
    /* 0x0E */ u16 unk_0E;
    /* 0x10 */ s32 X;
    /* 0x14 */ s32 Y;
    /* 0x18 */ s32 Z;
    /* 0x1C */ u8 color[4];
    /* 0x20 */ u8 Flag;
} fclSprite_t;

typedef struct
{
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ u8 Flag;
    /* 0x06 */ u16 res;
} fclSpriteEntry_t;

typedef struct
{
    /* 0x00 */ s8 tex;
    /* 0x02 */ fclSpriteRect_t rect;
} fclSpriteRes_t;

typedef struct
{
    /* 0x00 */ fclSpriteRes_t *res;
    /* 0x04 */ fclSpriteEntry_t *entries;
} fclSpriteTbl_t;

extern void fclDrawSpriteCore(s32 X, s32 Y, s32 W, s32 H, s32 *pCol, fclSprite_t *pSprite,
                              u8 FlipFlag, u8 AlphaFlag, u8 FilterFlag, s32 OtNo);

extern void fclDrawSpriteAngleCore(s32 X, s32 Y, s32 W, s32 H, f32 Angle, s32 *pCol,
                                   fclSprite_t *pSprite, u8 FlipFlag, u8 AlphaFlag, u8 FilterFlag, s32 OtNo);

extern void qsort(void *base, s32 count, s32 size, s32 (*cmp)(const void *a, const void *b));

extern s32 func_00213148(const void *a, const void *b);

extern s32 func_00213190(const void *a, const void *b);

extern void fclDrawSpriteEx(s32 X, s32 Y, s32 W, s32 H, fclSprite_t *pSprite, s32 OtNo);

typedef struct
{
    /* 0x00 */ u8 StepY[4];
    /* 0x04 */ s8 StopFlag;
    /* 0x05 */ u8 unk_05[3];
    /* 0x08 */ s32 CursorSize;
    /* 0x0C */ s32 CursorPos;
    /* 0x10 */ u16 unk_10;
    /* 0x12 */ u16 unk_12;
    /* 0x14 */ u16 unk_14;
    /* 0x16 */ u16 unk_16;
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ s32 unk_1C;
    /* 0x20 */ u8 unk_20;
    /* 0x21 */ u8 unk_21;
    /* 0x22 */ s16 unk_22;
    /* 0x24 */ s8 unk_24;
    /* 0x25 */ s8 unk_25;
    /* 0x26 */ u8 unk_26;
    /* 0x27 */ u8 unk_27[5];
    /* 0x2C */ s32 unk_2C;
    /* 0x30 */ s32 unk_30;
    /* 0x34 */ s32 unk_34;
} cmpCursorInfo_t;

typedef struct
{
    /* 0x0 */ u16 skill;
    /* 0x2 */ u8 order;
    /* 0x3 */ u8 unk_3;
} skillSortEntry_t;

extern void fclDrawSpriteRes(s32 X, s32 Y, s32 Z, s32 *pClip, s32 page, s32 spriteId, s32 *texList,
                             fclSpriteTbl_t *table, s32 OtNo);

extern s32 trmDrawListText(fclPanelPos_t *panel, char *text, s32 *texList,
                            fclSpriteTbl_t *table, s32 ot);

extern void shopDrawItemCount(fclPanelPos_t *pos, s32 color, s32 id);

extern void fclDrawSpriteFilter(s32 x, s32 y, s32 d, u8 filter, s32 *color, s32 id, s32 idx,
                                s32 *texs, fclSpriteTbl_t *table, s32 OtNo);

void fclDrawBG(fclSprite_t *pSprite, s32 OtNo)
{
    if (pSprite == NULL)
    {
        cmpDrawBoxF(0, 0, 0, 0x200, 0x1C0, 0x80, OtNo);
    }
    else
    {
        fclDrawSpriteEx(0, 0, pSprite->unk_04, pSprite->unk_06, pSprite, OtNo);
    }
}

void fclSetupSprite(s32 Z, fclSprite_t *pSprite, u8 page, u16 spriteId, s32 *texList, fclSpriteTbl_t *table)
{
    fclSpriteEntry_t *e;
    fclSpriteRes_t *res;

    e = &table[page].entries[spriteId];
    res = &table[page].res[e->res];
    fclSetSpriteEx(pSprite, &res->rect, e->x, e->y, Z, 0x80808080, e->Flag, texList[res->tex]);
}

void fclDrawSpriteRect(s32 x, s32 y, s32 d, s32 w, s32 h, s32 *color, s32 id, s32 idx,
                       s32 *texList, fclSpriteTbl_t *table, s32 OtNo)
{
    fclSprite_t sprite;

    fclSetupSprite(d, &sprite, id, idx, texList, table);
    sprite.unk_0C = sprite.unk_08 + (sprite.unk_0C - sprite.unk_08) * w / 1000;
    sprite.unk_0E = sprite.unk_0A + (sprite.unk_0E - sprite.unk_0A) * h / 1000;
    sprite.unk_04 = sprite.unk_04 * w / 1000;
    sprite.unk_06 = sprite.unk_06 * h / 1000;
    if ((w != 0) && (sprite.unk_04 < 4)) {
        sprite.unk_0C = sprite.unk_08 + 0x40;
        sprite.unk_04 = 4;
    }
    if ((h != 0) && (sprite.unk_06 < 4)) {
        sprite.unk_06 = 4;
        sprite.unk_0E = sprite.unk_0A + 0x40;
    }
    fclDrawSpriteCore(x + sprite.X, y + sprite.Y, sprite.unk_04, sprite.unk_06, color,
                      &sprite, sprite.Flag, 0, 1, OtNo);
}

void fclDrawSpriteAlpha(s32 x, s32 y, s32 d, u8 mode, s32 *color, s32 id, s32 idx, s32 *texs,
                        fclSpriteTbl_t *table, s32 OtNo)
{
    fclSprite_t sprite;

    fclSetupSprite(d, &sprite, id, idx, texs, table);
    fclDrawSpriteCore(x + sprite.X, y + sprite.Y, sprite.unk_04, sprite.unk_06, color,
                      &sprite, sprite.Flag, mode, 1, OtNo);
}

void fclDrawSpriteFilter(s32 x, s32 y, s32 d, u8 filter, s32 *color, s32 id, s32 idx,
                         s32 *texs, fclSpriteTbl_t *table, s32 OtNo)
{
    fclSprite_t sprite;

    fclSetupSprite(d, &sprite, id, idx, texs, table);
    fclDrawSpriteCore(x + sprite.X, y + sprite.Y, sprite.unk_04, sprite.unk_06, color,
                      &sprite, sprite.Flag, 0, filter, OtNo);
}

void fclDrawSpriteSize(s32 x, s32 y, s32 d, s32 w, s32 h, s32 *color, s32 id, s32 idx,
                       s32 *texList, fclSpriteTbl_t *table, s32 OtNo)
{
    fclSprite_t sprite;

    fclSetupSprite(d, &sprite, id, idx, texList, table);
    fclDrawSpriteCore(x + sprite.X, y + sprite.Y, w, h, color, &sprite, sprite.Flag, 0,
                      1, OtNo);
}

void fclDrawSpriteFull(s32 x, s32 y, s32 d, s32 w, s32 h, u8 mode, s32 *color, s32 id,
                       u16 spriteId, s32 *texList, fclSpriteTbl_t *table, s32 OtNo)
{
    fclSprite_t sprite;

    fclSetupSprite(d, &sprite, id, spriteId, texList, table);
    fclDrawSpriteCore(x + sprite.X, y + sprite.Y, w, h, color, &sprite, sprite.Flag,
                      mode, 1, OtNo);
}

void fclDrawSpriteSizeFilter(s32 x, s32 y, s32 d, s32 w, s32 h, u8 filter, s32 *color, s32 id,
                             u16 spriteId, s32 *texList, fclSpriteTbl_t *table, s32 OtNo)
{
    fclSprite_t sprite;

    fclSetupSprite(d, &sprite, id, spriteId, texList, table);
    fclDrawSpriteCore(x + sprite.X, y + sprite.Y, w, h, color, &sprite, sprite.Flag, 0,
                      filter, OtNo);
}

void fclDrawSpriteSizeAlphaFilter(s32 x, s32 y, s32 d, s32 w, s32 h, u8 mode, s32 *color,
                                  s32 id, f32 angle, u16 idx, s32 *texs, fclSpriteTbl_t *tbl,
                                  s32 ot)
{
    fclSprite_t sprite;

    fclSetupSprite(d, &sprite, id, idx, texs, tbl);
    fclDrawSpriteAngleCore(x + sprite.X, y + sprite.Y, w, h, angle, color, &sprite,
                           sprite.Flag, mode, 1, ot);
}

void fclDrawSpriteRes(s32 X, s32 Y, s32 Z, s32 *pClip, s32 page, s32 spriteId, s32 *texList, fclSpriteTbl_t *table,
                      s32 OtNo)
{
    fclSprite_t sprite;

    fclSetupSprite(Z, &sprite, page, spriteId, texList, table);
    fclDrawSpriteCore(X + sprite.X, Y + sprite.Y, sprite.unk_04, sprite.unk_06, pClip,
                      &sprite, sprite.Flag, 0, 1, OtNo);
}

void fcl_212910(s32 x, s32 y, s32 *color, u8 page, u16 spriteId, s32 *texList,
                fclSpriteTbl_t *table)
{
    fclSprite_t sprite;

    fclSetupSprite(0, &sprite, page, spriteId, texList, table);
    fclDrawSpriteCore(x + sprite.X, y + sprite.Y, sprite.unk_04, sprite.unk_06,
                      color, &sprite, sprite.Flag, 0, 1, 7);
}

extern void cmpDrawScrollBar(s32 a, s32 b, s32 c, s32 d, s32 e, s32 color, s32 g, s32 h,
                             cmpCursorInfo_t *cursor, s32 j, s32 k);

void fcl_2129A0(s32 a, s32 b, cmpCursorInfo_t *cursor, s32 d)
{
    cmpDrawScrollBar(cursor->CursorSize * 16, cursor->CursorPos * 8, a, cursor->unk_10,
                     cursor->unk_14, 0x80808080, 0x100, 0, cursor, 0, d);
}

void fcl_2129F8(s32 x, s32 y, s32 priority, u32 color, s32 value, s32 ot)
{
    char text[0x80];
    frFontHandle_t *request;

    sprintf(text, "%d", value);
    request = frwGetSmallNumFontFRQ(x, y, priority, color, text, NULL);
    frSetDrawPosition(request, x - ((request->advance >> 1) << 4), y);
    func_001C7E38(request, 1, ot);
    frDestroyFontRequestAfter(request);
}

s32 fclDrawStrCenterTarget(s32 x, s32 y, s32 priority, u16 width, u16 unused1, u32 unused2,
                           const char *text, s32 ot)
{
    frFontHandle_t *request;

    request = frwGetSmallCaptionFontFRQ(x, y, priority, 0xDEFFA180, text, NULL);
    frSetDrawPosition(request, x + (((s32)width - request->advance) >> 1) * 16,
                      y);
    func_001C7E38(request, 1, ot);
    return frDestroyFontRequestAfter(request);
}

s32 fclDrawCaption(fclPanelPos_t *panel, const char *text, s32 ot)
{
    frFontHandle_t *request;
    frFontHandle_t *glyph;
    s32 width;

    request = frwGetSmallFontFRQ2(0x100, panel->unk_04 + 0x10, panel->unk_08,
                                  0xA09DC380, text, NULL);
    width = 0;
    for (glyph = request; glyph != NULL; glyph = glyph->previous) {
        width += glyph->advance;
        width = width + glyph->glyphCodeOrContext.byteRoles.spacing + 3;
    }
    cmpDrawBoxF(0, panel->unk_04, panel->unk_08, width + 0x20, 0x1A,
                0x30303080, ot);
    func_001C7E38(request, 1, ot);
    return frDestroyFontRequestAfter(request);
}

s32 trmDrawListText(fclPanelPos_t *panel, char *text, s32 *texList,
                    fclSpriteTbl_t *table, s32 ot)
{
    frFontHandle_t *request;

    request = frwGetSmallFontFRQ2(0x270, panel->unk_04, panel->unk_08,
                                  0xA09DC380, text, NULL);
    fclDrawSpriteRes(0, 0, 0xFFFFF, NULL, 0, 0, texList, table, ot);
    func_001C7E38(request, 1, ot);
    return frDestroyFontRequestAfter(request);
}

INCLUDE_ASM(const s32, "facility/fclDraw", trm_212CB8);

/* Quantity layout shared by datAddItem and datCheckItem. */
typedef struct
{
    u8 reserved[0x1928];
    u8 itemQuantity[0xA0];
} fclInventoryView_t;

extern void fclInitPanelPos(s32 x, s32 y, s32 z, s16 width, s16 height,
                             s32 work, fclPanelPos_t *pos);
extern void frSetMessageStepY(u32 step);
extern void frSetMessageDefStepY(void);
extern s32 cmpDrawHelp(fclPanelPos_t *pos, u16 item, s8 mode, s8 hideSprite,
                        s32 alpha, s32 ot);
extern s32 datGetItemSlotIndex(u16 item);
extern s32 cmbChkGetHearts(s32 id);
extern fclSpriteTbl_t fclSpriteTable[];
extern s32 D_00345D88[];
extern char D_0054FC40[];
extern frFontHandle_t *frwGetNumFontFRQ(s32 x, s32 y, s32 priority, u32 color,
                                         const char *text, frFontHandle_t *previous);

void fclDrawPriceText(fclPanelPos_t *pos, u16 item, s32 ot)
{
    char text[16];
    fclPanelPos_t panel;
    fclPanelPos_t *panelPtr = &panel;
    s32 x = pos->unk_00;
    s32 y = pos->unk_04;
    s32 spriteY = y + 0x238;
    s32 slot, quantity;
    frFontHandle_t *request;

    fclInitPanelPos(x, y, pos->unk_08, 0, 0, 0, panelPtr);
    frSetMessageStepY(0x15);
    cmpDrawHelp(panelPtr, item, 1, 1, 0x100, 0x47);
    frSetMessageDefStepY();
    fclDrawSpriteFilter(x + 0x390, spriteY, pos->unk_08 << 4, 1, NULL,
                        10, 10, D_00345D88, fclSpriteTable, ot);
    fclDrawSpriteFilter(x + 0x960, spriteY, pos->unk_08 << 4, 1, NULL,
                        10, 11, D_00345D88, fclSpriteTable, ot);
    slot = datGetItemSlotIndex(item);
    if (slot != 0) {
        quantity = cmbChkGetHearts((s8)slot) != 0;
    } else {
        quantity = ((fclInventoryView_t *)DDS3_GBWK)->itemQuantity[item];
    }
    sprintf(text, D_0054FC40, quantity);
    request = frwGetNumFontFRQ(x + 0x7B0, y + 0x218, pos->unk_08,
                               0xA0F0EB80, text, NULL);
    func_001C7E38(request, 1, ot);
    frDestroyFontRequestAfter(request);
}

extern char D_0054FC48[]; /* Shared, padded "%7d" format. */

void shopDrawItemCount(fclPanelPos_t *pos, s32 color, s32 ot)
{
    s32 colors[4];
    char text[16];
    frFontHandle_t *request;

    colors[0] = color;
    colors[1] = color;
    colors[2] = color;
    colors[3] = color;
    fclDrawSpriteFilter(pos->unk_00 + 0x10D0, pos->unk_04 + 0x78,
                        pos->unk_08 << 4, 1, colors, 10, 0, D_00345D88,
                        fclSpriteTable, ot);
    fclDrawSpriteFilter(pos->unk_00 + 0x10D0, pos->unk_04 + 0x78,
                        pos->unk_08 << 4, 1, colors, 10, 0xC, D_00345D88,
                        fclSpriteTable, ot);
    fclDrawSpriteFilter(pos->unk_00 + 0x1E40, pos->unk_04 + 0xB0,
                        pos->unk_08 << 4, 1, colors, 10, 1, D_00345D88,
                        fclSpriteTable, ot);

    color = (color & 0xFF) | 0xA09DC300;
    sprintf(text, D_0054FC48, DDS3_GBWK->maka);
    /* The numeric baseline uses the X coordinate. */
    request = frwGetNumFontFRQ(pos->unk_00 + 0x1840, pos->unk_00 + 0x90,
                               pos->unk_08, color, text, NULL);
    func_001C7E38(request, 1, ot);
    frDestroyFontRequestAfter(request);
}

s32 func_00213148(const void *a, const void *b)
{
    u32 keyA = *(u32 *)a;
    u32 keyB = *(u32 *)b;
    u16 subA;
    u16 subB;

    if (keyB < keyA) {
        return -1;
    }
    if (keyA < keyB) {
        return 1;
    }
    subA = *(u16 *)((u8 *)a + 4);
    subB = *(u16 *)((u8 *)b + 4);
    if (subB < subA) {
        return 1;
    }
    if (subA < subB) {
        return -1;
    }
    return 0;
}

s32 func_00213190(const void *a, const void *b)
{
    u16 *pa = (u16 *)a;
    u16 *pb = (u16 *)b;

    if (*pa == 0) {
        if (*pb != 0) {
            return 1;
        }
    } else if (*pb == 0) {
        return -1;
    }
    if (*pb < *pa) {
        return 1;
    }
    if (*pa < *pb) {
        return -1;
    }
    return 0;
}

s32 cmpSkillOrder(const void *a, const void *b)
{
    u16 *pa = (u16 *)a;
    u16 *pb = (u16 *)b;

    if (*pa == 0) {
        if (*pb != 0) {
            return 1;
        }
    } else if (*pb == 0) {
        return -1;
    }
    if (*pb < *pa) {
        return 1;
    }
    if (*pa < *pb) {
        return -1;
    }
    return 0;
}

s32 cmbSetEvoResult(const void *a, const void *b)
{
    datDevilInfo_t *eb = *(datDevilInfo_t **)((u8 *)b + 4);
    datDevilInfo_t *ea = *(datDevilInfo_t **)((u8 *)a + 4);

    if (ea->race > eb->race) {
        return 1;
    }
    if (ea->race < eb->race) {
        return -1;
    }
    if (ea->unk_05 > eb->unk_05) {
        return -1;
    }
    return ea->unk_05 < eb->unk_05;
}

void buildSkillSortArray(cmbUnit_t *unit, skillSortEntry_t *out)
{
    s32 i;

    for (i = 0; i < 24; i++) {
        out[i].skill = unit->unit.keisyoskill[i];
        out[i].order = unit->unit.keiattr[i];
    }
}

void applySkillSortArray(cmbUnit_t *unit, skillSortEntry_t *in)
{
    s32 i;

    for (i = 0; i < 24; i++) {
        unit->unit.keisyoskill[i] = in[i].skill;
        unit->unit.keiattr[i] = in[i].order;
    }
}

void apply_213300(void *base, u8 count)
{
    qsort(base, count, 0xEC, func_00213148);
}

void sort_213328(u8 *base)
{
    qsort(&base[0x3A], 0x18, 2, func_00213190);
}

extern s32 cmpSkillOrder(const void *a, const void *b);

void sortSkillList(cmbUnit_t *unit)
{
    skillSortEntry_t order[24];

    buildSkillSortArray(unit, order);
    qsort(order, 0x18, 4, cmpSkillOrder);
    applySkillSortArray(unit, order);
}

typedef struct
{
    /* 0x00 */ u16 index;
    /* 0x02 */ u16 unk_02;
    /* 0x04 */ datDevilInfo_t *info;
} cmbRaceSortEntry_t;

void cmbSetRaceResult(cmbRaceSortEntry_t *list, u16 count)
{
    s32 i = 0;

    if (count != 0) {
        datDevilInfo_t *info = datDevilTable;
        cmbRaceSortEntry_t *p = list;

        do {
            p->index = i;
            i++;
            p->info = info;
            info++;
            p++;
        } while (i < count);
    }
    qsort(list, count, 8, cmbSetEvoResult);
}
INCLUDE_SDATA(const s32, "facility/fclDraw", D_0054FC40);

INCLUDE_SDATA(const s32, "facility/fclDraw", D_0054FC48);

