#include "common.h"

/* Only cut kinds 0 through 2 set a camera state. */
u16 effBTLCutCameraState[3] __attribute__((section(".data"))) = { 7, 8, 9 };
