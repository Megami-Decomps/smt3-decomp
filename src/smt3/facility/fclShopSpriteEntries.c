#include "facility.h"

SIZE_CHECK(fclShopSpriteEntrySize, fclSpriteEntry_t, 8);

/* The thirteen resource records for Shop page zero. */
SIZE_CHECK(fclShopSpriteResSize, fclSpriteRes_t, 14);
fclSpriteRes_t D_0032FA98[13] = {
    { 0, { 0, 0, 2, 1, 185, 33 } },
    { 0, { 0, 0, 2, 66, 185, 30 } },
    { 0, { 0, 0, 218, 68, 11, 15 } },
    { 0, { 0, 0, 239, 22, 11, 11 } },
    { 0, { 0, 0, 239, 34, 11, 11 } },
    { 0, { 0, 0, 1, 97, 49, 15 } },
    { 0, { 0, 0, 218, 50, 13, 17 } },
    { 0, { 0, 0, 51, 97, 37, 15 } },
    { 0, { 0, 0, 139, 97, 43, 15 } },
    { 0, { 0, 0, 89, 97, 49, 15 } },
    { 0, { 84, 0, 186, 66, 1, 30 } },
    { 0, { 0, 0, 184, 97, 13, 16 } },
    { 0, { 0, 0, 239, 61, 13, 2 } },
};

/* Page-zero shop sprite positions, flags and resource selectors. */
fclSpriteEntry_t D_0032FB50[14] = {
    { 0, 15, 0, 0 },
    { 0, 0, 0, 1 },
    { 0, 0, 0, 2 },
    { 0, 0, 0, 3 },
    { 0, 0, 0, 4 },
    { 79, 94, 0, 5 },
    { 200, 92, 0, 6 },
    { 263, 94, 0, 7 },
    { 260, 94, 0, 8 },
    { 383, 94, 0, 9 },
    { 184, 0, 0, 10 },
    { 0, 0, 0, 11 },
    { 0, 0, 2, 11 },
    { 0, 0, 0, 12 },
};
