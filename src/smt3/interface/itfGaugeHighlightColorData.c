#include "common.h"

/* Each three-point highlight strip uses one color. */
u8 itfGaugeHighlightColorIndex[6] __attribute__((section(".data"))) = { 0, 0, 0, 1, 1, 1 };
