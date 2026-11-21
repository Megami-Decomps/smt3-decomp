#include "itf_color_vector.h"

/* Draw-time RGB scaling uses these four-word fixed-point color rows. */
itfColorVector_t itfPanelSelWinColorRatio[4] = {
    { 0x29999, 0x29999, 0x29999, 0x1000 },
    { 0x10000, 0x10000, 0x10000, 0x1000 },
    { 0x29999, 0x29999, 0x29999, 0x1000 },
    { 0x10000, 0x10000, 0x10000, 0x1000 },
};

itfColorVector_t itfPanelPlainColorRatio[1] = {
    { 0x2000, 0x2000, 0x2000, 0x1000 },
};
