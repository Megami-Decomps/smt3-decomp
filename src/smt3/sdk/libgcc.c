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

extern long dptoul(s32 kind, s32 sign, s32 exp, long mant);

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


void __do_global_dtors(void) {
    void (**p)(void);

    while (*D_0052BB28 != NULL) {
        p = D_0052BB28;
        D_0052BB28 = p + 1;
        (*p)();
    }
    if (D_0052BB2C == 0) {
        D_0052BB2C = 1;
        __deregister_frame_info(D_0052D940);
    }
}

INCLUDE_ASM(const s32, "sdk/libgcc", __do_global_ctors);

void __main(void) {
    if (D_0059AD58 == 0) {
        D_0059AD58 = 1;
        __do_global_ctors();
    }
}

long sdfFloatToULong(f32 x) {
    fp_number_type u;

    sdfUnpackFloat(&x, &u);
    return dptoul(u.class, u.sign, u.normal_exp, (long)u.fraction << 30);
}

INCLUDE_ASM(const s32, "sdk/libgcc", libc_2D1590);

long dpadd(double a, double b) {
    fp_number_type_df ua;
    fp_number_type_df ub;
    fp_number_type_df r;

    sdfUnpackDouble(&a, &ua);
    sdfUnpackDouble(&b, &ub);
    return __pack_df(libc_2D1590(&ua, &ub, &r));
}

long dpsub(double a, double b) {
    fp_number_type_df ua;
    fp_number_type_df ub;
    fp_number_type_df r;

    sdfUnpackDouble(&a, &ua);
    sdfUnpackDouble(&b, &ub);
    ub.sign ^= 1;
    return __pack_df(libc_2D1590(&ua, &ub, &r));
}

INCLUDE_ASM(const s32, "sdk/libgcc", dpmul);

long dpcmp(double a, double b) {
    fp_number_type_df ua;
    fp_number_type_df ub;

    sdfUnpackDouble(&a, &ua);
    sdfUnpackDouble(&b, &ub);
    return __fpcmp_parts_df(&ua, &ub);
}

INCLUDE_ASM(const s32, "sdk/libgcc", litodp);

INCLUDE_ASM(const s32, "sdk/libgcc", dptoli);

long dptoul(s32 kind, s32 sign, s32 exp, long mant) {
    fp_number_type_df u;

    u.class = kind;
    u.sign = sign;
    u.normal_exp = exp;
    u.fraction = mant;
    return __pack_df(&u);
}

#ifdef FIX_BUGS
f32 sdfPackQuad(s32 kind, s32 sign, s32 exp, u32 mant);
#endif

u32 sdfDoubleToULong(double x) {
    fp_number_type_df u;
    u32 m;

    sdfUnpackDouble(&x, &u);
    m = u.fraction >> 30;
    if (u.fraction & 0x3FFFFFFF) {
        m |= 1;
    }
    /* BUG: sdfPackQuad returns f32, which this target returns in $f0, but
     * with nothing in scope ee-gcc assumes int and the result is read from
     * $v0. So what comes back is not the packed value at all. Retail does
     * this, and declaring the function moves the image, so the behaviour is
     * kept and the declaration sits behind FIX_BUGS. */
    return sdfPackQuad(u.class, u.sign, u.normal_exp, m);
}

INCLUDE_ASM(const s32, "sdk/libgcc", sdfDoubleToUInt);

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

INCLUDE_ASM(const s32, "sdk/libgcc", libc_2D1EA8);

INCLUDE_ASM(const s32, "sdk/libgcc", libc_2D1FA0);

s32 __register_frame_info(void *fn, void *entry) {
    object *e = entry;

    e->fde_begin = fn;
    e->pc_end = NULL;
    e->pc_begin = NULL;
    e->fde_array = NULL;
    e->count = NULL;
    WaitSema(D_0052C37C);
    e->next = D_0059AD60;
    D_0059AD60 = e;
    SignalSema(D_0052C37C);
    /* Dead store: blocks the SignalSema sibcall. */
    entry = NULL;
    return;
}

s32 __register_frame(void *fn) {
    return __register_frame_info(fn, malloc(0x18));
}

s32 __register_frame_info_table(void *fn, void *entry) {
    object *e = entry;

    e->fde_begin = fn;
    e->fde_array = fn;
    e->pc_end = NULL;
    e->pc_begin = NULL;
    e->count = NULL;
    WaitSema(D_0052C37C);
    e->next = D_0059AD60;
    D_0059AD60 = e;
    SignalSema(D_0052C37C);
    /* Dead store: blocks the SignalSema sibcall. */
    entry = NULL;
    return;
}

s32 __register_frame_table(void *fn) {
    return __register_frame_info_table(fn, malloc(0x18));
}

INCLUDE_ASM(const s32, "sdk/libgcc", __deregister_frame_info);

void __deregister_frame(void *fn) {
    free(__deregister_frame_info(fn));
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

void libc_2D2768(u32 *p, fde_vector *list, u32 *pmin, u32 *pmax) {
    u32 lo = *pmin;
    u32 hi = *pmax;

    while (p[0] != 0) {
        if (p[1] != 0) {
            u32 a = p[2];

            if (a != 0) {
                if (list->array != NULL) {
                    list->array[list->count++] = p;
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

INCLUDE_ASM(const s32, "sdk/libgcc", libc_2D2850);

INCLUDE_ASM(const s32, "sdk/libgcc", libc_2D29B8);

INCLUDE_RODATA(const s32, "sdk/libgcc", D_0054B460);

INCLUDE_RODATA(const s32, "sdk/libgcc", D_0054B480);

INCLUDE_ASM(const s32, "sdk/libgcc", libc_2D2B60);

INCLUDE_ASM(const s32, "sdk/libgcc", libc_2D2C70);

INCLUDE_ASM(const s32, "sdk/libgcc", libc_2D2D90);

INCLUDE_ASM(const s32, "sdk/libgcc", libc_2D31E0);

INCLUDE_ASM(const s32, "sdk/libgcc", libc_2D3398);

INCLUDE_ASM(const s32, "sdk/libgcc", sdfUnpackFloat);

f32 sdfPackQuad(s32 kind, s32 sign, s32 exp, u32 mant) {
    fp_number_type u;

    u.class = kind;
    u.sign = sign;
    u.normal_exp = exp;
    u.fraction = mant;
    return sdfPackFloat(&u);
}

INCLUDE_ASM(const s32, "sdk/libgcc", __pack_df);

INCLUDE_ASM(const s32, "sdk/libgcc", sdfUnpackDouble);

INCLUDE_ASM(const s32, "sdk/libgcc", __fpcmp_parts_df);

INCLUDE_ASM(const s32, "sdk/libgcc", sdfPackFloat);
