#include "common.h"

SIZE_CHECK(mallocSbrkBasePointerSize, void *, 4);

/* The allocator replaces this sentinel on its first noncontiguous growth. */
void *__malloc_sbrk_base __attribute__((section(".data"))) = (void *)-1;
