#include "effBattleLightStorage.h"

/* Runtime light records and their initial per-formation state. */
effBattleLight_effBTLPointLight_tag effBTLPointLight[32] __attribute__((section(".bss"), aligned(16)));
effBattleLight_effBTLPointLight_Base_tag effBTLPointLightBase[32] __attribute__((section(".bss"), aligned(16)));
