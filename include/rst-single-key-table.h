#ifndef RST_SINGLE_KEY_TABLE_H
#define RST_SINGLE_KEY_TABLE_H

#include "common.h"

typedef struct {
    u16 key;
    u16 trailingHalfword;
} rstLookupKey_t;

/* An incomplete declaration keeps the reader's native absolute address load. */
extern rstLookupKey_t D_00339638[];

#endif
