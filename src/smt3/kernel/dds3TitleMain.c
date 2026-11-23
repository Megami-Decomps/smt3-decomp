#include "sndManager.h"
#include "fileManager.h"
#include "common.h"
#include "sdfFrame.h"
#include "facility.h"
#include "fpu.h"
#include "libsdf.h"
#include "titleGlobal.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"
#include "kernel.h"


extern dds3TitleWork_t *titleWork;

extern u32 dds3GetKernelCount(void);

extern s32 p2sdStopAllSe(void);

extern void sdfMemFreeDirect(void *aHandle);

extern dds3ProcessID_t *titleAllocWork(s32 arg);

extern void movieStop(void);

extern s8 dds3PadButtonState[];

extern s32 scrGetIntPara(s32 n);

extern void p2sdPlayBGM(s32 MidiNo);

extern void sdfTexFree(sdfTexHandle_t *aHandle);

extern void p2sdPlaySE(s32 index, s32 volume, s32 pan);

extern s32 titleFreeWork(s32 arg);

extern s32 dds3KernelDebugSign;

extern void titleFreeTextures(void);

extern void (*DrawPhaseTable[])(fclEncyc_instance_tag *work);

extern char sndPathPrefix[]; /* "/sound/" */

extern char *strcat(char *dst, const char *src);

extern void *memcpy(void *a, const void *b, size_t c);

#include "mviManager.h"
#include "mselGlobal.h"

extern s32 dds3KillProcessName(char *name, s32 shutdowntimer);


extern const char dds3TitleProcName[];


extern mselGlobalWork_t *gGlobal;

extern void *sdfMemGetPtr(sdfMemHandle_t *aHandle);
#include "sdk_libc.h"

extern char mselLevelLabel[];
extern char mselLevelFormat[];
extern const char *datExplainTable[];
extern char *text2[];

extern void frDestroyFontRequestAfter(void *pFRQ);


extern void eglSetOt(u32 ot);

extern void eglLinkTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 zte, s32 ztst);

extern void eglLinkAlpha(s32 mode);

extern void eglLinkSprite(s32 x, s32 y, s32 w, s32 h,
                          s32 u, s32 v, s32 uw, s32 vh,
                          u32 rgba0, u32 rgba1, u32 rgba2, u32 rgba3, sdfTexHandle_t *th);


const char D_0053DCD0[24] __attribute__((aligned(16))) = "from the beginning.";

const char D_0053DCE8[16] = "Start the game";

const char D_0053DCF8[16] = "from a save.";

const char D_0053DD08[16] = "of the game.";

const char D_0053DD18[24] = "Adjust the settings";

const char D_0053DD30[24] = "coming from Atlus.";

const char D_0053DD48[16] = "the latest game";

const char D_0053DD58[16] = "Check out";

/* The menu renderer reads these strings as unsigned encoded bytes. */
const u8 *D_00342FD0[2] __attribute__((section(".data"))) = {
    (const u8 *)D_0053DCE8, (const u8 *)D_0053DCD0,
};
const u8 *D_00342FD8[2] __attribute__((section(".data"))) = {
    (const u8 *)D_0053DCE8, (const u8 *)D_0053DCF8,
};
const u8 *D_00342FE0[2] __attribute__((section(".data"))) = {
    (const u8 *)D_0053DD18, (const u8 *)D_0053DD08,
};
const u8 *D_00342FE8[4] = {
    (const u8 *)D_0053DD58, (const u8 *)D_0053DD48, (const u8 *)D_0053DD30, NULL,
};
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
extern f32 dds3TitleMenuPhase;

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
            alpha = (s32)(sdfSin(dds3TitleMenuPhase) * 20.0f + 10.0f) + 0x5C;
            work = titleWork;
            alpha = alpha * (work->unk_2C - 12) / 12;
        } else {
            phase = dds3TitleMenuPhase + 0.15f;
            dds3TitleMenuPhase = phase;
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

void dds3TitleDrawFade(void)
{
    dds3TitleWork_t *work;
    s32 count;
    s32 age;
    s32 alpha;

    rsetAlpha(1);
    work = titleWork;
    count = work->unk_30;
    work->unk_30 = (s32)((u32)count + SDF_FRAME_DUE());
    if ((s32)((u32)count + 1) >= 0x3C) {
        age = count - 0x3B;
        if (age >= 0x11) {
            age = 0x10;
        }
        alpha = age * 3;
        aDrawBoxG(-0x12C, 0x15, 0x32C, 0xCA,
            0x23, 0x25, 0x56, 0,
            0x23, 0x25, 0x56, 0,
            0x23, 0x25, 0x56, alpha,
            0x23, 0x25, 0x56, 0);
    }

    rsetAlpha(0);
    age = titleWork->unk_30;
    if (age >= 0x32) {
        age -= 0x32;
        if (age > 0x96) {
            age = 0x96;
        }
        alpha = (age * 0x80) / 0x96;
        DrwSprite2D(0xA7, 0x186, 0xB2, 0x18, 2, 0x7C, 0xB2, 0x18,
            ((u32)alpha << 24) | 0x00808080, titleWork->texHandle[2]);
    }

    DrwSprite2D(0x1A, 0x1A4, 0x1C7, 0x14, 0, 0x96, 0x1C7, 0x14,
        0x80808080, titleWork->texHandle[2]);
    rsetAlpha(1);
    age = titleWork->unk_30;
    if (age >= 0x32) {
        age -= 0x32;
        if (age < 0x78) {
            if (age < 0x3F) {
                alpha = (age * 0x80) / 0x3F;
            } else {
                age -= 0x3F;
                alpha = 0x80 - ((age * 0x66) / 0x39);
            }
            if (alpha < 0) alpha = 0;
            if (alpha > 0x80) alpha = 0x80;
            DrwSprite2D(0x4D, 0x111, 0x166, 0x63, 2, 0x15, 0x166, 0x63,
                ((u32)alpha << 24) | 0x00808080, titleWork->texHandle[2]);
        } else {
            age -= 0x78;
            age %= 0x42;
            if (age < 0x13) {
                alpha = 0x19 + ((age * 0x66) / 0x13);
            } else {
                alpha = 0x80 - (((age * 0x66) - 0x792) / 0x2F);
            }
            if (alpha < 0) alpha = 0;
            if (alpha > 0x80) alpha = 0x80;
            DrwSprite2D(0x4D, 0x111, 0x166, 0x63, 2, 0x15, 0x166, 0x63,
                ((u32)alpha << 24) | 0x00808080, titleWork->texHandle[2]);
        }
    }
    age = titleWork->unk_30;
    if (age >= 0x32) {
        age -= 0x32;
        if (age < 0x78) {
            if (age < 0x3F) {
                alpha = (age * 0x66) / 0x3F;
            } else {
                age -= 0x3F;
                alpha = 0x66 - ((age * 0x2C) / 0x39);
            }
            if (alpha < 0) alpha = 0;
            if (alpha > 0x80) alpha = 0x80;
            DrwSprite2D(0xA2, 0x13A, 0xBA, 0x11, 1, 1, 0xBA, 0x11,
                ((u32)alpha << 24) | 0x00808080, titleWork->texHandle[2]);
        } else {
            age -= 0x78;
            age %= 0x42;
            if (age < 0x0D) {
                alpha = 0x39 - ((age * 6) / 0x0D);
            } else if (age < 0x11) {
                alpha = 0x33 + (((age * 0x33) - 0x297) / 4);
            } else {
                alpha = 0x66 - (((age * 0x2C) - 0x2EC) / 0x31);
            }
            DrwSprite2D(0xA2, 0x13A, 0xBA, 0x11, 1, 1, 0xBA, 0x11,
                ((u32)alpha << 24) | 0x00808080, titleWork->texHandle[2]);
        }
    }
    rsetAlpha(0);
}

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

extern void rsetTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm,
                     s32 zte, s32 ztst);
extern void DrwSprite2D(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh,
                        s32 color, sdfTexHandle_t *texture);
extern f32 dds3TitleFadeLevel;
extern void dds3TitleDrawFade(void);

void dds3TitleUpdateFade(void)
{
    s32 alpha;
    s32 glowAlpha;
    s32 frame;
    u32 color;

    if (titleWork->unk_34 == -1) {
        return;
    }
    alpha = 0;
    rsetOtOfs(0x48);
    rsetAlpha(0);
    rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    if (titleWork->unk_3C == 0) {
        frame = titleWork->unk_34;
        if (frame >= 0x7E) {
            frame -= 0x7D;
            if (frame > 30) frame = 30;
            alpha = (s32)((u32)frame << 7) / 30;
        }
    } else {
        frame = titleWork->unk_34;
        if (frame > 12) frame = 12;
        alpha = (s32)((u32)frame << 7) / 12;
    }
    if (alpha > 0) {
        glowAlpha = alpha;
        if (alpha == 0x80) {
            glowAlpha += (s32)(sdfSin(dds3TitleFadeLevel) * 32.0f);
            dds3TitleFadeLevel += SDF_FRAME_STEP(0.04999999702f);
        }
        rsetAlpha(1);
        DrwSprite2D(0x67, 0xCD, 0x132, 0x3C, 1, 0xBD, 0x132, 0x3C,
                    ((u32)glowAlpha << 24) | 0x808080, titleWork->texHandle[1]);
        rsetAlpha(0);
        DrwSprite2D(0x6C, 0xD8, 0x128, 0x27, 0xCC, 0x85, 0x128, 0x27,
                    ((u32)alpha << 24) | 0x808080, titleWork->texHandle[1]);
    } else {
        dds3TitleFadeLevel = 0.0f;
    }
    frame = titleWork->unk_34;
    if (frame > 12) frame = 12;
    color = ((u32)((s32)((u32)frame << 7) / 12) << 24) | 0x808080;
    DrwSprite2D(0xF, 0x54, 0x1D5, 0x7D, 2, 2, 0x1D4, 0x7D,
                color, titleWork->texHandle[1]);
    DrwSprite2D(0x128, 0xAC, 0xC6, 0x34, 2, 0x84, 0xC6, 0x34,
                color, titleWork->texHandle[1]);
    frame = titleWork->unk_34;
    if (frame >= 13) {
        dds3TitleDrawFade();
    }
    titleWork->unk_34 = (s32)((u32)titleWork->unk_34 + SDF_FRAME_DUE());
}


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
    request = pacLoad("/title/titletex.LB");
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

const char dds3TitleProcName[10] = "titleProc";

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
    work->mode = 0;
    work->cursor = 0;
    work->unk_20 = 0;
    work->unk_30 = 0;
    work->unk_38 = 0;
    work->unk_3C = 0;
    work->unk_2C = 0;
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
        {
            dds3TitleWork_t *state = titleWork;
            s32 alpha = 0x80;
            s32 nextMode = 22;
            state->mode = nextMode;
            state->unk_28 = alpha;
            state->unk_30 = 0;
            state->unk_20 = 0;
        }
    }
    dds3BackColor = 0x80000000;
    return dds3StartProcess((char *)dds3TitleProcName, 0x2B19, 0, 0, dds3Titlemain, NULL, NULL);
}

s32 titleFreeWork(s32 arg)
{
    titleFreeTextures();
    sdfMemFreeDirect(titleWork->allocation);
    titleWork = NULL;
    movieStop();
    dds3KernelDebugSign = 1;
    return 1;
}

extern s32 D_0054FD2C, dds3TitleMovieCount, dds3KernelDebugSign;

extern void DrwSprite2D(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh,
                        s32 color, sdfTexHandle_t *texture);
extern void aDrawBoxG(s32 x, s32 y, s32 w, s32 h,
                      u32 r0, u32 g0, u32 b0, u32 a0, u32 r1, u32 g1, u32 b1, u32 a1,
                      u32 r2, u32 g2, u32 b2, u32 a2, u32 r3, u32 g3, u32 b3, u32 a3);
extern s32 p2sdLoadCheckMidi(s32 id);

extern void p2sdPlayBGM(s32 id);
extern void dds3FadeIn(s32 r, s32 g, s32 b, s32 frames);
extern void dds3FadeOut(s32 r, s32 g, s32 b, s32 frames);
extern s32 dds3FadeCheck(void);
extern s32 dds3WaitMovieEnd(void);
extern void dds3TitleClearFade(void);
extern void dds3TitleSetFade(void);
extern void dds3TitleUpdateFade(void);
extern void dds3TitleUpdateCursor(void);
extern void dds3TitleDrawMenu(void);
extern void dds3AdminiCallSequence(s32 sequence, void *data, u32 size, s32 restore);

#define TITLE_BUTTON_ACTIVE() (dds3PadButtonState[0x21] != 0 || dds3PadButtonState[0x23] != 0 || \
    dds3PadButtonState[0x22] != 0 || dds3PadButtonState[0x20] != 0 || dds3PadButtonState[0x2C] != 0)
#define TITLE_BUTTON_PRESSED() (dds3PadButtonState[0x21] < 0 || dds3PadButtonState[0x23] < 0 || \
    dds3PadButtonState[0x22] < 0 || dds3PadButtonState[0x20] < 0 || dds3PadButtonState[0x2C] < 0)

s32 dds3Titlemain(dds3ProcessID_t *id)
{
    s32 counter;
    s32 alpha;
    s32 endFrame;
    u32 initialMode;
    FAKEMATCH_ASM_REG(dds3TitleWork_t *, fadeWork, "$4");
    FAKEMATCH_ASM_REG(s32, fadeCounter, "$3");
    FAKEMATCH_ASM_REG(dds3TitleWork_t *, advanceWork, "$3");
    s32 stageScalar;
    FAKEMATCH_ASM_REG(dds3TitleWork_t *, callbackTimerWork, "$4");
    FAKEMATCH_ASM_REG(s32, callbackPriorTimer, "$3");
    FAKEMATCH_ASM_REG(s32, callbackUpdatedTimer, "$2");

    rsetOtOfs(0x48);
    rsetAlpha(0);
    rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    initialMode = (u32)titleWork->mode;
    switch (initialMode) {
    case 0:
        if (p2sdLoadCheckMidi(0x3A0000) != 0) {
            titleWork->mode = 2;
            titleWork->unk_20 = 0;
        } else {
            p2sdLoadSMGFile(0x3A0000);
            titleWork->unk_20 = 0;
            titleWork->mode = (s32)((u32)titleWork->mode + 1);
        }
        aDrawBoxG(0, 0, 0x200, 0xE0, 0, 0, 0, 0x80, 0, 0, 0, 0x80,
                  0, 0, 0, 0x80, 0, 0, 0, 0x80);
        break;
    case 1:
        if (p2sdLoadCheckMidi(0x3A0000) != 0) {
            advanceWork = titleWork;
            stageScalar = advanceWork->mode;
            goto advance_mode;
        }
        break;
    case 2:
        dds3FadeIn(0, 0, 0, 20);
        titleWork->unk_20 = 0;
        titleWork->mode = (s32)((u32)titleWork->mode + 1);
        dds3KernelDebugSign = 0;
        movieStop();
        break;
    case 3:
        DrwSprite2D(0x5A, 0x90, 0x13C, 0x81, 0x7C, 0, 0x13C, 0x81, 0x80808080, titleWork->texHandle[0]);
        if (dds3FadeCheck() == 0) {
            titleWork->unk_20 = 0;
            titleWork->mode = (s32)((u32)titleWork->mode + 1);
            movieStop();
        }
        break;
    case 4:
        DrwSprite2D(0x5A, 0x90, 0x13C, 0x81, 0x7C, 0, 0x13C, 0x81, 0x80808080, titleWork->texHandle[0]);
        if (TITLE_BUTTON_ACTIVE()) titleWork->unk_20 = 50;
        advanceWork = titleWork;
        counter = advanceWork->unk_20;
        if (counter < 50) {
            s32 updated = (s32)((u32)counter + SDF_FRAME_DUE());
            advanceWork->unk_20 = updated;
        } else {
            stageScalar = advanceWork->mode;
            goto advance_mode;
        }
        break;
    case 5:
        {
        FAKEMATCH_ASM_REG(s32, timer, "$3");
        alpha = (s32)(128U - ((u32)titleWork->unk_20 << 3));
        if (alpha > 0)
            DrwSprite2D(0x5A, 0x90, 0x13C, 0x81, 0x7C, 0, 0x13C, 0x81,
                        ((u32)alpha << 24) | 0x808080, titleWork->texHandle[0]);
        timer = titleWork->unk_20;
        if (timer < 22) {
            stageScalar = (s32)((u32)timer + SDF_FRAME_DUE());
            titleWork->unk_20 = stageScalar;
        }
        else {
            titleWork->unk_20 = 0;
            titleWork->mode = (s32)((u32)titleWork->mode + 1);
        }
        }
        break;
    case 6:
        alpha = (s32)((u32)titleWork->unk_20 * 6U);
        if (alpha > 128) alpha = 128;
        if (alpha > 0)
            DrwSprite2D(0xA4, 0x5E, 0xB8, 0x104, 0, 0, 0x7B, 0xAE,
                        ((u32)alpha << 24) | 0x808080, titleWork->texHandle[0]);
        {
        FAKEMATCH_ASM_REG(s32, sampled, "$3");
        sampled = titleWork->unk_20;
        if (sampled < 20) {
            stageScalar = (s32)((u32)sampled + SDF_FRAME_DUE());
            titleWork->unk_20 = stageScalar;
        }
        else {
            titleWork->unk_20 = 0;
            titleWork->mode = (s32)((u32)titleWork->mode + 1);
        }
        }
        break;
    case 7:
        DrwSprite2D(0xA4, 0x5E, 0xB8, 0x104, 0, 0, 0x7B, 0xAE, 0x80808080, titleWork->texHandle[0]);
        if (TITLE_BUTTON_ACTIVE()) titleWork->unk_20 = 50;
        callbackTimerWork = titleWork;
        callbackPriorTimer = callbackTimerWork->unk_20;
        {
        FAKEMATCH_ASM_REG(s32, timerActive, "$2");
        timerActive = callbackPriorTimer < 50;
        if (timerActive) {
            callbackUpdatedTimer = (s32)((u32)callbackPriorTimer + SDF_FRAME_DUE());
            goto store_callback_timer;
        }
        else {
            dds3FadeOut(0, 0, 0, 20);
            advanceWork = titleWork;
            goto read_advance_mode;
        }
        }
        break;
    case 8:
        if (titleWork->unk_20 < 22) {
            DrwSprite2D(0xA4, 0x5E, 0xB8, 0x104, 0, 0, 0x7B, 0xAE, 0x80808080, titleWork->texHandle[0]);
            titleWork->unk_20 = (s32)((u32)titleWork->unk_20 + SDF_FRAME_DUE());
        } else {
            titleWork->unk_34 = -1;
            dds3FadeIn(0, 0, 0, 30);
            if (dds3TitleMovieCount & 1) {
                dds3PlayMovieByIndex(24);
                titleWork->unk_3C = 1;
            } else {
                dds3PlayMovieByIndex(23);
                titleWork->unk_3C = 0;
            }
            {
                dds3TitleWork_t *work = titleWork;
                s32 count = (s32)((u32)dds3TitleMovieCount + 1);
                s32 mode = (s32)((u32)work->mode + 1);
                dds3TitleMovieCount = count;
                work->mode = mode;
                work->unk_20 = 0;
            }
        }
        break;
    case 9:
        if (movieCb.decodeCb.frame >= 2700) {
            dds3TitleClearFade();
            advanceWork = titleWork;
            goto read_advance_mode;
        } else if (TITLE_BUTTON_ACTIVE()) {
            dds3FadeOut(255, 255, 255, 15);
            titleWork->mode = 23;
            titleWork->unk_20 = 0;
        }
        break;
    case 10:
        dds3TitleUpdateFade();
        endFrame = (s32)((u32)movieCb.decodeCb.frameCount - 20U);
        if (dds3WaitMovieEnd() != 0 || movieCb.decodeCb.frame >= endFrame) {
            dds3FadeOut(0, 0, 0, 15);
            titleWork->mode = 13;
            titleWork->unk_20 = 0;
        } else if (TITLE_BUTTON_ACTIVE()) {
            dds3FadeOut(0, 0, 0, 15);
            p2sdPlaySE(0x3A0001, 127, 63);
            titleWork->mode = 11;
            titleWork->unk_20 = 0;
        }
        break;
    case 12:
        {
            dds3TitleWork_t *work = titleWork;
            FAKEMATCH_ASM_REG(s32, old, "$3");
            FAKEMATCH_ASM_REG(s32, incremented, "$2");
            old = work->unk_20;
            if (old < 4) {
                incremented = (s32)((u32)old + SDF_FRAME_DUE());
                FAKEMATCH_ASM_LIVE(incremented);
                work->unk_20 = incremented;
                break;
            }
        }
        {
            dds3TitleWork_t *work;
            dds3FadeIn2(15);
            dds3PlayMovieByIndex(13);
            work = titleWork;
            work->mode = 18;
            work->unk_24 = 0;
            work->unk_2C = 0;
            work->cursor = 0;
            FAKEMATCH_ASM_LIVE(work);
            work->unk_20 = 0;
        }
        break;
    case 13:
        dds3TitleUpdateFade();
        callbackTimerWork = titleWork;
        callbackPriorTimer = callbackTimerWork->unk_20;
        {
        FAKEMATCH_ASM_REG(s32, timerActive, "$2");
        timerActive = callbackPriorTimer < 15;
        if (timerActive) {
            callbackUpdatedTimer = (s32)((u32)callbackPriorTimer + SDF_FRAME_DUE());
            goto store_callback_timer;
        }
        else if (dds3WaitMovieEnd() != 0) {
            movieStop();
            titleWork->mode = 2;
            titleWork->unk_20 = 0;
        }
        }
        break;
    case 14:
        dds3FadeIn2(15);
        titleWork->unk_28 = 128;
        titleWork->unk_30 = 0;
        titleWork->mode = (s32)((u32)titleWork->mode + 1);
        titleWork->unk_20 = 0;
        dds3TitleSetFade();
        break;
    case 15:
        dds3TitleUpdateFade();
        titleWork->unk_20 = (s32)((u32)titleWork->unk_20 + 1);
        if (dds3FadeCheck() == 0) {
            advanceWork = titleWork;
            goto read_advance_mode;
        }
        break;
    case 16:
        dds3TitleUpdateFade();
        if (movieCb.decodeCb.frameCount == 0) {
            printf("Title Movie frameCount==0\n");
        } else {
            endFrame = (s32)((u32)movieCb.decodeCb.frameCount - 20U);
            if (dds3WaitMovieEnd() != 0 || movieCb.decodeCb.frame >= endFrame) {
                dds3FadeOut(0, 0, 0, 15);
                titleWork->mode = 13;
                titleWork->unk_20 = 0;
                break;
            }
        }
        if (TITLE_BUTTON_PRESSED()) {
            dds3FadeOut(0, 0, 0, 15);
            p2sdPlaySE(0x3A0001, 127, 63);
            titleWork->mode = 17;
            titleWork->unk_20 = 0;
        }
        break;
    case 17:
        dds3TitleUpdateFade();
        if (titleWork->unk_20 == 16) movieStop();
        {
        FAKEMATCH_ASM_REG(s32, sampled, "$3");
        sampled = titleWork->unk_20;
        if (sampled < 18) {
            stageScalar = (s32)((u32)sampled + SDF_FRAME_DUE());
            titleWork->unk_20 = stageScalar;
        }
        else {
            titleWork->unk_20 = 0;
            titleWork->mode = 12;
        }
        }
        break;
    case 18:
        titleWork->unk_2C = (s32)((u32)titleWork->unk_2C + SDF_FRAME_DUE());
        dds3TitleDrawMenu();
        fadeWork = titleWork;
        fadeCounter = fadeWork->unk_20;
        if (fadeCounter < 24)
            goto decay_menu_alpha;
        fadeWork->unk_24 = 0;
        fadeWork->unk_20 = 0;
        fadeWork->mode = (s32)((u32)fadeWork->mode + 1);
        break;
    case 19:
        if (titleWork->unk_24 == 1) {
            titleWork->unk_24 = 0;
            dds3PlayMovieByIndex(13);
        } else if (dds3WaitMovieEnd() != 0) {
            movieStop();
            titleWork->unk_24 = 1;
        }
        dds3TitleUpdateCursor();
        dds3TitleDrawMenu();
        if (dds3PadButtonState[0x23] < 0) {
            p2sdPlaySE(10, 127, 63);
            dds3FadeOut(0, 0, 0, 14);
            titleWork->mode = 25;
            titleWork->unk_20 = 0;
        } else if (dds3PadButtonState[0x21] < 0 || dds3PadButtonState[0x2C] < 0) {
            if (titleWork->cursor == 1) dds3FadeOut(0, 0, 0, 60);
            else dds3FadeOut(0, 0, 0, 15);
            p2sdPlaySE(8, 127, 63);
            advanceWork = titleWork;
            goto read_advance_mode;
        }
        break;
    case 21:
        counter = titleWork->unk_20;
        if (counter < 4) titleWork->unk_20 = (s32)((u32)counter + SDF_FRAME_DUE());
        else {
            switch (titleWork->cursor) {
            case 0:
                D_0054FD2C = -1;
                dds3AdminiCallSequence(3, NULL, 0, 0);
                titleWork->mode = 28;
                break;
            case 1:
                if (counter < 30) titleWork->unk_20 = (s32)((u32)counter + SDF_FRAME_DUE());
                else {
                    dds3FadeIn(0, 0, 0, 10);
                    dds3AdminiCallSequence(41, NULL, 0, 0);
                    titleWork->mode = 28;
                }
                break;
            case 2:
                dds3FadeIn(0, 0, 0, 10);
                p2sdPlayBGM(0x3A0000);
                dds3AdminiCallSequence(17, NULL, 0, 0);
                titleWork->mode = 28;
                break;
            case 3:
                if (counter < 30) titleWork->unk_20 = (s32)((u32)counter + SDF_FRAME_DUE());
                else {
                    titleWork->unk_20 = 0;
                    titleWork->mode = 26;
                    dds3FadeIn(0, 0, 0, 60);
                    dds3PlayMovieByIndex(0);
                }
                break;
            }
        }
        break;
    case 22:
        titleWork->unk_2C = (s32)((u32)titleWork->unk_2C + SDF_FRAME_DUE());
        dds3TitleDrawMenu();
        fadeWork = titleWork;
        fadeCounter = fadeWork->unk_20;
        if (fadeCounter < 24) {
        decay_menu_alpha:
            {
            s32 newAlpha;
            fadeWork->unk_20 = (s32)((u32)fadeCounter + SDF_FRAME_DUE());
            newAlpha = (s32)((u32)fadeWork->unk_28 - 5U * SDF_FRAME_DUE());
            fadeWork->unk_28 = newAlpha;
            if (newAlpha < 0) fadeWork->unk_28 = 0;
            }
        } else {
            fadeWork->unk_20 = 0;
            fadeWork->mode = 19;
            fadeWork->unk_24 = 0;
        }
        break;
    case 11: case 20: case 23:
        if (dds3FadeCheck() == 0) {
            movieStop();
            advanceWork = titleWork;
        read_advance_mode:
            stageScalar = advanceWork->mode;
        advance_mode:
            advanceWork->unk_20 = 0;
            FAKEMATCH_ASM_LIVE_WITH_MEM(stageScalar, advanceWork->unk_20);
            advanceWork->mode = (s32)((u32)stageScalar + 1);
        }
        break;
    case 24:
        {
        FAKEMATCH_ASM_REG(s32, timer, "$2");
        timer = (s32)((u32)titleWork->unk_20 + SDF_FRAME_DUE());
        titleWork->unk_20 = timer;
        if (timer >= 6) {
            dds3PlayMovieByIndex(18);
            titleWork->mode = 14;
            titleWork->unk_20 = 0;
        }
        }
        break;
    case 25:
        dds3TitleDrawMenu();
        callbackTimerWork = titleWork;
        callbackPriorTimer = callbackTimerWork->unk_20;
        if (callbackPriorTimer < 15) {
            callbackUpdatedTimer = (s32)((u32)callbackPriorTimer + SDF_FRAME_DUE());
        store_callback_timer:
            callbackTimerWork->unk_20 = callbackUpdatedTimer;
        }
        else {
            movieStop();
            titleWork->mode = 24;
            titleWork->unk_20 = 0;
        }
        break;
    case 26:
        {
        FAKEMATCH_ASM_REG(s32, timer, "$2");
        timer = titleWork->unk_20;
        if (timer > 0) titleWork->unk_20 = (s32)((u32)timer + SDF_FRAME_DUE());
        else if (dds3WaitMovieEnd() != 0 || (movieCb.decodeCb.frame >= 31 && TITLE_BUTTON_PRESSED())) {
            dds3FadeOut(255, 255, 255, 60);
            titleWork->unk_20 = 1;
        }
        if (titleWork->unk_20 >= 60) {
            movieStop();
            titleWork->mode = 27;
            titleWork->unk_20 = 0;
        }
        }
        break;
    case 27:
        {
        FAKEMATCH_ASM_REG(s32, timer, "$2");
        timer = (s32)((u32)titleWork->unk_20 + SDF_FRAME_DUE());
        titleWork->unk_20 = timer;
        if (timer >= 5) {
            dds3TitleWork_t *w;
            s32 fadeAlpha;
            s32 mode;
            dds3FadeIn(255, 255, 255, 15);
            dds3PlayMovieByIndex(13);
            w = titleWork;
            fadeAlpha = 128;
            mode = 22;
            w->mode = mode;
            w->unk_28 = fadeAlpha;
            w->unk_20 = 0;
            w->unk_30 = 0;
            w->unk_2C = 0;
        }
        }
        break;
    case 28:
        break;
    }
    return 0;
}
#undef TITLE_BUTTON_ACTIVE
#undef TITLE_BUTTON_PRESSED

s32 titleStart(s32 mode)
{
    titleAllocWork(mode);
    return -1;
}

s32 titleExit(void)
{
    titleFreeWork(0);
    dds3KillProcessName((char *)dds3TitleProcName, 1);
    return 0;
}

s32 msel_2286A0(void)
{
    titleAllocWork(0);
    return 0;
}

extern void *frwGetSmallFontFRQ(s32 X, s32 Y, s32 Pri, s32 Color, s32 pStr, s32 pFRQ);

extern void frDrawFontEx(void *pFRQ, s32 Mode);

void mselDrawMenuText(s32 x, s32 y, s32 text, saddr_t arg)
{
    void *frq = frwGetSmallFontFRQ(x * 16, y * 16, 0, text, arg, 0);

    frDrawFontEx(frq, 1);
    frDestroyFontRequestAfter(frq);
}

extern void *frwGetSmallNumFontFRQ(s32 X, s32 Y, s32 Pri, s32 Color, s32 pStr, s32 pFRQ);

void mselDrawSaveNum(s32 x, s32 y, s32 text, saddr_t arg)
{
    void *frq = frwGetSmallNumFontFRQ(x * 16, y * 16, 0, text, arg, 0);

    frDrawFontEx(frq, 1);
    frDestroyFontRequestAfter(frq);
}

void mselUpdateCursor(void)
{
    if ((dds3PadButtonState[0x26] & 2) != 0) {
        gGlobal->cursor--;
        p2sdPlaySE(0, 0x7F, 0x3F);
    }
    if ((dds3PadButtonState[0x27] & 2) != 0) {
        gGlobal->cursor++;
        p2sdPlaySE(0, 0x7F, 0x3F);
    }
    if (gGlobal->cursor < 0) {
        gGlobal->cursor = 1;
    }
    if (gGlobal->cursor >= 2) {
        gGlobal->cursor = 0;
    }
}

extern const char *mselTitleCursor0Lines[];
extern const char *mselTitleCursor1Lines[];

void mselDrawTitleAnim(void)
{
    u32 color;

    eglSetOt(0x47);
    eglLinkTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    eglLinkAlpha(0);
    eglLinkSprite(0, 0, 512, 448, 0, 0, 512, 448,
                  0x80808080, 0x80808080, 0x80808080, 0x80808080,
                  gGlobal->unk_40);
    gGlobal->cur_alphacnt = (gGlobal->cur_alphacnt + 10 * SDF_FRAME_DUE()) % 360;
    gGlobal->cur_alpha = (u32)((sdfSin((f32)((gGlobal->cur_alphacnt + 90) % 360) /
                                     180.0f * 0x1.921f9ep+1f) + 1.0f) *
                              0.5f * 88.0f + 104.0f);
    color = 0x80808000 | (u8)gGlobal->cur_alpha;
    switch (gGlobal->cursor) {
    case 0:
        eglLinkSprite(38, 162, 132, 30, 249, 455, 132, 30,
                      color, color, color, color, gGlobal->unk_40);
        eglLinkSprite(38, 198, 99, 30, 146, 455, 99, 30,
                      0x80808080, 0x80808080, 0x80808080, 0x80808080,
                      gGlobal->unk_40);
        break;
    case 1:
        eglLinkSprite(38, 162, 132, 30, 10, 455, 132, 30,
                      0x80808080, 0x80808080, 0x80808080, 0x80808080,
                      gGlobal->unk_40);
        eglLinkSprite(38, 198, 99, 30, 385, 455, 99, 30,
                      color, color, color, color, gGlobal->unk_40);
        break;
    }
    switch (gGlobal->cursor) {
    case 0:
        mselDrawMenuText(239, 82, 0x64618780, EE_ADDR_S(mselTitleCursor0Lines[0]));
        mselDrawMenuText(239, 94, 0x64618780, EE_ADDR_S(mselTitleCursor0Lines[1]));
        mselDrawMenuText(239, 106, 0x64618780, EE_ADDR_S(mselTitleCursor0Lines[2]));
        return;
    case 1:
        mselDrawMenuText(239, 82, 0x64618780, EE_ADDR_S(mselTitleCursor1Lines[0]));
        mselDrawMenuText(239, 94, 0x64618780, EE_ADDR_S(mselTitleCursor1Lines[1]));
        mselDrawMenuText(239, 106, 0x64618780, EE_ADDR_S(mselTitleCursor1Lines[2]));
        break;
    }
}

void mselDrawBackground(void)
{
    eglSetOt(0x47);
    eglLinkTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    eglLinkAlpha(0);
    eglLinkSprite(0, 0, 0x200, 0x1C0, 0, 0, 0x200, 0x1C0,
                  0x80101010, 0x80101010, 0x80101010, 0x80101010,
                  gGlobal->unk_48);
}

extern void mselToggleFlag(u8 *toggle, s32 *value, s8 dir);
extern void mselMakeSaveText(s32 *devilIds, u32 color, u32 nameColor);

void mselDrawTitleAnim2(s32 animate)
{
    eglSetOt(0x47);
    eglLinkTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    eglLinkAlpha(0);
    eglLinkSprite(0, 0, 0x200, 0x1C0, 0, 0, 0x200, 0x1C0,
                  0x80808080, 0x80808080, 0x80808080, 0x80808080,
                  gGlobal->unk_48);
    mselToggleFlag(&gGlobal->Zalpha_cnt, (s32 *)&gGlobal->Z_RGBA, 0);
    mselToggleFlag((u8 *)&gGlobal->unk_88, &gGlobal->unk_8C, 1);
    if (animate == 0) {
        mselMakeSaveText(gGlobal->tover, gGlobal->Z_RGBA, gGlobal->Z_RGBA);
    } else {
        gGlobal->ZNalpha_cnt = (gGlobal->ZNalpha_cnt + 10 * SDF_FRAME_DUE()) % 360;
        gGlobal->ZNalpha_cnt = gGlobal->ZNalpha_cnt <= 0 ? 0 :
            (gGlobal->ZNalpha_cnt <= 180 ? gGlobal->ZNalpha_cnt : 180);
        gGlobal->ZN_RGBA = (u32)((sdfSin((f32)((gGlobal->ZNalpha_cnt + 90) % 360) /
                                       180.0f * 0x1.921f9ep+1f) + 1.0f) *
                                0.5f * 127.0f + 128.0f);
        mselMakeSaveText(gGlobal->tover, gGlobal->Z_RGBA,
                         (gGlobal->Z_RGBA & 0xFFFFFF00) | gGlobal->ZN_RGBA);
    }
    mselMakeSaveText(gGlobal->unk_94, gGlobal->unk_8C, gGlobal->unk_8C);
}

void mselToggleFlag(u8 *toggle, s32 *value, s8 dir)
{
    s32 v;
    s32 level;

    *toggle = (*toggle == 0);
    if (dir == 0) {
        level = *value;
        v = level;
        level = (v & 0xFF) + 0x10 * SDF_FRAME_DUE();
    } else {
        level = *value;
        v = level;
        level = (v & 0xFF) - 0x10 * SDF_FRAME_DUE();
    }
    level = level <= 0 ? 0 : (level <= 0x80 ? level : 0x80);
    *value = (v & ~0xFF) | level;
}

extern datDevilFormat_t datDevilTable[];

void mselMakeSaveText(s32 *devilIds, u32 color, u32 nameColor)
{
    s32 index;
    s32 devilId;
    s32 level;
    s32 y;
    char text[0x20];
    mselSaveData_t *saveData;

    saveData = (mselSaveData_t *)sdfMemGetPtr(gGlobal->unk_08);
    for (index = 0; index < 5; index++) {
        devilId = devilIds[index];
        y = 0x32 + index * 0x10;
        if (devilId != -1) {
            fclencyceelem_t *record = &saveData->records[devilId];
            mselDrawMenuText(0x64, y, color, EE_ADDR_S(datExplainTable[datDevilTable[record->id].race]));
            mselDrawMenuText(0xA4, y, nameColor, EE_ADDR_S(text2[record->id]));
            sprintf(text, mselLevelLabel);
            mselDrawMenuText(0x140, y, color, EE_ADDR_S(text));
            level = record->level;
            if (level >= 0x64) {
                level = 0x63;
            }
            sprintf(text, mselLevelFormat, level);
            mselDrawSaveNum(0x15A, y + 2, color, EE_ADDR_S(text));
        }
    }
}

const char mselTitleCursor0Line0[32] __attribute__((aligned(16))) = "An average level of difficulty.";

const char mselTitleCursor1Line2[21] = "the thrill of death.";

const char mselTitleCursor1Line1[27] = "suitable for those seeking";

const char mselTitleCursor1Line0[22] = "A level of difficulty";

dds3TitleWork_t *titleWork = NULL;

s32 D_0054FD2C = 0;

f32 dds3TitleMenuPhase = 0.0f;

f32 dds3TitleFadeLevel = 0.0f;

s32 dds3TitleMovieCount = 0;

s32 D_0054FD3C = 0;

u32 D_0054FD40 = 0x0000EA80;

mselGlobalWork_t *gGlobal = NULL;

s32 fldWmapEnabled = 0;

SDATA_PAD(4);

char mselLevelLabel[3] = "Lv";

char mselLevelFormat[4] = "%2d";

char D_0054FD60[3] = "QA";

char D_0054FD68[8] = "Planner";

char D_0054FD70[6] = "Sales";

char D_0054FD78[8] = "Artwork";

char D_0054FD80[6] = "Staff";

char D_0054FD88[2] = " ";
