#include "common.h"

/* Map the five visible parameter rows to the underlying parameter slots.
 * The UI skips slot 1. */
s8 cmpParamGaugeColors[5] __attribute__((section(".data"))) = { 0, 2, 3, 4, 5 };
