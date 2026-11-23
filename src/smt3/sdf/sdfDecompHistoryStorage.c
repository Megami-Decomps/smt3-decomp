#include "common.h"

SIZE_CHECK(sdfDecompHistoryPointerSize, const u8 *, 4);

const u8 *sdfDecompHistory[16] __attribute__((section(".bss"), aligned(16)));
