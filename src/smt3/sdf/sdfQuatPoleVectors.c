#include "common.h"

#define SDF_QUAT_QUARTER_TURN_COMPONENT 0.707106769084930419921875f

/* Native rotations for the two pole cases in the direction-to-quaternion path. */
f32 sdfQuatPoleDown[4] __attribute__((aligned(16))) = {
    SDF_QUAT_QUARTER_TURN_COMPONENT, 0.0f, 0.0f, SDF_QUAT_QUARTER_TURN_COMPONENT,
};
f32 sdfQuatPoleUp[4] __attribute__((aligned(16))) = {
    SDF_QUAT_QUARTER_TURN_COMPONENT, 0.0f, 0.0f, -SDF_QUAT_QUARTER_TURN_COMPONENT,
};
