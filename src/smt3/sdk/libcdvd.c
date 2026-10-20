#include "common.h"

extern s32 D_0052BA70 __attribute__((section(".data")));
extern s32 D_00598C40 __attribute__((section(".data")));
extern u8 D_00598ED8[] __attribute__((section(".data")));

extern s32 D_0052A010 __attribute__((section(".data")));
extern void *D_0052BA48 __attribute__((section(".data")));

extern s32 DIntr(void);
extern int scePrintf(const char *fmt, ...);
extern s32 sceSifCheckStatRpc(void *cd);
extern void sceCdDelayThread(u16 ticks);
extern s32 EIntr(void);
extern s32 sceCdSync(s32 mode);
extern s32 sceCdStream(u32 lsn, u32 sectors, void *buf, s32 cmd, void *arg);
extern s32 sceCdLayerSearchFile(u32 file, const char *name, s32 layer);

INCLUDE_ASM(const s32, "sdk/libcdvd", gcc2_compiled__6);

INCLUDE_ASM(const s32, "sdk/libcdvd", sceCdDelayThread);

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

INCLUDE_ASM(const s32, "sdk/libcdvd", sceCdPOffCallback);

INCLUDE_ASM(const s32, "sdk/libcdvd", _sceCd_Poff_Intr);

INCLUDE_ASM(const s32, "sdk/libcdvd", PowerOffCB);

INCLUDE_ASM(const s32, "sdk/libcdvd", sceCdLayerSearchFile);

s32 sceCdSearchFile(u32 file, char *name)
{
    return sceCdLayerSearchFile(file, name, 0);
}

INCLUDE_ASM(const s32, "sdk/libcdvd", _sceCd_ncmd_prechk);

INCLUDE_ASM(const s32, "sdk/libcdvd", sceCdNcmdDiskReady);

INCLUDE_ASM(const s32, "sdk/libcdvd", sceCdSync);

/* NON_MATCHING: 112 bytes, retail's size; the wait loop's zero return is
 * materialised a scheduling slot away from where retail puts it. */
#ifdef NON_MATCHING
s32 sceCdSyncS(s32 mode)
{
    if (mode) {
        return sceSifCheckStatRpc(D_0052BA48);
    }
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
#else
INCLUDE_ASM(const s32, "sdk/libcdvd", sceCdSyncS);
#endif

INCLUDE_ASM(const s32, "sdk/libcdvd", _sceCd_scmd_prechk);

INCLUDE_ASM(const s32, "sdk/libcdvd", sceCdInit);

INCLUDE_ASM(const s32, "sdk/libcdvd", sceCdDiskReady);

INCLUDE_ASM(const s32, "sdk/libcdvd", sceCdMmode);

INCLUDE_ASM(const s32, "sdk/libcdvd", sceCdGetDiskType);

INCLUDE_ASM(const s32, "sdk/libcdvd", sceCdStatus);

INCLUDE_ASM(const s32, "sdk/libcdvd", sceCdPowerOff);

INCLUDE_ASM(const s32, "sdk/libcdvd", sceCdReadClock);

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
