#include "common.h"

extern const char D_0054BCA8[];
extern char D_0054BCB0[];

/* Initial filesystem binding state and the four-byte fallback version token. */
s32 fsInitialized __attribute__((section(".data"))) = 0;
s32 fsCommandSema __attribute__((section(".data"))) = -1;
s32 fsIobSema __attribute__((section(".data"))) = -1;
s32 fsIoctlSema __attribute__((section(".data"))) = -1;
const void *fsFallbackVersion __attribute__((section(".data"))) = D_0054BCA8;

/* Heap and loadfile start unbound. The loadfile token remains a separate object. */
s32 iopHeapStatus __attribute__((section(".data"))) = -1;
s32 lfBindStatus __attribute__((section(".data"))) = -1;
void *lfFallbackVersion __attribute__((section(".data"))) = D_0054BCB0;
