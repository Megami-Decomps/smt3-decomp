#include "mdlFileRegistry.h"

extern dds3ModelFileTable_t mdlBuiltinHumanPaths[2];
extern dds3ModelFileTable_t D_00327E38[368];
extern dds3ModelFileTable_t D_00329FF8[35];
extern dds3ModelFileTable_t D_0032A1A0[129];
extern dds3ModelFileTable_t D_0032A7B0[9];
extern dds3ModelFileTable_t D_00328F78[352];

/* Slot five is populated and cleared by the HDD model loader. */
mdlFileSlot_t modelFileTable[7] = {
    { mdlBuiltinHumanPaths, ARRAY_COUNT(mdlBuiltinHumanPaths) },
    { D_00327E38, ARRAY_COUNT(D_00327E38) },
    { D_00329FF8, ARRAY_COUNT(D_00329FF8) },
    { D_0032A1A0, ARRAY_COUNT(D_0032A1A0) },
    { D_0032A7B0, ARRAY_COUNT(D_0032A7B0) },
    { NULL, 0 },
    { D_00328F78, ARRAY_COUNT(D_00328F78) },
};
