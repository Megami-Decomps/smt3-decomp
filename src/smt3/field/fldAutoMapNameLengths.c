#include "common.h"

SIZE_CHECK(fldAutoMapNameLengthHalfword, u16, 2);

/* The font initializer fills 41 area, 41 floor, and 24 place lengths.
 * Keep the native alignment between the three separately indexed tables. */
u16 fldAutoMapAreaNameLen[41] __attribute__((section(".bss"), aligned(16)));
s16 fldAutoMapFloorNameLen[41] __attribute__((section(".bss"), aligned(8)));
u16 fldAutoMapPlaceNameLen[24] __attribute__((section(".bss"), aligned(8)));
