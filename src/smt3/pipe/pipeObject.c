#include "fpu.h"
#include "common.h"
#include "ps2asm.h"
#include "basic.h"
#include "motion.h"

struct pipeTextureFile_t;

extern void (*D_00551828)(void);

extern sdfMemHandle_t *sdfDevLoadFileEx(const char *name, u32 *data, s32 mode);

extern sdfTexHandle_t *sdfTexSet(struct pipeTextureFile_t *aTex);

extern void *memset(void *a, s32 b, u32 c);

typedef struct tagpipeBasic {
    void *base;
    mdlObj_t *mdl;
    u8 unk08[4];
    s32 enabled;
    f32 pos[4];
    f32 rot[4];
    f32 scl[4];
} tagpipeBasic;

typedef struct tagPipeObject {
    s32 offset;
    f32 w;
    f32 pos[2];
    f32 vpos[2];
    s8 id;
    u8 action;
    u8 enabled;
    u8 mdl;
    s16 count;
    u16 flags;
} tagPipeObject;

typedef struct tagPipeTexture {
    sdfTexHandle_t *th;
    sdfMemHandle_t *mh;
    struct pipeTextureFile_t *mp;
    u32 unk0C;
} tagPipeTexture;

typedef struct tagPipeEnemyInfo {
    u8 chID;
    f32 w;
    u8 resv;
    u16 resv2;
} tagPipeEnemyInfo;

typedef struct tagpipeParts {
    s32 offset;
    u8 id;
    u8 unk05;
    u8 mdl;
    s8 resv[5];
} tagpipeParts;

extern tagpipeParts *PartsWork;



extern void partsInit(void);

extern void Initmymdl(void);

extern void objInitMyAdd(void);

extern s32 texLoad(const char *filename, tagPipeTexture *texPtr);

extern void vecRotate2D(f32 *vector, f32 angle);

extern s32 offsetCount;

extern tagPipeEnemyInfo *objInfo(s32 id);

extern void objMdlMotion(tagPipeObject *objPtr, s32 motion, f32 speed);

extern tagpipeBasic *basicMy;

extern tagpipeBasic *PipeLine, *PipeCur, *PipeCur2, *PipeRBOX, *PipeEMS, *PipeEML;

extern tagpipeBasic *PipeMaka1, *PipeZMaka, *PipeDMaka, *PipeDRBOX;

extern void mymdlDelObject(tagpipeBasic *pWork);

extern void mymdlVisibleAll(tagpipeBasic *pWork, s16 mdlSize, s8 value);

extern void mymdlInitObject(tagpipeBasic *pWork, u16 mdlSize);

extern u8 objMake(tagPipeObject *objPtr, s32 offset, u8 id, u8 way, u8 action, u8 value);

extern void workRelease(void);

extern s32 PipeTexRelease(void);

extern void mdlSmoothLoopMotion(void *aHandle, s32 aGroup, s32 aNumber, f32 aBeforeLeng, f32 aSmoothLeng);

extern void DEBUG_PRINT(const char *str, ...);

extern tagPipeEnemyInfo D_00528F78[];

extern tagPipeObject *ObjWork;

extern tagPipeTexture *MakaTex;

extern void *hWorkArea;

extern s32 myMaka;

extern s32 pipeMyMakaCount;

extern void sdfMemFreeDelayedDirect(void *aHandle);

extern void objMdlDel(tagPipeObject *objPtr);

extern void objInit(void);

extern void objMyLoad(tagPipeObject *objPtr);

extern s32 objAdd(tagPipeObject *objPtr);

extern tagPipeObject *objMyData(void);

extern void objDel(tagPipeObject *objPtr);

extern void objDelAll(void);

extern void objVisibleAll(s8 value);

extern void sdfTexFreeDelayed(void *aHandle);

void workRelease(void)
{
    if (hWorkArea != NULL) {
        sdfMemFreeDelayedDirect(hWorkArea);
        hWorkArea = NULL;
    }
}

INCLUDE_ASM(const s32, "pipe/pipeObject", mymdlStartUp);

void Initmymdl(void)
{
    s32 mdlSize = 10;

    mymdlInitObject(PipeLine, mdlSize);
    mymdlInitObject(PipeCur, 10);
    mymdlInitObject(PipeCur2, 10);
    mymdlInitObject(PipeRBOX, 10);
    mymdlInitObject(PipeEMS, 50);
    mymdlInitObject(PipeEML, 50);
    mymdlInitObject(PipeMaka1, 50);
    mymdlInitObject(PipeZMaka, 2);
    mymdlInitObject(PipeDMaka, 4);
    mymdlInitObject(PipeDRBOX, 2);
    mdlSize = 0;
}

typedef struct tagPipePathNode {
    f32 position[3]; u32 opaque0C; f32 direction[3]; u32 opaque1C;
} tagPipePathNode;
typedef struct tagPipePosTable {
    sdfMemHandle_t *aHandle; s32 tSize; tagPipePathNode *points; u32 opaque0C;
} tagPipePosTable;
extern tagPipePosTable D_00528F58;
extern void vecTranslate(f32 *, f32 *, f32 *);
extern f32 vecNormalize(f32 *);
extern void mtxRotateX(sdfMatrix4_t *, f32);
extern void mtxRotateY(sdfMatrix4_t *, f32);
extern void mtxRotateZ(sdfMatrix4_t *, f32);
extern void mtxStore(sdfMatrix4_t *, sdfMatrix4_t *);
extern void vecDistance(f32 [4], f32 [4][4]);
extern void dds3SetRotPoseSet(void *, sdfVector4_t *);

extern void vecSet(f32 *, f32, f32, f32);
extern void vecScale(f32 *, f32);

void getPosTranslate(tagPipeObject *object, f32 *output)
{
    f32 tangent[4];
    f32 side[4];
    f32 basis[4];

    vecSet(tangent,
           D_00528F58.points[object->offset + 1].position[0] - D_00528F58.points[object->offset].position[0],
           D_00528F58.points[object->offset + 1].position[1] - D_00528F58.points[object->offset].position[1],
           D_00528F58.points[object->offset + 1].position[2] - D_00528F58.points[object->offset].position[2]);
    vecSet(basis, D_00528F58.points[object->offset].direction[0],
           D_00528F58.points[object->offset].direction[1], D_00528F58.points[object->offset].direction[2]);
    vecTranslate(side, tangent, basis);
    vecNormalize(side);
    vecScale(side, object->pos[0]);
    vecScale(basis, object->pos[1]);
    output[0] = D_00528F58.points[object->offset].position[0] - side[0] + basis[0];
    output[1] = D_00528F58.points[object->offset].position[1] - side[1] + basis[1];
    output[2] = D_00528F58.points[object->offset].position[2] - side[2] + basis[2];
}


void objMyLoad(tagPipeObject *objPtr)
{
    tagPipeEnemyInfo *pInfo;

    memset(objPtr, 0, 0x20);
    objPtr->id = -0x80;
    objPtr->pos[0] = 0.0f;
    objPtr->offset = offsetCount;
    objPtr->enabled = 1;
    objPtr->pos[1] = 0.0f;
    objPtr->vpos[0] = 0.0f;
    objPtr->vpos[1] = 0.0f;
    objPtr->count = 0;
    objPtr->action = 0;
    pInfo = objInfo(0x80);
    if (pInfo != NULL) {
        objPtr->w = pInfo->w;
        objMdlMotion(objPtr, 1, 1.0f);
        pInfo = NULL;
    }
}

INCLUDE_ASM(const s32, "pipe/pipeObject", objLoad);

INCLUDE_ASM(const s32, "pipe/pipeObject", objMake);

INCLUDE_ASM(const s32, "pipe/pipeObject", objOption);

void objOptionInc(tagPipeObject *objPtr, u8 op)
{
    f32 pos[3];

    if (op < 0x14) {
        objPtr->offset += 0x1E;
        return;
    }
    if (op >= 0x64) {
        return;
    }
    objPtr->offset += 0x1E;
    pos[0] = objPtr->pos[0];
    pos[1] = objPtr->pos[1];
    pos[2] = 0.0f;
    if (op < 0x28) {
        vecRotate2D(pos, 0.17453293f);
    } else if (op < 0x3C) {
        vecRotate2D(pos, -0.17453293f);
    } else if (op < 0x50) {
        vecRotate2D(pos, 0.087266461f);
    } else {
        vecRotate2D(pos, -0.087266461f);
    }
    objPtr->pos[0] = pos[0];
    objPtr->pos[1] = pos[1];
}

void objSetAction(tagPipeObject *objPtr, u8 action, s16 count)
{
    objPtr->action = action;
    objPtr->count = count;
}

void objInitMyAdd(void)
{
    tagPipeObject obj;

    objInit();
    objMyLoad(&obj);
    objAdd(&obj);
}

extern u32 dds3GetRandA(void *state);

void objCopy(s32 offset, f32 x, f32 y, u8 id, u8 action, s16 count, s16 num) {
    tagPipeObject object;
    s16 remaining = num;

    memset(&object, 0, 0x20);
    objMake(&object, offset, id, 0, action, 0);

    while (remaining != 0) {
        u32 randomValue;

        randomValue = dds3GetRandA(NULL);
        object.pos[0] = x + (f32)(randomValue % 400) - 200.0f;
        randomValue = dds3GetRandA(NULL);
        object.pos[1] = y + (f32)(randomValue % 400) - 200.0f;
        randomValue = dds3GetRandA(NULL);
        object.offset = offset + randomValue % 20;
        randomValue = dds3GetRandA(NULL);
        object.vpos[0] = (f32)(randomValue % 10) - 5.0f;
        randomValue = dds3GetRandA(NULL);
        object.vpos[1] = (f32)(randomValue % 10) - 5.0f;
        object.count = count;
        objAdd(&object);
        remaining--;
    }
}

void objCopy2(s32 offset, f32 x, f32 y, u8 id, u8 action, s16 count, s16 num, s32 range) {
    tagPipeObject object;
    memset(&object, 0, 0x20);
    objMake(&object, offset, id, 0, action, 0);

    if (num != 0) {
        f32 rangeAsFloat = (f32)range;
        s32 positionSpan = range * 2;

        do {
            u32 randomValue;

            randomValue = dds3GetRandA(NULL);
            object.pos[0] = x + (f32)(randomValue % positionSpan) - rangeAsFloat;
            randomValue = dds3GetRandA(NULL);
            object.pos[1] = y + (f32)(randomValue % positionSpan) - rangeAsFloat;
            randomValue = dds3GetRandA(NULL);
            object.offset = offset + randomValue % (range / 20);
            randomValue = dds3GetRandA(NULL);
            object.vpos[0] = (f32)(randomValue % 10) - 5.0f;
            randomValue = dds3GetRandA(NULL);
            object.vpos[1] = (f32)(randomValue % 10) - 5.0f;
            object.count = count;
            objAdd(&object);
            num--;
        } while (num != 0);
    }
}

void objAddDMaka(s32 offset, f32 x, f32 y)
{
    tagPipeObject obj;

    objMake(&obj, offset, 0xB, 0, 0, 0);
    obj.pos[0] = x;
    obj.pos[1] = y;
    obj.count = 0x1D;
    objAdd(&obj);
}

void objAddDRBOX(s32 offset, f32 x, f32 y)
{
    tagPipeObject obj;

    objMake(&obj, offset, 0xC, 0, 0, 0);
    obj.pos[0] = x;
    obj.pos[1] = y;
    obj.count = 0x1D;
    objAdd(&obj);
}

void objInit(void)
{
    tagPipeObject *objPtr = ObjWork;
    s32 i;

    memset(objPtr, 0, 0x12C0);
    for (i = 149; i != -1; i--) {
        objPtr->id = -1;
        objPtr++;
    }
}

extern s32 pipeObjList;
extern s32 pipeObjTail;
extern void objMdlAdd(tagPipeObject *object);

s32 objAdd(tagPipeObject *object)
{
    tagPipeObject *slot = ObjWork;
    s32 remaining = 150;

    if ((u8)object->id == 5) {
        if (object->action == 0x1A) {
            pipeObjList += 5;
        } else {
            pipeObjList++;
        }
    }
    if ((object->flags & 0x4000) != 0) {
        pipeObjTail++;
    }
    while (--remaining != -1) {
        if ((u8)slot->id == 0xFF) {
            objMdlAdd(object);
            if (object->mdl == 0xFF) {
                break;
            }
            *slot = *object;
            return 1;
        }
        slot++;
    }
    return 0;
}

void objDel(tagPipeObject *objPtr)
{
    objMdlDel(objPtr);
    objPtr->id = -1;
}

void objDelAll(void)
{
    tagPipeObject *objPtr = ObjWork;
    s32 i;

    for (i = 149; i != -1; i--) {
        if ((u8)objPtr->id != 0xFF) {
            objDel(objPtr);
        }
        objPtr++;
    }
}

tagPipeEnemyInfo *objInfo(s32 id)
{
    tagPipeEnemyInfo *pInfo = D_00528F78;
    s32 i;

    for (i = 10; i != -1; i--) {
        if (pInfo->chID == id) {
            return pInfo;
        }
        pInfo++;
    }
    return NULL;
}

tagPipeObject *objMyData(void)
{
    tagPipeObject *objPtr = ObjWork;
    s32 i;

    for (i = 149; i != -1; i--) {
        if ((u8)objPtr->id == 0x80) {
            return objPtr;
        }
        objPtr++;
    }
    return NULL;
}

extern s32 mymdlAddObject(tagpipeBasic *work, u16 count);
extern void mymdlSetMotion(tagpipeBasic *work, f32 frame);
extern void mymdlAlphaSet(tagpipeBasic *work, u8 alpha);
extern tagpipeBasic *MyLine;

INCLUDE_RODATA(const s32, "pipe/pipeObject", D_00548B50);

const char D_00548B60[24] = "EML %d ADD OBJECT\n";
const char D_00548B78[24] = "EMS %d ADD OBJECT\n";
const char D_00548B90[24] = "DRBOX %d ADD OBJECT\n";
const char D_00548BA8[24] = "DMaka %d ADD OBJECT\n";
const char D_00548BC0[24] = "Maka1 %d ADD OBJECT\n";
const char D_00548BD8[24] = "ZMaka %d ADD OBJECT\n";
const char D_00548BF0[24] = "RBOX %d ADD OBJECT\n";
const char D_00548C08[24] = "MY %d ADD OBJECT\n";

void objMdlAdd(tagPipeObject *objPtr)
{
    switch ((u8)objPtr->id) {
    case 1:
        objPtr->mdl = mymdlAddObject(PipeEML, 50);
        return DEBUG_PRINT(D_00548B60, (u8)objPtr->mdl);
    case 5:
    case 6:
        objPtr->mdl = mymdlAddObject(PipeMaka1, 50);
        return DEBUG_PRINT(D_00548BC0, (u8)objPtr->mdl);
    case 2:
        objPtr->mdl = mymdlAddObject(PipeEMS, 50);
        return DEBUG_PRINT(D_00548B78, (u8)objPtr->mdl);
    case 12:
        objPtr->mdl = mymdlAddObject(PipeDRBOX, 2);
        if ((u8)objPtr->mdl != 0xFF) {
            mymdlSetMotion(&PipeDRBOX[(u8)objPtr->mdl], 0.0f);
        }
        return DEBUG_PRINT(D_00548B90, (u8)objPtr->mdl);
    case 11:
        objPtr->mdl = mymdlAddObject(PipeDMaka, 4);
        if ((u8)objPtr->mdl != 0xFF) {
            mymdlSetMotion(&PipeDMaka[(u8)objPtr->mdl], 0.0f);
        }
        return DEBUG_PRINT(D_00548BA8, (u8)objPtr->mdl);
    case 9:
        objPtr->mdl = mymdlAddObject(PipeZMaka, 2);
        return DEBUG_PRINT(D_00548BD8, (u8)objPtr->mdl);
    case 10:
        objPtr->mdl = mymdlAddObject(PipeRBOX, 10);
        return DEBUG_PRINT(D_00548BF0, (u8)objPtr->mdl);
    case 128:
        objPtr->mdl = mymdlAddObject(basicMy, 1);
        mymdlAddObject(MyLine, 1);
        mymdlAlphaSet(MyLine, 0);
        DEBUG_PRINT(D_00548C08, (u8)objPtr->mdl);
        break;
    }
}

extern void objMdlSclSet(tagPipeObject *object, f32 scale);
extern void objMdlAlphaSet(tagPipeObject *object, u8 alpha);

const char D_00548C20[24] = "EML %d DEL OBJECT\n";
const char D_00548C38[24] = "EMS %d DEL OBJECT\n";
const char D_00548C50[24] = "DRBOX %d DEL OBJECT\n";
const char D_00548C68[24] = "DMaka %d DEL OBJECT\n";
const char D_00548C80[24] = "Maka1 %d DEL OBJECT\n";
const char D_00548C98[24] = "ZMaka %d DEL OBJECT\n";
const char D_00548CB0[24] = "RBOX %d DEL OBJECT\n";
const char D_00548CC8[24] = "MY %d DEL OBJECT\n";

extern void objMdlSclSet(tagPipeObject *objPtr, f32 scale);
extern void objMdlAlphaSet(tagPipeObject *objPtr, u8 alpha);
extern tagpipeBasic *MyLine;

void objMdlDel(tagPipeObject *objPtr) {
    if (objPtr->mdl == 0xFF) {
        return;
    }

    objMdlSclSet(objPtr, 1.0f);
    objMdlAlphaSet(objPtr, 0x80);

    switch ((u8)objPtr->id) {
    case 1:
        mymdlDelObject(&PipeEML[objPtr->mdl]);
        return DEBUG_PRINT(D_00548C20, objPtr->mdl);
    case 2:
        mymdlDelObject(&PipeEMS[objPtr->mdl]);
        return DEBUG_PRINT(D_00548C38, objPtr->mdl);
    case 12:
        mymdlDelObject(&PipeDRBOX[objPtr->mdl]);
        return DEBUG_PRINT(D_00548C50, objPtr->mdl);
    case 11:
        mymdlDelObject(&PipeDMaka[objPtr->mdl]);
        return DEBUG_PRINT(D_00548C68, objPtr->mdl);
    case 5:
    case 6:
        mymdlDelObject(&PipeMaka1[objPtr->mdl]);
        return DEBUG_PRINT(D_00548C80, objPtr->mdl);
    case 9:
        mymdlDelObject(&PipeZMaka[objPtr->mdl]);
        return DEBUG_PRINT(D_00548C98, objPtr->mdl);
    case 10:
        mymdlDelObject(&PipeRBOX[objPtr->mdl]);
        return DEBUG_PRINT(D_00548CB0, objPtr->mdl);
    case 128:
        mymdlDelObject(MyLine);
        mymdlDelObject(basicMy);
        DEBUG_PRINT(D_00548CC8, objPtr->mdl);
        break;
    default:
        break;
    }
}

extern void getPosTranslate(tagPipeObject *object, f32 *position);
extern void mymdlPosSet(tagpipeBasic *work, f32 x, f32 y, f32 z);
extern void mdlTask(tagPipeObject *object, tagpipeBasic *model, f32 x, f32 y, f32 z);
extern void eglSetOt(u32 ot);
extern void eglLinkTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 zte, s32 ztst);
extern f32 D_00551744, D_00551748;
extern tagpipeBasic *MyLine;

void objMdlPosSet(tagPipeObject *object)
{
    f32 position[3];

    if (object->mdl == 0xFF) {
        return;
    }
    getPosTranslate(object, position);
    switch ((u8)object->id) {
    case 1:
        mymdlPosSet(&PipeEML[object->mdl], position[0], position[1], position[2]);
        break;
    case 2:
        mymdlPosSet(&PipeEMS[object->mdl], position[0], position[1], position[2]);
        break;
    case 5:
    case 6:
        mymdlPosSet(&PipeMaka1[object->mdl], position[0], position[1], position[2]);
        break;
    case 12:
        mdlTask(object, &PipeDRBOX[object->mdl], 90.0f, 180.0f, 0.0f);
        mymdlPosSet(&PipeDRBOX[object->mdl], position[0], position[1], position[2]);
        break;
    case 11:
        mdlTask(object, &PipeDMaka[object->mdl], 90.0f, 180.0f, 0.0f);
        mymdlPosSet(&PipeDMaka[object->mdl], position[0], position[1], position[2]);
        break;
    case 9:
        mymdlPosSet(&PipeZMaka[object->mdl], position[0], position[1], position[2]);
        break;
    case 10:
        mymdlPosSet(&PipeRBOX[object->mdl], position[0], position[1], position[2]);
        break;
    case 128:
        mdlTask(object, basicMy, D_00551748, D_00551744, 0.0f);
        mdlTask(object, MyLine, -90.0f, 0.0f, 0.0f);
        mymdlPosSet(basicMy, position[0], position[1], position[2]);
        mymdlPosSet(MyLine, position[0], position[1], position[2]);
        eglSetOt(0x32);
        eglLinkTest(1, 5, 0x80, 1, 0, 0, 1, 1);
        eglSetOt(0x33);
        eglLinkTest(1, 5, 0x80, 1, 0, 0, 1, 1);
        eglSetOt(0x34);
        eglLinkTest(1, 5, 0x80, 1, 0, 0, 1, 1);
        eglSetOt(0x35);
        eglLinkTest(1, 5, 0x80, 1, 0, 0, 1, 1);
        eglSetOt(0x36);
        eglLinkTest(1, 5, 0x80, 1, 0, 0, 1, 1);
        eglSetOt(0x37);
        eglLinkTest(1, 5, 0x80, 1, 0, 0, 1, 3);
        break;
    default:
        break;
    }
}

extern tagpipeBasic *MyLine;
extern void mymdlSclSet(tagpipeBasic *work, f32 scale);

void objMdlSclSet(tagPipeObject *object, f32 scale) {
    switch ((u8)object->id) {
    case 1:
        return mymdlSclSet(&PipeEML[object->mdl], scale);
    case 2:
        return mymdlSclSet(&PipeEMS[object->mdl], scale);
    case 5:
    case 6:
        return mymdlSclSet(&PipeMaka1[object->mdl], scale);
    case 12:
        return mymdlSclSet(&PipeDRBOX[object->mdl], scale);
    case 11:
        return mymdlSclSet(&PipeDMaka[object->mdl], scale);
    case 9:
        return mymdlSclSet(&PipeZMaka[object->mdl], scale);
    case 10:
        return mymdlSclSet(&PipeRBOX[object->mdl], scale);
    case 0x80:
        mymdlSclSet(basicMy, scale);
        mymdlSclSet(MyLine, scale);
        break;
    default:
        break;
    }
}

extern void mymdlColorSet(tagpipeBasic *work, u32 color);

void objMdlColorSet(tagPipeObject *objPtr, u32 color)
{
    switch ((u8)objPtr->id) {
    case 1:
        return mymdlColorSet(&PipeEML[objPtr->mdl], color);
    case 2:
        return mymdlColorSet(&PipeEMS[objPtr->mdl], color);
    case 5:
    case 6:
        return mymdlColorSet(&PipeMaka1[objPtr->mdl], color);
    case 12:
        return mymdlColorSet(&PipeDRBOX[objPtr->mdl], color);
    case 11:
        return mymdlColorSet(&PipeDMaka[objPtr->mdl], color);
    case 9:
        return mymdlColorSet(&PipeZMaka[objPtr->mdl], color);
    case 10:
        return mymdlColorSet(&PipeRBOX[objPtr->mdl], color);
    case 128:
        mymdlColorSet(basicMy, color);
        mymdlColorSet(MyLine, color);
        break;
    }
}

extern void mymdlAlphaSet(tagpipeBasic *work, u8 alpha);

void objMdlAlphaSet(tagPipeObject *objPtr, u8 alpha) {
    switch ((u8)objPtr->id) {
    case 1:
        return mymdlAlphaSet(&PipeEML[objPtr->mdl], alpha);
    case 2:
        return mymdlAlphaSet(&PipeEMS[objPtr->mdl], alpha);
    case 5:
    case 6:
        return mymdlAlphaSet(&PipeMaka1[objPtr->mdl], alpha);
    case 12:
        return mymdlAlphaSet(&PipeDRBOX[objPtr->mdl], alpha);
    case 11:
        return mymdlAlphaSet(&PipeDMaka[objPtr->mdl], alpha);
    case 9:
        return mymdlAlphaSet(&PipeZMaka[objPtr->mdl], alpha);
    case 10:
        return mymdlAlphaSet(&PipeRBOX[objPtr->mdl], alpha);
    case 128:
        return mymdlAlphaSet(basicMy, alpha);
    case 129:
        mymdlAlphaSet(MyLine, alpha);
        break;
    default:
        break;
    }
}

void objMdlMotion(tagPipeObject *objPtr, s32 motion, f32 speed)
{
    s32 id = (u8)objPtr->id;

    if (id < 7) {
        return;
    }
    if (id < 9) {
        return;
    }
    if (id < 13) {
        return;
    }
    if (id != 0x80) {
        return;
    }
    basicMy->mdl->first->speed = speed;
    mdlSmoothLoopMotion(basicMy->mdl, 0, motion, 2.0f, 2.0f);
    objPtr = NULL;
}

void pipeMyMakaAdd(s32 addMaka)
{
    myMaka += addMaka;
    pipeMyMakaCount += addMaka;
}

void pipeMyMakaSub(s32 subMaka)
{
    myMaka -= subMaka;
}

void pipeMyMakaSubRate(f32 subMakaRate)
{
    myMaka = myMaka - myMaka * subMakaRate;
}

extern u8 attackCount;
extern u8 CameraAction;
extern u16 myStatus;
extern void pipeEffSet(tagPipeObject *object, s32 type);
extern void pipeBulrSetLimit(s32 frame);

const char D_00548CE0[32] = "player-motion NORMAL_MOTION\n";
const char D_00548D00[32] = "player-motion DAMAGE_MOTION\n";
const char D_00548D20[32] = "player-motion ATTACK_MOTION\n";

void objMyStatusChange(tagPipeObject *object, u8 status)
{
    if (object == NULL) {
        object = objMyData();
    }
    switch (status) {
    case 0:
        objMdlMotion(object, 1, 1.0f);
        object->action = 0;
        object->count = 0;
        object->flags &= 0xF8FF;
        attackCount = 0;
        return DEBUG_PRINT(D_00548CE0);
    case 2: {
        CameraAction = 6;
        objMdlMotion(object, 3, 1.0f);
        object->action = status;
        object->flags |= 0x208;
        object->count = 30;
        object->flags &= 0xFEFF;
        pipeEffSet(object, 2);
        attackCount = 0;
        pipeBulrSetLimit(10);
        return DEBUG_PRINT(D_00548D00);
    }
    case 1:
        if ((myStatus & 0xF) == 0) {
            objMdlMotion(object, 2, 1.0f);
        } else {
            objMdlMotion(object, 4, 1.0f);
        }
        object->action = 1;
        object->count = 27;
        object->flags |= 0x240;
        return DEBUG_PRINT(D_00548D20);
    case 10:
        pipeEffSet(object, 4);
        break;
    }
}


void objVisibleAll(s8 value)
{
    mymdlVisibleAll(PipeMaka1, 50, value);
    mymdlVisibleAll(PipeZMaka, 2, value);
    mymdlVisibleAll(PipeCur, 10, value);
    mymdlVisibleAll(PipeCur2, 10, value);
    mymdlVisibleAll(PipeLine, 10, value);
    mymdlVisibleAll(PipeEMS, 50, value);
    mymdlVisibleAll(PipeEML, 50, value);
    mymdlVisibleAll(PipeRBOX, 10, value);
    value = 0;
}

void mdlTask(tagPipeObject *objPtr, tagpipeBasic *basicPtr, f32 x, f32 y, f32 z) {
    f32 cross[4];
    f32 direction[4];
    f32 position[4];
    f32 translated[4];
    sdfMatrix4_t matrix;
    f32 radians;
    tagPipePathNode *node;
    tagPipePathNode *nextNode;

    getPosTranslate(objPtr, translated);
    /* Preserve the index-first EE address sum; see
     * docs/levers/addressing-and-types.md. */
    node = (tagPipePathNode *)((u32)offsetCount * sizeof(tagPipePathNode) + (u32)D_00528F58.points);
    nextNode = node + 1;
    position[0] = node->direction[0];
    radians = 0x1.1df46ap-6f;
    position[1] = node->direction[1];
    position[2] = node->direction[2];
    direction[0] = nextNode->position[0] - node->position[0];
    direction[1] = nextNode->position[1] - node->position[1];
    direction[2] = nextNode->position[2] - node->position[2];
    vecTranslate(cross, direction, position);
    vecNormalize(cross);
    vecNormalize(direction);
    vecNormalize(position);
    memset(&matrix, 0, 0x40);
    matrix.m[0] = cross[0]; matrix.m[1] = cross[1]; matrix.m[2] = cross[2];
    matrix.m[4] = direction[0]; matrix.m[5] = direction[1]; matrix.m[6] = direction[2];
    matrix.m[8] = position[0]; matrix.m[9] = position[1]; matrix.m[10] = position[2];
    matrix.m[15] = 1.0f;
    mtxRotateX(&matrix, x * radians);
    mtxRotateY(&matrix, y * radians);
    mtxRotateZ(&matrix, z * radians);
    mtxStore(&matrix, &matrix);
    vecDistance(basicPtr->rot, (f32 (*)[4])matrix.m);
    dds3SetRotPoseSet(basicPtr->base, (sdfVector4_t *)basicPtr->rot);
}


void partsInit(void)
{
    tagpipeParts *pParts = PartsWork;
    s32 i;

    memset(pParts, 0, 0xF0);
    for (i = 19; i != -1; i--) {
        pParts->id = -1;
        pParts++;
    }
}

INCLUDE_ASM(const s32, "pipe/pipeObject", partsAdd);

void partsDel(tagpipeParts *pPtr)
{
    if ((u8)pPtr->mdl == 0xFF) {
        return;
    }
    switch (pPtr->id) {
    case 1:
    case 3:
        mymdlDelObject(&PipeLine[pPtr->mdl]);
        DEBUG_PRINT("-----PipeLine %d DEL OBJECT\n", pPtr->mdl);
        break;
    case 2:
        mymdlDelObject(&PipeCur[pPtr->mdl]);
        DEBUG_PRINT("-----PipeCur %d DEL OBJECT\n", pPtr->mdl);
        break;
    case 4:
        mymdlDelObject(&PipeCur2[pPtr->mdl]);
        DEBUG_PRINT("-----PipeCur2 %d DEL OBJECT\n", pPtr->mdl);
        break;
    }
    pPtr->id = -1;
}




INCLUDE_ASM(const s32, "pipe/pipeObject", PipemdlRot);

s32 PipeTexLoad(void)
{
    memset(MakaTex, 0, 0x40);
    texLoad("/pipe/kek_no01.tmx", &MakaTex[0]);
    texLoad("/pipe/kek_no02.tmx", &MakaTex[1]);
    texLoad("/pipe/kek_no03.tmx", &MakaTex[2]);
    texLoad("/pipe/kek_no04.tmx", &MakaTex[3]);
    return 1;
}

s32 PipeTexRelease(void)
{
    s32 i;

    for (i = 0; i < 4; i++) {
        texRelease(&MakaTex[i]);
    }
    return 1;
}

s32 texLoad(const char *filename, tagPipeTexture *texPtr) {
    void *allocation = sdfDevLoadFileEx(filename, (u32 *)&texPtr->mp, 0);
    texPtr->mh = allocation;
    texPtr->th = sdfTexSet(texPtr->mp);
    if (texPtr->mh != NULL) {
        sdfMemFreeDelayedDirect(texPtr->mh);
        texPtr->mh = NULL;
        texPtr->mp = NULL;
    }
    return 1;
}

s32 texRelease(tagPipeTexture *texPtr)
{
    if (texPtr->th != NULL) {
        sdfTexFreeDelayed(texPtr->th);
        texPtr->th = NULL;
    }
    return 1;
}

INCLUDE_SDATA(const s32, "pipe/pipeObject", hWorkArea);

INCLUDE_SDATA(const s32, "pipe/pipeObject", WorkAreaPtr);

INCLUDE_SDATA(const s32, "pipe/pipeObject", PartsWork);

INCLUDE_SDATA(const s32, "pipe/pipeObject", PipeCur);

INCLUDE_SDATA(const s32, "pipe/pipeObject", PipeCur2);

INCLUDE_SDATA(const s32, "pipe/pipeObject", PipeLine);

INCLUDE_SDATA(const s32, "pipe/pipeObject", ObjWork);

INCLUDE_SDATA(const s32, "pipe/pipeObject", basicMy);

INCLUDE_SDATA(const s32, "pipe/pipeObject", MyLine);

INCLUDE_SDATA(const s32, "pipe/pipeObject", PipeEMS);

INCLUDE_SDATA(const s32, "pipe/pipeObject", PipeEML);

INCLUDE_SDATA(const s32, "pipe/pipeObject", PipeRBOX);

INCLUDE_SDATA(const s32, "pipe/pipeObject", PipeMaka1);

INCLUDE_SDATA(const s32, "pipe/pipeObject", PipeZMaka);

INCLUDE_SDATA(const s32, "pipe/pipeObject", PipeDMaka);

INCLUDE_SDATA(const s32, "pipe/pipeObject", PipeDRBOX);

INCLUDE_SDATA(const s32, "pipe/pipeObject", MakaTex);

INCLUDE_SDATA(const s32, "pipe/pipeObject", makaWork);

INCLUDE_SDATA(const s32, "pipe/pipeObject", pipe_CELL01_D3P);

INCLUDE_SDATA(const s32, "pipe/pipeObject", pipe_BREAK01_D3P);

INCLUDE_SDATA(const s32, "pipe/pipeObject", pipe_BREAK02_D3P);

INCLUDE_SDATA(const s32, "pipe/pipeObject", pipe_DAMAGE01_D3P);

INCLUDE_SDATA(const s32, "pipe/pipeObject", pipe_DAMAGE03_D3P);

INCLUDE_SDATA(const s32, "pipe/pipeObject", pipe_ATTACK01_D3P);

INCLUDE_SDATA(const s32, "pipe/pipeObject", D_005517DC);

