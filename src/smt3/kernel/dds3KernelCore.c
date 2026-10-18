#include "common.h"
#include "kernel.h"

/* Two ports, two sticks per port, and two axes per stick. */
extern s8 dds3PadButtonPrev[2][2][16];
extern u8 dds3PadButtonState[2][2][16];
extern u8 dds3PadStickPrev[2][2][2];
extern s8 dds3PadStickStatusPrev[2][2];
extern u8 dds3PadStickState[2][2][2];
extern u8 dds3PadStickStatusState[2][2];

extern dds3ProcessID_t *startinglistf;
extern dds3ProcessID_t *startinglistl;
extern u32 startinglistcnt;
extern dds3ProcessID_t *endinglistf;
extern dds3ProcessID_t *endinglistl;
extern u32 endinglistcnt;
extern dds3ProcessID_t *runprocess;
extern dds3ProcessID_t *runninglistf;
extern dds3ProcessID_t *runninglistl;
extern u32 runninglistcnt;

void DeleteProcessList(dds3ProcessID_t *delp);
void AddProcessList(dds3ProcessID_t *addp);
void StartProcess(dds3ProcessID_t *setp);
void StartingProcess(void);
void KillProcess(dds3ProcessID_t *killp);
void EndProcess(dds3ProcessID_t *endp);
void dds3UnlinkProcess(dds3ProcessID_t *unp);
s32 RunProcess(dds3ProcessID_t *runp);
void RunningProcess(void);
void dds3EndingProcess(void);
void ChangeProcessFlag(s32 onoff, dds3ProcessID_t *target, u32 flag, s32 type);
s32 dds3ExistProcess(dds3ProcessID_t *id);
dds3ProcessID_t *dds3SearchProcessName(char *name);
s32 dds3KillProcess(dds3ProcessID_t *id, s32 shutdowntimer);
void ChangeProcessFlagChild(s32 onoff, dds3ProcessID_t *target, u32 flag);

void *sdfChipAllocDirect(u32 size);
void sdfChipFreeDirect(void *ptr);
int printf(const char *fmt, ...);

extern char D_0052DD78[]; /* PAD: starting-list heading */
extern char D_0052DDA8[]; /* PAD: "<<< %d >>>\n" */
extern char D_0052DDB8[]; /* PAD: running-list heading */
extern char D_0052DDE8[]; /* PAD: ending-list heading */
extern char D_0054E8A8[];
extern char D_0054E8B0[];
extern char dds3ProcessTreeDepth[];
void DebugProcess(dds3ProcessID_t *process, const char *heading);

void DeleteProcessList(dds3ProcessID_t *delp)
{
    if ((delp->flag & 0xF) == 0) {
        return;
    }
    if ((delp->flag & 0xF) >= 4) {
        return;
    }

    if (delp->prev != NULL) {
        delp->prev->next = delp->next;
    } else {
        switch (delp->flag & 0xF) {
        case 1:
            startinglistf = delp->next;
            break;
        case 2:
            runninglistf = delp->next;
            break;
        case 3:
            endinglistf = delp->next;
            break;
        }
    }

    if (delp->next != NULL) {
        delp->next->prev = delp->prev;
    } else {
        switch (delp->flag & 0xF) {
        case 1:
            startinglistl = delp->prev;
            break;
        case 2:
            runninglistl = delp->prev;
            break;
        case 3:
            endinglistl = delp->prev;
            break;
        }
    }

    delp->next = NULL;
    delp->prev = NULL;

    switch (delp->flag & 0xF) {
    case 1:
        startinglistcnt--;
        break;
    case 2:
        runninglistcnt--;
        break;
    case 3:
        endinglistcnt--;
        break;
    }
}

void AddProcessList(dds3ProcessID_t *addp)
{
    dds3ProcessID_t *p;

    switch (addp->flag & 0xF) {
    case 1:
        p = startinglistf;
        break;
    case 2:
        p = runninglistf;
        break;
    case 3:
        p = endinglistf;
        break;
    case 0:
    default:
        return;
    }

    if (p == NULL) {
        switch (addp->flag & 0xF) {
        case 1:
            startinglistf = addp;
            startinglistl = addp;
            break;
        case 2:
            runninglistf = addp;
            runninglistl = addp;
            break;
        case 3:
            endinglistf = addp;
            endinglistl = addp;
            break;
        }
        addp->prev = NULL;
        addp->next = NULL;
    } else {
        while (p != NULL) {
            if (addp->priority < p->priority) {
                if (p->prev != NULL) {
                    p->prev->next = addp;
                    addp->prev = p->prev;
                    addp->next = p;
                    p->prev = addp;
                } else {
                    switch (addp->flag & 0xF) {
                    case 1:
                        startinglistf = addp;
                        break;
                    case 2:
                        runninglistf = addp;
                        break;
                    case 3:
                        endinglistf = addp;
                        break;
                    }
                    addp->prev = NULL;
                    addp->next = p;
                    p->prev = addp;
                }
                break;
            }
            p = p->next;
        }
        if (p == NULL) {
            switch (addp->flag & 0xF) {
            case 1:
                startinglistl->next = addp;
                addp->prev = startinglistl;
                startinglistl = addp;
                break;
            case 2:
                runninglistl->next = addp;
                addp->prev = runninglistl;
                runninglistl = addp;
                break;
            case 3:
                endinglistl->next = addp;
                addp->prev = endinglistl;
                endinglistl = addp;
                break;
            }
            addp->next = NULL;
        }
    }

    switch (addp->flag & 0xF) {
    case 1:
        startinglistcnt++;
        break;
    case 2:
        runninglistcnt++;
        break;
    case 3:
        endinglistcnt++;
        break;
    }
}

void StartProcess(dds3ProcessID_t *setp)
{
    DeleteProcessList(setp);
    setp->flag = (setp->flag & ~0xF) | 2;
    AddProcessList(setp);
    setp->starttime = 0;
    setp->runtime = 0;
}

void StartingProcess(void)
{
    dds3ProcessID_t *p;
    dds3ProcessID_t *startp;

    p = startinglistf;
    while (p != NULL) {
        if (p->starttimer > 0) {
            p->starttimer--;
        }
        startp = p;
        p = p->next;
        if (startp->starttimer == 0) {
            StartProcess(startp);
        }
    }
}

s32 RunProcess(dds3ProcessID_t *task)
{
    s32 port;
    s32 inputIndex;
    s32 nextUpdate;

    if (task->flag & 0x20) {
        return 1;
    }
    runprocess = task;

    if (task->flag & 0x10) {
        for (port = 0; port < 2; port++) {
            for (inputIndex = 0; inputIndex < 16; inputIndex++) {
                dds3PadButtonState[0][port][inputIndex] = 0;
                dds3PadButtonState[1][port][inputIndex] = 0;
            }
            for (inputIndex = 0; inputIndex < 2; inputIndex++) {
                dds3PadStickState[port][inputIndex][0] = 0;
                dds3PadStickStatusState[port][inputIndex] = 0;
                dds3PadStickState[port][inputIndex][1] = 0;
            }
        }
    } else {
        for (port = 0; port < 2; port++) {
            for (inputIndex = 0; inputIndex < 16; inputIndex++) {
                dds3PadButtonState[0][port][inputIndex] = dds3PadButtonPrev[0][port][inputIndex];
                dds3PadButtonState[1][port][inputIndex] = dds3PadButtonPrev[1][port][inputIndex];
            }
            for (inputIndex = 0; inputIndex < 2; inputIndex++) {
                dds3PadStickState[port][inputIndex][0] = dds3PadStickPrev[port][inputIndex][0];
                dds3PadStickState[port][inputIndex][1] = dds3PadStickPrev[port][inputIndex][1];
                dds3PadStickStatusState[port][inputIndex] = dds3PadStickStatusPrev[port][inputIndex];
            }
        }
    }

    if (task->processfunc != NULL && task->processfunc != (dds3ProcessFunc_t)-1) {
        nextUpdate = task->processfunc(task);
        if (nextUpdate != 0) {
            task->processfunc = (dds3ProcessFunc_t)nextUpdate;
        }
        if (nextUpdate == -1 && (task->flag & 0xF) == 2) {
            EndProcess(task);
            runprocess = 0;
            return 0;
        }
    }
    task->runtime++;
    runprocess = 0;
    return 1;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", RunningProcess);

void KillProcess(dds3ProcessID_t *killp)
{
    dds3ProcessID_t *p;
    dds3ProcessID_t *nextp;

    p = killp->child;
    while (p != NULL) {
        nextp = p->sibling;
        dds3KillProcess(p, 0);
        p = nextp;
    }

    DeleteProcessList(killp);
    printf("shutdown %s!!\n", killp->name);
    if (killp->shutdownfunc != NULL) {
        killp->shutdownfunc(killp);
    }
    killp->flag &= ~0xF;
    dds3UnlinkProcess(killp);
    sdfChipFreeDirect(killp);
}

void EndProcess(dds3ProcessID_t *endp)
{
    u32 stat = endp->flag & 0xF;

    if (stat < 3) {
        if (stat == 0) {
            return;
        }

        DeleteProcessList(endp);
        endp->flag = (endp->flag & ~0xF) | 3;
        AddProcessList(endp);
        if (endp->shutdowntimer == 0) {
            KillProcess(endp);
        }
    }
}


void dds3EndingProcess(void)
{
    dds3ProcessID_t *p;
    dds3ProcessID_t *endp;

    p = endinglistf;
    while (p != NULL) {
        if (p->shutdowntimer > 0) {
            p->shutdowntimer--;
        }
        endp = p;
        p = p->next;
        if (endp->shutdowntimer == 0) {
            KillProcess(endp);
        }
    }
}


void ChangeProcessFlagChild(s32 onoff, dds3ProcessID_t *target, u32 flag)
{
    dds3ProcessID_t *p;

    if (onoff != 0) {
        target->flag |= flag & 0x0FFFFFF0;
    } else {
        target->flag &= ~(flag & 0x0FFFFFF0);
    }
    p = target->child;
    while (p != NULL) {
        ChangeProcessFlagChild(onoff, p, flag);
        p = p->sibling;
    }
}

/* The 0x0FFFFFF0 mask keeps callers out of the state nibble and the top four
 * bits of flag. */
void ChangeProcessFlag(s32 onoff, dds3ProcessID_t *target, u32 flag, s32 type)
{
    dds3ProcessID_t *p = NULL;
    s32 list;

    switch (type) {
    case 0:
        if (onoff != 0) {
            target->flag |= flag & 0x0FFFFFF0;
        } else {
            target->flag &= ~(flag & 0x0FFFFFF0);
        }
        return;
    case 1:
    case 3:
        for (list = 0; list < 3; list++) {
            switch (list) {
            case 0:
                p = startinglistf;
                break;
            case 1:
                p = runninglistf;
                break;
            case 2:
                p = endinglistf;
                break;
            }
            if (p != NULL) {
                u32 mask = flag & 0x0FFFFFF0;

                while (p != NULL) {
                    if ((p != target && type == 1) || type == 3) {
                        if (onoff != 0) {
                            p->flag |= mask;
                        } else {
                            p->flag &= ~mask;
                        }
                    }
                    p = p->next;
                }
            }
        }
        return;
    case 2:
        ChangeProcessFlagChild(onoff, target, flag);
        break;
    }
}

u32 dds3GetProcessCount(u32 stat)
{
    switch (stat & 0xF) {
    case 1:
        return startinglistcnt;
    case 2:
        return runninglistcnt;
    case 3:
        return endinglistcnt;
    }
    return 0;
}

dds3ProcessID_t *dds3SearchProcessNumber(u32 stat, s32 n)
{
    dds3ProcessID_t *p = startinglistf;
    s32 i;

    switch (stat & 0xF) {
    case 1:
        break;
    case 2:
        p = runninglistf;
        break;
    case 3:
        p = endinglistf;
        break;
    case 0:
    default:
        return NULL;
    }
    for (i = 0; i < n && p != NULL; i++) {
        p = p->next;
    }
    return p;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", DebugProcess);

void dds3PrintProcessList(void)
{
    char *countFormat;

    DebugProcess(startinglistf, D_0052DD78);
    countFormat = D_0052DDA8;
    printf(countFormat, startinglistcnt);
    DebugProcess(runninglistf, D_0052DDB8);
    printf(countFormat, runninglistcnt);
    DebugProcess(endinglistf, D_0052DDE8);
    printf(countFormat, endinglistcnt);
    printf(D_0054E8A8);
}

void DebugProcessTree(dds3ProcessID_t *process, s32 depth)
{
    printf(D_0054E8B0, dds3ProcessTreeDepth, process);
    if (process->child != NULL) {
        if (process->parent == NULL) {
            dds3ProcessTreeDepth[0] = ' ';
        } else {
            if (process->sibling == NULL) {
                dds3ProcessTreeDepth[depth * 4] = ' ';
            } else {
                dds3ProcessTreeDepth[depth * 4] = '|';
            }
        }
        dds3ProcessTreeDepth[depth * 4 + 4] = 0;
        DebugProcessTree(process->child, depth + 1);
        dds3ProcessTreeDepth[depth * 4] = 0;
    }
    if (process->sibling != NULL) {
        DebugProcessTree(process->sibling, depth);
    }
}

INCLUDE_RODATA(const s32, "kernel/dds3KernelCore", D_0052DD78);

INCLUDE_RODATA(const s32, "kernel/dds3KernelCore", D_0052DDA8);

INCLUDE_RODATA(const s32, "kernel/dds3KernelCore", D_0052DDB8);

INCLUDE_RODATA(const s32, "kernel/dds3KernelCore", D_0052DDE8);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", dds3PrintProcessTree);

s32 dds3ProcessMain(void)
{
    StartingProcess();
    RunningProcess();
    dds3EndingProcess();
    return 1;
}

/* namehash is the name's byte sum, which dds3SearchProcessName uses to reject
 * most candidates without a compare. */
dds3ProcessID_t *dds3StartProcess(char *namestr, u32 priority, s32 starttimer, s32 shutdowntimer,
                                  dds3ProcessFunc_t processfunc, dds3ProcessFunc_t shutdownfunc,
                                  void *data)
{
    dds3ProcessID_t *setp = sdfChipAllocDirect(sizeof(dds3ProcessID_t));
    s32 i = 0;

    if (setp == NULL) {
        return NULL;
    }
    setp->namehash = 0;
    while ((setp->name[i] = namestr[i]) != 0 && i < 0x18) {
        setp->namehash += namestr[i];
        i++;
    }
    setp->priority = priority;
    setp->flag = 1;
    setp->starttimer = starttimer;
    setp->shutdowntimer = shutdowntimer;
    setp->processfunc = processfunc;
    setp->shutdownfunc = shutdownfunc;
    setp->workdata = data;
    setp->name[0x17] = 0;
    setp->starttime = 0;
    setp->runtime = 0;
    setp->next = NULL;
    setp->prev = NULL;
    setp->parent = NULL;
    setp->child = NULL;
    setp->sibling = NULL;
    AddProcessList(setp);
    if (setp->starttimer == 0) {
        StartProcess(setp);
    }
    return setp;
}

s32 dds3KillProcessName(char *name, s32 shutdowntimer)
{
    dds3ProcessID_t *id = dds3SearchProcessName(name);

    if (id == NULL) {
        return 0;
    }
    return dds3KillProcess(id, shutdowntimer);
}

s32 dds3KillProcess(dds3ProcessID_t *id, s32 shutdowntimer)
{
    if (id == NULL || id == (dds3ProcessID_t *)-1) {
        return 0;
    }
    switch (id->flag & 0xF) {
    case 1:
    case 2:
        id->shutdowntimer = shutdowntimer;
        EndProcess(id);
        return 1;
    case 3:
        id->shutdowntimer = shutdowntimer;
        if (id->shutdowntimer == 0) {
            KillProcess(id);
            return 1;
        }
        break;
    case 0:
    default:
        return 0;
    }
    return 1;
}

void dds3EndProcess(dds3ProcessID_t *id)
{
    if ((id->flag & 0xF) == 2) {
        DeleteProcessList(id);
        id->flag = (id->flag & ~0xF) | 3;
        AddProcessList(id);
    }
}

void dds3SetShutdownTimer(dds3ProcessID_t *id, s32 newtimer)
{
    u32 stat = id->flag & 0xF;

    if (stat != 0) {
        if (stat < 4) {
            id->shutdowntimer = newtimer;
        }
    }
}

u32 dds3GetProcessStat(dds3ProcessID_t *id)
{
    u32 stat;

    if (!dds3ExistProcess(id)) {
        return 0;
    }
    stat = id->flag & 0xF;
    if (stat >= 4) {
        stat = 0;
    }
    return stat;
}

/* The name compare runs back to front, from the terminator down to byte 0. */
dds3ProcessID_t *dds3SearchProcessName(char *name)
{
    s32 len;
    s32 hash;
    s32 list;
    s32 i;
    dds3ProcessID_t *p;

    p = NULL;
    hash = 0;
    len = 0;
    while (name[len] != '\0') {
        hash += name[len];
        len++;
    }

    for (list = 0; list < 3; list++) {
        switch (list) {
        case 0:
            p = startinglistf;
            break;
        case 1:
            p = runninglistf;
            break;
        case 2:
            p = endinglistf;
            break;
        }

        while (p != NULL) {
            if (p->namehash == hash) {
                for (i = len;; i--) {
                    if (name[i] != p->name[i]) {
                        break;
                    }
                    if (i == 0) {
                        return p;
                    }
                }
            }
            p = p->next;
        }
    }
    return NULL;
}

dds3ProcessID_t *dds3SearchProcessPriority(u32 priority)
{
    s32 list;
    dds3ProcessID_t *p;

    p = NULL;
    for (list = 0; list < 3; list++) {
        switch (list) {
        case 0:
            p = startinglistf;
            break;
        case 1:
            p = runninglistf;
            break;
        case 2:
            p = endinglistf;
            break;
        }
        while (p != NULL) {
            if (p->priority == priority) {
                return p;
            }
            p = p->next;
        }
    }
    return NULL;
}

s32 dds3ExistProcess(dds3ProcessID_t *id)
{
    dds3ProcessID_t *p = NULL;
    s32 list;

    for (list = 0; list < 3; list++) {
        switch (list) {
        case 0:
            p = startinglistf;
            break;
        case 1:
            p = runninglistf;
            break;
        case 2:
            p = endinglistf;
            break;
        }
        while (p != NULL) {
            if (p == id) {
                return 1;
            }
            p = p->next;
        }
    }
    return 0;
}

u32 dds3GetProcessStartTime(dds3ProcessID_t *id)
{
    return id->starttime;
}

u32 dds3GetProcessRunTime(dds3ProcessID_t *id)
{
    return id->runtime;
}

void dds3SetProcessData(dds3ProcessID_t *id, void *data)
{
    id->workdata = data;
}

void *dds3GetProcessData(dds3ProcessID_t *id)
{
    return id->workdata;
}

void dds3DebugNowProcess(void)
{
    if (runprocess == NULL) {
        printf("now process [NULL]\n");
    } else {
        printf("now process [%s]\n", runprocess->name);
    }
}

void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *child)
{
    dds3ProcessID_t *p;

    if (child->parent != NULL) {
        dds3UnlinkProcess(child);
    }
    if (parent->child != NULL) {
        p = parent->child;
        while (p->sibling != NULL) {
            p = p->sibling;
        }
        p->sibling = child;
    } else {
        parent->child = child;
    }
    child->parent = parent;
}

void dds3UnlinkProcess(dds3ProcessID_t *unp)
{
    dds3ProcessID_t *parent = unp->parent;
    dds3ProcessID_t *p;

    if (parent == NULL) {
        return;
    }
    if (parent->child == unp) {
        parent->child = unp->sibling;
    } else {
        p = parent->child;
        while (p->sibling != unp) {
            p = p->sibling;
        }
        p->sibling = unp->sibling;
    }
    unp->parent = NULL;
    unp->sibling = NULL;
}

void dds3SetProcessFlag(dds3ProcessID_t *target, u32 flag, s32 type)
{
    ChangeProcessFlag(1, target, flag, type);
}

void dds3ResetProcessFlag(dds3ProcessID_t *target, u32 flag, s32 type)
{
    ChangeProcessFlag(0, target, flag, type);
}
INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", startinglistf);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", startinglistl);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", startinglistcnt);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", endinglistf);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", endinglistl);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", endinglistcnt);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", runninglistf);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", runninglistl);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", runninglistcnt);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", runprocess);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_0054E878);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_0054E880);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_0054E888);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_0054E890);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_0054E898);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_0054E8A0);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_0054E8A8);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_0054E8B0);

