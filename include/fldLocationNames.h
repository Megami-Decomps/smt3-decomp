#ifndef FLD_LOCATION_NAMES_H
#define FLD_LOCATION_NAMES_H

#include "common.h"

typedef struct {
    s16 fld;
    s16 area;
    char text[24];
} fldNameEntry_t;

enum { FLD_AREA_NAME_COUNT = 400 };
extern fldNameEntry_t fldAreaNameTable[FLD_AREA_NAME_COUNT];
enum { FLD_PLACE_NAME_COUNT = 336 };
extern fldNameEntry_t fldPlaceNameTable[FLD_PLACE_NAME_COUNT];
enum { FLD_FLOOR_NAME_COUNT = 224 };
extern fldNameEntry_t fldFloorNameTable[FLD_FLOOR_NAME_COUNT];

#endif
