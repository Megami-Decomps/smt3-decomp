#ifndef FLD_EVE_HIT_SOURCE_TABLE_H
#define FLD_EVE_HIT_SOURCE_TABLE_H

#include "field.h"

/* The event-hit source records at fldEveHitSourceTable: a sphere, the put record that
   names it, the transform dds3AddPointBasic consumes, and the two resource
   headers the hit and the event are registered under. */
typedef struct {
    u32 Type;
    f32 r;
} fldResPutEveBall_t;

typedef struct {
    fldResPutEveBall_t ball;           /* 0x00 */
    fldResPut_t put;                   /* 0x08 */
    fldResourcePointUnaligned_t sisei; /* 0x18 */
    fldResHead_t hit;                  /* 0x48 */
    fldResHead_t event;                /* 0x6C */
} fldEveHitSource_t;

typedef char fldEveHitSource_put[((u32)&((fldEveHitSource_t *)0)->put == 0x08) ? 1 : -1];
typedef char fldEveHitSource_sisei[((u32)&((fldEveHitSource_t *)0)->sisei == 0x18) ? 1 : -1];
typedef char fldEveHitSource_hit[((u32)&((fldEveHitSource_t *)0)->hit == 0x48) ? 1 : -1];
typedef char fldEveHitSource_event[((u32)&((fldEveHitSource_t *)0)->event == 0x6C) ? 1 : -1];

typedef char fldEveHitSource_stride[(sizeof(fldEveHitSource_t) == 0x90) ? 1 : -1];

extern fldEveHitSource_t fldEveHitSourceTable[64];

#endif
