#ifndef SMT3_SDK_GS_STORE_IMAGE_H
#define SMT3_SDK_GS_STORE_IMAGE_H

#include "common.h"

typedef union {
    u128 qword;
    struct {
        u64 NLOOP : 15;
        u64 EOP : 1;
        u64 pad16 : 30;
        u64 PRE : 1;
        u64 PRIM : 11;
        u64 FLG : 2;
        u64 NREG : 4;
        u64 REGS0 : 4;
        u64 REGS1 : 4;
        u64 REGS2 : 4;
        u64 REGS3 : 4;
        u64 REGS4 : 4;
        u64 REGS5 : 4;
        u64 REGS6 : 4;
        u64 REGS7 : 4;
        u64 REGS8 : 4;
        u64 REGS9 : 4;
        u64 REGS10 : 4;
        u64 REGS11 : 4;
        u64 REGS12 : 4;
        u64 REGS13 : 4;
        u64 REGS14 : 4;
        u64 REGS15 : 4;
    } f;
} sceGifTag;

typedef struct sceGsStoreImage {
    u32 qwc;
    u32 addr;
    u32 vifcode0;
    u32 vifcode1;
    sceGifTag tag;
    u64 bitbltbuf;
    u64 reg_bitbltbuf;
    u64 trxpos;
    u64 reg_trxpos;
    u64 trxreg;
    u64 reg_trxreg;
    u64 finish;
    u64 reg_finish;
    u64 trxdir;
    u64 reg_trxdir;
} sceGsStoreImage;

SIZE_CHECK(sceGsStoreImageNativeSize, sceGsStoreImage, 0x70);

s32 sceGsSetDefStoreImage(sceGsStoreImage *p, s16 sbp, s16 sbw, s16 spsm,
                          s16 ssax, s16 ssay, s16 rrw, s16 rrh);
s32 sceGsExecStoreImage(sceGsStoreImage *pkt, void *addr);

#endif
