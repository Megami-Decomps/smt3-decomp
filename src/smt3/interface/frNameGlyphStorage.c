#include "common.h"
#include "font.h"

SIZE_CHECK(frNameGlyphPointerSize, GsFont_t *, 4);

GsFont_t *pFont[4][16] __attribute__((section(".bss"), aligned(16)));
s32 frNameRefCount[4] __attribute__((section(".bss"), aligned(16)));
