#include "common.h"

extern char D_5A0330[];
extern void serialPutchar(s32 character);

/* The builtin IOP module tag and the kernel's current EE service state. */
u8 kernelBuiltinVersion[4] __attribute__((section(".data"))) = { '2', '5', '0', '0' };
s32 ttyInitDone __attribute__((section(".data"))) = 0;
char *sbrkBreak __attribute__((section(".data"))) = D_5A0330;
s32 topThreadId __attribute__((section(".data"))) = 0;
s32 deci2LineIndex __attribute__((section(".data"))) = 0;
void (*printfPutchar)(s32) __attribute__((section(".data"))) = serialPutchar;
