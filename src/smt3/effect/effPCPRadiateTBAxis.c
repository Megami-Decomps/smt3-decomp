#include "common.h"

/* The radial draw loads this axis twice through VU0. */
f32 effPCPRadiateTBParam[4] __attribute__((aligned(16))) = {
    0.0f, 1.0f, 0.0f, 0.0f,
};
