#include "fpu.h"
#include "common.h"
#include "ps2asm.h"
#include "basic.h"
#include "motion.h"
#include "pipe.h"

extern void (*D_00551828)(void);

extern dds3Basic_t *dds3GetControlScene(void);

extern void *memset(void *a, s32 b, u32 c);

extern void sdfMemFreeDirect(void *aHandle);




extern u16 myStatus;

extern s32 datGetPlayerParam(s32 id);

extern s8 camMode, CameraAction;
extern u8 offsetStep, offsetStepCount;

extern s32 D_00551738, D_0055173C;
/* ActionKey updates these two axis rates with single-precision arithmetic. */
extern f32 D_00551744, D_00551748;

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





extern tagPipeLinkInfo linkHeader00;

extern tagPipePosTable D_00528F58;

extern tagPipeTable D_00528F68;

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
extern s32 pipeFileLoad(char *filename, tagPipePosTable *pos, tagPipeTable *objects, tagPipeLinkInfo *links);
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

#ifdef NON_MATCHING
extern tagPipePartsInfo *getPartsInfo(s16 partsID);
extern s32 D_00551740;

void pipeOffset(void)
{
    tagPipeLinkData *node = linkHeader00.head;
    tagPipePartsType *part = getPartsInfo(node->partsID)->data;
    s32 start = 0;
    s32 position = offsetCount;
    u16 id = node->id;
    s8 step;
    s8 count;
    u8 countBits;
    s32 next;

    while ((u32)position >= (u32)(start + part->frame) && position >= start && id != 0xFFFF) {
        node = node->next;
        start = (s32)((u32)start + part->frame);
        part = getPartsInfo(node->partsID)->data;
        id = node->id;
        position = offsetCount;
    }
    step = *(s8 *)&offsetStep;
    count = *(s8 *)&offsetStepCount;
    D_00551740 = id;
    next = (s32)((u32)position + (u32)(s32)step + 7);
    countBits = *(volatile u8 *)&offsetStepCount;
    if (count > 0) {
        offsetStepCount = countBits - 1;
        offsetStep = (u8)-3;
    } else if (count < 0) {
        offsetStepCount = countBits + 1;
        offsetStep = 3;
    } else {
        offsetStep = 0;
    }
    offsetCount = next >= 0 ? next : 10;
}
#else
INCLUDE_ASM(const s32, "pipe/pipeSection", pipeOffset);
#endif

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

typedef struct tagPipeObjectEntry {
    s32 offset;
    u8 parameters[4];
} tagPipeObjectEntry;

extern tagPipePartsInfo *getPartsInfo(s16 partsID);
extern u8 objLoad(tagPipeObject *object, s32 index);
extern void objOption(tagPipeObject *object, u8 option);
extern void partsAdd(tagPipeLinkData *node, s32 offset);

void pipeLoadObjects(void)
{
    tagPipeObject object;
    tagPipeLinkData *node = linkHeader00.head;
    s32 count = D_00528F68.tabMax;
    s8 *loaded = D_00528F68.data;
    tagPipeObjectEntry *entry = D_00528F68.tabPtr;
    tagPipePartsInfo *info;

    while (count != 0) {
        if (*loaded == 0 && entry->offset <= offsetCount + 500) {
            memset(&object, 0, sizeof(object));
            objOption(&object, objLoad(&object, D_00528F68.tabMax - count));
            *loaded = 1;
        }
        count--;
        entry++;
        loaded++;
    }

    getPartsInfo(node->partsID);
    count = 0;
    while (count < D_0055173C) {
        info = getPartsInfo(node->partsID);
        node = node->next;
        count += info->data->frame;
        if (node->id == 0xFFFF) {
            break;
        }
    }

    count = 5;
    while (count != 0 && node->id != 0xFFFF) {
        info = getPartsInfo(node->partsID);
        if (D_0055173C < offsetCount - 400) {
            node = node->next;
            D_0055173C += info->data->frame;
        } else {
            if (D_0055173C > offsetCount + 1520) {
                break;
            }
            partsAdd(node, D_0055173C);
            count--;
            node = node->next;
            D_0055173C += info->data->frame;
        }
    }
}


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


extern void pipeEffSet(tagPipeObject *, s32);
extern void ActionBound(tagPipeObject *);
extern void ActionKey(tagPipeObject *);
extern void objMyStatusChange(tagPipeObject *, u8);

#ifdef NON_MATCHING
void MyAction(tagPipeObject *object)
{
    u16 count;
    switch (object->action) {
    case 1:
        object->count--;
        if ((u16)(object->count - 6) < 19) {
            object->flags |= 0x100;
        } else {
            object->flags &= 0xFEFF;
        }
        if ((myStatus & 0xF) != 0) {
            s16 current = object->count;
            count = *(volatile u16 *)&object->count;
            if (current == 21) {
                pipeEffSet(object, 6);
                count = *(volatile u16 *)&object->count;
            }
        } else {
            count = *(volatile u16 *)&object->count;
        }
        {
            u8 attack = *(volatile u8 *)&attackCount;
            if (attack == 1 || attack == 2) {
                if ((u32)count - 8 < 9) {
                    object->flags &= 0xFDFF;
                } else {
                    object->flags |= 0x200;
                }
            }
        }
        if ((s16)count < 2) {
            object->flags &= 0xFDFF;
            attackCount = 0;
            count = object->count;
        }
        if (count == 0) {
            objMyStatusChange(object, 0);
        }
        break;
    case 2:
        object->count--;
        if ((u16)(object->count - 21) < 9) {
            object->flags |= 0x400;
        } else {
            object->flags &= 0xFBFF;
        }
        if (object->count < 20) {
            object->flags &= 0xFDFF;
        }
        if (object->count == 0) {
            objMyStatusChange(object, 0);
        }
        break;
    case 30:
        ActionBound(object);
        if (object->count == 0) {
            object->action = 0;
        }
        break;
    }
    if ((f32)(D_00528F58.tSize - 0x600) + 256.0f < (f32)offsetCount) {
        object->flags |= 0x200;
    }
    ActionKey(object);
}
#else
INCLUDE_ASM(const s32, "pipe/pipeSection", MyAction);
#endif

extern tagPipeObject *HITCHECK_CUR(tagPipeObject *object, f32 extension);
extern void objSetAction(tagPipeObject *object, u8 action, s16 count);
extern void pipeMyMakaAdd(s32 amount);
extern void pipeMyMakaSubRate(f32 rate);
extern void makaAdd(f32 x, f32 y, s32 amount);
extern void objAddDMaka(s32 offset, f32 x, f32 y);
extern void objAddDRBOX(s32 offset, f32 x, f32 y);
extern u32 dds3GetRandA(void *state);

#ifdef NON_MATCHING
static void pipeDropMaka(tagPipeObject *object, f32 rate)
{
    pipeMyMakaSubRate(rate);
    myMaka = myMaka < 0 ? 0 : myMaka;
    objAddDMaka(object->offset, object->pos[0], object->pos[1]);
    object->flags |= 0x10;
}

static void pipeDropRBox(tagPipeObject *object)
{
    rBoxNum--;
    objAddDRBOX(object->offset, object->pos[0], object->pos[1]);
}

void MyHitAction(tagPipeObject *object)
{
    tagPipeObject *hit;

    while ((hit = HITCHECK_CUR(object, 35.0f)) != NULL) {
        if ((hit->id == 5 || hit->id == 6 || hit->id == 10) && hit->action != 100) {
            objSetAction(hit, 100, 100);
            hit->vpos[0] = hit->pos[0];
            hit->vpos[1] = hit->pos[1];
            hit->w *= 1.5f;
        }
    }
    while ((hit = HITCHECK_CUR(object, 0.0f)) != NULL) {
        s32 damage = 0;
        s32 resources;
        s32 savedMaka;
        u8 savedBox;

        if (hit->id == 5 && !(hit->flags & 1)) {
            pipeMyMakaAdd(5);
            makaAdd(hit->vpos[0], hit->vpos[1], 5);
            hit->flags |= 1;
            object->flags |= 1;
        }
        if (hit->id == 6 && !(hit->flags & 1)) {
            pipeMyMakaAdd(100);
            makaAdd(hit->vpos[0], hit->vpos[1], 100);
            hit->flags |= 1;
            object->flags |= 1;
        }
        if (hit->id == 9 && !(hit->flags & 2) && hit->action == 21 && hit->count == 0) {
            if (ZmakaCount >= 7) {
                pipeMyMakaAdd(9999);
                makaAdd(hit->vpos[0], hit->vpos[1], (s32)0x8000270Fu);
            } else {
                pipeMyMakaAdd((s32)(100u << ((u32)ZmakaCount & 31)));
                makaAdd(hit->vpos[0], hit->vpos[1],
                        (s32)((100u << ((u32)ZmakaCount & 31)) | 0x80000000u));
                ZmakaCount = (s32)((u32)ZmakaCount + 1);
            }
            hit->flags |= 2;
            object->flags |= 2;
        }
        if (hit->id == 10 && !(hit->flags & 4)) {
            object->flags |= 4;
            hit->flags |= 4;
            rBoxNum++;
        }
        if (hit->id == 1 && !(hit->flags & 0x2000)) {
            u16 flags = object->flags;
            u16 status = *(volatile u16 *)&myStatus;
            if ((flags & 0x100) && (status & 0xF) > 0) {
                objSetAction(hit, 20, 0);
            } else {
                damage |= 1;
            }
        }
        savedMaka = myMaka;
        if (hit->id == 2 && !(hit->flags & 0x2000)) {
            u16 flags = object->flags;
            u16 status = *(volatile u16 *)&myStatus;
            if ((flags & 0x100) && (s32)(status & 0xF) >= 0) {
                objSetAction(hit, 20, 0);
                savedMaka = myMaka;
            } else {
                damage |= 2;
            }
        }

        resources = (savedMaka != 0 ? 4 : 0) | (*(volatile s8 *)&rBoxNum != 0 ? 8 : 0);
        savedBox = *(volatile u8 *)&rBoxNum;
        if ((damage & 1) && CameraAction == 0) {
            objMyStatusChange(object, 2);
            playerDAMAGE = (s32)((u32)playerDAMAGE + 0x10000u);
            if (resources == 4 ||
                (resources == 12 && (dds3GetRandA(NULL) & 0xFF) < 0xA6)) {
                pipeDropMaka(object, 0.25f);
            } else if (resources == 8 || (resources == 12 && rBoxNum > 0)) {
                pipeDropRBox(object);
            }
            savedMaka = myMaka;
            savedBox = (u8)rBoxNum;
        }
        resources = (savedMaka != 0 ? 4 : 0) | (savedBox != 0 ? 8 : 0);
        if ((damage & 2) && CameraAction == 0) {
            objMyStatusChange(object, 2);
            playerDAMAGE = (s32)((u32)playerDAMAGE + 1);
            if (resources == 4 ||
                (resources == 12 && (dds3GetRandA(NULL) & 0xFF) < 0xCD)) {
                pipeDropMaka(object, 0.09999999404f);
            } else if (resources == 8 || (resources == 12 && rBoxNum > 0)) {
                pipeDropRBox(object);
            }
        }
    }
}
#else
INCLUDE_ASM(const s32, "pipe/pipeSection", MyHitAction);
#endif

/* NON_MATCHING: 22 of 211 words differ; see docs/matching/pipe/pipeSection.md. */
#ifdef NON_MATCHING
extern u8 objMake(tagPipeObject *object, s32 offset, u8 id, u8 way, u8 action, u8 value);
extern s32 objAdd(tagPipeObject *object);
extern void ActionRot(tagPipeObject *object);
extern void ActionHm(tagPipeObject *object);
extern void ActionSlide(tagPipeObject *object);
extern void ActionCrush(tagPipeObject *object);
extern void ActionBound(tagPipeObject *object);
extern void ActionDeath(tagPipeObject *object);
extern void ActionAbsorp(tagPipeObject *object);
extern void ActionZMotion(tagPipeObject *object);
extern void objSetAction(tagPipeObject *object, u8 action, s16 count);
extern void objMdlSclSet(tagPipeObject *object, f32 scale);

void EmTask(tagPipeObject *object)
{
    tagPipeObject temporary;
    u8 action;
    u8 nextAction;

    memset(&temporary, 0, sizeof(temporary));
    action = object->action;
    if (action == 6 || action == 15 || action == 16) {
        s32 objectOffset = object->offset;
        s32 count = offsetCount;

        if (count + 100 < objectOffset && (object->flags & 0x4000) != 0) {
            ZmakaCount = 0;
            object->flags &= (u16)~0x4000;
        }
        if (count + 500 < objectOffset) {
            object->enabled = 0;
            return;
        }
    } else if (object->offset < offsetCount - 60) {
        ZmakaCount = (object->flags & 0x4000) != 0 ? 0 : ZmakaCount;
        object->enabled = 0;
        return;
    }

    if (object->id == 0xB) {
        tagPipeObject *player = objMyData();
        s32 currentOffset = offsetCount;
        s16 count = object->count - 1;

        object->offset = currentOffset;
        object->pos[0] = player->pos[0];
        object->count = count;
        object->pos[1] = player->pos[1];
        if (count == 0) {
            object->enabled = 0;
        }
    }
    if (object->id == 0xC) {
        tagPipeObject *player = objMyData();
        s32 currentOffset = offsetCount;
        s16 count = object->count - 1;

        object->offset = currentOffset;
        object->pos[0] = player->pos[0];
        object->count = count;
        object->pos[1] = player->pos[1];
        if (count == 0) {
            object->enabled = 0;
        }
    }

    nextAction = object->action;
    switch (nextAction) {
        case 0:
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            ActionRot(object);
            break;
        case 5:
            object->offset -= 7;
            break;
        case 6:
            object->offset += 9;
            break;
        case 7:
        case 8:
        case 9:
        case 10:
            ActionHm(object);
            break;
        case 11:
        case 12:
        case 13:
        case 14:
            ActionSlide(object);
            break;
        case 15:
        case 16:
            object->offset += 9;
            ActionRot(object);
            break;
        case 17:
            ActionCrush(object);
            ActionBound(object);
            if (object->count == 0) {
                objSetAction(object, 0x14, 5);
            }
            break;
        case 18:
            ActionCrush(object);
            ActionBound(object);
            objMake(&temporary, object->offset, 5, 0, 0, 0);
            temporary.pos[0] = object->pos[0];
            temporary.pos[1] = object->pos[1];
            objAdd(&temporary);
            if (object->count == 0) {
                object->enabled = 0;
            }
            break;
        case 26:
            object->enabled = 0;
            break;
        case 19:
            ActionBound(object);
            if (object->count == 0) {
                objSetAction(object, 0, 0);
            }
            break;
        case 20:
            ActionDeath(object);
            if (object->count == 0) {
                object->enabled = 0;
            }
            break;
        case 21:
            ActionZMotion(object);
            if ((object->flags & 2) != 0) {
                object->enabled = 0;
            }
            break;
        case 25:
            ActionCrush(object);
            if (object->count == 0) {
                object->enabled = 0;
            }
            break;
        case 60:
        case 61:
        case 62:
        case 63:
            object->offset += nextAction - 59;
            break;
        case 100:
            if (object->count == 0) {
                object->enabled = 0;
            } else {
                ActionAbsorp(object);
                objMdlSclSet(object, (f32)object->count / 100.0f);
            }
            break;
        default:
            break;
    }
    objMdlPosSet(object);
}
#else
INCLUDE_ASM(const s32, "pipe/pipeSection", EmTask);
#endif

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

void ActionSlide(tagPipeObject *object)
{
    s32 axis = 0;
    f32 step = 16.0f;

    if (object->action == 12 || object->action == 14) {
        axis = 1;
    }
    if (object->action == 13 || object->action == 14) {
        step = 32.0f;
    }
    if (object->count != 0) {
        step = -step;
    }
    object->pos[axis] += step;
    if (!(object->pos[0] * object->pos[0] + object->pos[1] * object->pos[1] <= 250000.0f)) {
        object->count ^= 1;
    }
}


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

/* NON_MATCHING: 6 of 80 words differ; see docs/matching/pipe/pipeSection.md. */
#ifdef NON_MATCHING
extern void objMdlSclSet(tagPipeObject *object, f32 scale);

void ActionZMotion(tagPipeObject *object)
{
    tagPipeObject *data;
    s16 count;
    s16 limitedCount;
    s32 complement;
    f32 countWeight;
    f32 complementWeight;

    data = objMyData();
    count = object->count;
    limitedCount = count;
    if (limitedCount >= 0x65) {
        limitedCount = 0x64;
    }
    if (limitedCount >= 0) {
        complement = 100 - limitedCount;
        complementWeight = (f32)complement;
        countWeight = (f32)limitedCount;
        object->offset = data->offset;
        object->pos[0] = (object->pos[0] * countWeight +
                          data->pos[0] * complementWeight) / 100.0f;
        object->pos[1] = (object->pos[1] * countWeight +
                          data->pos[1] * complementWeight) / 100.0f;
        if (count > 0) {
            object->count = (s16)((f32)count - 8.33333301544189453125f);
        }
        if (object->count < 0) {
            object->count = 0;
        }
    }
    countWeight = (f32)limitedCount;
    if (countWeight <= 66.66666412353515625f && 50.000003814697265625f <= countWeight) {
        objMdlSclSet(object,
                     ((countWeight - 50.000003814697265625f) / 16.666660308837890625f) *
                         0.800000011920928955078125f + 0.199999988079071044921875f);
        /* Like ObjectDraw, retain jal plus epilogue. Qualified cc1probe:
         * reset187 survives sibling selection, then disappears in life13;
         * the no-reset control tail-calls. Output parity passes. See
         * docs/CONTRIBUTING.md for this narrow local-reset exception. */
        limitedCount = 0;
    }
}
#else
INCLUDE_ASM(const s32, "pipe/pipeSection", ActionZMotion);
#endif

extern s8 dds3PadButtonState[];

#ifdef NON_MATCHING
void ActionKey(tagPipeObject *object)
{
    f32 vector[4];
    f32 horizontalStep = 0.0f;
    f32 verticalStep = 0.0f;
    f32 acceleration = 0.0f;
    f32 maximum = 0.0f;
    f32 friction = 0.0f;
    f32 rate;
    f32 velocity;
    f32 length;
    u8 mode;

    memset(vector, 0, sizeof(vector));
    if (dds3PadButtonState[0x23] < 0 && !(object->flags & 0x200)) {
        objMyStatusChange(object, 1);
        if (object->action == 1 && object->count >= 3) {
            attackCount++;
        }
    }
    mode = *(volatile u8 *)&myStatus >> 4;
    if (object->flags & 0x400) {
        mode = 0;
    }
    if (mode == 0) {
        horizontalStep = 2.0f;
        verticalStep = 2.0f;
        acceleration = 4.0f;
        maximum = 20.0f;
        friction = 1.0f;
    } else if (mode == 1) {
        horizontalStep = 2.5f;
        verticalStep = 2.5f;
        acceleration = 6.0f;
        maximum = 36.0f;
        friction = 2.0f;
    } else if (mode == 2) {
        horizontalStep = 3.0f;
        verticalStep = 3.0f;
        acceleration = 36.0f;
        maximum = 56.0f;
        friction = 14.0f;
    }

    if (dds3PadButtonState[0x26] != 0) {
        rate = D_00551748 - verticalStep;
        if (rate < 0.0f ? rate < -10.0f : rate > 10.0f) {
            rate = -10.0f;
        }
        D_00551748 = rate;
        object->vpos[1] -= acceleration;
        if (object->vpos[1] < -maximum) {
            object->vpos[1] = -maximum;
        }
    } else if (dds3PadButtonState[0x27] != 0) {
        rate = D_00551748 + verticalStep;
        if (rate < 0.0f ? rate < -10.0f : rate > 10.0f) {
            rate = 10.0f;
        }
        D_00551748 = rate;
        object->vpos[1] += acceleration;
        if (object->vpos[1] > maximum) {
            object->vpos[1] = maximum;
        }
    }
    if (dds3PadButtonState[0x24] != 0) {
        rate = D_00551744 - horizontalStep;
        if (rate < 0.0f ? rate < -10.0f : rate > 10.0f) {
            rate = -10.0f;
        }
        D_00551744 = rate;
        object->vpos[0] -= acceleration;
        if (object->vpos[0] < -maximum) {
            object->vpos[0] = -maximum;
        }
    } else if (dds3PadButtonState[0x25] != 0) {
        rate = D_00551744 + horizontalStep;
        if (rate < 0.0f ? rate < -10.0f : rate > 10.0f) {
            rate = 10.0f;
        }
        D_00551744 = rate;
        object->vpos[0] += acceleration;
        if (object->vpos[0] > maximum) {
            object->vpos[0] = maximum;
        }
    }

    rate = D_00551748;
    if (rate != 0.0f) {
        rate = rate > 0.0f ? rate - 0.5f : rate + 0.5f;
        D_00551748 = rate;
    }
    if (rate < 0.0f ? rate > -0.5f : rate < 0.5f) {
        D_00551748 = 0.0f;
    }
    velocity = object->vpos[1];
    if (velocity != 0.0f) {
        velocity = velocity > 0.0f ? velocity - friction : velocity + friction;
        object->vpos[1] = velocity;
    }
    if (velocity < 0.0f ? -velocity < friction : velocity < friction) {
        object->vpos[1] = 0.0f;
    }

    rate = D_00551744;
    if (rate != 0.0f) {
        rate = rate > 0.0f ? rate - 0.5f : rate + 0.5f;
        D_00551744 = rate;
    }
    if (rate < 0.0f ? rate > -0.5f : rate < 0.5f) {
        D_00551744 = 0.0f;
    }
    velocity = object->vpos[0];
    if (velocity != 0.0f) {
        velocity = velocity > 0.0f ? velocity - friction : velocity + friction;
        object->vpos[0] = velocity;
    }
    if (velocity < 0.0f ? -velocity < friction : velocity < friction) {
        object->vpos[0] = 0.0f;
    }

    vector[0] = object->vpos[0];
    vector[1] = object->vpos[1];
    vector[2] = 0.0f;
    length = vecNormalize(vector);
    if (object->flags & 0x800) {
        if (length > 8.0f) {
            length = 8.0f;
        }
    } else if (length > maximum) {
        length = maximum;
    }
    vecScale(vector, length);
    object->pos[0] += vector[0];
    object->pos[1] += vector[1];
}
#else
INCLUDE_ASM(const s32, "pipe/pipeSection", ActionKey);
#endif

#ifdef NON_MATCHING
extern volatile s32 pbHitCheckCur;

tagPipeObject *HITCHECK_CUR(tagPipeObject *object, f32 extension)
{
    s32 cursor = pbHitCheckCur;
    s32 remaining = 150;
    tagPipeObject *candidate = ObjWork;

    while (remaining != cursor) {
        remaining--;
        candidate++;
    }
    remaining = cursor - 1;
    while (remaining != -1) {
        u8 id = candidate->id;
        pbHitCheckCur = remaining;
        if (id != 0xFF && id != object->id) {
            f32 candidateRadius = candidate->w;
            f32 extra = candidate->action == 5 ? candidateRadius : 0.0f;
            f32 radius = object->w + candidateRadius + extension;
            f32 reach = radius + extra;
            s32 difference = (s32)((u32)candidate->offset - (u32)object->offset);
            s32 distance;

            if (difference >= 0) {
                distance = (s32)((u32)difference * 40);
            } else {
                difference = (s32)((u32)object->offset - (u32)candidate->offset);
                distance = (s32)((u32)difference * 40);
            }
            if ((f32)distance <= reach) {
                f32 dx = *(volatile f32 *)&object->pos[0] - *(volatile f32 *)&candidate->pos[0];
                f32 dy = *(volatile f32 *)&object->pos[1] - *(volatile f32 *)&candidate->pos[1];
                if (dx * dx + dy * dy <= radius * radius) {
                    return candidate;
                }
            }
        }
        candidate++;
        remaining = pbHitCheckCur - 1;
    }
    pbHitCheckCur = 150;
    return NULL;
}
#else
INCLUDE_ASM(const s32, "pipe/pipeSection", HITCHECK_CUR);
#endif



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

