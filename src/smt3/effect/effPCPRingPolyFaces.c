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

#undef STRIP_FACE
