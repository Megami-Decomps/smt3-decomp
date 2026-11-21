#include "common.h"

/* Shared by the PCM load command and the subsequent file RPC. */
char sdfSndPcmFileName[32] __attribute__((section(".data"))) = { 0 };
