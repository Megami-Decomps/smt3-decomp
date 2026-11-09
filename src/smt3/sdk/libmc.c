#include "common.h"
#include "ps2asm.h"

/* The sif client data of the mcserv module; +0x24 is non-zero once bind
 * succeeded, which every call tests before using the RPC. */
typedef struct mcRpcCd {
    u32 pad00[9];
    s32 bound;
} mcRpcCd;

extern mcRpcCd mcRpcClient __attribute__((section(".data")));
/* The 0x30-byte request shared by short commands. Their argument words vary;
 * the write view names the consumed fields and inline transfer prefix. */
typedef union mcReq {
    s32 w[8];
    struct {
        s32 descriptor;
        u8 opaque04[8];
        s32 bulkSize;
        u8 opaque10[4];
        u32 inlineSize;
        const void *bulkBuffer;
        u8 opaque1C[4];
        u8 inlineData[16];
    } write;
} mcReq;

extern mcReq mcRequest __attribute__((section(".data")));

/* The 0x40-byte entry block that sits just before the request: four words of
 * directory entry info and a 0x20-byte name. */
typedef struct mcEntInfo {
    u32 info[8];
    char name[0x20];
} mcEntInfo;

extern mcEntInfo mcEntInfoBuf __attribute__((section(".data")));

/* The name-carrying request: the same header, then a 0x400-byte path. */
typedef struct mcNameReq {
    s32 port;
    s32 slot;
    s32 flag;
    s32 maxent;
    void *pwd;
    char name[0x400];
} mcNameReq;

extern mcNameReq mcNameRequest __attribute__((section(".data")));

/* Read-specific view of the shared callback buffer. The producer supplies
 * the second payload extent; other commands use different reply layouts. */
typedef struct {
    s32 firstLength;
    s32 secondLength;
    u8 *firstDestination;
    u8 *secondDestination;
    u8 firstData[0x40];
    u8 secondData[0];
} mcReadReplyPrefix_t;

/* The 0xC0-byte block the RPC end functions read back, and the 0x400-byte
 * working directory sceMcChdir hands to mceStorePwd. */
extern u8 mcEndBuf[0xC0] __attribute__((section(".data")));
extern u8 mcPwdBuf[0x400] __attribute__((section(".data")));

extern saddr_t mcReplyDst0 __attribute__((section(".data")));
extern saddr_t mcReplyDst1 __attribute__((section(".data")));
extern saddr_t mcReplyDst2 __attribute__((section(".data")));
/* The shared reply begins with result/version words. The client accessor
 * also publishes the command semaphore at the end of its 0x40-byte buffer. */
typedef struct mcReply {
    s32 result;
    s32 serverVersion;
    s32 managerVersion;
    u8 reserved0C[0x30];
    s32 sema;
} mcReply;

extern mcReply mcReplyBuf __attribute__((section(".data")));
extern s32 mcLastCmd __attribute__((section(".data")));
extern s32 mcSema __attribute__((section(".data")));

extern s32 PollSema(s32 sema);
extern s32 SignalSema(s32 sema);
extern s32 DeleteSema(s32 sema);
extern s32 sceSifCallRpc(void *cd, u32 fno, u32 mode, void *send, s32 ssize,
                         void *recv, s32 rsize, void *endfunc, void *endpara);
extern s32 sceMcOpen(s32 a, s32 b, char *c, s32 d);
extern s32 sceSifCheckStatRpc(void *cd);
extern void mcDelayThread(u16 ticks);
extern char *strncpy(char *dst, const char *src, size_t n);
extern int scePrintf(const char *fmt, ...);

/* The chunk this string opens also carries the float constants the next
 * unit needs, so it stays one INCLUDE_RODATA rather than an inline literal. */
extern const char D_0054AEE8[];
extern void sceSifWriteBackDCache(void *addr, s32 size);
extern void FlushCache(s32 mode);
extern void mceIntrReadFixAlign(void *argument);
extern void mceGetInfoApdx(void *reply);
extern void mceStorePwd(char *dest);

typedef struct mcSemaParam_t {
    s32 count;
    s32 maxCount;
    s32 initCount;
    s32 waitThreads;
    u32 attr;
    u32 option;
} mcSemaParam_t;
extern s32 CreateSema(mcSemaParam_t *param);
extern s32 WaitSema(s32 sema);
extern s32 sceMcSync(s32 mode, s32 *command, s32 *result);
extern void sceSifInitRpc(s32 mode);
extern s32 sceSifBindRpc(mcRpcCd *client, u32 service, s32 mode);

s32 sceMcInit(void)
{
    mcSemaParam_t param;
    s32 ret;
    u32 delay;

    if (mcSema < 0) {
        param.option = 0;
        param.initCount = 1;
        param.maxCount = 1;
        mcSema = CreateSema(&param);
    }
    sceMcSync(0, NULL, NULL);
    WaitSema(mcSema);
    sceSifInitRpc(0);
    for (;;) {
        if (sceSifBindRpc(&mcRpcClient, 0x80000400, 0) < 0) {
            scePrintf("bind error libmc \n");
            for (;;) {
            }
        }
        if (mcRpcClient.bound != 0) {
            break;
        }
        for (delay = 0x100000; --delay != 0;) {
        }
    }
    ret = sceSifCallRpc(&mcRpcClient, 0xFE, 0, &mcRequest, 0x30,
                        &mcReplyBuf, 0xC, NULL, NULL);
    SignalSema(mcSema);
    if (ret < 0) {
        mcRpcClient.bound = 0;
        return ret - 100;
    }
    if (mcReplyBuf.serverVersion < 0x20A) {
        scePrintf("libmc: too old release of mcserv.irx\n");
        mcRpcClient.bound = 0;
        return -120;
    }
    if (mcReplyBuf.managerVersion < 0x20E) {
        scePrintf("libmc: too old release of mcman.irx\n");
        mcRpcClient.bound = 0;
        return -121;
    }
    return mcReplyBuf.result;
}

s32 sceMcEnd(void)
{
    if (mcSema >= 0) {
        DeleteSema(mcSema);
        mcSema = -1;
    }
    return 1;
}

/* Both outputs are generic pointer slots and may alias. */
mcRpcCd *_lmcGetClientPtr(void **reply, void **command)
{
    mcReply *buffer = &mcReplyBuf;

    *reply = buffer;
    *command = &mcLastCmd;
    buffer->sema = mcSema;
    return &mcRpcClient;
}

s32 sceMcChangeThreadPriority(s32 prio)
{
    s32 ret;

    if (PollSema(mcSema) < 0) {
        return -200;
    }
    if (mcRpcClient.bound == 0) {
        SignalSema(mcSema);
        return -100;
    }
    mcRequest.w[5] = prio;
    ret = sceSifCallRpc(&mcRpcClient, 20, 1, &mcRequest, 48, &mcReplyBuf, 4, 0, 0);
    if (ret == 0) {
        mcLastCmd = 20;
    } else {
        SignalSema(mcSema);
    }
    return ret;
}

s32 sceMcGetSlotMax(s32 port)
{
    s32 ret;

    if (PollSema(mcSema) < 0) {
        return -200;
    }
    if (mcRpcClient.bound == 0) {
        SignalSema(mcSema);
        return -100;
    }
    mcRequest.w[1] = port;
    ret = sceSifCallRpc(&mcRpcClient, 21, 0, &mcRequest, 48, &mcReplyBuf, 4, 0, 0);
    if (ret != 0) {
        SignalSema(mcSema);
        return ret;
    }
    SignalSema(mcSema);
    return mcReplyBuf.result;
}

s32 sceMcOpen(s32 a, s32 b, char *c, s32 d)
{
    s32 ret;

    if (PollSema(mcSema) < 0) {
        return -200;
    }
    if (mcRpcClient.bound == 0) {
        SignalSema(mcSema);
        return -100;
    }
    if (c == NULL || *c == 0) {
        SignalSema(mcSema);
        return -210;
    }
    strncpy(mcNameRequest.name, c, 0x3FF);
    mcNameRequest.port = a;
    mcNameRequest.flag = d;
    mcNameRequest.slot = b;
    mcNameRequest.name[0x3FF] = 0;
    ret = sceSifCallRpc(&mcRpcClient, 2, 1, &mcNameRequest, 0x414, &mcReplyBuf, 4, 0, 0);
    if (ret == 0) {
        mcLastCmd = 2;
    } else {
        SignalSema(mcSema);
    }
    return ret;
}

s32 sceMcMkdir(s32 a, s32 b, char *c)
{
    s32 ret = sceMcOpen(a, b, c, 0x40);

    if (ret == 0) {
        mcLastCmd = 11;
    }
    return ret;
}

s32 sceMcClose(s32 a)
{
    s32 ret;

    if (PollSema(mcSema) < 0) {
        return -200;
    }
    if (mcRpcClient.bound == 0) {
        SignalSema(mcSema);
        return -100;
    }
    mcRequest.w[0] = a;
    ret = sceSifCallRpc(&mcRpcClient, 3, 1, &mcRequest, 48, &mcReplyBuf, 4, 0, 0);
    if (ret == 0) {
        mcLastCmd = 3;
    } else {
        SignalSema(mcSema);
    }
    return ret;
}

s32 sceMcSeek(s32 fd, s32 offset, s32 origin)
{
    s32 ret;

    if (PollSema(mcSema) < 0) {
        return -200;
    }
    if (mcRpcClient.bound == 0) {
        SignalSema(mcSema);
        return -100;
    }
    mcRequest.w[0] = fd;
    mcRequest.w[4] = offset;
    mcRequest.w[5] = origin;
    ret = sceSifCallRpc(&mcRpcClient, 4, 1, &mcRequest, 48, &mcReplyBuf, 4, 0, 0);
    if (ret == 0) {
        mcLastCmd = 4;
    } else {
        SignalSema(mcSema);
    }
    return ret;
}

void mceIntrReadFixAlign(void *argument)
{
    mcReadReplyPrefix_t *reply = EE_ADDR_AS(mcReadReplyPrefix_t *, (EE_ADDR(argument) | 0x20000000));
    u8 *destination;
    s32 i;

    if (reply->firstLength != 0) {
        destination = reply->firstDestination;
        for (i = 0; i < reply->firstLength; i++) {
            *destination++ = reply->firstData[i];
        }
    }
    if (reply->secondLength != 0) {
        destination = reply->secondDestination;
        for (i = 0; i < reply->secondLength; i++) {
            *destination++ = reply->secondData[i];
        }
    }
}

s32 sceMcRead(s32 a, void *b, s32 c)
{
    s32 ret;

    if (PollSema(mcSema) < 0) {
        return -200;
    }
    if (mcRpcClient.bound == 0) {
        SignalSema(mcSema);
        return -100;
    }
    mcRequest.w[0] = a;
    mcRequest.w[7] = EE_ADDR_S(mcEndBuf);
    mcRequest.w[6] = EE_ADDR_S(b);
    mcRequest.w[3] = c;
    sceSifWriteBackDCache(b, c);
    sceSifWriteBackDCache(mcEndBuf, 0xC0);
    ret = sceSifCallRpc(&mcRpcClient, 5, 1, &mcRequest, 48, &mcReplyBuf, 4,
                        mceIntrReadFixAlign, mcEndBuf);
    if (ret == 0) {
        mcLastCmd = 5;
    } else {
        SignalSema(mcSema);
    }
    return ret;
}

s32 sceMcWrite(s32 a, const void *b, s32 c)
{
    s32 ret;
    u32 i;

    if (PollSema(mcSema) < 0) {
        return -200;
    }
    if (mcRpcClient.bound == 0) {
        SignalSema(mcSema);
        return -100;
    }
    mcRequest.write.descriptor = a;
    if (c < 17) {
        mcRequest.write.inlineSize = c;
        mcRequest.write.bulkSize = 0;
        mcRequest.write.bulkBuffer = 0;
    } else {
        /* The IOP side wants the bulk transfer 16-byte aligned, so the leading
         * bytes up to the next boundary travel inside the request itself. */
        addr_t top = (EE_ADDR(b) - 1) & ~15;
        addr_t bot = EE_ADDR(b) - 16;
        s32 head = top - bot;

        mcRequest.write.inlineSize = head;
        mcRequest.write.bulkSize = c - head;
        mcRequest.write.bulkBuffer = (const u8 *)b + head;
    }
    for (i = 0; i < mcRequest.write.inlineSize; i++) {
        mcRequest.write.inlineData[i] = ((const u8 *)b)[i];
    }
    FlushCache(0);
    ret = sceSifCallRpc(&mcRpcClient, 6, 1, &mcRequest, 48, &mcReplyBuf, 4, 0, 0);
    if (ret == 0) {
        mcLastCmd = 6;
    } else {
        SignalSema(mcSema);
    }
    return ret;
}

extern s32 iWakeupThread(s32 thread);
extern s32 GetThreadId(void);
extern s32 SetAlarm(u16 ticks, void (*callback)(s32, s32, s32), void *argument);
extern s32 SleepThread(void);

/* SetAlarm delivers the common argument in its third callback register. */
void mcHearAlarm(s32 cause, s32 address, s32 thread)
{
    iWakeupThread(thread);
    EE_SYNC_EI();
}

void mcDelayThread(u16 ticks)
{
    SetAlarm(ticks, mcHearAlarm, EE_ADDR_AS(void *, GetThreadId()));
    SleepThread();
}

/* NON_MATCHING: 224 bytes, retail's size, with the loop's done = 0 landing in
 * the branch delay slot where retail keeps a nop. */
#ifdef NON_MATCHING
s32 sceMcSync(s32 a, s32 *b, s32 *c)
{
    s32 done;
    s32 ok;

    if (mcLastCmd == 0) {
        return -1;
    }
    done = sceSifCheckStatRpc(&mcRpcClient);
    if (a == 0 && done != 0) {
        while (1) {
            done = 0;
            if (sceSifCheckStatRpc(&mcRpcClient) == 0) {
                break;
            }
            mcDelayThread(60);
        }
    }
    ok = done == 0;
    if (b != NULL) {
        *b = mcLastCmd;
    }
    if (ok) {
        mcLastCmd = 0;
        if (c != NULL) {
            *c = mcReplyBuf.result;
        }
        SignalSema(mcSema);
    }
    return ok;
}
#else
INCLUDE_ASM(const s32, "sdk/libmc", sceMcSync);
#endif

void mceGetInfoApdx(void *response)
{
    s32 *reply = EE_ADDR_AS(s32 *, (EE_ADDR(response) | 0x20000000));

    if (mcReplyDst0) {
        *EE_ADDR_AS(s32 *, mcReplyDst0) = reply[0];
    }
    if (mcReplyDst1) {
        *EE_ADDR_AS(s32 *, mcReplyDst1) = reply[1];
    }
    if (mcReplyDst2) {
        *EE_ADDR_AS(s32 *, mcReplyDst2) = reply[0x90 / sizeof(s32)];
    }
}

s32 sceMcGetInfo(s32 a, s32 b, s32 *c, s32 *d, s32 *e)
{
    s32 ret;

    if (PollSema(mcSema) < 0) {
        return -200;
    }
    if (mcRpcClient.bound == 0) {
        SignalSema(mcSema);
        return -100;
    }
    mcRequest.w[1] = a;
    mcRequest.w[2] = b;
    mcRequest.w[7] = EE_ADDR_S(mcEndBuf);
    if (c != NULL) {
        mcRequest.w[5] = 1;
    } else {
        mcRequest.w[5] = 0;
    }
    if (d != NULL) {
        mcRequest.w[4] = 1;
    } else {
        mcRequest.w[4] = 0;
    }
    if (e != NULL) {
        mcRequest.w[3] = 1;
    } else {
        mcRequest.w[3] = 0;
    }
    mcReplyDst0 = EE_ADDR_S(c);
    mcReplyDst1 = EE_ADDR_S(d);
    mcReplyDst2 = EE_ADDR_S(e);
    sceSifWriteBackDCache(mcEndBuf, 0xC0);
    ret = sceSifCallRpc(&mcRpcClient, 1, 1, &mcRequest, 48, &mcReplyBuf, 4,
                        mceGetInfoApdx, mcEndBuf);
    if (ret == 0) {
        mcLastCmd = 1;
    } else {
        SignalSema(mcSema);
    }
    return ret;
}

s32 sceMcUdCheckNewCard(void)
{
    if (sceSifCallRpc(&mcRpcClient, 53, 0, &mcRequest, 48, &mcReplyBuf, 4, 0, 0) != 0) {
        scePrintf(D_0054AEE8);
        return -1;
    }
    return mcReplyBuf.result;
}

s32 sceMcGetDir(s32 a, s32 b, char *c, s32 d, s32 e, void *f)
{
    s32 ret;

    if (PollSema(mcSema) < 0) {
        return -200;
    }
    if (mcRpcClient.bound == 0) {
        SignalSema(mcSema);
        return -100;
    }
    if (c == NULL || *c == 0) {
        SignalSema(mcSema);
        return -210;
    }
    mcNameRequest.port = a;
    mcNameRequest.slot = b;
    mcNameRequest.flag = d;
    mcNameRequest.maxent = e;
    mcNameRequest.pwd = f;
    strncpy(mcNameRequest.name, c, 0x3FF);
    mcNameRequest.name[0x3FF] = 0;
    if (e >= 0) {
        sceSifWriteBackDCache(f, e << 6);
    }
    ret = sceSifCallRpc(&mcRpcClient, 13, 1, &mcNameRequest, 0x414, &mcReplyBuf, 4, 0, 0);
    if (ret == 0) {
        mcLastCmd = 13;
    } else {
        SignalSema(mcSema);
    }
    return ret;
}

extern u32 strlen(const char *str);
extern void *memcpy(void *dest, const void *src, size_t size);

void mceStorePwd(char *dest)
{
    if (dest != NULL) {
        const char *source = (const char *)(EE_ADDR(mcPwdBuf) | 0x20000000);
        u32 count;

        /* Native evaluates the length again on the short-string path. */
        if (strlen(source) < 0x400) {
            count = strlen(source);
        } else {
            count = 0x3FF;
        }
        memcpy(dest, (const char *)(EE_ADDR(mcPwdBuf) | 0x20000000), count);
        dest[count] = 0;
    }
}

s32 sceMcChdir(s32 a, s32 b, char *c, char *d)
{
    s32 ret;

    if (PollSema(mcSema) < 0) {
        return -200;
    }
    if (mcRpcClient.bound == 0) {
        SignalSema(mcSema);
        return -100;
    }
    if (c == NULL || *c == 0) {
        SignalSema(mcSema);
        return -210;
    }
    mcNameRequest.port = a;
    mcNameRequest.pwd = mcPwdBuf;
    mcNameRequest.slot = b;
    strncpy(mcNameRequest.name, c, 0x3FF);
    mcNameRequest.name[0x3FF] = 0;
    sceSifWriteBackDCache(mcPwdBuf, 0x400);
    ret = sceSifCallRpc(&mcRpcClient, 12, 1, &mcNameRequest, 0x414, &mcReplyBuf, 4,
                        mceStorePwd, d);
    if (ret == 0) {
        mcLastCmd = 12;
    } else {
        SignalSema(mcSema);
    }
    return ret;
}

s32 sceMcFormat(s32 port, s32 slot)
{
    s32 ret;

    if (PollSema(mcSema) < 0) {
        return -200;
    }
    if (mcRpcClient.bound == 0) {
        SignalSema(mcSema);
        return -100;
    }
    mcRequest.w[2] = slot;
    mcRequest.w[1] = port;
    ret = sceSifCallRpc(&mcRpcClient, 16, 1, &mcRequest, 48, &mcReplyBuf, 4, 0, 0);
    if (ret == 0) {
        mcLastCmd = 16;
    } else {
        SignalSema(mcSema);
    }
    return ret;
}

s32 sceMcDelete(s32 port, s32 slot, char *name)
{
    s32 ret;

    if (PollSema(mcSema) < 0) {
        return -200;
    }
    if (mcRpcClient.bound == 0) {
        SignalSema(mcSema);
        return -100;
    }
    if (name == NULL || *name == 0) {
        SignalSema(mcSema);
        return -210;
    }
    strncpy(mcNameRequest.name, name, 0x3FF);
    mcNameRequest.port = port;
    mcNameRequest.slot = slot;
    mcNameRequest.name[0x3FF] = 0;
    mcNameRequest.flag = 0;
    ret = sceSifCallRpc(&mcRpcClient, 15, 1, &mcNameRequest, 0x414, &mcReplyBuf, 4, 0, 0);
    if (ret == 0) {
        mcLastCmd = 15;
    } else {
        SignalSema(mcSema);
    }
    return ret;
}

s32 sceMcFlush(s32 port)
{
    s32 ret;

    if (PollSema(mcSema) < 0) {
        return -200;
    }
    if (mcRpcClient.bound == 0) {
        SignalSema(mcSema);
        return -100;
    }
    mcRequest.w[0] = port;
    ret = sceSifCallRpc(&mcRpcClient, 10, 1, &mcRequest, 48, &mcReplyBuf, 4, 0, 0);
    if (ret == 0) {
        mcLastCmd = 10;
    } else {
        SignalSema(mcSema);
    }
    return ret;
}

s32 sceMcSetFileInfo(s32 port, s32 slot, char *name, mcEntInfo *info, s32 flags)
{
    s32 ret;
    /* Keep the live flags separate from the request cursor in $16. This
     * localized compiler constraint reconstructs the native allocation; it
     * does not establish the historical source spelling. $17 is callee-saved. */
    FAKEMATCH_ASM_REG(s32, fileFlags, "$17") = flags;

    if (PollSema(mcSema) < 0) {
        return -200;
    }
    if (mcRpcClient.bound == 0) {
        SignalSema(mcSema);
        return -100;
    }
    if (name == NULL || *name == 0) {
        SignalSema(mcSema);
        return -210;
    }
    mcNameRequest.port = port;
    mcNameRequest.slot = slot;
    mcNameRequest.flag = fileFlags & 7;
    mcEntInfoBuf = *info;
    mcNameRequest.pwd = &mcEntInfoBuf;
    strncpy(mcNameRequest.name, name, 0x3FF);
    mcNameRequest.name[0x3FF] = 0;
    FlushCache(0);
    ret = sceSifCallRpc(&mcRpcClient, 14, 1, &mcNameRequest, 0x414, &mcReplyBuf, 4, 0, 0);
    if (ret == 0) {
        mcLastCmd = 14;
    } else {
        SignalSema(mcSema);
    }
    return ret;
}

s32 sceMcRename(s32 port, s32 slot, char *name, char *newname)
{
    s32 ret;

    if (PollSema(mcSema) < 0) {
        return -200;
    }
    if (mcRpcClient.bound == 0) {
        SignalSema(mcSema);
        return -100;
    }
    if (name == NULL || *name == 0 || newname == NULL) {
        SignalSema(mcSema);
        return -210;
    }
    mcNameRequest.port = port;
    mcNameRequest.slot = slot;
    mcNameRequest.flag = 16;
    strncpy(mcNameRequest.name, name, 0x3FF);
    mcNameRequest.name[0x3FF] = 0;
    strncpy(mcEntInfoBuf.name, newname, 0x20);
    mcEntInfoBuf.name[0x1F] = 0;
    mcNameRequest.pwd = &mcEntInfoBuf;
    FlushCache(0);
    ret = sceSifCallRpc(&mcRpcClient, 14, 1, &mcNameRequest, 0x414, &mcReplyBuf, 4, 0, 0);
    if (ret == 0) {
        mcLastCmd = 19;
    } else {
        SignalSema(mcSema);
    }
    return ret;
}

s32 sceMcUnformat(s32 port, s32 slot)
{
    s32 ret;

    if (PollSema(mcSema) < 0) {
        return -200;
    }
    if (mcRpcClient.bound == 0) {
        SignalSema(mcSema);
        return -100;
    }
    mcRequest.w[2] = slot;
    mcRequest.w[1] = port;
    ret = sceSifCallRpc(&mcRpcClient, 17, 1, &mcRequest, 48, &mcReplyBuf, 4, 0, 0);
    if (ret == 0) {
        mcLastCmd = 17;
    } else {
        SignalSema(mcSema);
    }
    return ret;
}

s32 sceMcGetEntSpace(s32 port, s32 slot, char *name)
{
    s32 ret;

    if (PollSema(mcSema) < 0) {
        return -200;
    }
    if (mcRpcClient.bound == 0) {
        SignalSema(mcSema);
        return -100;
    }
    if (name == NULL || *name == 0) {
        SignalSema(mcSema);
        return -210;
    }
    mcNameRequest.port = port;
    mcNameRequest.slot = slot;
    strncpy(mcNameRequest.name, name, 0x3FF);
    mcNameRequest.name[0x3FF] = 0;
    ret = sceSifCallRpc(&mcRpcClient, 18, 1, &mcNameRequest, 0x414, &mcReplyBuf, 4, 0, 0);
    if (ret == 0) {
        mcLastCmd = 18;
    } else {
        SignalSema(mcSema);
    }
    return ret;
}

INCLUDE_RODATA(const s32, "sdk/libmc", D_0054AEE8);

