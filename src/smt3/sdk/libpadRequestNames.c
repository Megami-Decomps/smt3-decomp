#include "common.h"

extern const char padRequestNameComplete[];
extern const char padRequestNameFailed[];
extern const char padRequestNameBusy[];

/* Request states used by scePadReqIntToStr; the last native slot is null. */
const char *padRequestNames[4] __attribute__((section(".data"))) = {
    padRequestNameComplete,
    padRequestNameFailed,
    padRequestNameBusy,
    NULL,
};
