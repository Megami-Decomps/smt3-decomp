#include "common.h"
#include "kernel.h"
#include "libsdf.h"

extern u8 dds3PadVibrationState[2];

extern u8 dds3PadVibrationMode[2];

extern s32 dds3PadVibrationTable[2];

extern u32 dds3GetProcessRunTime(dds3ProcessID_t *id);

extern s32 dds3GetRandIntA(s32 aLimit, s32 name);

extern s32 dds3GetGlobalConfigFlag(u32 flag);

extern void dds3GetVibrationLevel(void);

extern void sdfPadSetAct0(s32 pad, u16 value);

extern void sdfPadSetAct1(s32 pad, u8 value);

extern void sdfPadSetAct(s32 pad, s32 value0, s32 value1);

extern u8 dds3PadButtonState[64];
extern u8 dds3PadButtonPrev[64];
extern u32 D_002F56C0[64];
extern u32 D_002F57C0[4];
extern u8 dds3PadStickPrev[8];
extern s8 dds3PadStickStatusPrev[4];
extern u8 dds3PadStickState[8];
extern u8 dds3PadStickStatusState[4];

void dds3PadVibrationSet(u32 motor, u8 level, s32 duration);

s32 dds3PadVibrationTest(dds3ProcessID_t *task) {
    if (dds3GetProcessRunTime(task) % 0x4650 == 0) {
        dds3PadVibrationSet(0, 1, 0xF);
        dds3PadVibrationSet(1, (u8)(dds3GetRandIntA(0, 150) + 100), 30);
    }
    return 0;
}

void dds3PadInitialize(void) {
    s32 pad;
    s32 half;
    s32 key;

    for (pad = 0; pad < 2; pad++) {
        for (half = 0; half < 2; half++) {
            for (key = 0; key < 16; key++) {
                D_002F56C0[pad * 16 + half * 32 + key] = 0;
                dds3PadButtonPrev[pad * 16 + half * 32 + key] = 0;
                dds3PadButtonState[pad * 16 + half * 32 + key] = 0;
            }
        }
        for (key = 0; key < 2; key++) {
            D_002F57C0[pad * 2 + key] = 0;
            dds3PadStickPrev[pad * 4 + key * 2] = 0;
            dds3PadStickPrev[pad * 4 + key * 2 + 1] = 0;
            dds3PadStickStatusPrev[pad * 2 + key] = 0;
            dds3PadStickState[pad * 4 + key * 2] = 0;
            dds3PadStickState[pad * 4 + key * 2 + 1] = 0;
            dds3PadStickStatusState[pad * 2 + key] = 0;
        }
    }
    dds3PadVibrationState[0] = 0;
    dds3PadVibrationMode[0] = 0;
    dds3PadVibrationTable[0] = 0;
    dds3PadVibrationState[1] = 0;
    dds3PadVibrationMode[1] = 0;
    dds3PadVibrationTable[1] = 0;
}

INCLUDE_ASM(const s32, "kernel/dds3PadManager", dds3RunProcessList);

extern u8 dds3PadVibrationLevel;
extern u8 D_00551B91;
extern s32 printf(const char *format, ...);

void dds3GetVibrationLevel(void)
{
    u8 target = dds3PadVibrationLevel;
    u8 *level = dds3PadVibrationState + 1;
    if (target > *level) {
        if (target - *level > 0x20) {
            *level += 0x20;
        } else {
            *level = target;
        }
    } else if (target < *level) {
        if (*level - target > 0x20) {
            *level -= 0x20;
        } else {
            *level = target;
        }
    } else {
        *level = target;
    }
    printf("PadActPow:%02x/%02x\n", D_00551B91, target);
}

void dds3PadVibrationSet(u32 motor, u8 level, s32 duration) {
    if (dds3GetGlobalConfigFlag(0) == 0) {
        return;
    }
    if (motor == 0) {
        if (level != 0) {
            level = 1;
        }
    } else if (motor >= 2) {
        motor = 1;
    }
    if (motor == 0) {
        dds3PadVibrationMode[0] = level;
        dds3PadVibrationState[0] = level;
    } else {
        dds3PadVibrationMode[motor] = level;
        dds3GetVibrationLevel();
    }
    dds3PadVibrationTable[motor] = duration;
    if (motor == 0) {
        sdfPadSetAct0(0, level);
    } else {
        sdfPadSetAct1(0, dds3PadVibrationState[motor]);
    }
}

void dds3_104170(void) {
    dds3PadVibrationState[0] = 0;
    dds3PadVibrationTable[0] = 0;
    dds3PadVibrationState[1] = 0;
    dds3PadVibrationTable[1] = 0;
    sdfPadSetAct(0, 0, 0);
}

void dds3UpdateVibrationSlots(void) {
    s32 largeMotor = 1;
    u8 *levels = dds3PadVibrationState;
    u8 *targets = dds3PadVibrationMode;
    u8 *largeLevel;
    s32 motor;

    for (motor = 0; motor < 2; motor++) {
        if (dds3PadVibrationTable[motor] != 0) {
            /* The native countdown wraps as a 32-bit word, including negative durations. */
            dds3PadVibrationTable[motor] = (s32)((u32)dds3PadVibrationTable[motor] - 1);
            if (dds3PadVibrationTable[motor] == 0) {
                levels[motor] = 0;
                if (motor == 0) {
                    sdfPadSetAct0(0, 0);
                } else {
                    sdfPadSetAct1(0, 0);
                }
            } else if (motor == largeMotor) {
                largeLevel = levels + 1;
                if (*largeLevel != targets[1]) {
                    dds3GetVibrationLevel();
                    sdfPadSetAct1(0, *largeLevel);
                }
            }
        }
    }
}

INCLUDE_RODATA(const s32, "kernel/dds3PadManager", D_0052DFC8);

INCLUDE_RODATA(const s32, "kernel/dds3PadManager", D_0052DFD8);

INCLUDE_RODATA(const s32, "kernel/dds3PadManager", D_0052DFE8);

INCLUDE_RODATA(const s32, "kernel/dds3PadManager", D_0052DFF8);

INCLUDE_RODATA(const s32, "kernel/dds3PadManager", D_0052E008);

INCLUDE_RODATA(const s32, "kernel/dds3PadManager", D_0052E018);

INCLUDE_RODATA(const s32, "kernel/dds3PadManager", D_0052E028);

INCLUDE_RODATA(const s32, "kernel/dds3PadManager", D_0052E038);

INCLUDE_RODATA(const s32, "kernel/dds3PadManager", D_0052E048);

INCLUDE_RODATA(const s32, "kernel/dds3PadManager", D_0052E058);

INCLUDE_RODATA(const s32, "kernel/dds3PadManager", D_0052E068);

INCLUDE_RODATA(const s32, "kernel/dds3PadManager", D_0052E078);

INCLUDE_RODATA(const s32, "kernel/dds3PadManager", D_0052E088);

INCLUDE_RODATA(const s32, "kernel/dds3PadManager", D_0052E098);

INCLUDE_RODATA(const s32, "kernel/dds3PadManager", D_0052E0A8);

INCLUDE_RODATA(const s32, "kernel/dds3PadManager", D_0052E0B8);

INCLUDE_RODATA(const s32, "kernel/dds3PadManager", D_0052E0C8);

INCLUDE_RODATA(const s32, "kernel/dds3PadManager", D_0052E0D8);

INCLUDE_RODATA(const s32, "kernel/dds3PadManager", D_0052E0E8);

INCLUDE_RODATA(const s32, "kernel/dds3PadManager", D_0052E0F8);

INCLUDE_RODATA(const s32, "kernel/dds3PadManager", D_0052E110);

INCLUDE_RODATA(const s32, "kernel/dds3PadManager", D_0052E120);

INCLUDE_RODATA(const s32, "kernel/dds3PadManager", D_0052E130);

INCLUDE_RODATA(const s32, "kernel/dds3PadManager", D_0052E140);

INCLUDE_RODATA(const s32, "kernel/dds3PadManager", D_0052E158);

INCLUDE_RODATA(const s32, "kernel/dds3PadManager", D_0052E170);

INCLUDE_RODATA(const s32, "kernel/dds3PadManager", D_0052E180);

