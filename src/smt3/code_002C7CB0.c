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

struct SdfKeyTreeItem;

struct SdfKeyPoolNode;

struct sdfSortOtNode_tag;

struct SdfPool;

#define SDF_POOL_FREE_KIND 0xFFFF

/* Tree view of a pool node: the link word at +0 becomes the replaced-item
 * link, +0x4..+0xB stay opaque, and +0xC is compared as a float key. */
typedef struct SdfKeyTreeItem {
    struct SdfKeyTreeItem *replaced; /* 0x00 */
    u8 pad04[8];                     /* 0x04: opaque */
    f32 key;                         /* 0x0C */
} SdfKeyTreeItem;

extern void StartThread();

extern void SleepThread(void);

extern s32 sceSifLoadModule(const char *, s32, const char *);

extern char *strcpy(char *, const char *);

extern char *strcat(char *, const char *);

extern void sceMcInit(void);

extern void sdfGrJoinDrawTag(void *left, void *right);

/* Pool release logic tests the packet head address and kind at 0xC;
 * bytes at 0x8 remain opaque. The float-key tree insertion helper reads the
 * same +0xC word through SdfKeyTreeItem. DDS1/2 game/code_002EEA18/003478C0.c. */
typedef struct SdfKeyPoolNode {
    struct SdfKeyPoolNode *next; /* 0x00 */
    s32 packetHeadAddress;       /* 0x04: first DMA packet address. */
    u8 unk8[4];                  /* 0x08 */
    s32 kind;                    /* 0x0C */
} SdfKeyPoolNode;

typedef struct sdfSortOtNode_tag {
    struct sdfSortOtNode_tag *right; /* 0x00 */
    struct sdfSortOtNode_tag *left;  /* 0x04 */
    struct SdfKeyTreeItem *item;  /* 0x08 */
    s32 balance;                  /* 0x0C */
} sdfSortOtNode_tag;

/* A key-sorted draw-surface pool: an SdfPoolNode whose append and prepend
 * callbacks release a node into the pool and update its free list. */
typedef struct SdfPool {
    void *link;                                                /* 0x00: the pool entry's link */
    SdfKeyPoolNode *head;                                      /* 0x04: first node in the emitted chain */
    SdfKeyPoolNode *tail;                                      /* 0x08: last node in the emitted chain */
    SdfKeyPoolNode *freeHead;                                  /* 0x0C */
    void (*release)(struct SdfPool *, SdfKeyPoolNode *);       /* 0x10: the entry's append */
    void (*update)(struct SdfPool *, s32, SdfKeyPoolNode *);   /* 0x14: the entry's prepend */
    sdfSortOtNode_tag *keyTree[1];                                /* 0x18: first root; allocation continues */
} SdfPool;

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */
s32 sdfGrAllocPacketBuff(s32 size);

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

sdfSortOtNode_tag *sdfSortOtAvlRotateLR(sdfSortOtNode_tag *heavyChild, sdfSortOtNode_tag *parent);

sdfSortOtNode_tag *sdfSortOtAvlRotateRL(sdfSortOtNode_tag *heavyChild, sdfSortOtNode_tag *parent);

void sdfSortOtAvlCheckBalance(sdfSortOtNode_tag **path, s32 depth, sdfSortOtNode_tag *child,
                   sdfSortOtNode_tag **root);

void sdfSortOtAvlRegist(sdfSortOtNode_tag **tree, SdfKeyTreeItem *item);

void sdfGrSortOtReg(SdfPool *pool, SdfKeyPoolNode *node);

void sdfGrSortOtFixup(void *first, SdfKeyPoolNode **head, SdfKeyPoolNode **tail);

void sdfGrSortOtTraverse(sdfSortOtNode_tag *node, SdfKeyPoolNode **head, SdfKeyPoolNode **tail);

void sdfGrSortOtSetup(SdfPool *pool);

void sdfGrSortOtCallback(SdfPool *pool, s32 mode, SdfKeyPoolNode *freeNode);

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

sdfSortOtNode_tag *sdfSortOtAvlRotateLR(sdfSortOtNode_tag *heavyChild, sdfSortOtNode_tag *parent) {
    sdfSortOtNode_tag *childRoot = heavyChild;
    sdfSortOtNode_tag *pivot;

    if (childRoot->balance < 0) {
        pivot = childRoot->left;
        parent->right = pivot->left;
        childRoot->left = pivot->right;
        pivot->left = parent;
        pivot->right = childRoot;
        if (pivot->balance > 0) {
            parent->balance = -1;
            childRoot->balance = 0;
        } else {
            parent->balance = 0;
            childRoot->balance = 1;
        }
        pivot->balance = 0;
    } else {
        parent->balance = 0;
        parent->right = childRoot->left;
        childRoot->balance = 0;
        childRoot->left = parent;
        pivot = childRoot;
    }
    return pivot;
}

sdfSortOtNode_tag *sdfSortOtAvlRotateRL(sdfSortOtNode_tag *heavyChild, sdfSortOtNode_tag *parent) {
    sdfSortOtNode_tag *childRoot = heavyChild;
    sdfSortOtNode_tag *pivot;

    if (childRoot->balance > 0) {
        pivot = childRoot->right;
        parent->left = pivot->right;
        childRoot->right = pivot->left;
        pivot->right = parent;
        pivot->left = childRoot;
        if (pivot->balance > 0) {
            parent->balance = -1;
            childRoot->balance = 0;
        } else {
            parent->balance = 0;
            childRoot->balance = 1;
        }
        pivot->balance = 0;
    } else {
        parent->balance = 0;
        parent->left = childRoot->right;
        childRoot->balance = 0;
        childRoot->right = parent;
        pivot = childRoot;
    }
    return pivot;
}

void sdfSortOtAvlCheckBalance(sdfSortOtNode_tag **path, s32 depth, sdfSortOtNode_tag *child,
                   sdfSortOtNode_tag **root) {
    sdfSortOtNode_tag *parent;
    sdfSortOtNode_tag *replacement = NULL;
    s32 balance;

    if (depth > 0) {
        do {
            parent = path[--depth];
            if (child == parent->right) {
                balance = ++parent->balance;
            } else {
                balance = --parent->balance;
            }
            if (balance == 0) {
                return;
            }
            if (balance > 1) {
                replacement = sdfSortOtAvlRotateLR(child, parent);
                break;
            }
            if (balance < -1) {
                replacement = sdfSortOtAvlRotateRL(child, parent);
                break;
            }
            child = parent;
        } while (depth > 0);
    }
    if (depth > 0) {
        sdfSortOtNode_tag *ancestor = path[--depth];
        if (ancestor->right == parent) {
            ancestor->right = replacement;
        } else {
            ancestor->left = replacement;
        }
    } else if (replacement != NULL) {
        *root = replacement;
    }
}

void sdfSortOtAvlRegist(sdfSortOtNode_tag **tree, SdfKeyTreeItem *item) {
    sdfSortOtNode_tag *path[SDF_KEY_TREE_PATH_LIMIT];
    s32 depth = 0;
    sdfSortOtNode_tag **link = tree;
    sdfSortOtNode_tag *cur = *tree;
    sdfSortOtNode_tag *node;
    f32 key = item->key;

    while (cur != NULL) {
        if (key == cur->item->key) {
            item->replaced = cur->item;
            cur->item = item;
            return;
        }
        path[depth] = cur;
        depth++;
        if (cur->item->key < key) {
            link = &cur->right;
        } else {
            link = &cur->left;
        }
        cur = *link;
    }
    item->replaced = NULL;
    node = sdfGrAllocPacketBuff(0x10);
    node->right = NULL;
    node->left = NULL;
    node->item = item;
    *link = node;
    node->balance = 0;
    if (depth != 0) {
        sdfSortOtAvlCheckBalance(path, depth, node, tree);
    }
}

void sdfGrSortOtReg(SdfPool *pool, SdfKeyPoolNode *node) {
    if (node->packetHeadAddress != 0) {
        if (node->kind == SDF_POOL_FREE_KIND) {
            node->next = pool->freeHead;
            pool->freeHead = node;
        } else {
            /* The key tree views the pool node's +0xC word as its float key. */
            sdfSortOtAvlRegist(pool->keyTree, (SdfKeyTreeItem *)node);
        }
    }
}

void sdfGrSortOtFixup(void *first, SdfKeyPoolNode **head, SdfKeyPoolNode **tail) {
    SdfKeyPoolNode *current;
    SdfKeyPoolNode *next;

    if (first != NULL) {
        if (*tail == NULL) {
            *head = first;
        } else {
            sdfGrJoinDrawTag(*tail, first);
        }
        current = first;
        while ((next = current->next) != NULL) {
            sdfGrJoinDrawTag(current, next);
            current = next;
        }
        *tail = current;
    }
}

void sdfGrSortOtTraverse(sdfSortOtNode_tag *node, SdfKeyPoolNode **head, SdfKeyPoolNode **tail) {
    sdfSortOtNode_tag *next;

    do {
        if (node->right != NULL) {
            sdfGrSortOtTraverse(node->right, head, tail);
        }
        sdfGrSortOtFixup(node->item, head, tail);
        next = node->left;
        node = next;
    } while (next != NULL);
}

void sdfGrSortOtSetup(SdfPool *pool) {
    SdfKeyPoolNode *head = NULL;
    SdfKeyPoolNode *tail = NULL;

    sdfGrSortOtFixup(pool->freeHead, &head, &tail);
    if (pool->keyTree[0] != NULL) {
        sdfGrSortOtTraverse(pool->keyTree[0], &head, &tail);
    }
    pool->head = head;
    pool->tail = tail;
}

void sdfGrSortOtCallback(SdfPool *pool, s32 mode, SdfKeyPoolNode *freeNode) {
    switch (mode) {
    case SDF_POOL_REBUILD_CHAIN:
        sdfGrSortOtSetup(pool);
        break;
    case SDF_POOL_PUSH_FREE:
        if (freeNode->packetHeadAddress != 0) {
            freeNode->next = pool->freeHead;
            pool->freeHead = freeNode;
        }
        break;
    }
}

void sdfModelSetOtCallbacks(SdfPool *pool) {
    pool->release = sdfGrSortOtReg;
    pool->update = sdfGrSortOtCallback;
}

INCLUDE_ASM(const s32, "code_002C7CB0", sceGsResetGraph);

INCLUDE_ASM(const s32, "code_002C7CB0", sceGsGetGParam);

INCLUDE_ASM(const s32, "code_002C7CB0", sceGsResetPath);

INCLUDE_ASM(const s32, "code_002C7CB0", checkModelVersion);

INCLUDE_ASM(const s32, "code_002C7CB0", _GetGsDxDyOffset);

INCLUDE_ASM(const s32, "code_002C7CB0", sceGsSetDefDispEnv);

INCLUDE_ASM(const s32, "code_002C7CB0", sceGsPutDispEnv);

INCLUDE_ASM(const s32, "code_002C7CB0", sceGsSyncPath);

INCLUDE_ASM(const s32, "code_002C7CB0", sceGsSetDefLoadImage);

INCLUDE_ASM(const s32, "code_002C7CB0", sceGsSetDefStoreImage);

INCLUDE_ASM(const s32, "code_002C7CB0", sceGsExecLoadImage);

INCLUDE_ASM(const s32, "code_002C7CB0", sceGsExecStoreImage);

INCLUDE_ASM(const s32, "code_002C7CB0", sceGsPutIMR);

INCLUDE_ASM(const s32, "code_002C7CB0", sceGsGetIMR);

INCLUDE_ASM(const s32, "code_002C7CB0", WaitDma);

INCLUDE_ASM(const s32, "code_002C7CB0", WatchDma);

INCLUDE_ASM(const s32, "code_002C7CB0", CheckAddress);

INCLUDE_ASM(const s32, "code_002C7CB0", memclr);

INCLUDE_ASM(const s32, "code_002C7CB0", sceDmaGetChan);

INCLUDE_ASM(const s32, "code_002C7CB0", sceDmaReset);

INCLUDE_ASM(const s32, "code_002C7CB0", sceDmaPutEnv);

INCLUDE_ASM(const s32, "code_002C7CB0", sceDmaGetEnv);

INCLUDE_ASM(const s32, "code_002C7CB0", sceDmaPutStallAddr);

INCLUDE_ASM(const s32, "code_002C7CB0", sceDmaSend);

INCLUDE_ASM(const s32, "code_002C7CB0", sceDmaSendN);

INCLUDE_ASM(const s32, "code_002C7CB0", sceDmaSendI);

INCLUDE_ASM(const s32, "code_002C7CB0", sceDmaRecv);

INCLUDE_ASM(const s32, "code_002C7CB0", sceDmaRecvN);

INCLUDE_ASM(const s32, "code_002C7CB0", sceDmaRecvI);

INCLUDE_ASM(const s32, "code_002C7CB0", sceDmaSync);

INCLUDE_ASM(const s32, "code_002C7CB0", sceDmaWatch);

INCLUDE_ASM(const s32, "code_002C7CB0", sceDmaPause);

INCLUDE_ASM(const s32, "code_002C7CB0", sceDmaRestart);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054A8F0);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054A908);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054A930);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054A940);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054A950);

INCLUDE_ASM(const s32, "code_002C7CB0", _send_to_iop);

INCLUDE_ASM(const s32, "code_002C7CB0", scePadInit);

INCLUDE_ASM(const s32, "code_002C7CB0", scePadInit2);

INCLUDE_ASM(const s32, "code_002C7CB0", scePadEnd);

INCLUDE_ASM(const s32, "code_002C7CB0", scePadPortOpen);

INCLUDE_ASM(const s32, "code_002C7CB0", scePadPortClose);

INCLUDE_ASM(const s32, "code_002C7CB0", scePadGetDmaStr);

INCLUDE_ASM(const s32, "code_002C7CB0", scePadGetFrameCount);

INCLUDE_ASM(const s32, "code_002C7CB0", scePadRead);

INCLUDE_ASM(const s32, "code_002C7CB0", scePadGetState);

INCLUDE_ASM(const s32, "code_002C7CB0", scePadStateIntToStr);

INCLUDE_ASM(const s32, "code_002C7CB0", scePadSetReqState);

INCLUDE_ASM(const s32, "code_002C7CB0", scePadGetReqState);

INCLUDE_ASM(const s32, "code_002C7CB0", scePadReqIntToStr);

INCLUDE_ASM(const s32, "code_002C7CB0", scePadInfoAct);

INCLUDE_ASM(const s32, "code_002C7CB0", scePadInfoComb);

INCLUDE_ASM(const s32, "code_002C7CB0", scePadInfoMode);

INCLUDE_ASM(const s32, "code_002C7CB0", scePadSetMainMode);

INCLUDE_ASM(const s32, "code_002C7CB0", scePadSetActDirect);

INCLUDE_ASM(const s32, "code_002C7CB0", scePadSetActAlign);

INCLUDE_ASM(const s32, "code_002C7CB0", scePadGetButtonMask);

INCLUDE_ASM(const s32, "code_002C7CB0", scePadSetButtonInfo);

INCLUDE_ASM(const s32, "code_002C7CB0", scePadInfoPressMode);

INCLUDE_ASM(const s32, "code_002C7CB0", scePadEnterPressMode);

INCLUDE_ASM(const s32, "code_002C7CB0", scePadExitPressMode);

INCLUDE_ASM(const s32, "code_002C7CB0", scePadSetVrefParam);

INCLUDE_ASM(const s32, "code_002C7CB0", scePadGetPortMax);

INCLUDE_ASM(const s32, "code_002C7CB0", scePadGetSlotMax);

INCLUDE_ASM(const s32, "code_002C7CB0", scePadGetModVersion);

INCLUDE_ASM(const s32, "code_002C7CB0", scePadSetWarningLevel);

INCLUDE_ASM(const s32, "code_002C7CB0", gcc2_compiled__6);

INCLUDE_ASM(const s32, "code_002C7CB0", sceCdDelayThread);

INCLUDE_ASM(const s32, "code_002C7CB0", sceCdCallback);

INCLUDE_ASM(const s32, "code_002C7CB0", _sceCd_cd_callback);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054AA58);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054AA60);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054AA68);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054AA70);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054AA80);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054AA88);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054AA98);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054AAA0);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054AAA8);

INCLUDE_ASM(const s32, "code_002C7CB0", _Cdvd_cbLoop);

INCLUDE_ASM(const s32, "code_002C7CB0", sceCdInitEeCB);

INCLUDE_ASM(const s32, "code_002C7CB0", _sceCd_cd_read_intr);

INCLUDE_ASM(const s32, "code_002C7CB0", cmd_sem_init);

INCLUDE_ASM(const s32, "code_002C7CB0", cdvd_exit);

INCLUDE_ASM(const s32, "code_002C7CB0", sceCdPOffCallback);

INCLUDE_ASM(const s32, "code_002C7CB0", _sceCd_Poff_Intr);

INCLUDE_ASM(const s32, "code_002C7CB0", PowerOffCB);

INCLUDE_ASM(const s32, "code_002C7CB0", sceCdLayerSearchFile);

INCLUDE_ASM(const s32, "code_002C7CB0", sceCdSearchFile);

INCLUDE_ASM(const s32, "code_002C7CB0", _sceCd_ncmd_prechk);

INCLUDE_ASM(const s32, "code_002C7CB0", sceCdNcmdDiskReady);

INCLUDE_ASM(const s32, "code_002C7CB0", sceCdSync);

INCLUDE_ASM(const s32, "code_002C7CB0", sceCdSyncS);

INCLUDE_ASM(const s32, "code_002C7CB0", _sceCd_scmd_prechk);

INCLUDE_ASM(const s32, "code_002C7CB0", sceCdInit);

INCLUDE_ASM(const s32, "code_002C7CB0", sceCdDiskReady);

INCLUDE_ASM(const s32, "code_002C7CB0", sceCdMmode);

INCLUDE_ASM(const s32, "code_002C7CB0", sceCdGetDiskType);

INCLUDE_ASM(const s32, "code_002C7CB0", sceCdStatus);

INCLUDE_ASM(const s32, "code_002C7CB0", sceCdPowerOff);

INCLUDE_ASM(const s32, "code_002C7CB0", sceCdReadClock);

INCLUDE_ASM(const s32, "code_002C7CB0", sceCdStInit);

INCLUDE_ASM(const s32, "code_002C7CB0", sceCdStStart);

INCLUDE_ASM(const s32, "code_002C7CB0", sceCdStSeekF);

INCLUDE_ASM(const s32, "code_002C7CB0", sceCdStSeek);

INCLUDE_ASM(const s32, "code_002C7CB0", sceCdStStop);

INCLUDE_ASM(const s32, "code_002C7CB0", sceCdStRead);

INCLUDE_ASM(const s32, "code_002C7CB0", sceCdStPause);

INCLUDE_ASM(const s32, "code_002C7CB0", sceCdStResume);

INCLUDE_ASM(const s32, "code_002C7CB0", sceCdStStat);

INCLUDE_ASM(const s32, "code_002C7CB0", sceCdStream);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifMInitRpc);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifMExitRpc);

INCLUDE_ASM(const s32, "code_002C7CB0", _sceRpcGetPacket);

INCLUDE_ASM(const s32, "code_002C7CB0", _sceRpcFreePacket);

INCLUDE_ASM(const s32, "code_002C7CB0", _request_end);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifMBindRpc);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifMBindRpcParam);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifMUnBindRpc);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054ADD8);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifMCallRpc);

INCLUDE_ASM(const s32, "code_002C7CB0", gcc2_compiled__7);

INCLUDE_ASM(const s32, "code_002C7CB0", setD4_CHCR);

INCLUDE_ASM(const s32, "code_002C7CB0", sceIpuStopDMA);

INCLUDE_ASM(const s32, "code_002C7CB0", sceIpuRestartDMA);

INCLUDE_ASM(const s32, "code_002C7CB0", sceIpuSync);

INCLUDE_ASM(const s32, "code_002C7CB0", gcc2_compiled__8);

INCLUDE_ASM(const s32, "code_002C7CB0", sceIpuInit);

INCLUDE_ASM(const s32, "code_002C7CB0", sceMcInit);

INCLUDE_ASM(const s32, "code_002C7CB0", sceMcEnd);

INCLUDE_ASM(const s32, "code_002C7CB0", _lmcGetClientPtr);

INCLUDE_ASM(const s32, "code_002C7CB0", sceMcChangeThreadPriority);

INCLUDE_ASM(const s32, "code_002C7CB0", sceMcGetSlotMax);

INCLUDE_ASM(const s32, "code_002C7CB0", sceMcOpen);

INCLUDE_ASM(const s32, "code_002C7CB0", sceMcMkdir);

INCLUDE_ASM(const s32, "code_002C7CB0", sceMcClose);

INCLUDE_ASM(const s32, "code_002C7CB0", sceMcSeek);

INCLUDE_ASM(const s32, "code_002C7CB0", mceIntrReadFixAlign);

INCLUDE_ASM(const s32, "code_002C7CB0", sceMcRead);

INCLUDE_ASM(const s32, "code_002C7CB0", sceMcWrite);

INCLUDE_ASM(const s32, "code_002C7CB0", mcHearAlarm);

INCLUDE_ASM(const s32, "code_002C7CB0", mcDelayThread);

INCLUDE_ASM(const s32, "code_002C7CB0", sceMcSync);

INCLUDE_ASM(const s32, "code_002C7CB0", mceGetInfoApdx);

INCLUDE_ASM(const s32, "code_002C7CB0", sceMcGetInfo);

INCLUDE_ASM(const s32, "code_002C7CB0", sceMcUdCheckNewCard);

INCLUDE_ASM(const s32, "code_002C7CB0", sceMcGetDir);

INCLUDE_ASM(const s32, "code_002C7CB0", mceStorePwd);

INCLUDE_ASM(const s32, "code_002C7CB0", sceMcChdir);

INCLUDE_ASM(const s32, "code_002C7CB0", sceMcFormat);

INCLUDE_ASM(const s32, "code_002C7CB0", sceMcDelete);

INCLUDE_ASM(const s32, "code_002C7CB0", sceMcFlush);

INCLUDE_ASM(const s32, "code_002C7CB0", sceMcSetFileInfo);

INCLUDE_ASM(const s32, "code_002C7CB0", sceMcRename);

INCLUDE_ASM(const s32, "code_002C7CB0", sceMcUnformat);

INCLUDE_ASM(const s32, "code_002C7CB0", sceMcGetEntSpace);

INCLUDE_ASM(const s32, "code_002C7CB0", cosf);

INCLUDE_ASM(const s32, "code_002C7CB0", sinf);

INCLUDE_ASM(const s32, "code_002C7CB0", tanf);

INCLUDE_ASM(const s32, "code_002C7CB0", __ieee754_acosf);

INCLUDE_ASM(const s32, "code_002C7CB0", __ieee754_atan2f);

INCLUDE_ASM(const s32, "code_002C7CB0", __ieee754_rem_pio2f);

INCLUDE_ASM(const s32, "code_002C7CB0", __ieee754_sqrtf);

INCLUDE_ASM(const s32, "code_002C7CB0", __kernel_cosf);

INCLUDE_ASM(const s32, "code_002C7CB0", __kernel_rem_pio2f);

INCLUDE_ASM(const s32, "code_002C7CB0", __kernel_sinf);

INCLUDE_ASM(const s32, "code_002C7CB0", __kernel_tanf);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054AF60);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054B274);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054B348);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054B358);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054B3C0);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054B3F8);

INCLUDE_ASM(const s32, "code_002C7CB0", atanf);

INCLUDE_ASM(const s32, "code_002C7CB0", fabsf);

INCLUDE_ASM(const s32, "code_002C7CB0", floorf);

INCLUDE_ASM(const s32, "code_002C7CB0", scalbnf);

INCLUDE_ASM(const s32, "code_002C7CB0", copysignf);

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

INCLUDE_ASM(const s32, "code_002C7CB0", initAtExit);

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

INCLUDE_ASM(const s32, "code_002C7CB0", libc_2D1590);

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

INCLUDE_ASM(const s32, "code_002C7CB0", dpmul);

long dpdiv(double a, double b) {
    SdfUnpackedD ua;
    SdfUnpackedD ub;

    sdfUnpackDouble(&a, &ua);
    sdfUnpackDouble(&b, &ub);
    return libc_2D3638(&ua, &ub);
}

INCLUDE_ASM(const s32, "code_002C7CB0", litodp);

INCLUDE_ASM(const s32, "code_002C7CB0", dptoli);

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

INCLUDE_ASM(const s32, "code_002C7CB0", sdfDoubleToUInt);

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

INCLUDE_ASM(const s32, "code_002C7CB0", libc_2D1EA8);

INCLUDE_ASM(const s32, "code_002C7CB0", libc_2D1FA0);

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

INCLUDE_ASM(const s32, "code_002C7CB0", libc_2D2648);

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

INCLUDE_ASM(const s32, "code_002C7CB0", libc_2D2850);

INCLUDE_ASM(const s32, "code_002C7CB0", libc_2D29B8);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054B408);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054B414);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054B418);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054B460);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054B480);

INCLUDE_ASM(const s32, "code_002C7CB0", libc_2D2B60);

INCLUDE_ASM(const s32, "code_002C7CB0", libc_2D2C70);

INCLUDE_ASM(const s32, "code_002C7CB0", libc_2D2D90);

INCLUDE_ASM(const s32, "code_002C7CB0", libc_2D31E0);

INCLUDE_ASM(const s32, "code_002C7CB0", libc_2D3398);

INCLUDE_ASM(const s32, "code_002C7CB0", sdfUnpackFloat);

f32 sdfPackQuad(s32 kind, s32 sign, s32 exp, u32 mant) {
    SdfUnpacked u;

    u.kind = kind;
    u.sign = sign;
    u.exp = exp;
    u.mant = mant;
    return sdfPackFloat(&u);
}

INCLUDE_ASM(const s32, "code_002C7CB0", libc_2D34A0);

INCLUDE_ASM(const s32, "code_002C7CB0", sdfUnpackDouble);

INCLUDE_ASM(const s32, "code_002C7CB0", libc_2D3638);

INCLUDE_ASM(const s32, "code_002C7CB0", sdfPackFloat);

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

INCLUDE_ASM(const s32, "code_002C7CB0", gcc2_compiled_);

INCLUDE_ASM(const s32, "code_002C7CB0", _malloc_r);

INCLUDE_ASM(const s32, "code_002C7CB0", memcmp);

INCLUDE_ASM(const s32, "code_002C7CB0", memcpy);

INCLUDE_ASM(const s32, "code_002C7CB0", memmove);

INCLUDE_ASM(const s32, "code_002C7CB0", memset);

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

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054B550);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054B658);

INCLUDE_ASM(const s32, "code_002C7CB0", _puts_r);

s32 puts(const char *s) {
    return _puts_r(D_0052BE28, s);
}

INCLUDE_ASM(const s32, "code_002C7CB0", qsort);

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

INCLUDE_ASM(const s32, "code_002C7CB0", _raise_r);

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

INCLUDE_ASM(const s32, "code_002C7CB0", strcasecmp);

INCLUDE_ASM(const s32, "code_002C7CB0", strcat);

INCLUDE_ASM(const s32, "code_002C7CB0", strchr);

INCLUDE_ASM(const s32, "code_002C7CB0", strcmp);

INCLUDE_ASM(const s32, "code_002C7CB0", strcpy);

INCLUDE_ASM(const s32, "code_002C7CB0", strlen);

INCLUDE_ASM(const s32, "code_002C7CB0", strncmp);

INCLUDE_ASM(const s32, "code_002C7CB0", strncpy);

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

INCLUDE_ASM(const s32, "code_002C7CB0", strstr);

INCLUDE_ASM(const s32, "code_002C7CB0", _strtol_r);

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

INCLUDE_ASM(const s32, "code_002C7CB0", __sbprintf);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054B670);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054B680);

INCLUDE_ASM(const s32, "code_002C7CB0", vfprintf);

INCLUDE_ASM(const s32, "code_002C7CB0", cvt);

INCLUDE_ASM(const s32, "code_002C7CB0", exponent);

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

INCLUDE_ASM(const s32, "code_002C7CB0", __swsetup);

INCLUDE_ASM(const s32, "code_002C7CB0", gcc2_compiled__1);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054B880);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054B890);

INCLUDE_ASM(const s32, "code_002C7CB0", _dtoa_r);

INCLUDE_ASM(const s32, "code_002C7CB0", fflush);

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

INCLUDE_ASM(const s32, "code_002C7CB0", __sfp);

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

INCLUDE_ASM(const s32, "code_002C7CB0", _free_r);

INCLUDE_ASM(const s32, "code_002C7CB0", _malloc_trim_r);

INCLUDE_ASM(const s32, "code_002C7CB0", __sfvwrite);

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

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054B8A0);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054B8A8);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054B8B0);

INCLUDE_RODATA(const s32, "code_002C7CB0", jtbl_0054B8C0);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054B8D8);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054B908);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054B910);

INCLUDE_ASM(const s32, "code_002C7CB0", _setlocale_r);

void *func_002DA308(_reent *r) {
    return D_0054B8D8;
}

s32 setlocale(s32 category, const char *locale) {
    return _setlocale_r(D_0052BE28, category, locale);
}

void *localeconv(void) {
    return func_002DA308(D_0052BE28);
}

INCLUDE_ASM(const s32, "code_002C7CB0", __smakebuf);

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

INCLUDE_ASM(const s32, "code_002C7CB0", memchr);

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

INCLUDE_ASM(const s32, "code_002C7CB0", _multadd);

INCLUDE_ASM(const s32, "code_002C7CB0", _s2b);

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

INCLUDE_ASM(const s32, "code_002C7CB0", _multiply);

INCLUDE_ASM(const s32, "code_002C7CB0", _pow5mult);

INCLUDE_ASM(const s32, "code_002C7CB0", _lshift);

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

INCLUDE_ASM(const s32, "code_002C7CB0", __mdiff);

INCLUDE_ASM(const s32, "code_002C7CB0", _ulp);

INCLUDE_ASM(const s32, "code_002C7CB0", _b2d);

INCLUDE_ASM(const s32, "code_002C7CB0", _d2b);

INCLUDE_ASM(const s32, "code_002C7CB0", _ratio);

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

INCLUDE_ASM(const s32, "code_002C7CB0", isinf);

INCLUDE_ASM(const s32, "code_002C7CB0", isnan);

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

INCLUDE_ASM(const s32, "code_002C7CB0", _calloc_r);

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

INCLUDE_ASM(const s32, "code_002C7CB0", RFU000_FullReset);

INCLUDE_ASM(const s32, "code_002C7CB0", ResetEE);

INCLUDE_ASM(const s32, "code_002C7CB0", libc_2DBAA0);

INCLUDE_ASM(const s32, "code_002C7CB0", RFU003);

INCLUDE_ASM(const s32, "code_002C7CB0", libc_2DBAC0);

INCLUDE_ASM(const s32, "code_002C7CB0", RFU005);

INCLUDE_ASM(const s32, "code_002C7CB0", libc_2DBAE0);

INCLUDE_ASM(const s32, "code_002C7CB0", _ExecPS2);

INCLUDE_ASM(const s32, "code_002C7CB0", RFU008);

INCLUDE_ASM(const s32, "code_002C7CB0", RFU009);

INCLUDE_ASM(const s32, "code_002C7CB0", AddSbusIntcHandler);

INCLUDE_ASM(const s32, "code_002C7CB0", RemoveSbusIntcHandler);

INCLUDE_ASM(const s32, "code_002C7CB0", Interrupt2Iop);

INCLUDE_ASM(const s32, "code_002C7CB0", SetVTLBRefillHandler);

INCLUDE_ASM(const s32, "code_002C7CB0", SetVCommonHandler);

INCLUDE_ASM(const s32, "code_002C7CB0", SetVInterruptHandler);

INCLUDE_ASM(const s32, "code_002C7CB0", AddIntcHandler);

INCLUDE_ASM(const s32, "code_002C7CB0", AddIntcHandler2);

INCLUDE_ASM(const s32, "code_002C7CB0", RemoveIntcHandler);

INCLUDE_ASM(const s32, "code_002C7CB0", AddDmacHandler);

INCLUDE_ASM(const s32, "code_002C7CB0", AddDmacHandler2);

INCLUDE_ASM(const s32, "code_002C7CB0", RemoveDmacHandler);

INCLUDE_ASM(const s32, "code_002C7CB0", _EnableIntc);

INCLUDE_ASM(const s32, "code_002C7CB0", _DisableIntc);

INCLUDE_ASM(const s32, "code_002C7CB0", _EnableDmac);

INCLUDE_ASM(const s32, "code_002C7CB0", _DisableDmac);

INCLUDE_ASM(const s32, "code_002C7CB0", SetAlarm);

INCLUDE_ASM(const s32, "code_002C7CB0", ReleaseAlarm);

INCLUDE_ASM(const s32, "code_002C7CB0", _iEnableIntc);

INCLUDE_ASM(const s32, "code_002C7CB0", _iDisableIntc);

INCLUDE_ASM(const s32, "code_002C7CB0", _iEnableDmac);

INCLUDE_ASM(const s32, "code_002C7CB0", _iDisableDmac);

INCLUDE_ASM(const s32, "code_002C7CB0", iSetAlarm);

INCLUDE_ASM(const s32, "code_002C7CB0", iReleaseAlarm);

INCLUDE_ASM(const s32, "code_002C7CB0", CreateThread);

INCLUDE_ASM(const s32, "code_002C7CB0", libc_2DBCB0);

INCLUDE_ASM(const s32, "code_002C7CB0", StartThread);

INCLUDE_ASM(const s32, "code_002C7CB0", ExitThread);

INCLUDE_ASM(const s32, "code_002C7CB0", ExitDeleteThread);

INCLUDE_ASM(const s32, "code_002C7CB0", TerminateThread);

INCLUDE_ASM(const s32, "code_002C7CB0", iTerminateThread);

INCLUDE_ASM(const s32, "code_002C7CB0", DisableDispatchThread);

INCLUDE_ASM(const s32, "code_002C7CB0", EnableDispatchThread);

INCLUDE_ASM(const s32, "code_002C7CB0", ChangeThreadPriority);

INCLUDE_ASM(const s32, "code_002C7CB0", iChangeThreadPriority);

INCLUDE_ASM(const s32, "code_002C7CB0", RotateThreadReadyQueue);

INCLUDE_ASM(const s32, "code_002C7CB0", _iRotateThreadReadyQueue);

INCLUDE_ASM(const s32, "code_002C7CB0", ReleaseWaitThread);

INCLUDE_ASM(const s32, "code_002C7CB0", iReleaseWaitThread);

INCLUDE_ASM(const s32, "code_002C7CB0", GetThreadId);

INCLUDE_ASM(const s32, "code_002C7CB0", ReferThreadStatus);

INCLUDE_ASM(const s32, "code_002C7CB0", iReferThreadStatus);

INCLUDE_ASM(const s32, "code_002C7CB0", SleepThread);

INCLUDE_ASM(const s32, "code_002C7CB0", WakeupThread);

INCLUDE_ASM(const s32, "code_002C7CB0", _iWakeupThread);

INCLUDE_ASM(const s32, "code_002C7CB0", CancelWakeupThread);

INCLUDE_ASM(const s32, "code_002C7CB0", iCancelWakeupThread);

INCLUDE_ASM(const s32, "code_002C7CB0", SuspendThread);

INCLUDE_ASM(const s32, "code_002C7CB0", _iSuspendThread);

INCLUDE_ASM(const s32, "code_002C7CB0", ResumeThread);

INCLUDE_ASM(const s32, "code_002C7CB0", iResumeThread);

INCLUDE_ASM(const s32, "code_002C7CB0", JoinThread);

INCLUDE_ASM(const s32, "code_002C7CB0", RFU060);

INCLUDE_ASM(const s32, "code_002C7CB0", RFU061);

INCLUDE_ASM(const s32, "code_002C7CB0", EndOfHeap);

INCLUDE_ASM(const s32, "code_002C7CB0", RFU063);

INCLUDE_ASM(const s32, "code_002C7CB0", CreateSema);

INCLUDE_ASM(const s32, "code_002C7CB0", DeleteSema);

INCLUDE_ASM(const s32, "code_002C7CB0", SignalSema);

INCLUDE_ASM(const s32, "code_002C7CB0", iSignalSema);

INCLUDE_ASM(const s32, "code_002C7CB0", WaitSema);

INCLUDE_ASM(const s32, "code_002C7CB0", PollSema);

INCLUDE_ASM(const s32, "code_002C7CB0", iPollSema);

INCLUDE_ASM(const s32, "code_002C7CB0", ReferSemaStatus);

INCLUDE_ASM(const s32, "code_002C7CB0", iReferSemaStatus);

INCLUDE_ASM(const s32, "code_002C7CB0", RFU073);

INCLUDE_ASM(const s32, "code_002C7CB0", SetOsdConfigParam);

INCLUDE_ASM(const s32, "code_002C7CB0", GetOsdConfigParam);

INCLUDE_ASM(const s32, "code_002C7CB0", GetGsHParam);

INCLUDE_ASM(const s32, "code_002C7CB0", GetGsVParam);

INCLUDE_ASM(const s32, "code_002C7CB0", SetGsHParam);

INCLUDE_ASM(const s32, "code_002C7CB0", SetGsVParam);

INCLUDE_ASM(const s32, "code_002C7CB0", RFU080_CreateEventFlag);

INCLUDE_ASM(const s32, "code_002C7CB0", RFU081_DeleteEventFlag);

INCLUDE_ASM(const s32, "code_002C7CB0", RFU082_SetEventFlag);

INCLUDE_ASM(const s32, "code_002C7CB0", RFU083_iSetEventFlag);

INCLUDE_ASM(const s32, "code_002C7CB0", RFU084_ClearEventFlag);

INCLUDE_ASM(const s32, "code_002C7CB0", RFU085_iClearEventFlag);

INCLUDE_ASM(const s32, "code_002C7CB0", RFU086_WaitEvnetFlag);

INCLUDE_ASM(const s32, "code_002C7CB0", RFU087_PollEvnetFlag);

INCLUDE_ASM(const s32, "code_002C7CB0", RFU088_iPollEvnetFlag);

INCLUDE_ASM(const s32, "code_002C7CB0", RFU089_ReferEventFlagStatus);

INCLUDE_ASM(const s32, "code_002C7CB0", RFU090_iReferEventFlagStatus);

INCLUDE_ASM(const s32, "code_002C7CB0", RFU091);

INCLUDE_ASM(const s32, "code_002C7CB0", EnableIntcHandler);

INCLUDE_ASM(const s32, "code_002C7CB0", iEnableIntcHandler);

INCLUDE_ASM(const s32, "code_002C7CB0", DisableIntcHandler);

INCLUDE_ASM(const s32, "code_002C7CB0", iDisableIntcHandler);

INCLUDE_ASM(const s32, "code_002C7CB0", EnableDmacHandler);

INCLUDE_ASM(const s32, "code_002C7CB0", iEnableDmacHandler);

INCLUDE_ASM(const s32, "code_002C7CB0", DisableDmacHandler);

INCLUDE_ASM(const s32, "code_002C7CB0", iDisableDmacHandler);

INCLUDE_ASM(const s32, "code_002C7CB0", KSeg0);

INCLUDE_ASM(const s32, "code_002C7CB0", EnableCache);

INCLUDE_ASM(const s32, "code_002C7CB0", DisableCache);

INCLUDE_ASM(const s32, "code_002C7CB0", GetCop0);

INCLUDE_ASM(const s32, "code_002C7CB0", FlushCache);

INCLUDE_ASM(const s32, "code_002C7CB0", CpuConfig);

INCLUDE_ASM(const s32, "code_002C7CB0", iGetCop0);

INCLUDE_ASM(const s32, "code_002C7CB0", iFlushCache);

INCLUDE_ASM(const s32, "code_002C7CB0", iCpuConfig);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifStopDma);

INCLUDE_ASM(const s32, "code_002C7CB0", SetCPUTimerHandler);

INCLUDE_ASM(const s32, "code_002C7CB0", SetCPUTimer);

INCLUDE_ASM(const s32, "code_002C7CB0", SetOsdConfigParam2);

INCLUDE_ASM(const s32, "code_002C7CB0", GetOsdConfigParam2);

INCLUDE_ASM(const s32, "code_002C7CB0", GsGetIMR);

INCLUDE_ASM(const s32, "code_002C7CB0", iGsGetIMR);

INCLUDE_ASM(const s32, "code_002C7CB0", GsPutIMR);

INCLUDE_ASM(const s32, "code_002C7CB0", iGsPutIMR);

INCLUDE_ASM(const s32, "code_002C7CB0", SetPgifHandler);

INCLUDE_ASM(const s32, "code_002C7CB0", SetVSyncFlag);

INCLUDE_ASM(const s32, "code_002C7CB0", RFU116);

INCLUDE_ASM(const s32, "code_002C7CB0", _print);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifDmaStat);

INCLUDE_ASM(const s32, "code_002C7CB0", isceSifDmaStat);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifSetDma);

INCLUDE_ASM(const s32, "code_002C7CB0", isceSifSetDma);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifSetDChain);

INCLUDE_ASM(const s32, "code_002C7CB0", isceSifSetDChain);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifSetReg);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifGetReg);

INCLUDE_ASM(const s32, "code_002C7CB0", libc_2DC2C0);

INCLUDE_ASM(const s32, "code_002C7CB0", Deci2Call);

INCLUDE_ASM(const s32, "code_002C7CB0", PSMode);

INCLUDE_ASM(const s32, "code_002C7CB0", MachineType);

INCLUDE_ASM(const s32, "code_002C7CB0", GetMemorySize);

INCLUDE_ASM(const s32, "code_002C7CB0", _InitTLB);

INCLUDE_ASM(const s32, "code_002C7CB0", sceResetttyinit);

INCLUDE_ASM(const s32, "code_002C7CB0", VSync);

INCLUDE_ASM(const s32, "code_002C7CB0", VSync2);

INCLUDE_ASM(const s32, "code_002C7CB0", write);

INCLUDE_ASM(const s32, "code_002C7CB0", read);

INCLUDE_ASM(const s32, "code_002C7CB0", open);

INCLUDE_ASM(const s32, "code_002C7CB0", close);

INCLUDE_ASM(const s32, "code_002C7CB0", ioctl);

INCLUDE_ASM(const s32, "code_002C7CB0", lseek);

INCLUDE_ASM(const s32, "code_002C7CB0", sbrk);

INCLUDE_ASM(const s32, "code_002C7CB0", isatty);

INCLUDE_ASM(const s32, "code_002C7CB0", fstat);

INCLUDE_ASM(const s32, "code_002C7CB0", libc_2DC670);

INCLUDE_ASM(const s32, "code_002C7CB0", kill);

INCLUDE_ASM(const s32, "code_002C7CB0", stat);

INCLUDE_ASM(const s32, "code_002C7CB0", unlink);

INCLUDE_ASM(const s32, "code_002C7CB0", _sceSDC);

INCLUDE_ASM(const s32, "code_002C7CB0", SyncDCache);

INCLUDE_ASM(const s32, "code_002C7CB0", iSyncDCache);

INCLUDE_ASM(const s32, "code_002C7CB0", _sceIDC);

INCLUDE_ASM(const s32, "code_002C7CB0", InvalidDCache);

INCLUDE_ASM(const s32, "code_002C7CB0", iInvalidDCache);

INCLUDE_ASM(const s32, "code_002C7CB0", func_002DC970);

INCLUDE_ASM(const s32, "code_002C7CB0", DisableIntc);

INCLUDE_ASM(const s32, "code_002C7CB0", EnableIntc);

INCLUDE_ASM(const s32, "code_002C7CB0", DisableDmac);

INCLUDE_ASM(const s32, "code_002C7CB0", EnableDmac);

INCLUDE_ASM(const s32, "code_002C7CB0", iEnableIntc);

INCLUDE_ASM(const s32, "code_002C7CB0", iDisableIntc);

INCLUDE_ASM(const s32, "code_002C7CB0", iEnableDmac);

INCLUDE_ASM(const s32, "code_002C7CB0", iDisableDmac);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054B91C);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054B928);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054B930);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054B9F8);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054BA18);

INCLUDE_ASM(const s32, "code_002C7CB0", topThread);

INCLUDE_ASM(const s32, "code_002C7CB0", InitThread);

INCLUDE_ASM(const s32, "code_002C7CB0", iWakeupThread);

INCLUDE_ASM(const s32, "code_002C7CB0", iRotateThreadReadyQueue);

INCLUDE_ASM(const s32, "code_002C7CB0", iSuspendThread);

INCLUDE_ASM(const s32, "code_002C7CB0", gcc2_compiled__10);

INCLUDE_ASM(const s32, "code_002C7CB0", QueuePeekWriteDone);

INCLUDE_ASM(const s32, "code_002C7CB0", QueuePeekReadDone);

INCLUDE_ASM(const s32, "code_002C7CB0", sceTtyHandler);

INCLUDE_ASM(const s32, "code_002C7CB0", sceTtyWrite);

INCLUDE_ASM(const s32, "code_002C7CB0", sceTtyRead);

INCLUDE_ASM(const s32, "code_002C7CB0", sceTtyInit);

INCLUDE_ASM(const s32, "code_002C7CB0", kputchar);

INCLUDE_ASM(const s32, "code_002C7CB0", deci2Putchar);

INCLUDE_ASM(const s32, "code_002C7CB0", serialPutchar);

INCLUDE_ASM(const s32, "code_002C7CB0", ftoi);

INCLUDE_ASM(const s32, "code_002C7CB0", printfloat);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054BAF8);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054BB00);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054BB08);

INCLUDE_ASM(const s32, "code_002C7CB0", _printf);

INCLUDE_ASM(const s32, "code_002C7CB0", kprintf);

INCLUDE_ASM(const s32, "code_002C7CB0", scePrintf);

INCLUDE_ASM(const s32, "code_002C7CB0", gcc2_compiled__12);

INCLUDE_ASM(const s32, "code_002C7CB0", _change_addr);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifGetSreg);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifSetSreg);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifGetDataTable);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifInitCmd);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifExitCmd);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifSetCmdBuffer);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifSetSysCmdBuffer);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifAddCmdHandler);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifRemoveCmdHandler);

INCLUDE_ASM(const s32, "code_002C7CB0", _sceSifSendCmd);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifSendCmd);

INCLUDE_ASM(const s32, "code_002C7CB0", isceSifSendCmd);

INCLUDE_ASM(const s32, "code_002C7CB0", _sceSifCmdIntrHdlr);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifWriteBackDCache);

INCLUDE_ASM(const s32, "code_002C7CB0", func_002DE504);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifInitRpc);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifExitRpc);

INCLUDE_ASM(const s32, "code_002C7CB0", _sceRpcGetPacket_0);

INCLUDE_ASM(const s32, "code_002C7CB0", _sceRpcFreePacket_0);

INCLUDE_ASM(const s32, "code_002C7CB0", _sceRpcGetFPacket);

INCLUDE_ASM(const s32, "code_002C7CB0", _sceRpcGetFPacket2);

INCLUDE_ASM(const s32, "code_002C7CB0", _request_end_0);

INCLUDE_ASM(const s32, "code_002C7CB0", _request_rdata);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifGetOtherData);

INCLUDE_ASM(const s32, "code_002C7CB0", _search_svdata);

INCLUDE_ASM(const s32, "code_002C7CB0", _request_bind);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifBindRpc);

INCLUDE_ASM(const s32, "code_002C7CB0", _request_call);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifCallRpc);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifCheckStatRpc);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifSetRpcQueue);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifRegisterRpc);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifRemoveRpc);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifRemoveRpcQueue);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifGetNextRequest);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifExecRequest);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifRpcLoop);

INCLUDE_ASM(const s32, "code_002C7CB0", gcc2_compiled__13);

INCLUDE_ASM(const s32, "code_002C7CB0", new_iob);

INCLUDE_ASM(const s32, "code_002C7CB0", get_iob);

INCLUDE_ASM(const s32, "code_002C7CB0", _sceFs_Rcv_Intr);

INCLUDE_ASM(const s32, "code_002C7CB0", _sceFsSemInit);

INCLUDE_ASM(const s32, "code_002C7CB0", _sceFsWaitS);

INCLUDE_ASM(const s32, "code_002C7CB0", _sceFsSigSema);

INCLUDE_ASM(const s32, "code_002C7CB0", scePowerOffHandler);

INCLUDE_ASM(const s32, "code_002C7CB0", _sceFs_Poff_Intr);

INCLUDE_ASM(const s32, "code_002C7CB0", sceFsInit);

INCLUDE_ASM(const s32, "code_002C7CB0", _fs_version);

INCLUDE_ASM(const s32, "code_002C7CB0", sceFsReset);

INCLUDE_ASM(const s32, "code_002C7CB0", sceOpen);

INCLUDE_ASM(const s32, "code_002C7CB0", sceClose);

INCLUDE_ASM(const s32, "code_002C7CB0", sceLseek);

INCLUDE_ASM(const s32, "code_002C7CB0", sceRead);

INCLUDE_ASM(const s32, "code_002C7CB0", sceWrite);

INCLUDE_ASM(const s32, "code_002C7CB0", sceIoctl);

INCLUDE_ASM(const s32, "code_002C7CB0", sceIoctl2);

INCLUDE_ASM(const s32, "code_002C7CB0", _sceCallCode);

INCLUDE_ASM(const s32, "code_002C7CB0", sceRemove);

INCLUDE_ASM(const s32, "code_002C7CB0", sceMkdir);

INCLUDE_ASM(const s32, "code_002C7CB0", sceRmdir);

INCLUDE_ASM(const s32, "code_002C7CB0", sceFormat);

INCLUDE_ASM(const s32, "code_002C7CB0", sceAddDrv);

INCLUDE_ASM(const s32, "code_002C7CB0", sceDelDrv);

INCLUDE_ASM(const s32, "code_002C7CB0", sceDopen);

INCLUDE_ASM(const s32, "code_002C7CB0", sceDclose);

INCLUDE_ASM(const s32, "code_002C7CB0", sceDread);

INCLUDE_ASM(const s32, "code_002C7CB0", sceGetstat);

INCLUDE_ASM(const s32, "code_002C7CB0", sceChstat);

INCLUDE_ASM(const s32, "code_002C7CB0", sceRename);

INCLUDE_ASM(const s32, "code_002C7CB0", sceChdir);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSync);

INCLUDE_ASM(const s32, "code_002C7CB0", sceMount);

INCLUDE_ASM(const s32, "code_002C7CB0", sceUmount);

INCLUDE_ASM(const s32, "code_002C7CB0", sceLseek64);

INCLUDE_ASM(const s32, "code_002C7CB0", sceDevctl);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSymlink);

INCLUDE_ASM(const s32, "code_002C7CB0", sceReadlink);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifInitIopHeap);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifAllocIopHeap);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifAllocSysMemory);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifFreeSysMemory);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifFreeIopHeap);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifLoadIopHeap);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifQueryMemSize);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifQueryMaxFreeMemSize);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifQueryTotalFreeMemSize);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifQueryBlockTopAddress);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifQueryBlockSize);

INCLUDE_ASM(const s32, "code_002C7CB0", _lf_bind);

INCLUDE_ASM(const s32, "code_002C7CB0", _lf_version);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifLoadFileReset);

INCLUDE_ASM(const s32, "code_002C7CB0", _sceSifLoadModuleBuffer);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifStopModule);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifUnloadModule);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifSearchModuleByName);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifSearchModuleByAddress);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifLoadModuleBuffer);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifLoadStartModuleBuffer);

INCLUDE_ASM(const s32, "code_002C7CB0", _sceSifLoadModule);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifLoadModule);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifLoadStartModule);

INCLUDE_ASM(const s32, "code_002C7CB0", _sceSifLoadElfPart);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifLoadElfPart);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054BCA8);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054BCB0);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifLoadElf);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifGetIopAddr);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifSetIopAddr);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifResetIop);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifIsAliveIop);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifSyncIop);

INCLUDE_ASM(const s32, "code_002C7CB0", sceSifRebootIop);

INCLUDE_ASM(const s32, "code_002C7CB0", DIntr);

INCLUDE_ASM(const s32, "code_002C7CB0", EIntr);

INCLUDE_ASM(const s32, "code_002C7CB0", supplement_crt0);

INCLUDE_ASM(const s32, "code_002C7CB0", Copy);

INCLUDE_ASM(const s32, "code_002C7CB0", kCopy);

INCLUDE_ASM(const s32, "code_002C7CB0", kFindAddress);

INCLUDE_ASM(const s32, "code_002C7CB0", FindAddress);

INCLUDE_ASM(const s32, "code_002C7CB0", GetSystemCallEntry);

INCLUDE_ASM(const s32, "code_002C7CB0", GetSystemCallTableEntry);

INCLUDE_ASM(const s32, "code_002C7CB0", setup);

INCLUDE_ASM(const s32, "code_002C7CB0", _InitSys);

INCLUDE_ASM(const s32, "code_002C7CB0", gcc2_compiled__15);

INCLUDE_ASM(const s32, "code_002C7CB0", Copy_0);

INCLUDE_ASM(const s32, "code_002C7CB0", kCopy_0);

INCLUDE_ASM(const s32, "code_002C7CB0", GetEntryAddress);

INCLUDE_ASM(const s32, "code_002C7CB0", PatchIsNeeded_0);

INCLUDE_ASM(const s32, "code_002C7CB0", InitExecPS2);

INCLUDE_ASM(const s32, "code_002C7CB0", gcc2_compiled__16);

INCLUDE_ASM(const s32, "code_002C7CB0", kCopy_1);

INCLUDE_ASM(const s32, "code_002C7CB0", PatchIsNeeded);

INCLUDE_ASM(const s32, "code_002C7CB0", SetArg);

INCLUDE_ASM(const s32, "code_002C7CB0", TerminateLibrary);

INCLUDE_ASM(const s32, "code_002C7CB0", ExecPS2);

INCLUDE_ASM(const s32, "code_002C7CB0", LoadExecPS2);

INCLUDE_ASM(const s32, "code_002C7CB0", Exit);

INCLUDE_ASM(const s32, "code_002C7CB0", ExecOSD);

INCLUDE_ASM(const s32, "code_002C7CB0", gcc2_compiled__17);

INCLUDE_ASM(const s32, "code_002C7CB0", Copy_1);

INCLUDE_ASM(const s32, "code_002C7CB0", kCopy_2);

INCLUDE_ASM(const s32, "code_002C7CB0", GetEntryAddress_0);

INCLUDE_ASM(const s32, "code_002C7CB0", InitAlarm);

INCLUDE_ASM(const s32, "code_002C7CB0", sceDeci2Open);

INCLUDE_ASM(const s32, "code_002C7CB0", sceDeci2Close);

INCLUDE_ASM(const s32, "code_002C7CB0", sceDeci2ReqSend);

INCLUDE_ASM(const s32, "code_002C7CB0", sceDeci2Poll);

INCLUDE_ASM(const s32, "code_002C7CB0", sceDeci2ExRecv);

INCLUDE_ASM(const s32, "code_002C7CB0", sceDeci2ExSend);

INCLUDE_ASM(const s32, "code_002C7CB0", sceDeci2ExReqSend);

INCLUDE_ASM(const s32, "code_002C7CB0", sceDeci2ExLock);

INCLUDE_ASM(const s32, "code_002C7CB0", sceDeci2ExUnLock);

INCLUDE_ASM(const s32, "code_002C7CB0", kputs);

INCLUDE_ASM(const s32, "code_002C7CB0", SetTLBHandler);

INCLUDE_ASM(const s32, "code_002C7CB0", SetDebugHandler);

INCLUDE_ASM(const s32, "code_002C7CB0", Copy_2);

INCLUDE_ASM(const s32, "code_002C7CB0", kCopy_3);

INCLUDE_ASM(const s32, "code_002C7CB0", GetEntryAddress_1);

INCLUDE_ASM(const s32, "code_002C7CB0", setup_0);

INCLUDE_ASM(const s32, "code_002C7CB0", InitTLBFunctions);

INCLUDE_ASM(const s32, "code_002C7CB0", PutTLBEntry);

INCLUDE_ASM(const s32, "code_002C7CB0", iPutTLBEntry_0);

INCLUDE_ASM(const s32, "code_002C7CB0", _SetTLBEntry);

INCLUDE_ASM(const s32, "code_002C7CB0", iPutTLBEntry);

INCLUDE_ASM(const s32, "code_002C7CB0", iSetTLBEntry);

INCLUDE_ASM(const s32, "code_002C7CB0", GetTLBEntry);

INCLUDE_ASM(const s32, "code_002C7CB0", iGetTLBEntry);

INCLUDE_ASM(const s32, "code_002C7CB0", ProbeTLBEntry);

INCLUDE_ASM(const s32, "code_002C7CB0", iProbeTLBEntry);

INCLUDE_ASM(const s32, "code_002C7CB0", ExpandScratchPad);

INCLUDE_ASM(const s32, "code_002C7CB0", InitTLB);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054BCF0);

INCLUDE_ASM(const s32, "code_002C7CB0", InitTLB32MB);

INCLUDE_ASM(const s32, "code_002C7CB0", _kTLBException);

INCLUDE_ASM(const s32, "code_002C7CB0", _xlaunch);

INCLUDE_ASM(const s32, "code_002C7CB0", _kDebugException);

INCLUDE_ASM(const s32, "code_002C7CB0", libc_2E52FC);

INCLUDE_ASM(const s32, "code_002C7CB0", libc_2E5320);

INCLUDE_ASM(const s32, "code_002C7CB0", libc_2E59B8);

INCLUDE_ASM(const s32, "code_002C7CB0", libc_2E6048);

INCLUDE_ASM(const s32, "code_002C7CB0", libc_2E65C8);

INCLUDE_ASM(const s32, "code_002C7CB0", libc_2E6B30);

INCLUDE_ASM(const s32, "code_002C7CB0", libc_2E6C48);

INCLUDE_ASM(const s32, "code_002C7CB0", libc_2E6D88);

INCLUDE_ASM(const s32, "code_002C7CB0", __errno_8);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054BD78);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054BE78);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054BF78);

INCLUDE_RODATA(const s32, "code_002C7CB0", D_0054C078);

