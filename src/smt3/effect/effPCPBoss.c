#include "common.h"

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
    u8 unk00[0x14];
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
    u8 unk9A[0xA];
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

#define VU0_MOVE_VF(dst, src) __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw " #dst ", " #src "\n\t.set reorder")

#define VU0_STORE_VF(vf, dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 " #vf ", 0(%0)\n\t.set reorder" \
    : : "r" (dst) : "memory")

void effPCPCrackSizeInit(effCrackWork_t *work, effCrackCell_t *cell);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossONGYTexRenderStart);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossONGYTexRenderEnd);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossONGYModelTexRender);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossONGYModelTexRenderCheck);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossONGYCreate);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossONGYDelete);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossONGYDraw);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossONGYGetLength);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossONGY2Create);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossONGY2Delete);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossONGY2Draw);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossONGY2GetLength);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossBUNSINCreate);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossBUNSINDelete);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossBUNSINDrawPoly);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossBUNSINDrawPoly2);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossBUNSINDraw);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossBUNSINGetLength);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossBUNSIN2Create);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossBUNSIN2Delete);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossBUNSIN2Draw);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossBUNSIN2GetLength);

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
                VU0_MOVE_VF(vf10, vf0);
                VU0_STORE_VF(vf10, vertices);
                VU0_STORE_VF(vf10, vertices + 4);
                VU0_STORE_VF(vf10, vertices + 8);
                VU0_STORE_VF(vf10, vertices + 12);
                VU0_STORE_VF(vf10, vertices + 16);
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

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPCrackCreatePCF);

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

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPCrackSetPosition);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPCrackSetColor);

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

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossBaelCreatePCF);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossBaelCopy);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossBaelDelete);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossBaelDraw);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossBaelSetPosition);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossBaelSetColor);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossDengekiCreate);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossDengekiCreatePCF);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossDengekiCopy);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossDengekiDelete);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossDengekiDraw);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossDengekiSetColor);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossThunderboltCreate);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossThunderboltCreatePCF);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossThunderboltCopy);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossThunderboltDelete);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossThunderboltDraw);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossThunderboltSetPosition);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossThunderboltSetColor);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUInvokeCreate);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUInvokeCreatePCF);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUInvokeCopy);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUInvokeDelete);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUInvokeDraw);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUInvokeSetColor);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUAttackCreate);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUAttackCreatePCF);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUAttackCopy);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUAttackDelete);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUAttackGetSplitPoint);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUAttackDraw);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUAttackSetColor);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUThunderboltCreate);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUThunderboltCreatePCF);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUThunderboltCopy);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUThunderboltDelete);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUThunderboltDrawSub);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUThunderboltDraw);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUThunderboltSetPosition);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUThunderboltSetColor);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGU2AttackCreate);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGU2AttackCreatePCF);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGU2AttackCopy);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGU2AttackDelete);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGU2AttackGetSplitPoint);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGU2AttackDraw);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGU2AttackSetColor);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGU2ThunderboltDraw);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGU2MUHENKCreate);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGU2MUHENKCreatePCF);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGU2MUHENKCopy);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGU2MUHENKDelete);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGU2MUHENKDraw);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGU2MUHENKSetColor);

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

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPCrack2CreatePCF);

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

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPCrack2Delete);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPCrack2Draw);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPCrack2SetPosition);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPCrack2SetColor);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossMETATOBeamCreate);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossMETATOBeamCreatePCF);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossMETATOBeamCopy);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossMETATOBeamDelete);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossMETATOBeamDraw);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossMETATOBeamSetPosition);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossMETATOBeamSetColor);

void effPCPScreenCrackModelInit(void) {
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPScreenCrackModelCreate);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPScreenCrackModelCreatePCF);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPScreenCrackModelCopy);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPScreenCrackModelDelete);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPScreenCrackModelDraw);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPScreenCrackModelSetColor);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPMuzzleFlashCreate);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPMuzzleFlashCreatePCF);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPMuzzleFlashCopy);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPMuzzleFlashDelete);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPMuzzleFlashDraw);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPMuzzleFlashSetColor);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPMuzzleFlashSetPosition);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPMuzzleFlashSetScale);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPMuzzleFlashSetRotateMatrix);

INCLUDE_RODATA(const s32, "effect/effPCPBoss", D_00533DA0);

INCLUDE_RODATA(const s32, "effect/effPCPBoss", D_00533DB0);

INCLUDE_RODATA(const s32, "effect/effPCPBoss", D_00533DC0);

INCLUDE_RODATA(const s32, "effect/effPCPBoss", D_00533DD0);

INCLUDE_RODATA(const s32, "effect/effPCPBoss", D_00533DE0);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_Thunder_Create);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_Thunder_ProcessDraw);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_Thunder_CreatePCF);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_Thunder_Delete);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_Thunder_Copy);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_Thunder_DrawSub);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_Thunder_Draw);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_Thunder_SetColor);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_Thunder_SetPosition);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_Thunder_SetRotateMatrix);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_Damage_Create);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_Damage_CreatePCF);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_Damage_Copy);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_Damage_Delete);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_Damage_Run);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_Damage_SetColor);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_PBCamera_Create);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_PBCamera_CreatePCF);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_PBCamera_Copy);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_PBCamera_Delete);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_PBCamera_Run);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_PBCamera_SetPosition);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_PBCamera_SetRotateMatrix);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_ShowTime_Create);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_ShowTime_CreatePCF);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_ShowTime_Copy);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_ShowTime_Delete);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_Sprite_Draw);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_ShowTime_Bingo_Draw);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_ShowTime_AreYouReady_Draw);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_ShowTime_ShowTime_Draw);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_ShowTime_Draw);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_ShowTime_SetPosition);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_BED_Create);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_BED_CreatePCF);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_BED_Copy);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_BED_Delete);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_BED_MatrixToQuaternion);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_BED_Draw);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_BED_SetPosition);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_BED_SetRotateMatrix);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_BED_SetColor);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_HBED2_Create);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_HBED2_CreatePCF);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_HBED2_Copy);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_HBED2_Delete);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_HBED2_Draw);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_HBED2_SetPosition);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_HBED2_SetRotateMatrix);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_HBED2_SetColor);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_HBED2_SetScale);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPadVibeFadeUpdate);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPadVibeFadeOUT);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPadVibeFadeIN);

INCLUDE_ASM(const s32, "effect/effPCPBoss", dds3EffectBlurDrawQuad);

INCLUDE_SDATA(const s32, "effect/effPCPBoss", effPCPBossTexHandle);

INCLUDE_SDATA(const s32, "effect/effPCPBoss", D_0054F1F4);

INCLUDE_SDATA(const s32, "effect/effPCPBoss", effPCPBossONGY_RFrame);

INCLUDE_SDATA(const s32, "effect/effPCPBoss", D_0054F1FC);

INCLUDE_SDATA(const s32, "effect/effPCPBoss", D_0054F210);

INCLUDE_SDATA(const s32, "effect/effPCPBoss", D_0054F218);

INCLUDE_RODATA(const s32, "effect/effPCPBoss", D_00533E10);

