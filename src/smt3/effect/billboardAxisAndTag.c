#include "common.h"

/* Draw paths pass these four-float slots to VU0 quadword loads.
 * Only the two tag vectors have their second lanes replaced during drawing. */
f32 dds3BillboardUpAxis[4] __attribute__((aligned(16))) = { 0.0f, 1.0f, 0.0f, 0.0f };
f32 dds3BillboardTagParam[4] __attribute__((aligned(16))) = { 1.0f, 1.0f, 1.0f, 0.0f };
