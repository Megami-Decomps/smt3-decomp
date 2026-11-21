#include "common.h"

/* The selectable Soften, Bias and enable-toggle rows. */
u16 softenBiasCursorY[3] __attribute__((section(".data"))) = {
    0x7A80, 0x7AE0, 0x7BA0,
};
