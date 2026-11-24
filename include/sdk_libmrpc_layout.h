#ifndef SMT3_SDK_LIBMRPC_LAYOUT_H
#define SMT3_SDK_LIBMRPC_LAYOUT_H

#include "common.h"

#include "sdk_libmrpc.h"

/* The M-RPC client extends the shared client prefix. Packet addresses are
 * opaque, as in SifRpcClientData_t in include/irx/iop.h. */
struct sceSifMClientData {
    void *packet;
    u32 sequence;
    s32 sema;
    u32 unk_C;
    u32 serverId;
    void *buffer;
    void *cbuffer;
    void (*endFunction)(void *);
    void *endParam;
    void *server;
    s32 completionSema;
    s32 result;
    u32 parameters[3];
};

SIZE_CHECK(sceSifMClientDataNativeSize, sceSifMClientData, 0x3C);
s32 sceSifMBindRpc(sceSifMClientData *client, u32 serverId, s32 mode);

#endif
