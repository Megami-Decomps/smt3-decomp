#include "common.h"

/* The additional request picker walks one 3-choice row of cumulative weights.
   The flat declaration matches the consumer's byte indexing. */
u8 datNegoTuikaReqtype[11 * 3] = {
    45, 35, 20,
    15, 15, 70,
    10, 80, 10,
    80, 10, 10,
    10, 10, 80,
    33, 33, 34,
    33, 33, 34,
    33, 33, 34,
    33, 33, 34,
    33, 33, 34,
    33, 33, 34,
};
