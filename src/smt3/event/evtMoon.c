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

INCLUDE_ASM(const s32, "event/evtMoon", evtMoonProcess2);

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

