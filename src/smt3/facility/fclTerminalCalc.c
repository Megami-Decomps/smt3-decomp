#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"

extern void frDestroyFontRequestAfter(void *pFRQ);

extern void func_001C7E38(void *frq, s32 mode, s32 arg);

INCLUDE_ASM(const s32, "facility/fclTerminalCalc", trmCalc);

extern char D_00311450[][0x20];

extern char D_00311850[][0x20];

extern void *frwGetSmallFontFRQ2(s32 X, s32 Y, s32 Pri, s32 Color, char *pStr, s32 pFRQ);

void *trmGetListFont(s32 x, s32 y, s32 color, s32 d, s8 index, s8 which)
{
    char *text;

    if (which == 0)
    {
        text = D_00311450[index];
    }
    else
    {
        text = D_00311850[index];
    }
    return frwGetSmallFontFRQ2(x, y, color, d, text, 0);
}

extern void *trmGetListFont(s32 a, s32 b, s32 c, s32 d, s8 e, s8 f);

extern void func_001C7E38(void *frq, s32 mode, s32 arg);

void trmFreeListFont(s32 a, s32 b, s32 c, s32 d, s8 e, s8 f, s32 g)
{
    void *frq = trmGetListFont(a, b, c, d, e, f);

    func_001C7E38(frq, 1, g);
    frDestroyFontRequestAfter(frq);
}

typedef struct trmPanelPos_t {
    s32 x;
    s32 y;
    s32 z;
    s16 width;
    s16 height;
    s32 flags;
    u8 reserved14[0xC];
} trmPanelPos_t;

typedef struct trmListCursor_t {
    s32 rowStep;
    u8 reserved04[0x1E];
    s16 rowBase;
    u8 reserved24[2];
    u8 visibleCount;
    u8 reserved27[0x11];
} trmListCursor_t;

typedef struct trmListWork_t {
    u8 reserved00[0x40];
    trmPanelPos_t panel;
    u8 reserved60[0xFC];
    trmListCursor_t cursor;
} trmListWork_t;

typedef struct trmDrawWork_t {
    u8 reserved00[0x1C];
    u8 listHeader[0x15C];
    u8 reserved0178[0xEE0];
    trmListWork_t terminalList;
    u8 reserved11EC[0x1005];
    s8 terminalType;
    u16 terminalNo;
    s8 terminalIndices[0x21];
    u8 reserved2215[0x23];
    s32 texHandle;
} trmDrawWork_t;

extern trmDrawWork_t *trmDrawWork;
extern void fclInitPanelPos(s32 x, s32 y, s32 z, s16 width, s16 height,
                           s32 flags, trmPanelPos_t *panel);
extern s32 cmpUpdateListSelection(trmPanelPos_t *panel, u8 *list, s32 row,
                                  s32 mode, s32 width, s32 ot);
extern u32 frGetStrWidth2(void *request);
extern void frSetDrawPosition(void *request, s32 x, s32 y);
extern void func_001C7E38(void *frq, s32 mode, s32 arg);
typedef struct {
    void *res;
    void *entries;
} fclSpriteTbl_t;
extern void fclDrawSpriteRes(s32 x, s32 y, s32 z, s32 *clip, s32 page,
                             s32 spriteId, s32 *texList, fclSpriteTbl_t *table, s32 ot);
extern void fclDrawSpriteSize(s32 x, s32 y, s32 d, s32 w, s32 h, s32 *color,
                              s32 id, s32 idx, s32 *texList, fclSpriteTbl_t *table, s32 ot);
extern fclSpriteTbl_t D_003308F8[];

void trmDrawListRow(trmPanelPos_t *panel, s32 ot)
{
    u32 width;
    s32 center;
    void *font;

    font = trmGetListFont(0, 0, panel->z, 0xA09DC380,
                          (s8)trmDrawWork->terminalNo, trmDrawWork->terminalType);
    width = frGetStrWidth2(font);
    center = ((0x1EC - width) << 4);
    fclDrawSpriteRes(center - 0x450, 0xC8, panel->z << 4, NULL, 0, 1,
                     &trmDrawWork->texHandle, D_003308F8, ot);
    fclDrawSpriteSize(center + 0x450, 0xC8, panel->z << 4, width - 0x31, 0x21,
                      NULL, 0, 2, &trmDrawWork->texHandle, D_003308F8, ot);
    frSetDrawPosition(font, center, 0xF8);
    func_001C7E38(font, 0, ot);
    frDestroyFontRequestAfter(font);
}

extern char *trmMenuTextTable[];
extern s32 trmDrawListText(trmPanelPos_t *panel, char *text, s32 *textures,
                          fclSpriteTbl_t *table, s32 ot);
extern void lstDrawListWindow(trmListWork_t *list, s8 noCursor, s32 ot);
extern void lstDrawListWindowCursor(trmListWork_t *list, s32 ot);
extern s32 cmpDrawListItemBox(trmListCursor_t *cursor, s32 row, s32 slot, s32 mode, s32 width);

void trmDrawList(void)
{
    trmPanelPos_t panel;
    trmListCursor_t *cursor = &trmDrawWork->terminalList.cursor;
    s32 row = 0;
    s32 baseX;
    s32 y;
    s32 z;

    fclInitPanelPos(0, 0xF8, 0xFFFFF, 0, 0, 0, &panel);
    trmDrawListText(&panel, trmMenuTextTable[12], &trmDrawWork->texHandle, D_003308F8, 0x47);
    fclInitPanelPos(0xC80, 0x100, 0xFFFFF, 0x80, 0, 0, &panel);
    trmDrawListRow(&panel, 0x47);
    lstDrawListWindow(&trmDrawWork->terminalList, 0, 0x47);
    baseX = trmDrawWork->terminalList.panel.x;
    y = trmDrawWork->terminalList.panel.y;
    z = trmDrawWork->terminalList.panel.z;
    if (cursor->visibleCount != 0) {
        s32 x = baseX + 0x330;
        do {
            s32 color = cmpDrawListItemBox(cursor, row, row, 0, 0x100);
            trmFreeListFont(x, y + 0x48, z, color,
                           trmDrawWork->terminalIndices[cursor->rowBase + row], 0, 0x47);
            row++;
            y += cursor->rowStep << 3;
        } while (row < cursor->visibleCount);
    }
    lstDrawListWindowCursor(&trmDrawWork->terminalList, 0x47);
}


void trmUpdateListSelection(void)
{
    trmPanelPos_t panel;

    fclInitPanelPos(0xC80, 0x100, 0xFFFFF, 0x80, 0, 0, &panel);
    trmDrawListRow(&panel, 0x47);
    if (trmDrawWork->terminalType != 0) {
        fclInitPanelPos(0, 0, 0xFFFFF, 0x80, 0, 0, &panel);
        cmpUpdateListSelection(&panel, trmDrawWork->listHeader, 0, 3, 0x100, 0x47);
        cmpUpdateListSelection(&panel, trmDrawWork->listHeader, 0, 1, 0x100, 0x47);
    }
}

void trmDrawSeq0(s8 mode)
{
    trmPanelPos_t panel;

    fclInitPanelPos(0, 0, 0xFFFFF, 0x80, 0, 0, &panel);
    cmpUpdateListSelection(&panel, trmDrawWork->listHeader, 0, 3, 0x100, 0x47);
    cmpUpdateListSelection(&panel, trmDrawWork->listHeader, 0, mode, 0x100, 0x47);
    fclInitPanelPos(0xC80, 0x100, 0xFFFFF, 0x80, 0, 0, &panel);
    trmDrawListRow(&panel, 0x47);
}
