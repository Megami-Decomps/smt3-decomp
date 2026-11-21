#include "effBattleSystemPartData.h"

extern const char D_005336E8[32];

SIZE_CHECK(battleSystemPartSize, effBattleSystem_effBTLSystem_Part_tag, 0x1C);

effBattleSystem_effBTLSystem_Part_tag effBTLSystem_Nego_Stone_PartList[1] __attribute__((section(".data"), aligned(4))) = {
    { 0xFFFD, D_005336E8, NULL, 0, NULL, NULL, NULL },
};
