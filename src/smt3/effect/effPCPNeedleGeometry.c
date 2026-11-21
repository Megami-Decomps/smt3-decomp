#include "common.h"

/* LookAtMatrix loads this up vector with a VU quadword load (lqc2). */
f32 dds3NeedleModelUpVector[4] __attribute__((aligned(16))) = { 0.0f, 1.0f, 0.0f, 0.0f };

/* Four fallback triangles; effPkt3DPrimMakePoly loads each row with lq. */
f32 dds3NeedleModelDefaultPoints[12][4] __attribute__((aligned(16))) = {
    { 0.0f, 0.0f, 0.0f, 0.0f },
    { -2.5f, 0.0f, 20.0f, 0.0f },
    { 0.0f, -2.5f, 20.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, -2.5f, 20.0f, 0.0f },
    { 2.5f, 0.0f, 20.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f },
    { -2.5f, 0.0f, 20.0f, 0.0f },
    { 0.0f, 2.5f, 20.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 2.5f, 20.0f, 0.0f },
    { 2.5f, 0.0f, 20.0f, 0.0f },
};

/* The three vertices of each face share an ABGR color. */
u32 dds3NeedleModelDefaultColors[12] = {
    0x80808080, 0x80808080, 0x80808080,
    0x80707070, 0x80707070, 0x80707070,
    0x80606060, 0x80606060, 0x80606060,
    0x80505050, 0x80505050, 0x80505050,
};
