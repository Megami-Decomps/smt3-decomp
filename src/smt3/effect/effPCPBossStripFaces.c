#include "common.h"

/* One packed index word per submitted triangle. */
#define STRIP_FACE(a, b, c) ((u32)(a) | ((u32)(b) << 8) | ((u32)(c) << 16))

u32 D_0031FF20[16] = {
    STRIP_FACE(0, 1, 3), STRIP_FACE(0, 3, 2),
    STRIP_FACE(4, 5, 7), STRIP_FACE(4, 7, 6),
    STRIP_FACE(8, 9, 11), STRIP_FACE(8, 11, 10),
    STRIP_FACE(12, 13, 15), STRIP_FACE(12, 15, 14),
    STRIP_FACE(16, 17, 19), STRIP_FACE(16, 19, 18),
    STRIP_FACE(20, 21, 23), STRIP_FACE(20, 23, 22),
    STRIP_FACE(24, 25, 27), STRIP_FACE(24, 27, 26),
    STRIP_FACE(28, 29, 31), STRIP_FACE(28, 31, 30),
};

/* Retained face words following the submitted list. */
u32 D_0031FF60[4] = {
    STRIP_FACE(32, 33, 35), STRIP_FACE(32, 35, 34),
    STRIP_FACE(36, 37, 39), STRIP_FACE(36, 39, 38),
};

#undef STRIP_FACE
