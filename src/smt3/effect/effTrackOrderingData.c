#include "kernel_draw.h"

/* Each group of two triangles joins adjacent stride-two track edges. */
u32 effModelTrackPolyFaceIndex[16] = {
    0x00030100, 0x00020300, 0x00050302, 0x00040502,
    0x00070504, 0x00060704, 0x00090706, 0x00080906,
    0x000B0908, 0x000A0B08, 0x000D0B0A, 0x000C0D0A,
    0x000F0D0C, 0x000E0F0C, 0x00110F0E, 0x0010110E,
};

extern sdfGrOt_t sdfGrOtTable[85];

sdfGrOt_t *effModelTrackPolyBlendOt[4] = {
    &sdfGrOtTable[43], &sdfGrOtTable[43], &sdfGrOtTable[44], &sdfGrOtTable[45],
};

sdfGrOt_t *effMagaGrOtBuckets[4] = {
    &sdfGrOtTable[43], &sdfGrOtTable[43], &sdfGrOtTable[44], &sdfGrOtTable[45],
};
