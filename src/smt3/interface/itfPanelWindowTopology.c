#include "common.h"

/* Three eight-index wide strips, and six four-index message borders. */
u8 itfPanelMesWinStripVertexIndex[24] = {
    0, 1, 4, 5, 8, 9, 12, 13,
    1, 2, 5, 6, 9, 10, 13, 14,
    2, 3, 6, 7, 10, 11, 14, 15,
};

u8 itfPanelMesWinStripColorIndex[24] = {
    0, 0, 0, 1, 0, 1, 0, 0,
    0, 0, 1, 1, 1, 1, 0, 0,
    0, 0, 1, 0, 1, 0, 0, 0,
};

u8 itfPanelMesWinLineColorIndex[24] = {
    0, 2, 2, 0,
    0, 3, 3, 0,
    0, 4, 4, 0,
    0, 5, 5, 0,
    0, 6, 6, 0,
    0, 7, 7, 0,
};

/* The selection window draws the same three wide strips. */
u8 itfPanelSelWinStripVertexIndex[24] = {
    0, 1, 4, 5, 8, 9, 12, 13,
    1, 2, 5, 6, 9, 10, 13, 14,
    2, 3, 6, 7, 10, 11, 14, 15,
};

u8 itfPanelSelWinStripColorIndex[24] = {
    0, 0, 0, 1, 0, 1, 0, 0,
    0, 0, 1, 1, 1, 1, 0, 0,
    0, 0, 1, 0, 1, 0, 0, 0,
};
