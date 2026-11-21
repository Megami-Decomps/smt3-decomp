#include "facility.h"

extern fclSpriteRes_t D_00528B98[18];
extern fclSpriteEntry_t D_00528C98[25];

/* One page, with the native resource and placement arrays. */
fclSpriteTbl_t dds3ConfigLayoutTable[1] __attribute__((section(".data"))) = {
    { D_00528B98, D_00528C98 },
};
