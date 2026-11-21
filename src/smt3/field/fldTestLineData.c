#include "sdf_types.h"

sdfVector4_t fldTestLineVertex[2] __attribute__((aligned(16))) = {
    { 0.0f, 100.0f, 100.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f },
};

u32 fldTestLineColor[2] __attribute__((section(".data"))) = { 0x80800080, 0x80800080 };
