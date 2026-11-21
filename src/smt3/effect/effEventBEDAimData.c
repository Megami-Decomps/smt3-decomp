#include "common.h"

/* The event and battle BED paths share the same selector encodings. */
f32 effEveBEDModeScale[6] = {
    -1.0f, 0.0f, 1.0f, 1.5f, 0.0f, 0.0f,
};

f32 effEveBEDDirAngle[8] = {
    0.0f, 0x1.921fb4p-1f, 0x1.921fb4p+0f, 0x1.2d97c6p+1f,
    0x1.921fb4p+1f, -0x1.2d97c6p+1f, -0x1.921fb4p+0f, -0x1.921fb4p-1f,
};
