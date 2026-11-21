#include "facility.h"

extern fclSpriteRes_t D_00330708[11];
extern fclSpriteEntry_t D_003307A8[11];

/* The resource and placement arrays for the single sprite page. */
fclSpriteTbl_t rcvSpriteTable[1] __attribute__((section(".data"))) = {
    { D_00330708, D_003307A8 },
};
