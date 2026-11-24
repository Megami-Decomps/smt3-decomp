#include "fldRunNativeRows.h"

SIZE_CHECK(FldRunEffectResourceSize, fldTest_FLDRUN_EFF_t, 0x28);

/* Field effect file, live part and placement state used by both effect lists. */
fldTest_FLDRUN_EFF_t fldDspark01File __attribute__((section(".bss")));
fldTest_FLDRUN_EFF_t fldDsmoke2File __attribute__((section(".bss")));
fldTest_FLDRUN_EFF_t fldDspark02File __attribute__((section(".bss")));
fldTest_FLDRUN_EFF_t fldGun02aFile __attribute__((section(".bss")));
fldTest_FLDRUN_EFF_t fldGun01a0File __attribute__((section(".bss")));
fldTest_FLDRUN_EFF_t fldMuzzleB010File __attribute__((section(".bss")));
fldTest_FLDRUN_EFF_t fldDsmoke1File __attribute__((section(".bss")));
fldTest_FLDRUN_EFF_t fldDflash1File __attribute__((section(".bss")));
