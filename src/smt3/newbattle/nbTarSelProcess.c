#include "common.h"
#include "newbattle.h"
#include "kernel.h"
#include "nb.h"



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

#ifdef NON_MATCHING
void AnimeTargetCursor(nbTarSelProcessData_t *process)
{
    s32 time = (s32)((u32)process->time + 1);
    s32 alphaA[7] = { 25, 31, 38, 64, 52, 40, 28 };
    s32 alphaB[7] = { 128, 102, 77, 89, 102, 114, 128 };
    f32 scaleA[7] = { 1.0f, 1.15f, 1.3f, 0.5f,
                      0.65999998f, 0.84999998f, 1.0f };
    f32 scaleB[7] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
    s32 index;
    f32 rate;
    f32 inverse;

    process->time = time;
    if (time >= 30) {
        process->time = 0;
    }
    time = process->time;
    index = time / 5;
    rate = (f32)(time % 5) / 5.0f;
    inverse = 1.0f - rate;
    process->alpha_a = (s32)((f32)alphaA[index] * inverse + (f32)alphaA[index + 1] * rate);
    process->alpha_b = (s32)((f32)alphaB[index] * inverse + (f32)alphaB[index + 1] * rate);
    process->scale_b = scaleB[index] * inverse + scaleB[index + 1] * rate;
    process->scale_a = scaleA[index] * inverse + scaleA[index + 1] * rate;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbTarSelProcess", AnimeTargetCursor);
#endif

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

#ifdef NON_MATCHING
s32 CheckFormCursor(s32 targetForm, s32 ctype, s32 crule, s32 carea, s32 formindex)
{
    nbMainProcessData_t *data = GetTarSelProcessData()->data;
    nbFormation_t *form = &data->form[formindex];
    s32 reversed = (data->stat & 8) != 0;
    s32 allowed = 1;
    u32 stat;

    if (ctype != 2) {
        allowed = 0;
        if ((carea & 3) == 3) {
            allowed = 1;
        } else if (carea & 1) {
            allowed = reversed ? (u32)formindex - 4 < 10 : formindex < 4;
        } else if (carea & 2) {
            allowed = reversed ? formindex < 4 : (u32)formindex - 4 < 10;
        }
    }
    if (allowed && (crule & 1)) {
        allowed = targetForm == formindex;
    }
    if (allowed && (crule & 2)) {
        allowed = targetForm != formindex;
    }
    if (allowed) {
        stat = form->stat;
        if (!(stat & 1)) {
            allowed = (crule & 4) != 0 && (stat & 0x800) != 0;
        }
        if (allowed && (crule & 8)) {
            allowed = (stat & 1) == 0;
        }
        if (allowed && (stat & 0x40) && (data->party[form->partyindex].flag & 0x38)) {
            allowed = 0;
        }
    }
    return allowed;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbTarSelProcess", CheckFormCursor);
#endif

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

#ifdef NON_MATCHING
void UpdateSelect(nbTarSelProcessData_t *process)
{
    nbFormation_t *form;
    u32 stat;
    s32 selected;
    s32 i;

    for (i = 0; i < 16; i++) {
        form = &process->data->form[i];
        stat = form->stat;
        form->stat = stat & ~8u;
        if (stat & 4) {
            selected = 0;
            switch (process->ctype) {
            case 0:
                if ((process->nowcarea == 1 && process->friendno[process->nowno] == i) ||
                    (process->nowcarea == 2 && process->enemyno[process->nowno] == i)) {
                    process->nowform = i;
                    selected = 1;
                }
                break;
            case 1:
                if (process->nowcarea == 1) {
                    selected = CheckFriend(process, i);
                } else if (process->nowcarea == 2) {
                    selected = CheckEnemy(process, i);
                }
                break;
            case 2:
                selected = 1;
                break;
            }
            if (selected) {
                process->data->form[i].stat |= 8;
            }
        }
    }
}
#else
INCLUDE_ASM(const s32, "newbattle/nbTarSelProcess", UpdateSelect);
#endif

#ifdef NON_MATCHING
void OnOffTargetCursor(nbTarSelProcessData_t *process)
{
    volatile nbTarSelProcessData_t *selection = process;
    volatile nbMainProcessData_t *data = selection->data;
    s32 i;

    for (i = 0; i < 16; i++) {
        if (selection->stat == 1 && (data->form[i].stat & 8) != 0) {
            data->form[i].anime |= 8;
        } else {
            data->form[i].anime &= ~8;
        }
    }
}
#else
INCLUDE_ASM(const s32, "newbattle/nbTarSelProcess", OnOffTargetCursor);
#endif

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

extern u8 dds3PadButtonState[64];
extern void nbPlaySe(s32 no);

#ifdef NON_MATCHING
void CursorSelect(nbTarSelProcessData_t *process)
{
    volatile nbTarSelProcessData_t *selection = process;
    volatile u8 *pad = dds3PadButtonState;
    s32 count = 0;
    s32 both;
    s32 moved = 0;
    s32 area;
    s8 friendCount;

    if ((process->data->stat & 0x200000) || process->stat != 1) {
        return;
    }
    if (pad[0x21] & 2) {
        MemoryCommand_nbTarSelProcess(process);
        process->stat = 0;
        return;
    }
    if ((s8)pad[0x23] < 0) {
        process->stat = 2;
        nbPlaySe(10);
    }
    if (process->ctype == 2) {
        return;
    }
    area = selection->nowcarea;
    if (area == 1) {
        count = selection->friendcnt;
    } else if (area == 2) {
        count = selection->enemycnt;
    }
    friendCount = selection->friendcnt;
    both = friendCount > 0 && selection->enemycnt > 0;
    if (count >= 2) {
        if (((s8)pad[0x25] < 0 || ((s8)pad[0x27] < 0 && !both)) &&
            selection->nowno == count - 1) {
            selection->nowno = 0;
            moved = 1;
        } else if ((pad[0x25] & 2) || ((pad[0x27] & 2) && !both)) {
            if (selection->nowno < count - 1) {
                selection->nowno++;
                moved = 1;
            }
        }
        if (((s8)pad[0x24] < 0 || ((s8)pad[0x26] < 0 && !both)) &&
            selection->nowno == 0) {
            selection->nowno = count - 1;
            moved = 1;
        } else if ((pad[0x24] & 2) || ((pad[0x26] & 2) && !both)) {
            if (selection->nowno > 0) {
                selection->nowno--;
                moved = 1;
            }
        }
    }
    if ((s8)pad[0x26] < 0 || (s8)pad[0x27] < 0) {
        if (area == 1 && selection->enemycnt > 0) {
            selection->nowno = 0;
            selection->nowcarea = 2;
            moved = 1;
        } else if (area == 2 && friendCount > 0) {
            selection->nowcarea = 1;
            selection->nowno = 0;
            moved = 1;
        }
    }
    if (moved) {
        UpdateSelect(process);
        nbPlaySe(3);
        process->statusTime = 0;
    }
}
#else
INCLUDE_ASM(const s32, "newbattle/nbTarSelProcess", CursorSelect);
#endif

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

