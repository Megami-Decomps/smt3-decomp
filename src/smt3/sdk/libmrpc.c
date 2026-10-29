#include "common.h"

/* One 0x40-byte request packet of the multi-threaded RPC queue. */
typedef struct rpcPacket {
    u32 w[16];
} rpcPacket;

/* Queue prefix: sequence number, packet base, signed capacity. */
typedef struct rpcQueue {
    u32 seq;
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

rpcPacket *_sceRpcGetPacket(rpcQueue *queue)
{
    rpcPacket *packet;
    s32 i;
    s32 count;

    DIntr();
    count = queue->count;
    packet = queue->base;
    for (i = 0; i < count; i++, packet++) {
        if ((packet->w[4] & 1) == 0) {
            u32 seq;

            packet->w[4] = (i << 16) | 5;
            queue->seq++;
            /* Capture this packet's sequence before advancing the queue again. */
            if (queue->seq == 1) {
                seq = queue->seq++;
            } else {
                seq = queue->seq;
            }
            packet->w[5] = (u32)packet;
            packet->w[6] = seq;
            EIntr();
            return packet;
        }
    }
    EIntr();
    return NULL;
}


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
