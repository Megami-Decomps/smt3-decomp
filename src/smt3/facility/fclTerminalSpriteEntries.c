#include "facility.h"

SIZE_CHECK(fclTerminalSpriteEntrySize, fclSpriteEntry_t, 8);

/* Resources for Terminal sprite page zero; every row uses texture zero. */
SIZE_CHECK(fclTerminalSpriteResSize, fclSpriteRes_t, 14);
fclSpriteRes_t D_00330848[7] = {
    { 0, { 0, 0, 60, 32, 185, 33 } },
    { 0, { 0, 0, 3, 67, 138, 33 } },
    { 0, { 0, 0, 140, 67, 1, 33 } },
    { 0, { 0, 0, 148, 68, 73, 3 } },
    { 0, { 189, 0, 140, 67, 1, 3 } },
    { 0, { 0, 0, 3, 1, 244, 29 } },
    { 0, { 0, 0, 3, 32, 53, 29 } },
};

/* Entries of terminal sprite page zero, selected through trmSpriteTable. */
fclSpriteEntry_t D_003308B0[9] = {
    { 0, 25, 0, 0 },
    { 0, 0, 0, 1 },
    { 0, 0, 0, 2 },
    { 190, 0, 0, 3 },
    { 0, 0, 0, 4 },
    { 190, 0, 2, 3 },
    { 0, 0, 2, 4 },
    { 53, 0, 0, 5 },
    { 0, 0, 0, 6 },
};
