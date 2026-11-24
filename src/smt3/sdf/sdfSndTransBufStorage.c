#include "sdfSoundRuntimeStorage.h"

SIZE_CHECK(sdfSndTransBufSize, sifDmaTransfer_t, 0x10);

/* One transfer descriptor reused by the SIF DMA submission loop. */
sifDmaTransfer_t sdfSndTransBuf __attribute__((section(".bss")));
