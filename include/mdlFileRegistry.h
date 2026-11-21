#ifndef MDL_FILE_REGISTRY_H
#define MDL_FILE_REGISTRY_H

#include "mdlBuiltinFilePaths.h"
#include "mdlFileIndex.h"

typedef struct
{
    dds3ModelFileTable_t *entries;
    s32 count;
} mdlFileSlot_t;

typedef struct
{
    dds3ModelFileIndex_t *entries;
    s32 count;
} mdlFileKindSlot_t;

SIZE_CHECK(mdlFileSlotSize, mdlFileSlot_t, 8);
SIZE_CHECK(mdlFileKindSlotSize, mdlFileKindSlot_t, 8);

extern mdlFileSlot_t modelFileTable[];
extern mdlFileKindSlot_t modelFileKindTable[];

#endif
