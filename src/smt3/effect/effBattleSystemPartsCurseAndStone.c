#include "effBattleSystemPartData.h"

extern char D_005333E8[];
extern char D_00533400[];
extern char D_00533450[];
extern char D_00533438[];
extern char D_00533418[];

effBattleSystem_effBTLSystem_Part_tag effBTLSystem_CurseCAMP_PartList[2] __attribute__((section(".data"), aligned(4))) = {
    { 0, D_005333E8, NULL, 0, NULL, NULL, NULL },
    { 0, D_00533400, NULL, 0, NULL, NULL, NULL },
};

effBattleSystem_effBTLSystem_Part_tag effBTLSystem_Stone_Dead_PartList[3] __attribute__((section(".data"), aligned(4))) = {
    { 3, D_00533450, NULL, 0, NULL, NULL, NULL },
    { 0, D_00533438, NULL, 0, NULL, NULL, NULL },
    { 0, D_00533418, NULL, 0, NULL, NULL, NULL },
};
