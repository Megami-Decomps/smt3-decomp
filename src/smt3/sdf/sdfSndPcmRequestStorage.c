#include "sdfSoundRuntimeStorage.h"

SIZE_CHECK(sdfSndPcmRequestSize, sndFileRequestState_t, 0x10C);

/* Shared PCM request state. */
sndFileRequestState_t sdfSndPcmRequest __attribute__((section(".bss")));
