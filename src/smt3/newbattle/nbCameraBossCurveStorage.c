#include "sdf_types.h"

typedef struct effBezier4PPart_t {
    sdfVector3_t point[4];
    f32 weight;
    f32 step;
} effBezier4PPart_t;

SIZE_CHECK(nbCameraBossCurveSize, effBezier4PPart_t, 0x38);

effBezier4PPart_t nbCameraMuret2Bezier[2] __attribute__((section(".bss")));
effBezier4PPart_t nbCamera_DOUTYO_B4P __attribute__((section(".bss")));
