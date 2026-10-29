#include "common.h"

#include "basic.h"
#include "field.h"

enum fldPackedResourceKind
{
    FLD_PACKED_RESOURCE_INFO_TABLE = 1,
    FLD_PACKED_RESOURCE_NPC_PALETTE = 2,
    FLD_PACKED_RESOURCE_SKY_BUFFER = 3,
    FLD_PACKED_RESOURCE_ACTOR_WAYPOINT_TABLE = 4,
    FLD_PACKED_RESOURCE_RETAINED_COPY = 5,
    FLD_PACKED_RESOURCE_SCENE_RECORD_CHUNK = 6
};

typedef struct objectTransform_t objectTransform_t;

typedef s32 (*dds3MoverUpdate_t)(struct objectTransform_t *, dds3Basic_t *);


typedef struct worldIndexNode_t worldIndexNode_t;

typedef struct motion_t motion_t;


typedef struct fldMoverWork_t fldMoverWork_t;

typedef struct fldTransferChunk_t fldTransferChunk_t;


typedef struct fileRequest_t fileRequest_t;

typedef struct filePacRequest_t filePacRequest_t;

typedef struct fldNpcPalette_t fldNpcPalette_t;

typedef struct fldSkyBuffer_t fldSkyBuffer_t;

extern u32 fldLoadNextStatus[];


/* Kind 3 allocates 0x10 bytes in dds3AllocateClearedObjectWork. */
struct fldMoverWork_t {
    dds3Basic_t *target; /* 0x00: object whose transform is updated. */
    dds3PathBasicControl_t *path; /* 0x04: retained curve work. */
    dds3MoverUpdate_t update; /* 0x08: used when no curve work is retained. */
    dds3Basic_t *sourceObject; /* 0x0C: kind-16 curve source. */
};

/* Packed-resource transfer descriptor; the first word's role is unknown. */
struct fldTransferChunk_t {
    u32 unk00; /* 0x00: opaque descriptor word */
    s32 offset; /* 0x04: signed byte offset from the resource base */
    u32 size; /* 0x08: byte length of the transferred payload */
};


/* A PAC archive request (kind 1), with its dispatch packet at 0x30. */
struct filePacRequest_t {
    u8 kind; /* 0x00 */ u8 state; /* 0x01 */ u8 retryCount; /* 0x02 */ u8 allocationMode; /* 0x03: a callback request's mode */ struct filePacRequest_t *next; /* 0x04 */ char *name; /* 0x08: the duplicated request name */ void *deviceRequest; /* 0x0C */ s32 transferBytes; /* 0x10: a value-pair request's byte count */ s32 totalBytes; /* 0x14 */ void * completionCallback; /* 0x18 */ void *completionArg; /* 0x1C */ sdfMemHandle_t *allocationHandle; /* 0x20: descriptor passed to sdfResourceRetainAddress */ u32 retainedAddress; /* 0x24: a value-pair request's source data */ u32 transferAddress; /* 0x28 */ struct filePacRequest_t *completionNext; /* 0x2C */ sdfPacDecodeCb_t packet; /* 0x30: PAC requests */ u16 readinessEnabled; /* 0x68: a gate on completion, not a state value */ u16 slot; /* 0x6A: the device-read slot */ struct filePacRequest_t *callbackNext; /* 0x6C: PAC completion callback queue link */
};


dds3Basic_t *fldMakeDmyMatter(void);

extern dds3Basic_t *dds3AddMatterObjectBasic(s32 serialID, f32 *pos, f32 *rot);

extern u32 dds3AllocObjectId(void);

extern void dds3SetUnitObjectId(dds3Basic_t *node, u32 value);

dds3Basic_t *fldGetMatter_From_ModelRangeID(u32 modelRangeID);

/* Return the borrowed secondary world node, or NULL when no world is active. */
dds3Basic_t *dds3GetControlScene(void);

extern fldFileResource_t *g_pHitData;

extern u32 g_nHitData;

extern dds3Basic_t *dds3SceneBasicSearchNameAndType(dds3Basic_t *scene, s32 type, const u8 *name);

/* Read or update the cached world object associated with a resource record ID. */
dds3Basic_t *fldFindMatterIndex(s32 id);

extern void fldFindMatterIndexFrom(s32 id, dds3Basic_t *worldNode);

dds3Basic_t *fldGetModelRange_From_MatterName(const char *matterName);

extern s32 strcmp(const char *left, const char *right);

void fldMakeFilePath(char *pathstr, s32 field, s32 area);


extern s32 sprintf(char *dst, const char *format, ...);

s32 fldMakeObjectBasic3(void);

void fldMakeObjectBasic2(void);

typedef struct fldModelData_t {
    u8 unk00[16]; /* 0x00 */
    u8 modelDef[16]; /* 0x10: the model definition dds3AddModelBasic2 takes */
    void *playBook; /* 0x20: motion play book */
} fldModelData_t;

typedef struct fldResourceAreaRef_t {
    u32 word00; /* 0x00 */
    const u8 *name; /* 0x04 */
} fldResourceAreaRef_t;

extern dds3Basic_t *dds3AddModelBasic2(u32 serialID, void *pModelDef, void *pMotDef, s32 pTexList, const char *pName);

extern void dds3MatterObjectBasicSetOt(dds3Basic_t *aPtr, s32 idx);

extern s32 fldCommand_GetHojiID(const char *pName);

extern void dds3MatterObjectBasicSetHojiID(dds3Basic_t *aPtr, s32 hojiID);

extern dds3Basic_t *dds3ObjectBaseSetControlBasic(dds3Basic_t *pObjectBasic, dds3Basic_t *pItemBasic);

extern void dds3ObjectBaseMakeMatter(dds3Basic_t *pObjectBasic);

extern s32 dds3ObjectBaseRegistArea(dds3Basic_t *pObjectBasic);

extern fldFileResource_t *D_00551C8C;

extern u32 D_00551C90;

extern s32 fldGetHojiFlag(s32 mapId, u32 slotIndex, s32 bit);

extern s32 fldCommand_SEARCH_ID(const char *);

extern dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *pBasic, u32 serialID, s32 type);


extern void dds3ObjectBaseCreateMover(void *pObjectBasic);

extern void dds3MoverBasicSetPath(dds3Basic_t *pMoverBasic, dds3Basic_t *pPathBasic);

extern void dds3MoverBasicMakePath(dds3Basic_t *pMoverBasic);


extern dds3PathBasicControl_t *dds3MoverBasicGetPathControl(dds3Basic_t *pMoverBasic);

extern void evtPathRate(dds3Basic_t *basic, f32 rate);

extern void dds3MoverBasicUpdate(dds3Basic_t *);

extern s32 fldGetModelFlag(s32 mapId, u32 slotIndex, s32 bit);

extern void dds3MatterObjectBasicSetAlpMode(dds3Basic_t *, u32);

u8 *fldGetInfoHeader(fldTransferChunk_t *pData);

void sdfAddressResolverReset(u32 *aDataBody, u32 aOffset, const u8 *aAddrInfoPtr, s32 aAddrInfoLeng);

void fldAddressResolver(u32 pFileTop, fldTransferChunk_t *pData);

extern void sdfAddressResolver(s32 *aDataBody, s32 aOffset, const u8 *aAddrInfoPtr, s32 aAddrInfoLeng);

void fldAddressResolverReset(u32 pFileTop, fldTransferChunk_t *pData);

void fldLoadNextFile_SetId_BtlRet(u32 field, u32 area);

extern u32 D_0054EBF4;

extern u32 D_0054EBF8;

s32 fldLoadNextFile_BtlRet(void);

void fldFreeNextFile(void);


extern struct fileRequest_t *pFldNextFileDataF2;

struct fileRequest_t *pacLoad(const char *filename);

extern void sdfMemFreeDelayedDirect(sdfMemHandle_t *aHandle);

extern s32 pacFileHandleFree_(void *pacHandle);

u8 fldLoadNextGetStatus2(void);

extern s32 pacCheckEnd(const struct fileRequest_t *pacHandle);

s32 fldLoadNextGetStatus3(s32 field, s32 area);

sdfMemHandle_t *fldLoadNextFileTB(void **pData, s32 field, s32 area);

sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

extern u32 sdfMemLock(sdfMemHandle_t *aHandle);

extern void *memcpy(void *a, const void *b, u32 c);

sdfMemHandle_t *fldLoadNextFileF1(void **pData, s32 field, s32 area);

sdfMemHandle_t *fldLoadNextFileF2(void **pData, s32 field, s32 area);

extern u32 D_0054EBE8, D_0054EBEC, D_0054EBEC, D_0054EBF0;

extern u32 D_0054EBDC, D_0054EBE0, D_0054EBE0, D_0054EBE4;

u8 fldCheckNextFile(void);

void fldPreloadNextFile(char *out);

extern s32 evtBitCheck(s32 no);





extern char fldFieldPackFlag[];

void fldLoadFieldPack(void);

extern char * strcpy(char *, const char *);

extern struct fileRequest_t *gFldFileF1;

extern void pacSync(struct fileRequest_t *pacFileHandle);

extern void fldInfLoadFromMem(const void *);

extern void fldGetEAEntry(const fldNpcPalette_t *source);

extern void fldGetSkyEntry(fldSkyBuffer_t *source);

extern void fldGetDoorEntry(const void *source);

extern sdfMemHandle_t *fldFieldPackData;

extern s32 sdfMemGetSize(const sdfMemHandle_t *aHandle);

extern u8 *sdfMemGetPtr(const sdfMemHandle_t *aHandle);

extern void fldAmbLoadFromMem(u8 *data, s32 mh);

extern void fldAutoMapFontInit(s32 bank);

void fldFreePac0(void);

void fldFreeFile(void);

u32 fldLoadFile(const char *name);

extern u32 gFldInf_WpExit;

extern s32 dds3GetMapNo(dds3Basic_t *scene);

extern void fldPlayerALoad(void);

extern void fldInitBlurEffect(void);

extern s32 sdfDevLoadTexList(const char *aFilename);

extern sdfMemHandle_t *sdfDevLoadFileEx(const char *path, void **destination, u32 *size);

extern void fldSetupDoorScenes(void);

extern s32 dds3SceneBasicLoadScriptFile_FromMem(dds3Basic_t *scene, void *mem);

extern volatile s32 fldSceneParam[];

extern char D_0054EC08[]; /* "%s.TB" */

extern char D_0054EC10[]; /* "%s.f2" */

extern char D_0054EC18[]; /* "%s.f1" */

extern s32 dds3SearchProcessName(const char *namestr);

extern void fldDrwZFree(void);

extern void fldClearSe3D(void);

extern s32 fldTitleChk(void);

extern s32 fldTitleMiniChk(void);

extern s32 dds3KillProcessName(const char *name, s32 shutdowntimer);

extern void fldFreeModelTable(void);

extern void fldClearTakaraSlots(void);

extern void fldFreeNpcModel(void);

extern void fldAutoMapFree(void);

extern void fldAutoMapFileFree(void);

extern void fldFreeBlurEffect(void);

extern void fldClearMatterFlags(void);

extern void fldClearEveRngTable(void);

extern void fldSe_Init(void);

extern void fldClearModelSlots(void);

extern s32 sdfTexListFree(s32 aList, s32 aReleaseItems);

extern sdfMemHandle_t *D_0055DA98[];

extern void *D_0055DAA8[];

extern u32 D_0054EBCC;

extern struct fileRequest_t *fldFileBuf1;




extern u8 D_0054EC00[];

void fldFileAddrResetMode(u32 mode);

void fldHojiDebugView(void);

void fldHojiDebugView2(void);

extern s32 fldGetAnimeFlag(s32 field, s32 area, s32 block);



extern void fldDbgPrint2D(s32 x, s32 y, const char *str);

extern s32 fldGbChkHit(s32 fld, s32 area, s32 blk);




extern char D_0054EC20[]; /* "%02d" */

extern char D_0054EC28[]; /* "." */

extern char D_0054EC30[]; /* "%d" */

extern u32 gFldFileResetMode;

typedef struct fldFileAreaDataFL1_t {
    u32 fileID;
    u32 SkyTblIdx;
    char *filename0;
    char *filename1;
    char *filename2;
    char *filename3;
} fldFileAreaDataFL1_t;

typedef struct fldFileInfoHeaderFL1_t {
    u32 nResourceTypeTable;
    void *pResourceTypeTable;
    fldFileAreaDataFL1_t *pAreaData;
} fldFileInfoHeaderFL1_t;

extern void fldFileResolverFL1(fldFileInfoHeaderFL1_t *info);
extern void fldSetFileResolverIdx(u32 index);
extern char *fldResolverFL1Data[4];
extern char D_0054EBC0[]; /* "%s%s", shared with fldMakeResouceBasicFL1. */
extern void *memset(void *buffer, s32 value, u32 size);
extern void fldMakeResouceBasicFL1(void *typeTable, u32 typeCount);

u32 fldLoadFile_FromMemory(s32 field, s32 area, u8 *fl1Buffer, u8 *fl2Buffer, void *textureBuffer);

extern void fldDrwZAlloc(void);

extern void fldAutoMapSetField(s32 fld, s32 area, s32 block);

extern s32 sdfTexSetTB(void *aData);

extern u32 D_0054EB98, D_0054EBD0, D_0054EBD4, fldFileBuf2;

extern u8 *FildMaskObj, *mat_black, *mat_white, *mat_line;

extern u32 fldJDataHandle[];

typedef struct fldConstruntAreaFL2_t {
    u32 flag; /* 0x00 */
    const char *areaName; /* 0x04 */
    u32 areaID; /* 0x08 */
} fldConstruntAreaFL2_t;

extern dds3Basic_t *dds3AddAreaBasic(u32 serialID, const char *pName);

void fldMakeBlockBasicFL2(fldConstruntAreaFL2_t *pBlockTable, u32 nBlock);

s32 fldLoadNextFile(s32 field, s32 area);

typedef struct fldFileAreaDataFL2_t {
    u32 word00;
    s32 label; /* 0x04: the script entry recorded for a field retry. */
} fldFileAreaDataFL2_t;

typedef struct fldFileInfoHeaderFL2_t {
    u32 nResourceTypeTable; /* 0x00 */
    void *pResourceTypeTable; /* 0x04 */
    fldFileAreaDataFL2_t *pAreaData; /* 0x08 */
    u32 nArea; /* 0x0C */
    fldConstruntAreaFL2_t *pAreaTable; /* 0x10 */
} fldFileInfoHeaderFL2_t;

extern void fldMakeResouceBasicFL2(void *typeTable, u32 typeCount);

extern s32 fldRetryFileId[];

void fldFileResolverFL2(fldFileInfoHeaderFL2_t *info);

s32 fldLoadNextTask(void);

dds3Basic_t *fldMakeDmyMatter(void) {
    f32 position[4];
    f32 rotation[4];
    dds3Basic_t *matter;
    position[0] = 0.0f;
    position[1] = 0.0f;
    position[2] = 0.0f;
    position[3] = 0.0f;
    rotation[0] = 0.0f;
    rotation[1] = 0.0f;
    rotation[2] = 0.0f;
    rotation[3] = 0.0f;
    matter = dds3AddMatterObjectBasic(dds3AllocObjectId(), position, rotation);
    dds3SetUnitObjectId(matter, (u32)"FLD_DMY_MATTER");
    return matter;
}

dds3Basic_t *fldGetMatter_From_ModelRangeID(u32 modelRangeID) {
    u32 resourceIndex = 0;
    dds3Basic_t *secondaryWorld = dds3GetControlScene();
    fldFileResource_t *resource = g_pHitData;

    for (; resourceIndex < g_nHitData; resourceIndex++, resource++) {
        if (modelRangeID == resource->id) {
            if (resource->names != NULL) {
                fldFileNameEntry_t *nameEntry = resource->names->entries;
                u32 nameIndex = 0;

                for (; nameIndex < resource->names->count; nameIndex++, nameEntry++) {
                    dds3Basic_t *resolvedObject = dds3SceneBasicSearchNameAndType(secondaryWorld, 6, (const u8 *)nameEntry->name);
                    if (resolvedObject != NULL) {
                        return resolvedObject;
                    }
                }
            } else {
                dds3Basic_t *resolvedObject = fldFindMatterIndex(resource->id);
                if (resolvedObject == NULL) {
                    resolvedObject = fldMakeDmyMatter();
                    fldFindMatterIndexFrom(resource->id, resolvedObject);
                }
                return resolvedObject;
            }
        }
    }
    return NULL;
}

dds3Basic_t *fldGetModelRange_From_MatterName(const char *matterName) {
    u32 resourceIndex = 0;
    dds3Basic_t *secondaryWorld = dds3GetControlScene();
    fldFileResource_t *resource = g_pHitData;

    for (; resourceIndex < g_nHitData; resourceIndex++, resource++) {
        if (resource->names != NULL) {
            fldFileNameEntry_t *nameEntry = resource->names->entries;
            u32 nameIndex = 0;

            for (; nameIndex < resource->names->count; nameIndex++, nameEntry++) {
                if (strcmp(matterName, nameEntry->name) == 0) {
                    dds3Basic_t *resolvedObject = dds3SceneBasicSearchNameAndType(secondaryWorld, 11, (const u8 *)resource->name);
                    if (resolvedObject != NULL) {
                        return resolvedObject;
                    }
                }
            }
        }
    }
    return NULL;
}

/* FL1 and FL2 use the same 0x0C-byte resource-type table entries. */
typedef struct fldSpawnBatch_t {
    u32 kind;
    u32 count;
    fldFileResource_t *entries;
} fldSpawnBatch_t;

typedef struct fldBillConfigView_t {
    u32 flags;
    u32 kind;
    u32 resourceIndex;
    f32 width;
} fldBillConfigView_t;

typedef struct fldLightDataView_t {
    u8 pad00[0x64];
    u32 flags;
} fldLightDataView_t;

typedef struct fldCollisionDescriptor_t fldCollisionDescriptor_t;

typedef struct fldSpawnCollisionView_t {
    u32 kind;
    u32 word04;
    fldCollisionDescriptor_t *header;
} fldSpawnCollisionView_t;

enum {
    FLD_SPAWN_BATCH_KIND_SCRIPT_RESOURCE = 2,
    FLD_SPAWN_BATCH_KIND_COLLISION = 3,
    FLD_SPAWN_BATCH_KIND_CAMERA = 4,
    FLD_SPAWN_BATCH_KIND_EVENT = 6,
    FLD_SPAWN_BATCH_KIND_UNSUPPORTED = 7,
    FLD_SPAWN_BATCH_KIND_PATH = 9,
    FLD_SPAWN_BATCH_KIND_POINT = 10,
    FLD_SPAWN_BATCH_KIND_BILLBOARD_EFFECT = 11,
    FLD_SPAWN_BATCH_KIND_LIGHT = 12
};

typedef struct fldResourceEffect_t fldResourceEffect_t;
typedef struct fldResourceLight_t {
    u32 resv, Flag, AnimeType;
    f32 InsideR, OutsideR, Soften, Bias;
    f32 DiffuseR, DiffuseG, DiffuseB;
    f32 AmbientR, AmbientG, AmbientB;
} fldResourceLight_t;

extern dds3Basic_t *dds3AddTMXEffectObjectBasic(const char *, void *, s32);
extern dds3Basic_t *dds3AddTMXEffectObjectBasic3(dds3Basic_t *, void *, s32);
extern dds3Basic_t *dds3AddP2AEffectObjectBasic(const char *, void *, s32);
extern dds3Basic_t *dds3AddP2AEffectObjectBasic3(dds3Basic_t *, void *, s32);
extern dds3Basic_t *dds3AddD3PEffectObjectBasic2(void *, void *, void *);
extern dds3Basic_t *dds3AddESPEffectObjectBasic(const char *, void *, void *, f32);
extern void dds3EffectObjectBasicStatOn(dds3Basic_t *, s32);
extern void dds3EffectObjectBasicStatOff(dds3Basic_t *, s32);
extern void dds3EffectObjectBasicSetFldEx(dds3Basic_t *, fldResourceEffect_t *);
extern dds3Basic_t *dds3AddLightObjectBasic(s32);
extern void dds3SetPosPoseSet(dds3Basic_t *, const sdfVector4_t *);
extern void dds3GetRadiusPoseSet(dds3Basic_t *, f32);
extern void dds3SetLightObjectResoruce(dds3Basic_t *, fldResourceLight_t *);
extern dds3Basic_t *dds3AddPathBasic(s32, void *, const char *);
extern void fldMakePlane(fldCollisionDescriptor_t *, f32 *, u32, s32, s32);
extern dds3Basic_t *dds3AddModelRangeBasicNew(s32, void *, f32 *, const char *);
extern void dds3SetPathBasicSpline(dds3Basic_t *, u32);

void fldMakeResouceBasicFL1(void *typeTable, u32 batchCount) {
    fldSpawnBatch_t *batch;
    f32 position[4];
    f32 rotation[4];
    char path[64];
    char directory[32];
    s8 loaded[4];
    void *cached[4];
    u32 cacheCount = sizeof(loaded);
    u32 batchIndex;
    u32 resourceCount;
    u32 resourceIndex;
    fldFileResource_t *resource;
    dds3Basic_t *spawnedObject;
    u32 pathIndex;

    batch = (fldSpawnBatch_t *)typeTable;
    memset(loaded, 0, cacheCount);
    memset(cached, 0, cacheCount * sizeof(*cached));
    D_00551C8C = NULL;
    D_00551C90 = 0;
    for (batchIndex = 0; batchIndex < batchCount; batchIndex++, batch++) {
        resource = batch->entries;
        resourceCount = batch->count;
        switch (batch->kind) {
        case 0:
        case 1:
            break;
        case FLD_SPAWN_BATCH_KIND_SCRIPT_RESOURCE:
            D_00551C8C = resource;
            D_00551C90 = resourceCount;
            break;
        case FLD_SPAWN_BATCH_KIND_BILLBOARD_EFFECT:
            for (resourceIndex = 0; resourceIndex < resourceCount; resourceIndex++, resource++) {
                fldBillConfigView_t *config;
                /* The resolver stores these borrowed string addresses as 32-bit words. */
                const char * const *resourcePathSuffixes;
                u32 cacheByteOffset;
                if (resource->transform != NULL) {
                    position[0] = resource->transform[0];
                    position[1] = resource->transform[1];
                    position[2] = resource->transform[2];
                    position[3] = 0.0f;
                    rotation[0] = resource->transform[4];
                    rotation[1] = resource->transform[5];
                    rotation[2] = resource->transform[6];
                    rotation[3] = resource->transform[7];
                } else {
                    position[0] = 0.0f;
                    position[1] = 0.0f;
                    position[2] = 0.0f;
                    position[3] = 0.0f;
                    rotation[0] = 0.0f;
                    rotation[1] = 0.0f;
                    rotation[2] = 0.0f;
                    rotation[3] = 0.0f;
                }
                config = resource->data;
                switch (config->kind) {
                case 0:
                case 3:
                    resourcePathSuffixes = (const char * const *)fldResolverFL1Data;
                    cacheByteOffset = config->resourceIndex * sizeof(*cached);
                    if (loaded[config->resourceIndex] == 0) {
                        fldMakeFilePath(directory, fldGlobalBase.area, fldGlobalBase.floor);
                        sprintf(path, D_0054EBC0, directory,
                                     resourcePathSuffixes[((fldBillConfigView_t *)resource->data)->resourceIndex]);
                        spawnedObject = dds3AddTMXEffectObjectBasic(path, position, (s32)rotation);
                        dds3EffectObjectBasicStatOn(spawnedObject, 1);
                        dds3EffectObjectBasicSetFldEx(spawnedObject, resource->data);
                        loaded[((fldBillConfigView_t *)resource->data)->resourceIndex] = 1;
                        cached[((fldBillConfigView_t *)resource->data)->resourceIndex] = spawnedObject;
                    } else {
                        spawnedObject = dds3AddTMXEffectObjectBasic3(*(void **)((u8 *)&cached + cacheByteOffset), position, (s32)rotation);
                        dds3EffectObjectBasicStatOn(spawnedObject, 1);
                        dds3EffectObjectBasicSetFldEx(spawnedObject, resource->data);
                    }
                    break;
                case 1:
                    resourcePathSuffixes = (const char * const *)fldResolverFL1Data;
                    cacheByteOffset = config->resourceIndex * sizeof(*cached);
                    if (loaded[config->resourceIndex] == 0) {
                        fldMakeFilePath(directory, fldGlobalBase.area, fldGlobalBase.floor);
                        sprintf(path, D_0054EBC0, directory,
                                     resourcePathSuffixes[((fldBillConfigView_t *)resource->data)->resourceIndex]);
                        spawnedObject = dds3AddP2AEffectObjectBasic(path, position, (s32)rotation);
                        dds3EffectObjectBasicStatOn(spawnedObject, 1);
                        loaded[((fldBillConfigView_t *)resource->data)->resourceIndex] = 1;
                        cached[((fldBillConfigView_t *)resource->data)->resourceIndex] = spawnedObject;
                        dds3EffectObjectBasicSetFldEx(spawnedObject, resource->data);
                    } else {
                        spawnedObject = dds3AddP2AEffectObjectBasic3(*(void **)((u8 *)&cached + cacheByteOffset), position, (s32)rotation);
                        dds3EffectObjectBasicStatOn(spawnedObject, 1);
                        dds3EffectObjectBasicSetFldEx(spawnedObject, resource->data);
                    }
                    break;
                case 2:
                    if (D_0055DAA8[config->resourceIndex] != NULL) {
                        spawnedObject = dds3AddD3PEffectObjectBasic2(D_0055DAA8[config->resourceIndex], position, rotation);
                        dds3EffectObjectBasicStatOn(spawnedObject, 1);
                        dds3EffectObjectBasicSetFldEx(spawnedObject, resource->data);
                    }
                    break;
                case 100:
                    spawnedObject = dds3AddESPEffectObjectBasic(resource->name, position, rotation, config->width * 0.5f);
                    dds3EffectObjectBasicStatOff(spawnedObject, 1);
                    break;
                }
            }
            break;
        case FLD_SPAWN_BATCH_KIND_LIGHT:
            for (resourceIndex = 0; resourceIndex < resourceCount; resourceIndex++, resource++) {
                if (resource->transform != NULL) {
                    position[0] = resource->transform[0];
                    position[1] = resource->transform[1];
                    position[2] = resource->transform[2];
                    position[3] = 0.0f;
                    rotation[0] = resource->transform[4];
                    rotation[1] = resource->transform[5];
                    rotation[2] = resource->transform[6];
                    rotation[3] = resource->transform[7];
                } else {
                    position[0] = 0.0f;
                    position[1] = 0.0f;
                    position[2] = 0.0f;
                    position[3] = 0.0f;
                    rotation[0] = 0.0f;
                    rotation[1] = 0.0f;
                    rotation[2] = 0.0f;
                    rotation[3] = 0.0f;
                }
                spawnedObject = dds3AddLightObjectBasic(resource->id);
                dds3SetUnitObjectId(spawnedObject, (u32)resource->name);
                dds3SetPosPoseSet(spawnedObject, (const sdfVector4_t *)position);
                /* The light resource also supplies the pose's outer radius. */
                dds3GetRadiusPoseSet(spawnedObject, ((fldResourceLight_t *)resource->data)->OutsideR);
                dds3SetLightObjectResoruce(spawnedObject, resource->data);
                if ((u32)fldGlobalBase.area - 200U < 300U) {
                    ((fldLightDataView_t *)spawnedObject->workdata)->flags |= 8;
                    dds3SetUnitObjectId(spawnedObject, (u32)"BTL_DEF_LIGHT");
                }
            }
            break;
        case FLD_SPAWN_BATCH_KIND_PATH:
            for (resourceIndex = 0; resourceIndex < resourceCount; resourceIndex++, resource++) {
                dds3AddPathBasic(resource->id, resource->data, resource->name);
            }
            break;
        case FLD_SPAWN_BATCH_KIND_COLLISION:
            for (resourceIndex = 0, pathIndex = 1; resourceIndex < resourceCount; resourceIndex++, resource++, pathIndex++) {
                resource->id |= 0x20000;
                if (resource->transform != NULL) {
                    position[0] = resource->transform[0];
                    position[1] = resource->transform[1];
                    position[2] = resource->transform[2];
                    position[3] = 0.0f;
                    rotation[0] = resource->transform[4];
                    rotation[1] = resource->transform[5];
                    rotation[2] = resource->transform[6];
                    rotation[3] = resource->transform[7];
                } else {
                    position[0] = 0.0f;
                    position[1] = 0.0f;
                    position[2] = 0.0f;
                    position[3] = 0.0f;
                    rotation[0] = 0.0f;
                    rotation[1] = 0.0f;
                    rotation[2] = 0.0f;
                    rotation[3] = 0.0f;
                }
                if (((fldSpawnCollisionView_t *)resource->data)->kind == 0) {
                    s32 room = fldCommand_GetHojiID(resource->name);
                    fldMakePlane(((fldSpawnCollisionView_t *)resource->data)->header, position,
                                  resource->id, room, -1);
                    spawnedObject = dds3AddModelRangeBasicNew(resource->id, resource->data, position, resource->name);
                    dds3SetPathBasicSpline(spawnedObject, pathIndex);
                }
            }
            break;
        }
    }
}

/* Resolved FL2 event and placement payloads, selected by the batch kind. */
typedef struct fldSpawnEventDataFL2_t {
    u32 kind;
    void *data;
} fldSpawnEventDataFL2_t;

typedef struct fldSpawnPointDataFL2_t {
    u32 kind;
    s32 eventIndex;
    u8 opaque08[4]; /* Not interpreted by this resource constructor. */
    void *parameters;
} fldSpawnPointDataFL2_t;

typedef struct fldSpawnSoundDataFL2_t {
    f32 radius;
    s32 soundId;
} fldSpawnSoundDataFL2_t;

typedef struct fldEventDataHeader_t fldEventDataHeader_t;
typedef struct fldResHead_t fldResHead_t;
extern dds3Basic_t *dds3AddEventBasic(s32 serialID, void *data, const char *name);
extern u32 fldEveHit_AddEveData(fldEventDataHeader_t *resource, dds3Basic_t *basic);
extern s32 fldEveHit_AddEveHit(fldResHead_t *resource, dds3Basic_t *basic);
extern dds3Basic_t *dds3AddPointBasic(u32 serialID, fldResourcePoint_t *data, const char *name);
extern void fldNpcLoad(s32 npcId, f32 x, f32 y, f32 z, const f32 *rotation, s32 room, const char *name, s32 eventIndex);
extern void fldItemBoxAdd(s32 itemId, f32 x, f32 y, f32 z, const f32 *rotation);
extern void fldAddSe3D(s32 soundId, f32 radius, f32 x, f32 y, f32 z);
extern dds3Basic_t *dds3AddCameraObjectBasicTypeFree(u32 serialID, const sdfVector4_t *position, const sdfVector4_t *rotation);

void fldMakeResouceBasicFL2(void *typeTable, u32 batchCount) {
    fldSpawnBatch_t *batch;
    f32 position[4];
    f32 rotation[4];
    u32 batchIndex;
    u32 resourceCount;
    u32 resourceIndex;
    fldFileResource_t *resource;
    dds3Basic_t *spawnedObject;
    u32 pathIndex;

    batch = (fldSpawnBatch_t *)typeTable;
    g_pHitData = NULL;
    g_nHitData = 0;
    for (batchIndex = 0; batchIndex < batchCount; batchIndex++, batch++) {
        resource = batch->entries;
        resourceCount = batch->count;
        switch (batch->kind) {
        case 0:
        case 1:
        case 2:
        case 5:
        case 8:
        case 11:
            break;
        case FLD_SPAWN_BATCH_KIND_COLLISION:
            g_pHitData = resource;
            g_nHitData = resourceCount;
            for (resourceIndex = 0, pathIndex = 1; resourceIndex < resourceCount; resourceIndex++, resource++, pathIndex++) {
                resource->id |= 0x10000;
                if (resource->transform != NULL) {
                    position[0] = resource->transform[0];
                    position[1] = resource->transform[1];
                    position[2] = resource->transform[2];
                    position[3] = 0.0f;
                    rotation[0] = resource->transform[4];
                    rotation[1] = resource->transform[5];
                    rotation[2] = resource->transform[6];
                    rotation[3] = resource->transform[7];
                } else {
                    position[0] = 0.0f;
                    position[1] = 0.0f;
                    position[2] = 0.0f;
                    position[3] = 0.0f;
                    rotation[0] = 0.0f;
                    rotation[1] = 0.0f;
                    rotation[2] = 0.0f;
                    rotation[3] = 0.0f;
                }
                if (((fldSpawnCollisionView_t *)resource->data)->kind == 0) {
                    s32 room = fldCommand_GetHojiID(resource->name);
                    fldMakePlane(((fldSpawnCollisionView_t *)resource->data)->header, position,
                                  resource->id, room, -1);
                    spawnedObject = dds3AddModelRangeBasicNew(resource->id, resource->data, position, resource->name);
                    dds3SetPathBasicSpline(spawnedObject, pathIndex);
                }
            }
            break;
        case FLD_SPAWN_BATCH_KIND_EVENT:
            for (resourceIndex = 0; resourceIndex < resourceCount; resourceIndex++, resource++) {
                spawnedObject = dds3AddEventBasic(resource->id, ((fldSpawnEventDataFL2_t *)resource->data)->data, resource->name);
                fldEveHit_AddEveData((fldEventDataHeader_t *)resource, spawnedObject);
            }
            break;
        case FLD_SPAWN_BATCH_KIND_PATH:
            for (resourceIndex = 0; resourceIndex < resourceCount; resourceIndex++, resource++) {
                dds3AddPathBasic(resource->id, resource->data, resource->name);
            }
            break;
        case FLD_SPAWN_BATCH_KIND_POINT:
            for (resourceIndex = 0; resourceIndex < resourceCount; resourceIndex++, resource++) {
                fldSpawnPointDataFL2_t *data;
                if (resource->transform != NULL) {
                    position[0] = resource->transform[0];
                    position[1] = resource->transform[1];
                    position[2] = resource->transform[2];
                    position[3] = 0.0f;
                    rotation[0] = resource->transform[4];
                    rotation[1] = resource->transform[5];
                    rotation[2] = resource->transform[6];
                    rotation[3] = resource->transform[7];
                } else {
                    position[0] = 0.0f;
                    position[1] = 0.0f;
                    position[2] = 0.0f;
                    position[3] = 0.0f;
                    rotation[0] = 0.0f;
                    rotation[1] = 0.0f;
                    rotation[2] = 0.0f;
                    rotation[3] = 0.0f;
                }
                data = resource->data;
                switch (data->kind) {
                case 0:
                    dds3AddPointBasic(resource->id, (fldResourcePoint_t *)resource->transform, resource->name);
                    break;
                case 1:
                    spawnedObject = dds3AddPointBasic(resource->id, (fldResourcePoint_t *)resource->transform, resource->name);
                    fldEveHit_AddEveHit((fldResHead_t *)resource, spawnedObject);
                    break;
                case 2: {
                    s32 npcId = *(s32 *)data->parameters;
                    s32 room = fldCommand_GetHojiID(resource->name);
                    fldNpcLoad(npcId, position[0], position[1], position[2], rotation, room,
                               resource->name, ((fldSpawnPointDataFL2_t *)resource->data)->eventIndex);
                    break;
                }
                case 3:
                    fldItemBoxAdd(*(s32 *)data->parameters, position[0], position[1], position[2], rotation);
                    break;
                case 7: {
                    fldSpawnSoundDataFL2_t *sound = data->parameters;
                    fldAddSe3D(sound->soundId, sound->radius, position[0], position[1], position[2]);
                    break;
                }
                }
            }
            break;
        case FLD_SPAWN_BATCH_KIND_CAMERA:
            for (resourceIndex = 0; resourceIndex < resourceCount; resourceIndex++, resource++) {
                if (resource->transform != NULL) {
                    position[0] = resource->transform[0];
                    position[1] = resource->transform[1];
                    position[2] = resource->transform[2];
                    position[3] = 0.0f;
                    rotation[0] = resource->transform[4];
                    rotation[1] = resource->transform[5];
                    rotation[2] = resource->transform[6];
                    rotation[3] = resource->transform[7];
                } else {
                    position[0] = 0.0f;
                    position[1] = 0.0f;
                    position[2] = 0.0f;
                    position[3] = 0.0f;
                    rotation[0] = 0.0f;
                    rotation[1] = 0.0f;
                    rotation[2] = 0.0f;
                    rotation[3] = 0.0f;
                }
                spawnedObject = dds3AddCameraObjectBasicTypeFree(resource->id, (const sdfVector4_t *)position,
                                                                (const sdfVector4_t *)rotation);
                ((dds3CameraObjectBasicWorkData_t *)spawnedObject->workdata)->fovy = *(f32 *)resource->data;
                dds3SetUnitObjectId(spawnedObject, (u32)resource->name);
            }
            break;
        case FLD_SPAWN_BATCH_KIND_UNSUPPORTED:
            /* Retail stops here for this unsupported resource class. */
            for (;;) {}
            break;
        }
    }
}


void fldMakeBlockBasicFL2(fldConstruntAreaFL2_t *pBlockTable, u32 nBlock) {
    u32 i;

    dds3GetControlScene();
    for (i = 0; i < nBlock; i++) {
        dds3Basic_t *area = dds3AddAreaBasic(pBlockTable[i].areaID, pBlockTable[i].areaName);
        if (i == 0) {
            fldGlobalBase.fastArea = area;
        }
    }
}

void fldMakeObjectBasic2(void) {
    f32 position[4];
    f32 rotation[4];
    fldFileResource_t *resource = D_00551C8C;
    u32 count = D_00551C90;

    if (resource != NULL) {
        s32 resourceIndex;
        dds3Basic_t *secondaryWorld = dds3GetControlScene();

        for (resourceIndex = 0; resourceIndex < count; resourceIndex++, resource++) {
            fldModelData_t *data = (fldModelData_t *)resource->data;
            dds3Basic_t *matter;
            u32 hitIndex;
            fldFileResource_t *hit;
            fldResourceAreaRef_t *areaRef;

            dds3AddModelBasic2(resource->id, data->modelDef, data->playBook, D_0054EB98, resource->name);
            if (resource->transform != NULL) {
                position[0] = resource->transform[0];
                position[1] = resource->transform[1];
                position[2] = resource->transform[2];
                position[3] = 0.0f;
                rotation[0] = resource->transform[4];
                rotation[1] = resource->transform[5];
                rotation[2] = resource->transform[6];
                rotation[3] = resource->transform[7];
            } else {
                position[0] = 0.0f;
                position[1] = 0.0f;
                position[2] = 0.0f;
                position[3] = 0.0f;
                rotation[0] = 0.0f;
                rotation[1] = 0.0f;
                rotation[2] = 0.0f;
                rotation[3] = 0.0f;
            }
            matter = dds3AddMatterObjectBasic(resource->id, position, rotation);
            dds3SetUnitObjectId(matter, (u32)resource->name);
            if (fldGlobalBase.area >= 200 && fldGlobalBase.area < 500) {
                switch (resourceIndex) {
                case 0:
                    dds3MatterObjectBasicSetOt(matter, 1);
                    break;
                case 1:
                    dds3MatterObjectBasicSetOt(matter, 2);
                    break;
                case 2:
                    dds3MatterObjectBasicSetOt(matter, 3);
                    break;
                case 3:
                    dds3MatterObjectBasicSetOt(matter, 4);
                    break;
                default:
                    dds3MatterObjectBasicSetOt(matter, 5);
                    break;
                }
            } else {
                dds3MatterObjectBasicSetOt(matter, 6);
            }
            dds3MatterObjectBasicSetHojiID(matter, fldCommand_GetHojiID(resource->name));
            dds3ObjectBaseSetControlBasic(matter, dds3SearchSceneBasic(secondaryWorld, resource->id, 10));
            dds3ObjectBaseMakeMatter(matter);
            hit = g_pHitData;
            for (hitIndex = 0; hitIndex < g_nHitData; hitIndex++, hit++) {
                if (hit->names != NULL) {
                    fldFileNameEntry_t *nameEntry = hit->names->entries;
                    u32 nameIndex = 0;

                    for (; nameIndex < hit->names->count; nameIndex++, nameEntry++) {
                        if (strcmp(resource->name, nameEntry->name) == 0) {
                            fldFindMatterIndexFrom(hit->id, matter);
                            break;
                        }
                    }
                }
            }
            areaRef = (fldResourceAreaRef_t *)resource->word14;
            if (matter != NULL) {
                dds3ObjectBaseSetControlBasic(matter,
                    dds3SceneBasicSearchNameAndType(secondaryWorld, 2, areaRef->name));
                dds3ObjectBaseRegistArea(matter);
            }
        }
    }
}

/* Restore the room's named objects and enable their retained path controllers. */
s32 fldMakeObjectBasic3(void) {
    char modelName[16];
    char collisionName[16];
    char pathName[16];
    void *world = dds3GetControlScene();
    s32 roomFlagIndex;

    for (roomFlagIndex = 1; roomFlagIndex < 16; roomFlagIndex++) {
        if (fldGetHojiFlag(fldGlobalBase.area, fldGlobalBase.floor + 1, roomFlagIndex)) {
            s32 modelKey, collisionKey;
            dds3Basic_t *object;
            dds3Basic_t *path;
            dds3ObjectBaseControlWorkData_t *data;
            dds3Basic_t *slot;
            fldMoverWork_t *resource;

            sprintf(modelName, "md_hoji_%02d", roomFlagIndex);
            sprintf(collisionName, "atari_hoji_%02d", roomFlagIndex);
            sprintf(pathName, "path_hoji_%02d", roomFlagIndex);
            modelKey = fldCommand_SEARCH_ID(modelName);
            collisionKey = fldCommand_SEARCH_ID(collisionName);
            if (modelKey == -1) {
                object = fldGetMatter_From_ModelRangeID(collisionKey);
            } else {
                object = (dds3Basic_t *)dds3SearchSceneBasic(world, modelKey, 6);
                if (object == NULL) {
                    object = fldGetMatter_From_ModelRangeID(collisionKey);
                }
            }
            path = (dds3Basic_t *)dds3SceneBasicSearchNameAndType(world, 0x10, (const u8 *)pathName);
            data = dds3MatterObjectBasicGetControlWorkData(object);
            slot = (dds3Basic_t *)data->pBasic[1];
            if (slot == NULL) {
                dds3ObjectBaseCreateMover(object);
                slot = (dds3Basic_t *)data->pBasic[1];
            }
            resource = (fldMoverWork_t *)slot->workdata;
            if (resource->sourceObject == 0) {
                dds3MoverBasicSetPath(slot, path);
                dds3MoverBasicMakePath(slot);
            }
            dds3PathBasicControlAlterMode(dds3MoverBasicGetPathControl(slot), 0);
            evtPathRate(object, 1.0f);
            dds3MoverBasicUpdate(slot);
        }
        if (fldGetModelFlag(fldGlobalBase.area, fldGlobalBase.floor + 1, roomFlagIndex)) {
            s32 key;
            dds3Basic_t *object;

            sprintf(modelName, "md_hoji_%02d", roomFlagIndex);
            key = fldCommand_SEARCH_ID(modelName);
            object = (dds3Basic_t *)dds3SearchSceneBasic(world, key, 6);
            dds3MatterObjectBasicSetAlpMode(object, 2);
        }
    }
}

void fldFileResolverFL1(fldFileInfoHeaderFL1_t *info)
{
    char directory[32];
    fldFileAreaDataFL1_t *areaData;
    char **files;
    char path[64];
    char name[24];
    s32 field;
    s32 i;
    s32 j;
    s32 inserted;
    char *filename;
    char *filename0;
    char *filename1;
    char *filename2;
    char *filename3;

    fldSetFileResolverIdx(info->pAreaData->SkyTblIdx);
    files = fldResolverFL1Data;
    areaData = info->pAreaData;
    field = fldGlobalBase.area;
    filename0 = areaData->filename0;
    filename1 = areaData->filename1;
    filename2 = areaData->filename2;
    filename3 = areaData->filename3;
    files[0] = filename0;
    files[1] = filename1;
    files[2] = filename2;
    files[3] = filename3;
    if (field == 24 && evtBitCheck(0x4F)) {
        for (i = 0; i < 4; i++) {
            inserted = 0;
            memset(name, 0, sizeof(name));
            filename = files[i];
            if (filename != NULL) {
                for (j = 0; j < 16 && filename[j] != '\0'; j++) {
                    if (filename[j] == '.') {
                        name[j + inserted] = 'b';
                        inserted = 1;
                    }
                    name[j + inserted] = filename[j];
                }
                strcpy(filename, name);
            }
        }
    }
    for (i = 0; i < 4; i++) {
        D_0055DA98[i] = NULL;
        D_0055DAA8[i] = NULL;
    }
    if (fldGlobalBase.area < 500) {
        for (i = 0; i < 4; i++) {
            if (fldResolverFL1Data[i] != NULL) {
                fldMakeFilePath(directory, fldGlobalBase.area, fldGlobalBase.floor + 1);
                sprintf(path, D_0054EBC0, directory, fldResolverFL1Data[i]);
                D_0055DA98[i] = sdfDevLoadFileEx(path, &D_0055DAA8[i], NULL);
            }
        }
    }
    fldMakeResouceBasicFL1(info->pResourceTypeTable, info->nResourceTypeTable);
}

void fldFileResolverFL2(fldFileInfoHeaderFL2_t *info) {
    fldMakeBlockBasicFL2(info->pAreaTable, info->nArea);
    fldMakeResouceBasicFL2(info->pResourceTypeTable, info->nResourceTypeTable);
    fldRetryFileId[0] = info->pAreaData->label;
}

u8 *fldGetInfoHeader(fldTransferChunk_t *pData) {
    return (u8 *)pData + sizeof(*pData);
}

void sdfAddressResolverReset(u32 *aDataBody, u32 aOffset, const u8 *aAddrInfoPtr, s32 aAddrInfoLeng) {
    const u8 *p = aAddrInfoPtr;
    s32 code;
    s32 i;

    while (p - aAddrInfoPtr < aAddrInfoLeng) {
        code = *p++;
        if ((code & 1) == 0) {
            code >>= 1;
        } else if ((code & 2) == 0) {
            code = (code | (*p++ << 8)) >> 2;
        } else if ((code & 4) == 0) {
            code = (code | (p[0] << 8) | (p[1] << 16)) >> 3;
            p += 2;
        } else {
            code = (code >> 3) + 2;
            for (i = 0; i < code; i++) {
                aDataBody++;
                *aDataBody -= aOffset;
            }
            continue;
        }
        aDataBody += code;
        *aDataBody -= aOffset;
    }
}

/* Relocate the words described by this packed-resource transfer chunk. */
void fldAddressResolver(u32 pFileTop, fldTransferChunk_t *pData) {
    sdfAddressResolver((s32 *)pFileTop, (s32)pFileTop, (const u8 *)((s32)pFileTop + pData->offset), pData->size);
}

void fldAddressResolverReset(u32 pFileTop, fldTransferChunk_t *pData) {
    sdfAddressResolverReset((u32 *)pFileTop, pFileTop, (u8 *)((s32)pFileTop + pData->offset), pData->size);
}

void fldLoadNextFile_SetId_BtlRet(u32 field, u32 area) {
    D_0054EBF4 = field;
    D_0054EBF8 = area;
}

s32 fldLoadNextFile_BtlRet(void) {
    char directory[64];
    char path[80];
    u32 area = D_0054EBF4;
    u32 floor = D_0054EBF8;

    if (area != 0 || floor != 0) {
        fldFreeNextFile();
        fldGlobalBase.resourceArea = area;
        fldGlobalBase.resourceFloor = floor;
        fldMakeFilePath(directory, area, 1);
        sprintf(path, "%sf%03d_%03d.LB", directory, area, floor);
        pFldNextFileDataF2 = pacLoad(path);
        fldGlobalBase.resourceFlag = 1;
        return 1;
    }
    return 0;
}

s32 fldLoadNextFile(s32 field, s32 area) {
    char directory[64];
    char path[80];

    if (field < 10) {
        return 1;
    }
    if (fldGlobalBase.area != field) {
        return 1;
    }
    if (fldGlobalBase.resourceFlag == 1) {
        return 1;
    }
    if (fldGlobalBase.resourceArea == field && fldGlobalBase.resourceFloor == area) {
        fldGlobalBase.resourceFlag = 2;
        return 1;
    }
    fldFreeNextFile();
    fldGlobalBase.resourceArea = field;
    fldGlobalBase.resourceFloor = area;
    fldMakeFilePath(directory, field, 1);
    sprintf(path, "%sf%03d_%03d.LB", directory, field, area);
    pFldNextFileDataF2 = pacLoad(path);
    fldGlobalBase.resourceFlag = 1;
    return 1;
}

void fldFreeNextFile(void) {
    if (pFldNextFileDataF2 != 0) {
        sdfPacDecodeResrc_t *work = ((filePacRequest_t *)pFldNextFileDataF2)->packet.resrcHead;

        if (work != NULL) {
            do {
                sdfMemFreeDelayedDirect((sdfMemHandle_t *)work->resrc);
                work = work->next;
            } while (work != NULL);
        }
        pacFileHandleFree_(pFldNextFileDataF2);
        pFldNextFileDataF2 = NULL;
    }
    D_0054EBF4 = 0;
    D_0054EBF8 = 0;
    fldGlobalBase.resourceArea = 0;
    fldGlobalBase.resourceFloor = 0;
    fldGlobalBase.resourceFlag = 0;
}

s32 fldLoadNextTask(void) {
    if (fldGlobalBase.resourceFlag != 0) {
        if (fldGlobalBase.resourceFlag == 1) {
            if (pacCheckEnd(pFldNextFileDataF2) != 0) {
                fldGlobalBase.resourceFlag = 0;
            }
        }
    }
    return 0;
}

u32 fldLoadNextGetStatus(void) {
    return fldLoadNextStatus[0];
}

u8 fldLoadNextGetStatus2(void) {
    if (pFldNextFileDataF2 != 0) {
        if (pacCheckEnd(pFldNextFileDataF2) != 0) {
            return 1;
        }
    }
    return fldLoadNextStatus[0] != 0;
}

s32 fldLoadNextGetStatus3(s32 field, s32 area) {
    if (fldGlobalBase.resourceArea != field || fldGlobalBase.resourceFloor != area) {
        return 0;
    }
    if (pFldNextFileDataF2 != 0 && pacCheckEnd(pFldNextFileDataF2) != 0) {
        return 1;
    }
    return fldGlobalBase.resourceFlag != 0;
}

sdfMemHandle_t *fldLoadNextFileTB(void **pData, s32 field, s32 area) {
    if (fldGlobalBase.resourceArea == field) {
        if (fldGlobalBase.resourceFloor == area) {
            sdfMemHandle_t *allocation = sdfMemAllocDirect(D_0054EBE8);
            void *data = (void *)sdfMemLock(allocation);
            *pData = data;
            memcpy(data, (void *)D_0054EBDC, D_0054EBE8);
            return allocation;
        }
    }
    return NULL;
}

sdfMemHandle_t *fldLoadNextFileF1(void **pData, s32 field, s32 area) {
    if (fldGlobalBase.resourceArea == field) {
        if (fldGlobalBase.resourceFloor == area) {
            sdfMemHandle_t *allocation = sdfMemAllocDirect(D_0054EBEC);
            void *data = (void *)sdfMemLock(allocation);
            *pData = data;
            memcpy(data, (void *)D_0054EBE0, D_0054EBEC);
            return allocation;
        }
    }
    return NULL;
}

sdfMemHandle_t *fldLoadNextFileF2(void **pData, s32 field, s32 area) {
    if (fldGlobalBase.resourceArea == field) {
        if (fldGlobalBase.resourceFloor == area) {
            sdfMemHandle_t *allocation = sdfMemAllocDirect(D_0054EBF0);
            void *data = (void *)sdfMemLock(allocation);
            *pData = data;
            memcpy(data, (void *)D_0054EBE4, D_0054EBF0);
            return allocation;
        }
    }
    return NULL;
}

void fldPreloadNextFile(char *out) {
    char directory[32];
    s32 area = fldGlobalBase.area;
    s32 floor = fldGlobalBase.floor + 1;

    fldMakeFilePath(directory, area, 1);
    if (area == 25) {
        if (evtBitCheck(0x599) != 0 && evtBitCheck(0x743) == 0) {
            sprintf(out, "%sf%03d_00c.LB", directory, 25);
            return;
        }
    }
    if (area == 25) {
        if (floor >= 0x11) {
            sprintf(out, "%sf%03d_00b.LB", directory, 25);
            return;
        }
    }
    if (area == 24) {
        if (evtBitCheck(0x4F) != 0) {
            sprintf(out, "%sf%03d_00b.LB", directory, 24);
            return;
        }
        if (evtBitCheck(0x16) != 0 && evtBitCheck(0x3E) == 0) {
            sprintf(out, "%sf%03d_00c.LB", directory, 24);
            return;
        }
    }
    if (area == 34) {
        if (floor < 4) {
            sprintf(out, "%sf%03d_000.LB", directory, 34);
            return;
        }
        if (floor < 0xB) {
            sprintf(out, "%sf%03d_00b.LB", directory, 34);
            return;
        }
        if (floor < 0x13) {
            sprintf(out, "%sf%03d_00c.LB", directory, 34);
            return;
        }
        if (floor < 0x1A) {
            sprintf(out, "%sf%03d_00d.LB", directory, 34);
            return;
        }
    }
    sprintf(out, "%sf%03d_000.LB", directory, area);
}

u8 fldCheckNextFile(void) {
    char buf[32];

    fldPreloadNextFile(buf);
    return strcmp(fldFieldPackFlag, buf) != 0;
}

void fldLoadFieldPack(void) {
    char name[32];
    filePacRequest_t *request;
    sdfPacDecodeResrc_t *work;

    if (fldGlobalBase.area < 200) {
        fldPreloadNextFile(name);
        strcpy(fldFieldPackFlag, name);
        gFldFileF1 = pacLoad(name);
        pacSync(gFldFileF1);
        request = (filePacRequest_t *)gFldFileF1;
        for (work = request->packet.resrcHead; work != NULL; work = work->next) {
            switch (((sdfPacStdBlockHeader_t *)work->header)->kind) {
            case FLD_PACKED_RESOURCE_INFO_TABLE:
                fldInfLoadFromMem(work->data);
                sdfMemFreeDelayedDirect(
                    (sdfMemHandle_t *)work->resrc);
                break;
            case FLD_PACKED_RESOURCE_NPC_PALETTE:
                fldGetEAEntry((const fldNpcPalette_t *)work->data);
                sdfMemFreeDelayedDirect(
                    (sdfMemHandle_t *)work->resrc);
                break;
            case FLD_PACKED_RESOURCE_SKY_BUFFER:
                fldGetSkyEntry((fldSkyBuffer_t *)work->data);
                sdfMemFreeDelayedDirect(
                    (sdfMemHandle_t *)work->resrc);
                break;
            case FLD_PACKED_RESOURCE_ACTOR_WAYPOINT_TABLE:
                fldGetDoorEntry(work->data);
                sdfMemFreeDelayedDirect(
                    (sdfMemHandle_t *)work->resrc);
                break;
            case FLD_PACKED_RESOURCE_RETAINED_COPY:
                fldFieldPackData = sdfMemAllocDirect(
                    sdfMemGetSize((sdfMemHandle_t *)work->resrc));
                memcpy((void *)sdfMemGetPtr(fldFieldPackData),
                       (void *)sdfMemGetPtr((sdfMemHandle_t *)work->resrc),
                       sdfMemGetSize((sdfMemHandle_t *)work->resrc));
                sdfMemFreeDelayedDirect(
                    (sdfMemHandle_t *)work->resrc);
                break;
            case FLD_PACKED_RESOURCE_SCENE_RECORD_CHUNK:
                fldAmbLoadFromMem(work->data, (s32)work->resrc);
                break;
            }
        }
        fldAutoMapFontInit(fldGlobalBase.area % 100);
    }
}

void fldFreePac0(void) {
    sdfMemHandle_t *cachedResource = fldFieldPackData;

    if (cachedResource != NULL) {
        sdfMemFreeDelayedDirect(cachedResource);
        fldFieldPackData = NULL;
    }
    if (gFldFileF1 != NULL) {
        pacFileHandleFree_(gFldFileF1);
        gFldFileF1 = NULL;
    }
    fldFieldPackFlag[0] = D_0054EC00[0];
}

u32 fldLoadFile(const char *name) {
    char path[128];
    char directory[64];
    void *textureData;
    void *fileData;
    s32 field;
    s32 area;
    s32 index;
    s32 resident;
    sdfPacDecodeResrc_t *work;
    fldTransferChunk_t *chunk;

    gFldFileResetMode = 0;
    D_0054EBD4 = 0;
    fldFileBuf2 = 0;
    gFldInf_WpExit = 0;
    field = dds3GetMapNo(dds3GetControlScene()) >> 16;
    area = dds3GetMapNo(dds3GetControlScene()) & 0xFFFF;
    fldAutoMapSetField(field, area - 1, 0);
    if (fldGlobalBase.area < 200) {
        fldPlayerALoad();
    }
    if (!((u32)(fldGlobalBase.area - 200) < 300)) {
        fldDrwZAlloc();
    }
    fldClearMatterFlags();
    fldClearEveRngTable();
    fldSe_Init();
    fldClearModelSlots();
    fldClearTakaraSlots();
    if (fldGlobalBase.reuseAreaResources == 0) {
        fldInitBlurEffect();
        fldLoadFieldPack();
    } else if (fldCheckNextFile() != 0) {
        fldAutoMapFree();
        fldAutoMapFileFree();
        fldFreePac0();
        fldLoadFieldPack();
    }
    resident = (u32)(fldGlobalBase.area - 200) < 300;
    if (fldGlobalBase.area < 100) {
        resident = 1;
    }
    if (resident != 0) {
        if (fldLoadNextGetStatus3(field, area) != 0) {
            fldFileBuf1 = pFldNextFileDataF2;
            pFldNextFileDataF2 = NULL;
            fldFreeNextFile();
        } else {
            fldMakeFilePath(directory, field, 1);
            sprintf(path, "%sf%03d_%03d.LB", directory, field, area);
            fldFileBuf1 = pacLoad(path);
            pacSync(fldFileBuf1);
        }
        D_0054EB98 = 0;
        D_0054EBD0 = 0;
        index = 0;
        work = ((filePacRequest_t *)fldFileBuf1)->packet.resrcHead;
        if (work != NULL) {
            do {
                switch (index) {
                case 0:
                    D_0054EB98 = sdfTexSetTB(work->data);
                    sdfMemFreeDelayedDirect((sdfMemHandle_t *)work->resrc);
                    D_0054EBD0 = 0;
                    break;
                case 1:
                    fldFileBuf2 = 0;
                    fileData = work->data;
                    chunk = (fldTransferChunk_t *)((u8 *)fileData + 8);
                    mat_line = fileData;
                    mat_white = (u8 *)chunk;
                    fldAddressResolver((u32)fileData, chunk);
                    fldFileResolverFL2((fldFileInfoHeaderFL2_t *)fldGetInfoHeader(chunk));
                    break;
                case 2:
                    D_0054EBD4 = 0;
                    fileData = work->data;
                    chunk = (fldTransferChunk_t *)((u8 *)fileData + 8);
                    mat_black = fileData;
                    FildMaskObj = (u8 *)chunk;
                    fldAddressResolver((u32)fileData, chunk);
                    fldFileResolverFL1(fldGetInfoHeader(chunk));
                    break;
                }
                work = work->next;
                index++;
            } while (work != NULL);
        }
    }
    if (resident == 0) {
        D_0054EBD0 = (u32)fldLoadNextFileTB(&textureData, field, area);
        if (D_0054EBD0 == 0) {
            sprintf(path, D_0054EC08, name);
            D_0054EB98 = sdfDevLoadTexList(path);
        } else {
            D_0054EB98 = sdfTexSetTB(textureData);
            sdfMemFreeDelayedDirect((sdfMemHandle_t *)D_0054EBD0);
            D_0054EBD0 = 0;
        }
    }
    if (resident == 0) {
        sprintf(path, D_0054EC10, name);
        fldFileBuf2 = (u32)fldLoadNextFileF2(&fileData, field, area);
        if (fldFileBuf2 == 0) {
            fldFileBuf2 = sdfDevLoadFileEx(path, &fileData, 0);
        }
        chunk = (fldTransferChunk_t *)((u8 *)fileData + 8);
        mat_line = fileData;
        mat_white = (u8 *)chunk;
        fldAddressResolver((u32)fileData, chunk);
        fldFileResolverFL2((fldFileInfoHeaderFL2_t *)fldGetInfoHeader(chunk));
        sprintf(path, D_0054EC18, name);
        D_0054EBD4 = (u32)fldLoadNextFileF1(&fileData, field, area);
        if (D_0054EBD4 == 0) {
            D_0054EBD4 = sdfDevLoadFileEx(path, &fileData, 0);
        }
        chunk = (fldTransferChunk_t *)((u8 *)fileData + 8);
        mat_black = fileData;
        FildMaskObj = (u8 *)chunk;
        fldAddressResolver((u32)fileData, chunk);
        fldFileResolverFL1(fldGetInfoHeader(chunk));
    }
    fldMakeObjectBasic2();
    fldMakeObjectBasic3();
    if (fldSceneParam[22] != 0 && field < 200 && fldFieldPackData != NULL) {
        u8 *destination;
        u8 *source;

        D_0054EBCC = (u32)sdfMemAllocDirect(sdfMemGetSize(fldFieldPackData));
        destination = sdfMemGetPtr((sdfMemHandle_t *)D_0054EBCC);
        source = sdfMemGetPtr(fldFieldPackData);
        memcpy(destination, source, sdfMemGetSize(fldFieldPackData));
        dds3SceneBasicLoadScriptFile_FromMem(dds3GetControlScene(), (void *)D_0054EBCC);
    }
    fldSetupDoorScenes();
    fldGlobalBase.reuseAreaResources = 0;
    return D_0054EBD4;
}

u32 fldLoadFile_FromMemory(s32 field, s32 area, u8 *fl1Buffer, u8 *fl2Buffer, void *textureBuffer) {
    fldTransferChunk_t *chunk;

    gFldFileResetMode = 0;
    D_0054EBD4 = 0;
    fldFileBuf2 = 0;
    D_0054EBD0 = 0;
    D_0054EB98 = 0;
    if (field != 800) {
        fldDrwZAlloc();
    }
    fldAutoMapSetField(field, area - 1, 0);
    if (textureBuffer != NULL) {
        D_0054EB98 = sdfTexSetTB(textureBuffer);
    }
    chunk = (fldTransferChunk_t *)(fl2Buffer + 8);
    mat_white = (u8 *)chunk;
    mat_line = fl2Buffer;
    fldAddressResolver((u32)fl2Buffer, chunk);
    fldFileResolverFL2((fldFileInfoHeaderFL2_t *)fldGetInfoHeader(chunk));
    if (fl1Buffer != NULL) {
        fl2Buffer = fl1Buffer;
    }
    chunk = (fldTransferChunk_t *)(fl2Buffer + 8);
    FildMaskObj = (u8 *)chunk;
    mat_black = fl2Buffer;
    fldAddressResolver((u32)fl2Buffer, chunk);
    fldFileResolverFL1(fldGetInfoHeader(chunk));
    fldMakeObjectBasic2();
    fldJDataHandle[0] = 0;
    return D_0054EBD4;
}

void fldFreeFile(void) {
    s32 i;
    filePacRequest_t *request;
    sdfPacDecodeResrc_t *work;

    if (dds3SearchProcessName("PipeMain") != 0) {
        return;
    }
    fldDrwZFree();
    fldClearSe3D();
    if (fldTitleChk() != 0) {
        dds3KillProcessName("fldTitle", 1);
    }
    if (fldTitleMiniChk() != 0) {
        dds3KillProcessName("fldTitleMini", 1);
    }
    fldFreeModelTable();
    fldClearTakaraSlots();
    fldFreeNpcModel();
    if (fldGlobalBase.area < 200) {
        if (fldGlobalBase.reuseAreaResources == 0) {
            fldAutoMapFree();
            fldAutoMapFileFree();
            fldFreeBlurEffect();
            fldFreePac0();
        }
        fldClearMatterFlags();
        fldClearEveRngTable();
        fldSe_Init();
        fldClearModelSlots();
    }
    sdfTexListFree(D_0054EB98, 1);
    D_0054EB98 = 0;
    for (i = 0; i < 4; i++) {
        if (D_0055DA98[i] != NULL) {
            sdfMemFreeDelayedDirect(D_0055DA98[i]);
            D_0055DA98[i] = NULL;
            D_0055DAA8[i] = NULL;
        }
    }
    if (gFldFileResetMode != 0) {
        if (FildMaskObj != NULL) {
            fldAddressResolverReset((u32)mat_black, (fldTransferChunk_t *)FildMaskObj);
            FildMaskObj = NULL;
            mat_black = NULL;
        }
        if (mat_white != NULL) {
            fldAddressResolverReset((u32)mat_line, (fldTransferChunk_t *)mat_white);
            mat_white = NULL;
            mat_line = NULL;
        }
    }
    D_0054EBCC = 0;
    if (D_0054EBD0 != 0) {
        sdfMemFreeDelayedDirect((sdfMemHandle_t *)D_0054EBD0);
        D_0054EBD0 = 0;
    }
    if (D_0054EBD4 != 0) {
        sdfMemFreeDelayedDirect((sdfMemHandle_t *)D_0054EBD4);
        D_0054EBD4 = 0;
    }
    if (fldFileBuf2 != 0) {
        sdfMemFreeDelayedDirect((sdfMemHandle_t *)fldFileBuf2);
        fldFileBuf2 = 0;
    }
    if (fldGlobalBase.area < 200) {
        if (fldGlobalBase.word10 != fldGlobalBase.area) {
            fldGlobalBase.word10 = fldGlobalBase.area;
        }
        if (fldGlobalBase.previousFloor != fldGlobalBase.floor) {
            fldGlobalBase.previousFloor = fldGlobalBase.floor;
        }
    }
    request = (filePacRequest_t *)fldFileBuf1;
    if (request != NULL) {
        work = request->packet.resrcHead;
        i = 0;
        if (work != NULL) {
            do {
                if (i > 0) {
                    sdfMemFreeDelayedDirect((sdfMemHandle_t *)work->resrc);
                }
                work = work->next;
                i++;
            } while (work != NULL);
        }
        pacFileHandleFree_(fldFileBuf1);
        fldFileBuf1 = NULL;
    }
}

/* Select the ordinary field, battle or event resource directory. */
void fldMakeFilePath(char *pathstr, s32 field, s32 area) {
    if (field < 200) sprintf(pathstr, "/fld/f/f%03d/", field);
    else if (field < 500) sprintf(pathstr, "/fld/b/f%03d/", field);
    else sprintf(pathstr, "/fld/e/f%03d/", field);
}

void fldHojiDebugView(void) {
    char buf[32];
    s32 i;
    s32 j;
    s32 x1;
    s32 x2;
    s32 flag;

    sprintf(buf, "GMC-HOJI");
    fldDbgPrint2D(0, -800, buf);
    sprintf(buf, "HIT-HOJI");
    fldDbgPrint2D(3520, -800, buf);
    sprintf(buf, "0123456789ABCDEF");
    fldDbgPrint2D(0, -704, buf);
    fldDbgPrint2D(3520, -704, buf);
    for (i = 0; i < 31; i++) {
        sprintf(buf, D_0054EC20, i + 1);
        fldDbgPrint2D(-512, ((i * 12) - 76) << 3, buf);
        x2 = 3520;
        x1 = 0;

        for (j = 0; j < 16; j++) {
            flag = fldGetHojiFlag(fldGlobalBase.area, i + 1, j);
            if (flag == 0) {
                sprintf(buf, D_0054EC28);
            } else {
                sprintf(buf, D_0054EC30, flag);
            }

            fldDbgPrint2D(x1, ((i * 12) - 76) << 3, buf);
            x1 += 0xC0;
            flag = fldGbChkHit(fldGlobalBase.area, i + 1, j);
            if (flag == 0) {
                sprintf(buf, D_0054EC28);
            } else {
                sprintf(buf, D_0054EC30, flag);
            }

            fldDbgPrint2D(x2, ((i * 12) - 76) << 3, buf);
            x2 += 0xC0;
        }
    }
}

void fldHojiDebugView2(void) {
    char buf[32];
    s32 i;
    s32 j;
    s32 x1;
    s32 x2;
    s32 flag;

    sprintf(buf, "VIS-HOJI");
    fldDbgPrint2D(0, -800, buf);
    sprintf(buf, "ANM-HOJI");
    fldDbgPrint2D(3520, -800, buf);
    sprintf(buf, "0123456789ABCDEF");
    fldDbgPrint2D(0, -704, buf);
    fldDbgPrint2D(3520, -704, buf);
    for (i = 0; i < 31; i++) {
        sprintf(buf, D_0054EC20, i + 1);
        fldDbgPrint2D(-512, ((i * 12) - 76) << 3, buf);
        x2 = 3520;
        x1 = 0;

        for (j = 0; j < 16; j++) {
            flag = fldGetModelFlag(fldGlobalBase.area, i + 1, j);
            if (flag == 0) {
                sprintf(buf, D_0054EC28);
            } else {
                sprintf(buf, D_0054EC30, flag);
            }

            fldDbgPrint2D(x1, ((i * 12) - 76) << 3, buf);
            x1 += 0xC0;
            flag = fldGetAnimeFlag(fldGlobalBase.area, i + 1, j);
            if (flag == 0) {
                sprintf(buf, D_0054EC28);
            } else {
                sprintf(buf, D_0054EC30, flag);
            }

            fldDbgPrint2D(x2, ((i * 12) - 76) << 3, buf);
            x2 += 0xC0;
        }
    }
}

/* Enable or skip relocation of remaining transfer chunks during field teardown. */
void fldFileAddrResetMode(u32 mode) {
    gFldFileResetMode = mode;
}
INCLUDE_SDATA(const s32, "field/fldFileResolver", D_0054EB98);

INCLUDE_SDATA(const s32, "field/fldFileResolver", FildMaskObj);

INCLUDE_SDATA(const s32, "field/fldFileResolver", mat_black);

INCLUDE_SDATA(const s32, "field/fldFileResolver", mat_white);

INCLUDE_SDATA(const s32, "field/fldFileResolver", mat_line);

INCLUDE_SDATA(const s32, "field/fldFileResolver", gFldFileResetMode);

INCLUDE_SDATA(const s32, "field/fldFileResolver", gFldFileF1);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldFileBuf1);

INCLUDE_SDATA(const s32, "field/fldFileResolver", pFldNextFileDataF2);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_0054EBBC);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_0054EBC0);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldFieldPackData);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_0054EBCC);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_0054EBD0);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_0054EBD4);

INCLUDE_SDATA(const s32, "field/fldFileResolver", fldFileBuf2);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_0054EBDC);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_0054EBE0);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_0054EBE4);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_0054EBE8);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_0054EBEC);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_0054EBF0);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_0054EBF4);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_0054EBF8);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_0054EC00);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_0054EC08);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_0054EC10);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_0054EC18);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_0054EC20);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_0054EC28);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_0054EC30);

INCLUDE_SDATA(const s32, "field/fldFileResolver", rsetFrameMaskOtOffset);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_0054EC42);

