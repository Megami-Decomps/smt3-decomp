#include "sdfSoundMapStorage.h"

/* The EE and IOP share this native 0x5A0-byte sound state. */
sdfSoundMapStorage_t sdfSndMap __attribute__((section(".bss"), aligned(16)));
