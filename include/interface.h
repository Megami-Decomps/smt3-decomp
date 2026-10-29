#ifndef INTERFACE_H
#define INTERFACE_H

#include "common.h"
#include "camp.h"
#include "facility.h"
#include "libsdf.h"
#include "font.h"

typedef struct lstWindowWork_t lstWindowWork_t;

typedef struct
{
    u8 unk_0000[9];
    u8 unk_0009[0x17];
    s32 flags;
    u8 unk_0024[0x1C];
    union {
        struct {
            fclPanelPos_t pos;
            u8 unk_0060[0xFC];
        };
        struct {
            u8 panelPos[0x14];
            s32 unk_54;
            u8 unk_58;
            u8 slotFlags[0x100];
            u8 unk_0159[3];
        };
    };
    cmpCursorInfo_t listCursor;
} lstWork_t;

typedef struct {
    u32 components[4];
} itfColor_t;

/* Common draw record allocated by the interface object constructors (0x34). */
/* Allocation fields own general-heap descriptors, not retained payload addresses. */
typedef struct itfpanel_t {
    sdfMemHandle_t *allocation;
    sdfMemHandle_t *payloadAllocation;
    u32 *pwork;
    s32 z;
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
#endif /* INTERFACE_H */
