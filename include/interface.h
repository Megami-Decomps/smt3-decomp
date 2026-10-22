#ifndef INTERFACE_H
#define INTERFACE_H

#include "common.h"
#include "libsdf.h"
#include "font.h"

/* Common draw record allocated by the interface object constructors (0x34). */
/* Allocation fields own general-heap descriptors, not retained payload addresses. */
typedef struct itfpanel_t {
    sdfMemHandle_t *allocation;
    sdfMemHandle_t *payloadAllocation;
    u32 *pwork;
    s32 renderValue;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    /* RGBA words consumed by the panel packet renderers. */
    s32 red;
    s32 green;
    s32 blue;
    s32 alpha;
    u8 kind; /* Unsigned index into the panel handler tables. */
    u8 unk31[3];
} itfpanel_t;

/* Point written by the panel vertex builders. */
typedef struct sdfIVector2_t {
    s32 x;
    s32 y;
} sdfIVector2_t;

#endif /* INTERFACE_H */
