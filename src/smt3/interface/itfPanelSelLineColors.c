#include "common.h"

/* DrawSelWin submits exactly two four-point border strips. */
u8 itfPanelSelWinLineColorIndex[8] __attribute__((section(".data"))) = {
    0, 2, 2, 0,
    0, 3, 3, 0,
};
