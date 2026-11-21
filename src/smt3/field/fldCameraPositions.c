#include "common.h"

/* Native camera copies load and store each complete 16-byte vector. */
typedef f32 fldCameraPosition_t[4] __attribute__((aligned(16)));

fldCameraPosition_t fldCamEye = { 0.0f };
fldCameraPosition_t fldCamTarget = { 0.0f };
