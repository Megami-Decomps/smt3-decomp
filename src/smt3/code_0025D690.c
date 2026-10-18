#include "common.h"
#include "kernel.h"

typedef struct
{
    u8 unk00[0x2C];
    s32 statusTime;
} nbTarSelWork_t;

extern char D_005442B8[];
extern dds3ProcessID_t *dds3SearchProcessName(char *name);
extern void *dds3GetProcessData(dds3ProcessID_t *process);

nbTarSelWork_t *GetTarSelProcessData(void)
{
    return dds3GetProcessData(dds3SearchProcessName(D_005442B8));
}

INCLUDE_ASM(const s32, "code_0025D690", nbGetTarSelStat);

INCLUDE_ASM(const s32, "code_0025D690", nbSetTarSelStat);

void nbSetTarSelJyokyoTime(s32 time)
{
    GetTarSelProcessData()->statusTime = time;
}

INCLUDE_ASM(const s32, "code_0025D690", AnimeTargetCursor);

INCLUDE_ASM(const s32, "code_0025D690", nbDrawTargetCursor);

INCLUDE_ASM(const s32, "code_0025D690", CheckFormCursor);

INCLUDE_ASM(const s32, "code_0025D690", nbSetSelectOkMark);

INCLUDE_ASM(const s32, "code_0025D690", SetCursor);

INCLUDE_ASM(const s32, "code_0025D690", nbSetTargetProcess);

INCLUDE_ASM(const s32, "code_0025D690", CheckFriend);

INCLUDE_ASM(const s32, "code_0025D690", CheckEnemy);

INCLUDE_ASM(const s32, "code_0025D690", UpdateSelect);

INCLUDE_ASM(const s32, "code_0025D690", OnOffTargetCursor);

INCLUDE_ASM(const s32, "code_0025D690", DispTargetHelp);

INCLUDE_ASM(const s32, "code_0025D690", MemoryCommand_nbTarSelProcess);

INCLUDE_ASM(const s32, "code_0025D690", CursorSelect);

INCLUDE_ASM(const s32, "code_0025D690", CursorSelectAuto);

INCLUDE_ASM(const s32, "code_0025D690", nbTarSelProcess1);

INCLUDE_ASM(const s32, "code_0025D690", nbTarSelShutdown);

INCLUDE_ASM(const s32, "code_0025D690", InitTarSelProcessData);

INCLUDE_ASM(const s32, "code_0025D690", nbInitTarSelProcess);

INCLUDE_RODATA(const s32, "code_0025D690", D_005442B8);

INCLUDE_RODATA(const s32, "code_0025D690", D_005442C8);

INCLUDE_RODATA(const s32, "code_0025D690", D_005442E8);

INCLUDE_RODATA(const s32, "code_0025D690", D_00544308);

INCLUDE_RODATA(const s32, "code_0025D690", D_00544328);

