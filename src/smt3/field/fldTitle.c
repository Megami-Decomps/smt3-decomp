#include "common.h"
#include "basic.h"
#include "model.h"
#include "newdata.h"
#include "scr.h"
#include "field.h"
#include "ps2asm.h"

enum sl_slot_flag
{
    SL_SLOT_FLAG_CAN_READ_SAVE = 1,
    SL_SLOT_FLAG_DIRECTORY_READY = 2,
    SL_SLOT_FLAG_DELETE_BEFORE_WRITE = 8
};

extern char D_005327B0[];

dds3ProcessID_t *dds3SearchProcessName(const char *name);

extern char D_00532820[];

extern sdfTexHandle_t *fldTitleMiniTex;

extern void sdfTexFreeDelayed(sdfTexHandle_t *texture);

extern sdfTexHandle_t *fldTitleMiniWork;

extern s32 (*proc_func_ans)(dds3ProcessID_t *process);

extern void fldTitleStart(s32 field, s32 option);

extern sdfTexHandle_t *D_0054EF98;

extern sdfTexHandle_t *D_0054EF9C;

extern sdfTexHandle_t *D_0054EFA0;

extern sdfTexHandle_t *D_0054EFA4;


extern char D_005327A0[];

s32 fldTitleChk(void) {
    return dds3SearchProcessName(D_005327B0) != 0;
}

INCLUDE_ASM(const s32, "field/fldTitle", fldTitle_Process);

void fldTitle_Shutdown(void) {
    if (D_0054EF98 != NULL) {
        sdfTexFreeDelayed(D_0054EF98);
        D_0054EF98 = NULL;
    }
    if (D_0054EF9C != NULL) {
        sdfTexFreeDelayed(D_0054EF9C);
        D_0054EF9C = NULL;
    }
    if (D_0054EFA0 != NULL) {
        sdfTexFreeDelayed(D_0054EFA0);
        D_0054EFA0 = NULL;
    }
    if (D_0054EFA4 != NULL) {
        sdfTexFreeDelayed(D_0054EFA4);
        D_0054EFA4 = NULL;
    }
}

INCLUDE_RODATA(const s32, "field/fldTitle", D_005327B0);

INCLUDE_ASM(const s32, "field/fldTitle", fldTitleStart);

s32 fldTitleMiniChk(void) {
    return dds3SearchProcessName(D_00532820) != 0;
}

INCLUDE_ASM(const s32, "field/fldTitle", fldTitleMini_Process);

void fldTitleMini_Shutdown(void) {
    if (fldTitleMiniTex != 0) {
        sdfTexFreeDelayed(fldTitleMiniTex);
        fldTitleMiniTex = 0;
    }
    if (fldTitleMiniWork != 0) {
        sdfTexFreeDelayed(fldTitleMiniWork);
        fldTitleMiniWork = 0;
    }
}

INCLUDE_RODATA(const s32, "field/fldTitle", D_00532820);

INCLUDE_ASM(const s32, "field/fldTitle", fldTitleMiniStart);
