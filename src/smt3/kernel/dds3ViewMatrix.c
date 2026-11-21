#include "sdf_types.h"

SIZE_CHECK(dds3MatrixSize, sdfMatrix4_t, 64);

/* The renderer fills the initially empty view matrix via SQC2/LQC2 paths. */
sdfMatrix4_t sdfViewMatrix __attribute__((aligned(16))) = { 0 };
