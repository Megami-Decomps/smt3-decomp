#include "common.h"
#include "kernel.h"

typedef struct dds3AdminiWork
{
    u32 flag;
    u32 value;
    s8 sequence;
    s8 nextsequence;
    u8 historyindex;
    u8 unk0B;
    s8 history[8];
    u8 historyflag[8];
    void *reqdata;
    u8 reqsize;
    u8 waittimer;
} dds3AdminiWork_t;

typedef struct dds3AdminiEntry
{
    s32 (*startfunc)(dds3AdminiWork_t *work, void *reqdata);
    s32 (*endfunc)(void);
    s32 (*mainfunc)(void);
} dds3AdminiEntry_t;

/* Named scheduler slots: retail strides entries by 0x44 bytes. */
typedef struct
{
    s32 kind;
    char name[32];
    dds3ProcessID_t *task;
    dds3ProcessFunc_t update;
    dds3ProcessFunc_t destroy;
    void (*setState)(s32);
    s32 enabled;
    void *userValue;
    s32 previous;
    s32 next;
} fclNamedSlot_t;

extern fclNamedSlot_t fclEncycTable[32];
extern s32 fclEncycHead;

extern char D_0054E8C8[];  /* "Admini" */

extern dds3AdminiEntry_t dds3AdminiList[];
extern void *dds3GetProcessData(dds3ProcessID_t *id);
extern dds3ProcessID_t *dds3SearchProcessName(char *name);
extern dds3ProcessID_t *dds3StartProcess(char *namestr, u32 priority, s32 starttimer,
                                         s32 shutdowntimer, dds3ProcessFunc_t processfunc,
                                         dds3ProcessFunc_t shutdownfunc, void *data);
extern void *sdfChipAllocDirect(u32 size);
extern void sdfChipFreeDirect(void *ptr);
extern void *memcpy(void *dest, const void *source, u32 size);

dds3AdminiWork_t *GetAdminiProcessData(void);
void dds3AdminiCallSequence(s32 sequence, void *data, u32 size, s32 keep);
s32 dds3AdminiStartSequence(dds3ProcessID_t *id);
s32 dds3AdminiNeoProcess(dds3ProcessID_t *id);
void dds3AdminiNeoShutdown(dds3ProcessID_t *id);

dds3AdminiWork_t *GetAdminiProcessData(void)
{
    return dds3GetProcessData(dds3SearchProcessName(D_0054E8C8));
}

u32 dds3_102828(void)
{
    dds3AdminiWork_t *work;

    work = GetAdminiProcessData();
    return work->value;
}

/* Requests retain byte-sized sequence/size storage: 256 bytes records zero.
 * Release the old payload before copying the new one. */
void dds3AdminiCallSequence(s32 sequence, void *data, u32 size, s32 keep)
{
    dds3AdminiWork_t *work;
    void *previousData;
    u32 flag;

    if (data == NULL || size <= 0x100) {
        work = GetAdminiProcessData();
        previousData = work->reqdata;
        work->nextsequence = sequence;
        flag = work->flag;
        flag |= 1;
        flag &= ~8;
        flag &= ~0x10000;
        work->flag = flag;
        work->waittimer = 2;
        if (previousData != NULL) {
            sdfChipFreeDirect(previousData);
            work->reqdata = NULL;
            work->reqsize = 0;
        }
        if (data != NULL) {
            work->reqdata = sdfChipAllocDirect(size);
            memcpy(work->reqdata, data, size);
            work->reqsize = size;
        } else {
            work->reqdata = NULL;
            work->reqsize = 0;
        }
        if (keep != 0) {
            work->flag |= 4;
        } else {
            work->flag &= ~4;
        }
    }
}

void dds3AdminiOverwriteSequence(s32 sequence, void *data, u32 size)
{
    dds3AdminiWork_t *work;

    dds3AdminiCallSequence(sequence, data, size, 0);
    work = GetAdminiProcessData();
    work->flag |= 8;
}

void dds3AdminiExitSequence(void)
{
    dds3AdminiWork_t *work;

    work = GetAdminiProcessData();
    work->flag |= 2;
}

s8 dds3AdminiGetActiveSequence(void)
{
    return GetAdminiProcessData()->sequence;
}

s8 dds3AdminiGetNextSequence(void)
{
    return GetAdminiProcessData()->nextsequence;
}

s8 dds3AdminiGetPrevSequence(void)
{
    dds3AdminiWork_t *work;

    work = GetAdminiProcessData();
    return work->history[(work->historyindex + 7) & 7];
}

u8 dds3AdminiGetPrevHistoryFlag(void)
{
    dds3AdminiWork_t *work;

    work = GetAdminiProcessData();
    return work->historyflag[(work->historyindex + 7) & 7];
}

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", dds3AdminiStartSequence);

s32 dds3AdminiEndCheck(dds3ProcessID_t *id)
{
    dds3AdminiWork_t *work = dds3GetProcessData(id);
    s32 sequence = work->sequence;
    s32 (*endfunc)(void);
    s32 wait;

    if (sequence >= 0) {
        endfunc = dds3AdminiList[sequence].endfunc;
        if (endfunc != NULL) {
            wait = endfunc();
            if (wait < 0) {
                return 0;
            }
            work->waittimer = wait + 1;
            work->sequence = -1;
        }
    }
    return (s32)dds3AdminiStartSequence;
}

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", dds3AdminiNeoProcess);

void dds3AdminiNeoShutdown(dds3ProcessID_t *id)
{
    dds3AdminiWork_t *work = dds3GetProcessData(id);
    s32 sequence = work->sequence;

    if (sequence >= 0) {
        s32 (*endfunc)(void) = dds3AdminiList[sequence].endfunc;

        if (endfunc != NULL) {
            endfunc();
        }
    }
    if (work->reqdata != NULL) {
        sdfChipFreeDirect(work->reqdata);
    }
    sdfChipFreeDirect(dds3GetProcessData(id));
}

void dds3InitAdmini(void)
{
    dds3AdminiWork_t *work;
    s32 i;

    if (dds3SearchProcessName(D_0054E8C8) == NULL) {
        work = sdfChipAllocDirect(sizeof(dds3AdminiWork_t));
        work->flag = 0;
        work->value = 0;
        work->sequence = -1;
        work->nextsequence = -1;
        work->historyindex = 0;
        for (i = 0; i < 8; i++) {
            work->history[i] = -1;
            work->historyflag[i] = 0;
        }
        work->reqdata = NULL;
        work->reqsize = 0;
        dds3StartProcess(D_0054E8C8, DDS3_PRIORITY_ADMINI, 0, 1, dds3AdminiNeoProcess,
                         (dds3ProcessFunc_t)dds3AdminiNeoShutdown, work);
    }
}

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", fclEncycFindFreeSlot);

s32 fclEncycFreeEntry(const char *name)
{
    s32 slotIndex = fclEncycHead;
    s32 characterIndex;

    while (slotIndex >= 0) {
        characterIndex = 0;
        while (fclEncycTable[slotIndex].name[characterIndex] != '\0') {
            if (name[characterIndex] != fclEncycTable[slotIndex].name[characterIndex]) {
                break;
            }
            characterIndex++;
        }
        if (fclEncycTable[slotIndex].name[characterIndex] == '\0' && name[characterIndex] == '\0') {
            return slotIndex;
        }
        slotIndex = fclEncycTable[slotIndex].next;
    }
    return -1;
}

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", fclEncycFreeEntries);

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", fcl_103420);

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", fclEncycAllocEntry);

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", fcl_103560);

INCLUDE_RODATA(const s32, "kernel/dds3AdminiProcess", D_0052DF78);

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", D_0054E8C8);

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", D_0054E8D0);

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", D_0054E8D8);

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", D_0054E8E0);

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", fclEncycHead);

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", fclEncycTail);

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", fclEncycCount);

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", fclEncycFree);

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", D_0054E8F4);

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", dds3PadInitDone);

