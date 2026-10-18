#include "common.h"
#include "basic.h"

typedef s32 (*dds3BasicProc_t)(dds3Basic_t *basic);

typedef struct dds3AreaBasicWork_t
{
    dds3BasicArrayHandle_t *items[2];
} dds3AreaBasicWork_t;

extern char D_0052F7C0[]; /* "エリアリストが見つかりません...\n" */
extern char D_0052F7E8[]; /* "not exist a area  エリアが見つかりません...\n" */

extern s32 printf(const char *fmt, ...);
extern void *sdfChipAllocDirect(u32 size);
extern void sdfChipFreeDirect(void *ptr);
extern dds3Basic_t *dds3AddSceneBasicData(s32 type);
extern dds3Basic_t *dds3GetControlScene(void);
extern dds3BasicArrayHandle_t *dds3SceneBasicGetList(dds3Basic_t *scene, s32 type);
extern dds3BasicArrayHandle_t *dds3GetBasicArrayHandle(s32 count);
extern void dds3FreeBasicArrayHandle(dds3BasicArrayHandle_t *handle);
extern u32 dds3GetBasicArrayCount(dds3BasicArrayHandle_t *handle);
extern void dds3ForEachBasicArray(dds3BasicArrayHandle_t *handle, dds3BasicProc_t func);
extern s32 dds3SetFirstBasicArray(dds3BasicArrayHandle_t *handle);
extern s32 dds3SetNextBasicArray(dds3BasicArrayHandle_t *handle);
extern dds3Basic_t *dds3GetBasicArray(dds3BasicArrayHandle_t *handle);
extern s32 dds3AddBasicArray(dds3BasicArrayHandle_t *handle, s32 count);
extern s32 dds3SetBasicArray(dds3BasicArrayHandle_t *handle, dds3Basic_t *basic);
extern s32 dds3SetSearchNextBasicArray(dds3BasicArrayHandle_t *handle, dds3Basic_t *basic);
extern s32 dds3SearchDeleteBasicArray(dds3BasicArrayHandle_t *handle, dds3Basic_t *basic, u32 mode);
extern dds3Basic_t *dds3ObjectBaseSetControlBasicType(dds3Basic_t *object, dds3Basic_t *item, s32 itemType);

s32 CB_ReleaseObjectBasic(dds3Basic_t *basic);
s32 CB_BasicDrawProcuder(dds3Basic_t *basic);
u32 dds3AreaBasicTypeHasItems(u32 type);
dds3BasicArrayHandle_t *dds3AreaBasicReleaseItem(dds3Basic_t *area, u32 itemType);

s32 AreaBasicConstructor(dds3Basic_t *basic)
{
    dds3AreaBasicWork_t *work;
    u32 i;

    work = sdfChipAllocDirect(sizeof(dds3AreaBasicWork_t));
    basic->workdata = work;
    for (i = 0; i < 2; i++) {
        work->items[i] = dds3GetBasicArrayHandle(0);
    }
    return 1;
}

void AreaBasicDestructor(dds3Basic_t *basic)
{
    dds3AreaBasicWork_t *work;
    u32 i;

    work = basic->workdata;
    dds3ForEachBasicArray(work->items[0], CB_ReleaseObjectBasic);
    dds3ForEachBasicArray(work->items[1], CB_ReleaseObjectBasic);
    for (i = 0; i < 2; i++) {
        dds3FreeBasicArrayHandle(work->items[i]);
    }
    sdfChipFreeDirect(work);
}

s32 AreaBasicMainprocuder(dds3Basic_t *basic)
{
    return 1;
}

s32 AreaBasicDrawprocuder(dds3Basic_t *basic)
{
    dds3AreaBasicWork_t *work = basic->workdata;

    dds3ForEachBasicArray(work->items[0], CB_BasicDrawProcuder);
    return 1;
}

s32 CB_ReleaseObjectBasic(dds3Basic_t *basic)
{
    dds3ObjectBaseSetControlBasicType(basic, NULL, 5);
    return 1;
}

s32 CB_BasicDrawProcuder(dds3Basic_t *basic)
{
    if (basic->basicfunction->drawprocuder != NULL) {
        return ((dds3BasicProc_t)basic->basicfunction->drawprocuder)(basic);
    }
    return 1;
}

u32 dds3AreaBasicTypeHasItems(u32 type)
{
    u32 ret = 0;

    if (type >= BASIC_TYPE_CAMERAOBJECT) {
        if (type >= BASIC_TYPE_EVENTOBJECT) {
            ret = type == BASIC_TYPE_EVENTOBJECT;
        }
    }
    return ret;
}

dds3Basic_t *dds3AddAreaBasic(u32 serialID, char *name)
{
    dds3Basic_t *basic = dds3AddSceneBasicData(BASIC_TYPE_AREA);

    basic->serialID = serialID;
    basic->name = name;
    return basic;
}

void dds3AreaBasicRegistItem(dds3Basic_t *area, dds3Basic_t *item)
{
    dds3BasicArrayHandle_t *handle;

    handle = dds3AreaBasicReleaseItem(area, dds3AreaBasicTypeHasItems(item->type >> 24));
    dds3SetFirstBasicArray(handle);
    if (dds3SetSearchNextBasicArray(handle, item) != 1) {
        dds3AddBasicArray(handle, 1);
        dds3SetBasicArray(handle, item);
    }
}

/* Remove the first matching entry from the list selected by the item's type. */
void dds3AreaBasicGetItemArray(dds3Basic_t *area, dds3Basic_t *item)
{
    dds3BasicArrayHandle_t *handle =
        dds3AreaBasicReleaseItem(area, dds3AreaBasicTypeHasItems(item->type >> 24));

    dds3SearchDeleteBasicArray(handle, item, DDS3_BASICARRAY_SERACHDELETE_MODE_FIRST);
    /* Retail and DDS1 dds3RemoveObjectFromOwnerWorldIndex retain a JAL tail.
     * The consumed list cursor is no longer needed after removal. */
    handle = NULL;
}

dds3BasicArrayHandle_t *dds3AreaBasicReleaseItem(dds3Basic_t *area, u32 itemType)
{
    dds3AreaBasicWork_t *work = area->workdata;

    return work->items[itemType];
}

dds3BasicArrayHandle_t *dds3AreaBasicArrangeGetItemArray(dds3Basic_t *area, u32 itemType, dds3BasicProc_t check)
{
    dds3BasicArrayHandle_t *src;
    dds3BasicArrayHandle_t *dst;
    dds3Basic_t *basic;

    src = dds3AreaBasicReleaseItem(area, itemType);
    if (dds3GetBasicArrayCount(src) == 0) {
        return NULL;
    }

    dst = dds3GetBasicArrayHandle(0);
    dds3SetFirstBasicArray(src);
    do {
        basic = dds3GetBasicArray(src);
        if (check == NULL || check(basic) != 0) {
            dds3AddBasicArray(dst, 1);
            dds3SetBasicArray(dst, basic);
        }
    } while (dds3SetNextBasicArray(src) != 0);

    if (dds3GetBasicArrayCount(dst) == 0) {
        dds3FreeBasicArrayHandle(dst);
        return NULL;
    }
    return dst;
}

dds3Basic_t *dds3AreaBasicGetFastArea(void)
{
    dds3BasicArrayHandle_t *handle;
    dds3Basic_t *area;

    handle = dds3SceneBasicGetList(dds3GetControlScene(), BASIC_TYPE_AREA);
    if (handle == NULL) {
        printf(D_0052F7C0);
        return NULL;
    }
    if (dds3GetBasicArrayCount(handle) == 0) {
        printf(D_0052F7E8);
        return NULL;
    }

    dds3SetFirstBasicArray(handle);
    area = dds3GetBasicArray(handle);
    dds3FreeBasicArrayHandle(handle);
    return area;
}

INCLUDE_RODATA(const s32, "basic/dds3AreaBasic", D_0052F7C0);

INCLUDE_RODATA(const s32, "basic/dds3AreaBasic", D_0052F7E8);

