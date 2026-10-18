#include "common.h"

INCLUDE_ASM(const s32, "kernel/dds3KernelMain", dds3FirstInit);

void KernelDebug(void) {
}

INCLUDE_ASM(const s32, "kernel/dds3KernelMain", dds3SoftReset);

INCLUDE_ASM(const s32, "kernel/dds3KernelMain", dds3GetKernelCount);

INCLUDE_ASM(const s32, "kernel/dds3KernelMain", dds3GetFrameID);

INCLUDE_ASM(const s32, "kernel/dds3KernelMain", dds3PauseOn);

INCLUDE_ASM(const s32, "kernel/dds3KernelMain", dds3PauseOff);

INCLUDE_ASM(const s32, "kernel/dds3KernelMain", dds3Stop);

INCLUDE_ASM(const s32, "kernel/dds3KernelMain", dds3SlowOn);

INCLUDE_ASM(const s32, "kernel/dds3KernelMain", dds3SlowOff);

void dds3ScreenCaptureOn(void) {
}

void dds3ScreenCaptureOff(void) {
}

INCLUDE_ASM(const s32, "kernel/dds3KernelMain", dds3SoftResetOn);

INCLUDE_ASM(const s32, "kernel/dds3KernelMain", dds3SoftResetOff);

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

