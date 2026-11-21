#ifndef NB_NEGO_MESSAGE_DATA_H
#define NB_NEGO_MESSAGE_DATA_H

#include "common.h"

struct itfMesResource_t;

typedef struct {
    struct itfMesResource_t *flow;
    struct itfMesResource_t *aisyo;
    struct itfMesResource_t *question;
    struct itfMesResource_t *info;
} nbNegoMsgEntry_t;

SIZE_CHECK(nbNegoMsgEntrySize, nbNegoMsgEntry_t, 0x10);

extern nbNegoMsgEntry_t negoMsgTable[];

#endif
