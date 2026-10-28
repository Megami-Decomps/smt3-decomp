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
    s8 enabled;
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

extern s8 camMode, CameraAction;
extern u8 offsetStep, offsetStepCount;

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

extern void pipeInitSection(void);

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

extern const sdfVector4_t D_00548910;
INCLUDE_RODATA(const s32, "pipe/pipeSection", D_00548910);

const char D_00548920[16] = "pipeArea";
extern s32 pipeIndex;
extern dds3Basic_t *dds3AddScene(void);
extern void dds3SetControlScene(dds3Basic_t *);
extern void dds3SetActiveScene(dds3Basic_t *);
extern dds3Basic_t *dds3AddAreaBasic(u32, char *);
extern void AddCamera(void);
extern void mymdlStartUp(void);
extern s32 PipeTexLoad(void);
extern void PlayerStatusSetUp(void);
extern void pipeEffLoad(void);
extern void dds3SetLightColor(s32, s32, f32 *);
extern void dds3SetLightDir(s32, s32, f32 *);
extern void dds3SetAmbientColor(s32, f32 *);
extern void dds3SetFogColor(s32, f32 *);
extern void dds3SetFogPara(s32, f32, f32, f32, f32);

void pipeInitSection(void)
{
    sdfVector4_t color = D_00548910;
    sdfVector4_t direction;
    dds3Basic_t *scene;

    memset(&direction, 0, sizeof(direction));
    direction.w = 1.0f;
    dds3BackColor = 0x80000000;
    scene = dds3AddScene();
    dds3SetControlScene(scene);
    dds3SetActiveScene(scene);
    AddCamera();
    dds3AddAreaBasic(0, (char *)D_00548920);
    pipeIndex = 0;
    mymdlStartUp();
    PipeTexLoad();
    PlayerStatusSetUp();
    pipeEffLoad();

    color.x = 1.0f;
    color.y = 0x1.666666p-1f;
    color.z = 0x1.999998p-2f;
    dds3SetLightColor(0, 0, &color.x);
    color.x = 0.0f;
    color.y = 0.0f;
    color.z = 0.0f;
    dds3SetLightColor(0, 1, &color.x);
    color.x = 0.0f;
    color.y = 0.0f;
    color.z = 0.0f;
    dds3SetLightColor(0, 2, &color.x);

    direction.x = 0.0f;
    direction.y = 1.0f;
    direction.z = 0.5f;
    dds3SetLightDir(0, 0, &direction.x);
    direction.x = 1.0f;
    direction.y = 0.0f;
    direction.z = 0.0f;
    dds3SetLightDir(0, 1, &direction.x);
    direction.x = 0.0f;
    direction.y = 0.0f;
    direction.z = 0.1f;
    dds3SetLightDir(0, 2, &direction.x);

    color.x = 0.3f;
    color.y = 0.3f;
    color.z = 0.2f;
    dds3SetAmbientColor(0, &color.x);
    color.x = 0.0f;
    color.y = 0.0f;
    color.z = 0.0f;
    dds3SetFogColor(0, &color.x);
    dds3SetFogPara(0, 255.0f, 0.0f, 3000.0f, 100000.0f);
}

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

typedef struct tagPipeMidiList {
    s32 entries[2];
} tagPipeMidiList;
extern u32 pipeLoadState;
extern s32 pipeFileLoad(char *filename, tagPipePosTable_t *pos, tagPipeTable_t *objects, tagPipeLinkInfo *links);
extern s32 sprintf(char *buffer, const char *format, ...);
extern s32 p2sdLoadCheckMidi(s32 midi);
extern void p2sdLoadSMGFile(s32 midi);
const char D_00548948[24] = "/pipe/PIPE_%02d.DAT";
const char D_00548960[40] = "PosTableData Loading [%s]....OK\n";
const char D_00548988[40] = "ObjectData Loading [%s]....OK\n";

INCLUDE_SDATA(const s32, "pipe/pipeSection", pipeLoadState);

INCLUDE_SDATA(const s32, "pipe/pipeSection", D_00551724);

s32 pipeLoadSection(s32 index)
{
    tagPipeMidiList midiList = {{0x670000, 0x9530000}};
    s32 *midi = midiList.entries;
    char filename[32];

    if (pipeLoadState == 0) {
        pipeDataRelease();
        sprintf(filename, D_00548948, index + 1);
        if (pipeFileLoad(filename, &D_00528F58, &D_00528F68, &linkHeader00)) {
            DEBUG_PRINT(D_00548960, filename);
            DEBUG_PRINT(D_00548988, filename);
            pipeLoadState = 1;
        }
    }
    if (pipeLoadState == 1) {
        if (p2sdLoadCheckMidi(*midi) != 0) {
            pipeLoadState = 0xFF;
        } else {
            p2sdLoadSMGFile(*midi);
            pipeLoadState = 2;
        }
    } else if (pipeLoadState == 2) {
        if (p2sdLoadCheckMidi(*midi) == 1) {
            if (*midi == 0x9530000) {
                pipeLoadState = 0xFF;
            } else {
                pipeLoadState = 1;
            }
        }
    }
    if (pipeLoadState == 0xFF) {
        pipeLoadState = 0;
        return 1;
    }
    return 0;
}

extern s32 pipeIndex;
extern void pipeOffset(void);
extern void pipeStart(void);
extern void pipeEnd(void);
extern void PipeMain(void);
extern void PipeCameraAction3(void);
extern void PipeCameraAction(void);
extern void PipeCamera(void);
extern void CamActionQuake(void);
extern void dds3FadeOut(s32 r, s32 g, s32 b, s32 frames);

s32 pipeMainSection(void)
{
    pipeOffset();
    switch (pipeIndex) {
    case 0:
        pipeStart();
        pipeIndex = 1;
        break;
    case 1:
        if (D_00528F58.tSize - 0x600 < offsetCount) {
            dds3FadeOut(0xFF, 0xFF, 0xFF, 0x3C);
            pipeIndex = 2;
        }
        /* Continue to the end threshold after the fade threshold. */
    case 2:
        if (D_00528F58.tSize - 0x400 < offsetCount) {
            pipeEnd();
            pipeIndex = 0;
            return 1;
        }
        break;
    }

    PipeMain();
    if (offsetCount < 0x320) {
        if ((s8)camMode != 0) {
            PipeCameraAction3();
        } else {
            PipeCameraAction();
        }
    } else {
        PipeCamera();
    }
    if ((s8)CameraAction != 0) {
        CamActionQuake();
    }
    return 0;
}

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

typedef struct tagpipeParts {
    s32 offset;
    u8 id;
    u8 unk05;
    u8 mdl;
    s8 resv[5];
} tagpipeParts;

typedef struct tagMakaObject {
    s32 maka;
    s8 count;
    s8 resv[3];
    s32 pos[2];
    void *makaObject;
} tagMakaObject;

extern tagpipeParts *PartsWork;
extern tagMakaObject *makaWork;
extern void pipeLoadObjects(void);
extern void MyTask(tagPipeObject *object);
extern void EmTask(tagPipeObject *object);
extern void MyHitAction(tagPipeObject *object);
extern void partsDel(tagpipeParts *parts);
extern void pipeEffCal(void);

void PipeMain(void)
{
    tagMakaObject *maka = makaWork;
    tagPipeObject *object = ObjWork;
    tagpipeParts *parts = PartsWork;
    tagPipeObject *lastMyObject = NULL;
    s32 i;

    pipeLoadObjects();
    for (i = 0x95; i != -1; i--) {
        switch (object->id & 0xF0) {
        case 0x10:
            break;
        case 0x80:
            MyTask(object);
            lastMyObject = object;
            break;
        case 0:
            EmTask(object);
            break;
        }
        if (object->id != 0xFF && (s8)object->enabled == 0) {
            objDel(object);
        }
        object++;
    }
    if (lastMyObject != NULL) {
        MyHitAction(lastMyObject);
    }
    pipeEffCal();

    for (i = 0x13; i != -1; i--) {
        if (parts->id != 0xFF && parts->offset < offsetCount - 0x190) {
            partsDel(parts);
        }
        parts++;
    }

    for (i = 0x0E; i != -1; i--) {
        if (maka->maka != 0) {
            maka->count--;
            if (maka->count == 0) {
                maka->maka = 0;
            }
        }
        maka++;
    }
}

INCLUDE_ASM(const s32, "pipe/pipeSection", pipeLoadObjects);

extern void MyAction(tagPipeObject *object);
extern void objMdlAlphaSet(tagPipeObject *object, u8 alpha);
extern f32 vecNormalize(f32 *vector);
extern void vecScale(f32 *vector, f32 scale);
extern void objMdlPosSet(tagPipeObject *object);

void MyTask(tagPipeObject *object)
{
    f32 vector[3];
    tagPipeObject temporary;
    s32 eventOffset = offsetCount;
    s32 speed = 500;
    f32 radiusSquared;

    if (object->offset != eventOffset) {
        object->offset = eventOffset;
    }
    if (eventOffset >= 681) {
        MyAction(object);
        eventOffset = offsetCount;
        if (eventOffset < 709) {
            s32 remaining = 708 - eventOffset;
            f32 alpha = (1.0f - (f32)remaining / 28.0f) * 128.0f;

            temporary.id = 0x81;
            objMdlAlphaSet(&temporary, (u8)(u32)alpha);
            eventOffset = offsetCount;
        }
    }
    if (eventOffset < 800) {
        speed = 490;
    }
    radiusSquared = object->pos[0] * object->pos[0] +
                    object->pos[1] * object->pos[1];
    if (!(radiusSquared <= (f32)(speed * speed))) {
        vector[0] = object->pos[0];
        vector[1] = object->pos[1];
        vector[2] = 0.0f;
        vecNormalize(vector);
        vecScale(vector, (f32)speed);
        object->pos[0] = vector[0];
        object->pos[1] = vector[1];
    }
    objMdlPosSet(object);
}


INCLUDE_ASM(const s32, "pipe/pipeSection", MyAction);

INCLUDE_ASM(const s32, "pipe/pipeSection", MyHitAction);

INCLUDE_ASM(const s32, "pipe/pipeSection", EmTask);

extern void vecRotate2D(f32 *vector, f32 angle);

void ActionRot(tagPipeObject *objPtr) {
    sdfVector4_t vector = (sdfVector4_t){objPtr->pos[0], objPtr->pos[1], 0.0f, 0.0f};
    f32 angle;
    u8 action = objPtr->action;

    if (action < 3) {
        angle = -4.0f;
    } else {
        angle = 8.0f;
    }
    if ((action & 1) == 0) {
        angle = -angle;
    }
    angle *= 0x1.1df46ap-6f;
    vecRotate2D((f32 *)&vector, angle);
    objPtr->pos[0] = vector.x;
    objPtr->pos[1] = vector.y;
}

extern f32 vecNormalize(f32 *vector);
extern void vecScale(f32 *vector, f32 scale);

void ActionHm(tagPipeObject *object) {
    tagPipeObject *data = objMyData();
    f32 vector[3];
    f32 speed;
    f32 newX;
    f32 newY;
    f32 radiusSquared;
    u8 action = object->action;

    if (action < 10) {
        speed = 4.0f;
    } else {
        speed = 8.0f;
    }
    if (action == 7 || action == 9) {
        speed = -speed;
    }
    vector[0] = data->pos[0] - object->pos[0];
    vector[1] = data->pos[1] - object->pos[1];
    vector[2] = 0.0f;
    vecNormalize(vector);
    vecScale(vector, speed);
    newX = object->pos[0] + vector[0];
    newY = object->pos[1] + vector[1];
    object->pos[0] = newX;
    object->pos[1] = newY;

    action = object->action;
    if (action == 10 || action == 8) {
        radiusSquared = newX * newX + newY * newY;
        if (!(radiusSquared <= 250000.0f)) {
            vector[0] = newX;
            vector[1] = newY;
            vector[2] = 0.0f;
            vecNormalize(vector);
            vecScale(vector, 500.0f);
            object->pos[0] = vector[0];
            object->pos[1] = vector[1];
        }
    }
}

INCLUDE_ASM(const s32, "pipe/pipeSection", ActionSlide);

extern tagPipeObject *HITCHECK_CUR(tagPipeObject *object, f32 extension);
extern f32 vecNormalize(f32 *vector);

void ActionCrush(tagPipeObject *object) {
    tagPipeObject *hit;
    f32 vector[3];

    object->offset += 11;
    while ((hit = HITCHECK_CUR(object, 0.0f)) != NULL) {
        vector[0] = object->pos[0] - hit->pos[0];
        vector[1] = object->pos[1] - hit->pos[1];
        vector[2] = 1.0f;
        vecNormalize(vector);
        if (hit->id < 3) {
            hit->action = 17;
            hit->count = 50;
            hit->vpos[0] = -vector[0];
            hit->vpos[1] = -vector[1];
        }
    }
    if (object->count != 0) {
        object->count--;
    }
}

extern f32 vecAngleBetween(f32 *a, f32 *b);

void ActionBound(tagPipeObject *object) {
    f32 velocity[3];
    f32 newPosition[4];
    f32 angledVelocity[4];
    f32 newX;
    f32 newY;
    f32 angle;
    s32 count;
    u8 id;

    velocity[0] = object->vpos[0];
    velocity[1] = object->vpos[1];
    velocity[2] = 0.0f;
    vecNormalize(velocity);
    id = object->id;

    if (id == 0x80) {
        count = object->count;
        if (count >= 6) {
            vecScale(velocity, 150.0f);
        } else {
            vecScale(velocity, (f32)(count * 30));
        }
    } else {
        count = object->count;
        if (count >= 6) {
            vecScale(velocity, 50.0f);
        } else {
            vecScale(velocity, (f32)(count * 10));
        }
    }

    newX = object->pos[0] + velocity[0];
    newY = object->pos[1] + velocity[1];
    object->pos[0] = newX;
    object->pos[1] = newY;
    if (!(newX * newX + newY * newY <= 250000.0f)) {
        newPosition[0] = newX;
        newPosition[1] = newY;
        newPosition[2] = 0x1.999998p-4f;
        angledVelocity[0] = object->vpos[0];
        angledVelocity[1] = object->vpos[1];
        angledVelocity[2] = 0x1.999998p-4f;
        angle = vecAngleBetween(newPosition, angledVelocity);
        if (newPosition[0] != 0.0f && angledVelocity[0] != 0.0f &&
            newPosition[1] / newPosition[0] <
                angledVelocity[1] / angledVelocity[0]) {
            angle = -angle;
        }
        angle = angle + angle;
        vecRotate2D(angledVelocity, angle);

        velocity[0] = object->pos[0];
        velocity[1] = object->pos[1];
        velocity[2] = 0.0f;
        object->vpos[0] = -angledVelocity[0];
        object->vpos[1] = -angledVelocity[1];
        vecNormalize(velocity);
        vecScale(velocity, 500.0f);
        object->pos[0] = velocity[0];
        object->pos[1] = velocity[1];
    }

    if (object->count != 0) {
        object->count--;
    }
}

void ActionAbsorp(tagPipeObject *objPtr)
{
    tagPipeObject *data = objMyData();
    s32 count = objPtr->count;
    s32 complement = 100 - count;
    f32 countWeight;
    f32 complementWeight;

    if (count < 0) {
        return;
    }
    countWeight = (f32)count;
    complementWeight = (f32)complement;
    objPtr->offset = data->offset;
    objPtr->pos[0] = (objPtr->pos[0] * countWeight +
                      data->pos[0] * complementWeight) / 100.0f;
    objPtr->pos[1] = (objPtr->pos[1] * countWeight +
                      data->pos[1] * complementWeight) / 100.0f;
    if (count > 0) {
        objPtr->count = (s16)((f32)count - 0x1.0aaaaap+4f);
    }
    if (objPtr->count < 0) {
        objPtr->count = 0;
    }
}

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



extern tagMakaObject *makaWork;
extern void MyDraw(tagPipeObject *object);
extern void makaDraw(tagMakaObject *makaPtr);
extern void pipeEffDraw(void);

void ObjectDraw(void)
{
    tagMakaObject *maka = makaWork;
    tagPipeObject *object = ObjWork;
    tagMakaObject *deferred = NULL;
    s32 i;

    for (i = 149; i != -1; i--) {
        if (((u8)object->id & 0xF0) == 0x80) {
            MyDraw(object);
        }
        object++;
    }
    pipeEffDraw();
    eglSetOt(0x47);
    eglLinkTest(1, 3, 0x80, 0, 0, 0, 1, 1);
    for (i = 14; i != -1; i--) {
        if (maka->maka != 0) {
            if (maka->count == 7) {
                if (deferred == NULL) {
                    deferred = maka;
                } else {
                    makaDraw(maka);
                }
            } else {
                makaDraw(maka);
            }
        }
        maka++;
    }
    if (deferred != NULL) {
        makaDraw(deferred);
    }
    eglSetOt(0x27);
    eglLinkAlpha(0);
    eglLinkTest(1, 5, 0x80, 1, 0, 0, 1, 3);
    /* Like pipeDrawSection, reset the local after the final call to retain
     * the native jal and epilogue (CONTRIBUTING matching-rule exception). */
    i = 0;
}


void MyDraw(tagPipeObject *object) {
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
INCLUDE_SDATA(const s32, "pipe/pipeSection", pbHitCheckCur);

INCLUDE_SDATA(const s32, "pipe/pipeSection", offsetCount);

INCLUDE_SDATA(const s32, "pipe/pipeSection", D_00551738);

INCLUDE_SDATA(const s32, "pipe/pipeSection", D_0055173C);

INCLUDE_SDATA(const s32, "pipe/pipeSection", D_00551740);

INCLUDE_SDATA(const s32, "pipe/pipeSection", D_00551744);

INCLUDE_SDATA(const s32, "pipe/pipeSection", D_00551748);

INCLUDE_SDATA(const s32, "pipe/pipeSection", pipeIndex);

INCLUDE_SDATA(const s32, "pipe/pipeSection", attackCount);

INCLUDE_SDATA(const s32, "pipe/pipeSection", D_00551751);

INCLUDE_SDATA(const s32, "pipe/pipeSection", myMaka);

INCLUDE_SDATA(const s32, "pipe/pipeSection", ZmakaCount);

INCLUDE_SDATA(const s32, "pipe/pipeSection", playerDAMAGE);

INCLUDE_SDATA(const s32, "pipe/pipeSection", rBoxNum);

INCLUDE_SDATA(const s32, "pipe/pipeSection", offsetStep);

INCLUDE_SDATA(const s32, "pipe/pipeSection", offsetStepCount);

INCLUDE_SDATA(const s32, "pipe/pipeSection", CameraAction);

INCLUDE_SDATA(const s32, "pipe/pipeSection", camMode);

INCLUDE_SDATA(const s32, "pipe/pipeSection", D_00551765);

INCLUDE_SDATA(const s32, "pipe/pipeSection", myStatus);

INCLUDE_SDATA(const s32, "pipe/pipeSection", pipeObjList);

INCLUDE_SDATA(const s32, "pipe/pipeSection", pipeObjTail);

INCLUDE_SDATA(const s32, "pipe/pipeSection", D_00551770);

INCLUDE_SDATA(const s32, "pipe/pipeSection", pipeMyMakaCount);

INCLUDE_SDATA(const s32, "pipe/pipeSection", D_00551778);

