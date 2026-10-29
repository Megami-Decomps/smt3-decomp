#include "common.h"
#include "sndManager.h"
#include "facility.h"
#include "fpu.h"
#include "libsdf.h"
#include "titleGlobal.h"
#include "fileManager.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"
#include "kernel.h"



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

extern void rsetOtOfs(s32 offset);
extern void rsetAlpha(s32 mode);
extern void rsetTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 zte, s32 ztst);
extern void DrwSprite2D(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh,
                        s32 color, sdfTexHandle_t *texture);
extern void aDrawBoxG(s32 x, s32 y, s32 w, s32 h, u32 r0, u32 g0, u32 b0, u32 a0,
                      u32 r1, u32 g1, u32 b1, u32 a1, u32 r2, u32 g2, u32 b2, u32 a2,
                      u32 r3, u32 g3, u32 b3, u32 a3);
extern f32 sdfSin(f32 angle);
extern void fldKanjiDraw(s32 x, s32 y, u32 rgba, const u8 *text);
extern const u8 *D_00342FD0[], *D_00342FD8[], *D_00342FE0[], *D_00342FE8[];
extern f32 D_0054FD30;

void dds3TitleDrawMenu(void)
{
    dds3TitleWork_t *work;
    s32 alpha, shift;
    f32 phase;

    rsetOtOfs(0x48);
    rsetAlpha(0);
    rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    rsetAlpha(1);
    aDrawBoxG(0, 0, 0xBE, 0xBE, 0x25, 0x27, 0x54, 0x3D,
              0x25, 0x27, 0x54, 0, 0x25, 0x27, 0x54, 0, 0x25, 0x27, 0x54, 0);
    rsetAlpha(0);
    /* Dead: recomputed below. Its division-by-zero trap survives in retail. */
    alpha = titleWork->unk_2C * 0x80 / 24;
    DrwSprite2D(0, 0x1D, 0xFC, 0x1C, 2, 3, 0xFC, 0x1C, 0x80808080, titleWork->texHandle[3]);
    DrwSprite2D(0xFC, 0x1D, 0x104, 0x1C, 0xFD, 3, 1, 0x1C, 0x80808080, titleWork->texHandle[3]);
    DrwSprite2D(0, 0xC7, 0xFC, 5, 2, 0x22, 0xFC, 5, 0x80808080, titleWork->texHandle[3]);
    DrwSprite2D(0xFC, 0xC7, 0x104, 5, 0xFD, 0x22, 1, 5, 0x80808080, titleWork->texHandle[3]);
    alpha = titleWork->unk_2C * 0x80 / 24;
    aDrawBoxG(0, 0x1C, 0x100, 0x48, 0, 0, 0, alpha,
              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, alpha);
    alpha = titleWork->unk_2C * 0x73 / 24;
    aDrawBoxG(0xC4, 0x1C, 0x13C, 0x48, 0, 0, 0, 0,
              0, 0, 0, alpha, 0, 0, 0, alpha, 0, 0, 0, 0);
    work = titleWork;
    if (work->unk_2C >= 12) {
        rsetAlpha(1);
        if (titleWork->unk_2C < 24) {
            alpha = (s32)(sdfSin(D_0054FD30) * 20.0f + 10.0f) + 0x5C;
            work = titleWork;
            alpha = alpha * (work->unk_2C - 12) / 12;
        } else {
            phase = D_0054FD30 + 0.15f;
            D_0054FD30 = phase;
            alpha = (s32)(sdfSin(phase) * 20.0f + 10.0f) + 0x5C;
            work = titleWork;
        }
        aDrawBoxG(0, work->cursor * 15 + 0x24, 0xDF, 0xB,
                  0, 0, 0x80, alpha, 0x40, 0, 0x80, 1,
                  0x60, 0, 0x80, 1, 0x20, 0, 0x80, alpha);
        rsetAlpha(0);
        work = titleWork;
    }
    DrwSprite2D(0x11, 0x23, 0x80, 0x17, 2, 0x59, 0x80, 0x17, 0x80808080, work->texHandle[3]);
    work = titleWork;
    shift = work->unk_2C * 24 - 0x100;
    if (shift > 0) shift = 0;
    alpha = work->unk_2C * 8;
    if (alpha < 0) alpha = 0;
    if (alpha > 0x80) alpha = 0x80;
    if (work->cursor == 0) {
        alpha <<= 24;
        alpha |= 0x808080;
        DrwSprite2D(shift + 0x29, 0x47, 0x14, 0x15, 0xDE, 0x69, 0x14, 0x15, alpha, titleWork->texHandle[3]);
        DrwSprite2D(shift + 0x39, 0x46, 0x86, 0x17, 0x2, 0x29, 0x86, 0x17, alpha, titleWork->texHandle[3]);
    } else {
        alpha <<= 24;
        alpha |= 0x808080;
        DrwSprite2D(shift + 0x2E, 0x4C, 0xA, 0xB, 0xF4, 0x73, 0xA, 0xB, alpha, titleWork->texHandle[3]);
        DrwSprite2D(shift + 0x40, 0x4B, 0x7A, 0xE, 0x2, 0x71, 0x7A, 0xE, alpha, titleWork->texHandle[3]);
    }
    work = titleWork;
    shift = work->unk_2C * 24 - 0x120;
    if (shift > 0) shift = 0;
    alpha = work->unk_2C * 8 - 8;
    if (alpha < 0) alpha = 0;
    if (alpha > 0x80) alpha = 0x80;
    if (work->cursor == 1) {
        alpha <<= 24;
        alpha |= 0x808080;
        DrwSprite2D(shift + 0x29, 0x65, 0x14, 0x15, 0xDE, 0x69, 0x14, 0x15, alpha, titleWork->texHandle[3]);
        DrwSprite2D(shift + 0x39, 0x64, 0x80, 0x17, 0x2, 0x41, 0x80, 0x17, alpha, titleWork->texHandle[3]);
    } else {
        alpha <<= 24;
        alpha |= 0x808080;
        DrwSprite2D(shift + 0x2E, 0x6A, 0xA, 0xB, 0xF4, 0x73, 0xA, 0xB, alpha, titleWork->texHandle[3]);
        DrwSprite2D(shift + 0x40, 0x69, 0x73, 0xE, 0x85, 0x42, 0x73, 0xE, alpha, titleWork->texHandle[3]);
    }
    work = titleWork;
    shift = work->unk_2C * 24 - 0x140;
    if (shift > 0) shift = 0;
    alpha = work->unk_2C * 8 - 16;
    if (alpha < 0) alpha = 0;
    if (alpha > 0x80) alpha = 0x80;
    if (work->cursor == 2) {
        alpha <<= 24;
        alpha |= 0x808080;
        DrwSprite2D(shift + 0x29, 0x83, 0x14, 0x15, 0xDE, 0x69, 0x14, 0x15, alpha, titleWork->texHandle[3]);
        DrwSprite2D(shift + 0x39, 0x82, 0x55, 0x17, 0x89, 0x29, 0x55, 0x17, alpha, titleWork->texHandle[3]);
    } else {
        alpha <<= 24;
        alpha |= 0x808080;
        DrwSprite2D(shift + 0x2E, 0x88, 0xA, 0xB, 0xF4, 0x73, 0xA, 0xB, alpha, titleWork->texHandle[3]);
        DrwSprite2D(shift + 0x40, 0x87, 0x48, 0xE, 0x85, 0x60, 0x48, 0xE, alpha, titleWork->texHandle[3]);
    }
    work = titleWork;
    shift = work->unk_2C * 24 - 0x160;
    if (shift > 0) shift = 0;
    alpha = work->unk_2C * 8 - 16;
    if (alpha < 0) alpha = 0;
    if (alpha > 0x80) alpha = 0x80;
    if (work->cursor == 3) {
        DrwSprite2D(shift + 0x2A, 0xA1, 0x80, 0x15, 0, 0x17, 0x80, 0x15,
                    (alpha << 24) | 0x808080, work->texHandle[4]);
    } else {
        DrwSprite2D(shift + 0x2A, 0xA1, 0x80, 0x15, 0, 0, 0x80, 0x15,
                    (alpha << 24) | 0x808080, work->texHandle[4]);
    }
    if (titleWork->unk_2C >= 24) {
        switch (titleWork->cursor) {
        case 0:
            fldKanjiDraw(0x117, 0x28, 0x64618780, D_00342FD8[0]);
            fldKanjiDraw(0x117, 0x34, 0x64618780, D_00342FD8[1]);
            break;
        case 1:
            fldKanjiDraw(0x117, 0x28, 0x64618780, D_00342FD0[0]);
            fldKanjiDraw(0x117, 0x34, 0x64618780, D_00342FD0[1]);
            break;
        case 2:
            fldKanjiDraw(0x117, 0x28, 0x64618780, D_00342FE0[0]);
            fldKanjiDraw(0x117, 0x34, 0x64618780, D_00342FE0[1]);
            break;
        case 3:
            /* The loop nesting blocks the sibling call on the last draw. */
            do {
                fldKanjiDraw(0x117, 0x28, 0x64618780, D_00342FE8[0]);
                fldKanjiDraw(0x117, 0x34, 0x64618780, D_00342FE8[1]);
                fldKanjiDraw(0x117, 0x40, 0x64618780, D_00342FE8[2]);
            } while (0);
            break;
        }
    }
}

#ifdef NON_MATCHING
extern void rsetAlpha(s32 mode);
extern void DrwSprite2D(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh,
                        s32 color, sdfTexHandle_t *texture);
extern void aDrawBoxG(s32 x, s32 y, s32 w, s32 h, u32 r0, u32 g0, u32 b0, u32 a0,
                      u32 r1, u32 g1, u32 b1, u32 a1, u32 r2, u32 g2, u32 b2, u32 a2,
                      u32 r3, u32 g3, u32 b3, u32 a3);

void dds3TitleDrawFade(void)
{
    dds3TitleWork_t *work;
    s32 count;
    s32 age;
    s32 progress;
    s32 alpha;
    s32 remainder;
    u32 color;

    rsetAlpha(1);
    work = titleWork;
    count = work->unk_30;
    work->unk_30 = (s32)((u32)count + 1);
    if ((s32)((u32)count + 1) >= 0x3C) {
        progress = count - 0x3B;
        if (progress >= 0x11) {
            progress = 0x10;
        }
        aDrawBoxG(-0x12C, 0x15, 0x32C, 0xCA,
                  0x23, 0x25, 0x56, 0,
                  0x23, 0x25, 0x56, 0,
                  0x23, 0x25, 0x56, progress * 3,
                  0x23, 0x25, 0x56, 0);
    }

    rsetAlpha(0);
    count = titleWork->unk_30;
    if (count >= 0x32) {
        age = count - 0x32;
        if (age > 0x96) {
            age = 0x96;
        }
        alpha = (age * 0x80) / 0x96;
        color = ((u32)alpha << 24) | 0x00808080;
        DrwSprite2D(0xA7, 0x186, 0xB2, 0x18, 2, 0x7C, 0xB2, 0x18,
                    color, titleWork->texHandle[2]);
    }

    DrwSprite2D(0x1A, 0x1A4, 0x1C7, 0x14, 0, 0x96, 0x1C7, 0x14,
                0x80808080, titleWork->texHandle[2]);
    rsetAlpha(1);
    count = titleWork->unk_30;
    if (count >= 0x32) {
        age = count - 0x32;
        if (age < 0x78) {
            if (age < 0x3F) {
                alpha = (age * 0x80) / 0x3F;
            } else {
                remainder = age - 0x3F;
                alpha = 0x80 - ((remainder * 0x66) / 0x39);
            }
            if (alpha < 0) alpha = 0;
            if (alpha > 0x80) alpha = 0x80;
            color = ((u32)alpha << 24) | 0x00808080;
            DrwSprite2D(0x4D, 0x111, 0x166, 0x63, 2, 0x15, 0x166, 0x63,
                        color, titleWork->texHandle[2]);
        } else {
            remainder = (age - 0x78) % 0x42;
            if (remainder < 0x13) {
                alpha = 0x19 + ((remainder * 0x33) / 0x13);
            } else {
                alpha = 0x80 - (((remainder * 0x66) - 0x792) / 0x2F);
            }
            if (alpha < 0) alpha = 0;
            if (alpha > 0x80) alpha = 0x80;
            color = ((u32)alpha << 24) | 0x00808080;
            DrwSprite2D(0x4D, 0x111, 0x166, 0x63, 2, 0x15, 0x166, 0x63,
                        color, titleWork->texHandle[2]);
        }

        count = titleWork->unk_30;
        if (count >= 0x32) {
            age = count - 0x32;
            if (age < 0x78) {
                if (age < 0x3F) {
                    alpha = (age * 0x66) / 0x3F;
                } else {
                    remainder = age - 0x3F;
                    alpha = 0x66 - ((remainder * 0x2C) / 0x39);
                }
                if (alpha < 0) alpha = 0;
                if (alpha > 0x80) alpha = 0x80;
                color = ((u32)alpha << 24) | 0x00808080;
                DrwSprite2D(0xA2, 0x13A, 0xBA, 0x11, 1, 1, 0xBA, 0x11,
                            color, titleWork->texHandle[2]);
            } else {
                remainder = (age - 0x78) % 0x42;
                if (remainder < 0x0D) {
                    alpha = 0x39 - ((remainder * 6) / 0x0D);
                } else if (remainder < 0x11) {
                    alpha = 0x33 + (((remainder * 0x33) - 0x297) / 4);
                } else {
                    alpha = 0x66 - (((remainder * 0x2C) - 0x2EC) / 0x31);
                }
                color = ((u32)alpha << 24) | 0x00808080;
                DrwSprite2D(0xA2, 0x13A, 0xBA, 0x11, 1, 1, 0xBA, 0x11,
                            color, titleWork->texHandle[2]);
            }
        }
    }
    rsetAlpha(0);
}
#else
INCLUDE_ASM(const s32, "sound/sndManager", dds3TitleDrawFade);
#endif

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


extern char D_0053DD68[];
extern char D_0053DD98[];
extern void *pacLoad(const char *path);
extern void pacSync(void *request);
extern s32 pacFileHandleFree_(void *request);

s32 titleLoadTextures(void)
{
    fileRequest_t *request;
    sdfPacDecodeResrc_t *entry;
    s32 index = 0;

    titleWork->texHandle[4] = titleWork->texHandle[3] = titleWork->texHandle[2] =
        titleWork->texHandle[1] = titleWork->texHandle[0] = 0;
    request = pacLoad(D_0053DD68);
    pacSync(request);
    for (entry = request->pac.resrcHead; entry != NULL; entry = entry->next) {
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
extern void dds3PlayMovieByIndex(s32 index);
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

