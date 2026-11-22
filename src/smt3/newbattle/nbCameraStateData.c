#include "nbCameraState.h"

typedef char nbCameraStateSizeCheck[(sizeof(nbCamera_t) == 0x58) ? 1 : -1];

/* Both camera vectors are also read and written as VU quadwords. */
nbCamera_t pNbCamera __attribute__((aligned(16))) = { 0 };
