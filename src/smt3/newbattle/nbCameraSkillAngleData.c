#include "common.h"

/* Eight skill-camera angle presets per axis, in radians. */
f32 nbCameraAngleHTable[8] = {
    0.0f, 0.349065811f, 0.2617993355f, 0.174532906f,
    0.0f, -0.349065811f, -0.2617993355f, -0.174532906f,
};

f32 nbCameraAngleWTable[8] = {
    0.0f, 0.78539807f, 0.5235986710f, 0.2617993355f,
    0.0f, -0.78539807f, -0.5235986710f, -0.2617993355f,
};
