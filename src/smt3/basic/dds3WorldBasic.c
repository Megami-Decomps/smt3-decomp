#include "common.h"
#include "basic.h"
#include "libsdf.h"

extern dds3Basic_t *dds3TheWorld;

void *sdfChipAllocDirect(u32 size);
void *sdfMemAllocDirect(u32 size);
void *sdfMemLock(sdfMemHandle_t *handle);
void sdfMemFreeDirect(sdfMemHandle_t *handle);
s32 dds3CallBasicMainProc(dds3Basic_t *basic);
s32 dds3CallBasicDrawProc(dds3Basic_t *basic);
void dds3DeleteScene(dds3Basic_t *scene);
void dds3FreeBasicArrayHandle(dds3BasicArrayHandle_t *handle);
u32 dds3SetBasicArray(dds3BasicArrayHandle_t *handle, dds3Basic_t *basic);
void sdfChipFreeDirect(void *ptr);
dds3Basic_t *dds3AllocBasic(u32 type);
void dds3DeleteBasic(dds3Basic_t *basic);
u32 dds3AddBasicArray(dds3BasicArrayHandle_t *handle, s32 count);
u16 dds3GetBasicArrayCount(const dds3BasicArrayHandle_t *handle);
dds3Basic_t *dds3GetBasicArray(const dds3BasicArrayHandle_t *handle);
u32 dds3SetFirstBasicArray(dds3BasicArrayHandle_t *handle);
u32 dds3SetNextBasicArray(dds3BasicArrayHandle_t *handle);
void dds3DeleteBasicArray(dds3BasicArrayHandle_t *handle);
void dds3DeleteAllBasicArray(dds3BasicArrayHandle_t *handle);
s32 dds3SetSearchNextBasicArray(dds3BasicArrayHandle_t *handle, const dds3Basic_t *basic);

s32 worldconstructor(dds3Basic_t *basic)
{
    dds3WorldBasic_t *world;

    world = sdfChipAllocDirect(sizeof(dds3WorldBasic_t));
    if (world == NULL) {
        return 0;
    }
    world->firstscene = NULL;
    world->lastscene = NULL;
    world->activescene = NULL;
    world->controlscene = NULL;
    world->basicarray_hndl = NULL;
    world->basicarray = NULL;
    world->basicarraycnt = 0;
    world->firstempty = -1;
    world->emptycount = 0;
    world->firstheader = NULL;
    world->lastheader = NULL;
    world->lastempty = -1;
    basic->workdata = world;
    return 1;
}

void worlddestructor(dds3Basic_t *basic)
{
    dds3WorldBasic_t *world;

    world = basic->workdata;
    if (world == NULL) {
        return;
    }
    while (world->firstscene != NULL) {
        dds3DeleteScene(world->firstscene);
    }
    while (world->firstheader != NULL) {
        dds3FreeBasicArrayHandle(world->firstheader);
    }
    if (world->basicarray_hndl != NULL) {
        sdfMemFreeDirect(world->basicarray_hndl);
    }
    sdfChipFreeDirect(world);
}

s32 worldmainprocuder(dds3Basic_t *basic)
{
    dds3Basic_t *scene;

    scene = ((dds3WorldBasic_t *)basic->workdata)->activescene;
    if (scene != NULL) {
        dds3CallBasicMainProc(scene);
    }
    return 1;
}

s32 worlddrawprocuder(dds3Basic_t *basic)
{
    dds3Basic_t *scene;

    scene = ((dds3WorldBasic_t *)basic->workdata)->activescene;
    if (scene != NULL) {
        dds3CallBasicDrawProc(scene);
    }
    return 1;
}

dds3Basic_t *dds3AddWorld(s32 count)
{
    dds3Basic_t *world;
    dds3WorldBasic_t *work;
    sdfMemHandle_t *handle;
    s32 i;

    if (dds3TheWorld != NULL) {
        return NULL;
    }
    if ((u32)(count - 32) >= 0x7FE0) {
        return NULL;
    }
    world = dds3AllocBasic(BASIC_TYPE_WORLD);
    work = world->workdata;
    handle = sdfMemAllocDirect(count * sizeof(dds3BasicArray_t));
    work->basicarray_hndl = handle;
    work->basicarray = sdfMemLock(handle);
    work->basicarraycnt = count;
    for (i = 0; i < count; i++) {
        if (i == 0) {
            work->basicarray[i].prev = -1;
        } else {
            work->basicarray[i].prev = i - 1;
        }
        if (i == count - 1) {
            work->basicarray[i].next = -1;
        } else {
            work->basicarray[i].next = i + 1;
        }
        work->basicarray[i].target = NULL;
    }
    work->firstempty = 0;
    work->lastempty = count - 1;
    work->emptycount = count;
    dds3TheWorld = world;
    return world;
}

void dds3DeleteWorld(void)
{
    if (dds3TheWorld != NULL) {
        dds3DeleteBasic(dds3TheWorld);
        dds3TheWorld = NULL;
    }
}

void dds3SetActiveScene(dds3Basic_t *scene)
{
    if (dds3TheWorld != NULL) {
        ((dds3WorldBasic_t *)dds3TheWorld->workdata)->activescene = scene;
    }
}

dds3Basic_t *dds3GetActiveScene(void)
{
    dds3Basic_t *world;

    world = dds3TheWorld;
    if (world == NULL) {
        return NULL;
    }
    return ((dds3WorldBasic_t *)world->workdata)->activescene;
}

void dds3SetControlScene(dds3Basic_t *scene)
{
    if (dds3TheWorld != NULL) {
        ((dds3WorldBasic_t *)dds3TheWorld->workdata)->controlscene = scene;
    }
}

dds3Basic_t *dds3GetControlScene(void)
{
    dds3Basic_t *world;

    world = dds3TheWorld;
    if (world == NULL) {
        return NULL;
    }
    return ((dds3WorldBasic_t *)world->workdata)->controlscene;
}

dds3Basic_t *dds3AddScene(void)
{
    dds3WorldBasic_t *world;
    dds3Basic_t *scene;

    if (dds3TheWorld == NULL) {
        return NULL;
    }
    world = (dds3WorldBasic_t *)dds3TheWorld->workdata;
    scene = dds3AllocBasic(BASIC_TYPE_SCENE);
    if (scene == NULL) {
        return NULL;
    }
    if (world->lastscene == NULL) {
        world->firstscene = scene;
        world->lastscene = scene;
    } else {
        world->lastscene->next = scene;
        scene->prev = world->lastscene;
        world->lastscene = scene;
    }
    return scene;
}

void dds3DeleteScene(dds3Basic_t *scene)
{
    dds3WorldBasic_t *world;

    if (scene == NULL) {
        return;
    }
    if (dds3TheWorld == NULL) {
        return;
    }
    world = (dds3WorldBasic_t *)dds3TheWorld->workdata;
    if (world->firstscene == scene) {
        world->firstscene = scene->next;
    }
    if (world->lastscene == scene) {
        world->lastscene = scene->prev;
    }
    if (world->activescene == scene) {
        world->activescene = NULL;
    }
    if (world->controlscene == scene) {
        world->controlscene = NULL;
    }
    dds3DeleteBasic(scene);
}

dds3BasicArrayHandle_t *dds3GetBasicArrayHandle(s32 count)
{
    dds3WorldBasic_t *world;
    dds3BasicArrayHandle_t *handle;

    if (dds3TheWorld == NULL) {
        return NULL;
    }
    world = (dds3WorldBasic_t *)dds3TheWorld->workdata;
    if (world->emptycount < count) {
        return NULL;
    }
    handle = sdfChipAllocDirect(sizeof(dds3BasicArrayHandle_t));
    if (handle == NULL) {
        return NULL;
    }
    handle->prev = NULL;
    handle->next = NULL;
    handle->firstindex = -1;
    handle->lastindex = -1;
    handle->nowindex = -1;
    handle->count = 0;
    if (world->lastheader == NULL) {
        world->firstheader = handle;
        world->lastheader = handle;
    } else {
        world->lastheader->next = handle;
        handle->prev = world->lastheader;
        world->lastheader = handle;
    }
    dds3AddBasicArray(handle, count);
    return handle;
}

void dds3FreeBasicArrayHandle(dds3BasicArrayHandle_t *handle)
{
    dds3WorldBasic_t *world;

    if (handle == NULL) {
        return;
    }
    if (dds3TheWorld == NULL) {
        return;
    }
    world = (dds3WorldBasic_t *)dds3TheWorld->workdata;
    dds3DeleteAllBasicArray(handle);
    if (handle->prev == NULL) {
        world->firstheader = handle->next;
    } else {
        handle->prev->next = handle->next;
    }
    if (handle->next == NULL) {
        world->lastheader = handle->prev;
    } else {
        handle->next->prev = handle->prev;
    }
    sdfChipFreeDirect(handle);
}

/* The cursor advances to the slot after the one deleted. */
void dds3DeleteBasicArray(dds3BasicArrayHandle_t *handle)
{
    dds3WorldBasic_t *world;
    dds3BasicArray_t *array;
    s16 prev;
    s16 next;

    if (handle == NULL) {
        return;
    }
    if (handle->count == 0) {
        return;
    }
    if (handle->nowindex < 0) {
        return;
    }
    if (dds3TheWorld == NULL) {
        return;
    }
    world = (dds3WorldBasic_t *)dds3TheWorld->workdata;
    array = world->basicarray;
    prev = array[handle->nowindex].prev;
    array[handle->nowindex].target = NULL;
    next = array[handle->nowindex].next;
    if (prev < 0) {
        handle->firstindex = next;
    } else {
        array[prev].next = next;
    }
    if (next < 0) {
        handle->lastindex = prev;
    } else {
        array[next].prev = prev;
    }
    array[handle->nowindex].prev = -1;
    array[handle->nowindex].next = -1;
    if (world->lastempty < 0) {
        world->firstempty = handle->nowindex;
        world->lastempty = handle->nowindex;
    } else {
        array[world->lastempty].next = handle->nowindex;
        array[handle->nowindex].prev = world->lastempty;
        world->lastempty = handle->nowindex;
    }
    world->emptycount++;
    handle->nowindex = next;
    handle->count--;
}

void dds3DeleteAllBasicArray(dds3BasicArrayHandle_t *handle)
{
    dds3WorldBasic_t *world;
    s32 index;

    if (handle == NULL) {
        return;
    }
    if (handle->count == 0) {
        return;
    }
    if (dds3TheWorld == NULL) {
        return;
    }
    world = dds3TheWorld->workdata;
    index = handle->firstindex;
    while (index >= 0) {
        world->basicarray[index].target = NULL;
        index = world->basicarray[index].next;
    }
    if (world->lastempty < 0) {
        world->firstempty = handle->firstindex;
        world->lastempty = handle->lastindex;
    } else {
        world->basicarray[world->lastempty].next = handle->firstindex;
        world->basicarray[handle->firstindex].prev = world->lastempty;
        world->lastempty = handle->lastindex;
    }
    world->emptycount += handle->count;
    handle->count = 0;
    handle->firstindex = -1;
    handle->lastindex = -1;
    handle->nowindex = -1;
}

s32 dds3SearchDeleteBasicArray(dds3BasicArrayHandle_t *handle, dds3Basic_t *basic, s32 all)
{
    s32 deleted;

    deleted = 0;
    if (dds3GetBasicArrayCount(handle) != 0) {
        dds3SetFirstBasicArray(handle);
        do {
            if (dds3SetSearchNextBasicArray(handle, basic) != 1) {
                break;
            }
            dds3DeleteBasicArray(handle);
            deleted = 1;
        } while (all != 0);
    }
    return deleted;
}

/* The walk stops at the first empty slot, so a NULL basic never matches. */
s32 dds3SetSearchNextBasicArray(dds3BasicArrayHandle_t *handle, const dds3Basic_t *basic)
{
    dds3Basic_t *target;

    do {
        target = dds3GetBasicArray(handle);
        if (target == NULL) {
            return 0;
        }
        if (basic == target) {
            return 1;
        }
    } while (dds3SetNextBasicArray(handle) != 0);
    return 0;
}

u32 dds3AddBasicArray(dds3BasicArrayHandle_t *handle, s32 count)
{
    dds3WorldBasic_t *world;
    s16 first;
    s16 last;
    s32 i;

    if (count == 0) {
        return 1;
    }
    if (dds3TheWorld == NULL) {
        return 0;
    }
    world = dds3TheWorld->workdata;
    if (world->emptycount < count || count < 0) {
        return 0;
    }
    if (handle == NULL) {
        return 0;
    }
    first = world->firstempty;
    for (i = 0; i < count - 1; i++) {
        world->firstempty = world->basicarray[world->firstempty].next;
    }
    last = world->firstempty;
    world->firstempty = world->basicarray[last].next;
    if (world->firstempty == -1) {
        world->lastempty = -1;
    } else {
        world->basicarray[last].next = -1;
        world->basicarray[world->firstempty].prev = -1;
    }
    world->emptycount -= count;
    if (handle->lastindex < 0) {
        handle->lastindex = last;
        handle->firstindex = first;
    } else {
        world->basicarray[handle->lastindex].next = first;
        world->basicarray[first].prev = handle->lastindex;
        handle->lastindex = last;
    }
    handle->nowindex = first;
    handle->count += count;
    return 1;
}

u16 dds3GetBasicArrayCount(const dds3BasicArrayHandle_t *handle)
{
    u16 count;

    count = 0;
    if (handle != NULL) {
        count = handle->count;
    }
    return count;
}

u32 dds3SetBasicArray(dds3BasicArrayHandle_t *handle, dds3Basic_t *basic)
{
    if (handle->count == 0) {
        return 0;
    }
    if (handle->nowindex < 0) {
        return 0;
    }
    ((dds3WorldBasic_t *)dds3TheWorld->workdata)->basicarray[handle->nowindex].target = basic;
    return 1;
}

dds3Basic_t *dds3GetBasicArray(const dds3BasicArrayHandle_t *handle)
{
    if (handle->count == 0) {
        return NULL;
    }
    if (handle->nowindex < 0) {
        return NULL;
    }
    return ((dds3WorldBasic_t *)dds3TheWorld->workdata)->basicarray[handle->nowindex].target;
}

u32 dds3SetFirstBasicArray(dds3BasicArrayHandle_t *handle)
{
    handle->nowindex = handle->firstindex;
    return handle->firstindex >= 0;
}

u32 dds3SetNextBasicArray(dds3BasicArrayHandle_t *handle)
{
    if (handle->nowindex < 0) {
        return 0;
    }
    handle->nowindex =
        ((dds3WorldBasic_t *)dds3TheWorld->workdata)->basicarray[handle->nowindex].next;
    return handle->nowindex >= 0;
}

s32 dds3ForEachBasicArray(dds3BasicArrayHandle_t *handle, s32 (*func)(dds3Basic_t *))
{
    u32 valid;

    if (func == NULL) {
        return 1;
    }
    if (dds3GetBasicArrayCount(handle) == 0) {
        return 1;
    }
    for (valid = dds3SetFirstBasicArray(handle); valid != 0;
         valid = dds3SetNextBasicArray(handle)) {
        if (func(dds3GetBasicArray(handle)) == 0) {
            return 0;
        }
    }
    return 1;
}
