#include "common.h"
#include "facility.h"

extern fclSpriteRes_t D_003305F0[9];
extern fclSpriteEntry_t D_00330670[9];

fclSpriteTbl_t ragSpriteTable[1] __attribute__((section(".data"))) = { { D_003305F0, D_00330670 } };
