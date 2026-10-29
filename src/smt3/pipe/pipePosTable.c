#include "fpu.h"
#include "common.h"
#include "ps2asm.h"
#include "basic.h"
#include "motion.h"
#include "pipe.h"

extern void (*D_00551828)(void);



extern tagPipePartsInfo D_00529030[];

s32 pipeFileSave(s8 filename, tagPipePosTable *posPtr, s8 index)
{
    return 1;
}



typedef struct {
    s32 unk_00;
    s32 unk_04;
    s32 unk_08;
    s32 unk_0C;
} pipeLinkTable_t;

extern char D_005517E8[];
extern char D_00548E78[]; /* "size %d\n" */
extern char D_00548E88[]; /* "tMAX %d\n" */
extern char D_00548E98[]; /* "linkNodeNum %d\n" */

typedef struct fileRequest_t fileRequest_t;
extern fileRequest_t *D_005517E0;

extern void DEBUG_PRINT(const char *format, ...);
extern fileRequest_t *fileLoad(const char *filename);
extern s32 pacFileCheckEnd(const fileRequest_t *fileHandle);
extern s32 *fileGetLoadPtr(const fileRequest_t *fileHandle);
extern sdfMemHandle_t *fileGetMemHandle(const fileRequest_t *fileHandle);
extern void fileHandleFree(fileRequest_t *fileHandle);
extern sdfMemHandle_t *sdfMemAllocDirect(s32 size);
extern void *sdfMemGetPtr(sdfMemHandle_t *handle);
extern void sdfMemFreeDirect(sdfMemHandle_t *handle);
extern void *memcpy(void *dst, const void *src, u32 size);
extern void LoadLinkData(pipeLinkTable_t *table, pipeLinkSource_t *source, s32 mode);

#ifdef NON_MATCHING
/* NON_MATCHING: 103 of 106 words match at the exact 424-byte size. */
s32 pipeFileLoad(const char *filename, tagPipePosTable *pos, tagPipeTable *type,
                 pipeLinkTable_t *link)
{
    s32 *header;
    u8 *src;
    s32 size;
    s32 bytes;
    void *data;
    s32 nodes;
    s32 offset;
    s32 count;
    pipeLinkSource_t source;
    pipeLinkTable_t table;

    if (D_005517E0 == NULL) {
        D_005517E0 = fileLoad(filename);
    }
    if (pacFileCheckEnd(D_005517E0) == 0) {
        return 0;
    }
    DEBUG_PRINT(D_005517E8, 0x20);
    header = fileGetLoadPtr(D_005517E0);
    count = header[0];
    src = (u8 *)header + header[1];
    pos->tSize = count;
    DEBUG_PRINT(D_00548E78, count);
    size = pos->tSize << 5;
    pos->aHandle = sdfMemAllocDirect(size);
    pos->points = sdfMemGetPtr(pos->aHandle);
    memcpy(pos->points, src, size);

    offset = header[3];
    count = header[2];
    type->tabMax = count;
    src = (u8 *)header + offset;
    DEBUG_PRINT(D_00548E88, count);
    type->aHandle = sdfMemAllocDirect(type->tabMax * 9);
    data = sdfMemGetPtr(type->aHandle);
    bytes = type->tabMax << 3;
    type->tabPtr = data;
    type->data = (u8 *)data + bytes;
    memcpy(data, src, bytes);

    src = (u8 *)header + header[5];
    nodes = header[4];
    source.count = nodes;
    DEBUG_PRINT(D_00548E98, nodes);
    source.data = src;
    LoadLinkData(&table, &source, 0);
    link->unk_00 = table.unk_00;
    link->unk_08 = table.unk_08;
    link->unk_0C = table.unk_0C;
    link->unk_04 = table.unk_04;
    sdfMemFreeDirect(fileGetMemHandle(D_005517E0));
    fileHandleFree(D_005517E0);
    D_005517E0 = NULL;
    return 1;
}
#else
INCLUDE_ASM(const s32, "pipe/pipePosTable", pipeFileLoad);
#endif


tagPipePartsInfo *getPartsInfo(s16 partsID)
{
    tagPipePartsInfo *pInfo = D_00529030;
    s32 i;

    for (i = 0; i < 60; i++) {
        if (pInfo->partsID == partsID) {
            return pInfo;
        }
        pInfo++;
    }
    return NULL;
}


s32 getPartsType(s32 id)
{
    s32 shiftedId = id >> 6;
    tagPipePartsInfo *pInfo = getPartsInfo((s16)id);
    s32 base = (s8)(shiftedId * 2);

    if (pInfo == NULL) {
        return (s32)pInfo;
    }
    return base + ((pInfo->goWay + pInfo->inWay == 7) ? 1 : 2);
}


s32 pipe_2A2F38(void)
{
    return 0;
}

s32 pipe_2A2F40(void)
{
    return 1;
}

void func_002A2F48(void) {
}

void func_002A2F50(void) {
}
INCLUDE_SDATA(const s32, "pipe/pipePosTable", D_005517E0);

INCLUDE_SDATA(const s32, "pipe/pipePosTable", D_005517E8);

