#include "effect.h"
#include "sdf_types.h"

/* The first axis and subsequent 16-byte vectors feed native VU0 loads.
 * Their 16-byte alignment is maintained by this section's real first axis. */
sdfVector4_t nbCameraUnitAxisX __attribute__((aligned(16))) = { 1.0f, 0.0f, 0.0f, 0.0f };

dds3Effect_Blur_t nbCameraPressBlurBit8Clear = {
    .abgr = 0x60808080, .blend = 0x44, .scale = 0.955f,
    .xy = {{ 0, 0 }, { 512, 448 }},
};
dds3Effect_Blur_t nbCameraPressBlurBit8Set = {
    .abgr = 0x60808080, .blend = 0x44, .scale = 1.045f,
    .xy = {{ 0, 0 }, { 512, 448 }},
};

sdfVector4_t nbCameraForuE913TargetStart = { 1209.42996f, -167.68999f, -881.04f, 0.0f };
sdfVector4_t nbCameraForuE913DirectionStart = { -0.58f, -0.29999999f, 0.76f, 0.0f };
sdfVector4_t nbCameraForuE913TargetEnd = { 1289.60999f, -155.29f, -1037.5199f, 0.0f };
sdfVector4_t nbCameraForuE913DirectionEnd = { -0.569999993f, -0.11f, 0.809999943f, 0.0f };
sdfVector4_t nbCameraKaguStartTarget = { 55.91f, -1888.64993f, -1254.71985f, 0.0f };
sdfVector4_t nbCameraKaguStartDirection = { -0.0f, 0.419999987f, 0.909999967f, 0.0f };
sdfVector4_t nbCameraBossEx02AxisX = { 1.0f, 0.0f, 0.0f, 0.0f };
