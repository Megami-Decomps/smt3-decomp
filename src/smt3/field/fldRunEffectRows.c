#include "fldRunNativeRows.h"

extern fldTest_FLDRUN_EFF_t fldDspark01File;
extern fldTest_FLDRUN_EFF_t fldDsmoke2File;
extern fldTest_FLDRUN_EFF_t fldDspark02File;
extern fldTest_FLDRUN_EFF_t fldGun02aFile;
extern fldTest_FLDRUN_EFF_t fldGun01a0File;
extern fldTest_FLDRUN_EFF_t fldMuzzleB010File;
extern fldTest_FLDRUN_EFF_t fldDsmoke1File;
extern fldTest_FLDRUN_EFF_t fldDflash1File;

/* The effect traversals stop at the null resource row. */
fldTest_FLDRUN_EFFTBL_t D_002FBE00[5] = {
    { &fldDspark01File, 30, 1, 0, -1.0f, 360.0f },
    { &fldDsmoke2File, 30, 0, 0, -1.0f, 360.0f },
    { &fldDspark02File, 30, 0, 0, -1.0f, 360.0f },
    { &fldGun02aFile, 30, 0, 0, -1.0f, 360.0f },
    { NULL },
};

fldTest_FLDRUN_EFFTBL_t D_002FBE78[5] = {
    { &fldGun01a0File, 30, 0, 0, -1.0f, 360.0f },
    { &fldMuzzleB010File, 30, 1, 1, 30.0f, 150.0f },
    { &fldDsmoke1File, 30, 0, 0, -1.0f, 360.0f },
    { &fldDflash1File, 30, 1, 0, -1.0f, 360.0f },
    { NULL },
};
