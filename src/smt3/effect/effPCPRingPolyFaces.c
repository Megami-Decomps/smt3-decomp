#include "common.h"

/* One packed index word per submitted triangle. */
#define STRIP_FACE(a, b, c) ((u32)(a) | ((u32)(b) << 8) | ((u32)(c) << 16))

u32 dds3RingPolyExpListParameters[12] = {
    STRIP_FACE(0, 1, 5), STRIP_FACE(0, 5, 4),
    STRIP_FACE(1, 2, 6), STRIP_FACE(1, 6, 5),
    STRIP_FACE(2, 3, 7), STRIP_FACE(2, 7, 6),
    STRIP_FACE(4, 5, 9), STRIP_FACE(4, 9, 8),
    STRIP_FACE(5, 6, 10), STRIP_FACE(5, 10, 9),
    STRIP_FACE(6, 7, 11), STRIP_FACE(6, 11, 10),
};

/* Retained face words following the submitted list. */
u32 D_0031FEE0[6] = {
    STRIP_FACE(8, 9, 13), STRIP_FACE(8, 13, 12),
    STRIP_FACE(9, 10, 14), STRIP_FACE(9, 14, 13),
    STRIP_FACE(10, 11, 15), STRIP_FACE(10, 15, 14),
};

#undef STRIP_FACE
