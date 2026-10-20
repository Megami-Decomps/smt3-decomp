#include "common.h"
#include "ps2asm.h"

extern void StartThread();

extern void SleepThread(void);

extern s32 sceSifLoadModule(const char *, s32, const char *);

extern char *strcpy(char *, const char *);

extern char *strcat(char *, const char *);

extern void sceMcInit(void);

extern s32 WaitSema(s32);

extern void FlushCache(s32);

extern s32 SignalSema(s32);

extern s32 sceSifSetDma(void *, s32);

extern s32 sceSifDmaStat(s32);

extern s32 GetThreadId(void);

/* One row per administrative mode. The entry receives the restore flag and
 * mode data; destroy and cleanup return scheduler results. Each row is 12 bytes
 * on the EE. */
typedef struct Dds3AdminModeDispatch {
    void (*entry)(s32 restoring, void *data);
    s32 (*destroy)(void);
    s32 (*cleanup)(void);
} Dds3AdminModeDispatch;

extern void *memcpy(void *, const void *, unsigned int);

typedef struct _reent _reent;

typedef __builtin_va_list va_list;

typedef struct __sbuf {
    u8 *_base;
    s32 _size;
} __sbuf;

typedef struct __sFILE {
    u8 *_p;
    s32 _r;
    s32 _w;
    s16 _flags;
    s16 _file;
    __sbuf _bf;
    s32 _lbfsize;
    void *_cookie;
    s32 (*_read)(void *cookie, char *buf, s32 n);
    s32 (*_write)(void *cookie, const char *buf, s32 n);
    long (*_seek)(void *cookie, s32 offset, s32 whence);
    s32 (*_close)(void *cookie);
    __sbuf _ub;
    u8 *_up;
    s32 _ur;
    u8 _ubuf[3];
    u8 _nbuf[1];
    __sbuf _lb;
    s32 _blksize;
    s32 _offset;
    _reent *_data;
} FILE;

typedef struct __siov {
    void *iov_base;
    u32 iov_len;
} __siov;

typedef struct __suio {
    __siov *uio_iov;
    s32 uio_iovcnt;
    s32 uio_resid;
} __suio;

typedef struct _Bigint {
    struct _Bigint *next;
    s32 k;
    s32 maxwds;
    s32 sign;
    s32 wds;
    u32 x[1];
} _Bigint;

typedef struct _glue {
    struct _glue *next;
    s32 niobs;
    FILE *iobs;
} _glue;

typedef struct AtExitEntry {
    void *unk00;
    void *unk04;
    void *fn;
    void *unk0C;
    void *unk10;
    struct AtExitEntry *next;
} AtExitEntry;

typedef struct AtExitList {
    u32 **entries;
    s32 count;
} AtExitList;

typedef struct _atexit {
    struct _atexit *_next;
    s32 _ind;
    void (*_fns[32])(void);
} _atexit;

struct _reent {
    s32 _errno;
    FILE *_stdin;
    FILE *_stdout;
    FILE *_stderr;
    s32 _inc;
    char _emergency[25];
    s32 _current_category;
    const char *_current_locale;
    s32 __sdidinit;
    void (*__cleanup)(struct _reent *r);
    _Bigint *_result;
    s32 _result_k;
    _Bigint *_p5s;
    _Bigint **_freelist;
    u8 _unused[0xF8];
    _atexit *_atexit;
    _atexit _atexit0;
    void (**_sig_func)(s32 sig);
    struct _glue __sglue;
    FILE __sf[3];
};

extern _reent *D_0052BE28 __attribute__((section(".data")));

extern s32 _puts_r(_reent *r, const char *s);

extern s32 _raise_r(_reent *r, s32 sig);

extern void *_signal_r(_reent *r, s32 sig, void *handler);

extern s32 _setlocale_r(_reent *r, s32 category, const char *locale);

extern void *func_002DA308(_reent *r);

extern void _cleanup_r(_reent *r);

extern s32 fflush(void *fp);

extern s32 _fwalk(_reent *r, s32 (*func)(void *));

extern s32 libc_2DC670(void);

extern long _strtol_r(_reent *r, const char *s, char **end, s32 base);

extern long strtol(const char *s, char **end, s32 base);

extern s32 raise(s32 sig);

extern void func_001001C8(s32 code) __attribute__((noreturn));

extern u8 D_0054B8D8[] __attribute__((section(".data")));

extern s32 D_00552178 __attribute__((section(".data")));

extern s32 D_0052C28C __attribute__((section(".data")));

extern s32 D_0052C288 __attribute__((section(".data")));

extern s32 D_0052C378 __attribute__((section(".data")));

extern s32 _init_signal_r(_reent *r);

extern void gcc2_compiled__2(FILE *fp, s32 flags, s32 file, _reent *data);

extern _glue *__sfmoreglue(_reent *r, s32 n);

extern s32 __sigtramp_r(_reent *r, s32 sig);

extern void *libc_2D2648(void *fn);

extern void free(void *ptr);

extern void *malloc(u32 size);

extern void __malloc_lock(_reent *r);

extern void __malloc_unlock(_reent *r);

extern void *_malloc_r(_reent *r, u32 size);

extern void _free_r(_reent *r, void *ptr);

extern s32 SignalSema(s32 sema);

extern void *sbrk(s32 incr);

extern s32 close(s32 fd);

extern _Bigint *_Balloc(_reent *r, s32 k);

extern s32 registerAtExit(void *fn, void *entry);

extern s32 libc_2D25B0(void *fn, void *entry);

extern s32 write(s32 fd, const void *buf, s32 count);

extern s32 read(s32 fd, void *buf, s32 count);

extern long lseek(s32 fd, s32 offset, s32 whence);

extern s32 fstat(s32 fd, void *buf);

extern s32 kill(s32 pid, s32 sig);

extern long _read_r(_reent *r, s32 fd, char *buf, s32 n);

extern long _write_r(_reent *r, s32 fd, const char *buf, s32 n);

extern long _lseek_r(_reent *r, s32 fd, s32 offset, s32 whence);

extern s32 __sfvwrite(FILE *fp, __suio *uio);

extern s32 __sread(void *cookie, char *buf, s32 n);

extern s32 __swrite(void *cookie, const char *buf, s32 n);

extern long __sseek(void *cookie, s32 offset, s32 whence);

extern s32 __sclose(void *cookie);

extern s32 GetThreadId(void);

extern s32 WaitSema(s32 sema);

extern s32 vfprintf(FILE *fp, const char *fmt, va_list ap);

extern void *memset(void *s, s32 c, u32 n);

extern void *_calloc_r(_reent *r, u32 n, u32 size);

extern const double D_0054B930[];

extern void abort(void);

extern s32 initAtExit(void);

extern s32 atexit(void (*fn)(void));

extern void (**D_0052BB28)(void) __attribute__((section(".data")));

extern s32 D_0052BB2C __attribute__((section(".data")));

extern s32 D_0052C37C __attribute__((section(".data")));

extern u8 D_0052D940[] __attribute__((section(".data")));

extern void (*D_002F5600[])(void) __attribute__((section(".data")));

extern s32 D_0059AD58 __attribute__((section(".data")));

extern AtExitEntry *D_0059AD60 __attribute__((section(".data")));

typedef struct SdfUnpacked {
    s32 kind;
    s32 sign;
    s32 exp;
    u32 mant;
} SdfUnpacked;

extern void sdfUnpackFloat(const void *x, SdfUnpacked *u);

extern f32 sdfPackFloat(const SdfUnpacked *u);

extern long dptoul(s32 kind, s32 sign, s32 exp, long mant);

#define va_start(ap, last) __builtin_stdarg_start((ap), last)

extern s32 func_002D6678(_reent *r, FILE *fp, const char *fmt, va_list ap);

typedef union f32_bits {
    f32 f;
    u32 u;
} f32_bits;

typedef struct SdfUnpackedD {
    s32 kind;
    s32 sign;
    s32 exp;
    long mant;
} SdfUnpackedD;

extern long libc_2D34A0(const SdfUnpackedD *u);

extern void sdfUnpackDouble(const double *x, SdfUnpackedD *u);

extern SdfUnpackedD *libc_2D1590(const SdfUnpackedD *a, const SdfUnpackedD *b, SdfUnpackedD *r);

extern long libc_2D3638(const SdfUnpackedD *a, const SdfUnpackedD *b);

INCLUDE_ASM(const s32, "code_002C81A8", sceGsResetGraph);

INCLUDE_ASM(const s32, "code_002C81A8", sceGsGetGParam);

INCLUDE_ASM(const s32, "code_002C81A8", sceGsResetPath);

INCLUDE_ASM(const s32, "code_002C81A8", checkModelVersion);

INCLUDE_ASM(const s32, "code_002C81A8", _GetGsDxDyOffset);

INCLUDE_ASM(const s32, "code_002C81A8", sceGsSetDefDispEnv);

INCLUDE_ASM(const s32, "code_002C81A8", sceGsPutDispEnv);

INCLUDE_ASM(const s32, "code_002C81A8", sceGsSyncPath);

INCLUDE_ASM(const s32, "code_002C81A8", sceGsSetDefLoadImage);

INCLUDE_ASM(const s32, "code_002C81A8", sceGsSetDefStoreImage);

INCLUDE_ASM(const s32, "code_002C81A8", sceGsExecLoadImage);

INCLUDE_ASM(const s32, "code_002C81A8", sceGsExecStoreImage);

INCLUDE_ASM(const s32, "code_002C81A8", sceGsPutIMR);

INCLUDE_ASM(const s32, "code_002C81A8", sceGsGetIMR);

INCLUDE_ASM(const s32, "code_002C81A8", WaitDma);

INCLUDE_ASM(const s32, "code_002C81A8", WatchDma);

INCLUDE_ASM(const s32, "code_002C81A8", CheckAddress);

INCLUDE_ASM(const s32, "code_002C81A8", memclr);

INCLUDE_ASM(const s32, "code_002C81A8", sceDmaGetChan);

INCLUDE_ASM(const s32, "code_002C81A8", sceDmaReset);

INCLUDE_ASM(const s32, "code_002C81A8", sceDmaPutEnv);

INCLUDE_ASM(const s32, "code_002C81A8", sceDmaGetEnv);

INCLUDE_ASM(const s32, "code_002C81A8", sceDmaPutStallAddr);

INCLUDE_ASM(const s32, "code_002C81A8", sceDmaSend);

INCLUDE_ASM(const s32, "code_002C81A8", sceDmaSendN);

INCLUDE_ASM(const s32, "code_002C81A8", sceDmaSendI);

INCLUDE_ASM(const s32, "code_002C81A8", sceDmaRecv);

INCLUDE_ASM(const s32, "code_002C81A8", sceDmaRecvN);

INCLUDE_ASM(const s32, "code_002C81A8", sceDmaRecvI);

INCLUDE_ASM(const s32, "code_002C81A8", sceDmaSync);

INCLUDE_ASM(const s32, "code_002C81A8", sceDmaWatch);

INCLUDE_ASM(const s32, "code_002C81A8", sceDmaPause);

INCLUDE_ASM(const s32, "code_002C81A8", sceDmaRestart);

INCLUDE_ASM(const s32, "code_002C81A8", _send_to_iop);

INCLUDE_ASM(const s32, "code_002C81A8", scePadInit);

INCLUDE_ASM(const s32, "code_002C81A8", scePadInit2);

INCLUDE_ASM(const s32, "code_002C81A8", scePadEnd);

INCLUDE_ASM(const s32, "code_002C81A8", scePadPortOpen);

INCLUDE_ASM(const s32, "code_002C81A8", scePadPortClose);

INCLUDE_ASM(const s32, "code_002C81A8", scePadGetDmaStr);

INCLUDE_ASM(const s32, "code_002C81A8", scePadGetFrameCount);

INCLUDE_ASM(const s32, "code_002C81A8", scePadRead);

INCLUDE_ASM(const s32, "code_002C81A8", scePadGetState);

INCLUDE_ASM(const s32, "code_002C81A8", scePadStateIntToStr);

INCLUDE_ASM(const s32, "code_002C81A8", scePadSetReqState);

INCLUDE_ASM(const s32, "code_002C81A8", scePadGetReqState);

INCLUDE_ASM(const s32, "code_002C81A8", scePadReqIntToStr);

INCLUDE_ASM(const s32, "code_002C81A8", scePadInfoAct);

INCLUDE_ASM(const s32, "code_002C81A8", scePadInfoComb);

INCLUDE_ASM(const s32, "code_002C81A8", scePadInfoMode);

INCLUDE_ASM(const s32, "code_002C81A8", scePadSetMainMode);

INCLUDE_ASM(const s32, "code_002C81A8", scePadSetActDirect);

INCLUDE_ASM(const s32, "code_002C81A8", scePadSetActAlign);

INCLUDE_ASM(const s32, "code_002C81A8", scePadGetButtonMask);

INCLUDE_ASM(const s32, "code_002C81A8", scePadSetButtonInfo);

INCLUDE_ASM(const s32, "code_002C81A8", scePadInfoPressMode);

INCLUDE_ASM(const s32, "code_002C81A8", scePadEnterPressMode);

INCLUDE_ASM(const s32, "code_002C81A8", scePadExitPressMode);

INCLUDE_ASM(const s32, "code_002C81A8", scePadSetVrefParam);

INCLUDE_ASM(const s32, "code_002C81A8", scePadGetPortMax);

INCLUDE_ASM(const s32, "code_002C81A8", scePadGetSlotMax);

INCLUDE_ASM(const s32, "code_002C81A8", scePadGetModVersion);

INCLUDE_ASM(const s32, "code_002C81A8", scePadSetWarningLevel);

INCLUDE_ASM(const s32, "code_002C81A8", gcc2_compiled__6);

INCLUDE_ASM(const s32, "code_002C81A8", sceCdDelayThread);

INCLUDE_ASM(const s32, "code_002C81A8", sceCdCallback);

INCLUDE_ASM(const s32, "code_002C81A8", _sceCd_cd_callback);

INCLUDE_ASM(const s32, "code_002C81A8", _Cdvd_cbLoop);

INCLUDE_ASM(const s32, "code_002C81A8", sceCdInitEeCB);

INCLUDE_ASM(const s32, "code_002C81A8", _sceCd_cd_read_intr);

INCLUDE_ASM(const s32, "code_002C81A8", cmd_sem_init);

INCLUDE_ASM(const s32, "code_002C81A8", cdvd_exit);

INCLUDE_ASM(const s32, "code_002C81A8", sceCdPOffCallback);

INCLUDE_ASM(const s32, "code_002C81A8", _sceCd_Poff_Intr);

INCLUDE_ASM(const s32, "code_002C81A8", PowerOffCB);

INCLUDE_ASM(const s32, "code_002C81A8", sceCdLayerSearchFile);

INCLUDE_ASM(const s32, "code_002C81A8", sceCdSearchFile);

INCLUDE_ASM(const s32, "code_002C81A8", _sceCd_ncmd_prechk);

INCLUDE_ASM(const s32, "code_002C81A8", sceCdNcmdDiskReady);

INCLUDE_ASM(const s32, "code_002C81A8", sceCdSync);

INCLUDE_ASM(const s32, "code_002C81A8", sceCdSyncS);

INCLUDE_ASM(const s32, "code_002C81A8", _sceCd_scmd_prechk);

INCLUDE_ASM(const s32, "code_002C81A8", sceCdInit);

INCLUDE_ASM(const s32, "code_002C81A8", sceCdDiskReady);

INCLUDE_ASM(const s32, "code_002C81A8", sceCdMmode);

INCLUDE_ASM(const s32, "code_002C81A8", sceCdGetDiskType);

INCLUDE_ASM(const s32, "code_002C81A8", sceCdStatus);

INCLUDE_ASM(const s32, "code_002C81A8", sceCdPowerOff);

INCLUDE_ASM(const s32, "code_002C81A8", sceCdReadClock);

INCLUDE_ASM(const s32, "code_002C81A8", sceCdStInit);

INCLUDE_ASM(const s32, "code_002C81A8", sceCdStStart);

INCLUDE_ASM(const s32, "code_002C81A8", sceCdStSeekF);

INCLUDE_ASM(const s32, "code_002C81A8", sceCdStSeek);

INCLUDE_ASM(const s32, "code_002C81A8", sceCdStStop);

INCLUDE_ASM(const s32, "code_002C81A8", sceCdStRead);

INCLUDE_ASM(const s32, "code_002C81A8", sceCdStPause);

INCLUDE_ASM(const s32, "code_002C81A8", sceCdStResume);

INCLUDE_ASM(const s32, "code_002C81A8", sceCdStStat);

INCLUDE_ASM(const s32, "code_002C81A8", sceCdStream);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifMInitRpc);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifMExitRpc);

INCLUDE_ASM(const s32, "code_002C81A8", _sceRpcGetPacket);

INCLUDE_ASM(const s32, "code_002C81A8", _sceRpcFreePacket);

INCLUDE_ASM(const s32, "code_002C81A8", _request_end);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifMBindRpc);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifMBindRpcParam);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifMUnBindRpc);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifMCallRpc);

INCLUDE_ASM(const s32, "code_002C81A8", gcc2_compiled__7);

INCLUDE_ASM(const s32, "code_002C81A8", setD4_CHCR);

INCLUDE_ASM(const s32, "code_002C81A8", sceIpuStopDMA);

INCLUDE_ASM(const s32, "code_002C81A8", sceIpuRestartDMA);

INCLUDE_ASM(const s32, "code_002C81A8", sceIpuSync);

INCLUDE_ASM(const s32, "code_002C81A8", gcc2_compiled__8);

INCLUDE_ASM(const s32, "code_002C81A8", sceIpuInit);

INCLUDE_ASM(const s32, "code_002C81A8", sceMcInit);

INCLUDE_ASM(const s32, "code_002C81A8", sceMcEnd);

INCLUDE_ASM(const s32, "code_002C81A8", _lmcGetClientPtr);

INCLUDE_ASM(const s32, "code_002C81A8", sceMcChangeThreadPriority);

INCLUDE_ASM(const s32, "code_002C81A8", sceMcGetSlotMax);

INCLUDE_ASM(const s32, "code_002C81A8", sceMcOpen);

INCLUDE_ASM(const s32, "code_002C81A8", sceMcMkdir);

INCLUDE_ASM(const s32, "code_002C81A8", sceMcClose);

INCLUDE_ASM(const s32, "code_002C81A8", sceMcSeek);

INCLUDE_ASM(const s32, "code_002C81A8", mceIntrReadFixAlign);

INCLUDE_ASM(const s32, "code_002C81A8", sceMcRead);

INCLUDE_ASM(const s32, "code_002C81A8", sceMcWrite);

INCLUDE_ASM(const s32, "code_002C81A8", mcHearAlarm);

INCLUDE_ASM(const s32, "code_002C81A8", mcDelayThread);

INCLUDE_ASM(const s32, "code_002C81A8", sceMcSync);

INCLUDE_ASM(const s32, "code_002C81A8", mceGetInfoApdx);

INCLUDE_ASM(const s32, "code_002C81A8", sceMcGetInfo);

INCLUDE_ASM(const s32, "code_002C81A8", sceMcUdCheckNewCard);

INCLUDE_ASM(const s32, "code_002C81A8", sceMcGetDir);

INCLUDE_ASM(const s32, "code_002C81A8", mceStorePwd);

INCLUDE_ASM(const s32, "code_002C81A8", sceMcChdir);

INCLUDE_ASM(const s32, "code_002C81A8", sceMcFormat);

INCLUDE_ASM(const s32, "code_002C81A8", sceMcDelete);

INCLUDE_ASM(const s32, "code_002C81A8", sceMcFlush);

INCLUDE_ASM(const s32, "code_002C81A8", sceMcSetFileInfo);

INCLUDE_ASM(const s32, "code_002C81A8", sceMcRename);

INCLUDE_ASM(const s32, "code_002C81A8", sceMcUnformat);

INCLUDE_ASM(const s32, "code_002C81A8", sceMcGetEntSpace);

INCLUDE_ASM(const s32, "code_002C81A8", cosf);

INCLUDE_ASM(const s32, "code_002C81A8", sinf);

INCLUDE_ASM(const s32, "code_002C81A8", tanf);

INCLUDE_ASM(const s32, "code_002C81A8", __ieee754_acosf);

INCLUDE_ASM(const s32, "code_002C81A8", __ieee754_atan2f);

INCLUDE_ASM(const s32, "code_002C81A8", __ieee754_rem_pio2f);

INCLUDE_ASM(const s32, "code_002C81A8", __ieee754_sqrtf);

INCLUDE_ASM(const s32, "code_002C81A8", __kernel_cosf);

INCLUDE_ASM(const s32, "code_002C81A8", __kernel_rem_pio2f);

INCLUDE_ASM(const s32, "code_002C81A8", __kernel_sinf);

INCLUDE_ASM(const s32, "code_002C81A8", __kernel_tanf);

INCLUDE_ASM(const s32, "code_002C81A8", atanf);

INCLUDE_ASM(const s32, "code_002C81A8", fabsf);

INCLUDE_ASM(const s32, "code_002C81A8", floorf);

INCLUDE_ASM(const s32, "code_002C81A8", scalbnf);

INCLUDE_ASM(const s32, "code_002C81A8", copysignf);

void runAtExitList(void) {
    void (**p)(void);

    while (*D_0052BB28 != NULL) {
        p = D_0052BB28;
        D_0052BB28 = p + 1;
        (*p)();
    }
    if (D_0052BB2C == 0) {
        D_0052BB2C = 1;
        libc_2D2648(D_0052D940);
    }
}

INCLUDE_ASM(const s32, "code_002C81A8", initAtExit);

void initFloatOnce(void) {
    if (D_0059AD58 == 0) {
        D_0059AD58 = 1;
        initAtExit();
    }
}

long sdfFloatToULong(f32 x) {
    SdfUnpacked u;

    sdfUnpackFloat(&x, &u);
    return dptoul(u.kind, u.sign, u.exp, (long)u.mant << 30);
}

INCLUDE_ASM(const s32, "code_002C81A8", libc_2D1590);

long dpadd(double a, double b) {
    SdfUnpackedD ua;
    SdfUnpackedD ub;
    SdfUnpackedD r;

    sdfUnpackDouble(&a, &ua);
    sdfUnpackDouble(&b, &ub);
    return libc_2D34A0(libc_2D1590(&ua, &ub, &r));
}

long dpsub(double a, double b) {
    SdfUnpackedD ua;
    SdfUnpackedD ub;
    SdfUnpackedD r;

    sdfUnpackDouble(&a, &ua);
    sdfUnpackDouble(&b, &ub);
    ub.sign ^= 1;
    return libc_2D34A0(libc_2D1590(&ua, &ub, &r));
}

INCLUDE_ASM(const s32, "code_002C81A8", dpmul);

long dpdiv(double a, double b) {
    SdfUnpackedD ua;
    SdfUnpackedD ub;

    sdfUnpackDouble(&a, &ua);
    sdfUnpackDouble(&b, &ub);
    return libc_2D3638(&ua, &ub);
}

INCLUDE_ASM(const s32, "code_002C81A8", litodp);

INCLUDE_ASM(const s32, "code_002C81A8", dptoli);

long dptoul(s32 kind, s32 sign, s32 exp, long mant) {
    SdfUnpackedD u;

    u.kind = kind;
    u.sign = sign;
    u.exp = exp;
    u.mant = mant;
    return libc_2D34A0(&u);
}

#ifdef FIX_BUGS
f32 sdfPackQuad(s32 kind, s32 sign, s32 exp, u32 mant);
#endif

u32 sdfDoubleToULong(double x) {
    SdfUnpackedD u;
    u32 m;

    sdfUnpackDouble(&x, &u);
    m = u.mant >> 30;
    if (u.mant & 0x3FFFFFFF) {
        m |= 1;
    }
    /* BUG: sdfPackQuad returns f32, which this target returns in $f0, but
     * with nothing in scope ee-gcc assumes int and the result is read from
     * $v0. So what comes back is not the packed value at all. Retail does
     * this, and declaring the function moves the image, so the behaviour is
     * kept and the declaration sits behind FIX_BUGS. */
    return sdfPackQuad(u.kind, u.sign, u.exp, m);
}

INCLUDE_ASM(const s32, "code_002C81A8", sdfDoubleToUInt);

u8 *libc_2D1E08(u8 *p, u32 *out) {
    u32 b;
    u32 v;
    s32 shift = 0;

    b = *p++;
    v = b & 0x7F;
    while (b & 0x80) {
        b = *p++;
        shift += 7;
        v |= (b & 0x7F) << shift;
    }
    *out = v;
    return p;
}

u8 *libc_2D1E50(u8 *p, s32 *out) {
    s32 shift = 0;
    u32 v = 0;
    u32 b;

    do {
        b = *p++;
        v |= (b & 0x7F) << shift;
        shift += 7;
    } while (b & 0x80);
    if ((u32)shift < 32 && (b & 0x40)) {
        v |= -1 << shift;
    }
    *out = v;
    return p;
}

INCLUDE_ASM(const s32, "code_002C81A8", libc_2D1EA8);

INCLUDE_ASM(const s32, "code_002C81A8", libc_2D1FA0);

s32 registerAtExit(void *fn, void *entry) {
    AtExitEntry *e = entry;

    e->fn = fn;
    e->unk04 = NULL;
    e->unk00 = NULL;
    e->unk0C = NULL;
    e->unk10 = NULL;
    WaitSema(D_0052C37C);
    e->next = D_0059AD60;
    D_0059AD60 = e;
    SignalSema(D_0052C37C);
    /* Dead store: blocks the SignalSema sibcall. */
    entry = NULL;
    return;
}

s32 libc_2D2580(void *fn) {
    return registerAtExit(fn, malloc(0x18));
}

s32 libc_2D25B0(void *fn, void *entry) {
    AtExitEntry *e = entry;

    e->fn = fn;
    e->unk0C = fn;
    e->unk04 = NULL;
    e->unk00 = NULL;
    e->unk10 = NULL;
    WaitSema(D_0052C37C);
    e->next = D_0059AD60;
    D_0059AD60 = e;
    SignalSema(D_0052C37C);
    /* Dead store: blocks the SignalSema sibcall. */
    entry = NULL;
    return;
}

s32 libc_2D2618(void *fn) {
    return libc_2D25B0(fn, malloc(0x18));
}

INCLUDE_ASM(const s32, "code_002C81A8", libc_2D2648);

void ctime_4(void *fn) {
    free(libc_2D2648(fn));
}

s32 libc_2D2728(u32 *p) {
    s32 n = 0;

    while (p[0] != 0) {
        if (p[1] != 0 && p[2] != 0) {
            n++;
        }
        p = (u32 *)((char *)p + p[0] + 4);
    }
    return n;
}

void libc_2D2768(u32 *p, AtExitList *list, u32 *pmin, u32 *pmax) {
    u32 lo = *pmin;
    u32 hi = *pmax;

    while (p[0] != 0) {
        if (p[1] != 0) {
            u32 a = p[2];

            if (a != 0) {
                if (list->entries != NULL) {
                    list->entries[list->count++] = p;
                }
                if (a < lo) {
                    lo = a;
                }
                if (hi < a + p[3]) {
                    hi = a + p[3];
                }
            }
        }
        p = (u32 *)((char *)p + p[0] + 4);
    }
    *pmin = lo;
    *pmax = hi;
}

void *libc_2D27F8(u32 *p, u32 addr) {
    while (p[0] != 0) {
        if (p[1] != 0 && p[2] != 0 && addr - p[2] < p[3]) {
            return p;
        }
        p = (u32 *)((char *)p + p[0] + 4);
    }
    return NULL;
}

INCLUDE_ASM(const s32, "code_002C81A8", libc_2D2850);

INCLUDE_ASM(const s32, "code_002C81A8", libc_2D29B8);

INCLUDE_ASM(const s32, "code_002C81A8", libc_2D2B60);

INCLUDE_ASM(const s32, "code_002C81A8", libc_2D2C70);

INCLUDE_ASM(const s32, "code_002C81A8", libc_2D2D90);

INCLUDE_ASM(const s32, "code_002C81A8", libc_2D31E0);

INCLUDE_ASM(const s32, "code_002C81A8", libc_2D3398);

INCLUDE_ASM(const s32, "code_002C81A8", sdfUnpackFloat);

f32 sdfPackQuad(s32 kind, s32 sign, s32 exp, u32 mant) {
    SdfUnpacked u;

    u.kind = kind;
    u.sign = sign;
    u.exp = exp;
    u.mant = mant;
    return sdfPackFloat(&u);
}

INCLUDE_ASM(const s32, "code_002C81A8", libc_2D34A0);

INCLUDE_ASM(const s32, "code_002C81A8", sdfUnpackDouble);

INCLUDE_ASM(const s32, "code_002C81A8", libc_2D3638);

INCLUDE_ASM(const s32, "code_002C81A8", sdfPackFloat);

void abort(void) {
    while (1) {
        raise(6);
        func_001001C8(1);
    }
}

s32 atexit(void (*fn)(void)) {
    _atexit *p;

    p = D_0052BE28->_atexit;
    if (p == NULL) {
        p = &D_0052BE28->_atexit0;
        D_0052BE28->_atexit = p;
    }
    if (p->_ind >= 32) {
        if ((p = malloc(sizeof(_atexit))) == NULL) {
            return -1;
        }
        p->_ind = 0;
        p->_next = D_0052BE28->_atexit;
        D_0052BE28->_atexit = p;
    }
    p->_fns[p->_ind++] = fn;
    return 0;
}

s32 atoi(const char *s) {
    return strtol(s, NULL, 10);
}

void *malloc(u32 size) {
    void *result;

    __malloc_lock(D_0052BE28);
    result = _malloc_r(D_0052BE28, size);
    __malloc_unlock(D_0052BE28);
    return result;
}

void free(void *ptr) {
    __malloc_lock(D_0052BE28);
    _free_r(D_0052BE28, ptr);
    __malloc_unlock(D_0052BE28);
}

INCLUDE_ASM(const s32, "code_002C81A8", gcc2_compiled_);

INCLUDE_ASM(const s32, "code_002C81A8", _malloc_r);

INCLUDE_ASM(const s32, "code_002C81A8", memcmp);

INCLUDE_ASM(const s32, "code_002C81A8", memcpy);

INCLUDE_ASM(const s32, "code_002C81A8", memmove);

INCLUDE_ASM(const s32, "code_002C81A8", memset);

void __malloc_lock(_reent *r) {
    s32 owner = GetThreadId();

    if (D_0052C288 == owner) {
        D_0052C28C++;
    } else {
        WaitSema(D_0052C378);
        D_0052C288 = owner;
        D_0052C28C++;
    }
}

void __malloc_unlock(_reent *r) {
    if (--D_0052C28C != 0) {
        return;
    }
    D_0052C288 = -1;
    SignalSema(D_0052C378);
}

s32 _printf_r(_reent *r, const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    return func_002D6678(r, r->_stdout, fmt, ap);
}

s32 printf(const char *fmt, ...) {
    _reent *r = D_0052BE28;
    va_list ap;

    va_start(ap, fmt);
    r->_stdout->_data = r;
    return vfprintf(r->_stdout, fmt, ap);
}

INCLUDE_ASM(const s32, "code_002C81A8", _puts_r);

s32 puts(const char *s) {
    return _puts_r(D_0052BE28, s);
}

INCLUDE_ASM(const s32, "code_002C81A8", qsort);

void *_sbrk_r(_reent *r, s32 incr) {
    void *result;

    D_00552178 = 0;
    result = sbrk(incr);
    if (result == (void *)-1 && D_00552178 != 0) {
        r->_errno = D_00552178;
    }
    return result;
}

s32 _init_signal_r(_reent *r) {
    s32 i;

    if (r->_sig_func == NULL) {
        r->_sig_func = _malloc_r(r, 32 * sizeof(void *));
        if (r->_sig_func == NULL) {
            return -1;
        }
        for (i = 0; i < 32; i++) {
            r->_sig_func[i] = NULL;
        }
    }
    return 0;
}

void *_signal_r(_reent *r, s32 sig, void *handler) {
    void *old;

    if ((u32)sig >= 32) {
        r->_errno = 22;
        return (void *)-1;
    }
    if (r->_sig_func == NULL && _init_signal_r(r) != 0) {
        return (void *)-1;
    }
    old = (void *)r->_sig_func[sig];
    r->_sig_func[sig] = (void (*)(s32))handler;
    return old;
}

INCLUDE_ASM(const s32, "code_002C81A8", _raise_r);

s32 __sigtramp_r(_reent *r, s32 sig) {
    void (*handler)(s32 sig);

    if ((u32)sig >= 32) {
        return -1;
    }
    if (r->_sig_func == NULL && _init_signal_r(r) != 0) {
        return -1;
    }
    handler = r->_sig_func[sig];
    if (handler == NULL) {
        return 1;
    }
    if (handler == (void (*)(s32))-1) {
        return 2;
    }
    if (handler == (void (*)(s32))1) {
        return 3;
    }
    r->_sig_func[sig] = NULL;
    handler(sig);
    return 0;
}

s32 raise(s32 sig) {
    return _raise_r(D_0052BE28, sig);
}

void *signal(s32 sig, void *handler) {
    return _signal_r(D_0052BE28, sig, handler);
}

s32 _init_signal(void) {
    return _init_signal_r(D_0052BE28);
}

s32 __sigtramp(s32 sig) {
    return __sigtramp_r(D_0052BE28, sig);
}

s32 _kill_r(_reent *r, s32 pid, s32 sig) {
    s32 result;

    D_00552178 = 0;
    result = kill(pid, sig);
    if (result == -1 && D_00552178 != 0) {
        r->_errno = D_00552178;
    }
    return result;
}

s32 _getpid_r(_reent *r) {
    return libc_2DC670();
}

s32 _sprintf_r(_reent *r, char *str, const char *fmt, ...) {
    s32 result;
    FILE f;
    va_list ap;

    va_start(ap, fmt);
    f._flags = 0x208;
    f._bf._base = f._p = (u8 *)str;
    f._bf._size = f._w = 0x7FFFFFFF;
    f._data = r;
    result = vfprintf(&f, fmt, ap);
    *f._p = '\0';
    return result;
}

s32 sprintf(char *str, const char *fmt, ...) {
    s32 result;
    FILE f;
    va_list ap;

    va_start(ap, fmt);
    f._flags = 0x208;
    f._bf._base = f._p = (u8 *)str;
    f._bf._size = f._w = 0x7FFFFFFF;
    f._data = D_0052BE28;
    result = vfprintf(&f, fmt, ap);
    *f._p = '\0';
    return result;
}

INCLUDE_ASM(const s32, "code_002C81A8", strcasecmp);

INCLUDE_ASM(const s32, "code_002C81A8", strcat);

INCLUDE_ASM(const s32, "code_002C81A8", strchr);

INCLUDE_ASM(const s32, "code_002C81A8", strcmp);

INCLUDE_ASM(const s32, "code_002C81A8", strcpy);

INCLUDE_ASM(const s32, "code_002C81A8", strlen);

INCLUDE_ASM(const s32, "code_002C81A8", strncmp);

INCLUDE_ASM(const s32, "code_002C81A8", strncpy);

char *strrchr(const char *s, s32 c) {
    char ch = c;
    const char *last = NULL;

    while (*s != '\0') {
        if (*s == ch) {
            last = s;
        }
        s++;
    }
    return (char *)(*s != ch ? last : s);
}

INCLUDE_ASM(const s32, "code_002C81A8", strstr);

INCLUDE_ASM(const s32, "code_002C81A8", _strtol_r);

long strtol(const char *s, char **end, s32 base) {
    return _strtol_r(D_0052BE28, s, end, base);
}

s32 gcc2_compiled__0(FILE *fp, __suio *uio) {
    s32 err;

    if (uio->uio_resid == 0) {
        uio->uio_iovcnt = 0;
        return 0;
    }
    err = __sfvwrite(fp, uio);
    uio->uio_resid = 0;
    uio->uio_iovcnt = 0;
    return err;
}

INCLUDE_ASM(const s32, "code_002C81A8", __sbprintf);

INCLUDE_ASM(const s32, "code_002C81A8", vfprintf);

INCLUDE_ASM(const s32, "code_002C81A8", cvt);

INCLUDE_ASM(const s32, "code_002C81A8", exponent);

s32 vsprintf(char *str, const char *fmt, va_list ap) {
    s32 result;
    FILE f;

    f._flags = 0x208;
    f._bf._base = f._p = (u8 *)str;
    f._bf._size = f._w = 0x7FFFFFFF;
    f._data = D_0052BE28;
    result = vfprintf(&f, fmt, ap);
    *f._p = '\0';
    return result;
}

s32 vsprintf_r(_reent *r, char *str, const char *fmt, va_list ap) {
    s32 result;
    FILE f;

    f._flags = 0x208;
    f._bf._base = f._p = (u8 *)str;
    f._bf._size = f._w = 0x7FFFFFFF;
    f._data = r;
    result = vfprintf(&f, fmt, ap);
    *f._p = '\0';
    return result;
}

INCLUDE_ASM(const s32, "code_002C81A8", __swsetup);

INCLUDE_ASM(const s32, "code_002C81A8", gcc2_compiled__1);

INCLUDE_ASM(const s32, "code_002C81A8", _dtoa_r);

INCLUDE_ASM(const s32, "code_002C81A8", fflush);

void gcc2_compiled__2(FILE *fp, s32 flags, s32 file, _reent *data) {
    fp->_p = NULL;
    fp->_r = 0;
    fp->_w = 0;
    fp->_flags = flags;
    fp->_file = file;
    fp->_bf._base = NULL;
    fp->_bf._size = 0;
    fp->_lbfsize = 0;
    fp->_cookie = fp;
    fp->_read = __sread;
    fp->_write = __swrite;
    fp->_seek = __sseek;
    fp->_close = __sclose;
    fp->_data = data;
}

_glue *__sfmoreglue(_reent *r, s32 n) {
    _glue *g;
    FILE *p;

    g = _malloc_r(r, sizeof(_glue) + n * sizeof(FILE));
    if (g == NULL) {
        return NULL;
    }
    p = (FILE *)(g + 1);
    g->next = NULL;
    g->niobs = n;
    g->iobs = p;
    memset(p, 0, n * sizeof(FILE));
    return g;
}

INCLUDE_ASM(const s32, "code_002C81A8", __sfp);

void _cleanup_r(_reent *r) {
    _fwalk(r, fflush);
}

void _cleanup(void) {
    _cleanup_r(D_0052BE28);
}

void __sinit(_reent *r) {
    r->__cleanup = _cleanup_r;
    r->__sdidinit = 1;
    gcc2_compiled__2(&r->__sf[0], 4, 0, r);
    gcc2_compiled__2(&r->__sf[1], 9, 1, r);
    gcc2_compiled__2(&r->__sf[2], 0xA, 2, r);
    r->__sglue.next = NULL;
    r->__sglue.niobs = 3;
    r->__sglue.iobs = &r->__sf[0];
}

INCLUDE_ASM(const s32, "code_002C81A8", _free_r);

INCLUDE_ASM(const s32, "code_002C81A8", _malloc_trim_r);

INCLUDE_ASM(const s32, "code_002C81A8", __sfvwrite);

s32 _fwalk(_reent *r, s32 (*func)(void *)) {
    _glue *g;
    FILE *fp;
    s32 n;
    s32 ret = 0;

    for (g = &r->__sglue; g != NULL; g = g->next) {
        for (fp = g->iobs, n = g->niobs; --n >= 0; fp++) {
            if (fp->_flags != 0) {
                ret |= func(fp);
            }
        }
    }
    return ret;
}

INCLUDE_ASM(const s32, "code_002C81A8", _setlocale_r);

void *func_002DA308(_reent *r) {
    return D_0054B8D8;
}

s32 setlocale(s32 category, const char *locale) {
    return _setlocale_r(D_0052BE28, category, locale);
}

void *localeconv(void) {
    return func_002DA308(D_0052BE28);
}

INCLUDE_ASM(const s32, "code_002C81A8", __smakebuf);

s32 _mbtowc_r(_reent *r, s32 *pwc, const char *s, u32 n, s32 *state) {
    s32 dummy;
    s32 *out = pwc;

    if (pwc == NULL) {
        out = &dummy;
    }
    if (s == NULL) {
        return 0;
    }
    if (n == 0) {
        return -1;
    }
    *out = *(u8 *)s;
    if (*(u8 *)s == 0) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_002C81A8", memchr);

_Bigint *_Balloc(_reent *r, s32 k) {
    _Bigint *rv;
    s32 x;

    if (r->_freelist == NULL) {
        r->_freelist = _calloc_r(r, 4, 16);
        if (r->_freelist == NULL) {
            return NULL;
        }
    }
    rv = r->_freelist[k];
    if (rv != NULL) {
        r->_freelist[k] = rv->next;
    } else {
        x = 1 << k;
        rv = _calloc_r(r, 1, 0x14 + x * sizeof(u32));
        if (rv == NULL) {
            return NULL;
        }
        rv->k = k;
        rv->maxwds = x;
    }
    rv->sign = rv->wds = 0;
    return rv;
}

void _Bfree(_reent *r, _Bigint *v) {
    if (v != NULL) {
        v->next = r->_freelist[v->k];
        r->_freelist[v->k] = v;
    }
}

INCLUDE_ASM(const s32, "code_002C81A8", _multadd);

INCLUDE_ASM(const s32, "code_002C81A8", _s2b);

s32 _hi0bits(u32 x) {
    s32 k = 0;

    if ((x & 0xFFFF0000) == 0) {
        k = 16;
        x <<= 16;
    }
    if ((x & 0xFF000000) == 0) {
        k += 8;
        x <<= 8;
    }
    if ((x & 0xF0000000) == 0) {
        k += 4;
        x <<= 4;
    }
    if ((x & 0xC0000000) == 0) {
        k += 2;
        x <<= 2;
    }
    if ((x & 0x80000000) == 0) {
        k++;
        if ((x & 0x40000000) == 0) {
            return 32;
        }
    }
    return k;
}

s32 _lo0bits(u32 *y) {
    s32 k;
    u32 x = *y;

    if (x & 7) {
        if (x & 1) {
            return 0;
        }
        if (x & 2) {
            *y = x >> 1;
            return 1;
        }
        *y = x >> 2;
        return 2;
    }
    k = 0;
    if ((x & 0xFFFF) == 0) {
        k = 16;
        x >>= 16;
    }
    if ((x & 0xFF) == 0) {
        k += 8;
        x >>= 8;
    }
    if ((x & 0xF) == 0) {
        k += 4;
        x >>= 4;
    }
    if ((x & 3) == 0) {
        k += 2;
        x >>= 2;
    }
    if ((x & 1) == 0) {
        k++;
        x >>= 1;
        if (x == 0) {
            return 32;
        }
    }
    *y = x;
    return k;
}

_Bigint *_i2b(_reent *r, s32 i) {
    _Bigint *b = _Balloc(r, 1);

    b->x[0] = i;
    b->wds = 1;
    return b;
}

INCLUDE_ASM(const s32, "code_002C81A8", _multiply);

INCLUDE_ASM(const s32, "code_002C81A8", _pow5mult);

INCLUDE_ASM(const s32, "code_002C81A8", _lshift);

s32 __mcmp(_Bigint *a, _Bigint *b) {
    u32 *xa, *xa0, *xb, *xb0;
    s32 i, j;

    i = a->wds;
    j = b->wds;
    if ((i -= j) != 0) {
        return i;
    }
    xa0 = a->x;
    xa = xa0 + j;
    xb0 = b->x;
    xb = xb0 + j;
    for (;;) {
        if (*--xa != *--xb) {
            return *xa < *xb ? -1 : 1;
        }
        if (xa <= xa0) {
            break;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_002C81A8", __mdiff);

INCLUDE_ASM(const s32, "code_002C81A8", _ulp);

INCLUDE_ASM(const s32, "code_002C81A8", _b2d);

INCLUDE_ASM(const s32, "code_002C81A8", _d2b);

INCLUDE_ASM(const s32, "code_002C81A8", _ratio);

double _mprec_log10(s32 dig) {
    double v = 1.0;

    if (dig < 24) {
        return D_0054B930[dig];
    }
    while (dig > 0) {
        v *= 10.0;
        dig--;
    }
    return v;
}

INCLUDE_ASM(const s32, "code_002C81A8", isinf);

INCLUDE_ASM(const s32, "code_002C81A8", isnan);

s32 __sread(void *cookie, char *buf, s32 n) {
    FILE *fp = cookie;
    s32 result;

    result = _read_r(fp->_data, fp->_file, buf, n);
    if (result >= 0) {
        fp->_offset += result;
    } else {
        fp->_flags &= ~0x1000;
    }
    return result;
}

s32 __swrite(void *cookie, const char *buf, s32 n) {
    FILE *fp = cookie;

    if (fp->_flags & 0x100) {
        _lseek_r(fp->_data, fp->_file, 0, 2);
    }
    fp->_flags &= ~0x1000;
    return _write_r(fp->_data, fp->_file, buf, n);
}

long __sseek(void *cookie, s32 offset, s32 whence) {
    FILE *fp = cookie;
    long result;

    if ((result = _lseek_r(fp->_data, fp->_file, offset, whence)) == -1) {
        fp->_flags &= ~0x1000;
    } else {
        fp->_flags |= 0x1000;
        fp->_offset = result;
    }
    return result;
}

s32 __sclose(void *cookie) {
    FILE *fp = cookie;

    return _close_r(fp->_data, fp->_file);
}

long _write_r(_reent *r, s32 fd, const char *buf, s32 n) {
    long result;

    D_00552178 = 0;
    if ((result = write(fd, buf, n)) == -1 && D_00552178 != 0) {
        r->_errno = D_00552178;
    }
    return result;
}

INCLUDE_ASM(const s32, "code_002C81A8", _calloc_r);

s32 _close_r(_reent *r, s32 fd) {
    s32 result;

    D_00552178 = 0;
    result = close(fd);
    if (result == -1 && D_00552178 != 0) {
        r->_errno = D_00552178;
    }
    return result;
}

s32 _fstat_r(_reent *r, s32 fd, void *buf) {
    s32 result;

    D_00552178 = 0;
    result = fstat(fd, buf);
    if (result == -1 && D_00552178 != 0) {
        r->_errno = D_00552178;
    }
    return result;
}

long _lseek_r(_reent *r, s32 fd, s32 offset, s32 whence) {
    long result;

    D_00552178 = 0;
    result = lseek(fd, offset, whence);
    if (result == -1 && D_00552178 != 0) {
        r->_errno = D_00552178;
    }
    return result;
}

long _read_r(_reent *r, s32 fd, char *buf, s32 n) {
    long result;

    D_00552178 = 0;
    if ((result = read(fd, buf, n)) == -1 && D_00552178 != 0) {
        r->_errno = D_00552178;
    }
    return result;
}

/* Retail puts 16 zero bytes between the end of this unit's C code and the
   handwritten syscall stubs below. Nothing in either side emits them, so the
   padding has to be stated here. */
__asm__(".space 16");

INCLUDE_ASM(const s32, "code_002C81A8", RFU000_FullReset);

INCLUDE_ASM(const s32, "code_002C81A8", ResetEE);

INCLUDE_ASM(const s32, "code_002C81A8", libc_2DBAA0);

INCLUDE_ASM(const s32, "code_002C81A8", RFU003);

INCLUDE_ASM(const s32, "code_002C81A8", libc_2DBAC0);

INCLUDE_ASM(const s32, "code_002C81A8", RFU005);

INCLUDE_ASM(const s32, "code_002C81A8", libc_2DBAE0);

INCLUDE_ASM(const s32, "code_002C81A8", _ExecPS2);

INCLUDE_ASM(const s32, "code_002C81A8", RFU008);

INCLUDE_ASM(const s32, "code_002C81A8", RFU009);

INCLUDE_ASM(const s32, "code_002C81A8", AddSbusIntcHandler);

INCLUDE_ASM(const s32, "code_002C81A8", RemoveSbusIntcHandler);

INCLUDE_ASM(const s32, "code_002C81A8", Interrupt2Iop);

INCLUDE_ASM(const s32, "code_002C81A8", SetVTLBRefillHandler);

INCLUDE_ASM(const s32, "code_002C81A8", SetVCommonHandler);

INCLUDE_ASM(const s32, "code_002C81A8", SetVInterruptHandler);

INCLUDE_ASM(const s32, "code_002C81A8", AddIntcHandler);

INCLUDE_ASM(const s32, "code_002C81A8", AddIntcHandler2);

INCLUDE_ASM(const s32, "code_002C81A8", RemoveIntcHandler);

INCLUDE_ASM(const s32, "code_002C81A8", AddDmacHandler);

INCLUDE_ASM(const s32, "code_002C81A8", AddDmacHandler2);

INCLUDE_ASM(const s32, "code_002C81A8", RemoveDmacHandler);

INCLUDE_ASM(const s32, "code_002C81A8", _EnableIntc);

INCLUDE_ASM(const s32, "code_002C81A8", _DisableIntc);

INCLUDE_ASM(const s32, "code_002C81A8", _EnableDmac);

INCLUDE_ASM(const s32, "code_002C81A8", _DisableDmac);

INCLUDE_ASM(const s32, "code_002C81A8", SetAlarm);

INCLUDE_ASM(const s32, "code_002C81A8", ReleaseAlarm);

INCLUDE_ASM(const s32, "code_002C81A8", _iEnableIntc);

INCLUDE_ASM(const s32, "code_002C81A8", _iDisableIntc);

INCLUDE_ASM(const s32, "code_002C81A8", _iEnableDmac);

INCLUDE_ASM(const s32, "code_002C81A8", _iDisableDmac);

INCLUDE_ASM(const s32, "code_002C81A8", iSetAlarm);

INCLUDE_ASM(const s32, "code_002C81A8", iReleaseAlarm);

INCLUDE_ASM(const s32, "code_002C81A8", CreateThread);

INCLUDE_ASM(const s32, "code_002C81A8", libc_2DBCB0);

INCLUDE_ASM(const s32, "code_002C81A8", StartThread);

INCLUDE_ASM(const s32, "code_002C81A8", ExitThread);

INCLUDE_ASM(const s32, "code_002C81A8", ExitDeleteThread);

INCLUDE_ASM(const s32, "code_002C81A8", TerminateThread);

INCLUDE_ASM(const s32, "code_002C81A8", iTerminateThread);

INCLUDE_ASM(const s32, "code_002C81A8", DisableDispatchThread);

INCLUDE_ASM(const s32, "code_002C81A8", EnableDispatchThread);

INCLUDE_ASM(const s32, "code_002C81A8", ChangeThreadPriority);

INCLUDE_ASM(const s32, "code_002C81A8", iChangeThreadPriority);

INCLUDE_ASM(const s32, "code_002C81A8", RotateThreadReadyQueue);

INCLUDE_ASM(const s32, "code_002C81A8", _iRotateThreadReadyQueue);

INCLUDE_ASM(const s32, "code_002C81A8", ReleaseWaitThread);

INCLUDE_ASM(const s32, "code_002C81A8", iReleaseWaitThread);

INCLUDE_ASM(const s32, "code_002C81A8", GetThreadId);

INCLUDE_ASM(const s32, "code_002C81A8", ReferThreadStatus);

INCLUDE_ASM(const s32, "code_002C81A8", iReferThreadStatus);

INCLUDE_ASM(const s32, "code_002C81A8", SleepThread);

INCLUDE_ASM(const s32, "code_002C81A8", WakeupThread);

INCLUDE_ASM(const s32, "code_002C81A8", _iWakeupThread);

INCLUDE_ASM(const s32, "code_002C81A8", CancelWakeupThread);

INCLUDE_ASM(const s32, "code_002C81A8", iCancelWakeupThread);

INCLUDE_ASM(const s32, "code_002C81A8", SuspendThread);

INCLUDE_ASM(const s32, "code_002C81A8", _iSuspendThread);

INCLUDE_ASM(const s32, "code_002C81A8", ResumeThread);

INCLUDE_ASM(const s32, "code_002C81A8", iResumeThread);

INCLUDE_ASM(const s32, "code_002C81A8", JoinThread);

INCLUDE_ASM(const s32, "code_002C81A8", RFU060);

INCLUDE_ASM(const s32, "code_002C81A8", RFU061);

INCLUDE_ASM(const s32, "code_002C81A8", EndOfHeap);

INCLUDE_ASM(const s32, "code_002C81A8", RFU063);

INCLUDE_ASM(const s32, "code_002C81A8", CreateSema);

INCLUDE_ASM(const s32, "code_002C81A8", DeleteSema);

INCLUDE_ASM(const s32, "code_002C81A8", SignalSema);

INCLUDE_ASM(const s32, "code_002C81A8", iSignalSema);

INCLUDE_ASM(const s32, "code_002C81A8", WaitSema);

INCLUDE_ASM(const s32, "code_002C81A8", PollSema);

INCLUDE_ASM(const s32, "code_002C81A8", iPollSema);

INCLUDE_ASM(const s32, "code_002C81A8", ReferSemaStatus);

INCLUDE_ASM(const s32, "code_002C81A8", iReferSemaStatus);

INCLUDE_ASM(const s32, "code_002C81A8", RFU073);

INCLUDE_ASM(const s32, "code_002C81A8", SetOsdConfigParam);

INCLUDE_ASM(const s32, "code_002C81A8", GetOsdConfigParam);

INCLUDE_ASM(const s32, "code_002C81A8", GetGsHParam);

INCLUDE_ASM(const s32, "code_002C81A8", GetGsVParam);

INCLUDE_ASM(const s32, "code_002C81A8", SetGsHParam);

INCLUDE_ASM(const s32, "code_002C81A8", SetGsVParam);

INCLUDE_ASM(const s32, "code_002C81A8", RFU080_CreateEventFlag);

INCLUDE_ASM(const s32, "code_002C81A8", RFU081_DeleteEventFlag);

INCLUDE_ASM(const s32, "code_002C81A8", RFU082_SetEventFlag);

INCLUDE_ASM(const s32, "code_002C81A8", RFU083_iSetEventFlag);

INCLUDE_ASM(const s32, "code_002C81A8", RFU084_ClearEventFlag);

INCLUDE_ASM(const s32, "code_002C81A8", RFU085_iClearEventFlag);

INCLUDE_ASM(const s32, "code_002C81A8", RFU086_WaitEvnetFlag);

INCLUDE_ASM(const s32, "code_002C81A8", RFU087_PollEvnetFlag);

INCLUDE_ASM(const s32, "code_002C81A8", RFU088_iPollEvnetFlag);

INCLUDE_ASM(const s32, "code_002C81A8", RFU089_ReferEventFlagStatus);

INCLUDE_ASM(const s32, "code_002C81A8", RFU090_iReferEventFlagStatus);

INCLUDE_ASM(const s32, "code_002C81A8", RFU091);

INCLUDE_ASM(const s32, "code_002C81A8", EnableIntcHandler);

INCLUDE_ASM(const s32, "code_002C81A8", iEnableIntcHandler);

INCLUDE_ASM(const s32, "code_002C81A8", DisableIntcHandler);

INCLUDE_ASM(const s32, "code_002C81A8", iDisableIntcHandler);

INCLUDE_ASM(const s32, "code_002C81A8", EnableDmacHandler);

INCLUDE_ASM(const s32, "code_002C81A8", iEnableDmacHandler);

INCLUDE_ASM(const s32, "code_002C81A8", DisableDmacHandler);

INCLUDE_ASM(const s32, "code_002C81A8", iDisableDmacHandler);

INCLUDE_ASM(const s32, "code_002C81A8", KSeg0);

INCLUDE_ASM(const s32, "code_002C81A8", EnableCache);

INCLUDE_ASM(const s32, "code_002C81A8", DisableCache);

INCLUDE_ASM(const s32, "code_002C81A8", GetCop0);

INCLUDE_ASM(const s32, "code_002C81A8", FlushCache);

INCLUDE_ASM(const s32, "code_002C81A8", CpuConfig);

INCLUDE_ASM(const s32, "code_002C81A8", iGetCop0);

INCLUDE_ASM(const s32, "code_002C81A8", iFlushCache);

INCLUDE_ASM(const s32, "code_002C81A8", iCpuConfig);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifStopDma);

INCLUDE_ASM(const s32, "code_002C81A8", SetCPUTimerHandler);

INCLUDE_ASM(const s32, "code_002C81A8", SetCPUTimer);

INCLUDE_ASM(const s32, "code_002C81A8", SetOsdConfigParam2);

INCLUDE_ASM(const s32, "code_002C81A8", GetOsdConfigParam2);

INCLUDE_ASM(const s32, "code_002C81A8", GsGetIMR);

INCLUDE_ASM(const s32, "code_002C81A8", iGsGetIMR);

INCLUDE_ASM(const s32, "code_002C81A8", GsPutIMR);

INCLUDE_ASM(const s32, "code_002C81A8", iGsPutIMR);

INCLUDE_ASM(const s32, "code_002C81A8", SetPgifHandler);

INCLUDE_ASM(const s32, "code_002C81A8", SetVSyncFlag);

INCLUDE_ASM(const s32, "code_002C81A8", RFU116);

INCLUDE_ASM(const s32, "code_002C81A8", _print);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifDmaStat);

INCLUDE_ASM(const s32, "code_002C81A8", isceSifDmaStat);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifSetDma);

INCLUDE_ASM(const s32, "code_002C81A8", isceSifSetDma);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifSetDChain);

INCLUDE_ASM(const s32, "code_002C81A8", isceSifSetDChain);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifSetReg);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifGetReg);

INCLUDE_ASM(const s32, "code_002C81A8", libc_2DC2C0);

INCLUDE_ASM(const s32, "code_002C81A8", Deci2Call);

INCLUDE_ASM(const s32, "code_002C81A8", PSMode);

INCLUDE_ASM(const s32, "code_002C81A8", MachineType);

INCLUDE_ASM(const s32, "code_002C81A8", GetMemorySize);

INCLUDE_ASM(const s32, "code_002C81A8", _InitTLB);

INCLUDE_ASM(const s32, "code_002C81A8", sceResetttyinit);

INCLUDE_ASM(const s32, "code_002C81A8", VSync);

INCLUDE_ASM(const s32, "code_002C81A8", VSync2);

INCLUDE_ASM(const s32, "code_002C81A8", write);

INCLUDE_ASM(const s32, "code_002C81A8", read);

INCLUDE_ASM(const s32, "code_002C81A8", open);

INCLUDE_ASM(const s32, "code_002C81A8", close);

INCLUDE_ASM(const s32, "code_002C81A8", ioctl);

INCLUDE_ASM(const s32, "code_002C81A8", lseek);

INCLUDE_ASM(const s32, "code_002C81A8", sbrk);

INCLUDE_ASM(const s32, "code_002C81A8", isatty);

INCLUDE_ASM(const s32, "code_002C81A8", fstat);

INCLUDE_ASM(const s32, "code_002C81A8", libc_2DC670);

INCLUDE_ASM(const s32, "code_002C81A8", kill);

INCLUDE_ASM(const s32, "code_002C81A8", stat);

INCLUDE_ASM(const s32, "code_002C81A8", unlink);

INCLUDE_ASM(const s32, "code_002C81A8", _sceSDC);

INCLUDE_ASM(const s32, "code_002C81A8", SyncDCache);

INCLUDE_ASM(const s32, "code_002C81A8", iSyncDCache);

INCLUDE_ASM(const s32, "code_002C81A8", _sceIDC);

INCLUDE_ASM(const s32, "code_002C81A8", InvalidDCache);

INCLUDE_ASM(const s32, "code_002C81A8", iInvalidDCache);

INCLUDE_ASM(const s32, "code_002C81A8", func_002DC970);

INCLUDE_ASM(const s32, "code_002C81A8", DisableIntc);

INCLUDE_ASM(const s32, "code_002C81A8", EnableIntc);

INCLUDE_ASM(const s32, "code_002C81A8", DisableDmac);

INCLUDE_ASM(const s32, "code_002C81A8", EnableDmac);

INCLUDE_ASM(const s32, "code_002C81A8", iEnableIntc);

INCLUDE_ASM(const s32, "code_002C81A8", iDisableIntc);

INCLUDE_ASM(const s32, "code_002C81A8", iEnableDmac);

INCLUDE_ASM(const s32, "code_002C81A8", iDisableDmac);

INCLUDE_ASM(const s32, "code_002C81A8", topThread);

INCLUDE_ASM(const s32, "code_002C81A8", InitThread);

INCLUDE_ASM(const s32, "code_002C81A8", iWakeupThread);

INCLUDE_ASM(const s32, "code_002C81A8", iRotateThreadReadyQueue);

INCLUDE_ASM(const s32, "code_002C81A8", iSuspendThread);

INCLUDE_ASM(const s32, "code_002C81A8", gcc2_compiled__10);

INCLUDE_ASM(const s32, "code_002C81A8", QueuePeekWriteDone);

INCLUDE_ASM(const s32, "code_002C81A8", QueuePeekReadDone);

INCLUDE_ASM(const s32, "code_002C81A8", sceTtyHandler);

INCLUDE_ASM(const s32, "code_002C81A8", sceTtyWrite);

INCLUDE_ASM(const s32, "code_002C81A8", sceTtyRead);

INCLUDE_ASM(const s32, "code_002C81A8", sceTtyInit);

INCLUDE_ASM(const s32, "code_002C81A8", kputchar);

INCLUDE_ASM(const s32, "code_002C81A8", deci2Putchar);

INCLUDE_ASM(const s32, "code_002C81A8", serialPutchar);

INCLUDE_ASM(const s32, "code_002C81A8", ftoi);

INCLUDE_ASM(const s32, "code_002C81A8", printfloat);

INCLUDE_ASM(const s32, "code_002C81A8", _printf);

INCLUDE_ASM(const s32, "code_002C81A8", kprintf);

INCLUDE_ASM(const s32, "code_002C81A8", scePrintf);

INCLUDE_ASM(const s32, "code_002C81A8", gcc2_compiled__12);

INCLUDE_ASM(const s32, "code_002C81A8", _change_addr);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifGetSreg);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifSetSreg);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifGetDataTable);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifInitCmd);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifExitCmd);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifSetCmdBuffer);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifSetSysCmdBuffer);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifAddCmdHandler);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifRemoveCmdHandler);

INCLUDE_ASM(const s32, "code_002C81A8", _sceSifSendCmd);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifSendCmd);

INCLUDE_ASM(const s32, "code_002C81A8", isceSifSendCmd);

INCLUDE_ASM(const s32, "code_002C81A8", _sceSifCmdIntrHdlr);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifWriteBackDCache);

INCLUDE_ASM(const s32, "code_002C81A8", func_002DE504);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifInitRpc);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifExitRpc);

INCLUDE_ASM(const s32, "code_002C81A8", _sceRpcGetPacket_0);

INCLUDE_ASM(const s32, "code_002C81A8", _sceRpcFreePacket_0);

INCLUDE_ASM(const s32, "code_002C81A8", _sceRpcGetFPacket);

INCLUDE_ASM(const s32, "code_002C81A8", _sceRpcGetFPacket2);

INCLUDE_ASM(const s32, "code_002C81A8", _request_end_0);

INCLUDE_ASM(const s32, "code_002C81A8", _request_rdata);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifGetOtherData);

INCLUDE_ASM(const s32, "code_002C81A8", _search_svdata);

INCLUDE_ASM(const s32, "code_002C81A8", _request_bind);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifBindRpc);

INCLUDE_ASM(const s32, "code_002C81A8", _request_call);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifCallRpc);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifCheckStatRpc);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifSetRpcQueue);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifRegisterRpc);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifRemoveRpc);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifRemoveRpcQueue);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifGetNextRequest);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifExecRequest);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifRpcLoop);

INCLUDE_ASM(const s32, "code_002C81A8", gcc2_compiled__13);

INCLUDE_ASM(const s32, "code_002C81A8", new_iob);

INCLUDE_ASM(const s32, "code_002C81A8", get_iob);

INCLUDE_ASM(const s32, "code_002C81A8", _sceFs_Rcv_Intr);

INCLUDE_ASM(const s32, "code_002C81A8", _sceFsSemInit);

INCLUDE_ASM(const s32, "code_002C81A8", _sceFsWaitS);

INCLUDE_ASM(const s32, "code_002C81A8", _sceFsSigSema);

INCLUDE_ASM(const s32, "code_002C81A8", scePowerOffHandler);

INCLUDE_ASM(const s32, "code_002C81A8", _sceFs_Poff_Intr);

INCLUDE_ASM(const s32, "code_002C81A8", sceFsInit);

INCLUDE_ASM(const s32, "code_002C81A8", _fs_version);

INCLUDE_ASM(const s32, "code_002C81A8", sceFsReset);

INCLUDE_ASM(const s32, "code_002C81A8", sceOpen);

INCLUDE_ASM(const s32, "code_002C81A8", sceClose);

INCLUDE_ASM(const s32, "code_002C81A8", sceLseek);

INCLUDE_ASM(const s32, "code_002C81A8", sceRead);

INCLUDE_ASM(const s32, "code_002C81A8", sceWrite);

INCLUDE_ASM(const s32, "code_002C81A8", sceIoctl);

INCLUDE_ASM(const s32, "code_002C81A8", sceIoctl2);

INCLUDE_ASM(const s32, "code_002C81A8", _sceCallCode);

INCLUDE_ASM(const s32, "code_002C81A8", sceRemove);

INCLUDE_ASM(const s32, "code_002C81A8", sceMkdir);

INCLUDE_ASM(const s32, "code_002C81A8", sceRmdir);

INCLUDE_ASM(const s32, "code_002C81A8", sceFormat);

INCLUDE_ASM(const s32, "code_002C81A8", sceAddDrv);

INCLUDE_ASM(const s32, "code_002C81A8", sceDelDrv);

INCLUDE_ASM(const s32, "code_002C81A8", sceDopen);

INCLUDE_ASM(const s32, "code_002C81A8", sceDclose);

INCLUDE_ASM(const s32, "code_002C81A8", sceDread);

INCLUDE_ASM(const s32, "code_002C81A8", sceGetstat);

INCLUDE_ASM(const s32, "code_002C81A8", sceChstat);

INCLUDE_ASM(const s32, "code_002C81A8", sceRename);

INCLUDE_ASM(const s32, "code_002C81A8", sceChdir);

INCLUDE_ASM(const s32, "code_002C81A8", sceSync);

INCLUDE_ASM(const s32, "code_002C81A8", sceMount);

INCLUDE_ASM(const s32, "code_002C81A8", sceUmount);

INCLUDE_ASM(const s32, "code_002C81A8", sceLseek64);

INCLUDE_ASM(const s32, "code_002C81A8", sceDevctl);

INCLUDE_ASM(const s32, "code_002C81A8", sceSymlink);

INCLUDE_ASM(const s32, "code_002C81A8", sceReadlink);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifInitIopHeap);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifAllocIopHeap);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifAllocSysMemory);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifFreeSysMemory);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifFreeIopHeap);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifLoadIopHeap);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifQueryMemSize);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifQueryMaxFreeMemSize);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifQueryTotalFreeMemSize);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifQueryBlockTopAddress);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifQueryBlockSize);

INCLUDE_ASM(const s32, "code_002C81A8", _lf_bind);

INCLUDE_ASM(const s32, "code_002C81A8", _lf_version);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifLoadFileReset);

INCLUDE_ASM(const s32, "code_002C81A8", _sceSifLoadModuleBuffer);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifStopModule);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifUnloadModule);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifSearchModuleByName);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifSearchModuleByAddress);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifLoadModuleBuffer);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifLoadStartModuleBuffer);

INCLUDE_ASM(const s32, "code_002C81A8", _sceSifLoadModule);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifLoadModule);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifLoadStartModule);

INCLUDE_ASM(const s32, "code_002C81A8", _sceSifLoadElfPart);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifLoadElfPart);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifLoadElf);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifGetIopAddr);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifSetIopAddr);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifResetIop);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifIsAliveIop);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifSyncIop);

INCLUDE_ASM(const s32, "code_002C81A8", sceSifRebootIop);

INCLUDE_ASM(const s32, "code_002C81A8", DIntr);

INCLUDE_ASM(const s32, "code_002C81A8", EIntr);

INCLUDE_ASM(const s32, "code_002C81A8", supplement_crt0);

INCLUDE_ASM(const s32, "code_002C81A8", Copy);

INCLUDE_ASM(const s32, "code_002C81A8", kCopy);

INCLUDE_ASM(const s32, "code_002C81A8", kFindAddress);

INCLUDE_ASM(const s32, "code_002C81A8", FindAddress);

INCLUDE_ASM(const s32, "code_002C81A8", GetSystemCallEntry);

INCLUDE_ASM(const s32, "code_002C81A8", GetSystemCallTableEntry);

INCLUDE_ASM(const s32, "code_002C81A8", setup);

INCLUDE_ASM(const s32, "code_002C81A8", _InitSys);

INCLUDE_ASM(const s32, "code_002C81A8", gcc2_compiled__15);

INCLUDE_ASM(const s32, "code_002C81A8", Copy_0);

INCLUDE_ASM(const s32, "code_002C81A8", kCopy_0);

INCLUDE_ASM(const s32, "code_002C81A8", GetEntryAddress);

INCLUDE_ASM(const s32, "code_002C81A8", PatchIsNeeded_0);

INCLUDE_ASM(const s32, "code_002C81A8", InitExecPS2);

INCLUDE_ASM(const s32, "code_002C81A8", gcc2_compiled__16);

INCLUDE_ASM(const s32, "code_002C81A8", kCopy_1);

INCLUDE_ASM(const s32, "code_002C81A8", PatchIsNeeded);

INCLUDE_ASM(const s32, "code_002C81A8", SetArg);

INCLUDE_ASM(const s32, "code_002C81A8", TerminateLibrary);

INCLUDE_ASM(const s32, "code_002C81A8", ExecPS2);

INCLUDE_ASM(const s32, "code_002C81A8", LoadExecPS2);

INCLUDE_ASM(const s32, "code_002C81A8", Exit);

INCLUDE_ASM(const s32, "code_002C81A8", ExecOSD);

INCLUDE_ASM(const s32, "code_002C81A8", gcc2_compiled__17);

INCLUDE_ASM(const s32, "code_002C81A8", Copy_1);

INCLUDE_ASM(const s32, "code_002C81A8", kCopy_2);

INCLUDE_ASM(const s32, "code_002C81A8", GetEntryAddress_0);

INCLUDE_ASM(const s32, "code_002C81A8", InitAlarm);

INCLUDE_ASM(const s32, "code_002C81A8", sceDeci2Open);

INCLUDE_ASM(const s32, "code_002C81A8", sceDeci2Close);

INCLUDE_ASM(const s32, "code_002C81A8", sceDeci2ReqSend);

INCLUDE_ASM(const s32, "code_002C81A8", sceDeci2Poll);

INCLUDE_ASM(const s32, "code_002C81A8", sceDeci2ExRecv);

INCLUDE_ASM(const s32, "code_002C81A8", sceDeci2ExSend);

INCLUDE_ASM(const s32, "code_002C81A8", sceDeci2ExReqSend);

INCLUDE_ASM(const s32, "code_002C81A8", sceDeci2ExLock);

INCLUDE_ASM(const s32, "code_002C81A8", sceDeci2ExUnLock);

INCLUDE_ASM(const s32, "code_002C81A8", kputs);

INCLUDE_ASM(const s32, "code_002C81A8", SetTLBHandler);

INCLUDE_ASM(const s32, "code_002C81A8", SetDebugHandler);

INCLUDE_ASM(const s32, "code_002C81A8", Copy_2);

INCLUDE_ASM(const s32, "code_002C81A8", kCopy_3);

INCLUDE_ASM(const s32, "code_002C81A8", GetEntryAddress_1);

INCLUDE_ASM(const s32, "code_002C81A8", setup_0);

INCLUDE_ASM(const s32, "code_002C81A8", InitTLBFunctions);

INCLUDE_ASM(const s32, "code_002C81A8", PutTLBEntry);

INCLUDE_ASM(const s32, "code_002C81A8", iPutTLBEntry_0);

INCLUDE_ASM(const s32, "code_002C81A8", _SetTLBEntry);

INCLUDE_ASM(const s32, "code_002C81A8", iPutTLBEntry);

INCLUDE_ASM(const s32, "code_002C81A8", iSetTLBEntry);

INCLUDE_ASM(const s32, "code_002C81A8", GetTLBEntry);

INCLUDE_ASM(const s32, "code_002C81A8", iGetTLBEntry);

INCLUDE_ASM(const s32, "code_002C81A8", ProbeTLBEntry);

INCLUDE_ASM(const s32, "code_002C81A8", iProbeTLBEntry);

INCLUDE_ASM(const s32, "code_002C81A8", ExpandScratchPad);

INCLUDE_ASM(const s32, "code_002C81A8", InitTLB);

INCLUDE_ASM(const s32, "code_002C81A8", InitTLB32MB);

INCLUDE_ASM(const s32, "code_002C81A8", _kTLBException);

INCLUDE_ASM(const s32, "code_002C81A8", _xlaunch);

INCLUDE_ASM(const s32, "code_002C81A8", _kDebugException);

INCLUDE_ASM(const s32, "code_002C81A8", libc_2E52FC);

INCLUDE_ASM(const s32, "code_002C81A8", libc_2E5320);

INCLUDE_ASM(const s32, "code_002C81A8", libc_2E59B8);

INCLUDE_ASM(const s32, "code_002C81A8", libc_2E6048);

INCLUDE_ASM(const s32, "code_002C81A8", libc_2E65C8);

INCLUDE_ASM(const s32, "code_002C81A8", libc_2E6B30);

INCLUDE_ASM(const s32, "code_002C81A8", libc_2E6C48);

INCLUDE_ASM(const s32, "code_002C81A8", libc_2E6D88);

INCLUDE_ASM(const s32, "code_002C81A8", __errno_8);

