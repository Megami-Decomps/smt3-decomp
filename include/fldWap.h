#ifndef FLD_WAP_H
#define FLD_WAP_H

#include "common.h"

typedef struct doormovetbl_s doormovetbl_s;

struct doormovetbl_s {
    s8 kind;
    u8 pad01;
    s16 requiredFlag;
    s16 floor;
    char name[0xC];
    s16 motion;
    s16 secondaryMotion;
    s16 sound;
    char motionName[0xC];
    char otherName[0xC];
    s8 variantMode;
    s8 flags31;
    s16 variant;
    s16 sequenceKind;
    s16 sequenceValue;
    char sequenceName[0xC];
    s8 linkKind;
    s8 rowIndex;
    char linkName[0xC];
    s8 sequenceCode;
    s8 selectedRoom;
    s8 flags54;
    char taskName[0xF];
};

/* A field's door table as its WAP file holds it (fldDoorLoad): five headers
 * (a lift's floors and blocks), then the 256 rows. */
typedef union fldWaypointHeaderValues_t fldWaypointHeaderValues_t;
typedef struct fldWaypointHeader_t fldWaypointHeader_t;
typedef struct fldWaypointBlock_t fldWaypointBlock_t;

enum { FLD_WAYPOINT_HEADER_COUNT = 5, FLD_WAYPOINT_BLOCK_BYTES = 0x64A0 };

union fldWaypointHeaderValues_t {
    s16 data[12];
    struct {
        s16 floors[6];
        s16 blocks[6];
    } elevator;
};

struct fldWaypointHeader_t {
    s32 area;
    s16 sound;
    s16 floorCount;
    fldWaypointHeaderValues_t body;
};

struct fldWaypointBlock_t {
    fldWaypointHeader_t headers[FLD_WAYPOINT_HEADER_COUNT];
    doormovetbl_s actors[256];
};

#endif
