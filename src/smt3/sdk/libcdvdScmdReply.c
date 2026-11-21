#include "common.h"

/* The S-command RPC fills at most four words of reply. */
s32 cdvdScmdReply[4] __attribute__((section(".data"))) = { 0 };
