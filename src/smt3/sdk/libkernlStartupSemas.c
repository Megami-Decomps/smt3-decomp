#include "common.h"

/* supplement_crt0 creates these IDs before the heap and frame registry use them.
 * The retail linker places the two zero initializers in writable .data. */
s32 mallocSema __attribute__((section(".data"))) = 0;
s32 frameObjectSema __attribute__((section(".data"))) = 0;
