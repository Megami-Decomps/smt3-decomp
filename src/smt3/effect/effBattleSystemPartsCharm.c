#include "effBattleSystemPartData.h"

extern char D_005333D0[];
extern char D_005333B0[];

effBattleSystem_effBTLSystem_Part_tag effBTLSystem_Charm_PartList[2] __attribute__((section(".data"), aligned(4))) = {
    { 0, D_005333D0, NULL, 0, NULL, NULL, NULL },
    { 0, D_005333B0, NULL, 0, NULL, NULL, NULL },
};
