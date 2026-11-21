#include "common.h"
#include "facility.h"

extern fclSpriteRes_t D_0032CBA0[];
extern fclSpriteRes_t D_0032CEC8[];
extern fclSpriteRes_t D_0032CF68[];
extern fclSpriteEntry_t D_0032CD90[39];
extern fclSpriteEntry_t D_0032CF30[7];

/* Stock-header sprite placement entries. */
fclSpriteEntry_t D_0032D0C8[25] = {
    { 0, 0, 0, 0 },
    { 0, 0, 0, 1 },
    { 0, 0, 0, 2 },
    { 0, 0, 0, 3 },
    { 0, 0, 0, 4 },
    { 0, 0, 0, 5 },
    { 0, 0, 0, 6 },
    { 0, 0, 0, 7 },
    { 0, 0, 0, 8 },
    { 0, 0, 0, 9 },
    { 0, 0, 0, 10 },
    { 0, 0, 0, 11 },
    { 0, 0, 0, 12 },
    { 0, 0, 0, 13 },
    { 0, 0, 0, 14 },
    { 0, 0, 0, 15 },
    { 0, 0, 0, 16 },
    { 0, 0, 0, 17 },
    { 0, 0, 0, 18 },
    { 0, 0, 0, 19 },
    { 0, 0, 0, 20 },
    { 0, 0, 0, 21 },
    { 0, 0, 0, 22 },
    { 0, 0, 0, 23 },
    { 16, 127, 0, 24 },
};

/* Resource banks are intentionally still backed by the original raw data. */
fclSpriteTbl_t cmbSpriteTable[3] = {
    { D_0032CBA0, D_0032CD90 },
    { D_0032CEC8, D_0032CF30 },
    { D_0032CF68, D_0032D0C8 },
};
