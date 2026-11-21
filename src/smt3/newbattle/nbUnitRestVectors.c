#include "sdf_types.h"

/* Party pose registration copies a quadword; the live scale update loads
 * its base vector into VU0. Both sources require quadword alignment. */
f32 nbPartyRotation[4] __attribute__((aligned(16))) = { 0.0f, 1.0f, 0.0f, 0.0f };
sdfVector4_t nbUnitBaseScale __attribute__((aligned(16))) = { 1.0f, 1.0f, 1.0f, 0.0f };
