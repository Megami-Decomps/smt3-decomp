#include "sdk_libkernl_fs_iob.h"

/* The native descriptor table has 32 slots. The semaphore and RPC client
 * are independent providers; only the table lives in this span. */
fsRequestEntry_t fsIobTable[32] __attribute__((section(".bss")));
