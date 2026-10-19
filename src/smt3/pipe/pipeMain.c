#include "fpu.h"
#include "common.h"
#include "ps2asm.h"
#include "basic.h"
#include "motion.h"

extern void (*D_00551828)(void);

extern void *dds3StartProcess(const char *name, s32 priority, s32 startDelay, s32 endDelay,
                             s32 (*update)(void *), void (*destroy)(void *), void *work);

extern s32 SectionIndex, mapIndex, mapPrev;

extern u8 debugMode, pipeInitDone;

extern s32 MainPipe(void *process);

extern s32 pipeInitSection(void);

extern s32 pipeCheckLevel(s32 level, s32 type);

extern void pipeEndSection(void);

extern char D_00548900[];

extern void DEBUG_PRINT(const char *fmt, ...);

extern char D_005488F0[];

extern void *dds3SearchProcessName(const char *name);

extern s32 dds3KillProcessName(char *name, s32 shutdowntimer);

INCLUDE_ASM(const s32, "pipe/pipeMain", MainPipe);

void InitPipe(s32 mapIdx, s32 prev)
{
    s32 priority = 0x3E8;

    dds3StartProcess(D_005488F0, priority, 0, 0, MainPipe, NULL, NULL);
    if (mapIdx == 0) {
        SectionIndex = 0x102;
        debugMode = 1;
    } else {
        debugMode = 0;
        SectionIndex = 1;
        mapIndex = mapIdx;
        mapPrev = prev;
        pipeInitDone = pipeCheckLevel(mapIdx, prev);
    }
    pipeInitSection();
    priority = 0;
}

void EndPipe(void)
{
    s32 shutdown = 1;

    pipeEndSection();
    dds3KillProcessName(D_005488F0, shutdown);
    DEBUG_PRINT(D_00548900);
    shutdown = 0;
}

s32 ChkPipe(void)
{
    return dds3SearchProcessName(D_005488F0) != NULL;
}

void DEBUG_PRINT(const char *fmt, ...) {
}

INCLUDE_RODATA(const s32, "pipe/pipeMain", D_005488F0);

INCLUDE_RODATA(const s32, "pipe/pipeMain", D_00548900);

