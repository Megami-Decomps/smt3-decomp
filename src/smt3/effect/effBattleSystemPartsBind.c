#include "effBattleSystemPartData.h"

extern char D_00533380[];
extern char D_00533368[];

effBattleSystem_effBTLSystem_Part_tag effBTLSystem_Bind_PartList[2] __attribute__((section(".data"), aligned(4))) = {
    { 0, D_00533380, NULL, 0, NULL, NULL, NULL },
    { 0, D_00533368, NULL, 0, NULL, NULL, NULL },
};
