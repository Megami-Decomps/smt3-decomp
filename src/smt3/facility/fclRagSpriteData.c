#include "common.h"
#include "facility.h"

/* One page, selected by the Rag drawing calls. The resource bank remains raw.
 * Each sprite entry selects the corresponding graphic in that nine-row bank. */
extern fclSpriteRes_t D_003305F0[9];

fclSpriteEntry_t D_00330670[9] = {
    { 0, 15, 0, 0 },
    { 0, 0, 0, 1 },
    { 0, 0, 0, 2 },
    { 119, 61, 0, 3 },
    { 336, 61, 0, 4 },
    { 268, 196, 0, 5 },
    { 407, 196, 0, 6 },
    { 99, 61, 0, 7 },
    { 190, 61, 0, 8 },
};

/* Keep the page descriptor beside the entries in the native .data bank. */
fclSpriteTbl_t ragSpriteTable[1] __attribute__((section(".data"))) = { { D_003305F0, D_00330670 } };
