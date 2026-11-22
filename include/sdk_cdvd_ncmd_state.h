#ifndef SDK_CDVD_NCMD_STATE_H
#define SDK_CDVD_NCMD_STATE_H

#include "common.h"

typedef struct {
    s32 busy;
    s32 command;
    s32 init;
} cdvdNcmdState_t;


typedef struct cdvdCallbackReasons_t {
    s32 pending;
    s32 saved;
} cdvdCallbackReasons_t;

typedef struct cdvdScmdState_t {
    s32 powerInit;
    s32 searchInit;
    s32 diskReadyInit;
    s32 commandInit;
    s32 statusInit;
    s32 invocationCount;
} cdvdScmdState_t;

/* These three adjacent startup records share one storage owner.
 * The C member alignment is four bytes; no packed attribute is needed. */
typedef struct cdvdStartupRecords_t {
    cdvdNcmdState_t ncmd;
    cdvdScmdState_t scmd;
    cdvdCallbackReasons_t callback;
} cdvdStartupRecords_t;

extern cdvdStartupRecords_t cdvdNcmdState;

#endif
