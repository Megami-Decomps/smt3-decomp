#ifndef SDK_MALLOC_INFO_H
#define SDK_MALLOC_INFO_H

#include "common.h"

typedef struct mallocInfo_t {
    s32 arena;
    u8 opaque04[0x24];
} mallocInfo_t;

#endif
