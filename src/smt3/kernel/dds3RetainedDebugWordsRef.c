#include "common.h"

typedef struct dds3DebugWordsRef_t {
    u32 *words;
    u32 trailingWord;
} dds3DebugWordsRef_t;

SIZE_CHECK(dds3DebugWordsRefSize, dds3DebugWordsRef_t, 8);

extern u32 D_0054E920[2];

/* Retained reference to the debug-menu words; no reader is known. */
dds3DebugWordsRef_t D_002F5940 __attribute__((section(".data"))) = { D_0054E920, 0 };
