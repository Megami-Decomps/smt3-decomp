#include "kernel_draw.h"

SIZE_CHECK(dds3RenderOrderingEntrySize, sdfGrOt_t, 0x20);

/* The native renderer initializes all 85 rows at startup. This zero-filled
 * array belongs to the file-backed data region, not .bss. */
sdfGrOt_t sdfGrOtTable[85] __attribute__((section(".data"), aligned(8))) = { { 0 } };
