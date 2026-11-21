#ifndef SDK_CDVD_RPC_CLIENT_H
#define SDK_CDVD_RPC_CLIENT_H

#include "common.h"

typedef struct cdvdRpcClient_t {
    u8 transactionAndCallbacks[0x24];
    void *server;
} cdvdRpcClient_t;

#endif
