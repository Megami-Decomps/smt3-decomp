#include "common.h"

/* The initial request picker walks one 5-choice row of cumulative weights.
   The flat declaration matches the consumer's byte indexing. */
u8 datNegoReqTable[7 * 5] = {
    15, 70, 0, 15, 0,
    10, 40, 0, 50, 0,
    40, 20, 0, 40, 0,
    20, 20, 20, 20, 20,
    20, 20, 20, 20, 20,
    20, 20, 20, 20, 20,
    20, 20, 20, 20, 20,
};
