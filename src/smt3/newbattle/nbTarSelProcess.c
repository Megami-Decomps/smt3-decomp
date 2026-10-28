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

/* Target selection is started with an nbActionProcessData_t from the normal
 * action path; the debug path passes NULL. Only its leading state word is
 * read here. */
typedef struct nbActionProcessData_t {
    nbMainProcessData_t *data;
    u32 stat;
} nbActionProcessData_t;

typedef struct nbTarSelProcessData_t {
    nbMainProcessData_t *data;
    nbActionProcessData_t *act;
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

extern void nbPnlSprite(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh,
                        u32 rgba, sdfTexHandle_t *texture);

void nbDrawTargetCursor(nbMainProcessData_t *data, s32 x, s32 y)
{
    nbTarSelProcessData_t *process = GetTarSelProcessData();
    s32 size;
    u32 colorMask = 0xFFFFFF;
    u32 color;

    size = (s32)(process->scale_a * 53.0f);
    color = ((u32)process->alpha_a << 24) | 0x808080;
    nbPnlSprite(x - size / 2, y - size / 2, size, size, 0xCA, 0x15, 0x35, 0x35,
                color, data->pnltex[3]);

    size = (s32)(process->scale_b * 53.0f);
    color = (color & colorMask) | ((u32)process->alpha_b << 24);
    nbPnlSprite(x - size / 2, y - size / 2, size, size, 0xCA, 0x15, 0x35, 0x35,
                color, data->pnltex[3]);
}

INCLUDE_ASM(const s32, "newbattle/nbTarSelProcess", CheckFormCursor);

extern s32 CheckFormCursor(s32 targetForm, s32 ctype, s32 crule, s32 carea, s32 formindex);

s32 nbSetSelectOkMark(s32 partyindex, s32 ctype, s32 crule, s32 carea)
{
    nbTarSelProcessData_t *process = GetTarSelProcessData();
    s32 i;
    s32 j;
    s32 a;
    s32 b;
    f32 xa;
    f32 xb;

    process->friendcnt = 0;
    process->enemycnt = 0;
    for (i = 0; i < 16; i++) {
        process->data->form[i].stat &= ~4u;
        process->data->form[i].stat &= ~8u;
        if (CheckFormCursor(process->data->party[partyindex].formindex,
                            ctype, crule, carea, i) == 0) {
            continue;
        }
        process->data->form[i].stat |= 4;

        if (i < 4) {
            if ((process->data->stat & 8) != 0) {
                process->enemyno[process->enemycnt] = i;
                process->enemycnt++;
            } else {
                process->friendno[process->friendcnt] = i;
                process->friendcnt++;
            }
        } else if (i < 14) {
            if ((process->data->stat & 8) != 0) {
                process->friendno[process->friendcnt] = i;
                process->friendcnt++;
            } else {
                process->enemyno[process->enemycnt] = i;
                process->enemycnt++;
            }
        }
    }

    i = 0;
    while (i < process->enemycnt - 1) {
        j = i + 1;
        while (j < process->enemycnt) {
            b = process->enemyno[j];
            a = process->enemyno[i];
            xa = process->data->form[a].orgpos.x;
            xb = process->data->form[b].orgpos.x;
            if (xb < xa) {
                process->enemyno[i] = process->enemyno[j];
                process->enemyno[j] = a;
            }
            j++;
        }
        i++;
    }

    return process->friendcnt + process->enemycnt != 0;
}

extern s32 dds3GetGlobalConfigFlag(s32 index);

void SetCursor(nbTarSelProcessData_t *process)
{
    datUnitWork_t *work;
    s32 previousArea;
    s32 previousForm;
    s32 i;

    if (process->friendcnt > 0) {
        process->nowcarea = 1;
    } else {
        process->nowcarea = 2;
    }
    process->nowno = 0;
    work = nbGetUnitWorkPointer(process->data,
                                &process->data->party[process->partyindex]);
    previousArea = work->prevtarea;
    previousForm = work->prevtform;
    if (process->act != NULL && dds3GetGlobalConfigFlag(1) == 0 &&
        (process->act->stat & 0x80) == 0) {
        previousArea = -1;
        previousForm = -1;
    }
    switch (previousArea) {
    case 1:
        if (process->friendcnt > 0) {
            process->nowcarea = previousArea;
            for (i = 0; i < process->friendcnt; i++) {
                if (process->friendno[i] == previousForm) {
                    process->nowno = i;
                    break;
                }
            }
        }
        break;
    case 2:
        if (process->enemycnt > 0) {
            process->nowcarea = previousArea;
            for (i = 0; i < process->enemycnt; i++) {
                if (process->enemyno[i] == previousForm) {
                    process->nowno = i;
                    break;
                }
            }
        }
        break;
    }
}


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

extern datUnitWork_t *nbGetUnitWorkFromFormindex(s32 index);
extern void nbDispDevilName(s32 id);

void DispTargetHelp(nbTarSelProcessData_t *process)
{
    s32 found = 0;
    s32 state = process->stat;
    s32 i;
    datUnitWork_t *unit;

    for (i = 0; i < 16; i++) {
        if (state == 1 && process->ctype == 0 &&
            (process->data->form[i].anime & 8) != 0) {
            found = 1;
            break;
        }
    }
    if (found != 0 && process->statusTime == 0) {
        unit = nbGetUnitWorkFromFormindex(i);
        if (unit != NULL) {
            nbDispDevilName(unit->id);
        }
    }
    if (process->statusTime > 0) {
        process->statusTime--;
    }
}

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

extern void CursorSelect(nbTarSelProcessData_t *process);
extern void AnimeTargetCursor(nbTarSelProcessData_t *process);
extern void OnOffTargetCursor(nbTarSelProcessData_t *process);

s32 nbTarSelProcess1(dds3ProcessID_t *id)
{
    nbTarSelProcessData_t *process = dds3GetProcessData(id);
    s32 automatic = 0;
    s32 resetState = 0;

    if (process->act != NULL && (process->act->stat & 0x80) != 0) {
        automatic = 1;
    }
    if (automatic != 0) {
        CursorSelectAuto(process);
    } else if (process->stat == 1) {
        CursorSelect(process);
        if (process->stat == 0) {
            process->stat = 1;
            resetState = 1;
        }
    }
    AnimeTargetCursor(process);
    OnOffTargetCursor(process);
    DispTargetHelp(process);
    if (resetState != 0) {
        process->stat = 0;
    }
    return 0;
}

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

