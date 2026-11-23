#include "sdf_types.h"

/* VU0 loads and stores one quadword per formation slot. */
sdfVector4_t effBTLPartCenterOffset[32] __attribute__((section(".bss"), aligned(16)));
