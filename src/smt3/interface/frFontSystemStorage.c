#include "frFontSystemStorage.h"

SIZE_CHECK(frFontSystemSize, frFontSystem_t, 0x19C);

/* Font style entries, cached item bookkeeping, and the shared glyph atlas. */
frFontSystem_t gFT __attribute__((section(".bss"), aligned(4)));
