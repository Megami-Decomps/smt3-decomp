#include "common.h"

struct nbKoukaPacket_t;

/* The event fills these two slots at different times and later reads both. */
struct nbKoukaPacket_t *nbE912CutPacket[2] __attribute__((section(".data"))) = {
    NULL, NULL,
};
