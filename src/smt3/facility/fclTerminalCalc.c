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

INCLUDE_ASM(const s32, "facility/fclTerminalCalc", trmUpdateListSelection);

INCLUDE_ASM(const s32, "facility/fclTerminalCalc", trmDrawSeq0);
