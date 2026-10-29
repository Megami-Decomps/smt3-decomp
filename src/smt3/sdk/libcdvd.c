#include "common.h"
#include "ps2asm.h"

extern s32 D_0052BA70 __attribute__((section(".data")));
extern s32 D_00598C40 __attribute__((section(".data")));
extern u8 D_00598ED8[] __attribute__((section(".data")));

extern s32 D_0052A010 __attribute__((section(".data")));
extern u8 D_0052BA48[0x40] __attribute__((section(".data")));

extern s32 D_0052A030 __attribute__((section(".data")));
extern s32 D_0052A02C __attribute__((section(".data")));
extern u8 D_0052B1D0[0x40] __attribute__((section(".data")));
extern s32 D_0052B640 __attribute__((section(".data")));
extern s32 D_0052B200[4] __attribute__((section(".data")));

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

typedef void (*cdvdPowerCallback_t)(void *argument);
extern cdvdPowerCallback_t D_00598C44 __attribute__((section(".data")));
extern void *D_00598C48 __attribute__((section(".data")));
extern s32 D_0052A024 __attribute__((section(".data")));
extern s32 D_0052A03C __attribute__((section(".data")));
extern void sceSifAddCmdHandler(s32 id, void (*handler)(void *, void *), void *argument);
void _sceCd_Poff_Intr(void *packet, void *argument);
s32 PowerOffCB(void);

/* The reply word is read back through the uncached mirror, since the IOP
 * wrote it behind the data cache. */
#define UNCACHED(a) ((s32 *)((u32)(a) | 0x20000000))

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
extern s32 sceCdLayerSearchFile(u32 file, const char *name, s32 layer);

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
    SetAlarm(ticks, gcc2_compiled__6, (void *)sema);
    WaitSema(sema);
    DeleteSema(sema);
}

s32 sceCdCallback(s32 func)
{
    s32 intr;
    s32 old;

    if (sceCdSync(1) != 0) {
        return 0;
    }
    intr = DIntr();
    old = D_00598C40;
    D_00598C40 = func;
    if (intr) {
        EIntr();
    }
    return old;
}

INCLUDE_ASM(const s32, "sdk/libcdvd", _sceCd_cd_callback);

INCLUDE_ASM(const s32, "sdk/libcdvd", _Cdvd_cbLoop);

INCLUDE_ASM(const s32, "sdk/libcdvd", sceCdInitEeCB);

INCLUDE_ASM(const s32, "sdk/libcdvd", _sceCd_cd_read_intr);

INCLUDE_ASM(const s32, "sdk/libcdvd", cmd_sem_init);

INCLUDE_ASM(const s32, "sdk/libcdvd", cdvd_exit);

cdvdPowerCallback_t sceCdPOffCallback(cdvdPowerCallback_t callback, void *argument)
{
    cdvdPowerCallback_t old;
    s32 intr;

    if (D_0052A03C < 0) {
        PowerOffCB();
    }
    intr = DIntr();
    old = D_00598C44;
    D_00598C48 = argument;
    D_00598C44 = callback;
    if (intr) {
        EIntr();
    }
    return old;
}

void _sceCd_Poff_Intr(void *packet, void *argument)
{
    if (D_00598C44 != NULL && D_0052A024 == 0) {
        D_00598C44(D_00598C48);
    }
}

s32 PowerOffCB(void)
{
    s32 intr;

    D_0052A024 = 1;
    intr = DIntr();
    sceSifAddCmdHandler(0x80000012, _sceCd_Poff_Intr, NULL);
    if (intr) {
        EIntr();
    }
    D_0052A024 = 0;
    D_0052A03C = 1;
    return 1;
}

INCLUDE_ASM(const s32, "sdk/libcdvd", sceCdLayerSearchFile);

s32 sceCdSearchFile(u32 file, char *name)
{
    return sceCdLayerSearchFile(file, name, 0);
}

INCLUDE_ASM(const s32, "sdk/libcdvd", _sceCd_ncmd_prechk);

extern s32 _sceCd_ncmd_prechk(s32 cmd);
extern s32 D_0052A080 __attribute__((section(".data")));

s32 sceCdNcmdDiskReady(void)
{
    s32 ret;

    if (!_sceCd_ncmd_prechk(2)) {
        return 0;
    }
    if (sceSifCallRpc(D_0052B1D0, 14, 0, 0, 0, &D_0052A080, 4, 0, 0) < 0) {
        SignalSema(cdvdNcmdSema);
        return 0;
    }
    ret = UNCACHED(&D_0052A080)[0];
    SignalSema(cdvdNcmdSema);
    return ret;
}

INCLUDE_ASM(const s32, "sdk/libcdvd", sceCdSync);

s32 sceCdSyncS(s32 mode)
{
    if (mode == 0) {
        if (D_0052A010 > 0) {
            scePrintf("S cmd wait\n");
        }
        while (1) {
            if (sceSifCheckStatRpc(D_0052BA48) == 0) {
                break;
            }
            sceCdDelayThread(60);
        }
        return 0;
    }
    return sceSifCheckStatRpc(D_0052BA48);
}

INCLUDE_ASM(const s32, "sdk/libcdvd", _sceCd_scmd_prechk);

INCLUDE_ASM(const s32, "sdk/libcdvd", sceCdInit);

INCLUDE_ASM(const s32, "sdk/libcdvd", sceCdDiskReady);

s32 sceCdMmode(s32 media)
{
    s32 *arg = &D_0052B640;
    s32 ret;

    if (!_sceCd_scmd_prechk(34)) {
        return 0;
    }
    D_0052B640 = media;
    sceSifWriteBackDCache(arg, 4);
    if (sceSifCallRpc(D_0052BA48, 34, 0, arg, 4, D_0052B200, 4, 0, 0) < 0) {
        SignalSema(D_0052A02C);
        return 0;
    }
    ret = UNCACHED(D_0052B200)[0];
    SignalSema(D_0052A02C);
    return ret;
}

s32 sceCdGetDiskType(void)
{
    s32 ret;

    if (!_sceCd_scmd_prechk(1)) {
        return 0;
    }
    if (sceSifCallRpc(D_0052BA48, 3, 0, 0, 0, D_0052B200, 4, 0, 0) < 0) {
        SignalSema(D_0052A02C);
        return 0;
    }
    ret = UNCACHED(D_0052B200)[0];
    SignalSema(D_0052A02C);
    return ret;
}

s32 sceCdStatus(void)
{
    s32 ret;

    if (!_sceCd_scmd_prechk(2)) {
        return -1;
    }
    if (sceSifCallRpc(D_0052BA48, 12, 0, 0, 0, D_0052B200, 4, 0, 0) < 0) {
        SignalSema(D_0052A02C);
        return -1;
    }
    ret = UNCACHED(D_0052B200)[0];
    SignalSema(D_0052A02C);
    if (D_0052A010 >= 2) {
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
    if (sceSifCallRpc(D_0052BA48, 33, 0, 0, 0, D_0052B200, 8, 0, 0) < 0) {
        SignalSema(D_0052A02C);
        return 0;
    }
    *result = UNCACHED(D_0052B200 + 1)[0];
    ret = UNCACHED(D_0052B200)[0];
    SignalSema(D_0052A02C);
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

static const char D_0054AC78[32] = "Libcdvd call Clock read 1\n";
static const char D_0054AC98[32] = "Libcdvd call Clock read 2\n";

s32 sceCdReadClock(sdfCdClock_t *clock)
{
    s32 result;

    if (!_sceCd_scmd_prechk(0xF)) {
        return 0;
    }
    if (D_0052A010 > 0) {
        scePrintf(D_0054AC78);
    }
    if (sceSifCallRpc(D_0052BA48, 1, 0, NULL, 0,
                      D_0052B200, 16, NULL, NULL) < 0) {
        SignalSema(D_0052A02C);
        return 0;
    }
    *clock = *(sdfCdClock_t *)UNCACHED(&D_0052B200[1]);
    if (D_0052A010 > 0) {
        scePrintf(D_0054AC98);
    }
    result = UNCACHED(D_0052B200)[0];
    SignalSema(D_0052A02C);
    return result;
}


s32 sceCdStInit(u32 bufmax, u32 bankmax, void *buf)
{
    D_0052BA70 = 0;
    return sceCdStream(bufmax, bankmax, buf, 5, D_00598ED8);
}

s32 sceCdStStart(u32 lsn, void *mode)
{
    D_0052BA70 = 1;
    return sceCdStream(lsn, 0, 0, 1, mode);
}

s32 sceCdStSeekF(u32 lsn)
{
    return sceCdStream(lsn, 0, 0, 9, D_00598ED8);
}

s32 sceCdStSeek(u32 lsn)
{
    return sceCdStream(lsn, 0, 0, 4, D_00598ED8);
}

s32 sceCdStStop(void)
{
    D_0052BA70 = 0;
    return sceCdStream(0, 0, 0, 3, D_00598ED8);
}

INCLUDE_ASM(const s32, "sdk/libcdvd", sceCdStRead);

s32 sceCdStPause(void)
{
    D_0052BA70 = 0;
    if (D_0052A010 > 0) {
        scePrintf("sceCdStPause call\n");
    }
    return sceCdStream(0, 0, 0, 7, D_00598ED8);
}

s32 sceCdStResume(void)
{
    D_0052BA70 = 1;
    if (D_0052A010 > 0) {
        scePrintf("sceCdStResume call\n");
    }
    return sceCdStream(0, 0, 0, 8, D_00598ED8);
}

s32 sceCdStStat(void)
{
    if (D_0052A010 > 0) {
        scePrintf("sceCdStStat call\n");
    }
    return sceCdStream(0, 0, 0, 6, D_00598ED8);
}

INCLUDE_ASM(const s32, "sdk/libcdvd", sceCdStream);
