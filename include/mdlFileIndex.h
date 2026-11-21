#ifndef MDL_FILE_INDEX_H
#define MDL_FILE_INDEX_H

#include "common.h"

typedef struct
{
    u16 major;
    u16 minor;
    u16 scale;
    u16 radius;
} dds3ModelFileIndex_t;

SIZE_CHECK(dds3ModelFileIndexSize, dds3ModelFileIndex_t, 8);

#endif
