#include "fldEveHitSourceTable.h"

/* AddEveHitItemBox and AddEveHitNpc populate this pool as hits are registered. */
fldEveHitSource_t fldEveHitSourceTable[64] __attribute__((section(".data"), aligned(16))) = { 0 };
