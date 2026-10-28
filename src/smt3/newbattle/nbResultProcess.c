#include "common.h"
#include "nb.h"
#include "effBattle.h"

extern void fldSkyBattle(void);

INCLUDE_ASM(const s32, "newbattle/nbResultProcess", nbResultLoadTextures);

extern sdfTexHandle_t *nbResultTex1, *nbResultTex2, *nbResultTexHandle;
extern void sdfTexFreeDelayed(sdfTexHandle_t *aHandle);

void nbResultFreeTextures(void)
{
    sdfTexFreeDelayed(nbResultTex1);
    sdfTexFreeDelayed(nbResultTex2);
    sdfTexFreeDelayed(nbResultTexHandle);
    nbResultTex1 = NULL;
    nbResultTex2 = NULL;
    nbResultTexHandle = NULL;
}

INCLUDE_ASM(const s32, "newbattle/nbResultProcess", nbResultCountEntries);

INCLUDE_ASM(const s32, "newbattle/nbResultProcess", nbResultInitLabels);

INCLUDE_ASM(const s32, "newbattle/nbResultProcess", nbResultDrawPC1);

INCLUDE_ASM(const s32, "newbattle/nbResultProcess", nbResultInitTimer);

typedef struct nbResultScoreEntry_t {
    u32 occupied;
    u32 bestScore;
    u32 unknown08[3];
    u32 *work;
    u32 unknown18[5];
    u32 scoreOffset;
    u32 unknown30;
} nbResultScoreEntry_t;

extern nbResultScoreEntry_t nbResultScoreTable[];
extern s32 nbResultBestScore;

s32 nbResultFindBestScore(void)
{
    s32 found;
    nbResultScoreEntry_t *entry;
    s32 one;
    s32 index;

    found = 0;
    entry = nbResultScoreTable;
    one = 1;
    index = 3;
    do {
        if (entry->occupied != 0) {
            if (entry->bestScore < entry->work[4] + entry->scoreOffset) {
                found = one;
            }
        }
        entry++;
    } while (--index >= 0);
    if (found == 0 && nbResultBestScore < 0x3C) {
        nbResultBestScore++;
    }
    return nbResultBestScore;
}

INCLUDE_ASM(const s32, "newbattle/nbResultProcess", nbFindTobiwarpLabel);

INCLUDE_ASM(const s32, "newbattle/nbResultProcess", ShutdownProcess);

INCLUDE_ASM(const s32, "newbattle/nbResultProcess", nbResultUpdateUnits);

extern s32 rstCalcExpFromLevel(u8 index, s32 level);

s32 GetAddExp(s32 index, s32 cap)
{
    s32 exp = rstCalcExpFromLevel((u8)index, 0x100) - 1;
    s32 current = DDS3_GBWK->akuma[index].exp;

    exp -= current;
    if (exp < 0) {
        exp = 0;
    }
    if (cap < exp) {
        exp = cap;
    }
    return exp;
}

INCLUDE_ASM(const s32, "newbattle/nbResultProcess", nbResultAddExp);

INCLUDE_ASM(const s32, "newbattle/nbResultProcess", nbResultProcess2);

extern void ShutdownProcess(void);
extern s32 nbGetKakutokuExp(nbMainProcessData_t *data);
extern s32 nbResultStep;
extern s32 nbTobiwarpLabelIdx;
extern s32 D_005510E8;
extern s32 nbResultBestScore;
extern s32 nbResultProcess2(dds3ProcessID_t *id);

dds3ProcessFunc_t nb_result_Process(dds3ProcessID_t *id)
{
    nbMainProcessData_t *data = nbGetMainProcessData();

    ShutdownProcess();
    D_005510E8 = nbGetKakutokuExp(data);
    nbResultLoadTextures();
    nbTobiwarpLabelIdx = 0;
    nbResultBestScore = 0;
    nbResultStep = 0;
    return nbResultProcess2;
}

extern void nbStageFilterAlp(s32 alpha);

void nb_result_Shutdown(void)
{
    nbMainProcessData_t *data = nbGetMainProcessData();

    data->ResultProcessID = NULL;
    nbResultFreeTextures();
    nbStageFilterAlp(0);
}

extern dds3ProcessID_t *dds3StartProcess(char *name, u32 priority, s32 a, s32 b,
                                          dds3ProcessFunc_t process, dds3ProcessFunc_t shutdown,
                                          void *work);
extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *child);

void nbInitResultProcess(nbMainProcessData_t *data)
{
    dds3ProcessID_t *process = dds3StartProcess("nb_result", 0x3F9, 1, 1,
        (dds3ProcessFunc_t)nb_result_Process, (dds3ProcessFunc_t)nb_result_Shutdown, NULL);
    dds3LinkProcess(data->MainProcessID, process);
    data->ResultProcessID = process;
}

void nbLoadStageSky(void)
{
    fldSkyBattle();
}

INCLUDE_RODATA(const s32, "newbattle/nbResultProcess", D_00543618);

INCLUDE_RODATA(const s32, "newbattle/nbResultProcess", D_00543628);

INCLUDE_RODATA(const s32, "newbattle/nbResultProcess", D_00543648);

INCLUDE_RODATA(const s32, "newbattle/nbResultProcess", D_00543668);

INCLUDE_RODATA(const s32, "newbattle/nbResultProcess", D_00543678);

INCLUDE_RODATA(const s32, "newbattle/nbResultProcess", D_00543688);

INCLUDE_RODATA(const s32, "newbattle/nbResultProcess", D_00543698);

