#include "common.h"

/* Per-letter offsets selected by the bounded ASCII glyph lookup. */
s32 frFontUpperYOffset[26] __attribute__((section(".data"))) = { 0 };
s32 frFontUpperXOffset[26] = { ['Y' - 'A'] = 1 };
s32 frFontLowerXOffset[26] = { ['i' - 'a'] = 16, ['k' - 'a'] = 1 };
s32 frFontLowerYOffset[26] = { ['i' - 'a'] = 7, ['l' - 'a'] = 7, ['r' - 'a'] = 1 };
