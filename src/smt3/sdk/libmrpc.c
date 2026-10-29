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

/* The M-RPC client extends the shared client prefix. Packet addresses are
 * opaque, as in SifRpcClientData_t in include/irx/iop.h. */
typedef struct rpcClient {
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
} rpcClient;

typedef struct sdfSemaParam_t {
    s32 count;
    s32 maxCount;
    s32 initCount;
    s32 waitThreads;
    u32 attr;
    u32 option;
} sdfSemaParam_t;

extern s32 CreateSema(sdfSemaParam_t *param);
extern s32 WaitSema(s32 sema);
extern s32 DeleteSema(s32 sema);
extern s32 scePrintf(const char *format, ...);
extern s32 sceSifSendCmd(s32 command, void *packet, s32 size, void *source, void *destination, s32 length);
extern char D_0054ADD8[];

s32 sceSifMBindRpcParam(rpcClient *client, u32 serverId, s32 mode, u32 arg0, u32 arg1, u32 arg2);

s32 sceSifMBindRpc(rpcClient *client, u32 serverId, s32 mode)
{
    return sceSifMBindRpcParam(client, serverId, mode, 0x800, 0x2000, 0x20);
}

s32 sceSifMBindRpcParam(rpcClient *client, u32 serverId, s32 mode, u32 arg0, u32 arg1, u32 arg2)
{
    sdfSemaParam_t param;
    rpcPacket *packet;
    s32 sema;

    if (mode != 0) {
        scePrintf(D_0054ADD8);
        return -1;
    }
    param.option = 0;
    param.initCount = 1;
    param.maxCount = 1;
    client->completionSema = CreateSema(&param);
    if (client->completionSema < 0) {
        return -3;
    }
    WaitSema(client->completionSema);
    client->serverId = serverId;
    client->server = NULL;
    client->parameters[0] = arg0;
    client->parameters[1] = arg1;
    client->parameters[2] = arg2;
    packet = _sceRpcGetPacket(&D_00599700);
    if (packet == NULL) {
        DeleteSema(client->completionSema);
        return -1;
    }
    client->packet = packet;
    client->sequence = packet->w[6];
    packet->w[8] = serverId;
    packet->w[9] = arg0;
    packet->w[10] = arg1;
    packet->w[11] = arg2;
    packet->w[5] = (u32)packet;
    packet->w[7] = (u32)client;
    param.maxCount = 1;
    param.initCount = 0;
    param.option = 0;
    sema = CreateSema(&param);
    client->sema = sema;
    if (sema < 0) {
        _sceRpcFreePacket(packet);
        DeleteSema(client->completionSema);
        return -3;
    }
    if (sceSifSendCmd(0x80000019, packet, 0x40, NULL, NULL, 0) == 0) {
        _sceRpcFreePacket(packet);
        DeleteSema(sema);
        DeleteSema(client->completionSema);
        return -2;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return 0;
}

/* The unbind request prefix after its 16-byte SIF command header. */
typedef struct rpcUnbindPacket {
    u32 commandHeader[4];
    u32 flags;
    void *self;
    u32 sequence;
    rpcClient *client;
    u32 serverId;
    void *server;
} rpcUnbindPacket;

/* NON_MATCHING: 60 of 71 words match at the native 284-byte size; the
 * request/client prefix loads and stores use different ordering/registers. */
#ifdef NON_MATCHING
s32 sceSifMUnBindRpc(rpcClient *client, s32 mode)
{
    sdfSemaParam_t param;
    rpcPacket *packet;
    rpcUnbindPacket *request;
    s32 sema;
    s32 result;

    if (mode != 0) {
        scePrintf(D_0054ADD8);
        return -1;
    }
    WaitSema(client->completionSema);
    packet = _sceRpcGetPacket(&D_00599700);
    result = -1;
    if (packet != NULL) {
        request = (rpcUnbindPacket *)packet;
        client->sequence = request->sequence;
        client->packet = packet;
        request->serverId = client->serverId;
        request->self = packet;
        request->client = client;
        request->server = client->server;
        param.maxCount = 1;
        param.initCount = 0;
        param.option = 0;
        sema = CreateSema(&param);
        client->sema = sema;
        if (sema < 0) {
            _sceRpcFreePacket(packet);
            return -3;
        }
        if (sceSifSendCmd(0x8000001D, packet, 0x40, NULL, NULL, 0) == 0) {
            _sceRpcFreePacket(packet);
            DeleteSema(sema);
            return -2;
        }
        WaitSema(sema);
        DeleteSema(sema);
        DeleteSema(client->completionSema);
        result = client->result;
    }
    return result;
}
#else
INCLUDE_ASM(const s32, "sdk/libmrpc", sceSifMUnBindRpc);
#endif

INCLUDE_RODATA(const s32, "sdk/libmrpc", D_0054ADD8);

INCLUDE_ASM(const s32, "sdk/libmrpc", sceSifMCallRpc);
