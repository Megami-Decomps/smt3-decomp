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

extern GParam sceGsGParam;
extern void scePrintf(const char *fmt, ...);

#include "sdk_libkernl.h"
extern s32 sceRead(s32 fd, void *buf, s32 size);
extern s32 sceClose(s32 a);
extern s32 atoi(const char *s);

extern s32 DisableIntc(s32 cause);
extern s32 RemoveIntcHandler(s32 cause, s32 handlerId);
/* Syscall 2 has the SetGsCrt void interface. */
extern void SetGsCrt(s16 interlace, s16 mode, s16 fieldMode);
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
        SetGsCrt(interlace & 1, outputMode & 0xFF, fieldMode & 1);
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
        SetGsCrt(interlace & 1, outputMode & 0xFF, fieldMode & 1);
        break;
    }
}

GParam *sceGsGetGParam(void)
{
    return &sceGsGParam;
}

/* EE register macros: pointer-valued volatile casts of the address constant. */
#define VIF1_FBRST ((volatile u32 *)0x10003C10)
#define VIF1_ERR   ((volatile u32 *)0x10003C20)
#define VIF1_FIFO  ((volatile u128 *)0x10005000)
#define GIF_CTRL   ((volatile u32 *)0x10003000)

#define putVIF1_FBRST(x) (*VIF1_FBRST = (x))
#define putVIF1_ERR(x)   (*VIF1_ERR = (x))
#define putVIF1_FIFO(x)  (*VIF1_FIFO = (x))
#define putGIF_CTRL(x)   (*GIF_CTRL = (x))

extern const u32 sceGsResetPathPackets[8];

#ifdef NON_MATCHING
/* The truthful output-only FBRST scratch omits retail's dead zero setup. */
void sceGsResetPath(void)
{
    u32 vu1;

    putVIF1_FBRST(1);
    putVIF1_ERR(2);
    EE_SYNC_L();
    EE_FBRST_SET(vu1, 0x200);
    putVIF1_FIFO(*(u128 *)&sceGsResetPathPackets[0]);
    putVIF1_FIFO(*(u128 *)&sceGsResetPathPackets[4]);
    putGIF_CTRL(1);
}
#else
INCLUDE_ASM(const s32, "sdk/libgraph", sceGsResetPath);
#endif

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

/* NON_MATCHING: 892 bytes against retail's 888. The first 64 words match and
 * the divergence starts inside the NTSC display arithmetic, where one extra
 * instruction shifts the rest of the function. What matches: the parameter-block call, the mode range test, the model-version
 * path, the _GetGsDxDyOffset call with its four frame addresses, the shared
 * zeroing block with the reloaded mode halfword duplicated into both
 * predecessors, pmode, the three cross-jumped smode2 stores and dispfb.
 * Two differences remain in the display term. Retail adds the horizontal
 * offset constant to the frame word 64-bit (daddiu) and only then the
 * dx-by-magnification product, where the plain C here keeps that sum 32-bit;
 * writing the term with a 64-bit operand does produce daddiu but folds the
 * constant into the product instead and costs seven more words elsewhere. And
 * retail decrements the magnification in the mflo register after both
 * multiplies, where this shape needs one extra register copy first. */
#ifdef NON_MATCHING
extern void _GetGsDxDyOffset(s32 mode, s32 *dx, s32 *dy, s32 *w, s32 *h);
extern void scePrintf(const char *fmt, ...);
extern const char sceGsDefDispEnvModeMsg[];

void sceGsSetDefDispEnv(sceGsDispEnv *disp, s16 psm, s16 w, s16 h, s16 dx, s16 dy)
{
    s32 offsetX;
    s32 offsetY;
    s32 sizeX;
    s32 sizeY;
    GParam *param;
    s32 magh;

    param = sceGsGetGParam();
    if (param->mode != 2 && param->mode != 3 && checkModelVersion() != 0) {
        _GetGsDxDyOffset((s16)param->mode, &offsetX, &offsetY, &sizeX, &sizeY);
    } else {
        sizeY = 0;
        sizeX = 0;
        offsetY = 0;
        offsetX = 0;
    }
    disp->pmode = 0x66;
    if (param->interlace != 0) {
        if (param->fieldMode != 0) {
            disp->smode2 = 3;
        } else {
            disp->smode2 = 1;
        }
    } else {
        disp->smode2 = 2;
    }
    disp->dispfb = ((u64)(psm & 0xF) << 15) | ((u64)(((w + 63) >> 6) & 0x3F) << 9);
    if ((s16)param->mode == 2) {
        if (param->interlace == 1) {
            magh = (2560 + w - 1) / w;
            disp->display = ((u64)(magh - 1) << 23) |
                ((u64)((param->fieldMode != 0 ? h * 2 : h) - 1) << 44) |
                ((u64)((636 + dx * magh + offsetX) & 0xFFF)) |
                ((u64)((dy + offsetY + 50) & 0xFFF) << 12) |
                ((u64)((magh * w) - 1) << 32);
        } else {
            magh = (2560 + w - 1) / w;
            disp->display = ((u64)(magh - 1) << 23) |
                ((u64)(h - 1) << 44) |
                ((u64)((636 + dx * magh + offsetX) & 0xFFF)) |
                ((u64)((dy + offsetY + 25) & 0xFFF) << 12) |
                ((u64)((magh * w) - 1) << 32);
        }
    } else if ((s16)param->mode == 3) {
        if (param->interlace == 1) {
            magh = (2560 + w - 1) / w;
            disp->display = ((u64)(magh - 1) << 23) |
                ((u64)((param->fieldMode != 0 ? h * 2 : h) - 1) << 44) |
                ((u64)((656 + dx * magh + offsetX) & 0xFFF)) |
                ((u64)((dy + offsetY + 72) & 0xFFF) << 12) |
                ((u64)((magh * w) - 1) << 32);
        } else {
            magh = (2560 + w - 1) / w;
            disp->display = ((u64)(magh - 1) << 23) |
                ((u64)(h - 1) << 44) |
                ((u64)((656 + dx * magh + offsetX) & 0xFFF)) |
                ((u64)((dy + offsetY + 36) & 0xFFF) << 12) |
                ((u64)((magh * w) - 1) << 32);
        }
    } else if ((s16)param->mode == 80) {
        disp->display = ((u64)(h - 1) << 44) |
            ((u64)((w * 2) - 1) << 32) |
            0x800000 |
            ((u64)((dy + offsetY + 35) & 0xFFF) << 12) |
            ((u64)((232 + dx * 2 + ((720 - w) / 2) * 2 + offsetX) & 0xFFF));
    } else {
        scePrintf(sceGsDefDispEnvModeMsg, (s16)param->mode);
    }
    disp->bgcolor = 0;
}
#else
INCLUDE_ASM(const s32, "sdk/libgraph", sceGsSetDefDispEnv);
#endif

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


#define D1_CHCR   (*(volatile u32 *)0x10009000)
#define VIF1_STAT (*(volatile u32 *)0x10003C00)
#define GIF_STAT  (*(volatile u32 *)0x10003020)


/* Native diagnostic strings are retained through DUMP_REGS macro ownership. */
/* The five timeout messages and the ten register-dump formats. */
extern const char sceGsSyncPathDma1Msg[];
extern const char sceGsDumpD1ChcrFmt[];
extern const char sceGsDumpD1TadrFmt[];
extern const char sceGsDumpD1MadrFmt[];
extern const char sceGsDumpD1QwcFmt[];
extern const char sceGsDumpD2ChcrFmt[];
extern const char sceGsDumpD2TadrFmt[];
extern const char sceGsDumpD2MadrFmt[];
extern const char sceGsDumpD2QwcFmt[];
extern const char sceGsDumpVif1StatFmt[];
extern const char sceGsDumpGifStatFmt[];
extern const char sceGsSyncPathDma2Msg[];
extern const char sceGsSyncPathVif1Msg[];
extern const char sceGsSyncPathVu1Msg[];
extern const char sceGsSyncPathGifMsg[];

#define DUMP(fmt, addr)                      \
    scePrintf(fmt, *(volatile u32 *)(addr))

#define DUMP_REGS()                          \
    DUMP(sceGsDumpD1ChcrFmt, 0x10009000);            \
    DUMP(sceGsDumpD1TadrFmt, 0x10009030);            \
    DUMP(sceGsDumpD1MadrFmt, 0x10009010);            \
    DUMP(sceGsDumpD1QwcFmt, 0x10009020);            \
    DUMP(sceGsDumpD2ChcrFmt, 0x1000A000);            \
    DUMP(sceGsDumpD2TadrFmt, 0x1000A030);            \
    DUMP(sceGsDumpD2MadrFmt, 0x1000A010);            \
    DUMP(sceGsDumpD2QwcFmt, 0x1000A020);            \
    DUMP(sceGsDumpVif1StatFmt, 0x10003C00);            \
    DUMP(sceGsDumpGifStatFmt, 0x10003020)

s32 sceGsSyncPath(s32 mode, u16 timeout)
{
    u32 stat;
    u32 i = 0;

    if (mode == 0) {
        while (D1_CHCR & 0x100) {
            if (i++ > 0x1000000) {
                scePrintf(sceGsSyncPathDma1Msg);
                DUMP_REGS();
                return -1;
            }
        }
        while (D2_CHCR & 0x100) {
            if (i++ > 0x1000000) {
                scePrintf(sceGsSyncPathDma2Msg);
                DUMP_REGS();
                return -1;
            }
        }
        while (VIF1_STAT & 0x1F000003) {
            if (i++ > 0x1000000) {
                scePrintf(sceGsSyncPathVif1Msg);
                DUMP_REGS();
                return -1;
            }
        }
        EE_GET_VPU_STAT(stat);
        while (stat & 0x100) {
            if (i++ > 0x1000000) {
                scePrintf(sceGsSyncPathVu1Msg);
                DUMP_REGS();
                return -1;
            }
            EE_GET_VPU_STAT(stat);
        }
        while (GIF_STAT & 0xC00) {
            if (i++ > 0x1000000) {
                scePrintf(sceGsSyncPathGifMsg);
                DUMP_REGS();
                return -1;
            }
        }
        return 0;
    } else {
        s32 result = 0;


        if (D1_CHCR & 0x100) {
            result |= 1;
        }
        if (D2_CHCR & 0x100) {
            result |= 2;
        }
        if (VIF1_STAT & 0x1F000003) {
            result |= 4;
        }
        EE_GET_VPU_STAT(stat);
        if (stat & 0x100) {
            result |= 8;
        }
        if (GIF_STAT & 0xC00) {
            result |= 16;
        }
        return result;
    }
}

INCLUDE_RODATA(const s32, "sdk/libgraph", sceGsSyncPathDma1Msg);

INCLUDE_RODATA(const s32, "sdk/libgraph", sceGsDumpD1ChcrFmt);

INCLUDE_RODATA(const s32, "sdk/libgraph", sceGsDumpD1TadrFmt);

INCLUDE_RODATA(const s32, "sdk/libgraph", sceGsDumpD1MadrFmt);

INCLUDE_RODATA(const s32, "sdk/libgraph", sceGsDumpD1QwcFmt);

INCLUDE_RODATA(const s32, "sdk/libgraph", sceGsDumpD2ChcrFmt);

INCLUDE_RODATA(const s32, "sdk/libgraph", sceGsDumpD2TadrFmt);

INCLUDE_RODATA(const s32, "sdk/libgraph", sceGsDumpD2MadrFmt);

INCLUDE_RODATA(const s32, "sdk/libgraph", sceGsDumpD2QwcFmt);

INCLUDE_RODATA(const s32, "sdk/libgraph", sceGsDumpVif1StatFmt);

INCLUDE_RODATA(const s32, "sdk/libgraph", sceGsDumpGifStatFmt);

INCLUDE_RODATA(const s32, "sdk/libgraph", sceGsSyncPathDma2Msg);

INCLUDE_RODATA(const s32, "sdk/libgraph", sceGsSyncPathVif1Msg);

INCLUDE_RODATA(const s32, "sdk/libgraph", sceGsSyncPathVu1Msg);

INCLUDE_RODATA(const s32, "sdk/libgraph", sceGsSyncPathGifMsg);

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

extern const char sceGsExecLoadImageDma2Msg[]; /* DMA channel timeout diagnostic, with native padding. */

s32 sceGsExecLoadImage(sceGsLoadImage *pkt, void *addr)
{
    u32 i = 0;

    while (D2_CHCR & 0x100) {
        if (i++ > 0x1000000) {
            scePrintf(sceGsExecLoadImageDma2Msg);
            return -1;
        }
    }
    D2_QWC = 6;
    if ((EE_ADDR(pkt) & 0x70000000) == 0x70000000) {
        D2_MADR = (EE_ADDR(pkt) & 0xFFFFFFF) | 0x80000000;
    } else {
        D2_MADR = EE_ADDR(pkt) & 0xFFFFFFF;
    }
    D2_CHCR = 0x101;
    while (D2_CHCR & 0x100) {
        if (i++ > 0x1000000) {
            scePrintf(sceGsExecLoadImageDma2Msg);
            return -1;
        }
    }
    D2_QWC = pkt->imagetag.f.NLOOP;
    if ((EE_ADDR(addr) & 0x70000000) == 0x70000000) {
        D2_MADR = (EE_ADDR(addr) & 0xFFFFFFF) | 0x80000000;
    } else {
        D2_MADR = EE_ADDR(addr) & 0xFFFFFFF;
    }
    D2_CHCR = 0x101;
    return 0;
}

/* Reference implementation; see docs/matching/sdk/libgraph.md. */
/* NON_MATCHING: 1672 bytes against retail's 1680, first difference at +0x3C.
 * The prologue's seven saved registers and both parameter copies, the single
 * load of each 64-bit record word and the width/height/mode extraction, the
 * jump table and its five case bodies, the uncached trxreg patch, every spin
 * loop and both FIFO loops now agree instruction for instruction; what is left
 * is scheduling inside the case bodies and the cleanup block, plus three
 * delay slots this compiler leaves empty because the access is volatile (see
 * the near-miss rules in docs/sdk-matching.md).
 * Four things moved it: the DMA count mask is the unsigned 0xFFFFFFF8, not
 * ~7, so it is materialized with lui/ori and shared with the align-up masks;
 * the cleanup is written out at each of the four timeout sites, which lets
 * cross-jumping merge the scePrintf call itself down to retail's three call
 * sites; the trxreg patch re-extracts the width from the record instead of
 * reusing the local, which local CSE cannot share across the block; and each
 * case body computes the byte remainder, then the quadword remainder, then the
 * DMA count, which halved the instruction-order difference. */
#ifdef NON_MATCHING
extern const char sceGsExecStoreImageDma1Msg[];
extern const char sceGsExecStoreImageGsMsg[];
extern const char sceGsExecStoreImageDma1ToMemMsg[];
extern const char sceGsExecStoreImageVif1Msg[];
extern const u128 sceGsStoreImageFlushPacket;

s32 sceGsExecStoreImage(sceGsStoreImage *pkt, void *addr)
{
    s32 width;
    s32 height;
    s32 pixelFormat;
    s32 bytes;
    s32 qwc;
    s32 dmaQwc = 0;
    s32 qwcTail = 0;
    s32 byteTail = 0;
    s32 extraQwc = 0;
    s32 roundedHeight;
    u32 timeout = 0;
    s32 i;
    u32 value;
    addr_t dmaAddress;
    u64 csr;
    u64 restoreImr;
    u128 fifoWord;
    u8 *fifoBytes;

    width = (s32)(pkt->trxreg & 0xFFF);
    height = (s32)((pkt->trxreg >> 32) & 0xFFF);
    pixelFormat = (s32)((pkt->bitbltbuf >> 24) & 0x3F);

    switch (pixelFormat) {
    case 0:
    case 48:
        bytes = width * height * 4;
        qwc = bytes >> 4;
        byteTail = bytes & 0xF;
        qwcTail = qwc & 7;
        dmaQwc = qwc & 0xFFFFFFF8;
        if (byteTail == 0) {
            extraQwc = 0;
            roundedHeight = height;
        } else {
            roundedHeight = (height + 3) & 0x1FFC;
            extraQwc = ((width * roundedHeight) >> 2) - dmaQwc - qwcTail - 1;
        }
        break;
    case 1:
    case 49:
        bytes = width * height * 3;
        qwc = bytes >> 4;
        byteTail = bytes & 0xF;
        qwcTail = qwc & 7;
        dmaQwc = qwc & 0xFFFFFFF8;
        if (byteTail == 0) {
            extraQwc = 0;
            roundedHeight = height;
        } else {
            roundedHeight = (height + 15) & 0x1FF0;
            extraQwc = ((width * roundedHeight * 3) >> 4) - dmaQwc - qwcTail - 1;
        }
        break;
    case 2:
    case 10:
    case 50:
    case 58:
        bytes = width * height * 2;
        qwc = bytes >> 4;
        byteTail = bytes & 0xF;
        qwcTail = qwc & 7;
        dmaQwc = qwc & 0xFFFFFFF8;
        if (byteTail == 0) {
            extraQwc = 0;
            roundedHeight = height;
        } else {
            roundedHeight = (height + 7) & 0xFFFFFFF8;
            extraQwc = ((width * roundedHeight) >> 3) - dmaQwc - qwcTail - 1;
        }
        break;
    case 19:
    case 27:
        bytes = width * height;
        qwc = bytes >> 4;
        byteTail = bytes & 0xF;
        qwcTail = qwc & 7;
        dmaQwc = qwc & 0xFFFFFFF8;
        if (byteTail == 0) {
            extraQwc = 0;
            roundedHeight = height;
        } else {
            roundedHeight = (height + 7) & 0xFFFFFFF8;
            extraQwc = ((width * roundedHeight) >> 4) - dmaQwc - qwcTail - 1;
        }
        break;
    case 20:
    case 36:
    case 44:
        bytes = (width * height) >> 1;
        qwc = bytes >> 4;
        byteTail = bytes & 0xF;
        qwcTail = qwc & 7;
        dmaQwc = qwc & 0xFFFFFFF8;
        if (byteTail == 0) {
            extraQwc = 0;
            roundedHeight = height;
        } else {
            roundedHeight = (height + 7) & 0xFFFFFFF8;
            extraQwc = ((width * roundedHeight) >> 5) - dmaQwc - qwcTail - 1;
        }
        break;
    }
    if (byteTail != 0) {
        *EE_ADDR_AS(volatile u64 *, ((EE_ADDR(&pkt->trxreg)) | 0x20000000)) =
            (u64)(u32)(s32)(pkt->trxreg & 0xFFF) |
            ((u64)(u32)roundedHeight << 32);
    }

    value = *(volatile u32 *)0x10009000;
    if ((value & 0x100) != 0) {
        do {
            if ((u32)timeout++ > 0x1000000U) {
                scePrintf(sceGsExecStoreImageDma1Msg);
                return -1;
            }
            value = *(volatile u32 *)0x10009000;
        } while ((value & 0x100) != 0);
    }

    restoreImr = GsPutIMR(GsGetIMR() | 0x200);
    *(volatile u64 *)0x12001000 = 2;
    *(volatile u32 *)0x10009020 = 7;
    dmaAddress = EE_ADDR(pkt);
    if ((dmaAddress & 0x70000000) == 0x70000000) {
        dmaAddress = (dmaAddress & 0x0FFFFFFF) | 0x80000000;
    } else {
        dmaAddress &= 0x0FFFFFFF;
    }
    *(volatile u32 *)0x10009010 = dmaAddress;
    *(volatile u32 *)0x10009000 = 0x101;

    value = *(volatile u32 *)0x10009000;
    if ((value & 0x100) != 0) {
        do {
            if ((u32)timeout++ > 0x1000000U) {
                scePrintf(sceGsExecStoreImageDma1Msg);
                return -1;
            }
            value = *(volatile u32 *)0x10009000;
        } while ((value & 0x100) != 0);
    }

    csr = *(volatile u64 *)0x12001000;
    if ((csr & 2) == 0) {
        do {
            if ((u32)timeout++ > 0x1000000U) {
                scePrintf(sceGsExecStoreImageGsMsg);
                *(volatile u128 *)0x10005000 = sceGsStoreImageFlushPacket;
                return -1;
            }
            csr = *(volatile u64 *)0x12001000;
        } while ((csr & 2) == 0);
    }

    *(volatile u32 *)0x10003C00 = 0x00800000;
    *(volatile u64 *)0x12001040 = 1;

    if (dmaQwc != 0) {
        *(volatile u32 *)0x10009020 = dmaQwc;
        dmaAddress = EE_ADDR(addr);
        if ((dmaAddress & 0x70000000) == 0x70000000) {
            dmaAddress = (dmaAddress & 0x0FFFFFFF) | 0x80000000;
        } else {
            dmaAddress &= 0x0FFFFFFF;
        }
        *(volatile u32 *)0x10009010 = dmaAddress;
        *(volatile u32 *)0x10009000 = 0x100;
        value = *(volatile u32 *)0x10009000;
        if ((value & 0x100) != 0) {
            do {
                if ((u32)timeout++ > 0x1000000U) {
                    scePrintf(sceGsExecStoreImageDma1ToMemMsg);
*(volatile u64 *)0x12001000 = 0x100;
                    *(volatile u64 *)0x12001040 = 0;
                    *(volatile u32 *)0x10003000 = 1;
                    *(volatile u32 *)0x10003C10 = 1;
                    return -1;
                }
                value = *(volatile u32 *)0x10009000;
            } while ((value & 0x100) != 0);
        }
    }

    if (qwcTail != 0) {
        for (i = 0; i < qwcTail; i++) {
            value = *(volatile u32 *)0x10003C00;
            if ((value & 0x1F000000) == 0) {
                do {
                    if ((u32)timeout++ > 0x1000000U) {
                        scePrintf(sceGsExecStoreImageVif1Msg);
*(volatile u64 *)0x12001000 = 0x100;
                        *(volatile u64 *)0x12001040 = 0;
                        *(volatile u32 *)0x10003000 = 1;
                        *(volatile u32 *)0x10003C10 = 1;
                        return -1;
                    }
                    value = *(volatile u32 *)0x10003C00;
                } while ((value & 0x1F000000) == 0);
            }
            ((u128 *)addr)[dmaQwc + i] = *(volatile u128 *)0x10005000;
        }
    }

    if (byteTail != 0) {
        value = *(volatile u32 *)0x10003C00;
        if ((value & 0x1F000000) == 0) {
            do {
                if ((u32)timeout++ > 0x1000000U) {
                    scePrintf(sceGsExecStoreImageVif1Msg);
*(volatile u64 *)0x12001000 = 0x100;
                    *(volatile u64 *)0x12001040 = 0;
                    *(volatile u32 *)0x10003000 = 1;
                    *(volatile u32 *)0x10003C10 = 1;
                    return -1;
                }
                value = *(volatile u32 *)0x10003C00;
            } while ((value & 0x1F000000) == 0);
        }
        fifoWord = *(volatile u128 *)0x10005000;
        fifoBytes = (u8 *)&fifoWord;
        for (i = 0; i < byteTail; i++) {
            ((u8 *)&((u128 *)addr)[dmaQwc + qwcTail])[i] = fifoBytes[i];
        }

        {
            for (i = 0; i < extraQwc; i++) {
                value = *(volatile u32 *)0x10003C00;
                if ((value & 0x1F000000) == 0) {
                    do {
                        if ((u32)timeout++ > 0x1000000U) {
                            scePrintf(sceGsExecStoreImageVif1Msg);
*(volatile u64 *)0x12001000 = 0x100;
                            *(volatile u64 *)0x12001040 = 0;
                            *(volatile u32 *)0x10003000 = 1;
                            *(volatile u32 *)0x10003C10 = 1;
                            return -1;
                        }
                        value = *(volatile u32 *)0x10003C00;
                    } while ((value & 0x1F000000) == 0);
                }
                fifoWord = *(volatile u128 *)0x10005000;
            }
        }
    }

    *(volatile u32 *)0x10003C00 = 0;
    *(volatile u64 *)0x12001040 = 0;
    GsPutIMR(restoreImr);
    *(volatile u64 *)0x12001000 = 2;
    *(volatile u128 *)0x10005000 = sceGsStoreImageFlushPacket;
    return 0;

}
#else
INCLUDE_RODATA(const s32, "sdk/libgraph", sceGsExecLoadImageDma2Msg);

INCLUDE_ASM(const s32, "sdk/libgraph", sceGsExecStoreImage);
#endif

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
