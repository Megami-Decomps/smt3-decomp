#ifndef NME_H
#define NME_H

#include "common.h"
#include "camp.h"
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
    u8 inputChars[3][16][2];
    u8 reserved51B1[3];
    sdfTexHandle_t *textures[4];
    s32 animation;
} nmeWork_t;


extern nmeWork_t *nmeProcessId;
extern nmeWork_t *nmeUpdateWork;

#endif
