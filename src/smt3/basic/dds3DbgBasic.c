#include "common.h"
#include "basic.h"
#include "kernel.h"
#include "newdata.h"
#include "ps2asm.h"

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

extern f64 fptodp(f32 value);


extern char D_0054E9A8[];

extern char D_0054E9B0[];

extern char D_0054E9B8[];

s32 DispBaseData(dds3Basic_t *node, s32 x, s32 y, sdfDrawTag_t *list) {
    dds3PoseSet_t *transform;
    s32 labelX;
    s32 valueX;
    s32 positionY;
    s32 rotationY;
    s32 scaleY;
    s32 lines;

    x *= 12;
    labelX = (x << 4) + 0x7000;
    sdfDrawTagLink(list, (u32)sdfDevPrintf(labelX,
        (y++ * 6 << 4) + 0x7900, 0xFEFFFF, 0, "ID    : 0x%05X", node->serialID));
    sdfDrawTagLink(list, (u32)sdfDevPrintf(labelX,
        (y++ * 6 << 4) + 0x7900, 0xFEFFFF, 0, "NAME  : %s", node->name));
    transform = node->poseset;
    lines = 2;
    if (transform != NULL) {
        y++;
        positionY = (y * 6 << 4) + 0x7900;
        sdfDrawTagLink(list, (u32)sdfDevPrintf(labelX,
            positionY, 0xFEFFFF, 0, D_0054E9A8));
        valueX = ((x + 84) << 4) + 0x7000;
        sdfDrawTagLink(list, (u32)sdfDevPrintf(valueX,
            positionY, 0xFEFFFF, 0xE, "%3.2f %3.2f %3.2f",
            transform->pos.x, transform->pos.y, transform->pos.z));
        y += 2;
        rotationY = (y * 6 << 4) + 0x7900;
        sdfDrawTagLink(list, (u32)sdfDevPrintf(labelX,
            rotationY, 0xFEFFFF, 0, D_0054E9B0));
        sdfDrawTagLink(list, (u32)sdfDevPrintf(valueX,
            rotationY, 0xFEFFFF, 0xE, "%3.2f %3.2f %3.2f %3.2f",
            transform->rot.x, transform->rot.y, transform->rot.z, transform->rot.w));
        y += 2;
        scaleY = (y * 6 << 4) + 0x7900;
        sdfDrawTagLink(list, (u32)sdfDevPrintf(labelX,
            scaleY, 0xFEFFFF, 0, D_0054E9B8));
        sdfDrawTagLink(list, (u32)sdfDevPrintf(valueX,
            scaleY, 0xFEFFFF, 0xE, "%3.1f %3.1f %3.1f",
            transform->scl.x, transform->scl.y, transform->scl.z));
        lines = 8;
    }
    return lines;
}

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

