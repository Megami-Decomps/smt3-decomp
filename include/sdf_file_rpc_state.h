#ifndef SDF_FILE_RPC_STATE_H
#define SDF_FILE_RPC_STATE_H

#include "common.h"

struct sdfFileCmdHandle_t;

typedef struct sdfFileRpcCb_t {
    struct sdfFileCmdHandle_t *deviceState;
    s32 semaphore;
    s32 readResult;
    s32 remainingBytes;
    u8 *destination; /* +0x10: current DMA destination; advanced per completed chunk. */
    s32 transferredBytes;
} sdfFileRpcCb_t;

SIZE_CHECK(sdfFileRpcStateSize, sdfFileRpcCb_t, 0x18);

#endif
