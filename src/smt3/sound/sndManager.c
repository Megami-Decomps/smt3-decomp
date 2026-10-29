#include "common.h"
#include "facility.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"
#include "kernel.h"

typedef struct
{
    /* 0x00 */ sdfMemHandle_t *allocation;
    /* 0x04 */ sdfTexHandle_t *texHandle[5];
    /* 0x18 */ s32 mode;
    /* 0x1C */ s32 cursor;
    /* 0x20 */ s32 unk_20;
    /* 0x24 */ s32 unk_24;
    /* 0x28 */ s32 unk_28;
    /* 0x2C */ s32 unk_2C;
    /* 0x30 */ s32 unk_30;
    /* 0x34 */ s32 unk_34;
    /* 0x38 */ s32 unk_38;
    /* 0x3C */ s32 unk_3C;
} dds3TitleWork_t;

extern dds3TitleWork_t *titleWork;

extern void p2sdStopAllSe(void);

extern void sdfMemFreeDirect(void *aHandle);

extern dds3ProcessID_t *titleAllocWork(s32 arg);

extern void movieStop(void);

extern s8 dds3PadButtonState[];

extern s32 scrGetIntPara(s32 n);

extern s32 p2sdPlayBGM(s32 MidiNo);

extern void sdfTexFree(sdfTexHandle_t *aHandle);

extern void p2sdPlaySE(s32 index, s32 volume, s32 pan);

extern s32 titleFreeWork(s32 arg);

extern s32 dds3KernelDebugSign;

extern void titleFreeTextures(void);

extern void (*DrawPhaseTable[])(fclEncyc_instance_tag *work);

extern char D_0054FD18[]; /* "/sound/" */

extern char *strcat(char *dst, const char *src);

extern void *memcpy(void *a, const void *b, u32 c);

char *sdfSndMakeFilename(char *aDstName, const char *aSrcName)
{
    memcpy(aDstName, D_0054FD18, 8);
    return strcat(aDstName, aSrcName);
}

extern s8 D_0054FD10;

extern s32 D_0054FD20;

extern s32 D_0054FD24;

extern s16 D_00551F94;

extern char D_00342FA0[];

extern char D_00342FC0[];

extern void *D_0033A540[];

extern void sdfSndInit(char *aVcInfo, s32 aNumVc, char *aSqInfo, s32 aNumSq);

extern void p2sdLoadSMGFile(void *MidiNo);

void sndInit(void)
{
    if (D_0054FD10 != 0) {
        p2sdStopAllSe();
        return;
    }
    D_0054FD10 = 1;
    sdfSndInit(D_00342FA0, 4, D_00342FC0, 4);
    p2sdLoadSMGFile(D_0033A540[0]);
    D_0054FD20 = 4;
    D_00551F94 = -1;
    D_0054FD24 = 0;
}

s32 snd_226388(void)
{
    return 0x7B0;
}

s32 snd_226390(void)
{
    p2sdPlayBGM(scrGetIntPara(0));
    return 1;
}

s32 dds3_2263B8(void)
{
    p2sdPlaySE(scrGetIntPara(0), 0x7F, 0x3F);
    return 1;
}

s32 dds3_2263E8(void)
{
    p2sdStopAllSe();
    return 1;
}

void dds3TitleUpdateCursor(void)
{
    if ((dds3PadButtonState[0x26] & 2) != 0) {
        titleWork->cursor--;
        p2sdPlaySE(0, 0x7F, 0x3F);
    }
    if ((dds3PadButtonState[0x27] & 2) != 0) {
        titleWork->cursor++;
        p2sdPlaySE(0, 0x7F, 0x3F);
    }
    if (titleWork->cursor < 0) {
        titleWork->cursor = 3;
    }
    if (titleWork->cursor >= 4) {
        titleWork->cursor = 0;
    }
}

INCLUDE_ASM(const s32, "sound/sndManager", dds3TitleDrawMenu);

INCLUDE_ASM(const s32, "sound/sndManager", dds3TitleDrawFade);

void dds3TitleClearFade(void)
{
    titleWork->unk_34 = 0;
    titleWork->unk_30 = 0;
}

void dds3TitleSetFade(void)
{
    titleWork->unk_34 = 0xC0;
    titleWork->unk_30 = 0x32;
}

extern void rsetOtOfs(s32 offset);
extern void rsetAlpha(s32 mode);
extern void rsetTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm,
                     s32 zte, s32 ztst);
extern void DrwSprite2D(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh,
                        s32 color, sdfTexHandle_t *texture);
extern f32 sdfSin(f32 angle);
extern f32 dds3TitleFadeLevel;
extern void dds3TitleDrawFade(void);

#ifdef NON_MATCHING
void dds3TitleUpdateFade(void)
{
    s32 alpha = 0;
    s32 glowAlpha;
    s32 frame;
    u32 color;

    if (titleWork->unk_34 == -1) {
        return;
    }
    rsetOtOfs(0x48);
    rsetAlpha(0);
    rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    frame = titleWork->unk_34;
    if (titleWork->unk_3C == 0) {
        if (frame >= 0x7E) {
            frame -= 0x7D;
            if (frame > 30) frame = 30;
            alpha = (s32)((u32)frame << 7) / 30;
        }
    } else {
        if (frame > 12) frame = 12;
        alpha = (s32)((u32)frame << 7) / 12;
    }
    if (alpha <= 0) {
        dds3TitleFadeLevel = 0.0f;
    } else {
        glowAlpha = alpha;
        if (alpha == 0x80) {
            glowAlpha += (s32)(sdfSin(dds3TitleFadeLevel) * 32.0f);
            dds3TitleFadeLevel += 0.04999999702f;
        }
        rsetAlpha(1);
        DrwSprite2D(0x67, 0xCD, 0x132, 0x3C, 1, 0xBD, 0x132, 0x3C,
                    ((u32)glowAlpha << 24) | 0x808080, titleWork->texHandle[1]);
        rsetAlpha(0);
        DrwSprite2D(0x6C, 0xD8, 0x128, 0x27, 0xCC, 0x85, 0x128, 0x27,
                    ((u32)alpha << 24) | 0x808080, titleWork->texHandle[1]);
    }
    frame = titleWork->unk_34;
    if (frame > 12) frame = 12;
    color = ((u32)((s32)((u32)frame << 7) / 12) << 24) | 0x808080;
    DrwSprite2D(0xF, 0x54, 0x1D5, 0x7D, 2, 2, 0x1D4, 0x7D,
                color, titleWork->texHandle[1]);
    DrwSprite2D(0x128, 0xAC, 0xC6, 0x34, 2, 0x84, 0xC6, 0x34,
                color, titleWork->texHandle[1]);
    if (titleWork->unk_34 >= 13) {
        dds3TitleDrawFade();
    }
    titleWork->unk_34 = (s32)((u32)titleWork->unk_34 + 1);
}
#else
INCLUDE_ASM(const s32, "sound/sndManager", dds3TitleUpdateFade);
#endif

typedef struct titlePacFileRequest_t {
    u8 fileRequestHeader[0x30];
    sdfPacDecodeCb_t packet;
} titlePacFileRequest_t;

extern char D_0053DD68[];
extern char D_0053DD98[];
extern void *pacLoad(const char *path);
extern void pacSync(void *request);
extern s32 pacFileHandleFree_(void *request);

s32 titleLoadTextures(void)
{
    titlePacFileRequest_t *request;
    sdfPacDecodeResrc_t *entry;
    s32 index = 0;

    titleWork->texHandle[4] = titleWork->texHandle[3] = titleWork->texHandle[2] =
        titleWork->texHandle[1] = titleWork->texHandle[0] = 0;
    request = pacLoad(D_0053DD68);
    pacSync(request);
    for (entry = request->packet.resrcHead; entry != NULL; entry = entry->next) {
        switch (index) {
        case 0:
            titleWork->texHandle[0] = entry->resrc;
            break;
        case 1:
            titleWork->texHandle[1] = entry->resrc;
            break;
        case 2:
            titleWork->texHandle[2] = entry->resrc;
            break;
        case 3:
            titleWork->texHandle[3] = entry->resrc;
            break;
        case 4:
            titleWork->texHandle[4] = entry->resrc;
            break;
        }
        index++;
    }
    return pacFileHandleFree_(request);
}

void titleFreeTextures(void)
{
    if (titleWork->texHandle[0] != 0) {
        sdfTexFree(titleWork->texHandle[0]);
        titleWork->texHandle[0] = 0;
    }
    if (titleWork->texHandle[1] != 0) {
        sdfTexFree(titleWork->texHandle[1]);
        titleWork->texHandle[1] = 0;
    }
    if (titleWork->texHandle[2] != 0) {
        sdfTexFree(titleWork->texHandle[2]);
        titleWork->texHandle[2] = 0;
    }
    if (titleWork->texHandle[3] != 0) {
        sdfTexFree(titleWork->texHandle[3]);
        titleWork->texHandle[3] = 0;
    }
    if (titleWork->texHandle[4] != 0) {
        sdfTexFree(titleWork->texHandle[4]);
        titleWork->texHandle[4] = 0;
    }
}

extern void evtBitOn(s32 no);
extern sdfMemHandle_t *sdfMemAllocDirect(s32 size);
extern void *sdfMemLock(sdfMemHandle_t *handle);
extern s32 p2sdStopBGM(s32 id);
extern void dds3FadeIn2(s32 frames);
extern s32 dds3PlayMovieByIndex(s32 index);
extern u32 dds3BackColor;
extern s32 dds3Titlemain(dds3ProcessID_t *id);
extern dds3ProcessID_t *dds3StartProcess(char *name, u32 priority, s32 startTimer,
                                         s32 shutdownTimer, dds3ProcessFunc_t process,
                                         dds3ProcessFunc_t shutdown, void *work);

#ifdef NON_MATCHING
dds3ProcessID_t *titleAllocWork(s32 mode)
{
    sdfMemHandle_t *allocation;
    dds3TitleWork_t *work;

    evtBitOn(0xC0F);
    allocation = sdfMemAllocDirect(sizeof(*work));
    work = sdfMemLock(allocation);
    titleWork = work;
    work->unk_34 = -1;
    work->allocation = allocation;
    work->unk_2C = 0;
    work->mode = 0;
    work->cursor = 0;
    work->unk_20 = 0;
    work->unk_30 = 0;
    work->unk_38 = 0;
    work->unk_3C = 0;
    titleLoadTextures();
    if (mode == 2) {
        titleWork->cursor = 0;
    }
    if (mode == 1) {
        titleWork->cursor = 2;
    }
    if (mode == 3) {
        titleWork->cursor = 1;
    }
    if (mode != 0) {
        p2sdStopBGM(0x3A0000);
        dds3FadeIn2(15);
        dds3PlayMovieByIndex(13);
        titleWork->mode = 22;
        titleWork->unk_28 = 0x80;
        titleWork->unk_20 = 0;
        titleWork->unk_30 = 0;
    }
    dds3BackColor = 0x80000000;
    return dds3StartProcess(D_0053DD98, 0x2B19, 0, 0, dds3Titlemain, NULL, NULL);
}
#else
INCLUDE_ASM(const s32, "sound/sndManager", titleAllocWork);
#endif

s32 titleFreeWork(s32 arg)
{
    titleFreeTextures();
    sdfMemFreeDirect(titleWork->allocation);
    titleWork = NULL;
    movieStop();
    dds3KernelDebugSign = 1;
    return 1;
}

INCLUDE_RODATA(const s32, "sound/sndManager", D_0053DD98);

INCLUDE_RODATA(const s32, "sound/sndManager", D_0053DDA8);

INCLUDE_SDATA(const s32, "sound/sndManager", D_0054FD10);

INCLUDE_SDATA(const s32, "sound/sndManager", D_0054FD18);

INCLUDE_SDATA(const s32, "sound/sndManager", D_0054FD20);

INCLUDE_SDATA(const s32, "sound/sndManager", D_0054FD24);

INCLUDE_SDATA(const s32, "sound/sndManager", titleWork);

INCLUDE_SDATA(const s32, "sound/sndManager", D_0054FD2C);

INCLUDE_SDATA(const s32, "sound/sndManager", D_0054FD30);

INCLUDE_SDATA(const s32, "sound/sndManager", dds3TitleFadeLevel);

