#include "nbResultWork.h"

/* These two arrays occupy consecutive storage in the native result BSS. */
nbResultScore_t nbResultScoreTable[4] __attribute__((section(".bss")));
nbResultCaption_t nbResultCaptions[64] __attribute__((section(".bss")));
