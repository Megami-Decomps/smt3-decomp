#include "effBattleSystemPartData.h"

extern const char D_005333E8[24];

SIZE_CHECK(battleSystemPartSize, effBattleSystem_effBTLSystem_Part_tag, 0x1C);

effBattleSystem_effBTLSystem_Part_tag effBTLSystem_Curse_PartList[1] __attribute__((section(".data"), aligned(4))) = {
    { 0, D_005333E8, NULL, 0, NULL, NULL, NULL },
};
