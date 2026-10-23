#include "common.h"
#include "basic.h"
#include "ps2asm.h"
#include "field.h"
#include "effBattle.h"
#include "model.h"
#include "billboard.h"

typedef struct
{
    sdfVector4_t position;
    sdfVector4_t rotation;
    f32 range;
    f32 height;
    f32 parameter;
    u32 color;
} dds3EffectEventParameters_t;

typedef struct
{
    sdfVector4_t position[2];
    f32 radius[2];
} effMagaSCArea_t;

typedef struct
{
    sdfVector4_t position[4];
    f32 radius[4];
} effMagaP4Point_t;

extern const dds3EffectEventParameters_t D_00530E20;
extern const effMagaSCArea_t D_00530E50;
extern const effMagaP4Point_t D_00530E78;
extern void *memcpy(void *a, const void *b, u32 c);
extern void dds3BillboardSetPosition(void *hBillboard, const void *pPos);
extern void dds3BillboardSetAnimNumber(void *hBillboard, u32 number);
extern void dds3EffectSetPosition(void *pData, const void *pPos);
extern struct effHandle_t *dds3EffectLoad(void *pD3P);
extern struct effHandle_t *dds3EffectFileLoad(u32 pFilename);
extern  effBTL_t *effEveBEDCreate( effBTL_t *pData);
extern void effMagaSCSetArea(void *hEffect, const void *pMagaSCArea);
extern void effMagaP4SetPoint(void *hEffect, const void *pP4Point);

extern sdfVector4_t aViewEye;
extern s32 fldGetModelFlag2(s32 map, u32 floor, s32 group);
extern s32 fldGetAnimeId2(s32 map, u32 floor, s32 group);
extern void dds3BillboardSetRGBA(void *hBillboard, u32 abgr);

typedef struct
{
    sdfVector4_t position;
    sdfVector4_t rotation;
} dds3EffectOwnerPose_t;

extern dds3Basic_t *dds3GetActiveScene(void);
extern s32 dds3SceneBasicExistBasicFromType(dds3Basic_t *scene, s32 type, dds3Basic_t *target);
extern void dds3EffectObjectBasicGetObjectIndex(dds3Basic_t *basic);
extern void dds3EffectObjectBasicUnlinkObject(dds3Basic_t *basic);
extern void dds3DeleteSceneBasicData(dds3Basic_t *basic);
extern void dds3OffPoseSetFlag(dds3PoseSet_t *pPoseSet, u32 flag);
extern void ToMatrix(void);
extern void MultMatrixRevNoArg(void);
extern void Normalize(void);
extern void dds3EffectSetRotateMatrix(void *pData, const sdfMatrix4_t *pMatrix);
extern s32 dds3EffectGetState(void *hEffect);
extern s32 dds3BillboardGetAnimState(void *hBillboard);
extern void effEveBEDCutSetParams(void *hEveBEDCut, const void *pParams);


typedef struct
{
    u32 flag;
    u32 type;
    u32 noEffect;
    f32 size[3];
    u32 offsetZ;
    s32 groupID;
    u32 param0;
    u32 param1;
} fldResourceEffect_t;


typedef struct
{
    dds3ObjectBaseControlWorkData_t *pObjectBaseControlWorkData;
    u32 stat;
    s32 type;
    void *handle;
    u32 linkpos[4]; /* sdfVector4_t in the IDB, written as words here */
    dds3Basic_t *linkbasic;
    u16 linkindex;
    u16 linkbasictype;
    void *para;
    void *cuth;
    s32 reserve1;
    s32 reserve2;
    s32 reserve3;
    fldResourceEffect_t *pFldEx;
    sdfVector4_t FldExBasePos;
} dds3EffectObjectBasicWorkData_t;


struct billHandle_t *dds3BillboardCopy(struct billHandle_t *hBillboard);
struct billHandle_t *dds3BillboardCreate(s32 kind, u32 id);
void effMagaP4Randomize(void *hEffect);
void *memset(void *a, s32 b, u32 c);
void *sdfDevLoadFileEx(void *resource, u32 *id, s32 mode);


struct BillChildPayload;
typedef struct BillChildPayload BillChildPayload;
struct dds3BillAnimResource_tag;
typedef struct dds3BillAnimResource_tag dds3BillAnimResource_tag;
struct dds3BillAnim_tag;
typedef struct dds3BillAnim_tag dds3BillAnim_tag;
struct EvtUnit;
typedef struct EvtUnit EvtUnit;
struct DevRequest;
typedef struct DevRequest DevRequest;
struct SdfChunkHeader;
typedef struct SdfChunkHeader SdfChunkHeader;
struct SdfDmaTagHeader;
typedef struct SdfDmaTagHeader SdfDmaTagHeader;
struct SdfDrawNode;
typedef struct SdfDrawNode SdfDrawNode;
struct effPartHandle_t;
typedef struct effPartHandle_t effPartHandle_t;

typedef struct effEveBEDCutHandle_t effEveBEDCutHandle_t;

typedef struct BattleEffect BattleEffect;

typedef void (*FileRequestCallback)(void *request, u32 arg);

typedef struct effHandle_t {
    u32 type;
    s32 arg;
    s32 instance;
    f32 unkC;
} effHandle_t;

enum {
    EFF_WORLD_KIND_WORLD_INFO = 0,
    EFF_WORLD_KIND_EVENT_TABLE = 1,
    EFF_WORLD_KIND_WORLD_INDEX_OWNER = 2,
    EFF_WORLD_KIND_SLOT_RING = 3,
    EFF_WORLD_KIND_CAMERA = 4,
    EFF_WORLD_KIND_FOLLOW_MODEL = 5,
    EFF_WORLD_KIND_EFFECT_TRANSFORM = 6,
    EFF_WORLD_KIND_EFFECT_OBJECT = 7,
    EFF_WORLD_KIND_RESOURCE_OWNER = 8,
    EFF_WORLD_KIND_LIGHT = 9,
    EFF_WORLD_KIND_SCRIPT_OBJECT = 10,
    EFF_WORLD_KIND_ACTION_B = 11,
    EFF_WORLD_KIND_ACTION_D = 13,
    EFF_WORLD_KIND_ACTION_10 = 16,
    EFF_WORLD_KIND_TRANSFORM_SOURCE = 17,
    EFF_WORLD_KIND_COUNT = 18
};

#define EFF_OBJ_STATE_EVENT_NODE 5

#define EFF_OBJ_ENTRY_ID_MASK 0xFFFF

#define EFF_OBJ_STATE_PARAMETERS_READY 6

#define EFF_OBJ_KIND EFF_WORLD_KIND_EFFECT_OBJECT

#define EFF_OBJ_STATE_MAGATUHI_TWO_ROWS 7

#define EFF_OBJ_STATE_MAGATUHI_FOUR_ROWS 8

#define EFF_OBJ_STATE_BOUND_BILL 1

#define EFF_OBJ_OWNER_KIND_FIRST 4

#define EFF_OBJ_OWNER_KIND_END 10

#define EFF_OBJ_OWNER_KIND_EXTRA 0x11

#define EFF_OBJ_FLAG_OWNER_LINK 4

#define EFF_OBJ_FLAG_OWNER_BILL_ENTRY 8

typedef struct EffOwnerBillPayloadView {
    u8 pad00[8];
    u32 state;
    void *params;
} EffOwnerBillPayloadView;

#define EFF_OBJ_OWNER_BILL_KIND 5

typedef struct EffectObjectData {
    dds3ObjectBaseControlWorkData_t *handle;
    s32 word04;
    struct EvtUnit *transitionWork;
    dds3ObjectBaseControlWorkData_t *modelHolder;
    s32 activeId;
    u32 followParameterIndex;
    u32 word18;
    s32 pendingTargetKey;
    s32 angleReturnDelayFrames;
    u32 word24;
    f32 angle;
    f32 limitMin2C;
    f32 limitMax30;
    f32 word34;
    f32 limitMin38;
    f32 limitMax3C;
} EffectObjectData;

typedef struct EffOwnerBillParamView {
    u8 pad00[0x18];
     sdfModelRootNode_t *model;
} EffOwnerBillParamView;

void dds3EffectDelete(effHandle_t *pData);
void dds3BillboardDelete(struct billHandle_t *hData);
void effEveBEDCutDelete(effEveBEDCutHandle_t *hEveBEDCut);
void effEveBEDDelete( effBTL_t *hEveBED);
void sdfChipFreeDirect(void *aPtr);
s32 dds3InitPoseSet(dds3Basic_t *basic);
void *sdfChipAllocDirect(u32 aSize);
extern void dds3ObjectBaseOnFlag(dds3Basic_t *basic, s32 mask);
extern u8 opaque40[0x60];
void dds3FreePoseSet(dds3Basic_t *basic);
void dds3EffectObjectBasicCalcFldEx(dds3Basic_t *basic);
u8 dds3ObjectBaseCheckFlag(struct dds3Basic_t *pObjectBasic, u32 flag);
void dds3EffectDraw(effHandle_t *pData);
extern void dds3BillboardDraw(struct billHandle_t *hData);
void effEveBEDCutDraw(effEveBEDCutHandle_t *hEveBEDCut);
dds3Basic_t *dds3AddSceneBasicData(s32 type);
void dds3ObjectBaseCreateMover(dds3Basic_t *pObjectBasic);
void dds3SetPosPoseSet(dds3Basic_t *pBasic, const sdfVector4_t *pos);
void dds3SetRotPoseSet(dds3Basic_t *pBasic, const sdfVector4_t *rot);
extern void dds3SetRadiusPoseSet(dds3PoseSet_t *node);
dds3Basic_t *AddP2AEffect(void *p2a, void *pos, s32 rot);
extern billHandle_t *dds3BillboardFileLoad(s32 kind, const char *path);
dds3Basic_t *AddTMXEffect(void *tmx, void *pos, s32 rot);
dds3Basic_t *AddBEDEffect();
void sdfMemFreeDirect( sdfMemHandle_t *aHandle);
effEveBEDCutHandle_t *effEveBEDCutCreate( effBTL_t *hEveBED, u16 CutNum, const void *pParams);
u32 dds3AllocObjectId(void);
dds3ObjectBaseControlWorkData_t *dds3EffectObjectBasicGetControlWorkData(dds3Basic_t *pBasic);
extern dds3Basic_t *dds3AreaBasicGetFastArea(void);
void dds3AreaBasicRegistItem(dds3Basic_t *pAreaBasic, dds3Basic_t *pItemBasic);
dds3Basic_t *AddMG12Effect();
extern void effMagaRandomize(void *hEffect);
extern s32 sdfModelGetMarkPosMatrix(sdfModelRootNode_t *aRoot, s32 aId);
void dds3BillboardSetXYOffset(struct billHandle_t *hBillboard, f32 x, f32 y);
void dds3BillboardSetDrawType(struct billHandle_t *hBillboard, s32 type);
void dds3BillboardSetMode(struct billHandle_t *hBillboard, s32 mode);

void FreeEffectData(dds3EffectObjectBasicWorkData_t *work) {
    dds3EffectObjectBasicWorkData_t *data = work;

    switch (data->type) {
    case 1:
        if (data->handle != NULL) {
            dds3EffectDelete(data->handle);
            data->handle = NULL;
        }
        break;
    case 2:
    case 3:
        if (data->handle != NULL) {
            dds3BillboardDelete(data->handle);
            data->handle = NULL;
        }
        break;
    case 4:
        if (data->handle != NULL) {
            data->handle = NULL;
        }
        break;
    case 5:
        if (data->cuth != NULL) {
            effEveBEDCutDelete(data->cuth);
            data->cuth = NULL;
        }
        if (data->handle != NULL) {
            effEveBEDDelete(data->handle);
            data->handle = NULL;
        }
        if (data->para != NULL) {
            sdfChipFreeDirect(data->para);
            data->para = NULL;
        }
        break;
    case 7:
    case 8:
        if (data->handle != NULL) {
            dds3EffectDelete(data->handle);
            data->handle = NULL;
        }
        if (data->cuth != NULL) {
            sdfChipFreeDirect(data->cuth);
            data->cuth = NULL;
        }
        if (data->para != NULL) {
            sdfChipFreeDirect(data->para);
            data->para = NULL;
        }
        break;
    }
    data->type = 0;
    data->handle = NULL;
}

s32 dds3EffectObjectBasicConstructor(dds3Basic_t *aPtr) {
    dds3EffectObjectBasicWorkData_t *data;

    dds3InitPoseSet(aPtr);
    aPtr->workdata = sdfChipAllocDirect(0x50);
    memset(aPtr->workdata, 0, 0x50);
    data = aPtr->workdata;
    data->pObjectBaseControlWorkData = dds3ObjectBaseInitControlWorkData(aPtr);
    dds3ObjectBaseOnFlag(aPtr, 0x60);
    return 1;
}

void dds3EffectObjectBasicDestructor(dds3Basic_t *aPtr) {
    dds3EffectObjectBasicWorkData_t *data;

    data = aPtr->workdata;
    FreeEffectData(data);
    dds3FreePoseSet(aPtr);
    dds3ObjectBaseFreeControlWorkData(data->pObjectBaseControlWorkData);
    sdfChipFreeDirect(aPtr->workdata);
    aPtr->workdata = NULL;
}

/* Resolve the owner transform, then forward it to the live effect backend. */
s32 dds3EffectObjectBasicMainProcedure(dds3Basic_t *aPtr)
{
    sdfMatrix4_t matrix;
    sdfVector4_t position;
    sdfVector4_t rotation = {0, 0, 0, 1.0f};
    dds3EffectObjectBasicWorkData_t *work;
    s32 moved = 0;
    effHandle_t *effect;
    billHandle_t *bill;
    s32 result;

    work = aPtr->workdata;
    if (work->stat & EFF_OBJ_FLAG_OWNER_LINK) {
        if (dds3SceneBasicExistBasicFromType(dds3GetActiveScene(), work->linkbasictype,
                                            work->linkbasic) != 0) {
            if (work->stat & EFF_OBJ_FLAG_OWNER_BILL_ENTRY) {
                dds3EffectObjectBasicGetObjectIndex(aPtr);
            } else {
                switch (work->linkbasictype) {
                case 4: case 5: case 6: case 7: case 8: case 9:
                    SDF_VU0_LOADVECTOR(&work->linkbasic->poseset->rot);
                    SDF_VU0_STOREVECTOR(&rotation);
                    ToMatrix();
                    break;
                case EFF_OBJ_OWNER_KIND_EXTRA:
                    SDF_VU0_LOADVECTOR(&((dds3EffectOwnerPose_t *)work->linkbasic->workdata)->rotation);
                    SDF_VU0_STOREVECTOR(&rotation);
                    ToMatrix();
                    break;
                default:
                    SDF_VU0_LOADIDENTITY();
                    break;
                }
            }
            SDF_VU0_COPYMATRIX_A_TO_B();
            SDF_VU0_LOADIDENTITY();
            SDF_VU0_LOADVECTOR(work->linkpos);
            SDF_VU0_VECTOR_W_SET_ONE();
            SDF_VU0_TRANSLATE();
            MultMatrixRevNoArg();
            SDF_VU0_LOADIDENTITYVECTOR();
            SDF_VU0_APPLYMATRIX();
            if ((work->stat & EFF_OBJ_FLAG_OWNER_BILL_ENTRY) == 0) {
                switch (work->linkbasictype) {
                case 4: case 5: case 6: case 7: case 8: case 9:
                    SDF_VU0_LOADVECTOR_B(&work->linkbasic->poseset->pos);
                    SDF_VU0_ADDVECTOR();
                    break;
                case EFF_OBJ_OWNER_KIND_EXTRA:
                    SDF_VU0_LOADVECTOR_B(&((dds3EffectOwnerPose_t *)work->linkbasic->workdata)->position);
                    SDF_VU0_ADDVECTOR();
                    break;
                }
            }
            SDF_VU0_STOREVECTOR(&position);
            dds3SetRadiusPoseSet(aPtr->poseset);
            SDF_VU0_STOREVECTOR(&aPtr->poseset->pos);
            moved = 1;
            dds3OffPoseSetFlag(aPtr->poseset, POSESET_FLAG_UPDATEPOSE);
        } else {
            dds3EffectObjectBasicUnlinkObject(aPtr);
        }
    } else {
        dds3OffPoseSetFlag(aPtr->poseset, POSESET_FLAG_UPDATEPOSE);
        SDF_VU0_LOADVECTOR(&aPtr->poseset->pos);
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_STOREVECTOR(&position);
        dds3SetRadiusPoseSet(aPtr->poseset);
        SDF_VU0_LOADVECTOR(&aPtr->poseset->rot);
        Normalize();
        SDF_VU0_STOREVECTOR(&rotation);
        moved = 1;
    }
    switch (work->type) {
    case EFF_OBJ_STATE_BOUND_BILL:
        effect = work->handle;
        if (effect != NULL && moved) {
            dds3EffectSetPosition(effect, &position);
            SDF_VU0_LOADVECTOR(&rotation);
            ToMatrix();
            SDF_VU0_STOREMATRIX(&matrix);
            dds3EffectSetRotateMatrix(effect, &matrix);
        }
        result = dds3EffectGetState(effect);
    expired:
        if (result == 0) {
            u32 flags = work->stat;

            work->stat = flags & ~1;
            if (flags & 2) {
                dds3DeleteSceneBasicData(aPtr);
            }
        }
        return 1;
    case 2:
        bill = work->handle;
        if (bill != NULL && moved) {
            dds3BillboardSetPosition(bill, &position);
        }
        result = dds3BillboardGetAnimState(bill);
        goto expired;
    case 3:
        if (work->handle != NULL && moved) {
            dds3BillboardSetPosition(work->handle, &position);
        }
        return 1;
    case 4:
        return 1;
    case EFF_OBJ_STATE_EVENT_NODE: {
        void *cut = work->cuth;

        if (cut != NULL) {
            if (moved) {
                dds3EffectEventParameters_t *parameters = work->para;

                if (parameters != NULL) {
                    SDF_VU0_COPYVECTOR(&parameters->position, &position);
                    SDF_VU0_COPYVECTOR(&parameters->rotation, &rotation);
                    effEveBEDCutSetParams(cut, parameters);
                }
            }
        }
        return 1;
    }
    }
    return 1;
}

s32 dds3EffectObjectBasicDrawProcedure(dds3Basic_t *aPtr) {
    dds3EffectObjectBasicWorkData_t *data;

    data = aPtr->workdata;
    dds3EffectObjectBasicCalcFldEx((dds3Basic_t *)aPtr);
    if ((data->stat & 1) == 0) {
        return 1;
    }
    if (dds3ObjectBaseCheckFlag(aPtr, 1)) {
        return 1;
    }

    switch (data->type) {
    case 1:
    case 7:
    case 8:
        if (data->handle != NULL) {
            dds3EffectDraw(data->handle);
        }
        break;
    case 2:
    case 3:
        if (data->handle != NULL) {
            dds3BillboardDraw(data->handle);
        }
        break;
    case 5:
        if (data->cuth != NULL) {
            effEveBEDCutDraw(data->cuth);
        }
        break;
    }
    return 1;
}

dds3ObjectBaseControlWorkData_t *dds3EffectObjectBasicGetControlWorkData(dds3Basic_t *pBasic)
{
    return ((dds3EffectObjectBasicWorkData_t *)pBasic->workdata)->pObjectBaseControlWorkData;
}

dds3Basic_t *dds3AddEffectObjectBasic(u32 serialID, void *aPos, void *aRot) {
    dds3Basic_t *obj;
    dds3EffectObjectBasicWorkData_t *data;

    obj = (dds3Basic_t *)dds3AddSceneBasicData(EFF_WORLD_KIND_EFFECT_OBJECT);
    if (obj == NULL) {
        return NULL;
    }
    obj->serialID = serialID;
    dds3ObjectBaseCreateMover(obj);
    dds3SetPosPoseSet(obj, aPos);
    dds3SetRotPoseSet(obj, aRot);
    dds3SetRadiusPoseSet(obj->poseset);
    data = obj->workdata;
    data->stat = 0;
    data->type = 0;
    data->handle = NULL;
    data->linkbasic = NULL;
    data->linkindex = 0;
    data->linkbasictype = 0;
    return obj;
}

dds3Basic_t *AddP2AEffect(void *p2a, void *pos, s32 rot)
{
    sdfVector4_t vector;
    dds3Basic_t *basic;
    dds3EffectObjectBasicWorkData_t *work;
    dds3ObjectBaseControlWorkData_t *control;
    dds3Basic_t *area;

    memset(&vector, 0, sizeof(vector));
    basic = dds3AddEffectObjectBasic(dds3AllocObjectId(), pos, (void *)rot);
    if (basic == NULL) {
        return NULL;
    }
    SDF_VU0_LOADVECTOR(pos);
    SDF_VU0_STOREVECTOR(&vector);
    dds3BillboardSetPosition(p2a, &vector);
    work = basic->workdata;
    work->handle = p2a;
    work->stat = 0;
    work->type = 2;
    work->linkbasic = NULL;
    work->linkindex = 0;
    work->linkbasictype = 0;
    control = dds3EffectObjectBasicGetControlWorkData(basic);
    control->modelType = 2;
    area = dds3AreaBasicGetFastArea();
    if (area != NULL) {
        control->pBasic[5] = area;
        dds3AreaBasicRegistItem(area, basic);
    }
    return basic;
}

dds3Basic_t *dds3AddP2AEffectObjectBasic3(dds3Basic_t *ebasic, void *pos, s32 rot) {
    struct billHandle_t *bill;

    bill = dds3BillboardCopy((struct billHandle_t *)((dds3EffectObjectBasicWorkData_t *)ebasic->workdata)->handle);
    return AddP2AEffect(bill, pos, rot);
}

void dds3AddP2AEffectObjectBasic2(u32 effdata, void *pos, s32 rot) {
    struct billHandle_t *bill;

    bill = dds3BillboardCreate(1, effdata);
    AddP2AEffect(bill, pos, rot);
}

dds3Basic_t *dds3AddP2AEffectObjectBasic(const char *path, void *firstVector, s32 secondVectorAddress) {
    void *bill;

    bill = dds3BillboardFileLoad(1, path);
    return AddP2AEffect(bill, firstVector, secondVectorAddress);
}

void dds3EffectObjectBasicSetAnmeID(dds3Basic_t *basic, u32 animeID)
{
    dds3BillboardSetAnimNumber(((dds3EffectObjectBasicWorkData_t *)basic->workdata)->handle, animeID);
}

dds3Basic_t *AddTMXEffect(void *tmx, void *pos, s32 rot)
{
    sdfVector4_t vector;
    dds3Basic_t *basic;
    dds3EffectObjectBasicWorkData_t *work;
    dds3ObjectBaseControlWorkData_t *control;
    dds3Basic_t *area;

    memset(&vector, 0, sizeof(vector));
    basic = dds3AddEffectObjectBasic(dds3AllocObjectId(), pos, (void *)rot);
    if (basic == NULL) {
        return NULL;
    }
    SDF_VU0_LOADVECTOR(pos);
    SDF_VU0_STOREVECTOR(&vector);
    dds3BillboardSetPosition(tmx, &vector);
    work = basic->workdata;
    work->type = 3;
    work->handle = tmx;
    work->stat = 0;
    work->linkbasic = NULL;
    work->linkindex = 0;
    work->linkbasictype = 0;
    control = dds3EffectObjectBasicGetControlWorkData(basic);
    control->modelType = 2;
    area = dds3AreaBasicGetFastArea();
    if (area != NULL) {
        control->pBasic[5] = area;
        dds3AreaBasicRegistItem(area, basic);
    }
    return basic;
}

dds3Basic_t *dds3AddTMXEffectObjectBasic3(dds3Basic_t *ebasic, void *pos, s32 rot) {
    struct billHandle_t *bill;

    bill = dds3BillboardCopy((struct billHandle_t *)((dds3EffectObjectBasicWorkData_t *)ebasic->workdata)->handle);
    return AddTMXEffect(bill, pos, rot);
}

void dds3AddTMXEffectObjectBasic2(u32 effdata, void *pos, s32 rot)
{
    struct billHandle_t *bill;

    bill = dds3BillboardCreate(0, effdata);
    AddTMXEffect(bill, pos, rot);
}

dds3Basic_t *dds3AddTMXEffectObjectBasic(const char *path, void *firstVector, s32 secondVectorAddress)
{
    void *bill;

    printf("LoadFile:%s\n", path);
    bill = dds3BillboardFileLoad(0, path);
    return AddTMXEffect(bill, firstVector, secondVectorAddress);
}

dds3Basic_t *AddD3PEffect(void *eff, void *pos, void *rot)
{
    sdfVector4_t vector;
    dds3Basic_t *basic;
    dds3EffectObjectBasicWorkData_t *work;
    dds3ObjectBaseControlWorkData_t *control;
    dds3Basic_t *area;

    memset(&vector, 0, sizeof(vector));
    basic = dds3AddEffectObjectBasic(dds3AllocObjectId(), pos, rot);
    if (basic == NULL) {
        return NULL;
    }
    SDF_VU0_LOADVECTOR(pos);
    SDF_VU0_STOREVECTOR(&vector);
    dds3EffectSetPosition(eff, &vector);
    work = basic->workdata;
    work->type = EFF_OBJ_STATE_BOUND_BILL;
    work->handle = eff;
    work->stat = 0;
    work->linkbasic = NULL;
    work->linkindex = 0;
    work->linkbasictype = 0;
    control = dds3EffectObjectBasicGetControlWorkData(basic);
    control->modelType = 2;
    area = dds3AreaBasicGetFastArea();
    if (area != NULL) {
        control->pBasic[5] = area;
        dds3AreaBasicRegistItem(area, basic);
    }
    return basic;
}

dds3Basic_t *dds3AddD3PEffectObjectBasic2(void *effdata, void *pos, void *rot)
{
    effHandle_t *effect;

    effect = dds3EffectLoad(effdata);
    return AddD3PEffect(effect, pos, rot);
}

dds3Basic_t *dds3AddD3PEffectObjectBasic(u32 file, void *pos, void *rot)
{
    effHandle_t *effect;

    effect = dds3EffectFileLoad(file);
    return AddD3PEffect(effect, pos, rot);
}

dds3Basic_t *AddBEDEffect(source, position, rotation)
    effBTL_t *source;
    const sdfVector4_t *position;
    const sdfVector4_t *rotation;
{
    dds3EffectEventParameters_t parameters = D_00530E20;
    dds3Basic_t *basic;
    effBTL_t *mixer;
    dds3EffectObjectBasicWorkData_t *work;
    dds3ObjectBaseControlWorkData_t *control;
    dds3Basic_t *area;

    basic = dds3AddEffectObjectBasic(dds3AllocObjectId(), (void *)position, (void *)rotation);
    if (basic == NULL) {
        return NULL;
    }
    SDF_VU0_COPYVECTOR(&parameters.position, position);
    SDF_VU0_COPYVECTOR(&parameters.rotation, rotation);
    mixer = effEveBEDCreate(source);
    work = basic->workdata;
    work->type = EFF_OBJ_STATE_EVENT_NODE;
    work->handle = mixer;
    work->stat = 0;
    work->cuth = NULL;
    work->linkbasic = NULL;
    work->linkindex = 0;
    work->linkbasictype = 0;
    work->para = sdfChipAllocDirect(sizeof(parameters));
    memcpy(work->para, &parameters, sizeof(parameters));
    control = dds3EffectObjectBasicGetControlWorkData(basic);
    control->modelType = 2;
    area = dds3AreaBasicGetFastArea();
    if (area != NULL) {
        control->pBasic[5] = area;
        dds3AreaBasicRegistItem(area, basic);
    }
    return basic;
}

void *dds3AddBEDEffectObjectBasic2(void *data, void *pos, void *rot)
{
    return AddBEDEffect(data, pos, rot);
}

void *dds3AddBEDEffectObjectBasic(void *file, void *pos, void *rot) {
    u32 resolvedId;
    void *resourceHandle;
    void *created;

    resolvedId = 0;
    resourceHandle = sdfDevLoadFileEx(file, &resolvedId, 0);
    created = AddBEDEffect((effBTL_t *)resolvedId, pos, rot);
    sdfMemFreeDirect(resourceHandle);
    return created;
}

void dds3StartBEDEffectObjectBasic(dds3Basic_t *basic, u32 cutno) {
    dds3EffectObjectBasicWorkData_t *data;

    data = basic->workdata;
    if (data->type != EFF_OBJ_STATE_EVENT_NODE) {
        return;
    }
    if (data->para == NULL) {
        return;
    }
    if (data->handle == NULL) {
        return;
    }
    if (data->cuth != NULL) {
        effEveBEDCutDelete(data->cuth);
        data->cuth = NULL;
    }
    data->cuth = effEveBEDCutCreate(data->handle, (u16)(cutno & EFF_OBJ_ENTRY_ID_MASK), data->para);
}

dds3Basic_t *AddESPEffect(const char *name, void *pos, void *rot, f32 r) {
    dds3Basic_t *obj;
    dds3EffectObjectBasicWorkData_t *data;
    dds3ObjectBaseControlWorkData_t *objectHandle;
    dds3Basic_t *worldNode;

    obj = dds3AddEffectObjectBasic(dds3AllocObjectId(), pos, rot);
    if (obj == NULL) {
        return NULL;
    }
    data = obj->workdata;
    data->type = EFF_OBJ_STATE_PARAMETERS_READY;
    obj->poseset->scl.x = r;
    obj->poseset->scl.y = r;
    obj->poseset->scl.z = r;
    data->stat = 0;
    data->handle = NULL;
    data->cuth = NULL;
    data->linkbasic = NULL;
    data->linkindex = 0;
    data->linkbasictype = 0;
    obj->name = name;
    objectHandle = dds3EffectObjectBasicGetControlWorkData((dds3Basic_t *)obj);
    objectHandle->modelType = 2;
    worldNode = dds3AreaBasicGetFastArea();
    if (worldNode != NULL) {
        objectHandle->pBasic[5] = worldNode;
        dds3AreaBasicRegistItem(worldNode, obj);
    }
    return obj;
}

dds3Basic_t *dds3AddESPEffectObjectBasic(const char *name, void *pos, void *rot, f32 r)
{
    return AddESPEffect(name, pos, rot, r);
}

/* Keep the legacy positional (kind, descriptor) ABI used by the file loader. */
dds3Basic_t *AddMG12Effect(kind, descriptor)
    s32 kind;
    void *descriptor;
{
    sdfVector4_t position;
    sdfVector4_t rotation;
    effMagaSCArea_t areaParameters;
    effMagaP4Point_t pointParameters;
    effHandle_t *effect;
    dds3Basic_t *basic;
    dds3EffectObjectBasicWorkData_t *work;
    dds3ObjectBaseControlWorkData_t *control;
    dds3Basic_t *area;

    memset(&position, 0, sizeof(position));
    memset(&rotation, 0, sizeof(rotation));
    rotation.w = 1.0f;
    memcpy(&areaParameters, &D_00530E50, sizeof(areaParameters));
    memcpy(&pointParameters, &D_00530E78, sizeof(pointParameters));
    effect = dds3EffectLoad(descriptor);
    basic = dds3AddEffectObjectBasic(dds3AllocObjectId(), &position, &rotation);
    if (basic == NULL) {
        return NULL;
    }
    work = basic->workdata;
    work->stat = 0;
    if (kind == 1) {
        work->type = EFF_OBJ_STATE_MAGATUHI_TWO_ROWS;
    } else {
        work->type = EFF_OBJ_STATE_MAGATUHI_FOUR_ROWS;
    }
    work->handle = effect;
    work->linkbasic = NULL;
    work->linkindex = 0;
    work->linkbasictype = 0;
    work->cuth = sdfChipAllocDirect(0x10);
    memset(work->cuth, 0, 0x10);
    switch (kind) {
    case 1:
        work->para = sdfChipAllocDirect(sizeof(areaParameters));
        memcpy(work->para, &areaParameters, sizeof(areaParameters));
        effMagaSCSetArea(effect, work->para);
        break;
    case 2:
        work->para = sdfChipAllocDirect(sizeof(pointParameters));
        memcpy(work->para, &pointParameters, sizeof(pointParameters));
        effMagaP4SetPoint(effect, work->para);
        break;
    }
    control = dds3EffectObjectBasicGetControlWorkData(basic);
    control->modelType = 2;
    area = dds3AreaBasicGetFastArea();
    if (area != NULL) {
        control->pBasic[5] = area;
        dds3AreaBasicRegistItem(area, basic);
    }
    return basic;
}

dds3Basic_t *dds3AddMG12EffectObjectBasic2(s32 type, void *effdata)
{
    return AddMG12Effect(type, effdata);
}

void *dds3AddMG12EffectObjectBasic(s32 type, void *file) {
    u32 resolvedId;
    void *resourceHandle;
    void *created;

    resolvedId = 0;
    resourceHandle = sdfDevLoadFileEx(file, &resolvedId, 0);
    created = AddMG12Effect(type, resolvedId);
    sdfMemFreeDirect(resourceHandle);
    return created;
}

dds3EffectObjectBasicWorkData_t *CheckESP(dds3Basic_t *b) {
    dds3EffectObjectBasicWorkData_t *data;

    data = NULL;
    if (b == NULL) {
        return data;
    }
    if (((u8 *)&b->type)[3] != EFF_OBJ_KIND) {
        return data;
    }
    data = b->workdata;
    if (data->type != EFF_OBJ_STATE_PARAMETERS_READY) {
        data = NULL;
    }
    return data;
}

s32 GetESPPos(dds3Basic_t *b)
{
    if (CheckESP(b) == NULL) {
        return 0;
    }
    SDF_VU0_LOADVECTOR(&b->poseset->pos);
    return 1;
}

s32 GetESPR(dds3Basic_t *b) {
    dds3PoseSet_t *parameters;

    if (CheckESP(b) == NULL) {
        return 0;
    }
    parameters = b->poseset;
    return (s32)parameters->scl.x;
}

s32 dds3EffectObjectPresetMG12(dds3Basic_t *eff) {
    if (((u8 *)&eff->type)[3] == EFF_OBJ_KIND) {
        dds3EffectObjectBasicWorkData_t *data = eff->workdata;

        switch (data->type) {
        case EFF_OBJ_STATE_MAGATUHI_TWO_ROWS:
            break;
        case EFF_OBJ_STATE_MAGATUHI_FOUR_ROWS:
            effMagaP4Randomize(data->handle);
            break;
        case EFF_OBJ_STATE_BOUND_BILL:
            effMagaRandomize(data->handle);
            break;
        }
    }
}

/* Missing parameter sources leave their existing position and radius intact. */
s32 dds3EffectObjectSetMG12Pos(dds3Basic_t *eff, dds3Basic_t *p1,
                              dds3Basic_t *p2, dds3Basic_t *p3, dds3Basic_t *p4)
{
    dds3EffectObjectBasicWorkData_t *work;
    effMagaSCArea_t *area;
    effMagaP4Point_t *points;

    if (eff->type >> 24 != EFF_OBJ_KIND) {
        return 0;
    }
    work = eff->workdata;
    switch (work->type) {
    case EFF_OBJ_STATE_MAGATUHI_TWO_ROWS:
        area = work->para;
        if (GetESPPos(p1)) {
            SDF_VU0_STOREVECTOR(&area->position[0]);
            area->radius[0] = GetESPR(p1);
        }
        if (GetESPPos(p2)) {
            SDF_VU0_STOREVECTOR(&area->position[1]);
            area->radius[1] = GetESPR(p2);
        }
        effMagaSCSetArea(work->handle, area);
        return 1;
    case EFF_OBJ_STATE_MAGATUHI_FOUR_ROWS:
        points = work->para;
        if (GetESPPos(p1)) {
            SDF_VU0_STOREVECTOR(&points->position[0]);
            points->radius[0] = GetESPR(p1);
        }
        if (GetESPPos(p2)) {
            SDF_VU0_STOREVECTOR(&points->position[1]);
            points->radius[1] = GetESPR(p2);
        }
        if (GetESPPos(p3)) {
            SDF_VU0_STOREVECTOR(&points->position[2]);
            points->radius[2] = GetESPR(p3);
        }
        if (GetESPPos(p4)) {
            SDF_VU0_STOREVECTOR(&points->position[3]);
            points->radius[3] = GetESPR(p4);
        }
        effMagaP4SetPoint(work->handle, points);
        return 1;
    }
    return 0;
}

void dds3EffectObjectBasicStatOn(dds3Basic_t *basic, s32 s) {
    ((dds3EffectObjectBasicWorkData_t *)basic->workdata)->stat |= s;
}

void dds3EffectObjectBasicStatOff(dds3Basic_t *basic, s32 s) {
    ((dds3EffectObjectBasicWorkData_t *)basic->workdata)->stat &= ~s;
}

s32 dds3EffectObjectBasicLinkObject(dds3Basic_t *basic, dds3Basic_t *target) {
    dds3EffectObjectBasicWorkData_t *data;
    u8 kind;

    kind = ((u8 *)&target->type)[3];
    if (kind < EFF_OBJ_OWNER_KIND_FIRST || (kind >= EFF_OBJ_OWNER_KIND_END && kind != EFF_OBJ_OWNER_KIND_EXTRA)) {
        return 0;
    }
    data = basic->workdata;
    data->linkpos[2] = 0;
    data->linkbasic = target;
    data->linkindex = 0;
    data->stat |= EFF_OBJ_FLAG_OWNER_LINK;
    data->stat &= ~EFF_OBJ_FLAG_OWNER_BILL_ENTRY;
    data->linkpos[0] = 0;
    data->linkbasictype = ((u8 *)&target->type)[3];
    data->linkpos[1] = 0;
    return 1;
}

void dds3EffectObjectBasicGetObjectIndex(dds3Basic_t *basic) {
    dds3EffectObjectBasicWorkData_t *data;
    dds3Basic_t *owner;
    EffOwnerBillPayloadView *bill;

    data = basic->workdata;
    if (data->stat & EFF_OBJ_FLAG_OWNER_BILL_ENTRY) {
        owner = data->linkbasic;
        if (((u8 *)&owner->type)[3] == EFF_OBJ_OWNER_BILL_KIND) {
            bill = (EffOwnerBillPayloadView *)((EffectObjectData *)owner->workdata)->modelHolder;
            if (bill->state != 0) {
                return;
            }
            sdfModelGetMarkPosMatrix(((EffOwnerBillParamView *)bill->params)->model, data->linkindex);
        }
    }
}

s32 dds3EffectObjectBasicLinkObjectIndex(dds3Basic_t *basic, dds3Basic_t *target, s32 index) {
    dds3EffectObjectBasicWorkData_t *data;

    if (dds3EffectObjectBasicLinkObject(basic, target) == 0) {
        return 1;
    }
    data = basic->workdata;
    data->linkindex = index;
    data->stat |= EFF_OBJ_FLAG_OWNER_BILL_ENTRY;
    return 1;
}

void dds3EffectObjectBasicUnlinkObject(dds3Basic_t *basic)
{
    dds3EffectObjectBasicWorkData_t *work = basic->workdata;

    work->linkindex = 0;
    work->linkbasic = NULL;
    work->linkbasictype = 0;
    work->stat &= ~EFF_OBJ_FLAG_OWNER_LINK;
    work->stat &= ~EFF_OBJ_FLAG_OWNER_BILL_ENTRY;
}

void dds3EffectObjectBasicSetFldEx(dds3Basic_t *basic, fldResourceEffect_t *pResEff) {
    dds3EffectObjectBasicWorkData_t *resource = basic->workdata;
    dds3PoseSet_t *source = basic->poseset;

    resource->pFldEx = pResEff;
    resource->FldExBasePos.x = source->pos.x;
    resource->FldExBasePos.y = source->pos.y;
    resource->FldExBasePos.z = source->pos.z;
    resource->FldExBasePos.w = source->pos.w;
    dds3EffectObjectBasicStatOn((struct dds3Basic_t *)basic, 0x10);
    switch (resource->pFldEx->type) {
    case 0:
    case 3:
        dds3BillboardSetXYOffset((struct billHandle_t *)resource->handle, resource->pFldEx->size[0], resource->pFldEx->size[1]);
        if (resource->pFldEx->type == 3) {
            dds3BillboardSetDrawType(resource->handle, 1);
        }
        if (resource->pFldEx->flag & 1) {
            dds3BillboardSetMode(resource->handle, 2);
            return;
        }
        if (resource->pFldEx->flag & 2) {
            dds3BillboardSetMode(resource->handle, 3);
            return;
        }
        break;
    case 2:
        break;
    case 1:
        dds3BillboardSetAnimNumber(resource->handle, resource->pFldEx->param0);
        break;
    }
}

/* Apply field visibility/animation selectors and the view-distance fade. */
void dds3EffectObjectBasicCalcFldEx(dds3Basic_t *basic)
{
    dds3EffectObjectBasicWorkData_t *work = basic->workdata;
    fldResourceEffect_t *config;
    sdfVector4_t direction __attribute__((aligned(16)));
    sdfVector4_t normalizedDirection __attribute__((aligned(16)));
    f32 dx;
    f32 dy;
    f32 offset; /* Z delta, then projection distance. */
    f32 distance;
    u32 projectionDistance;

    if ((work->stat & 0x10) == 0) {
        return;
    }
    config = work->pFldEx;
    if (config->groupID != 0) {
        s32 map = fldGlobalBase.area;
        u32 floor = fldGlobalBase.floor + 1;

        if (fldGetModelFlag2(map, floor, config->groupID)) {
            work->stat &= ~1;
        } else {
            work->stat |= 1;
        }
    }
    dx = aViewEye.x - work->FldExBasePos.x;
    dy = aViewEye.y - work->FldExBasePos.y;
    offset = aViewEye.z - work->FldExBasePos.z;
    projectionDistance = config->offsetZ;
    {
        f32 squaredDistance = dx * dx + dy * dy + offset * offset;
        distance = sdfSqrt(squaredDistance);
    }
    if (projectionDistance != 0) {
        direction.x = -dx;
        direction.y = -dy;
        direction.z = -offset;
        direction.w = 1.0f;
        SDF_VU0_LOADVECTOR(&direction);
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR(&normalizedDirection);
        {
            dds3PoseSet_t *pose;

            offset = (f32)projectionDistance;
            pose = basic->poseset;
            pose->pos.x = work->FldExBasePos.x - normalizedDirection.x * offset;
            pose->pos.y = work->FldExBasePos.y - normalizedDirection.y * offset;
            pose->pos.z = work->FldExBasePos.z - normalizedDirection.z * offset;
        }
    }
    switch (config->type) {
    case 0:
    case 3:
        if (config->param0 == 0 && config->param1 == 0) {
            return;
        }
        {
            f32 start = (f32)config->param0;
            f32 end = (f32)config->param1;
            s32 alpha;
            u32 color;

            if (distance < start) {
                color = 0x00808080;
            } else if (end < distance) {
                color = 0x80808080;
            } else {
                alpha = (s32)(((distance - start) * 128.0f) / (end - start));
                if (alpha >= 129) {
                    alpha = 128;
                }
                if (alpha < 0) {
                    alpha = 0;
                }
                color = ((u32)alpha << 24) | 0x00808080;
            }
            dds3BillboardSetRGBA(work->handle, color);
        }
        break;
    case 2:
        break;
    case 1:
        if (config->groupID != 0) {
            s32 entry = fldGetAnimeId2(fldGlobalBase.area,
                                                fldGlobalBase.floor + 1,
                                                config->groupID);

            if (entry != -1) {
                dds3BillboardSetAnimNumber(work->handle, (u32)entry);
            }
        }
        break;
    default:
        break;
    }
}

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00530E20);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00530E50);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00530E78);

