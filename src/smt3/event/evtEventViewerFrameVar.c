#include "common.h"
#include "kernel.h"

extern char D_0053BC98[];

extern dds3ProcessID_t *dds3StartProcess(char *namestr, u32 priority, s32 starttimer,
                                         s32 shutdowntimer, dds3ProcessFunc_t processfunc,
                                         dds3ProcessFunc_t shutdownfunc, void *data);

s32 FrameVarProcess(dds3ProcessID_t *id);

INCLUDE_ASM(const s32, "event/evtEventViewerFrameVar", FrameVarProcess);

void evtInitFrameVar(void)
{
    dds3StartProcess(D_0053BC98, 0x2AF9, 1, 1, FrameVarProcess, NULL, NULL);
}

INCLUDE_RODATA(const s32, "event/evtEventViewerFrameVar", D_0053BC98);

INCLUDE_RODATA(const s32, "event/evtEventViewerFrameVar", D_0053BCB0);

