#include "sdf_math_state.h"

SIZE_CHECK(dds3RandStateSize, sdfRandCB_t, 16);

/* The kernel seeds this four-word generator in dds3InitRandCB. */
sdfRandCB_t dds3RandCB __attribute__((section(".data"))) = { { 0 } };
