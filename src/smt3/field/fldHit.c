#include "fpu.h"
#include "common.h"
#include "libsdf.h"
#include "kernel.h"
#include "ps2asm.h"
#include "basic.h"
#include "billboard.h"
#include "draw.h"
#include "scr.h"
#include "draw3d.h"
#include "field.h"
#include "model.h"
#include "motion.h"
#include "newdata.h"
#include "event.h"
#include "effect.h"

typedef struct fldCollisionFace_t fldCollisionFace_t;

typedef struct collision_s collision_s;

typedef struct fldCollisionDescriptor_t fldCollisionDescriptor_t;

typedef struct fldWallMeshRecord_t fldWallMeshRecord_t;

struct fldCollisionFace_t {
    u32 attributes;
    u8 moveFloor;
    u8 sound;
    u16 stop;
    u16 place;
    u8 automap[2];
    u32 vertexIndices[4];
    s16 encounterType;
    s16 encounter;
    s16 special[2];
};

struct collision_s {
    s16 mode2D;
    s16 numedges;
    f32 normal[4];
    f32 num;
    f32 e_normal[4][4];
    f32 limit[4];
    f32 bound[4];
    f32 vertices[4][4];
    f32 elv[3];
    u32 unkC4;
    u16 unkC8;
    u16 unkCA;
    s32 id;
    dds3Basic_t *matter;
    fldCollisionFace_t *pFace;
    void *stopData;
    s32 hojiID;
    s32 npcID;
};

struct fldCollisionDescriptor_t {
    s32 vertexCount;
    s32 faceCount;
    s32 triangleCount;
    f32 (*vertices)[4];
    fldCollisionFace_t *faces;
    void *stopData;
};

struct fldWallMeshRecord_t {
    f32 vertices[8][4];
    fldCollisionFace_t faces[4];
    fldCollisionDescriptor_t geometry;
};

extern void *memset(void *, int, unsigned int);

extern u32 fldItemBoxObjBuf;

extern u32 fldMatterTable;

extern s32 D_0054EDA0;

extern s32 fldPlaneCount;

u32 dds3AllocObjectId(void);

extern s32 fldItemBoxObjCount;

extern void fldHitChkUnitElv(f32 *phys_pos);

extern void fldHitChkUnit(f32 *phys_pos);

void fldFreeMatterTable(void);

f32 fldVecDot(sdfVector4_t *a, sdfVector4_t *b);

f32 MATH_VectorLength(sdfVector4_t *v)
{
    f32 len;

    len = sdfSqrt(v->x * v->x + v->y * v->y + v->z * v->z);
    return len;
}

void fldMakeEveHitPlanes(f32 *vertices, f32 *normalX, f32 *normalY, f32 *normalZ, f32 *planeConstant) {
    f32 firstPoint[4];
    f32 secondPoint[4];
    f32 thirdPoint[4];
    f32 inverseLength;

    firstPoint[0] = vertices[0];
    firstPoint[1] = vertices[1];
    firstPoint[2] = vertices[2];
    secondPoint[0] = vertices[4];
    secondPoint[1] = vertices[5];
    secondPoint[2] = vertices[6];
    thirdPoint[0] = vertices[8];
    thirdPoint[1] = vertices[9];
    thirdPoint[2] = vertices[10];
    *normalX = (secondPoint[1] - firstPoint[1]) * (thirdPoint[2] - secondPoint[2]) - (secondPoint[2] - firstPoint[2]) * (thirdPoint[1] - secondPoint[1]);
    *normalY = (secondPoint[2] - firstPoint[2]) * (thirdPoint[0] - secondPoint[0]) - (secondPoint[0] - firstPoint[0]) * (thirdPoint[2] - secondPoint[2]);
    *normalZ = (secondPoint[0] - firstPoint[0]) * (thirdPoint[1] - secondPoint[1]) - (secondPoint[1] - firstPoint[1]) * (thirdPoint[0] - secondPoint[0]);
    inverseLength = 1.0f / fsqrtf(*normalX * *normalX + *normalY * *normalY + *normalZ * *normalZ);
    *normalX = *normalX * inverseLength;
    *normalY = *normalY * inverseLength;
    *normalZ = *normalZ * inverseLength;
    if (*normalX > -0.0001f && *normalX < 0.0001f) {
        *normalX = 0.0f;
    }
    if (*normalY > -0.0001f && *normalY < 0.0001f) {
        *normalY = 0.0f;
    }
    if (*normalZ > -0.0001f && *normalZ < 0.0001f) {
        *normalZ = 0.0f;
    }
    *planeConstant = -(*normalX * thirdPoint[0] + *normalY * thirdPoint[1] + *normalZ * thirdPoint[2]);
}

dds3Basic_t *fldFindMatterIndex(s32 id) {
    s32 index = 0;

    if (D_0054EDA0 > 0) {
        collision_s *record = (collision_s *)fldMatterTable;
        do {
            if (record->id == id) {
                return record->matter;
            }
            index++;
            record++;
        } while (index < D_0054EDA0);
    }
    return 0;
}

void fldFindMatterIndexFrom(s32 id, dds3Basic_t *worldNode) {
    s32 index;

    for (index = 0; index < D_0054EDA0; index++) {
        if (((collision_s *)fldMatterTable)[index].id == id) {
            ((collision_s *)fldMatterTable)[index].matter = worldNode;
        }
    }
}

extern s32 D_0054ED90;

extern s32 fldHitChkMatter;

void fldClearMatterFlags(void) {
    s32 count = D_0054EDA0;

    if (count > 0) {
        dds3Basic_t **node = &((collision_s *)fldMatterTable)->matter;
        do {
            count--;
            *node = NULL;
            node = (dds3Basic_t **)((u8 *)node + sizeof(collision_s));
        } while (count != 0);
    }
    D_0054EDA0 = 0;
    fldHitChkMatter = -1;
    D_0054ED90 = -1;
    fldPlaneCount = 0;
    fldItemBoxObjCount = 0;
    if (fldMatterTable != 0) {
        fldFreeMatterTable();
    }
}

void fldMakePlane(fldCollisionDescriptor_t *geometry, f32 *translation,
                   u32 id, s32 sceneFlag, s32 ownerIndex);

INCLUDE_ASM(const s32, "field/fldHit", fldMakePlane);

s32 HitPositionInside2(f32 *fret, s32 mode2D, s32 numE, f32 *pos, collision_s *r, f32 collision) {
    f32 probe[3];
    f32 planar[2];
    f32 dist;
    f32 slack;
    s32 inside = 0;
    s32 planeIndex;
    f32 best = collision;

    if (mode2D == 0) {
        probe[0] = 0.0f;
        probe[1] = pos[1];
        probe[2] = pos[2];
        planar[0] = pos[1];
        planar[1] = pos[2];
    } else if (mode2D == 1) {
        probe[0] = pos[0];
        probe[1] = 0.0f;
        probe[2] = pos[2];
        planar[0] = pos[0];
        planar[1] = pos[2];
    } else {
        probe[0] = pos[0];
        probe[1] = pos[1];
        probe[2] = 0.0f;
        planar[0] = pos[0];
        planar[1] = pos[1];
    }
    for (planeIndex = 0; planeIndex < numE; planeIndex++) {
        dist = fldVecDot((sdfVector4_t *)probe, (sdfVector4_t *)r->e_normal[planeIndex]) - r->limit[planeIndex];
        slack = dist + collision;
        if (slack < 0.0f) {
            *fret = -1.0f;
            return -1;
        }
        if (planar[0] < r->bound[0] - collision || planar[1] < r->bound[1] - collision ||
            r->bound[2] + collision < planar[0] || r->bound[3] + collision < planar[1]) {
            *fret = -1.0f;
            return -1;
        }
        if (dist < 0.0f) {
            if (mode2D == 1) {
                best = dist + 45.0f;
            } else {
                best = slack;
            }
            inside = 1;
        }
    }
    if (best < 0.001f) {
        best = -1.0f;
    }
    *fret = best;
    return inside;
}

extern f32 fldHitChkWork[];

extern s32 fldGbChkHit(s32 fld, s32 area, s32 blk);

extern s32 fldGbChkNpc(s32 fld, s32 area, s32 hojiID);

void fldHitChkUnitElvOnly(f32 *phys_pos) {
    f32 current[4];
    f32 previous[4];
    f32 currentSave[4];
    f32 previousSave[4];
    f32 travel[4];
    f32 clearance;
    f32 radius;
    f32 distance;
    s32 index;
    s32 moved;
    s32 inside;

    current[0] = phys_pos[0];
    current[1] = phys_pos[1];
    current[2] = phys_pos[2];
    previous[0] = fldHitChkWork[0];
    previous[1] = fldHitChkWork[1];
    previous[2] = fldHitChkWork[2];
    if (fldGlobalBase.area == 1) {
        radius = 25.0f;
        current[1] = phys_pos[1] - 35.0f;
    } else if (fldGlobalBase.area < 10) {
        radius = 30.0f;
        current[1] = phys_pos[1] - 35.0f;
    } else {
        radius = 45.0f;
        current[1] = phys_pos[1] - 35.0f;
    }
    travel[0] = sdfFabs(current[0] - previous[0]);
    travel[2] = sdfFabs(current[2] - previous[2]);
    for (index = 0; index < D_0054EDA0; index++) {
        if (!(((collision_s *)fldMatterTable)[index].pFace->attributes & 0x80)) {
            continue;
        }
        if (fldGbChkHit(fldGlobalBase.area, fldGlobalBase.floor + 1,
                        ((collision_s *)fldMatterTable)[index].hojiID) != 0) {
            continue;
        }
        if (((collision_s *)fldMatterTable)[index].npcID > 0 &&
            fldGbChkNpc(fldGlobalBase.area, fldGlobalBase.floor + 1,
                        ((collision_s *)fldMatterTable)[index].npcID) != 0) {
            continue;
        }
        currentSave[0] = current[0];
        currentSave[1] = current[1];
        currentSave[2] = current[2];
        previousSave[0] = previous[0];
        previousSave[1] = previous[1];
        previousSave[2] = previous[2];
        if (((collision_s *)fldMatterTable)[index].matter != NULL) {
            moved = 1;
            current[0] = currentSave[0] - ((collision_s *)fldMatterTable)[index].matter->poseset->pos.x;
            current[1] = currentSave[1] - ((collision_s *)fldMatterTable)[index].matter->poseset->pos.y;
            current[2] = currentSave[2] - ((collision_s *)fldMatterTable)[index].matter->poseset->pos.z;
            previous[0] = previousSave[0] - ((collision_s *)fldMatterTable)[index].matter->poseset->pos.x;
            previous[1] = previousSave[1] - ((collision_s *)fldMatterTable)[index].matter->poseset->pos.y;
            previous[2] = previousSave[2] - ((collision_s *)fldMatterTable)[index].matter->poseset->pos.z;
        } else {
            moved = 0;
        }
        distance = sdfFabs(fldVecDot((sdfVector4_t *)current,
                                       (sdfVector4_t *)((collision_s *)fldMatterTable)[index].normal) -
                                 ((collision_s *)fldMatterTable)[index].num);
        if (distance < radius) {
            HitPositionInside2(&clearance, ((collision_s *)fldMatterTable)[index].mode2D,
                               ((collision_s *)fldMatterTable)[index].numedges, current,
                               &((collision_s *)fldMatterTable)[index], radius);
            if (clearance < 0.0f) {
                inside = 0;
            } else {
                inside = 1;
            }
            if (inside == 1 && ((collision_s *)fldMatterTable)[index].matter != NULL) {
                fldGlobalBase.hitMatterIndex = index;
                ((collision_s *)fldMatterTable)[index].elv[0] =
                    ((collision_s *)fldMatterTable)[index].matter->poseset->pos.x;
                ((collision_s *)fldMatterTable)[index].elv[1] =
                    ((collision_s *)fldMatterTable)[index].matter->poseset->pos.y;
                ((collision_s *)fldMatterTable)[index].elv[2] =
                    ((collision_s *)fldMatterTable)[index].matter->poseset->pos.z;
            }
        }
        if (moved != 0) {
            current[0] += ((collision_s *)fldMatterTable)[index].matter->poseset->pos.x;
            current[1] += ((collision_s *)fldMatterTable)[index].matter->poseset->pos.y;
            current[2] += ((collision_s *)fldMatterTable)[index].matter->poseset->pos.z;
            previous[0] += ((collision_s *)fldMatterTable)[index].matter->poseset->pos.x;
            previous[1] += ((collision_s *)fldMatterTable)[index].matter->poseset->pos.y;
            previous[2] += ((collision_s *)fldMatterTable)[index].matter->poseset->pos.z;
        }
    }
}

/* NON_MATCHING: 34 of 97 words differ (first at +0x48). */
#ifdef NON_MATCHING
void fldHitChkUnitElv(f32 *phys_pos) {
    f32 result[4];
    f32 previous[4];
    f32 delta[4];
    collision_s *record;
    s32 index;
    f32 x;
    f32 y;
    f32 z;

    memset(delta, 0, sizeof(delta));
    delta[3] = 1.0f;
    fldGlobalBase.hitValid = 0;
    index = fldGlobalBase.hitMatterIndex;
    if (index == -1) {
        fldHitChkUnitElvOnly(phys_pos);
        index = fldGlobalBase.hitMatterIndex;
    }
    if (index == -1) {
        return;
    }
    record = &((collision_s *)fldMatterTable)[index];
    if (record->matter != NULL) {
        delta[0] = record->matter->poseset->pos.x - record->elv[0];
        delta[1] = record->matter->poseset->pos.y - record->elv[1];
        delta[2] = record->matter->poseset->pos.z - record->elv[2];
        record->elv[0] = record->matter->poseset->pos.x;
        record->elv[1] = record->matter->poseset->pos.y;
        record->elv[2] = record->matter->poseset->pos.z;
    }
    if (fldGbChkHit(fldGlobalBase.area, fldGlobalBase.floor + 1,
                    record->hojiID) != 0) {
        return;
    }
    x = phys_pos[0] + delta[0];
    y = phys_pos[1] + delta[1];
    z = phys_pos[2] + delta[2];
    previous[0] = fldHitChkWork[0];
    previous[1] = fldHitChkWork[1];
    previous[2] = fldHitChkWork[2];
    fldGlobalBase.hitValid = 1;
    fldGlobalBase.hitX = x;
    fldGlobalBase.hitY = y;
    fldGlobalBase.hitZ = z;
    phys_pos[0] = x;
    phys_pos[1] = y;
    phys_pos[2] = z;
    fldHitChkWork[0] = x;
    fldHitChkWork[1] = y;
    fldHitChkWork[2] = z;
    result[0] = x;
    result[1] = y;
    result[2] = z;
}
#else
INCLUDE_ASM(const s32, "field/fldHit", fldHitChkUnitElv);
#endif

INCLUDE_ASM(const s32, "field/fldHit", fldHitChkUnitDamage);

INCLUDE_ASM(const s32, "field/fldHit", fldHitChkUnit);

INCLUDE_ASM(const s32, "field/fldHit", fldHitChkUnitVWM);

void func_0013B658(void) {
}

void func_0013B660(void) {
}

void func_0013B668(void) {
}

INCLUDE_ASM(const s32, "field/fldHit", fldHitWM_Mask);

f32 HitPositionInside3(s32 mode2D, s32 numE, f32 pos, f32 *r, collision_s *collision) {
    f32 projectedPosition[3];
    f32 planarPosition[2];
    f32 clearance = pos;
    f32 planeDistance;
    f32 marginDistance;
    s32 planeIndex;

    if (mode2D == 0) {
        projectedPosition[0] = 0.0f;
        projectedPosition[1] = r[1];
        projectedPosition[2] = r[2];
        planarPosition[0] = r[1];
        planarPosition[1] = r[2];
    } else if (mode2D == 1) {
        projectedPosition[0] = r[0];
        projectedPosition[1] = 0.0f;
        projectedPosition[2] = r[2];
        planarPosition[0] = r[0];
        planarPosition[1] = r[2];
    } else {
        projectedPosition[0] = r[0];
        projectedPosition[1] = r[1];
        projectedPosition[2] = 0.0f;
        planarPosition[0] = r[0];
        planarPosition[1] = r[1];
    }
    for (planeIndex = 0; planeIndex < numE; planeIndex++) {
        planeDistance = fldVecDot((sdfVector4_t *)projectedPosition, (sdfVector4_t *)collision->e_normal[planeIndex]) - collision->limit[planeIndex];
        marginDistance = planeDistance + pos;
        if (marginDistance < 0.0f || planarPosition[0] < collision->bound[0] - pos || planarPosition[1] < collision->bound[1] - pos ||
            collision->bound[2] + pos < planarPosition[0] || collision->bound[3] + pos < planarPosition[1]) {
            return -1.0f;
        }
        if (planeDistance < 0.0f) {
            clearance = marginDistance;
        }
    }
    if (clearance < 0.001f) {
        clearance = -1.0f;
    }
    return clearance;
}

INCLUDE_ASM(const s32, "field/fldHit", fldHitChkCamera);

INCLUDE_ASM(const s32, "field/fldHit", fldHitChkEnemy);

void fldAddItemBoxObject(s32 id, f32 x, f32 y, f32 z, f32 extent, f32 height) {
    f32 origin[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    s32 index;
    f32 minX, minY, minZ, maxX, maxZ;

    index = fldItemBoxObjCount;
    minX = x - extent;
    maxX = x + extent;
    minY = y - height;
    /* Build the front boundary before the back boundary, matching the
     * native corner ordering. The traced sched1 death notes then place
     * the final Z/extent use on minZ; local allocation keeps Z with maxZ. */
    maxZ = z + extent;
    minZ = z - extent;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].geometry.vertexCount = 8;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].geometry.faceCount = 4;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].geometry.triangleCount = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].geometry.vertices = ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].geometry.faces = ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].geometry.stopData = NULL;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[0][0] = minX;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[0][1] = minY;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[0][2] = maxZ;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[0][3] = 1.0f;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[1][0] = maxX;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[1][1] = minY;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[1][2] = maxZ;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[1][3] = 1.0f;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[2][0] = minX;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[2][1] = minY;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[2][2] = minZ;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[2][3] = 1.0f;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[3][0] = maxX;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[3][1] = minY;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[3][2] = minZ;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[3][3] = 1.0f;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[4][0] = minX;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[4][1] = y;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[4][2] = maxZ;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[4][3] = 1.0f;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[5][0] = maxX;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[5][1] = y;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[5][2] = maxZ;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[5][3] = 1.0f;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[6][0] = minX;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[6][1] = y;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[6][2] = minZ;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[6][3] = 1.0f;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[7][0] = maxX;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[7][1] = y;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[7][2] = minZ;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[7][3] = 1.0f;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[0].attributes = 128;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[0].moveFloor = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[0].sound = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[0].stop = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[0].place = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[0].automap[0] = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[0].automap[1] = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[0].vertexIndices[0] = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[0].vertexIndices[1] = 1;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[0].vertexIndices[2] = 5;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[0].vertexIndices[3] = 4;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[0].encounterType = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[0].encounter = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[0].special[0] = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[0].special[1] = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[1].attributes = 128;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[1].moveFloor = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[1].sound = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[1].stop = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[1].place = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[1].automap[0] = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[1].automap[1] = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[1].vertexIndices[0] = 1;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[1].vertexIndices[1] = 3;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[1].vertexIndices[2] = 7;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[1].vertexIndices[3] = 5;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[1].encounterType = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[1].encounter = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[1].special[0] = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[1].special[1] = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[2].attributes = 128;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[2].moveFloor = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[2].sound = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[2].stop = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[2].place = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[2].automap[0] = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[2].automap[1] = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[2].vertexIndices[0] = 3;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[2].vertexIndices[1] = 2;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[2].vertexIndices[2] = 6;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[2].vertexIndices[3] = 7;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[2].encounterType = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[2].encounter = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[2].special[0] = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[2].special[1] = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[3].attributes = 128;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[3].moveFloor = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[3].sound = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[3].stop = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[3].place = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[3].automap[0] = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[3].automap[1] = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[3].vertexIndices[0] = 2;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[3].vertexIndices[1] = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[3].vertexIndices[2] = 4;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[3].vertexIndices[3] = 6;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[3].encounterType = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[3].encounter = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[3].special[0] = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[3].special[1] = 0;
    fldMakePlane(&((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].geometry,
                  origin, dds3AllocObjectId(), 0, id);
    fldItemBoxObjCount++;
}
INCLUDE_SDATA(const s32, "field/fldHit", fldItemBoxObjCount);

INCLUDE_SDATA(const s32, "field/fldHit", fldMatterTable);

INCLUDE_SDATA(const s32, "field/fldHit", D_0054EDA0);

INCLUDE_SDATA(const s32, "field/fldHit", fldPlaneCount);

INCLUDE_SDATA(const s32, "field/fldHit", D_0054EDA8);

INCLUDE_SDATA(const s32, "field/fldHit", D_0054EDAC);

