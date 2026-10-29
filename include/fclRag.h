#ifndef FCL_RAG_H
#define FCL_RAG_H

#include "newdata.h"
#include "facility.h"

typedef struct {
    u16 item;
    ragItemReq_t requirements[3];
} ragHeartsEntry_t;

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
