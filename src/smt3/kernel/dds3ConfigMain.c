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

extern void dds3DrawConfigPanel(s32 id);



extern char D_00551700[];

extern s8 dds3ConfigState;

extern u8 *GBWK;

extern u8 *DDS3_GBWK;

extern s32 cmpUpdateTerminate(u8 *p);

extern s32 dds3KillProcessName(char *name, s32 shutdowntimer);

extern s8 dds3PadButtonState[];

extern s32 cmpGetPressedButton(s32 mask);

extern s32 cmpMoveCursor(u8 *cursor, s32 dir);

extern s32 cmpChkPadPressUpDown(void);

extern s32 cmpDisablePadBitStop(u8 *work, s32 bits, s32 stop);

extern s32 cmpPlayCursorSE(s32 button);

extern s32 dds3GetConfigFlag(u32 index, s32 *table);

extern void dds3ToggleConfigFlag(u32 index, s32 *table);

extern void dds3UpdateConfigCursor(void);

extern void dds3ScrollConfigList(void);

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

/* NON_MATCHING: 27 of 119 overlapping words match; 476 bytes against retail's 492;
 * entry read of GBWK[0] is reused in the switch arms instead of reloaded. */
#ifdef NON_MATCHING
s32 dds3UpdateConfig(void *PID)
{
    u16 button = cmpGetPressedButton(0xF3);
    s32 playSE = 1;
    s32 index;

    index = *(s16 *)(GBWK + 0x3E) + GBWK[0x40];
    if (GBWK[0] & 0x40) {
        return -1;
    }
    switch (button) {
    case 1:
        if (index == 3) {
            GBWK[0] |= 0x10;
        } else {
            playSE = 0;
        }
        break;
    case 2:
        GBWK[0] |= 0x10;
        break;
    case 0x40:
        if (dds3PadButtonState[0x24] >= 0) {
            playSE = 0;
            break;
        }
        if (dds3GetConfigFlag(index, (s32 *)(DDS3_GBWK + 0x11AB8)) != 0) {
            playSE = 0;
            break;
        }
        goto toggle;
    case 0x80:
        if (dds3PadButtonState[0x25] >= 0) {
            playSE = 0;
            break;
        }
        if (dds3GetConfigFlag(index, (s32 *)(DDS3_GBWK + 0x11AB8)) == 0) {
            playSE = 0;
            break;
        }
    toggle:
        if (index == 3) {
            playSE = 0;
            break;
        }
        dds3ToggleConfigFlag(index, (s32 *)(DDS3_GBWK + 0x11AB8));
        break;
    case 0x10:
        cmpMoveCursor(GBWK + 0x1C, -1);
        break;
    case 0x20:
        cmpMoveCursor(GBWK + 0x1C, 1);
        break;
    }
    if (cmpChkPadPressUpDown() == 0) {
        cmpMoveCursor(GBWK + 0x1C, 0);
    }
    button = cmpDisablePadBitStop(GBWK + 0x1C, button, 0x30);
    dds3UpdateConfigCursor();
    dds3ScrollConfigList();
    if (playSE) {
        cmpPlayCursorSE(button);
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "kernel/dds3ConfigMain", dds3UpdateConfig);
#endif

INCLUDE_SDATA(const s32, "kernel/dds3ConfigMain", D_00551700);

