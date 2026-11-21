#include "sdfDrawQueue.h"

SIZE_CHECK(sdfDrawRequestRecordSize, sdfDrawReqRec_t, 16);

/* The producer advances the frame-request queue modulo three. */
sdfDrawReqRec_t sdfGrDrawState[3] __attribute__((section(".data"))) = { { 0 } };
