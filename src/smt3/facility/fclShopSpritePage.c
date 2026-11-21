#include "facility.h"

extern fclSpriteRes_t D_0032FA98[13];
extern fclSpriteEntry_t D_0032FB50[14];

/* The independent two-pointer descriptor for Shop page zero. */
fclSpriteTbl_t shopSpriteTable[1] __attribute__((section(".data"))) = {
    { D_0032FA98, D_0032FB50 },
};
