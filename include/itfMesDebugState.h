#ifndef ITF_MES_DEBUG_STATE_H
#define ITF_MES_DEBUG_STATE_H

#include "common.h"

struct itfMesLink_t;

/* Writable state for the interactive message-layout inspector. */
struct debug_tag {
    s32 cursor;
    s32 mode;
    struct itfMesLink_t *pmesflow;
};

#endif
