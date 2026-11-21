#include "itfMesDebugState.h"

SIZE_CHECK(itfMesDebugStateSize, struct debug_tag, 12);

struct debug_tag itfMesDebug __attribute__((section(".data"))) = { 0 };
