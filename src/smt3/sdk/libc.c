#include "common.h"
#include "newlib_stat.h"
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

extern void _StartThread();

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

/* libgcc frame-dwarf2.c's object list node. __register_frame_info stores the
 * FDE base and clears the rest; __register_frame allocates one of these. */
typedef struct object {
    void *pc_begin;
    void *pc_end;
    void *fde_begin;
    void *fde_array;
    void *count;
    struct object *next;
} object;

/* libgcc's fde_vector, as add_fdes fills it. */
typedef struct fde_vector {
    u32 **array;
    s32 count;
} fde_vector;

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

extern _reent *_impure_ptr __attribute__((section(".data")));

extern s32 _puts_r(_reent *r, const char *s);

extern s32 _raise_r(_reent *r, s32 sig);

extern void *_signal_r(_reent *r, s32 sig, void *handler);

extern char *_setlocale_r(_reent *r, s32 category, const char *locale);

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

extern void __sinit(_reent *r);

extern void __smakebuf(FILE *fp);

extern s32 __sigtramp_r(_reent *r, s32 sig);

extern void *__deregister_frame_info(void *fn);

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

extern s32 __register_frame_info(void *fn, void *entry);

extern s32 __register_frame_info_table(void *fn, void *entry);

extern s32 write(s32 fd, const void *buf, s32 count);

extern s32 read(s32 fd, void *buf, s32 count);

extern long lseek(s32 fd, s32 offset, s32 whence);

extern s32 fstat(s32 fd, struct stat *buf);

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

extern s32 __do_global_ctors(void);

extern s32 atexit(void (*fn)(void));

extern void (**D_0052BB28)(void) __attribute__((section(".data")));

extern s32 D_0052BB2C __attribute__((section(".data")));

extern s32 D_0052C37C __attribute__((section(".data")));

extern u8 D_0052D940[] __attribute__((section(".data")));

extern void (*D_002F5600[])(void) __attribute__((section(".data")));

extern s32 D_0059AD58 __attribute__((section(".data")));

extern object *D_0059AD60 __attribute__((section(".data")));

typedef struct fp_number_type {
    s32 class;
    s32 sign;
    s32 normal_exp;
    u32 fraction;
} fp_number_type;

extern void sdfUnpackFloat(const void *x, fp_number_type *u);

extern f32 sdfPackFloat(const fp_number_type *u);

extern long __make_dp(s32 kind, s32 sign, s32 exp, long mant);

#define va_start(ap, last) __builtin_stdarg_start((ap), last)

extern s32 func_002D6678(_reent *r, FILE *fp, const char *fmt, va_list ap);

typedef union f32_bits {
    f32 f;
    u32 u;
} f32_bits;

/* The same record from the double-precision build of fp-bit.c. */
typedef struct fp_number_type_df {
    s32 class;
    s32 sign;
    s32 normal_exp;
    long fraction;
} fp_number_type_df;

extern long __pack_df(const fp_number_type_df *u);

extern void sdfUnpackDouble(const double *x, fp_number_type_df *u);

extern fp_number_type_df *libc_2D1590(const fp_number_type_df *a, const fp_number_type_df *b, fp_number_type_df *r);

extern long __fpcmp_parts_df(const fp_number_type_df *a, const fp_number_type_df *b);


void abort(void) {
    while (1) {
        raise(6);
        func_001001C8(1);
    }
}

s32 atexit(void (*fn)(void)) {
    _atexit *p;

    p = _impure_ptr->_atexit;
    if (p == NULL) {
        p = &_impure_ptr->_atexit0;
        _impure_ptr->_atexit = p;
    }
    if (p->_ind >= 32) {
        if ((p = malloc(sizeof(_atexit))) == NULL) {
            return -1;
        }
        p->_ind = 0;
        p->_next = _impure_ptr->_atexit;
        _impure_ptr->_atexit = p;
    }
    p->_fns[p->_ind++] = fn;
    return 0;
}

s32 atoi(const char *s) {
    return strtol(s, NULL, 10);
}

void *malloc(u32 size) {
    void *result;

    __malloc_lock(_impure_ptr);
    result = _malloc_r(_impure_ptr, size);
    __malloc_unlock(_impure_ptr);
    return result;
}

void free(void *ptr) {
    __malloc_lock(_impure_ptr);
    _free_r(_impure_ptr, ptr);
    __malloc_unlock(_impure_ptr);
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
    _reent *r = _impure_ptr;
    va_list ap;

    va_start(ap, fmt);
    r->_stdout->_data = r;
    return vfprintf(r->_stdout, fmt, ap);
}

extern u32 strlen(const char *s);
/* The newline owns a padded 16-byte read-only block in retail. */
extern const char D_0054B660[];
s32 _puts_r(_reent *r, const char *s)
{
    __suio request;
    __siov vectors[2];
    u32 length = strlen(s);

    vectors[0].iov_base = (void *)s;
    vectors[0].iov_len = length;
    vectors[1].iov_base = (void *)D_0054B660;
    vectors[1].iov_len = 1;
    request.uio_resid = length + 1;
    request.uio_iov = vectors;
    request.uio_iovcnt = 2;
    return __sfvwrite(r->_stdout, &request) != 0 ? -1 : 10;
}

s32 puts(const char *s) {
    return _puts_r(_impure_ptr, s);
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

extern s32 _getpid_r(_reent *r);
extern s32 _kill_r(_reent *r, s32 pid, s32 sig);

s32 _raise_r(_reent *r, s32 sig)
{
    void (*handler)(s32);

    if ((u32)sig >= 32) {
        r->_errno = 22;
        return -1;
    }
    if (r->_sig_func == NULL && _init_signal_r(r) != 0) {
        return -1;
    }
    handler = r->_sig_func[sig];
    if (handler == NULL) {
        return _kill_r(r, _getpid_r(r), sig);
    }
    if (handler == (void (*)(s32))1) {
        return 0;
    }
    if (handler == (void (*)(s32))-1) {
        r->_errno = 22;
        return 1;
    }
    r->_sig_func[sig] = NULL;
    handler(sig);
    return 0;
}

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
    return _raise_r(_impure_ptr, sig);
}

void *signal(s32 sig, void *handler) {
    return _signal_r(_impure_ptr, sig, handler);
}

s32 _init_signal(void) {
    return _init_signal_r(_impure_ptr);
}

s32 __sigtramp(s32 sig) {
    return __sigtramp_r(_impure_ptr, sig);
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
    f._data = _impure_ptr;
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

char *strstr(const char *haystack, const char *needle)
{
    /* Signed byte views preserve the native loads; comparisons use equality. */
    const s8 *s = (const s8 *)haystack;
    const s8 *n = (const s8 *)needle;
    s32 i;
    if (*s == 0)
        return *n == 0 ? (char *)s : 0;
    do {
        i = 0;
        for (;;) {
            if (n[i] == 0)
                return (char *)s;
            if (n[i] != s[i])
                break;
            ++i;
        }
        ++s;
    } while (*s != 0);
    return 0;
}

extern const u8 _ctype_[];

long _strtol_r(_reent *r, const char *input, char **end, s32 base)
{
    const char *s = input;
    unsigned long acc;
    s32 c;
    unsigned long cutoff;
    s32 negative = 0;
    s32 any;
    s32 cutlim;

    do {
        c = *s++;
    } while ((_ctype_ + 1)[c] & 8);
    if (c == '-') {
        negative = 1;
        c = *s++;
    } else if (c == '+') {
        c = *s++;
    }
    if ((base == 0 || base == 16) && c == '0' &&
        (*s == 'x' || *s == 'X')) {
        c = s[1];
        s += 2;
        base = 16;
    }
    if (base == 0)
        base = c == '0' ? 8 : 10;
    cutoff = negative ? 0x8000000000000000UL : 0x7fffffffffffffffUL;
    cutlim = cutoff % (unsigned long)base;
    cutoff /= (unsigned long)base;
    for (acc = 0, any = 0;; c = *s++) {
        if ((_ctype_ + 1)[c] & 4)
            c -= '0';
        else if ((_ctype_ + 1)[c] & 3)
            c -= ((_ctype_ + 1)[c] & 1) ? 'A' - 10 : 'a' - 10;
        else
            break;
        if (c >= base)
            goto done;
        if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim)) {
            any = -1;
        } else {
            any = 1;
            acc *= base;
            acc += c;
        }
    }
done:
    if (any < 0) {
        acc = negative ? 0x8000000000000000UL : 0x7fffffffffffffffUL;
        r->_errno = 34;
    } else if (negative) {
        acc = -acc;
    }
    if (end != 0)
        *end = (char *)(any ? s - 1 : input);
    return acc;
}


long strtol(const char *s, char **end, s32 base) {
    return _strtol_r(_impure_ptr, s, end, base);
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

/* Buffer an unbuffered stream using only the fields the native path initializes. */
s32 __sbprintf(FILE *fp, const char *fmt, va_list ap)
{
    FILE fake;
    u8 buf[1024];
    s32 ret;

    fake._data = fp->_data;
    fake._flags = fp->_flags & ~2;
    fake._file = fp->_file;
    fake._cookie = fp->_cookie;
    fake._write = fp->_write;
    fake._bf._base = fake._p = buf;
    fake._bf._size = fake._w = sizeof(buf);
    fake._lbfsize = 0;
    ret = vfprintf(&fake, fmt, ap);
    if (ret >= 0 && fflush(&fake) != 0) {
        ret = -1;
    }
    if (fake._flags & 0x40) {
        fp->_flags |= 0x40;
    }
    return ret;
}

INCLUDE_RODATA(const s32, "sdk/libc", _ctype_);

INCLUDE_RODATA(const s32, "sdk/libc", D_0054B658);

INCLUDE_RODATA(const s32, "sdk/libc", D_0054B660);

INCLUDE_RODATA(const s32, "sdk/libc", D_0054B670);

INCLUDE_RODATA(const s32, "sdk/libc", D_0054B680);

INCLUDE_ASM(const s32, "sdk/libc", vfprintf);

INCLUDE_ASM(const s32, "sdk/libc", cvt);

s32 exponent(char *output, s32 value, s32 marker)
{
    /* The reverse-digit cursor ends at stack + 0x134 in the 0x140-byte frame. */
    char digits[0x134];
    char *cursor = output;
    char *digit;

    *cursor++ = marker;
    if (value < 0) {
        value = -value;
        *cursor++ = '-';
    } else {
        *cursor++ = '+';
    }
    digit = digits + sizeof(digits);
    if (value > 9) {
        while (value > 9) {
            *--digit = value % 10 + '0';
            value /= 10;
        }
        *--digit = value + '0';
        while (digit < digits + sizeof(digits)) {
            *cursor++ = *digit++;
        }
    } else {
        *cursor++ = '0';
        *cursor++ = value + '0';
    }
    return cursor - output;
}

s32 vsprintf(char *str, const char *fmt, va_list ap) {
    s32 result;
    FILE f;

    f._flags = 0x208;
    f._bf._base = f._p = (u8 *)str;
    f._bf._size = f._w = 0x7FFFFFFF;
    f._data = _impure_ptr;
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

s32 __swsetup(FILE *fp)
{
    /* Keep attachment and initialization as one phase; see levers/control-flow.md. */
    do {
        if (fp->_data == NULL) fp->_data = _impure_ptr;
        if (!fp->_data->__sdidinit) __sinit(fp->_data);
    } while (0);
    if (!(fp->_flags & 8)) {
        if (!(fp->_flags & 0x10)) return -1;
        if (fp->_flags & 4) {
            if (fp->_ub._base != NULL) {
                if (fp->_ub._base != fp->_ubuf)
                    _free_r(fp->_data, fp->_ub._base);
                fp->_ub._base = NULL;
            }
            fp->_flags &= ~(4 | 0x20);
            fp->_r = 0;
            fp->_p = fp->_bf._base;
        }
        fp->_flags |= 8;
    }
    if (fp->_bf._base == NULL) __smakebuf(fp);
    if (fp->_flags & 1) {
        fp->_w = 0;
        fp->_lbfsize = -fp->_bf._size;
    } else {
        fp->_w = (fp->_flags & 2) ? 0 : fp->_bf._size;
    }
    return 0;
}

INCLUDE_ASM(const s32, "sdk/libc", gcc2_compiled__1);

INCLUDE_RODATA(const s32, "sdk/libc", D_0054B880);

INCLUDE_RODATA(const s32, "sdk/libc", D_0054B890);

INCLUDE_ASM(const s32, "sdk/libc", _dtoa_r);

s32 fflush(void *stream)
{
    FILE *fp = stream;
    u8 *cursor;
    s32 remaining;
    s32 status;

    if (fp == NULL)
        return _fwalk(_impure_ptr, fflush);
    /* Same initialization phase as __swsetup; see levers/control-flow.md. */
    do {
        if (fp->_data == NULL)
            fp->_data = _impure_ptr;
        if (fp->_data->__sdidinit == 0)
            __sinit(fp->_data);
    } while (0);
    /* Live status scratch; see levers/cross-block-locals.md. */
    status = fp->_flags;
    if ((status & 8) == 0 || (cursor = fp->_bf._base) == NULL)
        return 0;
    remaining = fp->_p - cursor;
    fp->_p = cursor;
    fp->_w = (status & 3) ? 0 : fp->_bf._size;
    for (; remaining > 0; remaining -= status, cursor += status) {
        status = fp->_write(fp->_cookie, (char *)cursor, remaining);
        if (status <= 0) {
            fp->_flags |= 0x40;
            return -1;
        }
    }
    return 0;
}

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

FILE *__sfp(_reent *r)
{
    _glue *glue;
    FILE *fp;
    s32 i;

    if (r->__sdidinit == 0) {
        __sinit(r);
    }
    glue = &r->__sglue;
    for (;;) {
        i = glue->niobs;
        fp = glue->iobs;
        for (i--; i >= 0; i--, fp++) {
            if (fp->_flags == 0) {
                /* Keep initialization outside the short search loop. */
                goto found;
            }
        }
        if (glue->next == NULL) {
            glue->next = __sfmoreglue(r, 4);
            if (glue->next == NULL) {
                break;
            }
        }
        glue = glue->next;
    }
    r->_errno = 12;
    return NULL;

found:
    fp->_flags = 1;
    fp->_file = -1;
    fp->_data = r;
    fp->_p = NULL;
    fp->_w = 0;
    fp->_r = 0;
    fp->_bf._base = NULL;
    fp->_bf._size = 0;
    fp->_lbfsize = 0;
    fp->_ub._base = NULL;
    fp->_ub._size = 0;
    fp->_lb._base = NULL;
    fp->_lb._size = 0;
    return fp;
}

void _cleanup_r(_reent *r) {
    _fwalk(r, fflush);
}

void _cleanup(void) {
    _cleanup_r(_impure_ptr);
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

extern void *memmove(void *destination, const void *source, u32 size);
extern void *memchr(const void *source, s32 character, u32 size);

s32 __sfvwrite(FILE *fp, __suio *uio)
{
    u32 length;
    char *p;
    __siov *iov;
    s32 written;
    s32 available;
    s32 newlineKnown;
    s32 newlineDistance;
    char *newline;

    /* One byte count spans the entry and vector phases; see levers/cross-block-locals.md. */
    length = uio->uio_resid;
    if (length == 0)
        return 0;
    if ((fp->_flags & 8) == 0 || fp->_bf._base == 0)
        if (__swsetup(fp) != 0)
            return -1;
    iov = uio->uio_iov;
    length = 0;
    if (fp->_flags & 2) {
        do {
            while (length == 0) {
                p = iov->iov_base;
                length = iov->iov_len;
                iov++;
            }
            written = fp->_write(fp->_cookie, p, length > 1024 ? 1024 : length);
            if (written <= 0)
                goto error;
            p += written;
            length -= written;
        } while ((uio->uio_resid -= written) != 0);
    } else if ((fp->_flags & 1) == 0) {
        do {
            while (length == 0) {
                p = iov->iov_base;
                length = iov->iov_len;
                iov++;
            }
            written = fp->_w;
            if (fp->_flags & 0x200) {
                if (length < written)
                    written = length;
                memmove(fp->_p, p, written);
                fp->_w -= written;
                fp->_p += written;
                written = length;
            } else if (fp->_p > fp->_bf._base && length > written) {
                memmove(fp->_p, p, written);
                fp->_p += written;
                if (fflush(fp) != 0)
                    goto error;
            } else if (length >= (written = fp->_bf._size)) {
                written = fp->_write(fp->_cookie, p, written);
                if (written <= 0)
                    goto error;
            } else {
                written = length;
                memmove(fp->_p, p, written);
                fp->_w -= written;
                fp->_p += written;
            }
            p += written;
            length -= written;
        } while ((uio->uio_resid -= written) != 0);
    } else {
        newlineKnown = 0;
        do {
            if (length == 0) {
                newlineKnown = 0;
                while (length == 0) {
                    p = iov->iov_base;
                    length = iov->iov_len;
                    iov++;
                }
            }
            if (!newlineKnown) {
                newline = memchr(p, '\n', length);
                newlineDistance = newline ? newline + 1 - p : length + 1;
                newlineKnown = 1;
            }
            available = length < newlineDistance ? length : newlineDistance;
            written = fp->_w + fp->_bf._size;
            if (fp->_p > fp->_bf._base && written < available) {
                memmove(fp->_p, p, written);
                fp->_p += written;
                if (fflush(fp) != 0)
                    goto error;
            } else if (available >= (written = fp->_bf._size)) {
                written = fp->_write(fp->_cookie, p, written);
                if (written <= 0)
                    goto error;
            } else {
                written = available;
                memmove(fp->_p, p, written);
                fp->_w -= written;
                fp->_p += written;
            }
            if ((newlineDistance -= written) == 0) {
                if (fflush(fp) != 0)
                    goto error;
                newlineKnown = 0;
            }
            p += written;
            length -= written;
        } while ((uio->uio_resid -= written) != 0);
    }
    return 0;
error:
    fp->_flags |= 0x40;
    return -1;
}


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

extern char D_0054B908[];
extern char D_0054B918[];
extern s32 strcmp(const char *left, const char *right);

char *_setlocale_r(_reent *r, s32 category, const char *locale)
{
    if (locale != NULL) {
        if (strcmp(locale, D_0054B918) != 0 && strcmp(locale, D_0054B908) != 0)
            return NULL;
        r->_current_category = category;
        r->_current_locale = locale;
    }
    return D_0054B918;
}

void *func_002DA308(_reent *r) {
    return D_0054B8D8;
}

char *setlocale(s32 category, const char *locale) {
    return _setlocale_r(_impure_ptr, category, locale);
}

void *localeconv(void) {
    return func_002DA308(_impure_ptr);
}

extern s32 _fstat_r(_reent *r, s32 fd, struct stat *buf);
extern s32 isatty(s32 fd);

void __smakebuf(FILE *fp)
{
    struct stat st;
    u8 *buf;
    s32 size;
    s32 couldBeTty;

    if (fp->_flags & 2) {
        fp->_bf._base = fp->_p = fp->_nbuf;
        fp->_bf._size = 1;
        return;
    }
    if (fp->_file < 0 || _fstat_r(fp->_data, fp->_file, &st) < 0) {
        couldBeTty = 0;
        size = 1024;
        fp->_flags |= 0x800;
    } else {
        couldBeTty = (st.st_mode & 0xF000) == 0x2000;
        size = 1024;
        if ((st.st_mode & 0xF000) == 0x8000 && fp->_seek == __sseek) {
            fp->_flags |= 0x400;
            fp->_blksize = size;
        } else {
            fp->_flags |= 0x800;
        }
    }
    buf = _malloc_r(fp->_data, size);
    if (buf == NULL) {
        fp->_flags |= 2;
        fp->_bf._base = fp->_p = fp->_nbuf;
        fp->_bf._size = 1;
    } else {
        fp->_data->__cleanup = _cleanup_r;
        fp->_flags |= 0x80;
        fp->_bf._base = fp->_p = buf;
        fp->_bf._size = size;
        if (couldBeTty && isatty(fp->_file)) fp->_flags |= 1;
    }
}

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

_Bigint *_multadd(_reent *r, _Bigint *b, s32 m, s32 a)
{
    s32 wds = b->wds;
    u32 *x = b->x;
    s32 i = 0;
    u32 lo;
    u32 hi;
    _Bigint *b1;

    do {
        lo = (*x & 0xffff) * m + a;
        hi = (*x >> 16) * m + (lo >> 16);
        a = hi >> 16;
        *x++ = (hi << 16) + (lo & 0xffff);
    } while (++i < wds);
    if (a != 0) {
        if (wds >= b->maxwds) {
            b1 = _Balloc(r, b->k + 1);
            memcpy(&b1->sign, &b->sign, b->wds * sizeof(u32) + 2 * sizeof(s32));
            _Bfree(r, b);
            b = b1;
        }
        b->x[wds++] = a;
        b->wds = wds;
    }
    return b;
}

/* Ordinary source reconstruction of the newlib decimal-string to Bigint helper. */
_Bigint *_s2b(_reent *r, const char *s, s32 nd0, s32 nd, u32 y9)
{
    _Bigint *b;
    s32 n;
    s32 k;
    s32 i;
    s32 capacity;

    n = (nd + 8) / 9;
    k = 0;
    capacity = 1;
    while (capacity < n) {
        capacity <<= 1;
        k++;
    }

    b = _Balloc(r, k);
    b->x[0] = y9;
    b->wds = 1;

    i = 9;
    if (nd0 > 9) {
        s += 9;
        do {
            b = _multadd(r, b, 10, *s++ - '0');
        } while (++i < nd0);
        s++;
    } else {
        s += 10;
    }

    while (i < nd) {
        b = _multadd(r, b, 10, *s++ - '0');
        ++i;
    }
    return b;
}

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

/* Native indexed constants span the separately labeled word at 0x0054B928. */
extern const s32 D_0054B91C[4];
_Bigint *_multiply(_reent *r, _Bigint *a, _Bigint *b);

_Bigint *_pow5mult(_reent *r, _Bigint *b, s32 n)
{
    _Bigint *power;
    _Bigint *nextPower;
    _Bigint *product;
    s32 remainder;

    remainder = n & 3;
    if (remainder != 0) {
        b = _multadd(r, b, D_0054B91C[remainder], 0);
    }
    n >>= 2;
    if (n == 0) {
        return b;
    }
    power = r->_p5s;
    if (power == NULL) {
        power = r->_p5s = _i2b(r, 625);
        power->next = NULL;
    }
    for (;;) {
        if (n & 1) {
            product = _multiply(r, b, power);
            _Bfree(r, b);
            b = product;
        }
        n >>= 1;
        if (n == 0) {
            break;
        }
        if ((nextPower = power->next) == NULL) {
            nextPower = power->next = _multiply(r, power, power);
            nextPower->next = NULL;
        }
        power = nextPower;
    }
    return b;
}

/* Qualified allocator controls for the two count lifetimes are recorded in
 * docs/levers/cross-block-locals.md, "Retain bigint limb and bit counts". */
_Bigint *_lshift(_reent *r, _Bigint *b, s32 shift) {
    _Bigint *b1;
    u32 *src;
    u32 *dst;
    u32 *end;
    u32 carry;
    s32 n;
    s32 wds;
    s32 bitCount;
    s32 count;

    n = shift >> 5;
    count = b->maxwds;
    wds = n + b->wds + 1;
    bitCount = b->k;
    if (count < wds) {
        do {
            count *= 2;
            bitCount++;
        } while (count < wds);
    }
    b1 = _Balloc(r, bitCount);
    dst = b1->x;
    count = 0;
    while (count < n) {
        *dst++ = 0;
        count++;
    }
    src = b->x;
    shift &= 0x1F;
    end = src + b->wds;
    if (shift != 0) {
        bitCount = 0x20 - shift;
        carry = 0;
        do {
            *dst++ = (*src << shift) | carry;
            carry = *src++ >> bitCount;
        } while (src < end);
        *dst = carry;
        if (carry != 0) {
            wds++;
        }
    } else {
        do {
            *dst++ = *src++;
        } while (src < end);
    }
    b1->wds = wds - 1;
    _Bfree(r, b);
    return b1;
}

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

/* The shared pointer and limb intermediates have qualified allocator controls;
 * see docs/levers/cross-block-locals.md, "Retain bigint subtraction values". */
_Bigint *__mdiff(_reent *r, _Bigint *a, _Bigint *b) {
    s32 sign = __mcmp(a, b);
    _Bigint *result;
    u32 *srcA;
    u32 *srcB;
    u32 *dst;
    u32 *endA;
    u32 *endB;
    s32 countA;
    s32 countB;
    s32 borrow;
    s32 low;
    u32 limb;

    if (sign == 0) {
        result = _Balloc(r, 0);
        result->wds = 1;
        result->x[0] = 0;
        return result;
    }
    if (sign < 0) {
        result = a;
        a = b;
        b = result;
        sign = 1;
    } else {
        sign = 0;
    }

    result = _Balloc(r, a->k);
    result->sign = sign;
    srcA = a->x;
    srcB = b->x;
    dst = result->x;
    countA = a->wds;
    countB = b->wds;
    endA = srcA + countA;
    endB = srcB + countB;
    borrow = 0;
    do {
        u32 wordB;
        limb = *srcA++;
        wordB = *srcB++;
        low = (limb & 0xFFFF) - (wordB & 0xFFFF) + borrow;
        borrow = low >> 16;
        limb = (limb >> 16) - (wordB >> 16);
        limb += borrow;
        ((u16 *)dst)[0] = low;
        ((u16 *)dst)[1] = limb;
        dst++;
        borrow = (s32)limb >> 16;
    } while (srcB < endB);
    while (srcA < endA) {
        u32 word = *srcA++;
        low = (word & 0xFFFF) + borrow;
        borrow = low >> 16;
        limb = (word >> 16) + borrow;
        ((u16 *)dst)[0] = low;
        ((u16 *)dst)[1] = limb;
        dst++;
        borrow = (s32)limb >> 16;
    }
    dst--;
    while (*dst == 0) {
        dst--;
        countA--;
    }
    result->wds = countA;
    return result;
}

/* Little-endian IEEE binary64 word view used by the numeric helpers. */
typedef union libcDoubleBits_t {
    f64 value;
    u32 word[2];
} libcDoubleBits_t;

f64 _ulp(f64 x)
{
    libcDoubleBits_t input;
    libcDoubleBits_t result;
    s32 exponent;

    input.value = x;
    exponent = (input.word[1] & 0x7FF00000) - 0x03400000;
    if (exponent > 0) {
        result.word[1] = exponent;
        result.word[0] = 0;
    } else {
        exponent = -exponent >> 20;
        if (exponent < 20) {
            result.word[1] = 0x80000 >> exponent;
            result.word[0] = 0;
        } else {
            result.word[1] = 0;
            exponent -= 20;
            result.word[0] = exponent >= 31 ? 1 : 1 << (31 - exponent);
        }
    }
    return result.value;
}

INCLUDE_ASM(const s32, "sdk/libc", _b2d);

INCLUDE_ASM(const s32, "sdk/libc", _d2b);

f64 _b2d(_Bigint *b, s32 *exponent);

f64 _ratio(_Bigint *a, _Bigint *b)
{
    libcDoubleBits_t numerator;
    libcDoubleBits_t denominator;
    s32 aExponent;
    s32 bExponent;
    s32 difference;

    numerator.value = _b2d(a, &aExponent);
    denominator.value = _b2d(b, &bExponent);
    difference = aExponent - bExponent + 32 * (a->wds - b->wds);
    if (difference > 0) {
        numerator.word[1] += difference * 0x100000U;
    } else {
        denominator.word[1] -= difference * 0x100000U;
    }
    return numerator.value / denominator.value;
}

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

s32 _fstat_r(_reent *r, s32 fd, struct stat *buf) {
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

INCLUDE_RODATA(const s32, "sdk/libc", D_0054B8A0);

INCLUDE_RODATA(const s32, "sdk/libc", D_0054B8A8);

INCLUDE_RODATA(const s32, "sdk/libc", D_0054B8B0);

INCLUDE_RODATA(const s32, "sdk/libc", jtbl_0054B8C0);

INCLUDE_RODATA(const s32, "sdk/libc", D_0054B8D8);

INCLUDE_RODATA(const s32, "sdk/libc", D_0054B908);

INCLUDE_RODATA(const s32, "sdk/libc", D_0054B910);

INCLUDE_RODATA(const s32, "sdk/libc", D_0054B918);

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

