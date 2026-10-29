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

#endif
