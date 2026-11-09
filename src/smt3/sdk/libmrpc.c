#include "common.h"
#include "sdk_libmrpc.h"
extern void *memcpy(void *, const void *, size_t);

typedef struct sceSifMClientData rpcClient;
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

/* The allocator and command views occupy the same 64-byte packet storage. */
typedef union rpcPacket {
    u32 w[16];
    rpcUnbindPacket unbind;
} rpcPacket;
typedef char rpcPacketSize[(sizeof(rpcPacket) == 64) ? 1 : -1];
typedef char rpcPacketAlignment[(__alignof__(rpcPacket) == __alignof__(u32)) ? 1 : -1];

/* Observed prefix of the 0x40-byte M-RPC queue allocation. The remaining
 * bytes have no established fields and are not accessed through this view. */
typedef struct rpcQueue {
    u32 seq;
    rpcPacket *base;
    s32 count;
    u32 unk_C; /* Cleared during initialization. */
    u32 unk_10; /* Cleared during initialization. */
} rpcQueue;

extern s32 sceSifMRpcInitialized __attribute__((section(".data")));
extern rpcQueue sceSifMRpcQueue __attribute__((section(".data")));

extern s32 DIntr(void);
extern s32 EIntr(void);

extern rpcPacket sceSifMRpcPackets[32];
extern void _request_end(void *packet, void *queue);
extern void sceSifAddCmdHandler(s32 command, void (*handler)(void *, void *), void *argument);
extern u32 sceSifGetSreg(s32 index);
extern s32 sceSifSendCmd(s32 command, void *packet, s32 size, void *source, void *destination, s32 length);

void sceSifMInitRpc(s32 mode)
{
    rpcPacket *packets;
    rpcPacket *packet;
    rpcQueue *queue;

    if (sceSifMRpcInitialized != 0) {
        return;
    }
    sceSifMRpcInitialized = 1;
    DIntr();
    packets = sceSifMRpcPackets;
    queue = &sceSifMRpcQueue;
    queue->base = EE_ADDR_AS(rpcPacket *, (EE_ADDR(packets) | 0x20000000));
    queue->count = 32;
    queue->seq = 1;
    queue->unk_C = 0;
    queue->unk_10 = 0;
    sceSifAddCmdHandler(0x80000018, _request_end, queue);
    EIntr();
    packet = packets + 1;
    packet->w[4] = 1;
    packet->w[5] = 1;
    sceSifSendCmd(0x80000001, packet, 24, NULL, NULL, 0);
    while (sceSifGetSreg(1) == 0) {
    }
}

void sceSifMExitRpc(void)
{
    sceSifMRpcInitialized = 0;
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
            packet->w[5] = EE_ADDR(packet);
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

/* Observed completion-response prefix; fields after this prefix are unknown. */
typedef struct rpcResponse {
    u32 commandHeader[4];
    u32 flags;
    void *self;
    u32 sequence;
    rpcClient *client;
    u32 command;
    union {
        void *server;
        s32 result;
    } value;
    void *buffer;
    void *cbuffer;
} rpcResponse;

extern s32 iSignalSema(s32 sema);

void _request_end(void *packet, void *queue)
{
    rpcResponse *response = packet;
    rpcClient *client;

    switch (response->command) {
    case 0x8000001A:
        client = response->client;
        if (client->endFunction != NULL) {
            client->endFunction(client->endParam);
        }
        iSignalSema(client->completionSema);
        break;
    case 0x80000019:
        client = response->client;
        client->server = response->value.server;
        client->buffer = response->buffer;
        client->cbuffer = response->cbuffer;
        iSignalSema(client->completionSema);
        break;
    case 0x8000001D:
        client = response->client;
        client->result = response->value.result;
        iSignalSema(client->completionSema);
        break;
    case 0x8000001C:
        break;
    }
    client = response->client;
    if (client->sema >= 0) {
        iSignalSema(client->sema);
    }
    _sceRpcFreePacket(client->packet);
    client->packet = NULL;
}

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
extern char sceSifMModeErrorString[];

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
        scePrintf(sceSifMModeErrorString);
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
    packet = _sceRpcGetPacket(&sceSifMRpcQueue);
    if (packet == NULL) {
        DeleteSema(client->completionSema);
        return -1;
    }
    client->sequence = packet->w[6];
    client->packet = packet;
    packet->w[8] = serverId;
    packet->w[9] = arg0;
    packet->w[10] = arg1;
    packet->w[11] = arg2;
    packet->w[5] = EE_ADDR(packet);
    packet->w[7] = EE_ADDR(client);
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

/* The EE command transfers the four-byte server-pointer representation. */
typedef char rpcUnbindServerPointerSize[(sizeof(((rpcClient *)0)->server) == 4) ? 1 : -1];
typedef char rpcUnbindRequestPointerSize[(sizeof(((rpcUnbindPacket *)0)->server) == 4) ? 1 : -1];

s32 sceSifMUnBindRpc(rpcClient *client, s32 mode)
{
    sdfSemaParam_t param;
    rpcPacket *packet;
    s32 sema;
    s32 result;

    if (mode != 0) {
        scePrintf(sceSifMModeErrorString);
        return -1;
    }
    WaitSema(client->completionSema);
    packet = _sceRpcGetPacket(&sceSifMRpcQueue);
    result = -1;
    if (packet != NULL) {
        client->sequence = packet->unbind.sequence;
        client->packet = packet;
        packet->unbind.serverId = client->serverId;
        packet->unbind.self = packet;
        packet->unbind.client = client;
        memcpy(&packet->unbind.server, &client->server, sizeof(packet->unbind.server));
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

extern void sceSifWriteBackDCache(void *address, s32 size);

INCLUDE_RODATA(const s32, "sdk/libmrpc", sceSifMModeErrorString);

s32 sceSifMCallRpc(rpcClient *client, u32 function, u32 mode,
                   void *send, s32 sendSize, void *receive, s32 receiveSize,
                   void (*endFunction)(void *), void *endParam)
{
    sdfSemaParam_t param;
    rpcPacket *packet;
    s32 sema;

    if (mode != 0) {
        scePrintf(sceSifMModeErrorString);
        return -1;
    }
    WaitSema(client->completionSema);
    if (EE_ADDR(send) & 63) {
        scePrintf("%s: send buffer addr is not 64 byte align. %08x=n", "sceSifMCallRpc", EE_ADDR(send));
        return -1;
    }
    if (EE_ADDR(receive) & 63) {
        scePrintf("%s: receive buffer addr is not 64 byte align. %08x=n", "sceSifMCallRpc", EE_ADDR(receive));
        return -1;
    }
    packet = _sceRpcGetPacket(&sceSifMRpcQueue);
    if (packet == NULL) {
        return -1;
    }
    client->sequence = packet->w[6];
    client->packet = packet;
    client->endFunction = endFunction;
    client->endParam = endParam;
    packet->w[8] = function;
    packet->w[5] = EE_ADDR(packet);
    packet->w[9] = sendSize;
    packet->w[10] = EE_ADDR(receive);
    packet->w[11] = receiveSize;
    packet->w[7] = EE_ADDR(client);
    packet->w[13] = EE_ADDR(client->server);
    if (send == receive) {
        sceSifWriteBackDCache(send, sendSize < receiveSize ? receiveSize : sendSize);
    } else {
        if (sendSize > 0) {
            sceSifWriteBackDCache(send, sendSize);
        }
        if (receiveSize > 0) {
            sceSifWriteBackDCache(receive, receiveSize);
        }
    }
    /* The entry check rejects nonzero modes, but retail retains this
     * unreachable asynchronous-request arm. Keep its native call shape. */
    if (mode & 1) {
        if (endFunction == NULL) {
            packet->w[12] = 0;
        } else {
            packet->w[12] = 1;
        }
        client->sema = -1;
        if (sceSifSendCmd(0x8000001A, packet, 64, send, client->buffer, sendSize) == 0) {
            _sceRpcFreePacket(packet);
            return -2;
        }
        return 0;
    }
    param.maxCount = 1;
    param.initCount = 0;
    param.option = 0;
    sema = CreateSema(&param);
    client->sema = sema;
    if (sema < 0) {
        _sceRpcFreePacket(packet);
        return -3;
    }
    packet->w[12] = 1;
    if (sceSifSendCmd(0x8000001A, packet, 64, send, client->buffer, sendSize) == 0) {
        _sceRpcFreePacket(packet);
        DeleteSema(sema);
        return -2;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return 0;
}
