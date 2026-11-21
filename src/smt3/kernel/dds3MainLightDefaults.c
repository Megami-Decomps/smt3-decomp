#include "draw3d.h"

SIZE_CHECK(dds3MainLightSize, sdf3DLight_t, 0x40);

/* The primary route's parallel lights. The packet builder loads each
 * color and direction as quadwords; all three 64-byte records follow
 * the alignment of the first. */
sdf3DLight_t fldFixedLight __attribute__((aligned(16))) = {
    { 1.0f, 1.0f, 1.0f, 0.0f },
    { 0.0f, 1.0f, 0.5f, 0.0f },
};
sdf3DLight_t D_002F5AC0 = {
    { 0 },
    { 1.0f, 0.0f, 0.0f, 0.0f },
};
sdf3DLight_t D_002F5B00 = {
    { 0 },
    { 0.0f, 0.0f, 1.0f, 0.0f },
};
