#include "fldDoorEventTable.h"

/* Runtime state initialized by fldClearDoorEvents and fldSetupDoorScenes. */
doormovework_s fldDoorEventTable[256] __attribute__((section(".bss")));
