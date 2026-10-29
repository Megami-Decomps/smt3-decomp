#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"
#include "sdf_movie_types.h"

extern void *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2, s32 v3,
                              void *drawExit, void *deleteExit, void *p);

extern u8 D_00343E74[];

typedef struct
{
    /* 0x0000 */ u8 unk_00[0x20];
    /* 0x0020 */ f32 screenOfsX;
    /* 0x0024 */ f32 screenOfsY;
    /* 0x0028 */ u8 unk_28[0x10];
    /* 0x0038 */ s32 texHandle[7];
    /* 0x0054 */ s32 stat;
    /* 0x0058 */ u16 unk_58;
    /* 0x005A */ u16 unk_5A;
    /* 0x005C */ u8 unk_5C[8];
    /* 0x0064 */ s16 lw_cur[4];
    /* 0x006C */ u8 unk_6C;
    /* 0x006D */ u8 unk_6D;
    /* 0x006E */ u16 modelId;
    /* 0x0070 */ s32 unk_70;
    /* 0x0074 */ u8 unk_74[0x10];
    /* 0x0084 */ u8 unk_84[0x2DC];
    /* 0x0360 */ void *explainData;
    /* 0x0364 */ s32 unk_364;
    /* 0x0368 */ void *allocation;
    /* 0x036C */ s32 pwork;
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

extern void sdfChipFreeDirect(void *aPtr);

extern sdfMovieCb_t D_00343E40;

extern s32 sdfMovieIsFinished(sdfMovieCb_t *movie);

extern void movieStop(void);

extern char D_0053F100[];

extern s32 scrGetIntPara(s32 n);

extern void scrSetIntReturnValue(s32 i);

extern void dds3PlayMovieByIndex(s32 index);

extern sdfGrOt_t sdfGrOtTable[85];

extern void release_movieobj2(sdfMovieCb_t *movie, sdfGrOt_t *ot);

typedef struct
{
    /* 0x00 */ const char *path;
    /* 0x04 */ sdfMovieInitParm_t descriptor;
} dds3MovieEntry_t;

extern dds3MovieEntry_t D_00343BE8[];

extern void moviePlayFile_27041744(const char *path, sdfMovieInitParm_t *descriptor);

extern u16 dds3VblanksPerFrame;

extern s32 PlayingNo;

extern void sdfIpuRunDecode(s32 frames);

extern void movieCloseStream(sdfMovieCb_t *stream);

extern s32 dds3KillProcess(s32 id, s32 shutdowntimer);

extern void (*DrawPhaseTable[])(fclEncyc_instance_tag *work);

s32 movieDrawProc(void)
{
    release_movieobj2(&D_00343E40, &sdfGrOtTable[57]);
    return 0;
}

extern void movieOpenStream(sdfMovieCb_t *stream, sdfMovieInitParm_t *descriptor, const char *path);

extern char D_0053F3C8[];

void moviePlayFile_27041744(const char *path, sdfMovieInitParm_t *descriptor)
{
    if (PlayingNo == 0)
    {
        movieOpenStream(&D_00343E40, descriptor, path);
        PlayingNo =
            (s32)dds3StartProcess(D_0053F3C8, 0x2AFB, 1, 1, movieDrawProc, NULL, NULL);
    }
}

void dds3PlayMovieByIndex(s32 index)
{
    dds3MovieEntry_t *entry = &D_00343BE8[index];

    moviePlayFile_27041744(entry->path, &entry->descriptor);
}

void movieStop(void)
{
    if (PlayingNo != 0) {
        movieCloseStream(&D_00343E40);
        dds3KillProcess(PlayingNo, 0);
        PlayingNo = 0;
    }
}

s32 dds3WaitMovieEnd(void)
{
    return sdfMovieIsFinished(&D_00343E40);
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
    /* 0x08 */ s16 top;
    /* 0x0A */ s16 cursor;
    /* 0x0C */ u16 unk_0C;
    /* 0x0E */ s8 playing;
    /* 0x0F */ u8 pad_0F;
    /* 0x10 */ sdfDrawTag_t *packets;
} movieViewerChipList_t;

/* One entry: the next link, then the path it was scanned from. */
typedef struct movieViewerChip_t
{
    /* 0x00 */ struct movieViewerChip_t *next;
    /* 0x04 */ char path[1];
} movieViewerChip_t;

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
        movieViewerChips.top = 0;
        movieViewerChips.cursor = 0;
        movieViewerChips.unk_0C = 0;
    }
}

typedef struct
{
    u32 mode;
    u8 stat[0x3C];
    char name[0x100];
    u32 privateWords[2];
} mviDirectoryEntry_t;

extern u8 sdfHddValid;

extern char D_0053F3E8[];

extern char D_0054FD98[];

extern char D_0054FDA0[];

extern movieViewerChip_t *movieViewerFileList[];

extern u16 movieViewerFileCount[];

extern s32 sceDopen(const char *name);

extern s32 sceDread(s32 handle, mviDirectoryEntry_t *entry);

extern s32 sceDclose(s32 handle);

extern char *strrchr(const char *text, s32 character);

extern s32 strcasecmp(const char *first, const char *second);

extern void *sdfChipAllocDirect(s32 aSize);

extern void *memcpy(void *destination, const void *source, u32 bytes);

void movieViewerScanDir(void)
{
    mviDirectoryEntry_t entry;
    movieViewerChip_t **link;
    movieViewerChip_t *chip;
    char *ext;
    s32 handle;
    s32 length;
    s32 count;

    movieViewerFree();
    if (sdfHddValid == 0) {
        return;
    }
    handle = sceDopen(D_0053F3E8);
    if (handle < 0) {
        return;
    }
    count = 0;
    link = movieViewerFileList;
    while ((length = sceDread(handle, &entry)) > 0) {
        if ((entry.mode & 0xF000) == 0x1000) {
            continue;
        }
        ext = strrchr(entry.name, '.');
        if (ext == NULL) {
            continue;
        }
        if (strcasecmp(ext, D_0054FD98) != 0 && strcasecmp(ext, D_0054FDA0) != 0) {
            continue;
        }
        count++;
        chip = sdfChipAllocDirect(length + 8);
        memcpy(chip->path, entry.name, length);
        chip->path[length] = '\0';
        *link = chip;
        link = &chip->next;
    }
    *link = NULL;
    movieViewerFileCount[0] = count;
    sceDclose(handle);
}

/* The entry the cursor sits on, walked from the head. */
void *movieViewerStep(void)
{
    movieViewerChip_t *chip = movieViewerChips.chip;
    s32 remaining = movieViewerChips.cursor;

    if (chip != NULL && remaining > 0) {
        do {
            chip = chip->next;
            remaining--;
        } while (chip != NULL && remaining > 0);
    }
    return chip;
}

extern void *dds3PrimFrameRect(s32 aX, s32 aY, s32 aZ, s32 aW, s32 aH, u32 aBgCol, u32 aFrameCol);

extern void *sdfDevPrintf(s32 aX, s32 aY, s32 aZ, s32 aAttr, const char *aStr, ...);

extern void sdfDrawTagLink(sdfDrawTag_t *aTag, void *aPacket);

extern void sdfPrimLinkF3(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1,
                          s32 aY1, s32 aX2, s32 aY2, s32 aZ, s32 (*aAllocator)(s32));

/* "%c%s", in .sdata where a literal cannot go. */
extern char D_0054FDA8[];

/* One screen of the movie list, with an arrow on the selected row and a
 * marker at each end the list continues past. */
void movieDebugDrawInfo(void)
{
    movieViewerChip_t *chip;
    sdfDrawTag_t *tag;
    s32 selected;
    s32 i;

    if (movieViewerChips.chip == NULL || movieViewerChips.playing != 0) {
        return;
    }
    tag = movieViewerChips.packets;
    sdfDrawTagLink(tag, dds3PrimFrameRect(0x7150, 0x7948, 0xFF0080, 0xF60, 0x3F0, 0x30000000,
                                          0x60404040));
    selected = movieViewerChips.cursor;
    i = movieViewerChips.top;
    chip = movieViewerChips.chip;
    selected -= i;
    for (; i > 0; i--) {
        chip = chip->next;
    }
    for (i = 0; i < 8 && chip != NULL; i++, chip = chip->next) {
        sdfDrawTagLink(tag, sdfDevPrintf(0x7240, 0x79C0 + i * 0x60, 0xFF0080, 0, D_0054FDA8,
                                         (i == selected) ? '>' : ' ', chip->path));
    }
    if (movieViewerChips.top != 0) {
        sdfPrimLinkF3(tag, 0x8000A0C0, 0, 0x7900, 0x7978, 0x7840, 0x79A8, 0x79C0, 0x79A8,
                      0xFF0080, 0);
    }
    if (chip != NULL) {
        sdfPrimLinkF3(tag, 0x8000A0C0, 0, 0x7840, 0x7CD8, 0x79C0, 0x7CD8, 0x7900, 0x7D08,
                      0xFF0080, 0);
    }
}
INCLUDE_RODATA(const s32, "movie/mviManager", D_0053F3C8);

INCLUDE_RODATA(const s32, "movie/mviManager", D_0053F3D8);

INCLUDE_RODATA(const s32, "movie/mviManager", D_0053F3E8);

INCLUDE_SDATA(const s32, "movie/mviManager", PlayingNo);

INCLUDE_SDATA(const s32, "movie/mviManager", D_0054FD98);

INCLUDE_SDATA(const s32, "movie/mviManager", D_0054FDA0);

INCLUDE_SDATA(const s32, "movie/mviManager", D_0054FDA8);

