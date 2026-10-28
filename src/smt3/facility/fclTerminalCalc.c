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

void *trmGetListFont(s32 x, s32 y, s32 color, s32 d, s8 index, s8 which, s32 arg6)
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

extern void *trmGetListFont(s32 a, s32 b, s32 c, s32 d, s8 e, s8 f, s32 g);

extern void func_001C7E38(void *frq, s32 mode, s32 arg);

void trmFreeListFont(s32 a, s32 b, s32 c, s32 d, s8 e, s8 f, s32 g)
{
    void *frq = trmGetListFont(a, b, c, d, e, f, g);

    func_001C7E38(frq, 1, g);
    frDestroyFontRequestAfter(frq);
}

INCLUDE_ASM(const s32, "facility/fclTerminalCalc", trmDrawListRow);

INCLUDE_ASM(const s32, "facility/fclTerminalCalc", trmDrawList);

typedef struct trmPanelPos_t {
    s32 x;
    s32 y;
    s32 z;
    s16 width;
    s16 height;
    s32 flags;
    u8 unk14[0xC];
} trmPanelPos_t;

typedef struct trmDrawWork_t {
    u8 unk00[0x1C];
    /* List-window header prefix; cursor and trailing storage follow. */
    u8 listHeader[0x15C];
    u8 unk0178[0x2079];
    s8 terminalType;
} trmDrawWork_t;

extern trmDrawWork_t *trmDrawWork;
extern void fclInitPanelPos(s32 x, s32 y, s32 z, s16 width, s16 height,
                           s32 flags, trmPanelPos_t *panel);
extern s32 cmpUpdateListSelection(trmPanelPos_t *panel, u8 *list, s32 row,
                                  s32 mode, s32 width, s32 ot);
extern void trmDrawListRow(trmPanelPos_t *panel, s32 ot);

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
