#include "common.h"
#include "kernel_draw.h"

/* The renderer's face packet stores one vertex index in each low byte. */
#define TRI(a, b, c) ((u32)(a) | ((u32)(b) << 8) | ((u32)(c) << 16))

u32 effPCPScatterParams[20] __attribute__((aligned(16))) = {
    TRI(0, 1, 4), TRI(0, 4, 3), TRI(1, 2, 5),
    TRI(1, 5, 4), TRI(6, 7, 10), TRI(6, 10, 9),
    TRI(7, 8, 11), TRI(7, 11, 10), TRI(12, 13, 16),
    TRI(12, 16, 15), TRI(13, 14, 17), TRI(13, 17, 16),
    TRI(18, 19, 22), TRI(18, 22, 21), TRI(19, 20, 23),
    TRI(19, 23, 22), TRI(24, 25, 28), TRI(24, 28, 27),
    TRI(25, 26, 29), TRI(25, 29, 28),
};

/* 24 consecutive UV pairs passed to the polygon packet builder. */
f32 effPCPScatterTexcoords[48] = {
    1.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.5f, 0.0f,
    0.0f, 0.0f, 1.0f, 1.0f, 0.5f, 1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.5f, 0.0f,
    0.0f, 0.0f, 1.0f, 1.0f, 0.5f, 1.0f, 0.0f, 1.0f,
};

sdfGrOt_t *effPCPScatterSurfaceByPool[4] = {
    &sdfGrOtTable[43], &sdfGrOtTable[43], &sdfGrOtTable[44], &sdfGrOtTable[45],
};

u32 effScatterTriangleIndices[24] = {
    TRI(0, 1, 3), TRI(0, 3, 2), TRI(2, 3, 5),
    TRI(2, 5, 4), TRI(4, 5, 7), TRI(4, 7, 6),
    TRI(6, 7, 9), TRI(6, 9, 8), TRI(8, 9, 11),
    TRI(8, 11, 10), TRI(10, 11, 13), TRI(10, 13, 12),
    TRI(12, 13, 15), TRI(12, 15, 14), TRI(14, 15, 17),
    TRI(14, 17, 16), TRI(16, 17, 19), TRI(16, 19, 18),
    TRI(18, 19, 21), TRI(18, 21, 20), TRI(20, 21, 23),
    TRI(20, 23, 22), TRI(22, 23, 25), TRI(22, 25, 24),
};

sdfGrOt_t *effPCPScatterSurfaceByQueue[4] = {
    &sdfGrOtTable[43], &sdfGrOtTable[43], &sdfGrOtTable[44], &sdfGrOtTable[45],
};
