#ifndef SDF_PAD_CONTROL_H
#define SDF_PAD_CONTROL_H

#include "common.h"

typedef struct sdfPadCb_t {
    u8 port;                 /* 0x00 */
    u8 slot;                 /* 0x01 */
    u8 stat;                /* 0x02 */
    u8 opaque03;
    u8 mode;                 /* 0x04 */
    u8 reqMode;        /* 0x05 */
    u16 padData;             /* 0x06 */
    u16 lastPadData;         /* 0x08 */
    u8 opaque0A[2];
    u32 repeatDeadline;      /* 0x0C */
    u8 stick[4];             /* 0x10 */
    u8 pressure[12];         /* 0x14 */
    s16 actData;          /* 0x20 */
    s16 actDataBack;          /* 0x22 */
    s16 lastSmallMotor;      /* 0x24 */
    s16 lastLargeMotor;      /* 0x26 */
} sdfPadCb_t;

SIZE_CHECK(sdfPadControlRecordSize, sdfPadCb_t, 0x28);

#endif
