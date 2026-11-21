#ifndef PB_BOARD_DATA_H
#define PB_BOARD_DATA_H

#include "common.h"

typedef struct { u16 blk[20][20]; } pbMapBlock_t;
typedef struct { f32 from[4], to[4]; } pbOutlineEdge_t;

extern pbMapBlock_t pbMapSnapshot;
extern pbOutlineEdge_t pbOutlineEdgeTable[];
extern s32 pbMapCursorBaseTable[4][4], pbMapCursorUvTable[4][4], pbMapCursorColorTable[4][4];

#endif
