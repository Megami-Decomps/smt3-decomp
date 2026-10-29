#include "common.h"
#include "kernel.h"
#include "libsdf.h"

#include "ps2asm.h"
extern void dds3ParticleManagerExit(void);
extern void dds3PolygonManagerExit(void);
extern void dds3BillboardManagerExit(void);
extern sdfMemHandle_t *sdfDevLoadFileEx(const char *name, u32 *address, s32 flags);
extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

/* An effect handle: the type's entry in EffMajorTable plus the instance that
 * entry's create returned. version is stamped from the data revision. */
typedef struct effNode_t
{
    u32 type;
    u32 arg;
    s32 instance;
    f32 version;
} effNode_t;

/* One row per effect type, 0x30 bytes, indexed by effNode_t::type. */
typedef struct effMajorEntry_t
{
    s32 (*create)(u16 arg, s32 param);
    void (*draw)(s32 instance);
    void (*del)(s32 instance);
    void (*reset)(s32 instance);
    s32 (*getstate)(s32 instance);
    void (*setposition)(s32 instance, sdfVector4_t *pos);
    void (*setrotatematrix)(s32 instance, sdfMatrix4_t *matrix);
    void (*setrgba)(s32 instance, u32 rgba);
    void (*setloop)(s32 instance, s32 loop);
    s32 (*getloop)(s32 instance);
    s32 (*copy)(s32 instance);
    void (*setscale)(s32 instance, f32 scale);
} effMajorEntry_t;

extern effMajorEntry_t EffMajorTable[];

/* Effect resource header: the file data dds3EffectLoad is handed. */
typedef struct effResourceHeader_t
{
    u32 type;
    u32 arg;
    u32 unk08;
    f32 version;
    u8 payload[16];
} effResourceHeader_t;

/* The two instance layouts that carry a matrix a pre-1.03 file leaves unset. */
typedef struct effLegacyInstance_t
{
    u8 unk00[0x20];
    sdfMatrix4_t matrix20;
    u8 unk60[0x50];
    sdfMatrix4_t matrixB0;
} effLegacyInstance_t;


extern void *sdfChipAllocDirect(u32 aSize);
extern void sdfChipFreeDirect(void *aPtr);
int printf(const char *fmt, ...);

extern void dds3PolygonLinkOt(void);
extern void dds3TrackPolyLinkOt(void);
extern void dds3BillboardLinkOt(void);
extern void dds3MultiBillboardLinkOt(void);
extern void dds3CrossfadeRun(void);
extern void Eve_EffectDraw(void);

effNode_t *dds3EffectCreate(u16 type, u16 arg, s32 param);
void dds3EffectDataRevision(effResourceHeader_t *header);

extern u8 dds3EffectBase[];
extern char D_0054F0B0[];
extern sdfGrOt_t sdfGrOtTable[];
extern void sdfRandomizeByTime(void *aCB);
extern void dds3BillboardManagerInit(void);
extern void dds3PolygonManagerInit(void);
extern void dds3ParticleManagerInit(void);
extern void effDebugInit(void);
extern void Eve_EffectInit(void);
extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, dds3ProcessFunc_t drawExit,
                                         dds3ProcessFunc_t deleteExit, void *p);
extern s32 sdfGrAllocPacketBuff(s32 aSize);
extern void sdfDrawTagInit(sdfDrawTag_t *a);
extern void sdfDrawTagLink(sdfDrawTag_t *aTag, u32 aPacket);
s32 dds3EffectManagerRunF(dds3ProcessID_t *ProcessID);
s32 dds3EffectManagerRun(dds3ProcessID_t *ProcessID);
void dds3EffectManagerExit(void);

void dds3EffectManagerInit(void) {
    sdfRandomizeByTime(dds3EffectBase);
    dds3BillboardManagerInit();
    dds3PolygonManagerInit();
    dds3ParticleManagerInit();
    dds3StartProcess("effect_f", 0x2B04, 0, 0, dds3EffectManagerRunF, NULL, NULL);
    dds3StartProcess(D_0054F0B0, 0x2B18, 0, 0, dds3EffectManagerRun, (dds3ProcessFunc_t)dds3EffectManagerExit, NULL);
    effDebugInit();
    Eve_EffectInit();
}

void dds3EffectManagerExit(void) {
    dds3ParticleManagerExit();
    dds3PolygonManagerExit();
    dds3BillboardManagerExit();
}

s32 dds3EffectManagerRunF(dds3ProcessID_t *ProcessID)
{
    sdfDrawTag_t *list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfGrOt_t *ot;

    sdfDrawTagInit(list);
    {
        u64 *packet = (u64 *)sdfGrAllocPacketBuff(0x40);

        packet[0] = 3;
        packet[1] = 0x5000000310000000ULL;
        packet[2] = 0x1000000000008002ULL;
        packet[3] = 0xE;
        packet[4] = 0x51801;
        packet[5] = 0x47;
        packet[6] = 0x44;
        packet[7] = 0x42;
        sdfDrawTagLink(list, (u32)packet);
    }
    ot = &sdfGrOtTable[43];
    ot->regFunc(ot, list);
    return 0;
}

s32 dds3EffectManagerRun(dds3ProcessID_t *ProcessID)
{
    dds3PolygonLinkOt();
    dds3TrackPolyLinkOt();
    dds3BillboardLinkOt();
    dds3MultiBillboardLinkOt();
    dds3CrossfadeRun();
    Eve_EffectDraw();
    return 0;
}

void dds3EffectPCFSetLoop(void) {
}

u32 dds3EffectPCFGetLoop(void) {
    return 1;
}

void dds3EffectPCFReset(void) {
}

u32 dds3EffectPCFGetState(void) {
    return 1;
}

effNode_t *dds3EffectCreate(u16 type, u16 arg, s32 param)
{
    effNode_t *node = sdfChipAllocDirect(sizeof(effNode_t));

    node->type = type;
    node->version = 1.03f;
    node->arg = arg;
    node->instance = EffMajorTable[type].create(arg, param);
    return node;
}

void dds3EffectDelete(effNode_t *pData)
{
    EffMajorTable[pData->type].del(pData->instance);
    sdfChipFreeDirect(pData);
}

void dds3EffectDraw(effNode_t *pData)
{
    EffMajorTable[pData->type].draw(pData->instance);
}

void dds3EffectReset(effNode_t *hEffect)
{
    EffMajorTable[hEffect->type].reset(hEffect->instance);
}

void dds3EffectSetScale(effNode_t *pData, f32 scale)
{
    EffMajorTable[pData->type].setscale(pData->instance, scale);
}

s32 dds3EffectGetState(effNode_t *hEffect)
{
    return EffMajorTable[hEffect->type].getstate(hEffect->instance);
}

void dds3EffectSetPosition(effNode_t *pData, sdfVector4_t *pPos)
{
    EffMajorTable[pData->type].setposition(pData->instance, pPos);
}

void dds3EffectSetRotateMatrix(effNode_t *pData, sdfMatrix4_t *pMatrix)
{
    EffMajorTable[pData->type].setrotatematrix(pData->instance, pMatrix);
}

void dds3EffectSetRGBA(effNode_t *pData, u32 abgr)
{
    EffMajorTable[pData->type].setrgba(pData->instance, abgr);
}

void dds3EffectSetLoop(effNode_t *hEffect, u8 loop)
{
    EffMajorTable[hEffect->type].setloop(hEffect->instance, loop);
}

s32 dds3EffectGetLoop(effNode_t *hEffect)
{
    return EffMajorTable[hEffect->type].getloop(hEffect->instance);
}


effNode_t *dds3EffectLoad(effResourceHeader_t *pD3P)
{
    effNode_t *node;
    effLegacyInstance_t *instance;

    if (pD3P->version < 1.03f) {
        printf("old version!![%f]\n", pD3P->version);
        dds3EffectDataRevision(pD3P);
    }
    node = dds3EffectCreate((u16)pD3P->type, (u16)pD3P->arg, (s32)pD3P->payload);
    if (pD3P->version <= 1.02f) {
        switch (node->type) {
        case 0:
        case 1:
            instance = (effLegacyInstance_t *)node->instance;
            SDF_VU0_STOREIDENTITY(&instance->matrixB0);
            break;
        case 2:
            instance = (effLegacyInstance_t *)node->instance;
            SDF_VU0_STOREIDENTITY(&instance->matrix20);
            break;
        }
        printf("effManager:set Identity matrix\n");
    }
    return node;
}

effNode_t *dds3EffectFileLoad(const char *pFilename) {
    u32 resolvedId;
    sdfMemHandle_t *resourceHandle;
    effNode_t *node;

    printf("d3p file read...[%s]\n", pFilename);
    resourceHandle = sdfDevLoadFileEx(pFilename, &resolvedId, 0);
    node = dds3EffectLoad((effResourceHeader_t *)resolvedId);
    sdfMemFreeDirect(resourceHandle);
    return node;
}

effNode_t *dds3EffectCopy(effNode_t *node)
{
    effNode_t *copy = sdfChipAllocDirect(sizeof(effNode_t));
    s32 instance = node->instance;

    copy->type = node->type;
    copy->arg = node->arg;
    copy->instance = EffMajorTable[node->type].copy(instance);
    return copy;
}

void dds3EffectDataRevision(effResourceHeader_t *header)
{
    u8 *payload = header->payload;

    if (header->version == 1.0f) {
        u32 type = header->type;
        switch (type) {
        case 0:
            *(u32 *)(payload + 0xA0) = 0xAC;
            break;
        case 1:
            {
                u8 *source = payload + *(u32 *)(payload + 4);
                payload = source + 0x10;
                *(u32 *)(payload + 0xA0) = 0xAC;
            }
            break;
        case 2:
            *(u32 *)(payload + 0x18) = 0x20;
            break;
        }
    }

    if (header->version <= 1.01f) {
        u32 type = header->type;
        if (type == 1) {
            u8 *source = payload + *(u32 *)(payload + 4);
            payload = source + 0x10;
            *(f32 *)(payload + 0x8C) =
                *(f32 *)(payload + 0x8C) * 0.1f * (f32)*(s32 *)(payload + 0x24) +
                *(f32 *)(payload + 0x10);
        }
        if (type < 2 && header->arg == 0) {
            printf("effManager:par spiral old version!![%f]\n", header->version);
        }
    }
}

INCLUDE_RODATA(const s32, "effect/effManager", D_00532B70);

INCLUDE_RODATA(const s32, "effect/effManager", D_00532B88);

INCLUDE_RODATA(const s32, "effect/effManager", D_00532BA0);

INCLUDE_RODATA(const s32, "effect/effManager", D_00532BB8);

INCLUDE_RODATA(const s32, "effect/effManager", D_00532BD0);

INCLUDE_RODATA(const s32, "effect/effManager", D_00532BE8);

INCLUDE_RODATA(const s32, "effect/effManager", D_00532C00);

INCLUDE_RODATA(const s32, "effect/effManager", D_00532C18);

INCLUDE_SDATA(const s32, "effect/effManager", D_0054F0B0);

