#include "sdk_gs_display_environment.h"

/* Persistent display registers between the draw stack and ImageSet state. */
sceGsDispEnv sdfGrDispEnv __attribute__((section(".bss")));
