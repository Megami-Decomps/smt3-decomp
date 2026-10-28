#include "common.h"
#include "nb.h"
#include "effBattle.h"

nbMainProcessData_t *nbGetMainProcessData(void);

extern dds3ProcessID_t *dds3StartProcess(char *name, u32 priority, s32 startTimer,
                                         s32 shutdownTimer,
                                         dds3ProcessFunc_t processFunc,
                                         dds3ProcessFunc_t shutdownFunc, void *workData);
extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);
extern s32 nbAtoProcess1(dds3ProcessID_t *id);
extern char D_005510C8[];

INCLUDE_ASM(const s32, "newbattle/nbAtoProcess", nbAtoStoneProcess);

INCLUDE_ASM(const s32, "newbattle/nbAtoProcess", nbAtoGameoverProcess);

INCLUDE_ASM(const s32, "newbattle/nbAtoProcess", nbAtoGameover2Process);

INCLUDE_ASM(const s32, "newbattle/nbAtoProcess", nbAtoProcess1);

void nbAtoShutdown(void)
{
    nbGetMainProcessData()->AtoProcessID = 0;
}

void nbInitAtoProcess(nbMainProcessData_t *data)
{
    dds3ProcessID_t *id;

    id = dds3StartProcess(D_005510C8, 0x3F9, 1, 1, nbAtoProcess1,
                          (dds3ProcessFunc_t)nbAtoShutdown, NULL);
    dds3LinkProcess(data->MainProcessID, id);
    data->AtoProcessID = id;
}

INCLUDE_ASM(const s32, "newbattle/nbAtoProcess", dds3LoadBootFile);

