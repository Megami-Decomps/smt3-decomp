#include "common.h"
#include "libsdf.h"
#include "kernel.h"

/* Font chains and their children have different pool strides. Only the
 * coordinates, color word and links used by this unit are exposed. */
typedef struct frFontChild_t
{
    u8 unk00[0x10];
    union { u32 renderWord; } parentDimensionsOrRenderWord;
    u8 unk14[0x14];
    struct frFontChild_t *next;
} frFontChild_t;

typedef struct frFontRequest_t
{
    u32 unk00;
    s32 x;
    s32 y;
    u8 unk0C[0x10];
    frFontChild_t *firstChild;
    void *lastChild;
    struct frFontRequest_t *previous;
    u8 unk28[0x1C];
} frFontRequest_t;

extern frFontRequest_t *frTestFontHandle;

extern void fclDestroyMessage(void);

extern void frDestroyFontRequest(frFontRequest_t *pFRQ);

INCLUDE_ASM(const s32, "interface/frTestFont", frTestFontInit);

void test_font_Shutdown(void) {
    fclDestroyMessage();
    frDestroyFontRequest(frTestFontHandle);
}

INCLUDE_ASM(const s32, "interface/frTestFont", test_font_1CAD88);

s32 test_font_Process(void) {
    return 0;
}

