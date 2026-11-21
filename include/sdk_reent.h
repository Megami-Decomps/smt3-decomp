#ifndef SDK_REENT_H
#define SDK_REENT_H

#include "sdk_stdio_record.h"
#include "sdk_reent_aux.h"

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
    /* Initialized newlib RNG state inside the existing scratch area. */
    u8 _reserved_before_rand[0x58];
    u64 _rand_next;
    u8 _reserved_after_rand[0x98];
    _atexit *_atexit;
    _atexit _atexit0;
    void (**_sig_func)(s32 sig);
    struct _glue __sglue;
    FILE __sf[3];
};

extern struct _reent impure_data;
extern struct _reent *_impure_ptr __attribute__((section(".data")));

#endif
