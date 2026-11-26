#ifndef SDK_LIBKERNL_FS_IOB_H
#define SDK_LIBKERNL_FS_IOB_H

#include "common.h"

typedef struct {
    u32 word0;
    s32 active;
    u8 opaque[8];
} fsRequestEntry_t;

SIZE_CHECK(fs_request_entry_size, fsRequestEntry_t, 0x10);
extern fsRequestEntry_t fsIobTable[32];

#endif
