#ifndef FLD_EVE_HIT_STORAGE_H
#define FLD_EVE_HIT_STORAGE_H

#include "common.h"

enum {
    FIELD_ROOM_RECORD_COUNT = 64,
    FIELD_ROOM_CORNER_COUNT = 8,
    FIELD_ROOM_PLANE_COUNT = 6
};

typedef struct fldRoomState_t fldRoomState_t;

struct fldRoomState_t {
    f32 corner[FIELD_ROOM_CORNER_COUNT][4];
    f32 center[4];
    f32 plane[FIELD_ROOM_PLANE_COUNT][4];
    f32 limit[FIELD_ROOM_PLANE_COUNT];
    s32 worldObjectKey;
    f32 unk10C;
    f32 unk110;
    f32 unk114;
    s32 unk118;
    f32 unk11C;
    s16 unk120;
    s16 roomId;
    s16 unk124;
    s16 mode;
    s16 entryIndex;
    s16 axisMode;
    s32 shapeKind;
};

extern fldRoomState_t fldEveHitTable[FIELD_ROOM_RECORD_COUNT];

#endif
