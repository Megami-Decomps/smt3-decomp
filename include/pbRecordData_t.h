#ifndef PB_RECORD_DATA_T_H
#define PB_RECORD_DATA_T_H

#include "common.h"

typedef struct pbRecordData_t
{
    u8 flag[4];
    void *characters, *blocks, *floors;
} pbRecordSlot_t;

#endif
