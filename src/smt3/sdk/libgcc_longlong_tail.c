#include "common.h"
struct _reent;
extern struct _reent *_impure_ptr __attribute__((section(".data")));

/* _errno is the first member of the canonical struct _reent in libc.c. */
s32 *__errno_8(void)
{
    return (s32 *)_impure_ptr;
}
