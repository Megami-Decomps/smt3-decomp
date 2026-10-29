#include "common.h"
#include "sif_rpc.h"

/* libkernl's first object, klib.o, has .text aligned to 64, so the retail link
 * pads 16 bytes after libc's last function to reach it. */
TEXT_ALIGN(6);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU000_FullReset);

INCLUDE_ASM(const s32, "sdk/libkernl", ResetEE);

INCLUDE_ASM(const s32, "sdk/libkernl", libc_2DBAA0);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU003);

INCLUDE_ASM(const s32, "sdk/libkernl", libc_2DBAC0);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU005);

INCLUDE_ASM(const s32, "sdk/libkernl", libc_2DBAE0);

INCLUDE_ASM(const s32, "sdk/libkernl", _ExecPS2);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU008);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU009);

INCLUDE_ASM(const s32, "sdk/libkernl", AddSbusIntcHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", RemoveSbusIntcHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", Interrupt2Iop);

INCLUDE_ASM(const s32, "sdk/libkernl", SetVTLBRefillHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", SetVCommonHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", SetVInterruptHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", AddIntcHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", AddIntcHandler2);

INCLUDE_ASM(const s32, "sdk/libkernl", RemoveIntcHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", AddDmacHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", AddDmacHandler2);

INCLUDE_ASM(const s32, "sdk/libkernl", RemoveDmacHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", _EnableIntc);

INCLUDE_ASM(const s32, "sdk/libkernl", _DisableIntc);

INCLUDE_ASM(const s32, "sdk/libkernl", _EnableDmac);

INCLUDE_ASM(const s32, "sdk/libkernl", _DisableDmac);

INCLUDE_ASM(const s32, "sdk/libkernl", SetAlarm);

INCLUDE_ASM(const s32, "sdk/libkernl", ReleaseAlarm);

INCLUDE_ASM(const s32, "sdk/libkernl", _iEnableIntc);

INCLUDE_ASM(const s32, "sdk/libkernl", _iDisableIntc);

INCLUDE_ASM(const s32, "sdk/libkernl", _iEnableDmac);

INCLUDE_ASM(const s32, "sdk/libkernl", _iDisableDmac);

INCLUDE_ASM(const s32, "sdk/libkernl", iSetAlarm);

INCLUDE_ASM(const s32, "sdk/libkernl", iReleaseAlarm);

INCLUDE_ASM(const s32, "sdk/libkernl", CreateThread);

INCLUDE_ASM(const s32, "sdk/libkernl", libc_2DBCB0);

INCLUDE_ASM(const s32, "sdk/libkernl", StartThread);

INCLUDE_ASM(const s32, "sdk/libkernl", ExitThread);

INCLUDE_ASM(const s32, "sdk/libkernl", ExitDeleteThread);

INCLUDE_ASM(const s32, "sdk/libkernl", TerminateThread);

INCLUDE_ASM(const s32, "sdk/libkernl", iTerminateThread);

INCLUDE_ASM(const s32, "sdk/libkernl", DisableDispatchThread);

INCLUDE_ASM(const s32, "sdk/libkernl", EnableDispatchThread);

INCLUDE_ASM(const s32, "sdk/libkernl", ChangeThreadPriority);

INCLUDE_ASM(const s32, "sdk/libkernl", iChangeThreadPriority);

INCLUDE_ASM(const s32, "sdk/libkernl", RotateThreadReadyQueue);

INCLUDE_ASM(const s32, "sdk/libkernl", _iRotateThreadReadyQueue);

INCLUDE_ASM(const s32, "sdk/libkernl", ReleaseWaitThread);

INCLUDE_ASM(const s32, "sdk/libkernl", iReleaseWaitThread);

INCLUDE_ASM(const s32, "sdk/libkernl", GetThreadId);

INCLUDE_ASM(const s32, "sdk/libkernl", ReferThreadStatus);

INCLUDE_ASM(const s32, "sdk/libkernl", iReferThreadStatus);

INCLUDE_ASM(const s32, "sdk/libkernl", SleepThread);

INCLUDE_ASM(const s32, "sdk/libkernl", WakeupThread);

INCLUDE_ASM(const s32, "sdk/libkernl", _iWakeupThread);

INCLUDE_ASM(const s32, "sdk/libkernl", CancelWakeupThread);

INCLUDE_ASM(const s32, "sdk/libkernl", iCancelWakeupThread);

INCLUDE_ASM(const s32, "sdk/libkernl", SuspendThread);

INCLUDE_ASM(const s32, "sdk/libkernl", _iSuspendThread);

INCLUDE_ASM(const s32, "sdk/libkernl", ResumeThread);

INCLUDE_ASM(const s32, "sdk/libkernl", iResumeThread);

INCLUDE_ASM(const s32, "sdk/libkernl", JoinThread);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU060);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU061);

INCLUDE_ASM(const s32, "sdk/libkernl", EndOfHeap);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU063);

INCLUDE_ASM(const s32, "sdk/libkernl", CreateSema);

INCLUDE_ASM(const s32, "sdk/libkernl", DeleteSema);

INCLUDE_ASM(const s32, "sdk/libkernl", SignalSema);

INCLUDE_ASM(const s32, "sdk/libkernl", iSignalSema);

INCLUDE_ASM(const s32, "sdk/libkernl", WaitSema);

INCLUDE_ASM(const s32, "sdk/libkernl", PollSema);

INCLUDE_ASM(const s32, "sdk/libkernl", iPollSema);

INCLUDE_ASM(const s32, "sdk/libkernl", ReferSemaStatus);

INCLUDE_ASM(const s32, "sdk/libkernl", iReferSemaStatus);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU073);

INCLUDE_ASM(const s32, "sdk/libkernl", SetOsdConfigParam);

INCLUDE_ASM(const s32, "sdk/libkernl", GetOsdConfigParam);

INCLUDE_ASM(const s32, "sdk/libkernl", GetGsHParam);

INCLUDE_ASM(const s32, "sdk/libkernl", GetGsVParam);

INCLUDE_ASM(const s32, "sdk/libkernl", SetGsHParam);

INCLUDE_ASM(const s32, "sdk/libkernl", SetGsVParam);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU080_CreateEventFlag);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU081_DeleteEventFlag);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU082_SetEventFlag);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU083_iSetEventFlag);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU084_ClearEventFlag);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU085_iClearEventFlag);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU086_WaitEvnetFlag);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU087_PollEvnetFlag);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU088_iPollEvnetFlag);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU089_ReferEventFlagStatus);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU090_iReferEventFlagStatus);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU091);

INCLUDE_ASM(const s32, "sdk/libkernl", EnableIntcHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", iEnableIntcHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", DisableIntcHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", iDisableIntcHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", EnableDmacHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", iEnableDmacHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", DisableDmacHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", iDisableDmacHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", KSeg0);

INCLUDE_ASM(const s32, "sdk/libkernl", EnableCache);

INCLUDE_ASM(const s32, "sdk/libkernl", DisableCache);

INCLUDE_ASM(const s32, "sdk/libkernl", GetCop0);

INCLUDE_ASM(const s32, "sdk/libkernl", FlushCache);

INCLUDE_ASM(const s32, "sdk/libkernl", CpuConfig);

INCLUDE_ASM(const s32, "sdk/libkernl", iGetCop0);

INCLUDE_ASM(const s32, "sdk/libkernl", iFlushCache);

INCLUDE_ASM(const s32, "sdk/libkernl", iCpuConfig);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifStopDma);

INCLUDE_ASM(const s32, "sdk/libkernl", SetCPUTimerHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", SetCPUTimer);

INCLUDE_ASM(const s32, "sdk/libkernl", SetOsdConfigParam2);

INCLUDE_ASM(const s32, "sdk/libkernl", GetOsdConfigParam2);

INCLUDE_ASM(const s32, "sdk/libkernl", GsGetIMR);

INCLUDE_ASM(const s32, "sdk/libkernl", iGsGetIMR);

INCLUDE_ASM(const s32, "sdk/libkernl", GsPutIMR);

INCLUDE_ASM(const s32, "sdk/libkernl", iGsPutIMR);

INCLUDE_ASM(const s32, "sdk/libkernl", SetPgifHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", SetVSyncFlag);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU116);

INCLUDE_ASM(const s32, "sdk/libkernl", _print);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifDmaStat);

INCLUDE_ASM(const s32, "sdk/libkernl", isceSifDmaStat);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifSetDma);

INCLUDE_ASM(const s32, "sdk/libkernl", isceSifSetDma);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifSetDChain);

INCLUDE_ASM(const s32, "sdk/libkernl", isceSifSetDChain);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifSetReg);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifGetReg);

INCLUDE_ASM(const s32, "sdk/libkernl", libc_2DC2C0);

INCLUDE_ASM(const s32, "sdk/libkernl", Deci2Call);

INCLUDE_ASM(const s32, "sdk/libkernl", PSMode);

INCLUDE_ASM(const s32, "sdk/libkernl", MachineType);

INCLUDE_ASM(const s32, "sdk/libkernl", GetMemorySize);

INCLUDE_ASM(const s32, "sdk/libkernl", _InitTLB);

INCLUDE_ASM(const s32, "sdk/libkernl", sceResetttyinit);

INCLUDE_ASM(const s32, "sdk/libkernl", VSync);

INCLUDE_ASM(const s32, "sdk/libkernl", VSync2);

INCLUDE_ASM(const s32, "sdk/libkernl", write);

INCLUDE_ASM(const s32, "sdk/libkernl", read);

INCLUDE_ASM(const s32, "sdk/libkernl", open);

INCLUDE_ASM(const s32, "sdk/libkernl", close);

INCLUDE_ASM(const s32, "sdk/libkernl", ioctl);

INCLUDE_ASM(const s32, "sdk/libkernl", lseek);

INCLUDE_ASM(const s32, "sdk/libkernl", sbrk);

INCLUDE_ASM(const s32, "sdk/libkernl", isatty);

INCLUDE_ASM(const s32, "sdk/libkernl", fstat);

INCLUDE_ASM(const s32, "sdk/libkernl", libc_2DC670);

INCLUDE_ASM(const s32, "sdk/libkernl", kill);

INCLUDE_ASM(const s32, "sdk/libkernl", stat);

INCLUDE_ASM(const s32, "sdk/libkernl", unlink);

INCLUDE_ASM(const s32, "sdk/libkernl", _sceSDC);

INCLUDE_ASM(const s32, "sdk/libkernl", SyncDCache);

INCLUDE_ASM(const s32, "sdk/libkernl", iSyncDCache);

INCLUDE_ASM(const s32, "sdk/libkernl", _sceIDC);

INCLUDE_ASM(const s32, "sdk/libkernl", InvalidDCache);

INCLUDE_ASM(const s32, "sdk/libkernl", iInvalidDCache);

INCLUDE_ASM(const s32, "sdk/libkernl", func_002DC970);

INCLUDE_ASM(const s32, "sdk/libkernl", DisableIntc);

INCLUDE_ASM(const s32, "sdk/libkernl", EnableIntc);

INCLUDE_ASM(const s32, "sdk/libkernl", DisableDmac);

INCLUDE_ASM(const s32, "sdk/libkernl", EnableDmac);

INCLUDE_ASM(const s32, "sdk/libkernl", iEnableIntc);

INCLUDE_ASM(const s32, "sdk/libkernl", iDisableIntc);

INCLUDE_ASM(const s32, "sdk/libkernl", iEnableDmac);

INCLUDE_ASM(const s32, "sdk/libkernl", iDisableDmac);

INCLUDE_ASM(const s32, "sdk/libkernl", topThread);

INCLUDE_ASM(const s32, "sdk/libkernl", InitThread);

INCLUDE_ASM(const s32, "sdk/libkernl", iWakeupThread);

INCLUDE_ASM(const s32, "sdk/libkernl", iRotateThreadReadyQueue);

INCLUDE_ASM(const s32, "sdk/libkernl", iSuspendThread);

typedef struct ttyQueue_t {
    u32 capacity;
    /* The TTY callback produces bytes while sceTtyRead polls occupancy. */
    volatile s32 count;
    u8 *read;
    u8 *write;
    u8 data[256];
} ttyQueue_t;

extern ttyQueue_t D_0059B580;

ttyQueue_t *gcc2_compiled__10(u32 capacity)
{
    ttyQueue_t *queue = &D_0059B580;

    queue->capacity = capacity;
    queue->count = 0;
    queue->write = queue->data;
    queue->read = queue->data;
    return queue;
}

void QueuePeekWriteDone(ttyQueue_t *queue)
{
    queue->count++;
    queue->write++;
    if (queue->write == queue->data + queue->capacity) {
        queue->write = queue->data;
    }
}

void QueuePeekReadDone(ttyQueue_t *queue)
{
    queue->count--;
    queue->read++;
    if (queue->read == queue->data + queue->capacity) {
        queue->read = queue->data;
    }
}

INCLUDE_ASM(const s32, "sdk/libkernl", sceTtyHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", sceTtyWrite);

INCLUDE_ASM(const s32, "sdk/libkernl", sceTtyRead);

INCLUDE_ASM(const s32, "sdk/libkernl", sceTtyInit);

INCLUDE_ASM(const s32, "sdk/libkernl", kputchar);

extern s32 D_0052C2B0 __attribute__((section(".data")));
extern char D_0059B940[128];
extern s32 kputs(const char *text);

void deci2Putchar(s32 character)
{
    s32 index;

    index = D_0052C2B0;
    if (index >= 126) {
        D_0052C2B0 = 0;
        D_0059B940[127] = 0;
        kputs(D_0059B940);
        index = D_0052C2B0;
    }
    if (character == 10) {
        D_0052C2B0 = 0;
        D_0059B940[index] = character;
        D_0059B940[index + 1] = 0;
        kputs(D_0059B940);
    } else {
        D_0052C2B0 = index + 1;
        D_0059B940[index] = character;
    }
}

INCLUDE_ASM(const s32, "sdk/libkernl", serialPutchar);

INCLUDE_ASM(const s32, "sdk/libkernl", ftoi);

INCLUDE_ASM(const s32, "sdk/libkernl", printfloat);

INCLUDE_ASM(const s32, "sdk/libkernl", _printf);

/* In ee-gcc 2.9, next_arg names the first incoming stack argument. These
 * one-fixed-argument wrappers save the seven remaining argument GPRs in
 * contiguous eight-byte slots immediately before it. The formatter walks
 * that register-save area and then continues into the stack arguments. */
typedef char *kernelVaList_t;
extern void _printf(const char *format, kernelVaList_t arguments);
extern void (*D_0052C2B4)(s32 character) __attribute__((section(".data")));

void kprintf(const char *format, ...)
{
    kernelVaList_t arguments;

    arguments = (kernelVaList_t)__builtin_next_arg(format) - 7 * sizeof(long);
    _printf(format, arguments);
}

void scePrintf(const char *format, ...)
{
    kernelVaList_t arguments;
    void (*previous)(s32);

    previous = D_0052C2B4;
    D_0052C2B4 = deci2Putchar;
    arguments = (kernelVaList_t)__builtin_next_arg(format) - 7 * sizeof(long);
    _printf(format, arguments);
    D_0052C2B4 = previous;
}

INCLUDE_ASM(const s32, "sdk/libkernl", gcc2_compiled__12);

INCLUDE_ASM(const s32, "sdk/libkernl", _change_addr);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifGetSreg);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifSetSreg);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifGetDataTable);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifInitCmd);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifExitCmd);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifSetCmdBuffer);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifSetSysCmdBuffer);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifAddCmdHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifRemoveCmdHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", _sceSifSendCmd);

extern s32 _sceSifSendCmd(s32 command, s32 interruptMode, void *packet, s32 packetBytes,
                           void *source, void *destination, s32 dataBytes);

s32 sceSifSendCmd(s32 command, void *packet, s32 packetBytes,
                  void *source, void *destination, s32 dataBytes)
{
    return _sceSifSendCmd(command, 0, packet, packetBytes, source, destination, dataBytes);
}

s32 isceSifSendCmd(s32 command, void *packet, s32 packetBytes,
                   void *source, void *destination, s32 dataBytes)
{
    return _sceSifSendCmd(command, 1, packet, packetBytes, source, destination, dataBytes);
}

INCLUDE_ASM(const s32, "sdk/libkernl", _sceSifCmdIntrHdlr);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifWriteBackDCache);

INCLUDE_ASM(const s32, "sdk/libkernl", func_002DE504);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifInitRpc);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifExitRpc);

typedef struct rpcPacket {
    u32 w[16];
} rpcPacket;

typedef struct rpcQueue {
    u32 seq;
    rpcPacket *base;
    s32 count;
    u32 unk_C; /* Cleared by sceSifInitRpc. */
    u32 unk_10; /* Cleared by sceSifInitRpc. */
    rpcPacket *forwardBase;
    s32 forwardCount;
    rpcPacket *indexedBase;
    s32 indexedCount;
    s32 forwardIndex;
    sifRpcDataQueue_t *queues;
} rpcQueue;

extern s32 DIntr(void);
extern s32 EIntr(void);

rpcPacket *_sceRpcGetPacket_0(rpcQueue *queue)
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


void _sceRpcFreePacket_0(rpcPacket *packet)
{
    u32 flags = packet->w[4];

    packet->w[6] = 0;
    packet->w[4] = flags & ~1;
}

rpcPacket *_sceRpcGetFPacket(rpcQueue *queue)
{
    s32 index = queue->forwardIndex % queue->forwardCount;
    rpcPacket *packet = &queue->forwardBase[index];

    queue->forwardIndex = index + 1;
    return packet;
}

rpcPacket *_sceRpcGetFPacket2(rpcQueue *queue, s32 index)
{
    if (index < 0 || index >= queue->indexedCount) {
        return _sceRpcGetFPacket(queue);
    }
    return &queue->indexedBase[index];
}

INCLUDE_ASM(const s32, "sdk/libkernl", _request_end_0);

INCLUDE_ASM(const s32, "sdk/libkernl", _request_rdata);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifGetOtherData);

INCLUDE_ASM(const s32, "sdk/libkernl", _search_svdata);

INCLUDE_ASM(const s32, "sdk/libkernl", _request_bind);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifBindRpc);

INCLUDE_ASM(const s32, "sdk/libkernl", _request_call);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifCallRpc);

typedef struct rpcClientPrefix {
    void *packet;
    u32 sequence;
} rpcClientPrefix;

/* NON_MATCHING: 44 of 60 native bytes, 5 of 11 compared words match;
 * the final active-bit test becomes a direct boolean return. */
#ifdef NON_MATCHING
s32 sceSifCheckStatRpc(rpcClientPrefix *client)
{
    rpcPacket *packet = client->packet;

    if (packet != NULL) {
        if (client->sequence == packet->w[6]) {
            if ((packet->w[4] & 1) != 0) {
                return 1;
            }
        }
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "sdk/libkernl", sceSifCheckStatRpc);
#endif

extern rpcQueue D_0059D440;

/* NON_MATCHING: 140 of 148 native bytes; linked-list cursor copies coalesce. */
#ifdef NON_MATCHING
void sceSifSetRpcQueue(sifRpcDataQueue_t *queue, s32 threadId)
{
    sifRpcDataQueue_t *current;

    DIntr();
    queue->threadId = threadId;
    queue->active = 0;
    queue->link = NULL;
    queue->start = NULL;
    queue->end = NULL;
    queue->next = NULL;
    current = D_0059D440.queues;
    if (current == NULL) {
        D_0059D440.queues = queue;
    } else {
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = queue;
    }
    EIntr();
}
#else
INCLUDE_ASM(const s32, "sdk/libkernl", sceSifSetRpcQueue);
#endif

/* NON_MATCHING: 200 of 208 native bytes; the same append-loop cursor copies coalesce. */
#ifdef NON_MATCHING
void sceSifRegisterRpc(sifRpcServerData_t *server, s32 serviceId,
                        sifRpcFunc_t handler, void *buffer,
                        sifRpcFunc_t callback, void *callbackBuffer,
                        sifRpcDataQueue_t *queue)
{
    sifRpcServerData_t *current;

    DIntr();
    server->next = NULL;
    server->link = NULL;
    server->serviceId = serviceId;
    server->handler = handler;
    server->buffer = buffer;
    server->callback = callback;
    server->callbackBuffer = callbackBuffer;
    server->queue = queue;
    current = queue->link;
    if (current == NULL) {
        queue->link = server;
    } else {
        while (current->link != NULL) {
            current = current->link;
        }
        current->link = server;
    }
    EIntr();
}
#else
INCLUDE_ASM(const s32, "sdk/libkernl", sceSifRegisterRpc);
#endif

sifRpcServerData_t *sceSifRemoveRpc(sifRpcServerData_t *server, sifRpcDataQueue_t *queue)
{
    sifRpcServerData_t *current;

    DIntr();
    current = queue->link;
    if (current == server) {
        queue->link = current->link;
    } else {
        while (current != NULL) {
            if (current->link == server) {
                current->link = server->link;
                break;
            }
            current = current->link;
        }
    }
    EIntr();
    return current;
}

sifRpcDataQueue_t *sceSifRemoveRpcQueue(sifRpcDataQueue_t *queue)
{
    sifRpcDataQueue_t *current;

    DIntr();
    current = D_0059D440.queues;
    if (current == queue) {
        D_0059D440.queues = current->next;
    } else {
        while (current != NULL) {
            if (current->next == queue) {
                current->next = queue->next;
                break;
            }
            current = current->next;
        }
    }
    EIntr();
    return current;
}

sifRpcServerData_t *sceSifGetNextRequest(sifRpcDataQueue_t *queue)
{
    sifRpcServerData_t *request;

    DIntr();
    request = queue->start;
    if (request == NULL) {
        queue->active = 0;
    } else {
        queue->active = 1;
        queue->start = request->next;
    }
    EIntr();
    return request;
}

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifExecRequest);

extern void sceSifExecRequest(sifRpcServerData_t *request);
extern s32 SleepThread(void);

void sceSifRpcLoop(sifRpcDataQueue_t *queue)
{
    sifRpcServerData_t *request;

    for (;;) {
        while ((request = sceSifGetNextRequest(queue)) != NULL) {
            sceSifExecRequest(request);
        }
        SleepThread();
    }
}

INCLUDE_ASM(const s32, "sdk/libkernl", gcc2_compiled__13);

INCLUDE_ASM(const s32, "sdk/libkernl", new_iob);

INCLUDE_ASM(const s32, "sdk/libkernl", get_iob);

INCLUDE_ASM(const s32, "sdk/libkernl", _sceFs_Rcv_Intr);

INCLUDE_ASM(const s32, "sdk/libkernl", _sceFsSemInit);

INCLUDE_ASM(const s32, "sdk/libkernl", _sceFsWaitS);

INCLUDE_ASM(const s32, "sdk/libkernl", _sceFsSigSema);

INCLUDE_ASM(const s32, "sdk/libkernl", scePowerOffHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", _sceFs_Poff_Intr);

INCLUDE_ASM(const s32, "sdk/libkernl", sceFsInit);

INCLUDE_ASM(const s32, "sdk/libkernl", _fs_version);

INCLUDE_ASM(const s32, "sdk/libkernl", sceFsReset);

INCLUDE_ASM(const s32, "sdk/libkernl", sceOpen);

INCLUDE_ASM(const s32, "sdk/libkernl", sceClose);

INCLUDE_ASM(const s32, "sdk/libkernl", sceLseek);

INCLUDE_ASM(const s32, "sdk/libkernl", sceRead);

INCLUDE_ASM(const s32, "sdk/libkernl", sceWrite);

INCLUDE_ASM(const s32, "sdk/libkernl", sceIoctl);

INCLUDE_ASM(const s32, "sdk/libkernl", sceIoctl2);

INCLUDE_ASM(const s32, "sdk/libkernl", _sceCallCode);

INCLUDE_ASM(const s32, "sdk/libkernl", sceRemove);

INCLUDE_ASM(const s32, "sdk/libkernl", sceMkdir);

INCLUDE_ASM(const s32, "sdk/libkernl", sceRmdir);

INCLUDE_ASM(const s32, "sdk/libkernl", sceFormat);

INCLUDE_ASM(const s32, "sdk/libkernl", sceAddDrv);

INCLUDE_ASM(const s32, "sdk/libkernl", sceDelDrv);

INCLUDE_ASM(const s32, "sdk/libkernl", sceDopen);

INCLUDE_ASM(const s32, "sdk/libkernl", sceDclose);

INCLUDE_ASM(const s32, "sdk/libkernl", sceDread);

INCLUDE_ASM(const s32, "sdk/libkernl", sceGetstat);

INCLUDE_ASM(const s32, "sdk/libkernl", sceChstat);

INCLUDE_ASM(const s32, "sdk/libkernl", sceRename);

INCLUDE_ASM(const s32, "sdk/libkernl", sceChdir);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSync);

INCLUDE_ASM(const s32, "sdk/libkernl", sceMount);

INCLUDE_ASM(const s32, "sdk/libkernl", sceUmount);

INCLUDE_ASM(const s32, "sdk/libkernl", sceLseek64);

INCLUDE_ASM(const s32, "sdk/libkernl", sceDevctl);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSymlink);

INCLUDE_ASM(const s32, "sdk/libkernl", sceReadlink);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifInitIopHeap);

extern s32 D_0052C354;
/* The BSS map bounds this opaque RPC client to 0x40 bytes. */
extern u8 D_0059E840[0x40];
extern s32 D_0059E880;
extern s32 D_0059E8C0;
extern s32 sceSifCallRpc(void *client, s32 function, s32 mode, void *send,
                         s32 sendBytes, void *receive, s32 receiveBytes,
                         void *endFunction, void *endParameter);

s32 sceSifAllocIopHeap(s32 size)
{
    if (D_0052C354 < 0) {
        return 0;
    }
    D_0059E8C0 = size;
    if (sceSifCallRpc(D_0059E840, 1, 0, &D_0059E8C0, 4,
                      &D_0059E880, 4, NULL, NULL) >= 0) {
        return D_0059E880;
    }
    return 0;
}


INCLUDE_ASM(const s32, "sdk/libkernl", sceSifAllocSysMemory);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifFreeSysMemory);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifFreeIopHeap);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifLoadIopHeap);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifQueryMemSize);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifQueryMaxFreeMemSize);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifQueryTotalFreeMemSize);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifQueryBlockTopAddress);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifQueryBlockSize);

INCLUDE_ASM(const s32, "sdk/libkernl", _lf_bind);

INCLUDE_ASM(const s32, "sdk/libkernl", _lf_version);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifLoadFileReset);

INCLUDE_ASM(const s32, "sdk/libkernl", _sceSifLoadModuleBuffer);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifStopModule);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifUnloadModule);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifSearchModuleByName);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifSearchModuleByAddress);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifLoadModuleBuffer);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifLoadStartModuleBuffer);

INCLUDE_ASM(const s32, "sdk/libkernl", _sceSifLoadModule);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifLoadModule);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifLoadStartModule);

INCLUDE_ASM(const s32, "sdk/libkernl", _sceSifLoadElfPart);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifLoadElfPart);

INCLUDE_RODATA(const s32, "sdk/libkernl", D_0054BCA8);

INCLUDE_RODATA(const s32, "sdk/libkernl", D_0054BCB0);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifLoadElf);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifGetIopAddr);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifSetIopAddr);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifResetIop);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifIsAliveIop);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifSyncIop);

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifRebootIop);

INCLUDE_ASM(const s32, "sdk/libkernl", DIntr);

INCLUDE_ASM(const s32, "sdk/libkernl", EIntr);

INCLUDE_ASM(const s32, "sdk/libkernl", supplement_crt0);

INCLUDE_ASM(const s32, "sdk/libkernl", Copy);

INCLUDE_ASM(const s32, "sdk/libkernl", kCopy);

INCLUDE_ASM(const s32, "sdk/libkernl", kFindAddress);

INCLUDE_ASM(const s32, "sdk/libkernl", FindAddress);

INCLUDE_ASM(const s32, "sdk/libkernl", GetSystemCallEntry);

INCLUDE_ASM(const s32, "sdk/libkernl", GetSystemCallTableEntry);

INCLUDE_ASM(const s32, "sdk/libkernl", setup);

INCLUDE_ASM(const s32, "sdk/libkernl", _InitSys);

INCLUDE_ASM(const s32, "sdk/libkernl", gcc2_compiled__15);

INCLUDE_ASM(const s32, "sdk/libkernl", Copy_0);

INCLUDE_ASM(const s32, "sdk/libkernl", kCopy_0);

INCLUDE_ASM(const s32, "sdk/libkernl", GetEntryAddress);

INCLUDE_ASM(const s32, "sdk/libkernl", PatchIsNeeded_0);

INCLUDE_ASM(const s32, "sdk/libkernl", InitExecPS2);

INCLUDE_ASM(const s32, "sdk/libkernl", gcc2_compiled__16);

INCLUDE_ASM(const s32, "sdk/libkernl", kCopy_1);

INCLUDE_ASM(const s32, "sdk/libkernl", PatchIsNeeded);

INCLUDE_ASM(const s32, "sdk/libkernl", SetArg);

INCLUDE_ASM(const s32, "sdk/libkernl", TerminateLibrary);

INCLUDE_ASM(const s32, "sdk/libkernl", ExecPS2);

INCLUDE_ASM(const s32, "sdk/libkernl", LoadExecPS2);

INCLUDE_ASM(const s32, "sdk/libkernl", Exit);

INCLUDE_ASM(const s32, "sdk/libkernl", ExecOSD);

INCLUDE_ASM(const s32, "sdk/libkernl", gcc2_compiled__17);

INCLUDE_ASM(const s32, "sdk/libkernl", Copy_1);

INCLUDE_ASM(const s32, "sdk/libkernl", kCopy_2);

INCLUDE_ASM(const s32, "sdk/libkernl", GetEntryAddress_0);

INCLUDE_ASM(const s32, "sdk/libkernl", InitAlarm);

extern s32 Deci2Call(s32 command, void *packet);
extern u8 D_0059ECA8[];

typedef void (*deci2Handler_t)(s32 event, s32 parameter, void *argument);

typedef struct deci2Registration_t {
    u32 protocol;
    void *argument;
    deci2Handler_t handler;
    void *storage;
} deci2Registration_t;

s32 sceDeci2Open(u16 protocol, void *argument, deci2Handler_t handler)
{
    deci2Registration_t packet;

    packet.protocol = protocol;
    packet.argument = argument;
    packet.handler = handler;
    packet.storage = (void *)((u32)D_0059ECA8 | 0x20000000);
    return Deci2Call(1, &packet);
}

s32 sceDeci2Close(s32 handle)
{
    s32 packet[1];

    packet[0] = handle;
    return Deci2Call(2, packet);
}

s32 sceDeci2ReqSend(s32 handle, s8 destination)
{
    s32 packet[2];

    packet[0] = handle;
    packet[1] = destination;
    return Deci2Call(3, packet);
}

s32 sceDeci2Poll(s32 handle)
{
    s32 packet[1];

    packet[0] = handle;
    return Deci2Call(4, packet);
}

typedef struct deci2Transfer_t {
    s32 handle;
    void *buffer;
    u32 length;
} deci2Transfer_t;

s32 sceDeci2ExRecv(s32 handle, void *buffer, u16 length)
{
    deci2Transfer_t packet;

    packet.handle = handle;
    packet.buffer = buffer;
    packet.length = length;
    return Deci2Call(-5, &packet);
}

s32 sceDeci2ExSend(s32 handle, void *buffer, u16 length)
{
    deci2Transfer_t packet;

    packet.handle = handle;
    packet.buffer = buffer;
    packet.length = length;
    return Deci2Call(-6, &packet);
}

s32 sceDeci2ExReqSend(s32 handle, s8 destination)
{
    s32 packet[2];

    packet[0] = handle;
    packet[1] = destination;
    return Deci2Call(-7, packet);
}

s32 sceDeci2ExLock(s32 handle)
{
    s32 packet[1];

    packet[0] = handle;
    return Deci2Call(-8, packet);
}

s32 sceDeci2ExUnLock(s32 handle)
{
    s32 packet[1];

    packet[0] = handle;
    return Deci2Call(-9, packet);
}

s32 kputs(const char *text)
{
    const char *packet[1];

    packet[0] = text;
    return Deci2Call(16, packet);
}

INCLUDE_ASM(const s32, "sdk/libkernl", SetTLBHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", SetDebugHandler);

INCLUDE_ASM(const s32, "sdk/libkernl", Copy_2);

INCLUDE_ASM(const s32, "sdk/libkernl", kCopy_3);

INCLUDE_ASM(const s32, "sdk/libkernl", GetEntryAddress_1);

INCLUDE_ASM(const s32, "sdk/libkernl", setup_0);

INCLUDE_ASM(const s32, "sdk/libkernl", InitTLBFunctions);

INCLUDE_ASM(const s32, "sdk/libkernl", PutTLBEntry);

INCLUDE_ASM(const s32, "sdk/libkernl", iPutTLBEntry_0);

INCLUDE_ASM(const s32, "sdk/libkernl", _SetTLBEntry);

INCLUDE_ASM(const s32, "sdk/libkernl", iPutTLBEntry);

INCLUDE_ASM(const s32, "sdk/libkernl", iSetTLBEntry);

INCLUDE_ASM(const s32, "sdk/libkernl", GetTLBEntry);

INCLUDE_ASM(const s32, "sdk/libkernl", iGetTLBEntry);

INCLUDE_ASM(const s32, "sdk/libkernl", ProbeTLBEntry);

INCLUDE_ASM(const s32, "sdk/libkernl", iProbeTLBEntry);

INCLUDE_ASM(const s32, "sdk/libkernl", ExpandScratchPad);

INCLUDE_ASM(const s32, "sdk/libkernl", InitTLB);

INCLUDE_RODATA(const s32, "sdk/libkernl", D_0054BCF0);

INCLUDE_ASM(const s32, "sdk/libkernl", InitTLB32MB);

INCLUDE_ASM(const s32, "sdk/libkernl", _kTLBException);

INCLUDE_ASM(const s32, "sdk/libkernl", _xlaunch);

INCLUDE_ASM(const s32, "sdk/libkernl", _kDebugException);
