#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"

extern void *dds3StartProcess(char *namestr, u32 priority, s32 starttimer, s32 shutdowntimer,
                              void *processfunc, void *shutdownfunc, void *data);

extern u8 D_00343E74[];

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

extern void sdfChipFreeDirect(void *ptr);

extern u8 D_00343E40[];

extern s32 sdfMovieIsFinished(void *movie);

extern void movieStop(void);

extern char D_0053F100[];

extern s32 scrGetIntPara(s32 index);

extern void scrSetIntReturnValue(s32 value);

extern s32 dds3PlayMovieByIndex(s32 index);

extern sdfGrOt_t sdfGrOtTable[85];

extern void release_movieobj2(void *movie, sdfGrOt_t *ot);

typedef struct
{
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ char name[0x14];
} dds3MovieEntry_t;

extern dds3MovieEntry_t D_00343BE8[];

extern s32 moviePlayFile_27041744(s32 arg0, char *name);

extern u16 dds3VblanksPerFrame;

extern s32 PlayingNo;

extern void sdfIpuRunDecode(s32 frames);

extern void movieCloseStream(u8 *movie);

extern s32 dds3KillProcess(s32 process, s32 shutdowntimer);

extern void (*DrawPhaseTable[])(fclEncyc_instance_t *work);

s32 movieDrawProc(void)
{
    release_movieobj2(D_00343E40, &sdfGrOtTable[57]);
    return 0;
}

extern void movieOpenStream(dds3MovieEntry_t *table, char *name, s32 no);

extern char D_0053F3C8[];

s32 moviePlayFile_27041744(s32 no, char *name)
{
    if (PlayingNo == 0)
    {
        movieOpenStream(D_00343E40, name, no);
        PlayingNo =
            (s32)dds3StartProcess(D_0053F3C8, 0x2AFB, 1, 1, movieDrawProc, NULL, NULL);
    }
}

s32 dds3PlayMovieByIndex(s32 index)
{
    dds3MovieEntry_t *entry = &D_00343BE8[index];

    return moviePlayFile_27041744(entry->unk_00, entry->name);
}

void movieStop(void)
{
    if (PlayingNo != 0) {
        movieCloseStream(D_00343E40);
        dds3KillProcess(PlayingNo, 0);
        PlayingNo = 0;
    }
}

s32 dds3WaitMovieEnd(void)
{
    return sdfMovieIsFinished(D_00343E40);
}

u8 func_0022B7F8(void)
{
    return D_00343E74[0];
}

s32 movieMan_Process(void)
{
    sdfIpuRunDecode(60 / dds3VblanksPerFrame);
    return 0;
}

extern void sdfIpuInit(void);

extern char D_0053F3D8[];

void *dds3StartMovieManager(void)
{
    sdfIpuInit();
    return dds3StartProcess(D_0053F3D8, 0x385, 1, 0, movieMan_Process, NULL, NULL);
}

s32 dds3_22B880(void)
{
    dds3PlayMovieByIndex(scrGetIntPara(0));
    return 1;
}

s32 dds3_22B8A8(void)
{
    movieStop();
    return 1;
}

s32 movie_22B8C8(void)
{
    scrSetIntReturnValue(dds3WaitMovieEnd() == 0);
    return 1;
}

s32 movie_22B8F0(void)
{
    return func_0022B7F8() == 2;
}

typedef struct
{
    /* 0x00 */ u8 unk_00[4];
    /* 0x04 */ void *chip;
    /* 0x08 */ u16 unk_08;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ u16 unk_0C;
} movieViewerChipList_t;

/* Also declared above as void *movieViewerWork[], which is how the loader
 * paths use it; this is the chip list the viewer walks. One object, two
 * readings, so the alias stays. */
extern movieViewerChipList_t movieViewerChips asm("movieViewerWork");

void movieViewerFree(void)
{
    void *chip = movieViewerChips.chip;

    if (chip != NULL) {
        do {
            void *next = *(void **)chip;

            sdfChipFreeDirect(chip);
            chip = next;
        } while (chip != NULL);
        movieViewerChips.chip = NULL;
        movieViewerChips.unk_08 = 0;
        movieViewerChips.unk_0A = 0;
        movieViewerChips.unk_0C = 0;
    }
}

INCLUDE_RODATA(const s32, "mviManager", D_0053F3C8);

INCLUDE_RODATA(const s32, "mviManager", D_0053F3D8);

INCLUDE_ASM(const s32, "mviManager", movieViewerScanDir);

/* NON_MATCHING: 6 of 14 words match, 56 bytes against retail's 80; the
 * loop closes with a plain `b` where retail has `bgtzl` and five pad nops. */
#ifdef NON_MATCHING
void *movieViewerStep(void)
{
    void **chip = (void **)movieViewerChips.chip;
    s32 n = movieViewerChips.unk_0A;

    if (chip == NULL) {
        goto out;
    }
    if (n <= 0) {
        goto out;
    }
    chip = *chip;
loop:
    if (chip == NULL) {
        goto out;
    }
    n--;
    if (n > 0) {
        chip = *chip;
        goto loop;
    }
out:
    return chip;
}
#else
INCLUDE_ASM(const s32, "mviManager", movieViewerStep);
#endif

INCLUDE_ASM(const s32, "mviManager", movieDebugDrawInfo);
INCLUDE_SDATA(const s32, "mviManager", PlayingNo);

INCLUDE_SDATA(const s32, "mviManager", D_0054FD98);

INCLUDE_SDATA(const s32, "mviManager", D_0054FDA0);

INCLUDE_SDATA(const s32, "mviManager", D_0054FDA8);

