#include "common.h"
#include "basic.h"

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
    u32 flag;
    dds3BasicArrayHandle_t *hEventBasicArray;
    u32 modelType;
    void *pModel;
    dds3Basic_t *pBasic[8];
    void *pUserValue;
    void *pModelRangeRootNode;
    void *pMot;
    s32 MotMode;
    f32 MotSpeed;
    u32 svs;
    s32 sdwchrid;
    s32 sdwnodeidx[24];
} dds3ObjectBaseControlWorkData_t;

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

struct BillObj;

struct BillObj *dds3BillboardCopy(struct BillObj *bill);
struct BillObj *dds3BillboardCreate(s32 kind, u32 id);
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
struct sdfDrawTag_t;
typedef struct sdfDrawTag_t sdfDrawTag_t;
struct DevRequest;
typedef struct DevRequest DevRequest;
struct sdfMotionCtrlr_t;
typedef struct sdfMotionCtrlr_t sdfMotionCtrlr_t;
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

typedef struct BillRenderPair {
    struct BillChildPayload *children[2];
    u16 cameraFacingMode;
    u16 kind;
    u32 colors[2];
    sdfDrawTag_t *packetList;
    struct BillRenderPair *next;
} BillRenderPair;

typedef struct BillObj {
    f32 position[4];
    f32 childScaleX;
    f32 childScaleY;
    f32 unk18;
    f32 unk1C;
    f32 rotationAngle;
    u32 childParam;
    void (*callback)();
    u16 kind;
    u16 requestedPacketListIndex;
    union {
        void *payload;
        struct BillChildPayload *child;
        struct BillData *animationData;
    } anon0;
    BillRenderPair pair;
    u16 animationActive;
    u8 pad52[2];
    u32 modeFlags;
    u32 animationEntryIndex;
    s32 entryCount;
    struct BillOut *resolvedEntries;
} BillObj;

typedef struct EffAimParams {
    u8 scaleKind;
    u8 aimMode;
    u8 directionMode;
    u8 pad3;
    s32 rangeOverride;
} EffAimParams;

typedef union EffBattleParameterValue {
    u32 sourceOffset;
    struct effPartHandle_t *work;
} EffBattleParameterValue;

typedef struct EffBattleEntry {
    u16 paramKind;
    u8 pad02[2];
    EffBattleParameterValue value;
    EffAimParams aimParams;
    u32 zeroKindCount;
    u8 kind;
    u8 pad15[3];
} EffBattleEntry;

typedef struct EffBattleEntryList {
    s32 triggerFrame;
    u8 pad04[2];
    u16 shake;
    u16 shakeFrame;
    u16 shakeFade;
    u8 colorKeys[0x24];
    u8 rgbKeys[0x18];
    s32 callbackFrame;
    u8 pad4C[0x20];
    u16 count;
    u8 pad6E[2];
    s32 colorRampEndFrame;
    u8 actorScaleEnabled;
    u8 pad75[3];
    EffBattleEntry entries[1];
} EffBattleEntryList;

typedef struct SoundBank {
    u8 pad00[8];
    EffBattleEntryList list;
    u8 pad98[0x580];
} SoundBank;

typedef struct SoundMixer {
    SoundBank banks[2];
    u8 pad0C30[8];
    struct sdfMemHandle_t *resource;
    u32 active;
    BattleEffect *voiceList;
} SoundMixer;

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

typedef struct sdfModelRootNode_t {
    DevRequest *list;
    SdfDrawNode *rootNode;
    void *assetData;
    DevRequest *resources;
    DevRequest *slotPairs;
    struct sdfMotionCtrlr_t *motionList;
    u8 unk18;
    u8 flags;
    u16 drawFlags;
    u32 color;
    f32 matrix[4][4];
    f32 rotationQuaternion[4];
    f32 scaleVector[4];
    struct SdfDmaTagHeader *lighting;
    struct SdfDmaTagHeader *secondaryLighting;
    f32 scalarOverrideFirst;
    f32 scalarOverrideSecond;
    struct SdfChunkHeader *chunkTable;

    f32 modelScale;
    u8 lodIndex;
    u8 pad99[3];
} sdfModelRootNode_t;

typedef struct EffOwnerBillParamView {
    u8 pad00[0x18];
    struct sdfModelRootNode_t *model;
} EffOwnerBillParamView;

void dds3EffectDelete(EffNode *node);
void dds3BillboardDelete(struct BillObj *obj);
void effEveBEDCutDelete(EffEventWork *work);
void effEveBEDDelete(struct SoundMixer *mixer);
void sdfChipFreeDirect(void *memory);
s32 dds3InitPoseSet(dds3Basic_t *node);
void *sdfChipAllocDirect(s32 size);
dds3ObjectBaseControlWorkData_t *dds3ObjectBaseInitControlWorkData(void *owner);
extern void dds3ObjectBaseOnFlag(void *, s32);
extern u8 opaque40[0x60];
void dds3FreePoseSet(dds3Basic_t *node);
void dds3ObjectBaseFreeControlWorkData(void *objectBase);
void dds3EffectObjectBasicCalcFldEx(dds3Basic_t *object);
u8 dds3ObjectBaseCheckFlag(struct dds3Basic_t *object, u32 mask);
void dds3EffectDraw(EffNode *node);
extern void dds3BillboardDraw(struct BillObj *bill);
void effEveBEDCutDraw(EffEventWork *work);
dds3Basic_t *dds3AddSceneBasicData(s32 kind);
void dds3ObjectBaseCreateMover(void *object);
void dds3SetPosPoseSet(dds3Basic_t *object, u128 *vector);
void dds3SetRotPoseSet(dds3Basic_t *object, u128 *vector);
extern void dds3SetRadiusPoseSet(dds3PoseSet_t *node);
dds3Basic_t *AddP2AEffect(void *bill, void *vec, s32 extra);
extern BillObj *dds3BillboardFileLoad(s32 kind, const char *path);
dds3Basic_t *AddTMXEffect(void *bill, void *vec, s32 extra);
dds3Basic_t *AddBEDEffect();
void sdfMemFreeDirect(struct sdfMemHandle_t *allocation);
EffEventWork *effEveBEDCutCreate(struct SoundMixer *owner, u16 kind, const void *params);
u32 dds3AllocObjectId(void);
dds3ObjectBaseControlWorkData_t *dds3EffectObjectBasicGetControlWorkData(dds3Basic_t *object);
extern dds3Basic_t *dds3AreaBasicGetFastArea(void);
void dds3AreaBasicRegistItem(dds3Basic_t *indexOwner, dds3Basic_t *node);
dds3Basic_t *AddMG12Effect();
extern void effMagaRandomize(void *bill);
extern s32 sdfModelGetMarkPosMatrix(sdfModelRootNode_t *model, s32 id);
void dds3BillboardSetXYOffset(struct BillObj *billboard, f32 width, f32 height);
void dds3BillboardSetDrawType(struct BillObj *billboard, s32 variantValue);
void dds3BillboardSetMode(struct BillObj *billboard, s32 mode);

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

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", dds3EffectObjectBasicMainProcedure);

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

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", dds3EffectObjectBasicGetControlWorkData);

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

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", AddP2AEffect);

dds3Basic_t *dds3AddP2AEffectObjectBasic3(dds3Basic_t *obj, void *firstVector, s32 secondVectorAddress) {
    struct BillObj *bill;

    bill = dds3BillboardCopy((struct BillObj *)((dds3EffectObjectBasicWorkData_t *)obj->workdata)->handle);
    return AddP2AEffect(bill, firstVector, secondVectorAddress);
}

void dds3AddP2AEffectObjectBasic2(u32 billId, void *firstVector, s32 secondVectorAddress) {
    struct BillObj *bill;

    bill = dds3BillboardCreate(1, billId);
    AddP2AEffect(bill, firstVector, secondVectorAddress);
}

dds3Basic_t *dds3AddP2AEffectObjectBasic(const char *path, void *firstVector, s32 secondVectorAddress) {
    void *bill;

    bill = dds3BillboardFileLoad(1, path);
    return AddP2AEffect(bill, firstVector, secondVectorAddress);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", dds3EffectObjectBasicSetAnmeID);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", AddTMXEffect);

dds3Basic_t *dds3AddTMXEffectObjectBasic3(dds3Basic_t *obj, void *firstVector, s32 secondVectorAddress) {
    struct BillObj *bill;

    bill = dds3BillboardCopy((struct BillObj *)((dds3EffectObjectBasicWorkData_t *)obj->workdata)->handle);
    return AddTMXEffect(bill, firstVector, secondVectorAddress);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", dds3AddTMXEffectObjectBasic2);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", dds3AddTMXEffectObjectBasic);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", AddD3PEffect);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", dds3AddD3PEffectObjectBasic2);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", dds3AddD3PEffectObjectBasic);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", AddBEDEffect);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", dds3AddBEDEffectObjectBasic2);

void *dds3AddBEDEffectObjectBasic(void *resource, void *firstVector, void *secondVector) {
    u32 resolvedId;
    void *resourceHandle;
    void *created;

    resolvedId = 0;
    resourceHandle = sdfDevLoadFileEx(resource, &resolvedId, 0);
    created = AddBEDEffect((SoundMixer *)resolvedId, firstVector, secondVector);
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
        objectHandle->pBasic[5] = worldNode;
        dds3AreaBasicRegistItem(worldNode, obj);
    }
    return obj;
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", dds3AddESPEffectObjectBasic);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", AddMG12Effect);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", dds3AddMG12EffectObjectBasic2);

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

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", GetESPPos);

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

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", dds3EffectObjectSetMG12Pos);

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

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", dds3EffectObjectBasicUnlinkObject);

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
        dds3BillboardSetXYOffset((struct BillObj *)resource->handle, resource->pFldEx->size[0], resource->pFldEx->size[1]);
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

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", dds3EffectObjectBasicCalcFldEx);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00530E20);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00530E50);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00530E78);

