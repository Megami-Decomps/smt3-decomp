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

typedef struct fldCollisionDescriptor_t fldCollisionDescriptor_t;

typedef struct fldWallMeshRecord_t fldWallMeshRecord_t;





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

extern collision_s *fldMatterTable;

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
            if (record->resID == id) {
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
        if (((collision_s *)fldMatterTable)[index].resID == id) {
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

#ifdef NON_MATCHING
extern void fldAllocPlaneTable(void);
#endif

void fldMakePlane(fldCollisionDescriptor_t *geometry, f32 *translation,
                   u32 id, s32 sceneFlag, s32 ownerIndex) {
    f32 vertices[4][4];
    f32 copiedVertices[4][4];
    /* Retail never touches the fourth row at SP+0xB0..0xBF. The DDS1
     * fldBuildCollisionRecordsFromGeometry twin has the same gap. Three rows
     * shrink the frame to 0x190 instead of the native 0x1A0. */
    f32 edgeTriangle[4][4];
    f32 center[3];
    f32 capturedPlane[4];
    f32 minimum[2];
    f32 maximum[2];
    collision_s *record;
    s32 pass;
    s32 remaining;
    f32 (*sourceVertices)[4];
    void *stopData;
    s32 trianglesRemaining;
    fldCollisionFace_t *face;
    f32 *planeX;
    f32 *planeY;
    f32 *planeZ;
    f32 *limit0;
    f32 *limit1;
    f32 *limit2;
    f32 *limit3;
    f32 absX;
    f32 absY;
    f32 absZ;
    f32 runningMinimum;
    s32 i;
    s32 component;
    if (fldMatterTable == 0) {
        fldAllocPlaneTable();
    }
    for (pass = 0; pass < 5; pass++) {
        s32 faceCount;
        fldCollisionFace_t *firstFace;
        sourceVertices = geometry->vertices;
        faceCount = geometry->faceCount;
        firstFace = geometry->faces;
        stopData = geometry->stopData;
        trianglesRemaining = geometry->triangleCount;
        if (faceCount > 0) {
            face = firstFace;
            remaining = faceCount;
            for (; remaining != 0; remaining--, face++, trianglesRemaining--) {
                u32 vertexIndex;
                record = &((collision_s *)fldMatterTable)[D_0054EDA0];
                record->numedges = trianglesRemaining > 0 ? 3 : 4;
                vertexIndex = face->vertexIndices[0];
                vertices[0][0] = sourceVertices[vertexIndex][0] + translation[0];
                vertices[0][1] = sourceVertices[vertexIndex][1] + translation[1];
                vertices[0][2] = sourceVertices[vertexIndex][2] + translation[2];
                vertexIndex = face->vertexIndices[1];
                vertices[1][0] = sourceVertices[vertexIndex][0] + translation[0];
                vertices[1][1] = sourceVertices[vertexIndex][1] + translation[1];
                vertices[1][2] = sourceVertices[vertexIndex][2] + translation[2];
                vertexIndex = face->vertexIndices[2];
                vertices[2][0] = sourceVertices[vertexIndex][0] + translation[0];
                vertices[2][1] = sourceVertices[vertexIndex][1] + translation[1];
                vertices[2][2] = sourceVertices[vertexIndex][2] + translation[2];
                if (record->numedges == 3) {
                    for (i = 0; i < 3; i++) {
                        copiedVertices[i][0] = vertices[i][0];
                        copiedVertices[i][1] = vertices[i][1];
                        copiedVertices[i][2] = vertices[i][2];
                    }
                } else {
                    vertexIndex = face->vertexIndices[3];
                    vertices[3][0] = sourceVertices[vertexIndex][0] + translation[0];
                    vertices[3][1] = sourceVertices[vertexIndex][1] + translation[1];
                    vertices[3][2] = sourceVertices[vertexIndex][2] + translation[2];
                    for (i = 0; i < 4; i++) {
                        copiedVertices[i][0] = vertices[i][0];
                        copiedVertices[i][1] = vertices[i][1];
                        copiedVertices[i][2] = vertices[i][2];
                    }
                }
                fldMakeEveHitPlanes(copiedVertices[0], &capturedPlane[0],
                                     &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                for (i = 0; i < 3; i++) {
                    record->normal[i] = capturedPlane[i];
                }
                record->normal[3] = 0.0f;
                for (i = 0; i < record->numedges; i++) {
                    for (component = 0; component < 3; component++) {
                        record->point[i][component] = copiedVertices[i][component];
                    }
                    record->point[i][3] = 1.0f;
                }
                record->num = fldVecDot(record->normal, record->point[0]);
                absX = record->normal[0];
                absX = ffabsf(absX);
                absY = ffabsf(record->normal[1]);
                absZ = ffabsf(record->normal[2]);
                if (pass == 0) {
                    if (!(absX < absY) || !(absZ < absY)) continue;
                    if (record->normal[1] > 0.0f) continue;
                    if (record->normal[1] > -0.98f) continue;
                } else if (pass == 1) {
                    if (!(absX < absY) || !(absZ < absY)) continue;
                    if (record->normal[1] > 0.0f) continue;
                    if (record->normal[1] <= -0.98f) continue;
                } else if (pass == 2) {
                    if (!(absY <= absX) || !(absZ <= absX)) continue;
                } else if (pass == 3) {
                    if (!(absX < absZ) || !(absY <= absZ)) continue;
                } else if (pass == 4) {
                    if (!(absX <= absY) || !(absZ <= absY)) continue;
                    if (record->normal[1] <= 0.0f) continue;
                }
                if (absY <= absX && absZ <= absX) {
                    record->mode2D = 0;
                    runningMinimum = vertices[0][1];
                    minimum[0] = runningMinimum;
                    minimum[1] = vertices[0][2];
                    maximum[0] = minimum[0];
                    maximum[1] = minimum[1];
                    for (i = 1; i < record->numedges; i++) {
                        if (vertices[i][1] < runningMinimum) {
                            runningMinimum = vertices[i][1];
                            minimum[0] = runningMinimum;
                        }
                        if (vertices[i][2] < minimum[1]) minimum[1] = vertices[i][2];
                        if (vertices[i][1] > maximum[0]) maximum[0] = vertices[i][1];
                        if (vertices[i][2] > maximum[1]) maximum[1] = vertices[i][2];
                    }
                    record->min[0] = runningMinimum;
                    record->min[1] = minimum[1];
                    record->min[2] = maximum[0];
                    record->min[3] = maximum[1];
                    if (record->numedges == 3) {
                        center[0] = 0.0f;
                        center[1] = (vertices[0][1] + vertices[1][1] + vertices[2][1]) / 3.0f;
                        center[2] = (vertices[0][2] + vertices[1][2] + vertices[2][2]) / 3.0f;
                    } else {
                        center[0] = 0.0f;
                        center[1] = (vertices[0][1] + vertices[1][1] + vertices[2][1] + vertices[3][1]) * 0.25f;
                        center[2] = (vertices[0][2] + vertices[1][2] + vertices[2][2] + vertices[3][2]) * 0.25f;
                    }
                    edgeTriangle[0][0] = 0.0f;
                    edgeTriangle[0][1] = vertices[0][1];
                    edgeTriangle[0][2] = vertices[0][2];
                    edgeTriangle[1][0] = 0.0f;
                    edgeTriangle[1][1] = vertices[1][1];
                    edgeTriangle[1][2] = vertices[1][2];
                    edgeTriangle[2][0] = 10.0f;
                    edgeTriangle[2][1] = vertices[1][1];
                    edgeTriangle[2][2] = vertices[1][2];
                    planeX = &record->e_normal[0][0];
                    planeY = &record->e_normal[0][1];
                    planeZ = &record->e_normal[0][2];
                    limit0 = &record->e_num[0];
                    fldMakeEveHitPlanes(edgeTriangle[0], &capturedPlane[0],
                                         &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                    *planeX = capturedPlane[0];
                    *planeY = capturedPlane[1];
                    *planeZ = capturedPlane[2];
                    *limit0 = fldVecDot(planeX, edgeTriangle[0]);
                    if (fldVecDot(center, planeX) - *limit0 < 0.0f) {
                        *planeX = -capturedPlane[0];
                        *planeY = -capturedPlane[1];
                        *planeZ = -capturedPlane[2];
                        *limit0 = fldVecDot(planeX, edgeTriangle[0]);
                    }
                    edgeTriangle[0][0] = 0.0f;
                    edgeTriangle[0][1] = vertices[1][1];
                    edgeTriangle[0][2] = vertices[1][2];
                    edgeTriangle[1][0] = 0.0f;
                    edgeTriangle[1][1] = vertices[2][1];
                    edgeTriangle[1][2] = vertices[2][2];
                    edgeTriangle[2][0] = 10.0f;
                    edgeTriangle[2][1] = vertices[2][1];
                    edgeTriangle[2][2] = vertices[2][2];
                    planeX = &record->e_normal[1][0];
                    planeY = &record->e_normal[1][1];
                    planeZ = &record->e_normal[1][2];
                    limit1 = &record->e_num[1];
                    fldMakeEveHitPlanes(edgeTriangle[0], &capturedPlane[0],
                                         &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                    *planeX = capturedPlane[0];
                    *planeY = capturedPlane[1];
                    *planeZ = capturedPlane[2];
                    *limit1 = fldVecDot(planeX, edgeTriangle[1]);
                    if (fldVecDot(center, planeX) - *limit1 < 0.0f) {
                        *planeX = -capturedPlane[0];
                        *planeY = -capturedPlane[1];
                        *planeZ = -capturedPlane[2];
                        *limit1 = fldVecDot(planeX, edgeTriangle[1]);
                    }
                    if (record->numedges == 3) {
                        edgeTriangle[0][0] = 0.0f;
                        edgeTriangle[0][1] = vertices[2][1];
                        edgeTriangle[0][2] = vertices[2][2];
                        edgeTriangle[1][0] = 0.0f;
                        edgeTriangle[1][1] = vertices[0][1];
                        edgeTriangle[1][2] = vertices[0][2];
                        edgeTriangle[2][0] = 10.0f;
                        edgeTriangle[2][1] = vertices[0][1];
                        edgeTriangle[2][2] = vertices[0][2];
                        planeX = &record->e_normal[2][0];
                        planeY = &record->e_normal[2][1];
                        planeZ = &record->e_normal[2][2];
                        limit2 = &record->e_num[2];
                        fldMakeEveHitPlanes(edgeTriangle[0], &capturedPlane[0],
                                             &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                        *planeX = capturedPlane[0];
                        *planeY = capturedPlane[1];
                        *planeZ = capturedPlane[2];
                        *limit2 = fldVecDot(planeX, edgeTriangle[2]);
                        if (fldVecDot(center, planeX) - *limit2 < 0.0f) {
                            *planeX = -capturedPlane[0];
                            *planeY = -capturedPlane[1];
                            *planeZ = -capturedPlane[2];
                            *limit2 = fldVecDot(planeX, edgeTriangle[2]);
                        }
                    } else {
                        edgeTriangle[0][0] = 0.0f;
                        edgeTriangle[0][1] = vertices[2][1];
                        edgeTriangle[0][2] = vertices[2][2];
                        edgeTriangle[1][0] = 0.0f;
                        edgeTriangle[1][1] = vertices[3][1];
                        edgeTriangle[1][2] = vertices[3][2];
                        edgeTriangle[2][0] = 10.0f;
                        edgeTriangle[2][1] = vertices[3][1];
                        edgeTriangle[2][2] = vertices[3][2];
                        planeX = &record->e_normal[2][0];
                        planeY = &record->e_normal[2][1];
                        planeZ = &record->e_normal[2][2];
                        limit2 = &record->e_num[2];
                        fldMakeEveHitPlanes(edgeTriangle[0], &capturedPlane[0],
                                             &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                        *planeX = capturedPlane[0];
                        *planeY = capturedPlane[1];
                        *planeZ = capturedPlane[2];
                        *limit2 = fldVecDot(planeX, edgeTriangle[2]);
                        if (fldVecDot(center, planeX) - *limit2 < 0.0f) {
                            *planeX = -capturedPlane[0];
                            *planeY = -capturedPlane[1];
                            *planeZ = -capturedPlane[2];
                            *limit2 = fldVecDot(planeX, edgeTriangle[2]);
                        }
                        edgeTriangle[0][0] = 0.0f;
                        edgeTriangle[0][1] = vertices[3][1];
                        edgeTriangle[0][2] = vertices[3][2];
                        edgeTriangle[1][0] = 0.0f;
                        edgeTriangle[1][1] = vertices[0][1];
                        edgeTriangle[1][2] = vertices[0][2];
                        edgeTriangle[2][0] = 10.0f;
                        edgeTriangle[2][1] = vertices[0][1];
                        edgeTriangle[2][2] = vertices[0][2];
                        planeX = &record->e_normal[3][0];
                        planeY = &record->e_normal[3][1];
                        planeZ = &record->e_normal[3][2];
                        limit3 = &record->e_num[3];
                        fldMakeEveHitPlanes(edgeTriangle[0], &capturedPlane[0],
                                             &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                        *planeX = capturedPlane[0];
                        *planeY = capturedPlane[1];
                        *planeZ = capturedPlane[2];
                        *limit3 = fldVecDot(planeX, edgeTriangle[2]);
                        if (fldVecDot(center, planeX) - *limit3 < 0.0f) {
                            *planeX = -capturedPlane[0];
                            *planeY = -capturedPlane[1];
                            *planeZ = -capturedPlane[2];
                            *limit3 = fldVecDot(planeX, edgeTriangle[2]);
                        }
                    }
                }
                else if (absX <= absY && absZ <= absY) {
                    record->mode2D = 1;
                    runningMinimum = vertices[0][0];
                    minimum[0] = runningMinimum;
                    minimum[1] = vertices[0][2];
                    maximum[0] = minimum[0];
                    maximum[1] = minimum[1];
                    for (i = 1; i < record->numedges; i++) {
                        if (vertices[i][0] < runningMinimum) {
                            runningMinimum = vertices[i][0];
                            minimum[0] = runningMinimum;
                        }
                        if (vertices[i][2] < minimum[1]) minimum[1] = vertices[i][2];
                        if (vertices[i][0] > maximum[0]) maximum[0] = vertices[i][0];
                        if (vertices[i][2] > maximum[1]) maximum[1] = vertices[i][2];
                    }
                    record->min[0] = runningMinimum;
                    record->min[1] = minimum[1];
                    record->min[2] = maximum[0];
                    record->min[3] = maximum[1];
                    if (record->numedges == 3) {
                        center[1] = 0.0f;
                        center[0] = (vertices[0][0] + vertices[1][0] + vertices[2][0]) / 3.0f;
                        center[2] = (vertices[0][2] + vertices[1][2] + vertices[2][2]) / 3.0f;
                    } else {
                        center[1] = 0.0f;
                        center[0] = (vertices[0][0] + vertices[1][0] + vertices[2][0] + vertices[3][0]) * 0.25f;
                        center[2] = (vertices[0][2] + vertices[1][2] + vertices[2][2] + vertices[3][2]) * 0.25f;
                    }
                    edgeTriangle[0][0] = vertices[0][0];
                    edgeTriangle[0][1] = 0.0f;
                    edgeTriangle[0][2] = vertices[0][2];
                    edgeTriangle[1][0] = vertices[1][0];
                    edgeTriangle[1][1] = 0.0f;
                    edgeTriangle[1][2] = vertices[1][2];
                    edgeTriangle[2][0] = vertices[1][0];
                    edgeTriangle[2][1] = 10.0f;
                    edgeTriangle[2][2] = vertices[1][2];
                    planeX = &record->e_normal[0][0];
                    planeY = &record->e_normal[0][1];
                    planeZ = &record->e_normal[0][2];
                    limit0 = &record->e_num[0];
                    fldMakeEveHitPlanes(edgeTriangle[0], &capturedPlane[0],
                                         &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                    *planeX = capturedPlane[0];
                    *planeY = capturedPlane[1];
                    *planeZ = capturedPlane[2];
                    *limit0 = fldVecDot(planeX, edgeTriangle[0]);
                    if (fldVecDot(center, planeX) - *limit0 < 0.0f) {
                        *planeX = -capturedPlane[0];
                        *planeY = -capturedPlane[1];
                        *planeZ = -capturedPlane[2];
                        *limit0 = fldVecDot(planeX, edgeTriangle[0]);
                    }
                    edgeTriangle[0][0] = vertices[1][0];
                    edgeTriangle[0][1] = 0.0f;
                    edgeTriangle[0][2] = vertices[1][2];
                    edgeTriangle[1][0] = vertices[2][0];
                    edgeTriangle[1][1] = 0.0f;
                    edgeTriangle[1][2] = vertices[2][2];
                    edgeTriangle[2][0] = vertices[2][0];
                    edgeTriangle[2][1] = 10.0f;
                    edgeTriangle[2][2] = vertices[2][2];
                    planeX = &record->e_normal[1][0];
                    planeY = &record->e_normal[1][1];
                    planeZ = &record->e_normal[1][2];
                    limit1 = &record->e_num[1];
                    fldMakeEveHitPlanes(edgeTriangle[0], &capturedPlane[0],
                                         &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                    *planeX = capturedPlane[0];
                    *planeY = capturedPlane[1];
                    *planeZ = capturedPlane[2];
                    *limit1 = fldVecDot(planeX, edgeTriangle[1]);
                    if (fldVecDot(center, planeX) - *limit1 < 0.0f) {
                        *planeX = -capturedPlane[0];
                        *planeY = -capturedPlane[1];
                        *planeZ = -capturedPlane[2];
                        *limit1 = fldVecDot(planeX, edgeTriangle[1]);
                    }
                    if (record->numedges == 3) {
                        edgeTriangle[0][0] = vertices[2][0];
                        edgeTriangle[0][1] = 0.0f;
                        edgeTriangle[0][2] = vertices[2][2];
                        edgeTriangle[1][0] = vertices[0][0];
                        edgeTriangle[1][1] = 0.0f;
                        edgeTriangle[1][2] = vertices[0][2];
                        edgeTriangle[2][0] = vertices[0][0];
                        edgeTriangle[2][1] = 10.0f;
                        edgeTriangle[2][2] = vertices[0][2];
                        planeX = &record->e_normal[2][0];
                        planeY = &record->e_normal[2][1];
                        planeZ = &record->e_normal[2][2];
                        limit2 = &record->e_num[2];
                        fldMakeEveHitPlanes(edgeTriangle[0], &capturedPlane[0],
                                             &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                        *planeX = capturedPlane[0];
                        *planeY = capturedPlane[1];
                        *planeZ = capturedPlane[2];
                        *limit2 = fldVecDot(planeX, edgeTriangle[2]);
                        if (fldVecDot(center, planeX) - *limit2 < 0.0f) {
                            *planeX = -capturedPlane[0];
                            *planeY = -capturedPlane[1];
                            *planeZ = -capturedPlane[2];
                            *limit2 = fldVecDot(planeX, edgeTriangle[2]);
                        }
                    } else {
                        edgeTriangle[0][0] = vertices[2][0];
                        edgeTriangle[0][1] = 0.0f;
                        edgeTriangle[0][2] = vertices[2][2];
                        edgeTriangle[1][0] = vertices[3][0];
                        edgeTriangle[1][1] = 0.0f;
                        edgeTriangle[1][2] = vertices[3][2];
                        edgeTriangle[2][0] = vertices[3][0];
                        edgeTriangle[2][1] = 10.0f;
                        edgeTriangle[2][2] = vertices[3][2];
                        planeX = &record->e_normal[2][0];
                        planeY = &record->e_normal[2][1];
                        planeZ = &record->e_normal[2][2];
                        limit2 = &record->e_num[2];
                        fldMakeEveHitPlanes(edgeTriangle[0], &capturedPlane[0],
                                             &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                        *planeX = capturedPlane[0];
                        *planeY = capturedPlane[1];
                        *planeZ = capturedPlane[2];
                        *limit2 = fldVecDot(planeX, edgeTriangle[2]);
                        if (fldVecDot(center, planeX) - *limit2 < 0.0f) {
                            *planeX = -capturedPlane[0];
                            *planeY = -capturedPlane[1];
                            *planeZ = -capturedPlane[2];
                            *limit2 = fldVecDot(planeX, edgeTriangle[2]);
                        }
                        edgeTriangle[0][0] = vertices[3][0];
                        edgeTriangle[0][1] = 0.0f;
                        edgeTriangle[0][2] = vertices[3][2];
                        edgeTriangle[1][0] = vertices[0][0];
                        edgeTriangle[1][1] = 0.0f;
                        edgeTriangle[1][2] = vertices[0][2];
                        edgeTriangle[2][0] = vertices[0][0];
                        edgeTriangle[2][1] = 10.0f;
                        edgeTriangle[2][2] = vertices[0][2];
                        planeX = &record->e_normal[3][0];
                        planeY = &record->e_normal[3][1];
                        planeZ = &record->e_normal[3][2];
                        limit3 = &record->e_num[3];
                        fldMakeEveHitPlanes(edgeTriangle[0], &capturedPlane[0],
                                             &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                        *planeX = capturedPlane[0];
                        *planeY = capturedPlane[1];
                        *planeZ = capturedPlane[2];
                        *limit3 = fldVecDot(planeX, edgeTriangle[2]);
                        if (fldVecDot(center, planeX) - *limit3 < 0.0f) {
                            *planeX = -capturedPlane[0];
                            *planeY = -capturedPlane[1];
                            *planeZ = -capturedPlane[2];
                            *limit3 = fldVecDot(planeX, edgeTriangle[2]);
                        }
                    }
                }
                else {
                    record->mode2D = 2;
                    runningMinimum = vertices[0][0];
                    minimum[0] = runningMinimum;
                    minimum[1] = vertices[0][1];
                    maximum[0] = minimum[0];
                    maximum[1] = minimum[1];
                    for (i = 1; i < record->numedges; i++) {
                        if (vertices[i][0] < runningMinimum) {
                            runningMinimum = vertices[i][0];
                            minimum[0] = runningMinimum;
                        }
                        if (vertices[i][1] < minimum[1]) minimum[1] = vertices[i][1];
                        if (vertices[i][0] > maximum[0]) maximum[0] = vertices[i][0];
                        if (vertices[i][1] > maximum[1]) maximum[1] = vertices[i][1];
                    }
                    record->min[0] = runningMinimum;
                    record->min[1] = minimum[1];
                    record->min[2] = maximum[0];
                    record->min[3] = maximum[1];
                    if (record->numedges == 3) {
                        center[2] = 0.0f;
                        center[0] = (vertices[0][0] + vertices[1][0] + vertices[2][0]) / 3.0f;
                        center[1] = (vertices[0][1] + vertices[1][1] + vertices[2][1]) / 3.0f;
                    } else {
                        center[2] = 0.0f;
                        center[0] = (vertices[0][0] + vertices[1][0] + vertices[2][0] + vertices[3][0]) * 0.25f;
                        center[1] = (vertices[0][1] + vertices[1][1] + vertices[2][1] + vertices[3][1]) * 0.25f;
                    }
                    edgeTriangle[0][0] = vertices[0][0];
                    edgeTriangle[0][1] = vertices[0][1];
                    edgeTriangle[0][2] = 0.0f;
                    edgeTriangle[1][0] = vertices[1][0];
                    edgeTriangle[1][1] = vertices[1][1];
                    edgeTriangle[1][2] = 0.0f;
                    edgeTriangle[2][0] = vertices[1][0];
                    edgeTriangle[2][1] = vertices[1][1];
                    edgeTriangle[2][2] = 10.0f;
                    planeX = &record->e_normal[0][0];
                    planeY = &record->e_normal[0][1];
                    planeZ = &record->e_normal[0][2];
                    limit0 = &record->e_num[0];
                    fldMakeEveHitPlanes(edgeTriangle[0], &capturedPlane[0],
                                         &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                    *planeX = capturedPlane[0];
                    *planeY = capturedPlane[1];
                    *planeZ = capturedPlane[2];
                    *limit0 = fldVecDot(planeX, edgeTriangle[0]);
                    if (fldVecDot(center, planeX) - *limit0 < 0.0f) {
                        *planeX = -capturedPlane[0];
                        *planeY = -capturedPlane[1];
                        *planeZ = -capturedPlane[2];
                        *limit0 = fldVecDot(planeX, edgeTriangle[0]);
                    }
                    edgeTriangle[0][0] = vertices[1][0];
                    edgeTriangle[0][1] = vertices[1][1];
                    edgeTriangle[0][2] = 0.0f;
                    edgeTriangle[1][0] = vertices[2][0];
                    edgeTriangle[1][1] = vertices[2][1];
                    edgeTriangle[1][2] = 0.0f;
                    edgeTriangle[2][0] = vertices[2][0];
                    edgeTriangle[2][1] = vertices[2][1];
                    edgeTriangle[2][2] = 10.0f;
                    planeX = &record->e_normal[1][0];
                    planeY = &record->e_normal[1][1];
                    planeZ = &record->e_normal[1][2];
                    limit1 = &record->e_num[1];
                    fldMakeEveHitPlanes(edgeTriangle[0], &capturedPlane[0],
                                         &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                    *planeX = capturedPlane[0];
                    *planeY = capturedPlane[1];
                    *planeZ = capturedPlane[2];
                    *limit1 = fldVecDot(planeX, edgeTriangle[1]);
                    if (fldVecDot(center, planeX) - *limit1 < 0.0f) {
                        *planeX = -capturedPlane[0];
                        *planeY = -capturedPlane[1];
                        *planeZ = -capturedPlane[2];
                        *limit1 = fldVecDot(planeX, edgeTriangle[1]);
                    }
                    if (record->numedges == 3) {
                        edgeTriangle[0][0] = vertices[2][0];
                        edgeTriangle[0][1] = vertices[2][1];
                        edgeTriangle[0][2] = 0.0f;
                        edgeTriangle[1][0] = vertices[0][0];
                        edgeTriangle[1][1] = vertices[0][1];
                        edgeTriangle[1][2] = 0.0f;
                        edgeTriangle[2][0] = vertices[0][0];
                        edgeTriangle[2][1] = vertices[0][1];
                        edgeTriangle[2][2] = 10.0f;
                        planeX = &record->e_normal[2][0];
                        planeY = &record->e_normal[2][1];
                        planeZ = &record->e_normal[2][2];
                        limit2 = &record->e_num[2];
                        fldMakeEveHitPlanes(edgeTriangle[0], &capturedPlane[0],
                                             &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                        *planeX = capturedPlane[0];
                        *planeY = capturedPlane[1];
                        *planeZ = capturedPlane[2];
                        *limit2 = fldVecDot(planeX, edgeTriangle[2]);
                        if (fldVecDot(center, planeX) - *limit2 < 0.0f) {
                            *planeX = -capturedPlane[0];
                            *planeY = -capturedPlane[1];
                            *planeZ = -capturedPlane[2];
                            *limit2 = fldVecDot(planeX, edgeTriangle[2]);
                        }
                    } else {
                        edgeTriangle[0][0] = vertices[2][0];
                        edgeTriangle[0][1] = vertices[2][1];
                        edgeTriangle[0][2] = 0.0f;
                        edgeTriangle[1][0] = vertices[3][0];
                        edgeTriangle[1][1] = vertices[3][1];
                        edgeTriangle[1][2] = 0.0f;
                        edgeTriangle[2][0] = vertices[3][0];
                        edgeTriangle[2][1] = vertices[3][1];
                        edgeTriangle[2][2] = 10.0f;
                        planeX = &record->e_normal[2][0];
                        planeY = &record->e_normal[2][1];
                        planeZ = &record->e_normal[2][2];
                        limit2 = &record->e_num[2];
                        fldMakeEveHitPlanes(edgeTriangle[0], &capturedPlane[0],
                                             &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                        *planeX = capturedPlane[0];
                        *planeY = capturedPlane[1];
                        *planeZ = capturedPlane[2];
                        *limit2 = fldVecDot(planeX, edgeTriangle[2]);
                        if (fldVecDot(center, planeX) - *limit2 < 0.0f) {
                            *planeX = -capturedPlane[0];
                            *planeY = -capturedPlane[1];
                            *planeZ = -capturedPlane[2];
                            *limit2 = fldVecDot(planeX, edgeTriangle[2]);
                        }
                        edgeTriangle[0][0] = vertices[3][0];
                        edgeTriangle[0][1] = vertices[3][1];
                        edgeTriangle[0][2] = 0.0f;
                        edgeTriangle[1][0] = vertices[0][0];
                        edgeTriangle[1][1] = vertices[0][1];
                        edgeTriangle[1][2] = 0.0f;
                        edgeTriangle[2][0] = vertices[0][0];
                        edgeTriangle[2][1] = vertices[0][1];
                        edgeTriangle[2][2] = 10.0f;
                        planeX = &record->e_normal[3][0];
                        planeY = &record->e_normal[3][1];
                        planeZ = &record->e_normal[3][2];
                        limit3 = &record->e_num[3];
                        fldMakeEveHitPlanes(edgeTriangle[0], &capturedPlane[0],
                                             &capturedPlane[1], &capturedPlane[2], &capturedPlane[3]);
                        *planeX = capturedPlane[0];
                        *planeY = capturedPlane[1];
                        *planeZ = capturedPlane[2];
                        *limit3 = fldVecDot(planeX, edgeTriangle[2]);
                        if (fldVecDot(center, planeX) - *limit3 < 0.0f) {
                            *planeX = -capturedPlane[0];
                            *planeY = -capturedPlane[1];
                            *planeZ = -capturedPlane[2];
                            *limit3 = fldVecDot(planeX, edgeTriangle[2]);
                        }
                    }
                }
                record->blockID = fldPlaneCount;
                record->resID = id;
                record->matter = 0;
                record->pFace = face;
                record->pTome = stopData;
                record->hojiID = sceneFlag;
                record->npcID = ownerIndex;
                record->hitflag = 0;
                D_0054EDA0++;
            }
        }
    }
    fldPlaneCount++;
}


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
        dist = fldVecDot((sdfVector4_t *)probe, (sdfVector4_t *)r->e_normal[planeIndex]) - r->e_num[planeIndex];
        slack = dist + collision;
        if (slack < 0.0f) {
            *fret = -1.0f;
            return -1;
        }
        if (planar[0] < r->min[0] - collision || planar[1] < r->min[1] - collision ||
            r->min[2] + collision < planar[0] || r->min[3] + collision < planar[1]) {
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
                ((collision_s *)fldMatterTable)[index].previousPosition[0] =
                    ((collision_s *)fldMatterTable)[index].matter->poseset->pos.x;
                ((collision_s *)fldMatterTable)[index].previousPosition[1] =
                    ((collision_s *)fldMatterTable)[index].matter->poseset->pos.y;
                ((collision_s *)fldMatterTable)[index].previousPosition[2] =
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

/* Adapted from Megami-Decomps/dds-decomp, fldFileResolver.c at f79f86dc.
 * Keep native indexed fields and old/new position snapshots. SMT3 has one
 * moving platform, unlike the DDS primary-plus-secondary platform path. */
void fldHitChkUnitElv(f32 *position) {
    f32 newPosition[3];
    f32 previous[3];
    f32 movement[4];
    s32 index;

    memset(movement, 0, sizeof(movement));
    movement[3] = 1.0f;
    fldGlobalBase.hitValid = 0;
    if (fldGlobalBase.hitMatterIndex == -1) {
        fldHitChkUnitElvOnly(position);
    }
    index = fldGlobalBase.hitMatterIndex;
    if (index != -1) {
        if (((collision_s *)fldMatterTable)[index].matter != 0) {
            dds3PoseSet_t *transform;

            transform = ((collision_s *)fldMatterTable)[index].matter->poseset;
            movement[0] = transform->pos.x - ((collision_s *)fldMatterTable)[index].previousPosition[0];
            movement[1] = transform->pos.y - ((collision_s *)fldMatterTable)[index].previousPosition[1];
            movement[2] = transform->pos.z - ((collision_s *)fldMatterTable)[index].previousPosition[2];
            ((collision_s *)fldMatterTable)[index].previousPosition[0] = transform->pos.x;
            ((collision_s *)fldMatterTable)[index].previousPosition[1] = transform->pos.y;
            ((collision_s *)fldMatterTable)[index].previousPosition[2] = transform->pos.z;
        }
        if (fldGbChkHit(fldGlobalBase.area, fldGlobalBase.floor + 1,
                               ((collision_s *)fldMatterTable)[index].hojiID) == 0) {
            newPosition[0] = position[0] + movement[0];
            newPosition[1] = position[1] + movement[1];
            newPosition[2] = position[2] + movement[2];
            previous[0] = fldHitChkWork[0];
            previous[1] = fldHitChkWork[1];
            previous[2] = fldHitChkWork[2];
            fldGlobalBase.hitValid = 1;
            fldGlobalBase.hitX = newPosition[0];
            fldGlobalBase.hitY = newPosition[1];
            fldGlobalBase.hitZ = newPosition[2];
            position[0] = newPosition[0];
            position[1] = newPosition[1];
            position[2] = newPosition[2];
            fldHitChkWork[0] = newPosition[0];
            fldHitChkWork[1] = newPosition[1];
            fldHitChkWork[2] = newPosition[2];
        }
    }
}

#ifdef NON_MATCHING
void fldHitChkUnitDamage(f32 *phys_pos)
{
    sdfVector4_t current;
    sdfVector4_t previous;
    sdfVector4_t delta;
    s32 axis;
    s32 pass;
    s32 index;
    s32 count;
    f32 radius;
    volatile s32 *damageFlag = &fldGlobalBase.wordB0;
    volatile f32 *position = phys_pos;

    *damageFlag = 0;
    current.x = position[0];
    current.y = position[1];
    current.z = position[2];
    previous.x = fldHitChkWork[0];
    previous.y = fldHitChkWork[1];
    previous.z = fldHitChkWork[2];
    if (fldGlobalBase.area == 1) {
        radius = 25.0f;
    } else if (fldGlobalBase.area < 10) {
        radius = 30.0f;
    } else {
        radius = 45.0f;
    }
    current.y -= 35.0f;
    delta.x = current.x - previous.x;
    delta.y = current.y - previous.y;
    delta.z = current.z - previous.z;
    if (MATH_VectorLength(&delta) < 0.099999997f) {
        phys_pos[0] = previous.x;
        phys_pos[2] = previous.z;
        phys_pos[1] = previous.y;
        return;
    }
    axis = sdfFabs(current.z - previous.z) < sdfFabs(current.x - previous.x) ? 0 : 2;
    fldHitChkUnitElvOnly(phys_pos);
    count = D_0054EDA0;
    for (pass = 0; pass < 2; pass++) {
        for (index = 0; index < count; index++) {
            collision_s *face = &fldMatterTable[index];
            f32 distance;
            f32 inside;

            if (fldGlobalBase.area >= 11) {
                radius = face->mode2D == 1 ? 45.0f : 60.0f;
            }
            if ((pass == 0 && face->mode2D == axis) || (pass != 0 && face->mode2D != axis)) {
                continue;
            }
            if (fldGbChkHit(fldGlobalBase.area, fldGlobalBase.floor + 1, face->hojiID) != 0) {
                count = D_0054EDA0;
                continue;
            }
            face = &fldMatterTable[index];
            if (face->matter != NULL) {
                dds3PoseSet_t *pose = face->matter->poseset;
                current.x -= pose->pos.x;
                current.y -= pose->pos.y;
                current.z -= pose->pos.z;
                previous.x -= pose->pos.x;
                previous.y -= pose->pos.y;
                previous.z -= pose->pos.z;
            }
            distance = fldVecDot(&current, (sdfVector4_t *)face->normal);
            face = &fldMatterTable[index];
            distance = sdfFabs(distance - face->num);
            if ((face->pFace->attributes & 0x4000) && distance < radius) {
                HitPositionInside2(&inside, face->mode2D, face->numedges, &current.x, face, radius);
                if (!(inside < 0.0f)) {
                    fldGlobalBase.wordB0 = 1;
                }
            }
            count = D_0054EDA0;
        }
    }
}
#else
INCLUDE_ASM(const s32, "field/fldHit", fldHitChkUnitDamage);
#endif

#ifdef NON_MATCHING
extern fldDanteGunState_t fldRunDanteGunFlag;
extern s8 dds3PadButtonState[];
extern s32 fldHitChkSlope, D_0054ED90;
extern f32 D_0054C468;
extern void fldAutoMapNameD(s32 name);
extern void fldAutoMapNameU(s32 name);
extern void fldAutoMapOpenBlock(s32 block);
extern void fldSkySetTblUpper(s32 index, s32 frames);
extern void fldSkySetTblModosu(s32 frames);
extern void rsetOtOfs(s32 offset);
extern void rsetTest(s32, s32, s32, s32, s32, s32, s32, s32);
extern void rsetAlpha(s32 mode);

void fldHitChkUnit(f32 *position)
{
    sdfVector4_t current, previous, delta, movement;
    f32 radius, distance, clearance;
    s32 axis, pass, index, floorHit = -1;
    s16 sky = -1;
    s32 debug;

    memset(&movement, 0, 16);
    debug = fldRunDanteGunFlag.word18 != 0;
    movement.w = 1.0f;
    fldHitChkSlope = -1;
    fldHitChkMatter = -1;
    fldAutoMapNameD(-1);
    fldGlobalBase.word44 = 0;
    if (fldGlobalBase.hitMatterIndex != -1) {
        collision_s *face = &fldMatterTable[fldGlobalBase.hitMatterIndex];
        if (face->matter != NULL) {
            dds3PoseSet_t *pose = face->matter->poseset;
            movement.x = pose->pos.x - face->previousPosition[0];
            movement.y = (pose->pos.y - face->previousPosition[1]) + 10.0f;
            movement.z = pose->pos.z - face->previousPosition[2];
        }
        fldGlobalBase.hitMatterIndex = -1;
    }
    if (debug) {
        rsetOtOfs(0x51);
        rsetTest(1, 5, 0x80, 1, 0, 0, 1, 2);
        rsetAlpha(0);
    }
    current.x = position[0] + movement.x;
    current.y = position[1] + movement.y;
    current.z = position[2] + movement.z;
    previous.x = fldHitChkWork[0];
    previous.y = fldHitChkWork[1];
    previous.z = fldHitChkWork[2];
    if (fldGlobalBase.area == 1) {
        radius = 25.0f;
    } else if (fldGlobalBase.area < 10) {
        radius = 30.0f;
    } else {
        radius = 45.0f;
    }
    current.y -= 35.0f;
    delta.x = current.x - previous.x;
    delta.y = current.y - previous.y;
    delta.z = current.z - previous.z;
    if (MATH_VectorLength(&delta) < D_0054C468) {
        position[0] = previous.x;
        position[2] = previous.z;
        position[1] = previous.y;
        return;
    }
    for (index = 0; index < D_0054EDA0; index++) {
        fldMatterTable[index].hitflag = 0;
    }
    axis = sdfFabs(current.z - previous.z) < sdfFabs(current.x - previous.x) ? 0 : 2;
    fldGlobalBase.word4C = -1;
    fldGlobalBase.wordC0 = 0;
    fldHitChkUnitElvOnly(position);
    for (pass = 0; pass < 2; pass++) {
        for (index = 0; index < D_0054EDA0; index++) {
            collision_s *face = &fldMatterTable[index];
            fldCollisionFace_t *data;
            u32 attributes;
            s32 moved;
            f32 signedDistance;

            if (fldGlobalBase.area >= 11) {
                radius = face->mode2D == 1 ? 45.0f : 60.0f;
            }
            if ((pass == 0 && face->mode2D == axis) ||
                (pass != 0 && face->mode2D != axis)) {
                continue;
            }
            if (fldGbChkHit(fldGlobalBase.area, fldGlobalBase.floor + 1, face->hojiID) != 0) {
                continue;
            }
            face = &fldMatterTable[index];
            if (face->npcID > 0 && fldGbChkNpc(fldGlobalBase.area, fldGlobalBase.floor + 1, face->npcID) != 0) {
                continue;
            }
            face = &fldMatterTable[index];
            moved = face->matter != NULL;
            if (moved) {
                dds3PoseSet_t *pose = face->matter->poseset;
                current.x -= pose->pos.x;
                current.y -= pose->pos.y;
                current.z -= pose->pos.z;
                previous.x -= pose->pos.x;
                previous.y -= pose->pos.y;
                previous.z -= pose->pos.z;
            }
            face = &fldMatterTable[index];
            data = face->pFace;
            attributes = data->attributes;
            signedDistance = fldVecDot(&current, (sdfVector4_t *)face->normal) - face->num;
            distance = sdfFabs(signedDistance);
            if (attributes & 0x40) {
                if (distance < radius) {
                    HitPositionInside2(&clearance, face->mode2D, face->numedges, &current.x, face, radius);
                    if (!(clearance < 0.0f)) {
                        u8 floor = fldMatterTable[index].pFace->moveFloor;
                        fldAutoMapNameD((floor & 15) * 10 + (floor >> 4));
                    }
                }
            } else if (attributes & 0x1000) {
                if (distance < radius) {
                    HitPositionInside2(&clearance, face->mode2D, face->numedges, &current.x, face, radius);
                    if (!(clearance < 0.0f)) {
                        fldAutoMapNameD(fldMatterTable[index].pFace->place);
                    }
                }
            } else if (attributes & 0x2000) {
                if (distance < radius) {
                    HitPositionInside2(&clearance, face->mode2D, face->numedges, &current.x, face, radius);
                    if (!(clearance < 0.0f)) {
                        fldGlobalBase.wordC0 = fldMatterTable[index].pFace->encounter;
                    }
                }
            } else if (attributes & 0x200) {
                if (distance < radius) {
                    HitPositionInside2(&clearance, face->mode2D, face->numedges, &current.x, face, radius);
                    if (!(clearance < 0.0f)) {
                        fldGlobalBase.word44 = fldMatterTable[index].pFace->sound;
                        if (fldGlobalBase.word44 == 3) {
                            fldGlobalBase.word4C = index;
                        }
                    }
                }
            } else if (attributes & 0x8000) {
                if (distance < radius) {
                    HitPositionInside2(&clearance, face->mode2D, face->numedges, &current.x, face, radius);
                    if (!(clearance < 0.0f)) {
                        face = &fldMatterTable[index];
                        data = face->pFace;
                        if (data->special[0] == 1) {
                            sky = data->special[1];
                        } else if (data->special[0] == 3) {
                            fldGlobalBase.floatC4 = face->point[0][1];
                        }
                    }
                }
            } else if (!(attributes & 0x400)) {
                if (attributes & 0x800) {
                    if (distance < radius) {
                        HitPositionInside2(&clearance, face->mode2D, face->numedges, &current.x, face, radius);
                        if (!(clearance < 0.0f)) {
                            fldAutoMapOpenBlock(fldMatterTable[index].pFace->automap[0]);
                            if (fldMatterTable[index].pFace->automap[1] != 0) {
                                fldAutoMapNameU(fldMatterTable[index].pFace->automap[1]);
                            }
                        }
                    }
                } else if (!(attributes & 0x4000)) {
                    if (attributes & 0x100) {
                        if (distance < radius) {
                            HitPositionInside2(&clearance, face->mode2D, face->numedges, &current.x, face, radius);
                            if (!(clearance < 0.0f)) {
                                fldHitChkSlope = index;
                            }
                        }
                    } else {
                        if (distance < radius) {
                            HitPositionInside2(&clearance, face->mode2D, face->numedges, &current.x, face, radius);
                            if (!(clearance < 0.0f) &&
                                (fldRunDanteGunFlag.counter == 0 || dds3PadButtonState[0x2B] == 0)) {
                                sdfVector4_t projected;
                                face = &fldMatterTable[index];
                                projected.x = current.x - face->normal[0] * signedDistance;
                                projected.y = current.y - face->normal[1] * signedDistance;
                                projected.z = current.z - face->normal[2] * signedDistance;
                                if (face->normal[1] < -0.75f) {
                                    fldHitChkMatter = index;
                                    if (floorHit >= 0 && clearance < distance) {
                                        clearance = distance;
                                    }
                                    current.x = projected.x + face->normal[0] * clearance;
                                    current.y = (projected.y + face->normal[1] * clearance) - 1.0f;
                                    current.z = projected.z + face->normal[2] * clearance;
                                    if (clearance == radius) {
                                        floorHit = index;
                                    }
                                    if (face->matter != NULL) {
                                        dds3PoseSet_t *pose = face->matter->poseset;
                                        moved = 0;
                                        current.x += pose->pos.x;
                                        current.y += pose->pos.y;
                                        current.z += pose->pos.z;
                                        previous.x += pose->pos.x;
                                        previous.y += pose->pos.y;
                                        previous.z += pose->pos.z;
                                    }
                                    fldMatterTable[index].hitflag = 1;
                                    fldGlobalBase.hitX = current.x;
                                    fldGlobalBase.hitY = current.y;
                                    fldGlobalBase.hitZ = current.z;
                                    fldGlobalBase.hitValid = 1;
                                    D_0054ED90 = index;
                                } else if (!(signedDistance < 0.0f)) {
                                    if (clearance < distance) {
                                        clearance = distance;
                                    }
                                    current.x = projected.x + face->normal[0] * clearance;
                                    current.y = projected.y + face->normal[1] * clearance;
                                    current.z = projected.z + face->normal[2] * clearance;
                                    if (face->matter != NULL) {
                                        dds3PoseSet_t *pose = face->matter->poseset;
                                        moved = 0;
                                        current.x += pose->pos.x;
                                        current.y += pose->pos.y;
                                        current.z += pose->pos.z;
                                        previous.x += pose->pos.x;
                                        previous.y += pose->pos.y;
                                        previous.z += pose->pos.z;
                                    }
                                    fldMatterTable[index].hitflag = 1;
                                    fldGlobalBase.hitValid = 1;
                                    fldGlobalBase.hitX = current.x;
                                    fldGlobalBase.hitY = current.y;
                                    fldGlobalBase.hitZ = current.z;
                                    D_0054ED90 = index;
                                }
                            }
                        }
                        if (moved) {
                            dds3PoseSet_t *pose = fldMatterTable[index].matter->poseset;
                            current.x += pose->pos.x;
                            current.y += pose->pos.y;
                            current.z += pose->pos.z;
                            previous.x += pose->pos.x;
                            previous.y += pose->pos.y;
                            previous.z += pose->pos.z;
                        }
                    }
                }
            }
        }
    }
    if (sky != -1) {
        fldSkySetTblUpper(sky, 5);
    } else {
        fldSkySetTblModosu(5);
    }
    delta.x = current.x - previous.x;
    delta.y = current.y - previous.y;
    delta.z = current.z - previous.z;
    MATH_VectorLength(&delta);
    position[0] = current.x;
    position[2] = current.z;
    position[1] = current.y;
    fldHitChkWork[2] = current.z;
    fldHitChkWork[0] = current.x;
    fldHitChkWork[1] = current.y;
}
#else
INCLUDE_ASM(const s32, "field/fldHit", fldHitChkUnit);
#endif

#ifdef NON_MATCHING
extern f32 D_0054C46C;
extern f32 fldCamCalcSlopeOffset(f32 x, f32 z, f32 *xs, f32 *ys, f32 *zs);

/* 487 of 808 words match, first differing word at +0x7C; 3232 of 3256 native
 * bytes. The 24 missing bytes are three `addiu $4,$0,1` for the HImode `1`
 * stored into hitflag (cse2 here substitutes the lowpart of the SImode `1`
 * already in `inside`, retail rematerialises it) plus the three R5900 pad
 * nops that follow from the shorter blocks. The rest is register choice:
 * the prologue table pointer and the fldGlobalBase base register. */
void fldHitChkUnitVWM(f32 *position)
{
    f32 current[4];
    f32 previous[4];
    f32 projected[4];
    f32 delta[4];
    f32 currentSave[4];
    f32 previousSave[4];
    f32 travel[4];
    f32 unused[4];  /* retail leaves 16 bytes unused here, at sp+0x70 */
    f32 movement[4];
    f32 xs[4];
    f32 ys[4];
    f32 zs[4];
    f32 radius;
    f32 distance;
    f32 clearance;
    f32 signedDistance;
    s32 axis;
    s32 pass;
    s32 index;
    s32 floorHit;
    s32 sky;
    s32 debug;
    s32 moved;
    s32 inside;
    collision_s *face;
    collision_s *mface;
    fldCollisionFace_t *data;
    u32 attributes;
    dds3PoseSet_t *pose;

    memset(movement, 0, 16);
    debug = fldRunDanteGunFlag.word18 != 0;
    movement[3] = 1.0f;
    sky = -1;
    fldHitChkSlope = -1;
    floorHit = -1;
    fldHitChkMatter = -1;
    fldAutoMapNameD(-1);
    fldGlobalBase.word44 = 0;
    if (fldGlobalBase.hitMatterIndex != -1) {
        index = fldGlobalBase.hitMatterIndex;
        face = &fldMatterTable[index];
        if (face->matter != NULL) {
            pose = face->matter->poseset;
            movement[0] = pose->pos.x - face->previousPosition[0];
            movement[1] = (pose->pos.y - face->previousPosition[1]) + 10.0f;
            movement[2] = pose->pos.z - face->previousPosition[2];
        }
        fldGlobalBase.hitMatterIndex = -1;
    }
    if (debug) {
        rsetOtOfs(0x51);
        rsetTest(1, 5, 0x80, 1, 0, 0, 1, 2);
        rsetAlpha(0);
    }
    previous[0] = fldHitChkWork[0];
    previous[1] = fldHitChkWork[1];
    previous[2] = fldHitChkWork[2];
    current[0] = position[0] + movement[0];
    current[1] = position[1] + movement[1];
    current[2] = position[2] + movement[2];
    if (fldGlobalBase.area == 1) {
        radius = 25.0f;
        current[1] = current[1] - 35.0f;
    } else if (fldGlobalBase.area < 10) {
        radius = 30.0f;
        current[1] = current[1] - 35.0f;
    } else {
        radius = 45.0f;
        current[1] = current[1] - 35.0f;
    }
    delta[0] = current[0] - previous[0];
    delta[1] = current[1] - previous[1];
    delta[2] = current[2] - previous[2];
    if (MATH_VectorLength((sdfVector4_t *)delta) < D_0054C46C) {
        position[0] = previous[0];
        position[2] = previous[2];
        position[1] = previous[1];
        return;
    }
    for (index = 0; index < D_0054EDA0; index++) {
        fldMatterTable[index].hitflag = 0;
    }
    travel[0] = sdfFabs(current[0] - previous[0]);
    travel[2] = sdfFabs(current[2] - previous[2]);
    fldGlobalBase.word4C = -1;
    fldGlobalBase.wordC0 = 0;
    if (travel[2] < travel[0]) {
        axis = 0;
    } else {
        axis = 2;
    }
    fldHitChkUnitElvOnly(position);
    for (pass = 0; pass < 2; pass++) {
        for (index = 0; index < D_0054EDA0; index++) {
            if (pass == 0) {
                if (fldMatterTable[index].mode2D == axis) {
                    continue;
                }
            } else {
                if (fldMatterTable[index].mode2D != axis) {
                    continue;
                }
            }
            if (fldGbChkHit(fldGlobalBase.area, fldGlobalBase.floor + 1,
                            fldMatterTable[index].hojiID) != 0) {
                continue;
            }
            if (fldMatterTable[index].npcID > 0 &&
                fldGbChkNpc(fldGlobalBase.area, fldGlobalBase.floor + 1,
                            fldMatterTable[index].npcID) != 0) {
                continue;
            }
            currentSave[0] = current[0];
            currentSave[1] = current[1];
            currentSave[2] = current[2];
            previousSave[0] = previous[0];
            previousSave[1] = previous[1];
            previousSave[2] = previous[2];
            if (fldMatterTable[index].matter != NULL) {
                pose = fldMatterTable[index].matter->poseset;
                moved = 1;
                current[0] = currentSave[0] - pose->pos.x;
                current[1] = currentSave[1] - pose->pos.y;
                current[2] = currentSave[2] - pose->pos.z;
                previous[0] = previousSave[0] - pose->pos.x;
                previous[1] = previousSave[1] - pose->pos.y;
                previous[2] = previousSave[2] - pose->pos.z;
            } else {
                moved = 0;
            }
            signedDistance = fldVecDot((sdfVector4_t *)current,
                                       (sdfVector4_t *)fldMatterTable[index].normal);
            face = &fldMatterTable[index];
            signedDistance = signedDistance - face->num;
            distance = sdfFabs(signedDistance);
            attributes = face->pFace->attributes;
            if (attributes & 0x40) {
                if (distance < radius) {
                    HitPositionInside2(&clearance, face->mode2D, face->numedges,
                                       current, face, radius);
                    if (clearance < 0.0f) {
                        inside = 0;
                    } else {
                        inside = 1;
                    }
                    if (inside == 1) {
                        u8 floor = fldMatterTable[index].pFace->moveFloor;
                        fldAutoMapNameD((floor & 15) * 10 + (floor >> 4));
                    }
                }
            } else if (attributes & 0x1000) {
                if (distance < radius) {
                    HitPositionInside2(&clearance, face->mode2D, face->numedges,
                                       current, face, radius);
                    if (clearance < 0.0f) {
                        inside = 0;
                    } else {
                        inside = 1;
                    }
                    if (inside == 1) {
                        fldAutoMapNameD(fldMatterTable[index].pFace->place);
                    }
                }
            } else if (attributes & 0x2000) {
                if (distance < radius) {
                    HitPositionInside2(&clearance, face->mode2D, face->numedges,
                                       current, face, radius);
                    if (clearance < 0.0f) {
                        inside = 0;
                    } else {
                        inside = 1;
                    }
                    if (inside == 1) {
                        fldGlobalBase.wordC0 = fldMatterTable[index].pFace->encounter;
                    }
                }
            } else if (attributes & 0x200) {
                if (distance < radius) {
                    HitPositionInside2(&clearance, face->mode2D, face->numedges,
                                       current, face, radius);
                    if (clearance < 0.0f) {
                        inside = 0;
                    } else {
                        inside = 1;
                    }
                    if (inside == 1) {
                        fldGlobalBase.word44 = fldMatterTable[index].pFace->sound;
                        if (fldGlobalBase.word44 == 3) {
                            fldGlobalBase.word4C = index;
                        }
                    }
                }
            } else if (attributes & 0x8000) {
                if (distance < radius) {
                    HitPositionInside2(&clearance, face->mode2D, face->numedges,
                                       current, face, radius);
                    if (clearance < 0.0f) {
                        inside = 0;
                    } else {
                        inside = 1;
                    }
                    if (inside == 1) {
                        data = fldMatterTable[index].pFace;
                        switch (data->special[0]) {
                        case 1:
                            sky = data->special[1];
                            break;
                        case 2:
                            break;
                        case 3:
                            fldGlobalBase.floatC4 =
                                fldMatterTable[index].point[0][1];
                            break;
                        }
                    }
                }
            } else if (attributes & 0x400) {
            } else if (attributes & 0x800) {
                if (distance < radius) {
                    HitPositionInside2(&clearance, face->mode2D, face->numedges,
                                       current, face, radius);
                    if (clearance < 0.0f) {
                        inside = 0;
                    } else {
                        inside = 1;
                    }
                    if (inside == 1) {
                        fldAutoMapOpenBlock(fldMatterTable[index].pFace->automap[0]);
                        if (fldMatterTable[index].pFace->automap[1] != 0) {
                            fldAutoMapNameU(fldMatterTable[index].pFace->automap[1]);
                        }
                    }
                }
            } else if (attributes & 0x4000) {
            } else if (attributes & 0x100) {
                if (distance < radius) {
                    HitPositionInside2(&clearance, face->mode2D, face->numedges,
                                       current, face, radius);
                    if (clearance < 0.0f) {
                        inside = 0;
                    } else {
                        inside = 1;
                    }
                    if (inside == 1) {
                        fldHitChkSlope = index;
                    }
                }
            } else {
                if (distance < radius) {
                    HitPositionInside2(&clearance, face->mode2D, face->numedges,
                                       current, face, radius);
                    if (clearance < 0.0f) {
                        inside = 0;
                    } else {
                        inside = 1;
                    }
                    if (inside == 1 &&
                        (fldRunDanteGunFlag.counter == 0 ||
                         dds3PadButtonState[0x2B] == 0)) {
                        mface = &fldMatterTable[index];
                        if (mface->mode2D == 1) {
                            xs[0] = mface->point[0][0];
                            ys[0] = mface->point[0][1];
                            zs[0] = mface->point[0][2];
                            xs[1] = mface->point[1][0];
                            ys[1] = mface->point[1][1];
                            zs[1] = mface->point[1][2];
                            xs[2] = mface->point[2][0];
                            ys[2] = mface->point[2][1];
                            zs[2] = mface->point[2][2];
                            current[1] = fldCamCalcSlopeOffset(current[0], current[2],
                                                               xs, ys, zs) - 35.0f;
                            fldHitChkMatter = index;
                            fldGlobalBase.hitValid = 1;
                            fldMatterTable[index].hitflag = 1;
                            fldGlobalBase.hitX = current[0];
                            fldGlobalBase.hitY = current[1];
                            fldGlobalBase.hitZ = current[2];
                            D_0054ED90 = index;
                        } else {
                            projected[0] = current[0] - mface->normal[0] * signedDistance;
                            projected[1] = current[1] - mface->normal[1] * signedDistance;
                            projected[2] = current[2] - mface->normal[2] * signedDistance;
                            if (mface->normal[1] < -0.75f) {
                                fldHitChkMatter = index;
                                if (floorHit >= 0 && clearance < distance) {
                                    clearance = distance;
                                }
                                mface = &fldMatterTable[index];
                                current[0] = projected[0] + mface->normal[0] * clearance;
                                current[1] = (projected[1] + mface->normal[1] * clearance) - 1.0f;
                                current[2] = projected[2] + mface->normal[2] * clearance;
                                if (clearance == radius) {
                                    floorHit = index;
                                }
                                if (mface->matter != NULL) {
                                    pose = mface->matter->poseset;
                                    moved = 0;
                                    current[0] = current[0] + pose->pos.x;
                                    current[1] = current[1] + pose->pos.y;
                                    current[2] = current[2] + pose->pos.z;
                                    previous[0] = previous[0] + pose->pos.x;
                                    previous[1] = previous[1] + pose->pos.y;
                                    previous[2] = previous[2] + pose->pos.z;
                                }
                                fldMatterTable[index].hitflag = 1;
                                fldGlobalBase.hitX = current[0];
                                fldGlobalBase.hitY = current[1];
                                fldGlobalBase.hitZ = current[2];
                                fldGlobalBase.hitValid = 1;
                                D_0054ED90 = index;
                            } else if (!(signedDistance < 0.0f)) {
                                if (clearance < distance) {
                                    clearance = distance;
                                }
                                mface = &fldMatterTable[index];
                                current[0] = projected[0] + mface->normal[0] * clearance;
                                current[1] = projected[1] + mface->normal[1] * clearance;
                                current[2] = projected[2] + mface->normal[2] * clearance;
                                if (mface->matter != NULL) {
                                    pose = mface->matter->poseset;
                                    moved = 0;
                                    current[0] = current[0] + pose->pos.x;
                                    current[1] = current[1] + pose->pos.y;
                                    current[2] = current[2] + pose->pos.z;
                                    previous[0] = previous[0] + pose->pos.x;
                                    previous[1] = previous[1] + pose->pos.y;
                                    previous[2] = previous[2] + pose->pos.z;
                                }
                                fldMatterTable[index].hitflag = 1;
                                fldGlobalBase.hitValid = 1;
                                fldGlobalBase.hitX = current[0];
                                fldGlobalBase.hitY = current[1];
                                fldGlobalBase.hitZ = current[2];
                                D_0054ED90 = index;
                            }
                        }
                    }
                }
                if (moved != 0) {
                    pose = fldMatterTable[index].matter->poseset;
                    current[0] = current[0] + pose->pos.x;
                    current[1] = current[1] + pose->pos.y;
                    current[2] = current[2] + pose->pos.z;
                    previous[0] = previous[0] + pose->pos.x;
                    previous[1] = previous[1] + pose->pos.y;
                    previous[2] = previous[2] + pose->pos.z;
                }
            }
        }
    }
    if (sky != -1) {
        fldSkySetTblUpper(sky, 5);
    } else {
        fldSkySetTblModosu(5);
    }
    delta[0] = current[0] - previous[0];
    delta[1] = current[1] - previous[1];
    delta[2] = current[2] - previous[2];
    MATH_VectorLength((sdfVector4_t *)delta);
    position[0] = current[0];
    position[2] = current[2];
    position[1] = current[1];
    fldHitChkWork[2] = current[2];
    fldHitChkWork[0] = current[0];
    fldHitChkWork[1] = current[1];
}
#else
INCLUDE_ASM(const s32, "field/fldHit", fldHitChkUnitVWM);
#endif

void func_0013B658(void) {
}

void func_0013B660(void) {
}

void func_0013B668(void) {
}

#ifdef NON_MATCHING
extern void rsetOtOfs(s32 offset);
extern void rsetTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 zte, s32 ztst);
extern void rsetAlpha(s32 mode);
extern void rsetFrameMaskRgbOn(void);
extern void rsetFrameMaskRgbOff(void);
extern void aDrawBoxF(s32 x, s32 y, s32 w, s32 h, u32 r, u32 g, u32 b, u32 a);
extern void aDrawBoxFZ(s32 x, s32 y, s32 w, s32 h, u32 z, u32 r, u32 g, u32 b, u32 a);
extern void fldDrawPoly3(s32 c0, s32 c1, s32 c2,
                         f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1,
                         f32 x2, f32 y2, f32 z2);
extern void fldDrawPoly4(s32 c0, f32 x0, f32 y0, f32 z0,
                         s32 c1, f32 x1, f32 y1, f32 z1,
                         s32 c2, f32 x2, f32 y2, f32 z2,
                         s32 c3, f32 x3, f32 y3, f32 z3);

void fldHitWM_Mask(void)
{
    s32 i;
    s32 count;
    collision_s *table;
    collision_s *face;
    sdfVector4_t pos;

    rsetOtOfs(0x2F);
    rsetTest(1, 0, 0x80, 1, 0, 0, 1, 1);
    rsetAlpha(1);
    rsetFrameMaskRgbOn();
    aDrawBoxF(0, 0, 0x200, 0xE0, 0x80, 0, 0, 0x10);

    pos.w = 1.0f;
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = 0.0f;
    count = D_0054EDA0;
    if (count > 0) {
        table = fldMatterTable;
        for (i = 0; i < count; i++) {
            face = &table[i];
            if (face->matter != 0) {
                pos.x = face->matter->poseset->pos.x;
                pos.y = face->matter->poseset->pos.y;
                pos.z = face->matter->poseset->pos.z;
            }
            if ((face->pFace->attributes & 0x8000) && face->pFace->special[0] != 1 &&
                face->pFace->special[0] != 3) {
                if (face->numedges == 3) {
                    fldDrawPoly3(0x80000000, 0x80000000, 0x80000000,
                                 face->point[0][0] + pos.x, face->point[0][1] + pos.y, face->point[0][2] + pos.z,
                                 face->point[1][0] + pos.x, face->point[1][1] + pos.y, face->point[1][2] + pos.z,
                                 face->point[2][0] + pos.x, face->point[2][1] + pos.y, face->point[2][2] + pos.z);
                } else {
                    fldDrawPoly4(0x80000000, face->point[0][0] + pos.x, face->point[0][1] + pos.y, face->point[0][2] + pos.z,
                                 0x80000000, face->point[1][0] + pos.x, face->point[1][1] + pos.y, face->point[1][2] + pos.z,
                                 0x80000000, face->point[2][0] + pos.x, face->point[2][1] + pos.y, face->point[2][2] + pos.z,
                                 0x80000000, face->point[3][0] + pos.x, face->point[3][1] + pos.y, face->point[3][2] + pos.z);
                }
                count = D_0054EDA0;
            }
            table = fldMatterTable;
        }
    }
    rsetTest(1, 0, 0x80, 2, 1, 1, 1, 1);
    rsetAlpha(0);
    aDrawBoxFZ(0, 0, 0x200, 0xE0, -1, 0x80, 0x80, 0x80, 0x80);
    rsetFrameMaskRgbOff();
    rsetTest(1, 5, 0x80, 1, 0, 0, 1, 2);
    rsetAlpha(0);
}
#else
INCLUDE_ASM(const s32, "field/fldHit", fldHitWM_Mask);
#endif

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
        planeDistance = fldVecDot((sdfVector4_t *)projectedPosition, (sdfVector4_t *)collision->e_normal[planeIndex]) - collision->e_num[planeIndex];
        marginDistance = planeDistance + pos;
        if (marginDistance < 0.0f || planarPosition[0] < collision->min[0] - pos || planarPosition[1] < collision->min[1] - pos ||
            collision->min[2] + pos < planarPosition[0] || collision->min[3] + pos < planarPosition[1]) {
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

/* Native stop-table rows have stride 8 and a name pointer at +4. */
typedef struct fldCollisionStopEntry_t {
    u32 reserved;
    const char *name;
} fldCollisionStopEntry_t;

extern dds3Basic_t *dds3GetControlScene(void);
extern dds3BasicArrayHandle_t *dds3SceneBasicGetList(dds3Basic_t *scene, s32 type);
extern u16 dds3GetBasicArrayCount(const dds3BasicArrayHandle_t *handle);
extern u32 dds3SetFirstBasicArray(dds3BasicArrayHandle_t *handle);
extern dds3Basic_t *dds3GetBasicArray(const dds3BasicArrayHandle_t *handle);
extern u32 dds3SetNextBasicArray(dds3BasicArrayHandle_t *handle);
extern void dds3FreeBasicArrayHandle(dds3BasicArrayHandle_t *handle);
extern u32 dds3MatterObjectBasicGetAlpMode(const dds3Basic_t *object);
extern void dds3MatterObjectBasicSetAlpMode(dds3Basic_t *object, u32 mode);
extern dds3Basic_t *dds3SceneBasicSearchNameAndType(dds3Basic_t *scene, s32 type, const char *name);

/* Adapted from Megami-Decomps/dds-decomp, fldFileResolver.c at f79f86dc.
 * SMT3 inlines the kind-6 alpha reset instead of calling the DDS world helper. */
s32 fldHitChkCamera(const f32 start[3], const f32 end[3], f32 hitPoint[3]) {
    f32 origin[4];
    f32 target[4];
    f32 delta[4];
    f32 probeStart[4];
    f32 probeEnd[4];
    f32 anchor[4];
    dds3PoseSet_t *transform;
    dds3Basic_t *secondary;
    f32 startDistance;
    f32 endDistance;
    f32 total;
    f32 ratio;
    f32 clearance;
    s32 found;
    fldCollisionStopEntry_t *entry;
    s32 relative;
    s32 index;

    found = -1;
    secondary = dds3GetControlScene();
    if (!((u32)(fldGlobalBase.area - 200) < 300)) {
        dds3BasicArrayHandle_t *objects;
        dds3Basic_t *object;

        objects = dds3SceneBasicGetList(secondary, 6);
        if (dds3GetBasicArrayCount(objects) != 0) {
            dds3SetFirstBasicArray(objects);
            do {
                object = dds3GetBasicArray(objects);
                if (dds3MatterObjectBasicGetAlpMode(object) == 4) {
                    dds3MatterObjectBasicSetAlpMode(object, 3);
                }
            } while (dds3SetNextBasicArray(objects) != 0);
            dds3FreeBasicArrayHandle(objects);
        }
    }
    origin[0] = start[0];
    origin[1] = start[1];
    origin[2] = start[2];
    target[0] = end[0];
    target[1] = end[1];
    target[2] = end[2];
    for (index = 0; index < D_0054EDA0; index++) {
        if (((collision_s *)fldMatterTable)[index].mode2D == 1) {
            if (!(((collision_s *)fldMatterTable)[index].pFace->attributes & 0x100)) {
                continue;
            }
        }
        if (((collision_s *)fldMatterTable)[index].pFace->attributes & 0xA01) {
            continue;
        }
        if (fldGbChkHit(fldGlobalBase.area, fldGlobalBase.floor + 1,
                        ((collision_s *)fldMatterTable)[index].hojiID)) {
            continue;
        }
        if (((collision_s *)fldMatterTable)[index].matter != 0) {
            relative = 1;
            transform = ((collision_s *)fldMatterTable)[index].matter->poseset;
            origin[0] -= transform->pos.x;
            origin[1] -= transform->pos.y;
            origin[2] -= transform->pos.z;
            target[0] -= transform->pos.x;
            target[1] -= transform->pos.y;
            target[2] -= transform->pos.z;
        } else {
            relative = 0;
        }
        probeStart[0] = origin[0];
        probeStart[1] = origin[1];
        probeStart[2] = origin[2];
        probeEnd[0] = target[0];
        probeEnd[1] = target[1];
        probeEnd[2] = target[2];
        startDistance = fldVecDot((sdfVector4_t *)probeStart, (sdfVector4_t *)((collision_s *)fldMatterTable)[index].normal) -
                        ((collision_s *)fldMatterTable)[index].num;
        endDistance = fldVecDot((sdfVector4_t *)probeEnd, (sdfVector4_t *)((collision_s *)fldMatterTable)[index].normal) -
                      ((collision_s *)fldMatterTable)[index].num;
        if ((startDistance < 0.0f && 0.0f <= endDistance) || (endDistance < 0.0f && 0.0f <= startDistance)) {
            startDistance = ffabsf(startDistance);
            endDistance = ffabsf(endDistance);
            total = startDistance + endDistance;
            ratio = startDistance / total;
            anchor[0] = probeStart[0];
            anchor[1] = probeStart[1];
            anchor[2] = probeStart[2];
            delta[0] = probeEnd[0] - probeStart[0];
            delta[1] = probeEnd[1] - probeStart[1];
            delta[2] = probeEnd[2] - probeStart[2];
            probeStart[0] += delta[0] * ratio;
            probeStart[1] += delta[1] * ratio;
            probeStart[2] += delta[2] * ratio;
            clearance = HitPositionInside3(((collision_s *)fldMatterTable)[index].mode2D, ((collision_s *)fldMatterTable)[index].numedges, 2.0f, probeStart, &((collision_s *)fldMatterTable)[index]);
            if (0.0f <= clearance) {
                if (((collision_s *)fldMatterTable)[index].pFace->attributes & 0x400) {
                    entry = &((fldCollisionStopEntry_t *)((collision_s *)fldMatterTable)[index].pTome)
                                [((collision_s *)fldMatterTable)[index].pFace->stop];
                    dds3MatterObjectBasicSetAlpMode(dds3SceneBasicSearchNameAndType(secondary, 6, entry->name), 4);
                } else {
                    found = index;
                    ratio = (startDistance + clearance) / total;
                    delta[0] = probeEnd[0] - anchor[0];
                    delta[1] = probeEnd[1] - anchor[1];
                    delta[2] = probeEnd[2] - anchor[2];
                    anchor[0] += delta[0] * ratio;
                    anchor[1] += delta[1] * ratio;
                    anchor[2] += delta[2] * ratio;
                    probeStart[0] = anchor[0];
                    probeStart[1] = anchor[1];
                    probeStart[2] = anchor[2];
                    origin[0] = anchor[0];
                    origin[1] = anchor[1];
                    origin[2] = anchor[2];
                }
            }
        }
        if (relative) {
            transform = ((collision_s *)fldMatterTable)[index].matter->poseset;
            origin[0] += transform->pos.x;
            origin[1] += transform->pos.y;
            origin[2] += transform->pos.z;
            target[0] += transform->pos.x;
            target[1] += transform->pos.y;
            target[2] += transform->pos.z;
        }
    }
    hitPoint[0] = origin[0];
    hitPoint[1] = origin[1];
    hitPoint[2] = origin[2];
    return found;
}

/* Adapted from Megami-Decomps/dds-decomp, fldFileResolver.c at f79f86dc.
 * SMT3 uses the 0xE01 surface mask and 70-unit clearance, without DDS tags. */
s32 fldHitChkEnemy(const f32 start[3], const f32 end[3], f32 hitPoint[3]) {
    f32 origin[4];
    f32 target[4];
    f32 delta[4];
    f32 probeStart[4];
    f32 probeEnd[4];
    f32 anchor[4];
    dds3PoseSet_t *transform;
    f32 startDistance;
    f32 endDistance;
    f32 total;
    f32 ratio;
    f32 clearance;
    s32 found;
    s32 relative;
    s32 index;

    found = -1;
    dds3GetControlScene();
    origin[0] = start[0];
    origin[1] = start[1];
    origin[2] = start[2];
    target[0] = end[0];
    target[1] = end[1];
    target[2] = end[2];
    for (index = 0; index < D_0054EDA0; index++) {
        if (((collision_s *)fldMatterTable)[index].mode2D == 1) {
            if (!(((collision_s *)fldMatterTable)[index].pFace->attributes & 0x100)) {
                continue;
            }
        }
        if (((collision_s *)fldMatterTable)[index].pFace->attributes & 0xE01) {
            continue;
        }
        if (fldGbChkHit(fldGlobalBase.area, fldGlobalBase.floor + 1,
                        ((collision_s *)fldMatterTable)[index].hojiID)) {
            continue;
        }
        if (((collision_s *)fldMatterTable)[index].matter != 0) {
            relative = 1;
            transform = ((collision_s *)fldMatterTable)[index].matter->poseset;
            origin[0] -= transform->pos.x;
            origin[1] -= transform->pos.y;
            origin[2] -= transform->pos.z;
            target[0] -= transform->pos.x;
            target[1] -= transform->pos.y;
            target[2] -= transform->pos.z;
        } else {
            relative = 0;
        }
        probeStart[0] = origin[0];
        probeStart[1] = origin[1];
        probeStart[2] = origin[2];
        probeEnd[0] = target[0];
        probeEnd[1] = target[1];
        probeEnd[2] = target[2];
        startDistance = fldVecDot((sdfVector4_t *)probeStart, (sdfVector4_t *)((collision_s *)fldMatterTable)[index].normal) -
                        ((collision_s *)fldMatterTable)[index].num;
        endDistance = fldVecDot((sdfVector4_t *)probeEnd, (sdfVector4_t *)((collision_s *)fldMatterTable)[index].normal) -
                      ((collision_s *)fldMatterTable)[index].num;
        if ((startDistance < 0.0f && 0.0f <= endDistance) || (endDistance < 0.0f && 0.0f <= startDistance)) {
            startDistance = ffabsf(startDistance);
            endDistance = ffabsf(endDistance);
            total = startDistance + endDistance;
            ratio = startDistance / total;
            anchor[0] = probeStart[0];
            anchor[1] = probeStart[1];
            anchor[2] = probeStart[2];
            delta[0] = probeEnd[0] - probeStart[0];
            delta[1] = probeEnd[1] - probeStart[1];
            delta[2] = probeEnd[2] - probeStart[2];
            probeStart[0] += delta[0] * ratio;
            probeStart[1] += delta[1] * ratio;
            probeStart[2] += delta[2] * ratio;
            clearance = HitPositionInside3(((collision_s *)fldMatterTable)[index].mode2D, ((collision_s *)fldMatterTable)[index].numedges, 70.0f, probeStart, &((collision_s *)fldMatterTable)[index]);
            if (0.0f <= clearance) {
                found = index;
                ratio = (startDistance + clearance) / total;
                delta[0] = probeEnd[0] - anchor[0];
                delta[1] = probeEnd[1] - anchor[1];
                delta[2] = probeEnd[2] - anchor[2];
                anchor[0] += delta[0] * ratio;
                anchor[1] += delta[1] * ratio;
                anchor[2] += delta[2] * ratio;
                probeStart[0] = anchor[0];
                probeStart[1] = anchor[1];
                probeStart[2] = anchor[2];
                origin[0] = anchor[0];
                origin[1] = anchor[1];
                origin[2] = anchor[2];
            }
        }
        if (relative) {
            transform = ((collision_s *)fldMatterTable)[index].matter->poseset;
            origin[0] += transform->pos.x;
            origin[1] += transform->pos.y;
            origin[2] += transform->pos.z;
            target[0] += transform->pos.x;
            target[1] += transform->pos.y;
            target[2] += transform->pos.z;
        }
    }
    hitPoint[0] = origin[0];
    hitPoint[1] = origin[1];
    hitPoint[2] = origin[2];
    return found;
}

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

