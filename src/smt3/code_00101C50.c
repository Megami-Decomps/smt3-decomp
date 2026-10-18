#include "common.h"
#include "basic.h"
#include "kernel.h"

extern dds3Basic_t *dds3TheWorld;
extern void dds3CallBasicMainProc(dds3Basic_t *basic);
extern void dds3CallBasicDrawProc(dds3Basic_t *basic);

INCLUDE_ASM(const s32, "code_00101C50", main);

INCLUDE_ASM(const s32, "code_00101C50", dds3FirstProcess);

INCLUDE_ASM(const s32, "code_00101C50", dds3DrawFirstProcess);

INCLUDE_ASM(const s32, "code_00101C50", dds3DrawProcess);

s32 dds3BasicMainProcess(dds3ProcessID_t *process) {
    dds3CallBasicMainProc(dds3TheWorld);
    return 0;
}

s32 dds3BasicDrawProcess(dds3ProcessID_t *process) {
    dds3CallBasicDrawProc(dds3TheWorld);
    return 0;
}
INCLUDE_SDATA(const s32, "code_00101C50", D_0054E8BC);

INCLUDE_SDATA(const s32, "code_00101C50", dds3ClosedDrawTag);

