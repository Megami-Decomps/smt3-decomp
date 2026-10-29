#ifndef RESULT2_H
#define RESULT2_H

#include "common.h"
#include "camp.h"
#include "newdata.h"

typedef struct {
    u8 level;
    u8 kind;
    u16 value;
} fclSkillParam_t;

typedef struct
{
    s8 SkillCnt;
    u8 TargetLevel[0x19];
    u16 skill[25];
} rstSkillInfo_t;


typedef struct rstData_t {
    union { u8 flags; u8 unk_0000; };
    union { s8 phase; s8 unk_0001; };
    u8 reserved02;
    s8 unk_0003;
    union { s8 stepFlag; s8 unk_0004; };
    union { s8 drawFlag; u8 unk_0005[1]; };
    u8 reserved06[0xE];
    sdfMemHandle_t *allocation;
    union { s8 unk_18; u8 AsignParamMax; };
    union { s8 unk_19; u8 AsignParam; };
    s8 StartLevel;
    union { u8 LevelUpCnt; s8 signedLevelUpCnt; s8 unk_001B; u8 unk_1B; };
    fclSkillParam_t *eventEntries;
    s8 EventNums;
    s8 eventIndex;
    union { u16 eventValue; u16 EventParam; };
    union { u16 heartsSkill; u16 GetHeartsSkill; };
    s8 RecoverFlag;
    s8 HeartsSkFlag;
    union { s8 heartsResult; s8 HeartsSkillResult; };
    union { s8 heartsEvent; s8 unk_29; };
    u16 bonusParamIndex;
    union { s8 defaultSkillResult; s8 DefaultSkillResult; };
    u8 reserved2D[3];
    datUnitWork_t *levelUpUnit[16];
    s8 levelUpCount;
    s8 levelUpStep;
    union { u8 stockIndex; s8 unk_0072; };
    s8 powerState;
    union { s8 powerSlot; s8 unk_74; };
    u8 reserved75;
    u16 powerSkill;
    union { s32 counter; s32 unk_78; };
    union { s8 evoPhase; s8 unk_7C; };
    union { s8 unk_7D; u8 reserved7D; };
    s8 evoResult;
    u8 reserved7F;
    datUnitWork_t evoWork;
    union { datUnitWork_t *unit; datUnitWork_t *pCurrentStock; datUnitWork_t *unk_16C; };
    datUnitWork_t workStock;
    cmpPadStopWork_t paramCursor;
    u8 reserved0298[0x1000];
    union { u8 commentFromUnit; s8 giftKind; };
    u8 unk_1299;
    union { u16 commentId; u16 giftId; };
    union { u8 commentMessage; u8 giftMessage; };
    union {
        struct {
            s8 giftStep;
            s8 Flag;
        };
        u8 unk_129D[2];
    };
    union { u8 bonusParam[6]; u8 unk_129F[6]; };
    u8 reserved12A5[3];
    union { cmpCursorInfo_t cursorInfo; cmpCursorInfo_t skillSlot; };
    u8 reserved12E0[0x1004];
    u8 unk_22E4;
    s8 unk_22E5;
    s8 unk_22E6;
    u8 unk_22E7;
    f32 unk_22E8;
    f32 unk_22EC;
    u16 selectedSkill;
    u8 reserved22F2[2];
    s32 panelEffectIndex;
    sdfMemHandle_t *scriptAllocation;
    void *script;
    sdfMemHandle_t *messageAllocation;
    void *message;
    union { s32 messageSize; s32 unk_2308; };
    union { sdfTexHandle_t *texture; s32 unk_230C; };
    u32 unk_2310;
    union { u32 evoBgFade; s32 fadeColor; };
} rstData_t;

#endif
