#include "sdf_types.h"

SIZE_CHECK(PbMapBaseVectorSize, sdfVector4_t, 0x10);

/* pbBuildMapBaseVerts writes six vertices and normals for each of six faces. */
sdfVector4_t pbMapBaseVerts[36] __attribute__((section(".bss"), aligned(16)));
sdfVector4_t pbMapBaseNormals[36] __attribute__((section(".bss"), aligned(16)));
