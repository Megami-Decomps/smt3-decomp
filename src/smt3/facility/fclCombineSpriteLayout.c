#include "facility.h"

extern fclSpriteRes_t D_0032CBA0[];
extern fclSpriteRes_t D_0032CEC8[];
extern fclSpriteRes_t D_0032CF68[];
extern fclSpriteEntry_t D_0032CD90[39];
extern fclSpriteEntry_t D_0032CF30[7];
extern fclSpriteEntry_t D_0032D0C8[25];

/* The resource and placement arrays for the three Fusion sprite pages. */
fclSpriteTbl_t cmbSpriteTable[3] = {
    { D_0032CBA0, D_0032CD90 },
    { D_0032CEC8, D_0032CF30 },
    { D_0032CF68, D_0032D0C8 },
};
