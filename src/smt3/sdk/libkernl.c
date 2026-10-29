#include "common.h"
#include "newlib_stat.h"
#include "ps2asm.h"
#include "sif_rpc.h"
#include "ps2asm.h"

/* libkernl's first object, klib.o, has .text aligned to 64, so the retail link
 * pads 16 bytes after libc's last function to reach it. */
TEXT_ALIGN(6);

INCLUDE_ASM(const s32, "sdk/libkernl", RFU000_FullReset);

INCLUDE_ASM(const s32, "sdk/libkernl", ResetEE);

INCLUDE_ASM(const s32, "sdk/libkernl", SetGsCrt);

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

INCLUDE_ASM(const s32, "sdk/libkernl", DeleteThread);

INCLUDE_ASM(const s32, "sdk/libkernl", _StartThread);

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

INCLUDE_ASM(const s32, "sdk/libkernl", _ExecOSD);

INCLUDE_ASM(const s32, "sdk/libkernl", Deci2Call);

INCLUDE_ASM(const s32, "sdk/libkernl", PSMode);

INCLUDE_ASM(const s32, "sdk/libkernl", MachineType);

INCLUDE_ASM(const s32, "sdk/libkernl", GetMemorySize);

INCLUDE_ASM(const s32, "sdk/libkernl", _InitTLB);

INCLUDE_ASM(const s32, "sdk/libkernl", sceResetttyinit);

extern s32 DIntr(void);
extern s32 EIntr(void);

void VSync(void)
{
    s32 enabled;

    enabled = DIntr();
    *(volatile u32 *)0x1000F000 = 4;
    EE_SYNC();
    if (enabled != 0) {
        EIntr();
    }
    while ((*(volatile u32 *)0x1000F000 & 4) == 0) {
    }
    enabled = DIntr();
    *(volatile u32 *)0x1000F000 = 4;
    EE_SYNC();
    if (enabled != 0) {
        EIntr();
    }
}

extern void SetVSyncFlag(volatile u32 *flag, volatile u64 *result);

u64 VSync2(void)
{
    /* Kernel-published outputs: keep the result read after interrupt restore. */
    volatile u32 flag = 0;
    volatile u64 result;
    s32 enabled;

    SetVSyncFlag(&flag, &result);
    enabled = DIntr();
    *(volatile u32 *)0x1000F000 = 4;
    EE_SYNC();
    if (enabled != 0) {
        EIntr();
    }
    while ((*(volatile u32 *)0x1000F000 & 4) == 0 && flag == 0) {
    }
    enabled = DIntr();
    *(volatile u32 *)0x1000F000 = 4;
    EE_SYNC();
    if (enabled != 0) {
        EIntr();
    }
    return result;
}

extern s32 D_0052C2A4 __attribute__((section(".data")));
extern s32 sceTtyInit(void);
extern s32 sceTtyRead(char *buffer, s32 length);
extern s32 sceTtyWrite(const char *buffer, s32 length);

s32 write(s32 descriptor, const void *buffer, s32 length)
{
    if ((u32)descriptor - 1 < 2) {
        if (D_0052C2A4 == 0) {
            if (sceTtyInit() == 0) {
                return -1;
            }
            D_0052C2A4 = 1;
        }
        return sceTtyWrite(buffer, length);
    }
    return -1;
}

s32 read(s32 descriptor, void *buffer, s32 length)
{
    if (descriptor == 0) {
        if (D_0052C2A4 == 0) {
            if (sceTtyInit() == 0) {
                return -1;
            }
            D_0052C2A4 = 1;
        }
        return sceTtyRead(buffer, length);
    }
    return -1;
}

INCLUDE_ASM(const s32, "sdk/libkernl", open);

INCLUDE_ASM(const s32, "sdk/libkernl", close);

INCLUDE_ASM(const s32, "sdk/libkernl", ioctl);

INCLUDE_ASM(const s32, "sdk/libkernl", lseek);

INCLUDE_ASM(const s32, "sdk/libkernl", sbrk);

INCLUDE_ASM(const s32, "sdk/libkernl", isatty);

s32 fstat(s32 fd, struct stat *st)
{
    st->st_mode = 0x2000;
    st->st_blksize = 0;
    return 0;
}

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

typedef struct threadMgrQueue_t {
    u32 readIndex;
    u32 writeIndex;
    u8 commands[512][2];
} threadMgrQueue_t;

extern s32 threadMgrSema;
extern s32 WaitSema(s32 sema);
extern s32 WakeupThread(s32 threadId);
extern s32 RotateThreadReadyQueue(s32 priority);
extern s32 SuspendThread(s32 threadId);
extern void kprintf(const char *format, ...);
extern const char D_0054BA48[];

void topThread(void *argument)
{
    threadMgrQueue_t *queue = argument;
    u32 index;

    for (;;) {
        WaitSema(threadMgrSema);
        index = queue->readIndex & 0x1FF;
        queue->readIndex = index + 1;
        switch (queue->commands[index][0]) {
        case 0:
            WakeupThread(queue->commands[index][1]);
            break;
        case 1:
            RotateThreadReadyQueue(queue->commands[index][1]);
            break;
        case 2:
            SuspendThread(queue->commands[index][1]);
            break;
        default:
            kprintf(D_0054BA48);
            break;
        }
    }
}

typedef struct rpcSemaParam {
    s32 count;
    s32 maxCount;
    s32 initCount;
    s32 waitThreads;
    u32 attr;
    u32 option;
} rpcSemaParam;

extern s32 CreateSema(rpcSemaParam *param);
typedef struct kernelThreadParam_t {
    s32 status;
    void (*entry)(void *);
    void *stack;
    s32 stackSize;
    void *gp;
    s32 priority;
    u8 opaque[0x18];
} kernelThreadParam_t;
extern s32 D_0052C2AC __attribute__((section(".data")));
extern s32 threadMgrSema;
extern u8 D_0059AD70[0x400];
extern u8 _gp[];
extern threadMgrQueue_t D_0059B178;
extern void topThread(void *queue);
extern s32 CreateThread(kernelThreadParam_t *param);
extern s32 DeleteSema(s32 sema);
extern s32 _StartThread(s32 threadId, void *argument);
extern s32 GetThreadId(void);
extern s32 ChangeThreadPriority(s32 threadId, s32 priority);

s32 InitThread(void)
{
    kernelThreadParam_t thread;
    rpcSemaParam sema;

    if (D_0052C2AC > 0) {
        return -1;
    }
    sema.maxCount = 255;
    sema.initCount = 0;
    threadMgrSema = CreateSema(&sema);
    if (threadMgrSema < 0) {
        return -1;
    }
    thread.entry = topThread;
    thread.stack = D_0059AD70;
    thread.stackSize = 0x400;
    thread.gp = _gp;
    thread.priority = 0;
    D_0052C2AC = CreateThread(&thread);
    if (D_0052C2AC < 0) {
        DeleteSema(threadMgrSema);
        return -1;
    }
    D_0059B178.readIndex = 0;
    D_0059B178.writeIndex = 0;
    _StartThread(D_0052C2AC, &D_0059B178);
    ChangeThreadPriority(GetThreadId(), 1);
    return D_0052C2AC;
}

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

/* Native TTY context prefix, through its receive queue. */
typedef struct ttyState_t {
    s32 handle;
    s32 sendLength;
    s32 receiveLength;
    s32 sendBusy;
    u8 *sendBuffer;
    u8 *receiveBuffer;
    ttyQueue_t *queue;
} ttyState_t;

extern ttyState_t D_0059B690;

extern s32 sceDeci2ExRecv(s32 handle, void *buffer, u16 length);
extern s32 sceDeci2ExSend(s32 handle, void *buffer, u16 length);
extern void kprintf(const char *format, ...);
extern const char D_0054BA70[];
extern const char D_0054BA98[];
extern const char D_0054BAB0[];
extern const char D_0054BAC8[];

/* The asynchronous callback observes progress and publishes completion with
 * volatile word accesses. The interrupt-protected writer uses the ordinary
 * record view. */
#define TTY_CALLBACK_WORD(state, member) (*(volatile s32 *)&(state)->member)

void sceTtyHandler(s32 event, s32 byteCount, void *argument)
{
    /* Qualified EE 2.9 global_alloc trace: this shared receive/traversal count
     * has 17 refs over 44 live instructions, ahead of the context's 25/78.
     * It takes r16 and the context takes r17; splitting the count reverses them.
     * Ordinary, dumped and hooked assembly agree; both RTL dump sets agree. */
    ttyState_t *state = argument;
    u8 *packet;

    switch (event) {
    case 1:
    case 2:
        if (byteCount != 0) {
            if ((u32)TTY_CALLBACK_WORD(state, receiveLength) + (u32)byteCount > 320) {
                kprintf(D_0054BA70);
            }
            {
                s32 receiveOffset = TTY_CALLBACK_WORD(state, receiveLength);
                byteCount = sceDeci2ExRecv(state->handle,
                                          state->receiveBuffer + receiveOffset,
                                          (u16)byteCount);
            }
            if (byteCount < 0) {
                kprintf(D_0054BA98);
            }
            TTY_CALLBACK_WORD(state, receiveLength) += byteCount;
        } else {
            packet = state->receiveBuffer;
            for (byteCount = 12; byteCount < *(u16 *)packet; byteCount++) {
                *state->queue->write = state->receiveBuffer[byteCount];
                QueuePeekWriteDone(state->queue);
            }
            TTY_CALLBACK_WORD(state, receiveLength) = 0;
        }
        return;
    case 3: {
        s32 sent = sceDeci2ExSend(state->handle, state->sendBuffer,
                                (u16)TTY_CALLBACK_WORD(state, sendLength));
        if (sent < 0) {
            kprintf(D_0054BAB0, sent);
        } else {
            state->sendBuffer += sent;
            TTY_CALLBACK_WORD(state, sendLength) -= sent;
            return;
        }
        break;
    }
    case 4:
        if (TTY_CALLBACK_WORD(state, sendLength) != 0) {
            kprintf(D_0054BAC8, TTY_CALLBACK_WORD(state, sendLength));
        }
        break;
    default:
        return;
    }
    TTY_CALLBACK_WORD(state, sendBusy) = 0;
}


#undef TTY_CALLBACK_WORD
INCLUDE_ASM(const s32, "sdk/libkernl", sceTtyWrite);


s32 sceTtyRead(char *buffer, s32 length)
{
    s32 count;

    for (count = 0; count < length; count++) {
        while (D_0059B690.queue->count == 0) {
        }
        buffer[count] = *D_0059B690.queue->read;
        QueuePeekReadDone(D_0059B690.queue);
        if (buffer[count] == 10 || buffer[count] == 13) {
            return count + 1;
        }
    }
    return count;
}

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
    if (character == '\n') {
        D_0052C2B0 = 0;
        D_0059B940[index] = character;
        D_0059B940[index + 1] = 0;
        kputs(D_0059B940);
    } else {
        D_0052C2B0 = index + 1;
        D_0059B940[index] = character;
    }
}

extern s32 kputchar(s32 character);

void serialPutchar(s32 character)
{
    if (character == '\n') {
        kputchar('\r');
        kputchar('\n');
    } else {
        kputchar(character);
    }
}

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

typedef struct {
    void (*handler)(void *packet, void *argument);
    void *argument;
} sifCommandHandler_t;

/* Consumed command-context prefix; later state remains opaque. */
typedef struct {
    u8 prefix[8];
    void *remoteAddress;
    sifCommandHandler_t *systemHandlers;
    s32 systemCount;
    sifCommandHandler_t *handlers;
    s32 handlerCount;
    u32 *softwareRegisters;
} sifCommandContext_t;

extern sifCommandContext_t D_0059BA98;
extern u32 D_0059BBC0[];

typedef struct {
    u8 header[16];
    u32 index;
    u32 value;
} sifRegisterPacket_t;

typedef struct {
    u8 header[16];
    void *address;
} sifAddressPacket_t;

void gcc2_compiled__12(void *packet, void *argument)
{
    const sifRegisterPacket_t *command = packet;
    sifCommandContext_t *context = argument;
    context->softwareRegisters[command->index] = command->value;
}

void _change_addr(void *packet, void *argument)
{
    const sifAddressPacket_t *command = packet;
    sifCommandContext_t *context = argument;
    context->remoteAddress = command->address;
}

u32 sceSifGetSreg(s32 index)
{
    return D_0059BBC0[index];
}

u32 sceSifSetSreg(s32 index, u32 value)
{
    return D_0059BBC0[index] = value;
}

sifCommandContext_t *sceSifGetDataTable(void)
{
    return &D_0059BA98;
}

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifInitCmd);

extern s32 DisableDmac(s32 channel);
extern s32 RemoveDmacHandler(s32 channel, s32 handlerId);
extern s32 D_0059BA94;
extern s32 D_0052C2B8;

void sceSifExitCmd(void)
{
    DisableDmac(5);
    RemoveDmacHandler(5, D_0059BA94);
    D_0052C2B8 = 0;
}

sifCommandHandler_t *sceSifSetCmdBuffer(sifCommandHandler_t *buffer, s32 count)
{
    sifCommandHandler_t *previous = D_0059BA98.handlers;
    D_0059BA98.handlers = buffer;
    D_0059BA98.handlerCount = count;
    return previous;
}

sifCommandHandler_t *sceSifSetSysCmdBuffer(sifCommandHandler_t *buffer, s32 count)
{
    sifCommandHandler_t *previous = D_0059BA98.systemHandlers;
    D_0059BA98.systemHandlers = buffer;
    D_0059BA98.systemCount = count;
    return previous;
}

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

extern rpcQueue D_0059D440;

extern rpcPacket D_0059BC40[32];
extern rpcPacket D_0059C440[32];
extern rpcPacket D_0059CC40[32];
extern s32 D_0052C2BC;
extern void sceSifInitCmd(void);
extern u32 sceSifGetReg(u32 registerId);
extern s32 sceSifSetReg(u32 registerId, u32 value);
extern void sceSifAddCmdHandler(s32 command, void (*handler)(void *, void *), void *argument);
extern void _request_end_0(void *packet, void *queue);
extern void _request_bind(void *packet, void *queue);
extern void _request_call(void *packet, void *queue);
extern void _request_rdata(rpcPacket *packet, rpcQueue *queue);

#ifdef NON_MATCHING
/* Native-sized reconstruction; see docs/nonmatching/sdk/libkernl.md. */
void sceSifInitRpc(s32 mode)
{
    rpcPacket *packets;
    rpcPacket *packet;
    rpcQueue *queue;

    DIntr();
    if (D_0052C2BC != 0) {
        EIntr();
        return;
    }
    D_0052C2BC = 1;
    EIntr();
    sceSifInitCmd();
    DIntr();
    packets = D_0059BC40;
    queue = &D_0059D440;
    queue->indexedCount = queue->forwardCount = queue->count = 32;
    queue->seq = 1;
    queue->base = (rpcPacket *)((u32)packets | 0x20000000);
    queue->unk_C = 0;
    queue->unk_10 = 0;
    queue->forwardBase = (rpcPacket *)((u32)D_0059C440 | 0x20000000);
    queue->indexedBase = (rpcPacket *)((u32)D_0059CC40 | 0x20000000);
    queue->forwardIndex = 0;
    sceSifAddCmdHandler(0x80000008, _request_end_0, queue);
    sceSifAddCmdHandler(0x80000009, _request_bind, queue);
    sceSifAddCmdHandler(0x8000000a, _request_call, queue);
    sceSifAddCmdHandler(0x8000000c, (void (*)(void *, void *))_request_rdata, queue);
    EIntr();
    if (sceSifGetReg(0x80000002) != 0)
        return;
    packet = packets + 1;
    packet->w[3] = 1;
    sceSifSendCmd(0x80000002, packet, 16, 0, 0, 0);
    while (sceSifGetSreg(0) == 0) {
    }
    sceSifSetReg(0x80000002, 1);
}
#else
INCLUDE_ASM(const s32, "sdk/libkernl", sceSifInitRpc);
#endif

extern s32 D_0052C2BC;

void sceSifExitRpc(void)
{
    sceSifExitCmd();
    D_0052C2BC = 0;
}

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

void _request_rdata(rpcPacket *request, rpcQueue *queue)
{
    rpcPacket *packet = _sceRpcGetFPacket(queue);

    u32 packetAddress = request->w[5];
    u32 client = request->w[7];

    packet->w[5] = packetAddress;
    packet->w[7] = client;
    packet->w[8] = 0x8000000C;
    isceSifSendCmd(0x80000008, packet, 0x40, (void *)request->w[8],
                   (void *)request->w[9], request->w[10]);
}

/* Shared transaction fields used by read-data completion. */
typedef struct rpcTransactionPrefix {
    void *packet;
    u32 sequence;
    s32 sema;
} rpcTransactionPrefix;

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
} rpcClient;

extern s32 WaitSema(s32 sema);
extern s32 DeleteSema(s32 sema);

s32 sceSifGetOtherData(rpcTransactionPrefix *client, void *source, void *destination, s32 size, s32 mode)
{
    rpcSemaParam param;
    rpcPacket *packet;

    packet = _sceRpcGetPacket_0(&D_0059D440);
    if (packet == NULL) {
        return -1;
    }
    client->packet = packet;
    client->sequence = packet->w[6];
    packet->w[8] = (u32)source;
    packet->w[9] = (u32)destination;
    packet->w[10] = size;
    packet->w[5] = (u32)packet;
    packet->w[7] = (u32)client;
    if ((mode & 1) == 0) {
        param.maxCount = 1;
        param.initCount = 0;
        client->sema = CreateSema(&param);
        if (client->sema < 0) {
            _sceRpcFreePacket_0(packet);
            return -3;
        }
        if (sceSifSendCmd(0x8000000C, packet, 0x40, NULL, NULL, 0) == 0) {
            _sceRpcFreePacket_0(packet);
            DeleteSema(client->sema);
            return -2;
        }
        WaitSema(client->sema);
        DeleteSema(client->sema);
        return 0;
    }
    client->sema = -1;
    if (sceSifSendCmd(0x8000000C, packet, 0x40, NULL, NULL, 0) == 0) {
        _sceRpcFreePacket_0(packet);
        return -2;
    }
    return 0;
}

sifRpcServerData_t *_search_svdata(s32 serviceId, rpcQueue *queue)
{
    sifRpcDataQueue_t *current;
    sifRpcServerData_t *server;

    for (current = queue->queues; current != NULL; current = current->next) {
        for (server = current->link; server != NULL; server = server->link) {
            if (server->serviceId == serviceId) {
                return server;
            }
        }
    }
    return NULL;
}

INCLUDE_ASM(const s32, "sdk/libkernl", _request_bind);

s32 sceSifBindRpc(rpcClient *client, u32 serverId, s32 mode)
{
    rpcSemaParam param;
    rpcPacket *packet;

    client->serverId = 0;
    client->server = NULL;
    packet = _sceRpcGetPacket_0(&D_0059D440);
    if (packet == NULL) {
        return -1;
    }
    client->packet = packet;
    client->sequence = packet->w[6];
    packet->w[8] = serverId;
    packet->w[5] = (u32)packet;
    packet->w[7] = (u32)client;
    if ((mode & 1) == 0) {
        param.maxCount = 1;
        param.initCount = 0;
        client->sema = CreateSema(&param);
        if (client->sema < 0) {
            _sceRpcFreePacket_0(packet);
            return -3;
        }
        if (sceSifSendCmd(0x80000009, packet, 0x40, NULL, NULL, 0) == 0) {
            _sceRpcFreePacket_0(packet);
            DeleteSema(client->sema);
            return -2;
        }
        WaitSema(client->sema);
        DeleteSema(client->sema);
        return 0;
    }
    client->sema = -1;
    if (sceSifSendCmd(0x80000009, packet, 0x40, NULL, NULL, 0) == 0) {
        _sceRpcFreePacket_0(packet);
        return -2;
    }
    return 0;
}

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

/* DMA descriptor words: source, destination, byte count, attributes. */
typedef u32 sifDmaTransfer_t[4];

extern s32 sceSifSetDma(sifDmaTransfer_t *transfers, s32 count);
extern void sceSifWriteBackDCache(void *address, s32 size);

void sceSifExecRequest(sifRpcServerData_t *server)
{
    sifDmaTransfer_t transfers[2];
    rpcPacket *packet;
    void *reply;
    s32 replySize = 0;
    s32 count;
    s32 status;
    s32 delay;

    reply = server->handler(server->rpcNumber, server->buffer, server->size);
    if (reply != NULL) {
        replySize = server->receiveSize;
    }
    if (server->size > 0) {
        sceSifWriteBackDCache(server->buffer, server->size);
    }
    if (replySize > 0) {
        sceSifWriteBackDCache(reply, replySize);
    }
    DIntr();
    if (server->requestId & 4) {
        packet = _sceRpcGetFPacket2(&D_0059D440, (u32)server->requestId >> 16);
    } else {
        packet = _sceRpcGetFPacket(&D_0059D440);
    }
    EIntr();
    packet->w[8] = 0x8000000A;
    packet->w[7] = (u32)server->client;
    if (server->mode != 0) {
        while (sceSifSendCmd(0x80000008, packet, 0x40, reply, server->receiveBuffer, replySize) == 0) {
        }
        return;
    }
    packet->w[6] = 0;
    count = 0;
    packet->w[4] = 0;
    if (replySize > 0) {
        transfers[0][0] = (u32)reply;
        transfers[0][1] = (u32)server->receiveBuffer;
        transfers[0][2] = replySize;
        transfers[0][3] = 0;
        count++;
    }
    transfers[count][0] = (u32)packet;
    transfers[count][1] = (u32)server->packetAddress;
    transfers[count][2] = 0x40;
    transfers[count][3] = 0;
    count++;
    do {
        status = sceSifSetDma(transfers, count);
        if (status == 0) {
            for (delay = 0x100000; delay != -1; delay--) {
            }
        }
    } while (status == 0);
}

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

extern s32 D_0052C348 __attribute__((section(".data")));
extern s32 D_0052C34C __attribute__((section(".data")));

void gcc2_compiled__13(void)
{
    rpcSemaParam param;
    if (D_0052C348 == -1) {
        param.initCount = 1;
        param.maxCount = 1;
        param.option = 0;
        D_0052C348 = CreateSema(&param);
        D_0052C34C = CreateSema(&param);
    }
}


typedef struct {
    u32 word0;
    s32 active;
    u8 opaque[8];
} fsRequestEntry_t;
extern fsRequestEntry_t D_0059E580[32];
extern s32 D_0052C348;
extern void gcc2_compiled__13(void);
extern s32 SignalSema(s32 sema);

fsRequestEntry_t *new_iob(void)
{
    fsRequestEntry_t *entry;

    gcc2_compiled__13();
    WaitSema(D_0052C348);
    for (entry = D_0059E580; entry < &D_0059E580[32]; entry++) {
        if (entry->active == 0) {
            entry->active = 0x10000000;
            SignalSema(D_0052C348);
            return entry;
        }
    }
    SignalSema(D_0052C348);
    return NULL;
}

fsRequestEntry_t *get_iob(u32 index)
{
    fsRequestEntry_t *entry;

    gcc2_compiled__13();
    WaitSema(D_0052C348);
    if (index >= 32) {
        SignalSema(D_0052C348);
        return NULL;
    }
    entry = &D_0059E580[index];
    SignalSema(D_0052C348);
    return entry;
}

INCLUDE_ASM(const s32, "sdk/libkernl", _sceFs_Rcv_Intr);

extern s32 D_0052C344 __attribute__((section(".data")));

void _sceFsSemInit(void)
{
    rpcSemaParam param;
    if (D_0052C344 == -1) {
        /* Only these descriptor fields are initialized by the native helper. */
        param.initCount = 1;
        param.maxCount = 1;
        param.option = 0;
        D_0052C344 = CreateSema(&param);
    }
}

s32 _sceFsWaitS(s32 command)
{
    _sceFsSemInit();
    WaitSema(D_0052C344);
    return 0;
}

extern s32 SignalSema(s32 sema);

/* Internal notification: callers discard the SignalSema status. */
void _sceFsSigSema(void)
{
    SignalSema(D_0052C344);
}

typedef void (*fsPowerOffCallback_t)(void *argument);
typedef struct fsPowerOffState_t {
    fsPowerOffCallback_t callback;
    void *argument;
} fsPowerOffState_t;
extern fsPowerOffState_t D_0059E800;
extern s32 D_0052C340;
extern s32 sceFsInit(void);

fsPowerOffCallback_t scePowerOffHandler(fsPowerOffCallback_t callback, void *argument)
{
    fsPowerOffState_t *state = &D_0059E800;
    fsPowerOffCallback_t previous;
    s32 interrupts;

    _sceFsWaitS(0x1B);
    if (D_0052C340 == 0) {
        sceFsInit();
    }
    interrupts = DIntr();
    previous = D_0059E800.callback;
    state->argument = argument;
    D_0059E800.callback = callback;
    if (interrupts != 0) {
        EIntr();
    }
    _sceFsSigSema();
    return previous;
}

void _sceFs_Poff_Intr(void *packet, fsPowerOffState_t *state)
{
    if (state->callback != NULL) {
        state->callback(state->argument);
    }
    EE_SYNC_EI();
}

typedef struct { u8 bytes[4]; } fsVersionTag_t;
typedef struct { void *receiveBuffer; } fsRegistrationPacket_t;

extern u8 D_0059E7C0[];
extern rpcClient D_0059E780;
extern u8 D_0059E140[];
extern u8 D_0059E100[];
extern fsRegistrationPacket_t D_0059D480;
extern void sceSifInitRpc(s32 mode);
extern void sceSifAddCmdHandler(s32 command, void (*handler)(void *, void *), void *argument);
extern void _sceFs_Rcv_Intr(void *packet, void *argument);
extern s32 sceSifCallRpc(void *client, s32 function, s32 mode, void *send,
                         s32 sendBytes, void *receive, s32 receiveBytes,
                         void *endFunction, void *endParameter);
extern u8 D_0059E7A8[0x18];
extern s32 D_0052C340 __attribute__((section(".data")));
extern s32 SignalSema(s32 sema);

s32 sceFsInit(void)
{
    fsPowerOffState_t *powerOff = &D_0059E800;
    s32 previousInterrupts;
    s32 delay;
    fsRequestEntry_t *entry;

    sceSifInitRpc(0);
    powerOff->callback = NULL;
    powerOff->argument = NULL;
    previousInterrupts = DIntr();
    sceSifAddCmdHandler(0x80000011, _sceFs_Rcv_Intr, D_0059E7C0);
    sceSifAddCmdHandler(0x80000013, (void (*)(void *, void *))_sceFs_Poff_Intr, powerOff);
    if (previousInterrupts)
        EIntr();
    for (;;) {
        if (sceSifBindRpc(&D_0059E780, 0x80000001, 0) < 0)
            return -1;
        if (D_0059E780.server != NULL)
            break;
        for (delay = 0x100000; delay != -1; --delay) {
        }
    }
    gcc2_compiled__13();
    WaitSema(D_0052C348);
    for (entry = D_0059E580; entry < &D_0059E580[32]; entry++)
        entry->active = 0;
    SignalSema(D_0052C348);
    D_0059D480.receiveBuffer = D_0059E140;
    if (sceSifCallRpc(&D_0059E780, 255, 0, &D_0059D480, 4,
                      D_0059E100, 4, NULL, NULL) < 0)
        return -0x10001;
    *(fsVersionTag_t *)D_0059E7A8 =
        *(const fsVersionTag_t *)((u32)D_0059E100 | 0x20000000);
    D_0052C340 = 1;
    return 0;
}


extern u8 D_0059E7A8[0x18];
extern u8 D_0052C2A0[4] __attribute__((section(".data")));
extern const void *D_0052C350 __attribute__((section(".data")));
extern s32 memcmp(const void *left, const void *right, u32 count);

s32 _fs_version(void)
{
    const void *expected = D_0052C2A0;
    const void *version = D_0059E7A8;
    s32 mismatch = 0;
    if (memcmp(version, expected, 4) != 0) {
        if (memcmp(version, D_0052C350, 4) != 0) {
            mismatch = memcmp(expected, D_0052C350, 4) != 0;
        }
    }
    return mismatch;
}

extern s32 D_0052C340 __attribute__((section(".data")));
extern void *memset(void *destination, s32 value, u32 count);

s32 sceFsReset(void)
{
    D_0052C340 = 0;
    memset(D_0059E7A8, 0, 4);
    return 0;
}

INCLUDE_ASM(const s32, "sdk/libkernl", sceOpen);

typedef struct fsCloseRequest_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    u32 descriptor;
    s32 slot;
} fsCloseRequest_t;
typedef struct fsPathRequest_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    char name[0x400];
} fsPathRequest_t;
typedef struct fsMkdirRequest_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    s32 mode;
    char name[0x400];
} fsMkdirRequest_t;
typedef struct fsGetstatRequest_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    void *stat;
    u8 name[0x400];
} fsGetstatRequest_t;
typedef struct fsSyncRequest_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    u32 descriptor; /* Shared word, left untouched by sync. */
    s32 mode;
    char name[0x400];
} fsSyncRequest_t;
typedef struct fsRenameRequest_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    char oldName[0x400];
    char newName[0x400];
} fsRenameRequest_t;
typedef struct fsStat_t {
    u32 mode;
    u32 attributes;
    u32 size;
    u8 creationTime[8];
    u8 accessTime[8];
    u8 modificationTime[8];
    u32 highSize;
    u32 privateData[6];
} fsStat_t;
typedef struct fsChstatRequest_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    u32 mask;
    fsStat_t stat;
    u8 name[0x400];
} fsChstatRequest_t;
typedef struct fsReadlinkRequest_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    u32 count;
    char *buffer;
    char name[0x400];
} fsReadlinkRequest_t;
typedef struct fsFormatRequest_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    char device[0x400];
    char blockDevice[0x400];
    u8 args[0x400];
    s32 argSize;
} fsFormatRequest_t;
typedef struct fsMountRequest_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    char device[0x400];
    char blockDevice[0x400];
    u8 args[0x400];
    s32 mode;
    s32 argSize;
} fsMountRequest_t;
typedef struct fsDreadRequest_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    u32 descriptor;
    void *entry;
    u8 opaque14[0xC];
} fsDreadRequest_t;
typedef struct fsDevctlRequest_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    char name[0x400];
    u8 args[0x400];
    s32 command;
    u32 argSize;
    void *output;
    u32 outputSize;
} fsDevctlRequest_t;
typedef struct fsIoctl2Request_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    u32 descriptor;
    s32 command;
    u8 args[0x400];
    void *output;
    u32 outputSize;
    u32 argSize;
} fsIoctl2Request_t;
typedef union fsRequest_t {
    fsCloseRequest_t close;
    fsPathRequest_t path;
    fsMkdirRequest_t mkdir;
    fsGetstatRequest_t getstat;
    fsSyncRequest_t sync;
    fsRenameRequest_t rename;
    fsChstatRequest_t chstat;
    fsReadlinkRequest_t readlink;
    fsFormatRequest_t format;
    fsDevctlRequest_t devctl;
    fsIoctl2Request_t ioctl2;
    fsMountRequest_t mount;
    fsDreadRequest_t dread;
} fsRequest_t;
extern fsRequest_t D_0059D4C0;

/* NON_MATCHING: 376/380 bytes; the final nonnegative-result clamp
 * simplifies to a shorter immediate comparison. */
#ifdef NON_MATCHING
s32 sceClose(s32 descriptor)
{
    rpcSemaParam param;
    s32 result;
    fsCloseRequest_t *request = &D_0059D4C0.close;
    fsRequestEntry_t *entry;
    s32 sema;
    s32 accepted;
    s32 status;

    entry = get_iob(descriptor);
    _sceFsWaitS(1);
    if (D_0052C340 == 0) {
        _sceFsSigSema();
        return -1;
    }
    if (entry == NULL || entry->active == 0) {
        _sceFsSigSema();
        return -9;
    }
    request->descriptor = entry->word0;
    request->slot = entry - D_0059E580;
    param.maxCount = 1;
    param.initCount = 0;
    param.option = 0;
    sema = CreateSema(&param);
    D_0059D4C0.close.sema = sema;
    request->result = &result;
    request->resultSize = 4;
    if (sceSifCallRpc(&D_0059E780, 1, 0, request, 0x14,
                      D_0059E100, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    entry->active = 0;
    accepted = *(s32 *)((u32)D_0059E100 | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    status = result;
    if (status >= 0) {
        status = 0;
    }
    return status;
}
#else
INCLUDE_ASM(const s32, "sdk/libkernl", sceClose);
#endif

INCLUDE_ASM(const s32, "sdk/libkernl", sceLseek);

INCLUDE_ASM(const s32, "sdk/libkernl", sceRead);

INCLUDE_ASM(const s32, "sdk/libkernl", sceWrite);

typedef union fsIoctlStatus_t {
    s32 word;
    s64 doubleword;
} fsIoctlStatus_t;
extern fsIoctlStatus_t D_0059E150;
extern void *D_0059D484;
extern s32 D_0052C2C0[32];
extern void *memcpy(void *destination, const void *source, u32 size);

/* NON_MATCHING: native 844-byte size; early switch allocation and
 * pending-table setup differ in 46 instruction words. */
#ifdef NON_MATCHING
s32 sceIoctl(s32 descriptor, s32 command, void *argument)
{
    rpcSemaParam param;
    s32 result;
    fsIoctl2Request_t *request = &D_0059D4C0.ioctl2;
    fsRequestEntry_t *entry;
    s32 index;
    s32 sema;
    s32 accepted;
    fsRequest_t *packet;

    entry = get_iob(descriptor);
    _sceFsWaitS(5);
    D_0059D484 = argument;
    if (D_0052C340 == 0) {
        sceFsInit();
    }
    if (entry == NULL || entry->active == 0) {
        _sceFsSigSema();
        return -9;
    }
    request->output = NULL;
    request->outputSize = 0;
    switch (command) {
    case 1:
        WaitSema(D_0052C34C);
        for (index = 0; index < 32 && D_0052C2C0[index] == -1; index++) {
        }
        if (index == 32) {
            *(s32 *)D_0059D484 = 0;
        } else {
            *(s32 *)D_0059D484 = 1;
        }
        SignalSema(D_0052C34C);
        _sceFsSigSema();
        return 0;
    case 2:
        *(s32 *)argument = ((fsIoctlStatus_t *)((u32)&D_0059E150 | 0x20000000))->word;
        _sceFsSigSema();
        return 0;
    case 3:
        *(s64 *)argument = ((fsIoctlStatus_t *)((u32)&D_0059E150 | 0x20000000))->doubleword;
        _sceFsSigSema();
        return 0;
    }
    request->descriptor = entry->word0;
    request->command = command;
    if (argument == NULL) {
        request->argSize = 0;
    } else {
        request->argSize = 0x400;
        memcpy(request->args, argument, 0x400);
    }
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    sema = CreateSema(&param);
    packet = &D_0059D4C0;
    request->sema = sema;
    request->result = &result;
    request->resultSize = 4;
    sceSifWriteBackDCache(packet, 0x420);
    if (sceSifCallRpc(&D_0059E780, 5, 0, packet,
                      0x420, D_0059E100, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)((u32)D_0059E100 | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}
#else
INCLUDE_ASM(const s32, "sdk/libkernl", sceIoctl);
#endif

s32 sceIoctl2(s32 descriptor, s32 command, const void *args, u32 argSize,
              void *output, u32 outputSize)
{
    rpcSemaParam param;
    s32 result;
    fsIoctl2Request_t *request = &D_0059D4C0.ioctl2;
    fsRequestEntry_t *entry;
    s32 sema;
    s32 accepted;
    fsRequest_t *packet;

    entry = get_iob(descriptor);
    _sceFsWaitS(26);
    if (D_0052C340 == 0) {
        sceFsInit();
    }
    if (entry == NULL || entry->active == 0) {
        _sceFsSigSema();
        return -9;
    }
    if (argSize > 0x400 || outputSize > 0x400) {
        _sceFsSigSema();
        return -22;
    }
    if (args == NULL) {
        request->argSize = 0;
    } else {
        memcpy(request->args, args, argSize);
    }
    request->descriptor = entry->word0;
    request->command = command;
    request->argSize = argSize;
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    packet = &D_0059D4C0;
    sema = CreateSema(&param);
    request->outputSize = outputSize;
    request->result = &result;
    request->resultSize = 4;
    request->output = output;
    request->sema = sema;
    sceSifWriteBackDCache(packet, 0x420);
    if (sceSifCallRpc(&D_0059E780, 26, 0, packet,
                      0x420, D_0059E100, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)((u32)D_0059E100 | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}

s32 _sceCallCode(const char *path, s32 command)
{
    rpcSemaParam param;
    s32 result;
    fsPathRequest_t *request = &D_0059D4C0.path;
    s32 length;
    s32 sema;
    s32 accepted;

    _sceFsWaitS(command);
    if (D_0052C340 == 0) {
        sceFsInit();
    }
    for (length = 0; length < 0x400 &&
         (request->name[length] = path[length]) != 0; length++) {
    }
    if (length == 0x400) {
        request->name[0x3FF] = 0;
        length = 0x3FF;
    }
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    sema = CreateSema(&param);
    request->result = &result;
    request->sema = sema;
    request->resultSize = 4;
    if (sceSifCallRpc(&D_0059E780, command, 0, &D_0059D4C0,
                      length + 0xD, D_0059E100, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)((u32)D_0059E100 | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}

s32 sceRemove(const char *path)
{
    return _sceCallCode(path, 6);
}

s32 sceMkdir(const char *path, s32 mode)
{
    rpcSemaParam param;
    s32 result;
    fsMkdirRequest_t *request = &D_0059D4C0.mkdir;
    s32 length;
    s32 sema;
    s32 accepted;

    _sceFsWaitS(7);
    if (D_0052C340 == 0) {
        sceFsInit();
    }
    for (length = 0; length < 0x400 &&
         (request->name[length] = path[length]) != 0; length++) {
    }
    if (length == 0x400) {
        request->name[0x3FF] = 0;
        length = 0x3FF;
    }
    request->mode = mode;
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    sema = CreateSema(&param);
    request->result = &result;
    request->sema = sema;
    request->resultSize = 4;
    if (sceSifCallRpc(&D_0059E780, 7, 0, &D_0059D4C0,
                      length + 0x11, D_0059E100, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)((u32)D_0059E100 | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}


s32 sceRmdir(const char *path)
{
    return _sceCallCode(path, 8);
}

s32 sceFormat(const char *device, const char *blockDevice, const void *args, s32 argSize)
{
    rpcSemaParam param;
    s32 result;
    fsFormatRequest_t *request = &D_0059D4C0.format;
    s32 deviceLength;
    s32 blockLength;
    s32 argIndex;
    s32 sema;
    s32 accepted;
    fsRequest_t *packet;

    _sceFsWaitS(14);
    if (D_0052C340 == 0) {
        sceFsInit();
    }
    for (deviceLength = 0; deviceLength < 0x400 &&
         (request->device[deviceLength] = device[deviceLength]) != 0; deviceLength++) {
    }
    if (deviceLength == 0x400) {
        request->device[0x3FF] = 0;
    }
    if (blockDevice == NULL) {
        request->blockDevice[0] = 0;
    } else {
        for (blockLength = 0; blockLength < 0x400 &&
             (request->blockDevice[blockLength] = blockDevice[blockLength]) != 0; blockLength++) {
        }
        if (blockLength == 0x400) {
            request->blockDevice[0x3FF] = 0;
        }
    }
    if (argSize > 0x400) {
        _sceFsSigSema();
        return -7;
    }
    for (argIndex = 0; argIndex < argSize; argIndex++) {
        request->args[argIndex] = ((const u8 *)args)[argIndex];
    }
    request->argSize = argSize;
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    packet = &D_0059D4C0;
    sema = CreateSema(&param);
    request->sema = sema;
    request->result = &result;
    request->resultSize = 4;
    sceSifWriteBackDCache(packet, 0xC10);
    if (sceSifCallRpc(&D_0059E780, 14, 0, packet,
                      0xC10, D_0059E100, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)((u32)D_0059E100 | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}

s32 sceAddDrv(void *driver)
{
    rpcSemaParam param;
    s32 result;
    fsCloseRequest_t *request = &D_0059D4C0.close;
    s32 sema;
    s32 accepted;

    _sceFsWaitS(15);
    if (D_0052C340 == 0) {
        sceFsInit();
    }
    request->descriptor = (u32)driver;
    param.maxCount = 1;
    param.initCount = 0;
    param.option = 0;
    sema = CreateSema(&param);
    D_0059D4C0.close.sema = sema;
    request->result = &result;
    request->resultSize = 4;
    if (sceSifCallRpc(&D_0059E780, 15, 0, request, 16,
                      D_0059E100, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -1;
    }
    accepted = *(s32 *)((u32)D_0059E100 | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -1;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}

s32 sceDelDrv(const char *path)
{
    return _sceCallCode(path, 16);
}

s32 sceDopen(const char *path)
{
    fsRequestEntry_t *entry;
    s32 result;
    s32 sema;

    _sceFsWaitS(9);
    if (D_0052C340 == 0) {
        sceFsInit();
    }
    _sceFsSigSema();
    entry = new_iob();
    if (entry == NULL) {
        return -19;
    }
    result = _sceCallCode(path, 9);
    if (result < 0) {
        WaitSema(D_0052C348);
        entry->active = 0;
        sema = D_0052C348;
    } else {
        WaitSema(D_0052C348);
        entry->word0 = result;
        sema = D_0052C348;
        result = entry - D_0059E580;
    }
    SignalSema(sema);
    return result;
}

INCLUDE_ASM(const s32, "sdk/libkernl", sceDclose);

s32 sceDread(s32 descriptor, void *directoryEntry)
{
    rpcSemaParam param;
    s32 result;
    fsDreadRequest_t *request = &D_0059D4C0.dread;
    fsRequestEntry_t *entry;
    s32 sema;
    s32 accepted;

    entry = get_iob(descriptor);
    _sceFsWaitS(11);
    if (D_0052C340 == 0) {
        _sceFsSigSema();
        return -1;
    }
    if (entry == NULL || entry->active == 0) {
        _sceFsSigSema();
        return -9;
    }
    request->entry = directoryEntry;
    request->descriptor = entry->word0;
    param.maxCount = 1;
    param.initCount = 0;
    param.option = 0;
    sema = CreateSema(&param);
    D_0059D4C0.dread.sema = sema;
    request->result = &result;
    request->resultSize = 4;
    if (sceSifCallRpc(&D_0059E780, 11, 0, request, 0x20,
                      D_0059E100, 4, NULL, NULL) < 0) {
        /* The failure path waits before releasing the filesystem lock. */
        WaitSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)((u32)D_0059E100 | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}

s32 sceGetstat(const char *path, void *stat)
{
    rpcSemaParam param;
    s32 result;
    fsGetstatRequest_t *request = &D_0059D4C0.getstat;
    s32 length;
    s32 sema;
    s32 accepted;

    _sceFsWaitS(12);
    if (D_0052C340 == 0) {
        sceFsInit();
    }
    for (length = 0; length < 0x400 &&
         (request->name[length] = path[length]) != 0; length++) {
    }
    if (length == 0x400) {
        request->name[0x3FF] = 0;
        length = 0x3FF;
    }
    request->stat = stat;
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    sema = CreateSema(&param);
    request->result = &result;
    request->sema = sema;
    request->resultSize = 4;
    if (sceSifCallRpc(&D_0059E780, 12, 0, &D_0059D4C0,
                      length + 0x11, D_0059E100, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)((u32)D_0059E100 | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}


s32 sceChstat(const char *path, const fsStat_t *stat, u32 mask)
{
    rpcSemaParam param;
    s32 result;
    fsChstatRequest_t *request = &D_0059D4C0.chstat;
    s32 length;
    s32 sema;
    s32 accepted;
    fsRequest_t *packet;

    _sceFsWaitS(13);
    if (D_0052C340 == 0) {
        sceFsInit();
    }
    for (length = 0; length < 0x400 &&
         (request->name[length] = path[length]) != 0; length++) {
    }
    if (length == 0x400) {
        request->name[0x3FF] = 0;
        length = 0x3FF;
    }
    request->stat = *stat;
    request->mask = mask;
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    packet = &D_0059D4C0;
    sema = CreateSema(&param);
    request->sema = sema;
    request->result = &result;
    request->resultSize = 4;
    sceSifWriteBackDCache(packet, 0x450);
    if (sceSifCallRpc(&D_0059E780, 13, 0, packet,
                      length + 0x51, D_0059E100, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)((u32)D_0059E100 | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}


s32 sceRename(const char *oldPath, const char *newPath)
{
    rpcSemaParam param;
    s32 result;
    fsRenameRequest_t *request = &D_0059D4C0.rename;
    s32 oldLength;
    s32 newLength;
    s32 sema;
    s32 accepted;
    fsRequest_t *packet;

    _sceFsWaitS(17);
    if (D_0052C340 == 0) {
        sceFsInit();
    }
    for (oldLength = 0; oldLength < 0x400 &&
         (request->oldName[oldLength] = oldPath[oldLength]) != 0; oldLength++) {
    }
    if (oldLength == 0x400) {
        request->oldName[0x3FF] = 0;
    }
    for (newLength = 0; newLength < 0x400 &&
         (request->newName[newLength] = newPath[newLength]) != 0; newLength++) {
    }
    if (newLength == 0x400) {
        request->newName[0x3FF] = 0;
    }
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    packet = &D_0059D4C0;
    sema = CreateSema(&param);
    request->sema = sema;
    request->result = &result;
    request->resultSize = 4;
    sceSifWriteBackDCache(packet, 0x80C);
    if (sceSifCallRpc(&D_0059E780, 17, 0, packet,
                      0x80C, D_0059E100, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)((u32)D_0059E100 | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}

s32 sceChdir(const char *path)
{
    return _sceCallCode(path, 18);
}

s32 sceSync(const char *path, s32 mode)
{
    rpcSemaParam param;
    s32 result;
    fsSyncRequest_t *request = &D_0059D4C0.sync;
    s32 length;
    s32 sema;
    s32 accepted;

    _sceFsWaitS(19);
    if (D_0052C340 == 0) {
        sceFsInit();
    }
    for (length = 0; length < 0x400 &&
         (request->name[length] = path[length]) != 0; length++) {
    }
    if (length == 0x400) {
        request->name[0x3FF] = 0;
    }
    request->mode = mode;
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    sema = CreateSema(&param);
    request->result = &result;
    request->sema = sema;
    request->resultSize = 4;
    if (sceSifCallRpc(&D_0059E780, 19, 0, &D_0059D4C0,
                      0x414, D_0059E100, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)((u32)D_0059E100 | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}

s32 sceMount(const char *device, const char *blockDevice, s32 mode, const void *args, s32 argSize)
{
    rpcSemaParam param;
    s32 result;
    fsMountRequest_t *request = &D_0059D4C0.mount;
    s32 deviceLength;
    s32 blockLength;
    s32 argIndex;
    s32 sema;
    s32 accepted;
    fsRequest_t *packet;

    _sceFsWaitS(20);
    if (D_0052C340 == 0) {
        sceFsInit();
    }
    for (deviceLength = 0; deviceLength < 0x400 &&
         (request->device[deviceLength] = device[deviceLength]) != 0; deviceLength++) {
    }
    if (deviceLength == 0x400) {
        request->device[0x3FF] = 0;
    }
    for (blockLength = 0; blockLength < 0x400 &&
         (request->blockDevice[blockLength] = blockDevice[blockLength]) != 0; blockLength++) {
    }
    if (blockLength == 0x400) {
        request->blockDevice[0x3FF] = 0;
    }
    if (argSize > 0x400) {
        _sceFsSigSema();
        return -7;
    }
    for (argIndex = 0; argIndex < argSize; argIndex++) {
        request->args[argIndex] = ((const u8 *)args)[argIndex];
    }
    request->argSize = argSize;
    request->mode = mode;
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    packet = &D_0059D4C0;
    sema = CreateSema(&param);
    request->sema = sema;
    request->result = &result;
    request->resultSize = 4;
    sceSifWriteBackDCache(packet, 0xC14);
    if (sceSifCallRpc(&D_0059E780, 20, 0, packet,
                      0xC14, D_0059E100, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)((u32)D_0059E100 | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}


s32 sceUmount(const char *path)
{
    return _sceCallCode(path, 21);
}

INCLUDE_ASM(const s32, "sdk/libkernl", sceLseek64);

s32 sceDevctl(const char *device, s32 command, const void *args, u32 argSize,
              void *output, u32 outputSize)
{
    rpcSemaParam param;
    s32 result;
    fsDevctlRequest_t *request;
    s32 length;
    u32 argIndex;
    s32 sema;
    s32 accepted;
    fsRequest_t *packet;

    _sceFsWaitS(23);
    request = &D_0059D4C0.devctl;
    if (D_0052C340 == 0) {
        sceFsInit();
    }
    for (length = 0; length < 0x400 &&
         (request->name[length] = device[length]) != 0; length++) {
    }
    if (length == 0x400) {
        request->name[0x3FF] = 0;
    }
    if (argSize > 0x400 || outputSize > 0x400) {
        _sceFsSigSema();
        return -22;
    }
    for (argIndex = 0; argIndex < argSize; argIndex++) {
        request->args[argIndex] = ((const u8 *)args)[argIndex];
    }
    request->argSize = argSize;
    request->command = command;
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    packet = &D_0059D4C0;
    sema = CreateSema(&param);
    request->outputSize = outputSize;
    request->result = &result;
    request->resultSize = 4;
    request->output = output;
    request->sema = sema;
    sceSifWriteBackDCache(packet, 0x81C);
    if (sceSifCallRpc(&D_0059E780, 23, 0, packet,
                      0x81C, D_0059E100, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)((u32)D_0059E100 | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}

s32 sceSymlink(const char *oldPath, const char *newPath)
{
    rpcSemaParam param;
    s32 result;
    fsRenameRequest_t *request = &D_0059D4C0.rename;
    s32 oldLength;
    s32 newLength;
    s32 sema;
    s32 accepted;

    _sceFsWaitS(17);
    if (D_0052C340 == 0) {
        sceFsInit();
    }
    for (oldLength = 0; oldLength < 0x400 &&
         (request->oldName[oldLength] = oldPath[oldLength]) != 0; oldLength++) {
    }
    if (oldLength == 0x400) {
        request->oldName[0x3FF] = 0;
    }
    for (newLength = 0; newLength < 0x400 &&
         (request->newName[newLength] = newPath[newLength]) != 0; newLength++) {
    }
    if (newLength == 0x400) {
        request->newName[0x3FF] = 0;
    }
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    sema = CreateSema(&param);
    request->result = &result;
    request->sema = sema;
    request->resultSize = 4;
    if (sceSifCallRpc(&D_0059E780, 24, 0, &D_0059D4C0,
                      0x80C, D_0059E100, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)((u32)D_0059E100 | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}

s32 sceReadlink(const char *path, char *buffer, u32 count)
{
    rpcSemaParam param;
    s32 result;
    fsReadlinkRequest_t *request = &D_0059D4C0.readlink;
    s32 length;
    s32 sema;
    s32 accepted;

    _sceFsWaitS(17);
    if (D_0052C340 == 0) {
        sceFsInit();
    }
    for (length = 0; length < 0x400 &&
         (request->name[length] = path[length]) != 0; length++) {
    }
    if (length == 0x400) {
        request->name[0x3FF] = 0;
    }
    if (count >= 0x400) {
        count = 0x3FF;
    }
    request->buffer = buffer;
    request->count = count;
    sceSifWriteBackDCache(buffer, count);
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    sema = CreateSema(&param);
    request->result = &result;
    request->sema = sema;
    request->resultSize = 4;
    /* The fixed transfer also includes the shared buffer's untouched tail. */
    if (sceSifCallRpc(&D_0059E780, 25, 0, &D_0059D4C0,
                      0x80C, D_0059E100, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)((u32)D_0059E100 | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}

extern s32 D_0052C354;
/* The BSS map bounds this opaque RPC client to 0x40 bytes. */
extern u8 D_0059E840[0x40];
extern s32 D_0059E880;
/* Shared heap request storage is 0x40 bytes in the BSS map. */
extern u32 D_0059E8C0[16];
extern s32 sceSifCallRpc(void *client, s32 function, s32 mode, void *send,
                         s32 sendBytes, void *receive, s32 receiveBytes,
                         void *endFunction, void *endParameter);

/* NON_MATCHING: 31 of 34 words match at the native 136-byte size;
 * the final initialization store and return use different scheduling. */
#ifdef NON_MATCHING
s32 sceSifInitIopHeap(void)
{
    s32 delay;

    for (;;) {
        if (sceSifBindRpc((rpcClient *)D_0059E840, 0x80000003, 0) < 0) {
            return -1;
        }
        if (((rpcClient *)D_0059E840)->server != NULL) {
            break;
        }
        for (delay = 0x100000; delay != -1; delay--) {
        }
    }
    D_0052C354 = 0;
    return 0;
}
#else
INCLUDE_ASM(const s32, "sdk/libkernl", sceSifInitIopHeap);
#endif

s32 sceSifAllocIopHeap(s32 size)
{
    if (D_0052C354 < 0) {
        return 0;
    }
    D_0059E8C0[0] = size;
    if (sceSifCallRpc(D_0059E840, 1, 0, D_0059E8C0, 4,
                      &D_0059E880, 4, NULL, NULL) >= 0) {
        return D_0059E880;
    }
    return 0;
}


void *sceSifAllocSysMemory(s32 mode, s32 size, void *address)
{
    if (D_0052C354 < 0) {
        return NULL;
    }
    D_0059E8C0[0] = size;
    D_0059E8C0[1] = mode;
    D_0059E8C0[2] = (u32)address;
    if (sceSifCallRpc(D_0059E840, 4, 0, D_0059E8C0, 12,
                      &D_0059E880, 4, NULL, NULL) >= 0) {
        return (void *)D_0059E880;
    }
    return NULL;
}

s32 sceSifFreeSysMemory(void *address)
{
    if (D_0052C354 < 0) {
        return 0;
    }
    D_0059E8C0[0] = (u32)address;
    if (sceSifCallRpc(D_0059E840, 2, 0, D_0059E8C0, 4,
                      &D_0059E880, 4, NULL, NULL) >= 0) {
        return D_0059E880;
    }
    return -1;
}

s32 sceSifFreeIopHeap(void *address)
{
    return sceSifFreeSysMemory(address);
}

typedef struct heapLoadRequest_t {
    u32 address;
    char name[0xFC];
} heapLoadRequest_t;
extern heapLoadRequest_t D_0059E900;

s32 sceSifLoadIopHeap(const char *path, u32 address)
{
    s32 length;

    if (D_0052C354 < 0) {
        return 0;
    }
    for (length = 0; length < 0xFC &&
         (D_0059E900.name[length] = path[length]) != 0; length++) {
    }
    if (length == 0xFC) {
        D_0059E900.name[length - 1] = 0;
        length--;
    }
    D_0059E900.address = address;
    D_0059E900.name[0xFB] = 0;
    if (sceSifCallRpc(D_0059E840, 3, 0, &D_0059E900, length + 5,
                      &D_0059E880, 4, NULL, NULL) >= 0) {
        return D_0059E880;
    }
    return -1;
}

u32 sceSifQueryMemSize(void)
{
    if (D_0052C354 < 0) {
        return 0;
    }
    if (sceSifCallRpc(D_0059E840, 5, 0, NULL, 0,
                      &D_0059E880, 4, NULL, NULL) < 0) {
        return 0xFFFFFFFF;
    }
    return D_0059E880;
}

u32 sceSifQueryMaxFreeMemSize(void)
{
    if (D_0052C354 < 0) {
        return 0;
    }
    if (sceSifCallRpc(D_0059E840, 6, 0, NULL, 0,
                      &D_0059E880, 4, NULL, NULL) < 0) {
        return 0xFFFFFFFF;
    }
    return D_0059E880;
}

u32 sceSifQueryTotalFreeMemSize(void)
{
    if (D_0052C354 < 0) {
        return 0;
    }
    if (sceSifCallRpc(D_0059E840, 7, 0, NULL, 0,
                      &D_0059E880, 4, NULL, NULL) < 0) {
        return 0xFFFFFFFF;
    }
    return D_0059E880;
}

/* The IOP block key and query results travel as 32-bit words. */
u32 sceSifQueryBlockTopAddress(u32 block)
{
    if (D_0052C354 < 0) {
        return 0;
    }
    D_0059E8C0[0] = block;
    if (sceSifCallRpc(D_0059E840, 8, 0, D_0059E8C0, 4,
                      &D_0059E880, 4, NULL, NULL) < 0) {
        return 0xFFFFFFFF;
    }
    return D_0059E880;
}

u32 sceSifQueryBlockSize(u32 block)
{
    if (D_0052C354 < 0) {
        return 0;
    }
    D_0059E8C0[0] = block;
    if (sceSifCallRpc(D_0059E840, 9, 0, D_0059E8C0, 4,
                      &D_0059E880, 4, NULL, NULL) < 0) {
        return 0xFFFFFFFF;
    }
    return D_0059E880;
}

/* The load-file RPC sends this complete 0x200-byte shared request region. */
typedef struct lfRequest_t {
    s32 value;
    s32 moduleResult; /* Argument size on buffer-load/stop requests; result on replies. */
    char name[0xFC];
    u8 arguments[0xFC];
} lfRequest_t;

extern lfRequest_t D_0059EA00;
extern rpcClient D_0059EC00;
extern s32 _lf_bind(void);
extern s32 _lf_version(void);
extern char *strncpy(char *destination, const char *source, u32 count);

extern s32 D_0052C358;
extern u8 D_0059EC28[0x18];
extern u8 D_0052C2A0[4];
extern void *D_0052C35C;
extern void *memcpy(void *destination, const void *source, u32 size);
extern s32 memcmp(const void *a, const void *b, u32 size);

s32 _lf_bind(void)
{
    s32 delay;

    if (D_0052C358 < 0) {
        for (;;) {
            if (sceSifBindRpc(&D_0059EC00, 0x80000006, 0) < 0) {
                return -1;
            }
            if (D_0059EC00.server != NULL) {
                D_0052C358 = 0;
                if (sceSifCallRpc(&D_0059EC00, 0xFF, 0, NULL, 0,
                                  &D_0059EA00, 4, NULL, NULL) < 0) {
                    return -65537;
                }
                memcpy(D_0059EC28, &D_0059EA00, 4);
                return 0;
            }
            for (delay = 0x100000; delay != -1; delay--) {
            }
        }
    }
    return 0;
}

s32 _lf_version(void)
{
    const void *builtin = D_0052C2A0;
    const void *detected = D_0059EC28;
    s32 mismatch = 0;

    if (memcmp(detected, builtin, 4) != 0) {
        if (memcmp(detected, D_0052C35C, 4) != 0) {
            mismatch = memcmp(builtin, D_0052C35C, 4) != 0;
        }
    }
    return mismatch;
}

extern void *memset(void *destination, s32 value, u32 size);

s32 sceSifLoadFileReset(void)
{
    D_0052C358 = -1;
    memset(D_0059EC28, 0, 4);
    return 0;
}

s32 _sceSifLoadModuleBuffer(void *buffer, s32 argumentSize, const void *arguments, s32 *moduleResult)
{
    s32 result;

    if (_lf_bind() < 0) {
        return -65536;
    }
    if (_lf_version() != 0) {
        return -65540;
    }
    D_0059EA00.value = (s32)buffer;
    if (arguments != NULL) {
        if (argumentSize > 0xFC) {
            memcpy(D_0059EA00.arguments, arguments, 0xFC);
            D_0059EA00.moduleResult = 0xFC;
        } else {
            memcpy(D_0059EA00.arguments, arguments, argumentSize);
            D_0059EA00.moduleResult = argumentSize;
        }
    } else {
        D_0059EA00.moduleResult = 0;
    }
    if (sceSifCallRpc(&D_0059EC00, 6, 0, &D_0059EA00, 0x200,
                      &D_0059EA00, 8, NULL, NULL) < 0) {
        return -65537;
    }
    result = D_0059EA00.value;
    *moduleResult = D_0059EA00.moduleResult;
    return result;
}

s32 sceSifStopModule(s32 moduleId, s32 argumentSize, const void *arguments, s32 *moduleResult)
{
    s32 result;

    if (_lf_bind() < 0) {
        return -65536;
    }
    if (_lf_version() != 0) {
        return -65540;
    }
    D_0059EA00.value = moduleId;
    if (arguments != NULL) {
        if (argumentSize > 0xFC) {
            memcpy(D_0059EA00.arguments, arguments, 0xFC);
            D_0059EA00.moduleResult = 0xFC;
        } else {
            memcpy(D_0059EA00.arguments, arguments, argumentSize);
            D_0059EA00.moduleResult = argumentSize;
        }
    } else {
        D_0059EA00.moduleResult = 0;
    }
    if (sceSifCallRpc(&D_0059EC00, 7, 0, &D_0059EA00, 0x200,
                      &D_0059EA00, 8, NULL, NULL) < 0) {
        return -65537;
    }
    result = D_0059EA00.value;
    *moduleResult = D_0059EA00.moduleResult;
    return result;
}

s32 sceSifUnloadModule(s32 moduleId)
{
    if (_lf_bind() < 0) {
        return -65536;
    }
    if (_lf_version() != 0) {
        return -65540;
    }
    D_0059EA00.value = moduleId;
    if (sceSifCallRpc(&D_0059EC00, 8, 0, &D_0059EA00, 4,
                      &D_0059EA00, 4, NULL, NULL) < 0) {
        return -65537;
    }
    return D_0059EA00.value;
}

s32 sceSifSearchModuleByName(const char *name)
{
    if (_lf_bind() < 0) {
        return -65536;
    }
    if (_lf_version() != 0) {
        return -65540;
    }
    strncpy(D_0059EA00.name, name, 0xFC);
    D_0059EA00.name[0xFB] = 0;
    if (sceSifCallRpc(&D_0059EC00, 9, 0, &D_0059EA00, 512,
                      &D_0059EA00, 4, NULL, NULL) < 0) {
        return -65537;
    }
    return D_0059EA00.value;
}

s32 sceSifSearchModuleByAddress(void *address)
{
    if (_lf_bind() < 0) {
        return -65536;
    }
    if (_lf_version() != 0) {
        return -65540;
    }
    D_0059EA00.value = (s32)address;
    if (sceSifCallRpc(&D_0059EC00, 10, 0, &D_0059EA00, 4,
                      &D_0059EA00, 4, NULL, NULL) < 0) {
        return -65537;
    }
    return D_0059EA00.value;
}

s32 sceSifLoadModuleBuffer(void *buffer, s32 argumentSize, const void *arguments)
{
    s32 moduleResult;

    return _sceSifLoadModuleBuffer(buffer, argumentSize, arguments, &moduleResult);
}

s32 sceSifLoadStartModuleBuffer(void *buffer, s32 argumentSize, const void *arguments,
                                s32 *moduleResult)
{
    return _sceSifLoadModuleBuffer(buffer, argumentSize, arguments, moduleResult);
}

s32 _sceSifLoadModule(const char *path, s32 argumentSize, const void *arguments,
                      s32 *moduleResult, s32 command)
{
    s32 result;

    if (_lf_bind() < 0) {
        return -65536;
    }
    if (_lf_version() != 0) {
        return -65540;
    }
    strncpy(D_0059EA00.name, path, 0xFC);
    D_0059EA00.name[0xFB] = 0;
    if (arguments != NULL) {
        if (argumentSize > 0xFC) {
            memcpy(D_0059EA00.arguments, arguments, 0xFC);
            D_0059EA00.value = 0xFC;
        } else {
            memcpy(D_0059EA00.arguments, arguments, argumentSize);
            D_0059EA00.value = argumentSize;
        }
    } else {
        D_0059EA00.arguments[0] = 0;
        D_0059EA00.value = 0;
    }
    if (sceSifCallRpc(&D_0059EC00, command, 0, &D_0059EA00, 0x200,
                      &D_0059EA00, 8, NULL, NULL) < 0) {
        return -65537;
    }
    result = D_0059EA00.value;
    *moduleResult = D_0059EA00.moduleResult;
    return result;
}

extern s32 _sceSifLoadModule(const char *path, s32 argumentSize, const void *arguments,
                              s32 *moduleResult, s32 command);

s32 sceSifLoadModule(const char *path, s32 argumentSize, const void *arguments)
{
    s32 moduleResult;

    return _sceSifLoadModule(path, argumentSize, arguments, &moduleResult, 0);
}

s32 sceSifLoadStartModule(const char *path, s32 argumentSize, const void *arguments,
                          s32 *moduleResult)
{
    return _sceSifLoadModule(path, argumentSize, arguments, moduleResult, 0);
}

s32 _sceSifLoadElfPart(const char *path, const char *section, u32 *result, s32 command)
{
    if (_lf_bind() < 0) {
        return -65536;
    }
    if (_lf_version() != 0) {
        return -65540;
    }
    strncpy(D_0059EA00.name, path, 0xFC);
    D_0059EA00.name[0xFB] = 0;
    strncpy((char *)D_0059EA00.arguments, section, 0xFC);
    D_0059EA00.arguments[0xFB] = 0;
    if (sceSifCallRpc(&D_0059EC00, command, 0, &D_0059EA00, 0x200,
                      &D_0059EA00, 16, NULL, NULL) < 0) {
        return -65537;
    }
    if (D_0059EA00.value == 0) {
        return -65539;
    }
    result[0] = D_0059EA00.value;
    result[1] = D_0059EA00.moduleResult;
    return 0;
}

s32 sceSifLoadElfPart(const char *path, const char *section, u32 *result)
{
    return _sceSifLoadElfPart(path, section, result, 1);
}

INCLUDE_RODATA(const s32, "sdk/libkernl", D_0054BCA8);

INCLUDE_RODATA(const s32, "sdk/libkernl", D_0054BCB0);

s32 sceSifLoadElf(const char *path, u32 *result)
{
    return _sceSifLoadElfPart(path, "all", result, 1);
}

s32 sceSifGetIopAddr(void *address, void *output, u32 kind)
{
    if (_lf_bind() < 0) {
        return -65536;
    }
    if (kind >= 3) {
        return -65538;
    }
    D_0059EA00.value = (s32)address;
    D_0059EA00.moduleResult = kind;
    if (sceSifCallRpc(&D_0059EC00, 3, 0, &D_0059EA00, 32,
                      &D_0059EA00, 32, NULL, NULL) < 0) {
        return -65537;
    }
    if (kind == 0) {
        *(u8 *)output = *(u8 *)&D_0059EA00.value;
    } else if (kind == 1) {
        *(u16 *)output = *(u16 *)&D_0059EA00.value;
    } else if (kind == 2) {
        *(u32 *)output = D_0059EA00.value;
    } else {
        return -65538;
    }
    return 0;
}

/* NON_MATCHING: 51 of 55 words match at the native 220-byte size;
 * the final RPC result selection uses different constant scheduling. */
#ifdef NON_MATCHING
s32 sceSifSetIopAddr(void *address, const void *input, u32 kind)
{
    if (_lf_bind() < 0) {
        return -65536;
    }
    D_0059EA00.value = (s32)address;
    D_0059EA00.moduleResult = kind;
    if (kind == 0) {
        *(u8 *)D_0059EA00.name = *(const u8 *)input;
    } else if (kind == 1) {
        *(u16 *)D_0059EA00.name = *(const u16 *)input;
    } else if (kind == 2) {
        *(u32 *)D_0059EA00.name = *(const u32 *)input;
    } else {
        return -65538;
    }
    return sceSifCallRpc(&D_0059EC00, 2, 0, &D_0059EA00, 32,
                         &D_0059EA00, 16, NULL, NULL) < 0 ? -65537 : 0;
}
#else
INCLUDE_ASM(const s32, "sdk/libkernl", sceSifSetIopAddr);
#endif

typedef struct sifResetPacket_t {
    u64 packetSize : 8;
    u64 dataSize : 24;
    void *destination;
    u32 command;
    u32 option;
    s32 argumentSize;
    s32 mode;
    char arguments[80];
} sifResetPacket_t;

typedef char sifResetPacket_size_must_be_0x68[
    sizeof(sifResetPacket_t) == 0x68 ? 1 : -1];

extern sifResetPacket_t D_0059EC40;
extern void sceSifStopDma(void);
extern u32 sceSifGetReg(u32 registerId);
extern s32 sceSifSetReg(u32 registerId, u32 value);
extern void sceSifInitRpc(s32 mode);
extern void sceSifExitRpc(void);

s32 sceSifResetIop(const char *arguments, s32 mode)
{
    sifDmaTransfer_t transfer;
    u32 destination;
    s32 length;

    sceSifStopDma();
    destination = sceSifGetReg(0x80000000);
    D_0059EC40.mode = mode;
    for (length = 0; arguments[length] != 0; length++) {
        D_0059EC40.arguments[length] = arguments[length];
    }
    D_0059EC40.destination = NULL;
    D_0059EC40.argumentSize = length;
    D_0059EC40.command = 0x80000003;
    D_0059EC40.dataSize = 0;
    D_0059EC40.packetSize = 104;
    transfer[0] = (u32)&D_0059EC40;
    transfer[1] = destination;
    transfer[2] = 104;
    transfer[3] = 0x44;
    sceSifWriteBackDCache(&D_0059EC40, 104);
    sceSifSetReg(4, 0x40000);
    if (sceSifSetDma(&transfer, 1) != 0) {
        sceSifSetReg(4, 0x10000);
        sceSifSetReg(4, 0x20000);
        sceSifSetReg(0x80000002, 0);
        sceSifSetReg(0x80000000, 0);
        return 1;
    }
    return 0;
}

extern void sceResetttyinit(void);

s32 sceSifIsAliveIop(void)
{
    s32 alive = 0;

    if (sceSifGetReg(4) & 0x10000) {
        alive = 1;
    }
    return alive;
}

s32 sceSifSyncIop(void)
{
    if (sceSifGetReg(4) & 0x40000) {
        sceResetttyinit();
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "sdk/libkernl", sceSifRebootIop);

INCLUDE_ASM(const s32, "sdk/libkernl", DIntr);

INCLUDE_ASM(const s32, "sdk/libkernl", EIntr);

/* Four initialization stores differ; see docs/nonmatching/sdk/libkernl.md. */
#ifdef NON_MATCHING
extern s32 D_0052C378;
extern s32 D_0052C37C;

void supplement_crt0(void)
{
    rpcSemaParam first;
    rpcSemaParam second;

    first.maxCount = second.maxCount = 1;
    first.initCount = second.initCount = 1;
    D_0052C378 = CreateSema(&first);
    D_0052C37C = CreateSema(&second);
}
#else
INCLUDE_ASM(const s32, "sdk/libkernl", supplement_crt0);
#endif

INCLUDE_ASM(const s32, "sdk/libkernl", Copy);

s32 kCopy(u32 *dst, const u32 *src, u32 count)
{
    u32 i;

    count >>= 2;
    for (i = 0; i < count; i++) {
        *dst++ = *src++;
    }
    return 0;
}

u32 *kFindAddress(u32 *start, const u32 *end, u32 instruction)
{
    while (*start != instruction && start < end) {
        start++;
    }
    if (start >= end) {
        start = NULL;
    }
    return start;
}

INCLUDE_ASM(const s32, "sdk/libkernl", FindAddress);

extern u32 *D_0052C360 __attribute__((section(".data")));
extern s32 Copy(void *dst, const void *src, u32 count);

u32 GetSystemCallEntry(s32 index)
{
    u32 entry;

    Copy(&entry, &D_0052C360[index], 4);
    return entry;
}

typedef struct {
    u32 index;
    void *entry;
} kernelEntryPair_t;

extern kernelEntryPair_t D_0052C368[2];
extern void setup(u32 index, void *entry);
extern u32 *FindAddress(u32 *start, const u32 *end, u32 instruction);

void GetSystemCallTableEntry(void)
{
    u32 *searchEntry;
    u32 *copyEntry;
    u32 *searchBase;
    u32 *copyBase;

    setup(D_0052C368[0].index, D_0052C368[0].entry);
    setup(D_0052C368[1].index, D_0052C368[1].entry);
    searchEntry = FindAddress((u32 *)0x80000000, (u32 *)0x80080000,
                              (u32)kFindAddress);
    copyEntry = FindAddress((u32 *)0x80000000, (u32 *)0x80080000,
                            (u32)kCopy);
    searchBase = searchEntry - 0x83;
    copyBase = copyEntry - 0x5A;
    while (searchBase != copyBase) {
        if (searchBase < copyBase) {
            searchEntry = FindAddress(searchEntry + 1, (u32 *)0x80080000,
                                      (u32)kFindAddress);
            searchBase = searchEntry - 0x83;
        } else {
            copyEntry = FindAddress(copyEntry + 1, (u32 *)0x80080000,
                                    (u32)kCopy);
            copyBase = copyEntry - 0x5A;
        }
    }
    D_0052C360 = searchBase;
}

INCLUDE_ASM(const s32, "sdk/libkernl", setup);

extern void supplement_crt0(void);
extern void GetSystemCallTableEntry(void);
extern void InitAlarm(void);
extern s32 InitThread(void);
extern void InitExecPS2(void);
extern void InitTLBFunctions(void);

void _InitSys(void)
{
    supplement_crt0();
    GetSystemCallTableEntry();
    InitAlarm();
    InitThread();
    InitExecPS2();
    InitTLBFunctions();
}

INCLUDE_ASM(const s32, "sdk/libkernl", gcc2_compiled__15);

INCLUDE_ASM(const s32, "sdk/libkernl", Copy_0);

s32 kCopy_0(u32 *dst, const u32 *src, u32 count)
{
    u32 i;

    count >>= 2;
    for (i = 0; i < count; i++) {
        dst[i] = src[i];
    }
    return 0;
}

INCLUDE_ASM(const s32, "sdk/libkernl", GetEntryAddress);

extern void SetOsdConfigParam(const u32 *param);
extern void GetOsdConfigParam(u32 *param);

s32 PatchIsNeeded_0(void)
{
    u32 original;
    u32 changed;

    GetOsdConfigParam(&original);
    changed = (original & 0xFFFF1FFF) | 0x2000;
    SetOsdConfigParam(&changed);
    GetOsdConfigParam(&changed);
    SetOsdConfigParam(&original);
    return ((changed >> 13) & 7) == 0;
}

extern void FlushCache(s32 mode);

extern kernelEntryPair_t D_0052CB28[3];
extern const u8 D_0052C380[0x7A8];
extern s32 PatchIsNeeded_0(void);
extern void gcc2_compiled__15(u32 index, void *entry);
extern s32 Copy_0(void *destination, const void *source, u32 size);
extern void *GetEntryAddress(u32 index);

void InitExecPS2(void)
{
    u32 index;
    u32 i;
    void *entry;

    if (PatchIsNeeded_0() == 0) {
        return;
    }
    gcc2_compiled__15(D_0052CB28[0].index, D_0052CB28[0].entry);
    Copy_0((void *)0x80074000, D_0052C380, 0x7A8);
    FlushCache(0);
    FlushCache(2);
    gcc2_compiled__15(D_0052CB28[1].index, D_0052CB28[1].entry);
    for (i = 2; i < 3; i++) {
        index = D_0052CB28[i].index;
        entry = GetEntryAddress(index);
        gcc2_compiled__15(D_0052CB28[i].index, entry);
    }
}

INCLUDE_ASM(const s32, "sdk/libkernl", gcc2_compiled__16);

s32 kCopy_1(u8 *dst, const u8 *src, u32 count)
{
    u32 i;

    i = 0;
    if (count != 0) {
        do {
            *dst++ = *src++;
            i++;
        } while (i < count);
    }
    return 0;
}

INCLUDE_ASM(const s32, "sdk/libkernl", PatchIsNeeded);

typedef struct execArgumentBlock_t {
    char *arguments[16];
    char strings[1];
} execArgumentBlock_t;
extern void *D_0052D668;
extern kernelEntryPair_t D_0052CB40[1];
extern void PatchIsNeeded(u32 index, void *entry);
extern s32 gcc2_compiled__16(void *destination, const void *source, u32 size);
extern u32 strlen(const char *text);

char *SetArg(const char *path, s32 count, const char *const *arguments)
{
    char **table = ((execArgumentBlock_t *)D_0052D668)->arguments;
    char *cursor = (char *)&table[16];
    char *start = cursor;
    u32 length;

    PatchIsNeeded(D_0052CB40[0].index, D_0052CB40[0].entry);
    if (count >= 16) {
        count = 15;
    }
    gcc2_compiled__16(table, &cursor, 4);
    table++;
    length = strlen(path) + 1;
    gcc2_compiled__16(cursor, path, length);
    cursor += length;
    if (count > 0) {
        const char *const *argument = arguments;

        do {
            gcc2_compiled__16(table++, &cursor, 4);
            count--;
            length = strlen(*argument) + 1;
            gcc2_compiled__16(cursor, *argument++, length);
            cursor += length;
        } while (count != 0);
    }
    return start;
}

extern void InitTLB(void);

void TerminateLibrary(void)
{
    InitTLB();
}

INCLUDE_ASM(const s32, "sdk/libkernl", ExecPS2);

INCLUDE_ASM(const s32, "sdk/libkernl", LoadExecPS2);

INCLUDE_ASM(const s32, "sdk/libkernl", Exit);

INCLUDE_ASM(const s32, "sdk/libkernl", ExecOSD);

INCLUDE_ASM(const s32, "sdk/libkernl", gcc2_compiled__17);

INCLUDE_ASM(const s32, "sdk/libkernl", Copy_1);

s32 kCopy_2(u32 *dst, const u32 *src, u32 count)
{
    u32 i;

    count >>= 2;
    for (i = 0; i < count; i++) {
        dst[i] = src[i];
    }
    return 0;
}

INCLUDE_ASM(const s32, "sdk/libkernl", GetEntryAddress_0);


extern kernelEntryPair_t D_0052D2B0[8];
extern kernelEntryPair_t D_0052D670[8];
extern void *D_0052D668;
extern const u8 D_0052CB48[0x740];
extern const u8 D_0052D288[0x28];
extern const u8 D_0052D2F0[0x330];
extern void gcc2_compiled__17(u32 index, void *entry);
extern s32 Copy_1(void *destination, const void *source, u32 size);
extern s32 Copy_2(void *destination, const void *source, u32 size);
extern void *GetEntryAddress_0(u32 index);
extern void *GetEntryAddress_1(u32 index);
extern void setup_0(u32 index, void *entry);

void InitAlarm(void)
{
    u32 index;
    u32 i;
    void *entry;

    if ((*(volatile u32 *)0x10001810 & 0x100) != 0) {
        return;
    }
    gcc2_compiled__17(D_0052D2B0[0].index, D_0052D2B0[0].entry);
    Copy_1((void *)0x80076000, D_0052CB48, 0x740);
    Copy_1((void *)0x82000, D_0052D288, 0x28);
    FlushCache(0);
    FlushCache(2);
    gcc2_compiled__17(D_0052D2B0[1].index, D_0052D2B0[1].entry);
    for (i = 2; i < 8; i++) {
        index = D_0052D2B0[i].index;
        entry = GetEntryAddress_0(index);
        gcc2_compiled__17(D_0052D2B0[i].index, entry);
    }
}


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

s32 kCopy_3(u32 *dst, const u32 *src, u32 count)
{
    u32 i;

    count >>= 2;
    for (i = 0; i < count; i++) {
        dst[i] = src[i];
    }
    return 0;
}

INCLUDE_ASM(const s32, "sdk/libkernl", GetEntryAddress_1);

INCLUDE_ASM(const s32, "sdk/libkernl", setup_0);

void InitTLBFunctions(void)
{
    u32 index;
    u32 i;
    void *entry;

    setup_0(D_0052D670[0].index, D_0052D670[0].entry);
    Copy_2((void *)0x80075000, D_0052D2F0, 0x330);
    FlushCache(0);
    FlushCache(2);
    setup_0(D_0052D670[1].index, D_0052D670[1].entry);
    setup_0(D_0052D670[2].index, D_0052D670[2].entry);
    for (i = 3; i < 8; i++) {
        index = D_0052D670[i].index;
        entry = GetEntryAddress_1(index);
        setup_0(D_0052D670[i].index, entry);
    }
    D_0052D668 = GetEntryAddress_1(3);
}

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
