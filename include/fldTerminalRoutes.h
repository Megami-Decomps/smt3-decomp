#ifndef FLD_TERMINAL_ROUTES_H
#define FLD_TERMINAL_ROUTES_H

#include "common.h"

typedef struct {
    s16 field;
    s16 eventBit;
} fldTerminalRoute_t;

typedef struct {
    s16 field;
    s16 jumpTerminalNo;
} trmFieldEntry_t;

extern fldTerminalRoute_t fldTerminalTable[32];
extern trmFieldEntry_t fldAllCnt[64];

/* Non-owning view of eventBit from row zero, at the native stride. */
extern s16 fldTerminalEventBits[][2];

#endif
