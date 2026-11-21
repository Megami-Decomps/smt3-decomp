#include "sdf_types.h"

SIZE_CHECK(dds3MatrixSize, sdfMatrix4_t, 64);

/* The frame setup stores each projection matrix as four VU quadwords. */
sdfMatrix4_t dds3RenderViewportMatrix __attribute__((aligned(16))) = { 0 };
sdfMatrix4_t aProjMatrix __attribute__((aligned(16))) = { 0 };
