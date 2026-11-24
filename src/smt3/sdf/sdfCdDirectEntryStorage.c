#include "sdfCdDirectEntryStorage.h"

/* The direct file backend returns this single shared directory entry. */
sdfCdImageDirEntry_t sdfCdImageSearchFileDirect_ent __attribute__((section(".bss"), aligned(16)));
