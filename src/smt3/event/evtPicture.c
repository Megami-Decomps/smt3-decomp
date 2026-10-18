#include "common.h"
#include "kernel.h"

/* 0x68-byte picture work block; InitPictureProcessData fills the rest. */
typedef struct
{
    u32 flags;
    void *texture;
    s32 ot;
    u8 unk0C[0x20];
    s32 priority;
    u8 unk30[0x38];
} evtPictureWork_t;

extern char D_0054F8F0[];

extern void *sdfChipAllocDirect(u32 size);
extern void sdfChipFreeDirect(void *ptr);
extern void sdfTexFreeDelayed(void *tex);
extern void *itfMiscLoadTexture(const char *path);
extern dds3ProcessID_t *dds3StartProcess(char *namestr, u32 priority, s32 starttimer, s32 shutdowntimer,
                                         dds3ProcessFunc_t processfunc, dds3ProcessFunc_t shutdownfunc,
                                         void *data);
extern void *dds3GetProcessData(dds3ProcessID_t *id);
extern evtPictureWork_t *InitPictureProcessData(void);
extern s32 evtPictureProcess1(dds3ProcessID_t *id);
extern void *evtPicturePriList;
extern void evtPicturePriDraw(void);
extern void evtPicturePriClear(void);
extern s32 dds3KillProcess(dds3ProcessID_t *id, s32 hierarchy);

INCLUDE_ASM(const s32, "event/evtPicture", evtPicturePriClear);

INCLUDE_ASM(const s32, "event/evtPicture", evtPicturePriAdd);

INCLUDE_ASM(const s32, "event/evtPicture", evtPicturePriDraw);

void evtPictureDrawOn(dds3ProcessID_t *process)
{
    evtPictureWork_t *work = dds3GetProcessData(process);
    work->flags |= 1;
}

void evtPictureDrawOff(dds3ProcessID_t *process)
{
    evtPictureWork_t *work = dds3GetProcessData(process);
    work->flags &= ~1;
}

void evtKillPictureProcess(dds3ProcessID_t *process)
{
    dds3KillProcess(process, 1);
}

void evtPictureSetOt(dds3ProcessID_t *process, s32 ot)
{
    evtPictureWork_t *work = dds3GetProcessData(process);
    work->ot = ot;
}

void evtPictureSetPri(dds3ProcessID_t *process, s32 priority)
{
    evtPictureWork_t *work = dds3GetProcessData(process);
    work->priority = priority;
}

INCLUDE_ASM(const s32, "event/evtPicture", evtPictureSetSize);

INCLUDE_ASM(const s32, "event/evtPicture", evtPictureSetPos);

INCLUDE_ASM(const s32, "event/evtPicture", evtPictureSetAlpha);

INCLUDE_ASM(const s32, "event/evtPicture", evtPictureSetZoom);

void evtPictureSwap(void) {
}

INCLUDE_ASM(const s32, "event/evtPicture", evtPictureStat);

INCLUDE_ASM(const s32, "event/evtPicture", InitPictureProcessData);

void LoadTexture(evtPictureWork_t *work, const char *path)
{
    work->texture = itfMiscLoadTexture(path);
}

INCLUDE_ASM(const s32, "event/evtPicture", evtPictureProcess1);

s32 evtPictureShutdown(dds3ProcessID_t *id)
{
    evtPictureWork_t *work = dds3GetProcessData(id);

    if (work->texture != NULL) {
        sdfTexFreeDelayed(work->texture);
        work->texture = NULL;
    }
    sdfChipFreeDirect(work);
}

dds3ProcessID_t *evtStartPictureProcess2(u32 priority, const char *path)
{
    evtPictureWork_t *work = InitPictureProcessData();

    LoadTexture(work, path);
    return dds3StartProcess(D_0054F8F0, priority, 1, 1, evtPictureProcess1, evtPictureShutdown, work);
}

dds3ProcessID_t *evtStartPictureProcess(u32 priority, void *texture)
{
    evtPictureWork_t *work = InitPictureProcessData();

    work->texture = texture;
    return dds3StartProcess(D_0054F8F0, priority, 1, 1, evtPictureProcess1, evtPictureShutdown, work);
}

s32 picpri_Process(dds3ProcessID_t *process)
{
    dds3GetProcessData(process);
    evtPicturePriDraw();
    evtPicturePriClear();
    return 0;
}

void picpri_Shutdown(dds3ProcessID_t *process)
{
    sdfChipFreeDirect(dds3GetProcessData(process));
    evtPicturePriList = NULL;
}

INCLUDE_ASM(const s32, "event/evtPicture", evtPicturePriProcessInit);
INCLUDE_SDATA(const s32, "event/evtPicture", evtPicturePriList);

INCLUDE_SDATA(const s32, "event/evtPicture", D_0054F8EC);

INCLUDE_SDATA(const s32, "event/evtPicture", D_0054F8F0);

INCLUDE_SDATA(const s32, "event/evtPicture", D_0054F8F8);

