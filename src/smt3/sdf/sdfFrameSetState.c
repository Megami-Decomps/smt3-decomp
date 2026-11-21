#include "draw.h"

SIZE_CHECK(sdfFrameSetNativeSize, sdfGrFrameSet_t, 0x14);

/* Screen dimensions and two frame surfaces plus the shared depth surface,
 * initialized at runtime by sdfGrSetFrameSetMode and sdfGrMakeFrameSet. */
sdfGrFrameSet_t sdfGrFrameSet __attribute__((section(".data"))) = { 0 };
