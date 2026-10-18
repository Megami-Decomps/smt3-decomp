#include "common.h"
#include "newdata.h"
#include "kernel.h"
#include "libsdf.h"

typedef void (*evtOtRegFunc_t)(sdfGrOt_t *, sdfDrawTag_t *);

extern f32 gMoonCnt;
extern dds3ProcessID_t *moonprocess_id;
extern s32 evtDebugState;
extern void *evtSceneDispWork;
extern u32 dds3BackColor;
extern u8 dds3PadButtonState[];
extern sdfGrOt_t sdfGrOtTable[];
extern char D_0053B2E8[];
extern void *sdfGrAllocPacketBuff(u32 size);
extern void sdfDrawTagInit(sdfDrawTag_t *tag);
extern void sdfDrawTagLink(sdfDrawTag_t *tag, void *packet);
extern void dds3ConsPanelTagLink(sdfDrawTag_t *, s32, s32, s32, s32);
extern void *sdfDevPrintf(s32 x, s32 y, s32 z, s32 style, const char *format, ...);
extern s32 sprintf(char *, const char *, ...);
extern s32 sdfFileCheckExist(char *path);
extern void scrStartScriptByName(s32 handle, const char *path, s32 param);
extern void scrAllReleaseScript(void);
extern void evtFreeStage(void);
extern void evtBitAllOff(void);
extern void evtDebugPrintCamera(void);
s32 EventDebugProcess1(dds3ProcessID_t *id);

INCLUDE_ASM(const s32, "code_001E95A0", evtDeadMoonDraw);

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

void evtSetAgeOfMoon(u8 phase)
{
    DDS3_GBWK->moonPhase = phase & 0xF;
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

INCLUDE_ASM(const s32, "code_001E95A0", evtEnableMoonDraw);

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

INCLUDE_ASM(const s32, "code_001E95A0", evtMoonProcess2);

INCLUDE_ASM(const s32, "code_001E95A0", evtMoonAddMove);

INCLUDE_ASM(const s32, "code_001E95A0", evtMoonProcess1);

INCLUDE_ASM(const s32, "code_001E95A0", evtMoonShutdown);

INCLUDE_ASM(const s32, "code_001E95A0", evtInitMoonProcess);

void evt_1EB030(void)
{
    if (moonprocess_id != NULL) {
        dds3KillProcess(moonprocess_id, 1);
    }
}

INCLUDE_ASM(const s32, "code_001E95A0", evtDebugPrintCamera);

/* After launching a test script, keep the test task idle and return 0. */
s32 evtDebugCameraStep(dds3ProcessID_t *id)
{
    evtDebugPrintCamera();
    return 0;
}

/* Return the same update callback installed by evtInitEventTest. */
void *evtGetDebugProcess(void)
{
    return (void *)EventDebugProcess1;
}

INCLUDE_RODATA(const s32, "code_001E95A0", D_0053B228);

s32 EventDebugProcess1(dds3ProcessID_t *id)
{
    char path[0x40];
    sdfDrawTag_t *tag;
    sdfGrOt_t *ot;

    tag = sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    dds3ConsPanelTagLink(tag, 0x84, 0x54, 0x15, 10);
    sdfDrawTagLink(tag, sdfDevPrintf(0x7840, 0x7BA0, 0xFEFFFF, 0, "Event Debug"));
    sdfDrawTagLink(tag, sdfDevPrintf(0x7A80, 0x7C60, 0xFEFFFF, 6, "E%03d.BF", evtDebugState));
    sdfDrawTagLink(tag, sdfDevPrintf(0x7900, 0x7D20, 0xFEFFFF, 0, "L,R  = EVENT SELECT"));
    sdfDrawTagLink(tag, sdfDevPrintf(0x7900, 0x7D80, 0xFEFFFF, 0, "RR   = ENTER"));
    sdfDrawTagLink(tag, sdfDevPrintf(0x7900, 0x7DE0, 0xFEFFFF, 0, "RU   = SET E500"));
    sdfDrawTagLink(tag, sdfDevPrintf(0x7900, 0x7E40, 0xFEFFFF, 0, "RL   = SET E600"));
    sdfDrawTagLink(tag, sdfDevPrintf(0x7900, 0x7EA0, 0xFEFFFF, 0, "RD   = RESET FLAG"));
    sdfDrawTagLink(tag, sdfDevPrintf(0x7900, 0x7F00, 0xFEFFFF, 0, "L1R1 = +-100"));
    ot = &sdfGrOtTable[81];
    ((evtOtRegFunc_t)ot->regFunc)(ot, tag);

    if ((s8)dds3PadButtonState[0x21] < 0) {
        sprintf(path, "/event/e%03d/e%03d/scr/e%03d.bf",
                evtDebugState - evtDebugState % 10, evtDebugState, evtDebugState);
        if (sdfFileCheckExist(path) != 0) {
            dds3BackColor = 0x80000000;
            scrStartScriptByName(0x3EB, path, 0);
            return (s32)evtDebugCameraStep;
        }
    }
    if ((s8)dds3PadButtonState[0x23] < 0) {
        evtBitAllOff();
        return (s32)evtGetDebugProcess;
    }
    if (dds3PadButtonState[0x25] & 2) {
        evtDebugState++;
    }
    if ((dds3PadButtonState[0x24] & 2) && evtDebugState > 500) {
        evtDebugState--;
    }
    if ((s8)dds3PadButtonState[0x22] < 0) {
        evtDebugState = 500;
    }
    if ((s8)dds3PadButtonState[0x20] < 0) {
        evtDebugState = 600;
    }
    if ((dds3PadButtonState[0x28] & 2) && evtDebugState > 500) {
        evtDebugState -= 100;
        if (evtDebugState < 500) {
            evtDebugState = 500;
        }
    }
    if (dds3PadButtonState[0x2A] & 2) {
        evtDebugState += 100;
    }
    return 0;
}

/* Test-task teardown destroys the named script processes. */
void EventDebugShutdown(dds3ProcessID_t *id)
{
    scrAllReleaseScript();
}

void evtInitEventTest(void)
{
    dds3BackColor = 0x80000000;
    dds3StartProcess(D_0053B2E8, 0x2AF9, 1, 1,
                    (dds3ProcessFunc_t)EventDebugProcess1,
                    (dds3ProcessFunc_t)EventDebugShutdown, NULL);
}

INCLUDE_RODATA(const s32, "code_001E95A0", D_0053B2E8);

void evtChkEndProcess(void)
{
    dds3KillProcessName(D_0053B2E8, 1);
    dds3KillProcessName("PolygonMovie", 0);
    evtFreeStage();
}

INCLUDE_RODATA(const s32, "code_001E95A0", D_0053B308);

INCLUDE_RODATA(const s32, "code_001E95A0", D_0053B318);

INCLUDE_RODATA(const s32, "code_001E95A0", D_0053B328);

INCLUDE_RODATA(const s32, "code_001E95A0", D_0053B338);

INCLUDE_RODATA(const s32, "code_001E95A0", D_0053B348);

INCLUDE_ASM(const s32, "code_001E95A0", evtSceneDispProcess);

void func_001EBC90(void)
{
    evtSceneDispWork = NULL;
}
INCLUDE_SDATA(const s32, "code_001E95A0", gMoonCnt);

INCLUDE_SDATA(const s32, "code_001E95A0", evtMoonMove);

INCLUDE_SDATA(const s32, "code_001E95A0", evtDeadMoonDraw_total_cnt);

INCLUDE_SDATA(const s32, "code_001E95A0", evtDeadMoonDraw_a_cnt);

INCLUDE_SDATA(const s32, "code_001E95A0", moonprocess_id);

INCLUDE_SDATA(const s32, "code_001E95A0", D_0054F770);

INCLUDE_SDATA(const s32, "code_001E95A0", evtMoonTimer);

INCLUDE_SDATA(const s32, "code_001E95A0", evtMoonPhase);

INCLUDE_SDATA(const s32, "code_001E95A0", evtMoonState);

INCLUDE_SDATA(const s32, "code_001E95A0", D_0054F780);

INCLUDE_SDATA(const s32, "code_001E95A0", D_0054F788);

INCLUDE_SDATA(const s32, "code_001E95A0", evtDebugCameraPage);

INCLUDE_SDATA(const s32, "code_001E95A0", D_0054F790);

INCLUDE_SDATA(const s32, "code_001E95A0", evtDebugState);

INCLUDE_SDATA(const s32, "code_001E95A0", evtSceneDispWork);

INCLUDE_SDATA(const s32, "code_001E95A0", evtSceneDispTimer);

INCLUDE_SDATA(const s32, "code_001E95A0", evtSceneDispState);

INCLUDE_SDATA(const s32, "code_001E95A0", D_0054F7A4);

INCLUDE_SDATA(const s32, "code_001E95A0", D_0054F7A8);

INCLUDE_SDATA(const s32, "code_001E95A0", D_0054F7B0);

INCLUDE_SDATA(const s32, "code_001E95A0", D_0054F7B8);

INCLUDE_SDATA(const s32, "code_001E95A0", D_0054F7C0);

INCLUDE_SDATA(const s32, "code_001E95A0", D_0054F7C8);

INCLUDE_SDATA(const s32, "code_001E95A0", D_0054F7D0);

INCLUDE_SDATA(const s32, "code_001E95A0", D_0054F7D8);

INCLUDE_SDATA(const s32, "code_001E95A0", D_0054F7E0);

INCLUDE_SDATA(const s32, "code_001E95A0", D_0054F7E8);

INCLUDE_SDATA(const s32, "code_001E95A0", D_0054F7F0);

INCLUDE_SDATA(const s32, "code_001E95A0", D_0054F7F8);

INCLUDE_SDATA(const s32, "code_001E95A0", D_0054F800);

INCLUDE_SDATA(const s32, "code_001E95A0", D_0054F808);

INCLUDE_SDATA(const s32, "code_001E95A0", D_0054F810);

INCLUDE_SDATA(const s32, "code_001E95A0", D_0054F818);

