#include "common.h"

typedef struct
{
    /* 0x00 */ u16 devilId;
    /* 0x02 */ u16 unk_02;
    /* 0x04 */ u16 value;
} cmbPairResult_t;

/* Six native pair-result rows walked by func_00222DC0. */
cmbPairResult_t D_00339BF8[6] = {
    { 68, 62, 138 },
    { 67, 62, 138 },
    { 66, 62, 139 },
    { 65, 62, 139 },
    { 64, 62, 140 },
    { 63, 62, 140 },
};
