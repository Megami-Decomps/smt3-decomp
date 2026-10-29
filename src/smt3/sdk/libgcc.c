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
    u32 count;
    struct object *next;
} object;

/* libgcc's fde_vector, as add_fdes fills it. */
typedef struct fde_vector {
    u32 **array;
    u32 count;
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

extern f64 __make_dp(s32 kind, s32 sign, s32 exp, long mant);

#define va_start(ap, last) __builtin_stdarg_start((ap), last)

extern s32 func_002D6678(_reent *r, FILE *fp, const char *fmt, va_list ap);

typedef union f32_bits {
    f32 f;
    u32 u;
    struct {
        u32 fraction : 23;
        u32 exponent : 8;
        u32 sign : 1;
    } fields;
} f32_bits;

typedef union sdfDoubleBits_t {
    double value;
    struct {
        u64 fraction : 52;
        u32 exponent : 11;
        u32 sign : 1;
    } fields;
} sdfDoubleBits_t;

/* The same record from the double-precision build of fp-bit.c. */
typedef struct fp_number_type_df {
    s32 class;
    s32 sign;
    s32 normal_exp;
    long fraction;
} fp_number_type_df;

extern f64 __pack_df(const fp_number_type_df *u);

extern void sdfUnpackDouble(const double *x, fp_number_type_df *u);

extern fp_number_type_df *libc_2D1590(const fp_number_type_df *a, const fp_number_type_df *b, fp_number_type_df *r);

extern s32 __fpcmp_parts_df(const fp_number_type_df *a, const fp_number_type_df *b);


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

f64 fptodp(f32 x) {
    fp_number_type u;

    sdfUnpackFloat(&x, &u);
    return __make_dp(u.class, u.sign, u.normal_exp, (long)u.fraction << 30);
}

INCLUDE_ASM(const s32, "sdk/libgcc", libc_2D1590);

f64 dpadd(double a, double b) {
    fp_number_type_df ua;
    fp_number_type_df ub;
    fp_number_type_df r;

    sdfUnpackDouble(&a, &ua);
    sdfUnpackDouble(&b, &ub);
    return __pack_df(libc_2D1590(&ua, &ub, &r));
}

f64 dpsub(double a, double b) {
    fp_number_type_df ua;
    fp_number_type_df ub;
    fp_number_type_df r;

    sdfUnpackDouble(&a, &ua);
    sdfUnpackDouble(&b, &ub);
    ub.sign ^= 1;
    return __pack_df(libc_2D1590(&ua, &ub, &r));
}

INCLUDE_ASM(const s32, "sdk/libgcc", dpmul);

s32 dpcmp(double a, double b) {
    fp_number_type_df ua;
    fp_number_type_df ub;

    sdfUnpackDouble(&a, &ua);
    sdfUnpackDouble(&b, &ub);
    return __fpcmp_parts_df(&ua, &ub);
}

/* fp-bit tags: NaN classes 0/1, zero 2, normal 3, infinity 4. */
static inline s32 dfIsNaN(const fp_number_type_df *number) {
    return (u32)number->class < 2;
}

static inline s32 dfIsZero(const fp_number_type_df *number) {
    return number->class == 2;
}

static inline s32 dfIsInfinity(const fp_number_type_df *number) {
    return number->class == 4;
}

f64 litodp(s32 n) {
    fp_number_type_df value;

    value.class = 3;
    value.sign = (u32)n >> 31;
    if (n == 0) {
        value.class = 2;
    } else {
        value.normal_exp = 60;
        if (value.sign != 0) {
            if (n == (-2147483647 - 1)) {
                return -2147483648.0;
            }
            value.fraction = -n;
        } else {
            value.fraction = n;
        }
        while ((u64)value.fraction <= 0x0FFFFFFFFFFFFFFFULL) {
            value.fraction = (u64)value.fraction << 1;
            value.normal_exp--;
        }
    }
    return __pack_df(&value);
}

s32 dptoli(f64 value) {
    fp_number_type_df number;
    s32 result;

    sdfUnpackDouble(&value, &number);
    if (dfIsZero(&number)) {
        return 0;
    }
    if ((u32)number.class < 2) {
        return 0;
    }
    if (dfIsInfinity(&number)) {
        return number.sign ? (s32)0x80000000 : 0x7FFFFFFF;
    }
    if (number.normal_exp < 0) {
        return 0;
    }
    if (number.normal_exp > 30) {
        return number.sign ? (s32)0x80000000 : 0x7FFFFFFF;
    }
    result = (unsigned long)number.fraction >> (60 - number.normal_exp);
    return number.sign ? -result : result;
}

f64 __make_dp(s32 kind, s32 sign, s32 exp, long mant) {
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

u32 dptofp(double x) {
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

u32 dptoul(f64 value) {
    fp_number_type_df number;
    sdfUnpackDouble(&value, &number);
    if (dfIsZero(&number)) {
        return 0;
    }
    if ((u32)number.class < 2) {
        return 0;
    }
    if (number.sign != 0) {
        return 0;
    }
    if (dfIsInfinity(&number)) {
        return 0xFFFFFFFFU;
    }
    if (number.normal_exp < 0) {
        return 0;
    }
    if (number.normal_exp > 31) {
        return 0xFFFFFFFFU;
    }
    if (number.normal_exp > 60) {
        return (unsigned long)number.fraction << (number.normal_exp - 60);
    }
    return (unsigned long)number.fraction >> (60 - number.normal_exp);
}

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

static inline s32 fde_compare(u32 *left, u32 *right)
{
    return (s32)(left[2] - right[2]);
}

void libc_2D1EA8(fde_vector *left, const fde_vector *right)
{
    u32 leftIndex;
    u32 rightIndex;
    u32 *entry;

    /* Preserve owner-based indexing; see levers/cross-block-locals.md. */
    rightIndex = right->count;
    if (rightIndex > 0) {
        leftIndex = left->count;
        do {
            rightIndex--;
            entry = right->array[rightIndex];
            while (leftIndex > 0 && fde_compare(left->array[leftIndex - 1], entry) > 0) {
                left->array[leftIndex + rightIndex] = left->array[leftIndex - 1];
                leftIndex--;
            }
            left->array[leftIndex + rightIndex] = entry;
        } while (rightIndex > 0);
        left->count += right->count;
    }
}


INCLUDE_ASM(const s32, "sdk/libgcc", libc_2D1FA0);

s32 __register_frame_info(void *fn, void *entry) {
    object *e = entry;

    e->fde_begin = fn;
    e->pc_end = NULL;
    e->pc_begin = NULL;
    e->fde_array = NULL;
    e->count = 0;
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
    e->count = 0;
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

void *__deregister_frame_info(void *fn) {
    object **link;
    object *entry;

    WaitSema(D_0052C37C);
    for (link = &D_0059AD60; *link != NULL; link = &(*link)->next) {
        entry = *link;
        if (entry->fde_begin == fn) {
            *link = entry->next;
            if (entry->fde_array != NULL && entry->fde_array != fn) {
                free(entry->fde_array);
            }
            SignalSema(D_0052C37C);
            return entry;
        }
    }
    SignalSema(D_0052C37C);
    abort();
}

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

typedef struct {
    fde_vector linear;
    fde_vector erratic;
} fde_accumulator;

extern u32 **libc_2D1FA0(fde_accumulator *vectors, u32 count);

/* Preserve the allocation boundary; see levers/cross-block-locals.md. */
static inline s32 start_fde_sort(fde_accumulator *vectors, u32 count)
{
    vectors->linear.array = count ? malloc(count * sizeof(u32 *)) : NULL;
    vectors->erratic.array = vectors->linear.array ? malloc(count * sizeof(u32 *)) : NULL;
    vectors->linear.count = 0;
    vectors->erratic.count = 0;
    return vectors->linear.array != NULL;
}

void libc_2D2850(object *entry)
{
    fde_accumulator vectors;
    u32 minimum;
    u32 maximum;
    u32 count;
    u32 **sorted;

    if (entry->pc_begin != NULL) {
        count = entry->count;
    } else if (entry->fde_array != NULL) {
        u32 **table = entry->fde_array;
        for (count = 0; *table != NULL; table++)
            count += libc_2D2728(*table);
    } else {
        count = libc_2D2728(entry->fde_begin);
    }
    entry->count = count;
    if (!start_fde_sort(&vectors, count) && entry->pc_begin != NULL)
        return;
    minimum = 0xffffffff;
    maximum = 0;
    if (entry->fde_array != NULL) {
        u32 **table = entry->fde_array;
        for (; *table != NULL; table++)
            libc_2D2768(*table, &vectors.linear, &minimum, &maximum);
    } else {
        libc_2D2768(entry->fde_begin, &vectors.linear, &minimum, &maximum);
    }
    sorted = libc_2D1FA0(&vectors, count);
    if (sorted != NULL)
        entry->fde_array = sorted;
    entry->pc_begin = (void *)minimum;
    entry->pc_end = (void *)maximum;
}


u32 *libc_2D29B8(u32 pc)
{
    object *entry;
    u32 low;
    u32 high;
    u32 *found;

    WaitSema(D_0052C37C);
    for (entry = D_0059AD60; entry != NULL; entry = entry->next) {
        if (entry->pc_begin == NULL)
            libc_2D2850(entry);
        if (pc >= (u32)entry->pc_begin && pc < (u32)entry->pc_end)
            break;
    }
    if (entry == NULL) {
        SignalSema(D_0052C37C);
        return NULL;
    }
    if (entry->fde_array == NULL || entry->fde_array == entry->fde_begin)
        libc_2D2850(entry);
    if (entry->fde_array != NULL && entry->fde_array != entry->fde_begin) {
        SignalSema(D_0052C37C);
        for (low = 0, high = entry->count; low < high;) {
            u32 index = (low + high) / 2;
            u32 *frame = ((u32 **)entry->fde_array)[index];
            if (pc < frame[2])
                high = index;
            else if (pc >= frame[2] + frame[3])
                low = index + 1;
            else
                return frame;
        }
    } else {
        if (entry->fde_array != NULL) {
            u32 **table = entry->fde_array;
            do {
                found = libc_2D27F8(*table, pc);
            } while (found == NULL && *++table != NULL);
        } else {
            found = libc_2D27F8(entry->fde_begin, pc);
        }
        SignalSema(D_0052C37C);
        return found;
    }
    return NULL;
}


INCLUDE_RODATA(const s32, "sdk/libgcc", D_0054B460);

INCLUDE_RODATA(const s32, "sdk/libgcc", D_0054B480);

typedef struct {
    char *augmentation;
    void *ehPointer;
    u32 codeAlignment;
    s32 dataAlignment;
    u32 returnRegister;
} cieInfo_t;

/* The encoded EH pointer follows a variable-length string. */
typedef struct __attribute__((packed)) {
    void *value;
} cieEncodedPointer_t;

extern const char D_0054B480[];
extern const char D_0054B488[];
extern s32 strcmp(const char *left, const char *right);
extern u32 strlen(const char *text);

u8 *libc_2D2B60(u32 *frame, cieInfo_t *info)
{
    u8 *p;
    u32 length;

    info->augmentation = (char *)frame - frame[1] + 13;
    if (strcmp(info->augmentation, D_0054B480) != 0 &&
        strcmp(info->augmentation, D_0054B488) != 0 &&
        info->augmentation[0] != 'z')
        return NULL;
    length = strlen(info->augmentation);
    p = (u8 *)info->augmentation + length + 1;
    if (strcmp(info->augmentation, D_0054B488) == 0) {
        info->ehPointer = ((const cieEncodedPointer_t *)p)->value;
        p += sizeof(void *);
    } else {
        info->ehPointer = NULL;
    }
    p = libc_2D1E08(p, &info->codeAlignment);
    p = libc_2D1E50(p, &info->dataAlignment);
    info->returnRegister = *p++;
    if (info->augmentation[0] == 'z') {
        u32 payloadSize;
        p = libc_2D1E08(p, &payloadSize);
        p += payloadSize;
    }
    return p;
}

INCLUDE_RODATA(const s32, "sdk/libgcc", D_0054B488);


typedef struct {
    void *cfa;
    void *ehPointer;
    s64 cfaOffset;
    s64 baseOffset;
    s64 argumentSize;
    s64 registerOrOffset[123];
    u16 cfaRegister;
    u16 returnColumn;
    u8 saved[123];
    u8 indirect;
} frameState_t;

typedef char frameState_size_must_be_0x478[
    sizeof(frameState_t) == 0x478 ? 1 : -1];

u8 *libc_2D2C70(u8 *p, frameState_t *state)
{
    u32 opcode = *p++;
    union {
        s32 signedValue;
        u32 unsignedValue;
    } value;

    switch (opcode) {
    case 0x50 ... 0x6f:
        state->cfaRegister = opcode - 0x50;
        break;
    case 0x90:
        p = libc_2D1E50(p, &value.signedValue);
        state->cfaRegister = value.signedValue;
        /* The native regx case falls through and replaces this register. */
    case 0x70 ... 0x8f:
        state->cfaRegister = opcode - 0x70;
        p = libc_2D1E50(p, &value.signedValue);
        state->baseOffset = value.signedValue;
        break;
    case 0x92:
        p = libc_2D1E50(p, &value.signedValue);
        state->cfaRegister = value.signedValue;
        p = libc_2D1E50(p, &value.signedValue);
        state->baseOffset = value.signedValue;
        break;
    case 0x06:
        state->indirect = 1;
        break;
    case 0x23:
        p = libc_2D1E08(p, &value.unsignedValue);
        state->cfaOffset = value.signedValue;
        break;
    default:
        abort();
    }
    return p;
}


INCLUDE_ASM(const s32, "sdk/libgcc", libc_2D2D90);

INCLUDE_ASM(const s32, "sdk/libgcc", libc_2D31E0);

INCLUDE_ASM(const s32, "sdk/libgcc", __muldi3);

void sdfUnpackFloat(const void *input, fp_number_type *number) {
    f32_bits bits;
    u32 exponent;
    u32 fraction;

    bits.f = *(const f32 *)input;
    exponent = (bits.u >> 23) & 0xFF;
    fraction = bits.u & 0x7FFFFF;
    number->sign = bits.u >> 31;
    if (exponent == 0) {
        number->class = 2;
        return;
    }
    number->normal_exp = exponent - 127;
    number->class = 3;
    number->fraction = (fraction << 7) | 0x40000000;
}

f32 sdfPackQuad(s32 kind, s32 sign, s32 exp, u32 mant) {
    fp_number_type u;

    u.class = kind;
    u.sign = sign;
    u.normal_exp = exp;
    u.fraction = mant;
    return sdfPackFloat(&u);
}

f64 __pack_df(const fp_number_type_df *u) {
    sdfDoubleBits_t result;
    u64 fraction = u->fraction;
    u32 exponent = 0;
    u32 sign = u->sign;

    if (dfIsNaN(u)) {
        fraction |= (u64)1 << 51;
        exponent = 0x7FF;
    } else if (dfIsInfinity(u)) {
        exponent = 0x7FF;
        fraction = 0;
    } else if (dfIsZero(u)) {
        fraction = 0;
    } else if (fraction != 0) {
        if (u->normal_exp < -1022) {
            fraction = 0;
        } else if (u->normal_exp > 1023) {
            exponent = 0x7FF;
            fraction = 0;
        } else {
            exponent = u->normal_exp + 1023;
            fraction >>= 8;
        }
    }
    result.fields.fraction = fraction;
    result.fields.exponent = exponent;
    result.fields.sign = sign;
    return result.value;
}

void sdfUnpackDouble(const double *x, fp_number_type_df *u) {
    sdfDoubleBits_t input;
    u64 fraction;
    u32 exponent;

    input.value = *x;
    exponent = input.fields.exponent;
    fraction = input.fields.fraction;
    u->sign = input.fields.sign;
    if (exponent == 0) {
        u->class = 2;
    } else if (exponent == 0x7FF) {
        u->normal_exp = 0x3FF;
        if (fraction == 0) {
            u->class = 4;
            u->fraction = (u64)1 << 60;
        } else {
            if (fraction & ((u64)1 << 51)) {
                u->class = 1;
            } else {
                u->class = 0;
            }
            u->fraction = fraction;
        }
    } else {
        fraction = (fraction << 8) | ((u64)1 << 60);
        u->normal_exp = exponent - 0x3FF;
        u->class = 3;
        u->fraction = fraction;
    }
}

s32 __fpcmp_parts_df(const fp_number_type_df *a, const fp_number_type_df *b) {
    if (dfIsNaN(a) || dfIsNaN(b)) {
        return 1;
    }
    if (dfIsInfinity(a) && dfIsInfinity(b)) {
        return b->sign - a->sign;
    }
    if (dfIsInfinity(a)) {
        return a->sign ? -1 : 1;
    }
    if (dfIsInfinity(b)) {
        return b->sign ? 1 : -1;
    }
    if (dfIsZero(a) && dfIsZero(b)) {
        return 0;
    }
    if (dfIsZero(a)) {
        return b->sign ? 1 : -1;
    }
    if (dfIsZero(b)) {
        return a->sign ? -1 : 1;
    }
    if (a->sign != b->sign) {
        return a->sign ? -1 : 1;
    }
    if (a->normal_exp > b->normal_exp) {
        return a->sign ? -1 : 1;
    }
    if (a->normal_exp < b->normal_exp) {
        return a->sign ? 1 : -1;
    }
    if ((u64)a->fraction > (u64)b->fraction) {
        return a->sign ? -1 : 1;
    }
    if ((u64)a->fraction < (u64)b->fraction) {
        return a->sign ? 1 : -1;
    }
    return 0;
}

static inline s32 sfIsZero(const fp_number_type *number) {
    return number->class == 2;
}

f32 sdfPackFloat(const fp_number_type *number) {
    f32_bits result;
    s32 zero = sfIsZero(number);
    u32 fraction;
    s32 exponent;
    u32 sign;

    exponent = 0;
    fraction = number->fraction;
    sign = number->sign;
    if (zero) {
        fraction = 0;
    } else if (fraction != 0) {
        if (number->normal_exp < -126) {
            fraction = 0;
        } else if (number->normal_exp > 128 ||
                   (number->normal_exp == 128 && fraction >= 0x80000000U)) {
            exponent = 255;
            fraction = 0;
        } else {
            exponent = number->normal_exp + 127;
            fraction >>= 7;
        }
    }
    result.fields.fraction = fraction;
    result.fields.exponent = exponent;
    result.fields.sign = sign;
    return result.f;
}
