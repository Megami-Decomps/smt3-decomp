#include "common.h"

/* The camera shake offset is initialized before its first frame update. */
f32 dds3QuakeOffset[4] __attribute__((section(".data"), aligned(16))) = { 0 };
