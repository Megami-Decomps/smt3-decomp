#ifndef SDK_CDVD_NCMD_STATE_H
#define SDK_CDVD_NCMD_STATE_H

#include "common.h"

typedef struct {
    s32 busy;
    s32 command;
    s32 init;
} cdvdNcmdState_t;

extern cdvdNcmdState_t cdvdNcmdState;

#endif
