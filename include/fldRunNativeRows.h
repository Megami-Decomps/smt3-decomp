#ifndef FLD_RUN_NATIVE_ROWS_H
#define FLD_RUN_NATIVE_ROWS_H

#include "sdf_resource.h"

/* One loadable field effect: the resource handle trio the loader fills, then
 * the live effect or part handle and the placement the task applies. */
typedef struct {
    sdfMemHandle_t *mhFile;
    void *ptr;
    s32 size;
    void *pcf;
    void *d3p;
    s32 cnt;
    f32 x;
    f32 y;
    f32 z;
    f32 r;
} fldTest_FLDRUN_EFF_t;

typedef struct {
    fldTest_FLDRUN_EFF_t *pEff;
    s32 time;
    s32 effmode;
    s32 rotmode;
    f32 off_cam_range_min;
    f32 off_cam_range_max;
} fldTest_FLDRUN_EFFTBL_t;

extern fldTest_FLDRUN_EFFTBL_t *fldRunEffTables[];

typedef struct {
    char name[8];
    s16 ix;
    s16 iz;
    f32 angle;
} fldDanteDoor_t;

#endif
