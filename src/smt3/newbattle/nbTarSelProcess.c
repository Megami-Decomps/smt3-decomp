#include "common.h"
#include "kernel.h"
#include "nb.h"

struct nbParty_t {
    u16 flag;
    s16 formindex;
    s16 statindex;
    u16 partyindex;
    s16 count[20];
    u32 aisyo[16];
    u16 hojoskill[8];
};

typedef struct nbTarSelProcessData_t {
    nbMainProcessData_t *data;
    void *act;
    u8 stat;
    s8 ctype;
    u8 crule;
    u8 carea;
    u8 nowcarea;
    s8 nowno;
    s8 nowform;
    u8 unk0F;
    s16 partyindex;
    s8 friendcnt;
    s8 enemycnt;
    s8 friendno[12];
    s8 enemyno[12];
    s32 statusTime;
    s32 time;
    s32 alpha_a;
    s32 alpha_b;
    f32 scale_a;
    f32 scale_b;
} nbTarSelProcessData_t;

extern char D_005442B8[];
extern dds3ProcessID_t *dds3SearchProcessName(char *namestr);
extern void *dds3GetProcessData(dds3ProcessID_t *id);
extern void sdfChipFreeDirect(void *ptr);
extern datUnitWork_t *nbGetUnitWorkPointer(nbMainProcessData_t *data, struct nbParty_t *party);
extern void *sdfChipAllocDirect(u32 size);
extern void *memset(void *dest, s32 value, u32 size);
extern dds3ProcessID_t *dds3StartProcess(char *name, u32 priority, s32 mode,
                                         s32 flag, dds3ProcessFunc_t process,
                                         dds3ProcessFunc_t shutdown, void *data);
extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);
extern s32 nbTarSelProcess1(dds3ProcessID_t *id);
extern nbTarSelProcessData_t *InitTarSelProcessData(nbMainProcessData_t *data);
extern void MemoryCommand_nbTarSelProcess(nbTarSelProcessData_t *process);

nbTarSelProcessData_t *GetTarSelProcessData(void)
{
    return dds3GetProcessData(dds3SearchProcessName(D_005442B8));
}

s32 nbGetTarSelStat(void)
{
    return GetTarSelProcessData()->stat;
}

void nbSetTarSelStat(s32 stat)
{
    GetTarSelProcessData()->stat = stat;
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

extern s32 nbSetSelectOkMark(s32 partyindex, s32 ctype, s32 crule, s32 carea);
extern void SetCursor(nbTarSelProcessData_t *process);
extern void UpdateSelect(nbTarSelProcessData_t *process);

s32 nbSetTargetProcess(void *act, s32 partyindex, s32 ctype, s32 crule, s32 carea)
{
    nbTarSelProcessData_t *process = GetTarSelProcessData();

    if (!nbSetSelectOkMark(partyindex, ctype, crule, carea)) {
        return 0;
    }

    process->alpha_a = 0x80;
    process->alpha_b = 0x80;
    process->scale_a = 1.0f;
    process->scale_b = 1.0f;
    process->act = act;
    process->partyindex = partyindex;
    process->ctype = ctype;
    process->crule = crule;
    process->carea = carea;
    process->statusTime = 0;
    process->time = 0;
    SetCursor(process);
    UpdateSelect(process);
    process->stat = 1;
    return 1;
}

s32 CheckFriend(nbTarSelProcessData_t *t, s32 formindex)
{
    if ((t->data->stat & 8) != 0) {
        if ((u32)(formindex - 4) < 0xA) {
            return 1;
        }
    } else if (formindex < 4) {
        return 1;
    }
    return 0;
}

s32 CheckEnemy(nbTarSelProcessData_t *t, s32 formindex)
{
    if ((t->data->stat & 8) != 0) {
        if (formindex < 4) {
            return 1;
        }
    } else if ((u32)(formindex - 4) < 0xA) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbTarSelProcess", UpdateSelect);

INCLUDE_ASM(const s32, "newbattle/nbTarSelProcess", OnOffTargetCursor);

INCLUDE_ASM(const s32, "newbattle/nbTarSelProcess", DispTargetHelp);

void MemoryCommand_nbTarSelProcess(nbTarSelProcessData_t *process)
{
    nbMainProcessData_t *data = process->data;
    datUnitWork_t *work = nbGetUnitWorkPointer(data, &data->party[process->partyindex]);

    work->nowtarea = process->nowcarea;
    work->nowtform = process->nowform;
}

INCLUDE_ASM(const s32, "newbattle/nbTarSelProcess", CursorSelect);

void CursorSelectAuto(nbTarSelProcessData_t *process)
{
    if (process->stat == 1) {
        MemoryCommand_nbTarSelProcess(process);
        process->stat = 0;
    }
}

INCLUDE_ASM(const s32, "newbattle/nbTarSelProcess", nbTarSelProcess1);

void nbTarSelShutdown(dds3ProcessID_t *id)
{
    nbTarSelProcessData_t *process = dds3GetProcessData(id);

    process->data->tarSelProcess = 0;
    sdfChipFreeDirect(process);
}

nbTarSelProcessData_t *InitTarSelProcessData(nbMainProcessData_t *data)
{
    nbTarSelProcessData_t *process = sdfChipAllocDirect(sizeof(*process));

    memset(process, 0, sizeof(*process));
    process->act = NULL;
    process->data = data;
    process->alpha_a = 0x80;
    process->scale_a = 1.0f;
    process->alpha_b = 0x80;
    process->scale_b = 1.0f;
    process->stat = 0;
    return process;
}

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

