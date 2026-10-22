#include "common.h"
#include "kernel.h"
#include "libsdf.h"

extern void dds3AdminiCallSequence(s32 sequence, void *data, s32 datasize, s32 restore);

INCLUDE_ASM(const s32, "kernel/dds3StartMenu", InitTitleProcess);

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

INCLUDE_ASM(const s32, "kernel/dds3StartMenu", dds3StartMenuProcess);
