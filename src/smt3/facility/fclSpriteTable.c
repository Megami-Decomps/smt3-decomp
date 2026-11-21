#include "facility.h"

extern fclSpriteRes_t D_00343F50[31];
extern fclSpriteEntry_t D_00344108[36];
extern fclSpriteRes_t D_00344228[14];
extern fclSpriteEntry_t D_003442F0[16];
extern fclSpriteRes_t D_00344370[21];
extern fclSpriteEntry_t D_00344498[24];
extern fclSpriteRes_t D_00344558[5];
extern fclSpriteEntry_t D_003445A0[5];
extern fclSpriteRes_t D_003445C8[7];
extern fclSpriteEntry_t D_00344630[7];
extern fclSpriteRes_t D_00344668[4];
extern fclSpriteEntry_t D_003446A0[4];
extern fclSpriteRes_t D_003446C0[12];
extern fclSpriteEntry_t D_00344768[12];
extern fclSpriteRes_t D_003447C8[18];
extern fclSpriteEntry_t D_003448C8[22];
extern fclSpriteRes_t D_00344978[41];
extern fclSpriteEntry_t D_00344BB8[68];
extern fclSpriteRes_t D_00344DD8[9];
extern fclSpriteEntry_t D_00344E58[9];
extern fclSpriteRes_t D_00344EA0[12];
extern fclSpriteEntry_t D_00344F48[16];
extern fclSpriteRes_t D_00344FC8[14];
extern fclSpriteEntry_t D_00345090[14];

/* Native page descriptors retain the independent resource and entry pointers. */
fclSpriteTbl_t fclSpriteTable[12] = {
    { D_00343F50, D_00344108 },
    { D_00344228, D_003442F0 },
    { D_00344370, D_00344498 },
    { D_00344558, D_003445A0 },
    { D_003445C8, D_00344630 },
    { D_00344668, D_003446A0 },
    { D_003446C0, D_00344768 },
    { D_003447C8, D_003448C8 },
    { D_00344978, D_00344BB8 },
    { D_00344DD8, D_00344E58 },
    { D_00344EA0, D_00344F48 },
    { D_00344FC8, D_00345090 },
};
