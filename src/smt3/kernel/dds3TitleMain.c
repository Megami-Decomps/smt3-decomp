#include "common.h"
#include "mviManager.h"
#include "facility.h"
#include "fpu.h"
#include "libsdf.h"
#include "titleGlobal.h"
#include "mselGlobal.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"
#include "kernel.h"

extern void dds3KillProcessName(char *name, s32 shutdowntimer);

extern dds3ProcessID_t *titleAllocWork(s32 arg);

extern s8 dds3PadButtonState[];

extern char D_0053DD98[];

extern void p2sdPlaySE(s32 index, s32 volume, s32 pan);

extern s32 titleFreeWork(s32 arg);



extern mselGlobalWork_t *gGlobal;

extern void *sdfMemGetPtr(sdfMemHandle_t *aHandle);
#include "sdk_libc.h"

extern char D_0054FD50[];
extern char D_0054FD58[];
extern char *datExplainTable[];
extern char *text2[];


extern void frDestroyFontRequestAfter(void *pFRQ);

extern void (*DrawPhaseTable[])(fclEncyc_instance_tag *work);

extern void eglSetOt(u32 ot);

extern void eglLinkTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 zte, s32 ztst);

extern void eglLinkAlpha(s32 mode);

extern void eglLinkSprite(s32 x, s32 y, s32 w, s32 h,
                          s32 u, s32 v, s32 uw, s32 vh,
                          u32 rgba0, u32 rgba1, u32 rgba2, u32 rgba3, sdfTexHandle_t *th);

#ifdef NON_MATCHING
extern dds3TitleWork_t *titleWork;
extern s32 D_0054FD2C, D_0054FD38, dds3KernelDebugSign;
extern char D_0053DDA8[];

extern void rsetOtOfs(s32 offset);
extern void rsetAlpha(s32 mode);
extern void rsetTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 zte, s32 ztst);
extern void DrwSprite2D(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh,
                        s32 color, sdfTexHandle_t *texture);
extern void aDrawBoxG(s32 x, s32 y, s32 w, s32 h,
                      u32 r0, u32 g0, u32 b0, u32 a0, u32 r1, u32 g1, u32 b1, u32 a1,
                      u32 r2, u32 g2, u32 b2, u32 a2, u32 r3, u32 g3, u32 b3, u32 a3);
extern s32 p2sdLoadCheckMidi(s32 id);
extern void p2sdLoadSMGFile(s32 id);
extern s32 p2sdPlayBGM(s32 id);
extern void dds3FadeIn(s32 r, s32 g, s32 b, s32 frames);
extern void dds3FadeOut(s32 r, s32 g, s32 b, s32 frames);
extern void dds3FadeIn2(s32 frames);
extern s32 dds3FadeCheck(void);
extern s32 dds3PlayMovieByIndex(s32 index);
extern s32 dds3WaitMovieEnd(void);
extern void movieStop(void);
extern void dds3TitleClearFade(void);
extern void dds3TitleSetFade(void);
extern void dds3TitleUpdateFade(void);
extern void dds3TitleUpdateCursor(void);
extern void dds3TitleDrawMenu(void);
extern void dds3AdminiCallSequence(s32 sequence, void *data, s32 size, s32 restore);

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
            titleWork->unk_20 = 0;
            titleWork->mode = (s32)((u32)titleWork->mode + 1);
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
        counter = titleWork->unk_20;
        if (counter < 50) titleWork->unk_20 = (s32)((u32)counter + 1);
        else {
            titleWork->unk_20 = 0;
            titleWork->mode = (s32)((u32)titleWork->mode + 1);
        }
        break;
    case 5:
        alpha = (s32)(128U - ((u32)titleWork->unk_20 << 3));
        if (alpha > 0)
            DrwSprite2D(0x5A, 0x90, 0x13C, 0x81, 0x7C, 0, 0x13C, 0x81,
                        ((u32)alpha << 24) | 0x808080, titleWork->texHandle[0]);
        counter = titleWork->unk_20;
        if (counter < 22) titleWork->unk_20 = (s32)((u32)counter + 1);
        else {
            titleWork->unk_20 = 0;
            titleWork->mode = (s32)((u32)titleWork->mode + 1);
        }
        break;
    case 6:
        alpha = (s32)((u32)titleWork->unk_20 * 6U);
        if (alpha > 128) alpha = 128;
        if (alpha > 0)
            DrwSprite2D(0xA4, 0x5E, 0xB8, 0x104, 0, 0, 0x7B, 0xAE,
                        ((u32)alpha << 24) | 0x808080, titleWork->texHandle[0]);
        counter = titleWork->unk_20;
        if (counter < 20) titleWork->unk_20 = (s32)((u32)counter + 1);
        else {
            titleWork->unk_20 = 0;
            titleWork->mode = (s32)((u32)titleWork->mode + 1);
        }
        break;
    case 7:
        DrwSprite2D(0xA4, 0x5E, 0xB8, 0x104, 0, 0, 0x7B, 0xAE, 0x80808080, titleWork->texHandle[0]);
        if (TITLE_BUTTON_ACTIVE()) titleWork->unk_20 = 50;
        counter = titleWork->unk_20;
        if (counter < 50) titleWork->unk_20 = (s32)((u32)counter + 1);
        else {
            dds3FadeOut(0, 0, 0, 20);
            titleWork->unk_20 = 0;
            titleWork->mode = (s32)((u32)titleWork->mode + 1);
        }
        break;
    case 8:
        if (titleWork->unk_20 < 22) {
            DrwSprite2D(0xA4, 0x5E, 0xB8, 0x104, 0, 0, 0x7B, 0xAE, 0x80808080, titleWork->texHandle[0]);
            titleWork->unk_20 = (s32)((u32)titleWork->unk_20 + 1);
        } else {
            titleWork->unk_34 = -1;
            dds3FadeIn(0, 0, 0, 30);
            if (D_0054FD38 & 1) {
                dds3PlayMovieByIndex(24);
                titleWork->unk_3C = 1;
            } else {
                dds3PlayMovieByIndex(23);
                titleWork->unk_3C = 0;
            }
            D_0054FD38 = (s32)((u32)D_0054FD38 + 1);
            titleWork->mode = (s32)((u32)titleWork->mode + 1);
            titleWork->unk_20 = 0;
        }
        break;
    case 9:
        if (movieDecodeWork.frameIndex >= 2700) {
            dds3TitleClearFade();
            titleWork->unk_20 = 0;
            titleWork->mode = (s32)((u32)titleWork->mode + 1);
        } else if (TITLE_BUTTON_ACTIVE()) {
            dds3FadeOut(255, 255, 255, 15);
            titleWork->mode = 23;
            titleWork->unk_20 = 0;
        }
        break;
    case 10:
        dds3TitleUpdateFade();
        endFrame = (s32)((u32)movieDecodeWork.frameCount - 20U);
        if (dds3WaitMovieEnd() != 0 || movieDecodeWork.frameIndex >= endFrame) {
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
    case 11: case 20: case 23:
        if (dds3FadeCheck() == 0) {
            movieStop();
            titleWork->unk_20 = 0;
            titleWork->mode = (s32)((u32)titleWork->mode + 1);
        }
        break;
    case 12:
        counter = titleWork->unk_20;
        if (counter < 4) titleWork->unk_20 = (s32)((u32)counter + 1);
        else {
            dds3FadeIn2(15);
            dds3PlayMovieByIndex(13);
            titleWork->mode = 18;
            titleWork->unk_24 = 0;
            titleWork->unk_2C = 0;
            titleWork->cursor = 0;
            titleWork->unk_20 = 0;
        }
        break;
    case 13:
        dds3TitleUpdateFade();
        counter = titleWork->unk_20;
        if (counter < 15) titleWork->unk_20 = (s32)((u32)counter + 1);
        else if (dds3WaitMovieEnd() != 0) {
            movieStop();
            titleWork->mode = 2;
            titleWork->unk_20 = 0;
        }
        break;
    case 14:
        dds3FadeIn2(15);
        titleWork->unk_28 = 128;
        titleWork->unk_20 = 0;
        titleWork->mode = (s32)((u32)titleWork->mode + 1);
        titleWork->unk_30 = 0;
        dds3TitleSetFade();
        break;
    case 15:
        dds3TitleUpdateFade();
        titleWork->unk_20 = (s32)((u32)titleWork->unk_20 + 1);
        if (dds3FadeCheck() == 0) {
            titleWork->unk_20 = 0;
            titleWork->mode = (s32)((u32)titleWork->mode + 1);
        }
        break;
    case 16:
        dds3TitleUpdateFade();
        if (movieDecodeWork.frameCount == 0) {
            printf(D_0053DDA8);
        } else {
            endFrame = (s32)((u32)movieDecodeWork.frameCount - 20U);
            if (dds3WaitMovieEnd() != 0 || movieDecodeWork.frameIndex >= endFrame) {
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
        counter = titleWork->unk_20;
        if (counter < 18) titleWork->unk_20 = (s32)((u32)counter + 1);
        else {
            titleWork->unk_20 = 0;
            titleWork->mode = 12;
        }
        break;
    case 18: case 22:
        titleWork->unk_2C = (s32)((u32)titleWork->unk_2C + 1);
        dds3TitleDrawMenu();
        counter = titleWork->unk_20;
        if (counter < 24) {
            titleWork->unk_20 = (s32)((u32)counter + 1);
            alpha = (s32)((u32)titleWork->unk_28 - 5U);
            titleWork->unk_28 = alpha;
            if (alpha < 0) titleWork->unk_28 = 0;
        } else {
            titleWork->unk_24 = 0;
            if (initialMode == 18) {
                titleWork->unk_20 = 0;
                titleWork->mode = (s32)((u32)titleWork->mode + 1);
            } else {
                titleWork->mode = 19;
                titleWork->unk_20 = 0;
            }
        }
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
            titleWork->unk_20 = 0;
            titleWork->mode = (s32)((u32)titleWork->mode + 1);
        }
        break;
    case 21:
        counter = titleWork->unk_20;
        if (counter < 4) titleWork->unk_20 = (s32)((u32)counter + 1);
        else {
            switch (titleWork->cursor) {
            case 0:
                D_0054FD2C = -1;
                dds3AdminiCallSequence(3, NULL, 0, 0);
                titleWork->mode = 28;
                break;
            case 1:
                if (counter < 30) titleWork->unk_20 = (s32)((u32)counter + 1);
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
                if (counter < 30) titleWork->unk_20 = (s32)((u32)counter + 1);
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
    case 24:
        counter = (s32)((u32)titleWork->unk_20 + 1);
        titleWork->unk_20 = counter;
        if (counter >= 6) {
            dds3PlayMovieByIndex(18);
            titleWork->mode = 14;
            titleWork->unk_20 = 0;
        }
        break;
    case 25:
        dds3TitleDrawMenu();
        counter = titleWork->unk_20;
        if (counter < 15) titleWork->unk_20 = (s32)((u32)counter + 1);
        else {
            movieStop();
            titleWork->mode = 24;
            titleWork->unk_20 = 0;
        }
        break;
    case 26:
        counter = titleWork->unk_20;
        if (counter > 0) titleWork->unk_20 = (s32)((u32)counter + 1);
        else if (dds3WaitMovieEnd() != 0 || (movieDecodeWork.frameIndex >= 31 && TITLE_BUTTON_PRESSED())) {
            dds3FadeOut(255, 255, 255, 60);
            titleWork->unk_20 = 1;
        }
        if (titleWork->unk_20 >= 60) {
            movieStop();
            titleWork->mode = 27;
            titleWork->unk_20 = 0;
        }
        break;
    case 27:
        counter = (s32)((u32)titleWork->unk_20 + 1);
        titleWork->unk_20 = counter;
        if (counter >= 5) {
            dds3FadeIn(255, 255, 255, 15);
            dds3PlayMovieByIndex(13);
            titleWork->mode = 22;
            titleWork->unk_28 = 128;
            titleWork->unk_2C = 0;
            titleWork->unk_20 = 0;
            titleWork->unk_30 = 0;
        }
        break;
    }
    return 0;
}
#undef TITLE_BUTTON_ACTIVE
#undef TITLE_BUTTON_PRESSED
#else
INCLUDE_ASM(const s32, "kernel/dds3TitleMain", dds3Titlemain);
#endif

s32 titleStart(s32 mode)
{
    titleAllocWork(mode);
    return -1;
}

s32 titleExit(void)
{
    titleFreeWork(0);
    dds3KillProcessName(D_0053DD98, 1);
    return 0;
}

s32 msel_2286A0(void)
{
    titleAllocWork(0);
    return 0;
}

extern void *frwGetSmallFontFRQ(s32 X, s32 Y, s32 Pri, s32 Color, s32 pStr, s32 pFRQ);

extern void frDrawFontEx(void *pFRQ, s32 Mode);

void mselDrawMenuText(s32 x, s32 y, s32 text, s32 arg)
{
    void *frq = frwGetSmallFontFRQ(x * 16, y * 16, 0, text, arg, 0);

    frDrawFontEx(frq, 1);
    frDestroyFontRequestAfter(frq);
}

extern void *frwGetSmallNumFontFRQ(s32 X, s32 Y, s32 Pri, s32 Color, s32 pStr, s32 pFRQ);

void mselDrawSaveNum(s32 x, s32 y, s32 text, s32 arg)
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

extern f32 sdfSin(f32 angle);
extern char *D_00342FF8[];
extern char *D_00343008[];

void mselDrawTitleAnim(void)
{
    u32 color;

    eglSetOt(0x47);
    eglLinkTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    eglLinkAlpha(0);
    eglLinkSprite(0, 0, 512, 448, 0, 0, 512, 448,
                  0x80808080, 0x80808080, 0x80808080, 0x80808080,
                  gGlobal->unk_40);
    gGlobal->cur_alphacnt = (gGlobal->cur_alphacnt + 10) % 360;
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
        mselDrawMenuText(239, 82, 0x64618780, (s32)D_00342FF8[0]);
        mselDrawMenuText(239, 94, 0x64618780, (s32)D_00342FF8[1]);
        mselDrawMenuText(239, 106, 0x64618780, (s32)D_00342FF8[2]);
        return;
    case 1:
        mselDrawMenuText(239, 82, 0x64618780, (s32)D_00343008[0]);
        mselDrawMenuText(239, 94, 0x64618780, (s32)D_00343008[1]);
        mselDrawMenuText(239, 106, 0x64618780, (s32)D_00343008[2]);
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

extern f32 sdfSin(f32 angle);
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
        gGlobal->ZNalpha_cnt = (gGlobal->ZNalpha_cnt + 10) % 360;
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
        level = (v & 0xFF) + 0x10;
    } else {
        level = *value;
        v = level;
        level = (v & 0xFF) - 0x10;
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
            mselDrawMenuText(0x64, y, color, (s32)datExplainTable[datDevilTable[record->id].race]);
            mselDrawMenuText(0xA4, y, nameColor, (s32)text2[record->id]);
            sprintf(text, D_0054FD50);
            mselDrawMenuText(0x140, y, color, (s32)text);
            level = record->level;
            if (level >= 0x64) {
                level = 0x63;
            }
            sprintf(text, D_0054FD58, level);
            mselDrawSaveNum(0x15A, y + 2, color, (s32)text);
        }
    }
}

INCLUDE_RODATA(const s32, "kernel/dds3TitleMain", D_0053DE50);

INCLUDE_RODATA(const s32, "kernel/dds3TitleMain", D_0053DE70);

INCLUDE_RODATA(const s32, "kernel/dds3TitleMain", D_0053DE88);

INCLUDE_RODATA(const s32, "kernel/dds3TitleMain", D_0053DEA8);

INCLUDE_SDATA(const s32, "kernel/dds3TitleMain", D_0054FD38);

INCLUDE_SDATA(const s32, "kernel/dds3TitleMain", D_0054FD40);

INCLUDE_SDATA(const s32, "kernel/dds3TitleMain", gGlobal);

INCLUDE_SDATA(const s32, "kernel/dds3TitleMain", fldWmapEnabled);

INCLUDE_SDATA(const s32, "kernel/dds3TitleMain", D_0054FD4C);

INCLUDE_SDATA(const s32, "kernel/dds3TitleMain", D_0054FD50);

INCLUDE_SDATA(const s32, "kernel/dds3TitleMain", D_0054FD58);

INCLUDE_SDATA(const s32, "kernel/dds3TitleMain", D_0054FD60);

INCLUDE_SDATA(const s32, "kernel/dds3TitleMain", D_0054FD68);

INCLUDE_SDATA(const s32, "kernel/dds3TitleMain", D_0054FD70);

INCLUDE_SDATA(const s32, "kernel/dds3TitleMain", D_0054FD78);

INCLUDE_SDATA(const s32, "kernel/dds3TitleMain", D_0054FD80);

INCLUDE_SDATA(const s32, "kernel/dds3TitleMain", D_0054FD88);

