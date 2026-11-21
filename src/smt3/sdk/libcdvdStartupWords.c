#include "common.h"

/* Command/status words and invalid semaphore IDs before RPC startup. */
s32 cdvdDebugLevel __attribute__((section(".data"))) = 0;
s32 cdvdCallbackThreadId __attribute__((section(".data"))) = 0;
s32 cdvdScmdLastCommand __attribute__((section(".data"))) = 0;
s32 cdvdNcmdLastCommand __attribute__((section(".data"))) = 0;
s32 cdvdScmdSema __attribute__((section(".data"))) = -1;
s32 cdvdPowerOffActive __attribute__((section(".data"))) = 0;
s32 cdvdNcmdSema __attribute__((section(".data"))) = -1;
s32 cdvdScmdLockSema __attribute__((section(".data"))) = -1;
