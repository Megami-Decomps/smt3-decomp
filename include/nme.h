#ifndef NME_H
#define NME_H

#include "common.h"
#include "kernel.h"
#include "libsdf.h"

typedef struct {
    u8 flag;
    s8 current, next, last, change, messageFlag;
    u8 reserved06[2];
    dds3ProcessID_t *messageProcess;
    s32 timer;
    void *parent;
} nmeSeqInfo_t;

typedef struct
{
    /* 0x00 */ u8 StepY[4];
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
    /* 0x28 */ s32 unk_28;
    /* 0x2C */ s32 unk_2C;
    /* 0x30 */ s32 unk_30;
    /* 0x34 */ s32 unk_34;
} cmpCursorInfo_t;

typedef struct {
    nmeSeqInfo_t sequence;
    sdfMemHandle_t *memHandle;
    s8 targetNo;
    s8 entryMode;
    s8 inputMode;
    u8 unk1B;
    cmpCursorInfo_t modeCursor;
    u8 reserved54[0x1004];
    cmpCursorInfo_t inputCursor;
    u8 reserved1090[0x1004];
    cmpCursorInfo_t nameCursor;
    u8 reserved20CC[0x1004];
    s8 primaryCursorActive;
    u8 reserved30D1[3];
    cmpCursorInfo_t cursor;
    u8 reserved310C[0x1004];
    cmpCursorInfo_t wrapCursor;
    u8 reserved4148[0x1004];
    s8 alternateCursorActive;
    s8 inputRowEnabled[3];
    s8 inputPos;
    u8 reserved5151;
    u8 inputChars[3][16][2];
    u8 reserved51B2[2];
    sdfTexHandle_t *textures[4];
    s32 animation;
} nmeWork_t;


extern nmeWork_t *nmeProcessId;
extern nmeWork_t *nmeUpdateWork;

#endif
