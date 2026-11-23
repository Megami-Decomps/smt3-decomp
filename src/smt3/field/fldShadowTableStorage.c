#include "sdf_types.h"

SIZE_CHECK(fldShadowVectorSize, sdfVector4_t, 0x10);

/* Native shadow-edge position grids, each row contains 26 VU vectors. */
sdfVector4_t fldShadowTable[26][26] __attribute__((section(".bss"), aligned(16)));
sdfVector4_t fldShadowTableB[26][26] __attribute__((section(".bss"), aligned(16)));
