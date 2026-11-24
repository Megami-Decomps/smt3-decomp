#ifndef FLD_RUN_DANTE_STATE_H
#define FLD_RUN_DANTE_STATE_H

#include "field.h"
#include "fldRunNativeRows.h"

/* Dante's chase work block. Only the fields the chase code reaches are known. */
typedef struct
{
    s32 status;
    f32 x;
    f32 y;
    f32 z;
    f32 turn;
    f32 oldframe;
    f32 nowframe;
    s16 ix;
    s16 iz;
    u16 targetIx;
    u16 targetIz;
    u16 tikamiti;
    s16 wkmode;
    fldMapCell_t *routeCell;
    void *RouteDataTop;
    s32 now;
    s32 goal;
    s32 nextframe;
    f32 angleToPlayer;
    f32 speed;
    s32 cnt_ex;
    s32 gunlight_anime;
    u32 flag;
    s32 update_flag;
    s32 bgm_mode;
    f32 d_p_len;
    f32 d_p_dir;
    f32 d_c_dir;
    s32 stance;
    fldDanteDoor_t *pDoor;
    u16 mapIxOld;
    u16 mapIzOld;
    s32 old_status;
} fldTest_FLDRUN_DANTE_t;

extern fldTest_FLDRUN_DANTE_t fldRun_Dante;

#endif
