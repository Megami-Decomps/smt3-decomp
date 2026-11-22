#include "common.h"

/* Terminates the empty encoded glyph sequence used by the message analyzer. */
u8 frEmptyString[] __attribute__((section(".sdata"))) = { 0 };

INCLUDE_SDATA(const s32, "interface/frFontEmptyStrings", D_0054F438);

