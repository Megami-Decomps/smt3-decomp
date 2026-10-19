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
    s32 (*startfunc)(s32 restored, void *reqdata);
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
extern s32 fclEncycTail;
extern s32 fclEncycCount;
extern s32 fclEncycFree;

extern char D_0052DF78[];  /* "DebugOwner" */

extern char D_0054E8C8[];  /* "Admini" */
extern char D_0054E8D0[];  /* "[" */
extern char D_0054E8D8[];  /* "*" */
extern char D_0054E8E0[];  /* "\n" */
extern char D_0052DF50[];  /* "%d,0x%02X]" */
extern char D_0052DF60[];  /* "restore sequence!!\n" */

int printf(const char *fmt, ...);

extern dds3AdminiEntry_t dds3AdminiList[];
extern void *dds3GetProcessData(dds3ProcessID_t *id);
extern dds3ProcessID_t *dds3SearchProcessName(char *name);
extern dds3ProcessID_t *dds3StartProcess(char *namestr, u32 priority, s32 starttimer,
                                         s32 shutdowntimer, dds3ProcessFunc_t processfunc,
                                         dds3ProcessFunc_t shutdownfunc, void *data);
extern void *sdfChipAllocDirect(u32 size);
extern void sdfChipFreeDirect(void *ptr);
extern void *memcpy(void *dest, const void *source, u32 size);
extern s32 dds3ExistProcess(dds3ProcessID_t *id);
extern s32 dds3KillProcess(dds3ProcessID_t *id, s32 shutdowntimer);

dds3AdminiWork_t *GetAdminiProcessData(void);
void dds3AdminiCallSequence(s32 sequence, void *data, u32 size, s32 keep);
s32 dds3AdminiStartSequence(dds3ProcessID_t *id);
s32 dds3AdminiNeoProcess(dds3ProcessID_t *id);
s32 dds3AdminiEndCheck(dds3ProcessID_t *id);
void dds3AdminiNeoShutdown(dds3ProcessID_t *id);
s32 fclEncycFreeEntry(const char *name);

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

s32 dds3AdminiStartSequence(dds3ProcessID_t *id)
{
    dds3AdminiWork_t *work;
    s32 (*startfunc)(s32 restored, void *reqdata);
    u32 flag;
    u32 nextflag;
    s32 restored;
    s32 i;

    work = dds3GetProcessData(id);
    flag = work->flag;
    if (flag & 1) {
        if (work->nextsequence >= 0) {
            if (work->waittimer != 0) {
                work->waittimer--;
                return 0;
            }
            work->sequence = work->nextsequence;
            work->nextsequence = -1;
            nextflag = flag;
            nextflag &= ~1;
            nextflag &= ~2;
            work->flag = nextflag;
            work->value = 0;
            if (flag & 4) {
                work->historyflag[work->historyindex] |= 1;
                work->flag &= ~4;
            }
            if (work->flag & 0x10000) {
                restored = 1;
                work->flag &= ~0x10000;
                work->historyindex = (work->historyindex + 7) & 7;
            } else {
                restored = 0;
                if (work->flag & 8) {
                    work->flag &= ~8;
                } else {
                    work->historyindex = (work->historyindex + 1) & 7;
                }
            }
            work->history[work->historyindex] = work->sequence;
            work->historyflag[work->historyindex] = 0;
            startfunc = dds3AdminiList[work->sequence].startfunc;
            if (startfunc != NULL) {
                startfunc(restored, work->reqdata);
            }
            for (i = 0; i < 8; i++) {
                printf(D_0054E8D0);
                if (work->historyindex == i) {
                    printf(D_0054E8D8);
                }
                printf(D_0052DF50, work->history[i], work->historyflag[i]);
            }
            printf(D_0054E8E0);
        }
    }
    return (s32)dds3AdminiNeoProcess;
}

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

s32 dds3AdminiNeoProcess(dds3ProcessID_t *id)
{
    dds3AdminiWork_t *work;
    s32 (*mainfunc)(void);
    s32 index;

    work = dds3GetProcessData(id);
    if (!(work->flag & 2)) {
        if (work->sequence >= 0) {
            mainfunc = dds3AdminiList[work->sequence].mainfunc;
            if (mainfunc != NULL) {
                if (mainfunc() != 0) {
                    work->flag |= 2;
                }
            }
            work->value++;
        }
    }
    if (work->flag & 2) {
        if (work->sequence >= 0) {
            index = (work->historyindex + 7) & 7;
            if (work->history[index] >= 0 && (work->historyflag[index] & 1)) {
                printf(D_0052DF60);
                work->flag |= 0x10001;
                work->nextsequence = work->history[index];
                work->waittimer = 2;
                work->flag &= ~4;
                if (work->reqdata != NULL) {
                    sdfChipFreeDirect(work->reqdata);
                    work->reqdata = NULL;
                    work->reqsize = 0;
                }
            }
            if (!(work->flag & 1)) {
                dds3AdminiCallSequence(0, NULL, 0, 0);
            }
            work->flag &= ~2;
        }
    }
    if (work->flag & 1) {
        if (work->nextsequence >= 0) {
            return (s32)dds3AdminiEndCheck;
        }
    }
    return 0;
}

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

/* NON_MATCHING: 2 of 113 words differ (first at +0x158). */
#ifdef NON_MATCHING
s32 fclEncycFindFreeSlot(const char *name)
{
    s32 slot;
    s32 i;
    s32 tail;

    if (dds3SearchProcessName(D_0052DF78) == NULL) {
        return -1;
    }
    if (fclEncycFreeEntry(name) >= 0) {
        return -1;
    }
    for (slot = 0; slot < 32; slot++) {
        if (fclEncycTable[slot].kind == 0) {
            break;
        }
    }
    if (slot == 32) {
        return -1;
    }
    fclEncycTable[slot].kind = 0;
    i = 0;
    while ((fclEncycTable[slot].name[i] = name[i]) != '\0') {
        if (i == 31) {
            fclEncycTable[slot].name[i] = '\0';
            break;
        }
        i++;
    }
    fclEncycTable[slot].task = NULL;
    fclEncycTable[slot].update = NULL;
    fclEncycTable[slot].destroy = NULL;
    fclEncycTable[slot].setState = NULL;
    fclEncycTable[slot].enabled = 0;
    fclEncycTable[slot].userValue = NULL;
    fclEncycTable[slot].previous = -1;
    fclEncycTable[slot].next = -1;
    tail = fclEncycTail;
    if (tail < 0) {
        fclEncycHead = slot;
        fclEncycTail = slot;
        fclEncycFree = slot;
    } else {
        fclEncycTail = slot;
        fclEncycTable[tail].next = slot;
        fclEncycTable[slot].previous = tail;
    }
    fclEncycCount++;
    return slot;
}
#else
INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", fclEncycFindFreeSlot);
#endif

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

/* NON_MATCHING: 2 of 122 words differ (first at +0x100). */
#ifdef NON_MATCHING
s32 fclEncycFreeEntries(const char *name)
{
    s32 slot;
    s32 previous;
    s32 next;

    if (dds3SearchProcessName(D_0052DF78) == NULL) {
        return 0;
    }
    slot = fclEncycFreeEntry(name);
    if (slot < 0) {
        return 0;
    }
    if (fclEncycTable[slot].kind > 0) {
        if (fclEncycTable[slot].kind < 3) {
            if (dds3ExistProcess(fclEncycTable[slot].task) == 1) {
                dds3KillProcess(fclEncycTable[fclEncycFree].task, 1);
            }
        }
    }
    fclEncycTable[slot].kind = 0;
    fclEncycTable[slot].name[0] = '\0';
    fclEncycTable[slot].task = NULL;
    fclEncycTable[slot].update = NULL;
    fclEncycTable[slot].destroy = NULL;
    fclEncycTable[slot].setState = NULL;
    fclEncycTable[slot].enabled = 0;
    fclEncycTable[slot].userValue = NULL;
    previous = fclEncycTable[slot].previous;
    if (previous < 0) {
        fclEncycHead = fclEncycTable[slot].next;
    } else {
        fclEncycTable[previous].next = fclEncycTable[slot].next;
    }
    next = fclEncycTable[slot].next;
    if (next < 0) {
        fclEncycTail = fclEncycTable[slot].previous;
    } else {
        fclEncycTable[next].previous = fclEncycTable[slot].previous;
    }
    fclEncycTable[slot].previous = -1;
    fclEncycTable[slot].next = -1;
    if (fclEncycFree == slot) {
        fclEncycFree = fclEncycHead;
    }
    fclEncycCount--;
    return 1;
}
#else
INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", fclEncycFreeEntries);
#endif

/* NON_MATCHING: 7 of 37 words differ (first at +0x3C). */
#ifdef NON_MATCHING
s32 fcl_103420(const char *name, void (*setState)(s32))
{
    s32 slot = fclEncycFindFreeSlot(name);

    if (slot < 0) {
        return 0;
    }
    fclEncycTable[slot].kind = 3;
    fclEncycTable[slot].task = NULL;
    fclEncycTable[slot].update = NULL;
    fclEncycTable[slot].destroy = NULL;
    fclEncycTable[slot].setState = setState;
    fclEncycTable[slot].enabled = 0;
    fclEncycTable[slot].userValue = NULL;
    return 1;
}
#else
INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", fcl_103420);
#endif

/* NON_MATCHING: 11 of 42 words differ (first at +0x8). */
#ifdef NON_MATCHING
s32 fclEncycAllocEntry(const char *name, dds3ProcessFunc_t update, dds3ProcessFunc_t destroy,
                       void *userValue)
{
    s32 slot = fclEncycFindFreeSlot(name);

    if (slot < 0) {
        return 0;
    }
    fclEncycTable[slot].kind = 1;
    fclEncycTable[slot].task = NULL;
    fclEncycTable[slot].update = update;
    fclEncycTable[slot].destroy = destroy;
    fclEncycTable[slot].setState = NULL;
    fclEncycTable[slot].enabled = 0;
    fclEncycTable[slot].userValue = userValue;
    return 1;
}
#else
INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", fclEncycAllocEntry);
#endif

/* NON_MATCHING: 11 of 43 words differ (first at +0x8). */
#ifdef NON_MATCHING
s32 fcl_103560(const char *name, dds3ProcessFunc_t update, dds3ProcessFunc_t destroy,
               void *userValue)
{
    s32 slot = fclEncycFindFreeSlot(name);

    if (slot < 0) {
        return 0;
    }
    fclEncycTable[slot].kind = 2;
    fclEncycTable[slot].task = NULL;
    fclEncycTable[slot].update = update;
    fclEncycTable[slot].destroy = destroy;
    fclEncycTable[slot].setState = NULL;
    fclEncycTable[slot].enabled = 0;
    fclEncycTable[slot].userValue = userValue;
    return 1;
}
#else
INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", fcl_103560);
#endif

INCLUDE_RODATA(const s32, "kernel/dds3AdminiProcess", D_0052DF50);

INCLUDE_RODATA(const s32, "kernel/dds3AdminiProcess", D_0052DF60);

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

