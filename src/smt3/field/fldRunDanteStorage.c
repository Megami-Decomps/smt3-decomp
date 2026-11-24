#include "fldRunDanteState.h"

SIZE_CHECK(FldRunDanteStateSize, fldTest_FLDRUN_DANTE_t, 0x74);

fldTest_FLDRUN_DANTE_t fldRun_Dante __attribute__((section(".bss")));
