#include "facility.h"

SIZE_CHECK(fclRecoverSpriteEntrySize, fclSpriteEntry_t, 8);

/* The Recovery page has eleven resources and eleven placement entries. */
SIZE_CHECK(fclRecoverSpriteResSize, fclSpriteRes_t, 14);
fclSpriteRes_t D_00330708[11] = {
    { 0, { 0, 0, 2, 88, 185, 33 } },
    { 1, { 0, 0, 2, 47, 185, 30 } },
    { 0, { 0, 0, 1, 38, 88, 21 } },
    { 0, { 0, 0, 1, 60, 88, 21 } },
    { 0, { 0, 0, 124, 22, 88, 21 } },
    { 0, { 0, 0, 91, 44, 88, 21 } },
    { 0, { 0, 0, 1, 1, 55, 15 } },
    { 0, { 0, 0, 57, 1, 29, 15 } },
    { 0, { 0, 0, 87, 1, 31, 15 } },
    { 0, { 0, 0, 119, 1, 68, 15 } },
    { 1, { 84, 0, 186, 47, 1, 30 } },
};

fclSpriteEntry_t D_003307A8[11] = {
    { 0, 15, 0, 0 },
    { 0, 0, 0, 1 },
    { 0, 0, 0, 2 },
    { 0, 0, 0, 3 },
    { 0, 0, 0, 4 },
    { 0, 0, 0, 5 },
    { 94, 94, 0, 6 },
    { 213, 94, 0, 7 },
    { 306, 94, 0, 8 },
    { 388, 94, 0, 9 },
    { 184, 0, 0, 10 },
};
