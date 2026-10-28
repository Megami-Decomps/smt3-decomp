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

INCLUDE_ASM(const s32, "event/evtMoon", evtDeadMoonDraw);

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

INCLUDE_ASM(const s32, "event/evtMoon", evtMoonAddMove);

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
        *(s32 *)&DDS3_GBWK->unk11A7C[0] = 0;
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

