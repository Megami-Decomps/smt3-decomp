#include "common.h"
#include "kernel.h"

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

INCLUDE_ASM(const s32, "field/fldPanel", fldInputPanelProcess2);

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

INCLUDE_ASM(const s32, "field/fldPanel", fldInputPanelCheckKey);

INCLUDE_ASM(const s32, "field/fldPanel", fldAddressProcess2);

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

