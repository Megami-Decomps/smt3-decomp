#include "kernel_draw.h"

SIZE_CHECK(dds3DrawBankSize, dds3DrawBank_t, 0x1BE0);
typedef char dds3FrameSourceOffset[(sizeof(void *) != 4 || (size_t)&((dds3DrawBank_t *)0)->stdEnv == 0x1300) ? 1 : -1];
typedef char dds3DrawViewportOffset[(sizeof(void *) != 4 || (size_t)&((dds3DrawBank_t *)0)->viewport == 0x1520) ? 1 : -1];
typedef char dds3RenderClearStartOffset[(sizeof(void *) != 4 || (size_t)&((dds3DrawBank_t *)0)->renderEnv.clear.xyzA.data == 0x1800) ? 1 : -1];
typedef char dds3RenderClearEndOffset[(sizeof(void *) != 4 || (size_t)&((dds3DrawBank_t *)0)->renderEnv.clear.xyzB.data == 0x1810) ? 1 : -1];

/* The two frame banks are initially zero in the native file, before the
 * renderer initializes their DMA tags and draw packets. */
dds3DrawBank_t dds3DrawEnv[2] __attribute__((section(".data"), aligned(16))) = { 0 };
