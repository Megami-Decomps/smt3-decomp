#include "common.h"
#include "ps2asm.h"

enum {
    SDF_TMX_MAGIC = 0x30584D54,
    SDF_STREAM_CHUNK_BYTES = 0x4000,
    SDF_STREAM_COMMAND_CREATE_STATE = 32,
    SDF_STREAM_COMMAND_CONTROL = 33,
    SDF_STREAM_COMMAND_READ = 34,
    SDF_STREAM_COMMAND_RELEASE_STATE = 35,
    SDF_IOP_MODULES_PER_GROUP = 2,
    SDF_KEY_TREE_PATH_LIMIT = 32,
    SDF_POOL_REBUILD_CHAIN = 0,
    SDF_POOL_PUSH_FREE = 1
};


struct sdfDrawTag_t;

struct sdfSortOtNode_tag;

struct sdfGrOt_t;

#define SDF_POOL_FREE_KIND 0xFFFF

extern void StartThread();

extern void SleepThread(void);

extern s32 sceSifLoadModule(const char *, s32, const char *);

extern char *strcpy(char *, const char *);

extern char *strcat(char *, const char *);

extern void sceMcInit(void);

extern void sdfGrJoinDrawTag(void *aFore, void *aBack);

/* Pool release logic tests the packet head address and kind at 0xC;
 * bytes at 0x8 remain opaque. The float-key tree insertion helper reads the
 * same +0xC word through sdfDrawTag_t. DDS1/2 game/code_002EEA18/003478C0.c. */
typedef struct sdfDrawTag_t {
    struct sdfDrawTag_t *next; /* 0x00 */
    s32 head;       /* 0x04: first DMA packet address. */
    u8 tail[4];                  /* 0x08 */
    f32 z;                    /* 0x0C */
} sdfDrawTag_t;

typedef struct sdfSortOtNode_tag {
    struct sdfSortOtNode_tag *right; /* 0x00 */
    struct sdfSortOtNode_tag *left;  /* 0x04 */
    struct sdfDrawTag_t *tag;  /* 0x08 */
    s32 balance;                  /* 0x0C */
} sdfSortOtNode_tag;

/* A key-sorted draw-surface pool: a draw tag whose append and prepend
 * callbacks release a node into the pool and update its free list. */
typedef struct sdfGrOt_t {
    void *next;                                                /* 0x00: the pool entry's link */
    sdfDrawTag_t *tagHead;                                     /* 0x04: first node in the emitted chain */
    sdfDrawTag_t *tagTail;                                     /* 0x08: last node in the emitted chain */
    sdfDrawTag_t *noSortTag;                                   /* 0x0C: the free list */
    void (*regFunc)(struct sdfGrOt_t *, sdfDrawTag_t *);       /* 0x10 */
    void (*callbackFunc)(struct sdfGrOt_t *, s32, sdfDrawTag_t *); /* 0x14 */
    sdfSortOtNode_tag *data[1];                                /* 0x18: first root; allocation continues */
} sdfGrOt_t;

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */
s32 sdfGrAllocPacketBuff(s32 aSize);

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

sdfSortOtNode_tag *sdfSortOtAvlRotateLR(sdfSortOtNode_tag *node, sdfSortOtNode_tag *parent);

sdfSortOtNode_tag *sdfSortOtAvlRotateRL(sdfSortOtNode_tag *node, sdfSortOtNode_tag *parent);

void sdfSortOtAvlCheckBalance(sdfSortOtNode_tag **stack, s32 stack_depth, sdfSortOtNode_tag *node,
                   sdfSortOtNode_tag **root_p);

void sdfSortOtAvlRegist(sdfSortOtNode_tag **root_p, sdfDrawTag_t *tag);

void sdfGrSortOtReg(sdfGrOt_t *aOt, sdfDrawTag_t *aTag);

void sdfGrSortOtFixup(void *tag, sdfDrawTag_t **head_p, sdfDrawTag_t **tail_p);

void sdfGrSortOtTraverse(sdfSortOtNode_tag *node, sdfDrawTag_t **head_p, sdfDrawTag_t **tail_p);

void sdfGrSortOtSetup(sdfGrOt_t *aOt);

void sdfGrSortOtCallback(sdfGrOt_t *aOt, s32 aFunc, sdfDrawTag_t *aData);

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

extern void *memset(void *a, s32 b, u32 c);

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

INCLUDE_ASM(const s32, "sdk/libc", gcc2_compiled_);

INCLUDE_ASM(const s32, "sdk/libc", _malloc_r);

INCLUDE_ASM(const s32, "sdk/libc", memcmp);

INCLUDE_ASM(const s32, "sdk/libc", memcpy);

INCLUDE_ASM(const s32, "sdk/libc", memmove);

INCLUDE_ASM(const s32, "sdk/libc", memset);

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

INCLUDE_RODATA(const s32, "sdk/libc", D_0054B550);

INCLUDE_RODATA(const s32, "sdk/libc", D_0054B658);

INCLUDE_ASM(const s32, "sdk/libc", _puts_r);

s32 puts(const char *s) {
    return _puts_r(D_0052BE28, s);
}

INCLUDE_ASM(const s32, "sdk/libc", qsort);

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

INCLUDE_ASM(const s32, "sdk/libc", _raise_r);

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

INCLUDE_ASM(const s32, "sdk/libc", strcasecmp);

INCLUDE_ASM(const s32, "sdk/libc", strcat);

INCLUDE_ASM(const s32, "sdk/libc", strchr);

INCLUDE_ASM(const s32, "sdk/libc", strcmp);

INCLUDE_ASM(const s32, "sdk/libc", strcpy);

INCLUDE_ASM(const s32, "sdk/libc", strlen);

INCLUDE_ASM(const s32, "sdk/libc", strncmp);

INCLUDE_ASM(const s32, "sdk/libc", strncpy);

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

INCLUDE_ASM(const s32, "sdk/libc", strstr);

INCLUDE_ASM(const s32, "sdk/libc", _strtol_r);

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

INCLUDE_ASM(const s32, "sdk/libc", __sbprintf);

INCLUDE_RODATA(const s32, "sdk/libc", D_0054B670);

INCLUDE_RODATA(const s32, "sdk/libc", D_0054B680);

INCLUDE_ASM(const s32, "sdk/libc", vfprintf);

INCLUDE_ASM(const s32, "sdk/libc", cvt);

INCLUDE_ASM(const s32, "sdk/libc", exponent);

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

INCLUDE_ASM(const s32, "sdk/libc", __swsetup);

INCLUDE_ASM(const s32, "sdk/libc", gcc2_compiled__1);

INCLUDE_RODATA(const s32, "sdk/libc", D_0054B880);

INCLUDE_RODATA(const s32, "sdk/libc", D_0054B890);

INCLUDE_ASM(const s32, "sdk/libc", _dtoa_r);

INCLUDE_ASM(const s32, "sdk/libc", fflush);

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

INCLUDE_ASM(const s32, "sdk/libc", __sfp);

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

INCLUDE_ASM(const s32, "sdk/libc", _free_r);

INCLUDE_ASM(const s32, "sdk/libc", _malloc_trim_r);

INCLUDE_ASM(const s32, "sdk/libc", __sfvwrite);

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

INCLUDE_RODATA(const s32, "sdk/libc", D_0054B8A0);

INCLUDE_RODATA(const s32, "sdk/libc", D_0054B8A8);

INCLUDE_RODATA(const s32, "sdk/libc", D_0054B8B0);

INCLUDE_RODATA(const s32, "sdk/libc", jtbl_0054B8C0);

INCLUDE_RODATA(const s32, "sdk/libc", D_0054B8D8);

INCLUDE_RODATA(const s32, "sdk/libc", D_0054B908);

INCLUDE_RODATA(const s32, "sdk/libc", D_0054B910);

INCLUDE_ASM(const s32, "sdk/libc", _setlocale_r);

void *func_002DA308(_reent *r) {
    return D_0054B8D8;
}

s32 setlocale(s32 category, const char *locale) {
    return _setlocale_r(D_0052BE28, category, locale);
}

void *localeconv(void) {
    return func_002DA308(D_0052BE28);
}

INCLUDE_ASM(const s32, "sdk/libc", __smakebuf);

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

INCLUDE_ASM(const s32, "sdk/libc", memchr);

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

INCLUDE_ASM(const s32, "sdk/libc", _multadd);

INCLUDE_ASM(const s32, "sdk/libc", _s2b);

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

INCLUDE_ASM(const s32, "sdk/libc", _multiply);

INCLUDE_ASM(const s32, "sdk/libc", _pow5mult);

INCLUDE_ASM(const s32, "sdk/libc", _lshift);

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

INCLUDE_ASM(const s32, "sdk/libc", __mdiff);

INCLUDE_ASM(const s32, "sdk/libc", _ulp);

INCLUDE_ASM(const s32, "sdk/libc", _b2d);

INCLUDE_ASM(const s32, "sdk/libc", _d2b);

INCLUDE_ASM(const s32, "sdk/libc", _ratio);

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

INCLUDE_ASM(const s32, "sdk/libc", isinf);

INCLUDE_ASM(const s32, "sdk/libc", isnan);

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

INCLUDE_ASM(const s32, "sdk/libc", _calloc_r);

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

INCLUDE_RODATA(const s32, "sdk/libc", D_0054B91C);

INCLUDE_RODATA(const s32, "sdk/libc", D_0054B928);

INCLUDE_RODATA(const s32, "sdk/libc", D_0054B930);

INCLUDE_RODATA(const s32, "sdk/libc", D_0054B9F8);

INCLUDE_RODATA(const s32, "sdk/libc", D_0054BA18);

INCLUDE_RODATA(const s32, "sdk/libc", D_0054BA48);

INCLUDE_RODATA(const s32, "sdk/libc", D_0054BA70);

INCLUDE_RODATA(const s32, "sdk/libc", D_0054BA98);

INCLUDE_RODATA(const s32, "sdk/libc", D_0054BAB0);

INCLUDE_RODATA(const s32, "sdk/libc", D_0054BAC8);

INCLUDE_RODATA(const s32, "sdk/libc", D_0054BAE0);

INCLUDE_RODATA(const s32, "sdk/libc", D_0054BAE8);

INCLUDE_RODATA(const s32, "sdk/libc", D_0054BAF0);

INCLUDE_RODATA(const s32, "sdk/libc", D_0054BAF8);

INCLUDE_RODATA(const s32, "sdk/libc", D_0054BB00);

INCLUDE_RODATA(const s32, "sdk/libc", D_0054BB08);

