#include "facility.h"

extern fclSpriteRes_t D_005277B8[45];
extern fclSpriteEntry_t D_00527A30[74];
extern fclSpriteRes_t D_00527C80[17];
extern fclSpriteEntry_t D_00527D70[27];
extern fclSpriteRes_t D_00527E48[2];
extern fclSpriteEntry_t D_00527E68[2];
extern fclSpriteRes_t D_00527E78[22];
extern fclSpriteEntry_t D_00527FB0[30];

/* Four name-entry sprite pages, in the original resource/position order. */
fclSpriteTbl_t GBWK_2[4] = {
    { D_005277B8, D_00527A30 },
    { D_00527C80, D_00527D70 },
    { D_00527E48, D_00527E68 },
    { D_00527E78, D_00527FB0 },
};
