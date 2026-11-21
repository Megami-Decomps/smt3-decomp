#include "field.h"
#include "sdf_math_state.h"

SIZE_CHECK(fldSceneParamSize, fldSceneParam_t, 0xA0);

/* fldInitAllValue resets the two leading qwords with VU stores. */
fldSceneParam_t fldSceneParam __attribute__((aligned(16))) = { 0 };
sdfRandCB_t fldRandomBuf = { { 0 } };
sdfVector4_t fldPlayerLastPos = { 0.0f, 0.0f, 0.0f, 0.0f };
