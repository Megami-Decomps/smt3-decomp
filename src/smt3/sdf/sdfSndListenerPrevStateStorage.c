#include "sdfSoundRuntimeStorage.h"

SIZE_CHECK(sdfSndListenerPrevStateSize, sndListenerState_t, 0x20);

/* Last listener packet sent to the sound driver. */
sndListenerState_t sdfSndListenerPrevState __attribute__((section(".bss")));
