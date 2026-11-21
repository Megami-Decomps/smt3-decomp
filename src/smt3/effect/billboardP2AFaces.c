#include "common.h"

/* Two native triangles reference four points of the P2A quad. */
u32 dds3P2APolygonFaceIndex[2] __attribute__((section(".data"))) = {
    0x00030100, 0x00020300,
};
