#include "effBattleSystemPartData.h"

extern const char D_00533690[24];

SIZE_CHECK(battleSystemPartSize, effBattleSystem_effBTLSystem_Part_tag, 0x1C);

effBattleSystem_effBTLSystem_Part_tag effBTLSystem_Encount_PartList[1] __attribute__((section(".data"), aligned(4))) = {
    { 0xFFFD, D_00533690, NULL, 0, NULL, NULL, NULL },
};
