#include "common.h"
#include "kernel.h"
#include "libsdf.h"

#include "ps2asm.h"
extern void dds3ParticleManagerExit(void);
extern void dds3PolygonManagerExit(void);
extern void dds3BillboardManagerExit(void);
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
#include "sdk_libc.h"

extern void dds3PolygonLinkOt(void);
extern void dds3TrackPolyLinkOt(void);
extern void dds3BillboardLinkOt(void);
extern void dds3MultiBillboardLinkOt(void);
extern void dds3CrossfadeRun(void);
extern void Eve_EffectDraw(void);

effNode_t *dds3EffectCreate(u16 type, u16 arg, saddr_t param);
void dds3EffectDataRevision(effResourceHeader_t *header);

extern u8 dds3EffectBase[];
extern char dds3EffectProcessName[];
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
extern void sdfDrawTagLink(sdfDrawTag_t *aTag, addr_t aPacket);
s32 dds3EffectManagerRunF(dds3ProcessID_t *ProcessID);
s32 dds3EffectManagerRun(dds3ProcessID_t *ProcessID);
void dds3EffectManagerExit(void);

void dds3EffectManagerInit(void) {
    sdfRandomizeByTime(dds3EffectBase);
    dds3BillboardManagerInit();
    dds3PolygonManagerInit();
    dds3ParticleManagerInit();
    dds3StartProcess("effect_f", 0x2B04, 0, 0, dds3EffectManagerRunF, NULL, NULL);
    dds3StartProcess(dds3EffectProcessName, 0x2B18, 0, 0, dds3EffectManagerRun, (dds3ProcessFunc_t)dds3EffectManagerExit, NULL);
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
    sdfDrawTag_t *list = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    sdfGrOt_t *ot;

    sdfDrawTagInit(list);
    {
        u64 *packet = EE_ADDR_AS(u64 *, sdfGrAllocPacketBuff(0x40));

        packet[0] = 3;
        packet[1] = 0x5000000310000000ULL;
        packet[2] = 0x1000000000008002ULL;
        packet[3] = 0xE;
        packet[4] = 0x51801;
        packet[5] = 0x47;
        packet[6] = 0x44;
        packet[7] = 0x42;
        sdfDrawTagLink(list, EE_ADDR(packet));
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

effNode_t *dds3EffectCreate(u16 type, u16 arg, saddr_t param)
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
    node = dds3EffectCreate((u16)pD3P->type, (u16)pD3P->arg, EE_ADDR_S(pD3P->payload));
    if (pD3P->version <= 1.02f) {
        switch (node->type) {
        case 0:
        case 1:
            instance = EE_ADDR_AS(effLegacyInstance_t *, node->instance);
            SDF_VU0_STOREIDENTITY(&instance->matrixB0);
            break;
        case 2:
            instance = EE_ADDR_AS(effLegacyInstance_t *, node->instance);
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
    node = dds3EffectLoad(EE_ADDR_AS(effResourceHeader_t *, resolvedId));
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


s32 dds3ParticleCreate(u16 arg, s32 param);
void dds3ParticleDraw(s32 instance);
void dds3ParticleDelete(s32 instance);
void dds3ParticleReset(s32 instance);
s32 dds3ParticleGetState(s32 instance);
void dds3ParticleSetPosition(s32 instance, sdfVector4_t *pos);
void dds3ParticleSetRotateMatrix(s32 instance, sdfMatrix4_t *matrix);
void dds3ParticleSetRGBA(s32 instance, u32 rgba);
void dds3ParticleSetTypeFlag(s32 instance, s32 loop);
s32 dds3ParticleGetLoop(s32 instance);
s32 dds3ParticleCopy(s32 instance);
void dds3ParticleSetScale(s32 instance, f32 scale);
s32 dds3ParticleExpCreate(u16 arg, s32 param);
void dds3ParticleExpDraw(s32 instance);
void dds3ParticleExpDelete(s32 instance);
void dds3ParticleExpReset(s32 instance);
s32 dds3ParticleExpGetState(s32 instance);
void dds3ParticleExpSetPosition(s32 instance, sdfVector4_t *pos);
void dds3ParticleExpSetRotateMatrix(s32 instance, sdfMatrix4_t *matrix);
void dds3ParticleExpSetRGBA(s32 instance, u32 rgba);
void dds3ParticleExpSetLoop(s32 instance, s32 loop);
s32 dds3ParticleExpGetLoop(s32 instance);
s32 dds3ParticleExpCopy(s32 instance);
void dds3ParticleExpSetScale(s32 instance, f32 scale);
s32 dds3SpPolyCreate(u16 arg, s32 param);
void dds3SpPolyDraw(s32 instance);
void dds3SpPolyDelete(s32 instance);
void dds3SpPolyReset(s32 instance);
s32 dds3SpPolyGetState(s32 instance);
void dds3SpPolySetPosition(s32 instance, sdfVector4_t *pos);
void dds3SpPolySetRotateMatrix(s32 instance, sdfMatrix4_t *matrix);
void dds3SpPolySetRGBA(s32 instance, u32 rgba);
void dds3SpPolySetLoop(s32 instance, s32 loop);
s32 dds3SpPolyGetLoop(s32 instance);
s32 dds3SpPolyCopy(s32 instance);
void dds3SpPolySetScale(s32 instance, f32 scale);
s32 dds3MagatuhiParCreate(u16 arg, s32 param);
void dds3MagatuhiParDraw(s32 instance);
void dds3MagatuhiParDelete(s32 instance);
void func_001A8CD0(s32 instance);
s32 dds3MagatuhiParIsActive(s32 instance);
void dds3MagatuhiParSetPosition(s32 instance, sdfVector4_t *pos);
void dds3MagatuhiParSetRotateMatrix(s32 instance, sdfMatrix4_t *matrix);
void dds3MagatuhiParSetRGBA(s32 instance, u32 rgba);
void dds3MagatuhiParSetByType(s32 instance, s32 loop);
s32 func_001A8DF8(s32 instance);
s32 dds3MagatuhiParCopy(s32 instance);
void func_001A8EA8(s32 instance, f32 scale);
s32 dds3EffectPartCreate(u16 arg, s32 param);
void dds3EffectPartDraw(s32 instance);
void dds3EffectPartDelete(s32 instance);
void dds3EffectPartSetPosition(s32 instance, sdfVector4_t *pos);
void dds3EffectPartSetRotateMatrix(s32 instance, sdfMatrix4_t *matrix);
void dds3EffectPartSetColor(s32 instance, u32 rgba);
s32 dds3EffectPartCopy(s32 instance);
void dds3EffectPartSetScale(s32 instance, f32 scale);

effMajorEntry_t EffMajorTable[5] = {
    { dds3ParticleCreate, dds3ParticleDraw, dds3ParticleDelete, dds3ParticleReset, dds3ParticleGetState, dds3ParticleSetPosition, dds3ParticleSetRotateMatrix, dds3ParticleSetRGBA, dds3ParticleSetTypeFlag, dds3ParticleGetLoop, dds3ParticleCopy, dds3ParticleSetScale },
    { dds3ParticleExpCreate, dds3ParticleExpDraw, dds3ParticleExpDelete, dds3ParticleExpReset, dds3ParticleExpGetState, dds3ParticleExpSetPosition, dds3ParticleExpSetRotateMatrix, dds3ParticleExpSetRGBA, dds3ParticleExpSetLoop, dds3ParticleExpGetLoop, dds3ParticleExpCopy, dds3ParticleExpSetScale },
    { dds3SpPolyCreate, dds3SpPolyDraw, dds3SpPolyDelete, dds3SpPolyReset, dds3SpPolyGetState, dds3SpPolySetPosition, dds3SpPolySetRotateMatrix, dds3SpPolySetRGBA, dds3SpPolySetLoop, dds3SpPolyGetLoop, dds3SpPolyCopy, dds3SpPolySetScale },
    { dds3MagatuhiParCreate, dds3MagatuhiParDraw, dds3MagatuhiParDelete, func_001A8CD0, dds3MagatuhiParIsActive, dds3MagatuhiParSetPosition, dds3MagatuhiParSetRotateMatrix, dds3MagatuhiParSetRGBA, dds3MagatuhiParSetByType, func_001A8DF8, dds3MagatuhiParCopy, func_001A8EA8 },
    { dds3EffectPartCreate, dds3EffectPartDraw, dds3EffectPartDelete, (void (*)(s32 instance))dds3EffectPCFReset, (s32 (*)(s32 instance))dds3EffectPCFGetState, dds3EffectPartSetPosition, dds3EffectPartSetRotateMatrix, dds3EffectPartSetColor, (void (*)(s32 instance, s32 loop))dds3EffectPCFSetLoop, (s32 (*)(s32 instance))dds3EffectPCFGetLoop, dds3EffectPartCopy, dds3EffectPartSetScale },
};

INCLUDE_RODATA(const s32, "effect/effManager", billPathSmoke12);

INCLUDE_RODATA(const s32, "effect/effManager", billPathSmoke10);

INCLUDE_RODATA(const s32, "effect/effManager", billPathSmoke08);

INCLUDE_RODATA(const s32, "effect/effManager", billPathSmoke07);

INCLUDE_RODATA(const s32, "effect/effManager", billPathMblur8_3);

INCLUDE_RODATA(const s32, "effect/effManager", billPathMblur8);

INCLUDE_RODATA(const s32, "effect/effManager", billPathSmoke16);

INCLUDE_RODATA(const s32, "effect/effManager", billPathBil00);

INCLUDE_SDATA(const s32, "effect/effManager", dds3EffectProcessName);

