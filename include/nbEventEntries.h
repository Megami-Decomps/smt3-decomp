#ifndef NB_EVENT_ENTRIES_H
#define NB_EVENT_ENTRIES_H

#include "common.h"

struct dds3ProcessID_t;

/* 28-byte battle event entries. */
typedef struct {
    s32 rid;
    s32 (*process)(struct dds3ProcessID_t *id);
    s32 encno;
    s32 unk_0C;
    s32 unk_10;
    u16 fadetime;
    u16 fadeoutframe;
    u16 fadestopframe;
    u16 fadeinframe;
} nbEventEntry_t;

extern nbEventEntry_t nbEventEntryTable[16];

#endif
