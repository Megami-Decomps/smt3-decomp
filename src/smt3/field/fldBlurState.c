#include "common.h"

/* Automap and NPC effects share the one native handle slot. */
void *fldBlurEffectId[1] __attribute__((section(".data"))) = { NULL };
