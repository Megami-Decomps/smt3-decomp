#include "sdf_types.h"

/* The model-panel generator writes one color word per vertex. */
u32 cmbModelPanelColors[2][384] __attribute__((section(".bss"), aligned(16)));
