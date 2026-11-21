#ifndef MDL_DEBUG_DATA_H
#define MDL_DEBUG_DATA_H

#include "common.h"

struct sdfDrawTag_t;

typedef struct mdlDebugLight_t {
    f32 (**lights)[4];
    f32 *ambient;
    s8 *pad;
    u8 editing;
    u8 blinkTick;
    s16 selection;
    struct sdfDrawTag_t *packetList;
} mdlDebugLight_t;

typedef struct mdlDebugLabel_t {
    u16 x;              /* 0x00 */
    u16 y;              /* 0x02 */
    s32 style;          /* 0x04 */
    const char *format; /* 0x08 */
} mdlDebugLabel_t;

#endif
