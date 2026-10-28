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

void DeleteProcessList(dds3ProcessID_t *setp);
void AddProcessList(dds3ProcessID_t *setp);
void StartProcess(dds3ProcessID_t *setp);
void StartingProcess(void);
void KillProcess(dds3ProcessID_t *setp);
void EndProcess(dds3ProcessID_t *setp);
void dds3UnlinkProcess(dds3ProcessID_t *target);
s32 RunProcess(dds3ProcessID_t *pro);
void RunningProcess(void);
void dds3EndingProcess(void);
void ChangeProcessFlag(s32 change, dds3ProcessID_t *target, u32 flag, s32 type);
s32 dds3ExistProcess(dds3ProcessID_t *id);
dds3ProcessID_t *dds3SearchProcessName(char *namestr);
s32 dds3KillProcess(dds3ProcessID_t *id, s32 shutdowntimer);
void ChangeProcessFlagChild(s32 change, dds3ProcessID_t *parent, u32 flag);

void *sdfChipAllocDirect(u32 aSize);
void sdfChipFreeDirect(void *aPtr);
int printf(const char *fmt, ...);

extern char D_0054E878[];
extern char D_0054E880[];
extern char D_0054E888[];
extern char D_0054E890[];
extern char D_0054E898[];
extern char D_0054E8A0[];
extern char D_0054E8A8[];
extern char D_0054E8B0[];
extern char dds3ProcessTreeDepth[];
void DebugProcess(dds3ProcessID_t *pro, const char *mes);

void DeleteProcessList(dds3ProcessID_t *setp)
{
    if ((setp->flag & 0xF) == 0) {
        return;
    }
    if ((setp->flag & 0xF) >= 4) {
        return;
    }

    if (setp->prev != NULL) {
        setp->prev->next = setp->next;
    } else {
        switch (setp->flag & 0xF) {
        case 1:
            startinglistf = setp->next;
            break;
        case 2:
            runninglistf = setp->next;
            break;
        case 3:
            endinglistf = setp->next;
            break;
        }
    }

    if (setp->next != NULL) {
        setp->next->prev = setp->prev;
    } else {
        switch (setp->flag & 0xF) {
        case 1:
            startinglistl = setp->prev;
            break;
        case 2:
            runninglistl = setp->prev;
            break;
        case 3:
            endinglistl = setp->prev;
            break;
        }
    }

    setp->next = NULL;
    setp->prev = NULL;

    switch (setp->flag & 0xF) {
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

void AddProcessList(dds3ProcessID_t *setp)
{
    dds3ProcessID_t *p;

    switch (setp->flag & 0xF) {
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
        switch (setp->flag & 0xF) {
        case 1:
            startinglistf = setp;
            startinglistl = setp;
            break;
        case 2:
            runninglistf = setp;
            runninglistl = setp;
            break;
        case 3:
            endinglistf = setp;
            endinglistl = setp;
            break;
        }
        setp->prev = NULL;
        setp->next = NULL;
    } else {
        while (p != NULL) {
            if (setp->priority < p->priority) {
                if (p->prev != NULL) {
                    p->prev->next = setp;
                    setp->prev = p->prev;
                    setp->next = p;
                    p->prev = setp;
                } else {
                    switch (setp->flag & 0xF) {
                    case 1:
                        startinglistf = setp;
                        break;
                    case 2:
                        runninglistf = setp;
                        break;
                    case 3:
                        endinglistf = setp;
                        break;
                    }
                    setp->prev = NULL;
                    setp->next = p;
                    p->prev = setp;
                }
                break;
            }
            p = p->next;
        }
        if (p == NULL) {
            switch (setp->flag & 0xF) {
            case 1:
                startinglistl->next = setp;
                setp->prev = startinglistl;
                startinglistl = setp;
                break;
            case 2:
                runninglistl->next = setp;
                setp->prev = runninglistl;
                runninglistl = setp;
                break;
            case 3:
                endinglistl->next = setp;
                setp->prev = endinglistl;
                endinglistl = setp;
                break;
            }
            setp->next = NULL;
        }
    }

    switch (setp->flag & 0xF) {
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

s32 RunProcess(dds3ProcessID_t *pro)
{
    s32 port;
    s32 inputIndex;
    s32 nextUpdate;

    if (pro->flag & 0x20) {
        return 1;
    }
    runprocess = pro;

    if (pro->flag & 0x10) {
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

    if (pro->processfunc != NULL && pro->processfunc != (dds3ProcessFunc_t)-1) {
        nextUpdate = pro->processfunc(pro);
        if (nextUpdate != 0) {
            pro->processfunc = (dds3ProcessFunc_t)nextUpdate;
        }
        if (nextUpdate == -1 && (pro->flag & 0xF) == 2) {
            EndProcess(pro);
            runprocess = 0;
            return 0;
        }
    }
    pro->runtime++;
    runprocess = 0;
    return 1;
}

void RunningProcess(void)
{
    dds3ProcessID_t *active = runninglistf;
    dds3ProcessID_t *destroyPending = endinglistf;
    dds3ProcessID_t *current;
    /* Each queue cursor uses a saved predecessor if RunProcess unlinks its node. */
    dds3ProcessID_t *drainPrevious;

    for (;;) {
        if (active != 0) {
            if (destroyPending != 0) {
                dds3ProcessID_t *previous;

                if (active->priority <= destroyPending->priority) {
                    current = active;
                    previous = current->prev;
                    if (RunProcess(current) != 0) {
                        active = current->next;
                    } else {
                        active = previous != 0 ? previous->next : runninglistf;
                    }
                } else {
                    current = destroyPending;
                    previous = current->prev;
                    if (RunProcess(current) != 0) {
                        destroyPending = current->next;
                    } else {
                        destroyPending = previous != 0 ? previous->next : endinglistf;
                    }
                }
            } else {

                do {
                    current = active;
                    drainPrevious = current->prev;
                    if (RunProcess(current) != 0) {
                        active = current->next;
                    } else {
                        active = drainPrevious != 0 ? drainPrevious->next : runninglistf;
                    }
                } while (active != 0);
                return;
            }
        } else {
            if (destroyPending != 0) {

                do {
                    current = destroyPending;
                    drainPrevious = current->prev;
                    if (RunProcess(current) != 0) {
                        destroyPending = current->next;
                    } else {
                        destroyPending = drainPrevious != 0 ? drainPrevious->next : endinglistf;
                    }
                } while (destroyPending != 0);
            }
            return;
        }
    }
}

void KillProcess(dds3ProcessID_t *setp)
{
    dds3ProcessID_t *p;
    dds3ProcessID_t *nextp;

    p = setp->child;
    while (p != NULL) {
        nextp = p->sibling;
        dds3KillProcess(p, 0);
        p = nextp;
    }

    DeleteProcessList(setp);
    printf("shutdown %s!!\n", setp->name);
    if (setp->shutdownfunc != NULL) {
        setp->shutdownfunc(setp);
    }
    setp->flag &= ~0xF;
    dds3UnlinkProcess(setp);
    sdfChipFreeDirect(setp);
}

void EndProcess(dds3ProcessID_t *setp)
{
    u32 stat = setp->flag & 0xF;

    if (stat < 3) {
        if (stat == 0) {
            return;
        }

        DeleteProcessList(setp);
        setp->flag = (setp->flag & ~0xF) | 3;
        AddProcessList(setp);
        if (setp->shutdowntimer == 0) {
            KillProcess(setp);
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


void ChangeProcessFlagChild(s32 change, dds3ProcessID_t *parent, u32 flag)
{
    dds3ProcessID_t *p;

    if (change != 0) {
        parent->flag |= flag & 0x0FFFFFF0;
    } else {
        parent->flag &= ~(flag & 0x0FFFFFF0);
    }
    p = parent->child;
    while (p != NULL) {
        ChangeProcessFlagChild(change, p, flag);
        p = p->sibling;
    }
}

/* The 0x0FFFFFF0 mask keeps callers out of the state nibble and the top four
 * bits of flag. */
void ChangeProcessFlag(s32 change, dds3ProcessID_t *target, u32 flag, s32 type)
{
    dds3ProcessID_t *p = NULL;
    s32 list;

    switch (type) {
    case 0:
        if (change != 0) {
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
                        if (change != 0) {
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
        ChangeProcessFlagChild(change, target, flag);
        break;
    }
}

u32 dds3GetProcessCount(u32 type)
{
    switch (type & 0xF) {
    case 1:
        return startinglistcnt;
    case 2:
        return runninglistcnt;
    case 3:
        return endinglistcnt;
    }
    return 0;
}

dds3ProcessID_t *dds3SearchProcessNumber(u32 type, s32 n)
{
    dds3ProcessID_t *p = startinglistf;
    s32 i;

    switch (type & 0xF) {
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

void DebugProcess(dds3ProcessID_t *pro, const char *mes)
{
    printf(D_0054E878, mes);
    while (pro != NULL) {
        printf("NAME:(%-24s) ", pro->name);
        printf("PID:(0x%08X) ", pro);
        printf(D_0054E880);
        if (pro->flag & 0x10) {
            printf(D_0054E888);
        } else {
            printf(D_0054E890);
        }
        if (pro->flag & 0x20) {
            printf(D_0054E898);
        } else {
            printf(D_0054E888);
        }
        printf(D_0054E8A0);
        printf("PRIO:(%5d) ", pro->priority);
        printf("RUNTIME:(%d) ", pro->runtime);
        printf("TIMER:(%d, %d)\n", pro->starttimer, pro->shutdowntimer);
        pro = pro->next;
    }
}

void dds3PrintProcessList(void)
{
    char *countFormat;

    DebugProcess(startinglistf, "*** starting ***************************");
    countFormat = "<<< %d >>>\n";
    printf(countFormat, startinglistcnt);
    DebugProcess(runninglistf, "*** running ****************************");
    printf(countFormat, runninglistcnt);
    DebugProcess(endinglistf, "*** ending *****************************");
    printf(countFormat, endinglistcnt);
    printf(D_0054E8A8);
}

void DebugProcessTree(dds3ProcessID_t *ps, s32 level)
{
    printf(D_0054E8B0, dds3ProcessTreeDepth, ps);
    if (ps->child != NULL) {
        if (ps->parent == NULL) {
            dds3ProcessTreeDepth[0] = ' ';
        } else {
            if (ps->sibling == NULL) {
                dds3ProcessTreeDepth[level * 4] = ' ';
            } else {
                dds3ProcessTreeDepth[level * 4] = '|';
            }
        }
        dds3ProcessTreeDepth[level * 4 + 4] = 0;
        DebugProcessTree(ps->child, level + 1);
        dds3ProcessTreeDepth[level * 4] = 0;
    }
    if (ps->sibling != NULL) {
        DebugProcessTree(ps->sibling, level);
    }
}

void dds3PrintProcessTree(void)
{
    dds3ProcessID_t *p;
    s32 i;

    printf("<<< process tree >>>\n");
    for (i = 0; i < 0x40; i++) {
        dds3ProcessTreeDepth[i] = ' ';
    }
    p = startinglistf;
    while (p != NULL) {
        if (p->parent == NULL) {
            dds3ProcessTreeDepth[0] = 0;
            DebugProcessTree(p, 0);
        }
        p = p->next;
    }
    p = runninglistf;
    while (p != NULL) {
        if (p->parent == NULL) {
            dds3ProcessTreeDepth[0] = 0;
            DebugProcessTree(p, 0);
        }
        p = p->next;
    }
    p = endinglistf;
    while (p != NULL) {
        if (p->parent == NULL) {
            dds3ProcessTreeDepth[0] = 0;
            DebugProcessTree(p, 0);
        }
        p = p->next;
    }
    printf(D_0054E8A8);
}

s32 dds3ProcessMain(void)
{
    StartingProcess();
    RunningProcess();
    dds3EndingProcess();
    return 1;
}

/* namehash is the name's byte sum, which dds3SearchProcessName uses to reject
 * most candidates without a compare. */
dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2, s32 v3,
                                  dds3ProcessFunc_t drawExit, dds3ProcessFunc_t deleteExit,
                                  void *p)
{
    dds3ProcessID_t *setp = sdfChipAllocDirect(sizeof(dds3ProcessID_t));
    s32 i = 0;

    if (setp == NULL) {
        return NULL;
    }
    setp->namehash = 0;
    while ((setp->name[i] = v1[i]) != 0 && i < 0x18) {
        setp->namehash += v1[i];
        i++;
    }
    setp->priority = dDS3_PRIORITY_DRAW_PB;
    setp->flag = 1;
    setp->starttimer = v2;
    setp->shutdowntimer = v3;
    setp->processfunc = drawExit;
    setp->shutdownfunc = deleteExit;
    setp->workdata = p;
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
dds3ProcessID_t *dds3SearchProcessName(char *namestr)
{
    s32 len;
    s32 hash;
    s32 list;
    s32 i;
    dds3ProcessID_t *p;

    p = NULL;
    hash = 0;
    len = 0;
    while (namestr[len] != '\0') {
        hash += namestr[len];
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
                    if (namestr[i] != p->name[i]) {
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

void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target)
{
    dds3ProcessID_t *p;

    if (target->parent != NULL) {
        dds3UnlinkProcess(target);
    }
    if (parent->child != NULL) {
        p = parent->child;
        while (p->sibling != NULL) {
            p = p->sibling;
        }
        p->sibling = target;
    } else {
        parent->child = target;
    }
    target->parent = parent;
}

void dds3UnlinkProcess(dds3ProcessID_t *target)
{
    dds3ProcessID_t *parent = target->parent;
    dds3ProcessID_t *p;

    if (parent == NULL) {
        return;
    }
    if (parent->child == target) {
        parent->child = target->sibling;
    } else {
        p = parent->child;
        while (p->sibling != target) {
            p = p->sibling;
        }
        p->sibling = target->sibling;
    }
    target->parent = NULL;
    target->sibling = NULL;
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

