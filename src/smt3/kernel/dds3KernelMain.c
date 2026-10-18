#include "common.h"

extern u32 dds3FrameCount;
extern u32 dds3FrameID;
extern u32 dds3RunCount;
extern u8 dds3SoftResetFlag;
extern u8 dds3SoftResetDisableFlag;
extern u32 dds3PauseSign;
extern u32 dds3PauseTime;
extern u32 dds3SlowSign;
extern u32 dds3SlowTime;
extern u32 dds3SlowRunCount;
extern u32 dds3SlowEndCount;

INCLUDE_ASM(const s32, "kernel/dds3KernelMain", dds3FirstInit);

void KernelDebug(void) {
}

void dds3SoftReset(void)
{
    dds3SoftResetFlag = 1;
}

u32 dds3GetKernelCount(void)
{
    return dds3FrameCount;
}

u32 dds3GetFrameID(void)
{
    return dds3FrameID;
}

void dds3PauseOn(void)
{
    dds3PauseTime = 0;
    dds3PauseSign = 1;
    dds3SlowSign = 0;
}

void dds3PauseOff(void)
{
    dds3PauseSign = 0;
    dds3PauseTime = 0;
    dds3SlowSign = 0;
}

void dds3Stop(u32 pauseCount)
{
    dds3PauseTime = pauseCount;
    dds3PauseSign = 1;
    dds3SlowSign = 0;
}

void dds3SlowOn(u32 interval, u32 duration)
{
    u32 runCount = dds3RunCount;

    dds3SlowSign = 1;
    dds3SlowTime = interval;
    dds3SlowRunCount = runCount;
    dds3SlowEndCount = duration;
    dds3PauseSign = 0;
    dds3PauseTime = 0;
}

void dds3SlowOff(void)
{
    dds3PauseSign = 0;
    dds3SlowTime = 4;
    dds3PauseTime = 0;
    dds3SlowSign = 0;
}

void dds3ScreenCaptureOn(void) {
}

void dds3ScreenCaptureOff(void) {
}

void dds3SoftResetOn(void)
{
    dds3SoftResetDisableFlag = 0;
}

void dds3SoftResetOff(void)
{
    dds3SoftResetDisableFlag = 1;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelMain", dds3KernelMain);
INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3FrameCount);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3RunCount);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3SoftResetFlag);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", D_0054E809);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3SoftResetDisableFlag);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3PauseSign);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3PauseTime);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3SlowSign);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3SlowTime);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3SlowRunCount);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3SlowEndCount);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3DebugSign);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", D_0054E828);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", D_0054E82A);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3VblanksPerFrame);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", D_0054E82E);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3KernelDebugSign);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", D_0054E834);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", D_0054E838);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", D_0054E840);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3DrawProcessFlag);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", D_0054E849);

