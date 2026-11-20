#include "common.h"

typedef struct {
    s16 jouken[3];
    u8 pad[0xC6];
} nbInfoSelectEntry_t;

/* Two banks of 30 message-selection records. The USA initial conditions
 * are all zero; keep the native stride and writable initialized storage. */
nbInfoSelectEntry_t InfoSelectTable[60] = {{0}};
