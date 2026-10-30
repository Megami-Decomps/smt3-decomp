#include "common.h"

#define IPU_CMD  (*(volatile u32 *)0x10002000)
#define IPU_CTRL (*(volatile u32 *)0x10002010)
#define IPU_BP   (*(volatile u32 *)0x10002020)

#define D3_CHCR (*(volatile u32 *)0x1000B000)
#define D3_MADR (*(volatile u32 *)0x1000B010)
#define D3_QWC  (*(volatile u32 *)0x1000B020)

#define D4_CHCR (*(volatile u32 *)0x1000B400)
#define D4_MADR (*(volatile u32 *)0x1000B410)
#define D4_QWC  (*(volatile u32 *)0x1000B420)
#define D4_TADR (*(volatile u32 *)0x1000B430)

#define D_ENABLER (*(volatile u32 *)0x1000F520)
#define D_ENABLEW (*(volatile u32 *)0x1000F590)

extern s32 DIntr(void);
extern s32 EIntr(void);

/* The sibling that drives channel 3; the IDB still carries the object's
 * gcc2_compiled label as its name. */
extern void ipuWriteFromIpuChcr(u32 chcr);

/* With the configured 2.9 compiler, this barrier plus its generated padding
 * leaves four slots after an IPU_CTRL read: three nops after a mask, or four
 * nops before a signed test. See docs/matching/sdk/libipu.md. */
#define IPU_POLL_BARRIER() __asm__ volatile ("nop")

void ipuWriteFromIpuChcr(u32 chcr)
{
    DIntr();
    D_ENABLEW = D_ENABLER | 0x10000;
    D3_CHCR = chcr;
    D_ENABLEW = D_ENABLER & ~0x10000;
    EIntr();
}

void setD4_CHCR(u32 chcr)
{
    DIntr();
    D_ENABLEW = D_ENABLER | 0x10000;
    D4_CHCR = chcr;
    D_ENABLEW = D_ENABLER & ~0x10000;
    EIntr();
}

typedef struct sdfIpuDmaRegs_t {
    u32 inputAddress;
    u32 inputTagAddress;
    u32 inputQwords;
    u32 inputControl;
    u32 outputAddress;
    u32 outputQwords;
    u32 outputControl;
    u32 bitPointer;
    u32 ipuControl;
} sdfIpuDmaRegs_t;

void sceIpuStopDMA(sdfIpuDmaRegs_t *state)
{
    u32 ctrl;

    setD4_CHCR(1);
    state->inputAddress = D4_MADR;
    state->inputTagAddress = D4_TADR;
    state->inputQwords = D4_QWC;
    state->inputControl = D4_CHCR;
    do {
        ctrl = IPU_CTRL & 0xF0;
        IPU_POLL_BARRIER();
    } while (ctrl != 0);
    ipuWriteFromIpuChcr(0);
    state->outputAddress = D3_MADR;
    state->outputQwords = D3_QWC;
    state->outputControl = D3_CHCR;
    state->bitPointer = IPU_BP;
    state->ipuControl = IPU_CTRL;
}

/* NON_MATCHING: 74 of 84 words match at the native 336-byte size; the
 * bit-pointer fields and saved input counts use different prefix registers. */
#ifdef NON_MATCHING
void sceIpuRestartDMA(sdfIpuDmaRegs_t *state)
{
    u32 bp = state->bitPointer;
    u32 buffered = ((bp >> 8) & 15) + ((bp >> 16) & 3);
    u32 bit = bp & 127;
    u32 qwc = state->inputQwords + buffered;
    u32 madr = state->inputAddress - (buffered << 4);
    s32 ctrl;

    if (state->outputAddress != 0 && state->outputQwords != 0) {
        D3_MADR = state->outputAddress;
        D3_QWC = state->outputQwords;
        ipuWriteFromIpuChcr(state->outputControl | 0x100);
    }
    do {
        ctrl = IPU_CTRL;
        IPU_POLL_BARRIER();
    } while (ctrl < 0);
    IPU_CMD = bit;
    do {
        ctrl = IPU_CTRL;
        IPU_POLL_BARRIER();
    } while (ctrl < 0);
    if (madr != 0 && qwc != 0) {
        D4_MADR = madr;
        D4_TADR = state->inputTagAddress;
        D4_QWC = qwc;
        setD4_CHCR(state->inputControl | 0x100);
    }
}
#else
INCLUDE_ASM(const s32, "sdk/libipu", sceIpuRestartDMA);
#endif

u32 sceIpuSync(s32 mode)
{
    u32 ret = 0;

    switch (mode) {
    case 0: {
        s32 ctrl;

        do {
            ctrl = IPU_CTRL;
            IPU_POLL_BARRIER();
        } while (ctrl < 0);
        ret = 0;
        break;
    }
    case 1:
        ret = IPU_CTRL >> 31;
        break;
    }
    return ret;
}

void gcc2_compiled__8(u32 chcr)
{
    DIntr();
    D_ENABLEW = D_ENABLER | 0x10000;
    D4_CHCR = chcr;
    D_ENABLEW = D_ENABLER & ~0x10000;
    EIntr();
}

INCLUDE_ASM(const s32, "sdk/libipu", sceIpuInit);
