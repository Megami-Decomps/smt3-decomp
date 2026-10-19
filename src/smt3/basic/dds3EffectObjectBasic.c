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
} dds3MagaArea_t;

typedef struct
{
    sdfVector4_t position[4];
    f32 radius[4];
} dds3MagaPoints_t;

extern const dds3EffectEventParameters_t D_00530E20;
extern const dds3MagaArea_t D_00530E50;
extern const dds3MagaPoints_t D_00530E78;
extern void *memcpy(void *dst, const void *src, u32 size);
extern void dds3BillboardSetPosition(void *bill, const void *position);
extern void dds3BillboardSetAnimNumber(void *bill, u32 entry);
extern void dds3EffectSetPosition(void *effect, const void *position);
extern struct EffNode *dds3EffectLoad(void *descriptor);
extern struct EffNode *dds3EffectFileLoad(u32 parameter);
extern  effBTL_t *effEveBEDCreate( effBTL_t *source);
extern void effMagaSCSetArea(void *effect, const void *parameters);
extern void effMagaP4SetPoint(void *effect, const void *parameters);

extern sdfVector4_t aViewEye;
extern s32 dds3EffectObjectAddFld(s32 map, u32 floor, s32 group);
extern s32 dds3EffectObjectFindFld(s32 map, u32 floor, s32 group);
extern void dds3BillboardSetRGBA(void *bill, u32 color);

typedef struct
{
    sdfVector4_t position;
    sdfVector4_t rotation;
} dds3EffectOwnerPose_t;

extern dds3Basic_t *dds3GetActiveScene(void);
extern s32 dds3SceneBasicExistBasicFromType(dds3Basic_t *scene, s32 type, dds3Basic_t *basic);
extern void dds3EffectObjectBasicGetObjectIndex(dds3Basic_t *basic);
extern void dds3EffectObjectBasicUnlinkObject(dds3Basic_t *basic);
extern void dds3DeleteSceneBasicData(dds3Basic_t *basic);
extern void dds3OffPoseSetFlag(dds3PoseSet_t *pose, u32 mask);
extern void ToMatrix(void);
extern void MultMatrixRevNoArg(void);
extern void Normalize(void);
extern void dds3EffectSetRotateMatrix(void *effect, const sdfMatrix4_t *matrix);
extern s32 dds3EffectGetState(void *effect);
extern s32 dds3BillboardGetAnimState(void *bill);
extern void effEveBEDCutSetParams(void *cut, const void *parameters);


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


struct billObj_t *dds3BillboardCopy(struct billObj_t *bill);
struct billObj_t *dds3BillboardCreate(s32 kind, u32 id);
void effMagaP4Randomize(void *maga);
void *memset(void *dst, s32 fill, u32 len);
void *sdfDevLoadFileEx(void *resource, u32 *id, s32 mode);


struct BillChildPayload;
typedef struct BillChildPayload BillChildPayload;
struct BillData;
typedef struct BillData BillData;
struct BillOut;
typedef struct BillOut BillOut;
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

typedef struct EffEventWork EffEventWork;

typedef struct BattleEffect BattleEffect;

typedef void (*FileRequestCallback)(void *request, u32 arg);

typedef struct EffNode {
    u32 type;
    s32 arg;
    s32 instance;
    f32 unkC;
} EffNode;

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

void dds3EffectDelete(EffNode *node);
void dds3BillboardDelete(struct billObj_t *obj);
void effEveBEDCutDelete(EffEventWork *work);
void effEveBEDDelete( effBTL_t *mixer);
void sdfChipFreeDirect(void *memory);
s32 dds3InitPoseSet(dds3Basic_t *node);
void *sdfChipAllocDirect(u32 size);
extern void dds3ObjectBaseOnFlag(dds3Basic_t *basic, s32 mask);
extern u8 opaque40[0x60];
void dds3FreePoseSet(dds3Basic_t *node);
void dds3EffectObjectBasicCalcFldEx(dds3Basic_t *object);
u8 dds3ObjectBaseCheckFlag(struct dds3Basic_t *object, u32 mask);
void dds3EffectDraw(EffNode *node);
extern void dds3BillboardDraw(struct billObj_t *bill);
void effEveBEDCutDraw(EffEventWork *work);
dds3Basic_t *dds3AddSceneBasicData(s32 kind);
void dds3ObjectBaseCreateMover(dds3Basic_t *object);
void dds3SetPosPoseSet(dds3Basic_t *object, const sdfVector4_t *vector);
void dds3SetRotPoseSet(dds3Basic_t *object, const sdfVector4_t *vector);
extern void dds3SetRadiusPoseSet(dds3PoseSet_t *node);
dds3Basic_t *AddP2AEffect(void *bill, void *vec, s32 extra);
extern billObj_t *dds3BillboardFileLoad(s32 kind, const char *path);
dds3Basic_t *AddTMXEffect(void *bill, void *vec, s32 extra);
dds3Basic_t *AddBEDEffect();
void sdfMemFreeDirect( sdfMemHandle_t *allocation);
EffEventWork *effEveBEDCutCreate( effBTL_t *owner, u16 kind, const void *params);
u32 dds3AllocObjectId(void);
dds3ObjectBaseControlWorkData_t *dds3EffectObjectBasicGetControlWorkData(dds3Basic_t *object);
extern dds3Basic_t *dds3AreaBasicGetFastArea(void);
void dds3AreaBasicRegistItem(dds3Basic_t *indexOwner, dds3Basic_t *node);
dds3Basic_t *AddMG12Effect();
extern void effMagaRandomize(void *bill);
extern s32 sdfModelGetMarkPosMatrix(sdfModelRootNode_t *model, s32 id);
void dds3BillboardSetXYOffset(struct billObj_t *billboard, f32 width, f32 height);
void dds3BillboardSetDrawType(struct billObj_t *billboard, s32 variantValue);
void dds3BillboardSetMode(struct billObj_t *billboard, s32 mode);

void FreeEffectData(dds3EffectObjectBasicWorkData_t *state) {
    dds3EffectObjectBasicWorkData_t *data = state;

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

s32 dds3EffectObjectBasicConstructor(dds3Basic_t *obj) {
    dds3EffectObjectBasicWorkData_t *data;

    dds3InitPoseSet(obj);
    obj->workdata = sdfChipAllocDirect(0x50);
    memset(obj->workdata, 0, 0x50);
    data = obj->workdata;
    data->pObjectBaseControlWorkData = dds3ObjectBaseInitControlWorkData(obj);
    dds3ObjectBaseOnFlag(obj, 0x60);
    return 1;
}

void dds3EffectObjectBasicDestructor(dds3Basic_t *obj) {
    dds3EffectObjectBasicWorkData_t *data;

    data = obj->workdata;
    FreeEffectData(data);
    dds3FreePoseSet(obj);
    dds3ObjectBaseFreeControlWorkData(data->pObjectBaseControlWorkData);
    sdfChipFreeDirect(obj->workdata);
    obj->workdata = NULL;
}

/* Resolve the owner transform, then forward it to the live effect backend. */
s32 dds3EffectObjectBasicMainProcedure(dds3Basic_t *basic)
{
    sdfMatrix4_t matrix;
    sdfVector4_t position;
    sdfVector4_t rotation = {0, 0, 0, 1.0f};
    dds3EffectObjectBasicWorkData_t *work;
    s32 moved = 0;
    EffNode *effect;
    billObj_t *bill;
    s32 result;

    work = basic->workdata;
    if (work->stat & EFF_OBJ_FLAG_OWNER_LINK) {
        if (dds3SceneBasicExistBasicFromType(dds3GetActiveScene(), work->linkbasictype,
                                            work->linkbasic) != 0) {
            if (work->stat & EFF_OBJ_FLAG_OWNER_BILL_ENTRY) {
                dds3EffectObjectBasicGetObjectIndex(basic);
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
            dds3SetRadiusPoseSet(basic->poseset);
            SDF_VU0_STOREVECTOR(&basic->poseset->pos);
            moved = 1;
            dds3OffPoseSetFlag(basic->poseset, POSESET_FLAG_UPDATEPOSE);
        } else {
            dds3EffectObjectBasicUnlinkObject(basic);
        }
    } else {
        dds3OffPoseSetFlag(basic->poseset, POSESET_FLAG_UPDATEPOSE);
        SDF_VU0_LOADVECTOR(&basic->poseset->pos);
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_STOREVECTOR(&position);
        dds3SetRadiusPoseSet(basic->poseset);
        SDF_VU0_LOADVECTOR(&basic->poseset->rot);
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
                dds3DeleteSceneBasicData(basic);
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

s32 dds3EffectObjectBasicDrawProcedure(dds3Basic_t *obj) {
    dds3EffectObjectBasicWorkData_t *data;

    data = obj->workdata;
    dds3EffectObjectBasicCalcFldEx((dds3Basic_t *)obj);
    if ((data->stat & 1) == 0) {
        return 1;
    }
    if (dds3ObjectBaseCheckFlag(obj, 1)) {
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

dds3ObjectBaseControlWorkData_t *dds3EffectObjectBasicGetControlWorkData(dds3Basic_t *object)
{
    return ((dds3EffectObjectBasicWorkData_t *)object->workdata)->pObjectBaseControlWorkData;
}

dds3Basic_t *dds3AddEffectObjectBasic(u32 worldCounter, void *firstVec, void *secondVec) {
    dds3Basic_t *obj;
    dds3EffectObjectBasicWorkData_t *data;

    obj = (dds3Basic_t *)dds3AddSceneBasicData(EFF_WORLD_KIND_EFFECT_OBJECT);
    if (obj == NULL) {
        return NULL;
    }
    obj->serialID = worldCounter;
    dds3ObjectBaseCreateMover(obj);
    dds3SetPosPoseSet(obj, firstVec);
    dds3SetRotPoseSet(obj, secondVec);
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

dds3Basic_t *AddP2AEffect(void *bill, void *position, s32 rotationAddress)
{
    sdfVector4_t vector;
    dds3Basic_t *basic;
    dds3EffectObjectBasicWorkData_t *work;
    dds3ObjectBaseControlWorkData_t *control;
    dds3Basic_t *area;

    memset(&vector, 0, sizeof(vector));
    basic = dds3AddEffectObjectBasic(dds3AllocObjectId(), position, (void *)rotationAddress);
    if (basic == NULL) {
        return NULL;
    }
    SDF_VU0_LOADVECTOR(position);
    SDF_VU0_STOREVECTOR(&vector);
    dds3BillboardSetPosition(bill, &vector);
    work = basic->workdata;
    work->handle = bill;
    work->stat = 0;
    work->type = 2;
    work->linkbasic = NULL;
    work->linkindex = 0;
    work->linkbasictype = 0;
    control = dds3EffectObjectBasicGetControlWorkData(basic);
    control->modelType = 2;
    area = dds3AreaBasicGetFastArea();
    if (area != NULL) {
        control->control[5] = area;
        dds3AreaBasicRegistItem(area, basic);
    }
    return basic;
}

dds3Basic_t *dds3AddP2AEffectObjectBasic3(dds3Basic_t *obj, void *firstVector, s32 secondVectorAddress) {
    struct billObj_t *bill;

    bill = dds3BillboardCopy((struct billObj_t *)((dds3EffectObjectBasicWorkData_t *)obj->workdata)->handle);
    return AddP2AEffect(bill, firstVector, secondVectorAddress);
}

void dds3AddP2AEffectObjectBasic2(u32 billId, void *firstVector, s32 secondVectorAddress) {
    struct billObj_t *bill;

    bill = dds3BillboardCreate(1, billId);
    AddP2AEffect(bill, firstVector, secondVectorAddress);
}

dds3Basic_t *dds3AddP2AEffectObjectBasic(const char *path, void *firstVector, s32 secondVectorAddress) {
    void *bill;

    bill = dds3BillboardFileLoad(1, path);
    return AddP2AEffect(bill, firstVector, secondVectorAddress);
}

void dds3EffectObjectBasicSetAnmeID(dds3Basic_t *basic, u32 entry)
{
    dds3BillboardSetAnimNumber(((dds3EffectObjectBasicWorkData_t *)basic->workdata)->handle, entry);
}

dds3Basic_t *AddTMXEffect(void *bill, void *position, s32 rotationAddress)
{
    sdfVector4_t vector;
    dds3Basic_t *basic;
    dds3EffectObjectBasicWorkData_t *work;
    dds3ObjectBaseControlWorkData_t *control;
    dds3Basic_t *area;

    memset(&vector, 0, sizeof(vector));
    basic = dds3AddEffectObjectBasic(dds3AllocObjectId(), position, (void *)rotationAddress);
    if (basic == NULL) {
        return NULL;
    }
    SDF_VU0_LOADVECTOR(position);
    SDF_VU0_STOREVECTOR(&vector);
    dds3BillboardSetPosition(bill, &vector);
    work = basic->workdata;
    work->type = 3;
    work->handle = bill;
    work->stat = 0;
    work->linkbasic = NULL;
    work->linkindex = 0;
    work->linkbasictype = 0;
    control = dds3EffectObjectBasicGetControlWorkData(basic);
    control->modelType = 2;
    area = dds3AreaBasicGetFastArea();
    if (area != NULL) {
        control->control[5] = area;
        dds3AreaBasicRegistItem(area, basic);
    }
    return basic;
}

dds3Basic_t *dds3AddTMXEffectObjectBasic3(dds3Basic_t *obj, void *firstVector, s32 secondVectorAddress) {
    struct billObj_t *bill;

    bill = dds3BillboardCopy((struct billObj_t *)((dds3EffectObjectBasicWorkData_t *)obj->workdata)->handle);
    return AddTMXEffect(bill, firstVector, secondVectorAddress);
}

void dds3AddTMXEffectObjectBasic2(u32 billID, void *position, s32 rotationAddress)
{
    struct billObj_t *bill;

    bill = dds3BillboardCreate(0, billID);
    AddTMXEffect(bill, position, rotationAddress);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", dds3AddTMXEffectObjectBasic);

dds3Basic_t *AddD3PEffect(void *effect, void *position, void *rotation)
{
    sdfVector4_t vector;
    dds3Basic_t *basic;
    dds3EffectObjectBasicWorkData_t *work;
    dds3ObjectBaseControlWorkData_t *control;
    dds3Basic_t *area;

    memset(&vector, 0, sizeof(vector));
    basic = dds3AddEffectObjectBasic(dds3AllocObjectId(), position, rotation);
    if (basic == NULL) {
        return NULL;
    }
    SDF_VU0_LOADVECTOR(position);
    SDF_VU0_STOREVECTOR(&vector);
    dds3EffectSetPosition(effect, &vector);
    work = basic->workdata;
    work->type = EFF_OBJ_STATE_BOUND_BILL;
    work->handle = effect;
    work->stat = 0;
    work->linkbasic = NULL;
    work->linkindex = 0;
    work->linkbasictype = 0;
    control = dds3EffectObjectBasicGetControlWorkData(basic);
    control->modelType = 2;
    area = dds3AreaBasicGetFastArea();
    if (area != NULL) {
        control->control[5] = area;
        dds3AreaBasicRegistItem(area, basic);
    }
    return basic;
}

dds3Basic_t *dds3AddD3PEffectObjectBasic2(void *descriptor, void *position, void *rotation)
{
    EffNode *effect;

    effect = dds3EffectLoad(descriptor);
    return AddD3PEffect(effect, position, rotation);
}

dds3Basic_t *dds3AddD3PEffectObjectBasic(u32 parameter, void *position, void *rotation)
{
    EffNode *effect;

    effect = dds3EffectFileLoad(parameter);
    return AddD3PEffect(effect, position, rotation);
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
        control->control[5] = area;
        dds3AreaBasicRegistItem(area, basic);
    }
    return basic;
}

void *dds3AddBEDEffectObjectBasic2(void *source, void *position, void *rotation)
{
    return AddBEDEffect(source, position, rotation);
}

void *dds3AddBEDEffectObjectBasic(void *resource, void *firstVector, void *secondVector) {
    u32 resolvedId;
    void *resourceHandle;
    void *created;

    resolvedId = 0;
    resourceHandle = sdfDevLoadFileEx(resource, &resolvedId, 0);
    created = AddBEDEffect((effBTL_t *)resolvedId, firstVector, secondVector);
    sdfMemFreeDirect(resourceHandle);
    return created;
}

void dds3StartBEDEffectObjectBasic(dds3Basic_t *obj, u32 entryId) {
    dds3EffectObjectBasicWorkData_t *data;

    data = obj->workdata;
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
    data->cuth = effEveBEDCutCreate(data->handle, (u16)(entryId & EFF_OBJ_ENTRY_ID_MASK), data->para);
}

dds3Basic_t *AddESPEffect(const char *name, void *firstVector, void *secondVector, f32 scale) {
    dds3Basic_t *obj;
    dds3EffectObjectBasicWorkData_t *data;
    dds3ObjectBaseControlWorkData_t *objectHandle;
    dds3Basic_t *worldNode;

    obj = dds3AddEffectObjectBasic(dds3AllocObjectId(), firstVector, secondVector);
    if (obj == NULL) {
        return NULL;
    }
    data = obj->workdata;
    data->type = EFF_OBJ_STATE_PARAMETERS_READY;
    obj->poseset->scl.x = scale;
    obj->poseset->scl.y = scale;
    obj->poseset->scl.z = scale;
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
        objectHandle->control[5] = worldNode;
        dds3AreaBasicRegistItem(worldNode, obj);
    }
    return obj;
}

dds3Basic_t *dds3AddESPEffectObjectBasic(const char *name, void *position, void *rotation, f32 scale)
{
    return AddESPEffect(name, position, rotation, scale);
}

/* Keep the legacy positional (kind, descriptor) ABI used by the file loader. */
dds3Basic_t *AddMG12Effect(kind, descriptor)
    s32 kind;
    void *descriptor;
{
    sdfVector4_t position;
    sdfVector4_t rotation;
    dds3MagaArea_t areaParameters;
    dds3MagaPoints_t pointParameters;
    EffNode *effect;
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
        control->control[5] = area;
        dds3AreaBasicRegistItem(area, basic);
    }
    return basic;
}

dds3Basic_t *dds3AddMG12EffectObjectBasic2(s32 kind, void *descriptor)
{
    return AddMG12Effect(kind, descriptor);
}

void *dds3AddMG12EffectObjectBasic(s32 kind, void *resource) {
    u32 resolvedId;
    void *resourceHandle;
    void *created;

    resolvedId = 0;
    resourceHandle = sdfDevLoadFileEx(resource, &resolvedId, 0);
    created = AddMG12Effect(kind, resolvedId);
    sdfMemFreeDirect(resourceHandle);
    return created;
}

dds3EffectObjectBasicWorkData_t *CheckESP(dds3Basic_t *obj) {
    dds3EffectObjectBasicWorkData_t *data;

    data = NULL;
    if (obj == NULL) {
        return data;
    }
    if (((u8 *)&obj->type)[3] != EFF_OBJ_KIND) {
        return data;
    }
    data = obj->workdata;
    if (data->type != EFF_OBJ_STATE_PARAMETERS_READY) {
        data = NULL;
    }
    return data;
}

s32 GetESPPos(dds3Basic_t *basic)
{
    if (CheckESP(basic) == NULL) {
        return 0;
    }
    SDF_VU0_LOADVECTOR(&basic->poseset->pos);
    return 1;
}

s32 GetESPR(dds3Basic_t *obj) {
    dds3PoseSet_t *parameters;

    if (CheckESP(obj) == NULL) {
        return 0;
    }
    parameters = obj->poseset;
    return (s32)parameters->scl.x;
}

s32 dds3EffectObjectPresetMG12(dds3Basic_t *obj) {
    if (((u8 *)&obj->type)[3] == EFF_OBJ_KIND) {
        dds3EffectObjectBasicWorkData_t *data = obj->workdata;

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
s32 dds3EffectObjectSetMG12Pos(dds3Basic_t *basic, dds3Basic_t *first,
                              dds3Basic_t *second, dds3Basic_t *third, dds3Basic_t *fourth)
{
    dds3EffectObjectBasicWorkData_t *work;
    dds3MagaArea_t *area;
    dds3MagaPoints_t *points;

    if (basic->type >> 24 != EFF_OBJ_KIND) {
        return 0;
    }
    work = basic->workdata;
    switch (work->type) {
    case EFF_OBJ_STATE_MAGATUHI_TWO_ROWS:
        area = work->para;
        if (GetESPPos(first)) {
            SDF_VU0_STOREVECTOR(&area->position[0]);
            area->radius[0] = GetESPR(first);
        }
        if (GetESPPos(second)) {
            SDF_VU0_STOREVECTOR(&area->position[1]);
            area->radius[1] = GetESPR(second);
        }
        effMagaSCSetArea(work->handle, area);
        return 1;
    case EFF_OBJ_STATE_MAGATUHI_FOUR_ROWS:
        points = work->para;
        if (GetESPPos(first)) {
            SDF_VU0_STOREVECTOR(&points->position[0]);
            points->radius[0] = GetESPR(first);
        }
        if (GetESPPos(second)) {
            SDF_VU0_STOREVECTOR(&points->position[1]);
            points->radius[1] = GetESPR(second);
        }
        if (GetESPPos(third)) {
            SDF_VU0_STOREVECTOR(&points->position[2]);
            points->radius[2] = GetESPR(third);
        }
        if (GetESPPos(fourth)) {
            SDF_VU0_STOREVECTOR(&points->position[3]);
            points->radius[3] = GetESPR(fourth);
        }
        effMagaP4SetPoint(work->handle, points);
        return 1;
    }
    return 0;
}

void dds3EffectObjectBasicStatOn(dds3Basic_t *obj, s32 flags) {
    ((dds3EffectObjectBasicWorkData_t *)obj->workdata)->stat |= flags;
}

void dds3EffectObjectBasicStatOff(dds3Basic_t *obj, s32 flags) {
    ((dds3EffectObjectBasicWorkData_t *)obj->workdata)->stat &= ~flags;
}

s32 dds3EffectObjectBasicLinkObject(dds3Basic_t *obj, dds3Basic_t *owner) {
    dds3EffectObjectBasicWorkData_t *data;
    u8 kind;

    kind = ((u8 *)&owner->type)[3];
    if (kind < EFF_OBJ_OWNER_KIND_FIRST || (kind >= EFF_OBJ_OWNER_KIND_END && kind != EFF_OBJ_OWNER_KIND_EXTRA)) {
        return 0;
    }
    data = obj->workdata;
    data->linkpos[2] = 0;
    data->linkbasic = owner;
    data->linkindex = 0;
    data->stat |= EFF_OBJ_FLAG_OWNER_LINK;
    data->stat &= ~EFF_OBJ_FLAG_OWNER_BILL_ENTRY;
    data->linkpos[0] = 0;
    data->linkbasictype = ((u8 *)&owner->type)[3];
    data->linkpos[1] = 0;
    return 1;
}

void dds3EffectObjectBasicGetObjectIndex(dds3Basic_t *obj) {
    dds3EffectObjectBasicWorkData_t *data;
    dds3Basic_t *owner;
    EffOwnerBillPayloadView *bill;

    data = obj->workdata;
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

s32 dds3EffectObjectBasicLinkObjectIndex(dds3Basic_t *obj, dds3Basic_t *owner, s32 entryId) {
    dds3EffectObjectBasicWorkData_t *data;

    if (dds3EffectObjectBasicLinkObject(obj, owner) == 0) {
        return 1;
    }
    data = obj->workdata;
    data->linkindex = entryId;
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

void dds3EffectObjectBasicSetFldEx(dds3Basic_t *owner, fldResourceEffect_t *config) {
    dds3EffectObjectBasicWorkData_t *resource = owner->workdata;
    dds3PoseSet_t *source = owner->poseset;

    resource->pFldEx = config;
    resource->FldExBasePos.x = source->pos.x;
    resource->FldExBasePos.y = source->pos.y;
    resource->FldExBasePos.z = source->pos.z;
    resource->FldExBasePos.w = source->pos.w;
    dds3EffectObjectBasicStatOn((struct dds3Basic_t *)owner, 0x10);
    switch (resource->pFldEx->type) {
    case 0:
    case 3:
        dds3BillboardSetXYOffset((struct billObj_t *)resource->handle, resource->pFldEx->size[0], resource->pFldEx->size[1]);
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

        if (dds3EffectObjectAddFld(map, floor, config->groupID)) {
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
        EE_SQRT_F32(distance, squaredDistance);
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
            s32 entry = dds3EffectObjectFindFld(fldGlobalBase.area,
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

