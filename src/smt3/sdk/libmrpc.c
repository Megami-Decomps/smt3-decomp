#include "common.h"

/* One 0x40-byte request packet of the multi-threaded RPC queue. */
typedef struct rpcPacket {
    u32 w[16];
} rpcPacket;

/* The queue header: count, base, capacity. */
typedef struct rpcQueue {
    s32 seq;
    rpcPacket *base;
    s32 count;
} rpcQueue;

extern s32 D_0052BA88 __attribute__((section(".data")));
extern rpcQueue D_00599700 __attribute__((section(".data")));

extern s32 DIntr(void);
extern s32 EIntr(void);

INCLUDE_ASM(const s32, "sdk/libmrpc", sceSifMInitRpc);

void sceSifMExitRpc(void)
{
    D_0052BA88 = 0;
}

INCLUDE_ASM(const s32, "sdk/libmrpc", _sceRpcGetPacket);

void _sceRpcFreePacket(rpcPacket *p)
{
    u32 flags = p->w[4];

    p->w[6] = 0;
    p->w[4] = flags & ~1;
}

INCLUDE_ASM(const s32, "sdk/libmrpc", _request_end);

INCLUDE_ASM(const s32, "sdk/libmrpc", sceSifMBindRpc);

INCLUDE_ASM(const s32, "sdk/libmrpc", sceSifMBindRpcParam);

INCLUDE_ASM(const s32, "sdk/libmrpc", sceSifMUnBindRpc);

INCLUDE_RODATA(const s32, "sdk/libmrpc", D_0054ADD8);

INCLUDE_ASM(const s32, "sdk/libmrpc", sceSifMCallRpc);
