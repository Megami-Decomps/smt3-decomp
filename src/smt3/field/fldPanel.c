#include "common.h"
#include "kernel.h"
#include "basic.h"
#include "field.h"

/* The input panel keeps its cursor, message choice and draw request as halfwords. */
typedef struct
{
    u16 Count;
    u16 Type;
    u16 Draw;
    u16 reserved;
} fldInputPanelProcessData_t;

typedef struct
{
    s32 unk00;
    s32 unk04;
    s32 unk08;
    s32 unk0C;
} fldAddressProcessData_t;

extern dds3ProcessID_t *inputpanel_id;
extern dds3ProcessID_t *addressprocess_id;
extern char D_0054EE18[]; /* "address" */
extern void *sdfChipAllocDirect(s32 aSize);
extern void sdfChipFreeDirect(void *aPtr);
extern void *dds3GetProcessData(dds3ProcessID_t *id);
extern void dds3SetProcessData(dds3ProcessID_t *id, void *data);
extern dds3ProcessID_t *dds3StartProcess(const char *v1, s32 dDS3_PRIORITY_DRAW_PB,
    s32 v2, s32 v3, dds3ProcessFunc_t drawExit,
    dds3ProcessFunc_t deleteExit, s32 p);
extern void dds3KillProcess(dds3ProcessID_t *id, s32 shutdowntimer);
extern s32 fldInputPanelProcess2(dds3ProcessID_t *id);
extern s32 fldAddressProcess2(dds3ProcessID_t *id);
extern s32 fldProcCampChk(void);
extern s32 fldProcAmChk(void);
extern s32 fmProcessChk(void);
extern s32 fldTitleChk(void);
extern void fldDrawAddressPanel();
extern void fldDrawAreaBlockFont();
extern s32 gFldElvFloorNow;
extern s32 fldCamMiwatasiChk(void);
extern s32 fldProcKiretaChk(void);
extern s32 fldProcHirakenaiChk(void);
extern void dds3FadeIn2(s32 frame);
extern s32 fldRunDanteGunFlag[];
extern s32 gFldInf_WpExit;
extern s8 D_005298C9[];
extern u32 fldInputPanelState[];
extern s32 D_003095B0[];
extern char D_003094F0[][16];
extern void fldDrawInputPanel(void);
extern void fldDrawInputCursor(s32 x, s32 y);
extern void fldKanjiDraw(s32 x, s32 y, u32 rgba, const char *moji);

s32 fldInputPanelProcess2(dds3ProcessID_t *id)
{
    fldInputPanelProcessData_t *work;

    if (fldProcCampChk() != 0) {
        return 0;
    }
    if (fldProcAmChk() != 0) {
        return 0;
    }
    if (fmProcessChk() != 0) {
        return 0;
    }
    if (fldCamMiwatasiChk() != 0) {
        return 0;
    }
    if (fldProcKiretaChk() != 0) {
        return 0;
    }
    if (fldProcHirakenaiChk() != 0) {
        return 0;
    }
    if (fldInputPanelState[0] != 0) {
        return 0;
    }
    work = dds3GetProcessData(id);
    if (work->Draw == 0) {
        goto ret0;
    }
    fldDrawInputPanel();
    fldDrawInputCursor(D_003095B0[work->Type] - 0xC, 0x132);
    fldKanjiDraw(D_003095B0[work->Type] + 0xA, 0x98, 0xA09DC380,
        D_003094F0[work->Type]);
    work->Count++;
    work->Draw = 0;
ret0:
    return 0;
}

s32 fldInputPanelProcess1(dds3ProcessID_t *id)
{
    fldInputPanelProcessData_t *work = sdfChipAllocDirect(8);

    work->Type = 1;
    work->Count = 0;
    work->Draw = 0;
    work->reserved = 0;
    dds3SetProcessData(id, work);
    return (s32)fldInputPanelProcess2;
}

void fldInputPanelShutdown(dds3ProcessID_t *id)
{
    void *work;

    work = dds3GetProcessData(id);
    sdfChipFreeDirect(work);
    inputpanel_id = 0;
}

void fldInitInputPanel(void)
{
    inputpanel_id = dds3StartProcess("inputpanel", 0x2B0B, 1, 1,
        fldInputPanelProcess1, (dds3ProcessFunc_t)fldInputPanelShutdown, 0);
}

void fldFreeInputPanel(void)
{
    if (inputpanel_id != 0) {
        dds3KillProcess(inputpanel_id, 1);
    }
}

fldInputPanelProcessData_t *fldInputPanelSetDraw(s32 flag)
{
    fldInputPanelProcessData_t *work;

    work = dds3GetProcessData(inputpanel_id);
    work->Draw = flag;
    return work;
}

fldInputPanelProcessData_t *fldInputPanelSetMessage(s32 no)
{
    fldInputPanelProcessData_t *work;

    work = dds3GetProcessData(inputpanel_id);
    work->Type = no;
    return work;
}

s32 fldInputPanelCheckKey(void)
{
    fldInputPanelProcessData_t *work;

    if (fldProcCampChk() != 0) {
        goto ret0;
    }
    if (fldProcAmChk() != 0) {
        return 0;
    }
    if (fldRunDanteGunFlag[0] != 0) {
        return 0;
    }
    if (fmProcessChk() != 0) {
        return 0;
    }
    if (fldCamMiwatasiChk() != 0) {
        return 0;
    }
    if (fldProcKiretaChk() != 0) {
        return 0;
    }
    if (fldProcHirakenaiChk() != 0) {
        return 0;
    }
    if (fldGlobalBase.word80 != 0) {
        return 0;
    }
    work = dds3GetProcessData(inputpanel_id);
    if (work->Draw == 0) {
        return -1;
    }
    if (fldGlobalBase.wordCC == 0) {
        if (gFldInf_WpExit != 1) {
            if (D_005298C9[0] >= 0) {
                goto ret0;
            }
        }
    }
    work->Draw = 0;
    if (fldGlobalBase.wordCC != 0) {
        dds3FadeIn2(8);
        fldGlobalBase.wordCC = 0;
    }
    return 1;
ret0:
    return 0;
}


s32 fldAddressProcess2(dds3ProcessID_t *id)
{
    if (fldProcCampChk() != 0) {
        return 0;
    }
    if (fldProcAmChk() != 0) {
        return 0;
    }
    if (fmProcessChk() != 0) {
        return 0;
    }
    if (fldTitleChk() != 0) {
        return 0;
    }
    if (gFldElvFloorNow != -999) {
        fldDrawAddressPanel(0x80);
    } else {
        fldDrawAreaBlockFont(0);
    }
    return 0;
}

s32 fldAddressProcess1(dds3ProcessID_t *id)
{
    fldAddressProcessData_t *work;

    work = sdfChipAllocDirect(0x10);
    work->unk00 = 0;
    work->unk04 = 0;
    work->unk08 = 0;
    work->unk0C = 0;
    dds3SetProcessData(id, work);
    return (s32)fldAddressProcess2;
}

void fldAddressShutdown(dds3ProcessID_t *id)
{
    void *work;

    work = dds3GetProcessData(id);
    sdfChipFreeDirect(work);
    addressprocess_id = 0;
}

void fldInitAddressProcess(void)
{
    if (addressprocess_id == 0) {
        addressprocess_id = dds3StartProcess(D_0054EE18, 0x2B0B, 1, 1,
            fldAddressProcess1, (dds3ProcessFunc_t)fldAddressShutdown, 0);
    }
}

void fldFreeAddressProcess(void)
{
    if (addressprocess_id != 0) {
        dds3KillProcess(addressprocess_id, 1);
    }
}

INCLUDE_SDATA(const s32, "field/fldPanel", addressprocess_id);

INCLUDE_SDATA(const s32, "field/fldPanel", inputpanel_id);

INCLUDE_SDATA(const s32, "field/fldPanel", D_0054EE18);

