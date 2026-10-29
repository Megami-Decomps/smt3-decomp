#include "common.h"
#include "newdata.h"
#include "kernel.h"
#include "libsdf.h"

extern f32 gMoonCnt;

extern dds3ProcessID_t *moonprocess_id;
extern char D_0054F780[]; /* "moon" */
extern char D_0053B1F8[]; /* "/fld/f/bin/moon.LB" */
extern dds3ProcessID_t *dds3StartProcess(char *name, u32 priority, s32 startTimer,
                                         s32 shutdownTimer, dds3ProcessFunc_t process,
                                         dds3ProcessFunc_t shutdown, void *data);
extern void *dds3GetProcessData(dds3ProcessID_t *id);
extern void dds3SetProcessData(dds3ProcessID_t *id, void *data);
extern void *sdfChipAllocDirect(s32 size);
extern void sdfChipFreeDirect(void *ptr);
extern void sdfTexFreeDelayed(sdfTexHandle_t *handle);
extern void *pacLoad(const char *filename);
extern void pacSync(void *handle);
extern s32 pacFileHandleFree_(void *handle);

typedef struct
{
    sdfTexHandle_t *MoonTmx1;
    sdfTexHandle_t *MoonTmx2;
    sdfTexHandle_t *MoonTmx3;
    sdfTexHandle_t *MoonTmx4;
    sdfTexHandle_t *MoonTmx5;
    sdfTexHandle_t *MoonTmx6;
} evtMoonProcessData_t;

typedef struct evtMoonPacRequest_t
{
    u8 kind;
    u8 state;
    u8 retryCount;
    u8 mode;
    struct evtMoonPacRequest_t *next;
    char *name;
    void *deviceRequest;
    s32 transferBytes;
    s32 totalBytes;
    void *callback;
    void *callbackArg;
    sdfMemHandle_t *allocationHandle;
    u32 retainedAddress;
    u32 transferAddress;
    struct evtMoonPacRequest_t *completionNext;
    sdfPacDecodeCb_t packet;
} evtMoonPacRequest_t;

s32 evtMoonProcess1(dds3ProcessID_t *id);
extern s32 evtMoonProcess2(dds3ProcessID_t *id);
void evtMoonShutdown(dds3ProcessID_t *id);

extern sdfTexHandle_t *DeadMoonTmx;
extern s32 evtDeadMoonDraw_total_cnt;
extern s32 evtDeadMoonDraw_a_cnt;
extern void rsetOtOfs(s32 otofs);
extern void rsetAlpha(s32 mode);
extern void rsetTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 zte, s32 ztst);
extern void DrwSprite2D(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh, u32 rgba, sdfTexHandle_t *th);
extern void evtDrawDeadMoonSprite(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh,
                                  s32 rgb, sdfTexHandle_t *th, s32 ox, s32 oy,
                                  f32 cnt, f32 alpha0, f32 alpha1, f32 scale0, f32 scale1);

void evtDeadMoonDraw(evtMoonProcessData_t *data)
{
    s32 ox;
    s32 oy;
    s32 total_cnt;
    f32 alpha;

    if (DeadMoonTmx == NULL) {
        return;
    }
    ox = (gMoonCnt * 0.0f) + ((1.0f - gMoonCnt) * -250.0f);
    oy = (gMoonCnt * 0.0f) + ((1.0f - gMoonCnt) * 0.0f);

    if ((DDS3_GBWK->moonFlags & 2) != 0) {

        rsetOtOfs(0x47);
        rsetAlpha(0);
        rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
        DrwSprite2D(ox + 12, oy + 14, 108, 54, 1, 1, 108, 54, 0x80808080, DeadMoonTmx);
        rsetAlpha(1);

        if (evtDeadMoonDraw_a_cnt <= 23) {
            alpha = 80.0f - (((f32)evtDeadMoonDraw_a_cnt * 60.0f) / 24.0f);
        } else {
            alpha = ((f32)(evtDeadMoonDraw_a_cnt - 24) * 60.0f) / 24.0f + 20.0f;
        }
        if (alpha > 100.0f) alpha = 100.0f;
        if (alpha < 0.0f) alpha = 0.0f;
        evtDrawDeadMoonSprite(ox + 36, oy + 16, 33, 31, 59, 21, 33, 31, 0x808080,
                              data->MoonTmx3, ox, oy, 0.0f, alpha, alpha, 100.0f, 100.0f);

        if (evtDeadMoonDraw_a_cnt <= 23) {
            alpha = ((f32)evtDeadMoonDraw_a_cnt * 60.0f) / 24.0f;
        } else {
            alpha = 60.0f - (((f32)(evtDeadMoonDraw_a_cnt - 24) * 60.0f) / 24.0f);
        }
        if (alpha > 100.0f) alpha = 100.0f;
        if (alpha < 0.0f) alpha = 0.0f;
        evtDrawDeadMoonSprite(ox + 38, oy + 16, 35, 31, 93, 21, 35, 31, 0x808080,
                              data->MoonTmx3, ox, oy, 0.0f, alpha, alpha, 100.0f, 100.0f);

        total_cnt = evtDeadMoonDraw_total_cnt;
        if (total_cnt < 18) {
            alpha = 0.0f;
        } else {
            if (total_cnt < 48) {
                alpha = ((f32)(total_cnt - 18) * 80.0f) / 30.0f;
            } else {
                alpha = ((f32)(74 - total_cnt) * 80.0f) / 30.0f;
            }
        }
        if (alpha > 100.0f) alpha = 100.0f;
        if (alpha < 0.0f) alpha = 0.0f;
        evtDrawDeadMoonSprite(ox + 38, oy + 16, 37, 29, 110, 1, 17, 14, 0x808080,
                              DeadMoonTmx, ox, oy, 0.0f, alpha, alpha, 100.0f, 100.0f);
        rsetAlpha(0);
        evtDeadMoonDraw_total_cnt = (evtDeadMoonDraw_total_cnt + 1) % 74;
        evtDeadMoonDraw_a_cnt = (evtDeadMoonDraw_a_cnt + 1) % 48;
    }
}

/* Fold phases 9-15 back toward zero for the symmetric solar animation. */
s32 evtGetAgeOfMoon(void)
{
    s32 phase = DDS3_GBWK->moonPhase;
    if (phase >= 9) {
        phase = 8 - (phase & 7);
    }
    return phase;
}

s32 evtGetAgeOfMoon16(void)
{
    return DDS3_GBWK->moonPhase;
}

void evtSetAgeOfMoon(u8 age)
{
    DDS3_GBWK->moonPhase = age & 0xF;
    DDS3_GBWK->moonTimer = 0.0f;
}

void evtEnableMoonCalc(void)
{
    DDS3_GBWK->moonFlags |= 1;
}

void evtDisableMoonCalc(void)
{
    DDS3_GBWK->moonFlags &= ~1;
}

void evtEnableMoonDraw(void)
{
    DDS3_GBWK->moonFlags |= 2;
    gMoonCnt = 1.0f;
}

void evtDisableMoonDraw(void)
{
    DDS3_GBWK->moonFlags &= ~2;
    gMoonCnt = 0.0f;
}

void evtEnableMoonDraw2(void)
{
    DDS3_GBWK->moonFlags |= 2;
}

void evtDisableMoonDraw2(void)
{
    DDS3_GBWK->moonFlags &= ~2;
}

extern f32 D_0054E3B4, D_0054E3B8, D_0054E3BC, D_0054E3C0, D_0054E3C4;
extern u32 dds3NullDrawProc(void);
extern s32 fmProcessChk(void);
extern s32 fldChkVortex(void);
extern f32 D_0054F770;
extern f32 evtMoonTimer, evtMoonPhase, evtMoonState;
extern s32 evtMoonMove;

/* NON_MATCHING: 3984/4508 bytes; 13 of 996 emitted words match. */
#ifdef NON_MATCHING
s32 evtMoonProcess2(dds3ProcessID_t *id) {
    s32 glyphUW;
    s32 overlayUW;
    s32 glowUW;
    s32 glyphVH;
    s32 overlayVH;
    s32 glowVH;
    s32 overlayX;
    s32 overlayY;
    s32 glyphW;
    s32 overlayW;
    s32 glyphH;
    s32 overlayH;
    s32 glyphU;
    s32 splitU;
    s32 overlayU;
    s32 glowU;
    s32 glyphV;
    f32 nextShown;
    f32 nextHidden;
    f32 zeroTerm;
    f32 oneMinusShown;
    f32 moveTimer;
    f32 glowAngle;
    f32 pulseAlpha0;
    f32 glowAlpha0;
    f32 pulseAlpha1;
    f32 overlayAlpha;
    f32 glowAlpha1;
    f32 glowScale0;
    f32 glowScale1;
    f32 moveStrength;
    f32 moveScale;
    f32 moveAlpha;
    u32 packedAlpha;
    s32 offsetX;
    s32 offsetY;
    s32 iconX;
    s32 iconY;
    s32 firstAlpha;
    s32 baseAlpha;
    s32 secondAlpha;
    u32 spriteRgb;
    sdfTexHandle_t *glyphTexture;
    sdfTexHandle_t *overlayTexture;
    sdfTexHandle_t *glowTexture;
    s32 glyphX;
    s32 splitX;
    s32 glyphY;
    s32 splitY;
    u8 glyphPhase;
    u8 pulsePhase;
    u8 alphaPhase;
    u8 movePhase;
    u8 overlayPhase;
    u8 glowPhase;
    u8 finalPhase;
    evtMoonProcessData_t *data;

    moveScale = 0.0f;
    firstAlpha = 0;
    baseAlpha = 0;
    secondAlpha = 0;
    moveAlpha = 0.0f;
    if (fmProcessChk() != 0) return 0;
    if (fldChkVortex() == 0) return 0;
    if (dds3NullDrawProc() != 0) return 0;
    data = dds3GetProcessData(id);
    if (DDS3_GBWK->moonFlags & 2) {
        if (gMoonCnt < 1.0f) {
            nextShown = gMoonCnt + D_0054E3B4;
            if (!(nextShown > 1.0f)) {
                gMoonCnt = nextShown;
            } else {
                gMoonCnt = 1.0f;
            }
        }
    } else if (gMoonCnt > 0.0f) {
        nextHidden = gMoonCnt - D_0054E3B8;
        if (!(nextHidden < 0.0f)) {
            gMoonCnt = nextHidden;
        } else {
            gMoonCnt = 0.0f;
        }
    }
    if (DeadMoonTmx != 0) {
        evtDeadMoonDraw(data);
    } else {
        oneMinusShown = 1.0f - gMoonCnt;
        zeroTerm = gMoonCnt * 0.0f;
        offsetY = (s32) (zeroTerm + (oneMinusShown * 0.0f));
        offsetX = (s32) (zeroTerm + (oneMinusShown * -250.0f));
        if (DDS3_GBWK->moonFlags & 2) {
            iconY = offsetY + 0x15;
            rsetOtOfs(0x47);
            rsetAlpha(0);
            rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
            DrwSprite2D(offsetX + 0x10, iconY, 0x61, 0x2B, 1, 1, 0x61, 0x2B, 0x80808080, data->MoonTmx1);
            glyphPhase = DDS3_GBWK->moonPhase;
            switch (glyphPhase) {           /* switch 1 */
            case 0:                     /* switch 1 */
                glyphX = offsetX + 0x3C;
                glyphY = offsetY + 0x23;
                glyphW = 0x40;
                glyphH = 0x12;
                glyphU = 0x32;
                glyphTexture = data->MoonTmx4;
                glyphV = 0x27;
                glyphUW = 0x40;
                glyphVH = 0x12;
drawGlyph:
                DrwSprite2D(glyphX, glyphY, glyphW, glyphH, glyphU, glyphV, glyphUW, glyphVH, 0x80808080, glyphTexture);
                break;
            case 1:                     /* switch 1 */
            case 15:                    /* switch 1 */
                splitX = offsetX + 0x3D;
                splitY = offsetY + 0x22;
                splitU = 1;
drawSplitGlyph:
                DrwSprite2D(splitX, splitY, 0xE, 0x13, splitU, 1, 0xE, 0x13, 0x80808080, data->MoonTmx3);
                glyphX = offsetX + 0x4A;
                glyphY = offsetY + 0x27;
                glyphW = 0x15;
                glyphTexture = data->MoonTmx3;
                glyphH = 0x13;
                glyphU = 0x6A;
                glyphV = 1;
                glyphUW = 0x15;
                glyphVH = 0x13;
                goto drawGlyph;
            case 2:                     /* switch 1 */
            case 14:                    /* switch 1 */
                splitX = offsetX + 0x3D;
                splitY = offsetY + 0x22;
                splitU = 0x10;
                goto drawSplitGlyph;
            case 3:                     /* switch 1 */
            case 13:                    /* switch 1 */
                splitX = offsetX + 0x3D;
                splitY = offsetY + 0x22;
                splitU = 0x1F;
                goto drawSplitGlyph;
            case 4:                     /* switch 1 */
            case 12:                    /* switch 1 */
                glyphX = offsetX + 0x3C;
                glyphY = offsetY + 0x23;
                glyphW = 0x2F;
                glyphH = 0x12;
                glyphU = 2;
                glyphTexture = data->MoonTmx5;
                glyphV = 0x2D;
                glyphUW = 0x2F;
                glyphVH = 0x12;
                goto drawGlyph;
            case 5:                     /* switch 1 */
            case 11:                    /* switch 1 */
                splitX = offsetX + 0x3D;
                splitY = offsetY + 0x22;
                splitU = 0x3D;
                goto drawSplitGlyph;
            case 6:                     /* switch 1 */
            case 10:                    /* switch 1 */
                splitX = offsetX + 0x3D;
                splitY = offsetY + 0x22;
                splitU = 0x4C;
                goto drawSplitGlyph;
            case 7:                     /* switch 1 */
            case 9:                     /* switch 1 */
                splitX = offsetX + 0x3D;
                splitY = offsetY + 0x22;
                splitU = 0x5B;
                goto drawSplitGlyph;
            case 8:                     /* switch 1 */
                DrwSprite2D(offsetX + 0x3C, offsetY + 0x23, 0x2F, 0x12, 2, 0x27, 0x2F, 0x12, 0x80808080, data->MoonTmx4);
                break;
            }
            rsetAlpha(1);
            pulsePhase = DDS3_GBWK->moonPhase;
            switch (pulsePhase) {         /* switch 2 */
            case 5:                     /* switch 2 */
            case 11:                    /* switch 2 */
                pulseAlpha0 = 20.0f;
                pulseAlpha1 = 10.0f;
drawPulse:
                evtDrawDeadMoonSprite(0xF, 4, 0x3A, 0x3E, 0x3E, 1, 0x3A, 0x3E, 0x808080, data->MoonTmx2, offsetX, offsetY, evtMoonTimer, pulseAlpha0, pulseAlpha1, 100.0f, 100.0f);
                break;
            case 6:                     /* switch 2 */
            case 10:                    /* switch 2 */
                pulseAlpha0 = 40.0f;
pulseAlpha20:
                pulseAlpha1 = 20.0f;
                goto drawPulse;
            case 7:                     /* switch 2 */
            case 9:                     /* switch 2 */
                pulseAlpha0 = 50.0f;
                goto pulseAlpha20;
            case 8:                     /* switch 2 */
                evtDrawDeadMoonSprite(0xF, 4, 0x3A, 0x3E, 0x3E, 1, 0x3A, 0x3E, 0x808080, data->MoonTmx2, offsetX, offsetY, evtMoonTimer, 30.0f, 100.0f, 100.0f, 100.0f);
                break;
            }
            rsetAlpha(0);
            alphaPhase = DDS3_GBWK->moonPhase;
            switch (alphaPhase) {         /* switch 3 */
            case 0:                     /* switch 3 */
            case 1:                     /* switch 3 */
            case 15:                    /* switch 3 */
                firstAlpha = 0;
                secondAlpha = 0;
                baseAlpha = 0x80;
                break;
            case 2:                     /* switch 3 */
            case 14:                    /* switch 3 */
                firstAlpha = 0;
                secondAlpha = 0x80;
                baseAlpha = 0x46;
                break;
            case 3:                     /* switch 3 */
            case 13:                    /* switch 3 */
                firstAlpha = 0;
                secondAlpha = 0x73;
                baseAlpha = 0x26;
                break;
            case 4:                     /* switch 3 */
            case 12:                    /* switch 3 */
                firstAlpha = 0;
                secondAlpha = 0x80;
baseAlphaZero:
                baseAlpha = 0;
                break;
            case 5:                     /* switch 3 */
            case 11:                    /* switch 3 */
                firstAlpha = 0x2C;
                secondAlpha = 0x53;
                goto baseAlphaZero;
            case 6:                     /* switch 3 */
            case 10:                    /* switch 3 */
                firstAlpha = 0x40;
                secondAlpha = 0x40;
                goto baseAlphaZero;
            case 7:                     /* switch 3 */
            case 9:                     /* switch 3 */
                firstAlpha = 0x53;
                secondAlpha = 0x2C;
                goto baseAlphaZero;
            case 8:                     /* switch 3 */
                firstAlpha = 0x80;
                secondAlpha = 0;
                goto baseAlphaZero;
            }
            iconX = offsetX + 0x1E;
            DrwSprite2D(iconX, iconY, 0x1C, 0x1C, 0x1E, 0x15, 0x1C, 0x1C, ((u32)firstAlpha << 0x18) | 0x808080, data->MoonTmx3);
            DrwSprite2D(iconX, iconY, 0x1C, 0x1C, 0x63, 0x1E, 0x1C, 0x1C, ((u32)secondAlpha << 0x18) | 0x808080, data->MoonTmx1);
            rsetAlpha(1);
            overlayPhase = DDS3_GBWK->moonPhase;
            switch (overlayPhase) {
            case 5:
            case 11:
                evtDrawDeadMoonSprite(0x1D, 0x14, 0x1F, 0x20, 0x5A, 1,
                                      0x1F, 0x20, 0x808080, data->MoonTmx5,
                                      offsetX, offsetY, evtMoonState,
                                      35.0f, 100.0f, 100.0f, 100.0f);
                overlayX = 0x1D;
                overlayY = 0x14;
                overlayW = 0x1F;
                overlayH = 0x20;
                overlayU = 0x52;
                overlayUW = 0x1F;
                overlayVH = 0x20;
                overlayTexture = data->MoonTmx4;
                overlayAlpha = 35.0f;
                goto drawSecondOverlay;
            case 6:
            case 10:
                evtDrawDeadMoonSprite(0x1A, 0x12, 0x27, 0x24, 0x29, 1,
                                      0x27, 0x24, 0x808080, data->MoonTmx4,
                                      offsetX, offsetY, evtMoonState,
                                      35.0f, 100.0f, 100.0f, 100.0f);
                overlayX = 0x1A;
                overlayY = 0x12;
                overlayW = 0x27;
                overlayH = 0x24;
                overlayU = 1;
                overlayUW = 0x27;
                overlayVH = 0x24;
                overlayTexture = data->MoonTmx4;
                overlayAlpha = 35.0f;
                goto drawSecondOverlay;
            case 7:
            case 9:
                evtDrawDeadMoonSprite(0x18, 0xF, 0x2B, 0x2A, 0x2D, 1,
                                      0x2B, 0x2A, 0x808080, data->MoonTmx5,
                                      offsetX, offsetY, evtMoonState,
                                      40.0f, 100.0f, 100.0f, 100.0f);
                overlayX = 0x18;
                overlayY = 0xF;
                overlayW = 0x2B;
                overlayH = 0x2A;
                overlayU = 1;
                overlayUW = 0x2B;
                overlayVH = 0x2A;
                overlayTexture = data->MoonTmx5;
                overlayAlpha = 40.0f;
                goto drawSecondOverlay;
            case 8:
                evtDrawDeadMoonSprite(0x12, 0xA, 0x36, 0x33, 0x39, 1,
                                      0x36, 0x33, 0x808080, data->MoonTmx6,
                                      offsetX, offsetY, evtMoonState,
                                      35.0f, 100.0f, 100.0f, 100.0f);
                overlayX = 0x12;
                overlayY = 0xA;
                overlayW = 0x36;
                overlayH = 0x33;
                overlayU = 1;
                overlayUW = 0x36;
                overlayVH = 0x33;
                overlayTexture = data->MoonTmx6;
                overlayAlpha = 35.0f;
                goto drawSecondOverlay;
            }
            goto block_73;
drawSecondOverlay:
            evtDrawDeadMoonSprite(overlayX, overlayY, overlayW, overlayH, overlayU, 1,
                                  overlayUW, overlayVH, 0x808080, overlayTexture,
                                  offsetX, offsetY, evtMoonState,
                                  100.0f, overlayAlpha, 100.0f, 100.0f);
block_73:
            glowPhase = DDS3_GBWK->moonPhase;
            switch (glowPhase) {
            case 0:
                spriteRgb = 0x4A40FF;
                glowAlpha0 = 70.0f;
                glowAngle = evtMoonPhase;
                glowScale1 = 100.0f;
                glowTexture = data->MoonTmx1;
                glowU = 0x63;
                glowUW = 0x1C;
                glowVH = 0x1C;
                glowAlpha1 = 70.0f;
                glowScale0 = 70.0f;
                goto drawPhaseGlow;
            case 1:
            case 15:
                spriteRgb = 0x6060B0;
                glowAlpha0 = 75.0f;
                glowAngle = evtMoonPhase;
                glowScale0 = 60.0f;
                glowTexture = data->MoonTmx1;
                glowScale1 = 90.0f;
                glowU = 0x63;
                glowUW = 0x1C;
                glowVH = 0x1C;
                glowAlpha1 = 75.0f;
                goto drawPhaseGlow;
            case 2:
            case 14:
                spriteRgb = 0x7070A0;
                glowAlpha0 = 75.0f;
                glowAngle = evtMoonPhase;
                glowScale0 = 60.0f;
                glowTexture = data->MoonTmx1;
                glowScale1 = 90.0f;
                glowU = 0x63;
                glowUW = 0x1C;
                glowVH = 0x1C;
                glowAlpha1 = 75.0f;
                goto drawPhaseGlow;
            case 3:
            case 13:
                spriteRgb = 0x808090;
                glowAlpha0 = 75.0f;
                glowAngle = evtMoonPhase;
                glowScale0 = 70.0f;
                glowTexture = data->MoonTmx1;
                glowScale1 = 90.0f;
                glowU = 0x63;
                glowUW = 0x1C;
                glowVH = 0x1C;
                glowAlpha1 = 75.0f;
                goto drawPhaseGlow;
            case 4:
            case 12:
                spriteRgb = 0x808080;
                evtDrawDeadMoonSprite(0xF, 4, 0x3A, 0x3E, 0x63, 1, 0x1C, 0x1C,
                                      0x808080, data->MoonTmx1, offsetX, offsetY,
                                      evtMoonPhase, 70.0f, 70.0f, 70.0f, 90.0f);
                glowAlpha0 = 20.0f;
                glowTexture = data->MoonTmx2;
                glowAngle = D_0054F770;
                glowScale0 = 60.0f;
                glowU = 1;
                glowScale1 = 75.0f;
                glowUW = 0x3A;
                glowVH = 0x3E;
                glowAlpha1 = 20.0f;
                goto drawPhaseGlow;
            case 5:
            case 11:
                spriteRgb = 0x808080;
                evtDrawDeadMoonSprite(0xF, 4, 0x3A, 0x3E, 0x63, 1, 0x1C, 0x1C,
                                      0x808080, data->MoonTmx1, offsetX, offsetY,
                                      evtMoonPhase, 55.0f, 65.0f, 75.0f, 100.0f);
                glowScale1 = 65.0f;
                glowAngle = D_0054F770;
                glowTexture = data->MoonTmx2;
                glowAlpha0 = 40.0f;
                glowAlpha1 = 60.0f;
                glowScale0 = 50.0f;
                glowU = 1;
                glowUW = 0x3A;
                glowVH = 0x3E;
                goto drawPhaseGlow;
            case 6:
            case 10:
                spriteRgb = 0x808080;
                evtDrawDeadMoonSprite(0xF, 4, 0x3A, 0x3E, 0x63, 1, 0x1C, 0x1C,
                                      0x808080, data->MoonTmx1, offsetX, offsetY,
                                      evtMoonPhase, 70.0f, 70.0f, 75.0f, 100.0f);
                glowAngle = D_0054F770;
                glowTexture = data->MoonTmx2;
                glowAlpha0 = 40.0f;
                glowAlpha1 = 60.0f;
                glowU = 1;
                glowScale0 = 65.0f;
                glowScale1 = 85.0f;
                goto drawPhaseGlow;
            case 7:
            case 9:
                spriteRgb = 0x808080;
                evtDrawDeadMoonSprite(0xF, 4, 0x3A, 0x3E, 0x63, 1, 0x1C, 0x1C,
                                      0x808080, data->MoonTmx1, offsetX, offsetY,
                                      evtMoonPhase, 80.0f, 80.0f, 110.0f, 80.0f);
                glowScale0 = 80.0f;
                glowScale1 = 100.0f;
                glowAngle = D_0054F770;
                glowTexture = data->MoonTmx2;
                glowAlpha0 = 50.0f;
                glowAlpha1 = 70.0f;
                glowU = 1;
                glowUW = 0x3A;
                glowVH = 0x3E;
                goto drawPhaseGlow;
            case 8:
                evtDrawDeadMoonSprite(0xF, 4, 0x3A, 0x3E, 0x63, 1, 0x1C, 0x1C,
                                      0x808080, data->MoonTmx1, offsetX, offsetY,
                                      evtMoonPhase, 100.0f, 100.0f, 100.0f, 130.0f);
                evtDrawDeadMoonSprite(0xF, 4, 0x3A, 0x3E, 0x63, 1, 0x1C, 0x1C,
                                      0x808080, data->MoonTmx1, offsetX, offsetY,
                                      evtMoonPhase, 40.0f, 40.0f, 100.0f, 130.0f);
                evtDrawDeadMoonSprite(0xF, 4, 0x3A, 0x3E, 1, 1, 0x3A, 0x3E,
                                      0x808080, data->MoonTmx2, offsetX, offsetY,
                                      D_0054F770, 60.0f, 80.0f, 100.0f, 130.0f);
                break;
            }
            goto block_90;
drawPhaseGlow:
            evtDrawDeadMoonSprite(0xF, 4, 0x3A, 0x3E, glowU, 1,
                                  glowUW, glowVH, spriteRgb, glowTexture,
                                  offsetX, offsetY, glowAngle, glowAlpha0,
                                  glowAlpha1, glowScale0, glowScale1);
block_90:
            rsetAlpha(0);
            if (evtMoonMove > 0) {
                evtMoonMove -= 1;
                rsetAlpha(1);
                movePhase = DDS3_GBWK->moonPhase;
                switch (movePhase) {     /* switch 6 */
                case 0:                 /* switch 6 */
                    moveTimer = (f32) evtMoonMove;
                    moveStrength = 25.0f;
                    moveScale = 100.0f;
computeMoveAlpha:
                    moveAlpha = (moveTimer * moveStrength) / 30.0f;
                    break;
                case 1:                 /* switch 6 */
                case 15:                /* switch 6 */
                    moveTimer = (f32) evtMoonMove;
                    moveStrength = 25.0f;
                    moveScale = 100.0f;
                    goto computeMoveAlpha;
                case 2:                 /* switch 6 */
                case 14:                /* switch 6 */
                    moveTimer = (f32) evtMoonMove;
                    moveStrength = 25.0f;
                    moveScale = 100.0f;
                    goto computeMoveAlpha;
                case 3:                 /* switch 6 */
                case 13:                /* switch 6 */
                    moveTimer = (f32) evtMoonMove;
                    moveStrength = 25.0f;
                    moveScale = 100.0f;
                    goto computeMoveAlpha;
                case 4:                 /* switch 6 */
                case 12:                /* switch 6 */
                    moveTimer = (f32) evtMoonMove;
                    moveStrength = 35.0f;
                    moveScale = 110.0f;
                    goto computeMoveAlpha;
                case 5:                 /* switch 6 */
                case 11:                /* switch 6 */
                    moveTimer = (f32) evtMoonMove;
                    moveStrength = 60.0f;
moveScale120:
                    moveScale = 120.0f;
                    goto computeMoveAlpha;
                case 6:                 /* switch 6 */
                case 10:                /* switch 6 */
                    moveTimer = (f32) evtMoonMove;
                    moveStrength = 80.0f;
                    goto moveScale120;
                case 7:                 /* switch 6 */
                case 9:                 /* switch 6 */
                    moveTimer = (f32) evtMoonMove;
                    moveStrength = 80.0f;
                    goto moveScale120;
                case 8:                 /* switch 6 */
                    moveTimer = (f32) evtMoonMove;
                    moveStrength = 100.0f;
                    goto moveScale120;
                }
                evtDrawDeadMoonSprite(0xF, 4, 0x3A, 0x3E, 0x63, 1, 0x1C, 0x1C, 0xFFFFFF, data->MoonTmx1, offsetX, offsetY, 0.0f, moveAlpha, moveAlpha, moveScale, moveScale);
                rsetAlpha(0);
            }
            packedAlpha = (u32)baseAlpha << 0x18;
            if (DDS3_GBWK->moonPhase == 0) {
                DrwSprite2D(iconX, iconY, 0x1C, 0x1C, 1, 0x15, 0x1C, 0x1C, packedAlpha, data->MoonTmx3);
            }
            DrwSprite2D(iconX, iconY, 0x1C, 0x1C, 1, 0x15, 0x1C, 0x1C, packedAlpha | 0x808080, data->MoonTmx3);
            finalPhase = DDS3_GBWK->moonPhase;
            if ((finalPhase == 1) || (finalPhase == 0xF)) {
                DrwSprite2D(iconX, iconY, 0x1C, 0x1C, 0x63, 0x1E, 0x1C, 0x1C, 0x26808080, data->MoonTmx1);
            }
            D_0054F770 += D_0054E3BC;
            evtMoonPhase += D_0054E3BC;
            evtMoonTimer += D_0054E3C0;
            evtMoonState += D_0054E3C4;
        }
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "event/evtMoon", evtMoonProcess2);
#endif

extern s32 dds3GetRandIntA(s32 minimum, s32 range);
extern s32 evtBitCheck(s32 eventNo);
extern void evtBitOn(s32 eventNo);
extern void fldEsutoMaClearMsg(void);
extern void fldRiberaMaClearMsg(void);
extern void fldRifutoMaClearMsg(void);
extern void fldLightMaClearMsg(void);
extern s32 printf(const char *format, ...);
extern s32 evtMoonMove;

void evtMoonAddMove(f32 delta)
{
    if ((DDS3_GBWK->moonFlags & 1) == 0) {
        return;
    }
    DDS3_GBWK->moonTimer += delta;
    if (!(4500.0f < DDS3_GBWK->moonTimer)) {
        return;
    }
    {
        u8 age = DDS3_GBWK->moonPhase + 1;
        DDS3_GBWK->moonTimer = 0.0f;
        evtSetAgeOfMoon(age & 0xF);
        evtMoonMove = 30;
        if (evtGetAgeOfMoon() == 0) {
            fldEsutoMaClearMsg();
            fldRiberaMaClearMsg();
            fldRifutoMaClearMsg();
            fldLightMaClearMsg();
        }
    }
    DDS3_GBWK->unkA42[0]++;
    if (dds3GetRandIntA(0, 2) + 12 < DDS3_GBWK->unkA42[0]) {
        DDS3_GBWK->unkA42[0] = 0;
    }
    if (evtBitCheck(0x502) != 0) {
        if (DDS3_GBWK->moonAnahoriCount < 0xF0) {
            DDS3_GBWK->moonAnahoriCount++;
            printf("moon anahori:%d\n", DDS3_GBWK->moonAnahoriCount);
        }
        if (evtBitCheck(0x504) != 0) {
            if (DDS3_GBWK->moonAnahoriCount == 0x30) {
                evtBitOn(0x505);
            }
            if (DDS3_GBWK->moonAnahoriCount == 0x70) {
                evtBitOn(0x506);
            }
        } else {
            if (DDS3_GBWK->moonAnahoriCount == 0x30) {
                evtBitOn(0x505);
            }
            if (DDS3_GBWK->moonAnahoriCount == 0xA0) {
                evtBitOn(0x506);
            }
        }
    } else if (evtBitCheck(0x503) == 0) {
        DDS3_GBWK->moonAnahoriCount = 0;
    }
}

INCLUDE_RODATA(const s32, "event/evtMoon", D_0053B1F8);

s32 evtMoonProcess1(dds3ProcessID_t *id)
{
    evtMoonProcessData_t *work = sdfChipAllocDirect(sizeof(*work));
    evtMoonPacRequest_t *request;
    sdfPacDecodeResrc_t *entry;
    s32 index = 0;

    work->MoonTmx1 = NULL;
    work->MoonTmx2 = NULL;
    work->MoonTmx3 = NULL;
    work->MoonTmx4 = NULL;
    work->MoonTmx5 = NULL;
    work->MoonTmx6 = NULL;
    request = pacLoad(D_0053B1F8);
    pacSync(request);
    entry = request->packet.resrcHead;
    if (entry != NULL) {
        do {
            switch (index) {
            case 0:
                work->MoonTmx1 = entry->resrc;
                break;
            case 1:
                work->MoonTmx2 = entry->resrc;
                break;
            case 2:
                work->MoonTmx3 = entry->resrc;
                break;
            case 3:
                work->MoonTmx4 = entry->resrc;
                break;
            case 4:
                work->MoonTmx5 = entry->resrc;
                break;
            case 5:
                work->MoonTmx6 = entry->resrc;
                break;
            }
            entry = entry->next;
            index++;
        } while (entry != NULL);
    }
    pacFileHandleFree_(request);
    dds3SetProcessData(id, work);
    return (s32)evtMoonProcess2;
}

void evtMoonShutdown(dds3ProcessID_t *id)
{
    evtMoonProcessData_t *work = dds3GetProcessData(id);

    if (work->MoonTmx1 != NULL) {
        sdfTexFreeDelayed(work->MoonTmx1);
    }
    if (work->MoonTmx2 != NULL) {
        sdfTexFreeDelayed(work->MoonTmx2);
    }
    if (work->MoonTmx3 != NULL) {
        sdfTexFreeDelayed(work->MoonTmx3);
    }
    if (work->MoonTmx4 != NULL) {
        sdfTexFreeDelayed(work->MoonTmx4);
    }
    if (work->MoonTmx5 != NULL) {
        sdfTexFreeDelayed(work->MoonTmx5);
    }
    if (work->MoonTmx6 != NULL) {
        sdfTexFreeDelayed(work->MoonTmx6);
    }
    sdfChipFreeDirect(work);
    moonprocess_id = NULL;
}

void evtInitMoonProcess(void)
{
    if (moonprocess_id == NULL) {
        moonprocess_id = dds3StartProcess(D_0054F780, 0x2B0B, 1, 1, evtMoonProcess1,
                                          (dds3ProcessFunc_t)evtMoonShutdown, NULL);
        evtEnableMoonCalc();
        evtDisableMoonDraw();
        evtSetAgeOfMoon(0);
        DDS3_GBWK->unkA42[0] = 0;
        DDS3_GBWK->moonAnahoriCount = 0;
    }
}

void evt_1EB030(void)
{
    if (moonprocess_id != NULL) {
        dds3KillProcess(moonprocess_id, 1);
    }
}

INCLUDE_RODATA(const s32, "event/evtMoon", D_0053B228);

INCLUDE_SDATA(const s32, "event/evtMoon", gMoonCnt);

INCLUDE_SDATA(const s32, "event/evtMoon", evtMoonMove);

INCLUDE_SDATA(const s32, "event/evtMoon", evtDeadMoonDraw_total_cnt);

INCLUDE_SDATA(const s32, "event/evtMoon", evtDeadMoonDraw_a_cnt);

INCLUDE_SDATA(const s32, "event/evtMoon", moonprocess_id);

INCLUDE_SDATA(const s32, "event/evtMoon", D_0054F770);

INCLUDE_SDATA(const s32, "event/evtMoon", evtMoonTimer);

INCLUDE_SDATA(const s32, "event/evtMoon", evtMoonPhase);

INCLUDE_SDATA(const s32, "event/evtMoon", evtMoonState);

INCLUDE_SDATA(const s32, "event/evtMoon", D_0054F780);

INCLUDE_SDATA(const s32, "event/evtMoon", D_0054F788);

INCLUDE_SDATA(const s32, "event/evtMoon", evtDebugCameraPage);

INCLUDE_SDATA(const s32, "event/evtMoon", D_0054F790);

INCLUDE_SDATA(const s32, "event/evtMoon", evtDebugState);

INCLUDE_SDATA(const s32, "event/evtMoon", evtSceneDispWork);

INCLUDE_SDATA(const s32, "event/evtMoon", evtSceneDispTimer);

INCLUDE_SDATA(const s32, "event/evtMoon", evtSceneDispState);

INCLUDE_SDATA(const s32, "event/evtMoon", D_0054F7A4);

INCLUDE_SDATA(const s32, "event/evtMoon", D_0054F7A8);

INCLUDE_SDATA(const s32, "event/evtMoon", D_0054F7B0);

INCLUDE_SDATA(const s32, "event/evtMoon", D_0054F7B8);

INCLUDE_SDATA(const s32, "event/evtMoon", D_0054F7C0);

INCLUDE_SDATA(const s32, "event/evtMoon", D_0054F7C8);

INCLUDE_SDATA(const s32, "event/evtMoon", D_0054F7D0);

INCLUDE_SDATA(const s32, "event/evtMoon", D_0054F7D8);

INCLUDE_SDATA(const s32, "event/evtMoon", D_0054F7E0);

INCLUDE_SDATA(const s32, "event/evtMoon", D_0054F7E8);

INCLUDE_SDATA(const s32, "event/evtMoon", D_0054F7F0);

INCLUDE_SDATA(const s32, "event/evtMoon", D_0054F7F8);

INCLUDE_SDATA(const s32, "event/evtMoon", D_0054F800);

INCLUDE_SDATA(const s32, "event/evtMoon", D_0054F808);

INCLUDE_SDATA(const s32, "event/evtMoon", D_0054F810);

INCLUDE_SDATA(const s32, "event/evtMoon", D_0054F818);

