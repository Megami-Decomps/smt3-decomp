#include "common.h"
#include "sdf_types.h"

sdfVector4_t effBTLDefaultColor[1] __attribute__((section(".data"), aligned(16))) = { { 0.0f, 1.0f, 1.0f, 1.0f } };
f32 effBTLModelDirFwd[4] __attribute__((section(".data"), aligned(16))) = { 0.0f, 0.0f, 1.0f, 0.0f };
f32 effBTLModelDirBack[4] __attribute__((section(".data"), aligned(16))) = { 0.0f, 0.0f, -1.0f, 0.0f };
f32 effBTLModelLocalZ[4] __attribute__((section(".data"), aligned(16))) = { 0.0f, 0.0f, 1.0f, 0.0f };
f32 effBTLCircumferenceAxisZ[4] __attribute__((section(".data"), aligned(16))) = { 0.0f, 0.0f, 1.0f, 0.0f };
f32 effBTLCircumferenceAngleTable[8] __attribute__((section(".data"), aligned(4))) = { 0.0f, 0.785398126f, 1.5707962512969970703125f, 2.35619426f, 3.141592502593994140625f, -2.35619426f, -1.5707962512969970703125f, -0.785398126f };
f32 effBTLZeroVector[4] __attribute__((section(".data"), aligned(16))) = { 0.0f, 0.0f, 0.0f, 0.0f };
f32 effBTLGroupDirFwd[4] __attribute__((section(".data"), aligned(16))) = { 0.0f, 0.0f, 1.0f, 0.0f };
f32 effBTLGroupDirBack[4] __attribute__((section(".data"), aligned(16))) = { 0.0f, 0.0f, -1.0f, 0.0f };
f32 effBTLAimVerticalScale[6] __attribute__((section(".data"), aligned(4))) = { -1.0f, 0.0f, 1.0f, 1.5f, 0.0f, 0.0f };
f32 effBTL_Ptr_D[8] __attribute__((section(".data"), aligned(4))) = { 0.0f, 0.785398126f, 1.5707962512969970703125f, 2.35619426f, 3.141592502593994140625f, -2.35619426f, -1.5707962512969970703125f, -0.785398126f };
