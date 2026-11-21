#include "effBattleSystemPartData.h"

extern char D_005335C0[];
extern char D_005335A8[];
extern char D_00533590[];
extern char D_00533578[];
extern char D_00533558[];
extern char D_00533538[];
extern char D_00533668[];
extern char D_00533650[];
extern char D_00533638[];
extern char D_00533620[];
extern char D_00533608[];
extern char D_005335F0[];

effBattleSystem_effBTLSystem_Part_tag effBTLSystem_Attack_PartList[6] __attribute__((section(".data"), aligned(4))) = {
    { 0, D_005335C0, NULL, 0, NULL, NULL, NULL },
    { 6, D_005335A8, NULL, 0, NULL, NULL, NULL },
    { 6, D_00533590, NULL, 0, NULL, NULL, NULL },
    { 6, D_00533578, NULL, 0, NULL, NULL, NULL },
    { 0, D_00533558, NULL, 0, NULL, NULL, NULL },
    { 0, D_00533538, NULL, 0, NULL, NULL, NULL },
};

effBattleSystem_effBTLSystem_Part_tag effBTLSystem_AttackC_PartList[6] __attribute__((section(".data"), aligned(4))) = {
    { 6, D_00533668, NULL, 0, NULL, NULL, NULL },
    { 6, D_00533650, NULL, 0, NULL, NULL, NULL },
    { 6, D_00533638, NULL, 0, NULL, NULL, NULL },
    { 6, D_00533620, NULL, 0, NULL, NULL, NULL },
    { 6, D_00533608, NULL, 0, NULL, NULL, NULL },
    { 6, D_005335F0, NULL, 0, NULL, NULL, NULL },
};
