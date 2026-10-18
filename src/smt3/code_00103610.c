#include "common.h"
#include "kernel.h"
#include "libsdf.h"

extern s8 dds3PadButtonState[64];
extern u8 dds3PadVibrationState[2];
extern u8 dds3PadVibrationMode[2];
extern s32 dds3PadVibrationTable[2];
extern u8 D_002F5948[];
extern u64 *dds3PrimFrameRect(s32 x, s32 y, s32 z, s32 width, s32 height, u32 fill, u32 border);
extern void sdfDrawTagLink(sdfDrawTag_t *tag, u32 packet);
extern u32 dds3GetProcessRunTime(dds3ProcessID_t *process);
extern s32 dds3GetRandIntA(s32 stream, s32 limit);
extern s32 dds3GetGlobalConfigFlag(u32 flag);
extern void dds3GetVibrationLevel(void);
extern void sdfPadSetAct0(s32 pad, u16 level);
extern void sdfPadSetAct1(s32 pad, u8 level);
extern void sdfPadSetAct(s32 pad, s32 small, s32 large);
extern void sdfRandomize(void *state, u32 seed);
extern void dds3AdminiCallSequence(s32 sequence, void *data, s32 size, s32 mode);
void dds3PadVibrationSet(u32 motor, u8 level, s32 duration);


void dds3InitDebugSystem(void) {
}

void SelectSub(s32 *scrollOffset, s32 *cursorRow, s32 itemCount, s32 visibleRows, s32 padSet, s32 forwardInput, s32 backwardInput) {
    s32 backwardState;
    s32 forwardState;

    if (cursorRow == NULL) {
        return;
    }
    if (itemCount < 2) {
        return;
    }
    backwardState = dds3PadButtonState[backwardInput + padSet * 16];
    if (backwardState < 0) {
        if (scrollOffset == NULL) {
            if (*cursorRow > 0) {
                (*cursorRow)--;
            } else if (itemCount < visibleRows) {
                *cursorRow = itemCount - 1;
            } else {
                *cursorRow = visibleRows - 1;
            }
        } else if (*cursorRow > 0) {
            (*cursorRow)--;
        } else if (*scrollOffset > 0) {
            (*scrollOffset)--;
        } else if (visibleRows < itemCount) {
            *scrollOffset = itemCount - visibleRows;
            *cursorRow = visibleRows - 1;
        } else {
            *cursorRow = itemCount - 1;
        }
    } else if (dds3PadButtonState[backwardInput + padSet * 16] & 2) {
        if (scrollOffset == NULL) {
            if (*cursorRow > 0) {
                (*cursorRow)--;
            }
        } else if (*cursorRow > 0) {
            (*cursorRow)--;
        } else if (*scrollOffset > 0) {
            (*scrollOffset)--;
        }
    }
    forwardState = dds3PadButtonState[forwardInput + padSet * 16];
    if (forwardState < 0) {
        if (scrollOffset == NULL) {
            if (*cursorRow < visibleRows - 1) {
                (*cursorRow)++;
            } else {
                *cursorRow = 0;
            }
        } else if (*cursorRow < visibleRows - 1) {
            (*cursorRow)++;
        } else if (*scrollOffset + visibleRows < itemCount) {
            (*scrollOffset)++;
        } else {
            *scrollOffset = 0;
            *cursorRow = 0;
        }
    } else if (dds3PadButtonState[forwardInput + padSet * 16] & 2) {
        if (scrollOffset == NULL) {
            if (*cursorRow < visibleRows - 1) {
                (*cursorRow)++;
            }
        } else if (*cursorRow < visibleRows - 1) {
            (*cursorRow)++;
        } else if (*scrollOffset + visibleRows < itemCount) {
            (*scrollOffset)++;
        }
    }
}

s32 dds3SelectMenu(s32 padSet, s32 secondaryCount, s32 primaryCount, s32 secondaryVisible, s32 primaryVisible, s32 *secondaryScroll, s32 *primaryScroll, s32 *secondaryCursor, s32 *primaryCursor) {
    SelectSub(primaryScroll, primaryCursor, primaryCount, primaryVisible, padSet, 7, 6);
    SelectSub(secondaryScroll, secondaryCursor, secondaryCount, secondaryVisible, padSet, 5, 4);
    if (padSet == 0) {
        if (dds3PadButtonState[1] < 0) {
            return 1;
        }
        if (dds3PadButtonState[3] < 0) {
            return -1;
        }
    } else {
        if (dds3PadButtonState[0x11] < 0) {
            return 1;
        }
        if (dds3PadButtonState[0x13] < 0) {
            return -1;
        }
    }
    return 0;
}

void dds3ConsPanelTagLink(sdfDrawTag_t *packetList, s32 column, s32 row, s32 columnCount, s32 rowCount) {
    s32 columnSpan = 0xC0, rowSpan = 0x60;
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(column * 0x10 + 0x6FD0, row * 8 + 0x78E8, 0xFEFFFF,
                                           columnCount * columnSpan + rowSpan, rowCount * rowSpan + 0x30,
                                           0x60000000, 0x40806020));
}

void dds3ConsPanelTagLinkZ(sdfDrawTag_t *packetList, s32 column, s32 row, s32 columnCount, s32 rowCount, s32 depth) {
    s32 columnSpan = 0xC0, rowSpan = 0x60;
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(column * 0x10 + 0x6FD0, row * 8 + 0x78E8, depth,
                                           columnCount * columnSpan + rowSpan, rowCount * rowSpan + 0x30,
                                           0x60000000, 0x40806020));
}

s32 dds3PadVibrationTest(dds3ProcessID_t *task) {
    if (dds3GetProcessRunTime(task) % 0x4650 == 0) {
        dds3PadVibrationSet(0, 1, 0xF);
        dds3PadVibrationSet(1, (u8)(dds3GetRandIntA(0, 150) + 100), 30);
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_00103610", dds3PadInitialize);

INCLUDE_ASM(const s32, "code_00103610", dds3RunProcessList);

INCLUDE_ASM(const s32, "code_00103610", dds3GetVibrationLevel);

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

INCLUDE_ASM(const s32, "code_00103610", InitTitleProcess);

void InitNewBattleTestProcess(void) {
    dds3AdminiCallSequence(0x13, 0, 0, 0);
}

void func_001042F0(void) {
}

void InitKernelTestProcess(void) {
    dds3AdminiCallSequence(0x15, 0, 0, 0);
}

void InitEventTestProcess(void) {
    dds3AdminiCallSequence(0x16, 0, 0, 0);
}

void InitEventViewerProcess(void) {
    dds3AdminiCallSequence(0x17, 0, 0, 0);
}

void func_00104370(void) {
}

void InitMessageScriptTestProcess(void) {
    dds3AdminiCallSequence(0x19, 0, 0, 0);
}

void InitCampTestProcess(void) {
    dds3AdminiCallSequence(0x1A, 0, 0, 0);
}

void InitCombineTestProcess(void) {
    dds3AdminiCallSequence(0x1B, 0, 0, 0);
}

void InitShopTestProcess(void) {
    dds3AdminiCallSequence(0x1C, 0, 0, 0);
}

void InitRagTestProcess(void) {
    dds3AdminiCallSequence(0x1D, 0, 0, 0);
}

void InitRecoverTestProcess(void) {
    dds3AdminiCallSequence(0x1E, 0, 0, 0);
}

void InitTerminalTestProcess(void) {
    dds3AdminiCallSequence(0x1F, 0, 0, 0);
}

void InitResult2TestProcess(void) {
    dds3AdminiCallSequence(0x20, 0, 0, 0);
}

void InitNameTestProcess(void) {
    dds3AdminiCallSequence(0x21, 0, 0, 0);
}

void func_001044E0(void) {
}

void InitMovieViewerTestProcess(void) {
    dds3AdminiCallSequence(0x28, 0, 0, 0);
}

void InitSoundTestProcess(void) {
    dds3AdminiCallSequence(0x23, 0, 0, 0);
}

void InitFontTestProcess(void) {
    dds3AdminiCallSequence(0x25, 0, 0, 0);
}

void InitPuzzleBoyProcess(void) {
    dds3AdminiCallSequence(0x26, 0, 0, 0);
}

void InitStaffProcess(void) {
    dds3AdminiCallSequence(0x10, 0, 0, 0);
}

void InitEndSaveProcess(void) {
    dds3AdminiCallSequence(0x5, 0, 0, 0);
}

void InitPipeProcess(void) {
    dds3AdminiCallSequence(0x12, 0, 0, 0);
}

void InitModeSelectProcess(void) {
    dds3AdminiCallSequence(0x29, 0, 0, 0);
}

void InitTgsProcess(void) {
    dds3AdminiCallSequence(0x2A, 0, 0, 0);
}

INCLUDE_ASM(const s32, "code_00103610", dds3StartMenuProcess);

void dds3InitRandCB(void) {
    sdfRandomize(D_002F5948, 0x12345678);
}

INCLUDE_RODATA(const s32, "code_00103610", D_0052DFC8);

INCLUDE_RODATA(const s32, "code_00103610", D_0052DFD8);

INCLUDE_RODATA(const s32, "code_00103610", D_0052DFE8);

INCLUDE_RODATA(const s32, "code_00103610", D_0052DFF8);

INCLUDE_RODATA(const s32, "code_00103610", D_0052E008);

INCLUDE_RODATA(const s32, "code_00103610", D_0052E018);

INCLUDE_RODATA(const s32, "code_00103610", D_0052E028);

INCLUDE_RODATA(const s32, "code_00103610", D_0052E038);

INCLUDE_RODATA(const s32, "code_00103610", D_0052E048);

INCLUDE_RODATA(const s32, "code_00103610", D_0052E058);

INCLUDE_RODATA(const s32, "code_00103610", D_0052E068);

INCLUDE_RODATA(const s32, "code_00103610", D_0052E078);

INCLUDE_RODATA(const s32, "code_00103610", D_0052E088);

INCLUDE_RODATA(const s32, "code_00103610", D_0052E098);

INCLUDE_RODATA(const s32, "code_00103610", D_0052E0A8);

INCLUDE_RODATA(const s32, "code_00103610", D_0052E0B8);

INCLUDE_RODATA(const s32, "code_00103610", D_0052E0C8);

INCLUDE_RODATA(const s32, "code_00103610", D_0052E0D8);

INCLUDE_RODATA(const s32, "code_00103610", D_0052E0E8);

INCLUDE_RODATA(const s32, "code_00103610", D_0052E0F8);

INCLUDE_RODATA(const s32, "code_00103610", D_0052E110);

INCLUDE_RODATA(const s32, "code_00103610", D_0052E120);

INCLUDE_RODATA(const s32, "code_00103610", D_0052E130);

INCLUDE_RODATA(const s32, "code_00103610", D_0052E140);

INCLUDE_RODATA(const s32, "code_00103610", D_0052E158);

INCLUDE_RODATA(const s32, "code_00103610", D_0052E170);

INCLUDE_RODATA(const s32, "code_00103610", D_0052E180);

INCLUDE_SDATA(const s32, "code_00103610", dds3PadStickPrev);

INCLUDE_SDATA(const s32, "code_00103610", dds3PadStickStatusPrev);

INCLUDE_SDATA(const s32, "code_00103610", D_0054E904);

INCLUDE_SDATA(const s32, "code_00103610", D_0054E907);

INCLUDE_SDATA(const s32, "code_00103610", dds3PadStickState);

INCLUDE_SDATA(const s32, "code_00103610", dds3PadAnalogTable);

INCLUDE_SDATA(const s32, "code_00103610", D_0054E90A);

INCLUDE_SDATA(const s32, "code_00103610", dds3PadStickStatusState);

INCLUDE_SDATA(const s32, "code_00103610", D_0054E914);

INCLUDE_SDATA(const s32, "code_00103610", D_0054E918);

INCLUDE_SDATA(const s32, "code_00103610", D_0054E920);

INCLUDE_SDATA(const s32, "code_00103610", D_0054E928);

INCLUDE_SDATA(const s32, "code_00103610", D_0054E930);

INCLUDE_SDATA(const s32, "code_00103610", D_0054E938);

