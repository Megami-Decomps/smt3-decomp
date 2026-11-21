#include "common.h"

typedef struct
{
    u32 components[4];
} evtPictureColor_t;

s32 evtPictureSpritePos[4] = { 0, 0, 0x2000, 0x0E00 };
s32 evtPictureSpriteUv[4] = { 0, 0, 0x2000, 0x1C00 };
evtPictureColor_t evtPictureSpriteColor = { { 0x80, 0x80, 0x80, 0x7F } };
