#include "common.h"
#include "kernel.h"
#include "nb.h"

typedef struct
{
    nbMainProcessData_t *mainData;
    u32 pad_004;
    u8 state;
    u8 pad_009[3];
    u8 targetArea;
    u8 pad_00D;
    s8 targetForm;
    u8 pad_00F;
    s16 partyIndex;
    u8 pad_012[0x1A];
    s32 statusTime;
    u8 pad_030[4];
    u32 pad_034;
    u32 pad_038;
    f32 pad_03C;
    f32 pad_040;
} nbTarSelProcessData_t;

extern char D_005442B8[];
extern dds3ProcessID_t *dds3SearchProcessName(char *namestr);
extern void *dds3GetProcessData(dds3ProcessID_t *id);
extern void sdfChipFreeDirect(void *ptr);
extern dds3ProcessID_t *dds3StartProcess(char *name, u32 priority, s32 mode,
                                         s32 flag, dds3ProcessFunc_t process,
                                         dds3ProcessFunc_t shutdown, void *data);
extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);
extern s32 nbTarSelProcess1(dds3ProcessID_t *id);
extern nbTarSelProcessData_t *InitTarSelProcessData(nbMainProcessData_t *data);
extern datUnitWork_t *MemoryCommand_nbTarSelProcess(nbTarSelProcessData_t *process);

nbTarSelProcessData_t *GetTarSelProcessData(void)
{
    return dds3GetProcessData(dds3SearchProcessName(D_005442B8));
}

s32 nbGetTarSelStat(void)
{
    return GetTarSelProcessData()->state;
}

void nbSetTarSelStat(s32 stat)
{
    GetTarSelProcessData()->state = stat;
}

void nbSetTarSelJyokyoTime(s32 time)
{
    GetTarSelProcessData()->statusTime = time;
}

INCLUDE_ASM(const s32, "newbattle/nbTarSelProcess", AnimeTargetCursor);

INCLUDE_ASM(const s32, "newbattle/nbTarSelProcess", nbDrawTargetCursor);

INCLUDE_ASM(const s32, "newbattle/nbTarSelProcess", CheckFormCursor);

INCLUDE_ASM(const s32, "newbattle/nbTarSelProcess", nbSetSelectOkMark);

INCLUDE_ASM(const s32, "newbattle/nbTarSelProcess", SetCursor);

INCLUDE_ASM(const s32, "newbattle/nbTarSelProcess", nbSetTargetProcess);

INCLUDE_ASM(const s32, "newbattle/nbTarSelProcess", CheckFriend);

INCLUDE_ASM(const s32, "newbattle/nbTarSelProcess", CheckEnemy);

INCLUDE_ASM(const s32, "newbattle/nbTarSelProcess", UpdateSelect);

INCLUDE_ASM(const s32, "newbattle/nbTarSelProcess", OnOffTargetCursor);

INCLUDE_ASM(const s32, "newbattle/nbTarSelProcess", DispTargetHelp);

INCLUDE_ASM(const s32, "newbattle/nbTarSelProcess", MemoryCommand_nbTarSelProcess);

INCLUDE_ASM(const s32, "newbattle/nbTarSelProcess", CursorSelect);

void CursorSelectAuto(nbTarSelProcessData_t *process)
{
    if (process->state == 1) {
        MemoryCommand_nbTarSelProcess(process);
        process->state = 0;
    }
}

INCLUDE_ASM(const s32, "newbattle/nbTarSelProcess", nbTarSelProcess1);

void nbTarSelShutdown(dds3ProcessID_t *id)
{
    nbTarSelProcessData_t *process = dds3GetProcessData(id);

    process->mainData->tarSelProcess = 0;
    sdfChipFreeDirect(process);
}

INCLUDE_ASM(const s32, "newbattle/nbTarSelProcess", InitTarSelProcessData);

void nbInitTarSelProcess(nbMainProcessData_t *data)
{
    nbTarSelProcessData_t *process = InitTarSelProcessData(data);
    dds3ProcessID_t *id = dds3StartProcess(D_005442B8, 0x3F9, 1, 1,
                                           nbTarSelProcess1,
                                           (dds3ProcessFunc_t)nbTarSelShutdown,
                                           process);

    dds3LinkProcess(data->MainProcessID, id);
    data->tarSelProcess = id;
}

INCLUDE_RODATA(const s32, "newbattle/nbTarSelProcess", D_005442B8);

INCLUDE_RODATA(const s32, "newbattle/nbTarSelProcess", D_005442C8);

INCLUDE_RODATA(const s32, "newbattle/nbTarSelProcess", D_005442E8);

INCLUDE_RODATA(const s32, "newbattle/nbTarSelProcess", D_00544308);

INCLUDE_RODATA(const s32, "newbattle/nbTarSelProcess", D_00544328);

