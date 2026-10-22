#include "common.h"
#include "basic.h"
#include "kernel.h"
#include "newdata.h"

extern sdfGrOt_t sdfGrOtTable[];

extern s8 dds3PadButtonState[];

extern void *sdfDevPrintf(s32 aX, s32 aY, s32 aZ, s32 aAttr, const char *aStr, ...);

extern void sdfDrawTagLink(sdfDrawTag_t *aTag, u32 aPacket);

extern void *sdfGrAllocPacketBuff(u32 aSize);

extern void sdfDrawTagInit(sdfDrawTag_t *a);

extern dds3Basic_t *dds3GetActiveScene(void);

extern void *dds3GetProcessData(dds3ProcessID_t *id);

extern s32 dds3SceneBasicExistBasic(dds3Basic_t *scene, dds3Basic_t *target);

extern void dds3ConsPanelTagLink(sdfDrawTag_t *tag, s32 x, s32 y, s32 sx, s32 sy);

extern s32 evtSceneDispProcess(void *id);

extern s32 DispBaseData(dds3Basic_t *basic, s32 x, s32 y, sdfDrawTag_t *tag);

extern s32 sdfFloatToULong(f32 value);

extern char D_0054E9A8[];

extern char D_0054E9B0[];

extern char D_0054E9B8[];

/* NON_MATCHING: 170 of 190 words differ, one word short; retail spills the
 * label column and keeps the value column, and this keeps the label column. */

#ifdef NON_MATCHING

s32 DispBaseData(dds3Basic_t *basic, s32 x, s32 y, sdfDrawTag_t *tag)
{
    s32 valueX = ((x + 7) * 3 << 6) + 0x7000;
    s32 labelX = (x * 3 << 6) + 0x7000;
    dds3PoseSet_t *pose;
    void *command;
    char *format;
    s32 v0;
    s32 v1;
    s32 v2;

    command = sdfDevPrintf(labelX, (y * 3 << 5) + 0x7900, 0xFEFFFF, 0, "ID    : 0x%05X", basic->serialID);
    sdfDrawTagLink(tag, (u32)command);
    y++;
    command = sdfDevPrintf(labelX, (y * 3 << 5) + 0x7900, 0xFEFFFF, 0, "NAME  : %s", basic->name);
    sdfDrawTagLink(tag, (u32)command);
    y++;
    pose = basic->poseset;
    if (pose == NULL) {
        return 2;
    }
    y++;
    command = sdfDevPrintf(labelX, (y * 3 << 5) + 0x7900, 0xFEFFFF, 0, D_0054E9A8);
    sdfDrawTagLink(tag, (u32)command);
    format = "%3.2f %3.2f %3.2f";
    v0 = sdfFloatToULong(pose->pos.x);
    v1 = sdfFloatToULong(pose->pos.y);
    v2 = sdfFloatToULong(pose->pos.z);
    command = sdfDevPrintf(valueX, (y * 3 << 5) + 0x7900, 0xFEFFFF, 0xE, format, v0, v1, v2);
    sdfDrawTagLink(tag, (u32)command);
    y += 2;
    command = sdfDevPrintf(labelX, (y * 3 << 5) + 0x7900, 0xFEFFFF, 0, D_0054E9B0);
    sdfDrawTagLink(tag, (u32)command);
    format = "%3.2f %3.2f %3.2f %3.2f";
    v0 = sdfFloatToULong(pose->rot.x);
    v1 = sdfFloatToULong(pose->rot.y);
    v2 = sdfFloatToULong(pose->rot.z);
    command = sdfDevPrintf(valueX, (y * 3 << 5) + 0x7900, 0xFEFFFF, 0xE, format, v0, v1, v2,
                           sdfFloatToULong(pose->rot.w));
    sdfDrawTagLink(tag, (u32)command);
    y += 2;
    command = sdfDevPrintf(labelX, (y * 3 << 5) + 0x7900, 0xFEFFFF, 0, D_0054E9B8);
    sdfDrawTagLink(tag, (u32)command);
    format = "%3.1f %3.1f %3.1f";
    v0 = sdfFloatToULong(pose->scl.x);
    v1 = sdfFloatToULong(pose->scl.y);
    v2 = sdfFloatToULong(pose->scl.z);
    command = sdfDevPrintf(valueX, (y * 3 << 5) + 0x7900, 0xFEFFFF, 0xE, format, v0, v1, v2);
    sdfDrawTagLink(tag, (u32)command);
    return 8;
}

#else

INCLUDE_ASM(const s32, "basic/dds3DbgBasic", DispBaseData);

#endif

u32 DispObjectBaseData(dds3Basic_t *basic, s32 x, s32 y, sdfDrawTag_t *tag) {
    dds3ObjectBaseControlWorkData_t *handle;
    void *command;

    handle = dds3GetBaseControlWorkData(basic);
    command = sdfDevPrintf((x * 3 << 6) + 0x7000,
                                           (y * 3 << 5) + 0x7900,
                                           0xFEFFFF,
                                           0,
                                           "FLAG  : 0x%08X",
                                           handle->flag);
    sdfDrawTagLink(tag, (u32)command);
    return 1;
}

s32 dds3DebugBasic(dds3ProcessID_t *id) {
    s32 width;
    dds3Basic_t *node;
    sdfGrOt_t *surface;
    sdfDrawTag_t *list;
    sdfDrawTag_t *spriteList;

    if (dds3GetActiveScene() == NULL) {
        return -1;
    }
    node = (dds3Basic_t *)dds3GetProcessData(id);
    if (dds3SceneBasicExistBasic(dds3GetActiveScene(), (dds3Basic_t *)node) == 0) {
        return (s32)evtSceneDispProcess;
    }
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    width = DispBaseData(node, 3, 0xA, list) + 0xB;
    if (((u8 *)&node->type)[3] < 0xA) {
        if (((u8 *)&node->type)[3] >= 4) {
            width += DispObjectBaseData(node, 3, width, list);
        }
    }
    spriteList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(spriteList);
    dds3ConsPanelTagLink(spriteList, 0x24, 0x72, 0x23, width - 9);
    surface = &sdfGrOtTable[0x51];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))surface->regFunc)(surface, spriteList);
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))surface->regFunc)(surface, list);
    if (dds3PadButtonState[0x33] < 0) {
        return (s32)evtSceneDispProcess;
    }
    return 0;
}
INCLUDE_SDATA(const s32, "basic/dds3DbgBasic", D_0054E9A8);

INCLUDE_SDATA(const s32, "basic/dds3DbgBasic", D_0054E9B0);

INCLUDE_SDATA(const s32, "basic/dds3DbgBasic", D_0054E9B8);

