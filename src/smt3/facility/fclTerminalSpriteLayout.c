#include "facility.h"

extern fclSpriteRes_t D_00330848[7];
extern fclSpriteEntry_t D_003308B0[9];

/* The resource and placement arrays for the single sprite page. */
fclSpriteTbl_t trmSpriteTable[1] __attribute__((section(".data"))) = {
    { D_00330848, D_003308B0 },
};
