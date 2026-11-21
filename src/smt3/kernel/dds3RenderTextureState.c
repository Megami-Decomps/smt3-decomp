#include "draw.h"

SIZE_CHECK(dds3RenderFrameSetSize, sdfGrFrameSet_t, 0x14);

/* Framebuffer dimensions, formats and surfaces initialized by the renderer. */
sdfGrFrameSet_t dds3RenderTexId __attribute__((section(".data"))) = { 0 };
