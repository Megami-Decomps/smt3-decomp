#include "sdk_malloc_info.h"

/* The allocator updates the two maxima through native doubleword stores. */
u64 __malloc_max_sbrked_mem __attribute__((section(".data"))) = 0;
u64 __malloc_max_total_mem __attribute__((section(".data"))) = 0;
mallocInfo_t __malloc_current_mallinfo = { 0 };
