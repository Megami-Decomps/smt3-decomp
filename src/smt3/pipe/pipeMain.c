#include "fpu.h"
#include "common.h"
#include "ps2asm.h"
#include "basic.h"
#include "motion.h"

extern void (*D_00551828)(void);

extern void *dds3StartProcess(const char *v1, s32 dDS3_PRIORITY_DRAW_PB, s32 v2, s32 v3,
                             s32 (*drawExit)(void *), void (*deleteExit)(void *), void *p);

extern s32 SectionIndex, mapIndex, mapPrev;

extern s8 debugMode;

extern u8 pipeInitDone;

extern s32 MainPipe(void *aProcId);

extern s32 pipeInitSection(void);

extern s32 pipeCheckLevel(s32 level, s32 type);

extern void pipeEndSection(void);


extern void DEBUG_PRINT(const char *str, ...);


extern void *dds3SearchProcessName(const char *namestr);

extern s32 dds3KillProcessName(char *name, s32 shutdowntimer);

extern s32 pipeSelectSection(void);

extern s32 pipeLoadSection(s32 index);

extern s32 pipe_Section(void);

extern s32 pipeResultSection(void);

extern void dds3FadeIn(s32 r, s32 g, s32 b, s32 frame);

extern dds3Basic_t *dds3GetControlScene(void);

extern void dds3DeleteScene(dds3Basic_t *scene);

s32 MainPipe(void *aProcId)
{
    s32 selected;

    switch (SectionIndex) {
    case 1:
        if (mapIndex == 0) {
            SectionIndex = 0x102;
        } else if (pipeLoadSection(mapIndex - 1) != 0) {
            SectionIndex = 0x101;
        }
        break;
    case 0x102:
        selected = pipeSelectSection();
        if (selected != 0) {
            if (selected == -1) {
                debugMode = 0;
                SectionIndex = 0xFFFF;
            } else {
                mapIndex = selected;
                SectionIndex = 1;
            }
        }
        break;
    case 0x101:
        if (pipe_Section() != 0) {
            SectionIndex = 0x103;
        }
        break;
    case 0x103:
        if (pipeResultSection() != 0) {
            SectionIndex = 0xFFFF;
        }
        break;
    case 0xFFFF:
        if (debugMode == 2) {
            SectionIndex = 0x101;
        } else if (debugMode == 1) {
            dds3FadeIn(0, 0, 0, 0);
            SectionIndex = 0x102;
        } else {
            dds3DeleteScene(dds3GetControlScene());
            return -1;
        }
        break;
    }
    return 0;
}

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

