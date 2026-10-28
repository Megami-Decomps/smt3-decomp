#include "common.h"
#include "font.h"
#include "libsdf.h"
#include "kernel.h"

INCLUDE_ASM(const s32, "interface/frName", frInitNameData);

INCLUDE_ASM(const s32, "interface/frName", frSetNameData);

INCLUDE_ASM(const s32, "interface/frName", frTransNameImage);

INCLUDE_ASM(const s32, "interface/frName", frRestoreNameImage);

INCLUDE_ASM(const s32, "interface/frName", frCreateNameHandle);

INCLUDE_ASM(const s32, "interface/frName", frGetName1NFRQ);

INCLUDE_ASM(const s32, "interface/frName", frGetName2NFRQ);

extern frFontHandle_t *frGetName1NFRQ(s8 target, frFontHandle_t *chain, s32 firstOption);
extern frFontHandle_t *frGetName2NFRQ(s8 target, frFontHandle_t *chain, s32 firstOption);

frFontHandle_t *frGetNameFRQ(s8 target, frFontHandle_t *chain, s32 firstOption)
{
    frFontHandle_t *name = frGetName2NFRQ(target, chain, firstOption);
    name->advance += 8;
    name->lastChild->advance += 8;
    return frGetName1NFRQ(target, name, firstOption);
}

INCLUDE_ASM(const s32, "interface/frName", frGetCNameFRQ);

INCLUDE_SDATA(const s32, "interface/frName", D_0054F440);

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

