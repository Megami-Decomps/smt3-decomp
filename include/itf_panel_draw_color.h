#ifndef ITF_PANEL_DRAW_COLOR_H
#define ITF_PANEL_DRAW_COLOR_H

#include "common.h"

/* A color row is also a four-component vector for the template scaler. */
typedef union
{
    u32 components[4];
    struct
    {
        s32 red;
        s32 green;
        s32 blue;
        s32 alpha;
    } rgba;
} itfDrawColor_t;

#endif
