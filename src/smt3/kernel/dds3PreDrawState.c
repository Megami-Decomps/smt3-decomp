#include "common.h"
#include "sdfPreDrawChain.h"

SIZE_CHECK(dds3NativePreDrawChainSize, sdfPreDrawChain_t, 8);

/* The initial list endpoints are both null in writable retail data. */
sdfPreDrawChain_t dds3PreDrawChain __attribute__((section(".data"))) = { NULL, NULL };
