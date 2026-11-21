#include "fldRunNativeRows.h"

extern fldTest_FLDRUN_EFFTBL_t D_002FBE00[5];
extern fldTest_FLDRUN_EFFTBL_t D_002FBE78[5];

fldTest_FLDRUN_EFFTBL_t *fldRunEffTables[2] __attribute__((section(".data"))) = { D_002FBE00, D_002FBE78 };
