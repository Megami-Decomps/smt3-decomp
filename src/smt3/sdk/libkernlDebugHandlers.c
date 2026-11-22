#include "common.h"

/* Opaque handler address initially unset until SetTLBHandler installs it. */
void *tlbHandler __attribute__((section(".data"))) = NULL;

/* The kernel registration routine exchanges opaque handler addresses. */
void *debugHandlers[16] = { NULL };
