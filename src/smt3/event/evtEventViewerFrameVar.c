#include "common.h"
#include "kernel.h"

extern char D_0053BC98[];

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, dds3ProcessFunc_t drawExit,
                                         dds3ProcessFunc_t deleteExit, void *p);

s32 FrameVarProcess(dds3ProcessID_t *id);

INCLUDE_ASM(const s32, "event/evtEventViewerFrameVar", FrameVarProcess);

void evtInitFrameVar(void)
{
    dds3StartProcess(D_0053BC98, 0x2AF9, 1, 1, FrameVarProcess, NULL, NULL);
}

INCLUDE_RODATA(const s32, "event/evtEventViewerFrameVar", D_0053BC98);

INCLUDE_RODATA(const s32, "event/evtEventViewerFrameVar", D_0053BCB0);

