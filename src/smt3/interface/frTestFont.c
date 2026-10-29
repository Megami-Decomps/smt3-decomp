#include "common.h"
#include "scr.h"
#include "libsdf.h"
#include "kernel.h"
#include "font.h"

extern frFontHandle_t *frTestFontHandle;
extern void *D_0054F46C;

extern void fclDestroyMessage(void);
extern void *fclLoadTexture(char *pFileName);
extern void frwLoadStaffFont(void);
extern void frDestroyFontRequest(frFontHandle_t *pFRQ);
extern void frCreateUserFontPacket(s32 X, s32 Y, s32 Z, frTextHandle_t *pText, s32 Ot);
extern dds3ProcessID_t *dds3StartProcess(char *name, u32 priority, s32 v2, s32 v3,
                                         dds3ProcessFunc_t processfunc,
                                         dds3ProcessFunc_t shutdownfunc, void *p);

void test_font_Shutdown(void);
s32 test_font_Process(void);

void frTestFontInit(void) {
    D_0054F46C = fclLoadTexture("/camp/test00.tmx");
    frwLoadStaffFont();
    dds3StartProcess("test_font", 0x2EE0, 0, 0, (dds3ProcessFunc_t)test_font_Process,
                     (dds3ProcessFunc_t)test_font_Shutdown, NULL);
    scrStartScriptByName(0x258, "host0:../../../dds3data/facility/test.bf", 0);
}

void test_font_Shutdown(void) {
    fclDestroyMessage();
    frDestroyFontRequest(frTestFontHandle);
}

void test_font_1CAD88(frFontHandle_t *parent) {
    frTextHandle_t *child;
    s32 x;
    s32 y;
    s32 renderValue;
    s32 i;

    if (parent == NULL) {
        return;
    }
    parent = parent->chainHead;
    if (parent == NULL) {
        return;
    }
    do {
        x = parent->x;
        y = parent->y;
        renderValue = parent->renderValueOrSetupOrShade.renderValue;
        child = parent->firstChild;
        while (child != NULL) {
            for (i = 0; i < 8; i++) {
                frCreateUserFontPacket(x + (i * 16), y + (i * 16), renderValue, child, 0x47);
            }
            x += (child->advance + parent->glyphCodeOrContext.byteRoles.spacing) << 4;
            child = child->next;
        }
        parent = parent->next;
    } while (parent != NULL);
}

s32 test_font_Process(void) {
    return 0;
}
