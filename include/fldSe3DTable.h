#ifndef FLD_SE_3D_TABLE_H
#define FLD_SE_3D_TABLE_H

#include "common.h"

/* Runtime spatial sound row, in native field order. */
typedef struct
{
    s32 flg;
    s32 id;
    f32 r;
    f32 x;
    f32 y;
    f32 z;
} fldSe3D_tag;

SIZE_CHECK(fldSe3DEntrySize, fldSe3D_tag, 0x18);

extern fldSe3D_tag fldSe3DTable[8];

#endif
