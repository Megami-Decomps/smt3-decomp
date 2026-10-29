#ifndef NEWBATTLE_H
#define NEWBATTLE_H

#include "common.h"

/* The native 0x80-byte battle party record. */
typedef struct nbParty_t {
    u16 flag;
    s16 formindex;
    s16 statindex;
    u16 partyindex;
    s16 count[20];
    u32 aisyo[16];
    u16 hojoskill[8];
} nbParty_t;

#endif /* NEWBATTLE_H */
