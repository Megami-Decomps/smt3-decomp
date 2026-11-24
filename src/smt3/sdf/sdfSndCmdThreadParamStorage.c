#include "sdfSoundRuntimeStorage.h"

SIZE_CHECK(sdfSndCmdThreadParamSize, sdfSndInitThreadParam_t, 0x30);

/* Command worker descriptor passed to CreateThread. */
sdfSndInitThreadParam_t sdfSndCmdThreadParam __attribute__((section(".bss")));
