#include "libsdf.h"

SIZE_CHECK(sdfMemControlNativeSize, sdfMemHandleCB_tag, 0x28);

/* The SDF heap control owns both embedded sentinel handles. */
sdfMemHandleCB_tag sdfMemCB __attribute__((section(".bss"), aligned(8)));
