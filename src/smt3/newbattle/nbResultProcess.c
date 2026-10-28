#include "common.h"
#include "nb.h"
#include "effBattle.h"

/* Four native 0x34-byte rows; member names follow typeNbResultWork. */
typedef struct nbResultScore_t {
    s32 useflg;
    s32 exp_cnt;
    s32 l_lv;
    s32 lvupflg;
    s32 unitidx;
    datUnitWork_t *unit;
    s32 lvupanmflg;
    s32 lvupanmcnt;
    s32 lvuplvanmcnt;
    s32 ofsx;
    s32 stopcnt;
    s32 addexp;
    s32 resultadd;
} nbResultScore_t;

extern nbResultScore_t nbResultScoreTable[4];
extern s32 nbResultBestScore;

typedef struct nbResultCaption_t {
    s32 flag;
    s32 x;
    s32 y;
    s32 life;
} nbResultCaption_t;

extern nbResultCaption_t D_00574F50[64];
extern u32 nbResultTimer;
extern u32 nbResultCounter;
extern u8 rstCalcLevelFromExp(u8 index, u32 exp);
extern void nbResultCountEntries(s32 x, s32 y);
extern void p2sdPlaySE(s32 no, s32 volume, s32 pan);

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

void nbResultCountEntries(s32 x, s32 y)
{
    s32 i;

    for (i = 0; i < 64; i++) {
        if (D_00574F50[i].flag == 0) {
            D_00574F50[i].flag = 1;
            D_00574F50[i].x = x;
            D_00574F50[i].y = y;
            D_00574F50[i].life = 20;
            return;
        }
    }
}

INCLUDE_ASM(const s32, "newbattle/nbResultProcess", nbResultInitLabels);

INCLUDE_ASM(const s32, "newbattle/nbResultProcess", nbResultDrawPC1);

void nbResultInitTimer(void)
{
    s32 i;
    s32 oldLevel;
    s32 index;

    nbResultTimer = 900;
    nbResultCounter = 0;
    for (i = 0; i < 4; i++) {
        if (nbResultScoreTable[i].useflg != 0) {
            index = (u8)nbResultScoreTable[i].unitidx;
            oldLevel = rstCalcLevelFromExp(index, nbResultScoreTable[i].exp_cnt);
            nbResultScoreTable[i].exp_cnt = nbResultScoreTable[i].unit->exp + nbResultScoreTable[i].addexp;
            nbResultScoreTable[i].l_lv = rstCalcLevelFromExp(index, nbResultScoreTable[i].exp_cnt);
            if (oldLevel < nbResultScoreTable[i].l_lv) {
                nbResultScoreTable[i].lvupflg = 1;
                nbResultCounter |= 2;
                nbResultCountEntries(317, 227 + i * 40);
            }
        }
    }
    if (nbResultCounter & 2) {
        p2sdPlaySE(17, 127, 63);
    }
    nbResultCounter = 0;
}

s32 nbResultFindBestScore(void)
{
    nbResultScore_t *entry;
    s32 pending;
    s32 i;

    entry = nbResultScoreTable;
    pending = 0;
    for (i = 0; i < 4; i++, entry++) {
        if (entry->useflg != 0) {
            if (entry->exp_cnt < entry->unit->exp + entry->addexp) {
                pending = 1;
            }
        }
    }
    if (pending == 0 && nbResultBestScore < 60) {
        nbResultBestScore++;
    }
    return nbResultBestScore;
}

INCLUDE_ASM(const s32, "newbattle/nbResultProcess", nbFindTobiwarpLabel);

extern s32 dds3KillProcess(dds3ProcessID_t *id, s32 timer);

void ShutdownProcess(void)
{
    nbMainProcessData_t *data;

    data = nbGetMainProcessData();
    if (data->autoCommProcess != NULL) {
        dds3KillProcess(data->autoCommProcess, 1);
        data->autoCommProcess = NULL;
    }
    if (data->camera != NULL) {
        dds3KillProcess(data->camera, 1);
        data->camera = NULL;
    }
    if (data->EffectProcessID != NULL) {
        dds3KillProcess(data->EffectProcessID, 1);
        data->EffectProcessID = NULL;
    }
    if (data->UnitProcessID != NULL) {
        dds3KillProcess(data->UnitProcessID, 1);
        data->UnitProcessID = NULL;
    }
    if (data->helpProcess != NULL) {
        dds3KillProcess(data->helpProcess, 1);
        data->helpProcess = NULL;
    }
    if (data->panelProcess != NULL) {
        dds3KillProcess(data->panelProcess, 1);
        data->panelProcess = NULL;
    }
}

typedef struct nbParty_t {
    u16 flag;
    s16 formindex;
    s16 statindex;
    u16 partyindex;
    s16 count[20];
    u32 aisyo[16];
    u16 hojoskill[8];
} nbParty_t;

extern void pacFileHandleFree_(struct NB_FILEHANDLE_T *handle);
extern void evtFreeStage(void);

void nbResultUpdateUnits(void)
{
    nbMainProcessData_t *data;
    nbParty_t *party;
    s32 i;

    data = nbGetMainProcessData();
    for (i = 0; i < 32; i++) {
        if (data->form[i].stat & 1) {
            if (data->form[i].packh != NULL) {
                pacFileHandleFree_(data->form[i].packh);
                data->form[i].packh = NULL;
            }
        }
        data->form[i].stat = 0;
    }
    party = data->party;
    for (i = 0; i < 14; i++, party++) {
        party->flag |= 0x28;
    }
    evtFreeStage();
}

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

