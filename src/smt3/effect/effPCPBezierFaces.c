#include "common.h"

/* One packed index word per triangle; the upper byte is unused. */
#define BEZIER_FACE(a, b, c) ((u32)(a) | ((u32)(b) << 8) | ((u32)(c) << 16))

u32 dds3BezierVertexCountIndices[28] = {
    BEZIER_FACE(0, 1, 4), BEZIER_FACE(0, 4, 3), BEZIER_FACE(1, 2, 5), BEZIER_FACE(1, 5, 4),
    BEZIER_FACE(3, 4, 7), BEZIER_FACE(3, 7, 6), BEZIER_FACE(4, 5, 8), BEZIER_FACE(4, 8, 7),
    BEZIER_FACE(6, 7, 10), BEZIER_FACE(6, 10, 9), BEZIER_FACE(7, 8, 11), BEZIER_FACE(7, 11, 10),
    BEZIER_FACE(9, 10, 13), BEZIER_FACE(9, 13, 12), BEZIER_FACE(10, 11, 14), BEZIER_FACE(10, 14, 13),
    BEZIER_FACE(12, 13, 16), BEZIER_FACE(12, 16, 15), BEZIER_FACE(13, 14, 17), BEZIER_FACE(13, 17, 16),
    BEZIER_FACE(15, 16, 19), BEZIER_FACE(15, 19, 18), BEZIER_FACE(16, 17, 20), BEZIER_FACE(16, 20, 19),
    BEZIER_FACE(18, 19, 22), BEZIER_FACE(18, 22, 21), BEZIER_FACE(19, 20, 23), BEZIER_FACE(19, 23, 22),
};

u32 dds3BezierTrackPolyVertexIndices[6] = {
    BEZIER_FACE(0, 1, 2), BEZIER_FACE(0, 2, 3), BEZIER_FACE(0, 3, 4),
    BEZIER_FACE(0, 4, 5), BEZIER_FACE(0, 5, 6), BEZIER_FACE(0, 6, 7),
};

#undef BEZIER_FACE
