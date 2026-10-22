#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"

extern void *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2, s32 v3,
                              void *drawExit, void *deleteExit, void *p);

extern void *dds3SearchProcessName(const char *namestr);

typedef struct
{
    /* 0x0000 */ u8 unk_00[0x20];
    /* 0x0020 */ f32 screenOfsX;
    /* 0x0024 */ f32 screenOfsY;
    /* 0x0028 */ u8 unk_28[0x10];
    /* 0x0038 */ s32 texHandle[7];
    /* 0x0054 */ s32 flags;
    /* 0x0058 */ u16 unk_58;
    /* 0x005A */ u16 unk_5A;
    /* 0x005C */ u8 unk_5C[8];
    /* 0x0064 */ s16 listCursor[4];
    /* 0x006C */ u8 unk_6C;
    /* 0x006D */ u8 unk_6D;
    /* 0x006E */ u16 modelId;
    /* 0x0070 */ s32 unk_70;
    /* 0x0074 */ u8 unk_74[0x10];
    /* 0x0084 */ u8 unk_84[0x2DC];
    /* 0x0360 */ void *explainData;
    /* 0x0364 */ s32 unk_364;
    /* 0x0368 */ void *allocation;
    /* 0x036C */ s32 unk_36C;
} fclEncyc_instance_tag;

typedef struct
{
    /* 0x0000 */ u8 flags;
    /* 0x0001 */ u8 unk_0001[0x13];
    /* 0x0014 */ void *allocation;
    /* 0x0018 */ u8 unk_0018[0x44B0];
    /* 0x44C8 */ void *unk_44C8;
    /* 0x44CC */ s32 unk_44CC;
    /* 0x44D0 */ s32 unk_44D0;
} rcvWork_t;

extern rcvWork_t *shutdownfunc;

extern u8 D_00343E40[];

extern void movieStop(void);

/* The viewer's memory window: one word and one 0x40-byte block, each copied
 * from its source address when its pending flag is set. */
typedef struct
{
    /* 0x00 */ u8 started;
    /* 0x01 */ u8 cursor;
    /* 0x02 */ u8 wordPending;
    /* 0x03 */ u8 blockPending;
    /* 0x04 */ u32 regAddr;
    /* 0x08 */ u8 *buffer;
    /* 0x0C */ s32 word;
    /* 0x10 */ u8 block[0x40];
} movieViewerWork_t;

extern movieViewerWork_t movieViewerDebugFlag;

extern s32 dds3KillProcess(s32 id, s32 shutdowntimer);

extern void (*DrawPhaseTable[])(fclEncyc_instance_tag *work);

extern s32 dds3WaitMovieEnd(void);

/* The decoder's shared block, also reached as a byte buffer through
 * D_00343E40; only the two frame counters are read here. */
typedef struct
{
    /* 0x00 */ u8 unk_00[0x64];
    /* 0x64 */ s32 frameCount;
    /* 0x68 */ s32 unk_68;
    /* 0x6C */ s32 frameIndex;
} movieDecodeWork_t;

extern movieDecodeWork_t movieDecodeWork asm("D_00343E40");

/* Unsized so the reference is absolute, as retail's is. */
extern sdfDrawTag_t *movieViewerFrame[];

extern void *dds3PrimFrameRect(s32 aX, s32 aY, s32 aZ, s32 aW, s32 aH, u32 aBgCol, u32 aFrameCol);

extern void *sdfDevPrintf(s32 aX, s32 aY, s32 aZ, s32 aAttr, const char *aStr, ...);

extern void sdfDrawTagLink(sdfDrawTag_t *aTag, void *aPacket);

void MovieViewerDrawFrameInfo(void)
{
    sdfDrawTag_t *tag;

    if (dds3WaitMovieEnd() == 0) {
        tag = movieViewerFrame[0];
        sdfDrawTagLink(tag, dds3PrimFrameRect(0x8810, 0x85E8, 0xFF0080, 0x720, 0x90,
                                              0x30000000, 0x60404040));
        sdfDrawTagLink(tag, sdfDevPrintf(0x8840, 0x8600, 0xFF0080, 0, "%04d/%04d",
                                         movieDecodeWork.frameIndex,
                                         movieDecodeWork.frameCount));
    }
}

INCLUDE_ASM(const s32, "movie/MovieViewer", MovieViewerProc);

extern void movieViewerScanDir(void);

extern s32 MovieViewerProc(void);

extern void *movieViewerWork[];

extern char D_0053F418[];

void InitMovieViewer(void)
{
    movieViewerScanDir();
    movieViewerWork[0] =
        dds3StartProcess(D_0053F418, 0x2B02, 1, 0, MovieViewerProc, NULL, NULL);
}

extern char D_0053F418[];

extern void *movieViewerWork[];

extern void movieViewerFree(void);

void FinishMovieViewer(void)
{
    void *proc = dds3SearchProcessName(D_0053F418);

    if (proc != NULL) {
        dds3KillProcess((s32)proc, 0);
        movieViewerWork[0] = NULL;
        movieStop();
    }
    movieViewerFree();
}

void movieViewerSetFlags(void)
{
    movieViewerDebugFlag.wordPending = 1;
    movieViewerDebugFlag.blockPending = 1;
}

void movieViewerInitRegs(void)
{
    movieViewerDebugFlag.regAddr = 0x10002010;
    movieViewerDebugFlag.buffer = D_00343E40;
    movieViewerSetFlags();
}

extern void *memcpy(void *a, const void *b, u32 c);

/* Refresh the window from its two source addresses. */
void movieViewerDrawInfo(void)
{
    if (movieViewerDebugFlag.wordPending != 0) {
        movieViewerDebugFlag.word = *(s32 *)movieViewerDebugFlag.regAddr;
    }
    if (movieViewerDebugFlag.blockPending != 0) {
        memcpy(movieViewerDebugFlag.block, movieViewerDebugFlag.buffer, 0x40);
    }
}

INCLUDE_RODATA(const s32, "movie/MovieViewer", D_0053F418);

extern s8 dds3PadBitTable[];

extern sdfDrawTag_t *dds3FontReqAllocDrawTag(void);

extern void sdfPrimLinkRect(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0,
                            s32 aX1, s32 aY1, s32 aZ, s32 (*aAllocator)(s32));

extern sdfGrOt_t D_002F6938;

/* "%08X", "%02X" and "**", in .sdata where a literal cannot go. */
extern char D_0054FDB8[];
extern char D_0054FDC0[];
extern char D_0054FDC8[];

/* The memory window: a cursor over the eight nibbles of either source address,
 * the word above and the 8x8 block below. */
s32 movieViewerDebugDraw(void)
{
    sdfDrawTag_t *tag;
    s32 step;
    s32 i;
    s32 x;
    s32 y;
    s32 col;

    if (movieViewerDebugFlag.started == 0) {
        movieViewerDebugFlag.started = 1;
        movieViewerInitRegs();
    }
    if (dds3PadBitTable[0x11] != 0) {
        movieViewerSetFlags();
    } else if (dds3PadBitTable[0x12] != 0) {
        movieViewerInitRegs();
    } else if (dds3PadBitTable[0x15] & 2) {
        movieViewerDebugFlag.cursor++;
        if (movieViewerDebugFlag.cursor == 0x10) {
            movieViewerDebugFlag.cursor = 0;
        }
    } else if (dds3PadBitTable[0x14] & 2) {
        if (movieViewerDebugFlag.cursor != 0) {
            movieViewerDebugFlag.cursor--;
        } else {
            movieViewerDebugFlag.cursor = 0xF;
        }
    } else {
        step = 1 << ((~movieViewerDebugFlag.cursor & 7) * 4);
        i = movieViewerDebugFlag.cursor >> 3;
        if (dds3PadBitTable[0x17] & 2) {
            if (i == 0) {
                movieViewerDebugFlag.wordPending = 0;
                movieViewerDebugFlag.regAddr += step;
            } else {
                movieViewerDebugFlag.blockPending = 0;
                movieViewerDebugFlag.buffer += step;
            }
        }
        if (dds3PadBitTable[0x16] & 2) {
            if (i == 0) {
                movieViewerDebugFlag.wordPending = 0;
                movieViewerDebugFlag.regAddr -= step;
            } else {
                movieViewerDebugFlag.blockPending = 0;
                movieViewerDebugFlag.buffer -= step;
            }
        }
    }
    movieViewerDrawInfo();
    tag = dds3FontReqAllocDrawTag();
    sdfDrawTagLink(tag, dds3PrimFrameRect(0x7150, 0x79A8, 0xFF007E, 0x1860, 0x3F0, 0x60000000,
                                          0x40806020));
    sdfPrimLinkRect(tag, 0x80A03000, 0, (movieViewerDebugFlag.cursor & 7) * 0xC0 + 0x7180,
                    (movieViewerDebugFlag.cursor >> 3) * 0xC0 + 0x79C0,
                    (movieViewerDebugFlag.cursor & 7) * 0xC0 + 0x7240,
                    (movieViewerDebugFlag.cursor >> 3) * 0xC0 + 0x7A20, 0xFF007F, 0);
    sdfDrawTagLink(tag, sdfDevPrintf(0x7180, 0x79C0, 0xFF0080, 0, D_0054FDB8,
                                     movieViewerDebugFlag.regAddr));
    if (movieViewerDebugFlag.wordPending != 0) {
        sdfDrawTagLink(tag, sdfDevPrintf(0x7840, 0x79C0, 0xFF0080, 0, D_0054FDB8,
                                         movieViewerDebugFlag.word));
    } else {
        sdfDrawTagLink(tag, sdfDevPrintf(0x7840, 0x79C0, 0xFF0080, 0, "********"));
    }
    step = 0;
    y = 0x7A80;
    for (i = 0; i != 8; i++, y += 0x60) {
        sdfDrawTagLink(tag, sdfDevPrintf(0x7180, y, 0xFF0080, 0, D_0054FDB8,
                                         movieViewerDebugFlag.buffer + step));
        x = 0x7840;
        for (col = 0; col != 8; col++, x += 0x240, step++) {
            if (movieViewerDebugFlag.blockPending != 0) {
                sdfDrawTagLink(tag, sdfDevPrintf(x, y, 0xFF0080, 0, D_0054FDC0,
                                                 movieViewerDebugFlag.block[step]));
            } else {
                sdfDrawTagLink(tag, sdfDevPrintf(x, y, 0xFF0080, 0, D_0054FDC8));
            }
        }
    }
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))D_002F6938.regFunc)(&D_002F6938, tag);
    return 0;
}

INCLUDE_RODATA(const s32, "movie/MovieViewer", D_0053F438);

INCLUDE_RODATA(const s32, "movie/MovieViewer", D_0053F448);

INCLUDE_RODATA(const s32, "movie/MovieViewer", D_0053F458);

INCLUDE_RODATA(const s32, "movie/MovieViewer", D_0053F468);

INCLUDE_RODATA(const s32, "movie/MovieViewer", D_0053F478);

INCLUDE_SDATA(const s32, "movie/MovieViewer", movieViewerPhase);

INCLUDE_SDATA(const s32, "movie/MovieViewer", D_0054FDB4);

INCLUDE_SDATA(const s32, "movie/MovieViewer", D_0054FDB8);

INCLUDE_SDATA(const s32, "movie/MovieViewer", D_0054FDC0);

INCLUDE_SDATA(const s32, "movie/MovieViewer", D_0054FDC8);

