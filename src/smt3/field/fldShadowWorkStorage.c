#include "fldShadowWorkStorage.h"

SIZE_CHECK(fldSdwTriangleSize, fldSdwTriangle_t, 0x24);
SIZE_CHECK(fldSdwEdgeSize, fldSdwEdge_t, 0x02);

fldSdwTriangle_t fldSdwTriangles[50] __attribute__((section(".bss"), aligned(16)));
fldSdwEdge_t fldShadowVertexTable[26][26] __attribute__((section(".bss"), aligned(2)));
