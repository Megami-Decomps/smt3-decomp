#include "common.h"
#include "ps2asm.h"

/* The GS interrupt mask register, through the kernel's syscall stubs. */
extern u64 GsGetIMR(void);
extern u64 GsPutIMR(u64 imr);

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

typedef struct {
    s16 interlace;
    u16 mode;
    s16 fieldMode;
    s16 version;
    s32 intcHandlerEnabled;
    s32 intcHandlerId;
} GParam;

typedef struct {
    u64 pmode;
    u64 smode2;
    u64 dispfb;
    u64 display;
    u64 bgcolor;
} sceGsDispEnv;

typedef struct {
    sceGifTag tag;
    u64 bitbltbuf;
    u64 reg_bitbltbuf;
    u64 trxpos;
    u64 reg_trxpos;
    u64 trxreg;
    u64 reg_trxreg;
    u64 trxdir;
    u64 reg_trxdir;
    sceGifTag imagetag;
} sceGsLoadImage;

typedef struct {
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

#define D2_CHCR (*(volatile u32 *)0x1000A000)
#define D2_MADR (*(volatile u32 *)0x1000A010)
#define D2_QWC (*(volatile u32 *)0x1000A020)

extern GParam D_00529F20;
extern void scePrintf(const char *fmt, ...);

extern s32 sceOpen(const char *name, s32 flag);
extern s32 sceRead(s32 fd, void *buf, s32 size);
extern s32 sceClose(s32 a);
extern s32 atoi(const char *s);

extern s32 DisableIntc(s32 cause);
extern s32 RemoveIntcHandler(s32 cause, s32 handlerId);
/* Syscall 2 has the SetGsCrt void interface. */
extern void libc_2DBAA0(s16 interlace, s16 mode, s16 fieldMode);
GParam *sceGsGetGParam(void);

void sceGsResetGraph(s16 mode, s16 interlace, s16 outputMode, s16 fieldMode)
{
    GParam *param;

    switch (mode) {
    case 0:
        param = sceGsGetGParam();
        *(volatile u64 *)0x12001000 = 0x200;
        param->interlace = interlace;
        param->mode = outputMode;
        param->version = (*(volatile u64 *)0x12001000 >> 16) & 0xFF;
        GsPutIMR(0xFF00);
        param->fieldMode = fieldMode != 0;
        if (param->intcHandlerEnabled != 0) {
            DisableIntc(2);
            RemoveIntcHandler(2, param->intcHandlerId);
            param->intcHandlerEnabled = 0;
            param->intcHandlerId = 0;
        }
        libc_2DBAA0(interlace & 1, outputMode & 0xFF, fieldMode & 1);
        break;
    case 1:
        *(volatile u64 *)0x12001000 = 0x100;
        break;
    case 5:
        param = sceGsGetGParam();
        param->fieldMode = fieldMode != 0;
        param->interlace = interlace;
        param->mode = outputMode;
        param->version = (*(volatile u64 *)0x12001000 >> 16) & 0xFF;
        libc_2DBAA0(interlace & 1, outputMode & 0xFF, fieldMode & 1);
        break;
    }
}

GParam *sceGsGetGParam(void)
{
    return &D_00529F20;
}

INCLUDE_ASM(const s32, "sdk/libgraph", sceGsResetPath);

s32 checkModelVersion(void)
{
    s8 buf[0x100];
    s32 fd;
    u32 i;

    fd = sceOpen("rom0:ROMVER", 1);
    if (fd < 0) {
        return -1;
    }
    for (i = 0; i < 0x100; i++) {
        sceRead(fd, &buf[i], 1);
        if (buf[i] == 0) {
            break;
        }
    }
    sceClose(fd);
    return 0x1315670 < atoi(&buf[i - 9]);
}

INCLUDE_ASM(const s32, "sdk/libgraph", _GetGsDxDyOffset);

INCLUDE_ASM(const s32, "sdk/libgraph", sceGsSetDefDispEnv);

void sceGsPutDispEnv(sceGsDispEnv *disp)
{
    GParam *gp = sceGsGetGParam();

    if (gp->version == 1) {
        *(volatile u64 *)0x12000000 = disp->pmode;
        *(volatile u64 *)0x12000070 = disp->dispfb;
        *(volatile u64 *)0x12000080 = disp->display;
        *(volatile u64 *)0x120000C0 = disp->bgcolor;
    } else {
        *(volatile u64 *)0x12000000 = disp->pmode;
        *(volatile u64 *)0x12000020 = disp->smode2;
        *(volatile u64 *)0x12000090 = disp->dispfb;
        *(volatile u64 *)0x120000A0 = disp->display;
        *(volatile u64 *)0x120000E0 = disp->bgcolor;
    }
}

INCLUDE_ASM(const s32, "sdk/libgraph", sceGsSyncPath);

s32 sceGsSetDefLoadImage(sceGsLoadImage *p, s16 dbp, s16 dbw, s16 dpsm, s16 dsax, s16 dsay,
                         s16 rrw, s16 rrh)
{
    s32 qwc = 0;

    switch (dpsm) {
    case 0x00:
    case 0x30:
        qwc = (rrw * rrh) >> 2;
        break;
    case 0x01:
    case 0x31:
        qwc = (rrw * rrh * 3) >> 4;
        break;
    case 0x02:
    case 0x0A:
    case 0x32:
    case 0x3A:
        qwc = (rrw * rrh) >> 3;
        break;
    case 0x13:
    case 0x1B:
        qwc = (rrw * rrh) >> 4;
        break;
    case 0x14:
    case 0x24:
    case 0x2C:
        qwc = (rrw * rrh) >> 5;
        break;
    }
    if (qwc > 0x7FFF) {
        scePrintf("sceGsSetDefLoadImage: too big size\r\n");
        return 0;
    }
    p->tag.qword = 0;
    p->tag.f.NLOOP = 4;
    p->tag.f.NREG = 1;
    p->tag.f.REGS0 = 0xE;
    p->bitbltbuf = ((u64)dbp << 32) | ((u64)dbw << 48) | ((u64)dpsm << 56);
    p->reg_bitbltbuf = 0x50;
    p->trxpos = ((u64)dsax << 32) | ((u64)dsay << 48);
    p->reg_trxpos = 0x51;
    p->trxreg = (u64)rrw | ((u64)rrh << 32);
    p->reg_trxreg = 0x52;
    p->trxdir = 0;
    p->reg_trxdir = 0x53;
    p->imagetag.qword = 0;
    p->imagetag.f.NLOOP = qwc;
    p->imagetag.f.EOP = 1;
    p->imagetag.f.FLG = 2;
    EE_SYNC();
    return 6;
}

s32 sceGsSetDefStoreImage(sceGsStoreImage *p, s16 sbp, s16 sbw, s16 spsm, s16 ssax, s16 ssay,
                          s16 rrw, s16 rrh)
{
    p->qwc = 0;
    p->addr = 0x06008000;
    p->vifcode0 = 0x13000000;
    p->vifcode1 = 0x50000006;
    p->tag.qword = 0;
    p->tag.f.NLOOP = 5;
    p->tag.f.EOP = 1;
    p->tag.f.NREG = 1;
    p->tag.f.REGS0 = 0xE;
    p->bitbltbuf = (u64)sbp | ((u64)sbw << 16) | ((u64)spsm << 24);
    p->reg_bitbltbuf = 0x50;
    p->trxpos = (u64)ssax | ((u64)ssay << 16);
    p->reg_trxpos = 0x51;
    p->trxreg = (u64)rrw | ((u64)rrh << 32);
    p->reg_trxreg = 0x52;
    p->finish = 0;
    p->reg_finish = 0x61;
    p->trxdir = 1;
    p->reg_trxdir = 0x53;
    EE_SYNC();
    return 7;
}

extern const char D_0054A6E0[]; /* DMA channel timeout diagnostic, with native padding. */

s32 sceGsExecLoadImage(sceGsLoadImage *pkt, void *addr)
{
    u32 i = 0;

    while (D2_CHCR & 0x100) {
        if (i++ > 0x1000000) {
            scePrintf(D_0054A6E0);
            return -1;
        }
    }
    D2_QWC = 6;
    if (((u32)pkt & 0x70000000) == 0x70000000) {
        D2_MADR = ((u32)pkt & 0xFFFFFFF) | 0x80000000;
    } else {
        D2_MADR = (u32)pkt & 0xFFFFFFF;
    }
    D2_CHCR = 0x101;
    while (D2_CHCR & 0x100) {
        if (i++ > 0x1000000) {
            scePrintf(D_0054A6E0);
            return -1;
        }
    }
    D2_QWC = pkt->imagetag.f.NLOOP;
    if (((u32)addr & 0x70000000) == 0x70000000) {
        D2_MADR = ((u32)addr & 0xFFFFFFF) | 0x80000000;
    } else {
        D2_MADR = (u32)addr & 0xFFFFFFF;
    }
    D2_CHCR = 0x101;
    return 0;
}

INCLUDE_RODATA(const s32, "sdk/libgraph", D_0054A6E0);

INCLUDE_ASM(const s32, "sdk/libgraph", sceGsExecStoreImage);

u64 sceGsPutIMR(u64 imr)
{
    u64 old = GsGetIMR();

    GsPutIMR(imr);
    return old;
}

u64 sceGsGetIMR(void)
{
    return GsGetIMR();
}
