#ifndef BILLBOARD_GENERAL_DATA_H
#define BILLBOARD_GENERAL_DATA_H

#include "common.h"

typedef struct billGeneralRow_t {
    const char *fileName;
    s16 kind;
    s16 field06;
} billGeneralRow_t;

extern billGeneralRow_t dds3BillboardGeneralRows[];

#endif
