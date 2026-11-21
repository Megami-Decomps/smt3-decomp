#include "mdlDebugData.h"

SIZE_CHECK(mdlDebugLightWorkSize, mdlDebugLight_t, 20);

/* The light editor initializes its file-backed work record on first use. */
mdlDebugLight_t mdlDebugLightWork __attribute__((section(".data"))) = { 0 };
