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
extern void gcc2_compiled__7(u32 chcr);

/* The IPU needs four slots after a CTRL read before the value is usable; the
 * test instruction counts as one of them, so a masked read pads three. */
#define IPU_HAZARD_WAIT() __asm__ volatile ("nop\nnop\nnop\nnop")
#define IPU_HAZARD_WAIT3() __asm__ volatile ("nop\nnop\nnop")

INCLUDE_ASM(const s32, "sdk/libipu", gcc2_compiled__7);

void setD4_CHCR(u32 chcr)
{
    DIntr();
    D_ENABLEW = D_ENABLER | 0x10000;
    D4_CHCR = chcr;
    D_ENABLEW = D_ENABLER & ~0x10000;
    EIntr();
}

INCLUDE_ASM(const s32, "sdk/libipu", sceIpuStopDMA);

INCLUDE_ASM(const s32, "sdk/libipu", sceIpuRestartDMA);

/* NON_MATCHING: 100 bytes against retail's 104; the mode == 1 arm is emitted
 * inline where retail branches to it out of line. */
#ifdef NON_MATCHING
u32 sceIpuSync(s32 mode)
{
    u32 ret = 0;

    if (mode != 0) {
        if (mode == 1) {
            ret = IPU_CTRL >> 31;
        }
    } else {
        s32 ctrl;

        do {
            ctrl = IPU_CTRL;
            IPU_HAZARD_WAIT();
        } while (ctrl < 0);
        ret = 0;
    }
    return ret;
}
#else
INCLUDE_ASM(const s32, "sdk/libipu", sceIpuSync);
#endif

INCLUDE_ASM(const s32, "sdk/libipu", gcc2_compiled__8);

INCLUDE_ASM(const s32, "sdk/libipu", sceIpuInit);
