#ifndef CAMP_H
#define CAMP_H

#include "common.h"

typedef struct
{
    /* 0x00 */ s32 StepY;
    /* 0x04 */ s8 StopFlag;
    /* 0x05 */ u8 unk_05[3];
    /* 0x08 */ s32 CursorSize;
    /* 0x0C */ s32 CursorPos;
    /* 0x10 */ u16 unk_10;
    /* 0x12 */ u16 unk_12;
    /* 0x14 */ u16 unk_14;
    /* 0x16 */ u16 unk_16;
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ s32 unk_1C;
    /* 0x20 */ u8 unk_20;
    /* 0x21 */ u8 unk_21;
    /* 0x22 */ s16 unk_22;
    /* 0x24 */ s8 unk_24;
    /* 0x25 */ s8 unk_25;
    /* 0x26 */ u8 unk_26;
    /* 0x27 */ u8 unk_27;
    /* 0x28 */ union {
        s32 flag;
        char **text;
    } listData;
    /* 0x2C */ union {
        s32 value;
        u8 *bytes;
    } flagsData;
    /* 0x30 */ s32 unk_30;
    /* 0x34 */ s32 unk_34;
} cmpCursorInfo_t;

typedef struct
{
    s8 state;
    u8 param;
    u8 unk_02[2];
    s32 value;
} cmpCursorSlot_t;

typedef struct
{
    /* 0x0000 */ u8 unk_0000[0xE];
    /* 0x000E */ s16 unk_0E;
    /* 0x0010 */ u8 unk_10;
    /* 0x0011 */ u8 unk_0011[0x17];
    /* 0x0028 */ struct {
        cmpCursorSlot_t slot;
        u8 reserved[8];
    } rows[256];
    /* 0x1028 */ u8 unk_1028[0x14];
} cmpStockInfo_t;
typedef struct {
    cmpCursorInfo_t cursor;
    u8 reserved[0x1004];
} cmpCursorFullWork_t;

typedef struct cmpPadStopWork_t {
    cmpCursorInfo_t cursor;
    s8 unk_38;
} cmpPadStopWork_t;

typedef struct
{
    /* 0x00 */ u8 unk_00[0x22];
    /* 0x22 */ s16 cursor;
    /* 0x24 */ u8 scroll;
    /* 0x25 */ u8 unk_25[7];
    /* 0x2C */ u8 *itemFlags;
} rcvRootWork_t;

#endif
