#include "common.h"

/* Parameter slot 1 is not part of the record comparison. */
s8 fclEncycCompareIndex[5] __attribute__((section(".data"))) = {
    0, 2, 3, 4, 5,
};
