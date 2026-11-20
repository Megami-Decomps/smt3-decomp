#include "common.h"
#include "newdata.h"
#include "kernel.h"
#include "libsdf.h"
#include "fileManager.h"
#include "sdfFrame.h"

extern f32 gMoonCnt;

extern dds3ProcessID_t *moonprocess_id;
extern char evtMoonProcessName[]; /* "moon" */
extern const char evtMoonPacPath[]; /* "/fld/f/bin/moon.LB" */
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


saddr_t evtMoonProcess1(dds3ProcessID_t *id);
extern s32 evtMoonProcess2(dds3ProcessID_t *id);
void evtMoonShutdown(dds3ProcessID_t *id);

extern sdfTexHandle_t *DeadMoonTmx;
extern s32 evtDeadMoonDraw_total_cnt;
extern s32 evtDeadMoonDraw_a_cnt;
extern u32 dds3GetKernelCount(void);
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
        if (SDF_FRAME_DUE()) {
            evtDeadMoonDraw_total_cnt = (evtDeadMoonDraw_total_cnt + 1) % 74;
            evtDeadMoonDraw_a_cnt = (evtDeadMoonDraw_a_cnt + 1) % 48;
        }
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

extern f32 evtMoonFadeInStep, evtMoonFadeOutStep, evtMoonPhaseRate, evtMoonTimerRate, evtMoonStateRate;
extern u32 dds3NullDrawProc(void);
extern s32 fmProcessChk(void);
extern s32 fldChkVortex(void);
extern f32 evtMoonGlowAngle;
extern f32 evtMoonTimer, evtMoonPhase, evtMoonState;
extern s32 evtMoonMove;

s32 evtMoonProcess2(dds3ProcessID_t *id) {
    f32 nextShown;
    f32 nextHidden;
    f32 zeroTerm;
    f32 oneMinusShown;
    f32 moveTimer;
    f32 moveStrength;
    FAKEMATCH_ASM_REG(f32, duration, "$f2");
    f32 moveScale;
    f32 moveAlpha;
    FAKEMATCH_ASM_REG(f32, glowBeforeTick, "$f2");
    u32 packedAlpha;
    s32 offsetX;
    s32 offsetY;
    s32 iconX;
    s32 iconY;
    s32 firstAlpha;
    s32 baseAlpha;
    FAKEMATCH_ASM_REG(s32, secondAlpha, "$23");
    u8 glyphPhase;
    u8 pulsePhase;
    u8 alphaPhase;
    u8 movePhase;
    u8 overlayPhase;
    u8 glowPhase;
    u8 finalPhase;
    evtMoonProcessData_t *data;
    dds3GlobalWork_t *moonWork;
    FAKEMATCH_ASM_REG(dds3GlobalWork_t *, badgeWork, "$2");
    FAKEMATCH_ASM_REG(dds3GlobalWork_t *, initialWork, "$3");
    FAKEMATCH_ASM_REG(dds3GlobalWork_t *, incomingWork, "$4");

    moveScale = 0.0f;
    firstAlpha = 0;
    baseAlpha = 0;
    secondAlpha = 0;
    moveAlpha = 0.0f;
    if (fmProcessChk() != 0) return 0;
    if (fldChkVortex() == 0) return 0;
    if (dds3NullDrawProc() != 0) return 0;
    data = dds3GetProcessData(id);
    /* Separate the post-call acquisition from the two later member reads. */
    incomingWork = DDS3_GBWK;
    FAKEMATCH_ASM_LIVE(incomingWork);
    initialWork = incomingWork;
    FAKEMATCH_ASM_LIVE(initialWork);
    if (initialWork->moonFlags & 2) {
        if (gMoonCnt < 1.0f) {
            nextShown = gMoonCnt + 0.09999999404f;
            if (!(nextShown > 1.0f)) {
                gMoonCnt = nextShown;
            } else {
                gMoonCnt = 1.0f;
            }
        }
    } else if (gMoonCnt > 0.0f) {
        nextHidden = gMoonCnt - 0.09999999404f;
        if (nextHidden < 0.0f) {
            gMoonCnt = 0.0f;
        } else {
            gMoonCnt = nextHidden;
        }
    }
    if (DeadMoonTmx != 0) {
        evtDeadMoonDraw(data);
    } else {
        f32 yBias;
        f32 xBias;
        oneMinusShown = 1.0f - gMoonCnt;
        zeroTerm = gMoonCnt * 0.0f;
        xBias = oneMinusShown * -250.0f;
        yBias = oneMinusShown * 0.0f;
        offsetY = (s32)(zeroTerm + yBias);
        offsetX = (s32)(zeroTerm + xBias);
        if (initialWork->moonFlags & 2) {
            iconY = offsetY + 0x15;
            rsetOtOfs(0x47);
            rsetAlpha(0);
            rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
            DrwSprite2D(offsetX + 0x10, iconY, 0x61, 0x2B, 1, 1, 0x61, 0x2B, 0x80808080, data->MoonTmx1);
            glyphPhase = DDS3_GBWK->moonPhase;
            switch (glyphPhase) {
            case 0:
                DrwSprite2D(offsetX + 0x3C, offsetY + 0x23, 64, 0x12, 50, 39, 64, 0x12, 0x80808080, data->MoonTmx4);
                break;
            case 1:
            case 15:
                DrwSprite2D(offsetX + 0x3D, offsetY + 0x22, 0xE, 0x13, 1, 1, 0xE, 0x13, 0x80808080, data->MoonTmx3);
                DrwSprite2D(offsetX + 0x4A, offsetY + 0x27, 0x15, 0x13, 0x6A, 1, 0x15, 0x13, 0x80808080, data->MoonTmx3);
                break;
            case 2:
            case 14:
                DrwSprite2D(offsetX + 0x3D, offsetY + 0x22, 0xE, 0x13, 16, 1, 0xE, 0x13, 0x80808080, data->MoonTmx3);
                DrwSprite2D(offsetX + 0x4A, offsetY + 0x27, 0x15, 0x13, 0x6A, 1, 0x15, 0x13, 0x80808080, data->MoonTmx3);
                break;
            case 3:
            case 13:
                DrwSprite2D(offsetX + 0x3D, offsetY + 0x22, 0xE, 0x13, 31, 1, 0xE, 0x13, 0x80808080, data->MoonTmx3);
                DrwSprite2D(offsetX + 0x4A, offsetY + 0x27, 0x15, 0x13, 0x6A, 1, 0x15, 0x13, 0x80808080, data->MoonTmx3);
                break;
            case 4:
            case 12:
                DrwSprite2D(offsetX + 0x3C, offsetY + 0x23, 47, 0x12, 2, 45, 47, 0x12, 0x80808080, data->MoonTmx5);
                break;
            case 5:
            case 11:
                DrwSprite2D(offsetX + 0x3D, offsetY + 0x22, 0xE, 0x13, 61, 1, 0xE, 0x13, 0x80808080, data->MoonTmx3);
                DrwSprite2D(offsetX + 0x4A, offsetY + 0x27, 0x15, 0x13, 0x6A, 1, 0x15, 0x13, 0x80808080, data->MoonTmx3);
                break;
            case 6:
            case 10:
                DrwSprite2D(offsetX + 0x3D, offsetY + 0x22, 0xE, 0x13, 76, 1, 0xE, 0x13, 0x80808080, data->MoonTmx3);
                DrwSprite2D(offsetX + 0x4A, offsetY + 0x27, 0x15, 0x13, 0x6A, 1, 0x15, 0x13, 0x80808080, data->MoonTmx3);
                break;
            case 7:
            case 9:
                DrwSprite2D(offsetX + 0x3D, offsetY + 0x22, 0xE, 0x13, 91, 1, 0xE, 0x13, 0x80808080, data->MoonTmx3);
                DrwSprite2D(offsetX + 0x4A, offsetY + 0x27, 0x15, 0x13, 0x6A, 1, 0x15, 0x13, 0x80808080, data->MoonTmx3);
                break;
            case 8:
                DrwSprite2D(offsetX + 0x3C, offsetY + 0x23, 47, 0x12, 2, 39, 47, 0x12, 0x80808080, data->MoonTmx4);
                break;
            default:
                break;
            }
            rsetAlpha(1);
            pulsePhase = DDS3_GBWK->moonPhase;
            switch (pulsePhase) {
            case 5:
            case 11:
                evtDrawDeadMoonSprite(0xF, 4, 0x3A, 0x3E, 0x3E, 1, 0x3A, 0x3E,
                                      0x808080, data->MoonTmx2, offsetX, offsetY,
                                      evtMoonTimer, 20.0f, 10.0f, 100.0f, 100.0f);
                break;
            case 6:
            case 10:
                evtDrawDeadMoonSprite(0xF, 4, 0x3A, 0x3E, 0x3E, 1, 0x3A, 0x3E,
                                      0x808080, data->MoonTmx2, offsetX, offsetY,
                                      evtMoonTimer, 40.0f, 20.0f, 100.0f, 100.0f);
                break;
            case 7:
            case 9:
                evtDrawDeadMoonSprite(0xF, 4, 0x3A, 0x3E, 0x3E, 1, 0x3A, 0x3E,
                                      0x808080, data->MoonTmx2, offsetX, offsetY,
                                      evtMoonTimer, 50.0f, 20.0f, 100.0f, 100.0f);
                break;
            case 8:
                evtDrawDeadMoonSprite(0xF, 4, 0x3A, 0x3E, 0x3E, 1, 0x3A, 0x3E,
                                      0x808080, data->MoonTmx2, offsetX, offsetY,
                                      evtMoonTimer, 30.0f, 100.0f, 100.0f, 100.0f);
                break;
            case 0: case 1: case 2: case 3: case 4:
            case 12: case 13: case 14: case 15:
                break;
            }
            rsetAlpha(0);
            alphaPhase = DDS3_GBWK->moonPhase;
            switch (alphaPhase) {
            case 0:
            case 1:
            case 15:
                firstAlpha = 0;
                secondAlpha = 0;
                baseAlpha = 0x80;
                break;
            case 2:
            case 14:
                firstAlpha = 0;
                secondAlpha = 0x80;
                baseAlpha = 0x46;
                break;
            case 3:
            case 13:
                firstAlpha = 0;
                secondAlpha = 0x73;
                baseAlpha = 0x26;
                break;
            case 4:
            case 12:
                firstAlpha = 0;
                secondAlpha = 0x80;
                baseAlpha = 0;
                break;
            case 5:
            case 11:
                firstAlpha = 0x2C;
                secondAlpha = 0x53;
                baseAlpha = 0;
                break;
            case 6:
            case 10:
                firstAlpha = 0x40;
                secondAlpha = 0x40;
                baseAlpha = 0;
                break;
            case 7:
            case 9:
                firstAlpha = 0x53;
                secondAlpha = 0x2C;
                baseAlpha = 0;
                break;
            case 8:
                firstAlpha = 0x80;
                secondAlpha = 0;
                baseAlpha = 0;
                break;
            }
            iconX = offsetX + 0x1E;
            DrwSprite2D(iconX, iconY, 0x1C, 0x1C, 0x1E, 0x15, 0x1C, 0x1C, ((u32)firstAlpha << 0x18) | 0x808080, data->MoonTmx3);
            {
                FAKEMATCH_ASM_REG(u32, secondPacked, "$2");
                FAKEMATCH_ASM_REG(u32, shiftedBits, "$2");
                /* Form the second sprite payload only after the first draw. */
                FAKEMATCH_ASM_LIVE(secondAlpha);
                shiftedBits = (u32)secondAlpha << 0x18;
                FAKEMATCH_ASM_LIVE(shiftedBits);
                secondPacked = shiftedBits | 0x808080;

                DrwSprite2D(iconX, iconY, 0x1C, 0x1C, 0x63, 0x1E, 0x1C, 0x1C, secondPacked, data->MoonTmx1);
            }
            rsetAlpha(1);
            moonWork = DDS3_GBWK;
            overlayPhase = moonWork->moonPhase;
            /* Only an invalid phase skips both dispatches. */
            if (overlayPhase >= 16) goto afterGlow;
            switch (overlayPhase) {
            case 5:
            case 11:
                evtDrawDeadMoonSprite(0x1D, 0x14, 0x1F, 0x20, 0x5A, 1,
                                      0x1F, 0x20, 0x808080, data->MoonTmx5,
                                      offsetX, offsetY, evtMoonState,
                                      35.0f, 100.0f, 100.0f, 100.0f);
                evtDrawDeadMoonSprite(0x1D, 0x14, 0x1F, 0x20, 0x52, 1,
                                      0x1F, 0x20, 0x808080, data->MoonTmx4,
                                      offsetX, offsetY, evtMoonState,
                                      100.0f, 35.0f, 100.0f, 100.0f);
                moonWork = DDS3_GBWK;
                break;
            case 6:
            case 10:
                evtDrawDeadMoonSprite(0x1A, 0x12, 0x27, 0x24, 0x29, 1,
                                      0x27, 0x24, 0x808080, data->MoonTmx4,
                                      offsetX, offsetY, evtMoonState,
                                      35.0f, 100.0f, 100.0f, 100.0f);
                evtDrawDeadMoonSprite(0x1A, 0x12, 0x27, 0x24, 1, 1,
                                      0x27, 0x24, 0x808080, data->MoonTmx4,
                                      offsetX, offsetY, evtMoonState,
                                      100.0f, 35.0f, 100.0f, 100.0f);
                moonWork = DDS3_GBWK;
                break;
            case 7:
            case 9:
                evtDrawDeadMoonSprite(0x18, 0xF, 0x2B, 0x2A, 0x2D, 1,
                                      0x2B, 0x2A, 0x808080, data->MoonTmx5,
                                      offsetX, offsetY, evtMoonState,
                                      40.0f, 100.0f, 100.0f, 100.0f);
                evtDrawDeadMoonSprite(0x18, 0xF, 0x2B, 0x2A, 1, 1,
                                      0x2B, 0x2A, 0x808080, data->MoonTmx5,
                                      offsetX, offsetY, evtMoonState,
                                      100.0f, 40.0f, 100.0f, 100.0f);
                moonWork = DDS3_GBWK;
                break;
            case 8:
                evtDrawDeadMoonSprite(0x12, 0xA, 0x36, 0x33, 0x39, 1,
                                      0x36, 0x33, 0x808080, data->MoonTmx6,
                                      offsetX, offsetY, evtMoonState,
                                      35.0f, 100.0f, 100.0f, 100.0f);
                evtDrawDeadMoonSprite(0x12, 0xA, 0x36, 0x33, 1, 1,
                                      0x36, 0x33, 0x808080, data->MoonTmx6,
                                      offsetX, offsetY, evtMoonState,
                                      100.0f, 35.0f, 100.0f, 100.0f);
                moonWork = DDS3_GBWK;
                break;
            case 0: case 1: case 2: case 3: case 4:
            case 12: case 13: case 14: case 15:
                break;
            }
            {
                /* The native glow stage rereads the byte, including on no-overlay paths. */
                dds3GlobalWork_t *glowWork = moonWork;
                FAKEMATCH_ASM_LIVE(glowWork);
                glowPhase = glowWork->moonPhase;
            }
            switch (glowPhase) {
            case 0:
                evtDrawDeadMoonSprite(0xF, 4, 0x3A, 0x3E, 0x63, 1,
                                      0x1C, 0x1C, 0x4A40FF, data->MoonTmx1,
                                      offsetX, offsetY, evtMoonPhase, 70.0f,
                                      70.0f, 70.0f, 100.0f);
                break;
            case 1:
            case 15:
                evtDrawDeadMoonSprite(0xF, 4, 0x3A, 0x3E, 0x63, 1,
                                      0x1C, 0x1C, 0x6060B0, data->MoonTmx1,
                                      offsetX, offsetY, evtMoonPhase, 75.0f,
                                      75.0f, 60.0f, 90.0f);
                break;
            case 2:
            case 14:
                evtDrawDeadMoonSprite(0xF, 4, 0x3A, 0x3E, 0x63, 1,
                                      0x1C, 0x1C, 0x7070A0, data->MoonTmx1,
                                      offsetX, offsetY, evtMoonPhase, 75.0f,
                                      75.0f, 60.0f, 90.0f);
                break;
            case 3:
            case 13:
                evtDrawDeadMoonSprite(0xF, 4, 0x3A, 0x3E, 0x63, 1,
                                      0x1C, 0x1C, 0x808090, data->MoonTmx1,
                                      offsetX, offsetY, evtMoonPhase, 75.0f,
                                      75.0f, 70.0f, 90.0f);
                break;
            case 4:
            case 12:
                evtDrawDeadMoonSprite(0xF, 4, 0x3A, 0x3E, 0x63, 1, 0x1C, 0x1C,
                                      0x808080, data->MoonTmx1, offsetX, offsetY,
                                      evtMoonPhase, 70.0f, 70.0f, 70.0f, 90.0f);
                evtDrawDeadMoonSprite(0xF, 4, 0x3A, 0x3E, 1, 1,
                                      0x3A, 0x3E, 0x808080, data->MoonTmx2,
                                      offsetX, offsetY, evtMoonGlowAngle, 20.0f,
                                      20.0f, 60.0f, 75.0f);
                break;
            case 5:
            case 11:
                evtDrawDeadMoonSprite(0xF, 4, 0x3A, 0x3E, 0x63, 1, 0x1C, 0x1C,
                                      0x808080, data->MoonTmx1, offsetX, offsetY,
                                      evtMoonPhase, 55.0f, 65.0f, 75.0f, 100.0f);
                evtDrawDeadMoonSprite(0xF, 4, 0x3A, 0x3E, 1, 1,
                                      0x3A, 0x3E, 0x808080, data->MoonTmx2,
                                      offsetX, offsetY, evtMoonGlowAngle, 40.0f,
                                      60.0f, 50.0f, 65.0f);
                break;
            case 6:
            case 10:
                evtDrawDeadMoonSprite(0xF, 4, 0x3A, 0x3E, 0x63, 1, 0x1C, 0x1C,
                                      0x808080, data->MoonTmx1, offsetX, offsetY,
                                      evtMoonPhase, 70.0f, 70.0f, 75.0f, 100.0f);
                evtDrawDeadMoonSprite(0xF, 4, 0x3A, 0x3E, 1, 1,
                                      0x3A, 0x3E, 0x808080, data->MoonTmx2,
                                      offsetX, offsetY, evtMoonGlowAngle, 40.0f,
                                      60.0f, 65.0f, 85.0f);
                break;
            case 7:
            case 9:
                evtDrawDeadMoonSprite(0xF, 4, 0x3A, 0x3E, 0x63, 1, 0x1C, 0x1C,
                                      0x808080, data->MoonTmx1, offsetX, offsetY,
                                      evtMoonPhase, 80.0f, 80.0f, 110.0f, 80.0f);
                evtDrawDeadMoonSprite(0xF, 4, 0x3A, 0x3E, 1, 1,
                                      0x3A, 0x3E, 0x808080, data->MoonTmx2,
                                      offsetX, offsetY, evtMoonGlowAngle, 50.0f,
                                      70.0f, 80.0f, 100.0f);
                break;
            case 8:
                evtDrawDeadMoonSprite(0xF, 4, 0x3A, 0x3E, 0x63, 1, 0x1C, 0x1C,
                                      0x808080, data->MoonTmx1, offsetX, offsetY,
                                      evtMoonPhase, 100.0f, 100.0f, 100.0f, 130.0f);
                evtDrawDeadMoonSprite(0xF, 4, 0x3A, 0x3E, 0x63, 1, 0x1C, 0x1C,
                                      0x808080, data->MoonTmx1, offsetX, offsetY,
                                      evtMoonPhase, 40.0f, 40.0f, 100.0f, 130.0f);
                evtDrawDeadMoonSprite(0xF, 4, 0x3A, 0x3E, 1, 1, 0x3A, 0x3E,
                                      0x808080, data->MoonTmx2, offsetX, offsetY,
                                      evtMoonGlowAngle, 60.0f, 80.0f, 100.0f, 130.0f);
                break;
            default:
                break;
            }
        afterGlow:
            rsetAlpha(0);
            if (evtMoonMove > 0) {
                evtMoonMove -= 1;
                rsetAlpha(1);
                movePhase = DDS3_GBWK->moonPhase;
                switch (movePhase) {
                case 0:
                    moveTimer = (f32) evtMoonMove;
                    moveStrength = 25.0f;
                    duration = SDF_FRAMES_F(30.0f);
                    moveScale = 100.0f;
                    moveAlpha = (moveTimer * moveStrength) / duration;
                    break;
                case 1:
                case 15:
                    moveTimer = (f32) evtMoonMove;
                    moveStrength = 25.0f;
                    duration = SDF_FRAMES_F(30.0f);
                    moveScale = 100.0f;
                    moveAlpha = (moveTimer * moveStrength) / duration;
                    break;
                case 2:
                case 14:
                    moveTimer = (f32) evtMoonMove;
                    moveStrength = 25.0f;
                    duration = SDF_FRAMES_F(30.0f);
                    moveScale = 100.0f;
                    moveAlpha = (moveTimer * moveStrength) / duration;
                    break;
                case 3:
                case 13:
                    moveTimer = (f32) evtMoonMove;
                    moveStrength = 25.0f;
                    duration = SDF_FRAMES_F(30.0f);
                    moveScale = 100.0f;
                    moveAlpha = (moveTimer * moveStrength) / duration;
                    break;
                case 4:
                case 12:
                    moveTimer = (f32) evtMoonMove;
                    moveStrength = 35.0f;
                    duration = SDF_FRAMES_F(30.0f);
                    moveScale = 110.0f;
                    moveAlpha = (moveTimer * moveStrength) / duration;
                    break;
                case 5:
                case 11:
                    moveTimer = (f32) evtMoonMove;
                    moveStrength = 60.0f;
                    duration = SDF_FRAMES_F(30.0f);
                    moveScale = 120.0f;
                    moveAlpha = (moveTimer * moveStrength) / duration;
                    break;
                case 6:
                case 10:
                    moveTimer = (f32) evtMoonMove;
                    moveStrength = 80.0f;
                    duration = SDF_FRAMES_F(30.0f);
                    moveScale = 120.0f;
                    moveAlpha = (moveTimer * moveStrength) / duration;
                    break;
                case 7:
                case 9:
                    moveTimer = (f32) evtMoonMove;
                    moveStrength = 80.0f;
                    duration = SDF_FRAMES_F(30.0f);
                    moveScale = 120.0f;
                    moveAlpha = (moveTimer * moveStrength) / duration;
                    break;
                case 8:
                    moveTimer = (f32) evtMoonMove;
                    moveStrength = 100.0f;
                    duration = SDF_FRAMES_F(30.0f);
                    moveScale = 120.0f;
                    moveAlpha = (moveTimer * moveStrength) / duration;
                    break;
                }
                evtDrawDeadMoonSprite(0xF, 4, 0x3A, 0x3E, 0x63, 1, 0x1C, 0x1C, 0xFFFFFF, data->MoonTmx1, offsetX, offsetY, 0.0f, moveAlpha, moveAlpha, moveScale, moveScale);
                rsetAlpha(0);
                badgeWork = DDS3_GBWK;
            } else {
                badgeWork = DDS3_GBWK;
            }
            {
                u32 markerHigh;
                packedAlpha = (u32)baseAlpha << 0x18;
                if (badgeWork->moonPhase == 0) {
                    FAKEMATCH_ASM_REG(s32, badgeX, "$4") = iconX;
                    FAKEMATCH_ASM_LIVE_PAIR(badgeX, packedAlpha);
                    DrwSprite2D(badgeX, iconY, 0x1C, 0x1C, 1, 0x15, 0x1C, 0x1C, packedAlpha, data->MoonTmx3);
                    markerHigh = 0x800000;
                } else {
                    markerHigh = 0x800000;
                    FAKEMATCH_ASM_LIVE(markerHigh);
                }
                FAKEMATCH_ASM_LIVE(markerHigh);
                markerHigh |= 0x8080;
                DrwSprite2D(iconX, iconY, 0x1C, 0x1C, 1, 0x15, 0x1C, 0x1C, packedAlpha | markerHigh, data->MoonTmx3);
            }
            finalPhase = DDS3_GBWK->moonPhase;
            if ((finalPhase == 1) || (finalPhase == 0xF)) {
                DrwSprite2D(iconX, iconY, 0x1C, 0x1C, 0x63, 0x1E, 0x1C, 0x1C, 0x26808080, data->MoonTmx1);
                glowBeforeTick = evtMoonGlowAngle;
            } else {
                glowBeforeTick = evtMoonGlowAngle;
            }
            {
                f32 phaseRate;
                f32 phase;
                f32 timer;
                f32 timerRate;
                f32 state;
                f32 stateRate;
                phaseRate = SDF_FRAME_STEP(0.09999999404f);
                timer = evtMoonTimer;
                phase = evtMoonPhase;
                glowBeforeTick += phaseRate;
                state = evtMoonState;
                /* The stock EE compiler emits the original 0x3e999999 pool word. */
                timerRate = SDF_FRAME_STEP(0.2999999822f);
                phase += phaseRate;
                stateRate = SDF_FRAME_STEP(0.1999999881f);
                timer += timerRate;
                evtMoonGlowAngle = glowBeforeTick;
                state += stateRate;
                evtMoonPhase = phase;
                evtMoonTimer = timer;
                evtMoonState = state;
            }
        }
    }
    return 0;
}

extern s32 dds3GetRandIntA(s32 minimum, s32 range);
extern s32 evtBitCheck(s32 eventNo);
extern void evtBitOn(s32 eventNo);
extern void fldEsutoMaClearMsg(void);
extern void fldRiberaMaClearMsg(void);
extern void fldRifutoMaClearMsg(void);
extern void fldLightMaClearMsg(void);
#include "sdk_libc.h"

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
        evtMoonMove = SDF_FRAMES(30);
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

const char evtMoonPacPath[24] = "/fld/f/bin/moon.LB";

saddr_t evtMoonProcess1(dds3ProcessID_t *id)
{
    evtMoonProcessData_t *work = sdfChipAllocDirect(sizeof(*work));
    fileRequest_t *request;
    sdfPacDecodeResrc_t *entry;
    s32 index = 0;

    work->MoonTmx1 = NULL;
    work->MoonTmx2 = NULL;
    work->MoonTmx3 = NULL;
    work->MoonTmx4 = NULL;
    work->MoonTmx5 = NULL;
    work->MoonTmx6 = NULL;
    request = pacLoad(evtMoonPacPath);
    pacSync(request);
    entry = request->pac.resrcHead;
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
    return EE_ADDR_S(evtMoonProcess2);
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
        moonprocess_id = dds3StartProcess(evtMoonProcessName, 0x2B0B, 1, 1, evtMoonProcess1,
                                          (dds3ProcessFunc_t)evtMoonShutdown, NULL);
        evtEnableMoonCalc();
        evtDisableMoonDraw();
        evtSetAgeOfMoon(0);
        DDS3_GBWK->unkA42[0] = 0;
        DDS3_GBWK->moonAnahoriCount = 0;
    }
}

#ifdef NON_MATCHING
extern s32 dds3KillProcess(dds3ProcessID_t *id, s32 shutdowntimer);
#endif

void evt_1EB030(void)
{
    if (moonprocess_id != NULL) {
        dds3KillProcess(moonprocess_id, 1);
    }
}

const char D_0053B228[16] = "FREE CAMERA";

f32 gMoonCnt = 0;

s32 evtMoonMove = 0;

s32 evtDeadMoonDraw_total_cnt = 0;

s32 evtDeadMoonDraw_a_cnt = 0;

dds3ProcessID_t *moonprocess_id = NULL;

f32 evtMoonGlowAngle = 0;

f32 evtMoonTimer = 0;

f32 evtMoonPhase = 0;

f32 evtMoonState = 0;

INCLUDE_SDATA(const s32, "event/evtMoon", evtMoonProcessName);
SDATA_RESUME();

char *evtDebugDefaultCameraName = D_0053B228;

u32 evtDebugCameraPage = 1;

INCLUDE_SDATA(const s32, "event/evtMoon", evtDebugCameraNameFmt);
SDATA_RESUME();

s32 evtDebugState = 500;

s32 evtSceneDispWork = 0;

s32 evtSceneDispTimer = 0;

s32 evtSceneDispState = 0;

SDATA_PAD(4);

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

INCLUDE_SDATA(const s32, "event/evtMoon", evtSceneDispCountFmt);

INCLUDE_SDATA(const s32, "event/evtMoon", evtSceneDispSerialFmt);

