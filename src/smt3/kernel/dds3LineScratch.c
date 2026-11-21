#include "sdf_types.h"

/* Two mutable vertices and their packed colors for eglLinkLine. */
sdfVector4_t eglLineVerts[2] __attribute__((aligned(16))) = {
    { 0.0f, 100.0f, 100.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f },
};
u32 eglLineColors[2] __attribute__((section(".data"))) = { 0x80800080, 0x80800080 };
