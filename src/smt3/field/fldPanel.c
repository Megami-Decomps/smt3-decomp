#include "common.h"
#include "kernel.h"
#include "basic.h"
#include "field.h"

/* The input panel keeps its cursor, message choice and draw request as halfwords. */
typedef struct
{
    u16 position;
    u16 message;
    u16 draw;
    u16 reserved;
} fldInputPanelWork_t;

typedef struct
{
    s32 unk00;
    s32 unk04;
    s32 unk08;
    s32 unk0C;
} fldAddressWork_t;

extern dds3ProcessID_t *inputpanel_id;
extern dds3ProcessID_t *addressprocess_id;
extern char D_0054EE18[]; /* "address" */
extern void *sdfChipAllocDirect(s32 size);
extern void sdfChipFreeDirect(void *block);
extern void *dds3GetProcessData(dds3ProcessID_t *process);
extern void dds3SetProcessData(dds3ProcessID_t *process, void *data);
extern dds3ProcessID_t *dds3StartProcess(const char *name, s32 priority,
    s32 startTimer, s32 shutdownTimer, dds3ProcessFunc_t process,
    dds3ProcessFunc_t shutdown, s32 flag);
extern void dds3KillProcess(dds3ProcessID_t *process, s32 killChildren);
extern s32 fldInputPanelProcess2(dds3ProcessID_t *process);
extern s32 fldAddressProcess2(dds3ProcessID_t *process);
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
extern void fldKanjiDraw(s32 x, s32 y, u32 colors, const char *text);

s32 fldInputPanelProcess2(dds3ProcessID_t *process)
{
    fldInputPanelWork_t *work;

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
    work = dds3GetProcessData(process);
    if (work->draw == 0) {
        goto ret0;
    }
    fldDrawInputPanel();
    fldDrawInputCursor(D_003095B0[work->message] - 0xC, 0x132);
    fldKanjiDraw(D_003095B0[work->message] + 0xA, 0x98, 0xA09DC380,
        D_003094F0[work->message]);
    work->position++;
    work->draw = 0;
ret0:
    return 0;
}

s32 fldInputPanelProcess1(dds3ProcessID_t *process)
{
    fldInputPanelWork_t *work = sdfChipAllocDirect(8);

    work->message = 1;
    work->position = 0;
    work->draw = 0;
    work->reserved = 0;
    dds3SetProcessData(process, work);
    return (s32)fldInputPanelProcess2;
}

void fldInputPanelShutdown(dds3ProcessID_t *process)
{
    void *work;

    work = dds3GetProcessData(process);
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

fldInputPanelWork_t *fldInputPanelSetDraw(s32 draw)
{
    fldInputPanelWork_t *work;

    work = dds3GetProcessData(inputpanel_id);
    work->draw = draw;
    return work;
}

fldInputPanelWork_t *fldInputPanelSetMessage(s32 message)
{
    fldInputPanelWork_t *work;

    work = dds3GetProcessData(inputpanel_id);
    work->message = message;
    return work;
}

s32 fldInputPanelCheckKey(void)
{
    fldInputPanelWork_t *work;

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
    if (work->draw == 0) {
        return -1;
    }
    if (fldGlobalBase.wordCC == 0) {
        if (gFldInf_WpExit != 1) {
            if (D_005298C9[0] >= 0) {
                goto ret0;
            }
        }
    }
    work->draw = 0;
    if (fldGlobalBase.wordCC != 0) {
        dds3FadeIn2(8);
        fldGlobalBase.wordCC = 0;
    }
    return 1;
ret0:
    return 0;
}


s32 fldAddressProcess2(dds3ProcessID_t *process)
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

s32 fldAddressProcess1(dds3ProcessID_t *process)
{
    fldAddressWork_t *work;

    work = sdfChipAllocDirect(0x10);
    work->unk00 = 0;
    work->unk04 = 0;
    work->unk08 = 0;
    work->unk0C = 0;
    dds3SetProcessData(process, work);
    return (s32)fldAddressProcess2;
}

void fldAddressShutdown(dds3ProcessID_t *process)
{
    void *work;

    work = dds3GetProcessData(process);
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

