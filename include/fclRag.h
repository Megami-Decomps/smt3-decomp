#ifndef FCL_RAG_H
#define FCL_RAG_H

#include "newdata.h"
#include "facility.h"
#include "interface.h"

typedef struct {
    u16 item;
    ragItemReq_t requirements[3];
} ragHeartsEntry_t;

typedef struct {
    u8 reserved00[0x1A];
    s8 heartItems[32];
    s8 heartCount;
    s8 list2[8];
    s8 list2Count;
    s8 list3[8];
    s8 list3Count;
    s8 hasUnregistered;
    u8 reserved4E[0xF2];
    cmpCursorInfo_t cursor;
    u8 reserved178[0x1004];
    lstWork_t heartWindow;
    u8 reserved1310[0x1004];
    lstWork_t window2;
    u8 reserved24A8[0x1004];
    lstWork_t window3;
} ragListWork_t;

/* Inline custom values begin at +4 in each 0x2C-byte offer record. */
typedef struct {
    u8 level;
    u8 reserved01[5];
    u8 params[6];
    u16 skills[8];
} ragCustomUnit_t;

typedef struct ragOffer_t {
    u16 devilId;
    s8 custom;
    u8 reserved03;
    ragCustomUnit_t unit;
    ragItemReq_t requirements[3];
} ragOffer_t;

typedef struct {
    ragHeartsEntry_t hearts[25];
    u8 reserved15E[2];
    ragOffer_t offers2[21];
    u8 reserved4FC[4];
    ragOffer_t offers3[21];
} ragOfferTable_t;

typedef char ragCustomUnitSizeCheck[(sizeof(ragCustomUnit_t) == 0x1C) ? 1 : -1];
typedef char ragOfferSizeCheck[(sizeof(ragOffer_t) == 0x2C) ? 1 : -1];

void ragSetSeqPhase2(ragOffer_t *offer, datUnitWork_t *unit);

#endif
