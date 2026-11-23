#include "frFontSystemStorage.h"

/* The text decoder reads the same address slots that frSetVariableRequest fills. */
u32 gVarStr[32] __attribute__((section(".bss"), aligned(16)));
