#include "sdf_types.h"

SIZE_CHECK(fldNpcScratchSize, sdfVector4_t, 16);

/* Native NPC code reads and writes this scratch as a VU quadword. */
sdfVector4_t fldNpcWorkVec __attribute__((aligned(16))) = { 0.0f, 0.0f, 0.0f, 1.0f };
