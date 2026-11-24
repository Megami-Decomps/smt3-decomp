#include "sdk_gs_store_image.h"

/* The display capture paths share this native seven-quadword packet. */
sceGsStoreImage sdfGrStoreImage __attribute__((section(".bss"), aligned(16)));
