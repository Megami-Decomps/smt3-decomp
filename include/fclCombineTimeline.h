#ifndef FCL_COMBINE_TIMELINE_H
#define FCL_COMBINE_TIMELINE_H

#include "common.h"

typedef struct
{
    u32 frame;
    s8 mode;
    u8 reserved05[3];
    void (*action)(void);
} cmbTimeTableEntry_t;

typedef struct
{
    cmbTimeTableEntry_t *entries;
    s8 count;
    u8 reserved05[3];
} cmbTimeTable_t;

extern cmbTimeTable_t cmbTimeTable[];

#endif
