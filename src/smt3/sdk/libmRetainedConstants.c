#include "common.h"

/* Retained single-precision acosf and atan2f constants, in native order. */
/* Explicit readonly placement keeps scalar constants out of the small-data area. */
const f32 sdkAcosfOne __attribute__((section(".rodata"))) = 1.0f;
const f32 sdkAcosfPi __attribute__((section(".rodata"))) = 3.1415926f;
const f32 sdkAcosfPiOver2Hi __attribute__((section(".rodata"))) = 1.5707963f;
const f32 sdkAcosfPiOver2Lo __attribute__((section(".rodata"))) = 7.5497895e-8f;
const f32 sdkAcosfNumer0 __attribute__((section(".rodata"))) = 0.16666666f;
const f32 sdkAcosfNumer1 __attribute__((section(".rodata"))) = -0.3255658f;
const f32 sdkAcosfNumer2 __attribute__((section(".rodata"))) = 0.20121253f;
const f32 sdkAcosfNumer3 __attribute__((section(".rodata"))) = -0.040055536f;
const f32 sdkAcosfNumer4 __attribute__((section(".rodata"))) = 0.00079153502f;
const f32 sdkAcosfNumer5 __attribute__((section(".rodata"))) = 0.00003479331f;
const f32 sdkAcosfDenom1 __attribute__((section(".rodata"))) = -2.4033947f;
const f32 sdkAcosfDenom2 __attribute__((section(".rodata"))) = 2.0209456f;
const f32 sdkAcosfDenom3 __attribute__((section(".rodata"))) = -0.68828393f;
const f32 sdkAcosfDenom4 __attribute__((section(".rodata"))) = 0.07703815f;

const f32 sdkAtan2fTiny __attribute__((section(".rodata"))) = 9.9999991e-31f;
const f32 sdkAtan2fZero __attribute__((section(".rodata"))) = 0.0f;
const f32 sdkAtan2fPiOver4 __attribute__((section(".rodata"))) = 0.78539813f;
const f32 sdkAtan2fPiOver2 __attribute__((section(".rodata"))) = 1.5707963f;
const f32 sdkAtan2fPi __attribute__((section(".rodata"))) = 3.1415926f;
const f32 sdkAtan2fPiLo __attribute__((section(".rodata"))) = 1.5099579e-7f;
