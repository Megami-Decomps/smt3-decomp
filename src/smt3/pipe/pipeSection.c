#include "fpu.h"
#include "common.h"
#include "ps2asm.h"
#include "basic.h"
#include "motion.h"

extern void (*D_00551828)(void);

extern dds3Basic_t *dds3GetControlScene(void);

extern void *memset(void *a, s32 b, u32 c);

extern void sdfMemFreeDirect(void *aHandle);

typedef struct tagPipeObject {
    s32 offset;
    f32 w;
    f32 pos[2];
    f32 vpos[2];
    u8 id;
    u8 action;
    u8 enabled;
    u8 mdl;
    s16 count;
    u16 flags;
} tagPipeObject;

typedef struct tagPipePosTable tagPipePosTable;

typedef struct tagPipeLinkInfo {
    sdfMemHandle_t *aHandle;
    s32 curNum;
    s32 maxNum;
    s32 nextId;
} tagPipeLinkInfo;

extern u16 myStatus;

extern s32 datGetPlayerParam(s32 id);

extern u8 camMode, CameraAction, offsetStep, offsetStepCount;

extern s32 D_00551738, D_0055173C, D_00551744, D_00551748;

extern void *pipeObjList, *pipeObjTail;

extern void makaInit(void);

extern void partsInit(void);

extern void Initmymdl(void);

extern u8 attackCount;

extern void objInitMyAdd(void);

extern s32 p2sdPlayBGM(u32 MidiNo);

extern void dds3FadeIn(s32 r, s32 g, s32 b, s32 frame);

extern u8 debugMode, pipeInitDone;

extern s32 pipeInitSection(void);

extern void pipeFreeLink(tagPipeLinkInfo *pInfo);

typedef struct tagPipePosTable {
    sdfMemHandle_t *aHandle;
    s32 tSize;
    u8 unk08[8];
} tagPipePosTable_t;

typedef struct tagPipeTable {
    s32 tabMax;
    sdfMemHandle_t *aHandle;
    void *tabPtr;
    void *data;
} tagPipeTable_t;

extern tagPipeLinkInfo linkHeader00;

extern tagPipePosTable_t D_00528F58;

extern tagPipeTable_t D_00528F68;

extern s32 offsetCount;

extern void EmSound(tagPipeObject *objPtr);

extern void MySound(tagPipeObject *objPtr);


extern u32 dds3BackColor;

extern void dds3DeleteScene(dds3Basic_t *scene);

extern void workRelease(void);

extern void pipeEndSection(void);

extern void pipeDataRelease(void);

extern void pipeEffRelease(void);

extern s32 PipeTexRelease(void);


extern void DEBUG_PRINT(const char *str, ...);

extern void eglSetOt(s32 ot);

extern void eglLinkAlpha(s32 mode);

extern void eglLinkTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 zte, s32 ztst);

extern void ObjectDraw(void);

extern void pipeBulrLimit(void);

extern tagPipeObject *ObjWork;

extern s32 ZmakaCount;




extern s32 playerDAMAGE;

extern s32 myMaka;

extern s32 pipeMyMakaCount;

extern s8 rBoxNum;

extern s8 ObjectSound(void);

extern void p2sdPlaySE(s32 midino, s32 vol, s32 pan);

extern s32 p2sdStopBGM(s32 midino);

extern tagPipeObject *objMyData(void);

extern void objDel(tagPipeObject *objPtr);

extern void objDelAll(void);

extern void objVisibleAll(s8 value);

extern s32 pipeMainSection(void);

extern void pipeDrawSection(void);

extern s32 pipeSoundSection(void);

extern void CalcCamera(void);

s32 pipe_Section(void)
{
    s32 ret = pipeMainSection();

    pipeDrawSection();
    pipeSoundSection();
    CalcCamera();
    return ret;
}

INCLUDE_RODATA(const s32, "pipe/pipeSection", D_00548910);

INCLUDE_ASM(const s32, "pipe/pipeSection", pipeInitSection);

void pipeEndSection(void)
{
    dds3Basic_t *scene = dds3GetControlScene();

    dds3DeleteScene(scene);
    pipeEffRelease();
    workRelease();
    PipeTexRelease();
    pipeDataRelease();
    dds3BackColor = 0x80000000;
    DEBUG_PRINT("Release OK ....\n");
    scene = NULL;
}

void pipeDataRelease(void)
{
    if (linkHeader00.aHandle != NULL) {
        pipeFreeLink(&linkHeader00);
        linkHeader00.aHandle = NULL;
    }
    if (D_00528F58.aHandle != NULL) {
        sdfMemFreeDirect(D_00528F58.aHandle);
        D_00528F58.aHandle = NULL;
    }
    if (D_00528F68.aHandle != NULL) {
        sdfMemFreeDirect(D_00528F68.aHandle);
        D_00528F68.aHandle = NULL;
    }
}

INCLUDE_ASM(const s32, "pipe/pipeSection", pipeLoadSection);

INCLUDE_ASM(const s32, "pipe/pipeSection", pipeMainSection);

void pipeDrawSection(void)
{
    s32 ot = 0x27;

    eglSetOt(ot);
    eglLinkAlpha(0);
    eglLinkTest(1, 5, 0x80, 1, 0, 0, 1, 3);
    ObjectDraw();
    pipeBulrLimit();
    ot = 0;
}

s32 pipeSoundSection(void)
{
    return ObjectSound();
}

INCLUDE_ASM(const s32, "pipe/pipeSection", pipeOffset);

void pipeStart(void)
{
    s32 offset = 10;

    camMode = pipeInitDone;
    offsetCount = offset;
    CameraAction = 0;
    D_00551738 = 0;
    D_0055173C = 0;
    offsetStep = 0;
    offsetStepCount = 0;
    ZmakaCount = 0;
    playerDAMAGE = 0;
    rBoxNum = 0;
    D_00551748 = 0;
    D_00551744 = 0;
    attackCount = 0;
    pipeObjList = NULL;
    pipeObjTail = NULL;
    pipeMyMakaCount = 0;
    myMaka = 0;
    makaInit();
    objInitMyAdd();
    partsInit();
    Initmymdl();
    p2sdPlayBGM(0x670001);
    dds3FadeIn(0, 0, 0, 0x3C);
    memset(D_00528F68.data, 0, D_00528F68.tabMax);
    offset = 0;
}

void pipeEnd(void)
{
    tagPipeObject *objPtr;

    p2sdStopBGM(0x670001);
    objPtr = objMyData();
    objDel(objPtr);
    objDelAll();
    objVisibleAll(0);
    objPtr = NULL;
}

INCLUDE_ASM(const s32, "pipe/pipeSection", PipeMain);

INCLUDE_ASM(const s32, "pipe/pipeSection", pipeLoadObjects);

INCLUDE_ASM(const s32, "pipe/pipeSection", MyTask);

INCLUDE_ASM(const s32, "pipe/pipeSection", MyAction);

INCLUDE_ASM(const s32, "pipe/pipeSection", MyHitAction);

INCLUDE_ASM(const s32, "pipe/pipeSection", EmTask);

INCLUDE_ASM(const s32, "pipe/pipeSection", ActionRot);

INCLUDE_ASM(const s32, "pipe/pipeSection", ActionHm);

INCLUDE_ASM(const s32, "pipe/pipeSection", ActionSlide);

INCLUDE_ASM(const s32, "pipe/pipeSection", ActionCrush);

INCLUDE_ASM(const s32, "pipe/pipeSection", ActionBound);

INCLUDE_ASM(const s32, "pipe/pipeSection", ActionAbsorp);

extern void pipeEffSet(tagPipeObject *object, s32 type);
extern u8 objMake(tagPipeObject *object, s32 offset, u8 id, u8 way, u8 action, u8 value);
extern s32 objAdd(tagPipeObject *object);
extern void objMdlAlphaSet(tagPipeObject *object, u8 value);

void ActionDeath(tagPipeObject *object)
{
    object->flags &= 0xEFFF;
    if (object->count == 0) {
        if ((object->flags & 0x2000) == 0) {
            pipeEffSet(object, 4);
            object->flags |= 0x3000;
            object->count = 0x80;
            if (object->flags & 0x4000) {
                tagPipeObject effect;

                objMake(&effect, object->offset + 0x28, 9, 0, 0x15, 0);
                effect.pos[0] = object->pos[0];
                effect.pos[1] = object->pos[1];
                effect.count = 0xA0;
                objAdd(&effect);
            }
        }
    } else {
        object->count -= 0x20;
    }
    object->offset += 6;
    objMdlAlphaSet(object, (u8)object->count);
}

INCLUDE_ASM(const s32, "pipe/pipeSection", ActionZMotion);

INCLUDE_ASM(const s32, "pipe/pipeSection", ActionKey);

INCLUDE_ASM(const s32, "pipe/pipeSection", HITCHECK_CUR);

INCLUDE_ASM(const s32, "pipe/pipeSection", ObjectDraw);

void MyDraw(void) {
}

s8 ObjectSound(void)
{
    tagPipeObject *objPtr = ObjWork;
    s32 i;

    for (i = 149; i != -1; i--) {
        s32 kind = (u8)objPtr->id & 0xF0;

        if (kind != 0) {
            if (kind == 0x80) {
                MySound(objPtr);
            }
        } else {
            EmSound(objPtr);
        }
        objPtr++;
    }
}

void MySound(tagPipeObject *objPtr)
{
    if (objPtr->flags & 1) {
        p2sdPlaySE(0x670004, 0x7F, 0x3F);
    }
    if (objPtr->flags & 2) {
        p2sdPlaySE(0x670005, 0x7F, 0x3F);
    }
    if (objPtr->flags & 4) {
        p2sdPlaySE(0x670006, 0x7F, 0x3F);
    }
    if (objPtr->flags & 8) {
        p2sdPlaySE(0x670002, 0x7F, 0x3F);
    }
    if (objPtr->flags & 0x10) {
        p2sdPlaySE(0x670009, 0x7F, 0x3F);
    }
    if (objPtr->flags & 0x40) {
        p2sdPlaySE(0x09530000, 0x7F, 0x3F);
    }
    objPtr->flags &= 0xFFA0;
}

void EmSound(tagPipeObject *objPtr)
{
    if ((objPtr->flags & 0x1000) != 0) {
        p2sdPlaySE(0x670008, 0x7F, 0x3F);
        objPtr = NULL;
    }
}

s32 GetMaka(void)
{
    return myMaka;
}

s32 GetRBoxNum(void)
{
    return rBoxNum;
}

s32 GetDamage(void)
{
    return playerDAMAGE;
}

void PlayerStatusSetUp(void)
{
    s32 param;

    myStatus = 0;
    param = datGetPlayerParam(0);
    if (param >= 0xF) {
        if (param < 0x19) {
            myStatus |= 1;
        } else {
            myStatus |= 2;
        }
    }
    param = datGetPlayerParam(4);
    if (param >= 0) {
        if (param < 0xF) {
            myStatus |= 0x10;
        } else {
            myStatus |= 0x20;
        }
    }
    param = datGetPlayerParam(5);
    if (param >= 0xA) {
        if (param < 0x14) {
            myStatus |= 0x100;
        } else {
            myStatus |= 0x200;
        }
    }
}
