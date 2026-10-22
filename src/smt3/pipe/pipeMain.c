#include "fpu.h"
#include "common.h"
#include "ps2asm.h"
#include "basic.h"
#include "motion.h"

extern void (*D_00551828)(void);

extern void *dds3StartProcess(const char *v1, s32 dDS3_PRIORITY_DRAW_PB, s32 v2, s32 v3,
                             s32 (*drawExit)(void *), void (*deleteExit)(void *), void *p);

extern s32 SectionIndex, mapIndex, mapPrev;

extern u8 debugMode, pipeInitDone;

extern s32 MainPipe(void *aProcId);

extern s32 pipeInitSection(void);

extern s32 pipeCheckLevel(s32 level, s32 type);

extern void pipeEndSection(void);


extern void DEBUG_PRINT(const char *str, ...);


extern void *dds3SearchProcessName(const char *namestr);

extern s32 dds3KillProcessName(char *name, s32 shutdowntimer);

INCLUDE_ASM(const s32, "pipe/pipeMain", MainPipe);

void InitPipe(s32 data, s32 prevData)
{
    s32 priority = 0x3E8;

    dds3StartProcess("PipeMain", priority, 0, 0, MainPipe, NULL, NULL);
    if (data == 0) {
        SectionIndex = 0x102;
        debugMode = 1;
    } else {
        debugMode = 0;
        SectionIndex = 1;
        mapIndex = data;
        mapPrev = prevData;
        pipeInitDone = pipeCheckLevel(data, prevData);
    }
    pipeInitSection();
    priority = 0;
}

void EndPipe(void)
{
    s32 shutdown = 1;

    pipeEndSection();
    dds3KillProcessName("PipeMain", shutdown);
    DEBUG_PRINT("END PIPE\n");
    shutdown = 0;
}

s32 ChkPipe(void)
{
    return dds3SearchProcessName("PipeMain") != NULL;
}

void DEBUG_PRINT(const char *str, ...) {
}

