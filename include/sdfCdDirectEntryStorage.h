#ifndef SMT3_SDF_CD_DIRECT_ENTRY_STORAGE_H
#define SMT3_SDF_CD_DIRECT_ENTRY_STORAGE_H

#include "common.h"

typedef struct
{
    u32 nameOffset_;
    u32 offset_;
    s32 size_;
} sdfCdImageDirEntry_t;

SIZE_CHECK(sdfCdImageDirEntryNativeSize, sdfCdImageDirEntry_t, 0xC);
extern sdfCdImageDirEntry_t sdfCdImageSearchFileDirect_ent;

#endif
