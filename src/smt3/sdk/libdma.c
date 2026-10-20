#include "common.h"

/* D_STADR, the stall address the DMAC compares source-chain transfers against. */
#define D_STADR ((u32 *)0x1000E060)

/* A channel's register block. Fields are added as the functions that reach
 * them are recovered; sceDmaGetChan only hands the address back. */
typedef struct sceDmaChan sceDmaChan;

extern sceDmaChan *sceDmaChanRegs[10];

INCLUDE_ASM(const s32, "sdk/libdma", WaitDma);

INCLUDE_ASM(const s32, "sdk/libdma", WatchDma);

INCLUDE_ASM(const s32, "sdk/libdma", CheckAddress);

INCLUDE_ASM(const s32, "sdk/libdma", memclr);

sceDmaChan *sceDmaGetChan(u32 channel)
{
    if (channel < 10) {
        return sceDmaChanRegs[channel];
    }
    return NULL;
}

INCLUDE_ASM(const s32, "sdk/libdma", sceDmaReset);

INCLUDE_ASM(const s32, "sdk/libdma", sceDmaPutEnv);

INCLUDE_ASM(const s32, "sdk/libdma", sceDmaGetEnv);

u32 sceDmaPutStallAddr(u32 addr)
{
    u32 *reg = D_STADR;
    u32 old = *reg;

    *reg = addr;
    return old;
}

INCLUDE_ASM(const s32, "sdk/libdma", sceDmaSend);

INCLUDE_ASM(const s32, "sdk/libdma", sceDmaSendN);

INCLUDE_ASM(const s32, "sdk/libdma", sceDmaSendI);

INCLUDE_ASM(const s32, "sdk/libdma", sceDmaRecv);

INCLUDE_ASM(const s32, "sdk/libdma", sceDmaRecvN);

INCLUDE_ASM(const s32, "sdk/libdma", sceDmaRecvI);

INCLUDE_ASM(const s32, "sdk/libdma", sceDmaSync);

INCLUDE_ASM(const s32, "sdk/libdma", sceDmaWatch);

INCLUDE_ASM(const s32, "sdk/libdma", sceDmaPause);

INCLUDE_ASM(const s32, "sdk/libdma", sceDmaRestart);

INCLUDE_RODATA(const s32, "sdk/libdma", D_0054A8F0);

INCLUDE_RODATA(const s32, "sdk/libdma", D_0054A908);

INCLUDE_RODATA(const s32, "sdk/libdma", D_0054A930);

INCLUDE_RODATA(const s32, "sdk/libdma", D_0054A940);

INCLUDE_RODATA(const s32, "sdk/libdma", D_0054A950);

