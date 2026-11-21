#include "eff_camp_bad_params.h"

/* Writable aligned work populated by cmpCreateBadEffect before use. */
typedef char cmpBadEffectParamsSize[sizeof(effCAMPBadParams_t) == 0x40 ? 1 : -1];
effCAMPBadParams_t D_00349300 __attribute__((section(".data"), aligned(16))) = { 0 };
