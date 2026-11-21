#include "effBattleFieldFilter.h"

effBTLFieldFilter_t effBTLFieldColorFilter __attribute__((section(".data"))) = {
    .rect = { .blend = 0x48, .xy = {{0, 0}, {512, 448}} },
};
