#include "common.h"

/* Packed 16-bit U and V for the console's 12-pixel glyph cells (four fractional bits). */
#define GLYPH_UV(col, row) (((u32)(row) * 0xC0 << 16) | ((u32)(col) * 0xC0))
#define GLYPH_ROW(row) \
    GLYPH_UV(0, row), GLYPH_UV(1, row), GLYPH_UV(2, row), GLYPH_UV(3, row), GLYPH_UV(4, row), \
    GLYPH_UV(5, row), GLYPH_UV(6, row), GLYPH_UV(7, row), GLYPH_UV(8, row), GLYPH_UV(9, row)

u32 sdfDevConsFontUvTable[96] = {
    GLYPH_ROW(0), GLYPH_ROW(1), GLYPH_ROW(2),
    GLYPH_ROW(3), GLYPH_ROW(4), GLYPH_ROW(5),
    GLYPH_ROW(6), GLYPH_ROW(7), GLYPH_ROW(8),
    GLYPH_UV(0, 9), GLYPH_UV(1, 9), GLYPH_UV(2, 9),
    GLYPH_UV(3, 9), GLYPH_UV(4, 9), GLYPH_UV(5, 9),
};

#undef GLYPH_ROW
#undef GLYPH_UV

/* Native packed RGBA for the console attribute and packet mode indices. */
u32 sdfDevConsColorTable[15] = {
    0x80808080, 0x80800000, 0x80000080, 0x80800080, 0x80008000,
    0x80808000, 0x80008080, 0x80606060, 0x80202020, 0x80400000,
    0x80000040, 0x80400040, 0x80004000, 0x80404000, 0x80004040,
};
