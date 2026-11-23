#include "sdf_types.h"

SIZE_CHECK(cmbPanelVertexSize, sdfVector4_t, 0x10);

/* Two active model banks, with six vertices per angular segment. */
sdfVector4_t cmbModelPanelVertices[2][384] __attribute__((section(".bss"), aligned(16)));
