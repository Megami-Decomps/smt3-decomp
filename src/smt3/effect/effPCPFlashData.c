#include "common.h"
#include "kernel_draw.h"

/* The renderer's face packet stores one vertex index in each low byte. */
#define TRI(a, b, c) ((u32)(a) | ((u32)(b) << 8) | ((u32)(c) << 16))

/* VU lqc2 reads all four lanes of these native writable vectors. */
f32 effPCPFlashPolySphere2Vertex[4] __attribute__((aligned(16))) = { 0.0f, 0.0f, 0.0f, 0.0f };
f32 effPCPFlashPolySphereVertex[4] __attribute__((aligned(16))) = { 0.0f, 0.0f, 0.0f, 0.0f };
f32 effPCPFlashPolyRingVertex[4] __attribute__((aligned(16))) = { 0.0f, -1.0f, 0.0f, 0.0f };
f32 effPCPFlashPolyPipeVertex[4] __attribute__((aligned(16))) = { 0.0f, -1.0f, 0.0f, 0.0f };
f32 effPCPFlashPolyRingExpVertex[4] __attribute__((aligned(16))) = { 0.0f, -1.0f, 0.0f, 0.0f };
f32 effPCPFlashPolySphereExpVertex[4] __attribute__((aligned(16))) = { 0.0f, 0.0f, 0.0f, 0.0f };
f32 def_dir[4] __attribute__((aligned(16))) = { 0.0f, 0.0f, 0.0f, 0.0f };
f32 effPCPFlashPolyRingExp2Vertex[4] __attribute__((aligned(16))) = { 0.0f, -1.0f, 0.0f, 0.0f };
f32 effPCPFlashPolyCircleVertex[4] __attribute__((aligned(16))) = { 0.0f, 0.0f, 0.0f, 0.0f };
f32 effPCPFlashPolyRingExpHVertex[4] __attribute__((aligned(16))) = { 0.0f, -1.0f, 0.0f, 0.0f };

u32 effPCPFlashFanIndices[18] __attribute__((aligned(16))) = {
    TRI(0, 3, 2), TRI(0, 1, 3), TRI(1, 4, 3),
    TRI(5, 8, 7), TRI(5, 6, 8), TRI(6, 9, 8),
    TRI(10, 13, 12), TRI(10, 11, 13), TRI(11, 14, 13),
    TRI(15, 18, 17), TRI(15, 16, 18), TRI(16, 19, 18),
    TRI(20, 23, 22), TRI(20, 21, 23), TRI(21, 24, 23),
    TRI(25, 28, 27), TRI(25, 26, 28), TRI(26, 29, 28),
};

sdfGrOt_t *effPCPFlashFanSurfaces[4] = {
    &sdfGrOtTable[43], &sdfGrOtTable[43], &sdfGrOtTable[44], &sdfGrOtTable[45],
};

sdfGrOt_t *effPCPFlashTriangleSurfaces[4] = {
    &sdfGrOtTable[43], &sdfGrOtTable[43], &sdfGrOtTable[44], &sdfGrOtTable[45],
};

u32 effPCPFlashQuadIndices[18] __attribute__((aligned(16))) = {
    TRI(0, 2, 1), TRI(0, 3, 2), TRI(4, 6, 5),
    TRI(4, 7, 6), TRI(8, 10, 9), TRI(8, 11, 10),
    TRI(12, 14, 13), TRI(12, 15, 14), TRI(16, 18, 17),
    TRI(16, 19, 18), TRI(20, 22, 21), TRI(20, 23, 22),
    TRI(24, 26, 25), TRI(24, 27, 26), TRI(28, 30, 29),
    TRI(28, 31, 30), TRI(32, 34, 33), TRI(32, 35, 34),
};

sdfGrOt_t *effPCPFlashQuadSurfaces[4] = {
    &sdfGrOtTable[43], &sdfGrOtTable[43], &sdfGrOtTable[44], &sdfGrOtTable[45],
};

sdfGrOt_t *effPCPFlashFanHSurfaces[4] = {
    &sdfGrOtTable[43], &sdfGrOtTable[43], &sdfGrOtTable[44], &sdfGrOtTable[45],
};
