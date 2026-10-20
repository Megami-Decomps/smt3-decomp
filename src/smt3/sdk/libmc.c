#include "common.h"

/* The sif client data of the mcserv module; +0x24 is non-zero once bind
 * succeeded, which every call tests before using the RPC. */
typedef struct mcRpcCd {
    u32 pad00[9];
    s32 bound;
} mcRpcCd;

extern mcRpcCd D_00599740 __attribute__((section(".data")));
/* The 0x30-byte request every short command shares. The argument words mean
 * different things per command, so they stay a block; data is the inline
 * payload sceMcWrite's leading bytes travel in. */
typedef struct mcReq {
    s32 w[8];
    u8 data[16];
} mcReq;

extern mcReq D_005997C0 __attribute__((section(".data")));

/* The 0x40-byte entry block that sits just before the request: four words of
 * directory entry info and a 0x20-byte name. */
typedef struct mcEntInfo {
    u32 info[8];
    char name[0x20];
} mcEntInfo;

extern mcEntInfo D_00599780 __attribute__((section(".data")));

/* The name-carrying request: the same header, then a 0x400-byte path. */
typedef struct mcNameReq {
    s32 port;
    s32 slot;
    s32 flag;
    s32 maxent;
    void *pwd;
    char name[0x400];
} mcNameReq;

extern mcNameReq D_005997F0 __attribute__((section(".data")));

/* The 0xC0-byte block the RPC end functions read back, and the 0x400-byte
 * working directory sceMcChdir hands to mceStorePwd. */
extern u8 D_00599C40[0xC0] __attribute__((section(".data")));
extern u8 D_00599D00[0x400] __attribute__((section(".data")));

extern s32 D_00599768 __attribute__((section(".data")));
extern s32 D_0059976C __attribute__((section(".data")));
extern s32 D_00599770 __attribute__((section(".data")));
extern s32 D_0059AD00 __attribute__((section(".data")));
extern s32 D_0052BB20 __attribute__((section(".data")));
extern s32 D_0052BB24 __attribute__((section(".data")));

extern s32 PollSema(s32 sema);
extern s32 SignalSema(s32 sema);
extern s32 DeleteSema(s32 sema);
extern s32 sceSifCallRpc(void *cd, u32 fno, u32 mode, void *send, s32 ssize,
                         void *recv, s32 rsize, void *endfunc, void *endpara);
extern s32 sceMcOpen(s32 port, s32 slot, char *name, s32 flag);
extern s32 sceSifCheckStatRpc(void *cd);
extern void mcDelayThread(s32 ticks);
extern char *strncpy(char *dst, const char *src, u32 n);
extern int scePrintf(const char *fmt, ...);

/* The chunk this string opens also carries the float constants the next
 * unit needs, so it stays one INCLUDE_RODATA rather than an inline literal. */
extern const char D_0054AEE8[];
extern void sceSifWriteBackDCache(void *addr, s32 size);
extern void FlushCache(s32 mode);
extern void mceIntrReadFixAlign(void);
extern void mceGetInfoApdx(void);
extern void mceStorePwd(void);

INCLUDE_ASM(const s32, "sdk/libmc", sceMcInit);

s32 sceMcEnd(void)
{
    if (D_0052BB24 >= 0) {
        DeleteSema(D_0052BB24);
        D_0052BB24 = -1;
    }
    return 1;
}

INCLUDE_ASM(const s32, "sdk/libmc", _lmcGetClientPtr);

s32 sceMcChangeThreadPriority(s32 prio)
{
    s32 ret;

    if (PollSema(D_0052BB24) < 0) {
        return -200;
    }
    if (D_00599740.bound == 0) {
        SignalSema(D_0052BB24);
        return -100;
    }
    D_005997C0.w[5] = prio;
    ret = sceSifCallRpc(&D_00599740, 20, 1, &D_005997C0, 48, &D_0059AD00, 4, 0, 0);
    if (ret == 0) {
        D_0052BB20 = 20;
    } else {
        SignalSema(D_0052BB24);
    }
    return ret;
}

s32 sceMcGetSlotMax(s32 port)
{
    s32 ret;

    if (PollSema(D_0052BB24) < 0) {
        return -200;
    }
    if (D_00599740.bound == 0) {
        SignalSema(D_0052BB24);
        return -100;
    }
    D_005997C0.w[1] = port;
    ret = sceSifCallRpc(&D_00599740, 21, 0, &D_005997C0, 48, &D_0059AD00, 4, 0, 0);
    if (ret != 0) {
        SignalSema(D_0052BB24);
        return ret;
    }
    SignalSema(D_0052BB24);
    return D_0059AD00;
}

s32 sceMcOpen(s32 port, s32 slot, char *name, s32 flag)
{
    s32 ret;

    if (PollSema(D_0052BB24) < 0) {
        return -200;
    }
    if (D_00599740.bound == 0) {
        SignalSema(D_0052BB24);
        return -100;
    }
    if (name == NULL || *name == 0) {
        SignalSema(D_0052BB24);
        return -210;
    }
    strncpy(D_005997F0.name, name, 0x3FF);
    D_005997F0.port = port;
    D_005997F0.flag = flag;
    D_005997F0.slot = slot;
    D_005997F0.name[0x3FF] = 0;
    ret = sceSifCallRpc(&D_00599740, 2, 1, &D_005997F0, 0x414, &D_0059AD00, 4, 0, 0);
    if (ret == 0) {
        D_0052BB20 = 2;
    } else {
        SignalSema(D_0052BB24);
    }
    return ret;
}

s32 sceMcMkdir(s32 port, s32 slot, char *name)
{
    s32 ret = sceMcOpen(port, slot, name, 0x40);

    if (ret == 0) {
        D_0052BB20 = 11;
    }
    return ret;
}

s32 sceMcClose(s32 fd)
{
    s32 ret;

    if (PollSema(D_0052BB24) < 0) {
        return -200;
    }
    if (D_00599740.bound == 0) {
        SignalSema(D_0052BB24);
        return -100;
    }
    D_005997C0.w[0] = fd;
    ret = sceSifCallRpc(&D_00599740, 3, 1, &D_005997C0, 48, &D_0059AD00, 4, 0, 0);
    if (ret == 0) {
        D_0052BB20 = 3;
    } else {
        SignalSema(D_0052BB24);
    }
    return ret;
}

s32 sceMcSeek(s32 fd, s32 offset, s32 origin)
{
    s32 ret;

    if (PollSema(D_0052BB24) < 0) {
        return -200;
    }
    if (D_00599740.bound == 0) {
        SignalSema(D_0052BB24);
        return -100;
    }
    D_005997C0.w[0] = fd;
    D_005997C0.w[4] = offset;
    D_005997C0.w[5] = origin;
    ret = sceSifCallRpc(&D_00599740, 4, 1, &D_005997C0, 48, &D_0059AD00, 4, 0, 0);
    if (ret == 0) {
        D_0052BB20 = 4;
    } else {
        SignalSema(D_0052BB24);
    }
    return ret;
}

INCLUDE_ASM(const s32, "sdk/libmc", mceIntrReadFixAlign);

s32 sceMcRead(s32 fd, void *buf, s32 size)
{
    s32 ret;

    if (PollSema(D_0052BB24) < 0) {
        return -200;
    }
    if (D_00599740.bound == 0) {
        SignalSema(D_0052BB24);
        return -100;
    }
    D_005997C0.w[0] = fd;
    D_005997C0.w[7] = (s32)D_00599C40;
    D_005997C0.w[6] = (s32)buf;
    D_005997C0.w[3] = size;
    sceSifWriteBackDCache(buf, size);
    sceSifWriteBackDCache(D_00599C40, 0xC0);
    ret = sceSifCallRpc(&D_00599740, 5, 1, &D_005997C0, 48, &D_0059AD00, 4,
                        mceIntrReadFixAlign, D_00599C40);
    if (ret == 0) {
        D_0052BB20 = 5;
    } else {
        SignalSema(D_0052BB24);
    }
    return ret;
}

/* NON_MATCHING: 380 bytes against retail's 384; the store order inside the
 * two size branches and the saved-register pair holding the descriptor are
 * the only differences left. */
#ifdef NON_MATCHING
s32 sceMcWrite(s32 fd, void *buf, s32 size)
{
    s32 ret;
    u32 i;

    if (PollSema(D_0052BB24) < 0) {
        return -200;
    }
    if (D_00599740.bound == 0) {
        SignalSema(D_0052BB24);
        return -100;
    }
    D_005997C0.w[0] = fd;
    if (size < 17) {
        D_005997C0.w[5] = size;
        D_005997C0.w[6] = 0;
        D_005997C0.w[3] = 0;
    } else {
        /* The IOP side wants the bulk transfer 16-byte aligned, so the leading
         * bytes up to the next boundary travel inside the request itself. */
        u32 top = ((u32)buf - 1) & ~15;
        u32 bot = (u32)buf - 16;
        s32 head = top - bot;

        D_005997C0.w[6] = (s32)buf + head;
        D_005997C0.w[3] = size - head;
        D_005997C0.w[5] = head;
    }
    for (i = 0; i < D_005997C0.w[5]; i++) {
        D_005997C0.data[i] = ((u8 *)buf)[i];
    }
    FlushCache(0);
    ret = sceSifCallRpc(&D_00599740, 6, 1, &D_005997C0, 48, &D_0059AD00, 4, 0, 0);
    if (ret == 0) {
        D_0052BB20 = 6;
    } else {
        SignalSema(D_0052BB24);
    }
    return ret;
}
#else
INCLUDE_ASM(const s32, "sdk/libmc", sceMcWrite);
#endif

INCLUDE_ASM(const s32, "sdk/libmc", mcHearAlarm);

INCLUDE_ASM(const s32, "sdk/libmc", mcDelayThread);

/* NON_MATCHING: 224 bytes, retail's size, with the loop's done = 0 landing in
 * the branch delay slot where retail keeps a nop. */
#ifdef NON_MATCHING
s32 sceMcSync(s32 mode, s32 *cmd, s32 *result)
{
    s32 done;
    s32 ok;

    if (D_0052BB20 == 0) {
        return -1;
    }
    done = sceSifCheckStatRpc(&D_00599740);
    if (mode == 0 && done != 0) {
        while (1) {
            done = 0;
            if (sceSifCheckStatRpc(&D_00599740) == 0) {
                break;
            }
            mcDelayThread(60);
        }
    }
    ok = done == 0;
    if (cmd != NULL) {
        *cmd = D_0052BB20;
    }
    if (ok) {
        D_0052BB20 = 0;
        if (result != NULL) {
            *result = D_0059AD00;
        }
        SignalSema(D_0052BB24);
    }
    return ok;
}
#else
INCLUDE_ASM(const s32, "sdk/libmc", sceMcSync);
#endif

INCLUDE_ASM(const s32, "sdk/libmc", mceGetInfoApdx);

s32 sceMcGetInfo(s32 port, s32 slot, s32 *type, s32 *free, s32 *format)
{
    s32 ret;

    if (PollSema(D_0052BB24) < 0) {
        return -200;
    }
    if (D_00599740.bound == 0) {
        SignalSema(D_0052BB24);
        return -100;
    }
    D_005997C0.w[1] = port;
    D_005997C0.w[2] = slot;
    D_005997C0.w[7] = (s32)D_00599C40;
    if (type != NULL) {
        D_005997C0.w[5] = 1;
    } else {
        D_005997C0.w[5] = 0;
    }
    if (free != NULL) {
        D_005997C0.w[4] = 1;
    } else {
        D_005997C0.w[4] = 0;
    }
    if (format != NULL) {
        D_005997C0.w[3] = 1;
    } else {
        D_005997C0.w[3] = 0;
    }
    D_00599768 = (s32)type;
    D_0059976C = (s32)free;
    D_00599770 = (s32)format;
    sceSifWriteBackDCache(D_00599C40, 0xC0);
    ret = sceSifCallRpc(&D_00599740, 1, 1, &D_005997C0, 48, &D_0059AD00, 4,
                        mceGetInfoApdx, D_00599C40);
    if (ret == 0) {
        D_0052BB20 = 1;
    } else {
        SignalSema(D_0052BB24);
    }
    return ret;
}

s32 sceMcUdCheckNewCard(void)
{
    if (sceSifCallRpc(&D_00599740, 53, 0, &D_005997C0, 48, &D_0059AD00, 4, 0, 0) != 0) {
        scePrintf(D_0054AEE8);
        return -1;
    }
    return D_0059AD00;
}

s32 sceMcGetDir(s32 port, s32 slot, char *name, s32 mode, s32 maxent, void *table)
{
    s32 ret;

    if (PollSema(D_0052BB24) < 0) {
        return -200;
    }
    if (D_00599740.bound == 0) {
        SignalSema(D_0052BB24);
        return -100;
    }
    if (name == NULL || *name == 0) {
        SignalSema(D_0052BB24);
        return -210;
    }
    D_005997F0.port = port;
    D_005997F0.slot = slot;
    D_005997F0.flag = mode;
    D_005997F0.maxent = maxent;
    D_005997F0.pwd = table;
    strncpy(D_005997F0.name, name, 0x3FF);
    D_005997F0.name[0x3FF] = 0;
    if (maxent >= 0) {
        sceSifWriteBackDCache(table, maxent << 6);
    }
    ret = sceSifCallRpc(&D_00599740, 13, 1, &D_005997F0, 0x414, &D_0059AD00, 4, 0, 0);
    if (ret == 0) {
        D_0052BB20 = 13;
    } else {
        SignalSema(D_0052BB24);
    }
    return ret;
}

INCLUDE_ASM(const s32, "sdk/libmc", mceStorePwd);

s32 sceMcChdir(s32 port, s32 slot, char *name, char *pwd)
{
    s32 ret;

    if (PollSema(D_0052BB24) < 0) {
        return -200;
    }
    if (D_00599740.bound == 0) {
        SignalSema(D_0052BB24);
        return -100;
    }
    if (name == NULL || *name == 0) {
        SignalSema(D_0052BB24);
        return -210;
    }
    D_005997F0.port = port;
    D_005997F0.pwd = D_00599D00;
    D_005997F0.slot = slot;
    strncpy(D_005997F0.name, name, 0x3FF);
    D_005997F0.name[0x3FF] = 0;
    sceSifWriteBackDCache(D_00599D00, 0x400);
    ret = sceSifCallRpc(&D_00599740, 12, 1, &D_005997F0, 0x414, &D_0059AD00, 4,
                        mceStorePwd, pwd);
    if (ret == 0) {
        D_0052BB20 = 12;
    } else {
        SignalSema(D_0052BB24);
    }
    return ret;
}

s32 sceMcFormat(s32 port, s32 slot)
{
    s32 ret;

    if (PollSema(D_0052BB24) < 0) {
        return -200;
    }
    if (D_00599740.bound == 0) {
        SignalSema(D_0052BB24);
        return -100;
    }
    D_005997C0.w[2] = slot;
    D_005997C0.w[1] = port;
    ret = sceSifCallRpc(&D_00599740, 16, 1, &D_005997C0, 48, &D_0059AD00, 4, 0, 0);
    if (ret == 0) {
        D_0052BB20 = 16;
    } else {
        SignalSema(D_0052BB24);
    }
    return ret;
}

s32 sceMcDelete(s32 port, s32 slot, char *name)
{
    s32 ret;

    if (PollSema(D_0052BB24) < 0) {
        return -200;
    }
    if (D_00599740.bound == 0) {
        SignalSema(D_0052BB24);
        return -100;
    }
    if (name == NULL || *name == 0) {
        SignalSema(D_0052BB24);
        return -210;
    }
    strncpy(D_005997F0.name, name, 0x3FF);
    D_005997F0.port = port;
    D_005997F0.slot = slot;
    D_005997F0.name[0x3FF] = 0;
    D_005997F0.flag = 0;
    ret = sceSifCallRpc(&D_00599740, 15, 1, &D_005997F0, 0x414, &D_0059AD00, 4, 0, 0);
    if (ret == 0) {
        D_0052BB20 = 15;
    } else {
        SignalSema(D_0052BB24);
    }
    return ret;
}

s32 sceMcFlush(s32 port)
{
    s32 ret;

    if (PollSema(D_0052BB24) < 0) {
        return -200;
    }
    if (D_00599740.bound == 0) {
        SignalSema(D_0052BB24);
        return -100;
    }
    D_005997C0.w[0] = port;
    ret = sceSifCallRpc(&D_00599740, 10, 1, &D_005997C0, 48, &D_0059AD00, 4, 0, 0);
    if (ret == 0) {
        D_0052BB20 = 10;
    } else {
        SignalSema(D_0052BB24);
    }
    return ret;
}

/* NON_MATCHING: 464 bytes against retail's 472; retail spends one more
 * saved register, so its frame is 0xA0 where this draft needs 0x90. */
#ifdef NON_MATCHING
s32 sceMcSetFileInfo(s32 port, s32 slot, char *name, mcEntInfo *info, s32 flags)
{
    s32 ret;

    if (PollSema(D_0052BB24) < 0) {
        return -200;
    }
    if (D_00599740.bound == 0) {
        SignalSema(D_0052BB24);
        return -100;
    }
    if (name == NULL || *name == 0) {
        SignalSema(D_0052BB24);
        return -210;
    }
    D_005997F0.port = port;
    D_005997F0.slot = slot;
    D_005997F0.flag = flags & 7;
    D_00599780 = *info;
    D_005997F0.pwd = &D_00599780;
    strncpy(D_005997F0.name, name, 0x3FF);
    D_005997F0.name[0x3FF] = 0;
    FlushCache(0);
    ret = sceSifCallRpc(&D_00599740, 14, 1, &D_005997F0, 0x414, &D_0059AD00, 4, 0, 0);
    if (ret == 0) {
        D_0052BB20 = 14;
    } else {
        SignalSema(D_0052BB24);
    }
    return ret;
}
#else
INCLUDE_ASM(const s32, "sdk/libmc", sceMcSetFileInfo);
#endif

s32 sceMcRename(s32 port, s32 slot, char *name, char *newname)
{
    s32 ret;

    if (PollSema(D_0052BB24) < 0) {
        return -200;
    }
    if (D_00599740.bound == 0) {
        SignalSema(D_0052BB24);
        return -100;
    }
    if (name == NULL || *name == 0 || newname == NULL) {
        SignalSema(D_0052BB24);
        return -210;
    }
    D_005997F0.port = port;
    D_005997F0.slot = slot;
    D_005997F0.flag = 16;
    strncpy(D_005997F0.name, name, 0x3FF);
    D_005997F0.name[0x3FF] = 0;
    strncpy(D_00599780.name, newname, 0x20);
    D_00599780.name[0x1F] = 0;
    D_005997F0.pwd = &D_00599780;
    FlushCache(0);
    ret = sceSifCallRpc(&D_00599740, 14, 1, &D_005997F0, 0x414, &D_0059AD00, 4, 0, 0);
    if (ret == 0) {
        D_0052BB20 = 19;
    } else {
        SignalSema(D_0052BB24);
    }
    return ret;
}

s32 sceMcUnformat(s32 port, s32 slot)
{
    s32 ret;

    if (PollSema(D_0052BB24) < 0) {
        return -200;
    }
    if (D_00599740.bound == 0) {
        SignalSema(D_0052BB24);
        return -100;
    }
    D_005997C0.w[2] = slot;
    D_005997C0.w[1] = port;
    ret = sceSifCallRpc(&D_00599740, 17, 1, &D_005997C0, 48, &D_0059AD00, 4, 0, 0);
    if (ret == 0) {
        D_0052BB20 = 17;
    } else {
        SignalSema(D_0052BB24);
    }
    return ret;
}

s32 sceMcGetEntSpace(s32 port, s32 slot, char *name)
{
    s32 ret;

    if (PollSema(D_0052BB24) < 0) {
        return -200;
    }
    if (D_00599740.bound == 0) {
        SignalSema(D_0052BB24);
        return -100;
    }
    if (name == NULL || *name == 0) {
        SignalSema(D_0052BB24);
        return -210;
    }
    D_005997F0.port = port;
    D_005997F0.slot = slot;
    strncpy(D_005997F0.name, name, 0x3FF);
    D_005997F0.name[0x3FF] = 0;
    ret = sceSifCallRpc(&D_00599740, 18, 1, &D_005997F0, 0x414, &D_0059AD00, 4, 0, 0);
    if (ret == 0) {
        D_0052BB20 = 18;
    } else {
        SignalSema(D_0052BB24);
    }
    return ret;
}

INCLUDE_RODATA(const s32, "sdk/libmc", D_0054AEE8);

