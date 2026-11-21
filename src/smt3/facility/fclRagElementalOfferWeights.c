#include "common.h"

/* Each row contains seven cumulative pick thresholds followed by
 * four seven-item lists indexed by the shuffled choice category. */
u8 ragListItemTable2[2 * 35] = {
    64, 128, 192, 255, 255, 255, 255,
    2, 3, 4, 5, 0, 0, 0,
    7, 8, 9, 10, 0, 0, 0,
    12, 13, 14, 15, 0, 0, 0,
    17, 18, 19, 20, 0, 0, 0,

    64, 128, 192, 255, 255, 255, 255,
    2, 3, 4, 5, 0, 0, 0,
    7, 8, 9, 10, 0, 0, 0,
    12, 13, 14, 15, 0, 0, 0,
    17, 18, 19, 20, 0, 0, 0,
};
