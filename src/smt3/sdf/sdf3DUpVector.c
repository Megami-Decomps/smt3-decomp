#include "common.h"

/* Native camera up vector, loaded by the VU0 as a single qword. */
f32 sdf3DUpVector[4] __attribute__((aligned(16))) = { 0.0f, 1.0f, 0.0f, 0.0f };
