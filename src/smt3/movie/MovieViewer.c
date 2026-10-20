#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"

extern void *dds3StartProcess(char *namestr, u32 priority, s32 starttimer, s32 shutdowntimer,
                              void *processfunc, void *shutdownfunc, void *data);

extern void *dds3SearchProcessName(const char *name);

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
} fclEncyc_instance_t;

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

typedef struct
{
    /* 0x00 */ u8 unk_00[2];
    /* 0x02 */ u8 unk_02;
    /* 0x03 */ u8 unk_03;
    /* 0x04 */ u32 regAddr;
    /* 0x08 */ u8 *buffer;
} movieViewerWork_t;

extern movieViewerWork_t movieViewerDebugFlag;

extern s32 dds3KillProcess(s32 process, s32 shutdowntimer);

extern void (*DrawPhaseTable[])(fclEncyc_instance_t *work);

INCLUDE_ASM(const s32, "movie/MovieViewer", MovieViewerDrawFrameInfo);

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
    movieViewerDebugFlag.unk_02 = 1;
    movieViewerDebugFlag.unk_03 = 1;
}

void movieViewerInitRegs(void)
{
    movieViewerDebugFlag.regAddr = 0x10002010;
    movieViewerDebugFlag.buffer = D_00343E40;
    movieViewerSetFlags();
}

INCLUDE_ASM(const s32, "movie/MovieViewer", movieViewerDrawInfo);

INCLUDE_RODATA(const s32, "movie/MovieViewer", D_0053F418);

INCLUDE_ASM(const s32, "movie/MovieViewer", movieViewerDebugDraw);

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

