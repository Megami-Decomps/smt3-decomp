#ifndef SDF_SOUND_RUNTIME_STORAGE_H
#define SDF_SOUND_RUNTIME_STORAGE_H

#include "sifDmaTransfer.h"

typedef struct sndListenerState_t {
    s32 opaque00[2];
    s32 scaledValues[6];
} sndListenerState_t;

typedef struct sndFileRequestState_t {
    u8 reserved00[0x100];
    s32 size;
    s32 pending;
    s32 data;
} sndFileRequestState_t;

typedef struct sdfSndInitThreadParam_t {
    s32 status;
    void (*entry)(void);
    void *stack;
    s32 stackSize;
    void *gp;
    s32 priority;
    u8 reserved[0x18];
} sdfSndInitThreadParam_t;

extern sdfSndInitThreadParam_t sdfSndCmdThreadParam;
extern sndListenerState_t sdfSndListenerPrevState;
extern sifDmaTransfer_t sdfSndTransBuf;
extern sndFileRequestState_t sdfSndPcmRequest;

#endif
