#include "common.h"

/* Capture and draw visit the seven sample points in model-marker order.
 * The eighth slot is unused but is part of each native table. */
u16 effPCPMudoSampleStep[8] = { 1, 1, 10, 1, 8, 8, 10, 0 };
f32 effPCPMudoSampleScale[8] = {
    1.0f, 1.0f, 0.25f, 0.75f, 0.5f, 0.5f, 0.25f, 0.0f,
};
