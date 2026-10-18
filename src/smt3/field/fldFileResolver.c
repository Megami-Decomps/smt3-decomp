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
    u8 kind; /* 0x00 */ u8 state; /* 0x01 */ u8 retryCount; /* 0x02 */ u8 allocationMode; /* 0x03: a callback request's mode */ struct filePacRequest_t *next; /* 0x04 */ char *name; /* 0x08: the duplicated request name */ void *deviceRequest; /* 0x0C */ s32 transferBytes; /* 0x10: a value-pair request's byte count */ s32 totalBytes; /* 0x14 */ void * completionCallback; /* 0x18 */ void *completionArg; /* 0x1C */ sdfMemHandle_t *allocationHandle; /* 0x20: descriptor passed to sdfResourceRetainAddress */ u32 retainedAddress; /* 0x24: a value-pair request's source data */ u32 transferAddress; /* 0x28 */ struct filePacRequest_t *completionNext; /* 0x2C */ sdfPacState_t packet; /* 0x30: PAC requests */ u16 readinessEnabled; /* 0x68: a gate on completion, not a state value */ u16 slot; /* 0x6A: the device-read slot */ struct filePacRequest_t *callbackNext; /* 0x6C: PAC completion callback queue link */
};


dds3Basic_t *fldMakeDmyMatter(void);

extern dds3Basic_t *dds3AddMatterObjectBasic(s32 key, f32 *offset, f32 *rotation);

extern u32 dds3AllocObjectId(void);

extern void dds3SetUnitObjectId(dds3Basic_t *node, u32 value);

dds3Basic_t *fldGetMatter_From_ModelRangeID(u32 resourceId);

/* Return the borrowed secondary world node, or NULL when no world is active. */
dds3Basic_t *dds3GetControlScene(void);

extern fldFileResource_t *g_pHitData;

extern u32 g_nHitData;

extern dds3Basic_t *dds3SceneBasicSearchNameAndType(dds3Basic_t *world, s32 slotIndex, const u8 *name);

/* Read or update the cached world object associated with a resource record ID. */
dds3Basic_t *fldFindMatterIndex(s32 id);

extern void fldFindMatterIndexFrom(s32 id, dds3Basic_t *worldNode);

dds3Basic_t *fldGetModelRange_From_MatterName(const char *resourceEntryName);

extern s32 strcmp(const char *left, const char *right);

void fldMakeFilePath(char *path, s32 field, s32 unused);


extern void sprintf(char *dst, const char *format, ...);

void fldMakeObjectBasic3(void);

extern s32 fldGetHojiFlag(s32 mapId, u32 slotIndex, s32 bit);

extern s32 fldCommand_SEARCH_ID(const char *);

extern dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *world, u32 key, s32 kind);


extern void dds3ObjectBaseCreateMover(void *object);

extern void dds3MoverBasicSetPath(dds3Basic_t *object, dds3Basic_t *sourceObject);

extern void dds3MoverBasicMakePath(dds3Basic_t *object);


extern dds3PathBasicControl_t *dds3MoverBasicGetPathControl(dds3Basic_t *object);

extern void evtPathRate(dds3Basic_t *object, f32 multiplier);

extern void dds3MoverBasicUpdate(dds3Basic_t *);

extern s32 fldGetModelFlag(s32 mapId, u32 slotIndex, s32 bit);

extern void dds3MatterObjectBasicSetAlpMode(dds3Basic_t *, u32);

u8 *fldGetInfoHeader(fldTransferChunk_t *chunk);

void sdfAddressResolverReset(u32 *table, u32 base, const u8 *data, s32 size);

void fldAddressResolver(u32 buffer, fldTransferChunk_t *chunk);

extern void sdfAddressResolver(s32 *words, s32 relocationBase, const u8 *table, s32 tableBytes);

void fldAddressResolverReset(u32 buffer, fldTransferChunk_t *chunk);

void fldLoadNextFile_SetId_BtlRet(u32 area, u32 floor);

extern u32 D_0054EBF4;

extern u32 D_0054EBF8;

s32 fldLoadNextFile_BtlRet(void);

void fldFreeNextFile(void);

extern char D_00531A40[]; /* "%sf%03d_%03d.LB" */

extern struct fileRequest_t *pFldNextFileDataF2;

struct fileRequest_t *pacLoad(const char *requestName);

extern void sdfMemFreeDelayedDirect(sdfMemHandle_t *allocation);

extern s32 pacFileHandleFree_(void *request);

u8 fldLoadNextGetStatus2(void);

extern s32 pacCheckEnd(const struct fileRequest_t *request);

s32 fldLoadNextGetStatus3(s32 area, s32 room);

sdfMemHandle_t *fldLoadNextFileTB(void **destination, s32 area, s32 room);

sdfMemHandle_t *sdfMemAllocDirect(s32 requestedBytes);

extern u32 sdfMemLock(sdfMemHandle_t *allocation);

extern void *memcpy(void *destination, const void *source, u32 size);

sdfMemHandle_t *fldLoadNextFileF1(void **destination, s32 area, s32 room);

sdfMemHandle_t *fldLoadNextFileF2(void **destination, s32 area, s32 room);

extern u32 D_0054EBE8, D_0054EBEC, D_0054EBEC, D_0054EBF0;

extern u32 D_0054EBDC, D_0054EBE0, D_0054EBE0, D_0054EBE4;

u8 fldCheckNextFile(void);

extern void fldPreloadNextFile(char *out);

extern char fldFieldPackFlag[];

void fldLoadFieldPack(void);

extern char * strcpy(char *, const char *);

extern struct fileRequest_t *gFldFileF1;

extern void pacSync(struct fileRequest_t *request);

extern void fldInfLoadFromMem(const void *);

extern void fldGetEAEntry(const fldNpcPalette_t *source);

extern void fldGetSkyEntry(fldSkyBuffer_t *source);

extern void fldGetDoorEntry(const void *source);

extern sdfMemHandle_t *fldFieldPackData;

extern s32 sdfMemGetSize(const sdfMemHandle_t *block);

extern u8 *sdfMemGetPtr(const sdfMemHandle_t *block);

extern void fldAmbLoadFromMem(u8 *chunk, s32 resourceHandle);

extern void fldAutoMapFontInit(s32 mapIndex);

void fldFreePac0(void);

extern u8 D_0054EC00[];

void fldFileAddrResetMode(u32 relocateOnRelease);

extern u32 gFldFileResetMode;

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

dds3Basic_t *fldGetMatter_From_ModelRangeID(u32 resourceId) {
    u32 resourceIndex = 0;
    dds3Basic_t *secondaryWorld = dds3GetControlScene();
    fldFileResource_t *resource = g_pHitData;

    for (; resourceIndex < g_nHitData; resourceIndex++, resource++) {
        if (resourceId == resource->id) {
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

dds3Basic_t *fldGetModelRange_From_MatterName(const char *resourceEntryName) {
    u32 resourceIndex = 0;
    dds3Basic_t *secondaryWorld = dds3GetControlScene();
    fldFileResource_t *resource = g_pHitData;

    for (; resourceIndex < g_nHitData; resourceIndex++, resource++) {
        if (resource->names != NULL) {
            fldFileNameEntry_t *nameEntry = resource->names->entries;
            u32 nameIndex = 0;

            for (; nameIndex < resource->names->count; nameIndex++, nameEntry++) {
                if (strcmp(resourceEntryName, nameEntry->name) == 0) {
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

INCLUDE_ASM(const s32, "field/fldFileResolver", fldMakeResouceBasicFL1);

INCLUDE_ASM(const s32, "field/fldFileResolver", fldMakeResouceBasicFL2);

INCLUDE_ASM(const s32, "field/fldFileResolver", fldMakeBlockBasicFL2);

INCLUDE_ASM(const s32, "field/fldFileResolver", fldMakeObjectBasic2);

/* Restore the room's named objects and enable their retained path controllers. */
void fldMakeObjectBasic3(void) {
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
            dds3ObjectBaseControl_t *data;
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
            slot = (dds3Basic_t *)data->control[1];
            if (slot == NULL) {
                dds3ObjectBaseCreateMover(object);
                slot = (dds3Basic_t *)data->control[1];
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

INCLUDE_ASM(const s32, "field/fldFileResolver", fldFileResolverFL1);

INCLUDE_ASM(const s32, "field/fldFileResolver", fldFileResolverFL2);

u8 *fldGetInfoHeader(fldTransferChunk_t *chunk) {
    return (u8 *)chunk + sizeof(*chunk);
}

void sdfAddressResolverReset(u32 *table, u32 base, const u8 *data, s32 size) {
    const u8 *p = data;
    s32 code;
    s32 i;

    while (p - data < size) {
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
                table++;
                *table -= base;
            }
            continue;
        }
        table += code;
        *table -= base;
    }
}

/* Relocate the words described by this packed-resource transfer chunk. */
void fldAddressResolver(u32 buffer, fldTransferChunk_t *chunk) {
    sdfAddressResolver((s32 *)buffer, (s32)buffer, (const u8 *)((s32)buffer + chunk->offset), chunk->size);
}

void fldAddressResolverReset(u32 buffer, fldTransferChunk_t *chunk) {
    sdfAddressResolverReset((u32 *)buffer, buffer, (u8 *)((s32)buffer + chunk->offset), chunk->size);
}

void fldLoadNextFile_SetId_BtlRet(u32 area, u32 floor) {
    D_0054EBF4 = area;
    D_0054EBF8 = floor;
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
        sprintf(path, D_00531A40, directory, area, floor);
        pFldNextFileDataF2 = pacLoad(path);
        fldGlobalBase.resourceFlag = 1;
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "field/fldFileResolver", fldLoadNextFile);

void fldFreeNextFile(void) {
    if (pFldNextFileDataF2 != 0) {
        sdfPacWork_t *work = ((filePacRequest_t *)pFldNextFileDataF2)->packet.head;

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

INCLUDE_ASM(const s32, "field/fldFileResolver", fldLoadNextTask);

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

s32 fldLoadNextGetStatus3(s32 area, s32 room) {
    if (fldGlobalBase.resourceArea != area || fldGlobalBase.resourceFloor != room) {
        return 0;
    }
    if (pFldNextFileDataF2 != 0 && pacCheckEnd(pFldNextFileDataF2) != 0) {
        return 1;
    }
    return fldGlobalBase.resourceFlag != 0;
}

sdfMemHandle_t *fldLoadNextFileTB(void **destination, s32 area, s32 room) {
    if (fldGlobalBase.resourceArea == area) {
        if (fldGlobalBase.resourceFloor == room) {
            sdfMemHandle_t *allocation = sdfMemAllocDirect(D_0054EBE8);
            void *data = (void *)sdfMemLock(allocation);
            *destination = data;
            memcpy(data, (void *)D_0054EBDC, D_0054EBE8);
            return allocation;
        }
    }
    return NULL;
}

sdfMemHandle_t *fldLoadNextFileF1(void **destination, s32 area, s32 room) {
    if (fldGlobalBase.resourceArea == area) {
        if (fldGlobalBase.resourceFloor == room) {
            sdfMemHandle_t *allocation = sdfMemAllocDirect(D_0054EBEC);
            void *data = (void *)sdfMemLock(allocation);
            *destination = data;
            memcpy(data, (void *)D_0054EBE0, D_0054EBEC);
            return allocation;
        }
    }
    return NULL;
}

sdfMemHandle_t *fldLoadNextFileF2(void **destination, s32 area, s32 room) {
    if (fldGlobalBase.resourceArea == area) {
        if (fldGlobalBase.resourceFloor == room) {
            sdfMemHandle_t *allocation = sdfMemAllocDirect(D_0054EBF0);
            void *data = (void *)sdfMemLock(allocation);
            *destination = data;
            memcpy(data, (void *)D_0054EBE4, D_0054EBF0);
            return allocation;
        }
    }
    return NULL;
}

INCLUDE_RODATA(const s32, "field/fldFileResolver", D_00531A40);

INCLUDE_ASM(const s32, "field/fldFileResolver", fldPreloadNextFile);

u8 fldCheckNextFile(void) {
    char buf[32];

    fldPreloadNextFile(buf);
    return strcmp(fldFieldPackFlag, buf) != 0;
}

void fldLoadFieldPack(void) {
    char name[32];
    filePacRequest_t *request;
    sdfPacWork_t *work;

    if (fldGlobalBase.area < 200) {
        fldPreloadNextFile(name);
        strcpy(fldFieldPackFlag, name);
        gFldFileF1 = pacLoad(name);
        pacSync(gFldFileF1);
        request = (filePacRequest_t *)gFldFileF1;
        for (work = request->packet.head; work != NULL; work = work->next) {
            switch (((sdfPacPacketHeader_t *)work->header)->kind) {
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

INCLUDE_ASM(const s32, "field/fldFileResolver", fldLoadFile);

INCLUDE_ASM(const s32, "field/fldFileResolver", fldLoadFile_FromMemory);

INCLUDE_ASM(const s32, "field/fldFileResolver", fldFreeFile);

/* Select the ordinary field, battle or event resource directory. */
void fldMakeFilePath(char *path, s32 field, s32 unused) {
    if (field < 200) sprintf(path, "/fld/f/f%03d/", field);
    else if (field < 500) sprintf(path, "/fld/b/f%03d/", field);
    else sprintf(path, "/fld/e/f%03d/", field);
}

INCLUDE_ASM(const s32, "field/fldFileResolver", fldHojiDebugView);

INCLUDE_RODATA(const s32, "field/fldFileResolver", D_00531B28);

INCLUDE_ASM(const s32, "field/fldFileResolver", fldHojiDebugView2);

/* Enable or skip relocation of remaining transfer chunks during field teardown. */
void fldFileAddrResetMode(u32 relocateOnRelease) {
    gFldFileResetMode = relocateOnRelease;
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

