#include "common.h"

/* Previous resolved XYZ position shared by the field collision checks. */
f32 fldHitChkWork[3] __attribute__((section(".data"))) = { 0.0f };
