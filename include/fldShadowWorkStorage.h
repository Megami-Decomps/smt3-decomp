#ifndef FLD_SHADOW_WORK_STORAGE_H
#define FLD_SHADOW_WORK_STORAGE_H

#include "sdf_types.h"

typedef struct {
    sdfVector3_t vertex[3];
} fldSdwTriangle_t;

typedef struct {
    u8 count;
    s8 reverse;
} fldSdwEdge_t;

extern fldSdwTriangle_t fldSdwTriangles[50];
extern fldSdwEdge_t fldShadowVertexTable[26][26];

#endif
