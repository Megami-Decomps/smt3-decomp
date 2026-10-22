#include "fpu.h"
#include "common.h"
#include "ps2asm.h"
#include "basic.h"
#include "motion.h"

extern void (*D_00551828)(void);

extern void *dds3AllocConfigWork(s32 mode);

extern s32 dds3CalcConfig(void *PID);

extern s32 dds3DrawConfig(void *PID);

extern s32 dds3UpdateConfig(void *PID);

extern void dds3DestroyConfig(void *PID);

extern void *dds3StartProcess(const char *v1, s32 dDS3_PRIORITY_DRAW_PB, s32 v2, s32 v3,
                             s32 (*drawExit)(void *), void (*deleteExit)(void *), void *p);

extern void sdfMemFreeDirect(void *aHandle);

extern s32 dds3AdminiCallSequence(s32 sequence, s32 *data, s32 datasize, s32 restore);

extern void dds3FreeConfigTexture(void *texture);

extern s32 dds3ConfigBackup;

extern s32 dds3DrawConfigPanel(s32 id);



extern char D_00551700[];

extern s8 dds3ConfigState;

extern u8 *GBWK;

extern u8 *DDS3_GBWK;

extern s32 cmpUpdateTerminate(u8 *p);

extern s32 dds3KillProcessName(char *name, s32 shutdowntimer);

void dds3DestroyConfig(void *PID)
{
    s32 seq = 1;

    if (GBWK != NULL) {
        dds3ConfigBackup = *(s32 *)(DDS3_GBWK + 0x11AB8);
        if (GBWK[0x18] == 1) {
            dds3AdminiCallSequence(2, &seq, 4, 0);
        }
        dds3FreeConfigTexture(*(void **)(GBWK + 0x10A4));
        sdfMemFreeDirect(*(void **)(GBWK + 0x14));
        GBWK = NULL;
        dds3ConfigState = 0;
    }
}

void dds3ConfigProcessStart(s32 Mode) {
    if (GBWK == 0) {
        GBWK = dds3AllocConfigWork(Mode);
        dds3StartProcess(D_00551700, 0x3F2, 1, 1, dds3CalcConfig, NULL, GBWK);
        dds3StartProcess("config_draw", 0x2B07, 1, 1, dds3DrawConfig, NULL, GBWK);
        dds3StartProcess("config_update", 0x520B, 1, 1, dds3UpdateConfig, dds3DestroyConfig, GBWK);
        dds3ConfigState = 1;
    }
}

void dds3ConfigProcessEnd(void)
{
    dds3KillProcessName(D_00551700, 1);
    dds3KillProcessName("config_draw", 1);
    dds3KillProcessName("config_update", 1);
}

s32 checkConfigState(void)
{
    if (dds3ConfigState == 1) {
        return 1;
    }
    if (dds3ConfigState >= 2) {
        if (dds3ConfigState == 2) {
            dds3ConfigState = 0;
        }
    }
    return 0;
}

s32 dds3CalcConfig(void *PID)
{
    u8 *gbwk = GBWK;

    if ((gbwk[0] & 0x10) != 0) {
        cmpUpdateTerminate(gbwk);
        return -1;
    }
    return 0;
}

s32 dds3DrawConfig(void *PID)
{
    u8 *gbwk = GBWK;

    if (gbwk[0] & 0x10) {
        cmpUpdateTerminate(gbwk);
        return -1;
    }
    dds3DrawConfigPanel(0x47);
    return 0;
}

INCLUDE_ASM(const s32, "kernel/dds3ConfigMain", dds3UpdateConfig);

