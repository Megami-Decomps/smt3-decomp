#ifndef SDK_CDVD_STREAM_REQUEST_H
#define SDK_CDVD_STREAM_REQUEST_H

#include "common.h"

typedef struct cdvdStreamRequest_t {
    u32 lsn;
    u32 sectors;
    void *buffer;
    s32 command;
    u8 mode[3];
    u8 opaque13;
} cdvdStreamRequest_t;

#endif
