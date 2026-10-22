#include "common.h"
#include "kernel.h"

typedef struct
{
    u8 unk00[0x2C];
    s32 statusTime;
} nbTarSelWork_t;

extern char D_005442B8[];
extern dds3ProcessID_t *dds3SearchProcessName(char *namestr);
extern void *dds3GetProcessData(dds3ProcessID_t *id);

nbTarSelWork_t *GetTarSelProcessData(void)
{
    return dds3GetProcessData(dds3SearchProcessName(D_005442B8));
}

INCLUDE_ASM(const s32, "newbattle/nbTarSelProcess", nbGetTarSelStat);

INCLUDE_ASM(const s32, "newbattle/nbTarSelProcess", nbSetTarSelStat);

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

INCLUDE_ASM(const s32, "newbattle/nbTarSelProcess", CursorSelectAuto);

INCLUDE_ASM(const s32, "newbattle/nbTarSelProcess", nbTarSelProcess1);

INCLUDE_ASM(const s32, "newbattle/nbTarSelProcess", nbTarSelShutdown);

INCLUDE_ASM(const s32, "newbattle/nbTarSelProcess", InitTarSelProcessData);

INCLUDE_ASM(const s32, "newbattle/nbTarSelProcess", nbInitTarSelProcess);

INCLUDE_RODATA(const s32, "newbattle/nbTarSelProcess", D_005442B8);

INCLUDE_RODATA(const s32, "newbattle/nbTarSelProcess", D_005442C8);

INCLUDE_RODATA(const s32, "newbattle/nbTarSelProcess", D_005442E8);

INCLUDE_RODATA(const s32, "newbattle/nbTarSelProcess", D_00544308);

INCLUDE_RODATA(const s32, "newbattle/nbTarSelProcess", D_00544328);

