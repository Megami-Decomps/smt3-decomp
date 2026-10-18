#include "common.h"
#include "basic.h"
#include "kernel.h"

extern dds3Basic_t *dds3GetControlScene(void);
extern dds3Basic_t *dds3AllocBasic(u32 type);
extern void dds3DeleteBasic(dds3Basic_t *basic);
extern void drawSceneBasicList(void);
extern dds3BasicArrayHandle_t *dds3GetBasicArrayHandle(s32 count);
extern s32 dds3AddBasicArray(dds3BasicArrayHandle_t *handle, s32 count);
extern s32 dds3SetBasicArray(dds3BasicArrayHandle_t *handle, dds3Basic_t *basic);
extern u32 fldLoadFile(const char *name);
extern u32 fldLoadFile_FromMemory(u32 area, u32 room, void *resource0,
                                  void *resource1, void *textureOffsets, u32 flags);
extern void fldFreeFile(void);
extern void *sdfMemLock(sdfMemHandle_t *handle);
extern void sdfMemFreeDirect(sdfMemHandle_t *handle);
extern dds3ProcessID_t *dds3SearchProcessName(const char *name);
extern void dds3KillProcess(dds3ProcessID_t *process, s32 mode);

dds3Basic_t *dds3GetActiveCameraSceneBasic(dds3Basic_t *scene);
dds3Basic_t *dds3GetActiveUnitSceneBasic(dds3Basic_t *scene);
void dds3SceneBasicReleaseScriptFile(dds3Basic_t *scene);
dds3Basic_t *dds3SceneBasicSearchNameAndType(dds3Basic_t *scene, s32 type, const char *name);


INCLUDE_ASM(const s32, "basic/dds3SceneBasic", sceneconstructor);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", scenedestructor);

/* Preserve the next link across callbacks that can delete their own basic. */
u32 scenemainprocuder(dds3Basic_t *scene)
{
    dds3SceneWork_t *work = scene->workdata;
    dds3Basic_t *basic;
    dds3Basic_t *next;
    s32 type;

    if (work == NULL) {
        return 0;
    }
    for (type = 0; type <= 0x11; type++) {
        if (work->lists[type].count != 0) {
            basic = work->lists[type].head;
            if (basic->basicfunction != NULL && basic->basicfunction->mainprocuder != NULL) {
                do {
                    next = basic->next;
                    ((void (*)(dds3Basic_t *))basic->basicfunction->mainprocuder)(basic);
                    basic = next;
                } while (next != NULL);
            }
        }
    }
    return 1;
}

u32 scenedrawprocuder(dds3Basic_t *scene)
{
    dds3SceneWork_t *work = scene->workdata;
    dds3Basic_t *basic;
    dds3Basic_t *next;

    if (work == NULL) {
        return 0;
    }
    if (work->visible == 0) {
        return 1;
    }
    if (work->lists[BASIC_TYPE_AREA].count == 0) {
        return 1;
    }
    basic = work->lists[BASIC_TYPE_AREA].head;
    if (basic->basicfunction == NULL) {
        return 1;
    }
    if (basic->basicfunction->drawprocuder == NULL) {
        return 1;
    }
    drawSceneBasicList();
    do {
        next = basic->next;
        ((void (*)(dds3Basic_t *))basic->basicfunction->drawprocuder)(basic);
        basic = next;
    } while (next != NULL);
    return 1;
}

void dds3VisibleSceneBasic(dds3Basic_t *scene, s8 visible)
{
    if ((dds3SceneWork_t *)scene->workdata != NULL) {
        ((dds3SceneWork_t *)scene->workdata)->visible = visible;
    }
}

dds3Basic_t *dds3AddSceneBasicData(s32 type)
{
    dds3Basic_t *scene;
    dds3Basic_t *basic;
    dds3SceneList_t *list;

    if ((u32)(type - 2) >= 16) {
        return NULL;
    }
    scene = dds3GetControlScene();
    if (scene == NULL) {
        return NULL;
    }
    list = &((dds3SceneWork_t *)scene->workdata)->lists[type];
    basic = dds3AllocBasic(type);
    if (basic == NULL) {
        return NULL;
    }
    basic->workpointer = scene;
    if (list->tail == NULL) {
        list->head = basic;
        list->tail = basic;
    } else {
        list->tail->next = basic;
        basic->prev = list->tail;
        list->tail = basic;
    }
    list->count++;
    return basic;
}

void dds3DeleteSceneBasicData(dds3Basic_t *basic)
{
    dds3SceneList_t *list;

    if (basic != NULL && (u32)((basic->type >> 24) - 2) < 16) {
        list = &((dds3SceneWork_t *)basic->workpointer->workdata)->lists[basic->type >> 24];
        if (list->head == basic) {
            list->head = basic->next;
        }
        if (list->tail == basic) {
            list->tail = basic->prev;
        }
        dds3DeleteBasic(basic);
        list->count--;
    }
}

dds3Basic_t *dds3SetActiveCameraSceneBasic(dds3Basic_t *scene, dds3Basic_t *camera)
{
    dds3SceneWork_t *work;
    dds3Basic_t *previous;

    work = scene->workdata;
    previous = dds3GetActiveCameraSceneBasic(scene);
    work->camera = camera;
    return previous;
}

dds3Basic_t *dds3GetActiveCameraSceneBasic(dds3Basic_t *scene)
{
    return ((dds3SceneWork_t *)scene->workdata)->camera;
}

dds3Basic_t *dds3SetActiveUnitSceneBasic(dds3Basic_t *scene, dds3Basic_t *unit)
{
    dds3SceneWork_t *work;
    dds3Basic_t *previous;

    work = scene->workdata;
    previous = dds3GetActiveUnitSceneBasic(scene);
    work->unit = unit;
    return previous;
}

dds3Basic_t *dds3GetActiveUnitSceneBasic(dds3Basic_t *scene)
{
    return ((dds3SceneWork_t *)scene->workdata)->unit;
}

dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *scene, u32 serialID, s32 type)
{
    dds3Basic_t *basic;

    if (((dds3SceneWork_t *)scene->workdata)->lists[type].count == 0) {
        return NULL;
    }
    basic = ((dds3SceneWork_t *)scene->workdata)->lists[type].head;
    do {
        if (serialID == basic->serialID) {
            return basic;
        }
        basic = basic->next;
    } while (basic != NULL);
    return NULL;
}

dds3BasicArrayHandle_t *dds3SceneBasicGetList(dds3Basic_t *scene, s32 type)
{
    dds3SceneWork_t *work = scene->workdata;
    dds3BasicArrayHandle_t *handle;
    dds3Basic_t *basic;

    if (work->lists[type].count == 0) {
        return NULL;
    }
    handle = dds3GetBasicArrayHandle(0);
    basic = work->lists[type].head;
    do {
        dds3AddBasicArray(handle, 1);
        dds3SetBasicArray(handle, basic);
        basic = basic->next;
    } while (basic != NULL);
    return handle;
}

void dds3SceneBasicMakeScene(dds3Basic_t *scene, const char *name)
{
    dds3SceneWork_t *work;
    u32 handle;

    work = scene->workdata;
    handle = fldLoadFile(name);
    work->fieldHandle = handle;
}

void dds3SceneBasicMakeScene_FromMemory(dds3Basic_t *scene, u32 area, u32 room,
                                       void *resource0, void *resource1,
                                       void *textureOffsets, u32 flags)
{
    dds3SceneWork_t *work;
    u32 handle;

    work = scene->workdata;
    handle = fldLoadFile_FromMemory(area, room, resource0, resource1, textureOffsets, flags);
    work->fieldHandle = handle;
}

void dds3SceneBasicFreeScene(dds3Basic_t *scene)
{
    dds3SceneWork_t *work;

    work = scene->workdata;
    fldFreeFile();
    work->fieldHandle = 0;
}

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", dds3SceneBasicLoadScriptFile);

/* A null handle preserves the old script; replacement releases it first. */
s32 dds3SceneBasicLoadScriptFile_FromMem(dds3Basic_t *scene, sdfMemHandle_t *handle)
{
    s32 notAttached = 0;
    void *address;
    dds3SceneWork_t *work = scene->workdata;

    if (handle == NULL) {
        return notAttached;
    }
    if (work->scriptHandle != NULL) {
        dds3SceneBasicReleaseScriptFile(scene);
    }
    address = sdfMemLock(handle);
    if (address != NULL) {
        work->scriptHandle = handle;
        work->script = address;
        return 1;
    }
    return notAttached;
}

void dds3SceneBasicReleaseScriptFile(dds3Basic_t *scene)
{
    dds3SceneWork_t *work = scene->workdata;

    if (work->scriptHandle != NULL) {
        sdfMemFreeDirect(work->scriptHandle);
    }
    work->scriptHandle = NULL;
    work->script = NULL;
}

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", dds3SceneBasicStartScript);

void dds3SceneBasicEndScript(dds3Basic_t *scene, const char *name)
{
    dds3ProcessID_t *process;

    if (name != NULL) {
        process = dds3SearchProcessName(name);
        if (process == NULL) {
            return;
        }
        dds3KillProcess(process, 0);
    }
}

s32 dds3SceneBasicExistBasicFromType(dds3Basic_t *scene, s32 type, dds3Basic_t *target)
{
    dds3Basic_t *basic;

    if (scene == NULL || target == NULL) {
        return 0;
    }
    basic = ((dds3SceneWork_t *)scene->workdata)->lists[type].head;
    while (basic != NULL) {
        if (basic == target) {
            return 1;
        }
        basic = basic->next;
    }
    return 0;
}

s32 dds3SceneBasicExistBasic(dds3Basic_t *scene, dds3Basic_t *target)
{
    dds3SceneList_t *lists;
    s32 type;

    if (scene == NULL || target == NULL) {
        return 0;
    }
    lists = ((dds3SceneWork_t *)scene->workdata)->lists;
    for (type = 0; type < BASIC_TYPE_MAX; type++) {
        dds3Basic_t *basic = lists[type].head;

        if (basic == NULL) {
            continue;
        }
        do {
            if (basic == target) {
                return 1;
            }
            basic = basic->next;
        } while (basic != NULL);
    }
    return 0;
}

void dds3SetMapNo(dds3Basic_t *scene, u32 mapNo)
{
    if (scene != NULL) {
        ((dds3SceneWork_t *)scene->workdata)->mapNo = mapNo;
    }
}

s32 dds3GetMapNo(dds3Basic_t *scene)
{
    if (scene == NULL) {
        return -1;
    }
    return ((dds3SceneWork_t *)scene->workdata)->mapNo;
}

dds3Basic_t *dds3SceneBasicSearchNameAndType(dds3Basic_t *scene, s32 type, const char *name)
{
    dds3Basic_t *basic;
    u8 *candidate;
    s32 i;

    if (scene == NULL || name == NULL) {
        return NULL;
    }
    basic = ((dds3SceneWork_t *)scene->workdata)->lists[type].head;
    while (basic != NULL) {
        candidate = (u8 *)basic->name;
        if (candidate == NULL) {
            basic = basic->next;
            continue;
        }
        if (candidate[0] == '\0') {
            basic = basic->next;
            continue;
        }
        i = 0;
        if (((const u8 *)name)[0] == candidate[0]) {
            do {
                if (((const u8 *)name)[i] == '\0') {
                    return basic;
                }
                i++;
            } while (((const u8 *)name)[i] == ((u8 *)basic->name)[i]);
        }
        basic = basic->next;
    }
    return NULL;
}

dds3Basic_t *dds3SceneBasicSearchName(dds3Basic_t *scene, const char *name)
{
    dds3Basic_t *basic;
    s32 type;

    if (scene == NULL || name == NULL) {
        return NULL;
    }
    for (type = 0; type < BASIC_TYPE_MAX; type++) {
        basic = dds3SceneBasicSearchNameAndType(scene, type, name);
        if (basic != NULL) {
            return basic;
        }
    }
    return NULL;
}
