# Retail quirks

Behaviour of the retail code that looks like a bug and is not worth a fix. Each entry says why retail does not trip over it.

## sdfChipAllocNewPage keeps the old freeLink

`sdfChipAllocNewPage` (`src/smt3/sdf/sdfChip.c`) resets `allocCount`, `growLeft`, `owner` and `alloc` but not `page->freeLink`. A page returned to the page pool when its last chip is freed keeps the head of its old free list, and the next owner of the page (possibly another size class) starts with that stale head.

Retail does not follow it: frees link in front of the stale head, and `allocCount == chipsPerPage` ends the page before the stale tail is popped. Anything that checks the free list against the count (the port's `SDFX_DEV_CHIP_CHECK`) must not treat a non-NULL link on a full page as corruption.
