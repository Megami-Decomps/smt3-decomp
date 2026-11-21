#ifndef SDF_FILE_DEVICE_H
#define SDF_FILE_DEVICE_H

#include "common.h"

struct sdfFileCmdHandle_t;

typedef struct sdfFileDeviceCB_t {
    s32 thId;
    s32 semaId;
    struct sdfFileCmdHandle_t *first;
    struct sdfFileCmdHandle_t *last;
    s32 count;
    s32 blockSize;
} sdfFileDeviceCB_t;

SIZE_CHECK(sdfFileDeviceCBSize, sdfFileDeviceCB_t, 24);
extern sdfFileDeviceCB_t sdfFileDeviceCB[4];

#endif
