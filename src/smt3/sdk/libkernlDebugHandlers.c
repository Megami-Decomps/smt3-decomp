#include "common.h"

/* The kernel registration routine exchanges opaque handler addresses. */
void *debugHandlers[16] = { NULL };
