#ifndef NB_INNEN_TABLE_H
#define NB_INNEN_TABLE_H

#include "common.h"

typedef struct { u16 item; u16 cnt; } nbInnenPhase_t;

typedef struct { s16 phase; s16 side; s16 motion; } nbInnenMotion_t;

/* Complete 60-byte records, interpreted as phase items or message/motion data. */
typedef union {
    struct {
        s16 id;
        u16 pid;
        u16 eid;
        u16 unk_06;
        nbInnenPhase_t phase[13];
    } phase_record;
    struct {
        s16 id;
        u16 pid;
        u16 eid;
        u16 unk_06;
        u8 unk_08[24];
        u16 mainmes;
        u16 mescnt;
        u8 unk_24[6];
        nbInnenMotion_t motion[3];
    } message;
} nbInnenRec_t;

extern nbInnenRec_t nbInnenTable[64];

#endif
