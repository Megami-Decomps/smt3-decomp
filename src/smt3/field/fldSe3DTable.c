#include "fldSe3DTable.h"

/* Eight spatial sound emitters in the original field zero-fill region. */
fldSe3D_tag fldSe3DTable[8] __attribute__((section(".bss")));
