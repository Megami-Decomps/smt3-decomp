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
    return &D_00529F20;
}

/* The VIF1 register-initialization packet pushed through the FIFO. */
extern const u128 D_00529F30[2];

/* Resets VU1 through COP2 control register 28; the operand is an input even
 * though the block writes it, so the zero has to be materialized first. */
#define EE_VU1_RESET(t) __asm__ volatile ( \
    ".set noreorder\n\tcfc2.ni %0, $vi28\n\tori %0, %0, 0x200\n\t" \
    "ctc2.ni %0, $vi28\n\tsync.p\n\t.set reorder" : : "r" (t))

/* NON_MATCHING: 108 bytes against retail's 104. Everything through the VU1
 * reset matches. Retail ends with the GIF_CTRL write in the jr delay slot,
 * which this compiler will not do for a volatile access, and writing it
 * through a plain pointer instead turns the address into a two-instruction
 * assembler macro that is equally ineligible. Taking the address from a
 * declaration-initialized pointer local does fill the slot at the right 104
 * bytes, but then the address is materialized 12 words early, before the
 * asm block, where retail materializes it between the first quadword load
 * and its store. */
#ifdef NON_MATCHING
void sceGsResetPath(void)
{
    u32 vu1 = 0;

    *(volatile u32 *)0x10003C10 = 1;
    *(volatile u32 *)0x10003C20 = 2;
    EE_SYNC();
    EE_VU1_RESET(vu1);
    *(volatile u128 *)0x10005000 = D_00529F30[0];
    *(volatile u128 *)0x10005000 = D_00529F30[1];
    *(volatile u32 *)0x10003000 = 1;
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
extern const char D_0054A3F0[];

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
        scePrintf(D_0054A3F0, (s16)param->mode);
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

/* VPU_STAT, COP2 control register 29. */
#define EE_GET_VPU_STAT(out) __asm__ volatile ( \
    ".set noreorder\n\tcfc2.ni %0, $vi29\n\t.set reorder" : "=r" (out))

/* NON_MATCHING: 828 bytes against retail's 792, nine words of it the unfilled
 * delay slots of the ten diagnostic calls. Retail reads each register in its
 * scePrintf delay slot off an address held in a register (lui plus ori, zero
 * displacement), which is the shape this compiler emits only for a volatile
 * access, and it never moves a volatile access into a delay slot. A plain read
 * is eligible but then the address collapses into a single two-instruction
 * macro load, which is ineligible for the slot in turn, and ee-as does not
 * swap instructions into delay slots at all. Everything else matches: the five
 * spin loops and their rotated shape, the shared eleven-call tail that
 * cross-jumping merges out of the five written-out copies, the counter test,
 * and the five conditional moves of the non-blocking section. The two
 * remaining words are branch displacements that the extra nops shift. */
#ifdef NON_MATCHING
/* The five timeout messages and the ten register-dump formats. */
extern const char D_0054A428[];
extern const char D_0054A458[];
extern const char D_0054A468[];
extern const char D_0054A478[];
extern const char D_0054A488[];
extern const char D_0054A498[];
extern const char D_0054A4A8[];
extern const char D_0054A4B8[];
extern const char D_0054A4C8[];
extern const char D_0054A4D8[];
extern const char D_0054A4F0[];
extern const char D_0054A508[];
extern const char D_0054A538[];
extern const char D_0054A568[];
extern const char D_0054A590[];

#define DUMP(fmt, addr)                      \
    scePrintf(fmt, *(volatile u32 *)(addr))

#define DUMP_REGS()                          \
    DUMP(D_0054A458, 0x10009000);            \
    DUMP(D_0054A468, 0x10009030);            \
    DUMP(D_0054A478, 0x10009010);            \
    DUMP(D_0054A488, 0x10009020);            \
    DUMP(D_0054A498, 0x1000A000);            \
    DUMP(D_0054A4A8, 0x1000A030);            \
    DUMP(D_0054A4B8, 0x1000A010);            \
    DUMP(D_0054A4C8, 0x1000A020);            \
    DUMP(D_0054A4D8, 0x10003C00);            \
    DUMP(D_0054A4F0, 0x10003020)

s32 sceGsSyncPath(s32 mode, u16 timeout)
{
    u32 stat;
    u32 i = 0;

    if (mode == 0) {
        while (D1_CHCR & 0x100) {
            if (i++ > 0x1000000) {
                scePrintf(D_0054A428);
                DUMP_REGS();
                return -1;
            }
        }
        while (D2_CHCR & 0x100) {
            if (i++ > 0x1000000) {
                scePrintf(D_0054A508);
                DUMP_REGS();
                return -1;
            }
        }
        while (VIF1_STAT & 0x1F000003) {
            if (i++ > 0x1000000) {
                scePrintf(D_0054A538);
                DUMP_REGS();
                return -1;
            }
        }
        EE_GET_VPU_STAT(stat);
        while (stat & 0x100) {
            if (i++ > 0x1000000) {
                scePrintf(D_0054A568);
                DUMP_REGS();
                return -1;
            }
            EE_GET_VPU_STAT(stat);
        }
        while (GIF_STAT & 0xC00) {
            if (i++ > 0x1000000) {
                scePrintf(D_0054A590);
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
#else
INCLUDE_ASM(const s32, "sdk/libgraph", sceGsSyncPath);
#endif

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

/* Reference implementation; see docs/matching/sdk/libgraph.md. */
#ifdef NON_MATCHING
extern const char D_0054A720[];
extern const char D_0054A758[];
extern const char D_0054A788[];
extern const char D_0054A7C8[];
extern const u128 D_00529F50;

s32 sceGsExecStoreImage(sceGsStoreImage *pkt, void *addr)
{
    s32 width;
    s32 height;
    s32 pixelFormat;
    s32 bytes;
    s32 qwc;
    s32 dmaQwc;
    s32 qwcTail;
    s32 byteTail;
    s32 extraQwc;
    s32 roundedHeight;
    s32 timeout;
    s32 i;
    u32 value;
    u32 dmaAddress;
    u128 *destination;
    u64 csr;
    u64 restoreImr;
    u128 fifoWord;
    u8 *fifoBytes;

    extraQwc = 0;
    byteTail = 0;
    dmaQwc = 0;
    qwcTail = 0;
    timeout = 0;
    width = (s32)(pkt->trxreg & 0xFFF);
    height = (s32)((pkt->trxreg >> 32) & 0xFFF);
    pixelFormat = (s32)((pkt->bitbltbuf >> 24) & 0x3F);
    roundedHeight = 0;

    switch (pixelFormat) {
    case 0:
    case 48:
        bytes = width * height * 4;
        qwc = bytes >> 4;
        dmaQwc = qwc & ~7;
        qwcTail = qwc & 7;
        byteTail = bytes & 0xF;
        if (byteTail != 0) {
            roundedHeight = (height + 3) & 0x1FFC;
            extraQwc = ((width * roundedHeight) >> 2) - dmaQwc - qwcTail - 1;
        } else {
            roundedHeight = height;
        }
        break;
    case 1:
    case 49:
        bytes = width * height * 3;
        qwc = bytes >> 4;
        dmaQwc = qwc & ~7;
        qwcTail = qwc & 7;
        byteTail = bytes & 0xF;
        if (byteTail != 0) {
            roundedHeight = (height + 15) & 0x1FF0;
            extraQwc = ((width * roundedHeight * 3) >> 4) - dmaQwc - qwcTail - 1;
        } else {
            roundedHeight = height;
        }
        break;
    case 2:
    case 10:
    case 50:
    case 58:
        bytes = width * height * 2;
        qwc = bytes >> 4;
        dmaQwc = qwc & ~7;
        qwcTail = qwc & 7;
        byteTail = bytes & 0xF;
        if (byteTail != 0) {
            roundedHeight = (height + 7) & ~7;
            extraQwc = ((width * roundedHeight) >> 3) - dmaQwc - qwcTail - 1;
        } else {
            roundedHeight = height;
        }
        break;
    case 19:
    case 27:
        bytes = width * height;
        qwc = bytes >> 4;
        dmaQwc = qwc & ~7;
        qwcTail = qwc & 7;
        byteTail = bytes & 0xF;
        if (byteTail != 0) {
            roundedHeight = (height + 7) & ~7;
            extraQwc = ((width * roundedHeight) >> 4) - dmaQwc - qwcTail - 1;
        } else {
            roundedHeight = height;
        }
        break;
    case 20:
    case 36:
    case 44:
        bytes = (width * height) >> 1;
        qwc = bytes >> 4;
        dmaQwc = qwc & ~7;
        qwcTail = qwc & 7;
        byteTail = bytes & 0xF;
        if (byteTail != 0) {
            roundedHeight = (height + 7) & ~7;
            extraQwc = ((width * roundedHeight) >> 5) - dmaQwc - qwcTail - 1;
        } else {
            roundedHeight = height;
        }
        break;
    }
    if (byteTail != 0) {
        *(volatile u64 *)(((u32)&pkt->trxreg) | 0x20000000) =
            (u64)(u32)width | ((u64)(u32)roundedHeight << 32);
    }

    value = *(volatile u32 *)0x10009000;
    if ((value & 0x100) != 0) {
        do {
            if ((u32)timeout++ > 0x1000000U) {
                scePrintf(D_0054A720);
                return -1;
            }
            value = *(volatile u32 *)0x10009000;
        } while ((value & 0x100) != 0);
    }

    restoreImr = GsPutIMR(GsGetIMR() | 0x200);
    *(volatile u64 *)0x12001000 = 2;
    *(volatile u32 *)0x10009020 = 7;
    dmaAddress = (u32)pkt;
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
                scePrintf(D_0054A720);
                return -1;
            }
            value = *(volatile u32 *)0x10009000;
        } while ((value & 0x100) != 0);
    }

    csr = *(volatile u64 *)0x12001000;
    if ((csr & 2) == 0) {
        do {
            if ((u32)timeout++ > 0x1000000U) {
                scePrintf(D_0054A758);
                *(volatile u128 *)0x10005000 = D_00529F50;
                return -1;
            }
            csr = *(volatile u64 *)0x12001000;
        } while ((csr & 2) == 0);
    }

    *(volatile u32 *)0x10003C00 = 0x00800000;
    *(volatile u64 *)0x12001040 = 1;

    if (dmaQwc != 0) {
        *(volatile u32 *)0x10009020 = dmaQwc;
        dmaAddress = (u32)addr;
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
                    scePrintf(D_0054A788);
                    goto transfer_error;
                }
                value = *(volatile u32 *)0x10009000;
            } while ((value & 0x100) != 0);
        }
    }

    destination = (u128 *)((u8 *)addr + (dmaQwc << 4));
    if (qwcTail != 0) {
        for (i = 0; i < qwcTail; i++) {
            value = *(volatile u32 *)0x10003C00;
            if ((value & 0x1F000000) == 0) {
                do {
                    if ((u32)timeout++ > 0x1000000U) {
                        scePrintf(D_0054A7C8);
                        goto transfer_error;
                    }
                    value = *(volatile u32 *)0x10003C00;
                } while ((value & 0x1F000000) == 0);
            }
            fifoWord = *(volatile u128 *)0x10005000;
            *destination++ = fifoWord;
        }
    }

    if (byteTail != 0) {
        value = *(volatile u32 *)0x10003C00;
        if ((value & 0x1F000000) == 0) {
            do {
                if ((u32)timeout++ > 0x1000000U) {
                    scePrintf(D_0054A7C8);
                    goto transfer_error;
                }
                value = *(volatile u32 *)0x10003C00;
            } while ((value & 0x1F000000) == 0);
        }
        fifoWord = *(volatile u128 *)0x10005000;
        fifoBytes = (u8 *)&fifoWord;
        for (i = 0; i < byteTail; i++) {
            ((u8 *)addr)[((dmaQwc + qwcTail) << 4) + i] = fifoBytes[i];
        }

        if (extraQwc > 0) {
            for (i = 0; i < extraQwc; i++) {
                value = *(volatile u32 *)0x10003C00;
                if ((value & 0x1F000000) == 0) {
                    do {
                        if ((u32)timeout++ > 0x1000000U) {
                            scePrintf(D_0054A7C8);
                            goto transfer_error;
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
    *(volatile u128 *)0x10005000 = D_00529F50;
    return 0;

transfer_error:
    *(volatile u64 *)0x12001000 = 0x100;
    *(volatile u64 *)0x12001040 = 0;
    *(volatile u32 *)0x10003000 = 1;
    *(volatile u32 *)0x10003C10 = 1;
    return -1;
}
#else
INCLUDE_RODATA(const s32, "sdk/libgraph", D_0054A6E0);

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
