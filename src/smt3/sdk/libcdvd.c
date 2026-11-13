#include "sdk_libcdvd.h"
#include "common.h"
#include "ps2asm.h"

extern s32 cdvdStreamActive __attribute__((section(".data")));
typedef void (*cdvdCallback_t)(s32 reason);
extern cdvdCallback_t cdvdCallback __attribute__((section(".data")));
extern u8 cdvdDefaultStreamMode[] __attribute__((section(".data")));

extern s32 cdvdDebugLevel __attribute__((section(".data")));
typedef struct cdvdRpcClient_t {
    u8 transactionAndCallbacks[0x24];
    void *server;
} cdvdRpcClient_t;
extern cdvdRpcClient_t cdvdScmdClient __attribute__((section(".data")));

extern s32 cdvdNcmdState __attribute__((section(".data")));
extern s32 cdvdScmdLockSema __attribute__((section(".data")));
extern cdvdRpcClient_t cdvdNcmdClient __attribute__((section(".data")));
extern s32 cdvdMmodeArg __attribute__((section(".data")));
extern s32 cdvdScmdReply[4] __attribute__((section(".data")));

typedef struct sdfSemaParam_t {
    s32 count;
    s32 maxCount;
    s32 initCount;
    s32 waitThreads;
    u32 attr;
    u32 option;
} sdfSemaParam_t;

extern s32 CreateSema(sdfSemaParam_t *param);
extern s32 iSignalSema(s32 sema);
extern s32 WaitSema(s32 sema);
extern s32 DeleteSema(s32 sema);
extern s32 SetAlarm(u16 ticks, void (*callback)(s32, s32, s32), void *argument);
extern void gcc2_compiled__6(s32 cause, s32 address, s32 argument);
extern s32 cdvdNcmdSema __attribute__((section(".data")));

/* Same native CreateThread descriptor as the matched SDF constructor. */
typedef struct {
    s32 status;
    void (*entry)(void);
    void *stack;
    s32 stackSize;
    void *gp;
    s32 priority;
    u8 unk18[0x18];
} sdfKernelThreadParam_t;

extern s32 cdvdCallbackThreadId __attribute__((section(".data")));
extern s32 cdvdOwnerThreadId __attribute__((section(".data")));
extern u8 cdvdThreadStatus[0x30] __attribute__((section(".data")));
extern sdfKernelThreadParam_t cdvdCallbackThreadParam __attribute__((section(".data")));
extern s32 _gp;
extern s32 GetThreadId(void);
extern s32 ReferThreadStatus(s32 threadId, void *status);
extern s32 CreateThread(sdfKernelThreadParam_t *param);
#include "sdk_libkernl.h"

extern s32 ChangeThreadPriority(s32 threadId, s32 priority);
void _Cdvd_cbLoop(void);

/* Consumed transfer view: payloads begin at 0x10 and 0x50. The trailing
 * array leaves the second payload extent to the producer; no total size is known. */
typedef struct {
    s32 firstLength;
    s32 secondLength;
    u8 *firstDestination;
    u8 *secondDestination;
    u8 firstData[0x40];
    u8 secondData[0];
} cdvdReadReplyPrefix_t;

extern s32 cdvdCallbackReason __attribute__((section(".data")));
void _sceCd_cd_callback(s32 *reason);

typedef void (*cdvdPowerCallback_t)(void *argument);
extern cdvdPowerCallback_t cdvdPowerCallback __attribute__((section(".data")));
extern void *cdvdPowerArgument __attribute__((section(".data")));
extern s32 cdvdPowerOffActive __attribute__((section(".data")));
extern s32 cdvdScmdState[6] __attribute__((section(".data")));
extern void sceSifAddCmdHandler(s32 id, void (*handler)(void *, void *), void *argument);
void _sceCd_Poff_Intr(void *packet, void *argument);
s32 PowerOffCB(void);

/* The reply word is read back through the uncached mirror, since the IOP
 * wrote it behind the data cache. */
#define UNCACHED(a) ((s32 *)(EE_ADDR(a) | 0x20000000))

extern s32 DIntr(void);
extern s32 SignalSema(s32 sema);
extern void sceSifWriteBackDCache(void *addr, s32 size);
extern s32 sceSifCallRpc(void *cd, u32 fno, u32 mode, void *send, s32 ssize,
                         void *recv, s32 rsize, void *endfunc, void *endpara);
extern s32 _sceCd_scmd_prechk(s32 cmd);
extern int scePrintf(const char *fmt, ...);
extern s32 sceSifCheckStatRpc(void *cd);
extern void sceCdDelayThread(u16 ticks);
extern s32 EIntr(void);
extern s32 sceCdSync(s32 mode);
extern s32 sceCdStream(u32 lsn, u32 sectors, void *buf, s32 cmd, void *arg);
extern s32 sceCdLayerSearchFile(addr_t file, const char *name, s32 layer);

void gcc2_compiled__6(s32 cause, s32 address, s32 argument)
{
    iSignalSema(argument);
    EE_SYNC_EI();
}

void sceCdDelayThread(u16 ticks)
{
    sdfSemaParam_t param;
    s32 sema;

    param.maxCount = 1;
    param.initCount = 0;
    param.option = 0;
    sema = CreateSema(&param);
    SetAlarm(ticks, gcc2_compiled__6, EE_ADDR_AS(void *, sema));
    WaitSema(sema);
    DeleteSema(sema);
}

cdvdCallback_t sceCdCallback(cdvdCallback_t func)
{
    s32 intr;
    cdvdCallback_t old;

    if (sceCdSync(1) != 0) {
        return 0;
    }
    intr = DIntr();
    old = cdvdCallback;
    cdvdCallback = func;
    if (intr) {
        EIntr();
    }
    return old;
}

extern s32 cdvdScmdSema __attribute__((section(".data")));
/* The native 8-byte object holds pending reason at +0 and saved reason at +4.
 * _Cdvd_cbLoop tests pending and passes saved to the registered callback. */
SYMBOL_AT(volatile s32, cdvdCallbackSavedReason, cdvdCallbackReason, 4);

void _sceCd_cd_callback(s32 *reason)
{
    /* Interrupt-published state is accessed at each native read/write.
     * Qualify only these accesses; ordinary callers retain their own contract. */
    (*(volatile s32 *)&cdvdCallbackReason) = *reason;
    cdvdCallbackSavedReason = (*(volatile s32 *)&cdvdCallbackReason);
    if ((*(volatile s32 *)&cdvdCallbackReason) == 11) {
        (*(volatile s32 *)&cdvdCallbackReason) = 0;
        (*(volatile s32 *)&cdvdNcmdState) = 0;
        return;
    }
    iSignalSema((*(volatile s32 *)&cdvdNcmdSema));
    if (cdvdCallbackThreadId != 0 && cdvdCallback != NULL) {
        iSignalSema((*(volatile s32 *)&cdvdScmdSema));
    } else {
        (*(volatile s32 *)&cdvdNcmdState) = 0;
    }
    (*(volatile s32 *)&cdvdCallbackReason) = 0;
}

INCLUDE_ASM(const s32, "sdk/libcdvd", _Cdvd_cbLoop);

s32 sceCdInitEeCB(s32 priority, void *stack, s32 stackSize)
{
    s32 created = 1;

    if (cdvdCallbackThreadId == 0) {
        cdvdOwnerThreadId = GetThreadId();
        ReferThreadStatus(cdvdOwnerThreadId, cdvdThreadStatus);
        cdvdCallbackThreadParam.stackSize = stackSize;
        cdvdCallbackThreadParam.gp = &_gp;
        cdvdCallbackThreadParam.entry = _Cdvd_cbLoop;
        cdvdCallbackThreadParam.stack = stack;
        cdvdCallbackThreadParam.priority = priority;
        cdvdCallbackThreadId = CreateThread(&cdvdCallbackThreadParam);
        _StartThread(cdvdCallbackThreadId, NULL);
    } else {
        ChangeThreadPriority(cdvdCallbackThreadId, priority);
        created = 0;
    }
    return created;
}

void _sceCd_cd_read_intr(void *argument)
{
    cdvdReadReplyPrefix_t *reply = EE_ADDR_AS(cdvdReadReplyPrefix_t *, (EE_ADDR(argument) | 0x20000000));
    u8 *destination;
    s32 i;

    if (reply->firstLength > 0) {
        destination = reply->firstDestination;
        for (i = 0; i < reply->firstLength; i++) {
            destination[i] = reply->firstData[i];
        }
    }
    if (reply->secondLength > 0) {
        destination = reply->secondDestination;
        for (i = 0; i < reply->secondLength; i++) {
            destination[i] = reply->secondData[i];
        }
    }
    _sceCd_cd_callback(&cdvdCallbackReason);
}

extern s32 cdvdScmdSema __attribute__((section(".data")));
extern void sceSifRemoveCmdHandler(s32 command);

void cmd_sem_init(void)
{
    /* Retain the native partial descriptor initialization. The opaque kernel
     * service's treatment of the untouched fields is not inferred here. */
    sdfSemaParam_t param;

    if (cdvdNcmdSema == -1 || cdvdScmdLockSema == -1) {
        param.option = 0;
        param.initCount = 1;
        param.maxCount = 1;
        {
            s32 sema = CreateSema(&param);
            /* Target-specific aid on the actual second-call argument: prepare
             * it before publishing the first result. Its lifetime begins after
             * the first call and ends at the second, with no intervening call. */
            FAKEMATCH_ASM_REG(sdfSemaParam_t *, nextParam, "a0") = &param;
            FAKEMATCH_ASM_LIVE(nextParam);
            cdvdNcmdSema = sema;
            cdvdScmdLockSema = CreateSema(nextParam);
        }
        param.initCount = 0;
        cdvdScmdSema = CreateSema(&param);
        /* Keep the real published semaphore in storage before the following
         * clear-address setup. This empty read-only operand emits no access
         * and is not a memory barrier or an original-source claim. */
        FAKEMATCH_ASM_READ_MEM(cdvdScmdSema);
        cdvdNcmdState = 0;
    }
}


void cdvd_exit(void)
{
    s32 intr;

    if (cdvdCallbackThreadId != 0) {
        /* The one-pass block preserves the SDK compiler's native ordering:
         * publish the stop reason before loading the semaphore to signal. */
        do {
            cdvdCallbackReason = -1;
        } while (0);
        SignalSema(cdvdScmdSema);
    }
    DeleteSema(cdvdNcmdSema);
    DeleteSema(cdvdScmdLockSema);
    DeleteSema(cdvdScmdSema);
    intr = DIntr();
    sceSifRemoveCmdHandler(0x80000012);
    if (intr) {
        EIntr();
    }
}

cdvdPowerCallback_t sceCdPOffCallback(cdvdPowerCallback_t callback, void *argument)
{
    cdvdPowerCallback_t old;
    s32 intr;

    if (cdvdScmdState[0] < 0) {
        PowerOffCB();
    }
    intr = DIntr();
    old = cdvdPowerCallback;
    cdvdPowerArgument = argument;
    cdvdPowerCallback = callback;
    if (intr) {
        EIntr();
    }
    return old;
}

void _sceCd_Poff_Intr(void *packet, void *argument)
{
    if (cdvdPowerCallback != NULL && cdvdPowerOffActive == 0) {
        cdvdPowerCallback(cdvdPowerArgument);
    }
}

s32 PowerOffCB(void)
{
    s32 intr;

    cdvdPowerOffActive = 1;
    intr = DIntr();
    sceSifAddCmdHandler(0x80000012, _sceCd_Poff_Intr, NULL);
    if (intr) {
        EIntr();
    }
    cdvdPowerOffActive = 0;
    cdvdScmdState[0] = 1;
    return 1;
}

INCLUDE_ASM(const s32, "sdk/libcdvd", sceCdLayerSearchFile);

s32 sceCdSearchFile(sceCdlFILE *file, const char *name)
{
    return sceCdLayerSearchFile(EE_ADDR(file), (char *)name, 0);
}

extern void cmd_sem_init(void);
extern s32 PollSema(s32 sema);
extern void sceSifInitRpc(s32 mode);
extern s32 sceSifBindRpc(void *client, u32 service, s32 mode);
extern s32 cdvdNcmdLastCommand __attribute__((section(".data")));
SYMBOL_AT(s32, cdvdNcmdInit, cdvdNcmdState, 8);

s32 _sceCd_ncmd_prechk(s32 command)
{
    s32 status;
    s32 delay;

    cmd_sem_init();
    {
        s32 sema = cdvdNcmdSema;
        FAKEMATCH_ASM_LIVE(sema);
        status = PollSema(sema);
    }
    if (cdvdNcmdSema != status) {
        if (cdvdDebugLevel > 0) {
            scePrintf("Ncmd fail sema cur_cmd:%d keep_cmd:%d\n", command, cdvdNcmdLastCommand);
        }
        return 0;
    }
    cdvdNcmdLastCommand = command;
    ReferThreadStatus(cdvdOwnerThreadId, cdvdThreadStatus);
    status = sceCdSync(1);
    {
        FAKEMATCH_ASM_REG(s32, rpcMode, "$4") = 0;
        if (status != 0) {
            s32 sema = cdvdNcmdSema;
            FAKEMATCH_ASM_LIVE(sema);
            SignalSema(sema);
            return 0;
        }
        sceSifInitRpc(rpcMode);
    }
    if (cdvdNcmdInit < 0) {
        for (;;) {
            cdvdRpcClient_t *client = &cdvdNcmdClient;

            if (sceSifBindRpc(client, 0x80000595, 0) < 0) {
                if (cdvdDebugLevel > 0) {
                    scePrintf("Libcdvd bind err N CMD\n");
                }
                for (delay = 0x100000; delay != -1; --delay) {
                }
                continue;
            }
            if (client->server != NULL) {
                break;
            }
            for (delay = 0x100000; delay != -1; --delay) {
            }
        }
        cdvdNcmdInit = 0;
    }
    return 1;
}


extern s32 _sceCd_ncmd_prechk(s32 cmd);
extern s32 cdvdNcmdReply __attribute__((section(".data")));

s32 sceCdNcmdDiskReady(void)
{
    s32 ret;

    if (!_sceCd_ncmd_prechk(2)) {
        return 0;
    }
    if (sceSifCallRpc(&cdvdNcmdClient, 14, 0, 0, 0, &cdvdNcmdReply, 4, 0, 0) < 0) {
        SignalSema(cdvdNcmdSema);
        return 0;
    }
    ret = UNCACHED(&cdvdNcmdReply)[0];
    SignalSema(cdvdNcmdSema);
    return ret;
}

s32 sceCdSync(s32 mode)
{
    if (mode == 0) {
        if (cdvdDebugLevel > 0) {
            scePrintf("N cmd wait\n");
        }
        while (cdvdNcmdState != 0 || sceSifCheckStatRpc(&cdvdNcmdClient) != 0) {
            sceCdDelayThread(60);
        }
        return 0;
    }
    if (cdvdNcmdState != 0 || sceSifCheckStatRpc(&cdvdNcmdClient) != 0) {
        return 1;
    }
    return 0;
}

s32 sceCdSyncS(s32 mode)
{
    if (mode == 0) {
        if (cdvdDebugLevel > 0) {
            scePrintf("S cmd wait\n");
        }
        while (1) {
            if (sceSifCheckStatRpc(&cdvdScmdClient) == 0) {
                break;
            }
            sceCdDelayThread(60);
        }
        return 0;
    }
    return sceSifCheckStatRpc(&cdvdScmdClient);
}

extern void cmd_sem_init(void);
extern s32 PollSema(s32 sema);
extern void sceSifInitRpc(s32 mode);
extern s32 sceSifBindRpc(void *client, u32 service, s32 mode);
extern s32 cdvdScmdLastCommand __attribute__((section(".data")));
SYMBOL_AT(s32, cdvdScmdInit, cdvdScmdState, 12);

s32 _sceCd_scmd_prechk(s32 command)
{
    s32 status;
    s32 delay;

    cmd_sem_init();
    {
        s32 sema = cdvdScmdLockSema;
        FAKEMATCH_ASM_LIVE(sema);
        status = PollSema(sema);
    }
    if (cdvdScmdLockSema != status) {
        if (cdvdDebugLevel > 0) {
            scePrintf("Scmd fail sema cur_cmd:%d keep_cmd:%d\n", command, cdvdScmdLastCommand);
        }
        return 0;
    }
    cdvdScmdLastCommand = command;
    ReferThreadStatus(cdvdOwnerThreadId, cdvdThreadStatus);
    status = sceCdSyncS(1);
    {
        FAKEMATCH_ASM_REG(s32, rpcMode, "$4") = 0;
        if (status != 0) {
            s32 sema = cdvdScmdLockSema;
            FAKEMATCH_ASM_LIVE(sema);
            SignalSema(sema);
            return 0;
        }
        sceSifInitRpc(rpcMode);
    }
    if (cdvdScmdInit < 0) {
        for (;;) {
            cdvdRpcClient_t *client = &cdvdScmdClient;

            if (sceSifBindRpc(client, 0x80000593, 0) < 0) {
                if (cdvdDebugLevel > 0) {
                    scePrintf("Libcdvd bind err S cmd\n");
                }
                for (delay = 0x100000; delay != -1; --delay) {
                }
                continue;
            }
            if (client->server != NULL) {
                break;
            }
            for (delay = 0x100000; delay != -1; --delay) {
            }
        }
        cdvdScmdInit = 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "sdk/libcdvd", sceCdInit);

/* NON_MATCHING: native 504-byte shape, saved-register allocation differs. */
#ifdef NON_MATCHING
extern cdvdRpcClient_t D_00598E90;
extern s32 D_00598ED0;
SYMBOL_AT(s32, cdvdDiskReadyInit, cdvdScmdState, 8);

/* The failure-return branch and RPC mode retain native scheduling; the
 * pending nonmatch is the allocation of the long-lived saved values. The
 * two semaphore constraints keep actual freshly loaded call arguments out
 * of the native empty call delay slots, as in the matched precheck siblings. */
s32 sceCdDiskReady(s32 mode)
{
    s32 status;
    s32 delay;
    s32 ret;
    if (cdvdDebugLevel > 0) {
        scePrintf("DiskReady 0\n");
    }
    cmd_sem_init();
    {
        s32 sema = cdvdScmdLockSema;
        FAKEMATCH_ASM_LIVE(sema);
        status = PollSema(sema);
    }
    if (cdvdScmdLockSema != status) {
        return 6;
    }
    status = sceCdSyncS(1);
    {
        FAKEMATCH_ASM_REG(s32, rpcMode, "$4") = 0;
        if (status != 0) goto fail;
        sceSifInitRpc(rpcMode);
    }
    if (cdvdDiskReadyInit < 0) {
        for (;;) {
            cdvdRpcClient_t *client = &D_00598E90;
            if (sceSifBindRpc(client, 0x8000059A, 0) < 0) {
                if (cdvdDebugLevel > 0) {
                    scePrintf("Libcdvd bind err CdDiskReady\n");
                }
                for (delay = 0x100000; delay != -1; --delay) {
                }
                continue;
            }
            if (client->server != NULL) {
                break;
            }
            for (delay = 0x100000; delay != -1; --delay) {
            }
        }
        cdvdDiskReadyInit = 0;
    }
    D_00598ED0 = mode;
    sceSifWriteBackDCache(&D_00598ED0, 4);
    if (sceSifCallRpc(&D_00598E90, 0, 0, &D_00598ED0, 4, cdvdScmdReply, 4, 0, 0) < 0) {
fail:
        {
            s32 sema = cdvdScmdLockSema;
            FAKEMATCH_ASM_LIVE(sema);
            SignalSema(sema);
        }
        if (mode == 8) return -1;
        return 6;
    }
    if (cdvdDebugLevel > 0) {
        scePrintf("DiskReady ended\n");
    }
    ret = UNCACHED(cdvdScmdReply)[0];
    SignalSema(cdvdScmdLockSema);
    return ret;
}

#else
INCLUDE_ASM(const s32, "sdk/libcdvd", sceCdDiskReady);
#endif

s32 sceCdMmode(s32 media)
{
    s32 *arg = &cdvdMmodeArg;
    s32 ret;

    if (!_sceCd_scmd_prechk(34)) {
        return 0;
    }
    cdvdMmodeArg = media;
    sceSifWriteBackDCache(arg, 4);
    if (sceSifCallRpc(&cdvdScmdClient, 34, 0, arg, 4, cdvdScmdReply, 4, 0, 0) < 0) {
        SignalSema(cdvdScmdLockSema);
        return 0;
    }
    ret = UNCACHED(cdvdScmdReply)[0];
    SignalSema(cdvdScmdLockSema);
    return ret;
}

s32 sceCdGetDiskType(void)
{
    s32 ret;

    if (!_sceCd_scmd_prechk(1)) {
        return 0;
    }
    if (sceSifCallRpc(&cdvdScmdClient, 3, 0, 0, 0, cdvdScmdReply, 4, 0, 0) < 0) {
        SignalSema(cdvdScmdLockSema);
        return 0;
    }
    ret = UNCACHED(cdvdScmdReply)[0];
    SignalSema(cdvdScmdLockSema);
    return ret;
}

s32 sceCdStatus(void)
{
    s32 ret;

    if (!_sceCd_scmd_prechk(2)) {
        return -1;
    }
    if (sceSifCallRpc(&cdvdScmdClient, 12, 0, 0, 0, cdvdScmdReply, 4, 0, 0) < 0) {
        SignalSema(cdvdScmdLockSema);
        return -1;
    }
    ret = UNCACHED(cdvdScmdReply)[0];
    SignalSema(cdvdScmdLockSema);
    if (cdvdDebugLevel >= 2) {
        scePrintf("status called\n");
    }
    return ret;
}

s32 sceCdPowerOff(s32 *result)
{
    s32 ret;

    if (!_sceCd_scmd_prechk(33)) {
        return 0;
    }
    if (sceSifCallRpc(&cdvdScmdClient, 33, 0, 0, 0, cdvdScmdReply, 8, 0, 0) < 0) {
        SignalSema(cdvdScmdLockSema);
        return 0;
    }
    *result = UNCACHED(cdvdScmdReply + 1)[0];
    ret = UNCACHED(cdvdScmdReply)[0];
    SignalSema(cdvdScmdLockSema);
    return ret;
}

typedef struct sdfCdClock_t {
    u8 status;
    u8 second;
    u8 minute;
    u8 hour;
    u8 pad04;
    u8 day;
    u8 month;
    u8 year;
} sdfCdClock_t;

static const char cdvdMsgClockRead1[32] = "Libcdvd call Clock read 1\n";
static const char cdvdMsgClockRead2[32] = "Libcdvd call Clock read 2\n";

s32 sceCdReadClock(sdfCdClock_t *clock)
{
    s32 result;

    if (!_sceCd_scmd_prechk(0xF)) {
        return 0;
    }
    if (cdvdDebugLevel > 0) {
        scePrintf(cdvdMsgClockRead1);
    }
    if (sceSifCallRpc(&cdvdScmdClient, 1, 0, NULL, 0,
                      cdvdScmdReply, 16, NULL, NULL) < 0) {
        SignalSema(cdvdScmdLockSema);
        return 0;
    }
    *clock = *(sdfCdClock_t *)UNCACHED(&cdvdScmdReply[1]);
    if (cdvdDebugLevel > 0) {
        scePrintf(cdvdMsgClockRead2);
    }
    result = UNCACHED(cdvdScmdReply)[0];
    SignalSema(cdvdScmdLockSema);
    return result;
}


s32 sceCdStInit(u32 bufmax, u32 bankmax, void *buf)
{
    cdvdStreamActive = 0;
    return sceCdStream(bufmax, bankmax, buf, 5, cdvdDefaultStreamMode);
}

s32 sceCdStStart(u32 lsn, void *mode)
{
    cdvdStreamActive = 1;
    return sceCdStream(lsn, 0, 0, 1, mode);
}

s32 sceCdStSeekF(u32 lsn)
{
    return sceCdStream(lsn, 0, 0, 9, cdvdDefaultStreamMode);
}

s32 sceCdStSeek(u32 lsn)
{
    return sceCdStream(lsn, 0, 0, 4, cdvdDefaultStreamMode);
}

s32 sceCdStStop(void)
{
    cdvdStreamActive = 0;
    return sceCdStream(0, 0, 0, 3, cdvdDefaultStreamMode);
}

/* NON_MATCHING: native size; the active-flag address/load uses the opposite
 * pair of temporary registers (three words). */
#ifdef NON_MATCHING
s32 sceCdStRead(u32 sectors, u32 *buffer, u32 mode, u32 *status)
{
    s32 error;
    s32 completed;
    s32 read;
    u32 reply;
    s32 currentError;

    if (cdvdDebugLevel > 0) {
        scePrintf("sceCdStRead call read size= %d mode= %d\n", sectors, mode);
    }
    error = 0;
    if (cdvdStreamActive == 0) {
        return 0;
    }
    completed = 0;
    sceSifWriteBackDCache(buffer, sectors << 11);
    if (mode != 0) {
        do {
            reply = sceCdStream(0, sectors - completed,
                               (u8 *)buffer + (completed << 11), 2, cdvdDefaultStreamMode);
            read = reply & 0xFFFF;
            currentError = reply >> 16;
            completed += read;
            if (currentError != 0) {
                error = currentError;
                if (cdvdDebugLevel > 0) {
                    scePrintf("sceCdStRead BLK Read cur_size= %d read_size= %d req_size= %d err 0x%x\n",
                              completed, read, sectors, currentError);
                }
            } else if (read == 0) {
                sceCdDelayThread(8);
            }
        } while (completed != sectors && (currentError == 0 || read != 0));
        if (cdvdDebugLevel > 0) {
            scePrintf("sceCdStRead BLK Read Ended\n");
        }
        *status = error;
    } else {
        reply = sceCdStream(0, sectors, buffer, 2, cdvdDefaultStreamMode);
        currentError = reply >> 16;
        completed = reply & 0xFFFF;
        *status = currentError;
    }
    return completed;
}
#else
INCLUDE_ASM(const s32, "sdk/libcdvd", sceCdStRead);
#endif

s32 sceCdStPause(void)
{
    cdvdStreamActive = 0;
    if (cdvdDebugLevel > 0) {
        scePrintf("sceCdStPause call\n");
    }
    return sceCdStream(0, 0, 0, 7, cdvdDefaultStreamMode);
}

s32 sceCdStResume(void)
{
    cdvdStreamActive = 1;
    if (cdvdDebugLevel > 0) {
        scePrintf("sceCdStResume call\n");
    }
    return sceCdStream(0, 0, 0, 8, cdvdDefaultStreamMode);
}

s32 sceCdStStat(void)
{
    if (cdvdDebugLevel > 0) {
        scePrintf("sceCdStStat call\n");
    }
    return sceCdStream(0, 0, 0, 6, cdvdDefaultStreamMode);
}

typedef struct cdvdStreamRequest_t {
    u32 lsn;
    u32 sectors;
    void *buffer;
    s32 command;
    u8 mode[3];
    u8 opaque13;
} cdvdStreamRequest_t;
extern cdvdStreamRequest_t cdvdStreamRequest __attribute__((section(".data")));
extern s32 _sceCd_ncmd_prechk(s32 command);

s32 sceCdStream(u32 lsn, u32 sectors, void *buf, s32 cmd, void *arg)
{
    cdvdStreamRequest_t *request = &cdvdStreamRequest;
    u8 *mode = arg;
    s32 result;

    if (_sceCd_ncmd_prechk(15) == 0) {
        return 0;
    }
    if (cdvdDebugLevel > 0) {
        scePrintf("call cdreadstm call\n");
    }
    cdvdStreamRequest.lsn = lsn;
    request->sectors = sectors;
    request->buffer = buf;
    request->command = cmd;
    if (mode != NULL) {
        request->mode[0] = mode[0];
        request->mode[1] = mode[1];
        request->mode[2] = mode[2];
    }
    if (cdvdDebugLevel > 0) {
        scePrintf("call cdreadstm cmd\n");
    }
    sceSifWriteBackDCache(request, 0x14);
    if (sceSifCallRpc(&cdvdNcmdClient, 9, 0, request, 0x14,
                      &cdvdNcmdReply, 4, NULL, NULL) < 0) {
        SignalSema(cdvdNcmdSema);
        return 0;
    }
    if (cdvdDebugLevel > 0) {
        scePrintf("cdread end\n");
    }
    result = *UNCACHED(&cdvdNcmdReply);
    SignalSema(cdvdNcmdSema);
    return result;
}
