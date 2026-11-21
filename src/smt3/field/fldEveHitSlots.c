#include "field.h"

/* One source pointer and running script ID per event-hit slot. */
fldResHead_t *gEvnRng[64] __attribute__((section(".data"), aligned(16))) = { NULL };
u32 gEvnRngPid[64] __attribute__((section(".data"), aligned(16))) = { 0 };
