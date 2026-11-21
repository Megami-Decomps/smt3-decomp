#include "facility.h"

SIZE_CHECK(fclTerminalSpriteEntrySize, fclSpriteEntry_t, 8);

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
