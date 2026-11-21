#include "dds3PadHoldCounters.h"

/* The native arrays start cleared in file-backed pad state. */
u32 dds3PadButtonHoldCounters[64] = { 0 };
u32 dds3PadStickHoldCounters[4] = { 0 };
