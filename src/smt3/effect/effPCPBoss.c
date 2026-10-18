#include "common.h"
#include "ps2asm.h"

/* One crack segment: the extents effPCPCrackSizeInit randomizes plus the
 * negative age that delays its start. */
typedef struct effCrackCell_t
{
    f32 offsetDistance;
    f32 baseExtent;
    f32 tipExtent;
    s32 age;
    u8 flip;
    u8 unk11[3];
} effCrackCell_t;

typedef struct effCrackGroup_t
{
    void *drawPool;
    f32 direction[3];
    u8 unk10[4];
    f32 rotationAngle;
    s32 rotationStartAge;
    f32 angularSpeed;
    effCrackCell_t *cells;
} effCrackGroup_t;

typedef struct effCrackWork_t
{
    u8 position[0x10];
    u8 unk10[4];
    f32 frameStep;
    f32 scale;
    u8 unk1C[0x1C];
    u16 drawBucket;
    u8 unk3A[2];
    u8 hasCells;
    u8 unk3D[7];
    s32 delaySpread;
    u8 unk48[0x10];
    f32 offsetDistance;
    f32 offsetRandomness;
    f32 baseExtent;
    f32 tipExtent;
    f32 extentRandomness;
    u8 unk6C[0x18];
    s32 blend;
    u8 directionMode;
    u8 unk89[3];
    effCrackGroup_t *groups;
    void *groupsHandle;
    u32 groupCount;
    u16 cellCount;
    u8 unk9A[6];
    u32 color;
    void *system;
    s32 part;
} effCrackWork_t;

/* The parameter block effPCPCrackCreate copies into the work head; the two
 * rotation fields sit where the work keeps its group pointer. */
typedef struct effCrackParam_t
{
    u8 unk00[0x8C];
    s32 rotationStartAge;
    f32 angularSpeed;
} effCrackParam_t;

typedef struct effCrackMotion_t
{
    u8 unk00[0x20];
    f32 frameStep;
    u8 unk24[0xA];
    u16 frameCount;
} effCrackMotion_t;

typedef struct effCrackModel_t
{
    u8 unk00[0x18];
    void *inner;
    effCrackMotion_t *first;
} effCrackModel_t;

/* Five vertex colour words per cell. */
typedef struct effCrackColorSlot_t
{
    u32 color[5];
} effCrackColorSlot_t;

typedef struct effBaelWork_t
{
    u8 unk00[0x10];
    u8 parameterVector[0x10];
    s32 frame;
    u32 color;
    s32 modelResource;
    s32 auxResource;
} effBaelWork_t;

typedef struct effPCPBossONGY_t
{
    u8 unk00[4];
    void *matrix;
} effPCPBossONGY_t;

typedef struct effPCPBossBUNSIN_t
{
    u8 unk00[0x28];
    void *matrix;
} effPCPBossBUNSIN_t;

typedef struct effPCPBossBUNSIN2_t
{
    u8 unk00[0x18];
    effPCPBossBUNSIN_t *parts[3];
} effPCPBossBUNSIN2_t;

typedef struct effPCPBossDengeki_t
{
    u8 unk00[0x74];
    u32 color;
} effPCPBossDengeki_t;

typedef struct effPCPBossThunderbolt_t
{
    u8 position[0x10];
    u8 unk10[0x54];
    void *polyList;
    u8 unk68[4];
    u32 color;
} effPCPBossThunderbolt_t;

typedef struct effPCPBossKAGUInvoke_t
{
    u8 unk000[0x430];
    u32 color;
} effPCPBossKAGUInvoke_t;

typedef struct effPCPBossKAGUAttack_t
{
    u8 unk00[0xF8];
    u32 color;
} effPCPBossKAGUAttack_t;

typedef struct effPCPBossKAGUThunderbolt_t
{
    u8 position[0x10];
    u8 unk010[0x104];
    u32 color;
} effPCPBossKAGUThunderbolt_t;

typedef struct effPCPBossKAGU2Attack_t
{
    u8 unk00[0xB8];
    u32 color;
} effPCPBossKAGU2Attack_t;

typedef struct effPCPBossKAGU2MUHENK_t
{
    s32 unk00;
    u32 color;
} effPCPBossKAGU2MUHENK_t;

typedef struct effPCPBossMETATOBeam_t
{
    u8 position[0x10];
    u8 unk10[0x60];
    void *polyLists[2];
    u8 unk78[4];
    u32 color;
} effPCPBossMETATOBeam_t;

typedef struct effPCPScreenCrackModel_t
{
    u8 unk00[4];
    s32 part;
    u8 unk08[4];
    u32 color;
} effPCPScreenCrackModel_t;

typedef struct effPCPMuzzleFlash_t
{
    u8 matrix[0x40];
    u8 position[0x10];
    u8 unk50[8];
    void *billboard;
    u8 unk5C[4];
    f32 scale;
} effPCPMuzzleFlash_t;

typedef struct effPCPDANTE_Thunder_t
{
    u8 position[0x10];
    u8 matrix[0x40];
    u8 params[0x68];
    u32 color;
    void *poly;
    u8 unkC0[4];
    u8 drawRequest;
    u8 unkC5[3];
    s32 process;
} effPCPDANTE_Thunder_t;

typedef struct effPCPDANTE_Damage_t
{
    u8 position[0x10];
    u8 unk010[0x24];
    u8 unk034;
    u8 unk035[3];
    s32 unk038;
    u8 unk03C[0x100];
    u32 color;
} effPCPDANTE_Damage_t;

typedef struct effPCP_PBCamera_t
{
    u8 matrix[0x40];
    u8 position[0x10];
    u8 unk50[4];
    s32 part;
} effPCP_PBCamera_t;

typedef struct effPCPDANTE_ShowTime_t
{
    u8 position[0x10];
    u8 unk010[0xAC];
    void *textures[2];
    u8 unk0C4[0x10];
    u8 texLoaded;
} effPCPDANTE_ShowTime_t;

typedef struct effPCP_BED_t
{
    u8 matrix[0x40];
    u8 position[0x10];
    u8 unk50[0xC];
    u32 color;
} effPCP_BED_t;

typedef struct effPCP_HBED2_t
{
    u8 unk00[0x1C];
    void *helper;
} effPCP_HBED2_t;

extern u8 dds3EffectBase[];

extern void *memcpy(void *dst, const void *src, u32 size);
extern void *sdfChipAllocDirect(u32 size);
extern void sdfChipFreeDirect(void *ptr);
extern void *sdfMemAllocDirect(u32 size);
extern void *sdfMemLock(void *handle);
extern void sdfMemFreeDirect(void *handle);
extern u32 dds3GetRandA(u8 *state);
extern f32 dds3GetRandFloatA(u8 *state);
extern s32 dds3EffectPartCreate(s32 kind, void *param);
extern s32 dds3EffectPartCopy(s32 part);
extern void dds3EffectPartDelete(s32 part);
extern effCrackModel_t *dds3EffectPartGetHandle(s32 part);
extern void mdlSingleMotion(effCrackModel_t *model, s32 motion, s32 arg);
extern s32 sdfModelGetMarkPosCount(void *inner);
extern void *dds3TrackPolyCreate(s32 groups, s32 cells, s32 arg, s32 topology);
extern void dds3TrackPolySetMode(void *system, s32 mode);
extern void dds3TrackPolyDelete(void *system);
extern void *dds3FlashPolyListCreate(s32 count);
extern void dds3FlashPolyListDelete(void *pool);
extern f32 *dds3FlashPolyListGetVertex(void *pool, s32 index);
extern effCrackColorSlot_t *dds3FlashPolyListGetVC(void *pool, s32 index);
extern void dds3FlashPolyListSetBlend(void *pool, s32 blend);
extern effCrackWork_t *effPCPCrackCreate(effCrackParam_t *param, void *effectParam);
extern effCrackWork_t *effPCPCrackCopy(effCrackWork_t *src);

/* vf10 = vf0, the hardwired (0,0,0,1). */
#define VU0_COPY_VF0_TO_VF10() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf10, vf0\n\t.set reorder")

extern void *effPCPBossTexHandle;
extern s32 D_0054F1F4;
extern s32 effPCPBossONGY_RFrame;
extern s32 dds3RenderDrawEnv;

extern void dds3AllocRenderTexture(s32 width, s32 height, f32 arg2);
extern void dds3RenderTextureON(void);
extern void dds3RenderTextureOFF(void);
extern void dds3FreeRenderTexture(void);
extern void *mdlLoad(u32 group, u32 id);
extern void mdlFree(void *ctx);
extern void mdlSmoothLoopMotion(void *ctx, s32 searchId, s32 motionIndex, f32 blendLeadFrames,
                                f32 blendDurationFrames);
extern s32 dds3GetKernelCount(void);
extern s32 printf(const char *format, ...);

extern void sdfMatFreeDelayed(void *asset);
extern void effPCP_ThunderPolyListDelete(void *list);
extern void *dds3EffectPCPDataGetData(void *data, u32 number, ...);
extern effPCPBossThunderbolt_t *effPCPBossThunderboltCreate(void *params);

extern f32 effPCPBossKAGUDrawParam[4];
extern f32 effPCPBossKAGU2DrawParam[4];

extern void dds3BillboardDelete(void *billboard);
extern void dds3BillboardSetRGBA(void *billboard, u32 color);
extern effPCPBossKAGUInvoke_t *effPCPBossKAGUInvokeCreate(void *params);
extern effPCPBossMETATOBeam_t *effPCPBossMETATOBeamCreate(void *params);
extern void effPCPBossKAGUThunderboltDrawSub(effPCPBossKAGUThunderbolt_t *work, const f32 *param);
extern void effPCPCrackDraw(effCrackWork_t *work);

extern void dds3KillProcess(s32 process, s32 flags);
extern void effPCP_ThunderPolyDelete(void *poly);
extern void sdfTexFreeDelayed(void *texture);
extern void mdlSetRotate(void *model);
extern void effPCP_BED_MatrixToQuaternion(const void *matrix);
extern void *effPCPHelperBEDGetModel(void *helper);
extern void effPCPHelperBEDDelete(void *helper);
extern void effPCPHelperBEDDraw(void *helper);
extern void effPCPHelperBEDSetPosition(void *helper, const void *source);
extern void effPCPHelperBEDSetColor(void *helper, u32 color);
extern void effPCPHelperBEDSetScale(void *helper, f32 scale);
extern effPCPDANTE_Thunder_t *effPCPDANTE_Thunder_Create(void *params);
extern effPCPDANTE_Damage_t *effPCPDANTE_Damage_Create(void *params, void *bedParams);

extern s32 D_0054F200[2], D_0054F208[2];
extern u8 D_0054F210[2], D_0054F218[2];
extern effBaelWork_t *effPCPBossBaelCreate(void *p0, void *p1, void *p2);
extern void *effPCPBossDengekiCreate(void *p0, void *p1);
extern void *effPCPBossKAGUAttackCreate(void *p0, void *p1);
extern void *effPCPBossKAGUThunderboltCreate(void *p0, void *p1, void *p2);
extern void *effPCPBossKAGU2AttackCreate(void *p0, void *p1);
extern effCrackWork_t *effPCPCrack2Create(effCrackParam_t *p0, void *p1);
extern void *effPCPScreenCrackModelCreate(void *p0, void *p1);
extern void *effPCPMuzzleFlashCreate(void *p0, void *p1);
extern void *effPCP_PBCamera_Create(void *p0, void *p1);
extern void *effPCPDANTE_ShowTime_Create(void *p0, void *p1, void *p2);
extern void *effPCP_BED_Create(void *p0, void *p1);
extern void *effPCP_HBED2_Create(void *p0, void *p1, void *p2);
void effPCPCrackSizeInit(effCrackWork_t *work, effCrackCell_t *cell);

void effPCPBossONGYTexRenderStart(void)
{
    if (D_0054F1F4 == 0) {
        dds3RenderDrawEnv = 0;
        dds3AllocRenderTexture(0x100, 0x100, 0.8f);
        dds3RenderTextureON();
        effPCPBossTexHandle = mdlLoad(1, 0x10D);
        mdlSmoothLoopMotion(effPCPBossTexHandle, 0, 0, 0.0f, 4.0f);
        effPCPBossONGY_RFrame = dds3GetKernelCount() - 1;
        printf("effPCPBossONGY:Model Load\n");
    }
    D_0054F1F4++;
}

void effPCPBossONGYTexRenderEnd(void)
{
    if (--D_0054F1F4 == 0) {
        dds3RenderTextureOFF();
        dds3FreeRenderTexture();
        mdlFree(effPCPBossTexHandle);
        effPCPBossTexHandle = NULL;
        printf("effPCPBossONGY:Model Free\n");
    }
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossONGYModelTexRender);

s32 effPCPBossONGYModelTexRenderCheck(void)
{
    return effPCPBossONGY_RFrame != dds3GetKernelCount();
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossONGYCreate);

void effPCPBossONGYDelete(effPCPBossONGY_t *work)
{
    effPCPBossONGYTexRenderEnd();
    sdfMatFreeDelayed(work->matrix);
    sdfChipFreeDirect(work);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossONGYDraw);

s32 effPCPBossONGYGetLength(void)
{
    return 0x16;
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossONGY2Create);

void effPCPBossONGY2Delete(effPCPBossONGY_t *work)
{
    effPCPBossONGYTexRenderEnd();
    sdfMatFreeDelayed(work->matrix);
    sdfChipFreeDirect(work);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossONGY2Draw);

s32 effPCPBossONGY2GetLength(void)
{
    return 0x1B;
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossBUNSINCreate);

void effPCPBossBUNSINDelete(effPCPBossBUNSIN_t *work)
{
    effPCPBossONGYTexRenderEnd();
    sdfMatFreeDelayed(work->matrix);
    sdfChipFreeDirect(work);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossBUNSINDrawPoly);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossBUNSINDrawPoly2);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossBUNSINDraw);

s32 effPCPBossBUNSINGetLength(void)
{
    return 0x18;
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossBUNSIN2Create);

void effPCPBossBUNSIN2Delete(effPCPBossBUNSIN2_t *work)
{
    effPCPBossBUNSINDelete(work->parts[2]);
    effPCPBossBUNSINDelete(work->parts[1]);
    effPCPBossBUNSINDelete(work->parts[0]);
    sdfChipFreeDirect(work);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossBUNSIN2Draw);

s32 effPCPBossBUNSIN2GetLength(void)
{
    return effPCPBossBUNSINGetLength() * 2;
}

void effPCPCrackSizeInit(effCrackWork_t *work, effCrackCell_t *cell)
{
    f32 blend = work->offsetRandomness;
    f32 scale = work->scale;
    f32 t;

    cell->offsetDistance = work->offsetDistance *
        (dds3GetRandFloatA(dds3EffectBase) * blend + (1.0f - blend)) * scale;
    blend = work->extentRandomness;
    t = dds3GetRandFloatA(dds3EffectBase) * blend + (1.0f - blend);
    cell->baseExtent = work->baseExtent * t * scale;
    cell->tipExtent = work->tipExtent * t * scale;
    cell->flip = dds3GetRandA(dds3EffectBase) & 1;
    cell->age = -(dds3GetRandA(dds3EffectBase) % work->delaySpread);
}

void effPCPCrackInit(effCrackWork_t *work)
{
    effCrackModel_t *model = dds3EffectPartGetHandle(work->part);
    effCrackGroup_t *group;
    effCrackCell_t *nextCells;
    u32 groupIndex;
    u32 cellIndex;

    if (work->delaySpread <= 0) {
        work->delaySpread = 1;
    }
    model->first->frameStep = work->frameStep;
    mdlSingleMotion(model, 0, 0);
    work->cellCount = model->first->frameCount;
    work->groupCount = sdfModelGetMarkPosCount(model->inner);
    work->system = dds3TrackPolyCreate(work->groupCount, work->cellCount, 1, 1);
    dds3TrackPolySetMode(work->system, work->drawBucket);
    work->groupsHandle = NULL;
    if (work->hasCells) {
        work->groupsHandle = sdfMemAllocDirect(work->groupCount * sizeof(effCrackGroup_t)
            + work->cellCount * work->groupCount * sizeof(effCrackCell_t));
        nextCells = (effCrackCell_t *)sdfMemLock(work->groupsHandle);
        work->groups = (effCrackGroup_t *)nextCells;
        nextCells = (effCrackCell_t *)((u8 *)nextCells + work->groupCount * sizeof(effCrackGroup_t));
        for (groupIndex = 0, group = work->groups; groupIndex < work->groupCount; groupIndex++, group++) {
            effCrackCell_t *cell;

            group->drawPool = dds3FlashPolyListCreate(work->cellCount);
            if (work->directionMode == 0) {
                group->direction[0] = 0.0f;
                group->direction[1] = -1.0f;
                group->direction[2] = 0.0f;
            } else {
                group->direction[0] = 0.0f;
                group->direction[1] = 1.0f;
                group->direction[2] = 0.0f;
            }
            group->angularSpeed = (0.1f * (3.14159265f / 180.0f));
            group->rotationAngle = 0.0f;
            for (cellIndex = 0; cellIndex < work->cellCount; cellIndex++) {
                f32 *vertices = dds3FlashPolyListGetVertex(group->drawPool, cellIndex);
                effCrackColorSlot_t *colors = dds3FlashPolyListGetVC(group->drawPool, cellIndex);

                colors->color[0] = 0;
                colors->color[1] = 0;
                colors->color[2] = 0;
                colors->color[3] = 0;
                colors->color[4] = 0;
                VU0_COPY_VF0_TO_VF10();
                VU0_STORE_VF10(vertices);
                VU0_STORE_VF10(vertices + 4);
                VU0_STORE_VF10(vertices + 8);
                VU0_STORE_VF10(vertices + 12);
                VU0_STORE_VF10(vertices + 16);
            }
            dds3FlashPolyListSetBlend(group->drawPool, work->blend);
            group->cells = nextCells;
            nextCells += work->cellCount;
            for (cellIndex = 0, cell = group->cells; cellIndex < work->cellCount; cellIndex++, cell++) {
                effPCPCrackSizeInit(work, cell);
            }
        }
    }
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPCrackCreate);

void effPCPCrackCreatePCF(void *parameterTable) {
    effCrackParam_t *bossParams;
    void *effectParams;

    bossParams = dds3EffectPCPDataGetData(parameterTable, 0);
    effectParams = dds3EffectPCPDataGetData(parameterTable, 1);
    effPCPCrackCreate(bossParams, effectParams);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPCrackCopy);

void effPCPCrackDelete(effCrackWork_t *work)
{
    u32 groupIndex;

    if (work->groupsHandle != NULL) {
        for (groupIndex = 0; groupIndex < work->groupCount; groupIndex++) {
            dds3FlashPolyListDelete(work->groups[groupIndex].drawPool);
        }
        sdfMemFreeDirect(work->groupsHandle);
    }
    dds3TrackPolyDelete(work->system);
    dds3EffectPartDelete(work->part);
    sdfChipFreeDirect(work);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPCrackDraw);

void effPCPCrackSetPosition(effCrackWork_t *work, const void *source)
{
    PS2_COPY_QUAD(work, source);
}

void effPCPCrackSetColor(effCrackWork_t *work, u32 color)
{
    work->color = color;
}

effBaelWork_t *effPCPBossBaelCreate(void *vector, void *paramA, void *paramB)
{
    effBaelWork_t *work;

    work = sdfChipAllocDirect(sizeof(effBaelWork_t));
    memcpy(work->parameterVector, vector, 0x10);
    work->color = 0x80808080;
    work->frame = 0;
    work->modelResource = dds3EffectPartCreate(3, paramA);
    work->auxResource = dds3EffectPartCreate(6, paramB);
    return work;
}

void effPCPBossBaelCreatePCF(void *parameterTable) {
    void *firstBlock;
    void *secondBlock;
    void *thirdBlock;

    firstBlock = dds3EffectPCPDataGetData(parameterTable, 0);
    secondBlock = dds3EffectPCPDataGetData(parameterTable, 1);
    thirdBlock = dds3EffectPCPDataGetData(parameterTable, 2);
    effPCPBossBaelCreate(firstBlock, secondBlock, thirdBlock);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossBaelCopy);

void effPCPBossBaelDelete(effBaelWork_t *work)
{
    dds3EffectPartDelete(work->auxResource);
    dds3EffectPartDelete(work->modelResource);
    sdfChipFreeDirect(work);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossBaelDraw);

void effPCPBossBaelSetPosition(effBaelWork_t *work, const void *source)
{
    PS2_COPY_QUAD(work->parameterVector, source);
}

void effPCPBossBaelSetColor(effBaelWork_t *work, u32 color)
{
    work->color = color;
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossDengekiCreate);

void effPCPBossDengekiCreatePCF(void *parameterTable) {
    void *firstBlock;
    void *secondBlock;

    firstBlock = dds3EffectPCPDataGetData(parameterTable, 0);
    secondBlock = dds3EffectPCPDataGetData(parameterTable, 1);
    effPCPBossDengekiCreate(firstBlock, secondBlock);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossDengekiCopy);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossDengekiDelete);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossDengekiDraw);

void effPCPBossDengekiSetColor(effPCPBossDengeki_t *work, u32 color)
{
    work->color = color;
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossThunderboltCreate);

effPCPBossThunderbolt_t *effPCPBossThunderboltCreatePCF(void *data, s32 index, s32 type)
{
    return effPCPBossThunderboltCreate(dds3EffectPCPDataGetData(data, 0, type));
}

effPCPBossThunderbolt_t *effPCPBossThunderboltCopy(void *params, s32 index, s32 type)
{
    return effPCPBossThunderboltCreate(params);
}

void effPCPBossThunderboltDelete(effPCPBossThunderbolt_t *work)
{
    effPCP_ThunderPolyListDelete(work->polyList);
    sdfChipFreeDirect(work);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossThunderboltDraw);

void effPCPBossThunderboltSetPosition(effPCPBossThunderbolt_t *work, const void *source)
{
    PS2_COPY_QUAD(work, source);
}

void effPCPBossThunderboltSetColor(effPCPBossThunderbolt_t *work, u32 color)
{
    work->color = color;
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUInvokeCreate);

effPCPBossKAGUInvoke_t *effPCPBossKAGUInvokeCreatePCF(void *data, s32 index, s32 type)
{
    return effPCPBossKAGUInvokeCreate(dds3EffectPCPDataGetData(data, 0, type));
}

effPCPBossKAGUInvoke_t *effPCPBossKAGUInvokeCopy(void *params, s32 index, s32 type)
{
    return effPCPBossKAGUInvokeCreate(params);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUInvokeDelete);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUInvokeDraw);

void effPCPBossKAGUInvokeSetColor(effPCPBossKAGUInvoke_t *work, u32 color)
{
    work->color = color;
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUAttackCreate);

void effPCPBossKAGUAttackCreatePCF(void *parameterTable) {
    void *firstBlock;
    void *secondBlock;

    firstBlock = dds3EffectPCPDataGetData(parameterTable, 0);
    secondBlock = dds3EffectPCPDataGetData(parameterTable, 1);
    effPCPBossKAGUAttackCreate(firstBlock, secondBlock);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUAttackCopy);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUAttackDelete);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUAttackGetSplitPoint);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUAttackDraw);

void effPCPBossKAGUAttackSetColor(effPCPBossKAGUAttack_t *work, u32 color)
{
    work->color = color;
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUThunderboltCreate);

void effPCPBossKAGUThunderboltCreatePCF(void *parameterTable) {
    void *firstBlock;
    void *secondBlock;
    void *thirdBlock;

    firstBlock = dds3EffectPCPDataGetData(parameterTable, 0);
    secondBlock = dds3EffectPCPDataGetData(parameterTable, 1);
    thirdBlock = dds3EffectPCPDataGetData(parameterTable, 2);
    effPCPBossKAGUThunderboltCreate(firstBlock, secondBlock, thirdBlock);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUThunderboltCopy);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUThunderboltDelete);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUThunderboltDrawSub);

void effPCPBossKAGUThunderboltDraw(effPCPBossKAGUThunderbolt_t *work)
{
    effPCPBossKAGUThunderboltDrawSub(work, effPCPBossKAGUDrawParam);
}

void effPCPBossKAGUThunderboltSetPosition(effPCPBossKAGUThunderbolt_t *work, const void *source)
{
    PS2_COPY_QUAD(work, source);
}

void effPCPBossKAGUThunderboltSetColor(effPCPBossKAGUThunderbolt_t *work, u32 color)
{
    work->color = color;
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGU2AttackCreate);

void effPCPBossKAGU2AttackCreatePCF(void *parameterTable) {
    void *firstBlock;
    void *secondBlock;

    firstBlock = dds3EffectPCPDataGetData(parameterTable, 0);
    secondBlock = dds3EffectPCPDataGetData(parameterTable, 1);
    effPCPBossKAGU2AttackCreate(firstBlock, secondBlock);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGU2AttackCopy);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGU2AttackDelete);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGU2AttackGetSplitPoint);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGU2AttackDraw);

void effPCPBossKAGU2AttackSetColor(effPCPBossKAGU2Attack_t *work, u32 color)
{
    work->color = color;
}

void effPCPBossKAGU2ThunderboltDraw(effPCPBossKAGUThunderbolt_t *work)
{
    effPCPBossKAGUThunderboltDrawSub(work, effPCPBossKAGU2DrawParam);
}

effPCPBossKAGU2MUHENK_t *effPCPBossKAGU2MUHENKCreate(void *params)
{
    effPCPBossKAGU2MUHENK_t *work = sdfChipAllocDirect(sizeof(effPCPBossKAGU2MUHENK_t));

    work->unk00 = 0;
    work->color = 0x80808080;
    return work;
}

effPCPBossKAGU2MUHENK_t *effPCPBossKAGU2MUHENKCreatePCF(void *data, s32 index, s32 type)
{
    return effPCPBossKAGU2MUHENKCreate(dds3EffectPCPDataGetData(data, 0, type));
}

effPCPBossKAGU2MUHENK_t *effPCPBossKAGU2MUHENKCopy(void *params, s32 index, s32 type)
{
    return effPCPBossKAGU2MUHENKCreate(params);
}

void effPCPBossKAGU2MUHENKDelete(effPCPBossKAGU2MUHENK_t *work)
{
    sdfChipFreeDirect(work);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGU2MUHENKDraw);

void effPCPBossKAGU2MUHENKSetColor(effPCPBossKAGU2MUHENK_t *work, u32 color)
{
    work->color = color;
}

effCrackWork_t *effPCPCrack2Create(effCrackParam_t *param, void *effectParam)
{
    effCrackWork_t *work;
    u32 groupIndex;

    work = effPCPCrackCreate(param, effectParam);
    for (groupIndex = 0; groupIndex < work->groupCount; groupIndex++) {
        work->groups[groupIndex].rotationStartAge = param->rotationStartAge;
        work->groups[groupIndex].rotationAngle = 0.0f;
        work->groups[groupIndex].angularSpeed = param->angularSpeed;
    }
    return work;
}

void effPCPCrack2CreatePCF(void *data) {
    effCrackParam_t *bossParams;
    void *effectParams;

    bossParams = dds3EffectPCPDataGetData(data, 0);
    effectParams = dds3EffectPCPDataGetData(data, 1);
    effPCPCrack2Create(bossParams, effectParams);
}

effCrackWork_t *effPCPCrack2Copy(effCrackWork_t *src)
{
    effCrackWork_t *work;
    u32 groupIndex;

    work = effPCPCrackCopy(src);
    for (groupIndex = 0; groupIndex < work->groupCount; groupIndex++) {
        work->groups[groupIndex].rotationStartAge = src->groups[groupIndex].rotationStartAge;
        work->groups[groupIndex].rotationAngle = 0.0f;
        work->groups[groupIndex].angularSpeed = src->groups[groupIndex].angularSpeed;
    }
    return work;
}

void effPCPCrack2Delete(effCrackWork_t *work)
{
    effPCPCrackDelete(work);
}

void effPCPCrack2Draw(effCrackWork_t *work)
{
    effPCPCrackDraw(work);
}

void effPCPCrack2SetPosition(effCrackWork_t *work, const void *source)
{
    effPCPCrackSetPosition(work, source);
}

void effPCPCrack2SetColor(effCrackWork_t *work, u32 color)
{
    effPCPCrackSetColor(work, color);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossMETATOBeamCreate);

effPCPBossMETATOBeam_t *effPCPBossMETATOBeamCreatePCF(void *data, s32 index, s32 type)
{
    return effPCPBossMETATOBeamCreate(dds3EffectPCPDataGetData(data, 0, type));
}

effPCPBossMETATOBeam_t *effPCPBossMETATOBeamCopy(void *params, s32 index, s32 type)
{
    return effPCPBossMETATOBeamCreate(params);
}

void effPCPBossMETATOBeamDelete(effPCPBossMETATOBeam_t *work)
{
    effPCP_ThunderPolyListDelete(work->polyLists[1]);
    effPCP_ThunderPolyListDelete(work->polyLists[0]);
    sdfChipFreeDirect(work);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossMETATOBeamDraw);

void effPCPBossMETATOBeamSetPosition(effPCPBossMETATOBeam_t *work, const void *source)
{
    PS2_COPY_QUAD(work, source);
}

void effPCPBossMETATOBeamSetColor(effPCPBossMETATOBeam_t *work, u32 color)
{
    work->color = color;
}

void effPCPScreenCrackModelInit(void) {
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPScreenCrackModelCreate);

void effPCPScreenCrackModelCreatePCF(void *parameterTable) {
    void *firstBlock;
    void *secondBlock;

    firstBlock = dds3EffectPCPDataGetData(parameterTable, 0);
    secondBlock = dds3EffectPCPDataGetData(parameterTable, 1);
    effPCPScreenCrackModelCreate(firstBlock, secondBlock);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPScreenCrackModelCopy);

void effPCPScreenCrackModelDelete(effPCPScreenCrackModel_t *work)
{
    dds3EffectPartDelete(work->part);
    sdfChipFreeDirect(work);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPScreenCrackModelDraw);

void effPCPScreenCrackModelSetColor(effPCPScreenCrackModel_t *work, u32 color)
{
    work->color = color;
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPMuzzleFlashCreate);

void effPCPMuzzleFlashCreatePCF(void *parameterTable) {
    void *firstBlock;
    void *secondBlock;

    firstBlock = dds3EffectPCPDataGetData(parameterTable, 0);
    secondBlock = dds3EffectPCPDataGetData(parameterTable, 1);
    effPCPMuzzleFlashCreate(firstBlock, secondBlock);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPMuzzleFlashCopy);

void effPCPMuzzleFlashDelete(effPCPMuzzleFlash_t *work)
{
    dds3BillboardDelete(work->billboard);
    sdfChipFreeDirect(work);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPMuzzleFlashDraw);

void effPCPMuzzleFlashSetColor(effPCPMuzzleFlash_t *work, u32 color)
{
    dds3BillboardSetRGBA(work->billboard, color);
}

void effPCPMuzzleFlashSetPosition(effPCPMuzzleFlash_t *work, const void *source)
{
    PS2_COPY_QUAD(work->position, source);
}

void effPCPMuzzleFlashSetScale(effPCPMuzzleFlash_t *work, f32 scale)
{
    work->scale = 1.0f;
}

/* vu0 routine: copy the rotation matrix through vf28-vf31 */
void effPCPMuzzleFlashSetRotateMatrix(effPCPMuzzleFlash_t *work, const void *source)
{
    VU0_LOAD_MATRIX(source);
    VU0_STORE_MATRIX(work);
}

INCLUDE_RODATA(const s32, "effect/effPCPBoss", D_00533DA0);

INCLUDE_RODATA(const s32, "effect/effPCPBoss", D_00533DB0);

INCLUDE_RODATA(const s32, "effect/effPCPBoss", D_00533DC0);

INCLUDE_RODATA(const s32, "effect/effPCPBoss", D_00533DD0);

INCLUDE_RODATA(const s32, "effect/effPCPBoss", D_00533DE0);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_Thunder_Create);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_Thunder_ProcessDraw);

effPCPDANTE_Thunder_t *effPCPDANTE_Thunder_CreatePCF(void *data, s32 index, s32 type)
{
    return effPCPDANTE_Thunder_Create(dds3EffectPCPDataGetData(data, 0, type));
}

void effPCPDANTE_Thunder_Delete(effPCPDANTE_Thunder_t *work)
{
    dds3KillProcess(work->process, 0);
    effPCP_ThunderPolyDelete(work->poly);
    sdfChipFreeDirect(work);
}

effPCPDANTE_Thunder_t *effPCPDANTE_Thunder_Copy(effPCPDANTE_Thunder_t *src, s32 index, s32 type)
{
    return effPCPDANTE_Thunder_Create(src->params);
}

void effPCPDANTE_Thunder_DrawSub(effPCPDANTE_Thunder_t *work)
{
    work->drawRequest = 1;
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_Thunder_Draw);

void effPCPDANTE_Thunder_SetColor(effPCPDANTE_Thunder_t *work, u32 color)
{
    work->color = color;
}

void effPCPDANTE_Thunder_SetPosition(effPCPDANTE_Thunder_t *work, const void *source)
{
    PS2_COPY_QUAD(work, source);
}

/* vu0 routine: copy the rotation matrix through vf28-vf31 */
void effPCPDANTE_Thunder_SetRotateMatrix(effPCPDANTE_Thunder_t *work, const void *source)
{
    VU0_LOAD_MATRIX(source);
    VU0_STORE_MATRIX(work->matrix);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_Damage_Create);

void effPCPDANTE_Damage_CreatePCF(void *parameterTable) {
    void *firstBlock;
    void *secondBlock;

    firstBlock = dds3EffectPCPDataGetData(parameterTable, 0);
    secondBlock = dds3EffectPCPDataGetData(parameterTable, 1);
    effPCPDANTE_Damage_Create(firstBlock, secondBlock);
}

effPCPDANTE_Damage_t *effPCPDANTE_Damage_Copy(effPCPDANTE_Damage_t *src, s32 index, s32 type)
{
    effPCPDANTE_Damage_t *work = effPCPDANTE_Damage_Create(src, NULL);

    work->unk034 = 0;
    work->unk038 = src->unk038;
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_Damage_Delete);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_Damage_Run);

void effPCPDANTE_Damage_SetColor(effPCPDANTE_Damage_t *work, u32 color)
{
    work->color = color;
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_PBCamera_Create);

void effPCP_PBCamera_CreatePCF(void *parameterTable) {
    void *firstBlock;
    void *secondBlock;

    firstBlock = dds3EffectPCPDataGetData(parameterTable, 0);
    secondBlock = dds3EffectPCPDataGetData(parameterTable, 1);
    effPCP_PBCamera_Create(firstBlock, secondBlock);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_PBCamera_Copy);

void effPCP_PBCamera_Delete(effPCP_PBCamera_t *work)
{
    dds3EffectPartDelete(work->part);
    sdfChipFreeDirect(work);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_PBCamera_Run);

void effPCP_PBCamera_SetPosition(effPCP_PBCamera_t *work, const void *source)
{
    PS2_COPY_QUAD(work->position, source);
}

/* vu0 routine: copy the rotation matrix through vf28-vf31 */
void effPCP_PBCamera_SetRotateMatrix(effPCP_PBCamera_t *work, const void *source)
{
    VU0_LOAD_MATRIX(source);
    VU0_STORE_MATRIX(work);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_ShowTime_Create);

void effPCPDANTE_ShowTime_CreatePCF(void *parameterTable) {
    void *firstBlock;
    void *secondBlock;
    void *thirdBlock;

    firstBlock = dds3EffectPCPDataGetData(parameterTable, 0);
    secondBlock = dds3EffectPCPDataGetData(parameterTable, 1);
    thirdBlock = dds3EffectPCPDataGetData(parameterTable, 2);
    effPCPDANTE_ShowTime_Create(firstBlock, secondBlock, thirdBlock);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_ShowTime_Copy);

void effPCPDANTE_ShowTime_Delete(effPCPDANTE_ShowTime_t *work)
{
    if (work->texLoaded != 0) {
        sdfTexFreeDelayed(work->textures[1]);
        sdfTexFreeDelayed(work->textures[0]);
    }
    sdfChipFreeDirect(work);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_Sprite_Draw);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_ShowTime_Bingo_Draw);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_ShowTime_AreYouReady_Draw);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_ShowTime_ShowTime_Draw);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_ShowTime_Draw);

void effPCPDANTE_ShowTime_SetPosition(effPCPDANTE_ShowTime_t *work, const void *source)
{
    PS2_COPY_QUAD(work, source);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_BED_Create);

void effPCP_BED_CreatePCF(void *parameterTable) {
    void *firstBlock;
    void *secondBlock;

    firstBlock = dds3EffectPCPDataGetData(parameterTable, 0);
    secondBlock = dds3EffectPCPDataGetData(parameterTable, 1);
    effPCP_BED_Create(firstBlock, secondBlock);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_BED_Copy);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_BED_Delete);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_BED_MatrixToQuaternion);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_BED_Draw);

void effPCP_BED_SetPosition(effPCP_BED_t *work, const void *source)
{
    PS2_COPY_QUAD(work->position, source);
}

/* vu0 routine: copy the rotation matrix through vf28-vf31 */
void effPCP_BED_SetRotateMatrix(effPCP_BED_t *work, const void *source)
{
    VU0_LOAD_MATRIX(source);
    VU0_STORE_MATRIX(work);
}

void effPCP_BED_SetColor(effPCP_BED_t *work, u32 color)
{
    work->color = color;
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_HBED2_Create);

void effPCP_HBED2_CreatePCF(void *parameterTable) {
    void *firstBlock;
    void *secondBlock;
    void *thirdBlock;

    firstBlock = dds3EffectPCPDataGetData(parameterTable, 0);
    secondBlock = dds3EffectPCPDataGetData(parameterTable, 1);
    thirdBlock = dds3EffectPCPDataGetData(parameterTable, 2);
    effPCP_HBED2_Create(firstBlock, secondBlock, thirdBlock);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_HBED2_Copy);

void effPCP_HBED2_Delete(effPCP_HBED2_t *work)
{
    effPCPHelperBEDDelete(work->helper);
    sdfChipFreeDirect(work);
}

void effPCP_HBED2_Draw(effPCP_HBED2_t *work)
{
    effPCPHelperBEDDraw(work->helper);
}

void effPCP_HBED2_SetPosition(effPCP_HBED2_t *work, const void *source)
{
    effPCPHelperBEDSetPosition(work->helper, source);
}

void effPCP_HBED2_SetRotateMatrix(effPCP_HBED2_t *work, const void *source)
{
    void *model = effPCPHelperBEDGetModel(work->helper);

    effPCP_BED_MatrixToQuaternion(source);
    mdlSetRotate(model);
}

void effPCP_HBED2_SetColor(effPCP_HBED2_t *work, u32 color)
{
    effPCPHelperBEDSetColor(work->helper, color);
}

void effPCP_HBED2_SetScale(effPCP_HBED2_t *work, f32 scale)
{
    effPCPHelperBEDSetScale(work->helper, scale);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPadVibeFadeUpdate);

void effPadVibeFadeOUT(u32 port, u8 intensity, s32 frames)
{
    if (port < 2) {
        D_0054F200[port] = frames;
        D_0054F210[port] = intensity;
        D_0054F218[port] = 0;
        D_0054F208[port] = frames;
    }
}

void effPadVibeFadeIN(u32 port, u8 intensity, s32 frames)
{
    if (port < 2) {
        D_0054F200[port] = frames;
        D_0054F210[port] = intensity;
        D_0054F218[port] = 1;
        D_0054F208[port] = frames;
    }
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", dds3EffectBlurDrawQuad);

INCLUDE_RODATA(const s32, "effect/effPCPBoss", D_00533E10);

INCLUDE_SDATA(const s32, "effect/effPCPBoss", effPCPBossTexHandle);

INCLUDE_SDATA(const s32, "effect/effPCPBoss", D_0054F1F4);

INCLUDE_SDATA(const s32, "effect/effPCPBoss", effPCPBossONGY_RFrame);

INCLUDE_SDATA(const s32, "effect/effPCPBoss", D_0054F1FC);

INCLUDE_SDATA(const s32, "effect/effPCPBoss", D_0054F200);

INCLUDE_SDATA(const s32, "effect/effPCPBoss", D_0054F208);

INCLUDE_SDATA(const s32, "effect/effPCPBoss", D_0054F210);

INCLUDE_SDATA(const s32, "effect/effPCPBoss", D_0054F218);

