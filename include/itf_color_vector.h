#ifndef ITF_COLOR_VECTOR_H
#define ITF_COLOR_VECTOR_H

#include "common.h"

typedef struct { s32 x, y, z, w; } itfColorVector_t;

void itfMiscSetRatioRGB(itfColorVector_t *output, s32 scaleX, s32 scaleY,
                        s32 scaleZ, s32 w, const itfColorVector_t *input, s32 count);

#endif
