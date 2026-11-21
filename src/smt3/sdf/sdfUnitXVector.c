#include "common.h"

/* The quaternion fallback loads this writable native vector as one qword. */
f32 sdfUnitXVector[4] __attribute__((aligned(16))) = { 1.0f, 0.0f, 0.0f, 0.0f };
