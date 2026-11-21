#include "common.h"

/* The six-byte hardware mode table is in writable .data, not small data. */
u8 sdfIpuRgbModeBits[3][2] __attribute__((section(".data"))) = {
    { 0, 0 },
    { 1, 0 },
    { 1, 1 },
};
